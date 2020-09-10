/** @file

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

#include <Library/MemoryCoreLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <UncoreCommonIncludes.h>
#include <Library/CsrPseudoOffsetConvertLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UsraCsrLib.h>

/**

  Converts NodeId, ChannelId to instance of type BoxType/FuncBlk based on
  Cpu type and cpu sub type

  @param NodeId    - Memory controller index
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param ChannelId - DDR channel Id within a Memory controller, 0 based, 0xFF if not used
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h

  @retval Box Instance

**/
STATIC
UINT8
MemGetBoxInst (
  UINT8    McId,
  UINT8    BoxType,
  UINT8    ChannelId,
  UINT8    FuncBlk,
  UINT8    MaxCh,
  UINT8    NumChPerMC
  )
{
  UINT8 BoxInst = 0xFF;

  switch (BoxType) {
  case BOX_MC:
    if ((FuncBlk > 1) && (FuncBlk != BOX_FUNC_MC_UNSPTD)) {
      BoxInst = McId * MAX_MC_CH;
    } else {
      BoxInst = ChannelId;
    }
    break;
  case BOX_MCCPGC:
  case BOX_MCCADB:
  case BOX_HBM:
  case BOX_HBM2E_MC:
  case BOX_MCDDC:
    if (ChannelId < MaxCh) {
      BoxInst = ChannelId;             // One instance per DDR Channel
    } else {
      CpuCsrAccessError ("Wrong Channel ID parameter: 0x%08x passed with BOX_MCDDC\n", (UINT32) ChannelId);
    }
    break;

  case BOX_MCIO:
    switch (FuncBlk) {
    case BOX_FUNC_MCIO_DDRIO:
    case BOX_FUNC_MCIO_DDRIOEXT:
    case BOX_FUNC_MCIO_DDRIOMC:
    case BOX_FUNC_MCIO_DDRIO_UNSPTD:
      BoxInst = ChannelId;                                        // one instance per DDR Channel
      break;
    default:
      CpuCsrAccessError ("Invalid FuncBlk: 0x%08x passed with BOX_MCIO\n", (UINT32) FuncBlk);
    } // funcblk
    break;

  default:
    CpuCsrAccessError ("Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType);
  }
  return BoxInst;
}

/**

  Returns regBase with true offset (after processing pseudo offset, if needed)

  @param[in] NodeId        - Memory controller index
  @param[in] ChIdOrBoxInst - DDR channel Id within a memory controller
                             or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param[in] regBase       - Register offset; values come from the auto generated header file
                       - may be a pseudo offset

  @retval Updated Register OFfset

**/
UINT32
MemPciCfgOffset (
  IN  UINT8    NodeId,
  IN  UINT8    ChIdOrBoxInst,
  IN  UINT32   regBase
  )
{
  CSR_OFFSET RegOffset;
  CSR_OFFSET TempReg;

  RegOffset.Data = regBase;

  //
  // Adjust offset if pseudo
  //
  if (RegOffset.Bits.pseudo) {
    TempReg.Data = GetPseudoRegisterOffset (RegOffset);
    RegOffset.Bits.offset = TempReg.Bits.offset;
    RegOffset.Bits.size = TempReg.Bits.size;
    //
    // Clear offset bit
    //
    RegOffset.Bits.pseudo = 0;
  }

  //
  // Return updated reg offset
  //
  return RegOffset.Data;
}

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] socket        - Socket number
  @param[in] ChIdOrBoxInst - DDR channel Id within a memory controller
                             or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param[in] Offset        - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
