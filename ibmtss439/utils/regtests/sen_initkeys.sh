#!/bin/bash
#
echo "# path to me --------------->  ${0}     "
echo "# my name ------------------>  ${0##*/} "
#################################################################################
#										#
#			TPM2 regression test					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#		$Id: initkeys.sh 352 2015-06-25 19:25:35Z kgoldman $		#
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


checkSuccess()
{
if [ $1 -ne 0 ]; then
    echo " ERROR:"
    cat run.out
    exit 255
else
    echo " INFO:"
fi

}

checkWarning()
{
if [ $1 -ne 0 ]; then
    echo " WARN: $2"
    ((WARN++))
else
    echo " INFO:"
fi

}

checkFailure()
{
if [ $1 -eq 0 ]; then
    echo " ERROR:"
    cat run.out
    exit 255
else
    echo " INFO:"
fi

}

cleanup()
{
# stdout
    rm -f run.out
# general purpose keys
    rm -f storepriv.bin
    rm -f storepub.bin
    rm -f signpriv.bin
    rm -f signpub.bin
    rm -f signrpriv.bin
    rm -f signrpub.bin
    rm -f derpriv.bin
    rm -f derpub.bin
    rm -f despriv.bin
    rm -f despub.bin
    rm -f khprivsha1.bin
    rm -f khpubsha1.bin
    rm -f khprivsha256.bin
    rm -f khpubsha256.bin
    rm -f khpubsha384.bin
    rm -f khprivsha384.bin
# misc
    rm -f dec.bin
    rm -f enc.bin
    rm -f msg.bin
    rm -f noncetpm.bin
    rm -f policyapproved.bin
    rm -f pssig.bin
    rm -f sig.bin
    rm -f tkt.bin
    rm -f tmp.bin
    rm -f tmp1.bin
    rm -f tmp2.bin
    rm -f tmpsha1.bin
    rm -f tmpsha256.bin
    rm -f tmpsha384.bin
    rm -f tmppriv.bin
    rm -f tmppub.bin
    rm -f tmpspriv.bin
    rm -f tmpspub.bin
    rm -f to.bin
    rm -f zero.bin
}


echo -n "1234567890123456" > msg.bin
touch zero.bin

# try to undefine any NV index left over from a previous test.  Do not check for errors.
./nvundefinespace -hi e -ha 01000000 > run.out
./nvundefinespace -hi e -ha 01000000 -pwdp ppp > run.out
./nvundefinespace -hi e -ha 01000001 > run.out
# same for persistent objects
./evictcontrol -ho 81800000 -hp 81800000 -hi e > run.out

echo ""
echo "Initialize Regression Test Keys"
echo ""

echo "Create a platform primary storage key"
./createprimary -hi e -pwdk pps > run.out
checkSuccess $?

echo "Create a storage key under the primary key"
./create -hp 80000001 -st -kt f -kt p -opr storepriv.bin -opu storepub.bin -pwdp pps -pwdk sto > run.out
checkSuccess $?

echo "Create an unrestricted signing key under the primary key"
./create -hp 80000001 -si -kt f -kt p -opr signpriv.bin -opu signpub.bin -pwdp pps -pwdk sig > run.out
checkSuccess $?

echo "Create a restricted signing key under the primary key"
./create -hp 80000001 -sir -kt f -kt p -opr signrpriv.bin -opu signrpub.bin -pwdp pps -pwdk sig > run.out
checkSuccess $?

echo "Create an RSA decryption key under the primary key"
./create -hp 80000001 -den -kt f -kt p -opr derpriv.bin -opu derpub.bin -pwdp pps -pwdk dec > run.out
checkSuccess $?

echo "Create a symmetric cipher key under the primary key"
./create -hp 80000001 -des -kt f -kt p -opr despriv.bin -opu despub.bin -pwdp pps -pwdk aes > run.out
RC=$?
checkWarning $RC "Symmetric cipher key may not support sign attribute"

if [ $RC -ne 0 ]; then
    echo "Create a rev 116 symmetric cipher key under the primary key"
    ./create -hp 80000001 -des -116 -kt f -kt p -opr despriv.bin -opu despub.bin -pwdp pps -pwdk aes > run.out
    checkSuccess $?
fi

for HALG in sha1 sha256 sha384

do

    echo "Create a ${HALG} keyed hash key under the primary key"
    ./create -hp 80000001 -kh -kt f -kt p -opr khpriv${HALG}.bin -opu khpub${HALG}.bin -pwdp pps -pwdk khk -halg ${HALG} > run.out
    checkSuccess $?

done

exit ${WARN}
