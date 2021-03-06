/********************************************************************************/
/*										*/
/*			    Startup		 				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: startup.c 290 2015-05-12 17:55:09Z kgoldman $		*/
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

#include "tse.h"
#include "tseresponsecode.h"

static void printUsage(void);
TPM_RC selftestCommand(void);
TPM_RC startupCommand(TPM_SU startupType);

int verbose = FALSE;

int main(int argc, char *argv[])
{
    TPM_RC 		rc = 0;
    int			i;				/* argc iterator */
    int                 doStartup = TRUE;		/* default no startup */
    int                 doSelftest = FALSE;		/* default no self test */
    TPM_SU		startupType = TPM_SU_CLEAR;
   
    TSE_SetProperty(TPM_TRACE_LEVEL, "1");

    /* command line argument defaults */
    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-c") == 0) {
	    startupType = TPM_SU_CLEAR;
	    doStartup = TRUE;
	}
	else if (strcmp(argv[i],"-h") == 0) {
	    printUsage();
	}
	else if (strcmp(argv[i],"-v") == 0) {
	    verbose = TRUE;
	    TSE_SetProperty(TPM_TRACE_LEVEL, "2");
	}
	else if (strcmp(argv[i],"-s") == 0) {
	    doStartup = TRUE;
	    startupType = TPM_SU_STATE;
	}
	else if (strcmp(argv[i],"-st") == 0) {
	    doSelftest = TRUE;
	}
	else {
	    printf("\n%s is not a valid option\n", argv[i]);
	    printUsage();
	}
    }
    if ((rc == 0) && doStartup) {
	rc = startupCommand(startupType);
    }
    if ((rc == 0) && doSelftest ) {
	rc = selftestCommand();
    }
    if (rc == 0) {
	if (verbose) printf("startup: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("startup: failed, rc %08x\n", rc);
	TSEResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

static void printUsage(void)
{
    printf("\n");
    printf("startup\n");
    printf("\n");
    printf("Runs TPM_Startup\n");
    printf("\n");
    printf("\t-c startup clear (default)\n");
    printf("\t-s startup state\n");
    printf("\t-st run TPM2_SelfTest\n");
    exit(1);	
}

TPM_RC startupCommand(TPM_SU startupType)
{
    TPM_RC 			rc = 0;
    Startup_In 			in;

    in.startupType = startupType;
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute(NULL, 
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_Startup,
			 TPM_RH_NULL, NULL, 0);
    }
    return rc;
}

TPM_RC selftestCommand()
{
    TPM_RC 			rc = 0;
    SelfTest_In 		in;

    in.fullTest = YES;
    /* call TSE to execute the command */
    if (rc == 0) {
	rc = TSE_Execute(NULL, 
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_SelfTest,
			 TPM_RH_NULL, NULL, 0);
    }
    return rc;
}
