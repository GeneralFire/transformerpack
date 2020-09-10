/** @file
  Platform Info PEIM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation.

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

#include "PlatformInfo.h"
#include "BootMode.h"
#include <Pins/GpioPinsVer3.h>
#include <Library/GpioLib.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/PchInfoLib.h>
#include <Library/GbeLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START
//#include <Platform/SpiFvbServices/SpiFlashDeviceTable.h>  //Use AMI SPI services
#include <Library/AmiPlatformHooksLib.h>  //Use OEM functions from AmiPlatformHooksLib.h 
// APTIOV_SERVER_OVERRIDE_RC_END
#include <UncoreCommonIncludes.h>
#include <PlatformInfoTypes.h>

#define  TEMP_BUS_NUMBER    (0x3F)


STATIC EFI_PEI_PPI_DESCRIPTOR       mPlatformInfoPpi = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPlatformInfoGuid,
    NULL
  };

STATIC EFI_PEI_NOTIFY_DESCRIPTOR   mUpdateBootModeNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardInitGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) UpdateBootMode
};

#define  BOARD_ID_GPIO_PADS_NUMBER     6
#define  BOARD_REV_ID_GPIO_PADS_NUMBER 3

//
// These pads shall not be board specific as these are used for Board ID and Rev ID detection
// Therefore can not be moved to UBA and are common for all Purley boards
//
GPIO_PAD                 mBoardId [BOARD_ID_GPIO_PADS_NUMBER] = {
  // WARNING: The pad number must be obtained from board schematics
  GPIO_VER3_GPP_M0,
  GPIO_VER3_GPP_M1,
  GPIO_VER3_GPP_M2,
  GPIO_VER3_GPP_M3,
  GPIO_VER3_GPP_M4,
  GPIO_VER3_GPP_M5
};

GPIO_PAD                 mBoardRevId [BOARD_REV_ID_GPIO_PADS_NUMBER] = {
  // WARNING: This should be obtained from board schematics
  GPIO_VER3_GPP_M6,
  GPIO_VER3_GPP_M7,
  GPIO_VER3_GPP_M8
};

GPIO_CONFIG              mBoardAndRevIdConfig = {
  // Board and Revision ID pads configuration required for proper reading the values
  GpioPadModeGpio, GpioHostOwnDefault, GpioDirIn, GpioOutDefault, GpioIntDefault,
    GpioPlatformReset, GpioTermDefault, GpioLockDefault, GpioRxRaw1Default
};

VOID
GpioConfigForBoardId (
  VOID
  )
{
  UINT8                i;
  EFI_STATUS           Status;
  GPIO_CONFIG          PadConfig;

  PadConfig.PadMode          = mBoardAndRevIdConfig.PadMode;
  PadConfig.HostSoftPadOwn   = mBoardAndRevIdConfig.HostSoftPadOwn;
  PadConfig.Direction        = mBoardAndRevIdConfig.Direction;
  PadConfig.OutputState      = mBoardAndRevIdConfig.OutputState;
  PadConfig.InterruptConfig  = mBoardAndRevIdConfig.InterruptConfig;
  PadConfig.PowerConfig      = mBoardAndRevIdConfig.PowerConfig;
  PadConfig.ElectricalConfig = mBoardAndRevIdConfig.ElectricalConfig;
  PadConfig.LockConfig       = mBoardAndRevIdConfig.LockConfig;
  PadConfig.OtherSettings    = mBoardAndRevIdConfig.OtherSettings;

  for (i = 0; i < BOARD_ID_GPIO_PADS_NUMBER; i++) {
    Status = GpioSetPadConfig (mBoardId[i], &PadConfig);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "!!! GPIO ERROR: BoardId[%d], please check GPIO ownership in SPS/CSME \n", i));
    }
  }
}

VOID
GpioConfigForBoardRevId (
  VOID
  )
{
  UINT8                i;
  EFI_STATUS           Status;
  GPIO_CONFIG          PadConfig;

  PadConfig.PadMode          = mBoardAndRevIdConfig.PadMode;
  PadConfig.HostSoftPadOwn   = mBoardAndRevIdConfig.HostSoftPadOwn;
  PadConfig.Direction        = mBoardAndRevIdConfig.Direction;
  PadConfig.OutputState      = mBoardAndRevIdConfig.OutputState;
  PadConfig.InterruptConfig  = mBoardAndRevIdConfig.InterruptConfig;
  PadConfig.PowerConfig      = mBoardAndRevIdConfig.PowerConfig;
  PadConfig.ElectricalConfig = mBoardAndRevIdConfig.ElectricalConfig;
  PadConfig.LockConfig       = mBoardAndRevIdConfig.LockConfig;
  PadConfig.OtherSettings    = mBoardAndRevIdConfig.OtherSettings;

  for (i = 0; i < BOARD_REV_ID_GPIO_PADS_NUMBER; i++) {
    Status = GpioSetPadConfig (mBoardRevId[i], &PadConfig);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "!!! GPIO ERROR: BoardRevId[%d], please check GPIO ownership in SPS/CSME \n", i));
    }
  }
}

/**

    Reads GPIO pins to get Board ID value

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetBoardId (
  OUT UINT32 *BoardId
  )
{
  EFI_STATUS              Status = EFI_DEVICE_ERROR;
  UINT32                  Data32;
  UINT8                   i;
  UINT32                  BdId;

  if (BoardId == NULL) {
    return EFI_UNSUPPORTED;
  }
  BdId = 0;

  GpioConfigForBoardId ();

  for (i = 0; i < BOARD_ID_GPIO_PADS_NUMBER; i++) {
    Status = GpioGetInputValue (mBoardId[i], &Data32);
    if (EFI_ERROR(Status)) {
      break;
    }
    if (Data32) {
      BdId = BdId | (1 << i);
    }
  }
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  *BoardId = BdId;
  return EFI_SUCCESS;
}

/**

    Reads GPIO pins to get Board Revision ID value

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetBoardRevId (
  OUT UINT32 *BoardRevId
  )
{
  EFI_STATUS              Status = EFI_DEVICE_ERROR;
  UINT32                  Data32;
  UINT8                   i;
  UINT32                  RevId;

  if (BoardRevId == NULL) {
    return EFI_UNSUPPORTED;
  }

  RevId = 0;

  GpioConfigForBoardRevId ();

  for (i = 0; i < BOARD_REV_ID_GPIO_PADS_NUMBER; i++){
    Status = GpioGetInputValue (mBoardRevId[i], &Data32);
    if (EFI_ERROR(Status)) {
      break;
    }
    if (Data32) {
      RevId = RevId | (1 << i);
    }
  }
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  *BoardRevId = RevId;
  return EFI_SUCCESS;

}

/**

   Returns the Model ID of the CPU.
   Model ID = EAX[7:4]

**/
VOID
GetCpuInfo (
  UINT32    *CpuType,
  UINT8     *CpuStepping
  )

