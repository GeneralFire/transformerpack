/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
// Do not include any of this functionality in sim builds
//

#include <Uefi.h>
#include <PiPei.h>
#include <RcRegs.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>

#ifdef CDF_SC_FLAG
#include <Private/Library/PeiPchDmiLib.h>
#else // LBG-way
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/PchInit.h>
#include <Private/Library/PchRcLib.h>
#include <Library/PchMultiPch.h>
#endif // CDF_SC_FLAG

#include <IioPlatformData.h>
#include <Guid/IioPolicyHob.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDmiLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioDmiConf.h>

/**
  This routine:
  - retrieve the PCH side TC/VC configuration
  - merge IIO and PCH configurations

  @param[in out]  IioGlobalData      Pointer to IIO_GLOBALS
  @param[in out]  PchDmiTcVcMap      Structure used to exchange data with PCH
**/
VOID
PrepareTcVcConfiguration (
  IN      IIO_GLOBALS               *IioGlobalData,
  IN OUT  PCH_DMI_TC_VC_MAP         *PchDmiTcVcMap
 )
{
  UINT8  TcIndex;
  UINT8  VcId;

#ifndef CDF_SC_FLAG
  EFI_STATUS                   Status;
  PCH_INIT_PPI                 *PchInitPpi;
#endif

  //
  // Get default Tc/Vc mapping from PCH side
  //
#ifdef CDF_SC_FLAG
  PchDmiTcVcMapInit (PchDmiTcVcMap);
#else // LBG-way
  Status = PeiServicesLocatePpi (&gPchInitPpiGuid, 0, NULL, &PchInitPpi);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }
  PchInitPpi->DmiTcVcMapInit (PchDmiTcVcMap);
#endif //CDF_SC_FLAG

  //
  // Compare IIO and PCH settings
  //

  //
  // If VCx is disabled on one side => disable at all
  // Use PCH VcId for enabled VCs
  //
  if (!PchDmiTcVcMap->DmiVc[DmiVcTypeVc1].Enable || IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] == 0) {
    //
    // If Vc1 is disabled on IIO or PCH side VC1 is disabled
    //
    PchDmiTcVcMap->DmiVc[DmiVcTypeVc1].Enable = FALSE;
    IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] = 0;
  } else {
    IioGlobalData->IioVar.IioVData.DmiVcId[DmiTypeVc1] = PchDmiTcVcMap->DmiVc[DmiVcTypeVc1].VcId;
  }

  if (!PchDmiTcVcMap->DmiVc[DmiVcTypeVcm].Enable || IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] == 0) {
    //
    // If Vcm is disabled on IIO or PCH side VCm is disabled
    //
    PchDmiTcVcMap->DmiVc[DmiVcTypeVcm].Enable = FALSE;
    IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] = 0;
  } else {
    IioGlobalData->IioVar.IioVData.DmiVcId[DmiVcTypeVcm] = PchDmiTcVcMap->DmiVc[DmiVcTypeVcm].VcId;
  }

  //
  // TC to VC mapping
  //
  IIO_D_DMILOG ("Final Tc/VC mapping:\n");
  for (TcIndex = DmiTypeTc0; TcIndex < MaxDmiTcType; TcIndex++) {

    switch (PchDmiTcVcMap->DmiTc[TcIndex].Vc) {

    case DmiVcTypeVc1:
      if (IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] == 1) {
        VcId = DmiTypeVc1;
      } else {
        VcId = DmiTypeVc0;
      }
      break;
    case DmiVcTypeVcm:
      if (IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] == 1) {
        VcId = DmiTypeVcm;
      } else {
        VcId = DmiTypeVc0;
      }
      break;
    case DmiVcTypeVc0:
    default:
      VcId = DmiTypeVc0;
      break;
    }

    IIO_D_DMILOG (" - Tc:%d pch-VC:%d mapped to: Vc:%d\n", TcIndex, PchDmiTcVcMap->DmiTc[TcIndex].Vc, VcId);

    PchDmiTcVcMap->DmiTc[TcIndex].Vc = VcId;
    IioGlobalData->IioVar.IioVData.DmiTc[TcIndex] = VcId;
  }
}

