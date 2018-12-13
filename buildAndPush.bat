make
if %ERRORLEVEL% == 0 goto upload
goto end
:upload
nxlink -a 192.168.2.154 NXoodles.nro
:end