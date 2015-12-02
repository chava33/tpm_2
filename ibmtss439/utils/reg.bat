@echo off

set soc=
set mssim=
if "%TPM_INTERFACE_TYPE%" == "" (
   set soc=1
)
if "%TPM_INTERFACE_TYPE%" == "socsim" (
   set soc=1
)
if defined soc (
   if "%TPM_SERVER_TYPE%" == "" (
       set mssim=1
   )
   if "%TPM_SERVER_TYPE%" == "mssim" (
      set mssim=1
   )
)

if defined mssim (
   call regtests\inittpm.bat
   IF %ERRORLEVEL% NEQ 0 (
      echo ""
      echo "Failed inittpm.bat"
      exit /B 1
   )
)

call regtests\initkeys.bat
IF %ERRORLEVEL% NEQ 0 (
   echo ""
   echo "Failed initkeys.bat"
   exit /B 1
)

call regtests\testrng.bat
IF %ERRORLEVEL% NEQ 0 (
   echo ""
   echo "Failed testrng.bat"
  exit /B 1
)

call regtests\testpcr.bat
IF %ERRORLEVEL% NEQ 0 (
   echo ""
   echo "Failed testpcr.bat"
  exit /B 1
)

call regtests\testprimary.bat
IF %ERRORLEVEL% NEQ 0 (
   echo ""
   echo "Failed testprimary.bat"
  exit /B 1
)

call regtests\testhmacsession.bat
IF %ERRORLEVEL% NEQ 0 (
   echo ""
   echo "Failed testhmacsession.bat"
   exit /B 1
)

call regtests\testbind.bat
IF %ERRORLEVEL% NEQ 0 (
   echo ""
   echo "Failed testbind.bat"
  exit /B 1
)

REM call regtests\testsalt.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testhierarchy.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\teststorage.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testchangeauth.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testencsession.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testsign.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM    echo ""
REM    echo "Failed"
REM   exit /B 1
REM )

REM call regtests\testnv.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testevict.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testrsa.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testaes.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testhmac.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testattest.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

call regtests\testpolicy.bat
IF %ERRORLEVEL% NEQ 0 (
   echo ""
   echo "Failed testpolicy.bat"
  exit /B 1
)

REM call regtests\testcontext.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testclocks.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testda.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testunseal.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testdup.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testecc.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\tes.batutdown.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

REM call regtests\testchangeseed.bat
REM IF %ERRORLEVEL% NEQ 0 (
REM   exit /B 1
REM )

echo ""
echo "Success"
