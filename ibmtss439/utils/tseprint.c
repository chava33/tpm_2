/********************************************************************************/
/*										*/
/*			     Structure Print Utilities				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tseprint.c 348 2015-06-22 20:36:45Z kgoldman $		*/
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "Unmarshal_fp.h"

#include "tseprint.h"

/* TPM_PrintAll() prints 'string', the length, and then the entire byte array
 */

void TPM_PrintAll(const char *string, const unsigned char* buff, uint32_t length)
{
    uint32_t i;
    if (buff != NULL) {
        printf("%s length %u\n ", string, length);
        for (i = 0 ; i < length ; i++) {
            if (i && !( i % 16 )) {
                printf("\n ");
            }
            printf("%.2x ",buff[i]);
        }
        printf("\n");
    }
    else {
        printf("%s null\n", string);
    }
    return;
}

/* Table 9 - Definition of (UINT16) TPM_ALG_ID Constants <IN/OUT, S> */

void TPM_ALG_ID_Print(TPM_ALG_ID source)
{
    switch (source) {
#ifdef TPM_ALG_AES
      case TPM_ALG_AES:
	printf("TPM_ALG_ID TPM_ALG_AES\n");
	break;
#endif
#ifdef TPM_ALG_CAMELLIA
      case TPM_ALG_CAMELLIA:
	printf("TPM_ALG_ID TPM_ALG_CAMELLIA\n");
	break;
#endif
#ifdef TPM_ALG_CBC
      case TPM_ALG_CBC:
	printf("TPM_ALG_ID TPM_ALG_CBC\n");
	break;
#endif
#ifdef TPM_ALG_CFB
      case TPM_ALG_CFB:
	printf("TPM_ALG_ID TPM_ALG_CFB\n");
	break;
#endif
#ifdef TPM_ALG_CTR
      case TPM_ALG_CTR:
	printf("TPM_ALG_ID TPM_ALG_CTR\n");
	break;
#endif
#ifdef TPM_ALG_ECB
      case TPM_ALG_ECB:
	printf("TPM_ALG_ID TPM_ALG_ECB\n");
	break;
#endif
#ifdef TPM_ALG_ECC
      case TPM_ALG_ECC:
	printf("TPM_ALG_ID TPM_ALG_ECC\n");
	break;
#endif
#ifdef TPM_ALG_ECDAA
      case TPM_ALG_ECDAA:
	printf("TPM_ALG_ID TPM_ALG_ECDAA\n");
	break;
#endif
#ifdef TPM_ALG_ECDH
      case TPM_ALG_ECDH:
	printf("TPM_ALG_ID TPM_ALG_ECDH\n");
	break;
#endif
#ifdef TPM_ALG_ECDSA
      case TPM_ALG_ECDSA:
	printf("TPM_ALG_ID TPM_ALG_ECDSA\n");
	break;
#endif
#ifdef TPM_ALG_ECMQV
      case TPM_ALG_ECMQV:
	printf("TPM_ALG_ID TPM_ALG_ECMQV\n");
	break;
#endif
#ifdef TPM_ALG_ECSCHNORR
      case TPM_ALG_ECSCHNORR:
	printf("TPM_ALG_ID TPM_ALG_ECSCHNORR\n");
	break;
#endif
#ifdef TPM_ALG_HMAC
      case TPM_ALG_HMAC:
	printf("TPM_ALG_ID TPM_ALG_HMAC\n");
	break;
#endif
#ifdef TPM_ALG_KDF1_SP800_108
      case TPM_ALG_KDF1_SP800_108:
	printf("TPM_ALG_ID TPM_ALG_KDF1_SP800_108\n");
	break;
#endif
#ifdef TPM_ALG_KDF1_SP800_56A
      case TPM_ALG_KDF1_SP800_56A:
	printf("TPM_ALG_ID TPM_ALG_KDF1_SP800_56A\n");
	break;
#endif
#ifdef TPM_ALG_KDF2
      case TPM_ALG_KDF2:
	printf("TPM_ALG_ID TPM_ALG_KDF2\n");
	break;
#endif
#ifdef TPM_ALG_KEYEDHASH
      case TPM_ALG_KEYEDHASH:
	printf("TPM_ALG_ID TPM_ALG_KEYEDHASH\n");
	break;
#endif
#ifdef TPM_ALG_MGF1
      case TPM_ALG_MGF1:
	printf("TPM_ALG_ID TPM_ALG_MGF1\n");
	break;
#endif
#ifdef TPM_ALG_OAEP
      case TPM_ALG_OAEP:
	printf("TPM_ALG_ID TPM_ALG_OAEP\n");
	break;
#endif
#ifdef TPM_ALG_OFB
      case TPM_ALG_OFB:
	printf("TPM_ALG_ID TPM_ALG_OFB\n");
	break;
#endif
#ifdef TPM_ALG_RSA
      case TPM_ALG_RSA:
	printf("TPM_ALG_ID TPM_ALG_RSA\n");
	break;
#endif
#ifdef TPM_ALG_RSAES
      case TPM_ALG_RSAES:
	printf("TPM_ALG_ID TPM_ALG_RSAES\n");
	break;
#endif
#ifdef TPM_ALG_RSAPSS
      case TPM_ALG_RSAPSS:
	printf("TPM_ALG_ID TPM_ALG_RSAPSS\n");
	break;
#endif
#ifdef TPM_ALG_RSASSA
      case TPM_ALG_RSASSA:
	printf("TPM_ALG_ID TPM_ALG_RSASSA\n");
	break;
#endif
#ifdef TPM_ALG_SHA1
      case TPM_ALG_SHA1:
	printf("TPM_ALG_ID TPM_ALG_SHA1\n");
	break;
#endif
#ifdef TPM_ALG_SHA256
      case TPM_ALG_SHA256:
	printf("TPM_ALG_ID TPM_ALG_SHA256\n");
	break;
#endif
#ifdef TPM_ALG_SHA384
      case TPM_ALG_SHA384:
	printf("TPM_ALG_ID TPM_ALG_SHA384\n");
	break;
#endif
#ifdef TPM_ALG_SHA512
      case TPM_ALG_SHA512:
	printf("TPM_ALG_ID TPM_ALG_SHA512\n");
	break;
#endif
#ifdef TPM_ALG_SM2
      case TPM_ALG_SM2:
	printf("TPM_ALG_ID TPM_ALG_SM2\n");
	break;
#endif
#ifdef TPM_ALG_SM3_256
      case TPM_ALG_SM3_256:
	printf("TPM_ALG_ID TPM_ALG_SM3_256\n");
	break;
#endif
#ifdef TPM_ALG_SM4
      case TPM_ALG_SM4:
	printf("TPM_ALG_ID TPM_ALG_SM4\n");
	break;
#endif
#ifdef TPM_ALG_SYMCIPHER
      case TPM_ALG_SYMCIPHER:
	printf("TPM_ALG_ID TPM_ALG_SYMCIPHER\n");
	break;
#endif
#ifdef TPM_ALG_XOR
      case TPM_ALG_XOR:
	printf("TPM_ALG_ID TPM_ALG_XOR\n");
	break;
#endif
      default:
	printf("TPM_ALG_ID algorithm %04hx unknown\n", source);
    }
    return;
}