MemReadPciCfgEp (
  IN  UINT8       socket,
  IN  UINT8       ChIdOrBoxInst,
  IN  UINT32      Offset
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 McId;
  UINT8 BoxType;
  UINT8 FuncBlk;
  UINT8 MemSsType;
  CSR_OFFSET RegOffset;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  McId = 0;
  SocId = socket;
  MemSsType = CpuCsrAccessVar->MemSsType[SocId];

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;
  if ( ((BoxType == BOX_MC) && (MemSsType != Hbm2MemSs))  ||
       ((BoxType == BOX_MCDDC) && (MemSsType != Hbm2MemSs)) ||
       (BoxType == BOX_MCIO) ||
       (BoxType == BOX_MCCPGC) ||
       (BoxType == BOX_MCCADB)) {

    if ((FuncBlk != BOX_FUNC_MCIO_DDRIO_UNSPTD) && (FuncBlk != BOX_FUNC_MC_UNSPTD)) {
      if (ChIdOrBoxInst >= CpuCsrAccessVar->DdrMaxCh) {
        CpuCsrAccessError ("Assert on access to ch >= DdrMaxCh\n");
      }

      McId = CpuCsrAccessVar->mcId[socket][ChIdOrBoxInst];
    } else {
      //
      // McId is calclated later during physical address translation for the MCIO_DDRIO_UNSPTD
      // and MC_UNSPTD FuncBlk types, and is unused during box instance resolution
      //
      McId = 0;
    }

    //
    // Need to convert the NodeId/DDR channel ID to box Instance for MC boxes
    //
    BoxInst = MemGetBoxInst (McId, BoxType, ChIdOrBoxInst, FuncBlk, CpuCsrAccessVar->DdrMaxCh, CpuCsrAccessVar->DdrNumChPerMc);
  }  else if ((BOX_HBM == BoxType) || (BOX_HBM2E_MC == BoxType) || (Hbm2MemSs == MemSsType)) {

    CpuCsrAccessError ("HBM or Hbm2MemSs are not supported in this function\n");
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for MC boxes
    //
    BoxInst = (ALL_CH == ChIdOrBoxInst) ? 0:ChIdOrBoxInst;
  } else {
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = ChIdOrBoxInst;
  }

  return UsraCsrRead (SocId, BoxInst, Offset);
}

/**
  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] socket    - Socket number
  @param[in] ch        - Channel number (0-based)
  @param[in] Offset    - Register offset; values come from the auto generated header file
  @param[in] chOffset  - Channel offset

  @retval Register value

**/
UINT32
MemReadPciCfgMC_Ch (
  IN  UINT8    socket,
  IN  UINT8    ch,
  IN  UINT32   Offset,
  IN  UINT32   chOffset
  )
{
  UINT8 mcId;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  mcId = CpuCsrAccessVar->mcId[socket][ch];

  //
  // Look up chOffset (the input parameter has been deprecated)
  //
  chOffset = GetCpgcRegOffset (socket, ch, Offset);

  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)  || IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    return MemReadPciCfgEp (socket, ch, chOffset);
  } else {
    return MemReadPciCfgMC(socket, mcId, chOffset);
  }
}

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] socket        - Socket number
  @param[in] mcId          - Memory controller ID
  @param[in] Offset        - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
MemReadPciCfgMC (
  IN  UINT8       socket,
  IN  UINT8       mcId,
  IN  UINT32      Offset
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8              MemSsType;
  CSR_OFFSET RegOffset;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  SocId = socket;
  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  MemSsType      = CpuCsrAccessVar->MemSsType[SocId];

  if (( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO || BoxType == BOX_MCCPGC || BoxType == BOX_MCCADB ) &&
      (MemSsType != Hbm2MemSs)){

    // SKX hack for now...
    if (mcId >= CpuCsrAccessVar->DdrMaxImc) {
      CpuCsrAccessError ("Assert on access to mc >= MAX_IMC\n");
    }

    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = mcId * CpuCsrAccessVar->DdrNumChPerMc;
  } else if ((BOX_HBM == BoxType) || (BOX_HBM2E_MC == BoxType) || (MemSsType == Hbm2MemSs)){
    CpuCsrAccessError ("HBM or Hbm2MemSs are not supported in this function\n");
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for MC boxes
    //
    BoxInst = mcId;
  } else {

    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = mcId;
  }

  return UsraCsrRead (SocId, BoxInst, Offset);
} // MemReadPciCfgMC

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] socket        - Socket number
  @param[in] ChIdOrBoxInst - DDR channel Id within a memory controller
                             or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param[in] Offset        - Register offset; values come from the auto generated header file
  @param[in] Data          - Register data to be written

