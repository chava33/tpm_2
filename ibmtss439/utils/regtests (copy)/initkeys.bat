
echo -n "1234567890123456" > msg.bin
touch zero.bin

REM try to undefine any NV index left over from a previous test.  Do not check for errors.
nvundefinespace -hi p -ha 01000000 > run.out
nvundefinespace -hi p -ha 01000000 -pwdp ppp > run.out
nvundefinespace -hi p -ha 01000001 > run.out
REM same for persistent objects
evictcontrol -ho 81800000 -hp 81800000 -hi p > run.out

echo ""
echo "Initialize Regression Test Keys"
echo ""

echo "Create a platform primary storage key"
createprimary -hi p -pwdk pps > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Create a storage key under the primary key"
create -hp 80000000 -st -kt f -kt p -opr storepriv.bin -opu storepub.bin -pwdp pps -pwdk sto > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Create an unrestricted signing key under the primary key"
create -hp 80000000 -si -kt f -kt p -opr signpriv.bin -opu signpub.bin -pwdp pps -pwdk sig > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Create a restricted signing key under the primary key"
create -hp 80000000 -sir -kt f -kt p -opr signrpriv.bin -opu signrpub.bin -pwdp pps -pwdk sig > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Create an RSA decryption key under the primary key"
create -hp 80000000 -den -kt f -kt p -opr derpriv.bin -opu derpub.bin -pwdp pps -pwdk dec > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

echo "Create a symmetric cipher key under the primary key"
create -hp 80000000 -des -kt f -kt p -opr despriv.bin -opu despub.bin -pwdp pps -pwdk aes > run.out
IF %ERRORLEVEL% NEQ 0 (
  exit /B 1
)

for %%H in (sha1 sha256 sha384) do (
    echo "Create a %%H keyed hash key under the primary key"
    create -hp 80000000 -kh -kt f -kt p -opr khpriv%%H.bin -opu khpub%%H.bin -pwdp pps -pwdk khk -halg %%H
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )
)

exit /B 0
