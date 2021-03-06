#!/bin/bash
#

#################################################################################
#										#
#			TPM2 regression test					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#		$Id: testhmac.sh 321 2015-06-04 19:14:39Z kgoldman $		#
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
echo "Keyed hash HMAC key"
echo ""

echo "Start an HMAC auth session"
./startauthsession -se h > run.out
checkSuccess $?

for HALG in sha1 sha256 sha384
do

    echo "Load the ${HALG} keyed hash key under the primary key"
    ./load -hp 80000000 -ipr khpriv${HALG}.bin -ipu khpub${HALG}.bin -pwdp pps > run.out
    checkSuccess $?

    for SESS in "" "-se0 02000000 1"
    do

	echo "HMAC ${HALG} using the keyed hash key ${SESS}"
	./hmac -hk 80000001 -if msg.bin -os sig.bin -pwdk khk -halg ${HALG} ${SESS} > run.out
	checkSuccess $?

	echo "HMAC ${HALG} start using the keyed hash key ${SESS}"
	./hmacstart -hk 80000001 -pwdk khk -pwda aaa ${SESS} -halg ${HALG} > run.out
	checkSuccess $?

	echo "HMAC ${HALG} sequence update ${SESS}"
	./sequenceupdate -hs 80000002 -pwds aaa -if msg.bin ${SESS} > run.out
	checkSuccess $?

	echo "HMAC ${HALG} sequence complete ${SESS}"
	./sequencecomplete -hs 80000002 -pwds aaa -of tmp.bin ${SESS} > run.out
	checkSuccess $?

	echo "Verify the HMAC ${HALG} using the two methods"
	diff sig.bin tmp.bin
	checkSuccess $?

    done

    echo "Flush the ${HALG} symmetric cipher key"
    ./flushcontext -ha 80000001 > run.out
    checkSuccess $?

done

echo ""
echo "Hash"
echo ""

for HALG in sha1 sha256 sha384
do

    for SESS in "" "-se0 02000000 1"
    do

	echo "Hash ${HALG} in one call"
	./hash -hi p -halg ${HALG} -if policies/aaa -oh tmp.bin > run.out
	checkSuccess $?

	echo "Verify the hash ${HALG}"
	diff tmp.bin policies/${HALG}aaa.bin > run.out
	checkSuccess $?

	echo "Hash ${HALG} sequence start"
	./hashsequencestart -halg ${HALG} -pwda aaa > run.out
	checkSuccess $?

	echo "Hash ${HALG} sequence update ${SESS}"
	./sequenceupdate -hs 80000001 -pwds aaa -if policies/aaa ${SESS} > run.out
	checkSuccess $?

	echo "Hash ${HALG} sequence complete ${SESS}"
	./sequencecomplete -hi p -hs 80000001 -pwds aaa -of tmp.bin ${SESS} > run.out
	checkSuccess $?

	echo "Verify the ${HALG} hash"
	diff tmp.bin policies/${HALG}aaa.bin > run.out
	checkSuccess $?

    done
done

echo "Flush the auth session"
./flushcontext -ha 02000000 > run.out
checkSuccess $?

# ./getcapability -cap 1 -pr 80000000
# ./getcapability -cap 1 -pr 02000000

echo ""
echo "Sign with ticket"
echo ""

echo "Load the signing key under the primary key"
./load -hp 80000000 -ipr signrpriv.bin -ipu signrpub.bin -pwdp pps > run.out
checkSuccess $?

echo "Hash and create ticket"
./hash -hi p -halg sha256 -if msg.bin -oh sig.bin -tk tkt.bin > run.out
checkSuccess $?

echo "Sign a digest with a restricted signing key and no ticket - should fail"
./sign -hk 80000001 -halg sha256 -if msg.bin -os sig.bin -pwdk sig  > run.out
checkFailure $?

echo "Sign a digest with a restricted signing key and ticket"
./sign -hk 80000001 -halg sha256 -if msg.bin -tk tkt.bin -os sig.bin -pwdk sig  > run.out
checkSuccess $?

echo "Hash and create null ticket, msg with TPM_GENERATED"
./hash -hi p -halg sha256 -if policies/msgtpmgen.bin -oh sig.bin -tk tkt.bin > run.out
checkSuccess $?

echo "Sign a digest with a restricted signing key and ticket - should fail"
./sign -hk 80000001 -halg $HALG -if msg.bin -tk tkt.bin -os sig.bin -pwdk sig  > run.out
checkFailure $?

echo "Hash sequence start"
./hashsequencestart -halg sha256 -pwda aaa > run.out
checkSuccess $?

echo "Hash sequence update "
./sequenceupdate -hs 80000002 -pwds aaa -if msg.bin > run.out
checkSuccess $?

echo "Hash sequence complete"
./sequencecomplete -hi p -hs 80000002 -pwds aaa -of tmp.bin -tk tkt.bin > run.out
checkSuccess $?

echo "Sign a digest with a restricted signing key and no ticket - should fail"
./sign -hk 80000001 -halg  sha256 -if msg.bin -os sig.bin -pwdk sig  > run.out
checkFailure $?

echo "Sign a digest with a restricted signing key and ticket"
./sign -hk 80000001 -halg sha256 -if msg.bin -tk tkt.bin -os sig.bin -pwdk sig  > run.out
checkSuccess $?

echo "Hash sequence start"
./hashsequencestart -halg sha256 -pwda aaa -halg sha256 > run.out
checkSuccess $?

echo "Hash sequence update, msg with TPM_GENERATED"
./sequenceupdate -hs 80000002 -pwds aaa -if policies/msgtpmgen.bin > run.out
checkSuccess $?

echo "Hash sequence complete"
./sequencecomplete -hi p -hs 80000002 -pwds aaa -of tmp.bin -tk tkt.bin > run.out
checkSuccess $?

echo "Sign a digest with a restricted signing key and ticket - should fail"
./sign -hk 80000001 -halg $HALG -if msg.bin -tk tkt.bin -os sig.bin -pwdk sig  > run.out
checkFailure $?

echo "Flush the signing key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

# ./getcapability -cap 1 -pr 80000000
# ./getcapability -cap 1 -pr 02000000

