/** @file
  BIOS Guard EarlyPost initializations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2018 Intel Corporation. <BR>

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

#include "BiosGuardInit.h"
#include "Library/BaseCryptLib.h"
#include "FlashMap.h"
#include <Guid/PlatformInfo.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/PcdLib.h>
#include <Library/ProcMemInitChipLib.h>

extern EFI_GUID gBiosGuardHobGuid;

/*++

  Perform the restoration of anti-flash wear out settings in case of S3 resume.

  @param None

  @retval None

--*/
VOID
RestoreAntiFlashWearout(
  VOID
)
{
  UINT64_STRUCT                     MsrReg;
  ICX_MSR_BIOS_INFO_FLAGS_REGISTER  BiosInfoFlags;
  UINT8                             SocketId;

  //
  // Set the WPE bit to enable Anti Flash Wearout on each CPU
  //
  BiosInfoFlags.Uint64 = AsmReadMsr64 (ICX_MSR_BIOS_INFO_FLAGS);
  BiosInfoFlags.Bits.BiosInfoFlagsData = B_MSR_BIOS_INFO_FLAGS_WPE;

  MsrReg = *((UINT64_STRUCT *) &BiosInfoFlags.Uint64);

  for(SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (SocketPresent (SocketId)) {
      WriteMsrPipeHL (SocketId, ICX_MSR_BIOS_INFO_FLAGS, MsrReg);
    }
  }
}

/**

  Initial settings for BIOS Guard Config structure.  Taken from Sample and/or Ref Code.

  @param BiosGuardConfig           - Pointer to the BIOS Guard config structure.

  @retval None

**/
VOID
InitializeBiosGuardConfigData(
  IN BIOSGUARD_CONFIG *BiosGuardConfig
  )
{
  UINT8 PlatIdStr[] = "PURLEY";

  ///
  /// If CpuConfig->BiosGuard is set to ENABLE '1' then
  ///   PlatformData->SmmBwp (found in PchPolicyInitPei.c file) has to be set to ENABLE '1'
  /// This is a BIOS Guard Security requirement that needs to be addressed
  /// If CpuConfig->BiosGuard is set to DISABLE '0' then
  ///   PlatformData->SmmBwp (found in PchPolicyInitPei.c file) value don't care, it can be
  ///   set to either ENABLE '1' or DISABLE '0' based on customer implementation
  ///
  ZeroMem (&BiosGuardConfig->Bgpdt, sizeof (BGPDT));
  BiosGuardConfig->Bgpdt.BgpdtMajVer      = BGPDT_MAJOR_VERSION;
  BiosGuardConfig->Bgpdt.BgpdtMinVer      = BGPDT_MINOR_VERSION;
  CopyMem (&BiosGuardConfig->Bgpdt.PlatId[0], &PlatIdStr[0], sizeof (PlatIdStr));
  BiosGuardConfig->Bgpdt.BgModSvn         = BIOSGUARD_SVN;
  BiosGuardConfig->Bgpdt.BiosSvn          = 0x00640000;
  BiosGuardConfig->Bgpdt.LastSfam         = MIN_SFAM_COUNT - 1;
  BiosGuardConfig->Bgpdt.SfamData[0].FirstByte = 0x00580000;
  BiosGuardConfig->Bgpdt.SfamData[0].LastByte  = 0x0058FFFF;
  BiosGuardConfig->Bgpdt.BgpdtSize             = (sizeof (BGPDT) - sizeof (BiosGuardConfig->Bgpdt.SfamData) + ((BiosGuardConfig->Bgpdt.LastSfam + 1) * sizeof (SFAM_DATA)));
  BiosGuardConfig->BgpdtHash[0]                = 0xae7295370672663c;
  BiosGuardConfig->BgpdtHash[1]                = 0x220375c996d23a36;
  BiosGuardConfig->BgpdtHash[2]                = 0x73aaea0f2afded9d;
  BiosGuardConfig->BgpdtHash[3]                = 0x707193b768a0829e;
  ZeroMem (&BiosGuardConfig->BgupHeader, sizeof (BGUP_HEADER));
  BiosGuardConfig->BgupHeader.Version          = BGUP_HDR_VERSION;
  CopyMem (&BiosGuardConfig->BgupHeader.PlatId[0], &PlatIdStr[0], sizeof (PlatIdStr));
  BiosGuardConfig->BgupHeader.PkgAttributes    = 0;
  BiosGuardConfig->BgupHeader.PslMajorVer      = PSL_MAJOR_VERSION;
  BiosGuardConfig->BgupHeader.PslMinorVer      = PSL_MINOR_VERSION;
  BiosGuardConfig->BgupHeader.BiosSvn          = BiosGuardConfig->Bgpdt.BiosSvn;
  BiosGuardConfig->BgupHeader.EcSvn            = 0;
  BiosGuardConfig->BgupHeader.VendorSpecific   = 0x808655AA;
  ZeroMem (&BiosGuardConfig->BiosGuardLog, sizeof (BIOSGUARD_LOG));
  BiosGuardConfig->BiosGuardLog.Version        = BIOSGUARD_LOG_VERSION;
  BiosGuardConfig->BiosGuardMemSize       = (UINT8)RShiftU64(FLASH_SIZE + SIZE_1MB, 20);

  return;
}

