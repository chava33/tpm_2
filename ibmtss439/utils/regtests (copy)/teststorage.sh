#!/bin/bash
#

#################################################################################
#										#
#			TPM2 regression test					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#	$Id: teststorage.sh 321 2015-06-04 19:14:39Z kgoldman $			#
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

# Primary storage key at 80000000 password pps
# storage key at 80000001 password sto

echo ""
echo "Storage key"
echo ""

echo "Load the storage key under the primary key"
./load -hp 80000000 -ipr storepriv.bin -ipu storepub.bin -pwdp pps > run.out
checkSuccess $?

echo "Start an HMAC auth session"
./startauthsession -se h > run.out
checkSuccess $?

for NALG in "sha1" "sha256" "sha384"
do

    for SESS in "" "-se0 02000000 1"
    do

	echo "Create an unrestricted signing key under the storage key ${NALG} ${SESS}"
	./create -hp 80000001 -si -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp sto -pwdk 111 -nalg ${NALG} ${SESS} > run.out
	checkSuccess $?

	echo "Load the signing key under the storage key ${SESS}"
	./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp sto ${SESS} > run.out
	checkSuccess $?

	echo "Flush the signing key"
	./flushcontext -ha 80000002 > run.out
	checkSuccess $?

	echo "Load external just the storage key public part ${NALG}"
	./loadexternal -halg sha256 -nalg ${NALG} -ipu storepub.bin > run.out
	checkSuccess $?

	echo "Flush the public key"
	./flushcontext -ha 80000002 > run.out
	checkSuccess $?

    done
done

echo "Flush the storage key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

echo "Flush the auth session"
./flushcontext -ha 02000000 > run.out
checkSuccess $?

# getcapability  -cap 1 -pr 80000000
# getcapability  -cap 1 -pr 02000000