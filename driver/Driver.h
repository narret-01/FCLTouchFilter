#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <hidport.h>

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD FclEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP FclEvtDriverContextCleanup;