/**

  Final customizations to the BIOS Guard Platform structure are made here.
  Keys or any other specific changes should be done here.

  @param **PeiServices          - Indirect reference to the PEI Services Table.
  @param *BiosGuardConfig       - Pointer to the BIOS Guard config structure.
  @param FlashWearOutProtection - Value from Setup if FlashWearOut is enabled.

  @retval None

**/
VOID
CustomizeBiosGuardConfigData(
  IN EFI_PEI_SERVICES     **PeiServices,
  IN BIOSGUARD_CONFIG     *BiosGuardConfig,
  IN UINT8                FlashWearOutProtection
  )
{
  EFI_STATUS        Status;
  UINT32            FlashBase;
  UINT8             NumSpiComponents;
  UINT32            TotalFlashSize;
  VOID              *Sha256Context;
  VOID              *Data;
  UINTN             DataSize;
  UINT8             HashValue[SHA256_DIGEST_SIZE];
#ifdef BIOSGUARD_EC_FLAG
  UINT8             EcStatus;
  PEI_STALL_PPI     *StallPpi;
  PEI_CPU_IO_PPI    *CpuIo;
#endif //BIOSGUARD_EC_FLAG
  UINT8             BgpdtPkeySlot0[] = { 0x6a, 0x94, 0x81, 0x87, 0xfd, 0x8c, 0x51, 0x0a, 0x2a, 0x74, 0x9c, 0x7b, 0xe3, 0xf7, 0x5c, 0x3a, 0xbf, 0xce, 0x60, 0x4f, 0x68, 0xeb, 0xc6, 0x8f, 0x69, 0x98, 0x8c, 0xc5, 0xdb, 0xe3, 0x29, 0x70 };
  UINT8             BgpdtPkeySlot1[] = { 0x1c, 0x51, 0x3a, 0xc5, 0x1c, 0x3d, 0x63, 0x10, 0xef, 0xed, 0xfe, 0xc5, 0xf3, 0x4b, 0xf2, 0xe0, 0x9b, 0x22, 0xe8, 0x06, 0xab, 0xd3, 0x19, 0x2e, 0xfa, 0xfb, 0x6e, 0xd9, 0x36, 0x0d, 0x68, 0x18 };
  UINT8             BgpdtPkeySlot2[] = { 0x19, 0x0b, 0x33, 0xf8, 0xde, 0x3a, 0xa7, 0x9b, 0x57, 0xad, 0xb2, 0x45, 0x86, 0x0e, 0x7f, 0x0e, 0x40, 0x62, 0x80, 0x22, 0x8f, 0x04, 0x92, 0xec, 0x87, 0x44, 0x81, 0xd9, 0xef, 0xed, 0x9f, 0xa3 };
  UINTN             PchSpiBase;

  PchSpiBase = MmioRead32 (MmPciBase (
                            DEFAULT_PCI_BUS_NUMBER_PCH,
                            PCI_DEVICE_NUMBER_PCH_SPI,
                            PCI_FUNCTION_NUMBER_PCH_SPI)
                            + R_SPI_CFG_BAR0) & ~B_SPI_CFG_BAR0_MASK;

  //
  // Select to Flash Map 0 Register to get the number of flash Component
  //
  MmioAndThenOr32 (
    (PchSpiBase + R_SPI_MEM_FDOC),
    (UINT32) (~(B_SPI_MEM_FDOC_FDSS_MASK | B_SPI_MEM_FDOC_FDSI_MASK)),
    (UINT32) (V_SPI_MEM_FDOC_FDSS_FSDM | R_SPI_FLASH_FDBAR_FLASH_MAP0)
  );
  /**
    Copy Zero based Number Of Components
    Valid Bit Settings:
      - 00 : 1 Component
      - 01 : 2 Components
      - All other settings : Reserved
  **/
  NumSpiComponents = (UINT8) ((MmioRead16 (PchSpiBase + R_SPI_MEM_FDOD) & B_SPI_FLASH_FDBAR_NC) >> N_SPI_FLASH_FDBAR_NC);
  //
  // Select to Flash Components Register to get Components Density
  //
  MmioAndThenOr32 (
    (PchSpiBase + R_SPI_MEM_FDOC),
    (UINT32) (~(B_SPI_MEM_FDOC_FDSS_MASK | B_SPI_MEM_FDOC_FDSI_MASK)),
    (UINT32) (V_SPI_MEM_FDOC_FDSS_COMP | R_SPI_FLASH_FCBA_FLCOMP)
  );
  ///
  /// Calculate TotalFlashSize from Descriptor information
  ///
  FlashBase = (UINT8) MmioRead32 (PchSpiBase + R_SPI_MEM_FDOD);
  TotalFlashSize = (SPI_SIZE_BASE_512KB << ((UINT8) (FlashBase & B_SPI_FLASH_FLCOMP_COMP0_MASK)));
  if (NumSpiComponents == 1) {
    TotalFlashSize += (SPI_SIZE_BASE_512KB << ((UINT8) ((FlashBase & B_SPI_FLASH_FLCOMP_COMP1_MASK) >> 4)));
  }

  FlashBase = TotalFlashSize - FLASH_SIZE;

  BiosGuardConfig->TotalFlashSize = TotalFlashSize;

  BiosGuardConfig->Bgpdt.LastSfam              = 4;
  BiosGuardConfig->Bgpdt.SfamData[0].FirstByte = FLASH_REGION_FVMAIN_BASE;
  BiosGuardConfig->Bgpdt.SfamData[0].LastByte  = FLASH_REGION_FVMAIN_BASE + (FLASH_REGION_FVMAIN_SIZE - 1);
  BiosGuardConfig->Bgpdt.SfamData[1].FirstByte = FLASH_REGION_OEM_FV_BASE;
  BiosGuardConfig->Bgpdt.SfamData[1].LastByte  = FLASH_REGION_OEM_FV_BASE + (FLASH_REGION_OEM_FV_SIZE - 1);
  BiosGuardConfig->Bgpdt.SfamData[2].FirstByte = FLASH_REGION_MICROCODE_FV_BASE;
  BiosGuardConfig->Bgpdt.SfamData[2].LastByte  = FLASH_REGION_MICROCODE_FV_BASE + (FLASH_REGION_MICROCODE_FV_SIZE - 1);
  BiosGuardConfig->Bgpdt.SfamData[3].FirstByte = FLASH_REGION_FV_MRC_NORMAL_BASE;
  BiosGuardConfig->Bgpdt.SfamData[3].LastByte  = FLASH_REGION_FV_MRC_NORMAL_BASE + (FLASH_REGION_FV_MRC_NORMAL_SIZE - 1);
  BiosGuardConfig->Bgpdt.SfamData[4].FirstByte = FLASH_REGION_BIOS_ACM_FV_BASE;
  BiosGuardConfig->Bgpdt.SfamData[4].LastByte  = FLASH_REGION_BIOS_ACM_FV_BASE + (FLASH_REGION_BIOS_ACM_FV_SIZE - 1);
  CopyMem (&BiosGuardConfig->Bgpdt.PkeySlot0[0], &BgpdtPkeySlot0[0], sizeof (BgpdtPkeySlot0));
  CopyMem (&BiosGuardConfig->Bgpdt.PkeySlot1[0], &BgpdtPkeySlot1[0], sizeof (BgpdtPkeySlot1));
  CopyMem (&BiosGuardConfig->Bgpdt.PkeySlot2[0], &BgpdtPkeySlot2[0], sizeof (BgpdtPkeySlot2));
  BiosGuardConfig->BiosGuardLog.LastPage       = MAX_BIOSGUARD_LOG_PAGE - 1;
  BiosGuardConfig->BiosGuardLog.LoggingOptions = BIOSGUARD_LOG_OPT_DEBUG | BIOSGUARD_LOG_OPT_FLASH_ERROR | BIOSGUARD_LOG_OPT_FLASH_ERASE | BIOSGUARD_LOG_OPT_FLASH_WRITE | BIOSGUARD_LOG_OPT_BRANCH_TRACE | BIOSGUARD_LOG_OPT_STEP_TRACE;
#ifdef BIOSGUARD_EC_FLAG
  if (PlatformInfoHob->EcPresent == TRUE) {
    DEBUG ((EFI_D_INFO, "EC is Present\n"));
    BiosGuardConfig->Bgpdt.PlatAttr           |= EC_PRESENT;
    BiosGuardConfig->Bgpdt.EcCmd               = EC_C_PORT;
    BiosGuardConfig->Bgpdt.EcData              = EC_D_PORT;
    BiosGuardConfig->EcCmdDiscovery            = 0xB0;
    BiosGuardConfig->EcCmdProvisionEav         = 0xB1;
    BiosGuardConfig->EcCmdLock                 = 0xB2;
    Status = SendEcCommand (BiosGuardConfig->EcCmdDiscovery);
    if (Status == EFI_SUCCESS) {
      Status = ReceiveEcData (&EcStatus);
      if (((EcStatus & 0xF8) == 0) && (EcStatus & BIT0)) {
        if (((EcStatus & (BIT2 | BIT1)) == (BIT2 | BIT1)) || (((EcStatus & (BIT2 | BIT1)) == 0))) {
          DEBUG ((EFI_D_INFO, "EC FW supports BIOS Guard\n"));
          BiosGuardConfig->Bgpdt.PlatAttr     |= EC_BIOSGUARD_PROTECTED;
          BiosGuardConfig->Bgpdt.EcCmdGetSvn   = 0xB3;
          BiosGuardConfig->Bgpdt.EcCmdOpen     = 0xB4;
          BiosGuardConfig->Bgpdt.EcCmdClose    = 0xB5;
          BiosGuardConfig->Bgpdt.EcCmdPortTest = 0xB6;
          BiosGuardConfig->BgupHeader.EcSvn    = 0x00010000;
        } else {
          DEBUG ((EFI_D_ERROR, "EC FW Error\n"));
        }
      } else {
        DEBUG ((EFI_D_WARN, "EC FW do not support BIOS Guard\n"));
      }
    } else {
      DEBUG ((EFI_D_ERROR, "SendEcCommand(EcCmdDiscovery) Failed\n"));
    }
  }
#endif //BIOSGUARD_EC_FLAG
  if (FlashWearOutProtection) {
    BiosGuardConfig->Bgpdt.PlatAttr |= FLASH_WEAR_OUT_PROTECTION;
  }
  BiosGuardConfig->Bgpdt.PlatAttr |= FTU_ENABLE;
  BiosGuardConfig->Bgpdt.BgpdtSize = (sizeof (BGPDT) - sizeof (BiosGuardConfig->Bgpdt.SfamData) + ((BiosGuardConfig->Bgpdt.LastSfam + 1) * sizeof (SFAM_DATA)));
  DataSize = Sha256GetContextSize ();
  Status   = (*PeiServices)->AllocatePool (PeiServices, DataSize, &Sha256Context);
  ASSERT_EFI_ERROR (Status);
  DataSize = BiosGuardConfig->Bgpdt.BgpdtSize;
  Data     = (VOID *) &BiosGuardConfig->Bgpdt;
  Sha256Init   (Sha256Context);
  Sha256Update (Sha256Context, Data, DataSize);
  Sha256Final  (Sha256Context, HashValue);
  CopyMem (&BiosGuardConfig->BgpdtHash[0], &HashValue[0], 8);
  CopyMem (&BiosGuardConfig->BgpdtHash[1], &HashValue[8], 8);
  CopyMem (&BiosGuardConfig->BgpdtHash[2], &HashValue[16], 8);
  CopyMem (&BiosGuardConfig->BgpdtHash[3], &HashValue[24], 8);

  return;
}

