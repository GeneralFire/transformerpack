/** @file
  Code File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include "CpuPpmIncludes.h"
#include <Library/PcuMailBoxLib.h>
#include <Library/BaseMemoryLib.h>
#include <ProcessorPpmSetup.h>


typedef struct _LOGICAL_IP_TYPE_TABLE {
  UINT8           LogicalIpTypeMin;
  UINT8           LogicalIpTypeMax;
  CHAR16          *IpTypeString;
} LOGICAL_IP_TYPE_TABLE;

static LOGICAL_IP_TYPE_TABLE mLogicalIpTypeTable[] = {
  //LogicalIpTypeMin    LogicalIpTypeMax               IpTypeString
  {IP_TYPE_KTI,       IP_TYPE_KTI,                   (CHAR16 *)L" KTI "    },
  {IP_TYPE_RLINK,     IP_TYPE_RLINK,                 (CHAR16 *)L" RLINK "  },
  {IP_TYPE_FXR,       IP_TYPE_FXR,                   (CHAR16 *)L" FXR "    },
  {IP_TYPE_MCDDR,     IP_TYPE_MCDDR_MAX,             (CHAR16 *)L" MCDDR "  },
  {IP_TYPE_IIO,       IP_TYPE_IIO_MAX,               (CHAR16 *)L" IIO "    },
  {IP_TYPE_HQM,       IP_TYPE_HQM,                   (CHAR16 *)L" HQM "    },
  {IP_TYPE_NAC,       IP_TYPE_NAC,                   (CHAR16 *)L" NAC "    },
  {IP_TYPE_TIP,       IP_TYPE_TIP,                   (CHAR16 *)L" TIP "    },
  {IP_TYPE_MDFS,      IP_TYPE_MDFS,                  (CHAR16 *)L" MDFS "   },
  {IP_TYPE_HCX,       IP_TYPE_HCX,                   (CHAR16 *)L" HCX "    },
  {IP_TYPE_DINO,      IP_TYPE_DINO,                  (CHAR16 *)L" DINO "   },
  {IP_TYPE_HBM,       IP_TYPE_HBM_MAX,               (CHAR16 *)L" HBM "    },
};

UINT32  mPcodeResultTable[ARRAY_SIZE(mLogicalIpTypeTable)];


/**
  Get the Index value of LogicalIpTypeTable according to input parameter(IP type).

  @param      LogicalIpType     IP type value

  @retval     The index of LogicalIpTypeTable, 0xff means the invalid index
**/
static UINT8
GetIndexOfLogicalIpTypeTable (
  IN     UINT8  LogicalIpType
  )
{
  UINT8 Index;

  for (Index = 0; Index < ARRAY_SIZE(mLogicalIpTypeTable); Index++) {
    if ((mLogicalIpTypeTable[Index].LogicalIpTypeMin <= LogicalIpType) && (LogicalIpType <= mLogicalIpTypeTable[Index].LogicalIpTypeMax)) {
      return Index;
    }
  }
  return 0xff;
}

/**

  This routine is called to program B2P WRITE_PKGC_SA_PS_CRITERIA directly.

  @param SocketNumber          Socket index that is executing.
  @param LogicalIpType         Logical IP Type value.
  @param MailboxCommand        MailBox Command.
  @param MailboxData           MailBox Data.

  @retval                      MailBox value.

**/
UINT32
AcknowledgeB2PPkgcSaPsCriteria (
  UINT8                       SocketNumber,
  UINT8                       LogicalIpType,
  UINT32                      MailboxCommand,
  UINT32                      MailboxData
  )
{
  UINT32  Tempdata = 0;
  UINT8   Index = 0xff;

  if (MailboxData == 0) {
    return Tempdata;
  }


  Tempdata = ReadModifyWritePcuMailbox (
               SocketNumber,
               0,
               MailboxCommand,
               &MailboxData,
               0,
               0
               );
  PpmS3BootScriptSaveB2PWrite (SocketNumber, MailboxCommand, MailboxData);
  DEBUG ((EFI_D_INFO, "\n:PPM: Write B2P WRITE_PKGC_SA_PS_CRITERIA to"));

  Index = GetIndexOfLogicalIpTypeTable (LogicalIpType);

  if (Index != 0xff && (Index < ARRAY_SIZE(mLogicalIpTypeTable))) {
    mPcodeResultTable[Index] = Tempdata;
    DEBUG ((EFI_D_INFO, "%s", mLogicalIpTypeTable[Index].IpTypeString));
  }

  DEBUG ((EFI_D_INFO, " - Index: %d   0x%x as PM_MBX_CMD = 0x%x, PM_MBX_DATA = 0x%x on S%d\n", Index, LogicalIpType, MailboxCommand, MailboxData, SocketNumber));

  return Tempdata;
}

