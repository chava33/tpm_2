REM #################################################################################
REM #										#
REM #			TPM2 regression test					#
REM #			     Written by Ken Goldman				#
REM #		       IBM Thomas J. Watson Research Center			#
REM #	$Id$			#
REM #										#
REM # (c) Copyright IBM Corporation 2015					#
REM # 										#
REM # All rights reserved.							#
REM # 										#
REM # Redistribution and use in source and binary forms, with or without	#
REM # modification, are permitted provided that the following conditions are	#
REM # met:									#
REM # 										#
REM # Redistributions of source code must retain the above copyright notice,	#
REM # this list of conditions and the following disclaimer.			#
REM # 										#
REM # Redistributions in binary form must reproduce the above copyright		#
REM # notice, this list of conditions and the following disclaimer in the	#
REM # documentation and/or other materials provided with the distribution.	#
REM # 										#
REM # Neither the names of the IBM Corporation nor the names of its		#
REM # contributors may be used to endorse or promote products derived from	#
REM # this software without specific prior written permission.			#
REM # 										#
REM # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	#
REM # "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		#
REM # LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR	#
REM # A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT	#
REM # HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	#
REM # SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		#
REM # LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,	#
REM # DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY	#
REM # THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT	#
REM # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE	#
REM # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.	#
REM #										#
REM #################################################################################
REM 

echo ""
echo "Bind session"
echo ""

echo ""
echo "Bind session to Primary Key"
echo ""

echo "Bind session bound to primary key at 80000000"
startauthsession -se h -bi 80000000 -pwdb pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session, same object 80000000"
create -hp 80000000 -st -kt f -kt p -pwdp pps -pwdk 222 -se0 02000000 1 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session, same object 80000000, wrong password does not matter"
create -hp 80000000 -st -kt f -kt p -pwdp xxx -pwdk 222 -se0 02000000 0 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create second primary key with different password 000 and Name"
createprimary -hi o -pwdk 000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Bind session bound to second primary key at 80000001, correct password"
startauthsession -se h -bi 80000001 -pwdb 000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session, different object 80000000"
create -hp 80000000 -st -kt f -kt p -pwdp pps -pwdk 222 -se0 02000000 1 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session, different object 80000000, wrong password - should fail"
create -hp 80000000 -st -kt f -kt p -pwdp xxx -pwdk 222 -se0 02000000 1 > run.out
    IF %ERRORLEVEL% EQU 0 (
       exit /B 1
       )

echo "Flush the session"
flushcontext -ha 02000000  > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Bind session bound to primary key at 80000000, wrong password"
startauthsession -se h -bi 80000000 -pwdb xxx > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session, same object 80000000 - should fail"
create -hp 80000000 -st -kt f -kt p -pwdp pps -pwdk 222 -se0 02000000 0 > run.out
    IF %ERRORLEVEL% EQU 0 (
       exit /B 1
       )

echo "Flush the failing session"
flushcontext -ha 02000000  > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the second primary key"
flushcontext -ha 80000001 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo ""
echo "Bind session to Hierarchy"
echo ""

echo "Change platform hierarchy auth"
hierarchychangeauth -hi p -pwdn ppp > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Bind session bound to platform hierarchy"
startauthsession -se h -bi 4000000c -pwdb ppp > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session, wrong password - should fail"
create -hp 80000000 -st -kt f -kt p -pwdp xxx -pwdk 222 -se0 02000000 0 > run.out
    IF %ERRORLEVEL% EQU 0 (
       exit /B 1
       )

echo "Create storage key using that bind session"
create -hp 80000000 -st -kt f -kt p -pwdp pps -pwdk 222 -se0 02000000 0 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Bind session bound to platform hierarchy, wrong password"
startauthsession -se h -bi 4000000c -pwdb xxx > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session - should fail"
create -hp 80000000 -st -kt f -kt p -pwdp pps -pwdk 222 -se0 02000000 0 > run.out
    IF %ERRORLEVEL% EQU 0 (
       exit /B 1
       )

echo "Change platform hierarchy auth back to null"
hierarchychangeauth -hi p -pwda ppp > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the session"
flushcontext -ha 02000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo ""
echo "Bind session to NV"
echo ""

echo "NV Undefine Space"
nvundefinespace -hi o -ha 01000000 > run.out