//#if 0
// ** Removing to make space
/**

  Prints all the BIOS Guard related settings to the screen.

  @param BiosGuardConfig           - Pointer to the BIOS Guard config structure.

  @retval None

**/
VOID
PrintBiosGuardInfo(
  IN BIOSGUARD_CONFIG          *BiosGuardConfig
  )
{
  UINT8 Index;
  //
  // BIOSGUARD_CONFIG - Print values
  //
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : Version : 0x%X\n", BiosGuardConfig->BgupHeader.Version));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : PlatId[16] :\n"));
  for (Index = 0; Index < 16; Index++) {
    if (Index == 15) {
      DEBUG ((EFI_D_VERBOSE, " 0x%X\n", BiosGuardConfig->Bgpdt.PlatId[Index]));
    } else {
      DEBUG ((EFI_D_VERBOSE, " 0x%X ,", BiosGuardConfig->Bgpdt.PlatId[Index]));
    }
  }
  DEBUG ((EFI_D_VERBOSE, " \n"));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : PkgAttributes : 0x%X\n", BiosGuardConfig->BgupHeader.PkgAttributes));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : PslMajorVer : 0x%X\n", BiosGuardConfig->BgupHeader.PslMajorVer));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : PslMinorVer : 0x%X\n", BiosGuardConfig->BgupHeader.PslMinorVer));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : ScriptSectionSize : 0x%X\n", BiosGuardConfig->BgupHeader.ScriptSectionSize));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : DataSectionSize : 0x%X\n", BiosGuardConfig->BgupHeader.DataSectionSize));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : BiosSvn : 0x%X\n", BiosGuardConfig->BgupHeader.BiosSvn));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : EcSvn : 0x%X\n", BiosGuardConfig->BgupHeader.EcSvn));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGUP_HEADER : VendorSpecific : 0x%X\n", BiosGuardConfig->BgupHeader.VendorSpecific));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : BgpdtSize : 0x%X\n", BiosGuardConfig->Bgpdt.BgpdtSize));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : BgpdtMajVer : 0x%X\n", BiosGuardConfig->Bgpdt.BgpdtMajVer));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : BgpdtMinVer : 0x%X\n", BiosGuardConfig->Bgpdt.BgpdtMinVer));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : PlatId[16] :\n"));
  for (Index = 0; Index < 16; Index++) {
    if (Index == 15) {
      DEBUG ((EFI_D_VERBOSE, " 0x%X\n", BiosGuardConfig->Bgpdt.PlatId[Index]));
    } else {
      DEBUG ((EFI_D_VERBOSE, " 0x%X ,", BiosGuardConfig->Bgpdt.PlatId[Index]));
    }
  }
  DEBUG ((EFI_D_VERBOSE, " \n"));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : PkeySlot0[32] :\n"));
  for (Index = 0; Index < 32; Index++) {
    if ((Index == 15) || (Index == 31)) {
      DEBUG ((EFI_D_VERBOSE, " 0x%X\n", BiosGuardConfig->Bgpdt.PkeySlot0[Index]));
    } else {
      DEBUG ((EFI_D_VERBOSE, " 0x%X ,", BiosGuardConfig->Bgpdt.PkeySlot0[Index]));
    }
  }
  DEBUG ((EFI_D_VERBOSE, " \n"));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : PkeySlot1[32] :\n"));
  for (Index = 0; Index < 32; Index++) {
    if ((Index == 15) || (Index == 31)) {
      DEBUG ((EFI_D_VERBOSE, " 0x%X\n", BiosGuardConfig->Bgpdt.PkeySlot1[Index]));
    } else {
      DEBUG ((EFI_D_VERBOSE, " 0x%X ,", BiosGuardConfig->Bgpdt.PkeySlot1[Index]));
    }
  }
  DEBUG ((EFI_D_VERBOSE, " \n"));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : PkeySlot2[32] :\n"));
  for (Index = 0; Index < 32; Index++) {
    if ((Index == 15)|| (Index == 31)) {
      DEBUG ((EFI_D_VERBOSE, " 0x%X\n", BiosGuardConfig->Bgpdt.PkeySlot2[Index]));
    } else {
      DEBUG ((EFI_D_VERBOSE, " 0x%X ,", BiosGuardConfig->Bgpdt.PkeySlot2[Index]));
    }
  }
  DEBUG ((EFI_D_VERBOSE, " \n"));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : BiosGuardModSvn : 0x%X\n", BiosGuardConfig->Bgpdt.BgModSvn));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : BiosSvn : 0x%X\n", BiosGuardConfig->Bgpdt.BiosSvn));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : ExecLim : 0x%X\n", BiosGuardConfig->Bgpdt.ExecLim));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : PlatAttr : 0x%X\n", BiosGuardConfig->Bgpdt.PlatAttr));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : EcCmd : 0x%X\n", BiosGuardConfig->Bgpdt.EcCmd));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : EcData : 0x%X\n", BiosGuardConfig->Bgpdt.EcData));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : EcCmdGetSvn : 0x%X\n", BiosGuardConfig->Bgpdt.EcCmdGetSvn));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : EcCmdOpen : 0x%X\n", BiosGuardConfig->Bgpdt.EcCmdOpen));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : EcCmdClose : 0x%X\n", BiosGuardConfig->Bgpdt.EcCmdClose));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : EcCmdPortTest : 0x%X\n", BiosGuardConfig->Bgpdt.EcCmdPortTest));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : LastSfam : 0x%X\n", BiosGuardConfig->Bgpdt.LastSfam));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BGPDT : SfamData[64] - first %2d entries:\n", BiosGuardConfig->Bgpdt.LastSfam));
  //
  // Change the array size according to MAX_SFAM_COUNT
  //
  //for (Index = 0; Index < 64; Index++) {
  for (Index = 0; Index <= BiosGuardConfig->Bgpdt.LastSfam; Index++) {
    if ((Index == 15) || (Index == 31) || (Index == 47) || (Index == 63) ) {
      DEBUG ((EFI_D_VERBOSE, " 0x%X\n", BiosGuardConfig->Bgpdt.SfamData[Index]));
    } else {
      DEBUG ((EFI_D_VERBOSE, " 0x%X ,", BiosGuardConfig->Bgpdt.SfamData[Index]));
    }
  }
  DEBUG ((EFI_D_VERBOSE, " \n"));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BgpdtHash[4] : 0x%lX , 0x%lX , 0x%lX , 0x%lX \n",   BiosGuardConfig->BgpdtHash[0], \
          BiosGuardConfig->BgpdtHash[1], \
          BiosGuardConfig->BgpdtHash[2], \
          BiosGuardConfig->BgpdtHash[3]));

  DEBUG ((EFI_D_VERBOSE, " \n"));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BiosGuardMemSize : 0x%X\n", BiosGuardConfig->BiosGuardMemSize));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: EcCmdDiscovery : 0x%X\n", BiosGuardConfig->EcCmdDiscovery));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: EcCmdProvisionEav : 0x%X\n", BiosGuardConfig->EcCmdProvisionEav));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: EcCmdLock : 0x%X\n", BiosGuardConfig->EcCmdLock));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BiosGuardLOG:: Version : 0x%X\n", BiosGuardConfig->BiosGuardLog.Version));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BiosGuardLOG:: LastPage : 0x%X\n", BiosGuardConfig->BiosGuardLog.LastPage));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BiosGuardLOG:: LoggingOptions : 0x%X\n", BiosGuardConfig->BiosGuardLog.LoggingOptions));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BiosGuardLOG:: BiosGuardModSvn : 0x%X\n", BiosGuardConfig->BiosGuardLog.BgModSvn));
  DEBUG ((EFI_D_VERBOSE, " BiosGuard:: BiosGuardLOG:: NumOfEntriesInLog : 0x%X\n", BiosGuardConfig->BiosGuardLog.NumOfEntriesInLog));

  return;
}
//#endif