/* Table 85 - Definition of TPMS_PCR_SELECTION Structure */

void TPMS_PCR_SELECTION_Print(TPMS_PCR_SELECTION *source)
{
    TPM_ALG_ID_Print(source->hash);
    TPM_PrintAll("TPMS_PCR_SELECTION",
		 source->pcrSelect,
		 source->sizeofSelect);
    return;
}

/* Table 102 - Definition of TPML_PCR_SELECTION Structure */

void TPML_PCR_SELECTION_Print(TPML_PCR_SELECTION *source)
{
    UINT32 i;
    printf("TPML_PCR_SELECTION count %u\n", source->count);
    for (i = 0 ; (i < source->count) ; i++) {
	TPMS_PCR_SELECTION_Print(&source->pcrSelections[i]);
    }
    return;
}

/* Table 109 - Definition of TPMS_CLOCK_INFO Structure */

void TPMS_CLOCK_INFO_Print(TPMS_CLOCK_INFO *source)
{
    printf("TPMS_CLOCK_INFO clock %"PRIu64"\n", source->clock);
    printf("TPMS_CLOCK_INFO resetCount %u\n", source->resetCount);
    printf("TPMS_CLOCK_INFO restartCount %u\n", source->restartCount);
    printf("TPMS_CLOCK_INFO safe %x\n", source->safe);
    return;
}

/* Table 110 - Definition of TPMS_TIME_INFO Structure */

