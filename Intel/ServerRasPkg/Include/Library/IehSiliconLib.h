/** @file
  Interface of IEH silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef __IEH_SILICON_LIB_H__
#define __IEH_SILICON_LIB_H__

#define IEH_DID_VID     0x09988086
//
// Wave3 satellite IEH have Device ID 0x0B23
//
#define IEH_DID_VID2    0x0B238086

#define PCH_IEHTYPE_REG         0x26C
#define GLOBAL_IEH_VALUE        0x00
#define NORTH_IEH_VALUE         0x01
#define South_IEH_VALUE         0x02

#define IEH_CORRECT_ERROR     BIT0
#define IEH_NON_FATAL_ERROR   BIT1
#define IEH_FATAL_ERROR       BIT2
/**

  Get the Ieh in this tack

  @param [in]  Socket  --  Socket index
  @param [in]  Stack   --  Stack index
  @param [in out] *BusNum -- Input Bus Number, and can be updated if necessary
  @param [out] *Dev    --  Device address
  @param [out] *Func    --  Function address
  @param [out] *IehDevType    --  Ieh Device Type
  @param [Out] *IehVersion    --  Ieh Version

  @retval TRUE it is IEH, FALSE it is not IEH
**/
BOOLEAN
EFIAPI
GetIeh (
  IN     UINT8  Socket,
  IN     UINT8  Stack,
  IN OUT UINT8  *BusNum,
  OUT    UINT8  *Dev,
  OUT    UINT8  *Func,
  OUT    UINT8  *IehDevType,
  OUT    UINT8  *IehVersion
  );

/**

  Get LCL_ERR_SRC from register, it is the local error source count for Wave 3 IEH

  @param [IN] Ieh -- pointer to IEH.

  @retval Local Error source count, return 0  if it is not wave3 IEH
**/
UINT8
EFIAPI
GetLclErrSrc (
  IN  NODE_DEVICE *Ieh
  );

/**
  This routine purpose is to Set Global/satellite IEH Bus Number

  @param [IN]   Ieh  --  IEH pointer

  @retval  none

**/

VOID
EFIAPI
ProgramIehBusNumber (
  NODE_DEVICE * Ieh
  );

/**
  This routine purpose is to Set satellite IEH Bitmap

  @param [IN]   Ieh  --  IEH pointer

  @retval  none

**/

VOID
EFIAPI
SetSatIehBitmap (
  NODE_DEVICE * Ieh
  );

/**
  This routine purpose is to dump ieh internal device error status.

  @param  [IN]  Ieh      -- IEH pointer

  @retval none

**/
VOID
EFIAPI
DumpIehLocalErrorStatus (
  IN  NODE_DEVICE    *Ieh
  );

/**
  This routine purpose is to dump ieh Global error status.

  @param  [IN]  Ieh      -- IEH pointer

  @retval none

**/
VOID
EFIAPI
DumpIehGlobalErrorStatus (
  IN  NODE_DEVICE    *Ieh
  );

/**
  This routine purpose is to clear ieh internal device error status.

  @param [IN]   Socket  --  socket index
  @param [IN]   Bus  --     Bus number
  @param [IN]   Device  --  Device number
  @param [IN]   Function -- Function number
  @param  [IN]  Index    -- device bitmap in IEH
  @param [IN]   Severity  -- error severity

  @retval none

**/
VOID
EFIAPI
ClearIehErrorStatusPerBitSi (
  IN  UINT8           Socket,
  IN  UINT8           Bus,
  IN  UINT8           Device,
  IN  UINT8           Function,
  IN  UINT8           Index,
  IN  UINT32          Severity,
  IN  IEH_DEVICE_TYPE Type
  );

/**
  This routine purpose is to clear ieh internal device error status.

  @param [IN]   Ieh -- IEH pointer
  @param [IN]   ErrSrcId -- Local Error Source Id, for wave1/2, this is always 0
  @param [IN]   ErrSeverity  -- error severity

  @retval none

**/
VOID
EFIAPI
ClearIehLocalErrorStatus (
  IN  NODE_DEVICE   *Ieh,
  IN  UINT8         ErrSrcId,
  IN  UINT32        ErrSeverity
  );

/**
  This routine purpose is to clear ieh Global error status.

  @param [IN]   Socket  --  socket index
  @param [IN]   Bus  --     Bus number
  @param [IN]   Device  --  Device number
  @param [IN]   Function -- Function number
  @param [IN]   Type  --    IEH Device Type
  @retval none

**/
VOID
EFIAPI
ClearIehGlobalErrorStatus (
  IN  UINT8           Socket,
  IN  UINT8           Bus,
  IN  UINT8           Device,
  IN  UINT8           Function,
  IN  IEH_DEVICE_TYPE Type
  );

