/** @file
  Prototypes for KTI functions

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

#ifndef _KTI_API_H_
#define _KTI_API_H_

#include <Uefi.h>
#include <Upi/KtiHost.h>

#define  IIO_LINK_STATUS_MASK  0x03  //IIO_LINK_STATUS only has 2 bit encoding

typedef enum {
  NotTrained = 0,          //The IIO Link's initial status before Link Training
  NotSupportCxlMode,       //The IIO Link does not support CXL mode
  NotInCxlMode,            //The IIO Link can support CXL mode but currently not in CXL mode
  AlreadyInCxlMode         //The IIO Link can support CXL mode and already in CXL mode
} IIO_LINK_STATUS;

enum TYPE_FEATURE_ENUM {
  TypeSnc = 0,
  TypeHitMe,
  TypeKtiPrefetch,
  TypeDirectoryMode,
  TypeXptPrefetch,
  TypeIsoc,
  TypeFeatureMax
};

//
// 11:XCC; 10:HCC,UCC; 01:MCC; 00:LCC;
//
typedef enum {
  TypeLcc = 0,
  TypeMcc,
  TypeHcc,
  TypeUcc = 2,
  TypeXcc,
  TypeChopTypeMax = 0xff
} TYPE_CHOPTYPE_ENUM;

#define ONE_DIE_IN_SOCKET   1
#define TWO_DIE_IN_SOCKET   2
#define THREE_DIE_IN_SOCKET 3
#define FOUR_DIE_IN_SOCKET  4
#define INVALID_DIE_COUNT   0xFF

/**

  Check if feature specified by TypeFeature is enabled
  @param[in]    TypeFeature    - Type of Feature to be inspected

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IfFeatureEnabled (
  IN UINT8    TypeFeature
  );

/**

  Get Total Chas/LLCs. All CPUs must have same number of Cbo/LLCs
  @param[in] SocId    - Socket Index

  @retval    Tot Cha Count in the socket

**/
UINT8
EFIAPI
GetTotChaCount (
  IN UINT8    SocId
  );

/**

  Get Num Of Cluster Per System
  @param[in] none

  @retval    Num Of Cluster Per System

**/
UINT8
EFIAPI
GetNumOfClusterPerSystem (
  VOID
  );

/**

  Get UMA Clustering setting determined by UPI RC
  @param[in] none

  @retval    UMA Clustering mode

**/
UINT8
EFIAPI
GetKtiOutputUmaClusterSetting (
  VOID
  );

/**

  Check if GB Alignment required for SNC
  @param   none

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
SncGbAlignRequired (
  VOID
  );


/**

  Check system topology
  @param[in] SysTopology - Topology Type to be checked

  @retval    TRUE        Topology Type is same as input
  @retval    FALSE       Topology Type is not same as input

**/
BOOLEAN
EFIAPI
CheckSysConfig (
  IN UINT8    SysTopology
  );

/**

  Return if fpga is enabled in this socket or not
  @param[in] SocId    - Socket Index

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IfFpgaActive (
  IN UINT8     SocId
  );

/**

  Get IRQ Threshold
  @param   none

  @retval    IRQ Threshold

**/
UINT8
EFIAPI
GetIRQThreshold (
  VOID
  );

/**

  Check if KTI RAS is in progress
  @param   none

  @retval    TRUE       KTIRC is called to handle a CPU/IOH O*L event
  @retval    FALSE

**/
BOOLEAN
EFIAPI
RasInProgress (
  VOID
  );


/**
   Determine the RAS (Reliability Availability Servicabliity)
   type configured for the system.

   @return System RAS type (see RasTypes.h for values)
**/
UINT8
GetSystemRasType (
  VOID
  );


/**

  Get RAS event socket ID
  @param   none

  @retval    RAS event socket ID

**/
UINT8
EFIAPI
GetRasSocId (
  VOID
  );

