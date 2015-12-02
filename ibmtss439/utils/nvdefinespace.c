/********************************************************************************/
/*										*/
/*			    NV Define Space	 				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: nvdefinespace.c 357 2015-07-02 19:24:56Z kgoldman $		*/
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

#include "../src/TPM_Types.h"

static void printUsage(void);

int verbose = FALSE;

int main(int argc, char *argv[])
{
    TPM_RC			rc = 0;
    int				i;    /* argc iterator */
    NV_DefineSpace_In 		in;
    char 			hierarchyChar = 0;
    char 			hierarchyAuthChar = 0;
    TPMI_ALG_HASH		nalg = TPM_ALG_SHA256;
    unsigned int		hashSize = SHA256_DIGEST_SIZE;
    char 			typeChar = 'o';
    unsigned int		typeCount = 0;
    TPMI_RH_NV_INDEX		nvIndex = 0;
    uint16_t 			dataSize = 0;
    TPMA_NV			nvAttributes;
    const char			*policyFilename = NULL;
    const char			*nvPassword = NULL; 
    const char			*parentPassword = NULL; 
    TPMI_SH_AUTH_SESSION    	sessionHandle0 = TPM_RS_PW;
    unsigned int		sessionAttributes0 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle1 = TPM_RH_NULL;
    unsigned int		sessionAttributes1 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle2 = TPM_RH_NULL;
    unsigned int		sessionAttributes2 = 0;
    
    TSE_SetProperty(TPM_TRACE_LEVEL, "1");

    /* default values */
    nvAttributes.val = TPMA_NVA_NO_DA;

    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-hi") == 0) {
	    i++;
	    if (i < argc) {
		hierarchyChar = argv[i][0];
	    }
	    else {
		printf("Missing parameter for -hi\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-hia") == 0) {
	    i++;
	    if (i < argc) {
		hierarchyAuthChar = argv[i][0];
	    }
	    else {
		printf("Missing parameter for -hia\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-nalg") == 0) {
	    i++;
	    if (i < argc) {
		if (strcmp(argv[i],"sha1") == 0) {
		    nalg = TPM_ALG_SHA1;
		    hashSize = SHA1_DIGEST_SIZE;
		}
		else if (strcmp(argv[i],"sha256") == 0) {
		    nalg = TPM_ALG_SHA256;
		    hashSize = SHA256_DIGEST_SIZE;
		}
		else if (strcmp(argv[i],"sha384") == 0) {
		    nalg = TPM_ALG_SHA384;
		    hashSize = SHA384_DIGEST_SIZE;
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
	else if (strcmp(argv[i],"-ha") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &nvIndex);
	    }
	    else {
		printf("Missing parameter for -ha\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pwdp") == 0) {
	    i++;
	    if (i < argc) {
		parentPassword = argv[i];
	    }
	    else {
		printf("-pwdp option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pwdn") == 0) {
	    i++;
	    if (i < argc) {
		nvPassword = argv[i];
	    }
	    else {
		printf("-pwdn option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pol") == 0) {
	    i++;
	    if (i < argc) {
		policyFilename = argv[i];
	    }
	    else {
		printf("-pol option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-sz") == 0) {
	    i++;
	    if (i < argc) {
		dataSize = atoi(argv[i]);
	    }
	    else {
		printf("-sz option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-ty") == 0) {
	    i++;
	    if (i < argc) {
		typeChar = argv[i][0];
		typeCount++;
	    }
	    else {
		printf("-ty option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i], "-at") == 0) {
	    i++;
	    if (i < argc) {
		if (strcmp(argv[i], "wd")  == 0) {
		    nvAttributes.val |= TPMA_NVA_WRITEDEFINE;
		}
		else if (strcmp(argv[i], "wst")  == 0) {
		    nvAttributes.val |= TPMA_NVA_WRITE_STCLEAR;
		}
		else if (strcmp(argv[i], "gl")  == 0) {
		    nvAttributes.val |= TPMA_NVA_GLOBALLOCK;
		}
		else if (strcmp(argv[i], "rst")  == 0) {
		    nvAttributes.val |= TPMA_NVA_READ_STCLEAR;
		}
		else if (strcmp(argv[i], "pold")  == 0) {
		    nvAttributes.val |= TPMA_NVA_POLICY_DELETE;
		}
		else {
		    printf("Bad parameter for -at\n");
		    printUsage();
		}
	    }
	    else {
		printf("Missing parameter for -at\n");
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
    if ((nvIndex >> 24) != TPM_HT_NV_INDEX) {
	printf("NV index handle not specified or out of range, MSB not 01\n");
	printUsage();
    }
    if (typeCount > 1) {
	printf("-ty can only be specified once\n");
	printUsage();
    }
    /* Authorization attributes */
    if (rc == 0) {
	if (hierarchyAuthChar == 'o') {
	    nvAttributes.val |= TPMA_NVA_OWNERWRITE | TPMA_NVA_OWNERREAD;
	}
	else if (hierarchyAuthChar == 'p') {
	    nvAttributes.val |= TPMA_NVA_PPWRITE | TPMA_NVA_PPREAD;
	}
	else if (hierarchyAuthChar == 0) {
	    nvAttributes.val |= TPMA_NVA_AUTHWRITE | TPMA_NVA_AUTHREAD;
	}
	else {
	    printf("-hia has bad parameter\n");
	    printUsage();
	}
    }
    if (rc == 0) {
	if (hierarchyChar == 'o') {
	    in.authHandle = TPM_RH_OWNER;
	}
	else if (hierarchyChar == 'p') {
	    in.authHandle = TPM_RH_PLATFORM;
	    nvAttributes.val |= TPMA_NVA_PLATFORMCREATE;
	}
	else {
	    printf("Missing or illegal -hi\n");
	    printUsage();
	}
    }
    if (rc == 0) {
	switch (typeChar) {
	  case 'o':
	    nvAttributes.val |= TPMA_NVA_ORDINARY;
	    break;
	  case 'c':
	    nvAttributes.val |= TPMA_NVA_COUNTER;
	    dataSize = 8;
	    break;
	  case 'b':
	    nvAttributes.val |= TPMA_NVA_BITS;
	    dataSize = 8;
	    break;
	  case 'e':
	    nvAttributes.val |= TPMA_NVA_EXTEND;
	    dataSize = hashSize;
	    break;
	  default:
	      printf("Illegal -ty\n");
	      printUsage();
	}
    }	
    /* Table 75 - Definition of Types for TPM2B_AUTH */
    if (rc == 0) {
	if (nvPassword == NULL) {
	    in.auth.b.size = 0;
	}
	else {
	    rc = TPM2B_StringCopy(&in.auth.b,
				  nvPassword, sizeof(TPMU_HA));
	}
    }
    /* optional authorization policy */
    if (rc == 0) {
	if (policyFilename != NULL) {
	    nvAttributes.val |= TPMA_NVA_POLICYWRITE | TPMA_NVA_POLICYREAD;
	    rc = File_Read2B(&in.publicInfo.t.nvPublic.authPolicy.b,
			     sizeof(TPMU_HA),
			     policyFilename);
	}
	else {
	    in.publicInfo.t.nvPublic.authPolicy.t.size = 0;	/* default empty policy */
	}
    }
    /* Table 197 - Definition of TPM2B_NV_PUBLIC Structure publicInfo */
    /* Table 196 - Definition of TPMS_NV_PUBLIC Structure nvPublic */
    if (rc == 0) {
	in.publicInfo.t.nvPublic.nvIndex = nvIndex;		/* the handle of the data area */
	in.publicInfo.t.nvPublic.nameAlg = nalg;		/* hash algorithm used to compute
								   the name of the Index and used
								   for the authPolicy */
	in.publicInfo.t.nvPublic.attributes = nvAttributes;	/* the Index attributes */
	in.publicInfo.t.nvPublic.dataSize = dataSize;		/* the size of the data area */
    }
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute(NULL,
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_NV_DefineSpace,
			 sessionHandle0, parentPassword, sessionAttributes0,
			 sessionHandle1, NULL, sessionAttributes1,
			 sessionHandle2, NULL, sessionAttributes2,
			 TPM_RH_NULL, NULL, 0);
    }
    if (rc == 0) {
	printf("nvdefinespace: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("nvdefinespace: failed, rc %08x\n", rc);
	TSEResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

static void printUsage(void)
{
    printf("\n");
    printf("nvdefinespace\n");
    printf("\n");
    printf("Runs TPM2_NV_DefineSpace\n");
    printf("\n");
    printf("\t-hi hierarchy (o, p)\n");
    printf("\t[-hia hierarchy authorization (o, p)(default index authorization)]\n");
    printf("\t[-pwdp password for hierarchy (default empty)]\n");
    printf("\t-ha NV index handle\n");
    printf("\t\t01xxxxxx\n");
    printf("\t[-pwdn password for NV index (default empty)]\n");
    printf("\t[-nalg [sha1, sha256, sha384] (default sha256)]\n");
    printf("\t[-sz data size (default 0)]\n");
    printf("\t\tIgnored for other than ordinary index\n");
    printf("\t[-ty index type (o, c, b, e) (default ordinary)]\n");
    printf("\t\tordinary, counter, bits, extend\n");
    printf("\t[-at attributes (may be specified more than once)(default none)]\n");
    printf("\t\twd (write define)\n");
    printf("\t\twst (write stclear)\n");
    printf("\t\tgl (global lock)\n");
    printf("\t\trst (read stclear)\n");
    printf("\t\tpold (policy delete)\n");
    printf("\t[-pol policy file (default empty)]\n");
    printf("\n");
    printf("\t-se[0-2] session handle / attributes (default PWAP)\n");
    printf("\t\t01 continue\n");
    exit(1);	
}
