/** @file
  WHR FPGA silicon programming interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Base.h>
#include "Include/WhrRegisters.h"
#include <Library/UsraCsrLib.h>
#include <Guid/FpgaIpInterface.h>
#include <KtiSetupDefinitions.h> // FPGA Link speed definitions
#include <KtiMisc.h>             // Assert error code definitions
#include <Library/DebugLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/IoAccessLib.h>   // TSC counter
#include <Library/SystemInfoLib.h> // CheckEmulationType
#include <Library/KtiApi.h>        // SocketPresent() function

#define WHR_TXEQ_TIMEOUT  50000    // 50 ms

/**
  Returns boolean if a given Node ID is an FPGA

  @param NodeId            - Node ID that may be an FPGA

  @retval TRUE  - Node is an Fpga
  @retval FALSE - Node is NOT an Fpga

**/
BOOLEAN
FpgaIpIsNodeFpga (
  UINT8                      NodeId
  )
{
  UINT32 DidVid;
  DidVid = UsraCsrRead (NodeId, 0, PCIE_FUNC0_0_FPGA_UPILINK_FUNC0_REG);
  //
  // If DID on BDF 0/4/0 matches WHR
  //
  if (DidVid == WHR_DID_VID) {
    return TRUE;
  }

  return FALSE;
}

/**
  Returns boolean if a given Node ID is an FPGA Home-Agent

  @param NodeId            - Node ID that may be an FPGA Home-Agent

  @retval TRUE  - Node is an Fpga Home-Agent
  @retval FALSE - Node is NOT an Fpga Home-Agent

**/
BOOLEAN
FpgaIpIsNodeHomeAgent (
  UINT8                      NodeId
  )
{
  UINT32 DidVid;
  DidVid = UsraCsrRead (NodeId, 0, PCIE_00_FPGA_SIP_MC_REG);
  //
  // If DID on FPGA BDF 1(SIP)/0/1 matches WHR SIP MC, SIP Memory controller present and FPGA is a home agent
  //
  if (DidVid == WHR_SIP_MC_DID_VID) {
    return TRUE;
  }

  return FALSE;
}

/**
  Set the Bus Number for WHR and SIP, on a given FPGA node.
  Also set the valid bit to enable usage of this bus number.

  @param NodeId             - Node ID of the FPGA
  @param WhrBusDeviceNumber - Bus and device number assigned to WHR
  @param SipBusNumber       - Bus number assigned to SIP

  @retval EFI_SUCCESS      - on successful completion

**/
EFI_STATUS
FpgaIpSetBusNumbers (
  UINT8  NodeId,
  UINT32 WhrBusDeviceNumber,
  UINT8  SipBusNumber
  )
{
  WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_STRUCT WhrBusNumberStruct;
  FPGA_BUSNO_FPGA_SIP_PROTO_STRUCT           FpgaBusNumberStruct;

  //
  // Program bus number into WHR.  This is first CSR-write to the device
  //
  WhrBusNumberStruct.Data = UsraCsrRead (NodeId, 0, WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_REG);
  WhrBusNumberStruct.Bits.num = WhrBusDeviceNumber;
  WhrBusNumberStruct.Bits.num_vld = 1;
  UsraCsrWrite (NodeId, 0, WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_REG, WhrBusNumberStruct.Data);

  //
  // Program bus number into Fpga.  This is the second CSR-write to the device
  //
  FpgaBusNumberStruct.Data = UsraCsrRead (NodeId, 0, FPGA_BUSNO_FPGA_SIP_PROTO_REG);
  FpgaBusNumberStruct.Bits.busnumb0 = SipBusNumber;
  FpgaBusNumberStruct.Bits.busnumbprog = 1;
  UsraCsrWrite (NodeId, 0, FPGA_BUSNO_FPGA_SIP_PROTO_REG, FpgaBusNumberStruct.Data);

  return EFI_SUCCESS;
}

