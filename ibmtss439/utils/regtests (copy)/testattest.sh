#!/bin/bash
#

#################################################################################
#										#
#			TPM2 regression test					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#	$Id: testattest.sh 365 2015-07-10 21:28:31Z kgoldman $			#
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
echo "Attestation"
echo ""

echo "Load the signing key under the primary key"
./load -hp 80000000 -ipr signpriv.bin -ipu signpub.bin -pwdp pps > run.out
checkSuccess $?

echo "NV Define Space"
./nvdefinespace -hi o -ha 01000000 -pwdn nnn -sz 16 > run.out
checkSuccess $?

echo "NV Read Public, unwritten Name"
./nvreadpublic -ha 01000000 > run.out
checkSuccess $?

echo "NV write"
./nvwrite -ha 01000000 -pwdn nnn -if msg.bin > run.out
checkSuccess $?

echo "Start an HMAC session"
./startauthsession -se h > run.out
checkSuccess $?

for SESS in "" "-se0 02000000 1"
do
    for HALG in sha1 sha256 sha384
    do

	echo "Signing Key Self Certify ${HALG} ${SESS}"
	./certify -hk 80000001 -ho 80000001 -halg ${HALG} -pwdk sig -pwdo sig ${SESS} -os sig.bin -oa tmp.bin -qd policies/aaa > run.out
	checkSuccess $?

	echo "Verify the signature ${HALG}"
	./verifysignature -hk 80000001 -halg ${HALG} -if tmp.bin -is sig.bin > run.out
	checkSuccess $?

	echo "Quote ${HALG} ${HALG} ${SESS}"
	./quote -hp 0 -hk 80000001 -halg ${HALG} -pwdk sig ${SESS} -os sig.bin -oa tmp.bin -qd policies/aaa > run.out
	checkSuccess $?

	echo "Verify the signature ${HALG}"
	./verifysignature -hk 80000001 -halg ${HALG} -if tmp.bin -is sig.bin > run.out
	checkSuccess $?

	echo "Get Time ${HALG} ${SESS}"
	./gettime -hk 80000001 -halg ${HALG} -pwdk sig ${SESS} -os sig.bin -oa tmp.bin -qd policies/aaa > run.out
	checkSuccess $?

	echo "Verify the signature ${HALG}"
	./verifysignature -hk 80000001 -halg ${HALG} -if tmp.bin -is sig.bin > run.out
	checkSuccess $?

	echo "NV Certify ${HALG} ${SESS}"
	./nvcertify -ha 01000000 -pwdn nnn -hk 80000001 -pwdk sig -halg ${HALG} -sz 16 ${SESS} -os sig.bin -oa tmp.bin > run.out
	checkSuccess $?

	echo "Verify the signature ${HALG}"
	./verifysignature -hk 80000001 -halg ${HALG} -if tmp.bin -is sig.bin > run.out
	checkSuccess $?

	echo "Get command audit digest ${HALG} ${SESS}"
	./getcommandauditdigest -hk 80000001 -halg ${HALG} ${SESS} -pwdk sig -os sig.bin -oa tmp.bin -qd policies/aaa > run.out
	checkSuccess $?

	echo "Verify the signature"
	./verifysignature -hk 80000001 -halg ${HALG} -if tmp.bin -is sig.bin > run.out
	checkSuccess $?

    done
done

echo "Flush the attestation key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

echo "NV Undefine Space"
./nvundefinespace -hi o -ha 01000000 > run.out
checkSuccess $?

echo "Flush the auth session"
./flushcontext -ha 02000000 > run.out
checkSuccess $?

echo ""
echo "Audit"
echo ""

# 80000001 signing key
# 02000000 hmac and audit session

echo ""
echo "Audit with one session"
echo ""

echo "Load the audit signing key"
./load -hp 80000000 -ipr signpriv.bin -ipu signpub.bin -pwdp pps > run.out
checkSuccess $?

for BIND in "" "-bi 80000001 -pwdb sig"
do
    for HALG in sha1 sha256 sha384
    do

	echo "Start an HMAC auth session ${HALG} ${BIND}"
	./startauthsession -se h -halg ${HALG} ${BIND} > run.out
	checkSuccess $?

	echo "Sign a digest ${HALG}"
	./sign -hk 80000001 -halg ${HALG} -if policies/aaa -os sig.bin -pwdk sig -ipu signpub.bin -se0 02000000 81 > run.out
	checkSuccess $?

	echo "Sign a digest ${HALG}"
	./sign -hk 80000001 -halg ${HALG} -if policies/aaa -os sig.bin -pwdk sig -se0 02000000 81 -ipu signpub.bin > run.out
	checkWarning $? "Interaction between bind and audit session response HMAC may not be fixed"

	echo "Get Session Audit Digest ${HALG}"
	./getsessionauditdigest -hs 02000000 -hk 80000001 -pwdk sig -halg ${HALG} -os sig.bin -oa tmp.bin -qd policies/aaa > run.out
	checkSuccess $?

	echo "Verify the signature ${HALG}"
	./verifysignature -hk 80000001 -halg ${HALG} -if tmp.bin -is sig.bin > run.out
	checkSuccess $?

	echo "Flush the session"
	./flushcontext -ha 02000000 > run.out
	checkSuccess $?

    done
done

echo "Flush the signing key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

# 80000001 signing key
# 02000000 hmac session
# 02000001 audit session

echo ""
echo "Audit with HMAC and audit sessions"
echo ""

echo "Load the audit signing key"
./load -hp 80000000 -ipr signpriv.bin -ipu signpub.bin -pwdp pps > run.out
checkSuccess $?

echo "Start an HMAC auth session"
./startauthsession -se h > run.out
checkSuccess $?

for SESS in "" "-se0 02000000 1"
do

    for HALG in sha1 sha256 sha384
    do

	echo "Start an audit session ${HALG}"
	./startauthsession -se h -halg ${HALG} > run.out
	checkSuccess $?

	echo "Sign a digest ${HALG}"
	./sign -hk 80000001 -halg $HALG -if policies/aaa -os sig.bin -pwdk sig -ipu signpub.bin -se0 02000001 81 > run.out
	checkSuccess $?

	echo "Get Session Audit Digest ${SESS}"
	./getsessionauditdigest -hs 02000001 -hk 80000001 -pwdk sig -os sig.bin -oa tmp.bin ${SESS} -qd policies/aaa > run.out
	checkSuccess $?

	echo "Verify the signature"
	./verifysignature -hk 80000001 -if tmp.bin -is sig.bin > run.out
	checkSuccess $?

	echo "Flush the session"
	./flushcontext -ha 02000001 > run.out
	checkSuccess $?

    done
done

echo "Flush the signing key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

echo "Flush the session"
./flushcontext -ha 02000000 > run.out
checkSuccess $?

exit ${WARN}

# ./getcapability -cap 1 -pr 80000000
# ./getcapability -cap 1 -pr 02000000
