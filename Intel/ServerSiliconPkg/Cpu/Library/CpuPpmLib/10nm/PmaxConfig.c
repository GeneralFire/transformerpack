/** @file
  Code File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "Common/CpuPpmIncludes.h"
#include <Library/PcuMailBoxLib.h>
#include <Library/CpuAndRevisionLib.h>

PACKAGE_TDP_PMAX_OFFSET             *mPMAXOffsetvsTDPTable;

PACKAGE_TDP_PMAX_OFFSET  mPMAXOffsetvsTDPTableIcx[] = {
  // TDP (W)       MSR Value (2mV DAC Offset in a 0.6mOHM board design)
  {   95        ,     16 },
  {  105        ,     21 },
  {  120        ,     25 },
  {  135        ,     28 },
  {  150        ,     31 },
  {  165        ,     33 },
  {  185        ,     38 },
  {  205        ,     0 },
  {  220        ,     0 },
  {  230        ,     0 },
  {  250        ,     0 },
  {  270        ,     0 },
  {  300        ,     0 },
  {END_OF_TABLE,END_OF_TABLE}
};

PACKAGE_TDP_PMAX_OFFSET  mPMAXOffsetvsTDPTableSpr[] = {
  // TDP (W)       MSR Value (2mV DAC Offset in a 0.5mOHM board design)
  {   85        ,     14 },
  {  105        ,     19 },
  {  125        ,     24 },
  {  135        ,     27 },
  {  145        ,     29 },
  {  155        ,     31 },
  {  165        ,     34 },
  {  185        ,     39 },
  {  205        ,     44 },
  {  225        ,     0 },
  {  250        ,     0 },
  {  270        ,     0 },
  {  300        ,     0 },
  {END_OF_TABLE,END_OF_TABLE}
};

/**

  This routine is called to get MAILBOX_BIOS_CMD_VR_HANDLER.GET_ACDC_LOADLINE.

  @param SocketNumber          Socket index that is executing.

  @retval                      TRUE-- Get the value successfully; FALSE -- Failed to get the value.

**/
BOOLEAN
GetB2pAcDcLoadLine (
  UINT8                 SocketNumber
  )
{
  B2P_VR_HANDLER_SUBCOMMAND_STRUCT        VrHandlerCmdB2p;
  UINT32                                  MailBoxData = 0;
  UINT32                                  Tempdata = 0;
  BOOLEAN                                 UsePmaxTableFlag;

  VrHandlerCmdB2p.Data = 0;
  UsePmaxTableFlag = TRUE;

  VrHandlerCmdB2p.Bits.Command = MAILBOX_BIOS_CMD_SVID_VR_HANDLER;
  VrHandlerCmdB2p.Bits.SubCommand = MAILBOX_BIOS_CMD_SVID_VR_HANDLER_INTERFACE_SUB_COMMAND_GET_ACDC_LOADLINE;

  DEBUG((EFI_D_INFO, " \n: PPM: Send B2P VR_HANDLER.GET_ACDC_LOADLINE (0x%x), Value = %d in S#%d\n", VrHandlerCmdB2p.Data, MailBoxData, SocketNumber));
  Tempdata = ReadModifyWritePcuMailbox (
                 SocketNumber,
                 VrHandlerCmdB2p.Data,
                 MAILBOX_BIOS_CMD_ZERO,
                 &MailBoxData,
                 0,
                 0
               );

  if (Tempdata == 0) {
    DEBUG ((EFI_D_INFO, " Get B2P VR_HANDLER.GET_ACDC_LOADLINE on socket (%d) = 0x%x\n", SocketNumber, MailBoxData));
    if (((MailBoxData / 100) * 100) != PMAX_LOAD_LINE_0P50_VALUE) {
      UsePmaxTableFlag = FALSE;

      if (((MailBoxData / 100) * 100) != PMAX_LOAD_LINE_0P85_VALUE) {
        DEBUG((EFI_D_ERROR, " \n:PPM: Get B2P VR_HANDLER.GET_ACDC_LOADLINE (0x%x), Value = %d in S#%d is NOT in the range!!!\n", VrHandlerCmdB2p.Data, MailBoxData, SocketNumber));
      }
    }
  }

  return UsePmaxTableFlag;
}

