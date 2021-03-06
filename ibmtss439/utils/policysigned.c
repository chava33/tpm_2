/********************************************************************************/
/*										*/
/*			    PolicySigned	 				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: policysigned.c 317 2015-05-28 18:25:09Z kgoldman $		*/
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

/* 

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "tse.h"
#include "tseutils.h"
#include "tseresponsecode.h"
#include "tseprint.h"
#include "tssmarshal.h"

static void   printUsage(void);
static TPM_RC signatureCallback(TPMT_HA *digest,
				uint8_t *buffer,
				uint16_t *length);

const char			*signingKeyFilename = NULL;
const char			*signingKeyPassword = NULL;

int verbose = FALSE;

int main(int argc, char *argv[])
{
    TPM_RC			rc = 0;
    int				i;    /* argc iterator */
    PolicySigned_In 		in;
    PolicySigned_Out 		out;
    PolicySigned_Extra		extra;
    TPMI_DH_OBJECT		authObject = 0;
    TPMI_SH_POLICY		policySession = 0;
    const char 			*nonceTPMFilename = NULL;
    const char 			*cpHashAFilename = NULL;
    const char			*policyRefFilename = NULL;
    const char			*ticketFilename = NULL;
    const char			*timeoutFilename = NULL;
    INT32			expiration = 0;
    TPMI_ALG_HASH		halg = TPM_ALG_SHA256;
    
    TSE_SetProperty(TPM_TRACE_LEVEL, "1");
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    /* command line argument defaults */

    in.nonceTPM.b.size = 0;
    in.cpHashA.b.size = 0;
    in.policyRef.b.size = 0;

    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-hk") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &authObject);
	    }
	    else {
		printf("Missing parameter for -hk\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-ha") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &policySession);
	    }
	    else {
		printf("Missing parameter for -ha\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-in") == 0) {
	    i++;
	    if (i < argc) {
		nonceTPMFilename = argv[i];
	    }
	    else {
		printf("-in option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-cp") == 0) {
	    i++;
	    if (i < argc) {
		cpHashAFilename = argv[i];
	    }
	    else {
		printf("-cp option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pref") == 0) {
	    i++;
	    if (i < argc) {
		policyRefFilename = argv[i];
	    }
	    else {
		printf("-pref option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-exp") == 0) {
	    i++;
	    if (i < argc) {
		expiration = atoi(argv[i]);
	    }
	    else {
		printf("Missing parameter for -exp\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-sk") == 0) {
	    i++;
	    if (i < argc) {
		signingKeyFilename = argv[i];
	    }
	    else {
		printf("-sk option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-tk") == 0) {
	    i++;
	    if (i < argc) {
		ticketFilename = argv[i];
	    }
	    else {
		printf("-tk option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-to") == 0) {
	    i++;
	    if (i < argc) {
		timeoutFilename = argv[i];
	    }
	    else {
		printf("-to option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pwdk") == 0) {
	    i++;
	    if (i < argc) {
		signingKeyPassword = argv[i];
	    }
	    else {
		printf("-pwdk option needs a value\n");
		printUsage();
	    }
	}
 	else if (strcmp(argv[i],"-halg") == 0) {
	    i++;
	    if (i < argc) {
		if (strcmp(argv[i],"sha256") == 0) {
		    halg = TPM_ALG_SHA256;
		}
		else if (strcmp(argv[i],"sha1") == 0) {
		    halg = TPM_ALG_SHA1;
		}
		else {
		    printf("Bad parameter for -halg\n");
		    printUsage();
		}
	    }
	    else {
		printf("-halg option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-h") == 0) {
	    printUsage();
	}
	else if (strcmp(argv[i],"-v") == 0) {
	    verbose = TRUE;
	    TSE_SetProperty(TPM_TRACE_LEVEL, "2");
	}
	else {
	    printf("\n%s is not a valid option\n", argv[i]);
	    printUsage();
	}
    }
    if (authObject == 0) {
	printf("Missing handle parameter -hk\n");
	printUsage();
    }
    if (policySession == 0) {
	printf("Missing handle parameter -ha\n");
	printUsage();
    }
    if (signingKeyFilename == 0) {
	printf("Missing handle parameter -sk\n");
	printUsage();
    }
    if (rc == 0) {
	in.authObject = authObject;
	in.policySession = policySession;
    }
    if ((rc == 0) && (nonceTPMFilename != NULL)) {
	rc = File_Read2B(&in.nonceTPM.b,
			 sizeof(TPMU_HA),
			 nonceTPMFilename);
    }
    if ((rc == 0) && (cpHashAFilename != NULL)) {
	rc = File_Read2B(&in.cpHashA.b,
			 sizeof(TPMU_HA),
			 cpHashAFilename);
    }
    if ((rc == 0) && (policyRefFilename != NULL)) {
	rc = File_Read2B(&in.policyRef.b,
			 sizeof(TPMU_HA),
			 policyRefFilename);
    }
    if (rc == 0) {
	in.expiration = expiration;
	in.auth.sigAlg = TPM_ALG_RSASSA;
	in.auth.signature.rsassa.hash = halg;
    }
    /* register call back for the signature program */
    if (rc == 0) {
	extra.signatureCallback = signatureCallback;
    }
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute((RESPONSE_PARAMETERS *)&out, 
			 (COMMAND_PARAMETERS *)&in,
			 (EXTRA_PARAMETERS *)&extra,
			 TPM_CC_PolicySigned,
			 TPM_RH_NULL, NULL, 0);
    }
    if ((rc == 0) && (ticketFilename != NULL)) {
	rc = File_WriteStructure(&out.policyTicket,
				 (MarshalFunction_t)TSS_TPMT_TK_AUTH_Marshal,
				 ticketFilename);
    }
    if ((rc == 0) && (timeoutFilename != NULL)) {
	rc = File_WriteBinaryFile(out.timeout.b.buffer,
				  out.timeout.b.size,
				  timeoutFilename); 
    }
    if (rc == 0) {
	if (verbose) printf("policysigned: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("policysigned: failed, rc %08x\n", rc);
	TSEResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

/* signatureCallback() signs digest, returns signature

   This sample uses a pem file signingKeyFilename with signingKeyPassword
*/

static TPM_RC signatureCallback(TPMT_HA *digest,
				uint8_t *signature,
				uint16_t *signatureLength)
{
    TPM_RC		rc = 0;
    int			irc;
    RSA			*rsaKey = NULL;
    FILE		*keyFile = NULL;
    int			nid;			/* openssl hash algorithm */
    unsigned int 	length;			/* openssl signature length */
    UINT32  		sizeInBytes;		/* hash algorithm mapped to size */
    
    if (rc == 0) {
	sizeInBytes = _cpri__GetDigestSize(digest->hashAlg);
#if 0
	if (verbose) {
	    TPM_PrintAll("signatureCallback: digest",
			 (uint8_t *)(&digest->digest), sizeInBytes);
	}
#endif
    }
    /* map the  hash algorithm to the openssl NID */
    if (rc == 0) {
	switch (digest->hashAlg) {
	  case TPM_ALG_SHA256:
	    nid = NID_sha256;
	    break;
	  case TPM_ALG_SHA1:
	    nid = NID_sha1;
	    break;
	  default:
	    rc = -1;
	}
    }
    if (rc == 0) {
	keyFile = fopen(signingKeyFilename, "r");
	if (keyFile == NULL) {
	    rc = -1;
	}
    }
    if (rc == 0) {
	rsaKey = PEM_read_RSAPrivateKey(keyFile, NULL, NULL, (void *)signingKeyPassword);
	if (rsaKey == NULL) {
	    ERR_print_errors_fp(stdout);
	    rc = -1;
	}
    }
    if (rc == 0) {
	irc = RSA_sign(nid,
		       (uint8_t *)(&digest->digest), sizeInBytes,
		       signature, &length,
		       rsaKey);
	if (irc != 1) {
	    rc = -1;
	}
	/* FIXME range check */
	else {
	    *signatureLength = length;
	}
    }
    if (rc == 0) {
#if 0
	if (verbose) TPM_PrintAll("signatureCallback: signature",
				  signature, *signatureLength);
#endif
    }
    if (keyFile != NULL) {
	fclose(keyFile);
    }
    if (rsaKey != NULL) {
	RSA_free(rsaKey);
    }
    return rc;
}


static void printUsage(void)
{
    printf("\n");
    printf("policysigned\n");
    printf("\n");
    printf("Runs TPM2_PolicySigned\n");
    printf("\n");
    printf("\t-hk authorizing key handle\n");
    printf("\t-ha session handle\n");
    printf("\t-in nonceTPM file (default none)\n");
    printf("\t-cp cpHash file (default none)\n");
    printf("\t-pref policyRef file (default none)\n");
    printf("\t-exp expiration (default none)\n");
    printf("\t-halg [sha1, sha256] (default sha256)\n");
    printf("\t-sk signing key file name (PEM format)\n");
    printf("\t-pwdk signing key password (default null)\n");
    printf("\t[-tk ticket file name]\n");
    printf("\t[-to timeout file name]\n");
    exit(1);	
}