/**

  Check stack's personality
  @param[in] SocId    - Socket Index
  @param[in] StackId  - Stack Index
  @param[in] Personality - Personality Type to be checked

  @retval    TRUE       Personality Type is same as input
  @retval    FALSE      Personality Type is not same as input

**/
BOOLEAN
EFIAPI
CheckStackPersonality (
  IN UINT8     SocId,
  IN UINT8     StackId,
  IN UINT8     Personality
  );

/**

  Return if the stack indexed is present or not
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index

  @retval    TRUE       Stack present
  @retval    FALSE      Stack not present

**/
BOOLEAN
EFIAPI
IfStackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**

  Get Socket Resource
  @param[in] SocId    - Socket Index
  @param[out] SocketResouce    - pointer to SocketResouce

  @retval    KTI_CPU_RESOURCE

**/
VOID
EFIAPI
GetSocketResouce (
  IN UINT8     SocId,
  OUT KTI_CPU_RESOURCE *SocketResouce
  );

/**

  Get Stack Resource
  @param[in] SocId    - Socket Index
  @param[in] Stack    - Stack Index
  @param[out] StackResouce    - pointer to StackResouce

  @retval    KTI_STACK_RESOURCE

**/
VOID
EFIAPI
GetStackResouce (
  IN UINT8     SocId,
  IN UINT8     Stack,
  OUT KTI_STACK_RESOURCE *StackResouce
  );

/**

  Get IODC Mode: 0 - Disable IODC,  1 - AUTO (default), 2 - IODC_EN_REM_INVITOM_PUSH, 3 - IODC_EN_REM_INVITOM_ALLOCFLOW
  @param[in] none

  @retval    IODC Mode

**/
UINT8
EFIAPI
GetIoDcMode (
  VOID
  );

/**

  Get Segment Num
  @param[in] SocId

  @retval    SegmentNum

**/
UINT8
EFIAPI
GetSegmentNum (
  IN UINT8     SocId
  );

/**

  Get Request Threshold
  @param   none

  @retval  RequestThreshold

**/
UINT8
EFIAPI
GetRequestThreshold (
  VOID
  );

/**

  Get whether Board VS CPU conflicts.

  @param   none

  @retval  Board VS CPU conflicts

**/
UINT8
EFIAPI
GetBoardVsCpuConflicts (
  VOID
  );

/**

  Get KTI DebugPrintLevel.

  @param   none

  @retval  KTI DebugPrintLevel

**/
UINT8
EFIAPI
GetKtiDebugPrintLevel (
  VOID
  );

/**

  Set KTI DebugPrintLevel.

  @param   [in] DbgPrintLevel

  @retval  KTI DebugPrintLevel

**/
VOID
EFIAPI
SetKtiDebugPrintLevel (
  IN UINT8   DbgPrintLevel
  );

/**

  Check if KTI Cpu Socket HotPlug Enabled
  @param   none

  @retval    TRUE       KTI Cpu Socket HotPlug Enabled
  @retval    FALSE

**/
BOOLEAN
EFIAPI
KtiCpuSktHotPlugEnabled (
  VOID
  );

/**

  Get KTI LEP info
  @param[in]  SocId      - Socket Index
  @param[in]  PortId     - Port Index
  @param[out] KtiLepInfo - pointer to KtiLepInfo

  @retval    KTI_LINK_DATA

**/
VOID
EFIAPI
GetKtiLepInfo (
  IN UINT8     SocId,
  IN UINT8     PortId,
  OUT KTI_LINK_DATA *KtiLepInfo
  );

/**

  Get KTI Cpu Setting
  @param[in]  SocId         - Socket Index
  @param[out] KtiCpuSetting - KTI_CPU_SETTING structure

  @retval    TRUE  - Get KtiCpuSetting success
  @retval    FALSE - Get KtiCpuSetting Fail

**/
BOOLEAN
EFIAPI
GetPhyLinkPerPortSetting (
  IN  UINT8           SocId,
  OUT KTI_CPU_SETTING *KtiCpuSetting
  );

