/********************************************************************************/
/*										*/
/*			   Load External					*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: loadexternal.c 317 2015-05-28 18:25:09Z kgoldman $		*/
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
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "tse.h"
#include "tseutils.h"
#include "tseresponsecode.h"
#include "Unmarshal_fp.h"

static void printUsage(void);
TPM_RC loadExternalTPM(LoadExternal_In 	*in,
		       const char	*publicKeyFilename,
		       const char	*privateKeyFilename);
TPM_RC loadExternalPEM(LoadExternal_In 	*in,
		       TPMI_ALG_HASH 	nalg,
		       TPMI_ALG_HASH	halg,
		       const char	*pemKeyFilename);

int verbose = FALSE;

int main(int argc, char *argv[])
{
    TPM_RC			rc = 0;
    int				i;    /* argc iterator */
    LoadExternal_In 		in;
    LoadExternal_Out 		out;
    char 			hierarchyChar = 0;
    TPMI_RH_HIERARCHY		hierarchy = TPM_RH_NULL;
    TPMI_ALG_HASH		halg = TPM_ALG_SHA256;
    TPMI_ALG_HASH		nalg = TPM_ALG_SHA256;
    const char			*publicKeyFilename = NULL;
    const char			*privateKeyFilename = NULL;
    const char			*pemKeyFilename = NULL;
    TPMI_SH_AUTH_SESSION    	sessionHandle0 = TPM_RH_NULL;
    unsigned int		sessionAttributes0 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle1 = TPM_RH_NULL;
    unsigned int		sessionAttributes1 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle2 = TPM_RH_NULL;
    unsigned int		sessionAttributes2 = 0;

    TSE_SetProperty(TPM_TRACE_LEVEL, "1");

    /* command line argument defaults */
    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-hi") == 0) {
	    i++;
	    if (i < argc) {
		if (argv[i][0] != 'e' && argv[i][0] != 'o' &&
		    argv[i][0] != 'p' && argv[i][0] != 'h') {
		    printUsage();
		}
		hierarchyChar = argv[i][0];
	    }
	    else {
		printf("Missing parameter for -hi\n");
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
	else if (strcmp(argv[i],"-nalg") == 0) {
	    i++;
	    if (i < argc) {
		if (strcmp(argv[i],"sha1") == 0) {
		    nalg = TPM_ALG_SHA1;
		}
		else if (strcmp(argv[i],"sha256") == 0) {
		    nalg = TPM_ALG_SHA256;
		}
		else if (strcmp(argv[i],"sha384") == 0) {
		    nalg = TPM_ALG_SHA384;
		}
		else {
		    printf("Bad parameter for -nalg\n");
		    printUsage();
		}
	    }
	    else {
		printf("-nalg option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-ipu") == 0) {
	    i++;
	    if (i < argc) {
		publicKeyFilename = argv[i];
	    }
	    else {
		printf("-ipu option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-ipr") == 0) {
	    i++;
	    if (i < argc) {
		privateKeyFilename = argv[i];
	    }
	    else {
		printf("-ipr option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-ipem") == 0) {
	    i++;
	    if (i < argc) {
		pemKeyFilename = argv[i];
	    }
	    else {
		printf("-ipem option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-se0") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionHandle0);
	    }
	    else {
		printf("Missing parameter for -se0\n");
		printUsage();
	    }
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionAttributes0);
		if (sessionAttributes0 > 0xff) {
		    printf("Out of range session attributes for -se0\n");
		    printUsage();
		}
	    }
	    else {
		printf("Missing parameter for -se0\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-se1") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionHandle1);
	    }
	    else {
		printf("Missing parameter for -se1\n");
		printUsage();
	    }
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionAttributes1);
		if (sessionAttributes1 > 0xff) {
		    printf("Out of range session attributes for -se1\n");
		    printUsage();
		}
	    }
	    else {
		printf("Missing parameter for -se1\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-se2") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionHandle2);
	    }
	    else {
		printf("Missing parameter for -se2\n");
		printUsage();
	    }
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionAttributes2);
		if (sessionAttributes2 > 0xff) {
		    printf("Out of range session attributes for -se2\n");
		    printUsage();
		}
	    }
	    else {
		printf("Missing parameter for -se2\n");
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
    if ((pemKeyFilename == NULL) && (publicKeyFilename == NULL)) {
	printf("Missing key parameter -ipu or -ipem\n");
	printUsage();
    }
    if ((pemKeyFilename != NULL) &&
	((publicKeyFilename != NULL) || (privateKeyFilename != NULL))) {
	printf("Two key parameters -ipu,-ipr and -ipem\n");
	printUsage();
    }
    /* Table 50 - TPMI_RH_HIERARCHY primaryHandle */
    if (rc == 0) {
	if (hierarchyChar == 'e') {
	    hierarchy = TPM_RH_ENDORSEMENT;
	}
	else if (hierarchyChar == 'o') {
	    hierarchy = TPM_RH_OWNER;
	}
	else if (hierarchyChar == 'p') {
	    hierarchy = TPM_RH_PLATFORM;
	}
	else if (hierarchyChar == 'n') {
	    hierarchy = TPM_RH_NULL;
	}
    }
    if (rc == 0) {
	/* TPM format key, output from create */
	if (publicKeyFilename != NULL) {
	    rc = loadExternalTPM(&in,
				 publicKeyFilename,
				 privateKeyFilename);
	}
	/* PEM format, output from e.g. openssl */
	else {
	    loadExternalPEM(&in,
			    nalg,
			    halg,
			    pemKeyFilename);
	}
    }
    if (rc == 0) {
	in.hierarchy = hierarchy;
    }
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute((RESPONSE_PARAMETERS *)&out, 
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_LoadExternal,
			 sessionHandle0, NULL, sessionAttributes0,
			 sessionHandle1, NULL, sessionAttributes1,
			 sessionHandle2, NULL, sessionAttributes2,
			 TPM_RH_NULL, NULL, 0);
    }
    if (rc == 0) {
	printf("Handle %08x\n", out.objectHandle);
	if (verbose) printf("loadexternal: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("loadexternal: failed, rc %08x\n", rc);
	TSEResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

TPM_RC loadExternalTPM(LoadExternal_In 	*in,
		       const char	*publicKeyFilename,
		       const char	*privateKeyFilename)
{
    TPM_RC			rc = 0;

    if (privateKeyFilename != NULL) {
	rc = File_ReadStructure(&in->inPrivate,
				(UnmarshalFunction_t)TPM2B_SENSITIVE_Unmarshal,
				privateKeyFilename);
    }
    else {
	in->inPrivate.t.size = 0;
    }
    if (rc == 0) {
	rc = File_ReadStructure(&in->inPublic,
				(UnmarshalFunction_t)TPM2B_PUBLIC_Unmarshal,
				publicKeyFilename);
    }
    return rc;
}

TPM_RC loadExternalPEM(LoadExternal_In 	*in,
		       TPMI_ALG_HASH 	nalg,
		       TPMI_ALG_HASH	halg,
		       const char	*pemKeyFilename)
{
    TPM_RC			rc = 0;
    FILE 			*pemKeyFile = NULL;
    EVP_PKEY 			*pemKeyEvp = NULL;
    RSA 			*rsaPubkey = NULL;

    /* open the pem format file */
    if (rc == 0) {
	rc = File_Open(&pemKeyFile, pemKeyFilename, "rb"); 
    }
    /* convert the file to an EVP public key */
    if (rc == 0) {
	pemKeyEvp = PEM_read_PUBKEY(pemKeyFile, NULL, NULL, NULL);
	if (pemKeyEvp == NULL) {
	    printf("Error PEM_read_PUBKEY reading public key file %s\n", pemKeyFilename);
	    rc = EXIT_FAILURE;
	}
    }
    /* convert to openssl key token */
    if (rc == 0) {
	rsaPubkey = EVP_PKEY_get1_RSA(pemKeyEvp);
	if (rsaPubkey == NULL) {
	    printf("Error: EVP_PKEY_get1_RSA converting public key\n");
	    rc = EXIT_FAILURE;
	}
    }
    if (rc == 0) {
	/* Table 184 - Definition of TPMT_PUBLIC Structure */
	in->inPublic.t.publicArea.type = TPM_ALG_RSA;
	in->inPublic.t.publicArea.nameAlg = nalg;
	in->inPublic.t.publicArea.objectAttributes.val = TPMA_OBJECT_SIGN;
	in->inPublic.t.publicArea.authPolicy.t.size = 0;
	/* Table 182 - Definition of TPMU_PUBLIC_PARMS Union <IN/OUT, S> */
	/* Table 180 - Definition of {RSA} TPMS_RSA_PARMS Structure */
	in->inPublic.t.publicArea.parameters.rsaDetail.symmetric.algorithm = TPM_ALG_NULL;
	/* Table 155 - Definition of {RSA} TPMT_RSA_SCHEME Structure */
	/* FIXME is this the scheme openssl uses on the command line? */
	in->inPublic.t.publicArea.parameters.rsaDetail.scheme.scheme = TPM_ALG_RSASSA;
	/* Table 152 - Definition of TPMU_ASYM_SCHEME Union */
	in->inPublic.t.publicArea.parameters.rsaDetail.scheme.details.rsassa.hashAlg = halg;
	in->inPublic.t.publicArea.parameters.rsaDetail.keyBits = 2048;	
	in->inPublic.t.publicArea.parameters.rsaDetail.exponent = 0;
	/* Table 177 - Definition of TPMU_PUBLIC_ID Union <IN/OUT, S> */
    }
    /* get the public modulus */
    /* Table 158 - Definition of {RSA} TPM2B_PUBLIC_KEY_RSA Structure */
    if (rc == 0) {
	int         bytes;
	bytes = BN_num_bytes(rsaPubkey->n);
	if (bytes > MAX_RSA_KEY_BYTES) {
	    printf("Error, public key modulus %d greater than %u\n", bytes, MAX_RSA_KEY_BYTES);
	    rc = EXIT_FAILURE;
	}
	else {
	    in->inPublic.t.publicArea.unique.rsa.t.size =
		BN_bn2bin(rsaPubkey->n,
			  (uint8_t *)&in->inPublic.t.publicArea.unique.rsa.t.buffer);
	}
    }
    if (rc == 0) {
	in->inPrivate.t.size = 0;
    }
    if (rsaPubkey != NULL) {
	RSA_free(rsaPubkey);	
    }
    if (pemKeyFile != NULL) {
	fclose(pemKeyFile);
    }
    return rc;
}

static void printUsage(void)
{
    printf("\n");
    printf("loadexternal\n");
    printf("\n");
    printf("Runs TPM2_LoadExternal\n");
    printf("\n");
    printf("\t-hi hierarchy (e, o, p, n) (default NULL)\n");
    printf("\t[-nalg name hash algorithm [sha1, sha256, sha384] (default sha256)]\n");
    printf("\t-halg [sha256, sha1] (default sha256)\n");
    printf("\t-ipu public key file name\n");
    printf("\t-ipr private key file name\n");
    printf("\t-ipem PEM format public key file name\n");
    exit(1);	
}