/**

  This routine is called to get MAILBOX_BIOS_CMD_VR_HANDLER.SET_ACDC_LOADLINE.

  @param SocketNumber          Socket index that is executing.
  @param PmaxConfig            Pointer to PMAX_CONFIG instance.

  @retval                      None

**/
VOID
SetB2pAcDcLoadLine (
  UINT8               SocketNumber,
  PMAX_CONFIG         *PmaxConfig
  )
{
  B2P_VR_HANDLER_SUBCOMMAND_STRUCT        VrHandlerCmdB2p;
  UINT32                                  MailBoxData = 0;
  UINT32                                  Tempdata = 0;

  if (PmaxConfig->PmaxLoadLine == 0) {
    return;
  }

  VrHandlerCmdB2p.Data = 0;

  VrHandlerCmdB2p.Bits.Command = MAILBOX_BIOS_CMD_SVID_VR_HANDLER;
  VrHandlerCmdB2p.Bits.SubCommand = MAILBOX_BIOS_CMD_SVID_VR_HANDLER_INTERFACE_SUB_COMMAND_SET_ACDC_LOADLINE;


  switch(PmaxConfig->PmaxLoadLine) {
    case 1:
        MailBoxData = (UINT32) PMAX_LOAD_LINE_0P50_VALUE;  // 0.50mohm platform LL
        break;

    case 2:
    default:
        MailBoxData = (UINT32) PMAX_LOAD_LINE_0P85_VALUE;  // 0.85mohm platform LL
        break;
  }

  DEBUG((EFI_D_INFO, " \n: PPM: Send B2P VR_HANDLER.SET_ACDC_LOADLINE (0x%x), Value = %d in S#%d\n", VrHandlerCmdB2p.Data, MailBoxData, SocketNumber));
  Tempdata = ReadModifyWritePcuMailbox (
                 SocketNumber,
                 MAILBOX_BIOS_CMD_ZERO,
                 VrHandlerCmdB2p.Data,
                 &MailBoxData,
                 0,
                 0
                 );

  if (Tempdata != 0) {
    DEBUG ((EFI_D_ERROR, " \n\n :: !!! BIOS failed to send VR_HANDLER.SET_ACDC_LOADLINE on socket (%d). Pcode returns (%d) !!!. \n\n", SocketNumber, Tempdata));
  }
}

/**

  This routine is called to PMax Offset from the table based on each socket TDP value.

  @param SocketNumber          Socket index that is executing.
  @param ppm                   Pointer to EFI_CPU_PM_STRUCT instance.
  @param PmaxConfig            Pointer to PMAX_CONFIG instance.

  @retval                      PMax Offset

**/
UINT32
GetFromPMaxOffsetTable (
  UINT8               SocketNumber,
  EFI_CPU_PM_STRUCT   *ppm,
  PMAX_CONFIG         *PmaxConfig
  )
{
  UINT16                                Index;
  UINT32                                data;
  UINT32                                GetValue;
  BOOLEAN                               FoundinTBL;


  GetValue = 0;
  FoundinTBL = FALSE;

  mPMAXOffsetvsTDPTable = mPMAXOffsetvsTDPTableIcx;
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    mPMAXOffsetvsTDPTable = mPMAXOffsetvsTDPTableSpr;
  }

  data = (UINT32) PmaxConfig->BasePackageTdp[SocketNumber];

  for (Index = 0; mPMAXOffsetvsTDPTable[Index].PackageTDP != END_OF_TABLE; Index++) {

    if (data == mPMAXOffsetvsTDPTable[Index].PackageTDP) {
      FoundinTBL = TRUE;
      GetValue = mPMAXOffsetvsTDPTable[Index].PmaxOffsetValue;
      break;
    }

    if (Index > 0) {
      if ((data > (UINT32) mPMAXOffsetvsTDPTable[Index - 1].PackageTDP) && (data < (UINT32) mPMAXOffsetvsTDPTable[Index].PackageTDP)) {
        FoundinTBL = TRUE;
        GetValue = mPMAXOffsetvsTDPTable[Index - 1].PmaxOffsetValue;
        DEBUG ( (EFI_D_INFO, "\nNOTE: Found TDP [%d] closer value in PMaxOffset table %d: %d, %d on socket%d!!!\n\n", data, Index, mPMAXOffsetvsTDPTable[Index - 1].PackageTDP, GetValue, SocketNumber) );
        break;
      }
    }
  }

  if ((!FoundinTBL) && (((PmaxConfig->PmaxSign == 0) && (PmaxConfig->PmaxOffset > 0)) || ((PmaxConfig->PmaxSign == 1) && (PmaxConfig->PmaxOffsetNegative > 0)))) {
    DEBUG ((EFI_D_ERROR, " \n!!!Did not find the TDP [%d] in PMaxOffset table, forced Register value as %d on socket%d!!!\n\n", data, GetValue, SocketNumber) );
  }

  DEBUG ((EFI_D_INFO, "PMAX Offset: Socket: %d :: Pmax Package TDP value: %dW, Table Offset Correction: %d\n", SocketNumber, data, GetValue));

  return GetValue;
}