VOID
ReportB2PPkgcSaPsCriteriaResult (
  UINT8   SocketNumber,
  UINT32  PcodeResult
  )
{
  UINT8   Index;

  //
  // Check mPcodeResultTable
  //
  for (Index = 0; Index < ARRAY_SIZE(mLogicalIpTypeTable); Index++) {
    if (mPcodeResultTable[Index] > 0) {

      //
      // Report Pcode Error
      //
      DEBUG ((EFI_D_ERROR, "\n:PPM: Invalid B2P WRITE_PKGC_SA_PS_CRITERIA access to%s", mLogicalIpTypeTable[Index].IpTypeString));

      DEBUG ((EFI_D_ERROR, "!!! Error code 0x%x on #S%d.\n", mPcodeResultTable[Index], SocketNumber));
    }
  }

}


/**

  This routine is called to program B2P WRITE_PKGC_SA_PS_CRITERIA. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param PkgcCriteria          Pointer to PKGC_SA_PS_CRITERIA_STRUCT instance.

  @retval                      None

**/
VOID
EFIAPI
InitializePkgcSaPsCriteria (
  UINT8                       SocketNumber,
  PKGC_SA_PS_CRITERIA_STRUCT  *PkgcCriteria
  )
{
  B2P_WRITE_PKGC_SA_PS_CRITERIA_INTERFACE_STRUCT  PkgcCriteriaInterfaceB2p;
  B2P_WRITE_PKGC_SA_PS_CRITERIA_DATA_STRUCT       PkgcCriteriaDataB2p;
  UINT32                                          PcodeResult;

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return;
  }


  PkgcCriteriaInterfaceB2p.Data = 0;
  PkgcCriteriaDataB2p.Data = 0;
  PcodeResult = 0;

  //
  //initialize mPcodeResultTable
  //
  ZeroMem (&mPcodeResultTable, sizeof(mPcodeResultTable));

  if (PkgcCriteria->EnablePkgcCriteria) {

    PkgcCriteriaInterfaceB2p.Bits.command         = (UINT32) MAILBOX_BIOS_CMD_WRITE_PKGC_SA_PS_CRITERIA;

    //
    // Logical_ip_type 0h: KTI
    //
    if (PkgcCriteria->EnablePkgCCriteriaKti[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_KTI;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoKti[SocketNumber];

      DEBUG ((EFI_D_INFO, "\n:PPM: logical_ip_type: 0x%x  instance_type: %d\n", PkgcCriteriaInterfaceB2p.Bits.logical_ip_type, PkgcCriteriaInterfaceB2p.Bits.instance_type));

      if (PkgcCriteria->EnableLinkInL1Kti > 0) {
        if ((IsCpuAndRevision (CPU_ICXSP, REV_ALL))) {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = KTI_ICXSP_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = KTI_ICXSP_CRITERIA_PS_MASK;
        } else {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = KTI_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = KTI_CRITERIA_PS_MASK;
        }

        DEBUG ((EFI_D_INFO, "\n:PPM: Write [KTI] B2P [7:0] bios_pkgc_allowed_ps_mask: 0x%x  [15:8] bios_pkgc_criteria_ps_mask: 0x%x\n",
               PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask, PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask));

        PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );

      }
    }

    //
    // Logical_ip_type 1h: Rlink
    //
    if (PkgcCriteria->EnablePkgCCriteriaRlink[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_RLINK;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoRlink[SocketNumber];

      if (PkgcCriteria->EnableLinkInL1Rlink > 0) {
        if ((IsCpuAndRevision (CPU_ICXSP, REV_ALL))) {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = RLINK_ICXSP_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = RLINK_ICXSP_CRITERIA_PS_MASK;
        } else {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = RLINK_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = RLINK_CRITERIA_PS_MASK;
        }

        PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
      }
    }

    //
    // Logical_ip_type 2h: FXR
    //
    if (PkgcCriteria->EnablePkgCCriteriaFxr[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_FXR;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoFxr[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = (UINT32) PkgcCriteria->PkgCCriteriaAllowedPsMaskFxr[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = (UINT32) PkgcCriteria->PkgcCriteraPsMaskFxr[SocketNumber];

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
     }

    //
    // Logical_ip_type 10h-17h: MCDDR0, 1....
    //
    if (PkgcCriteria->EnablePkgCCriteriaMcddr[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) PkgcCriteria->PkgCCriteriaLogicalIpTypeMcddr[SocketNumber];
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoMcddr[SocketNumber];
      PkgcCriteriaDataB2p.Data = 0;

      DEBUG ((EFI_D_INFO, "\n:PPM: logical_ip_type: 0x%x  instance_type: %d\n", PkgcCriteriaInterfaceB2p.Bits.logical_ip_type, PkgcCriteriaInterfaceB2p.Bits.instance_type));

      // Default setting

      if ((IsCpuAndRevision (CPU_ICXSP, REV_R0)) && (PkgcCriteria->EnablePkgcCriteria == PPM_AUTO) && (PkgcCriteria->EnablePkgCCriteriaMcddr[SocketNumber] == PPM_AUTO)) {
        PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = MCDDR_ICXSP_R0_WA_ALLOWED_PS_MASK;
        PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = MCDDR_ICXSP_R0_WA_CRITERIA_PS_MASK;
      }

      switch (PkgcCriteria->PkgcCriteriaPsOptionMcddr[SocketNumber]) {
        case OPTION_1:
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = OPTION_1_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = OPTION_1_CRITERIA_PS_MASK;
          break;
        case OPTION_2:
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = OPTION_2_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = OPTION_2_CRITERIA_PS_MASK;
          break;
        case OPTION_3:
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = OPTION_3_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = OPTION_3_CRITERIA_PS_MASK;
          break;
        default:
          break;
      }

      DEBUG ((EFI_D_INFO, "\n:PPM: Write [MCDDR] B2P [7:0] bios_pkgc_allowed_ps_mask: 0x%x  [15:8] bios_pkgc_criteria_ps_mask: 0x%x\n",
               PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask, PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask));

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
    }

    //
    // Logical_ip_type 18h-1Fh: HBM0, 1....
    //
    if (PkgcCriteria->EnablePkgCCriteriaHbm[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) PkgcCriteria->PkgCCriteriaLogicalIpTypeHbm[SocketNumber];
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoHbm[SocketNumber];
      PkgcCriteriaDataB2p.Data = 0;

      DEBUG ((EFI_D_INFO, "\n:PPM: logical_ip_type: 0x%x  instance_type: %d\n", PkgcCriteriaInterfaceB2p.Bits.logical_ip_type, PkgcCriteriaInterfaceB2p.Bits.instance_type));

      switch (PkgcCriteria->PkgcCriteriaPsOptionHbm[SocketNumber]) {
        case OPTION_1:
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = OPTION_1_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = OPTION_1_CRITERIA_PS_MASK;
          break;
        case OPTION_2:
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = OPTION_2_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = OPTION_2_CRITERIA_PS_MASK;
          break;
        case OPTION_3:
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = OPTION_3_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = OPTION_3_CRITERIA_PS_MASK;
          break;
        default:
          break;
      }

      DEBUG ((EFI_D_INFO, "\n:PPM: Write [HBM] B2P [7:0] bios_pkgc_allowed_ps_mask: 0x%x  [15:8] bios_pkgc_criteria_ps_mask: 0x%x\n",
               PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask, PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask));

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
    }

    //
    // Logical_ip_type 20h-27h: IIO0, 1....
    //
    if (PkgcCriteria->EnablePkgCCriteriaIio[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) PkgcCriteria->PkgCCriteriaLogicalIpTypeIio[SocketNumber];
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoIio[SocketNumber];

      DEBUG ((EFI_D_INFO, "\n:PPM: logical_ip_type: 0x%x  instance_type: %d\n", PkgcCriteriaInterfaceB2p.Bits.logical_ip_type, PkgcCriteriaInterfaceB2p.Bits.instance_type));

      if (PkgcCriteria->EnableLinkInL1Iio > 0) {
        if ((IsCpuAndRevision (CPU_ICXSP, REV_ALL))) {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = IIO_ICXSP_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = IIO_ICXSP_CRITERIA_PS_MASK;
        } else {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = IIO_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = IIO_CRITERIA_PS_MASK;
        }

        DEBUG ((EFI_D_INFO, "\n:PPM: Write [IIO] B2P [7:0] bios_pkgc_allowed_ps_mask: 0x%x  [15:8] bios_pkgc_criteria_ps_mask: 0x%x\n",
               PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask, PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask));

        PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );

      }
    }

    //
    // Logical_ip_type 30h: HQM
    //
    if (PkgcCriteria->EnablePkgCCriteriaHqm[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_HQM;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoHqm[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = (UINT32) PkgcCriteria->PkgCCriteriaAllowedPsMaskHqm[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = (UINT32) PkgcCriteria->PkgcCriteraPsMaskHqm[SocketNumber];

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
    }

    //
    // Logical_ip_type 31h: NAC
    //
    if (PkgcCriteria->EnablePkgCCriteriaNac[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_NAC;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoNac[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = (UINT32) PkgcCriteria->PkgCCriteriaAllowedPsMaskNac[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = (UINT32) PkgcCriteria->PkgcCriteraPsMaskNac[SocketNumber];

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
    }

    //
    // Logical_ip_type 32h: TIP
    //
    if (PkgcCriteria->EnablePkgCCriteriaTip[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_TIP;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoTip[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = (UINT32) PkgcCriteria->PkgCCriteriaAllowedPsMaskTip[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = (UINT32) PkgcCriteria->PkgcCriteraPsMaskTip[SocketNumber];

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
    }

    //
    // Logical_ip_type 33h: MDFS
    //
    if (PkgcCriteria->EnablePkgCCriteriaMdfs[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_MDFS;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoMdfs[SocketNumber];

      DEBUG ((EFI_D_INFO, "\n:PPM: logical_ip_type: 0x%x  instance_type: %d\n", PkgcCriteriaInterfaceB2p.Bits.logical_ip_type, PkgcCriteriaInterfaceB2p.Bits.instance_type));

      if (PkgcCriteria->AllowLpStateMdfs > 0) {
        if ((IsCpuAndRevision (CPU_ICXSP, REV_ALL))) {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = MDFS_ICXSP_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = MDFS_ICXSP_CRITERIA_PS_MASK;
        } else {
          PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = MDFS_ALLOWED_PS_MASK;
          PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = MDFS_CRITERIA_PS_MASK;
        }

        DEBUG ((EFI_D_INFO, "\n:PPM: Write [MDFS] B2P [7:0] bios_pkgc_allowed_ps_mask: 0x%x  [15:8] bios_pkgc_criteria_ps_mask: 0x%x\n",
               PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask, PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask));

        PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );

      }
    }

    //
    // Logical_ip_type 34h: HCX
    //

    if (PkgcCriteria->EnablePkgCCriteriaHcx[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_HCX;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoHcx[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = (UINT32) PkgcCriteria->PkgCCriteriaAllowedPsMaskHcx[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = (UINT32) PkgcCriteria->PkgcCriteraPsMaskHcx[SocketNumber];

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
    }

    //
    // Logical_ip_type 35h: DINO
    //

    if (PkgcCriteria->EnablePkgCCriteriaDino[SocketNumber] > 0) {
      PkgcCriteriaInterfaceB2p.Bits.logical_ip_type = (UINT32) IP_TYPE_DINO;
      PkgcCriteriaInterfaceB2p.Bits.instance_type = (UINT32) PkgcCriteria->PkgCCriteriaInstanceNoDino[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_allowed_ps_mask = (UINT32) PkgcCriteria->PkgCCriteriaAllowedPsMaskDino[SocketNumber];
      PkgcCriteriaDataB2p.Bits.bios_pkgc_criteria_ps_mask = (UINT32) PkgcCriteria->PkgcCriteraPsMaskDino[SocketNumber];

      PcodeResult |= AcknowledgeB2PPkgcSaPsCriteria (
                       SocketNumber,
                       (UINT8) PkgcCriteriaInterfaceB2p.Bits.logical_ip_type,
                       PkgcCriteriaInterfaceB2p.Data,
                       PkgcCriteriaDataB2p.Data
                     );
    }
    //
    // Report B2P Error Code
    //
    ReportB2PPkgcSaPsCriteriaResult (SocketNumber, PcodeResult);
  }
}
