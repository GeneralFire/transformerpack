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
#include "Register/PcieRegDef.h"
#include <Library/PcieRasLib.h>
#include <Library/PcieStdLib.h>
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Library/CsrRegTableLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/SystemInfoLib.h>
#include "IehSiliconLibInternal.h"
#include <Library/IioTopologyLib.h>
#include <Upi/KtiSi.h>
#include <Library/CpuSbiAccessLib.h>
#include <Library/SiliconWorkaroundLib.h>

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
  IEHCAP_IEH_SATELLITE_STRUCT      IehCap;
  UINT8                            IehType;
  UINT32                           DevId;
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

  Bus  = *BusNum;

  //
  // Get Ieh Information
  //
  DevId = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, 0, NULL));

  if (DevId == IEH_DID_VID || DevId == IEH_DID_VID2) {
    IehCap.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(IEHCAP_IEH_SATELLITE_REG), NULL));
    IehType = (UINT8) IehCap.Bits.ieh_type;
    //
    // SouthCluster CDF IEH type register offset is different, so need to check both register offset
    //
    if (IehType == 0xFF) {
      PchRasGetIehDevice (GetPcieSegment (Socket), Bus, NULL, NULL, NULL, &IehType);
    }

    *IehVersion = (UINT8) IehCap.Bits.ieh_ver;

    if (IehType == GLOBAL_IEH_VALUE) {
      *IehDevType = GlobalIeh;
      return TRUE;
    } else if (IehType == NORTH_IEH_VALUE) {
      *IehDevType = SatelliteIehNorth;
      return TRUE;
    } else if (IehType == South_IEH_VALUE) {
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
  IEHCAP_IEH_SATELLITE_STRUCT       IehCap;
  UINT8                             LclErrSrc = 0;
  UINT8                             Skt;
  UINT8                             Bus;
  UINT8                             Dev;
  UINT8                             Fun;

  //
  // EBG IEH is legacy one with legacy register's layout
  //
  if (Ieh->Common.Type == SatelliteIehSouth) {
    return 0;
  }

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Fun = Ieh->Common.Func;

  IehCap.Data = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Fun, ONLY_REGISTER_OFFSET(IEHCAP_IEH_SATELLITE_REG), NULL));
  //
  // Note:LCL_ERR_SRC register value doesn't include error source 0.
  //
  LclErrSrc = (UINT8) (IehCap.Bits.lcl_err_src);
  return LclErrSrc;
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
  UINT32                          IehCapReg;
  IEHCAP_IEH_SATELLITE_STRUCT     IehCap;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Func = Ieh->Common.Func;

  if (Ieh->Common.Type == SatelliteIehSouth) {
    IehCapReg = PCH_IEHTYPE_REG;
  } else {
    IehCapReg = ONLY_REGISTER_OFFSET(IEHCAP_IEH_SATELLITE_REG);
  }

  //
  //program IEH register to update IEH Bus number and satellite IEH Bitmap
  //
  IehCap.Data = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Func, IehCapReg, NULL));
  IehCap.Bits.ieh_busnum = Ieh->Common.Bus;
  MmioWrite32 (MmPciAddressExt (Skt, Bus, Dev, Func, IehCapReg, NULL), IehCap.Data);
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
//
// WAVE3 don't need to Program Bitmap for satellite IEH
//
}

