/********************************************************************************/
/*										*/
/*			    TSS Configuration Properties			*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tseproperties.c 430 2015-11-19 16:24:33Z kgoldman $		*/
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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>

#include "tse.h"
#include "tseproperties.h"
#include "tsecrypto.h"

/* local prototypes */

static TPM_RC TSE_SetTraceLevel(const char *value);
static TPM_RC TSE_SetDataDir(const char *value);
static TPM_RC TSE_SetCommandPort(const char *value);
static TPM_RC TSE_SetPlatformPort(const char *value);
static TPM_RC TSE_SetServerName(const char *value);
static TPM_RC TSE_SetServerType(const char *value);
static TPM_RC TSE_SetInterfaceType(const char *value);
static TPM_RC TSE_SetDevice(const char *value);
static TPM_RC TSE_SetEncryptSessions(const char *value);

/* tracing */
BOOL tseVerbose = TRUE;		/* initial value so TSE_Init errors emit message */
BOOL tseVverbose = FALSE;

/* directory for persistant storage */
const char *tseDataDirectory;

/* encrypt saved session state */
BOOL tseEncryptSessions = FALSE;

/* tseFirstCall TRUE for the first time through, indicates TSE property (and any other)
   initialization is required

   tseFirstTransmit TRUE for the first time through, indicates that interface open must occur.

*/

TSE_CONTEXT tseContext = { .tseFirstCall = TRUE,
			   .tseFirstTransmit = TRUE
};

/* defaults for global settings */

#ifndef TPM_TRACE_LEVEL_DEFAULT 	
#define TPM_TRACE_LEVEL_DEFAULT 	"0"
#endif

#ifndef TPM_COMMAND_PORT_DEFAULT
#define TPM_COMMAND_PORT_DEFAULT 	"2321"		/* default for MS simulator */
#endif

#ifndef TPM_PLATFORM_PORT_DEFAULT
#define TPM_PLATFORM_PORT_DEFAULT 	"2322"		/* default for MS simulator */
#endif

#ifndef TPM_SERVER_NAME_DEFAULT
#define TPM_SERVER_NAME_DEFAULT		"localhost"	/* default to local machine */
#endif

#ifndef TPM_SERVER_TYPE_DEFAULT
#define TPM_SERVER_TYPE_DEFAULT		"mssim"		/* default to MS simulator format */
#endif

#ifndef TPM_DATA_DIR_DEFAULT
#define TPM_DATA_DIR_DEFAULT		"."		/* default to current working directory */
#endif

#ifndef TPM_INTERFACE_TYPE_DEFAULT
#define TPM_INTERFACE_TYPE_DEFAULT	"socsim"	/* default to MS simulator interface */
#endif

#ifndef TPM_DEVICE_DEFAULT
#ifdef TPM_POSIX
#define TPM_DEVICE_DEFAULT		"/dev/tpm0"	/* default to Linux device driver */
#endif
#ifdef TPM_WINDOWS
#define TPM_DEVICE_DEFAULT		"tddl.dll"	/* default to Windows TPM interface dll */
#endif
#endif

#ifndef TPM_ENCRYPT_SESSIONS_DEFAULT
#define TPM_ENCRYPT_SESSIONS_DEFAULT	"1"
#endif

/* TSE_Init() does any required TSE initialization.

   It sets all globally properties based on defaults that can be overriden by environment variables.
*/

TPM_RC TSE_Init()
{
    TPM_RC		rc = 0;
    const char 		*value;

    /* crypto module initializations */
    if (rc == 0) {
	rc = TSE_Crypto_Init();
    }
    /* trace level */
    if (rc == 0) {
	value = getenv("TPM_TRACE_LEVEL");
	rc = TSE_SetTraceLevel(value);
    }
    /* data directory */
    if (rc == 0) {
	value = getenv("TPM_DATA_DIR");
	rc = TSE_SetDataDir(value);
    }
    /* TPM socket command port */
    if (rc == 0) {
	value = getenv("TPM_COMMAND_PORT");
	rc = TSE_SetCommandPort(value);
    }
    /* TPM simulator socket platform port */
    if (rc == 0) {
	value = getenv("TPM_PLATFORM_PORT");
	rc = TSE_SetPlatformPort(value);
    }
    /* TPM socket host name */
    if (rc == 0) {
	value = getenv("TPM_SERVER_NAME");
	rc = TSE_SetServerName(value);
    }
    /* TPM socket server type */
    if (rc == 0) {
	value = getenv("TPM_SERVER_TYPE");
	rc = TSE_SetServerType(value);
    }
    /* TPM interface type */
    if (rc == 0) {
	value = getenv("TPM_INTERFACE_TYPE");
	rc = TSE_SetInterfaceType(value);
    }
    /* TPM device within the interface type */
    if (rc == 0) {
	value = getenv("TPM_DEVICE");
	rc = TSE_SetDevice(value);
    }
    /* flag whether session state should be encrypted */
    if (rc == 0) {
	value = getenv("TPM_ENCRYPT_SESSIONS");
	rc = TSE_SetEncryptSessions(value);
    }
    return rc;    
}

/* TSE_SetProperty() sets the property to the value.

   The format of the property and value the same as that of the environment variable.

   A NULL value sets the property to the default.
*/

