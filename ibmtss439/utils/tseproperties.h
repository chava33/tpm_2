/********************************************************************************/
/*										*/
/*			    TSS Configuration Properties			*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tseproperties.h 434 2015-11-19 23:18:23Z kgoldman $		*/
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

#ifndef TSEPROPERTIES_H
#define TSEPROPERTIES_H

#include "../src/TPM_Types.h"

#ifdef TPM_POSIX
typedef int TST_SOCKET_FD;
#endif	/* TPM_POSIX */

#ifdef TPM_WINDOWS
#include <windows.h>
#include <specstrings.h>

#ifdef TPM_WINDOWS_TBSI
/* Windows 7 */
#if defined TPM_WINDOWS_TBSI_WIN7
#include <c:/progra~1/Micros~2/Windows/v7.1/include/tbs.h>
/* Windows 8, 10 */
#elif defined  TPM_WINDOWS_TBSI_WIN8
#include <tbs.h>
#else
#error "Must define either TPM_WINDOWS_TBSI_WIN7 or TPM_WINDOWS_TBSI_WIN8"
#endif
#endif

typedef SOCKET TST_SOCKET_FD; 
#endif /* TPM_WINDOWS */

/* Context for TSE global parameters */

typedef struct {
    /* TRUE if the TSE has not been called yet */
    int tseFirstCall;

    /* ports, host name, server (packet) type for socket interface */
    short tseCommandPort;
    short tsePlatformPort;
    const char *tseServerName;
    const char *tseServerType;

    /* interface type */
    const char *tseInterfaceType;

    /* device driver interface */
    const char *tseDevice;

    /* transmit */
    int tseFirstTransmit;

    /* socket file descriptor */
    TST_SOCKET_FD sock_fd;

    /* Linux device file descriptor */
    int dev_fd;

    /* Windows device driver handle */
#ifdef TPM_WINDOWS
#ifdef TPM_WINDOWS_TBSI
    TBS_HCONTEXT hContext;
#endif
#endif

} TSE_CONTEXT;

TPM_RC TSE_Init(void);


#endif
