/** @file
  Platform Info Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _PLATFORM_INFO_INTERNAL_H_
#define _PLATFORM_INFO_INTERNAL_H_

#include <PiPei.h>
#include <Ppi/CpuIo.h>
#include <Ppi/Spi.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PlatformHooksLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SetupLib.h>
#include <Guid/SocketVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include <IndustryStandard/Pci22.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/MmPciBaseLib.h>
#include <Platform.h>
#include "SioRegs.h"
#include <Register/PchRegsSpi.h>
#include <PchAccess.h>
#include <FlashMap.h>
#include <Register/PchRegsLpc.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Use AMI SPI services
//#include <Platform/SpiFvbServices/SpiFlashDevice.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Use AMI SPI services
#include <Library/ReportStatusCodeLib.h>
#include <Library/PlatformStatusCodes.h>
#include <Library/FruInformationLib.h>
#include <Register/Cpuid.h>

#define EFI_PLATFORMINFO_DRIVER_PRIVATE_SIGNATURE SIGNATURE_32 ('P', 'I', 'N', 'F')

//
// CPU Model
//
#define  INVALID_MODEL             0x0

#define R_SB_SPI_FDOC         0xB0
#define R_SB_SPI_FDOD         0xB4
//
// APTIOV_SERVER_OVERRIDE_RC_START : Added to resolve redefinition error.
//
#ifndef SPI_OPCODE_READ_INDEX
#define SPI_OPCODE_READ_INDEX            4
#endif
//
// APTIOV_SERVER_OVERRIDE_RC_END : Added to resolve redefinition error.
//
#define PDR_REGION_START_OFFSET    0x0

typedef union BOARD_ID
{
   struct{
      UINT8  BoardID0            :1;
      UINT8  BoardID1            :1;
      UINT8  BoardID2            :1;
      UINT8  BoardID3            :1;
      UINT8  BoardID4            :1;
      UINT8  BoardRev0           :1;
      UINT8  BoardRev1           :1;
      UINT8  Rsvd                :1;
   }BoardID;
}BOARD_ID;

typedef union RISER_ID
{
   struct{
      UINT8  RiserID0            :1;
      UINT8  RiserID1            :1;
      UINT8  RiserID2            :1;
      UINT8  RiserID3            :1;
      UINT8  Rsvd                :4;
   }RiserID;
}RISER_ID;



EFI_STATUS
PdrGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  );

EFI_STATUS
GPIOGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
);

#endif