{
  UINT32                    RegEax=0;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  *CpuStepping = (UINT8) (RegEax & 0x0F);
  *CpuType     = (UINT32) (RegEax >> 4);
}


/**
    Get Platform Type by read Platform Data Region in SPI flash.
    SPI Descriptor Mode Routines for Accessing Platform Info from Platform Data Region (PDR)

    @param PeiServices  -  General purpose services available to every PEIM.
    @param PlatformInfoHob - Platform Type is returned in PlatformInfoHob->BoardId

    @retval Status EFI_SUCCESS - PDR read success
    @retval Status EFI_INCOMPATIBLE_VERSION - PDR read but it is not valid Platform Type

**/
EFI_STATUS
PdrGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO         *PlatformInfoHob
  )
{
  EFI_STATUS              Status;
  PCH_SPI_PROTOCOL        *SpiPpi;
  UINTN                   Size;

  //
  // Locate the SPI PPI Interface
  //
  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gPchSpiPpiGuid,
                    0,
                    NULL,
                    &SpiPpi
                    );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Read the PIT (Platform Info Table) from the SPI Flash Platform Data Region
  //
  Size = sizeof (EFI_PLATFORM_INFO);
  Status = SpiPpi->FlashRead (
                              SpiPpi,
                              FlashRegionPlatformData,
                              PDR_REGION_START_OFFSET,
                              (UINT32) Size,
                              (UINT8 *) PlatformInfoHob
                             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if ((PlatformInfoHob->BoardId >= TypePlatformMin) && (PlatformInfoHob->BoardId <= TypePlatformMax)) {
    //
    // Valid Platform Identified
    //
    DEBUG ((EFI_D_ERROR, "Platform Info from PDR: Type = %x\n",PlatformInfoHob->BoardId));
  } else {
    //
    // Reading PIT from SPI PDR Failed or a unknown platform identified
    //
    DEBUG ((EFI_D_ERROR, "PIT from SPI PDR reports Platform ID as %x. This is unknown ID. Assuming Greencity Platform!\n", PlatformInfoHob->BoardId));
    PlatformInfoHob->BoardId = TypePlatformUnknown;
    Status = EFI_INCOMPATIBLE_VERSION;
  }
  return Status;
}