/**
  This routine is to Get Local Error registers offset to LERRUNCSTS0_IEH_SATELLITE_REG (0x280h)

  @param  Ieh -- Which IEH we are proccessing
  @param  RegType -- Register Type
  @param  LclErrSrcId -- Local Error Source ID

  @retval Register Offset

**/
UINT32
EFIAPI
GetLclErrRegOffset(
  IN  NODE_DEVICE     * Ieh,
  IN  LERR_REG_TYPE   RegType,
  IN  UINT8           LclErrSrcId
)
{
  UINT32  Offset;
  UINT32  LclErrSrc;

  //
  // EBG IEH is legacy one with legacy register's layout
  //
  if (Ieh->Common.Type == SatelliteIehSouth && LclErrSrcId == 0) {
    switch (RegType) {
      case LERRUNCSTS:
        return PCH_IEH_LERRUNCSTS_REG;
      case LERRUNCMSK:
        return PCH_IEH_LERRUNCMSK_REG;
      case LERRCORSTS:
        return PCH_IEH_LERRCORSTS_REG;
      case LERRCORMSK:
        return PCH_IEH_LERRCORMSK_REG;
      default:
        RAS_ASSERT(FALSE); //should not enter here
        return PCH_IEH_LERRUNCSTS_REG;
    }
  }

  if (Ieh->Common.Type == GlobalIeh) {
    LclErrSrc = Ieh->GlobalIEH.LocalErrorSource;
  } else {
    LclErrSrc = Ieh->SatelliteIEH.LocalErrorSource;
  }

  //
  // LCL_ERR_SRC register value doesn't include error source 0, so plus one upon it.
  //
  LclErrSrc += 1;

  if (RegType < 4) {
    Offset = RegType * 4 * LclErrSrc + LclErrSrcId * 4;
  } else if ( RegType == LERRUNCFEP){
    Offset = LERRUNCFEP * 4 * LclErrSrc;
  } else {
    Offset = 0;
  }

  return ONLY_REGISTER_OFFSET (LERRUNCSTS0_IEH_SATELLITE_REG) + Offset;
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
  UINT8             Skt;
  UINT8             Bus;
  UINT8             Dev;
  UINT8             Fun;
  UINT32            Register;
  UINT32            LocalCeSts;
  UINT32            LocalUcSts;
  UINT8             LclErrSrcId;
  UINT8             LclErrSrc;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Fun = Ieh->Common.Func;

  //
  // Get How many Local Error Source
  //
  if (Ieh->Common.Type == GlobalIeh) {
    LclErrSrc = Ieh->GlobalIEH.LocalErrorSource;
  } else {
    LclErrSrc = Ieh->SatelliteIEH.LocalErrorSource;
  }
  //
  // Dump status for each error source
  //
  for (LclErrSrcId = 0; LclErrSrcId <= LclErrSrc; LclErrSrcId++) {
    Register = GetLclErrRegOffset (Ieh, LERRCORSTS, LclErrSrcId);
    LocalCeSts = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL));

    Register = GetLclErrRegOffset (Ieh, LERRUNCSTS, LclErrSrcId);
    LocalUcSts = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "[IEH] Skt: 0x%x Bus:0x%x Dev:0x%x Fun:0x%x  Src:0x%x LocalCeSts:0x%x LocalUcSts:0x%x\n", Skt, Bus, Dev, Fun, LclErrSrcId, LocalCeSts, LocalUcSts));
  }
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
  UINT32   GCoErrStsReg;
  UINT32   GNfErrStsReg;
  UINT32   GFaErrStsReg;
  BOOLEAN  IsGlobalIeh;
  BOOLEAN  IsSouthIeh;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  IsGlobalIeh = Ieh->Common.Type == GlobalIeh ? TRUE : FALSE;
  IsSouthIeh = Ieh->Common.Type == SatelliteIehSouth ? TRUE : FALSE;

  GCoErrStsReg = IsSouthIeh ? PCH_IEH_GCOERRSTS_REG : ONLY_REGISTER_OFFSET(GERRCORSTS_IEH_SATELLITE_REG);
  GNfErrStsReg = IsSouthIeh ? PCH_IEH_GNFERRSTS_REG : ONLY_REGISTER_OFFSET(GERRNONSTS_IEH_SATELLITE_REG);
  GFaErrStsReg = IsSouthIeh ? PCH_IEH_GFAERRSTS_REG : ONLY_REGISTER_OFFSET(GERRFATSTS_IEH_SATELLITE_REG);

  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GFaErrStsReg, NULL));
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Socket 0x%x  Bus 0x%x Device 0x%x Function 0x%x  GFAERRSTS = 0x%x\n", Socket, Bus, Device, Function, ErrorStatus));
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GNfErrStsReg, NULL));
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Socket 0x%x  Bus 0x%x Device 0x%x Function 0x%x  GNFERRSTS = 0x%x\n", Socket, Bus, Device, Function, ErrorStatus));
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GCoErrStsReg, NULL));
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Socket 0x%x  Bus 0x%x Device 0x%x Function 0x%x  GCOERRSTS = 0x%x\n", Socket, Bus, Device, Function, ErrorStatus));

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
  BOOLEAN  IsSouthIeh;

  IsSouthIeh = Type == SatelliteIehSouth ? TRUE : FALSE;

  if (Severity ==  IEH_FATAL_ERROR) {
    ErrorStatusReg = IsSouthIeh ? PCH_IEH_GFAERRSTS_REG : ONLY_REGISTER_OFFSET(GERRFATSTS_IEH_SATELLITE_REG);
  }

  if (Severity ==  IEH_NON_FATAL_ERROR) {
    ErrorStatusReg = IsSouthIeh ? PCH_IEH_GNFERRSTS_REG : ONLY_REGISTER_OFFSET(GERRNONSTS_IEH_SATELLITE_REG);
  }

  if (Severity ==  IEH_CORRECT_ERROR) {
    ErrorStatusReg = IsSouthIeh ? PCH_IEH_GCOERRSTS_REG : ONLY_REGISTER_OFFSET(GERRCORSTS_IEH_SATELLITE_REG);
  }

  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ErrorStatusReg, NULL), (UINT32)1 << Index);

  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ErrorStatusReg, NULL));
}

