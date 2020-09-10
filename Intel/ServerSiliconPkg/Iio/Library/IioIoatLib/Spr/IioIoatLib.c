/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/KtiApi.h>
#include <LibraryPrivate/IioDebug.h>
#include <Library/IioIoatLib.h>
#include <PcieRegs.h>

/**

  DSA initialization routine for SPR

  @param IioIndex              - Socket Index
  @param InstId                - Instance ID

  @retval None

**/
VOID
IioDsaInit (
  IN  UINT8                        IioIndex,
  IN  UINT8                        InstId
  )
{
  VC0CTL_IIO_DSA_STRUCT         Vc0CtlDsa;
  VC1CTL_IIO_DSA_STRUCT         Vc1CtlDsa;
  TPHCTL_IIO_DSA_STRUCT         TphCtlDsa;
  TPHSTTBL0_IIO_DSA_STRUCT      TphStTbl0Dsa;
  TPHSTTBL1_IIO_DSA_STRUCT      TphStTbl1Dsa;
  DEVCTL_IIO_DSA_STRUCT          DevctlDsa;

  Vc0CtlDsa.Data = IioReadCpuCsr32 (IioIndex, InstId, VC0CTL_IIO_DSA_REG);
  Vc0CtlDsa.Bits.tcvcmap_7_1 &= ~BIT0;
  IioWriteCpuCsr32 (IioIndex, InstId, VC0CTL_IIO_DSA_REG, Vc0CtlDsa.Data);

  Vc1CtlDsa.Data = IioReadCpuCsr32 (IioIndex, InstId, VC1CTL_IIO_DSA_REG);
  Vc1CtlDsa.Bits.tcvcmap_7_1 = 1; // TC1 map to VC1
  Vc1CtlDsa.Bits.vcid = 1;  // VC1
  Vc1CtlDsa.Bits.vcen = 1;
  IioWriteCpuCsr32 (IioIndex, InstId, VC1CTL_IIO_DSA_REG, Vc1CtlDsa.Data);

  TphCtlDsa.Data = IioReadCpuCsr32 (IioIndex, InstId, TPHCTL_IIO_DSA_REG);
  TphCtlDsa.Bits.stmodesel = 2;
  TphCtlDsa.Bits.tphreqen_8_8 = 1;
  IioWriteCpuCsr32 (IioIndex, InstId, TPHCTL_IIO_DSA_REG, TphCtlDsa.Data);

  TphStTbl0Dsa.Data = IioReadCpuCsr16 (IioIndex, InstId, TPHSTTBL0_IIO_DSA_REG);
  TphStTbl0Dsa.Bits.stle = 0;
  IioWriteCpuCsr16 (IioIndex, InstId, TPHSTTBL0_IIO_DSA_REG, TphStTbl0Dsa.Data);

  TphStTbl1Dsa.Data = IioReadCpuCsr16 (IioIndex, InstId, TPHSTTBL1_IIO_DSA_REG);
  TphStTbl1Dsa.Bits.stle = 0xA;
  IioWriteCpuCsr16 (IioIndex, InstId, TPHSTTBL1_IIO_DSA_REG, TphStTbl1Dsa.Data);

  DevctlDsa.Data = IioReadCpuCsr16 (IioIndex, InstId, DEVCTL_IIO_DSA_REG);
  DevctlDsa.Bits.mps = 2;
  IioWriteCpuCsr16 (IioIndex, InstId, DEVCTL_IIO_DSA_REG, DevctlDsa.Data);
}

