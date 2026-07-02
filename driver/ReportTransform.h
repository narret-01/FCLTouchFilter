#pragma once

#include "Driver.h"

#define FCL_REPORT_SIZE 8
#define FCL_MAX_COORD   0x3FFF

VOID
FclTransformReport(
    _Inout_updates_bytes_(Length) PUCHAR Report,
    _In_ size_t Length
    );
