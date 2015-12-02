/********************************************************************************/
/*										*/
/*			     				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*            $Id: TpmTcpProtocol.h 174 2015-03-22 16:48:16Z kgoldman $		*/
/*										*/
/*  Licenses and Notices							*/
/*										*/
/*  1. Copyright Licenses:							*/
/*										*/
/*  - Trusted Computing Group (TCG) grants to the user of the source code in	*/
/*    this specification (the "Source Code") a worldwide, irrevocable, 		*/
/*    nonexclusive, royalty free, copyright license to reproduce, create 	*/
/*    derivative works, distribute, display and perform the Source Code and	*/
/*    derivative works thereof, and to grant others the rights granted herein.	*/
/*										*/
/*  - The TCG grants to the user of the other parts of the specification 	*/
/*    (other than the Source Code) the rights to reproduce, distribute, 	*/
/*    display, and perform the specification solely for the purpose of 		*/
/*    developing products based on such documents.				*/
/*										*/
/*  2. Source Code Distribution Conditions:					*/
/*										*/
/*  - Redistributions of Source Code must retain the above copyright licenses, 	*/
/*    this list of conditions and the following disclaimers.			*/
/*										*/
/*  - Redistributions in binary form must reproduce the above copyright 	*/
/*    licenses, this list of conditions	and the following disclaimers in the 	*/
/*    documentation and/or other materials provided with the distribution.	*/
/*										*/
/*  3. Disclaimers:								*/
/*										*/
/*  - THE COPYRIGHT LICENSES SET FORTH ABOVE DO NOT REPRESENT ANY FORM OF	*/
/*  LICENSE OR WAIVER, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE, WITH	*/
/*  RESPECT TO PATENT RIGHTS HELD BY TCG MEMBERS (OR OTHER THIRD PARTIES)	*/
/*  THAT MAY BE NECESSARY TO IMPLEMENT THIS SPECIFICATION OR OTHERWISE.		*/
/*  Contact TCG Administration (admin@trustedcomputinggroup.org) for 		*/
/*  information on specification licensing rights available through TCG 	*/
/*  membership agreements.							*/
/*										*/
/*  - THIS SPECIFICATION IS PROVIDED "AS IS" WITH NO EXPRESS OR IMPLIED 	*/
/*    WARRANTIES WHATSOEVER, INCLUDING ANY WARRANTY OF MERCHANTABILITY OR 	*/
/*    FITNESS FOR A PARTICULAR PURPOSE, ACCURACY, COMPLETENESS, OR 		*/
/*    NONINFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS, OR ANY WARRANTY 		*/
/*    OTHERWISE ARISING OUT OF ANY PROPOSAL, SPECIFICATION OR SAMPLE.		*/
/*										*/
/*  - Without limitation, TCG and its members and licensors disclaim all 	*/
/*    liability, including liability for infringement of any proprietary 	*/
/*    rights, relating to use of information in this specification and to the	*/
/*    implementation of this specification, and TCG disclaims all liability for	*/
/*    cost of procurement of substitute goods or services, lost profits, loss 	*/
/*    of use, loss of data or any incidental, consequential, direct, indirect, 	*/
/*    or special damages, whether under contract, tort, warranty or otherwise, 	*/
/*    arising in any way out of use or reliance upon this specification or any 	*/
/*    information herein.							*/
/*										*/
/*  (c) Copyright IBM Corp. and others, 2012-2015				*/
/*										*/
/********************************************************************************/

/* rev 119 */

#include "BaseTypes.h"		/* kgold added for BOOL */

// D.2	TpmTcpProtocol.h
// D.2.1.	Introduction

