/** @file
  Internal header file for IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _IEH_RAS_LIB_H_
#define _IEH_RAS_LIB_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Library/RasDebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/IioRasLib.h>
#include <Library/IioSiliconLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/SystemInfoLib.h>
#include <IehTypes.h>
#include <Library/PcieStdLib.h>
#include <Library/PcieRasLib.h>
#include <Library/PchRasLib.h>
#include <Library/CxlRasLib.h>
#include <Library/SmbusRecLib.h>
#include <PCIeErrorTypes.h>
#include <Upi/KtiSi.h>
#include <RcRegs.h>
#include <Register/IioRegDef.h>
#include <Register/PcieRegDef.h>
#include <Library/LookupTable.h>
#include <Library/IehSiliconLib.h>
#include <Library/ErrorReportLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/RasAcpiLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CsrRegTableLib.h>

#define MAX_IEH_DEVICE  0x40
#define MAX_IEH_CHILD   0x20
#define FIND_ALL_DEVICE 0xff


#define IEH_GSYSEVTSTS_MASK   7

/**
  Check Ieh lookup table entry validation

  @param   LookupTblEntry  -- pointer to a LookupTblEntry

  @retval  TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsLookupTableEntryValid (
  IN  LOOKUP_TBL_ENTRY  *LookupTblEntry
  );

/**
  get Ieh lookup table entry pointer per stack, bitmap and SharedIndex

  @param   Stack  -- stack index
  @param   BitIndex -- bitmap index
  @param   SharedIndex -- The device index that share the same BitIndex, if not shared it should be 0
  @retval  bitmap table pointer

**/
LOOKUP_TBL_ENTRY *
EFIAPI
GetIehLookupTableEntry (
  IN  UINT8           Stack,
  IN  UINT8           BitIndex,
  IN  UINT8           SharedIndex
  );

/**
  Check if south Ieh in lookup table

  @param   None

  @retval  TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsSouthIehInLookupTable (
  VOID
  );

/**
  Display IEH device Type to serial port.

  @param   IehDevType  -- Ieh device Type
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayIehDevType (
  IN  IEH_DEVICE_TYPE IehDevTye,
  IN  BOOLEAN         CrLf
  );

/**
  Display Device Type that direct connect to IEH to serial port.

  @param   IehEndPointType  -- Device Type that direct connect to IEH
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayIehEndPointDevType (
  IN  IEH_END_POINT_TYPE  IehEndPointType,
  IN  BOOLEAN             CrLf
  );

/**
  Display IEH Local device Type to serial port.

  @param   IehLclDevType  -- Ieh Local device Type
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayLocalDeviceType (
  IN  IEH_LOCAL_DEV_TYPE  IehLclDevType,
  IN  BOOLEAN             CrLf
  );

/**
  Display IEH Device sub Type to serial port.

  @param   Node  --  device Node
  @param   CrLf  -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayIehDevSubType (
  IN  NODE_DEVICE   * Node,
  IN  BOOLEAN       CrLf
  );

/**
  Display Error Severity.

  @param   SeverityStatus  -- SeverityStatus
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplaySeverityStatus (
  IN  UINT32            SeverityStatus,
  IN  BOOLEAN           CrLf
  );

/**

  Check IEH end point device Type

  @param [in]  Socket  --  Socket index
  @param [in]  Bus     --  Bus address
  @param [in]  Device  --  Device address
  @param [in]  Func    --  Function address

  @retval device type
**/
IEH_END_POINT_TYPE
EFIAPI
GetIehEndPointType (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  );

/**

  Get particualr IEH counter from array list.

  @param [IN]   TempIeh   --  IEH array point
  @param [IN]   Socket    --  socket index to search
  @param [IN ]  Type     --   IEH Type
  @param [OUT]  DeviceCount -- IEH counter of this Type.

  @retval TRUE  -- particular IEH exist.
          FALSE -- particualr IEH not exist.

**/
BOOLEAN
EFIAPI
GetIehDeviceCount (
  IN     NODE_DEVICE      *TempIeh,
  IN     UINT8            Socket,
  IN     IEH_DEVICE_TYPE  Type,
  OUT    UINT8            *DeviceCount
  );

/**
 This routine 's purpose is to search IEH devices from whole system.

 @param [IN,OUT]  IEH pointer to save IEH device information
 @retval  IEH device number. If none, return 0.
**/
UINT8
EFIAPI
SearchIehDevice (
  IN OUT  NODE_DEVICE *TempIEH
  );

/**
  Search IEH array to find specific type IEH.

  @param [IN]   TempIeh   --  IEH array point
  @param [IN]   Socket    --  Socket index to search
  @param [IN]   Stack     --  Stack index to search, 0xff means don't care stack
  @param [IN]   Type      --  IEH type
  @param [IN]   Index     --  index of IEH array point, 0xff means search all specific type IEH

  @retval TRUE  -- found the device, otherwise not found.

**/
BOOLEAN
EFIAPI
FindIehDevice (
  IN     NODE_DEVICE      *TempIeh,
  IN     UINT8            Socket,
  IN     UINT8            Stack,
  IN     IEH_DEVICE_TYPE  Type,
  IN OUT UINT8            *IndexIEH
  );

/**
  This routine purpose is to creat whole system IEH tree.

  @param [IN] TempIEH  -- IEH device pointer.
  @retval none.
**/
VOID
EFIAPI
BuildIehTree (
  IN  NODE_DEVICE  * TempIEH
  );

/**
  Print created tree.
**/
VOID
EFIAPI
PrintTree (
  VOID
  );

