/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#ifndef _SMBIOS_IFWI_H
#define _SMBIOS_IFWI_H

#define EFI_SMBIOS_TYPE_IFWI_CFG    221
#define MAX_StringLength            86
#define ReservedNum                 5
//SKX_TEMP_SKIP_EXT need to debug why fail with 3 when snc enabled.
//                  It doesn't affect boot becuase this is for diaplay
//#define MicrocodeNumMax             3
#define MicrocodeNumMax             2
#define OpRomNumMax                 21
#define RomNumMax                   21
#define BOARD_MFG_OFFSET            6
#define MaxSMBDataRead              16
#ifndef FRUMAXSTRING
#define FRUMAXSTRING 128

#define PLATFORM_FVI_STATUS_DISABLED "Disabled"
#define DFT_ID "SmbiosIFWI"
#define VersionStringNumber 4

enum {
  Major = 0,
  Minor,
  Revision,
  BuildNumber,
}VersionsIndex;

#define DEBUG_FUN_ENTRY DEBUG((EFI_D_INFO, "["DFT_ID"] %s Entry\n", __FUNCTION__))
#define DEBUG_FUN_EXIT DEBUG((EFI_D_INFO, "["DFT_ID"] %s Exit\n", __FUNCTION__))

/**
  Plat_GetRCVer: Get Platform RC Version

  @param VersionString                    - pointer to RC Version string

  @retval None

**/

VOID
Plat_GetRCVer(
  OUT UINT16 VersionString[VersionStringNumber]
);

/**
  Plat_GetMEVer: Get Platform ME Version

  @param VersionString                    - pointer to ME Version string

  @retval None

**/

VOID
Plat_GetMEVer(
  OUT UINT16 VersionString[VersionStringNumber]
);

#define FVI_VERSION(Version) \
{ \
  TO_BE_FILLED, \
  TO_BE_FILLED, \
  Version \
}

#define ME_FW_STRING  "ME FW Version"
#define ME_FW_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define BIOS_ACM_STRING  "BIOS ACM Version"
#define BIOS_ACM_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define BMC_FW_STRING  "BMC FW Version"
#define BMC_FW_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define FRU_SDR_STRING  "FRU SDR Version"
#define FRU_SDR_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define FRU_STRING  "FRU Version"
#define FRU_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define SDR_STRING  "SDR Version"
#define SDR_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define CPU_UCODE1_STRING  "CPU uCODE1 Version"
#define CPU_UCODE1_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define CPU_UCODE2_STRING  "CPU uCODE2 Version"
#define CPU_UCODE2_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define CPU_UCODE3_STRING  "CPU uCODE3 Version"
#define CPU_UCODE3_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define SINIT_ACM_STRING  "SINIT ACM Version"
#define SINIT_ACM_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define MRC_STRING  "MRC Version"
#define MRC_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define QPIRC_STRING  "QPIRC Version"
#define QPIRC_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define PSU_FW_STRING  "PSU FW Version"
#define PSU_FW_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define RSTeSataEFI_STRING  "RSTeSataEFI Version"
#define RSTeSataEFI_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define RSTesSataEFI_STRING  "RSTesSataEFI Version"
#define RSTesSataEFI_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }
/***
#define RSTSataEfi_STRING  "RSTSataEfi Version"
#define RSTSataEfi_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }
***/
#define OneGEFI_STRING  "1GEFI Version"
#define OneGEFI_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define iBMCGOP_STRING  "iBMCGOP Version"
#define iBMCGOP_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define VMDDxeEfi_STRING  "VMDDxeEfi Version"
#define VMDDxeEfi_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define RSTeSataRaidEfi_STRING  "RSTeSataRaidEfi Version"
#define RSTeSataRaidEfi_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define NvmDimmDriver_STRING  "NvmDimmDriver Version"
#define NvmDimmDriver_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define NvmDimmHii_STRING  "NvmDimmHii Version"
#define NvmDimmHii_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

enum {
  ME_FW_VER = 0,
  BIOS_ACM_VER,
  BMC_FW_VER,
  FRU_SDR_VER,
  FRU_VER,
  SDR_VER,
  CPU_UCODE1_VER,
  CPU_UCODE2_VER,
  CPU_UCODE3_VER,
  SINIT_ACM_VER,
  MRC_VER,
  QPIRC_VER,
  PSU_FW_VER,
  RSTeSataEFI_VER,
  RSTesSataEFI_VER,
  //RSTSataEfi_VER,
  OneGEFI_VER,
  iBMCGOP_VER,
  VMDDxeEfi_VER,
  RSTeSataRaidEfi_VER,
  NvmDimmDriver_VER,
  NvmDimmHii_VER,
} PLATFORM_FVI_INDEX;

#endif

#pragma pack(1)
typedef struct {
  UINT8       BusID;
  UINT8       SlaveAddress;
  UINT8       ReadCount;
  UINT8       DataToWrite;
} EFI_IPMI_MASTER_WRITE_READ_REQ;

typedef struct {
  UINT8   CompletionCode;
  UINT8   MinorRevSec;
  UINT8   MinorRevPri;
  UINT8   MajorRev : 7;
  UINT8   AvoidDownGrade : 1;
} PSU_REV_DATA;

typedef struct {
  UINT8 FormatVersionNumber : 4;
  UINT8 Reserved : 4;
  UINT8 InternalUseStartingOffset;
  UINT8 ChassisInfoStartingOffset;
  UINT8 BoardAreaStartingOffset;
  UINT8 ProductInfoStartingOffset;
  UINT8 MultiRecInfoStartingOffset;
  UINT8 Pad;
  UINT8 Checksum;
} IPMI_FRU_COMMON_HEADER;

typedef struct {
  EFI_GUID                          Ffs_Guid;
  CHAR8                             RomName[MAX_StringLength];
  UINT8                             Ffs_SectionType;
  UINT32                            Rom_checksum;
  BOOLEAN                           bDisplayInOpromList;
} EFI_MISC_IFWI_Rom_DATA;

typedef struct {
  CHAR8               IFWIVersion[MAX_StringLength];
  CHAR8               BMCVersion[MAX_StringLength];
  CHAR8               MEVersion[MAX_StringLength];
  CHAR8               FRUSDRVersion[MAX_StringLength];
  CHAR8               FRUVersion[MAX_StringLength];
  CHAR8               SDRVersion[MAX_StringLength];
  CHAR8               CPUPatchVersion[3][MAX_StringLength];
  CHAR8               BIOSACMVersion[MAX_StringLength];
  CHAR8               SINITACMVersion[MAX_StringLength];
  CHAR8               MRCVersion[MAX_StringLength];
  CHAR8               QPIRCVersion[MAX_StringLength];
  CHAR8               PSUVersion[MAX_StringLength];
  CHAR8               Reserved[ReservedNum][MAX_StringLength];
  CHAR8               OnboardOpRomVersion[OpRomNumMax][MAX_StringLength];
} EFI_MISC_IFWI_CFG_VERSION;

typedef enum {
  RSTeSataEFI_Index = 1,
  RSTesSataEFI_Index,
  RSTSataEfi_Index,
  OneGEFI_Index,
  iBMCGOP_Index,
  VMDDxeEfi_Index,
  RSTeSataRaidEfi_Index,
  NvmDimmDriver_Index,
  NvmDimmHii_Index
} OnboardOpRomVersion_INDEX;

#pragma pack()

#endif
