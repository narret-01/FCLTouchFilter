@echo off
setlocal
cd /d %~dp0

echo Installing FCLTouchFilter test certificate...
certutil -addstore -f Root FCLTouchFilterTestCert.cer
certutil -addstore -f TrustedPublisher FCLTouchFilterTestCert.cer

echo Installing driver package...
pnputil /add-driver FCLTouchFilter.inf /install

echo.
echo Done. Reboot Windows, or unplug/replug the touchscreen if possible.
endlocal