/**
  This routine purpose is to clear ieh Local device error status.

  @param  Ieh -- Which IEH we are proccessing
  @param  LclErrSrcId -- Local Error Source ID
  @param [IN]   ErrSeverity  -- error severity

  @retval none

**/
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
  IN  NODE_DEVICE   * Ieh,
  IN  UINT8         LclErrSrcId,
  IN  UINT32        ErrSeverity
  )
{
  UINT8             Skt;
  UINT8             Bus;
  UINT8             Dev;
  UINT8             Fun;
  UINT32            Register;
  UINT32            Data32;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Fun = Ieh->Common.Func;

  if (ErrSeverity & IEH_CORRECT_ERROR) {
    Register = GetLclErrRegOffset (Ieh, LERRCORSTS, LclErrSrcId);
    Data32 = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL));
    if (Data32 != 0) {
      MmioWrite32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL), Data32);
    }
  }

  if (ErrSeverity & (IEH_NON_FATAL_ERROR | IEH_FATAL_ERROR)) {
    Register = GetLclErrRegOffset (Ieh, LERRUNCSTS, LclErrSrcId);
    Data32 = MmioRead32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL));
    if (Data32 != 0) {
      MmioWrite32 (MmPciAddressExt (Skt, Bus, Dev, Fun,  Register, NULL), Data32);
    }
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
  UINT32   GCoErrStsReg;
  UINT32   GNfErrStsReg;
  UINT32   GFaErrStsReg;
  BOOLEAN  IsSouthIeh;

  IsSouthIeh = Type == SatelliteIehSouth ? TRUE : FALSE;

  GCoErrStsReg = IsSouthIeh ? PCH_IEH_GCOERRSTS_REG : ONLY_REGISTER_OFFSET(GERRCORSTS_IEH_SATELLITE_REG);
  GNfErrStsReg = IsSouthIeh ? PCH_IEH_GNFERRSTS_REG : ONLY_REGISTER_OFFSET(GERRNONSTS_IEH_SATELLITE_REG);
  GFaErrStsReg = IsSouthIeh ? PCH_IEH_GFAERRSTS_REG : ONLY_REGISTER_OFFSET(GERRFATSTS_IEH_SATELLITE_REG);

  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GFaErrStsReg, NULL));
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, GFaErrStsReg, NULL), ErrorStatus);
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GNfErrStsReg, NULL));
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, GNfErrStsReg, NULL), ErrorStatus);
  ErrorStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GCoErrStsReg, NULL));
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, GCoErrStsReg, NULL), ErrorStatus);
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

  //
  // EBG IEH has opposite definition for GSYSEVTCTL Register
  // EBG IEH named this regiter as GSYSEVTMSK, so need to reverse for EBG IEH
  //
  if (IehDevType == SatelliteIehSouth) {
    Enable = !Enable;
  }

  ErrSysCtl.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(GSYSEVTCTL_IEH_GLOBAL_REG), NULL));
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
  SBERRMSG_SB_PRVT_PCIE_G5_SPRA0_STRUCT  CxlSbErrMesg;
  //
  // update IOSF Sidband Error Message Bitmap for CXL DP
  //
  CxlSbErrMesg.Data = CpuSbiReadPcrReg32 (Socket, SbPortId, ONLY_REGISTER_OFFSET(SBERRMSG_SB_PRVT_PCIE_G5_REG));
  // Init the bitmap with value provided by design
  CxlSbErrMesg.Bits.bitmap = 0x6;
  CpuSbiWritePcrReg32 (Socket, SbPortId, ONLY_REGISTER_OFFSET(SBERRMSG_SB_PRVT_PCIE_G5_REG), CxlSbErrMesg.Data);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "IOSF Sidband Error Message Bitmap: Socket:%d; Stack:%d;  0x%lx\n", Socket, Stack, CxlSbErrMesg.Data));
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
    Register = GetLclErrRegOffset (Ieh, LERRUNCSTS, ErrSrcId);
  } else if (ErrSeverity & IEH_CORRECT_ERROR) {
    Register = GetLclErrRegOffset (Ieh, LERRCORSTS, ErrSrcId);
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
  UINT8                             Socket;
  UINT8                             Bus;
  UINT8                             Device;
  UINT8                             Function;
  UINT32                            Register;
  REG_LERRUNCFEP_IEH_GLOBAL_STRUCT  LerrUncFep;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  //
  // for error type LERRUNCFEP, the Local error source id is not relevant
  //
  Register = GetLclErrRegOffset (Ieh, LERRUNCFEP, 0);
  LerrUncFep.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, Register, NULL));
  *ErrSrcId = (UINT8) LerrUncFep.Bits.fes;
  *ErrType = (UINT8) LerrUncFep.Bits.fep;
}