/**

  Return if the socket indexed is present or not
  @param[in] SocId    - Socket Index

  @retval    TRUE       Socket present
  @retval    FALSE      Socket not present

**/
BOOLEAN
EFIAPI
SocketPresent (
  IN UINT32     SocId
  );

/**

  Get current socketPresentBitMap
  @retval    current socketPresentBitMap

**/
UINT32
EFIAPI
GetSocketPresentBitMap (
  VOID
  );

/**

  Set socketPresentBitMap
  @param[in] SocketPresentBitMap     - SocketPresentBitMap to be set
  @retval    current socketPresentBitMap

**/
VOID
EFIAPI
SetSocketPresentBitMap (
  UINT32       SocketPresentBitMap
  );

/**

  Override CxlVid to 0x1e98 or not
  @param[in] SocId    - Socket Index

  @retval    Set CxlVid to 0x1e98 or not

**/
BOOLEAN
EFIAPI
OverrideCxlVid (
  VOID
  );

/**

  Get mmCfgBase
  @retval    mmCfgBase

**/
UINT32
EFIAPI
GetmmCfgBase (
  VOID
  );

/**

  Get mmCfgSize
  @retval    mmCfgSize

**/
UINT32
EFIAPI
GetmmCfgSize (
  VOID
  );

/**

  Get LowGap
  @retval    LowGap

**/
UINT8
EFIAPI
GetLowGap (
  VOID
  );

/**

  Get mmCfgBaseH in requested Socket
  @param[in] SocId    - Socket Index

@retval                 mmCfgBaseH
**/
UINT32
EFIAPI
GetmmCfgBaseH (
  IN UINT32     SocId
  );

/**

  Get mmCfgBaseL in requested Socket
  @param[in] SocId    - Socket Index

@retval                 mmCfgBaseL
**/
UINT32
EFIAPI
GetmmCfgBaseL (
  IN UINT32     SocId
  );

/*++

  Checks top of low memory limit

  @param N/A

@retval Returns UINT16 value for TolLimit

--*/
UINT16
EFIAPI
GetKtiTolmLimit (
  VOID
  );

/*++

  Checks current type of reset

  @param CurrentReset   - Type of reset to be check

@retval Returns TRUE or FALSE

--*/
BOOLEAN
EFIAPI
CheckCurrentReset (
  UINT8     CurrentReset
  );

/**

  Get Kti Port Count
  @param[in] None
  @retval    current KtiPortCnt

**/
UINT8
EFIAPI
GetKtiPortCnt (
  VOID
  );

/**

  Set Kti Port Count
  @param[in] KtiPortCnt     - KtiPortCnt to be set

**/
VOID
EFIAPI
SetKtiPortCnt (
  UINT8 KtiPortCnt
  );

/**

  Get Kti Port Bitmap
  @param[in] SocId     - Socket ID
  @retval    current KtiPortCnt

**/
UINT8
EFIAPI
GetKtiPortPresentBitmap (
  UINT8 SocId
  );

/**

  Return if the stack indexed is present or not
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index

  @retval    TRUE       Stack present
  @retval    FALSE      Stack not present

**/
BOOLEAN
EFIAPI
StackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**

  Get current stackPresentBitMap
  @param[in] SocId    - Socket Index
  @retval    current stackPresentBitMap

**/
UINT16
EFIAPI
GetStackPresentBitMap (
  IN UINT8     SocId
  );

/**

  Set stackPresentBitMap for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index

**/
VOID
EFIAPI
SetStackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**

  Clear stackPresentBitMap for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index

**/
VOID
EFIAPI
ClearStackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**

  Get StackBus for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId  - Stack Index

