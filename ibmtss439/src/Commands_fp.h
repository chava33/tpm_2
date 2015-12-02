/********************************************************************************/
/*										*/
/*			  COmmand and Response Marshal and Unmarshal		*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*            $Id: Commands_fp.h 373 2015-08-25 21:31:29Z kgoldman $		*/
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

#ifndef COMMANDS_FP_H
#define COMMANDS_FP_H

#include "TPM_Types.h"

#include "ActivateCredential_fp.h"
#include "CertifyCreation_fp.h"
#include "Certify_fp.h"
#include "ChangeEPS_fp.h"
#include "ChangePPS_fp.h"
#include "ClearControl_fp.h"
#include "Clear_fp.h"
#include "ClockRateAdjust_fp.h"
#include "ClockSet_fp.h"
#include "Commit_fp.h"
#include "Commit_fp.h"
#include "ContextLoad_fp.h"
#include "ContextSave_fp.h"
#include "CreatePrimary_fp.h"
#include "Create_fp.h"
#include "DictionaryAttackLockReset_fp.h"
#include "DictionaryAttackParameters_fp.h"
#include "Duplicate_fp.h"
#include "ECC_Parameters_fp.h"
#include "ECDH_KeyGen_fp.h"
#include "ECDH_ZGen_fp.h"
#include "EC_Ephemeral_fp.h"
#include "EncryptDecrypt_fp.h"
#include "EventSequenceComplete_fp.h"
#include "EvictControl_fp.h"
#include "FlushContext_fp.h"
#include "GetCapability_fp.h"
#include "GetCommandAuditDigest_fp.h"
#include "GetRandom_fp.h"
#include "GetSessionAuditDigest_fp.h"
#include "GetTestResult_fp.h"
#include "GetTime_fp.h"
#include "HMAC_Start_fp.h"
#include "HMAC_fp.h"
#include "HashSequenceStart_fp.h"
#include "Hash_fp.h"
#include "HierarchyChangeAuth_fp.h"
#include "HierarchyControl_fp.h"
#include "Import_fp.h"
#include "IncrementalSelfTest_fp.h"
#include "LoadExternal_fp.h"
#include "Load_fp.h"
#include "MakeCredential_fp.h"
#include "NV_Certify_fp.h"
#include "NV_ChangeAuth_fp.h"
#include "NV_DefineSpace_fp.h"
#include "NV_Extend_fp.h"
#include "NV_GlobalWriteLock_fp.h"
#include "NV_Increment_fp.h"
#include "NV_ReadLock_fp.h"
#include "NV_ReadPublic_fp.h"
#include "NV_Read_fp.h"
#include "NV_SetBits_fp.h"
#include "NV_UndefineSpaceSpecial_fp.h"
#include "NV_UndefineSpace_fp.h"
#include "NV_WriteLock_fp.h"
#include "NV_Write_fp.h"
#include "ObjectChangeAuth_fp.h"
#include "PCR_Allocate_fp.h"
#include "PCR_Event_fp.h"
#include "PCR_Extend_fp.h"
#include "PCR_Read_fp.h"
#include "PCR_Reset_fp.h"
#include "PCR_SetAuthPolicy_fp.h"
#include "PCR_SetAuthValue_fp.h"
#include "PP_Commands_fp.h"
#include "PolicyAuthValue_fp.h"
#include "PolicyAuthorize_fp.h"
#include "PolicyCommandCode_fp.h"
#include "PolicyCounterTimer_fp.h"
#include "PolicyCpHash_fp.h"
#include "PolicyDuplicationSelect_fp.h"
#include "PolicyGetDigest_fp.h"
#include "PolicyLocality_fp.h"
#include "PolicyNV_fp.h"
#include "PolicyNvWritten_fp.h"
#include "PolicyNameHash_fp.h"
#include "PolicyOR_fp.h"
#include "PolicyPCR_fp.h"
#include "PolicyPassword_fp.h"
#include "PolicyPhysicalPresence_fp.h"
#include "PolicyRestart_fp.h"
#include "PolicySecret_fp.h"
#include "PolicySigned_fp.h"
#include "PolicyTicket_fp.h"
#include "Quote_fp.h"
#include "RSA_Decrypt_fp.h"
#include "RSA_Encrypt_fp.h"
#include "ReadClock_fp.h"
#include "ReadPublic_fp.h"
#include "Rewrap_fp.h"
#include "SelfTest_fp.h"
#include "SequenceComplete_fp.h"
#include "SequenceUpdate_fp.h"
#include "SetAlgorithmSet_fp.h"
#include "SetCommandCodeAuditStatus_fp.h"
#include "SetPrimaryPolicy_fp.h"
#include "Shutdown_fp.h"
#include "Sign_fp.h"
#include "StartAuthSession_fp.h"
#include "Startup_fp.h"
#include "StirRandom_fp.h"
#include "TestParms_fp.h"
#include "Unseal_fp.h"
#include "VerifySignature_fp.h"
#include "ZGen_2Phase_fp.h"