void TPMS_TIME_INFO_Print(TPMS_TIME_INFO *source)
{
    printf("TPMS_TIME_INFO time %"PRIu64"\n", source->time);
    TPMS_CLOCK_INFO_Print(&source->clockInfo);
    return;
}
    
/* Table 111 - Definition of TPMS_TIME_ATTEST_INFO Structure <OUT> */

void TPMS_TIME_ATTEST_INFO_Print(TPMS_TIME_ATTEST_INFO *source)
{
    TPMS_TIME_INFO_Print(&source->time);
    printf("TPMS_TIME_ATTEST_INFO firmwareVersion %"PRIu64"\n", source->firmwareVersion);
    return;
}

/* Table 112 - Definition of TPMS_CERTIFY_INFO Structure <OUT> */

void TPMS_CERTIFY_INFO_Print(TPMS_CERTIFY_INFO *source)
{
    TPM_PrintAll("TPMS_CERTIFY_INFO name",
		 source->name.b.buffer,
		 source->name.b.size);
    TPM_PrintAll("TPMS_CERTIFY_INFO qualifiedName",
		 source->qualifiedName.b.buffer,
		 source->qualifiedName.b.size);
    return;
}

/* Table 113 - Definition of TPMS_QUOTE_INFO Structure <OUT> */

void TPMS_QUOTE_INFO_Print(TPMS_QUOTE_INFO *source)
{
    TPML_PCR_SELECTION_Print(&source->pcrSelect);
    TPM_PrintAll("TPMS_QUOTE_INFO pcrDigest",
		 source->pcrDigest.b.buffer,
		 source->pcrDigest.b.size);
    return;
}

/* Table 115 - Definition of TPMS_SESSION_AUDIT_INFO Structure <OUT> */

void TPMS_SESSION_AUDIT_INFO_Print(TPMS_SESSION_AUDIT_INFO *source)
{
    printf("TPMS_SESSION_AUDIT_INFO exclusiveSession %d\n",  source->exclusiveSession);
    TPM_PrintAll("TPMS_SESSION_AUDIT_INFO sessionDigest",
		 source->sessionDigest.b.buffer,
		 source->sessionDigest.b.size);
    return;
}

/* Table 118 - Definition of (TPM_ST) TPMI_ST_ATTEST Type <OUT> */

void TPMI_ST_ATTEST_Print(TPMI_ST_ATTEST selector)
{
    switch (selector) {
      case TPM_ST_ATTEST_CERTIFY:
	printf("TPMI_ST_ATTEST TPM_ST_ATTEST_CERTIFY\n");
	break;
      case TPM_ST_ATTEST_CREATION:
	printf("TPMI_ST_ATTEST TPM_ST_ATTEST_CREATION\n");
	break;
      case TPM_ST_ATTEST_QUOTE:
	printf("TPMI_ST_ATTEST TPM_ST_ATTEST_QUOTE\n");
	break;
      case TPM_ST_ATTEST_COMMAND_AUDIT:
	printf("TPMI_ST_ATTEST TPM_ST_ATTEST_COMMAND_AUDIT\n");
	break;
      case TPM_ST_ATTEST_SESSION_AUDIT:
	printf("TPMI_ST_ATTEST TPM_ST_ATTEST_SESSION_AUDIT\n");
	break;
      case TPM_ST_ATTEST_TIME:
	printf("TPMI_ST_ATTEST TPM_ST_ATTEST_TIME\n");
	break;
      case TPM_ST_ATTEST_NV:
	printf("TPMI_ST_ATTEST TPM_ST_ATTEST_NV\n");
	break;
      default:
	printf("TPMI_ST_ATTEST_Print: selection %04hx not implemented\n", selector);
    }
    return;
    
}

/* Table 119 - Definition of TPMU_ATTEST Union <OUT> */