TPM_RC TSE_SetProperty(int property,
		       const char *value)
{
    TPM_RC		rc = 0;

    if (rc == 0) {
	if (tseContext.tseFirstCall) {
	    rc = TSE_Init();
	    tseContext.tseFirstCall = FALSE;
	}
    }
    if (rc == 0) {
	switch (property) {
	  case TPM_TRACE_LEVEL:
	    rc = TSE_SetTraceLevel(value);
	    break;
	  case TPM_DATA_DIR:
	    rc = TSE_SetDataDir(value);
	    break;
	  case TPM_COMMAND_PORT:	
	    rc = TSE_SetCommandPort(value);
	    break;
	  case TPM_PLATFORM_PORT:	
	    rc = TSE_SetPlatformPort(value);
	    break;
	  case TPM_SERVER_NAME:		
	    rc = TSE_SetServerName(value);
	    break;
	  case TPM_SERVER_TYPE:		
	    rc = TSE_SetServerType(value);
	    break;
	  case TPM_INTERFACE_TYPE:
	    rc = TSE_SetInterfaceType(value);
	    break;
	  case TPM_DEVICE:
	    rc = TSE_SetDevice(value);
	    break;
	  case TPM_ENCRYPT_SESSIONS:
	    rc = TSE_SetEncryptSessions(value);
	    break;
	  default:
	    rc = TSE_RC_BAD_PROPERTY;
	}
    }
    return rc;
}

/* TSE_SetTraceLevel() sets the trace level.

   0:	no printing
   1:	error primting
   2:	trace printing
*/

static TPM_RC TSE_SetTraceLevel(const char *value)
{
    TPM_RC		rc = 0;
    int			irc;
    int 		level;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_TRACE_LEVEL_DEFAULT;
	}
    }
    if (rc == 0) {
	irc = sscanf(value, "%u", &level);
	if (irc != 1) {
	    if (tseVerbose) printf("TSE_SetTraceLevel: Error, value invalid\n");
	    rc = TSE_RC_BAD_PROPERTY_VALUE;
	}
    }
    if (rc == 0) {
	switch (level) {
	  case 0:
	    tseVerbose = FALSE;
	    tseVverbose = FALSE;
	    break;
	  case 1:
	    tseVerbose = TRUE;
	    tseVverbose = FALSE;
	    break;
	  default:
	    tseVerbose = TRUE;
	    tseVverbose = TRUE;
	    break;
	}
    }
    return rc;
}

static TPM_RC TSE_SetDataDir(const char *value)
{
    TPM_RC		rc = 0;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_DATA_DIR_DEFAULT;
	}
    }
    if (rc == 0) {
	tseDataDirectory = value;
	/* FIXME check length, don't hard code max length, use max path size */
    }
    return rc;
}

static TPM_RC TSE_SetCommandPort(const char *value)
{
    int			irc;
    TPM_RC		rc = 0;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_COMMAND_PORT_DEFAULT;
	}
    }
    if (rc == 0) {
	irc = sscanf(value, "%hu", &tseContext.tseCommandPort);
	if (irc != 1) {
	    if (tseVerbose) printf("TSE_SetCommandPort: Error, value invalid\n");
	    rc = TSE_RC_BAD_PROPERTY_VALUE;
	}
    }
    return rc;
}

static TPM_RC TSE_SetPlatformPort(const char *value)
{
    TPM_RC		rc = 0;
    int			irc;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_PLATFORM_PORT_DEFAULT;
	}
    }
    if (rc == 0) {
	irc = sscanf(value, "%hu", &tseContext.tsePlatformPort);
	if (irc != 1) {
	    if (tseVerbose) printf("TSE_SetPlatformPort: Error, , value invalid\n");
	    rc = TSE_RC_BAD_PROPERTY_VALUE;
	}
    }
    return rc;
}

static TPM_RC TSE_SetServerName(const char *value)
{
    TPM_RC		rc = 0;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_SERVER_NAME_DEFAULT;
	}
    }
    if (rc == 0) {
	tseContext.tseServerName = value;
    }
    return rc;
}

static TPM_RC TSE_SetServerType(const char *value)
{
    TPM_RC		rc = 0;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_SERVER_TYPE_DEFAULT;
	}
    }
    if (rc == 0) {
	tseContext.tseServerType = value;
    }
    return rc;
}

static TPM_RC TSE_SetInterfaceType(const char *value)
{
    TPM_RC		rc = 0;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_INTERFACE_TYPE_DEFAULT;
	}
    }
    if (rc == 0) {
	tseContext.tseInterfaceType = value;
    }
    return rc;
}

static TPM_RC TSE_SetDevice(const char *value)
{
    TPM_RC		rc = 0;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_DEVICE_DEFAULT;
	}
    }
    if (rc == 0) {
	tseContext.tseDevice = value;
    }
    return rc;
}

static TPM_RC TSE_SetEncryptSessions(const char *value)
{
    TPM_RC		rc = 0;
    int			irc;

    if (rc == 0) {
	if (value == NULL) {
	    value = TPM_ENCRYPT_SESSIONS_DEFAULT;
	}
    }
    if (rc == 0) {
	irc = sscanf(value, "%u", &tseEncryptSessions);
	if (irc != 1) {
	    if (tseVerbose) printf("TSE_SetEncryptSessions: Error, value invalid\n");
	    rc = TSE_RC_BAD_PROPERTY_VALUE;
	}
    }
    return rc;
}