typedef union {
    ActivateCredential_In         ActivateCredential;
    CertifyCreation_In            CertifyCreation;
    Certify_In                    Certify;
    ChangeEPS_In                  ChangeEPS;
    ChangePPS_In                  ChangePPS;
    ClearControl_In               ClearControl;
    Clear_In                      Clear;
    ClockRateAdjust_In            ClockRateAdjust;
    ClockSet_In                   ClockSet;
    Commit_In                     Commit;
    ContextLoad_In                ContextLoad;
    ContextSave_In                ContextSave;
    CreatePrimary_In              CreatePrimary;
    Create_In                     Create;
    DictionaryAttackLockReset_In  DictionaryAttackLockReset;
    DictionaryAttackParameters_In DictionaryAttackParameters;
    Duplicate_In                  Duplicate;
    ECC_Parameters_In             ECC_Parameters;
    ECDH_KeyGen_In                ECDH_KeyGen;
    ECDH_ZGen_In                  ECDH_ZGen;
    EC_Ephemeral_In               EC_Ephemeral;
    EncryptDecrypt_In             EncryptDecrypt;
    EventSequenceComplete_In      EventSequenceComplete;
    EvictControl_In               EvictControl;
    FlushContext_In               FlushContext;
    GetCapability_In              GetCapability;
    GetCommandAuditDigest_In      GetCommandAuditDigest;
    GetRandom_In                  GetRandom;
    GetSessionAuditDigest_In      GetSessionAuditDigest;
    GetTime_In                    GetTime;
    HMAC_In                       HMAC;
    HMAC_Start_In                 HMAC_Start;
    HashSequenceStart_In          HashSequenceStart;
    Hash_In                       Hash;
    HierarchyChangeAuth_In        HierarchyChangeAuth;
    HierarchyControl_In           HierarchyControl;
    Import_In                     Import;
    IncrementalSelfTest_In        IncrementalSelfTest;
    LoadExternal_In               LoadExternal;
    Load_In                       Load;
    MakeCredential_In             MakeCredential;
    NV_Certify_In                 NV_Certify;
    NV_ChangeAuth_In              NV_ChangeAuth;
    NV_DefineSpace_In             NV_DefineSpace;
    NV_Extend_In                  NV_Extend;
    NV_GlobalWriteLock_In         NV_GlobalWriteLock;
    NV_Increment_In               NV_Increment;
    NV_ReadLock_In                NV_ReadLock;
    NV_ReadPublic_In              NV_ReadPublic;
    NV_Read_In                    NV_Read;
    NV_SetBits_In                 NV_SetBits;
    NV_UndefineSpaceSpecial_In    NV_UndefineSpaceSpecial;
    NV_UndefineSpace_In           NV_UndefineSpace;
    NV_WriteLock_In               NV_WriteLock;
    NV_Write_In                   NV_Write;
    ObjectChangeAuth_In           ObjectChangeAuth;
    PCR_Allocate_In               PCR_Allocate;
    PCR_Event_In                  PCR_Event;
    PCR_Extend_In                 PCR_Extend;
    PCR_Read_In                   PCR_Read;
    PCR_Reset_In                  PCR_Reset;
    PCR_SetAuthPolicy_In          PCR_SetAuthPolicy;
    PCR_SetAuthValue_In           PCR_SetAuthValue;
    PP_Commands_In                PP_Commands;
    PolicyAuthValue_In            PolicyAuthValue;
    PolicyAuthorize_In            PolicyAuthorize;
    PolicyCommandCode_In          PolicyCommandCode;
    PolicyCounterTimer_In         PolicyCounterTimer;
    PolicyCpHash_In               PolicyCpHash;
    PolicyDuplicationSelect_In    PolicyDuplicationSelect;
    PolicyGetDigest_In            PolicyGetDigest;
    PolicyLocality_In             PolicyLocality;
    PolicyNV_In                   PolicyNV;
    PolicyNameHash_In             PolicyNameHash;
    PolicyOR_In                   PolicyOR;
    PolicyPCR_In                  PolicyPCR;
    PolicyPassword_In             PolicyPassword;
    PolicyPhysicalPresence_In     PolicyPhysicalPresence;
    PolicyRestart_In              PolicyRestart;
    PolicySecret_In               PolicySecret;
    PolicySigned_In               PolicySigned;
    PolicyTicket_In               PolicyTicket;
    Quote_In                      Quote;
    RSA_Decrypt_In                RSA_Decrypt;
    RSA_Encrypt_In                RSA_Encrypt;
    ReadPublic_In                 ReadPublic;
    Rewrap_In                     Rewrap;
    SelfTest_In                   SelfTest;
    SequenceComplete_In           SequenceComplete;
    SequenceUpdate_In             SequenceUpdate;
    SetAlgorithmSet_In            SetAlgorithmSet;
    SetCommandCodeAuditStatus_In  SetCommandCodeAuditStatus;
    SetPrimaryPolicy_In           SetPrimaryPolicy;
    Shutdown_In                   Shutdown;
    Sign_In                       Sign;
    StartAuthSession_In           StartAuthSession;
    Startup_In                    Startup;
    StirRandom_In                 StirRandom;
    TestParms_In                  TestParms;
    Unseal_In                     Unseal;
    VerifySignature_In            VerifySignature;
    ZGen_2Phase_In                ZGen_2Phase;
} COMMAND_PARAMETERS;

