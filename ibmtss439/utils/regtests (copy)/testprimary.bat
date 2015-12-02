echo ""
echo "Primary key - CreatePrimary"
echo ""

echo "Create a primary storage key"
createprimary -hi p -pwdk pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Read the public part"
readpublic -ho 80000001  > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Create a storage key under the primary key"
create -hp 80000001 -st -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sto > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Load the storage key under the primary key"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the storage key"
flushcontext -ha 80000002 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the primary storage key"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Load the storage key under the primary key - should fail"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
IF %ERRORLEVEL% EQU 0 (
  exit /B 1
  )

echo ""
echo "Primary key - CreatePrimary with no unique field"
echo ""

REM no unique 

echo "Create a primary storage key with no unique field"
createprimary -hi p -pwdk pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Create a storage key under the primary key"
create -hp 80000001 -st -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sto > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Load the storage key under the primary key"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the storage key"
flushcontext -ha 80000002 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the primary storage key"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

REM empty unique

echo "Create a primary storage key with no unique field"
touch empty.bin
createprimary -hi p -pwdk pps -iu empty.bin > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Load the original storage key under the primary key with empty unique field"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the storage key"
flushcontext -ha 80000002 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the primary storage key"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo ""
echo "Primary key - CreatePrimary with unique field"
echo ""

REM unique

echo "Create a primary storage key with unique field"
touch empty.bin
createprimary -hi p -pwdk pps -iu policies/aaa > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Load the original storage key under the primary key - should fail"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
IF %ERRORLEVEL% EQU 0 (
  exit /B 1
  )

echo "Create a storage key under the primary key"
create -hp 80000001 -st -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sto > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Load the storage key under the primary key"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the storage key"
flushcontext -ha 80000002 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the primary storage key"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

REM same unique

echo "Create a primary storage key with same unique field"
createprimary -hi p -pwdk pps -iu policies/aaa > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Load the previous storage key under the primary key"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the storage key"
flushcontext -ha 80000002 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

echo "Flush the primary storage key"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
  )

REM cleanup

exit /B 0

REM getcapability  -cap 1 -pr 80000000

