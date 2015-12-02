/********************************************************************************/
/*										*/
/*			   PCR_Extend 						*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: pcrextend.c 412 2015-11-12 19:32:55Z kgoldman $		*/
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

static void printUsage(void);

int verbose = FALSE;

int main(int argc, char *argv[])
{
    TPM_RC			rc = 0;
    int				i;    /* argc iterator */
    PCR_Extend_In 		in;
    TPMI_DH_PCR 		pcrHandle = IMPLEMENTATION_PCR;
    TPMI_ALG_HASH		halg = TPM_ALG_SHA256;
    unsigned int		hashSize = SHA256_DIGEST_SIZE;
    const char 			*data = NULL;
    const char 			*datafilename = NULL;
   
    TSE_SetProperty(TPM_TRACE_LEVEL, "1");

    /* command line argument defaults */
    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-ha") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%u", &pcrHandle);
	    }
	    else {
		printf("Missing parameter for -ha\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-halg") == 0) {
	    i++;
	    if (i < argc) {
		if (strcmp(argv[i],"sha1") == 0) {
		    halg = TPM_ALG_SHA1;
		    hashSize = SHA1_DIGEST_SIZE;
		}
		else if (strcmp(argv[i],"sha256") == 0) {
		    halg = TPM_ALG_SHA256;
		    hashSize = SHA256_DIGEST_SIZE;
		}
		else if (strcmp(argv[i],"sha384") == 0) {
		    halg = TPM_ALG_SHA384;
		    hashSize = SHA384_DIGEST_SIZE;
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
	else if (strcmp(argv[i],"-ic") == 0) {
	    i++;
	    if (i < argc) {
		data = argv[i];
	    }
	    else {
		printf("-ic option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i], "-if")  == 0) {
	    i++;
	    if (i < argc) {
		datafilename = argv[i];
	    } else {
		printf("-if option needs a value\n");
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
    if (pcrHandle >= IMPLEMENTATION_PCR) {
	printf("Missing or bad PCR handle parameter -ha\n");
	printUsage();
    }
    if ((data == NULL) && (datafilename == NULL)) {
	printf("Data string or data file must be specified\n");
	printUsage();
    }
    if ((data != NULL) && (datafilename != NULL)) {
	printf("Data string and data file cannot both be specified\n");
	printUsage();
    }
    if ((data != NULL) && (strlen(data) > hashSize)) {
	printf("Data length greater than hash size %u\n", hashSize);
	printUsage();
    }
    if (rc == 0) {
	in.pcrHandle = pcrHandle;
	/* Table 100 - Definition of TPML_DIGEST_VALUES Structure */
	in.digests.count = 1;
	/* Table 100 - Definition of TPML_DIGEST_VALUES Structure digests */
	/* Table 71 - Definition of TPMT_HA Structure <IN/OUT> */
	/* Table 59 - Definition of (TPM_ALG_ID) TPMI_ALG_HASH Type hashAlg */
	in.digests.digests[0].hashAlg = halg;
	/* Table 70 - Definition of TPMU_HA Union <IN/OUT, S> */
	/* append zero padding to hash algorithm length */
	memset((uint8_t *)&in.digests.digests[0].digest, 0, hashSize);
    }
    if (rc == 0) {
	if (data != NULL) {
	    if (verbose) printf("Extending %u bytes from stream\n", (unsigned int)strlen(data));
	    memcpy((uint8_t *)&in.digests.digests[0].digest, data, strlen(data));
	}
    }
    if (datafilename != NULL) {
	unsigned char 	*fileData = NULL;
	size_t 		length;
	if (rc == 0) {
	    rc = File_ReadBinaryFile(&fileData, &length, datafilename);
	}
	if (rc == 0) {
	    if (length > hashSize) {
		printf("Data length greater than hash size %u\n", hashSize);
		rc = EXIT_FAILURE;
	    } 
	}
	if (rc == 0) {
	    if (verbose) printf("Extending %u bytes from file\n", (unsigned int)length);
	    memcpy((uint8_t *)&in.digests.digests[0].digest, fileData, length);
	}
	free(fileData);
    }
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute(NULL, 
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_PCR_Extend,
			 TPM_RS_PW, NULL, 0,
			 TPM_RH_NULL, NULL, 0);
    }
    if (rc == 0) {
	if (verbose) printf("pcrextend: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("pcrextend: failed, rc %08x\n", rc);
	TSEResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

static void printUsage(void)
{
    printf("\n");
    printf("pcrextend\n");
    printf("\n");
    printf("Runs TPM2_PCR_Extend\n");
    printf("\n");
    printf("\t-ha pcr handle\n");
    printf("\t[-halg [sha1, sha256, sha384] (default sha256)]\n");
    printf("\t-ic data string, 0 pad appended to halg length\n");
    printf("\t-if data file, 0 pad appended to halg length\n");
    exit(1);	
}
