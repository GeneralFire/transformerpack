/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _ERROR_REPORT_LIB_LISTENER_H_
#define _ERROR_REPORT_LIB_LISTENER_H_

//
// Includes
//
#include <Pi/PiStatusCode.h>
#include <Guid/Cper.h>

//
// Definition
//
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to collect Additional Error Logging Data
#define MAX_NUM_MCA_BANKS       32
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to collect Additional Error Logging Data

//
//Reported by RAS Error Log driver
//
#define SMM_RAS_ENHANCED_ERROR_LOG  (EFI_SOFTWARE_SMM_DRIVER | EFI_SUBCLASS_SPECIFIC | 8)


typedef struct _MCA_BANK_INFO{
  UINT32                    BankNum;
  UINT32                    ApicId;
  UINT32                    BankType;
  UINT32                    BankScope;
  UINT64                    McaStatus;
  UINT64                    McaAddress;
  UINT64                    McaMisc;
} MCA_BANK_INFO;

// APTIOV_SERVER_OVERRIDE_RC_START: Added support to collect Additional Error Logging Data
typedef struct _AMI_MEMORY_ERROR_INFO {
  UINT64                    RankAddress; // Rank Address
  UINT8                     OtherMcBankCount;  // Total valid MCBanks
  MCA_BANK_INFO             OtherMcBankInfo[MAX_NUM_MCA_BANKS];  // Total valid MC Banks data
} AMI_MEMORY_ERROR_INFO;

typedef struct _AMI_PROCESSOR_ERROR_INFO {
  UINT8                     OtherMcBankCount;  // Total valid MCBanks
  MCA_BANK_INFO             OtherMcBankInfo[MAX_NUM_MCA_BANKS];  // Total valid MC Banks data
} AMI_PROCESSOR_ERROR_INFO;

typedef struct AMI_PCIE_ERROR_INFO {
  UINT16                      PrimaryStatus;
  UINT16                      PcieDeviceStatus;
  UINT32                      XpCorErrSts;
  UINT32                      XpUncErrSts;
} AMI_PCIE_ERROR_INFO;
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to collect Additional Error Logging Data

typedef struct _EMCA_MEMORY_ERROR_SECTION {
  EFI_PLATFORM_MEMORY_ERROR_DATA  MemoryErrorData;
  MCA_BANK_INFO                   McaBankInfo;
} EMCA_MEMORY_ERROR_SECTION;

typedef struct _EMCA_PROCESSOR_ERROR_SECTION {
  EFI_PROCESSOR_GENERIC_ERROR_DATA  ProcessorGenericErrorData;
  MCA_BANK_INFO                     McaBankInfo;
} EMCA_PROCESSOR_ERROR_SECTION;

typedef struct _PCIE_ERROR_SECTION {
  EFI_PCIE_ERROR_DATA                     Section;
  UINT32                                  Uncerrsts;
  UINT32                                  Corerrsts;
  UINT32                                  Uncerrmsk;
  UINT32                                  Corerrmsk;
  UINT32                                  Rperrsts;
} PCIE_ERROR_SECTION;

typedef struct _EMCA_MEM_ERROR_RECORD {
  EFI_COMMON_ERROR_RECORD_HEADER          Header;
  EFI_ERROR_SECTION_DESCRIPTOR            Descriptor;
  EMCA_MEMORY_ERROR_SECTION               EmcaMemErrorSection;
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to collect Additional Error Logging Data
  AMI_MEMORY_ERROR_INFO                   AmiAdditionalMemoryErrorInfo;
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to collect Additional Error Logging Data  
} EMCA_MEM_ERROR_RECORD;

typedef struct _MEM_ERROR_RECORD {
  EFI_COMMON_ERROR_RECORD_HEADER          Header;
  EFI_ERROR_SECTION_DESCRIPTOR            Descriptor;
  EFI_PLATFORM_MEMORY_ERROR_DATA          MemErrorSection;
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to collect Additional Error Logging Data
  AMI_MEMORY_ERROR_INFO                   AmiAdditionalMemoryErrorInfo;
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to collect Additional Error Logging Data  
} MEM_ERROR_RECORD;

typedef struct _PROCESSOR_ERROR_RECORD {
  EFI_COMMON_ERROR_RECORD_HEADER          Header;
  EFI_ERROR_SECTION_DESCRIPTOR            Descriptor;
  EMCA_PROCESSOR_ERROR_SECTION            EmcaProcErrorSection;
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to collect Additional Error Logging Data
  AMI_PROCESSOR_ERROR_INFO                AmiAdditionalProcessorErrorInfo;
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to collect Additional Error Logging Data  
} PROCESSOR_ERROR_RECORD;

typedef struct _PCIE_ERROR_RECORD {
  EFI_COMMON_ERROR_RECORD_HEADER          Header;
  EFI_ERROR_SECTION_DESCRIPTOR            Descriptor;
  PCIE_ERROR_SECTION                      PcieErrorSection;
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to collect Additional Error Logging Data
  AMI_PCIE_ERROR_INFO                     AmiAdditionalPcieErrorInfo;
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to collect Additional Error Logging Data
} PCIE_ERROR_RECORD;

typedef struct _FIRMWARE_ERROR_RECORD {
  EFI_COMMON_ERROR_RECORD_HEADER          Header;
  EFI_ERROR_SECTION_DESCRIPTOR            Descriptor;
  EFI_FIRMWARE_ERROR_DATA                 Section;
} FIRMWARE_ERROR_RECORD;

#endif  //_ERROR_REPORT_LIB_LISTENER_H_
