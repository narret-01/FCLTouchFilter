# FCLTouchFilter

KMDF HID upper filter for Fujitsu/FCL USB Touch Panel `USB\VID_0430&PID_0508`.

First build transforms the 8-byte HID input report by swapping the two observed coordinate fields:

- bytes 1..2
- bytes 4..5

If direction is wrong, change these defines in `driver/ReportTransform.c` and rebuild:

```c
#define FCL_SWAP_AB     1
#define FCL_INVERT_A    0
#define FCL_INVERT_B    0
```

## Test-mode install

On the target machine, run as Administrator:

```cmd
bcdedit /set testsigning on
shutdown /r /t 0
```

Then download the GitHub Actions artifact and run:

```cmd
install.cmd
```

Reboot or replug the touchscreen.
