/** @file
  Null library implementation of FPGA Silicon interface.

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
#include <Guid/FpgaIpInterface.h>

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
  return FALSE;
}

/**
  Set the Bus Number for WHR and SIP, on all FPGA nodes.
  Also set the valid bit to enable usage of this bus number.
  This function should be defined in an FPGA HW IP library.

  @param NodeId             - Node ID of the FPGA
  @param WhrBusDeviceNumber - Bus and device number assigned to WHR
  @param SipBusNumber       - Bus number assigned to SIP

  @retval                   - EFI_SUCCESS - (NULL Lib implementation)

**/
EFI_STATUS
FpgaIpSetBusNumbers (
  UINT8  NodeId,
  UINT32 WhrBusDeviceNumber,
  UINT8  SipBusNumber
  )
{
  return EFI_SUCCESS;
}

/**
  Tear down the Bus Number assignments for WHR and SIP, on a given FPGA node.
  Clear the valid bit to indicate that bus numbers are NOT assigned.

  @param NodeId             - Node ID of the FPGA

  @retval                   - EFI_SUCCESS - (NULL Lib implementation)

**/
EFI_STATUS
FpgaIpTearDownBusNumbers (
  UINT8  NodeId
  )
{
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
  return 0xFF;
}

/**
  Set the Opspeed bit on WHR when ready to transition to full speed

  @param[in] NodeId              Node ID of the FPGA
  @param[in] Port                WHR UPI Port ID on the FPGA

  @retval                        EFI_SUCCESS - (NULL Lib implementation)
**/
EFI_STATUS
FpgaIpSetLinkSpeed (
  UINT8 NodeId,
  UINT8 Port
  )
{
  return EFI_SUCCESS;
}

/**
  Set the DRAM Rules on WHR and SIP

  @param[in] NodeId                  Node ID of the FPGA
  @param[in] LocalLimit              CPU Local Rule SAD/TAD Limit
  @param[in] RemoteLimit             CPU Remote Rule SAD/TAD Limit
  @param[in] LocalInterleaveList     CPU Local Rule Interleave List (Remote to FPGA)
  @param[in] RemoteInterleaveList    CPU Remote Rule Interleave List

  @retval                            EFI_SUCCESS - (NULL Lib implementation)
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
  if (MemSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *MemSize = 0;
  return EFI_INVALID_PARAMETER;
}

/**
  Set the memory base address in FPGA Add in card Soft-IP Memory Controller

  @param[in]  NodeId                 Node ID of the FPGA
  @param[out] BaseAddress            Base address of memory in FPGA, in 64MB units

  @retval                            EFI_SUCCESS - (NULL Lib implementation)
**/
EFI_STATUS
FpgaIpSetMemBaseAddress (
  UINT8   NodeId,
  UINT32  BaseAddress
  )
{
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

  @retval                            EFI_SUCCESS - (NULL Lib implementation)
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
  return EFI_SUCCESS;
}

/**
  Determine whether BIOS should override CHA snoop_credits default programming, based on the FPGA Soft IP configuration.
  Note that the CHA snoop_credits override is a silicon workaround ONLY required for CLX, and ICX LCC CPUs.
  It is not required for CPX, or ICX HCC/XCC CPUs.

  @param[in]  NodeId                 Node ID of the FPGA

  @retval                            TRUE           - TxEq parameters programmed
  @retval                            FALSE          - TxEq register never self-cleared, timeout error
**/
BOOLEAN
FpgaIpSnoopCreditOverrideRequired (
  UINT8   CaNodeIdArray[]
  )
{
  return FALSE;
}

/**
  Tell the FPGA SIP Proto layer which CPUs are present in the system,
  so that it knows which CPUs to send snoops to.

  @param[in]  NodeId                 Node ID of the FPGA

  @retval                            EFI_SUCCESS - (NULL Lib implementation)
**/
EFI_STATUS
FpgaIpProgramSnoopCpuList (
  UINT8   NodeId
  )
{
  return EFI_SUCCESS;
}