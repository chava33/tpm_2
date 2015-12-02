echo ""
echo "PCR Extend"
echo ""

for %%H in (sha1 sha256 sha384) do (

    echo "PCR Reset"
    pcrreset -ha 16 > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

    echo "PCR Extend %%H"
    pcrextend -ha 16 -halg %%H -if policies/aaa > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

    echo "PCR Read %%H"
    pcrread -ha 16 -halg %%H -of tmp.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

    echo "Verify the read data %%H"
    diff policies/%%Hextaaa0.bin tmp.bin
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

    echo "PCR Reset"
    pcrreset -ha 16 > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )
)

echo ""
echo "PCR Event"
echo ""

echo "PCR Reset"
pcrreset -ha 16 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "PCR Read"
pcrread -ha 16 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "PCR Extend"
pcrevent -ha 16 -if policies/aaa -of1 tmpsha1.bin -of2 tmpsha256.bin -of3 tmpsha384.bin > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

for %%H in (sha1 sha256 sha384) do (

    echo "Verify Digest %%H"
    diff policies/%%Haaa.bin tmp%%H.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

    echo "PCR Read %%H"
    pcrread -ha 16 -halg %%H -of tmp%%H.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

    echo "Verify Digest %%H"
    diff policies/%%Hexthaaa.bin tmp%%H.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

)

echo ""
echo "Event Sequence Complete"
echo ""

echo "PCR Reset"
pcrreset -ha 16 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Event sequence start, alg null"
hashsequencestart -halg null -pwda aaa > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Event Sequence Complete"
eventsequencecomplete -hs 80000001 -pwds aaa -ha 16 -if policies/aaa -of1 tmpsha1.bin -of2 tmpsha256.bin -of3 tmpsha384.bin > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

for %%H in (sha1 sha256 sha384) do (

    echo "Verify Digest %%H"
    diff policies/%%Haaa.bin tmp%%H.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )
 
    echo "PCR Read %%H"
    pcrread -ha 16 -halg %%H -of tmp%%H.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

    echo "Verify Digest %%H"
    diff policies/%%Hexthaaa.bin tmp%%H.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
      exit /B 1
      )

)

echo "PCR Reset"
pcrreset -ha 16 > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

exit /B 0
