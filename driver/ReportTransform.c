#include "ReportTransform.h"

// Report format observed from USBPcap:
//   Byte 0      : flags/button/touch state
//   Byte 1..2   : coordinate A, little-endian, 14-bit range-ish
//   Byte 3      : padding
//   Byte 4..5   : coordinate B, little-endian, 14-bit range-ish
//   Byte 6..7   : extra/status
//
// First test build: swap coordinate fields A and B.
// If direction is wrong, change these defines and rebuild.
#define FCL_SWAP_AB     1
#define FCL_INVERT_A    0
#define FCL_INVERT_B    0

static USHORT
ReadLe16(
    _In_reads_bytes_(2) const UCHAR* p
    )
{
    return (USHORT)(p[0] | ((USHORT)p[1] << 8));
}

static VOID
WriteLe16(
    _Out_writes_bytes_(2) UCHAR* p,
    _In_ USHORT v
    )
{
    p[0] = (UCHAR)(v & 0xFF);
    p[1] = (UCHAR)((v >> 8) & 0xFF);
}

VOID
FclTransformReport(
    _Inout_updates_bytes_(Length) PUCHAR Report,
    _In_ size_t Length
    )
{
    USHORT a;
    USHORT b;
    USHORT t;

    if (Report == NULL || Length < FCL_REPORT_SIZE) {
        return;
    }

    // Do not transform idle/liftoff packets unless bit0 says touch/button down.
    // If this blocks needed movement, remove this check in the next build.
    if ((Report[0] & 0x01) == 0) {
        return;
    }

    a = ReadLe16(&Report[1]);
    b = ReadLe16(&Report[4]);

#if FCL_SWAP_AB
    t = a;
    a = b;
    b = t;
#endif

#if FCL_INVERT_A
    a = (USHORT)(FCL_MAX_COORD - (a & FCL_MAX_COORD));
#endif

#if FCL_INVERT_B
    b = (USHORT)(FCL_MAX_COORD - (b & FCL_MAX_COORD));
#endif

    WriteLe16(&Report[1], a);
    WriteLe16(&Report[4], b);
}
