/** @file
  FPGA generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of FPGA IP blocks.
  This library is designed to be directly linked into the Uncore consumer of these
  services
  OR
  To be used to expose a dynamic interface (PPI, DXE Protocol, or SMM Protocol) as
  appropriate for the phase being executed.

  Furthermore, this subsystem is designed to support multiple different versions
  dynamically linked at runtime.

  The expected valid uses for this library are thus:
  (1) One Uncore PEIM supporting a homogenous system and the single instance is statically linked
  (2) One Uncore PEIM supporting a heterogeneous system where a router instance is statically linked
    and multiple PEIM produce silicon specific instances of the library and the router selects the
    correct instance per socket.

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

#ifndef _FPGA_IP_INTERFACE_H_
#define _FPGA_IP_INTERFACE_H_

#include <Uefi.h>
#include <Base.h>

/**
  Returns boolean if a given Node ID is an FPGA

  @param NodeId            - Node ID that may be an FPGA

  @retval TRUE  - Node is an Fpga
  @retval FALSE - Node is NOT an Fpga

**/
BOOLEAN
FpgaIpIsNodeFpga (
  UINT8                      NodeId
);
//
// Function pointer definiton for FpgaIpIsNodeFpga ()
//
typedef
BOOLEAN
(EFIAPI *IS_NODE_FPGA) (
  IN UINT8                 NodeId
);

/**
  Returns boolean if a given Node ID is an FPGA Home-Agent

  @param NodeId            - Node ID that may be an FPGA Home-Agent

  @retval TRUE  - Node is an Fpga Home-Agent
  @retval FALSE - Node is NOT an Fpga Home-Agent

**/
BOOLEAN
FpgaIpIsNodeHomeAgent (
  UINT8                      NodeId
);
//
// Function pointer definiton for FpgaIpIsNodeHomeAgent ()
//
typedef
BOOLEAN
(EFIAPI *IS_NODE_HOME_AGENT) (
  IN UINT8                 NodeId
);

/**
  Set the Bus Number for WHR and SIP, on all FPGA nodes.
  Also set the valid bit to enable usage of this bus number.
  This function should be defined in an FPGA HW IP library.

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
);
//
// Function pointer definiton for FpgaIpSetBusNumbers ()
//
typedef
EFI_STATUS
(EFIAPI *SET_BUS_NUMBERS) (
  IN UINT8  NodeId,
  IN UINT32 WhrBusDeviceNumber,
  IN UINT8  SipBusNumber
);

/**
  Tear down the Bus Number assignments for WHR and SIP, on a given FPGA node.
  Clear the valid bit to indicate that bus numbers are NOT assigned.

  @param NodeId             - Node ID of the FPGA

  @retval EFI_SUCCESS      - on successful completion

**/
EFI_STATUS
FpgaIpTearDownBusNumbers (
  UINT8  NodeId
);
//
// Function pointer definiton for FpgaIpTearDownBusNumbers ()
//
typedef
EFI_STATUS
(EFIAPI *TEAR_DOWN_BUS_NUMBERS) (
  IN UINT8  NodeId
);

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
);
//
// Function pointer definiton for FpgaIpGetOpSpeed ()
//
typedef
UINT8
(EFIAPI *GET_WHR_OP_SPEED) (
  IN UINT8 NodeId,
  IN UINT8 Port
);

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
);
//
// Function pointer definiton for FpgaIpSetLinkSpeed ()
//
typedef
EFI_STATUS
(EFIAPI *SET_WHR_OP_SPEED) (
  IN UINT8 NodeId,
  IN UINT8 Port
);

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
);
//
// Function pointer definiton for FpgaIpSetDramRules ()
//
typedef
EFI_STATUS
(EFIAPI *SET_FPGA_DRAM_RULES) (
  IN UINT8 NodeId,
  IN UINT32 LocalLimit,
  IN UINT32 RemoteLimit,
  IN UINT32 LocalInterleaveList,
  IN UINT32 RemoteInterleaveList
);

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
);
//
// Function pointer definiton for FpgaIpGetMemorySize ()
//
typedef
EFI_STATUS
(EFIAPI *GET_MEMORY_SIZE) (
  IN  UINT8  NodeId,
  OUT UINT32 *MemSize
);

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
);
//
// Function pointer definiton for FpgaIpSetMemBaseAddress ()
//
typedef
EFI_STATUS
(EFIAPI *SET_MEM_BASE_ADDRESS) (
  IN  UINT8  NodeId,
  IN UINT32  BaseAddress
);

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
);
//
// Function pointer definiton for FpgaIpProgramTxEqParameters ()
//
typedef
EFI_STATUS
(EFIAPI *PROGRAM_WHR_TXEQ_PARAM) (
  UINT8   NodeId,
  UINT8   Port,
  UINT8   Lane,
  UINT8   PreCursor,
  UINT8   Cursor,
  UINT8   PostCursor
);

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
);
//
// Function pointer definiton for FpgaIpSnoopCreditOverrideRequired ()
//
typedef
EFI_STATUS
(EFIAPI *WA_SNOOP_CREDIT) (
  UINT8   CaNodeIdArray[]
);

/**
  Tell the FPGA SIP Proto layer which CPUs are present in the system,
  so that it knows which CPUs to send snoops to.

  @param[in]  NodeId                 Node ID of the FPGA

  @retval                            EFI_SUCCESS           - KTI_CTRL6 programmed
**/
EFI_STATUS
FpgaIpProgramSnoopCpuList (
  UINT8   NodeId
);
//
// Function pointer definiton for FpgaIpProgramSnoopCpuList ()
//
typedef
EFI_STATUS
(EFIAPI *PROGRAM_SNOOP_CPU_LIST) (
  UINT8   NodeId
);


typedef struct {
  //
  // Function pointers
  //
  IS_NODE_FPGA              FpgaIpIsNodeFpga;
  IS_NODE_HOME_AGENT        FpgaIpIsNodeHomeAgent;
  SET_BUS_NUMBERS           FpgaIpSetBusNumbers;
  TEAR_DOWN_BUS_NUMBERS     FpgaIpTearDownBusNumbers;
  GET_WHR_OP_SPEED          FpgaIpGetOpSpeed;
  SET_WHR_OP_SPEED          FpgaIpSetLinkSpeed;
  SET_FPGA_DRAM_RULES       FpgaIpSetDramRules;
  GET_MEMORY_SIZE           FpgaIpGetMemorySize;
  SET_MEM_BASE_ADDRESS      FpgaIpSetMemBaseAddress;
  PROGRAM_WHR_TXEQ_PARAM    FpgaIpProgramTxEqParameters;
  WA_SNOOP_CREDIT           FpgaIpSnoopCreditOverrideRequired;
  PROGRAM_SNOOP_CPU_LIST    FpgaIpProgramSnoopCpuList;
} FPGA_IP_INTERFACE;

//
// This API supports one instance with the GUID identifying the instance
//
#define FPGA_IP_INTERFACE_GUID \
  { 0x69FC448A, 0x28FC, 0x40E3, {0xAB, 0x2A, 0x70, 0x22, 0xC4, 0x36, 0xCF, 0xED }} // {69FC448A-28FC-40E3-AB2A-7022C436CFED}
extern EFI_GUID gFpgaIpInterfaceGuid;

#endif //_FPGA_IP_INTERFACE_H_