EFI_STATUS
GetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  )
/**

    GC_TODO: add routine description

    @param PeiServices     - GC_TODO: add arg description
    @param PlatformInfoHob - GC_TODO: add arg description

    @retval EFI_UNSUPPORTED - GC_TODO: add retval description
    @retval EFI_SUCCESS     - GC_TODO: add retval description

**/
{

  UINT32                  BoardId;
  UINT32                  BoardRev;
  EFI_STATUS              Status;

  PlatformInfoHob->BoardId = TypeArcherCityRP;
  DEBUG ((EFI_D_ERROR, "Use GPIO to read Board ID\n"));

  Status = GpioGetBoardId (&BoardId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error: Can't read GPIO to get Board ID!\n"));
    return Status;
  }

  Status = GpioGetBoardRevId (&BoardRev);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Error: Can't read GPIO to get Board RevID!\n"));
    return Status;
  }

  DEBUG ((EFI_D_ERROR, "Board ID  : %d\n ",BoardId));
  DEBUG ((EFI_D_ERROR, "Board Rev.: %d\n", BoardRev));

  switch (BoardId) {
    case 0x00:  // for Simics
#if MAX_SOCKET > 4
      PlatformInfoHob->BoardId = TypeArcherCityModular;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_8S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityModular for simics\n"));
#else
      PlatformInfoHob->BoardId = TypeArcherCityRP;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_UNDEFINED;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityRP for simics\n"));
#endif
      break;
    case 0x01:
      PlatformInfoHob->BoardId = TypeArcherCityRP;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_2S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityRP\n"));
      break;
    case 0x04:
      PlatformInfoHob->BoardId = TypeArcherCityRP;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_2S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityCPV\n"));
      break;
    case 0x05:
      PlatformInfoHob->BoardId = TypeArcherCityRP;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_2S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityPPV\n"));
      break;
    case 0x02:
      PlatformInfoHob->BoardId = TypeArcherCityModular;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_8S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityModular\n"));
      break;
    case 0x07:
      PlatformInfoHob->BoardId = TypeArcherCityEVB;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_2S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityEVB\n"));
      break;
    case 0x06:
      PlatformInfoHob->BoardId = TypeArcherCityXPV;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_1SWS;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityXPV - 1S\n"));
      break;
    case 0x08:
      PlatformInfoHob->BoardId = TypeArcherCityXPV;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_2S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeArcherCityXPV - 2S\n"));
      break;
    case 0x11:
      PlatformInfoHob->BoardId = TypeBigPineKey;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_2S;
      DEBUG ((EFI_D_ERROR, "Board ID = TypeBigPineKey - 2S\n"));
      break;
    default:
      PlatformInfoHob->BoardId = TypeArcherCityRP;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_UNDEFINED;
      DEBUG ((EFI_D_ERROR, "Default set to TypeArcherCityRP.\n"));
      break;
  }

      DEBUG ((EFI_D_ERROR, "[ASTDBG] ################################# !!! \n"));
      DEBUG ((EFI_D_ERROR, "[ASTDBG] Force to set platform to TypeArcherCityRP !!! \n"));
      DEBUG ((EFI_D_ERROR, "[ASTDBG] ################################# !!! \n"));	
      PlatformInfoHob->BoardId = TypeArcherCityRP;
      PlatformInfoHob->SysData.SocketConfig = SOCKET_2S;


  PlatformInfoHob->TypeRevisionId = BoardRev;

  return EFI_SUCCESS;
}

