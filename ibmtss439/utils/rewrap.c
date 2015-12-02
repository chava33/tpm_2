/********************************************************************************/
/*										*/
/*			   Rewrap		 				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: rewrap.c 310 2015-05-26 14:43:50Z kgoldman $			*/
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

static void printUsage(void);

int verbose = FALSE;

int main(int argc, char *argv[])
{
    TPM_RC			rc = 0;
    int				i;    /* argc iterator */
    Rewrap_In 			in;
    Rewrap_Out 			out;
    TPMI_DH_OBJECT		oldParent = 0;
    TPMI_DH_OBJECT		newParent = 0;
    const char			*oldParentPassword = NULL; 
    const char			*inDuplicateFilename = NULL;
    const char			*nameFilename = NULL;			
    const char			*inSymSeedFilename = NULL;
    const char			*outDuplicateFilename = NULL;
    const char			*outSymSeedFilename = NULL;
    TPMI_SH_AUTH_SESSION    	sessionHandle0 = TPM_RS_PW;
    unsigned int		sessionAttributes0 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle1 = TPM_RH_NULL;
    unsigned int		sessionAttributes1 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle2 = TPM_RH_NULL;
    unsigned int		sessionAttributes2 = 0;
    
    TSE_SetProperty(TPM_TRACE_LEVEL, "1");

    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-ho") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &oldParent);
	    }
	    else {
		printf("Missing parameter for -ho\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pwdo") == 0) {
	    i++;
	    if (i < argc) {
		oldParentPassword = argv[i];
	    }
	    else {
		printf("-pwdo option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-hn") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &newParent);
	    }
	    else {
		printf("Missing parameter for -hp\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-id") == 0) {
	    i++;
	    if (i < argc) {
		inDuplicateFilename = argv[i];
	    }
	    else {
		printf("-id option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-in") == 0) {
	    i++;
	    if (i < argc) {
		nameFilename = argv[i];
	    }
	    else {
		printf("-in option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-iss") == 0) {
	    i++;
	    if (i < argc) {
		inSymSeedFilename = argv[i];
	    }
	    else {
		printf("-iss option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-od") == 0) {
	    i++;
	    if (i < argc) {
		outDuplicateFilename = argv[i];
	    }
	    else {
		printf("-od option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-oss") == 0) {
	    i++;
	    if (i < argc) {
		outSymSeedFilename = argv[i];
	    }
	    else {
		printf("-oss option needs a value\n");
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
    if (oldParent == 0) {
	printf("Missing or bad object handle parameter -ho\n");
	printUsage();
    }
    if (newParent == 0) {
	printf("Missing or bad object handle parameter -hn\n");
	printUsage();
    }
    if (inDuplicateFilename == NULL) {
	printf("Missing or bad object handle parameter -id\n");
	printUsage();
    }
    if (nameFilename == NULL) {
	printf("Missing or bad object handle parameter -in\n");
	printUsage();
    }
    if (inSymSeedFilename == NULL) {
	printf("Missing or bad object handle parameter -iss\n");
	printUsage();
    }
    if (rc == 0) {
	in.oldParent = oldParent;
	in.newParent = newParent;
    }
    if (rc == 0) {
	rc = File_Read2B(&in.inDuplicate.b,
			 sizeof(_PRIVATE),
			 inDuplicateFilename);
    }
    if (rc == 0) {
	rc = File_Read2B(&in.name.b,
			 sizeof(TPMU_NAME),
			 nameFilename);
    }
    if (rc == 0) {
	rc = File_Read2B(&in.inSymSeed.b,
			 sizeof(TPMU_ENCRYPTED_SECRET),
			 inSymSeedFilename);
    }
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute((RESPONSE_PARAMETERS *)&out,
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_Rewrap,
			 sessionHandle0, oldParentPassword, sessionAttributes0,
			 sessionHandle1, NULL, sessionAttributes1,
			 sessionHandle2, NULL, sessionAttributes2,
			 TPM_RH_NULL, NULL, 0);
    }
    if ((rc == 0) && (outDuplicateFilename != NULL)) {
	rc = File_WriteBinaryFile(out.outDuplicate.t.buffer,
				  out.outDuplicate.t.size,
				  outDuplicateFilename);
    }
    if ((rc == 0) && (outSymSeedFilename != NULL)) {
	rc = File_WriteBinaryFile(out.outSymSeed.t.secret,
				  out.outSymSeed.t.size,
				  outSymSeedFilename);
    }
    if (rc == 0) {
	if (verbose) printf("rewrap: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("rewrap: failed, rc %08x\n", rc);
	TSEResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

static void printUsage(void)
{
    printf("\n");
    printf("Rewrap\n");
    printf("\n");
    printf("Runs TPM2_Rewrap\n");
    printf("\n");
    printf("\t-ho handle of object old parent\n");
    printf("\t[-pwdo password for old parent] (default empty)\n");
    printf("\t-hn handle of object new parent\n");
    printf("\t-id duplicate private area file name\n");
    printf("\t-in object name file name\n");
    printf("\t-iss input symmetric seed file name");
    printf("\n");
    printf("\t[-od rewrap private area file name]\n");
    printf("\t[-oss symmetric seed file name]\n");
    printf("\t-se[0-2] session handle / attributes (default PWAP)\n");
    printf("\t\t01 continue\n");
    printf("\t\t20 command decrypt\n");
    printf("\t\t40 response encrypt\n");
    exit(1);	
}