/**
  This routine will:
  - program TC/VC settings first on CPU side
  - next program and poll

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
**/
VOID
ProgrammTcVcMapping (
  IN  IIO_GLOBALS *IioGlobalData
  )
{
  EFI_STATUS             Status;
  PCH_DMI_TC_VC_MAP      PchDmiTcVcMap;

#ifndef CDF_SC_FLAG
  PCH_INIT_PPI          *PchInitPpi;

  Status = PeiServicesLocatePpi (&gPchInitPpiGuid, 0, NULL, &PchInitPpi);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }
#endif // CDF_SC_FLAG

  //
  // Initialize PchDmiTcVcMap based in IIO DMI Vc/Tc settings
  //
  ZeroMem (&PchDmiTcVcMap, sizeof (PCH_DMI_TC_VC_MAP));
  PrepareTcVcConfiguration (IioGlobalData, &PchDmiTcVcMap);

  //
  // Program CPU side TC/VC mapping
  //
  IIO_D_DMILOG ("Program TC/VC mapping on IIO side\n");
  IioDmiProgramTcVcRegs (IioGlobalData);

  //
  // Program and poll TC/VC on PCH side
  //
  IIO_D_DMILOG ("Program/Poll TC/VC mapping on PCH side\n");
#ifdef CDF_SC_FLAG
  Status = PchDmiTcVcProgPoll (&PchDmiTcVcMap);
  ASSERT_EFI_ERROR (Status);
#else // LBG-way
  Status = PchInitPpi->DmiTcVcProg (PchGetPchIdBySocketId (SOCKET_0_INDEX), &PchDmiTcVcMap);
  ASSERT_EFI_ERROR (Status);

  Status = PchInitPpi->DmiTcVcPoll (PchGetPchIdBySocketId (SOCKET_0_INDEX), &PchDmiTcVcMap);
  ASSERT_EFI_ERROR (Status);
#endif // CDF_SC_FLAG

  //
  // Poll TC/VC settings (IIO side)
  //
  IIO_D_DMILOG ("Poll TC/VC mapping on IIO side\n");
  IioDmiPollTcVcRegs (IioGlobalData);
}


/**
  This routine will apply all Si Workarounds for DMI
  if required.

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
**/
VOID
ApplyDmiWorkarounds (
  IN IIO_GLOBALS *IioGlobalData
  )
{
  UINT8  Bus;
  UINT32 ClassCode;
  UINT8  IioIndex;

  // TODO - this is probably not correct for SPR!
  if (IioGlobalData->IioVar.IioVData.MultiPch) {
    // WA for Multi-PCH, Send a dummy write to non legacy PCH D17:F0:Off:0 to make sure MROM0 and MROM1 can capture the bus number.
    // Make sure this is done before any read\write access to non legacy PCH.
    for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
      if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
        continue;
      }

      Bus = IioGlobalData->IioVar.IioVData.SocketBaseBusNumber[IioIndex];
      ClassCode = IioPciExpressRead32 (IioIndex, Bus, 0, 0, 0x8); // Read RID (Class code in offset 0x9)
      if ((ClassCode >> 8) == (PCI_CLASS_BRIDGE << 16 | PCI_CLASS_BRIDGE_HOST << 8)) {  // If RP configured as DMI then send dummy write
        IioPciExpressWrite16 (IioIndex, Bus, 17, 0, 0, 0x8086);
      }
    }
  } // if IioVar.IioVData.MultiPch
}