/**

  This routine is called to program PERF_P_LIMIT_CONTROL. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param ppm                   Pointer to EFI_CPU_PM_STRUCT instance.
  @param PmaxConfig            Pointer to PMAX_CONFIG instance.

  @retval                      None

**/
VOID
EFIAPI
InitializePmaxConfiguration (
  UINT8               SocketNumber,
  EFI_CPU_PM_STRUCT   *ppm,
  PMAX_CONFIG         *PmaxConfig
  )
{
  UINT8                                   Negative;
  UINT64                                  RegAddr;
  UINT32                                  Tempdata = 0;
  UINT32                                  TempOffset = 0;
  UINT32                                  TempOffsetNegative = 0;
  B2P_STANDARD_INTERFACE_STRUCT           PmaxConfigCmdB2p;
  B2P_PMAX_CONFIG_DATA_STRUCT             PmaxConfigDataB2p;
  PMAX_GPIO_TRIGGER_PCU_FUN0_STRUCT       PmaxGpioTriggerCsr;
  B2P_PCU_MISC_CONFIG_DATA_STRUCT         PcuMiscConfigB2pOrData;
  UINT32                                  PmaxOffsetLimit = 0;

  if (PmaxConfig->PmaxDetector != 1) {
    DEBUG ((EFI_D_INFO, " [PMAX] detector is disabled.\n"));
    return ;
  }

  RegAddr = 0;
  PmaxConfigCmdB2p.Data = 0;
  PmaxConfigDataB2p.Data = 0;
  Tempdata = 0;

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (PmaxConfig->EnhancedPmaxDetector == 0) {
      PcuMiscConfigB2pOrData.Data = 0;
      PcuMiscConfigB2pOrData.Bits.DisableEnhancedPmax = 1;

      ReadModifyWritePcuMailbox (
          SocketNumber,
          MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
          MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
          &Tempdata,
          (UINT32)-1,
          PcuMiscConfigB2pOrData.Data
          );
    }
  }

  //
  // Initialize local variables with policy inputs
  //
  Negative           = PmaxConfig->PmaxSign;
  TempOffset         = (UINT32) PmaxConfig->PmaxOffset;
  TempOffsetNegative = (UINT32) PmaxConfig->PmaxOffsetNegative;

  if (PmaxConfig->PmaxAutoAdjustment == 1) {
    //
    // SNR and ICX-D are expected to have only one LL so the tables do not apply for them.
    //
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      PmaxConfigDataB2p.Bits.offset = (UINT32) GetFromPMaxOffsetTable (SocketNumber, ppm, PmaxConfig);
    }
  }

  PmaxOffsetLimit = PMAX_OFFSET_MAX_LIMIT_10nm;

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    PmaxOffsetLimit = PMAX_OFFSET_MAX_LIMIT_10nm_SPR;

    if ((PmaxConfig->PmaxLoadLine == PMAX_LOAD_LINE_0P50) || (!PmaxConfig->PmaxLoadLine && GetB2pAcDcLoadLine (SocketNumber))) {
      PmaxConfigDataB2p.Bits.offset = (UINT32) GetFromPMaxOffsetTable (SocketNumber, ppm, PmaxConfig);
    }
    SetB2pAcDcLoadLine (SocketNumber, PmaxConfig);
  }

  //
  // Calculate Pmax magnitude, Bits [5:0]
  // If we are adding positive value, make sure to cap at PmaxOffsetLimit
  // Bit 6 declares magnitude is negative or positive.
  //

  if (Negative == 1) {
    if (PmaxConfigDataB2p.Bits.offset >= TempOffsetNegative) {
      TempOffsetNegative = PmaxConfigDataB2p.Bits.offset - TempOffsetNegative;
    } else {
      TempOffsetNegative -= PmaxConfigDataB2p.Bits.offset;
    }
    TempOffsetNegative |= (1 << 6);
  } else {
    if ((TempOffset + PmaxConfigDataB2p.Bits.offset) > PmaxOffsetLimit) {
      TempOffset = PmaxOffsetLimit;
    } else {
      TempOffset += PmaxConfigDataB2p.Bits.offset;
    }
  }

  //
  // PMAX Config Register field is only 7 bits, need to mask
  //
  PmaxConfigDataB2p.Bits.offset = ((Negative == 0) ? TempOffset : TempOffsetNegative);
  PmaxConfigDataB2p.Bits.lock = 1;

  PmaxConfigCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_PMAX_CONFIG;
  PmaxConfigCmdB2p.Bits.read_write = 1;
  Tempdata = ReadModifyWritePcuMailbox (
               SocketNumber,
               0,
               PmaxConfigCmdB2p.Data,
               &PmaxConfigDataB2p.Data,
               0,
               0
               );
  PpmS3BootScriptSaveB2PWrite (SocketNumber, PmaxConfigCmdB2p.Data, PmaxConfigDataB2p.Data);
  if (Tempdata != 0) {
    DEBUG ((EFI_D_ERROR, " \n\n :: !!! BIOS failed to program B2P PMAX_CONFIG on socket (%d). Pcode returns (%d) !!!. \n\n", SocketNumber, Tempdata));
  }

  //
  // PMax Trigger Setup
  //
  PmaxGpioTriggerCsr.Data = UsraCsrRead (SocketNumber, 0, PMAX_GPIO_TRIGGER_PCU_FUN0_REG);

  if (PmaxConfig->PmaxTriggerSetup == 0) {
    PmaxGpioTriggerCsr.Bits.pmax_gpio_transmitter_enable = 0;
    PmaxGpioTriggerCsr.Bits.external_trigger             = 0;
    PmaxGpioTriggerCsr.Bits.pmax_trigger_disable         = 0;
  } else if (PmaxConfig->PmaxTriggerSetup == 1) {
    PmaxGpioTriggerCsr.Bits.pmax_gpio_transmitter_enable = 0;
    PmaxGpioTriggerCsr.Bits.external_trigger             = 1;
    PmaxGpioTriggerCsr.Bits.pmax_trigger_disable         = 0;
  } else if (PmaxConfig->PmaxTriggerSetup == 2) {
    PmaxGpioTriggerCsr.Bits.pmax_gpio_transmitter_enable = 1;
    PmaxGpioTriggerCsr.Bits.external_trigger             = 0;
    PmaxGpioTriggerCsr.Bits.pmax_trigger_disable         = 0;
  }

  UsraCsrWrite (SocketNumber, 0, PMAX_GPIO_TRIGGER_PCU_FUN0_REG, PmaxGpioTriggerCsr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PMAX_GPIO_TRIGGER_PCU_FUN0_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PmaxGpioTriggerCsr.Data);
}

