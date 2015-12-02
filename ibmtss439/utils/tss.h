/********************************************************************************/
/*										*/
/*			     TSS SAPI Equivalent				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*            $Id: tss.h 299 2015-05-19 22:24:02Z kgoldman $	*/
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

#ifndef TSS_H
#define TSS_H

#include "../src/TPM_Types.h"
#include "../src/Global.h"
#include "../src/Commands_fp.h"

struct TSS_CONTEXT;

LIB_EXPORT 
TPM_RC TSS_GetContext(struct TSS_CONTEXT **tssContext);

LIB_EXPORT
TPM_RC TSS_FreeContext(struct TSS_CONTEXT *tssContext);

LIB_EXPORT
TPM_RC TSS_Marshal(struct TSS_CONTEXT *tssContext,
		   COMMAND_PARAMETERS *in,
		   TPM_CC commandCode);
LIB_EXPORT
TPM_RC TSS_Unmarshal(struct TSS_CONTEXT *tssContext,
		     RESPONSE_PARAMETERS *out);

LIB_EXPORT
TPM_RC TSS_SetCmdAuths(struct TSS_CONTEXT *tssContext, ...);

LIB_EXPORT
TPM_RC TSS_GetRspAuths(struct TSS_CONTEXT *tssContext, ...);

LIB_EXPORT
TPM_CC TSS_GetCommandCode(struct TSS_CONTEXT *tssContext);

LIB_EXPORT
TPM_RC TSS_GetCpBuffer(struct TSS_CONTEXT *tssContext,
		       uint32_t *cpBufferSize,
		       uint8_t **cpBuffer);
LIB_EXPORT
TPM_RC TSS_GetCommandDecryptParam(struct TSS_CONTEXT *tssContext,
				  uint32_t *decryptParamSize,
				  uint8_t **decryptParamBuffer);
LIB_EXPORT
TPM_RC TSS_SetCommandDecryptParam(struct TSS_CONTEXT *tssContext,
				  uint32_t encryptParamSize,
				  uint8_t *encryptParamBuffer);
LIB_EXPORT
TPM_RC TSS_GetCommandHandleCount(struct TSS_CONTEXT *tssContext,
				 uint32_t *commandHandleCount);
LIB_EXPORT
AUTH_ROLE TSS_GetAuthRole(struct TSS_CONTEXT *tssContext,
			  UINT32 handleIndex);
LIB_EXPORT
TPM_RC TSS_GetCommandHandle(struct TSS_CONTEXT *tssContext,
			    TPM_HANDLE *commandHandle,
			    uint32_t index);
LIB_EXPORT
TPM_RC TSS_GetRpBuffer(struct TSS_CONTEXT *tssContext,
		       uint32_t *rpBufferSize,
		       uint8_t **rpBuffer);
LIB_EXPORT
TPM_RC TSS_GetResponseEncryptParam(struct TSS_CONTEXT *tssContext,
				   uint32_t *encryptParamSize,
				   uint8_t **encryptParamBuffer);
LIB_EXPORT
TPM_RC TSS_SetResponseDecryptParam(struct TSS_CONTEXT *tssContext,
				   uint32_t decryptParamSize,
				   uint8_t *decryptParamBuffer);

LIB_EXPORT
TPM_RC TSS_Execute(struct TSS_CONTEXT *tssContext);

#endif
