/** @file
  Provides data structure information used by SiliconIp SGX-TEM

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

//
// SGX
//
UINT8  EnableSgx;
UINT8  SgxFactoryReset;                // Delete all registration data, if SGX enabled force IPE/FirstBinding flow
UINT64 PrmrrSize;                      // SGX PRMRR size
UINT64 SprspOrLaterPrmSize;            // SGX PRM size (SPR+ only)
UINT8  SgxQoS;                         // SGX Quality of Service
UINT8  SgxAutoRegistrationAgent;
UINT8  SgxPackageInfoInBandAccess;     // Expose Package Info to OS
UINT8  EpochUpdate;
UINT64 SgxEpoch0;                      // SGX EPOCH0 value {0 - 0xFFFFFFFFFFFFFFFF}
UINT64 SgxEpoch1;                      // SGX EPOCH1 value {0 - 0xFFFFFFFFFFFFFFFF}
UINT8  SgxLeWr;                        // Flexible Launch Enclave Policy (Wr En)
UINT64 SgxLePubKeyHash0;               // Launch Enclave Hash 0
UINT64 SgxLePubKeyHash1;               // Launch Enclave Hash 1
UINT64 SgxLePubKeyHash2;               // Launch Enclave Hash 2
UINT64 SgxLePubKeyHash3;               // Launch Enclave Hash 3
// Client SGX - unused in server
UINT8  SgxSinitNvsData;                // SGX NVS data from Flash passed during previous boot using CPU_INFO_PROTOCOL.SGX_INFO;
                                       // Pass value of zero if there is not data saved or when SGX is disabled.
UINT8  SgxSinitDataFromTpm;            // SGX SVN data from TPM; 0: when SGX is disabled or TPM is not present or no data
                                       // is present in TPM.
UINT8  SgxDebugMode;
// DFX
UINT8  DfxSgxDebugPrint;
UINT8  DfxMcheckMsr72;
UINT8  DfxMockSgxIsNotCapable;
UINT8  DfxSgxRegistrationServerSelect;
UINT8  DfxSgxAddPackageSupport;