/**
  Clear Error pin status.

  @param [IN]   Socket  --  socket index
  @param [IN]   Bus  --     Bus number
  @param [IN]   Device  --  Device number
  @param [IN]   Function -- Function number

  @retval none
..
**/
VOID
EFIAPI
ClearErrorPinStatus (
  IN  UINT8         Socket,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function
  );

/**
  Enable system error interrupt map.

  @param [IN]   Socket    --  socket index
  @param [IN]   Bus       --  Bus number
  @param [IN]   Device    --  Device number
  @param [IN]   Function  -- Function number
  @Param [IN]   IehDevType  -- IEH Type
  @Param [IN]   IoMcaEn  -- IoMCA Enabling Flag
  @retval none
..
**/
VOID
EFIAPI
SetGsysevtmapReg (
  IN  UINT8             Socket,
  IN  UINT8             Bus,
  IN  UINT8             Device,
  IN  UINT8             Function,
  IN  IEH_DEVICE_TYPE   IehDevType,
  IN  UINT8             IoMcaEn
  );

/**
  Enable/disable system error event register.

  @param [IN]   Socket    --  socket index
  @param [IN]   Bus       --  Bus number
  @param [IN]   Device    --  Device number
  @param [IN]   Function  --  Function number
  @Param [IN]   IehDevType  -- IEH Type
  @param [IN]   IehVer    --  IEH Version
  @Param [IN]...Enable  --  TRUE - enable, FALSE - disable

  @retval none
..
**/
VOID
EFIAPI
SetGsysevtctlReg (
  IN  UINT8             Socket,
  IN  UINT8             Bus,
  IN  UINT8             Device,
  IN  UINT8             Function,
  IN  IEH_DEVICE_TYPE   IehDevType,
  IN  UINT8             IehVer,
  IN  BOOLEAN           Enable
  );

/**
  Enable Error pin.

  @param [IN]   Socket  --  socket index
  @param [IN]   Bus  --     Bus number
  @param [IN]   Device  --  Device number
  @param [IN]   Function -- Function number
  @param [IN]   IioErrorPin0En -- Setup knob for Pin0
  @param [IN]   IioErrorPin1En -- Setup knob for Pin1
  @param [IN]   IioErrorPin2En -- Setup knob for Pin2
  @retval none
..
**/
VOID
EFIAPI
SetErrpinctlReg (
  IN  UINT8         Socket,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  UINT8         IioErrorPin0En,
  IN  UINT8         IioErrorPin1En,
  IN  UINT8         IioErrorPin2En
  );

/**
  Set specific MCChan with LBITMAP.

  All McChan under MC share the same bitmap value.

  @param[in]  Socket          Socket index.
  @param[in]  Instance        McChan index.
  @param[in]  LBitmap         Bitmap value to be set.
  @param[in]  Bus             Bus number
  @param[in]  Device          Device number
  @param[in]  Function        Function number
**/
VOID
EFIAPI
SetMcLBitmapMcChan (
  IN  UINT8   Socket,
  IN  UINT8   Instance,
  IN  UINT8   LBitmap,
  IN  UINT8   Bus,
  IN  UINT8   Device,
  IN  UINT8   Function
  );

/**
  This function is to set PcieGen5Ial BitMap

  @param[in] Socket          Socket Number
  @param[in] Stack           Stack Number
  @param[in] SbPortId        Side Band Port Id

  @retval    None
**/
VOID
EFIAPI
SetPiGen5IalBitmap (
  IN UINT8       Socket,
  IN UINT8       Stack,
  IN UINT32      SbPortId
  );

/**
  This routine is to get the value of GSYSEVTSTS_IEH_GLOBAL_REG

  @param [IN]   socket  --  socket index
  @param [IN]   Bus  --     Bus number
  @param [IN]   Device  --  Device number
  @param [IN]   Function -- Function number
  @Param [IN]   Global  --  TRUE for Global IEH, FALSE for Satellite IEH

  @retval the value of GSYSEVTSTS_IEH_GLOBAL_REG

**/
UINT32
EFIAPI
GetGlobalSysEvtSts (
  IN  UINT8         socket,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  BOOLEAN       Global
  );

/**
  This routine is to get the status value of a single Local Error Source

  @param [IN]   Ieh -- Which IEH we are proccessing
  @param [IN]   ErrSrcId -- Local Error Source ID
  @param [IN]   ErrSeverity  --  Severity input

  @retval the status of specific local error source

**/
UINT32
EFIAPI
GetIehLocalErrSrcStatus (
  IN  NODE_DEVICE   * Ieh,
  IN  UINT8         ErrSrcId,
  IN  UINT8         ErrSeverity
  );

/**
  This routine is to get the error source by checking Local UNC First Error pointer

  @param [IN]   Ieh -- Which IEH we are proccessing
  @param [OUT]  *ErrSrcId -- to store Local Error Source ID
  @param [OUT]  *ErrType --  to store error type

  @retval VOID

**/
VOID
EFIAPI
GetIehLocalUncFirstErr (
  IN  NODE_DEVICE   *Ieh,
  OUT UINT8         *ErrSrcId,
  OUT UINT8         *ErrType
  );

/**
  This routine is to get the value of GFAERRSTS_IEH_GLOBAL_REG

  @param [IN]   Socket  --    Socket index
  @param [IN]   Bus  --       Bus number
  @param [IN]   Device  --    Device number
  @param [IN]   Function --   Function number
  @Param [IN]   IehDevType -- IEH Type

  @retval the value of GFAERRSTS_IEH_GLOBAL_REG

**/
UINT32
EFIAPI
GetGlobalFatErrSts (
  IN  UINT8           Socket,
  IN  UINT8           Bus,
  IN  UINT8           Device,
  IN  UINT8           Function,
  IN  IEH_DEVICE_TYPE IehDevType
);

/**
  This routine is to get the value of GNFERRSTS_IEH_GLOBAL_REG

  @param [IN]   Socket  --    Socket index
  @param [IN]   Bus  --       Bus number
  @param [IN]   Device  --    Device number
  @param [IN]   Function --   Function number
  @Param [IN]   IehDevType -- IEH Type

  @retval the value of GNFERRSTS_IEH_GLOBAL_REG

**/
UINT32
EFIAPI
GetGlobalNfatErrSts (
  IN  UINT8           Socket,
  IN  UINT8           Bus,
  IN  UINT8           Device,
  IN  UINT8           Function,
  IN  IEH_DEVICE_TYPE IehDevType
);

/**
  This routine is to get the value of GCOERRSTS_IEH_GLOBAL_REG

  @param [IN]   Socket  --    Socket index
  @param [IN]   Bus  --       Bus number
  @param [IN]   Device  --    Device number
  @param [IN]   Function --   Function number
  @Param [IN]   IehDevType -- IEH Type

  @retval the value of GCOERRSTS_IEH_GLOBAL_REG

**/
UINT32
EFIAPI
GetGlobalCorErrSts (
  IN  UINT8           Socket,
  IN  UINT8           Bus,
  IN  UINT8           Device,
  IN  UINT8           Function,
  IN  IEH_DEVICE_TYPE IehDevType
);

/**

  Check Silicon specific Ieh End Point Type

  @param [in]  DevId  --  Vendor ID and Device ID
  @param [in]  IehEndPointType     --  return type

  @retval  TRUE - Type is valid  FASLE - Type is invalid
**/
BOOLEAN
EFIAPI
GetIehEndPointTypeSilicon (
  IN    UINT32              DevId,
  OUT   IEH_END_POINT_TYPE  *IehEndPointType
  );

/**
  This routine purpose is to Program Local Error Source Mask bit

  @param [IN]   Ieh -- Which IEH we are proccessing
  @param [IN]   ErrSrcId -- Local Error Source ID, for wave1/2 this is always 0
  @param [IN]   CorrErrMask  --  Correctable Error Mask value
  @param [IN]   UnCorrErrMask  --  Uncorrectable Error Mask value

  @retval none

**/
VOID
EFIAPI
SetIehLocalErrorMask (
  IN  NODE_DEVICE * Ieh,
  IN  UINT8       ErrSrcId,
  IN  UINT32      CorrErrMask,
  IN  UINT32      UnCorrErrMask
);

/**
  This routine is to Set the override of root control and root error registers

  @param [IN]   Socket  --  socket index
  @param [IN]   Bus  --     Bus number
  @param [IN]   Device  --  Device number
  @param [IN]   Function -- Function number

  @retval None

**/
VOID
EFIAPI
SetIioRootCtlOverride (
  IN  UINT8         Socket,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function
);

/**
  This routine is to get the value of GSYSEVTCTL_IEH_GLOBAL_REG

  @param [IN]   socket  --  socket index
  @param [IN]   Bus  --     Bus number
  @param [IN]   Device  --  Device number
  @param [IN]   Function -- Function number
  @Param [IN]   Global  --  TRUE for Global IEH, FALSE for Satellite IEH

  @retval the value of GSYSEVTCTL_IEH_GLOBAL_REG

**/
UINT32
EFIAPI
GetGlobalSysEvtCtl (
  IN  UINT8         Socket,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  BOOLEAN       Global
  );

/**
  This routine purpose is to clear satellite IEH agent source Ids

  @param [IN]   Ieh  --  IEH pointer

  @retval  none

**/
VOID
EFIAPI
ClearAgentSrcId (
  IN NODE_DEVICE * Ieh
);
#endif