/**

  Perform the platform spefific BIOS Guard initializations.

  @param **PeiServices                 - Indirect reference to the PEI Services Table.
  @param *TotalSystemConfigurationPtr  - Pointer to the Setup structure

  @retval None

**/
VOID
BiosGuardInit (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           ResetRequired;
  BIOSGUARD_HOB                     *BiosGuardHobPtr;
  UINT8                             SocketId;
  BIOSGUARD_CONFIG                  BiosGuardConfig;
  BIOSGUARD_CONFIG                  *BiosGuardConfigPtr;
  EFI_PLATFORM_INFO                 *PlatformInfoHob;
  EFI_HOB_GUID_TYPE                 *GuidHob;
#ifdef BIOSGUARD_EC_FLAG
  volatile UINT32                   EphemeralAuthValue;
  UINT8                             EcStatus;
  PEI_STALL_PPI                     *StallPpi;
  PEI_CPU_IO_PPI                    *CpuIo;
#endif  //BIOSGUARD_EC_FLAG
  EFI_PEI_RESET_PPI                 *ResetPpi;
  UINT64_STRUCT                     MsrReg;
  EFI_BOOT_MODE                     BootMode=0;
  UINT32                            PlatformFeatureFlag = 0;
  ICX_MSR_PLATFORM_INFO_REGISTER    PlatformInfoMsr;
  MSR_PLAT_FRMW_PROT_CTRL_REGISTER  PlatFrmwCtrlMsr;

  DEBUG ((EFI_D_INFO, "BiosGuardInit\n"));

  BiosGuardConfigPtr = &BiosGuardConfig;
  PlatformFeatureFlag = PcdGet32(PcdOemSkuPlatformFeatureFlag);
  
  //
  // Locate Platform Info Hob
  //
  GuidHob          = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT(GuidHob != NULL);
  if (GuidHob == NULL) {
    return;
  }
  PlatformInfoHob  = GET_GUID_HOB_DATA(GuidHob);

  //
  // Write WPE bit on S3 Resume in BIOS Info Flags MSR to renable Anti-Flash wearout protection
  //
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if ((BootMode == BOOT_ON_S3_RESUME) && (TotalSystemConfigurationPtr->EnableAntiFlashWearout) &&
        (TotalSystemConfigurationPtr->AntiFlashWearoutSupported == TRUE)) {
    RestoreAntiFlashWearout();
  }


  ResetRequired = FALSE;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
  if (PlatformInfoMsr.Bits.PfatEnable == 0) {
    DEBUG ((EFI_D_INFO, "BIOS Guard Feature is not supported\n"));
    return;
  }

  //
  // Need to make sure that only supported Platforms can enable BIOS Guard
  //
  if ((PlatformFeatureFlag & BIOSGUARD_SUPPORT_ENABLED) == 0) {
    TotalSystemConfigurationPtr->BiosGuardEnabled = FALSE;
    DEBUG ((EFI_D_INFO, "Platform ID check failed. BiosGuardInit skipped.\n"));
  }

  if (TotalSystemConfigurationPtr->BiosGuardEnabled) {
    DEBUG ((EFI_D_INFO, "BIOS Guard Module is Enabled\n"));

    InitializeBiosGuardConfigData(BiosGuardConfigPtr);
    CustomizeBiosGuardConfigData(PeiServices, BiosGuardConfigPtr, TotalSystemConfigurationPtr->EnableAntiFlashWearout);
    PrintBiosGuardInfo(BiosGuardConfigPtr);

    //
    // Save BIOS Guard Mem Size in the Platform Info Hob
    //
    PlatformInfoHob->MemData.BiosGuardMemSize  = BiosGuardConfigPtr->BiosGuardMemSize;

    ///
    /// Read BIOS Guard Control Register
    ///
    PlatFrmwCtrlMsr.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (PlatFrmwCtrlMsr.Bits.PfatLock) {
      if (!(PlatFrmwCtrlMsr.Bits.PfatEnable)) {
        ///
        /// Reset required as the BIOS Guard CTRL MSR is locked and needs to be toggled
        ///
        ResetRequired = TRUE;
      }
    } else {
#ifdef BIOSGUARD_EC_FLAG
      if ((BiosGuardConfig->Bgpdt.PlatAttr & EC_PRESENT) && (BiosGuardConfig->Bgpdt.PlatAttr & EC_BIOSGUARD_PROTECTED)) {
        DEBUG ((EFI_D_INFO, "EC is Present and EC FW supports BIOS Guard\n"));
        CpuIo  = (**PeiServices).CpuIo;
        Status = (*PeiServices)->LocatePpi (PeiServices, &gPeiStallPpiGuid, 0, NULL, &StallPpi);
        ASSERT_PEI_ERROR (PeiServices, Status);
        Status = SendKscCommand (PeiServices, CpuIo, StallPpi, BiosGuardConfigPtr->EcCmdProvisionEav);
        if (Status == EFI_SUCCESS) {
          EphemeralAuthValue = RandomNumber ();
          for (i = 0; (i < (sizeof (EphemeralAuthValue))); i++) {
            Status = SendKscData (PeiServices, CpuIo, StallPpi, (UINT8) ((EphemeralAuthValue >> (i * 8)) & 0xFF));
          }
          Status = ReceiveKscData (PeiServices, CpuIo, StallPpi, &EcStatus);
          if (EcStatus != 0) {
            ResetRequired = TRUE;
          }
          AsmWriteMsr32 (MSR_PFAT_EC_PASSWORD, EphemeralAuthValue);
          EphemeralAuthValue = 0;
          Status = SendKscCommand (PeiServices, CpuIo, StallPpi, BiosGuardConfigPtr->EcCmdLock);
        }
      }
#endif //BIOSGUARD_EC_FLAG
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_0, BiosGuardConfigPtr->BgpdtHash[0]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_1, BiosGuardConfigPtr->BgpdtHash[1]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_2, BiosGuardConfigPtr->BgpdtHash[2]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_3, BiosGuardConfigPtr->BgpdtHash[3]);
      PlatFrmwCtrlMsr.Bits.PfatLock = 1;
      PlatFrmwCtrlMsr.Bits.PfatEnable = 1;

      //
      // Check to see if we need to set the Dual socket bit
      //
      if (KTI_HOST_OUT_PTR->numCpus > 1) {
        PlatFrmwCtrlMsr.Bits.PfatDp = 1;
      }

      MsrReg = *((UINT64_STRUCT *) &PlatFrmwCtrlMsr.Uint64);

      for(SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
        if (SocketPresent (SocketId)) {
          WriteMsrPipeHL(SocketId, MSR_PLAT_FRMW_PROT_CTRL, MsrReg);
        }
      }
    }
    //
    // Create BIOS Guard HOB
    //
    if (!ResetRequired) {
      Status = (*PeiServices)->CreateHob (PeiServices, EFI_HOB_TYPE_GUID_EXTENSION, sizeof (BIOSGUARD_HOB), (VOID **) &BiosGuardHobPtr);
      BiosGuardHobPtr->EfiHobGuidType.Name = gBiosGuardHobGuid;
      CopyMem (&BiosGuardHobPtr->Bgpdt, &BiosGuardConfigPtr->Bgpdt, BiosGuardConfigPtr->Bgpdt.BgpdtSize);
      CopyMem (&BiosGuardHobPtr->BgupHeader, &BiosGuardConfigPtr->BgupHeader, sizeof (BGUP_HEADER));
      CopyMem (&BiosGuardHobPtr->BiosGuardLog, &BiosGuardConfigPtr->BiosGuardLog, sizeof (BIOSGUARD_LOG));
      BiosGuardHobPtr->TotalFlashSize = (UINT16) RShiftU64(BiosGuardConfigPtr->TotalFlashSize, 10);
      BiosGuardHobPtr->BiosSize = (UINT16) RShiftU64(FLASH_SIZE, 10);
      BiosGuardHobPtr->BiosGuardModulePtr = BiosGuardConfigPtr->BiosGuardModulePtr;
    }
  } else {
    DEBUG ((EFI_D_INFO, "BIOS Guard Module is Disabled\n"));
    PlatFrmwCtrlMsr.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (PlatFrmwCtrlMsr.Bits.PfatLock) {
      if (PlatFrmwCtrlMsr.Bits.PfatEnable) {
        ///
        /// Reset required as the BIOS Guard CTRL MSR is locked and needs to be toggled
        ///
        ResetRequired = TRUE;
      }
    } else {
      PlatFrmwCtrlMsr.Bits.PfatEnable = 0;
      PlatFrmwCtrlMsr.Bits.PfatLock = 1;

      MsrReg = *((UINT64_STRUCT *) &PlatFrmwCtrlMsr.Uint64);

      for(SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
        if (SocketPresent (SocketId)) {
          WriteMsrPipeHL(SocketId, MSR_PLAT_FRMW_PROT_CTRL, MsrReg);
        }
      }
    }
  }

  if (ResetRequired) {
    ResetPpi = NULL;
    Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gEfiPeiResetPpiGuid,
                    0,
                    NULL,
                    &ResetPpi
                    );
    ASSERT_EFI_ERROR (Status);
    ASSERT (ResetPpi != NULL);
    if (Status != EFI_SUCCESS) {
      goto err_exit;
    }
    if (ResetPpi == NULL) {
      goto err_exit;
    }

    ///
    /// Perform Cold Reset
    ///
    DEBUG((EFI_D_INFO, "Reset Required. Performing Cold Reset to unlock BIOS Guard CONTROL MSR\n"));
    ResetPpi->ResetSystem (PeiServices);
  }

err_exit:
  return;
}
