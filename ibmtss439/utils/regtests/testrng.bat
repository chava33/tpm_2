echo ""
echo "Random Number Generator"
echo ""

echo "Stir Random"
stirrandom -if policies/aaa > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Get Random"
getrandom -by 64 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

exit /B 0
