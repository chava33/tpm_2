REM #################################################################################
REM #										#
REM #			TPM2 regression test					#
REM #			     Written by Ken Goldman				#
REM #		       IBM Thomas J. Watson Research Center			#
REM #		$Id: testsign.sh 365 2015-07-10 21:28:31Z kgoldman $		#
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

echo ""
echo "Signing key"
echo ""

REM # loop over unrestricted hash algorithms

echo "Load the signing key under the primary key"
load -hp 80000000 -ipr signpriv.bin -ipu signpub.bin -pwdp pps > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

for %%H in (sha1 sha256) do (


    echo "Sign a digest - %%H"
    sign -hk 80000001 -halg %%H -if policies/aaa -os sig.bin -pwdk sig -ipu signpub.bin -ipem signpub.pem > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
    )

    echo "Verify the signature - %%H"
    verifysignature -hk 80000001 -halg %%H -if policies/aaa -is sig.bin > run.out
    IF %ERRORLEVEL% NEQ 0 (
       exit /B 1
    )

)

echo "Flush the signing key"
flushcontext -ha 80000001 > run.out
IF %ERRORLEVEL% NEQ 0 (
   exit /B 1
   )

REM # getcapability  -cap 1 -pr 80000000
REM # getcapability  -cap 1 -pr 02000000