echo "NV Define Space"
nvdefinespace -hi o -ha 01000000 -pwdn nnn -sz 3 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "NV Read Public, unwritten Name"
nvreadpublic -ha 01000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "NV write PWAP to set written"
nvwrite -ha 01000000 -pwdn nnn -ic 123 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Bind session bound to NV index at 01000000"
startauthsession -se h -bi 01000000 -pwdb nnn > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "NV Write HMAC using bind session"
nvwrite -ha 01000000 -pwdn nnn -ic 123 -se0 02000000 1  > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "NV Read HMAC using bind session"
nvread -ha 01000000 -pwdn nnn -sz 3 -se0 02000000 1  > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "NV Read HMAC using bind session, wrong password does not matter"
nvread -ha 01000000 -pwdn xxx -sz 3 -se0 02000000 1  > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create storage key using that bind session"
create -hp 80000000 -st -kt f -kt p -pwdp pps -pwdk 222 -se0 02000000 0 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "NV Undefine Space"
nvundefinespace -hi o -ha 01000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo ""
echo "Encrypt with bind to same object"
echo ""

for %%M in (xor aes) do (

    echo "Start an HMAC auth session with %%M encryption and bind to primary key at 80000000"
    startauthsession -se h -sym %%M -bi 80000000 -pwdb pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

    echo "Create storage key using bind session, same object, wrong password"
    create -hp 80000000 -st -kt f -kt p -pwdk 222 -pwdp xxx -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

    echo "Create storage key using bind session, same object 80000000"
    create -hp 80000000 -st -kt f -kt p -pwdk 222 -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

    echo "Load the key, with %%M encryption"
    load -hp 80000000 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps -se0 02000000 61 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

    echo "Flush the sealed object"
    flushcontext -ha 80000001 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

    echo "Flush the %%M session"
    flushcontext -ha 02000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

)

REM WHy doesn't the error return work within the for loop?

REM echo ""
REM echo "Encrypt with bind to different object"
REM echo ""

REM for %%M in (xor aes) do (

REM     echo "Start an HMAC auth session with %%M encryption and bind to platform auth"
REM     startauthsession -se h -sym %%M -bi 4000000c > run.out
REM     IF %ERRORLEVEL% NEQ 0 (
REM        exit /B 1
REM        )

REM     echo "Create storage key using bind session, different object, wrong password, should fail"
REM     create -hp 80000000 -st -kt f -kt p -pwdk 222 -pwdp xxx -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61
REM     echo  %ERRORLEVEL%
REM     echo error level %ERRORLEVEL%
REM     IF %ERRORLEVEL% EQU 0 (
REM       exit /B 1
REM         )

REM     echo "Create storage key using bind session, different object"
REM     create -hp 80000000 -st -kt f -kt p -pwdk 222 -pwdp pps -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61 > run.out
REM     IF %ERRORLEVEL% NEQ 0 (
REM        exit /B 1
REM        )

REM     echo "Load the key, with %%M encryption"
REM     load -hp 80000000 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps -se0 02000000 61 > run.out
REM     IF %ERRORLEVEL% NEQ 0 (
REM        exit /B 1
REM        )

REM     echo "Flush the sealed object"
REM     flushcontext -ha 80000001 > run.out
REM     IF %ERRORLEVEL% NEQ 0 (
REM        exit /B 1
REM        )

REM     echo "Flush the %%M session"
REM     flushcontext -ha 02000000 > run.out
REM     IF %ERRORLEVEL% NEQ 0 (
REM        exit /B 1
REM        )

REM )

echo ""
echo "Encrypt with bind to different object, xor"
echo ""

echo "Start an HMAC auth session with xor encryption and bind to platform auth"
startauthsession -se h -sym xor -bi 4000000c > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Create storage key using bind session, different object, wrong password, should fail"
create -hp 80000000 -st -kt f -kt p -pwdk 222 -pwdp xxx -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61 > run.out
IF %ERRORLEVEL% EQU 0 (
  exit /B 1
    )

echo "Create storage key using bind session, different object"
create -hp 80000000 -st -kt f -kt p -pwdk 222 -pwdp pps -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Load the key, with xor encryption"
load -hp 80000000 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps -se0 02000000 61 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Flush the sealed object"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Flush the xor session"
flushcontext -ha 02000000 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo ""
echo "Encrypt with bind to different object, aes"
echo ""

echo "Start an HMAC auth session with aes encryption and bind to platform auth"
startauthsession -se h -sym aes -bi 4000000c > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Create storage key using bind session, different object, wrong password, should fail"
create -hp 80000000 -st -kt f -kt p -pwdk 222 -pwdp xxx -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61 > run.out
IF %ERRORLEVEL% EQU 0 (
  exit /B 1
    )