/**
  Tear down the Bus Number assignments for WHR and SIP, on a given FPGA node.
  Clear the valid bit to indicate that bus numbers are NOT assigned.

  @param NodeId             - Node ID of the FPGA

  @retval EFI_SUCCESS      - on successful completion

**/
EFI_STATUS
FpgaIpTearDownBusNumbers (
  UINT8  NodeId
  )
{
  WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_STRUCT WhrBusNumberStruct;
  FPGA_BUSNO_FPGA_SIP_PROTO_STRUCT           FpgaBusNumberStruct;

  WhrBusNumberStruct.Data = UsraCsrRead (NodeId, 0, WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_REG);
  if (WhrBusNumberStruct.Bits.num_vld == 1) {
    //
    // WHR Bus number has previously been assigned, check SIP bus number register and tear it down first
    //
    FpgaBusNumberStruct.Data = UsraCsrRead (NodeId, 0, FPGA_BUSNO_FPGA_SIP_PROTO_REG);
    if (FpgaBusNumberStruct.Bits.busnumbprog == 1) {
      //
      // SIP Bus number has previously been assigned, tear it down
      //
      FpgaBusNumberStruct.Data = 0;
      UsraCsrWrite (NodeId, 0, FPGA_BUSNO_FPGA_SIP_PROTO_REG, FpgaBusNumberStruct.Data);
    }
    WhrBusNumberStruct.Data = 0;
    UsraCsrWrite (NodeId, 0, WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_REG, WhrBusNumberStruct.Data);
  }
  return EFI_SUCCESS;
}

/**
  Read the Opspeed register on WHR

  @param[in] NodeId              Node ID of the FPGA
  @param[in] Port                WHR UPI Port ID on the FPGA

  @retval    OpSpeed             UPI Link speed (as defined for KTI Setup options)
**/
UINT8
FpgaIpGetOpSpeed (
  UINT8 NodeId,
  UINT8 Port
  )
{
  UINT8 OpSpeed;
  WHR_PH_TOPOLOGY_FPGA_UPIPHY_FUNC2_STRUCT WhrLinkSpeed;
  //
  // Find the UPI Link speed supported by FPGA (9.6, 10.4, or 11.2GT) and set the system-wide UPI frequency to that speed
  //
  WhrLinkSpeed.Data = UsraCsrRead (NodeId, Port, WHR_PH_TOPOLOGY_FPGA_UPIPHY_FUNC2_REG);

  OpSpeed = 0xFF;
  switch (WhrLinkSpeed.Bits.hw_mode & 0xF) {
  case (WHR_HW_MODE_112GT):
    OpSpeed = SPEED_REC_112GT;
    break;
  case (WHR_HW_MODE_104GT):
    OpSpeed = SPEED_REC_104GT;
    break;
  case (WHR_HW_MODE_96GT):
    OpSpeed = SPEED_REC_96GT;
    break;
  default:
    DEBUG ((DEBUG_INFO, "\n  Invalid FPGA link frequency detected: hw_mode = 0x%04x", WhrLinkSpeed.Bits.hw_mode));
    break;
  }

  DEBUG ((DEBUG_INFO, "\n  Fpga detected, UPI Link hw_mode = 0x%04x", WhrLinkSpeed.Bits.hw_mode));
  return OpSpeed;
}

/**
  Set the Opspeed bit on WHR when ready to transition to full speed

  @param[in] NodeId              Node ID of the FPGA
  @param[in] Port                WHR UPI Port ID on the FPGA

  @retval                        EFI_SUCCESS - transition to full speed
  @retval                        EFI_FAILURE - can't transition to full speed
**/
EFI_STATUS
FpgaIpSetLinkSpeed (
  UINT8 NodeId,
  UINT8 Port
  )
{
  PH_CTR1_FPGA_UPIPHY_FUNC2_STRUCT WhrSpeedRegister;
  WhrSpeedRegister.Data = UsraCsrRead (NodeId, Port, PH_CTR1_FPGA_UPIPHY_FUNC2_REG);
  WhrSpeedRegister.Bits.copspeed = 1;
  UsraCsrWrite (NodeId, Port, PH_CTR1_FPGA_UPIPHY_FUNC2_REG, WhrSpeedRegister.Data);

  return EFI_SUCCESS;
}