// TPM commands are communicated as BYTE streams on a TCP connection.  The TPM command protocol is
// enveloped with the interface protocol described in this file. The command is indicated by a
// UINT32 with one of the values below.  Most commands take no parameters return no TPM errors.  In
// these cases the TPM interface protocol acknowledges that command processing is complete by
// returning a UINT32=0. The command TPM_SIGNAL_HASH_DATA takes a UINT32-prepended variable length
// BYTE array and the interface protocol acknowledges command completion with a UINT32=0. Most TPM
// commands are enveloped using the TPM_SEND_COMMAND interface command. The parameters are as
// indicated below.  The interface layer also appends a UIN32=0 to the TPM response for regularity.

// D.2.2.	Typedefs and Defines

#ifndef     TCP_TPM_PROTOCOL_H
#define     TCP_TPM_PROTOCOL_H

// TPM Commands. All commands acknowledge processing by returning a UINT32 == 0 except where noted

#define TPM_SIGNAL_POWER_ON         1
#define TPM_SIGNAL_POWER_OFF        2
#define TPM_SIGNAL_PHYS_PRES_ON     3
#define TPM_SIGNAL_PHYS_PRES_OFF    4
#define TPM_SIGNAL_HASH_START       5
#define TPM_SIGNAL_HASH_DATA        6

// {UINT32 BufferSize, BYTE[BufferSize] Buffer}

#define TPM_SIGNAL_HASH_END         7
#define TPM_SEND_COMMAND            8

// {BYTE Locality, UINT32 InBufferSize, BYTE[InBufferSize] InBuffer} ->
//     {UINT32 OutBufferSize, BYTE[OutBufferSize] OutBuffer}

#define TPM_SIGNAL_CANCEL_ON        9
#define TPM_SIGNAL_CANCEL_OFF       10
#define TPM_SIGNAL_NV_ON            11
#define TPM_SIGNAL_NV_OFF           12
#define TPM_SIGNAL_KEY_CACHE_ON     13
#define TPM_SIGNAL_KEY_CACHE_OFF    14
#define TPM_REMOTE_HANDSHAKE        15
#define TPM_SET_ALTERNATIVE_RESULT  16
#define TPM_SIGNAL_RESET            17
#define TPM_SESSION_END             20
#define TPM_STOP                    21
#define TPM_GET_COMMAND_RESPONSE_SIZES  25
#define TPM_TEST_FAILURE_MODE      30

enum TpmEndPointInfo
    {
	tpmPlatformAvailable = 0x01,
	tpmUsesTbs = 0x02,
	tpmInRawMode = 0x04,
	tpmSupportsPP = 0x08
    };

// Existing RPC interface type definitions retained so that the implementation
// can be re-used

typedef struct
{
    unsigned long BufferSize;
    unsigned char *Buffer;
} _IN_BUFFER;

typedef unsigned char *_OUTPUT_BUFFER;

typedef struct
{
    uint32_t         BufferSize;
    _OUTPUT_BUFFER   Buffer;
} _OUT_BUFFER;

//** TPM Command Function Prototypes

void _rpc__Signal_PowerOn(BOOL isReset);
void _rpc__Signal_PowerOff(void);
void _rpc__ForceFailureMode(void);
void _rpc__Signal_PhysicalPresenceOn(void);
void _rpc__Signal_PhysicalPresenceOff(void);
void _rpc__Signal_Hash_Start(void);
void _rpc__Signal_Hash_Data(
			    _IN_BUFFER input
			    );
void _rpc__Signal_HashEnd(void);
void _rpc__Send_Command(
			unsigned char   locality,
			_IN_BUFFER      request,
			_OUT_BUFFER     *response
			);
void _rpc__Signal_CancelOn(void);
void _rpc__Signal_CancelOff(void);
void _rpc__Signal_NvOn(void);
void _rpc__Signal_NvOff(void);
void _rpc__Shutdown(void);

BOOL _rpc__InjectEPS(
		     const char* seed,
		     int seedSize
		     );

// start the TPM server on the indicated socket. The TPM is single-threaded and will accept
// connections first-come-first-served. Once a connection is dropped another client can connect.

#ifdef TPM_POSIX	/* kgold */
typedef int SOCKET;
#endif

BOOL TpmServer(SOCKET ServerSocket);

#endif