/**
  This routine will initialize Vc/Tc settings in IioGlobalData
  if required (specifically what VC are enabled).

  @param[in out]  IioGlobalData - Pointer to IIO_GLOBALS
**/
VOID
GetBaseTcVcConfigurationSkx (
  IN  IIO_GLOBALS                 *IioGlobalData
  )
{
  //
  // SKX code needs to support compilation without ME etc.
  //
#if defined(ME_SUPPORT_FLAG) || defined(IE_SUPPORT)
  if (IioGlobalData->IioVar.IioVData.IsocEnable) {
     IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] = 1;
   } else { /// Use default values
     IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] = 0;
  }
  IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] = 0;
#if defined(ME_SUPPORT_FLAG)
  IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] = (IioGlobalData->IioVar.IioVData.meRequestedSize > 0) ? 1 : 0;
#endif // ME_SUPPORT_FLAG
#if defined(IE_SUPPORT)
  IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] = (IioGlobalData->IioVar.IioVData.ieRequestedSize > 0) ? 1 : IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm];
#endif // IE_SUPPORT
#else
  IIO_D_DMIDBG ("Disable all DMI VCx\n");
  IioGlobalData->IioVar.IioVData.IsocEnable = FALSE;
  IioGlobalData->IioVar.IioVData.meRequestedSize = 0;
  IioGlobalData->IioVar.IioVData.ieRequestedSize = 0;
  IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] = 0;
  IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] = 0;
#endif // ME_SUPPORT_FLAG

}


/**
  This routine will initialize Vc/Tc settings in IioGlobalData
  if required (specifically what VC are enabled).

  @param[in out]  IioGlobalData - Pointer to IIO_GLOBALS
**/
VOID
GetBaseTcVcConfiguration (
  IN  IIO_GLOBALS                 *IioGlobalData
  )
{
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) ||
      IsCpuAndRevision (CPU_CLX, REV_ALL) ||
      IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    //
    // For 14nm CPUs use specific configuration
    //
    GetBaseTcVcConfigurationSkx (IioGlobalData);
    return;
  }

  //
  // VC0 is always enabled
  //
  IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc0] = 1;

  //
  // VC1 is:
  // - always enabled for RLINK setups (SNR/ICXD/TNR)
  // - enabled for DMI if ISOC is also enabled
  //
  if (HasRlink() || IioGlobalData->IioVar.IioVData.IsocEnable) {

    IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] = 1;
  } else {
    IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] = 0;
  }

  //
  // VCm is:
  // - always enabled for RLINK setups (SNR/ICXD/TNR)
  // - enabled for DMI if ME UMA is present
  //
  if (IsCpuAndRevision (CPU_ICXD, REV_ALL) ||
      IsCpuAndRevision (CPU_SNR, REV_ALL)  ||
      IsCpuAndRevision (CPU_TNR, REV_ALL)  ||
      IioGlobalData->IioVar.IioVData.meRequestedSize != 0) {
    IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] = 1;
  } else {
    IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] = 0;
  }
}


/**
  This routine will program DMI TcVc mappings for IIO and PCH side.
  Also it will update LNKCTRL PCH side to support DMI Max Link Speed.

  @param[in] IioGlobalData    - Pointer to IIO_GLOBALS structure
**/
VOID
IioDmiTcVcSetup (
  IN  IIO_GLOBALS                 *IioGlobalData
  )
{
  IIO_D_DMIDBG ("TcVc Setup Start\n");
  //
  //  Apply any DMI specific Si workarounds first
  //
  ApplyDmiWorkarounds (IioGlobalData);

  //
  // Initialize Vc/Tc settings (in IioGlobalData - this is a base configuration on IIO side)
  //
  GetBaseTcVcConfiguration (IioGlobalData);

  IIO_D_DMIDBG ("Base VC config: Vc1=%x Vcm=%x\n",
      IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1],
      IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm]);

  if (IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] == 0 &&
      IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVcm] == 0) {
    //
    // Both Vc1 and VCm are disabled on IIo side => there is nothing to program
    //
    return;
  }

  //
  // Program (and poll) Tc/Vc mappings on both (bpu and pch) sides
  //
  ProgrammTcVcMapping (IioGlobalData);

  IIO_D_DMIDBG ("TcVc Setup Done\n");
}