@retval                 BusNum
**/
UINT8
EFIAPI
GetStackBus (
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**

  Set StackBus for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId  - Stack Index

**/
VOID
EFIAPI
SetStackBus (
  IN UINT8     SocId,
  IN UINT8     StackId,
  IN UINT8     BusNum
  );

/**

  Get Last Bus Number in requested Socket
  @param[in] SocId    - Socket Index

@retval                 BusNum
**/
UINT8
EFIAPI
GetSocketLastBus (
  IN UINT8     SocId
  );

/**

  Set Last Bus Number in requested Socket
  @param[in] SocId    - Socket Index

**/
VOID
EFIAPI
SetSocketLastBus (
  IN UINT8     SocId,
  IN UINT8     BusNum
  );

/**

  Get First Bus Number in requested Socket
  @param[in] SocId    - Socket Index

@retval                 BusNum
**/
UINT8
EFIAPI
GetSocketFirstBus (
  IN UINT8     SocId
  );

/**

  Set First Bus Number in requested Socket
  @param[in] SocId    - Socket Index

**/
VOID
EFIAPI
SetSocketFirstBus (
  IN UINT8     SocId,
  IN UINT8     BusNum
  );

/**

  Get the maximum used physical address bits (42, 46, 52, etc.)
  @param[in]          - none

  @retval              the maximum used physical address bits

**/
UINT8
EFIAPI
GetMaxPhysicalAddrBits (
  VOID
  );

/**

  This routine calculates and returns the 256MB boundary aligned Top Of Lower Memory (TOLM) address in units of 64MB

  @retval TOLM in units of 64MB

**/

UINT16
EFIAPI
GetTolmAddress (
  VOID
  );

/**

  Get the maximum consume physical address bits (42, 46, 52, etc.)
  @param[in]          - none

  @retval             - the maximum used physical address bits

**/
UINT8
EFIAPI
GetLatestConsumePA (
  VOID
  );

/**

Get Max Kti Agent
@param[in] SocId

@retval    MaxKtiAgnet

**/
UINT8
EFIAPI
GetMaxKtiAgent (
  IN UINT8    SocId
  );

/**

  Get DDRT Qos Enable

  @retval    DDRTQos Mode

**/
UINT8
EFIAPI
GetDdrtQosMode (
  VOID
  );

/**

  Get Socket's ChopType
  @param[in] SocId

  @retval    Socket's Chop Type --- 11:XCC; 10:HCC; 01:MCC; 00:LCC;

**/
TYPE_CHOPTYPE_ENUM
EFIAPI
GetChopType (
  UINT8 SocId
  );

/**

  Returns if SBSP Chop data is valid in KtiApi

  @retval    BOOLEAN - TRUE  - SBSP chop is ready
                       FALSE - Not ready

**/
BOOLEAN
EFIAPI
IsChopTypeValid (
  VOID
  );

/**

  Get Socket's Die Count
  @param[in] SocId

  @retval    The number of Die in the Socket

**/
UINT8
EFIAPI
GetDieCount (
  UINT8 SocId
  );

/**
  Get the CXL status of the IIO stack indexed by SocId/StackId.

  @param[in]    SocId             - Socket number
  @param[in]    StackId           - Stack number

  @retval       0/1/2/3           - Please refer to the encoding in enum IIO_LINK_STATUS
**/
UINT8
EFIAPI
GetCxlStatus (
  IN  UINT8     SocId,
  IN  UINT8     StackId
  );

/**
  Set SlaveDieBitMap. Bit is 1 indicates the socket is Slave Die, 0 indicates Master Die.
  This SlaveDieBitMap is a subset of SocketPresentBitMap.
  The bit in SlaveDieBitMap is only valid if the same bit is set to 1 in SocketPresentBitMap.

  @param[in] SlaveDieBitMap     - SlaveDieBitMap to be set.

**/
VOID
EFIAPI
SetSlaveDieBitMap (
  UINT32 SlaveDieBitMap
  );

/**

  Get SlaveDieBitMap. Bit is 1 indicates the socket is Slave Die, 0 indicates Master Die.
  This SlaveDieBitMap is a subset of SocketPresentBitMap.
  The bit in SlaveDieBitMap is only valid if the same bit is set to 1 in SocketPresentBitMap.

**/
UINT32
EFIAPI
GetSlaveDieBitMap (
  VOID
  );

/**
  Set the CXL status for the IIO stack indexed by SocId/StackId.

  @param[in]    SocId             - Socket number
  @param[in]    StackId           - Stack number
  @param[in]    Value             - Please refer to the encoding in enum IIO_LINK_STATUS

  @retval       VOID
**/
VOID
EFIAPI
SetCxlStatus (
  IN  UINT8     SocId,
  IN  UINT8     StackId,
  IN  UINT8     Value
  );

/**
  Set the CXL stack bit map in KTI_OUTPUT structure

  @param[in]    SocketId          - Socket number
  @param[in]    StackId           - Stack number

  @retval       VOID
**/
VOID
EFIAPI
SetCxlBitMap (
  IN  UINT8     SocketId,
  IN  UINT8     StackId
);

/**
  Save CXL Uncorrectable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based
  @param[in] ErrorStatus - The error status to be saved

  @retval    VOID
**/
VOID
EFIAPI
SaveCxlUncorrectableErrorStatus (
  IN UINT8   SocId,
  IN UINT8   StackId,
  IN UINT32  ErrorStatus
  );

/**
  Get CXL Uncorrectable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based

  @retval    The saved Uncorrectable Error Status or 0 when input invalid parameter.
**/
UINT32
EFIAPI
GetCxlUncorrectableErrorStatus (
  IN UINT8 SocId,
  IN UINT8 StackId
  );

/**
  Save CXL Correctable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based
  @param[in] ErrorStatus - The error status to be saved

  @retval    VOID
**/
VOID
EFIAPI
SaveCxlCorrectableErrorStatus (
  IN UINT8   SocId,
  IN UINT8   StackId,
  IN UINT32  ErrorStatus
  );

/**
  Get CXL Correctable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based

  @retval    The saved Correctable Error Status or 0 when input invalid parameter.
**/
UINT32
EFIAPI
GetCxlCorrectableErrorStatus (
  IN UINT8 SocId,
  IN UINT8 StackId
  );

/**

  Return if UPI Affinity feature enabled or not
  @param[in] VOID

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IsUpiAffinityEnabled (
  VOID
  );

/**

  Return if UPI Twisted links exist
  @param[in] VOID

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IsUpiTwistedLinks (
  VOID
  );

/**
  This API is invoked to get the current PCIE Gen4 Speed Limit setting of
  all stacks of the specified socket.

  @param[in] Socket      - Socket ID

  @retval    The current PCIE Gen4 Speed Limit setting of the specified socket.
             in which each bit map to a stack.
             1'b1: Limit to Gen4 Speed.
             1'b0: No limit, PCIE can go up to Gen5.
**/
UINT8
EFIAPI
GetPcieGen4SpeedLimit (
  IN  UINT8    Socket
  );

/**
  This API is invoked to update the PCIE Gen4 Speed Limit settings for each stack.
  Note: Stack[0] of Socket-L is for DMI, in this case the input LimitNeeded should be FALSE.

  @param[in] Socket      - Socket ID
  @param[in] Stack       - Stack ID
  @param[in] LimitNeeded - TRUE: Limit to Gen4 Speed; FALSE: No limit, PCIE can go up to Gen5.

  @retval    VOID
**/
VOID
EFIAPI
SetPcieGen4SpeedLimit (
  IN  UINT8    Socket,
  IN  UINT8    Stack,
  IN  BOOLEAN  LimitNeeded
  );

/**
  Return if it's HBM Sku
  @param[in] VOID

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IsHbmSku (
  VOID
  );

/**

  Get PCH present bit map. Bit seeting indicates the corresponding socket has legay PCH or PCH.IO expander present.
  For instance, 0x03 indicates socket 0 and socket 1 have Pches.

  @param[in] VOID

  @retval    UINT8       - PCH present bit map
**/
UINT8
EFIAPI
GetPchPresentBitMap (
  VOID
  );

/**
  Get the size of reserved MMIOL resouce for DMI mem bar.

  @param[in] UINT8  Socket  - Socket Index

  @retval    UINT32         - return the size of reserved MMIOL resouce for DMI mem bar.
**/
UINT32
EFIAPI
GetDmiMemBarSize (
  IN  UINT8   Socket
  );

/**
  This routine is used by IIO to pass the PI5 supported protocol information to KTI RC.

  @param[in] Socket       - Socket ID
  @param[in] Stack        - Stack ID
  @param[in] Protocol     - Current PI5 protocol supported, which is encoded as following:
                            0x00: PI5_IP_DISABLE_CONFIG_DMI_PCIEGEN4_ONLY_VALUE
                            0x01: PI5_IP_DISABLE_CONFIG_SINGLE_PROTOCOL_VALUE
                            0x02: PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE
                            0xFF: PI5_IP_DISABLE_CONFIG_INVALID_VALUE

  @retval    VOID
**/
VOID
EFIAPI
SetPi5ProtocolStatus (
  IN  UINT8    Socket,
  IN  UINT8    Stack,
  IN  UINT8    Protocol
  );

/**
  This routine is to get the PI5 supported protocol information of a given stack.

  @param[in] Socket       - Socket ID
  @param[in] Stack        - Stack ID

  @retval    Current supported PI5 protocol of the given stack, which is encoded as following:
                            0x00: PI5_IP_DISABLE_CONFIG_DMI_PCIEGEN4_ONLY_VALUE
                            0x01: PI5_IP_DISABLE_CONFIG_SINGLE_PROTOCOL_VALUE
                            0x02: PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE
                            0xFF: PI5_IP_DISABLE_CONFIG_INVALID_VALUE
**/
UINT8
EFIAPI
GetPi5ProtocolStatus (
  IN  UINT8    Socket,
  IN  UINT8    Stack
  );

/**

  Get the UPI txeq adaptation table Pointer

  @param[out] TablePointer    - pointer to UPI txeq adaptation table

  @retval
          - EFI_SUCCESS           Command succeeded.
          - EFI_INVALID_PARAMETER Invalid read data detected from pcode.
          - EFI_NOT_FOUND         No valid txeq adaptation table.

**/
EFI_STATUS
EFIAPI
GetUpiTxeqAdaptationTable (
  OUT KTI_ADAPTATION_TABLE *TablePointer
  );

/**
  This routine can be used to get the MicroCode Revision loaded on Sbsp.

  @param[in] VOID

  @retval    MicroCode Revision
**/
UINT32
EFIAPI
GetSbspMicrocodeRev (
  VOID
  );

/**

  Update KTI specific default policy into KtiHostInDataHob

  @param none

  @retval VOID

**/
VOID
GetKtiSetupOptions (
  VOID
  );

/**
  Get CXL port number.

  @param          - VOID

  @retval         The number of CXL Ports in this system.
**/
UINT8
EFIAPI
GetCxlPortNumber (
  VOID
  );

/**
  Get the MMIO32 resource size used by CXL DP/UP Port.

  @param[in]        Socket     -  Socket number
  @param[in]        Stack      -  Stack number

  @retval           MMIO32 resource size (Unit: in Byte)
**/
UINT32
EFIAPI
GetCxlDpUpReservedMmio32Size (
  IN  UINT8       Socket,
  IN  UINT8       Stack
  );

/**

  Returns if the UPI port is the UPIMDF subsystem

  @param[in] Socket  - Socket Index
  @param[in] UpiPort - UPI Port number

  @retval    BOOLEAN - TRUE  - The port is a UPIMDF subsystem
                       FALSE - The port is normal UPI port

**/
BOOLEAN
EFIAPI
IsUpiPortUpimdf (
  IN UINT8      Socket,
  IN UINT8      UpiPort
  );

#endif // _KTI_API_H_
