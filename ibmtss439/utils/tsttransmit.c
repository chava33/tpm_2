/********************************************************************************/
/*										*/
/*			    Transmit and Receive Utility			*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tsttransmit.c 434 2015-11-19 23:18:23Z kgoldman $		*/
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

/* This file contains the interface that is not platform or interface specific
 */

#include <string.h>
#include <stdio.h>

#include "tseproperties.h"
#include "tstsocket.h"
#include "tseerror.h"

#ifdef TPM_POSIX
#include "tstdev.h"
#endif

#ifdef TPM_WINDOWS
#ifdef TPM_WINDOWS_TBSI
#include "tsttbsi.h"
#endif
#endif

#include "tsttransmit.h"

extern BOOL tseVverbose;
extern BOOL tseVerbose;
extern TSE_CONTEXT tseContext;

/* local prototypes */

/* TST_TransmitPlatform() transmits an administrative out of band command to the TPM.

   Supported by the simulator, not the TPM device.
*/

TPM_RC TST_TransmitPlatform(uint32_t command, const char *message)
{
    TPM_RC rc = 0;

    if (rc == 0) {
	if (tseContext.tseFirstCall) {
	    rc = TSE_Init();
	    tseContext.tseFirstCall = FALSE;
	}
    }
    if ((strcmp(tseContext.tseInterfaceType, "socsim") == 0)) {
	rc = TST_Socket_TransmitPlatform(command, message);
    }
    else if ((strcmp(tseContext.tseInterfaceType, "dev") == 0)) {
	if (tseVerbose) printf("TST_TransmitPlatform: device %s unsupported\n",
			       tseContext.tseInterfaceType);
	rc = TSE_RC_INSUPPORTED_INTERFACE;	
    }
    else {
	if (tseVerbose) printf("TST_TransmitPlatform: device %s unsupported\n",
			       tseContext.tseInterfaceType);
	rc = TSE_RC_INSUPPORTED_INTERFACE;	
    }
    return rc;
}

/* TST_Transmit() transmits a TPM command packet and receives a response.

*/

TPM_RC TST_Transmit(uint8_t *responseBuffer, uint32_t *read,
		    const uint8_t *commandBuffer, uint32_t written,
		    const char *message)
{
    TPM_RC rc = 0;

    if ((strcmp(tseContext.tseInterfaceType, "socsim") == 0)) {
	rc = TST_Socket_Transmit(responseBuffer, read,
				 commandBuffer, written,
				 message);
    }
    else if ((strcmp(tseContext.tseInterfaceType, "dev") == 0)) {
#ifdef TPM_POSIX	/* transmit through Linux device driver */
	rc = TST_Dev_Transmit(responseBuffer, read,
			      commandBuffer, written,
			      message);
#endif

#ifdef TPM_WINDOWS	/* transmit through Windows TBSI */
#ifdef TPM_WINDOWS_TBSI
	rc = TST_Tbsi_Transmit(responseBuffer, read,
			       commandBuffer, written,
			       message);
#else
	if (tseVerbose) printf("TST_Transmit: device %s unsupported\n",
			       tseContext.tseInterfaceType);
	rc = TSE_RC_INSUPPORTED_INTERFACE;	
#endif
#endif
    }
    else {
	if (tseVerbose) printf("TST_Transmit: device %s unsupported\n",
			       tseContext.tseInterfaceType);
	rc = TSE_RC_INSUPPORTED_INTERFACE;	
    }
    return rc;
}

