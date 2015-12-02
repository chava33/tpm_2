/********************************************************************************/
/*										*/
/*			     				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tsecrypto.h 366 2015-07-12 16:35:59Z kgoldman $		*/
/*										*/
/* (c) Copyright IBM Corporation 2015.						*/
/*										*/
/* All rights reserved.								*/
/* 										*/
/* Redistribution and use in source and binary forms, with or without		*/
/* modification, are permitted provided that the following conditions are	*/
/* met:										*/
/* 										*/
/* Redistributions of source code must retain the above copyright notice,	*/
/* this list of conditions and the following disclaimer.			*/
/* 										*/
/* Redistributions in binary form must reproduce the above copyright		*/
/* notice, this list of conditions and the following disclaimer in the		*/
/* documentation and/or other materials provided with the distribution.		*/
/* 										*/
/* Neither the names of the IBM Corporation nor the names of its		*/
/* contributors may be used to endorse or promote products derived from		*/
/* this software without specific prior written permission.			*/
/* 										*/
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		*/
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		*/
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR	*/
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT		*/
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	*/
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		*/
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,	*/
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY	*/
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		*/
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE	*/
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		*/
/********************************************************************************/

#ifndef TSECRYPTO_H
#define TSECRYPTO_H

#include "../src/TPM_Types.h"

LIB_EXPORT
TPM_RC TSE_Crypto_Init(void);

LIB_EXPORT
TPM_RC TSE_HMAC_Generate(TPMT_HA *digest,
			 const TPM2B_KEY *hmacKey,
			 ...);
LIB_EXPORT
TPM_RC TSE_HMAC_Verify(TPMT_HA *expect,
		       const TPM2B_KEY *hmacKey,
		       UINT32 sizeInBytes,
		       ...);
LIB_EXPORT
TPM_RC TSE_Hash_Generate(TPMT_HA *digest,
			 ...);

LIB_EXPORT
TPM_RC TSE_RandBytes(unsigned char *buffer, uint32_t size);

LIB_EXPORT
TPM_RC TSE_RSAPublicEncrypt(unsigned char* encrypt_data,
			    size_t encrypt_data_size,
			    const unsigned char *decrypt_data,
			    size_t decrypt_data_size,
			    unsigned char *narr,
			    uint32_t nbytes,
			    unsigned char *earr,
			    uint32_t ebytes,
			    unsigned char *p,
			    int pl,
			    TPMI_ALG_HASH halg);

TPM_RC TSE_AES_KeyGenerate(void);
TPM_RC TSE_AES_Encrypt(unsigned char **encrypt_data,
		       uint32_t *encrypt_length,
		       const unsigned char *decrypt_data,
		       uint32_t decrypt_length);
TPM_RC TSE_AES_Decrypt(unsigned char **decrypt_data,
		       uint32_t *decrypt_length,
		       const unsigned char *encrypt_data,
		       uint32_t encrypt_length);
#endif
