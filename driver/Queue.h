#pragma once

#include "Driver.h"

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL FclEvtIoInternalDeviceControl;
EVT_WDF_REQUEST_COMPLETION_ROUTINE FclReadReportCompletion;

NTSTATUS
FclQueueInitialize(
    _In_ WDFDEVICE Device
    );
