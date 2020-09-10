/** @file
  PCH configuration based on PCH policy

@copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _PCH_POLICY_COMMON_H_
#define _PCH_POLICY_COMMON_H_

#include <ConfigBlock.h>
#include <ConfigBlock/UsbConfig.h>
#include <ConfigBlock/Usb2PhyConfig.h>
#include <ConfigBlock/Usb3HsioConfig.h>

#include <PchLimits.h>
#include "ConfigBlock/PchGeneralConfig.h"
#include <ConfigBlock/PcieRpConfig.h>
#include "ConfigBlock/SataConfig.h"
#include "ConfigBlock/IoApicConfig.h"
#include "ConfigBlock/DmiConfig.h"
#include "ConfigBlock/FlashProtectionConfig.h"
#include "ConfigBlock/HdAudioConfig.h"
#include "ConfigBlock/InterruptConfig.h"
#include "ConfigBlock/IshConfig.h"
#include "ConfigBlock/LanConfig.h"
#include "ConfigBlock/LockDownConfig.h"
#include "ConfigBlock/P2sbConfig.h"
#include "ConfigBlock/PmConfig.h"
#include "ConfigBlock/ScsConfig.h"
#include "ConfigBlock/SerialIoConfig.h"
#include "ConfigBlock/SerialIrqConfig.h"
#include "ConfigBlock/ThermalConfig.h"
#include "ConfigBlock/EspiConfig.h"
#include "ConfigBlock/CnviConfig.h"
#include "ConfigBlock/HsuartConfig.h"
#include "ConfigBlock/AdrConfig.h"
//
// CDF_HSIO_TODO: It should be removed from inside the flag when Icelake will merge HSIO code from CNL
//
#include "ConfigBlock/HsioConfig.h"
#include <ConfigBlock/SiPreMemConfig.h>
//
// SC_CONV
// remove flags after sync to client
//
#include <Private/Library/PchPciExpressHelpersLib.h>
#include "ConfigBlock/FivrConfig.h"

#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif
#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif


#endif // _PCH_POLICY_COMMON_H_