/**

  This function initializes the board related flag to indicates if
  PCH and Lan-On-Motherboard (LOM) devices is supported.

**/
VOID
GetPchLanSupportInfo(
  IN EFI_PLATFORM_INFO   *PlatformInfoHob
  )
{
  PlatformInfoHob->PchData.LomLanSupported  = 0;
}

/**

    GC_TODO: add routine description

    @param PeiVariable     - GC_TODO: add arg description
    @param PlatformInfoHob - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
GetIioCommonRcPlatformSetupPolicy(
  OUT EFI_PLATFORM_INFO                     *PlatformInfoHob
  )
  {
  EFI_STATUS                            Status;
  UINT8                                 IsocEn;

  Status = GetOptionData (&gEfiSocketCommonRcVariableGuid, OFFSET_OF (SOCKET_COMMONRC_CONFIGURATION, IsocEn), &IsocEn, sizeof(UINT8));

  if (EFI_ERROR (Status)) {
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
    PlatformInfoHob->SysData.IsocEn = 1;
#else
    PlatformInfoHob->SysData.IsocEn = 0;
#endif // ME_SUPPORT_FLAG
  } else {
    PlatformInfoHob->SysData.IsocEn    = IsocEn;       // ISOC enabled
  }

  return EFI_SUCCESS;
}
/**

    GC_TODO: add routine description

    @param PeiVariable     - GC_TODO: add arg description
    @param PlatformInfoHob - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
GetIioPlatformSetupPolicy(
  OUT EFI_PLATFORM_INFO                     *PlatformInfoHob
  )
{
  return EFI_SUCCESS;
}


/**
  Platform Type detection. Because the PEI globle variable
  is in the flash, it could not change directly.So use
  2 PPIs to distinguish the platform type.

  @param FfsHeader    -  Pointer to Firmware File System file header.
  @param PeiServices  -  General purpose services available to every PEIM.

  @retval EFI_SUCCESS  -  Memory initialization completed successfully.
  @retval Others       -  All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
PlatformInfoInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_PCI_CFG2_PPI                  *PciCfgPpi;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  EFI_PLATFORM_INFO                     PlatformInfoHob;
  EFI_PLATFORM_INFO                     tempPlatformInfoHob;
  UINT8                                 ChipId;
  UINT32                                Delay;
  UINT32                                CpuType;
  UINT8                                 CpuStepping;

  //
  // Report Status Code EFI_CU_CHIPSET_PLATFORM_TYPE_INIT
  //
  REPORT_STATUS_CODE (
    EFI_PROGRESS_CODE,
    (EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_CHIPSET_PLATFORM_TYPE_INIT)
    );

  PciCfgPpi = (**PeiServices).PciCfg;
  if (PciCfgPpi == NULL) {
    DEBUG ((EFI_D_ERROR, "\nError! PlatformInfoInit() - PeiServices is a NULL Pointer!!!\n"));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate Variable PPI
  //
  Status = (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  (*PeiServices)->SetMem (
                    &PlatformInfoHob,
                    sizeof (PlatformInfoHob),
                    0
                    );

  //
  // --------------------------------------------------
  //
  // Detect the iBMC SIO for CV/CRB Platforms
  // 0x2E/0x2F decoding has been enabled in MonoStatusCode PEIM.
  //
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_SIO_UNLOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_CHIP_ID_REG);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  ChipId = IoRead8  (PILOTIV_SIO_DATA_PORT);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_SIO_LOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);

  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "LocatePpi Error in PlatformInfo.c !\n"));
  }

  Status = GetIioPlatformSetupPolicy (&PlatformInfoHob);
  ASSERT_EFI_ERROR (Status);
  Status = GetIioCommonRcPlatformSetupPolicy (&PlatformInfoHob);
  ASSERT_EFI_ERROR (Status);

  Status = GetPlatformInfo (PeiServices, &PlatformInfoHob);
  if (EFI_ERROR(Status)) {
    Status = PdrGetPlatformInfo (PeiServices, &tempPlatformInfoHob);
    if (!EFI_ERROR (Status)) {
      PlatformInfoHob.BoardId = tempPlatformInfoHob.BoardId;
      PlatformInfoHob.TypeRevisionId = tempPlatformInfoHob.TypeRevisionId;
    } else {
      PlatformInfoHob.BoardId = TypeArcherCityRP;
    }
  }

  //
  // Update IIO Type
  //
  PlatformInfoHob.IioRevision = 0;

  //
  // Update PCH Type
  //
  PlatformInfoHob.PchSku = PchGetLpcDid ();
  PlatformInfoHob.PchType = PchSeries ();
  PlatformInfoHob.PchRevision = PchStepping ();
  PlatformInfoHob.MaxNumOfPchs = 1;

  GetPchLanSupportInfo(&PlatformInfoHob);

  if (IsGbeRegionValid () == FALSE) {
    PlatformInfoHob.PchData.GbeRegionInvalid = 1;
  } else {
    PlatformInfoHob.PchData.GbeRegionInvalid = 0;
  }
  PlatformInfoHob.PchData.GbePciePortNum = 13;  //hard code to lane 13, or invoke PchFiaGetGbeLaneNum() <private silicon initialization library>
  PlatformInfoHob.PchData.GbeEnabled = IsGbePresent();
  PlatformInfoHob.PchData.PchStepping = PchStepping();

  PlatformInfoHob.SysData.SysSioExist = (UINT8)IsSioExist();

  //
  // Update CPU Type
  //
  GetCpuInfo (&CpuType, &CpuStepping);
  PlatformInfoHob.CpuType     = CpuType;
  PlatformInfoHob.CpuStepping = CpuStepping;
  // APTIOV_SERVER_OVERRIDE_RC_START : Oem Hook to update PlatformInfo HOB
  OemUpdatePlatformInfoHob (PeiServices, (VOID *) &PlatformInfoHob);
  // APTIOV_SERVER_OVERRIDE_RC_END : Oem Hook to update PlatformInfo HOB

  //
  // Set default memory topology to DaisyChainTopology. This should be modified in UBA board
  // specific file.
  //
  (*PeiServices)->SetMem (&PlatformInfoHob.MemoryTopology, sizeof (PlatformInfoHob.MemoryTopology), DaisyChainTopology);

  //
  // Set default memory type connector to DimmConnectorPth. This should be modified in UBA board
  // specific file.
  //
  (*PeiServices)->SetMem (&PlatformInfoHob.MemoryConnectorType, sizeof (PlatformInfoHob.MemoryConnectorType), DimmConnectorPth);

  //
  // Build HOB for setup memory information
  //
  BuildGuidDataHob (
      &gEfiPlatformInfoGuid,
      &(PlatformInfoHob),
      sizeof (EFI_PLATFORM_INFO)
      );

  Status = (**PeiServices).InstallPpi (PeiServices, &mPlatformInfoPpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Save PlatformInfoHob.BoardId in CMOS
  //
  IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_LO);
  IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)PlatformInfoHob.BoardId);

  IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_HI);
  IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)((PlatformInfoHob.PcieRiser2Type << 4) + (PlatformInfoHob.PcieRiser1Type)));

  Status = PeiServicesNotifyPpi (&mUpdateBootModeNotifyList);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