echo "Create storage key using bind session, different object"
create -hp 80000000 -st -kt f -kt p -pwdk 222 -pwdp pps -opr tmppriv.bin -opu tmppub.bin -se0 02000000 61 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Load the key, with aes encryption"
load -hp 80000000 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps -se0 02000000 61 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Flush the sealed object"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo "Flush the aes session"
flushcontext -ha 02000000 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

echo ""
echo "PolicyAuthValue and bind to different object, command encryption"
echo ""

echo "Create a signing key under the primary key - policy command code - sign, auth"
create -hp 80000000 -si -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sig -pol policies/policyccsign-auth.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Load the signing key under the primary key"
load -hp 80000000 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Start a policy session, bind to primary key"
startauthsession -se p -bi 80000000 -pwdb pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy command code - sign"
policycommandcode -ha 03000000 -cc 15d > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy authvalue"
policyauthvalue -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Sign a digest - policy, command encrypt"
sign -hk 80000001 -if policies/aaa -os sig.bin -ipu tmppub.bin -se0 03000000 21 -pwdk sig > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Verify the signature"
verifysignature -hk 80000001 -if policies/aaa -is sig.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the signing key"
flushcontext -ha 80000001 > run.out 
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the session"
flushcontext -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo ""
echo "PolicyAuthValue and bind to same object, command encryption"
echo ""

echo "Create a signing key under the primary key - policy command code - sign, auth"
create -hp 80000000 -si -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sig -pol policies/policyccsign-auth.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Load the signing key under the primary key"
load -hp 80000000 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Start a policy session"
startauthsession -se p -bi 80000001 -pwdb sig > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy command code - sign"
policycommandcode -ha 03000000 -cc 15d > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy authvalue"
policyauthvalue -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Sign a digest - policy, command encrypt"
sign -hk 80000001 -if policies/aaa -os sig.bin -ipu tmppub.bin -se0 03000000 21 -pwdk sig > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Verify the signature"
verifysignature -hk 80000001 -if policies/aaa -is sig.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the signing key"
flushcontext -ha 80000001 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the session"
flushcontext -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo ""
echo "PolicyAuthValue and bind to different object, response encryption"
echo ""

echo "Create a storage key under the primary key - policy command code - create, auth"
create -hp 80000000 -st -kt f -kt p -opr tmpspriv.bin -opu tmpspub.bin -pwdp pps -pwdk sto -pol policies/policycccreate-auth.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Load the storage key under the primary key"
load -hp 80000000 -ipr tmpspriv.bin -ipu tmpspub.bin -pwdp pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Start a policy session, bind to primary key"
startauthsession -se p -bi 80000000 -pwdb pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy command code - create"
policycommandcode -ha 03000000 -cc 153 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy authvalue"
policyauthvalue -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create a signing key with response encryption"
create -hp 80000001 -si -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp sto -pwdk sig -se0 03000000 41 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Load the signing key to verify response encryption"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp sto > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the storage key"
flushcontext -ha 80000001 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the signing key"
flushcontext -ha 80000002 > run.out 
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the session"
flushcontext -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo ""
echo "PolicyAuthValue and bind to same object, response encryption"
echo ""

echo "Create a storage key under the primary key - policy command code - create, auth"
create -hp 80000000 -st -kt f -kt p -opr tmpspriv.bin -opu tmpspub.bin -pwdp pps -pwdk sto -pol policies/policycccreate-auth.bin  > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Load the storage key under the primary key"
load -hp 80000000 -ipr tmpspriv.bin -ipu tmpspub.bin -pwdp pps > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Start a policy session, bind to storage key"
startauthsession -se p -bi 80000001 -pwdb sto > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy command code - create"
policycommandcode -ha 03000000 -cc 153 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Policy authvalue"
policyauthvalue -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Create a signing key with response encryption"
create -hp 80000001 -si -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp sto -pwdk sig -se0 03000000 41 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Load the signing key to verify response encryption"
load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp sto > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the storage key"
flushcontext -ha 80000001 > run.out 
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the signing key"
flushcontext -ha 80000002 > run.out 
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

echo "Flush the session"
flushcontext -ha 03000000 > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
       )

REM # getcapability -cap 1 -pr 80000000
REM # getcapability -cap 1 -pr 02000000
