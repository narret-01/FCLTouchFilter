#include "Queue.h"
#include "ReportTransform.h"

static VOID
FclForwardRequest(
    _In_ WDFREQUEST Request,
    _In_ WDFIOTARGET Target
    )
{
    NTSTATUS status;
    WDF_REQUEST_SEND_OPTIONS options;

    status = WdfRequestFormatRequestUsingCurrentType(Request);
    if (!NT_SUCCESS(status)) {
        WdfRequestComplete(Request, status);
        return;
    }

    WDF_REQUEST_SEND_OPTIONS_INIT(&options, WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET);

    if (!WdfRequestSend(Request, Target, &options)) {
        status = WdfRequestGetStatus(Request);
        WdfRequestComplete(Request, status);
    }
}

NTSTATUS
FclQueueInitialize(
    _In_ WDFDEVICE Device
    )
{
    WDF_IO_QUEUE_CONFIG queueConfig;

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
        &queueConfig,
        WdfIoQueueDispatchParallel);

    queueConfig.EvtIoInternalDeviceControl = FclEvtIoInternalDeviceControl;

    return WdfIoQueueCreate(
        Device,
        &queueConfig,
        WDF_NO_OBJECT_ATTRIBUTES,
        WDF_NO_HANDLE);
}

VOID
FclEvtIoInternalDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
{
    NTSTATUS status;
    WDFDEVICE device;
    WDFIOTARGET target;

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    device = WdfIoQueueGetDevice(Queue);
    target = WdfDeviceGetIoTarget(device);

    if (IoControlCode == IOCTL_HID_READ_REPORT) {
        status = WdfRequestFormatRequestUsingCurrentType(Request);
        if (!NT_SUCCESS(status)) {
            WdfRequestComplete(Request, status);
            return;
        }

        WdfRequestSetCompletionRoutine(
            Request,
            FclReadReportCompletion,
            NULL);

        if (!WdfRequestSend(Request, target, WDF_NO_SEND_OPTIONS)) {
            status = WdfRequestGetStatus(Request);
            WdfRequestComplete(Request, status);
        }

        return;
    }

    FclForwardRequest(Request, target);
}

VOID
FclReadReportCompletion(
    _In_ WDFREQUEST Request,
    _In_ WDFIOTARGET Target,
    _In_ PWDF_REQUEST_COMPLETION_PARAMS Params,
    _In_ WDFCONTEXT Context
    )
{
    NTSTATUS status;
    PVOID buffer;
    size_t length;
    ULONG_PTR information;

    UNREFERENCED_PARAMETER(Target);
    UNREFERENCED_PARAMETER(Context);

    status = Params->IoStatus.Status;
    information = Params->IoStatus.Information;

    if (NT_SUCCESS(status) && information >= FCL_REPORT_SIZE) {
        status = WdfRequestRetrieveOutputBuffer(
            Request,
            FCL_REPORT_SIZE,
            &buffer,
            &length);

        if (NT_SUCCESS(status) && length >= FCL_REPORT_SIZE) {
            FclTransformReport((PUCHAR)buffer, length);
        } else {
            // Do not fail a successful lower-driver read just because we could
            // not map the buffer. Preserve original completion status.
            status = Params->IoStatus.Status;
        }
    }

    WdfRequestCompleteWithInformation(Request, status, information);
}