/**

  IAX initialization routine for SPR

  @param IioIndex              - Socket Index
  @param InstId                - Instance ID

  @retval None

**/
VOID
IioIaxInit (
  IN  UINT8                        IioIndex,
  IN  UINT8                        InstId
  )
{
  VC0CTL_IIO_IAX_STRUCT         Vc0CtlIax;
  VC1CTL_IIO_IAX_STRUCT         Vc1CtlIax;
  TPHCTL_IIO_IAX_STRUCT         TphCtlIax;
  TPHSTTBL0_IIO_IAX_STRUCT      TphStTbl0Iax;
  TPHSTTBL1_IIO_IAX_STRUCT      TphStTbl1Iax;

  Vc0CtlIax.Data = IioReadCpuCsr32 (IioIndex, InstId, VC0CTL_IIO_IAX_REG);
  Vc0CtlIax.Bits.tcvcmap_7_1 &= ~BIT0;
  IioWriteCpuCsr32 (IioIndex, InstId, VC0CTL_IIO_IAX_REG, Vc0CtlIax.Data);

  Vc1CtlIax.Data = IioReadCpuCsr32 (IioIndex, InstId, VC1CTL_IIO_IAX_REG);
  Vc1CtlIax.Bits.tcvcmap_7_1 = 1; // TC1 map to VC1
  Vc1CtlIax.Bits.vcid = 1;        // VC1
  Vc1CtlIax.Bits.vcen = 1;
  IioWriteCpuCsr32 (IioIndex, InstId, VC1CTL_IIO_IAX_REG, Vc1CtlIax.Data);

  TphCtlIax.Data = IioReadCpuCsr32 (IioIndex, InstId, TPHCTL_IIO_IAX_REG);
  TphCtlIax.Bits.stmodesel = 2;
  TphCtlIax.Bits.tphreqen_8_8 = 1;
  IioWriteCpuCsr32 (IioIndex, InstId, TPHCTL_IIO_IAX_REG, TphCtlIax.Data);

  TphStTbl0Iax.Data = IioReadCpuCsr16 (IioIndex, InstId, TPHSTTBL0_IIO_IAX_REG);
  TphStTbl0Iax.Bits.stle = 0;
  IioWriteCpuCsr16 (IioIndex, InstId, TPHSTTBL0_IIO_IAX_REG, TphStTbl0Iax.Data);

  TphStTbl1Iax.Data = IioReadCpuCsr16 (IioIndex, InstId, TPHSTTBL1_IIO_IAX_REG);
  TphStTbl1Iax.Bits.stle = 0xA;
  IioWriteCpuCsr16 (IioIndex, InstId, TPHSTTBL1_IIO_IAX_REG, TphStTbl1Iax.Data);
}
/**

    Code to be executed during boot event.

    @param IioGlobalData - Pointer to IIO Global information
    @param Iio           - Index with the changed IIO (Socket)

    @retval None

**/
VOID
IioIoatInitBootEvent (
  IN  IIO_GLOBALS                  *IioGlobalData,
  IN  UINT8                        IioIndex
  )
{
  UINT8                           InstId = 0;
  UINT8                           StackIndex = 0;

  IIO_D_LOG ("IOAT_INIT_BOOT_EVENT_START\n");

  for (StackIndex = 0; StackIndex < MAX_IIO_STACK_SPR; StackIndex++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO)) {
      continue;
    }

    InstId = StackIndex % MAX_DINO_STACK;
    // Initial DAS during boot event.

    if(IioGlobalData->SetupData.DsaEn[IioIndex*NUM_DSA+InstId] == IIO_OPTION_ENABLE) {
      IIO_D_LOG ("DSA init IioIndex : %d InstId : %d\n", IioIndex, InstId);
      IioDsaInit (IioIndex, InstId);
    }
    if(IioGlobalData->SetupData.IaxEn[IioIndex*NUM_IAX+InstId] == IIO_OPTION_ENABLE) {
      IIO_D_LOG ("IAX init IioIndex : %d InstId : %d\n", IioIndex, InstId);
      IioIaxInit (IioIndex, InstId);
    }
  }

  IIO_D_LOG ("IOAT_INIT_BOOT_EVENT_END\n");
}

/**

    Code to be executed on ReadyToBoot event.

    @param IioGlobalData - Pointer to IIO Global information
    @param IioIndex      - Socket Index

**/
VOID
IioIoatReadyToBootEvent (
  IN  IIO_GLOBALS                  *IioGlobalData,
  IN  UINT8                        IioIndex
  )
{
  PCICMD_IIO_DSA_STRUCT           CmdRegDsa;
  UINT8                           InstId;
  UINT8                           StackIndex = 0;

  IIO_D_LOG ("IOAT_INIT_READY_TO_BOOT_START\n");

  for (StackIndex = 0; StackIndex < MAX_IIO_STACK_SPR; StackIndex++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO)) {
      continue;
    }

    InstId = StackIndex % MAX_DINO_STACK;
    CmdRegDsa.Data = IioReadCpuCsr16 (IioIndex, InstId, PCICMD_IIO_DSA_REG);
    CmdRegDsa.Bits.bme = 0x01;
    IioNonS3WriteCpuCsr16 (IioIndex, InstId, PCICMD_IIO_DSA_REG, CmdRegDsa.Data);
  }

  IIO_D_LOG ("IOAT_INIT_READY_TO_BOOT_END\n");
}
