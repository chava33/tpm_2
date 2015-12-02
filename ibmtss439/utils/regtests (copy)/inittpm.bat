echo "Power cycle"
.\powerup -v > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Startup"
.\startup -c -v > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

exit /B 0