/**
  Set the DRAM Rules on WHR and SIP

  @param[in] NodeId                  Node ID of the FPGA
  @param[in] LocalLimit              CPU Local Rule SAD/TAD Limit
  @param[in] RemoteLimit             CPU Remote Rule SAD/TAD Limit
  @param[in] LocalInterleaveList     CPU Local Rule Interleave List (Remote to FPGA)
  @param[in] RemoteInterleaveList    CPU Remote Rule Interleave List

  @retval                        EFI_SUCCESS - transition to full speed
  @retval                        EFI_FAILURE - can't transition to full speed
**/
EFI_STATUS
FpgaIpSetDramRules (
  UINT8  NodeId,
  UINT32 LocalLimit,
  UINT32 RemoteLimit,
  UINT32 LocalInterleaveList,
  UINT32 RemoteInterleaveList
  )
{
  UINT32                                          WhrInterleaveList0;
  UINT32                                          WhrInterleaveList1;
  UINT32                                          SipInterleaveList0;
  UINT32                                          SipInterleaveList1;

  WHR_UPI_SA_DRAM_RULE0_FPGA_UPILINK_FUNC0_STRUCT WhrDramRule0;
  WHR_UPI_SA_DRAM_RULE1_FPGA_UPILINK_FUNC0_STRUCT WhrDramRule1;
  DRAM_RULE0_FPGA_SIP_PROTO_STRUCT                SipDramRule0;
  DRAM_RULE1_FPGA_SIP_PROTO_STRUCT                SipDramRule1;
  UINT32                                          DramRuleCfgWhr[] =
    {WHR_UPI_SA_DRAM_RULE0_FPGA_UPILINK_FUNC0_REG, WHR_UPI_SA_DRAM_RULE1_FPGA_UPILINK_FUNC0_REG};
  UINT32                                          DramRuleCfgSip[] =
    {DRAM_RULE0_FPGA_SIP_PROTO_REG, DRAM_RULE1_FPGA_SIP_PROTO_REG};
  UINT32                                          InterleaveListCfgWhr[] =
    {WHR_UPI_SA_INTRV_L0_FPGA_UPILINK_FUNC0_REG, WHR_UPI_SA_INTRV_L1_FPGA_UPILINK_FUNC0_REG};
  UINT32                                          InterleaveListCfgSip[] =
    {INTERLEAVE_LIST0_FPGA_SIP_PROTO_REG, INTERLEAVE_LIST1_FPGA_SIP_PROTO_REG};

  WhrInterleaveList0                 = 0;
  WhrInterleaveList1                 = 0;
  SipInterleaveList0                 = 0;
  SipInterleaveList1                 = 0;
  WhrDramRule0.Data                  = 0;
  WhrDramRule1.Data                  = 0;
  SipDramRule0.Data                  = 0;
  SipDramRule1.Data                  = 0;

  //
  // Start to build FPGA Rules and Interleave list
  // Enable FPGA Rule0 if we get here.
  //
  WhrDramRule0.Bits.rule_enable = 1;
  SipDramRule0.Bits.ruleenable  = 1;

  if (LocalLimit == 0) {
    //
    // if there's no local Rule on CPU, FPGA Rule0 is the only Rule that need be written.
    //
    WhrDramRule0.Bits.limit     = RemoteLimit;
    SipDramRule0.Bits.addrlimit = RemoteLimit;
    //
    // And it is a remote Rule
    //
    WhrInterleaveList0 = RemoteInterleaveList;
    SipInterleaveList0 = RemoteInterleaveList;
  } else if (RemoteLimit == 0) {
    //
    // if there's no remote Rule on CPU, FPGA Rule0 is the only Rule that need be written.
    //
    WhrDramRule0.Bits.limit     = LocalLimit;
    SipDramRule0.Bits.addrlimit = LocalLimit;
    //
    // And it is still a remote Rule to FPGA
    //
    WhrInterleaveList0 = LocalInterleaveList;
    SipInterleaveList0 = LocalInterleaveList;
  } else {
    //
    // CPU have both local and Remote Rules. So we need writer both FPGA Rule0 and Rule1.
    // Rule0's limit is Rule1's base. Rule1 must have the higher limit.
    //
     if (LocalLimit < RemoteLimit) {
       WhrDramRule0.Bits.limit     = LocalLimit;
       WhrDramRule1.Bits.limit     = RemoteLimit;
       SipDramRule0.Bits.addrlimit = LocalLimit;
       SipDramRule1.Bits.addrlimit = RemoteLimit;
       WhrInterleaveList0 = LocalInterleaveList;
       WhrInterleaveList1 = RemoteInterleaveList;
       SipInterleaveList0 = LocalInterleaveList;
       SipInterleaveList1 = RemoteInterleaveList;
     } else {
       WhrDramRule0.Bits.limit     = RemoteLimit;
       WhrDramRule1.Bits.limit     = LocalLimit;
       SipDramRule0.Bits.addrlimit = RemoteLimit;
       SipDramRule1.Bits.addrlimit = LocalLimit;
       WhrInterleaveList0 = RemoteInterleaveList;
       WhrInterleaveList1 = LocalInterleaveList;
       SipInterleaveList0 = RemoteInterleaveList;
       SipInterleaveList1 = LocalInterleaveList;
     }
     WhrDramRule1.Bits.rule_enable = 1;
     SipDramRule1.Bits.ruleenable = 1;
  }

  //
  // Print the data to serial debug log
  //
  DEBUG ((DEBUG_INFO, "          En   Base  Limit  IntMode  Attr    Md3  Md3AsMd2  Md3Mode\n"));
  DEBUG ((DEBUG_INFO, "Rule0  :  %x  %05x  %05x     %x       %x\n",WhrDramRule0.Bits.rule_enable,                          0, WhrDramRule0.Bits.limit, WhrDramRule0.Bits.interleave_mode, WhrDramRule0.Bits.attr));
  DEBUG ((DEBUG_INFO, "Rule1  :  %x  %05x  %05x     %x       %x\n",WhrDramRule1.Bits.rule_enable, WhrDramRule0.Bits.limit+1, WhrDramRule1.Bits.limit, WhrDramRule1.Bits.interleave_mode, WhrDramRule1.Bits.attr));
  DEBUG ((DEBUG_INFO, "\nINTERLEAVE0 : %08x\n", WhrInterleaveList0));
  DEBUG ((DEBUG_INFO, "INTERLEAVE1 : %08x\n", WhrInterleaveList1));

  //
  // Update the FPGA CSR
  //
  DEBUG ((DEBUG_INFO, "\t\tWrite FPGA SAD CSRs for NodeId: %d\n", NodeId));
  UsraCsrWrite (NodeId, 0, InterleaveListCfgWhr[0], WhrInterleaveList0);
  UsraCsrWrite (NodeId, 0, InterleaveListCfgSip[0], SipInterleaveList0);
  UsraCsrWrite (NodeId, 0, DramRuleCfgWhr[0], WhrDramRule0.Data);
  UsraCsrWrite (NodeId, 0, DramRuleCfgSip[0], SipDramRule0.Data);
  if (WhrDramRule1.Bits.rule_enable) {
    UsraCsrWrite (NodeId, 0, InterleaveListCfgWhr[1], WhrInterleaveList1);
    UsraCsrWrite (NodeId, 0, InterleaveListCfgSip[1], SipInterleaveList1);
    UsraCsrWrite (NodeId, 0, DramRuleCfgWhr[1], WhrDramRule1.Data);
    UsraCsrWrite (NodeId, 0, DramRuleCfgSip[1], SipDramRule1.Data);
  }

  return EFI_SUCCESS;
}