void TPMU_ATTEST_Print(TPMU_ATTEST *source, TPMI_ST_ATTEST selector)
{
    switch (selector) {
      case TPM_ST_ATTEST_CERTIFY:
	TPMS_CERTIFY_INFO_Print(&source->certify);
	break;
#if 0
      case TPM_ST_ATTEST_CREATION:
	TPMS_CREATION_INFO_Print(&source->creation);
	break;
#endif
      case TPM_ST_ATTEST_QUOTE:
	TPMS_QUOTE_INFO_Print(&source->quote);
	break;
#if 0
      case TPM_ST_ATTEST_COMMAND_AUDIT:
	TPMS_COMMAND_AUDIT_INFO_Print(&source->commandAudit);
	break;
#endif
      case TPM_ST_ATTEST_SESSION_AUDIT:
	TPMS_SESSION_AUDIT_INFO_Print(&source->sessionAudit);
	break;
      case TPM_ST_ATTEST_TIME:
	TPMS_TIME_ATTEST_INFO_Print(&source->time);
	break;
#if 0
      case TPM_ST_ATTEST_NV:
	TPMS_NV_CERTIFY_INFO_Print(&source->nv);
	break;
#endif
      default:
	printf("TPMU_ATTEST selection %04hx not implemented\n", selector);
	
    }
    return;
}

/* Table 120 - Definition of TPMS_ATTEST Structure <OUT> */

void TPMS_ATTEST_Print(TPMS_ATTEST *source)
{
    printf("TPMS_ATTEST magic %08x\n", source->magic);
    TPMI_ST_ATTEST_Print(source->type);
    TPM_PrintAll("TPMS_ATTEST extraData",
		 source->extraData.b.buffer,
    		 source->extraData.b.size);
    TPMS_CLOCK_INFO_Print(&source->clockInfo);
    TPMU_ATTEST_Print(&source->attested, source->type);
    return ;
}

/* Table 121 - Definition of TPM2B_ATTEST Structure <OUT> */

void TPM2B_ATTEST_Print(TPM2B_ATTEST *source)
{
    TPM_RC			rc = 0;
    TPMS_ATTEST 		attests;
    INT32			size;
    uint8_t			*buffer = NULL;

    /* unmarshal the TPMS_ATTEST from the TPM2B_ATTEST */
    if (rc == 0) {
	buffer = source->t.attestationData;
	size = source->t.size;
	rc = TPMS_ATTEST_Unmarshal(&attests, &buffer, &size);
    }
    if (rc == 0) {
	TPMS_ATTEST_Print(&attests);
    }
    else {
	printf("TPMS_ATTEST_Unmarshal failed\n");
    }
    return;
}

/* Table 168 - Definition of {RSA} TPMS_SIGNATURE_RSA Structure */

void TPMS_SIGNATURE_RSA_Print(TPMS_SIGNATURE_RSA *source)
{
    TPM_ALG_ID_Print(source->hash);
    TPM_PrintAll("TPMS_SIGNATURE_RSA",
		 source->sig.t.buffer,
		 source->sig.t.size);
    return;
}

/* Table 169 - Definition of Types for {RSA} Signature */

void TPMS_SIGNATURE_RSASSA_Print(TPMS_SIGNATURE_RSASSA *source)
{
    TPMS_SIGNATURE_RSA_Print(source);
    return;
}

/* Table 172 - Definition of TPMU_SIGNATURE Union <IN/OUT, S> */

void TPMU_SIGNATURE_Print(TPMU_SIGNATURE *source, TPMI_ALG_SIG_SCHEME selector)
{
    switch (selector) {
      case TPM_ALG_RSASSA:
	TPMS_SIGNATURE_RSASSA_Print(&source->rsassa);
	break;
#if 0
      case TPM_ALG_RSAPSS:
	TPMS_SIGNATURE_RSAPSS_Print(&source->rsapss);
	break;
      case TPM_ALG_ECDSA:
	TPMS_SIGNATURE_ECDSA_Print(&source->ecdsa);
	break;
      case TPM_ALG_ECDAA:
	TPMS_SIGNATURE_ECDSA_Print(&source->ecdaa);
	break;
      case TPM_ALG_SM2:
	TPMS_SIGNATURE_ECDSA_Print(&source->sm2);
	break;
      case TPM_ALG_ECSCHNORR:
	TPMS_SIGNATURE_ECDSA_Print(&source->ecschnorr);
	break;
      case TPM_ALG_HMAC:
	TPMT_HA_Print(&source->hmac);
	break;
#endif
      default:
	printf("TPMU_SIGNATURE selection not implemented\n");
	
    }
}

/* Table 173 - Definition of TPMT_SIGNATURE Structure */

void TPMT_SIGNATURE_Print(TPMT_SIGNATURE *source)
{
    TPM_ALG_ID_Print(source->sigAlg);
    TPMU_SIGNATURE_Print(&source->signature, source->sigAlg);
    return;
}

