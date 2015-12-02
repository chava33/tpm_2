/********************************************************************************/
/*										*/
/*			     Structure Print Utilities				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tseprint.h 348 2015-06-22 20:36:45Z kgoldman $		*/
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

#ifndef TSAPRINT_H
#define TSAPRINT_H

#include <stdint.h>

#include "../src/TPM_Types.h"

LIB_EXPORT 
void TPM_PrintAll(const char *string, const unsigned char* buff, uint32_t length);

LIB_EXPORT
void TPM_ALG_ID_Print(TPM_ALG_ID source);
LIB_EXPORT
void TPMS_PCR_SELECTION_Print(TPMS_PCR_SELECTION *source);
LIB_EXPORT
void TPML_PCR_SELECTION_Print(TPML_PCR_SELECTION *source);
LIB_EXPORT
void TPMI_ALG_HASH_Print(TPMI_ALG_HASH *source);
LIB_EXPORT
void TPMS_CLOCK_INFO_Print(TPMS_CLOCK_INFO *source);
LIB_EXPORT
void TPMS_TIME_INFO_Print(TPMS_TIME_INFO *source);
LIB_EXPORT
void TPMS_TIME_ATTEST_INFO_Print(TPMS_TIME_ATTEST_INFO *source);
LIB_EXPORT
void TPMS_CERTIFY_INFO_Print(TPMS_CERTIFY_INFO *source);
LIB_EXPORT
void TPMS_QUOTE_INFO_Print(TPMS_QUOTE_INFO *source);
LIB_EXPORT
void TPMS_SESSION_AUDIT_INFO_Print(TPMS_SESSION_AUDIT_INFO *source);
LIB_EXPORT
void TPMI_ST_ATTEST_Print(TPMI_ST_ATTEST selector);
LIB_EXPORT
void TPMU_ATTEST_Print(TPMU_ATTEST *source, TPMI_ST_ATTEST selector);
LIB_EXPORT
void TPMS_ATTEST_Print(TPMS_ATTEST *source);
LIB_EXPORT
void TPM2B_ATTEST_Print(TPM2B_ATTEST *source);
LIB_EXPORT
void TPMS_SIGNATURE_RSA_Print(TPMS_SIGNATURE_RSA *source);
LIB_EXPORT
void TPMS_SIGNATURE_RSASSA_Print(TPMS_SIGNATURE_RSASSA *source);
LIB_EXPORT
void TPMU_SIGNATURE_Print(TPMU_SIGNATURE *source, TPMI_ALG_SIG_SCHEME selector);
LIB_EXPORT
void TPMT_SIGNATURE_Print(TPMT_SIGNATURE *source);

#endif