/**
  This routine is to get the value of GERRFATSTS_IEH_SATELLITE_REG

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
  UINT32   GFaErrStsReg;
  BOOLEAN  IsSouthIeh;

  IsSouthIeh = IehDevType == SatelliteIehSouth ? TRUE : FALSE;
  GFaErrStsReg = IsSouthIeh ? PCH_IEH_GFAERRSTS_REG : ONLY_REGISTER_OFFSET(GERRFATSTS_IEH_SATELLITE_REG);

  return MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GFaErrStsReg, NULL));

}

/**
  This routine is to get the value of GERRNONSTS_IEH_SATELLITE_REG

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
)
{
  UINT32   GNfErrStsReg;
  BOOLEAN  IsSouthIeh;

  IsSouthIeh = IehDevType == SatelliteIehSouth ? TRUE : FALSE;
  GNfErrStsReg = IsSouthIeh ? PCH_IEH_GNFERRSTS_REG : ONLY_REGISTER_OFFSET(GERRNONSTS_IEH_SATELLITE_REG);

  return MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GNfErrStsReg, NULL));

}

/**
  This routine is to get the value of GERRCORSTS_IEH_SATELLITE_REG

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
  UINT32   GCoErrStsReg;
  BOOLEAN  IsSouthIeh;

  IsSouthIeh = IehDevType == SatelliteIehSouth ? TRUE : FALSE;
  GCoErrStsReg = IsSouthIeh ? PCH_IEH_GCOERRSTS_REG : ONLY_REGISTER_OFFSET(GERRCORSTS_IEH_SATELLITE_REG);

  return MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, GCoErrStsReg, NULL));

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

  UINT8                           Skt;
  UINT8                           Bus;
  UINT8                           Dev;
  UINT8                           Fun;
  UINT32                          Register;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Fun = Ieh->Common.Func;

  //
  // First to enable Local Error Source 0, as it is IEH Local and psf errors which  is
  // different from other local error source
  //

  //
  // Ieh Local and psf error are always fatal error and no correctable error, always enable them
  //

  if (ErrSrcId == 0) {
    Register = GetLclErrRegOffset (Ieh, LERRUNCMSK, 0);
    MmioWrite32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL), UnCorrErrMask);
  }
  //
  // For other Local error sources, there are AER style mask regiters, init them as AER style
  //
  //
  // Program Error Mask Registers
  //
  Register = GetLclErrRegOffset (Ieh, LERRCORMSK, ErrSrcId);
  MmioWrite32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL), CorrErrMask);
  Register = GetLclErrRegOffset (Ieh, LERRUNCMSK, ErrSrcId);
  MmioWrite32 (MmPciAddressExt (Skt, Bus, Dev, Fun, Register, NULL), UnCorrErrMask);

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
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(ROOTERRMSK_IEH_GLOBAL_REG), NULL), 7);
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(ROOTCTLOVR_IEH_GLOBAL_REG), NULL), 7);
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
  UINT8                                 Socket;
  UINT8                                 Bus;
  UINT8                                 Device;
  UINT8                                 Function;
  G_AGENT_SRCID_06_IEH_SATELLITE_STRUCT AgentSrcIdSatIeh;
  DFX1_WAC_N0_IEH_SATELLITE_STRUCT      Dfx1WacN0;
  DFX1_RAC_N0_IEH_SATELLITE_STRUCT      Dfx1RacN0;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  //
  // Open HOSTIA_SMM_SAI for programming G_AGENT_SRCID registers
  //

  Dfx1WacN0.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_WAC_N0_IEH_SATELLITE_REG), NULL));
  Dfx1WacN0.Bits.dfx1_sai_pol_4 = 1;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_WAC_N0_IEH_SATELLITE_REG), NULL), Dfx1WacN0.Data);
  Dfx1RacN0.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_RAC_N0_IEH_SATELLITE_REG), NULL));
  Dfx1RacN0.Bits.dfx1_sai_pol_4 = 1;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_RAC_N0_IEH_SATELLITE_REG), NULL), Dfx1RacN0.Data);

  RAS_DEBUG((LEVEL_BASIC_FLOW,"  [Sat IEH] ClearAgentSrcId: Socket:%d; Bus:%d; Dev:%d; Func:%d; \n", Socket, Bus, Device, Function));

  AgentSrcIdSatIeh.Data = MmioRead32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_06_IEH_SATELLITE_REG), NULL));
  AgentSrcIdSatIeh.Bits.portid = 0x0;
  MmioWrite32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_06_IEH_SATELLITE_REG), NULL), AgentSrcIdSatIeh.Data);

  AgentSrcIdSatIeh.Data = MmioRead32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_07_IEH_SATELLITE_REG), NULL));
  AgentSrcIdSatIeh.Bits.portid = 0x0;
  MmioWrite32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_07_IEH_SATELLITE_REG), NULL), AgentSrcIdSatIeh.Data);

  AgentSrcIdSatIeh.Data = MmioRead32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_08_IEH_SATELLITE_REG), NULL));
  AgentSrcIdSatIeh.Bits.portid = 0x0;
  MmioWrite32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_08_IEH_SATELLITE_REG), NULL), AgentSrcIdSatIeh.Data);

  AgentSrcIdSatIeh.Data = MmioRead32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_09_IEH_SATELLITE_REG), NULL));
  AgentSrcIdSatIeh.Bits.portid = 0x0;
  MmioWrite32(MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(G_AGENT_SRCID_09_IEH_SATELLITE_REG), NULL), AgentSrcIdSatIeh.Data);

  //
  // Close HOSTIA_SMM_SAI for programming G_AGENT_SRCID registers
  //

  Dfx1WacN0.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_WAC_N0_IEH_SATELLITE_REG), NULL));
  Dfx1WacN0.Bits.dfx1_sai_pol_4 = 0;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_WAC_N0_IEH_SATELLITE_REG), NULL), Dfx1WacN0.Data);
  Dfx1RacN0.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_RAC_N0_IEH_SATELLITE_REG), NULL));
  Dfx1RacN0.Bits.dfx1_sai_pol_4 = 0;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET(DFX1_RAC_N0_IEH_SATELLITE_REG), NULL), Dfx1RacN0.Data);

}
