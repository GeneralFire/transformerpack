/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include <Memory/JedecDefinitions.h>
#include <Library/MemoryCoreLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include "Include/BaseMemoryCoreInternal.h"

UINT16 
LrdimmCodeDeCodeDrvStr (
  UINT16    value,
  UINT8     code
  )
{
  UINT16 convertODT = 0;

  if (CODE_ODT == code) {
    // get Register value and convert it to ODT value
    switch (value) {
    case 0:
      convertODT = DDR_RON_40;
      break;
    case 1:
      convertODT = DDR_RON_34;
      break;
    case 2:
      convertODT = DDR_RON_48;
      break;
    default:
      convertODT = 0;
      break;
    } // end switch value
  } else {
    // get ODT value and convert it to Register value
    switch (value) {
    case DDR_RON_40:
      convertODT = 0;
      break;
    case DDR_RON_34:
      convertODT = 1;
      break;
    case DDR_RON_48:
      convertODT = 2;
      break;
    default:
      convertODT = 0;
      break;
    } // end switch value
  } // if code

  return convertODT;
} //LrdimmCodeDeCodeDrvStr


UINT16 
CodeDeCodeDrvStr (
  UINT16    value,
  UINT8     code,
  UINT8     dimmtype
  )
{
  UINT16 convertODT = 0;

#ifdef  LRDIMM_SUPPORT
  if (dimmtype == LRDIMM) {
    convertODT = LrdimmCodeDeCodeDrvStr (value, code);
  } else
#endif
  {
    if (CODE_ODT == code) {
      // get Register value and convert it to ODT value
      switch (value) {
      case 0:
        convertODT = DDR_RON_34;
        break;
      case 1:
        convertODT = DDR_RON_48;
        break;
      default:
        convertODT = 0;
        break;
      } // end switch value
    } else {
      // get ODT value and convert it to Register value
      switch (value) {
      case DDR_RON_34:
        convertODT = 0;
        break;
      case DDR_RON_48:
        convertODT = 1;
        break;
      default:
        convertODT = 0;
        break;
      } // end switch value
    } // if code
  }


  return convertODT;
} // CodeDeCodeDrvStr

UINT16  
CodeDeCodeNomParkOdt (
  UINT16 value,
  UINT8  code
  )
{
  UINT16 convertODT = 0;

  if (CODE_ODT == code) {
    // get Register value and convert it to ODT value
    switch (value) {
    case 0:
      convertODT = 0;
      break;
    case DDR4_RTT_NOM_60:
      convertODT = 60;
      break;
    case DDR4_RTT_NOM_120:
      convertODT = 120;
      break;
    case DDR4_RTT_NOM_40:
      convertODT = 40;
      break;
    case DDR4_RTT_NOM_240:
      convertODT = 240;
      break;
    case DDR4_RTT_NOM_48:
      convertODT = 48;
      break;
    case DDR4_RTT_NOM_80:
      convertODT = 80;
      break;
    case DDR4_RTT_NOM_34:
      convertODT = 34;
      break;
    default:
      convertODT = 0;
    } // end switch value
  } else {
    // get ODT value and convert it to Register value
    switch (value) {
    case 0:
      convertODT = 0;
      break;
    case 60:
      convertODT = DDR4_RTT_NOM_60;
      break;
    case 120:
      convertODT = DDR4_RTT_NOM_120;
      break;
    case 40:
      convertODT = DDR4_RTT_NOM_40;
      break;
    case 240:
      convertODT = DDR4_RTT_NOM_240;
      break;
    case 48:
      convertODT = DDR4_RTT_NOM_48;
      break;
    case 80:
      convertODT = DDR4_RTT_NOM_80;
      break;
    case 34:
      convertODT = DDR4_RTT_NOM_34;
      break;
    default:
      convertODT = 0;
    } // end switch value
  } // if code

  return convertODT;
} // CodeDeCodeNomParkOdt


UINT16 
LrdimmCodeDeCodeWrOdt (
  UINT16    value,
  UINT8     code
  )
{
  UINT16 convertODT = 0;

  if (CODE_ODT == code) {
    // get Register value and convert it to ODT value
    switch (value) {
    case 0:
      convertODT = RTTWR_DIS;
      break;
    case 1:
      convertODT = RTTWR_60;
      break;
    case 2:
      convertODT = RTTWR_120;
      break;
    case 4:
      convertODT = RTTWR_240;
      break;
    case 6:
      convertODT = RTTWR_80;
      break;
    case 7:
      convertODT = RTTWR_INF;
      break;
    default:
      convertODT = 0;
      break;
    } // end switch value
  } else {
    // get ODT value and convert it to Register value
    switch (value) {
    case RTTWR_DIS:
      convertODT = 0;
      break;
    case RTTWR_60:
      convertODT = 1;
      break;
    case RTTWR_120:
      convertODT = 2;
      break;
    case RTTWR_240:
      convertODT = 4;
      break;
    case RTTWR_80:
      convertODT = 6;
      break;
    case RTTWR_INF:
      convertODT = 7;
      break;
    default:
      convertODT = 0;
      break;
    } // end switch value
  } // if code
  return convertODT;
} //LrdimmCodeDeCodeWrOdt



UINT16 
CodeDeCodeWrOdt (
  UINT16    value,
  UINT8     code,
  UINT8     dimmtype
  )
{
  UINT16 convertODT = 0;

#ifdef  LRDIMM_SUPPORT
  if (dimmtype == LRDIMM) {
    convertODT = LrdimmCodeDeCodeWrOdt (value, code);
  } else
#endif
  {
    if (CODE_ODT == code) {
      // get Register value and convert it to ODT value
      switch (value) {
      case 0:
        convertODT = RTTWR_DIS;
        break;
      case 1:
        convertODT = RTTWR_120;
        break;
      case 2:
        convertODT = RTTWR_240;
        break;
      case 4:
        convertODT = RTTWR_80;
        break;
      case 3:
        convertODT = RTTWR_INF;
        break;
      default:
        convertODT = 0;
        break;
      } // end switch value
    } else {
      // get ODT value and convert it to Register value
      switch (value) {
      case RTTWR_DIS:
        convertODT = 0;
        break;
      case RTTWR_120:
        convertODT = 1;
        break;
      case RTTWR_240:
        convertODT = 2;
        break;
      case RTTWR_80:
        convertODT = 4;
        break;
      case RTTWR_INF:
        convertODT = 3;
        break;
      default:
        convertODT = 0;
        break;
      } // end switch value
    } // if code
  }

  return convertODT;
} // CodeDeCodeWrOdt

UINT16 
ConvertOdtValue (
  PSYSHOST  Host,
  MRC_GT    group,
  UINT16    value,
  UINT8     code,
  UINT8     dimmtype
  )
{
  UINT16 convertODT = 0;

  switch (group) {
  case RxOdt:
    convertODT = CodeDeCodeRxOdt     (value, code);
    break;
  case TxRon:
    convertODT = CodeDeCodeTxRon     (value, code);
    break;
  case DramDrvStr:
    convertODT = CodeDeCodeDrvStr    (value, code, dimmtype);
    break;
  case NomOdt:
  case ParkOdt:
    convertODT = CodeDeCodeNomParkOdt (value, code);
    break;
  case WrOdt:
    convertODT = CodeDeCodeWrOdt      (value, code, dimmtype);
    break;
  default:
    convertODT = value;
    break;
  } // end switch group

  return convertODT;
} // ConvertOdtValue
