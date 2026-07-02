#include "Device.h"
#include "Queue.h"

NTSTATUS
FclEvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
{
    UNREFERENCED_PARAMETER(Driver);

    return FclCreateDevice(DeviceInit);
}

NTSTATUS
FclCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
{
    NTSTATUS status;
    WDFDEVICE device;
    WDF_OBJECT_ATTRIBUTES attributes;

    // This driver is installed by INF only on USB\VID_0430&PID_0508.
    // Mark it as a filter so PnP stack behavior stays pass-through by default.
    WdfFdoInitSetFilter(DeviceInit);

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);

    status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    return FclQueueInitialize(device);
}
