/********************************************************************************/
/*										*/
/*			    GetTime						*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: gettime.c 348 2015-06-22 20:36:45Z kgoldman $		*/
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

#include "tse.h"
#include "tseutils.h"
#include "tseresponsecode.h"
#include "Unmarshal_fp.h"
#include "tssmarshal.h"
#include "tseprint.h"

static void printUsage(void);

int verbose = FALSE;

int main(int argc, char *argv[])
{
    TPM_RC			rc = 0;
    int				i;    /* argc iterator */
    GetTime_In 			in;
    GetTime_Out 		out;
    TPMI_DH_OBJECT		signHandle = 0;
    const char			*keyPassword = NULL; 
    const char			*endorsementPassword = NULL; 
    TPMI_ALG_HASH		halg = TPM_ALG_SHA256;
    const char			*signatureFilename = NULL;
    const char			*attestInfoFilename = NULL;
    const char			*qualifyingDataFilename = NULL;
    TPMS_ATTEST 		tpmsAttest;
    TPMI_SH_AUTH_SESSION    	sessionHandle0 = TPM_RS_PW;
    unsigned int		sessionAttributes0 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle1 = TPM_RS_PW;
    unsigned int		sessionAttributes1 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle2 = TPM_RH_NULL;
    unsigned int		sessionAttributes2 = 0;
 
    TSE_SetProperty(TPM_TRACE_LEVEL, "1");

    /* command line argument defaults */
    
    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-hk") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &signHandle);
	    }
	    else {
		printf("Missing parameter for -hk\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pwdk") == 0) {
	    i++;
	    if (i < argc) {
		keyPassword = argv[i];
	    }
	    else {
		printf("-pwdk option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pwde") == 0) {
	    i++;
	    if (i < argc) {
		endorsementPassword = argv[i];
	    }
	    else {
		printf("-pwde option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-halg") == 0) {
	    i++;
	    if (i < argc) {
		if (strcmp(argv[i],"sha1") == 0) {
		    halg = TPM_ALG_SHA1;
		}
		else if (strcmp(argv[i],"sha256") == 0) {
		    halg = TPM_ALG_SHA256;
		}
		else if (strcmp(argv[i],"sha384") == 0) {
		    halg = TPM_ALG_SHA384;
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
	else if (strcmp(argv[i],"-os") == 0) {
	    i++;
	    if (i < argc) {
		signatureFilename = argv[i];
	    }
	    else {
		printf("-os option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-oa") == 0) {
	    i++;
	    if (i < argc) {
		attestInfoFilename = argv[i];
	    }
	    else {
		printf("-oa option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-qd") == 0) {
	    i++;
	    if (i < argc) {
		qualifyingDataFilename = argv[i];
	    }
	    else {
		printf("-qd option needs a value\n");
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
    if (signHandle == 0) {
	printf("Missing sign handle parameter -hs\n");
	printUsage();
    }
    if (rc == 0) {
	/* handle of the privacy administrator */
	in.privacyAdminHandle = TPM_RH_ENDORSEMENT;
	/* Handle of key that will perform signing */
	in.signHandle = signHandle;
	/* Table 145 - Definition of TPMT_SIG_SCHEME Structure */
	in.inScheme.scheme = TPM_ALG_RSASSA;	
	/* Table 144 - Definition of TPMU_SIG_SCHEME Union <IN/OUT, S> */
	/* Table 142 - Definition of {RSA} Types for RSA Signature Schemes */
	/* Table 135 - Definition of TPMS_SCHEME_HASH Structure */
	in.inScheme.details.rsassa.hashAlg = halg;
    }
    /* data supplied by the caller */
    if (rc == 0) {
	if (qualifyingDataFilename != NULL) {
	    rc = File_Read2B(&in.qualifyingData.b,
			     sizeof(TPMT_HA),
			     qualifyingDataFilename);
	}
	else {
	    in.qualifyingData.t.size = 0;
	}
    }
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute((RESPONSE_PARAMETERS *)&out,
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_GetTime,
			 sessionHandle0, endorsementPassword, sessionAttributes0,
			 sessionHandle1, keyPassword, sessionAttributes1,
			 sessionHandle2, NULL, sessionAttributes2,
			 TPM_RH_NULL, NULL, 0);
    }
    if (rc == 0) {
	BYTE *tmpBuffer = out.timeInfo.t.attestationData;
	INT32 tmpSize = out.timeInfo.t.size;
	rc = TPMS_ATTEST_Unmarshal(&tpmsAttest, &tmpBuffer, &tmpSize);
	if (verbose) TPMS_ATTEST_Print(&tpmsAttest);
    }
    if (rc == 0) {
	BOOL match;
	match = TPM2B_Compare(&in.qualifyingData.b, &tpmsAttest.extraData.b);
	if (!match) {
	    printf("quote: failed, extraData != qualifyingData\n");
	    rc = EXIT_FAILURE;
	}
    }
    if ((rc == 0) && (signatureFilename != NULL)) {
	rc = File_WriteStructure(&out.signature,
				 (MarshalFunction_t)TSS_TPMT_SIGNATURE_Marshal,
				 signatureFilename);
    }    
    if ((rc == 0) && (attestInfoFilename != NULL)) {
	rc = File_WriteBinaryFile(out.timeInfo.t.attestationData,
				  out.timeInfo.t.size,
				  attestInfoFilename);
    }
    if (rc == 0) {
	if (verbose) TPMT_SIGNATURE_Print(&out.signature);
	if (verbose) TPM2B_ATTEST_Print(&out.timeInfo);
	if (verbose) printf("gettime: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("gettime: failed, rc %08x\n", rc);
	TSEResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

static void printUsage(void)
{
    printf("\n");
    printf("gettime\n");
    printf("\n");
    printf("Runs TPM2_GetTime\n");
    printf("\n");
    printf("\t-hk signing key handle\n");
    printf("\t[-pwdk password for signing key (default empty)]\n");
    printf("\t[-pwde password for endorsement hierarchy (default empty)]\n");
    printf("\t[-halg [sha1, sha256, sha384] (default sha256)]\n");
    printf("\t[-qd qualifying data file name]\n");
    printf("\t[-os gettimeature file name (default none)]\n");
    printf("\t[-oa attestation output file name]\n");
    printf("\n");
    printf("\t-se[0-2] session handle / attributes (default PWAP)\n");
    printf("\t-se1 session handle / attributes (default PWAP)\n");
    printf("\t\t01 continue\n");
    exit(1);	
}