typedef union
{
    ActivateCredential_Out         ActivateCredential;
    CertifyCreation_Out            CertifyCreation;
    Certify_Out                    Certify;
    Commit_Out                     Commit;
    ContextLoad_Out                ContextLoad;
    ContextSave_Out                ContextSave;
    CreatePrimary_Out              CreatePrimary;
    Create_Out                     Create;
    Duplicate_Out                  Duplicate;
    ECC_Parameters_Out             ECC_Parameters;
    ECDH_KeyGen_Out                ECDH_KeyGen;
    ECDH_ZGen_Out                  ECDH_ZGen;
    EC_Ephemeral_Out               EC_Ephemeral;
    EncryptDecrypt_Out             EncryptDecrypt;
    EventSequenceComplete_Out      EventSequenceComplete;
    GetCapability_Out              GetCapability;
    GetCommandAuditDigest_Out      GetCommandAuditDigest;
    GetRandom_Out                  GetRandom;
    GetSessionAuditDigest_Out      GetSessionAuditDigest;
    GetTestResult_Out              GetTestResult;
    GetTime_Out                    GetTime;
    HMAC_Out                       HMAC;
    HMAC_Start_Out                 HMAC_Start;
    HashSequenceStart_Out          HashSequenceStart;
    Hash_Out                       Hash;
    Import_Out                     Import;
    IncrementalSelfTest_Out        IncrementalSelfTest;
    LoadExternal_Out               LoadExternal;
    Load_Out                       Load;
    MakeCredential_Out             MakeCredential;
    NV_Certify_Out                 NV_Certify;
    NV_ReadPublic_Out              NV_ReadPublic;
    NV_Read_Out                    NV_Read;
    ObjectChangeAuth_Out           ObjectChangeAuth;
    PCR_Allocate_Out               PCR_Allocate;
    PCR_Event_Out                  PCR_Event;
    PCR_Read_Out                   PCR_Read;
    PolicyGetDigest_Out            PolicyGetDigest;
    PolicySecret_Out               PolicySecret;
    PolicySigned_Out               PolicySigned;
    Quote_Out                      Quote;
    RSA_Decrypt_Out                RSA_Decrypt;
    RSA_Encrypt_Out                RSA_Encrypt;
    ReadClock_Out                  ReadClock;
    ReadPublic_Out                 ReadPublic;
    Rewrap_Out                     Rewrap;
    SequenceComplete_Out           SequenceComplete;
    Sign_Out                       Sign;
    StartAuthSession_Out           StartAuthSession;
    Unseal_Out                     Unseal;
    VerifySignature_Out            VerifySignature;
    ZGen_2Phase_Out                ZGen_2Phase;
} RESPONSE_PARAMETERS;