/**
  This implementation is to set each MCChan with LBITMAP.

  All McChan under MC share the same bitmap value.

  @param  Socket -- socket index
  @param  Mc -- Mc index
  @param  Ch -- Channel index
  @param  LBitmap  -- bitmap value
  @param  Bus         Bus value.
  @param  Device      Device value
  @param  Function    Function value

  @retval  none.
**/
VOID
EFIAPI
SetMcLBitmap (
  IN    UINT8    Socket,
  IN    UINT8    Mc,
  IN    UINT8    Ch,
  IN    UINT8    LBitmap,
  IN    UINT8    Bus,
  IN    UINT8    Device,
  IN    UINT8    Function
  );

/**
  This implementation is to Program ABM, AbnLbn, AbnNbn for RCEC

  @param  Ieh -- Which IEH we are proccessing

  @retval  none.
**/
VOID
EFIAPI
SetRcecAbmNbnLbn (
  IN  NODE_DEVICE * Ieh
  );

/**
  This routine purpose is to dump IEH error status.

  @param  [IN]  IEH      -- IEH pointer

  @retval  none
**/
VOID
EFIAPI
DumpIehErrorStatus (
  IN  NODE_DEVICE    *IEH
  );

/**
  This routine purpose is to dump IEH Node error status.

  @param  NodeList -- Device array that include IEH Local error source and RCiEP devices
  @param  DevCount -- How many device in this list
  @param  SatIehFlag -- TRUE it is satellite IEH

  @retval  none
**/
VOID
EFIAPI
DumpIehNodeErrorStatus (
  NODE_DEVICE    *NodeList,
  UINT8          DevCount,
  BOOLEAN        SatIehFlag
  );

/**
  This routine purpose is to dump IEH Node error status.

  @param   none

  @retval  none
**/
VOID
EFIAPI
DumpIehSysErrorStatus (
  VOID
  );

/**
  This routine purpose is to clear IEH device error status for a single bit.

  @param  [IN]  IEH      -- IEH pointer
  @param  [IN]  Index    -- device bitmap in IEH
  @param  [IN]  severity -- error severity

  @retval  none
**/
VOID
EFIAPI
ClearIehErrorStatusPerBit (
  IN  NODE_DEVICE    *IEH,
  IN  UINT8           Index,
  IN  UINT8           Severity
  );

/**
  disable system error event register.

  @param [IN]   Ieh  --  IEH Node

  @retval  none
..
**/
VOID
EFIAPI
DisableSysCtl (
  IN   NODE_DEVICE   *Ieh
  );

/**
  Enable system error event register.

  @param [IN]   Ieh  --  IEH Node

  @retval none
..
**/
VOID
EFIAPI
EnableSysCtl (
  IN   NODE_DEVICE   *Ieh
  );

/**
  This routine purpose is to Clean IEH device error status.

  @param  [IN]  IEH      -- IEH pointer

  @retval  none
**/
VOID
EFIAPI
ClearIehErrorStatus (
  IN  NODE_DEVICE    *IEH
  );

/**

  Initialize the attribute of NODE Device according to Lookup Table.

  @param [IN]   DestIEH   --  The IEH that Node Device connect to
  @param [IN]   ChildNode    --  Node Device to initialize
  @param [IN ]  BitIndex     --  The bit index that the Node device connect to IEH
  @param [OUT]  ShareIndex --  The index to the device list that share the same Bit Map

  @retval EFI_SUCCESS  -- Initialize success.

**/
EFI_STATUS
EFIAPI
InstantiateChildNode (
  IN     NODE_DEVICE   *DestIEH,
  IN     NODE_DEVICE   *ChildNode,
  IN     UINT8          BitIndex,
  IN     UINT8          SharedIndex
  );

/**
  This routine is to search IEH device in the system, and create IEH tree.
  No register writing allowed at create tree phase.
  If any register need to set, please do it at enable phase.

  @param [IN]  GlobalIEH  -- global IEH pointer
  @retval  none

**/
EFI_STATUS
EFIAPI
InitializeIEH (
  );

/**
  get device counter. wave1/2 -- get from lookup table; wave 3 -- read from regiser.

  @param   Ieh  -- ieh device pointer.

  @retval  device counter value.

**/
UINT8
EFIAPI
GetDevCount (
  IN  NODE_DEVICE   *Ieh
  );

/**
  To return how many device connect to the same bit.

  @param   Ieh  -- ieh device pointer.
  @param   BitIndex  -- bitmap index.

  @retval  Device count that connect to BitIndex

**/
UINT8
EFIAPI
GetDevCountOnBit (
  IN  NODE_DEVICE     *Ieh,
  IN  UINT8           BitIndex
  );

/**
  get Ieh stack max direct connect device number.

  @param   Ieh  -- ieh device pointer.

  @retval  number of direct connect device.

**/
UINT8
EFIAPI
GetMaxBitMap (
  IN  NODE_DEVICE  *Ieh
  );

/**
  This routine purpose is to enable satellite IEH.

  @param [IN]   SatelliteIeh  -- Satellite IEH pointer

  @retval  none

**/

VOID
EFIAPI
SatalliteIehInit (
  NODE_DEVICE * SatelliteIeh
  );

/**
  This routine purpose is to handle global IEH and north IEH.

  @param [IN]  IEH  -- pointer to global IEH or north IEH.
  @param [IN]  InputSeverity  -- The severity that this handler will process.

  @retval none

**/
VOID
EFIAPI
IEHHandler (
  IN  NODE_DEVICE   * IEH,
  IN  UINT32        InputSeverity
  );
#endif
