#!/bin/bash
#
echo "# path to me --------------->  ${0}     "
echo "# my name ------------------>  ${0##*/} "
#################################################################################
#										#
#			TPM2 regression test					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#		$Id: testda.sh 362 2015-07-07 21:05:59Z kgoldman $		#
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
echo "DA Logic"
echo ""

echo "Create an signing key with DA protection"
./create -hp 80000000 -si -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sig -da > run.out
checkSuccess $?

echo "Load the signing key"
./load -hp 80000000 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkSuccess $?

echo "Set DA recovery time to 0, disables DA"
./dictionaryattackparameters -nrt 0 > run.out
checkSuccess $?

echo "Sign a digest with bad password - should fail"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk xxx > run.out
checkFailure $?

echo "Sign a digest with good password, no lockout"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk sig > run.out
checkSuccess $?

echo "Set DA recovery time to 120 sec, enables DA"
./dictionaryattackparameters -nrt 120 > run.out
checkSuccess $?

echo "Sign a digest with bad password - should fail"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk xxx > run.out
checkFailure $?

echo "Sign a digest with good password, lockout - should fail"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk sig > run.out
checkFailure $?

echo "Reset DA lock"
./dictionaryattacklockreset > run.out
checkSuccess $?

echo "Sign a digest with good password"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk sig > run.out
checkSuccess $?

echo "Set DA recovery time to 120 sec, enables DA, max tries 2"
./dictionaryattackparameters -nrt 120 -nmt 2 > run.out
checkSuccess $?

echo "Sign a digest with bad password - should fail"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk xxx > run.out
checkFailure $?

echo "Sign a digest with good password, no lockout yet"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk sig > run.out
checkSuccess $?

echo "Sign a digest with bad password - should fail"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk xxx > run.out
checkFailure $?

echo "Sign a digest with good password, lockout - should fail"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk sig > run.out
checkFailure $?

echo "Reset DA lock"
./dictionaryattacklockreset > run.out
checkSuccess $?

echo "Sign a digest with good password, no lockout"
./sign -hk 80000001 -if msg.bin -os sig.bin -pwdk sig > run.out
checkSuccess $?

echo "Set DA recovery time to 0, disables DA"
./dictionaryattackparameters -nrt 0 > run.out
checkSuccess $?

echo ""
echo "Lockout Auth"
echo ""

echo "Change lockout auth"
./hierarchychangeauth -hi l -pwdn lll > run.out
checkSuccess $?

echo "Reset DA lock with good password"
./dictionaryattacklockreset -pwd lll
checkSuccess $?

echo "Set DA recovery time to 0 with good password"
./dictionaryattackparameters -nrt 0 -pwd lll
checkSuccess $?

echo "Clear lockout auth"
./hierarchychangeauth -hi l -pwda lll
checkSuccess $?

echo "Set DA recovery time to 0"
./dictionaryattackparameters -nrt 0
checkSuccess $?

echo "Reset DA lock"
./dictionaryattacklockreset
checkSuccess $?

echo "Flush signing key"
./flushcontext -ha 80000001
checkSuccess $?

# ./getcapability -cap 1 -pr 80000000
