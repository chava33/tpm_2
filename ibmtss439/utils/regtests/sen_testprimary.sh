#!/bin/bash
#
echo "# path to me --------------->  ${0}     "
echo "# my name ------------------>  ${0##*/} "
#################################################################################
#										#
#			TPM2 regression test					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#	$Id: testprimary.sh 371 2015-08-14 21:11:56Z kgoldman $			#
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

echo ""
echo "Primary key - CreatePrimary"
echo ""

echo "Create a primary storage key"
./createprimary -hi e -pwdk pps > run.out
checkSuccess $?

echo "Read the public part"
./readpublic -ho 80000001  > run.out
checkSuccess $?

echo "Create a storage key under the primary key"
./create -hp 80000001 -st -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sto > run.out
checkSuccess $?

echo "Load the storage key under the primary key"
./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkSuccess $?

echo "Flush the storage key"
./flushcontext -ha 80000002 > run.out
checkSuccess $?

echo "Flush the primary storage key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

echo "Load the storage key under the primary key - should fail"
./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkFailure $?

echo ""
echo "Primary key - CreatePrimary with no unique field"
echo ""

# no unique 

echo "Create a primary storage key with no unique field"
./createprimary -hi e -pwdk pps > run.out
checkSuccess $?

echo "Create a storage key under the primary key"
./create -hp 80000001 -st -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sto > run.out
checkSuccess $?

echo "Load the storage key under the primary key"
./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkSuccess $?

echo "Flush the storage key"
./flushcontext -ha 80000002 > run.out
checkSuccess $?

echo "Flush the primary storage key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

# empty unique

echo "Create a primary storage key with no unique field"
touch empty.bin
./createprimary -hi e -pwdk pps -iu empty.bin > run.out
checkSuccess $?

echo "Load the original storage key under the primary key with empty unique field"
./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkSuccess $?

echo "Flush the storage key"
./flushcontext -ha 80000002 > run.out
checkSuccess $?

echo "Flush the primary storage key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

echo ""
echo "Primary key - CreatePrimary with unique field"
echo ""

# unique

echo "Create a primary storage key with unique field"
touch empty.bin
./createprimary -hi e -pwdk pps -iu policies/aaa > run.out
checkSuccess $?

echo "Load the original storage key under the primary key - should fail"
./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkFailure $?

echo "Create a storage key under the primary key"
./create -hp 80000001 -st -kt f -kt p -opr tmppriv.bin -opu tmppub.bin -pwdp pps -pwdk sto > run.out
checkSuccess $?

echo "Load the storage key under the primary key"
./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkSuccess $?

echo "Flush the storage key"
./flushcontext -ha 80000002 > run.out
checkSuccess $?

echo "Flush the primary storage key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

# same unique

echo "Create a primary storage key with same unique field"
./createprimary -hi e -pwdk pps -iu policies/aaa > run.out
checkSuccess $?

echo "Load the previous storage key under the primary key"
./load -hp 80000001 -ipr tmppriv.bin -ipu tmppub.bin -pwdp pps > run.out
checkSuccess $?

echo "Flush the storage key"
./flushcontext -ha 80000002 > run.out
checkSuccess $?

echo "Flush the primary storage key"
./flushcontext -ha 80000001 > run.out
checkSuccess $?

# cleanup

rm -f empty.bin

# getcapability  -cap 1 -pr 80000000
