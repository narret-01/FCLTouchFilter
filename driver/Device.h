#pragma once

#include "Driver.h"

NTSTATUS
FclCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );
