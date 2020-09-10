/** @file
  Implementation of Iio  lib.

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


#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/RasDebugLib.h>
#include <IehTypes.h>
#include <Library/IehSiliconLib.h>
#include <RcRegs.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <Library/PchRasLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/WorkaroundLib.h>
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Library/CsrRegTableLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/SystemInfoLib.h>
#include "IehSiliconLibInternal.h"
#include <Library/IioTopologyLib.h>
#include <Upi/KtiSi.h>

extern RAS_MAIL_BOX  *mMailBox2;

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
  )
{
  IEHTYPEVER_IEH_SATELLITE_STRUCT  IEHTypeVer;
  UINT8                            IEHType;
  UINT32                           DevID;
  UINT8                            Bus;
  UINT8                            Device;
  UINT8                            Function;

  //
  //  All the input parameters are necessary, check invalid pointer
  //
  if (BusNum == NULL || Dev == NULL || Func == NULL || IehDevType == NULL || IehVersion == NULL) {
    return FALSE;
  }

  //
  // Validate stack and Get stack bus dev func number
  //
  if (Stack < MAX_IIO_STACK) {
    Device = NORTH_SAT_IEH_DEV;
    Function = NORTH_SAT_IEH_FUN;
    *Dev = Device;
    *Func = Function;
  } else if ((Stack == UBOX_STACK)) {
    Device = GLOBAL_IEH_DEV;
    Function = GLOBAL_IEH_FUN;
    *Dev = Device;
    *Func = Function;
  } else {
    return FALSE;
  }

  Bus = *BusNum;
  //
  // Get Ieh Information
  //
  DevID = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, 0, NULL));

  if (DevID == IEH_DID_VID) {
    IEHTypeVer.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(IEHTYPEVER_IEH_SATELLITE_REG), NULL));
    IEHType = (UINT8) IEHTypeVer.Bits.ieh_type;
    //
    // SouthCluster CDF IEH type register offset is different, so need to check both register offset
    //
    if (IEHType == 0xFF) {
      PchRasGetIehDevice (GetPcieSegment (Socket), Bus, NULL, NULL, NULL, &IEHType);
    }

    *IehVersion = (UINT8) IEHTypeVer.Bits.ieh_ver;

    if (IEHType == GLOBAL_IEH_VALUE) {
      *IehDevType = GlobalIeh;
      return TRUE;
    } else if (IEHType == NORTH_IEH_VALUE) {
      *IehDevType = SatelliteIehNorth;
      return TRUE;
    } else if (IEHType == South_IEH_VALUE) {
      *IehDevType = SatelliteIehSouth;
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

/**

  Get LCL_ERR_SRC from register, it is the local error source count for Wave 3 IEH

  @param [IN] Ieh -- pointer to IEH.

  @retval Local Error source count, return 0  if it is not wave3 IEH
**/
UINT8
EFIAPI
GetLclErrSrc (
  IN  NODE_DEVICE *Ieh
  )
{
  return 0;
}

/**
  This routine purpose is to Set Global/satellite IEH Bus Number

  @param [IN]   Ieh  --  IEH pointer

  @retval  none

**/

VOID
EFIAPI
ProgramIehBusNumber (
  NODE_DEVICE * Ieh
  )
{

  UINT8                           Skt;
  UINT8                           Bus;
  UINT8                           Dev;
  UINT8                           Func;
  UINT32                          IehTypeReg;
  IEHTYPEVER_IEH_SATELLITE_STRUCT IehTypeVer;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Func = Ieh->Common.Func;

  if (Ieh->Common.Type == SatelliteIehSouth) {
    IehTypeReg = PCH_IEHTYPE_REG;
  } else {
    IehTypeReg = ONLY_REGISTER_OFFSET(IEHTYPEVER_IEH_SATELLITE_REG);
  }

  //
  //program IEH register to update IEH Bus number and satellite IEH Bitmap
  //
  IehTypeVer.Data = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Func, IehTypeReg, NULL));
  IehTypeVer.Bits.ieh_busnum = Ieh->Common.Bus;
  MmioWrite32 (MmPciAddressExt (Skt, Bus, Dev, Func, IehTypeReg, NULL), IehTypeVer.Data);
  RAS_DEBUG((LEVEL_FUNC_FLOW,"  [IEH] Assgin bus number :0x%x \n", Bus));

}

/**
  This routine purpose is to Set satellite IEH Bitmap

  @param [IN]   Ieh  --  IEH pointer

  @retval  none

**/

VOID
EFIAPI
SetSatIehBitmap (
  NODE_DEVICE * Ieh
  )
{

  UINT8                           Skt;
  UINT8                           Bus;
  UINT8                           Dev;
  UINT8                           Func;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Func = Ieh->Common.Func;

  MmioWrite8 (MmPciAddressExt (Skt, Bus, Dev, Func, ONLY_REGISTER_OFFSET(BITMAP_IEH_SATELLITE_REG), NULL),
                                                                            Ieh->SatelliteIEH.BitIndex);
  RAS_DEBUG((LEVEL_FUNC_FLOW,"  [Sat IEH] Assgin Bit map :0x%x \n", Ieh->SatelliteIEH.BitIndex));

}

/**
  This routine purpose is to dump ieh internal device error status.

  @param  [IN]  Ieh      -- IEH pointer

  @retval none

**/
VOID
EFIAPI
DumpIehLocalErrorStatus (
  IN  NODE_DEVICE    *Ieh
  )
{
  UINT8            Skt;
  UINT8            Bus;
  UINT8            Dev;
  UINT8            Func;
  UINT32           LocalUcSts;
  UINT32           LocalCeSts;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Func = Ieh->Common.Func;

  LocalCeSts = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Func, ONLY_REGISTER_OFFSET(LERRCORSTS_IEH_SATELLITE_REG), NULL));
  LocalUcSts = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Func, ONLY_REGISTER_OFFSET(LERRUNCSTS_IEH_SATELLITE_REG), NULL));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "[IEH] Skt: 0x%x  Bus:0x%x  Dev:0x%x  Func:0x%x  LocalCeSts:0x%x LocalUcSts:0x%x\n", Skt, Bus, Dev, Func, LocalCeSts, LocalUcSts));
}

/**
  This routine purpose is to dump ieh Global error status.

  @param  [IN]  Ieh      -- IEH pointer

  @retval none

**/
VOID
EFIAPI
DumpIehGlobalErrorStatus (
  IN  NODE_DEVICE    *Ieh
  )
{
  UINT8    Socket;
  UINT8    Bus;
  UINT8    Device;
  UINT8    Function;
  UINT32   ErrorStatus;
  BOOLEAN  IsGlobalIeh;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  IsGlobalIeh = Ieh->Common.Type == GlobalIeh ? TRUE : FALSE;

  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GFAERRSTS_IEH_GLOBAL_REG), NULL));
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Socket 0x%x  Bus 0x%x Device 0x%x Function 0x%x  GFAERRSTS = 0x%x\n", Socket, Bus, Device, Function, ErrorStatus));
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GNFERRSTS_IEH_GLOBAL_REG), NULL));
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Socket 0x%x  Bus 0x%x Device 0x%x Function 0x%x  GNFERRSTS = 0x%x\n", Socket, Bus, Device, Function, ErrorStatus));
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GCOERRSTS_IEH_GLOBAL_REG), NULL));
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Socket 0x%x  Bus 0x%x Device 0x%x Function 0x%x  GCOERRSTS = 0x%x\n", Socket, Bus, Device, Function, ErrorStatus));
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GCOERRSTS_IEH_GLOBAL_REG), NULL));

  if (IsGlobalIeh) {
    ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GSYSEVTSTS_IEH_GLOBAL_REG), NULL));
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Socket 0x%x  Bus 0x%x Device 0x%x Function 0x%x  GSYSEVTSTS = 0x%x\n", Socket, Bus, Device, Function, ErrorStatus));
  }
}

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
  )
{
  UINT32   ErrorStatusReg = 0;
  UINT32   ErrorStatus;

  if (Severity ==  IEH_FATAL_ERROR) {
    ErrorStatusReg = ONLY_REGISTER_OFFSET(GFAERRSTS_IEH_GLOBAL_REG);
  }

  if (Severity ==  IEH_NON_FATAL_ERROR) {
    ErrorStatusReg = ONLY_REGISTER_OFFSET(GNFERRSTS_IEH_GLOBAL_REG);
  }

  if (Severity ==  IEH_CORRECT_ERROR) {
    ErrorStatusReg = ONLY_REGISTER_OFFSET(GCOERRSTS_IEH_GLOBAL_REG);
  }

  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ErrorStatusReg, NULL), (UINT32)1 << Index);

  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ErrorStatusReg, NULL));
}

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
  )
{
  UINT8     Socket;
  UINT8     Bus;
  UINT8     Device;
  UINT8     Function;
  UINT32    LocalUcSev;
  UINT32    LocalUcSts;
  UINT32    LocalCeSts;
  UINT32    Data32;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  if (ErrSeverity & IEH_CORRECT_ERROR) {
    LocalCeSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRCORSTS_IEH_SATELLITE_REG), NULL));
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRCORSTS_IEH_SATELLITE_REG), NULL), LocalCeSts);
    RAS_DEBUG((LEVEL_FUNC_FLOW, "[IEH] Skt: 0x%x  Bus:0x%x  Dev:0x%x  Func:0x%x  LocalCeSts:0x%x\n", Socket, Bus, Device, Function, LocalCeSts));
  }

  if (ErrSeverity & (IEH_NON_FATAL_ERROR | IEH_FATAL_ERROR)) {
    LocalUcSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRUNCSTS_IEH_SATELLITE_REG), NULL));
    LocalUcSev = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRUNCSEV_IEH_SATELLITE_REG), NULL));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "[IEH] Skt: 0x%x  Bus:0x%x  Dev:0x%x  Func:0x%x LocalUcSts:0x%x\n", Socket, Bus, Device, Function, LocalUcSts));
    //
    // Only clear error status corresponding severity
    //
    Data32 = 0;
    if (ErrSeverity & IEH_FATAL_ERROR) {
      if (LocalUcSts & LocalUcSev) {
        Data32 |= LocalUcSev;
      }
    }

    if (ErrSeverity & IEH_NON_FATAL_ERROR) {
      if (LocalUcSts &  (~LocalUcSev)) {
        Data32 |=( ~LocalUcSev);
      }
    }

    LocalUcSts = Data32;

    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRUNCSTS_IEH_SATELLITE_REG), NULL), LocalUcSts);

  }
}

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
  )
{
  UINT32   ErrorStatus;

  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GFAERRSTS_IEH_GLOBAL_REG), NULL));
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GFAERRSTS_IEH_GLOBAL_REG), NULL), ErrorStatus);
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GNFERRSTS_IEH_GLOBAL_REG), NULL));
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GNFERRSTS_IEH_GLOBAL_REG), NULL), ErrorStatus);
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GCOERRSTS_IEH_GLOBAL_REG), NULL));
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GCOERRSTS_IEH_GLOBAL_REG), NULL), ErrorStatus);

}

/**
  Clear Error pin status.

  @param [IN]   Socket  -- socket index
  @param [IN]   Bus  -- Bus number
  @param [IN]   Device  -- Device number
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
  )
{
  ERRPINSTS_IEH_GLOBAL_STRUCT   ErrorPinSts;

  ErrorPinSts.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(ERRPINSTS_IEH_GLOBAL_REG), NULL));
  ErrorPinSts.Data &= IEH_ERRPINSTS_MASK;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(ERRPINSTS_IEH_GLOBAL_REG), NULL), ErrorPinSts.Data);

}

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
  )
{
  GSYSEVTMAP_IEH_GLOBAL_STRUCT  ErrSysMap;

  if (IehDevType == GlobalIeh) {
    ErrSysMap.Data = 0;
    if (IoMcaEn) {
      //
      // Error PIN knob enabled, check for out of band mode knob and decide the value for severity map
      // otherwise set to default value
      //
      if (mMailBox2->IioInitPar.IioErrorPin0En) {
        if (mMailBox2->IioInitPar.IioOOBMode == INBAND_ENABLED) {
          ErrSysMap.Bits.co_sysev_map = GENERATE_SMI;
        } else if (mMailBox2->IioInitPar.IioOOBMode == INBAND_DISABLED) {
          ErrSysMap.Bits.co_sysev_map = NO_INBANDMSG;
        }
      } else {
        ErrSysMap.Bits.co_sysev_map = GENERATE_SMI;
      }
    } else {
      //
      // Control the SMI generation based on knob(IioOOBMode) and IIO pins
      // Error PIN knob enabled, check for out of band mode knob and decide the value for severity map
      // otherwise set to default value
      //
      if (mMailBox2->IioInitPar.IioErrorPin0En) {
        if (mMailBox2->IioInitPar.IioOOBMode == INBAND_ENABLED) {
          ErrSysMap.Bits.co_sysev_map = GENERATE_SMI;
        } else if (mMailBox2->IioInitPar.IioOOBMode == INBAND_DISABLED) {
          ErrSysMap.Bits.co_sysev_map = NO_INBANDMSG;
        }
      } else {
        ErrSysMap.Bits.co_sysev_map = GENERATE_SMI;
      }

      // check for PIN1
      if (mMailBox2->IioInitPar.IioErrorPin1En) {
        if (mMailBox2->IioInitPar.IioOOBMode == INBAND_ENABLED) {
          ErrSysMap.Bits.nf_sysev_map = GENERATE_SMI;
        } else if (mMailBox2->IioInitPar.IioOOBMode == INBAND_DISABLED) {
          ErrSysMap.Bits.nf_sysev_map = NO_INBANDMSG;
        }
      } else {
        ErrSysMap.Bits.nf_sysev_map = GENERATE_SMI;
      }

      // check for PIN2
      if (mMailBox2->IioInitPar.IioErrorPin2En) {
        if (mMailBox2->IioInitPar.IioOOBMode == INBAND_ENABLED) {
          ErrSysMap.Bits.fa_sysev_map = GENERATE_SMI;
        } else if (mMailBox2->IioInitPar.IioOOBMode == INBAND_DISABLED) {
          ErrSysMap.Bits.fa_sysev_map = NO_INBANDMSG;
        }
      } else {
        ErrSysMap.Bits.fa_sysev_map = GENERATE_SMI;
      }
    }
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GSYSEVTMAP_IEH_GLOBAL_REG), NULL), ErrSysMap.Data);
  }
}

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
  )
{
  GSYSEVTCTL_IEH_GLOBAL_STRUCT   ErrSysCtl;

  ErrSysCtl.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GSYSEVTCTL_IEH_GLOBAL_REG), NULL));
  //
  // CDF IEH has opposite definition for GSYSEVTCTL Register
  // CDF IEH named this regiter as GSYSEVTMSK, so need to reverse for CDF IEH
  //
  if (IehDevType == SatelliteIehSouth && IehVer == 0) {
    Enable = !Enable;
  }
  ErrSysCtl.Bits.co_sysev_m = Enable? 1:0;
  ErrSysCtl.Bits.nf_sysev_m = Enable? 1:0;
  ErrSysCtl.Bits.fa_sysev_m = Enable? 1:0;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GSYSEVTCTL_IEH_GLOBAL_REG), NULL), ErrSysCtl.Data);
}

/**
  Enable Error pin.

  @param [IN]   socket  --  socket index
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
  )
{
  ERRPINCTRL_IEH_GLOBAL_STRUCT  ErrPinCtl;

  ErrPinCtl.Data = 0;
  if (IioErrorPin0En) {
    ErrPinCtl.Bits.err0_enable = ASSERT_ERROR_PIN;
  }
  if (IioErrorPin1En) {
    ErrPinCtl.Bits.err1_enable = ASSERT_ERROR_PIN;
  }
  if (IioErrorPin2En) {
    ErrPinCtl.Bits.err2_enable = ASSERT_ERROR_PIN;
  }

  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(ERRPINCTRL_IEH_GLOBAL_REG), NULL), ErrPinCtl.Data);
}

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
  )
{
  UINT64                              MmioAddr;
  UINT8                               Size = 4;
  PCIE_ERROR_MSG_MC_2LM_STRUCT        PcieErrMsgMc2Lm;

  PcieErrMsgMc2Lm.Data = 0;
  PcieErrMsgMc2Lm.Bits.function= Function;
  PcieErrMsgMc2Lm.Bits.device = Device;
  PcieErrMsgMc2Lm.Bits.busnumber = Bus;
  PcieErrMsgMc2Lm.Bits.bitmap = LBitmap;

  MmioAddr = GetCpuCsrAddress (Socket, Instance, PCIE_ERROR_MSG_MC_2LM_REG, &Size);
  WriteCsrRegTable (MmioAddr, PcieErrMsgMc2Lm.Data);
}

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
  )
{
  //
  // WAVE12 not support
  //
}

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
  IN  UINT8         Socket,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  BOOLEAN       Global
  )
{

  return MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GSYSEVTSTS_IEH_GLOBAL_REG), NULL));

}

/**
  This routine is to get the status value of a single Local Error Source

  @param [IN]   Ieh -- Which IEH we are proccessing
  @param [IN]   ErrSrcId -- Local Error Source ID, for wave1/2 this is always 0
  @param [IN]   ErrSeverity  --  Severity input

  @retval the status of specific local error source

**/
UINT32
EFIAPI
GetIehLocalErrSrcStatus (
  IN  NODE_DEVICE   * Ieh,
  IN  UINT8         ErrSrcId,
  IN  UINT8         ErrSeverity
  )
{
  UINT32                Register;
  UINT8                 Socket;
  UINT8                 Bus;
  UINT8                 Device;
  UINT8                 Function;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  if (ErrSeverity & (IEH_NON_FATAL_ERROR | IEH_FATAL_ERROR)) {
    Register = ONLY_REGISTER_OFFSET(LERRUNCSTS_IEH_SATELLITE_REG);
  } else if (ErrSeverity & IEH_CORRECT_ERROR) {
    Register = ONLY_REGISTER_OFFSET(LERRCORSTS_IEH_SATELLITE_REG);
  } else {
    return 0;
  }
  return MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, Register, NULL));
}

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
  )
{
  //
  // Wave1/2 IEH doesn't support this
  //
  *ErrSrcId = 0;
  *ErrType = 0;
}

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
)
{
  return MmioRead32 (MmPciAddressExt (Socket,
                                        Bus,
                                        Device,
                                        Function,
                                        ONLY_REGISTER_OFFSET(GFAERRSTS_IEH_GLOBAL_REG),
                                        NULL));

}

/**
  This routine is to get the value of GNFERRSTS_IEH_GLOBAL_REG

  @param [IN]   socket  --    socket index
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
)
{
  return MmioRead32 (MmPciAddressExt (Socket,
                                        Bus,
                                        Device,
                                        Function,
                                        ONLY_REGISTER_OFFSET(GNFERRSTS_IEH_GLOBAL_REG),
                                        NULL));

}

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
)
{
  return MmioRead32 (MmPciAddressExt (Socket,
                                        Bus,
                                        Device,
                                        Function,
                                        ONLY_REGISTER_OFFSET(GCOERRSTS_IEH_GLOBAL_REG),
                                        NULL));

}

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
  )
{
  BOOLEAN       Found = FALSE;

  switch (DevId) {
    case VTD_DID_VID:
      *IehEndPointType = VTDDevice;
      Found = TRUE;
      break;
    case M2PCIE_DID_VID:
      *IehEndPointType = M2PCIeDevice;
      Found = TRUE;
      break;
    case RAS_DID_VID:
      *IehEndPointType = RASDevice;
      Found = TRUE;
      break;
    case CBDMA_DID_VID:
      *IehEndPointType = CbDMADevice;
      Found = TRUE;
      break;
    case SPD_DID_VID:
      *IehEndPointType = SPDDevice;
      Found = TRUE;
      break;
    case MCDDR_DID_VID:
      *IehEndPointType = McDDRDevice;
      Found = TRUE;
      break;
    case VPP_DID_VID:
      *IehEndPointType = VppDevice;
      Found = TRUE;
      break;
    default:
      Found = FALSE;
      break;
  }
  return Found;
}

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
  )
{
  UINT32 IehLocalUcSeverity;
  UINT8                  Socket;
  UINT8                  Bus;
  UINT8                  Device;
  UINT8                  Function;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  //
  // enable all IEH PSF severity bits
  //
  if (Ieh->Common.Type == SatelliteIehSouth) {
    IehLocalUcSeverity = mMailBox2->IioInitPar.IehLocalUcSeverity | 0xFFFFFF00;
    //
    //CDF IEH behavior of CSR parity error is inconsistent. In the event of a CSR parity error, IEH will keep on sending URs for
    //any register read and gests locked up.
    //due to this reason, WA is to
    //1. map CSR parity error condition to a fatal error.
    //2. during initialization when BIOS read CDF ieh VID/DID, sIEH is sending USs, BIOS gets 0xFFFFFFFF. BIOS treat it as a CSR parity error, and do global reset.
    //   this part code should be implemented in PCH package.
    //
    IehLocalUcSeverity = Wa_2006679835 (IehLocalUcSeverity);
  } else {
    IehLocalUcSeverity = mMailBox2->IioInitPar.IehLocalUcSeverity | 0x000FFF00;
  }

  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRUNCMSK_IEH_SATELLITE_REG), NULL), mMailBox2->IioInitPar.IehLocalUcMask);
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRUNCSEV_IEH_SATELLITE_REG), NULL), IehLocalUcSeverity);
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(LERRCORMSK_IEH_SATELLITE_REG), NULL), mMailBox2->IioInitPar.IehLocalCeMask);
}

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
)
{
  //
  // Wave1/2 don't support this
  //
}

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
  )
{
  return MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GSYSEVTCTL_IEH_GLOBAL_REG), NULL));
}

/**
  This routine purpose is to clear satellite IEH agent source Ids

  @param [IN]   Ieh  --  IEH pointer

  @retval  none

**/
VOID
EFIAPI
ClearAgentSrcId (
  NODE_DEVICE * Ieh
)
{
  //
  // No change needed
  //
}