#ifndef TPM_TSS
extern COMMAND_PARAMETERS in;
extern RESPONSE_PARAMETERS out;
#endif

TPM_RC
Startup_In_Unmarshal(Startup_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
Shutdown_In_Unmarshal(Shutdown_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
SelfTest_In_Unmarshal(SelfTest_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
IncrementalSelfTest_In_Unmarshal(IncrementalSelfTest_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
IncrementalSelfTest_Out_Marshal(IncrementalSelfTest_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
UINT16
GetTestResult_Out_Marshal(GetTestResult_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
StartAuthSession_In_Unmarshal(StartAuthSession_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
StartAuthSession_Out_Marshal(StartAuthSession_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
PolicyRestart_In_Unmarshal(PolicyRestart_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
Create_In_Unmarshal(Create_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Create_Out_Marshal(Create_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Load_In_Unmarshal(Load_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Load_Out_Marshal(Load_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
LoadExternal_In_Unmarshal(LoadExternal_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
LoadExternal_Out_Marshal(LoadExternal_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ReadPublic_In_Unmarshal(ReadPublic_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ReadPublic_Out_Marshal(ReadPublic_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ActivateCredential_In_Unmarshal(ActivateCredential_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ActivateCredential_Out_Marshal(ActivateCredential_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
MakeCredential_In_Unmarshal(MakeCredential_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
MakeCredential_Out_Marshal(MakeCredential_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Unseal_In_Unmarshal(Unseal_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Unseal_Out_Marshal(Unseal_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ObjectChangeAuth_In_Unmarshal(ObjectChangeAuth_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ObjectChangeAuth_Out_Marshal(ObjectChangeAuth_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Duplicate_In_Unmarshal(Duplicate_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Duplicate_Out_Marshal(Duplicate_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Rewrap_In_Unmarshal(Rewrap_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Rewrap_Out_Marshal(Rewrap_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Import_In_Unmarshal(Import_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Import_Out_Marshal(Import_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
RSA_Encrypt_In_Unmarshal(RSA_Encrypt_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
RSA_Encrypt_Out_Marshal(RSA_Encrypt_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
RSA_Decrypt_In_Unmarshal(RSA_Decrypt_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
RSA_Decrypt_Out_Marshal(RSA_Decrypt_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ECDH_KeyGen_In_Unmarshal(ECDH_KeyGen_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ECDH_KeyGen_Out_Marshal(ECDH_KeyGen_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ECDH_ZGen_In_Unmarshal(ECDH_ZGen_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ECDH_ZGen_Out_Marshal(ECDH_ZGen_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ECC_Parameters_In_Unmarshal(ECC_Parameters_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ECC_Parameters_Out_Marshal(ECC_Parameters_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ZGen_2Phase_In_Unmarshal(ZGen_2Phase_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ZGen_2Phase_Out_Marshal(ZGen_2Phase_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
EncryptDecrypt_In_Unmarshal(EncryptDecrypt_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
EncryptDecrypt_Out_Marshal(EncryptDecrypt_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Hash_In_Unmarshal(Hash_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Hash_Out_Marshal(Hash_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
HMAC_In_Unmarshal(HMAC_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
HMAC_Out_Marshal(HMAC_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
GetRandom_In_Unmarshal(GetRandom_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
GetRandom_Out_Marshal(GetRandom_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
StirRandom_In_Unmarshal(StirRandom_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
HMAC_Start_In_Unmarshal(HMAC_Start_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
HMAC_Start_Out_Marshal(HMAC_Start_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
HashSequenceStart_In_Unmarshal(HashSequenceStart_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
HashSequenceStart_Out_Marshal(HashSequenceStart_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
SequenceUpdate_In_Unmarshal(SequenceUpdate_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
SequenceComplete_In_Unmarshal(SequenceComplete_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
SequenceComplete_Out_Marshal(SequenceComplete_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
EventSequenceComplete_In_Unmarshal(EventSequenceComplete_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
EventSequenceComplete_Out_Marshal(EventSequenceComplete_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Certify_In_Unmarshal(Certify_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Certify_Out_Marshal(Certify_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
CertifyCreation_In_Unmarshal(CertifyCreation_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
CertifyCreation_Out_Marshal(CertifyCreation_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Quote_In_Unmarshal(Quote_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Quote_Out_Marshal(Quote_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
GetSessionAuditDigest_In_Unmarshal(GetSessionAuditDigest_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
GetSessionAuditDigest_Out_Marshal(GetSessionAuditDigest_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
GetCommandAuditDigest_In_Unmarshal(GetCommandAuditDigest_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
GetCommandAuditDigest_Out_Marshal(GetCommandAuditDigest_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
GetTime_In_Unmarshal(GetTime_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
GetTime_Out_Marshal(GetTime_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Commit_In_Unmarshal(Commit_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Commit_Out_Marshal(Commit_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
EC_Ephemeral_In_Unmarshal(EC_Ephemeral_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
EC_Ephemeral_Out_Marshal(EC_Ephemeral_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
VerifySignature_In_Unmarshal(VerifySignature_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
VerifySignature_Out_Marshal(VerifySignature_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
Sign_In_Unmarshal(Sign_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
Sign_Out_Marshal(Sign_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
SetCommandCodeAuditStatus_In_Unmarshal(SetCommandCodeAuditStatus_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PCR_Extend_In_Unmarshal(PCR_Extend_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PCR_Event_In_Unmarshal(PCR_Event_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
PCR_Event_Out_Marshal(PCR_Event_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
PCR_Read_In_Unmarshal(PCR_Read_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
PCR_Read_Out_Marshal(PCR_Read_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
PCR_Allocate_In_Unmarshal(PCR_Allocate_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
PCR_Allocate_Out_Marshal(PCR_Allocate_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
PCR_SetAuthPolicy_In_Unmarshal(PCR_SetAuthPolicy_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PCR_SetAuthValue_In_Unmarshal(PCR_SetAuthValue_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PCR_Reset_In_Unmarshal(PCR_Reset_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicySigned_In_Unmarshal(PolicySigned_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
PolicySigned_Out_Marshal(PolicySigned_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
PolicySecret_In_Unmarshal(PolicySecret_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
PolicySecret_Out_Marshal(PolicySecret_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
PolicyTicket_In_Unmarshal(PolicyTicket_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyOR_In_Unmarshal(PolicyOR_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyPCR_In_Unmarshal(PolicyPCR_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyLocality_In_Unmarshal(PolicyLocality_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyNV_In_Unmarshal(PolicyNV_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyCounterTimer_In_Unmarshal(PolicyCounterTimer_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyCommandCode_In_Unmarshal(PolicyCommandCode_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyPhysicalPresence_In_Unmarshal(PolicyPhysicalPresence_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyCpHash_In_Unmarshal(PolicyCpHash_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyNameHash_In_Unmarshal(PolicyNameHash_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyDuplicationSelect_In_Unmarshal(PolicyDuplicationSelect_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyAuthorize_In_Unmarshal(PolicyAuthorize_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyAuthValue_In_Unmarshal(PolicyAuthValue_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyPassword_In_Unmarshal(PolicyPassword_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PolicyGetDigest_In_Unmarshal(PolicyGetDigest_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
PolicyGetDigest_Out_Marshal(PolicyGetDigest_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
PolicyNvWritten_In_Unmarshal(PolicyNvWritten_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
CreatePrimary_In_Unmarshal(CreatePrimary_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
CreatePrimary_Out_Marshal(CreatePrimary_Out *source, TPMI_ST_COMMAND_TAG  tag, BYTE **buffer, INT32 *size);
TPM_RC
HierarchyControl_In_Unmarshal(HierarchyControl_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
SetPrimaryPolicy_In_Unmarshal(SetPrimaryPolicy_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
ChangePPS_In_Unmarshal(ChangePPS_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
ChangeEPS_In_Unmarshal(ChangeEPS_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
Clear_In_Unmarshal(Clear_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
ClearControl_In_Unmarshal(ClearControl_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
HierarchyChangeAuth_In_Unmarshal(HierarchyChangeAuth_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
DictionaryAttackLockReset_In_Unmarshal(DictionaryAttackLockReset_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
DictionaryAttackParameters_In_Unmarshal(DictionaryAttackParameters_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
PP_Commands_In_Unmarshal(PP_Commands_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
SetAlgorithmSet_In_Unmarshal(SetAlgorithmSet_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
ContextSave_In_Unmarshal(ContextSave_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ContextSave_Out_Marshal(ContextSave_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ContextLoad_In_Unmarshal(ContextLoad_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ContextLoad_Out_Marshal(ContextLoad_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
FlushContext_In_Unmarshal(FlushContext_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
EvictControl_In_Unmarshal(EvictControl_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
ReadClock_Out_Marshal(ReadClock_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
ClockSet_In_Unmarshal(ClockSet_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
ClockRateAdjust_In_Unmarshal(ClockRateAdjust_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
GetCapability_In_Unmarshal(GetCapability_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
GetCapability_Out_Marshal(GetCapability_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
TestParms_In_Unmarshal(TestParms_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_DefineSpace_In_Unmarshal(NV_DefineSpace_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_UndefineSpace_In_Unmarshal(NV_UndefineSpace_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_UndefineSpaceSpecial_In_Unmarshal(NV_UndefineSpaceSpecial_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_ReadPublic_In_Unmarshal(NV_ReadPublic_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
NV_ReadPublic_Out_Marshal(NV_ReadPublic_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
NV_Write_In_Unmarshal(NV_Write_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_Increment_In_Unmarshal(NV_Increment_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_Extend_In_Unmarshal(NV_Extend_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_SetBits_In_Unmarshal(NV_SetBits_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_WriteLock_In_Unmarshal(NV_WriteLock_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_GlobalWriteLock_In_Unmarshal(NV_GlobalWriteLock_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_Read_In_Unmarshal(NV_Read_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
NV_Read_Out_Marshal(NV_Read_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);
TPM_RC
NV_ReadLock_In_Unmarshal(NV_ReadLock_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_ChangeAuth_In_Unmarshal(NV_ChangeAuth_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
TPM_RC
NV_Certify_In_Unmarshal(NV_Certify_In *target, BYTE **buffer, INT32 *size, TPM_HANDLE handles[]);
UINT16
NV_Certify_Out_Marshal(NV_Certify_Out *source, TPMI_ST_COMMAND_TAG tag, BYTE **buffer, INT32 *size);

#endif