**/
VOID
MemWritePciCfgEp (
  IN  UINT8       socket,
  IN  UINT8       ChIdOrBoxInst,
  IN  UINT32      Offset,
  IN  UINT32      Data
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8 McId;
  UINT8 FuncBlk;
  UINT8 MemSsType;
  CSR_OFFSET RegOffset;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  McId = 0;
  SocId = socket;
  MemSsType = CpuCsrAccessVar->MemSsType[SocId];

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;
  if (((BoxType == BOX_MC) && (MemSsType != Hbm2MemSs)) || ((BoxType == BOX_MCDDC) && (MemSsType != Hbm2MemSs)) || BoxType == BOX_MCIO) {

    if ((FuncBlk != BOX_FUNC_MCIO_DDRIO_UNSPTD) && (FuncBlk != BOX_FUNC_MC_UNSPTD)) {
      if (ChIdOrBoxInst >= CpuCsrAccessVar->DdrMaxCh) {
        return;
      }

      McId = CpuCsrAccessVar->mcId[socket][ChIdOrBoxInst];
    } else {

      //
      // McId is calclated later during physical address translation for the MCIO_DDRIO_UNSPTD
      // and MC_UNSPTD FuncBlk types, and is unused during box instance resolution
      //
      McId = 0;
    }

    //
    // Need to convert the NodeId/DDR channel ID to box Instance for MC boxes
    //
    BoxInst = MemGetBoxInst (McId, BoxType, ChIdOrBoxInst, FuncBlk, CpuCsrAccessVar->DdrMaxCh, CpuCsrAccessVar->DdrNumChPerMc);
  } else {
    //
    // For non-MC boxes pass the Box Instance directly
    //
    if ((BOX_HBM == BoxType) || (BOX_HBM2E_MC == BoxType)) {
      CpuCsrAccessError ("HBM or Hbm2MemSs are not supported in this function\n");
      BoxInst = (ALL_CH == ChIdOrBoxInst) ? 0 : ChIdOrBoxInst;
    } else {
      BoxInst = ChIdOrBoxInst;
    }
  }
  UsraCsrWrite (SocId, BoxInst, Offset, Data);
}

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] socket   - Socket number
  @param[in] ch       - Channel number (0-based)
  @param[in] Offset   - Register offset; values come from the auto generated header file
  @param[in] chOffset - Channel offset
  @param[in] Data     - Register data to be written

**/
VOID
MemWritePciCfgMC_Ch (
  IN  UINT8 socket,
  IN  UINT8 ch,
  IN  UINT32 Offset,
  IN  UINT32 chOffset,
  IN  UINT32 Data
  )
{
  UINT8 mcId;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  mcId = CpuCsrAccessVar->mcId[socket][ch];

  //
  // Look up chOffset (the input parameter has been deprecated)
  //
  chOffset = GetCpgcRegOffset (socket, ch, Offset);

  //ICX_TODO
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    MemWritePciCfgEp (socket, ch, chOffset, Data);
  } else {
    MemWritePciCfgMC (socket, mcId, chOffset, Data);
  }
}

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] socket        - Socket number
  @param[in] mcId          - Memory controller ID
  @param[in] Offset        - Register offset; values come from the auto generated header file
  @param[in] Data          - Register data to be written

**/
VOID
MemWritePciCfgMC (
  IN  UINT8       socket,
  IN  UINT8       mcId,
  IN  UINT32      Offset,
  IN  UINT32      Data
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  CSR_OFFSET RegOffset;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  SocId = socket;

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;

  if ( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ) {

    // SKX hack for now...
    if (mcId >= CpuCsrAccessVar->DdrMaxImc) {
      CpuCsrAccessError ("Assert on access to mc %d >= MAX_IMC\n", mcId);
    }

    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = mcId * CpuCsrAccessVar->DdrNumChPerMc;

  } else {

    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = mcId;
  }
  UsraCsrWrite (SocId, BoxInst, Offset, Data);
} // MemWritePciCfgMC

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] socket        - Socket number
  @param[in] mcId          - Memory controller ID
  @param[in] Offset        - Register offset; values come from the auto generated header file
  @param[in] Data          - Register data to be written