/**
  Get the amount of memory reported by FPGA Add in card Soft-IP Memory Controller

  @param[in]  NodeId                 Node ID of the FPGA
  @param[out] *MemorySize            Amount of memory reported by FPGA, in 64MB units

  @retval                            EFI_SUCCESS           - MemorySize pointer initialized to indicate FPGA memory size
  @retval                            EFI_INVALID_PARAMETER - can't indicate FPGA memory in MemorySize pointer
**/
EFI_STATUS
FpgaIpGetMemorySize (
  UINT8   NodeId,
  UINT32 *MemSize
  )
{
  MC_IAL_MEM_SIZE_LO_FPGA_SIP_MC_STRUCT   MemSizeLo;
  MC_IAL_MEM_SIZE_HI_FPGA_SIP_MC_STRUCT   MemSizeHi;

  //
  // validate the function parameters
  //
  if (MemSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (FpgaIpIsNodeFpga (NodeId) == FALSE) {
    *MemSize = 0;
    return EFI_INVALID_PARAMETER;
  }

  MemSizeLo.Data = UsraCsrRead (NodeId, 0, MC_IAL_MEM_SIZE_LO_FPGA_SIP_MC_REG);
  MemSizeHi.Data = UsraCsrRead (NodeId, 0, MC_IAL_MEM_SIZE_HI_FPGA_SIP_MC_REG);

  //
  // Check the CSR value and report any problems with the amount of memory:
  // mem_size_lo bits [19:0]  - bitfields not related to FPGA memory size
  // mem_size_lo bits [25:20] - Included in CSR, but indicates some wasted memory (not a multiple of 64MB)
  // mem_size_hi bits [32:27] - Too large to return from this function (address bits [63:58])
  //
  if (MemSizeLo.Bits.memsizelow & (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)) {
    DEBUG ((DEBUG_ERROR, "\n FPGA reporting memory size that is NOT a multiple of 64MB: 0x%08x %08x", MemSizeHi.Bits.memsizehigh, (MemSizeLo.Bits.memsizelow << 20)));
  }
  if (MemSizeHi.Bits.memsizehigh & (BIT32 | BIT31 | BIT30 | BIT29 | BIT28 | BIT27)) {
    DEBUG ((DEBUG_ERROR, "\n FPGA reporting memory size that is too large to return from this function: 0x%08x %08x", MemSizeHi.Bits.memsizehigh, (MemSizeLo.Bits.memsizelow << 20)));
  }

  //
  // convert 64-bit value from register reads to 64MiB units (1<<26)
  //
  *MemSize = MemSizeLo.Bits.memsizelow; // memsize low bits 31:20 (shifts >> 20)
  *MemSize >>= 6; // shift low bits to 64MiB units (shifts remaining >> 6)
  MemSizeHi.Bits.memsizehigh <<= 6; // shift high bits to 64MiB units (reading shifts >> 32, remaining << 6)
  *MemSize |= MemSizeHi.Bits.memsizehigh; // OR in the result

  return EFI_SUCCESS;
}

/**
  Set the memory base address in FPGA Add in card Soft-IP Memory Controller

  @param[in]  NodeId                 Node ID of the FPGA
  @param[out] BaseAddress            Base address of memory in FPGA, in 64MB units

  @retval                            EFI_SUCCESS           - FPGA memory BaseAddress register programmed
**/
EFI_STATUS
FpgaIpSetMemBaseAddress (
  UINT8   NodeId,
  UINT32  BaseAddress
  )
{
  MC_IAL_MEM_BASE_LO_FPGA_SIP_MC_STRUCT BaseAddrLo;
  MC_IAL_MEM_BASE_HI_FPGA_SIP_MC_STRUCT BaseAddrHi;

  BaseAddrLo.Data = 0;
  BaseAddrHi.Data = 0;

  //
  // Convert from 64MiB units (1<<26) to register values (1 << 20)
  //
  BaseAddrLo.Bits.membaselow = (BaseAddress << 6) & 0xFFF; // Bits [5:0] of BaseAddress parameter, with 6 bits of padding
  BaseAddrHi.Bits.membasehigh = BaseAddress >> 6;          //Bits [31:6] of BaseAddress parameter

  UsraCsrWrite (NodeId, 0, MC_IAL_MEM_BASE_LO_FPGA_SIP_MC_REG, BaseAddrLo.Data);
  UsraCsrWrite (NodeId, 0, MC_IAL_MEM_BASE_HI_FPGA_SIP_MC_REG, BaseAddrHi.Data);

  return EFI_SUCCESS;
}

/**
  Program Platform Tx Eq Parameters for WHR UPI port

  @param[in]  NodeId                 Node ID of the FPGA
  @param[in] Port                    WHR UPI port ID on the FPGA
  @param[in] Lane                    WHR UPI port lane index
  @param[in] PreCursor               WHR UPI Tx Eq PreCursor Parameter
  @param[in] Cursor                  WHR UPI Tx Eq Cursor Parameter
  @param[in] PostCursor              WHR UPI Tx Eq PostCursor Parameter

  @retval                            EFI_SUCCESS           - TxEq parameters programmed
  @retval                            EFI_DEVICE_ERROR      - TxEq register never self-cleared, timeout error
**/
EFI_STATUS
FpgaIpProgramTxEqParameters (
  UINT8   NodeId,
  UINT8   Port,
  UINT8   Lane,
  UINT8   PreCursor,
  UINT8   Cursor,
  UINT8   PostCursor
  )
{
  volatile WHR_PH_ADAPT_FPGA_UPIPHY_FUNC2_STRUCT WhrPhAdapt;
  UINT64 StartCount;
  StartCount = GetCountTsc ();

  //
  // Wait for bit to clear, or time out
  //
  if (!CheckEmulationType (SIMICS_FLAG)) {
    do {
      if (GetDelayTsc (StartCount) > WHR_TXEQ_TIMEOUT) {
        DEBUG ((DEBUG_ERROR, "\n WHR UPI Port TxEq programming interface has timed-out"));
        return EFI_DEVICE_ERROR;
      }
      WhrPhAdapt.Data = UsraCsrRead (NodeId, Port, WHR_PH_ADAPT_FPGA_UPIPHY_FUNC2_REG);
    } while ((WhrPhAdapt.Bits.deemph_wr == 1) || (WhrPhAdapt.Bits.rxeqeval == 1));
  }

  //
  // Set Lane, Write, and TxEq data fields
  //
  WhrPhAdapt.Data = 0;
  WhrPhAdapt.Bits.deemph_sel = Lane + 1; // 1-based Lane index
  WhrPhAdapt.Bits.deemph_wr = 1;

  //
  // TxEq coefficients encoded: txX_eq_post[17:12], txX_eq_main[11:6], txX_eq_pre[5:0],
  //
  WhrPhAdapt.Bits.tx_deemph = (PostCursor << 12) | (Cursor << 6) | (PreCursor);

  UsraCsrWrite (NodeId, Port, WHR_PH_ADAPT_FPGA_UPIPHY_FUNC2_REG, WhrPhAdapt.Data);
  return EFI_SUCCESS;
}

/**
Determine whether BIOS should override CHA snoop_credits default programming, based on the FPGA Soft IP configuration.
Note that the CHA snoop_credits override is a silicon workaround ONLY required for CLX, and ICX LCC CPUs.
It is not required for CPX, or ICX HCC/XCC CPUs.

@param[in]  NodeId                 Array, of size MAX_SOCKET, of Cache Agent Node IDs of the FPGA

@retval                            TRUE           - TxEq parameters programmed
@retval                            FALSE          - TxEq register never self-cleared, timeout error
**/
BOOLEAN
FpgaIpSnoopCreditOverrideRequired (
  UINT8   CaNodeIdArray[]
)
{
  KTI_CTRL7_FPGA_SIP_PROTO_STRUCT KtiCtrl7;
  UINT8 NodeId;
  for (NodeId = 0; NodeId < MAX_SOCKET; NodeId++) {
    if (CaNodeIdArray[NodeId] == 1) {
      KtiCtrl7.Data = UsraCsrRead (NodeId, 0, KTI_CTRL7_FPGA_SIP_PROTO_REG);
      if ((KtiCtrl7.Data & BIT31) == 0) {
        //
        // FPGA Soft IP indicates EvictClean is disabled,
        // so HA_THRESHOLDS.snoop_credits should be set to 0x1 IF connected to CPX or ICX-LCC CPU
        //
        return TRUE;
      } else {
        return FALSE;
      }
    }
  }
  return FALSE;
}

/**
  Tell the FPGA SIP Proto layer which CPUs are present in the system,
  so that it knows which CPUs to send snoops to.

  @param[in]  NodeId                 Node ID of the FPGA

  @retval                            EFI_SUCCESS           - KTI_CTRL6 programmed
**/
EFI_STATUS
FpgaIpProgramSnoopCpuList (
  UINT8   NodeId
  )
{
  KTI_CTRL6_FPGA_SIP_PROTO_STRUCT KtiCtrl6;
  UINT8                           CpuSocket;
  UINT8                           CpuCounter;

  KtiCtrl6.Data = UsraCsrRead (NodeId, 0, KTI_CTRL6_FPGA_SIP_PROTO_REG);
  CpuCounter = 0;

  for (CpuSocket = 0; CpuSocket < MAX_SOCKET; CpuSocket++) {
    if (SocketPresent (CpuSocket)) {
      if (CpuCounter == 0) {
        KtiCtrl6.Bits.cpunodeid0 = CpuSocket;
      } else if (CpuCounter == 1) {
        KtiCtrl6.Bits.cpunodeid1 = CpuSocket;
      } else if (CpuCounter == 2) {
        KtiCtrl6.Bits.cpunodeid2 = CpuSocket;
      } else if (CpuCounter == 3) {
        KtiCtrl6.Bits.cpunodeid3 = CpuSocket;
      }
      CpuCounter++;
    }
  }
  ASSERT (CpuCounter <= 4);
  UsraCsrWrite (NodeId, 0, KTI_CTRL6_FPGA_SIP_PROTO_REG, KtiCtrl6.Data);
  return EFI_SUCCESS;
}