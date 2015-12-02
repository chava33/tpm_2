#!/bin/bash
#

#################################################################################
#										#
#			TPM2 regression test					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#	$Id: testshutdown.sh 364 2015-07-10 20:15:33Z kgoldman $			#
#										#
# (c) Copyright IBM Corporation 2015						#
# 										#
# All rights reserved.								#
# 										#
# Redistribution and use in source and binary forms, with or without		#
# modification, are permitted provided that the following conditions are	#
# met:										#
# 										#
# Redistributions of source code must retain the above copyright notice,	#
# this list of conditions and the following disclaimer.				#
# 										#
# Redistributions in binary form must reproduce the above copyright		#
# notice, this list of conditions and the following disclaimer in the		#
# documentation and/or other materials provided with the distribution.		#
# 										#
# Neither the names of the IBM Corporation nor the names of its			#
# contributors may be used to endorse or promote products derived from		#
# this software without specific prior written permission.			#
# 										#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		#
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		#
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR		#
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT		#
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	#
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		#
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,		#
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY		#
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		#
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE		#
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		#
#										#
#################################################################################

echo ""
echo "TPM Resume (state/state)"
echo ""

echo "PCR 0 Extend"
./pcrextend -ha 0 -if policies/aaa > run.out
checkSuccess $?

echo "PCR 0 Read"
./pcrread -ha 0 -of tmp1.bin > run.out
checkSuccess $?

echo "Start an HMAC session"
./startauthsession -se h > run.out
checkSuccess $?

echo "Start an HMAC session"
./startauthsession -se h > run.out
checkSuccess $?

echo "Save the session context"
./contextsave -ha 02000001 -of tmp.bin > run.out 
checkSuccess $?

echo "Load the signing key"
./load -hp 80000000 -ipr signpriv.bin -ipu signpub.bin -pwdp pps > run.out
checkSuccess $?

echo "Define index with write stclear, read stclear"
./nvdefinespace -hi o -ha 01000000 -pwdn nnn -sz 16 -at rst -at wst > run.out
checkSuccess $?

echo "NV Read Public, unwritten Name"
./nvreadpublic -ha 01000000 > run.out
checkSuccess $?

echo "NV write"
./nvwrite -ha 01000000 -pwdn nnn -if policies/aaa > run.out
checkSuccess $?

echo "Read lock"
./nvreadlock -ha 01000000 -pwdn nnn > run.out
checkSuccess $?

echo "Write lock"
./nvwritelock -ha 01000000 -pwdn nnn > run.out
checkSuccess $?

echo "Shutdown state"
./shutdown -s > run.out
checkSuccess $?

echo "Power cycle"
./powerup > run.out
checkSuccess $?

echo "Startup state"
./startup -s > run.out
checkSuccess $?

echo "PCR 0 Read"
./pcrread -ha 0 -of tmp2.bin > run.out
checkSuccess $?

echo "Verify that PCR 0 is restored"
diff tmp1.bin tmp2.bin > run.out
checkSuccess $?

echo "Signing Key Self Certify - should fail, signing key missing"
./certify -hk 80000001 -ho 80000001 -pwdk sig -pwdo sig -se0 02000000 1 > run.out
checkFailure $?

echo "Load the signing key - should fail, primary key missing"
./load -hp 80000000 -ipr signpriv.bin -ipu signpub.bin -pwdp pps > run.out
checkFailure $?

echo "Create a platform primary storage key"
./createprimary -hi e -pwdk pps > run.out
checkSuccess $?

echo "Signing Key Self Certify - should fail, signing key missing"
./certify -hk 80000001 -ho 80000001 -pwdk sig -pwdo sig -se0 02000000 1 > run.out
checkFailure $?

echo "Load the signing key"
./load -hp 80000000 -ipr signpriv.bin -ipu signpub.bin -pwdp pps > run.out
checkSuccess $?

echo "Signing Key Self Certify - should fail, session missing"
./certify -hk 80000001 -ho 80000001 -pwdk sig -pwdo sig -se0 02000000 1 > run.out
checkFailure $?

echo "Load the saved session context"
./contextload -if tmp.bin > run.out
checkSuccess $?

echo "Signing Key Self Certify"
./certify -hk 80000001 -ho 80000001 -pwdk sig -pwdo sig -se0 02000001 0 > run.out
checkSuccess $?

echo "NV write - should fail, still locked"
./nvwrite -ha 01000000 -pwdn nnn -if policies/aaa > run.out
checkFailure $?

echo "NV read - should fail, still locked"
./nvread -ha 01000000 -pwdn nnn -sz 16 > run.out
checkFailure $?

echo ""
echo "TPM Restart (state/clear)"
echo ""

echo "Shutdown state"
./shutdown -s > run.out
checkSuccess $?

echo "Power cycle"
./powerup > run.out
checkSuccess $?

echo "Startup clear"
./startup -c > run.out
checkSuccess $?

echo "PCR 0 Read"
./pcrread -ha 0 -halg sha1 -of tmp2.bin > run.out
checkSuccess $?

echo "Verify that PCR 0 is reset"
diff policies/policypcr0.bin tmp2.bin > run.out
checkSuccess $?

echo "NV write"
./nvwrite -ha 01000000 -pwdn nnn -if policies/aaa > run.out
checkSuccess $?

echo "NV read"
./nvread -ha 01000000 -pwdn nnn -sz 16 > run.out
checkSuccess $?



echo "NV Undefine Space"
./nvundefinespace -hi e -ha 01000000 > run.out
checkSuccess $?

echo "Recreate a platform primary storage key"
./createprimary -hi e -pwdk pps > run.out
checkSuccess $?

exit


getcapability  -cap 1 -pr 80000000
getcapability  -cap 1 -pr 02000000
getcapability  -cap 1 -pr 01000000