**/
VOID
MemWritePciCfgMC_AllCh (
  IN  UINT8    socket,
  IN  UINT8    mcId,
  IN  UINT32   Offset,
  IN  UINT32   Data
  )
{
  UINT8 SocId;
  UINT8 BoxType;
  UINT8              MemSsType;
  CSR_OFFSET RegOffset;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;
  UINT8 i;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  SocId = socket;

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  MemSsType      = CpuCsrAccessVar->MemSsType[SocId];

  //
  // Write all channels in MC
  //
  if (MemSsType != Hbm2MemSs) {
    // For DDR
    if (mcId >= CpuCsrAccessVar->DdrMaxImc){
      CpuCsrAccessError ("Assert on access to ddr mc %d >= MAX_IMC\n", mcId);
    }
    for (i = 0; i < CpuCsrAccessVar->DdrNumChPerMc; i++) {
      MemWritePciCfgEp (SocId, i + (mcId * CpuCsrAccessVar->DdrNumChPerMc), Offset, Data);
    }
  } else {
    CpuCsrAccessError ("HBM or Hbm2MemSs are not supported in this function\n");
  }
} // MemWritePciCfgMC_AllCh

/**

  Get the Memory Controller PCI config Address

  @param[in] socket   - CPU Socket Node number (Socket ID)
  @param[in] mcId     - Memory controller ID
  @param[in] Offset   - Register offset; values come from the auto generated header file

  @retval Returns the return value from UsraGetCsrRegisterAddress

**/
UINTN
MemGetPciCfgMCAddr (
  IN  UINT8    socket,
  IN  UINT8    mcId,
  IN  UINT32   Offset
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  CSR_OFFSET RegOffset;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  SocId = socket;

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;

  if ( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ) {

    // SKX hack for now...
    if (mcId >= CpuCsrAccessVar->DdrMaxImc) {
      CpuCsrAccessError ("Assert on access to mc %d >= MAX_IMC\n", mcId);
    }

    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = mcId * CpuCsrAccessVar->DdrNumChPerMc;

  } else {

    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = mcId;
  }
  return UsraGetCsrRegisterAddress (SocId, BoxInst, Offset);
} // MemGetPciCfgMCAddr

/**

  Get a Memory channel's EPMC Main value

  @param[in] socket    - CPU Socket Node number (Socket ID)
  @param[in] regBase   - MMIO Reg address of first base device

  @retval EPMC main value

**/
UINT32
MemReadPciCfgMain (
  IN  UINT8       socket,
  IN  UINT32      regBase
  )
{
  UINT32  data = 0;
  UINT8 i;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  for (i = 0; i < CpuCsrAccessVar->DdrMaxImc; i++) {
    if (CpuCsrAccessVar->imcEnabled[socket][i]) {
      data = MemReadPciCfgMC (socket, i, regBase);
    }
  }
  return data;

} // MemReadPciCfgMain

/**

  Write a Memory channel's EPMC Main value

  @param[in] socket    - CPU Socket Node number (Socket ID)
  @param[in] regBase   - MMIO Reg address of first base device
  @param[in] data      - Data to write

**/
VOID
MemWritePciCfgMain (
  IN  UINT8       socket,
  IN  UINT32      regBase,
  IN  UINT32      data
  )
{
  UINT8 i;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  for (i = 0; i < CpuCsrAccessVar->DdrMaxImc; i++) {
    if (CpuCsrAccessVar->imcEnabled[socket][i]) {
      MemWritePciCfgMC_AllCh (socket, i, regBase, data);
    }
  }
} // MemWritePciCfgMain
