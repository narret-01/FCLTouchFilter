@echo off
setlocal

echo Removing FCLTouchFilter driver package if present...
for /f "tokens=2 delims=: " %%i in ('pnputil /enum-drivers ^| findstr /i "FCLTouchFilter.inf"') do pnputil /delete-driver %%i /uninstall /force

echo Done. Reboot Windows.
endlocal
