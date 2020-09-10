/** @file

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

#include "SetupPlatform.h"
#include <IioUniversalData.h>
#include <Library/CmosAccessLib.h>
#include "Platform.h"
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
#include <Library/MeTypeLib.h>
#include <Library/DxeMeLib.h>
#include <Library/ServerDxeMeLib.h>
#include <Library/MeWatchdogControlLib.h>
#include <Guid/SpsInfoHobGuid.h>
#include <MeSetup.h>
#include <Protocol/HeciControlProtocol.h>
#if ME_TESTMENU_FLAG
#include <Library/HeciConfigureLib.h>
#endif // ME_TESTMENU_FLAG
BOOLEAN         gMeResetRequests = FALSE;
#if defined(ME_WS_SUPPORT) && ME_WS_SUPPORT
extern BOOLEAN  gMeClientReset;
#endif // ME_WS_SUPPORT
#if defined(WS_ICC_SUPPORT) && WS_ICC_SUPPORT
extern BOOLEAN  gIccReset;
#endif // WS_ICC_SUPPORT
#include "IccSetupData.h"
#endif // ME_SUPPORT_FLAG
#include <Private/PchPolicyHob.h>

#include <Private/Library/PmcPrivateLib.h>
#include <PchInfoHob.h>

#include <Library/UbaPlatLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/PchPciBdfLib.h>
#include <Protocol/PlatformSpecificResetFilter.h>
#include <PchResetPlatformSpecific.h>
#include <Library/PchRtcLib.h>

#define STRING_WIDTH_100    100

// Status returned by ME on Dual Watchdog Suspend and Resume messages
static BOOLEAN  mWatchdogMsgRspFailStatus = FALSE;

EFI_GUID                        mErrorManagerGuid        = ERROR_MANAGER_GUID;
EFI_GUID                        mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;
EFI_GUID                        mAmiTseSetupEnterProtocolGuid = AMITSE_SETUP_ENTER_GUID;
EFI_GUID                        mEfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
// FakeVarstore is added to call default Extract/RouteConfig callbacks on multi platform code
EFI_GUID                        mFakeVarstoreGuid        = FAKE_VARSTORE_GUID;
// APTIOV_SERVER_OVERRIDE_RC_START : To solve redefinition error
INTEL_SETUP_DATA                mSetupData;
// APTIOV_SERVER_OVERRIDE_RC_END : To solve redefinition error
SYSTEM_CONFIGURATION            mSystemConfiguration;
PCH_SETUP                       mPchSetup;
SOCKET_IIO_CONFIGURATION        mSocketIioConfiguration;
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
// original set, any change after After EndOfPost requires reset
ME_RC_CONFIGURATION             MeRcConfigurationOrg;
// last processed set to determine is anything has been changed since last time
ME_RC_CONFIGURATION             MeRcConfigurationProcessed;
#endif // ME_SUPPORT_FLAG

SOCKET_MP_LINK_CONFIGURATION    mSocketMpLinkConfiguration;
SOCKET_COMMONRC_CONFIGURATION   mSocketCommonRcConfiguration;
SOCKET_POWERMANAGEMENT_CONFIGURATION mSocketPowermanagementConfiguration;
SOCKET_PROCESSORCORE_CONFIGURATION   mSocketProcessorCoreConfiguration;
// APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
SOCKET_IIO_CONFIGURATION        mSocketIioConfiguration;
// APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support


EFI_HII_HANDLE                  mAdvancedHiiHandle = NULL;
EFI_HII_HANDLE                  mBootOptionsHiiHandle = NULL;
EFI_HII_HANDLE                  mErrorManagerHiiHandle = NULL;
EFI_HII_HANDLE                  mMainHiiHandle = NULL;
EFI_HII_HANDLE                  mMaintenanceHiiHandle = NULL;
EFI_HII_HANDLE                  mSecurityHiiHandle = NULL;

EFI_HANDLE                      mAdvancedDriverHandle = NULL;
EFI_HANDLE                      mBootOptionsDriverHandle = NULL;
EFI_HANDLE                      mErrorManagerDriverHandle = NULL;
EFI_HANDLE                      mMainDriverHandle = NULL;
EFI_HANDLE                      mSecurityDriverHandle = NULL;
EFI_HANDLE                      mImageHandle = NULL;

EFI_HII_DATABASE_PROTOCOL        *mHiiDataBase = NULL;
EFI_HII_CONFIG_ROUTING_PROTOCOL  *mHiiConfigRouting = NULL;
// APTIOV_SERVER_OVERRIDE_RC_START
CHAR16      mVariableName[] = L"IntelSetup";
CHAR16      mVarStoreName[] = L"IntelSetup";
//CHAR16      mTempVariableName[] = L"SetupTemp"; //Setup power good reset support
// APTIOV_SERVER_OVERRIDE_RC_END
const CHAR16 mFakeVarstoreName[] = L"FakeVar";
const CHAR16 mMeSetupStorageName[] = L"MeSetupStorage";
EFI_STRING  mSystemConfigurationRequest;
UINT8       gPwFlag         = VALIDATE_NONE;
UINTN       gRow,gColumn;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mMainConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mAdvancedConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mIohConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mEventLogConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mSecurityConfigAccess;
EFI_HII_CONFIG_ACCESS_PROTOCOL  mBootOptionsConfigAccess;

EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *mPciRootBridgeIo;
EFI_PLATFORM_TYPE_PROTOCOL      *mPlatformType;
// APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
SYSTEM_CONFIGURATION                      *gOldSystemConfiguration = NULL;
SOCKET_MP_LINK_CONFIGURATION              *gOldSocketMpLinkConfiguration = NULL;
SOCKET_COMMONRC_CONFIGURATION             *gOldSocketCommonRcConfiguration = NULL;
SOCKET_POWERMANAGEMENT_CONFIGURATION      *gOldSocketPowermanagementConfiguration = NULL;
SOCKET_PROCESSORCORE_CONFIGURATION        *gOldSocketProcessorCoreConfiguration = NULL;
FPGA_SOCKET_CONFIGURATION                 *gOldFpgaSocketConfiguration = NULL;
SOCKET_IIO_CONFIGURATION                  *gOldSocketIioConfiguration = NULL;
// APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support

STATIC UINT8                    mSmbusRsvdAddresses[PLATFORM_NUM_SMBUS_RSVD_ADDRESSES] = {
  SMBUS_ADDR_CH_A_1,
  SMBUS_ADDR_CH_A_2,
  SMBUS_ADDR_CH_B_1,
  SMBUS_ADDR_CH_B_2
};

EFI_PLATFORM_POLICY_PROTOCOL    mPlatformPolicyData = {
  PLATFORM_NUM_SMBUS_RSVD_ADDRESSES,
  mSmbusRsvdAddresses
};

// APTIOV_SERVER_OVERRIDE_RC_START : Remove Ps2 platform code
#if 0
EFI_PS2_POLICY_PROTOCOL         mPs2PolicyData = {
  EFI_KEYBOARD_NUMLOCK,
  (EFI_PS2_INIT_HARDWARE) Ps2InitHardware
};
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Remove Ps2 platform code
EFI_BOOT_MODE       mBootMode;

EFI_STATUS
EFIAPI
XmpCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION Action,
  IN EFI_QUESTION_ID KeyValue,
  IN UINT8 Type,
  IN EFI_IFR_TYPE_VALUE *Value,
  OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest OPTIONAL
  );

VOID
UpdateIioDefaultConfigViaBoard (
   IN SYSTEM_CONFIGURATION *Default
   );

VOID
RestoreAdminPassword (
   IN SYSTEM_CONFIGURATION *Defaults
   );

VOID
DisableSetupOptions (VOID);

VOID
SuppressSetupOptionsBasedOnFmcType (VOID);

VOID
InstallPlatformReset (VOID);

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
VOID
EFIAPI
MeOnEnterSetup (
  IN EFI_EVENT Event,
  IN VOID *Context
);
#endif // ME_SUPPORT_FLAG
VOID
EFIAPI
MeOnExitSetup (
  IN EFI_EVENT Event,
  IN VOID *Context
  );
VOID
EFIAPI
PchOnSetupEnter(
  IN EFI_EVENT Event,
  IN VOID *Context
);

VOID
GrayoutSecureEraseIntelDcpmm (
  VOID
);

#if defined (IPMI_SUPPORT)
#endif // IPMI_SUPPORT

//
// HII Vendor Device Path template for all package lists
// Differentiated by incrementing UniqueId
//
HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePathTemplate = {
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {
          (UINT8) (sizeof (HII_VENDOR_DEVICE_PATH_NODE)),
          (UINT8) ((sizeof (HII_VENDOR_DEVICE_PATH_NODE)) >> 8)
        }
      },
      //
      // {396C010D-219A-458a-BA7D-A54D092B4BE5}
      //
      { 0x396c010d, 0x219a, 0x458a, { 0xba, 0x7d, 0xa5, 0x4d, 0x9, 0x2b, 0x4b, 0xe5 } }
    },
    0
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

/**
  Gray out setup options of FMC

  This function grays out FMC defeatured setup options by setting the AepOnSystem flag
  if there is DCPMM DIMM populated on system.

  @param N/A

  @retval N/A

**/
VOID
GrayoutSetupOptionsFMC (
  VOID
  )
{
  UINT8                        AepOnSystem;
  EFI_STATUS                   Status;
  SOCKET_MEMORY_CONFIGURATION  Buffer;
  SYSTEM_MEMORY_MAP_HOB        *SystemMemoryMap;

  //
  // Search for the Memory Configuration GUID HOB
  //
  Status = GetSpecificConfigGuid (&gEfiSocketMemoryVariableGuid, &Buffer);
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL || EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  AepOnSystem = 0;
#ifdef DDRT_SUPPORT
  if ((SystemMemoryMap != NULL) && (SystemMemoryMap->DcpmmPresent != 0)) {
    DEBUG ((EFI_D_INFO, "PMem discovered on system, graying out defeatured options\n"));
    AepOnSystem = 1;
  }
#endif // DDRT_SUPPORT

  Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF (SOCKET_MEMORY_CONFIGURATION, AepOnSystem), &AepOnSystem, sizeof (AepOnSystem));

  if (!EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO,"\nFMC defeatured option is successfully grayed out\n"));
  } else {
    DEBUG ((EFI_D_INFO,"\nFailed to gray out FMC defeatured option!\n"));
  }
}

/**

  This is the standard EFI driver point for the Driver. This
  driver is responsible for setting up any platform specific policy or
  initialization information.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS  -  Policy decisions set.

**/
EFI_STATUS
EFIAPI
SetupDriverEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;
  UINT8                   PcieSlotItemCtrl;
  SYSTEM_CONFIGURATION    SystemConfiguration;
  VOID                    *mEventReg;
  EFI_EVENT               mEvent;
  UINTN                   BufferSize;
  UINT32                  BspCpuidSignature;
  UINT32                  RegEax, RegEbx, RegEcx, RegEdx;
  UINT32                  PlatformFeatureFlag = 0;

  Status = GetEntireConfig (&mSetupData);
  if (EFI_ERROR (Status)) {
    Status = PcdSet64S (PcdUartDefaultBaudRate, 115200);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    Status = PcdSet8S (PcdUartDefaultDataBits, 8);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    Status = PcdSet8S (PcdUartDefaultParity, 1);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    Status = PcdSet8S (PcdUartDefaultStopBits, 1);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    Status = PcdSet8S (PcdDefaultTerminalType, 0);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    DEBUG ((EFI_D_ERROR, "We can't get the mSetupData variable!!!  \n"));
  } else {
    //
    // Update any PCDs based on Setup variable values.
    //
    if (mSetupData.SystemConfig.UefiOptimizedBootToggle == EFI_UEFIOPTIMIZEDBOOT_TOGGLE_ENABLE) {
      Status = PcdSetBoolS (PcdUefiOptimizedBoot, TRUE);
      ASSERT_EFI_ERROR(Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
      mSetupData.SystemConfig.UefiOptimizedBootToggle = 0;
      DEBUG ((EFI_D_INFO, "UEFI Optimized Boot toggled to Enabled!\n"));
      SetEntireConfig(&mSetupData);
    }
    if (mSetupData.SystemConfig.UefiOptimizedBootToggle == EFI_UEFIOPTIMIZEDBOOT_TOGGLE_DISABLE) {
      Status = PcdSetBoolS (PcdUefiOptimizedBoot, FALSE);
      ASSERT_EFI_ERROR(Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
      mSetupData.SystemConfig.UefiOptimizedBootToggle = 0;
      DEBUG ((EFI_D_INFO, "UEFI Optimized Boot toggled to Disabled!\n"));
      SetEntireConfig(&mSetupData);
    }

    if (PcdGetBool(PcdUefiOptimizedBoot)) {
      Status = PcdSetBoolS (PcdUefiWindowsInt10Workaround, (BOOLEAN)mSetupData.SystemConfig.EfiWindowsInt10Workaround);
      ASSERT_EFI_ERROR(Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    } else {
      Status = PcdSetBoolS (PcdUefiWindowsInt10Workaround, FALSE);
      ASSERT_EFI_ERROR(Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    }

    Status = PcdSetBoolS(PcdResetOnMemoryTypeInformationChange, (BOOLEAN)mSetupData.SystemConfig.ResetOnMemMapChange);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = PcdSetBoolS (PcdBdsBootAllOptions, mSetupData.SystemConfig.BootAllOptions);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = PcdSetBoolS (PcdBdsSetShellFirst, mSetupData.SystemConfig.SetShellFirst);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  mImageHandle  = ImageHandle;
  BufferSize    = sizeof (SYSTEM_CONFIGURATION);

  //
  // Get current Boot Mode
  //
  mBootMode = GetBootModeHob ();

  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID**)&mHiiDataBase);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID**)&mHiiConfigRouting);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (
                  &gEfiPlatformTypeProtocolGuid,
                  NULL,
                  &mPlatformType
                  );
  ASSERT_EFI_ERROR (Status);

  // APTIOV_SERVER_OVERRIDE_RC_START
  // Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
  ParseIfrPackForIntelRcSetup(mImageHandle);
  
  #if 0
  InstallHiiDataAndGetSettings (
        (EFI_HII_PACKAGE_HEADER *) PlatformStrings,
        (EFI_HII_PACKAGE_HEADER *) BootOptionsBin,
        //(EFI_HII_PACKAGE_HEADER *) ErrorManagerBin,
        (EFI_HII_PACKAGE_HEADER *) AdvancedBin,
        (EFI_HII_PACKAGE_HEADER *) MainBin,
        NULL
        );
  #endif

  //
  // Setup local variable according to Setup variables
  //
  CopyMem (&mPchSetup, &(mSetupData.PchSetup), sizeof(PCH_SETUP));  //Update PchSetup before consume
// APTIOV_SERVER_OVERRIDE_RC_END
  // APTIOV_SERVER_OVERRIDE_RC_START : Remove Ps2 platform code
  #if 0
  if (!mSystemConfiguration.Numlock) {
    mPs2PolicyData.KeyboardLight &= ~EFI_KEYBOARD_NUMLOCK;
  }
  #endif
  // APTIOV_SERVER_OVERRIDE_RC_END : Remove Ps2 platform code
  Status = SetIdeMode ();

  //
  // Process Processor Setup
  //
  Status = ProcessorHtMode ();

  //
  // Process Memory Setup
  //
  //  Status = MemorySetup();

  //
  // Process dynamic entries
  //
  Status = FindDataRecords ();

  //
  //Update PCDs that are kept consistent with setup
  //
  UpdatePcds();

  //
  // Control the count of Setup question PCI-E Slot Oprom.
  //
  PlatformSetupPcieSlotNumber(&PcieSlotItemCtrl);
  DEBUG ((EFI_D_ERROR, "PcieSlotItemCtrl = %d \n", PcieSlotItemCtrl));
  mSocketIioConfiguration.PcieSlotItemCtrl = PcieSlotItemCtrl;
  //
  // Initialize BiosGuardPlatformSupported to FALSE
  //
  mSystemConfiguration.BiosGuardPlatformSupported = FALSE;

  //
  // Need to make sure that only supported Platforms can enable BIOS Guard
  //
  PlatformFeatureFlag = PcdGet32(PcdOemSkuPlatformFeatureFlag);
  if (PlatformFeatureFlag & BIOSGUARD_SUPPORT_ENABLED) {
    mSystemConfiguration.BiosGuardPlatformSupported = 1;
  }
  else
    mSystemConfiguration.BiosGuardPlatformSupported = 0;

  //
  // Make sure BIOS Guard is disabled on unsupported platforms
  //
  if (!mSystemConfiguration.BiosGuardPlatformSupported) {
    mSystemConfiguration.BiosGuardEnabled = FALSE;
  }

  //
  // Set AntiFlash Wearout to be supported
  //
  mSystemConfiguration.AntiFlashWearoutSupported = FALSE;
  AsmCpuid (0x01, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  BspCpuidSignature = (RegEax >> 4) & 0x0000FFFF;

  if (PlatformFeatureFlag & OC_SUPPORT_ENABLED) {
    mSystemConfiguration.PlatformOCSupport = 1;
  }
  else
    mSystemConfiguration.PlatformOCSupport = 0;
// APTIOV_SERVER_OVERRIDE_RC_START: Password functionality is handled by AMITSE
#if 0 
  //
  // If BootMode is Manufacturing Mode, clear Admin and User passwords
  //
  if (mBootMode == BOOT_WITH_MFG_MODE_SETTINGS) {
    ZeroMem (&mSystemConfiguration.AdminPassword, SHA256_DIGEST_LENGTH);
    ZeroMem (&mSystemConfiguration.UserPassword, SHA256_DIGEST_LENGTH);
  }
#endif  
// APTIOV_SERVER_OVERRIDE_RC_END: Password functionality is handled by AMITSE

  CopyMem (&(mSetupData.SystemConfig), &mSystemConfiguration, sizeof(SYSTEM_CONFIGURATION));
  Status = SetSpecificConfigGuid (&gEfiSetupVariableGuid, &mSystemConfiguration);

// APTIOV_SERVER_OVERRIDE_RC_START : Password functionality is handled by AMITSE
#if 0 
  SetPasswordState (&mAdminPassword);
  SetPasswordState (&mUserPassword);
#endif  
// APTIOV_SERVER_OVERRIDE_RC_END : Password functionality is handled by AMITSE
  //
  // Install protocol to to allow access to this Policy.
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiPlatformPolicyProtocolGuid,
                  &mPlatformPolicyData,
                  // APTIOV_SERVER_OVERRIDE_RC_START : Remove Ps2 platform code
                  #if 0
                  &gEfiPs2PolicyProtocolGuid,
                  &mPs2PolicyData,
                  #endif
                  // APTIOV_SERVER_OVERRIDE_RC_END : Remove Ps2 platform code
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register platform reset callback
  //
  InstallPlatformReset ();

  if (PcdGetBool (PcdUefiOptimizedBoot)) {

      //
      // Locate the setup variable
      //
      CopyMem (&SystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));

    //
    // We now need to see if we must hack the INT10 vector for Windows UEFI.
    //
    if (SystemConfiguration.EfiWindowsInt10Workaround == 0)
    {
      //
      // The UEFI Video ROM is dependent upon the following protocol.  If we install it, the UEFI
      // video ROM will take control based upon the EFI over Legacy priority
      //
      Status = gBS->InstallProtocolInterface (
        &Handle,
        &gEfiWindowsInt10Workaround,
        EFI_NATIVE_INTERFACE,
        NULL // Not required as we are just using this as a dummy marker
        );
    } else {
      //
      // Here we are doing an EFI boot, but need the legacy ROM.  As such we need to also signal
      // the legacy boot path.  This has the effect of allowing the legacy infrastructure to be
      // dispatched.  However, all selections of ROM priorities etc are based upon only the deep boot
      // marker
      //
      Handle = NULL;
      Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiLegacyBootMarkerGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL  // Not required as we are just using this as a dummy marker
                    );
    }
  } else {
    //
    // If we reached here, then we are not doing the setup option of EFI
    // Optimized boot.  As such we need to install the legacy BIOS marker protocol guid.
    //
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiLegacyBootMarkerGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL  // Not required as we are just using this as a dummy marker
                    );

  }
 //
 // Register Notification for password check before entering into setup
 //
 // APTIOV_SERVER_OVERRIDE_RC_START
// Setup power good reset support
  #if 0
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  BackupSetupDataAndCheckForPassword,
                  NULL,
                  &mEvent);
  ASSERT_EFI_ERROR (Status);

   Status = gBS->RegisterProtocolNotify (
                   &mAmiTseSetupEnterProtocolGuid,
                   mEvent,
                   &mEventReg
                    );
   ASSERT_EFI_ERROR (Status);
  #endif
  BackupIntelSetupOptions();
// APTIOV_SERVER_OVERRIDE_RC_END

  DisableSetupOptions();

  //
  // Set the AepOnSystem flag if DCPMM DIMM is present on the system.
  //
  GrayoutSetupOptionsFMC ();

  GrayoutSecureEraseIntelDcpmm ();

  SuppressSetupOptionsBasedOnFmcType ();

// APTIOV_SERVER_OVERRIDE_RC_START : Using mSystemConfiguration
  CopyMem (&mSystemConfiguration, &(mSetupData.SystemConfig), sizeof (SYSTEM_CONFIGURATION));
  switch (mSystemConfiguration.BaudRate) { 
// APTIOV_SERVER_OVERRIDE_RC_END : Using mSystemConfiguration
   case 1:
    Status = PcdSet64S (PcdUartDefaultBaudRate, 9600);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    break;

   case 2:
    Status = PcdSet64S (PcdUartDefaultBaudRate, 19200);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    break;

   case 3:
    Status = PcdSet64S (PcdUartDefaultBaudRate, 38400);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    break;

   case 4:
    Status = PcdSet64S (PcdUartDefaultBaudRate, 57600);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    break;

   case 5:
    Status = PcdSet64S (PcdUartDefaultBaudRate, 115200);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    break;

   default:
    Status = PcdSet64S (PcdUartDefaultBaudRate, 115200);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
    // APTIOV_SERVER_OVERRIDE_RC_START : Using mSystemConfiguration
    Status = PcdSet8S (PcdUartDefaultDataBits, mSystemConfiguration.DataBits);
    // APTIOV_SERVER_OVERRIDE_RC_END : Using mSystemConfiguration
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
// APTIOV_SERVER_OVERRIDE_RC_START : Using mSystemConfiguration
    Status = PcdSet8S (PcdUartDefaultParity, mSystemConfiguration.Parity);
// APTIOV_SERVER_OVERRIDE_RC_END : Using mSystemConfiguration
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
// APTIOV_SERVER_OVERRIDE_RC_START : Using mSystemConfiguration
    Status = PcdSet8S (PcdUartDefaultStopBits, mSystemConfiguration.StopBits);
// APTIOV_SERVER_OVERRIDE_RC_END : Using mSystemConfiguration
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
// APTIOV_SERVER_OVERRIDE_RC_START : Using mSystemConfiguration
    Status = PcdSet8S (PcdDefaultTerminalType, mSystemConfiguration.TerminalType);
// APTIOV_SERVER_OVERRIDE_RC_END : Using mSystemConfiguration
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  //
  // Register ME On Setup Enter callback
  //
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL,
                             TPL_CALLBACK,
                             MeOnEnterSetup,
                             NULL,
                             &mEvent
                            );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR(Status)) {
// APTIOV_SERVER_OVERRIDE_RC_START : Changing the GUID to make MeOnEnterSetup called before entering setup.
    Status = gBS->RegisterProtocolNotify (&gSignalBeforeEnterSetupGuid,
// APTIOV_SERVER_OVERRIDE_RC_END : Changing the GUID to make MeOnEnterSetup called before entering setup.
                                          mEvent,
                                          &mEventReg
                                          );
  }
#endif // ME_SUPPORT_FLAG

#if ME_SPS_SUPPORT
  if (MeTypeIsSps ()) {
    // Register the event to resume ME watchdog when on setup exit.
    Status = EfiNamedEventListen (
               &gEfiSetupExitGuid,
               TPL_NOTIFY,
               MeOnExitSetup,
               NULL,
               NULL);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: DualBiosWatchdog: Error: Can't init onSetupExit event. \n"));
  }
#endif // ME_SPS_SUPPORT

#if defined (IPMI_SUPPORT)
#endif // IPMI_SUPPORT

  //
  // Register Pch On Setup Enter callback
  //
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL,
                             TPL_CALLBACK,
                             PchOnSetupEnter,
                             NULL,
                             &mEvent
                            );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR(Status)) {
// APTIOV_SERVER_OVERRIDE_RC_START : Changing the GUID to make PchOnSetupEnter called before entering setup.
    Status = gBS->RegisterProtocolNotify (&gSignalBeforeEnterSetupGuid,
// APTIOV_SERVER_OVERRIDE_RC_END : Changing the GUID to make PchOnSetupEnter called before entering setup.
                                          mEvent,
                                          &mEventReg
                                          );
  }

  //
  // TBD or do in other places.
  // 1. Clear BIS Credentials.  - Wait for Jan's fw protocol
  // 2. Chassis Intrusion, Date/Time, EventLog, Pnp O/S,      - Do we have to do this?
  // 3. Boot (BBS, USB, PXE, VGA) - Jim's code.
  //
  return EFI_SUCCESS;
}

/**

    This routine disables setup options:
    - SecureErase setup options so that the DDRT dimms don't
      get erased on every reboot.

    - partitionDDRTDimm

    @param VOID
    @retval None

**/

VOID
DisableSetupOptions (VOID)
{
  EFI_STATUS                      Status;
  UINT8                           partitionDDRTDimm;
  UINT8                           setSecureEraseAllDIMMs;
  UINT8                           setSecureEraseSktCh[MAX_DCPMMS]; //[MAX_SOCKET * MAX_CH * MAX_DCPMMS]
  SOCKET_MEMORY_CONFIGURATION     Buffer;
  UINT8                           dimm;

  //Disable setSecureEraseAllDIMMs, setSecureEraseSktCh[socket index][channel index] so that the persistent region of DDRT
  //DIMM doesn't get erased after every rebooting.
  Status = GetSpecificConfigGuid (&gEfiSocketMemoryVariableGuid, &Buffer);

  if (Buffer.setSecureEraseAllDIMMs == 1) {
    DEBUG((EFI_D_INFO,"setSecureEraseAllDIMMs is enabled!\n"));
    DEBUG((EFI_D_INFO,"Disabling setSecureEraseAllDIMMs...\n"));
    setSecureEraseAllDIMMs = 0;
    Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF(SOCKET_MEMORY_CONFIGURATION,setSecureEraseAllDIMMs), &setSecureEraseAllDIMMs, sizeof(UINT8));

    if (Status == EFI_SUCCESS) {
      DEBUG((EFI_D_INFO,"\nsetSecureEraseAllDIMMs is Successfully disabled\n"));
    } else {
      DEBUG((EFI_D_INFO,"\nFailed to disable setSecureEraseAllDIMMs!!!\n"));
    }
  }

  for (dimm = 0; dimm < MAX_DCPMMS; dimm++) {
      if (Buffer.setSecureEraseSktCh[dimm] == 1) {
        DEBUG((EFI_D_INFO,"secure erase is enabled!\n"));
        DEBUG((EFI_D_INFO,"Disabling secure erase...\n"));
        setSecureEraseSktCh[dimm] = 0;
        Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF(SOCKET_MEMORY_CONFIGURATION,setSecureEraseSktCh[dimm]), &(setSecureEraseSktCh[dimm]), sizeof(UINT8));

        if (Status == EFI_SUCCESS) {
          DEBUG((EFI_D_INFO,"\nSecure erase is Successfully disabled\n"));
        } else {
          DEBUG((EFI_D_INFO,"\nFailed to disable secure erase is enabled!\n"));
        }
      }
  }

  //HSD-4928648: partitionDDRTDimm should be disabled after rebooting so that DDRT DIMMs don't get repartitioned
  if (Buffer.dfxPartitionDDRTDimm == 1) {
    DEBUG((EFI_D_INFO,"partition Pmem Dimm is enabled!\n"));
    DEBUG((EFI_D_INFO,"Disabling partition Pmem Dimm...\n"));
    partitionDDRTDimm = 0;
    Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF(SOCKET_MEMORY_CONFIGURATION,dfxPartitionDDRTDimm), &partitionDDRTDimm, sizeof(UINT8));
    if (Status == EFI_SUCCESS) {
      DEBUG((EFI_D_INFO,"\npartition Pmem Dimm is Successfuly disabled\n"));
    } else {
      DEBUG((EFI_D_INFO,"\nFailed to disable partition Pmem Dimm!!!\n"));
    }
  } else {
    DEBUG((EFI_D_INFO,"partition Pmem Dimm is disabled!!!\n"));
  }
}

/**

    GC_TODO: add routine description

    @param Offset - GC_TODO: add arg description
    @param Size   - GC_TODO: add arg description
    @param Data   - GC_TODO: add arg description

    @retval None

**/
STATIC
VOID
AccessExtendedCmos (
  IN      INT8                      Offset,
  IN      UINTN                     Size,
  IN OUT  VOID                      *Data
  )
{
  EFI_STATUS                        Status;
  EFI_CPU_IO2_PROTOCOL               *CpuIo2;
  BOOLEAN                           ReadCmos;

  Status = gBS->LocateProtocol (
                  &gEfiCpuIo2ProtocolGuid,
                  NULL,
                  &CpuIo2
                  );
  ASSERT_EFI_ERROR (Status);

  if (Offset < 0) {
    ReadCmos = FALSE;
    Offset = -Offset;
  } else {
    ReadCmos = TRUE;
  }

  while (Size > 0) {
    //
    // Write to extended CMOS address register
    //
    CpuIo2->Io.Write (CpuIo2, EfiCpuIoWidthUint8, R_IOPORT_CMOS_UPPER_INDEX, 1, &Offset);

    //
    // Read/Write from/to the specified location
    //
    if (ReadCmos) {
      CpuIo2->Io.Read (CpuIo2, EfiCpuIoWidthUint8, R_IOPORT_CMOS_UPPER_DATA, 1, Data);
    } else {
      CpuIo2->Io.Write (CpuIo2, EfiCpuIoWidthUint8, R_IOPORT_CMOS_UPPER_DATA, 1, Data);
    }

    //
    // Advance to next element
    //
    Size--;
    Offset++;
    Data = (UINT8*)Data + 1;
  }
}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
BOOLEAN
IsCMOSBad(
     VOID
)
{
  UINT8           Nmi;
  volatile UINT32 Data32;
  Nmi     = 0;
  Data32  = 0;
  //
  // Preserve NMI bit setting
  //
  Nmi = (ReadNmiEn () & B_PCH_IO_NMI_EN_NMI_EN);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  if (IoRead8 (R_IOPORT_CMOS_STANDARD_DATA) & (BIT6 + BIT7)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

VOID
clearCmos0E()
{
  UINT8           Value;
  UINT8           Nmi;
  volatile UINT32 Data32;
  Nmi     = 0;
  Data32  = 0;
  //
  // Preserve NMI bit setting
  //
  Nmi = (ReadNmiEn () & B_PCH_IO_NMI_EN_NMI_EN);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  Value = IoRead8 (R_IOPORT_CMOS_STANDARD_DATA);
  IoWrite8 (R_IOPORT_CMOS_STANDARD_DATA, Value & 0x3F);
}

/**

    GC_TODO: add routine description

    @param Default - GC_TODO: add arg description

    @retval None

**/
VOID
UpdateIioDefaultConfigViaBoard (
   IN SYSTEM_CONFIGURATION *Default
   )
{
  UpdateIioDefaultConfig (Default);
}


/**

  Restore Admin password if it was saved in the HOB.

    @param Defaults - pointer to the structure of the configuration vars

  @retval None.

**/
VOID
RestoreAdminPassword (
   IN SYSTEM_CONFIGURATION *Defaults
   )
{

    EFI_HOB_GUID_TYPE       *GuidHob;
    UINT32                  DataSize;

    GuidHob = GetFirstGuidHob(&gEfiAdminPasswordHobGuid);
    if( GuidHob!= NULL) {
        DataSize = GET_GUID_HOB_DATA_SIZE(GuidHob);
        if(sizeof( Defaults->AdminPassword ) >= DataSize){
            CopyMem (
                &(Defaults->AdminPassword),
                GET_GUID_HOB_DATA( GuidHob ),
                DataSize
            );
        }else{
            DEBUG((EFI_D_ERROR, "AdminPassword HOB too large. Password isn't restored.\n"));
        }
    }
}

// APTIOV_SERVER_OVERRIDE_RC_START : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
EFI_STATUS
ParseIfrPackForIntelRcSetup(
IN EFI_HANDLE        ImageHandle
)
{
    EFI_STATUS                            Status = EFI_SUCCESS;
    EFI_HANDLE                            DriverHandle;
    EFI_HII_HANDLE                        *HiiHandle;
    EFI_GUID                              *FormsetGuid;
    HII_VENDOR_DEVICE_PATH_NODE           *VendorDevicePath;

    BOOLEAN                               ActionFlag;
    BOOLEAN                               SetDefaultFlag;
    EFI_STRING                            ConfigRequestHdr;
    EFI_HII_PACKAGE_LIST_HEADER           *PackageList;
    EFI_HII_DATABASE_PROTOCOL             *HiiDatabase;
    UINT16                                Offset;
    EFI_IFR_OP_HEADER                     *IfrOpHdr;
// APTIOV_SERVER_OVERRIDE_RC_START : Changes done for EIP# 118513 
    EFI_FORM_ID                           FormId = VFR_FORMID_INTELRCSETUP;
    BOOLEAN                               GetFormSet = FALSE;
    BOOLEAN                               GetForm = FALSE;
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done for EIP# 118513 
    
    ZeroMem (&mSystemConfiguration, sizeof (mSystemConfiguration));

    CopyMem (&mSystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));
    
    ConfigRequestHdr = NULL;
    ActionFlag       = FALSE;
    SetDefaultFlag   = FALSE;
    
    if (mBootMode == BOOT_WITH_DEFAULT_SETTINGS) {
        SetDefaultFlag = TRUE;

        UpdateIioDefaultConfigViaBoard(&mSystemConfiguration);

        Status = SetSpecificConfigGuid (&gEfiSetupVariableGuid, &mSystemConfiguration);
        
	    ASSERT_EFI_ERROR (Status);
    }

    Status = gBS->OpenProtocol (
                          ImageHandle,
                          &gEfiHiiPackageListProtocolGuid,
                          (VOID **) &PackageList,
                          ImageHandle,
                          NULL,
                          EFI_OPEN_PROTOCOL_GET_PROTOCOL
                          );
    if (EFI_ERROR (Status)) {
        return Status;
    }
    
    Status = gBS->LocateProtocol (
                            &gEfiHiiDatabaseProtocolGuid,
                            NULL,
                            &HiiDatabase
                            );
    if (EFI_ERROR (Status)) {
        return Status;
    }
    
    {
        EFI_HII_PACKAGE_HEADER *IfrPackagePtr;
    
        for (IfrPackagePtr = (EFI_HII_PACKAGE_HEADER*)(PackageList+1);
             IfrPackagePtr < (EFI_HII_PACKAGE_HEADER*)((UINT8*)PackageList+PackageList->PackageLength);
             IfrPackagePtr = (EFI_HII_PACKAGE_HEADER*)((UINT8*)IfrPackagePtr+IfrPackagePtr->Length)) {
             if (IfrPackagePtr->Type == EFI_HII_PACKAGE_FORMS) {
                break;
            }
        }

        IfrOpHdr   = (EFI_IFR_OP_HEADER *)((UINT8 *) IfrPackagePtr + sizeof (EFI_HII_PACKAGE_HEADER));
    // APTIOV_SERVER_OVERRIDE_RC_START : Changes done for EIP# 118513 
        Offset     = sizeof (EFI_HII_PACKAGE_HEADER);
        while (Offset < IfrPackagePtr->Length) {
            if (IfrOpHdr->OpCode == EFI_IFR_FORM_SET_OP) {
                if (CompareGuid((GUID *)(VOID *)&((EFI_IFR_FORM_SET *) IfrOpHdr)->Guid, &mSystemConfigurationGuid)) {
                    GetFormSet = TRUE;
                }
                else {
                    GetFormSet = FALSE;
                }
            }
            else if (IfrOpHdr->OpCode == EFI_IFR_FORM_OP || IfrOpHdr->OpCode == EFI_IFR_FORM_MAP_OP) {
                if (CompareMem (&((EFI_IFR_FORM *) IfrOpHdr)->FormId, &FormId, sizeof (EFI_FORM_ID)) == 0) {
                    GetForm = TRUE;
                }
                else {
                    GetForm = FALSE;
                }
            }

            if (GetFormSet && GetForm) {
        // APTIOV_SERVER_OVERRIDE_RC_END : Changes done for EIP# 118513 
                DEBUG((EFI_D_ERROR, "INTELRCSETUP page FOUND in IFRPACK \n"));
                mHiiVendorDevicePathTemplate.Node.UniqueId++;
                VendorDevicePath = AllocateCopyPool (sizeof (HII_VENDOR_DEVICE_PATH), &mHiiVendorDevicePathTemplate);
                ASSERT (VendorDevicePath != NULL);
        
                DriverHandle = NULL;
                Status = gBS->InstallMultipleProtocolInterfaces (
                                                                &DriverHandle,
                                                                &gEfiDevicePathProtocolGuid,
                                                                VendorDevicePath,
                                                                NULL
                                                                );
                ASSERT_EFI_ERROR (Status);
                mAdvancedDriverHandle = DriverHandle;
                FormsetGuid = &gAdvancedPkgListGuid;
                HiiHandle = &mAdvancedHiiHandle;
                InstallAdvancedCallbackRoutine (mAdvancedDriverHandle, &mAdvancedConfigAccess);
                break;
            }
            //
            // Go to the next Op-Code
            //
            Offset   += IfrOpHdr->Length;
            IfrOpHdr = (EFI_IFR_OP_HEADER *) ((CHAR8 *) (IfrOpHdr) + IfrOpHdr->Length);
        }
    
        if (Offset >= IfrPackagePtr->Length)
        {
            DEBUG((EFI_D_ERROR, "INTELRCSETUP page not found in IFRPACK \n"));
            ASSERT_EFI_ERROR (Status);
        }
    
         Status = HiiDatabase->NewPackageList(HiiDatabase, PackageList, DriverHandle, HiiHandle);
    
        
         ConfigRequestHdr = HiiConstructConfigHdr (&mSystemConfigurationGuid, mVarStoreName, DriverHandle);
         ASSERT (ConfigRequestHdr != NULL);
         if (SetDefaultFlag) {
               ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
               ASSERT (ActionFlag);
          } else if (!SetDefaultFlag) { 
    #if INTELRCSETUP_HiiValidateSettings	
         DEBUG((EFI_D_ERROR, "Validating INTELRCSETUP IFR options \n"));			 
               ActionFlag = HiiValidateSettings (ConfigRequestHdr);
               ASSERT (ActionFlag);
    #endif	
          }		       
          FreePool (ConfigRequestHdr);
    }
// APTIOV_SERVER_OVERRIDE_RC_START : Update ME setup strings
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
#if ME_WS_SUPPORT
  if (MeTypeIsClient ()) {
    InitMeInfo (mAdvancedHiiHandle, VFR_INTELRCSETUP_CLASS);
    InitIccStrings (mAdvancedHiiHandle, VFR_INTELRCSETUP_CLASS);
  }
#endif // ME_WS_SUPPORT
#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
  if (MeTypeIsSps ()) {
    MeInitSpsSetupStrings(mAdvancedHiiHandle, mMainHiiHandle);
  }
#endif // ME_SPS_SUPPORT
#endif // ME_SUPPORT_FLAG
// APTIOV_SERVER_OVERRIDE_RC_END : Update ME setup strings
    return Status;
    }
// Entire routine commented out
#if 0
VOID
EFIAPI
InstallHiiDataAndGetSettings (
  IN EFI_HII_PACKAGE_HEADER      *StringPack,
  ...
  )
/**

  Installs a string and ifr pack set.

  @param Hii         -  pointer to an HII database protocol structure.
  @param StringPack  -  string pack to store in database and associate with IfrPack.
  @param IrfPack     -  ifr pack to store in database (it will use StringPack data).

  @retval None.

**/
{
  EFI_STATUS                            Status;
  VA_LIST                               Args;
  EFI_HANDLE                            DriverHandle;
  EFI_HII_HANDLE                        *HiiHandle;
  EFI_HII_PACKAGE_HEADER                *IfrPack;
  EFI_GUID                              *FormsetGuid;
  HII_VENDOR_DEVICE_PATH_NODE           *VendorDevicePath;

  UINTN                           BufferSize;
  BOOLEAN                         ActionFlag;
  BOOLEAN                         SetDefaultFlag;
  EFI_STRING                      ConfigRequestHdr;
  BOOLEAN                         CMOSStatus;

  ZeroMem (&mSystemConfiguration, sizeof (mSystemConfiguration));
  ConfigRequestHdr = NULL;
  ActionFlag       = FALSE;
  SetDefaultFlag   = FALSE;
  BufferSize       = sizeof (mSystemConfiguration);
  CMOSStatus       = IsCMOSBad();

  if (CMOSStatus || (mBootMode == BOOT_WITH_DEFAULT_SETTINGS)) {
    SetDefaultFlag = TRUE;
    DEBUG ((EFI_D_ERROR, "Is CMOS Bad = %x\n", CMOSStatus));
    // If the Clear CMOS is set, reset it.
    if (PmcIsRtcBatteryGood())
      PmcClearRtcPowerFailureStatus ();
    // If CMOS bits were set, we reset this:
    DEBUG ((EFI_D_INFO, "CMOS bad, force user to enter setup.\n"));
    clearCmos0E ();

    UpdateIioDefaultConfigViaBoard(&(mSetupData.SystemConfig));

    //
    // Prevent resetting admin password
    //
    RestoreAdminPassword(&(mSetupData.SystemConfig));

  }

  CopyMem (&mSystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));
  HiiHandle = NULL;
  FormsetGuid = NULL;

  IfrPack = NULL;
  VA_START (Args, StringPack);
  while ((IfrPack = VA_ARG (Args, EFI_HII_PACKAGE_HEADER *)) != NULL) {
    //
    // Increment UniqueID to distinguish from different device paths
    //
    mHiiVendorDevicePathTemplate.Node.UniqueId++;

    VendorDevicePath = AllocateCopyPool (sizeof (HII_VENDOR_DEVICE_PATH), &mHiiVendorDevicePathTemplate);
    ASSERT (VendorDevicePath != NULL);

    DriverHandle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &DriverHandle,
                    &gEfiDevicePathProtocolGuid,
                    VendorDevicePath,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);

    if (IfrPack == (EFI_HII_PACKAGE_HEADER *) MainBin) {
      mMainDriverHandle = DriverHandle;
      FormsetGuid = &gMainPkgListGuid;
      HiiHandle = &mMainHiiHandle;
      InstallDefaultCallbackRoutine (mMainDriverHandle, &mMainConfigAccess);
    } else if (IfrPack == (EFI_HII_PACKAGE_HEADER *) AdvancedBin) {
      mAdvancedDriverHandle = DriverHandle;
      FormsetGuid = &gAdvancedPkgListGuid;
      HiiHandle = &mAdvancedHiiHandle;
      InstallAdvancedCallbackRoutine (mAdvancedDriverHandle, &mAdvancedConfigAccess);
    } else if (IfrPack == (EFI_HII_PACKAGE_HEADER *) BootOptionsBin) {
      mBootOptionsDriverHandle = DriverHandle;
      FormsetGuid = &gBootOptionsPkgListGuid;
      HiiHandle = &mBootOptionsHiiHandle;
      InstallDefaultCallbackRoutine (mBootOptionsDriverHandle, &mBootOptionsConfigAccess);
    } else {
      /*
        if IfrPack is ExitBin, LanguageBin, etc:
        These Vfr/Ifr Bins will not contribute to the default image, mSystemDefaults.
        So we "continue".
      */
      continue;
    }

    *HiiHandle = HiiAddPackages (
                 FormsetGuid,
                 DriverHandle,
                 IfrPack,
                 StringPack,
                 NULL
                 );
    ASSERT (*HiiHandle != NULL);

    ConfigRequestHdr = HiiConstructConfigHdr (&mSystemConfigurationGuid, mVarStoreName, DriverHandle);
    ASSERT (ConfigRequestHdr != NULL);

    if (SetDefaultFlag) {
// SKX_TODO: Remove this as it causing an assert, debug and fix.
//      ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
//      ASSERT (ActionFlag);
    } else if (!SetDefaultFlag) {
//
// HSX 4167642: Long boot up time
// The below functionality consumes ~5 seconds. This is not mandatory
//
//      ActionFlag = HiiValidateSettings (ConfigRequestHdr);
//      ASSERT (ActionFlag);
    }
    FreePool (ConfigRequestHdr);
  }

  VA_END(Args);
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
#if ME_WS_SUPPORT
  if (MeTypeIsClient ()) {
    InitMeInfo (mAdvancedHiiHandle, VFR_ADVANCED_CLASS);
    InitIccStrings (mAdvancedHiiHandle, VFR_ADVANCED_CLASS);
  }
#endif // ME_WS_SUPPORT
#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
  if (MeTypeIsSps ()) {
    MeInitSpsSetupStrings(mAdvancedHiiHandle, mMainHiiHandle);
  }
#endif // ME_SPS_SUPPORT
#endif // ME_SUPPORT_FLAG
}
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.

/**

    GC_TODO: Route the PCH Refference code config


    @param Request  - Request packet
    @param Progress - Progress string
    @param Results  - REsults string

    @retval EFI_INVALID_PARAMETER - GC_TODO: add retval description
    @retval EFI_NOT_FOUND         - GC_TODO: add retval description
    @retval Status                - GC_TODO: add retval description

**/

EFI_STATUS
PchRcExtractConfig (
  IN CONST EFI_STRING  Request,
  OUT EFI_STRING      *Progress,
  OUT EFI_STRING      *Results
  )
{
  EFI_STATUS Status;
  UINTN BufferSize;
  PCH_SETUP CfgData;

  ASSERT (Request  != NULL);
  ASSERT (Progress != NULL);
  ASSERT (Results  != NULL);

  if (Request == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Request;
  BufferSize = sizeof (CfgData);

  CopyMem (&CfgData, &(mSetupData.PchSetup), sizeof (PCH_SETUP));

  //
  // Convert buffer data to <ConfigResp>.
  //
  Status = mHiiConfigRouting->BlockToConfig (
                                mHiiConfigRouting,
                                Request,
                                (VOID *) &CfgData,
                                BufferSize,
                                Results,
                                Progress
                                );

  return Status;
}

/**
  Function performs initialization on Setup Enter Event

  @return Void.
**/
VOID
EFIAPI
PchOnSetupEnter(
  IN EFI_EVENT Event,
  IN VOID *Context
)
{
// APTIOV_SERVER_OVERRIDE_RC_START : Changes made for Updating the setup variables before entering Setup.
  SYSTEM_CONFIGURATION SystemConfig;
// APTIOV_SERVER_OVERRIDE_RC_END : Changes made for Updating the setup variables before entering Setup.
  EFI_STATUS            Status;
  VOID                  *HobPtr;
  PCH_INFO_HOB          *PchInfoHob;
  PCH_SETUP             PchSetupData;

  Status = GetSpecificConfigGuid (&gPchSetupVariableGuid, &PchSetupData);
  ASSERT_EFI_ERROR (Status);

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  if (PchInfoHob == NULL) {
    ASSERT (FALSE);
    return;
  }
// APTIOV_SERVER_OVERRIDE_RC_START : Changes made for Updating the setup variables before entering Setup.
    //
    //Initializing the values
    //
  PchSetupData.GbeRegionInvalid = mPlatformType->PchData.GbeRegionInvalid;
  PchSetupData.LomLanSupported = mPlatformType->PchData.LomLanSupported;
// APTIOV_SERVER_OVERRIDE_RC_END : Changes made for Updating the setup variables before entering Setup.
  if (PchSetupData.PchCridSupport != (UINT8) PchInfoHob->CridSupport) {
    PchSetupData.PchCridSupport = (UINT8) PchInfoHob->CridSupport;
    Status = SetSpecificConfigGuid (&gPchSetupVariableGuid, &PchSetupData);
    ASSERT_EFI_ERROR (Status);
  }    
// APTIOV_SERVER_OVERRIDE_RC_START : Changes made for Updating the setup variables before entering Setup.
  Status = GetSpecificConfigGuid (&gEfiSetupVariableGuid, &SystemConfig);
  if (!EFI_ERROR(Status)) {
      //
      //Initializing the values of "IntelSetup" setup variable.
      //
      SystemConfig.GbePciePortNum = mPlatformType->PchData.GbePciePortNum;
      SystemConfig.GbeEnabled = mPlatformType->PchData.GbeEnabled;
      SystemConfig.PchStepping = mPlatformType->PchData.PchStepping;
      SystemConfig.PciePortConfig1 = mPlatformType->PchData.PchPciePortCfg1;
      SystemConfig.PciePortConfig2 = mPlatformType->PchData.PchPciePortCfg2;
      SystemConfig.PciePortConfig3 = mPlatformType->PchData.PchPciePortCfg3;
      SystemConfig.PciePortConfig4 = mPlatformType->PchData.PchPciePortCfg4;
      SystemConfig.PciePortConfig5 = mPlatformType->PchData.PchPciePortCfg5;
      SystemConfig.PcieSBDE = mPlatformType->PchData.PchPcieSBDE;
      
      Status = SetSpecificConfigGuid (&gEfiSetupVariableGuid, &SystemConfig);
      if(EFI_ERROR(Status))
          DEBUG((DEBUG_ERROR,"The Setup variables are not updated... Status: %r\n", Status));
  }
// APTIOV_SERVER_OVERRIDE_RC_END : Changes made for Updating the setup variables before entering Setup.
}

/**

    GC_TODO: add routine description

    @param This     - GC_TODO: add arg description
    @param Request  - GC_TODO: add arg description
    @param Progress - GC_TODO: add arg description
    @param Results  - GC_TODO: add arg description

    @retval EFI_INVALID_PARAMETER - GC_TODO: add retval description
    @retval EFI_NOT_FOUND         - GC_TODO: add retval description
    @retval Status                - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
DefaultExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  EFI_HANDLE                       Handle;
  EFI_STRING                       ConfigRequestHdr;
  EFI_STRING                       ConfigRequest;
  UINTN                            Size;
  BOOLEAN                          AllocatedRequest;

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }


  *Progress        = Request;

#if ME_WS_SUPPORT
  if (MeTypeIsClient ()) {
    //
    // special handling for the ME setup storage
    //
    if ((Request != NULL) && HiiIsConfigHdrMatch (Request, &gEfiMeRcVariableGuid, mMeSetupStorageName)) {
      return MeExtractConfigClientWrapper (Request, Progress, Results);
    }
    if ((Request != NULL) && HiiIsConfigHdrMatch (Request, &gEfiMeRcVariableGuid, ICC_SETUP_DATA_C_NAME)) {
      return IccExtractConfigWrapper (Request, Progress, Results);
    }
  }
#endif // ME_WS_SUPPORT

  //
  // special handling for the PCh config
  //
  if ((Request != NULL) && HiiIsConfigHdrMatch (Request, &gPchSetupVariableGuid, PCH_SETUP_NAME)) {
    return PchRcExtractConfig (Request, Progress, Results);
  }

  //
  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  //
  if ((Request != NULL) && !HiiIsConfigHdrMatch (Request, &mSystemConfigurationGuid, mVarStoreName)) {
    return EFI_NOT_FOUND;
  }

  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  Size             = 0;
  AllocatedRequest = FALSE;

  //
  // Get Buffer Storage data from EFI variable
  //

  BufferSize = sizeof (SYSTEM_CONFIGURATION);

  CopyMem (&mSystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));
  mPchSetup.LomLanSupported = mPlatformType->PchData.LomLanSupported;

  ConfigRequest = Request;
  if (Request == NULL || (StrStr (Request, L"OFFSET") == NULL)) {
    //
    // Return all configurable elements
    //
    if (This == &mAdvancedConfigAccess) {
      Handle = mAdvancedDriverHandle;
    } else if (This == &mMainConfigAccess) {
      Handle = mMainDriverHandle;
    } else {
      return EFI_NOT_FOUND;
    }

    //
    // Request is set to NULL, construct full request string.
    //

    //
    // First Set ConfigRequestHdr string.
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&mSystemConfigurationGuid, mVarStoreName, Handle);
    ASSERT (ConfigRequestHdr != NULL);

    if (ConfigRequestHdr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    ASSERT (ConfigRequest != NULL);

    if (ConfigRequest == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
  }

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = mHiiConfigRouting->BlockToConfig (
                                mHiiConfigRouting,
                                ConfigRequest,
                                (UINT8 *) &mSystemConfiguration,
                                BufferSize,
                                Results,
                                Progress
                                );

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }
  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = (EFI_STRING)(Request + StrLen (Request));
  }

  return Status;
}

/**
  setup reset call back function

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
PlatformSetupResetCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  static BOOLEAN              IsExecuted;
  EFI_RESET_TYPE              OrgResetType;
  PCH_RESET_DATA              NewResetData;

  if (IsExecuted) {
    return;
  }

  IsExecuted = TRUE;

  DEBUG ((DEBUG_INFO, "%a() Executed\n", __FUNCTION__));

  OrgResetType = ResetType;
  ResetType = PlatformReset (ResetType);
  if (OrgResetType == ResetType) {
    return;
  }

  DEBUG ((DEBUG_INFO, "New ResetType is %d\n", ResetType));

  if (ResetType != EfiResetPlatformSpecific) {
    gRT->ResetSystem (ResetType, EFI_SUCCESS, 0, NULL);
  } else {
    DataSize = sizeof (PCH_RESET_DATA);
    CopyMem (&NewResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (NewResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, DataSize, &NewResetData);
  }
}

/**
  Hook the system reset to properly reset function for platform Setup reset

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnResetFilterInstall (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS                      Status;
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetNotify;

  Status = gBS->LocateProtocol (&gEdkiiPlatformSpecificResetFilterProtocolGuid, NULL, (VOID**)&ResetNotify);
  if (!EFI_ERROR (Status)) {
    Status = ResetNotify->RegisterResetNotify (ResetNotify, PlatformSetupResetCallback);
    if (Event) {
      gBS->CloseEvent (Event);
    }
  }
}

/**
  Add a reset filter protocol call back for Setup exit function.
**/
VOID
InstallPlatformReset (
  VOID
  )
{
  VOID   *Registration;

  EfiCreateProtocolNotifyEvent (
    &gEdkiiPlatformSpecificResetFilterProtocolGuid,
    TPL_CALLBACK,
    OnResetFilterInstall,
    NULL,
    &Registration
    );
}

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
/**
  Function performs initialization of ME Setup on Setup enter event

  @param[in]     Event
  @param[in out] *Context

  @return Void.
**/
VOID
EFIAPI
MeOnEnterSetup(
  IN EFI_EVENT Event,
  IN VOID *Context
)
{
  ME_RC_CONFIGURATION MeRcConfig;
  EFI_STATUS          Status;
#if ME_TESTMENU_FLAG
  UINT8               DamState = FALSE;
#endif // ME_TESTMENU_FLAG
  SPS_SMM_WATCHDOG_HEADER  SpsSmmWatchdogHeader;
  EFI_HOB_GUID_TYPE        *pGuidHob;
  SPS_INFO_HOB             *pSpsHob;
  UINT8                    MeType;

  DEBUG((EFI_D_ERROR, "[ME] MeOnEnterSetup() called\n"));

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, &MeRcConfig);

  if (!EFI_ERROR (Status)) {
    CopyMem (&MeRcConfigurationOrg, &MeRcConfig, sizeof (ME_RC_CONFIGURATION));

    MeType = (UINT8) MeTypeGet ();
    if ((MeRcConfig.MeType == METYPE_UNDEF || MeRcConfig.MeType == METYPE_DISABLED)
        && MeType != MeRcConfig.MeType) {
      MeRcConfig.MeType = MeType;
      MeTypeShowDebug (L"Set MeType in MeOnEnterSetup");
    }

#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
    if(MeTypeIsSps() || MeTypeIsDfx())
    {
      Status = MeExtractSpsConfigOnSetupEnter(&MeRcConfig);
      ASSERT_EFI_ERROR (Status);
    }
#endif // ME_SPS_SUPPORT

#if ME_TESTMENU_FLAG
    MeRcConfig.MeEnableHeciTraces = (UINT8) GetHeciTraceDumpState ();
    // Get DAM state
    if (!EFI_ERROR (ServerHeciGetDamState (&DamState))) {
      MeRcConfig.DelayedAuthenticationModeEnabled = 1;
    } else {
      // Can't determine DAM state
      MeRcConfig.DelayedAuthenticationModeEnabled = 0;
    }
    DEBUG((EFI_D_INFO, "[ME] Set Delayed Authentication mode to setup: %d\n", DamState));
    MeRcConfig.DelayedAuthenticationMode = DamState;
#endif // ME_TESTMENU_FLAG

    pGuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
    if (pGuidHob != NULL) {
      pSpsHob = GET_GUID_HOB_DATA (pGuidHob);
      if (pSpsHob != NULL) {
        MeRcConfig.MeDualBIOSSupport = (pSpsHob->FeatureSet.Bits.DualBIOSSupport ? 1 : 0);
      }
    } else {
      DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot find SPS info HOB\n"));
    }

    if (CompareMem (&MeRcConfigurationOrg, &MeRcConfig, sizeof(ME_RC_CONFIGURATION)) != 0) {
      Status = SetSpecificConfigGuid (&gEfiMeRcVariableGuid, &MeRcConfig);
    }

    // On Setup entry get configuration to determine if anything has been changed
    CopyMem (&MeRcConfigurationOrg, &MeRcConfig, sizeof(ME_RC_CONFIGURATION));
    CopyMem (&MeRcConfigurationProcessed, &MeRcConfig, sizeof(ME_RC_CONFIGURATION));
  }
#if ME_SPS_SUPPORT
#if ME_TESTMENU_FLAG
  MeUefiFwHealthStatusInitSetupStrings (mAdvancedHiiHandle, mMainHiiHandle);
#endif // ME_TESTMENU_FLAG
#endif // ME_SPS_SUPPORT

  if (MeTypeIsSps () && !MeIsAfterEndOfPost () && MeWatchdogControlIsEnabed ()) {
    //select command for watchdog
    DEBUG ((DEBUG_INFO, "[SPS] Sending SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ\n"));
    ZeroMem (&SpsSmmWatchdogHeader, sizeof (SpsSmmWatchdogHeader));
    SpsSmmWatchdogHeader.ActionFlag = BIOS_UPDATE_WATCHDOG_CTRL_FLAG_SUSPEND;
    Status = MeWatchdogControlNotifySmm (&SpsSmmWatchdogHeader);
  }
} // MeOnEnterSetup

/** Callback to run on UEFI FW Setup Exit.

 @param[in]    Event    the event, not used
 @param[in]    Context  context, not used

 @return Void.
 */
VOID
EFIAPI
MeOnExitSetup (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{
  HECI_CONTROL             *pHeci;
  EFI_STATUS               Status;
  SPS_SMM_WATCHDOG_HEADER  SpsSmmWatchdogHeader;

  DEBUG ((DEBUG_INFO, "[SPS] %a called.\n", __FUNCTION__));

  if (!MeTypeIsSps() || MeIsAfterEndOfPost () || !MeWatchdogControlIsEnabed ()) {
    return;
  }

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: HECI protocol not found (%r)\n", Status));
    return;
  }

  //select command for watchdog
  DEBUG ((DEBUG_INFO, "[SPS] Sending SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ\n"));
  ZeroMem (&SpsSmmWatchdogHeader, sizeof (SpsSmmWatchdogHeader));
  SpsSmmWatchdogHeader.ActionFlag = BIOS_UPDATE_WATCHDOG_CTRL_FLAG_RESUME;
  Status = MeWatchdogControlNotifySmm (&SpsSmmWatchdogHeader);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: WatchdogControl command returns error=%r.\n", Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "[SPS] DualBiosWatchdog: Watchdog resumed. \n"));
}

/**

    RouteConfig for ME options

    @param None

    @retval Status                - function status

**/
EFI_STATUS
MeRcRouteConfig (
  VOID
  )
{
  EFI_STATUS          Status;
  ME_RC_CONFIGURATION MeRcConfig;

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, &MeRcConfig);

  if (!EFI_ERROR (Status)) {
    gMeResetRequests = FALSE;
#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
    if (MeTypeIsSps () || MeTypeIsDfx ()) {
      Status = MeRouteSpsAdvancedConfig (&MeRcConfig, &gMeResetRequests);
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "[ME] ERROR: MeRouteSpsAdvancedConfig() returns (%r)\n", Status));
        gMeResetRequests = TRUE;
      }
    }
#endif // ME_SPS_SUPPORT
    if (MeIsAfterEndOfPost () &&
        (CompareMem (&MeRcConfigurationOrg, &MeRcConfig, sizeof (ME_RC_CONFIGURATION)) != 0)) {
      gMeResetRequests = TRUE;
    }

    if (Status == EFI_SUCCESS) {
      if (CompareMem (&MeRcConfigurationProcessed, &MeRcConfig, sizeof (ME_RC_CONFIGURATION)) != 0) {
        Status = SetSpecificConfigGuid (&gEfiMeRcVariableGuid, &MeRcConfig);
        if (!EFI_ERROR (Status)) {
          // Set new processed configuration.
          CopyMem (&MeRcConfigurationProcessed, &MeRcConfig, sizeof (ME_RC_CONFIGURATION));
        }
      }
    }
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: MeRcRouteConfig() returns (%r)\n", Status));
  }

  return Status;
}
#endif // ME_SUPPORT_FLAG

#if defined (IPMI_SUPPORT)
#endif // IPMI_SUPPORT

/**

    GC_TODO: add routine description

    @param This          - GC_TODO: add arg description
    @param Configuration - GC_TODO: add arg description
    @param Progress      - GC_TODO: add arg description

    @retval EFI_INVALID_PARAMETER - GC_TODO: add retval description
    @retval EFI_NOT_FOUND         - GC_TODO: add retval description
    @retval Status                - GC_TODO: add retval description

**/
EFI_STATUS
PchRcRouteConfig (
  IN CONST EFI_STRING  Configuration,
  OUT EFI_STRING      *Progress
  )
{
  EFI_STATUS Status;
  PCH_SETUP CfgData;
  UINTN BufferSize;

  if (Configuration == NULL || Progress == NULL) {
    DEBUG ((EFI_D_ERROR, "Error!! PchRcRouteConfig () - Invalid Parameters\n"));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  //
  // Convert <ConfigResp> to buffer data.
  //
  BufferSize = sizeof (CfgData);
  Status = mHiiConfigRouting->ConfigToBlock (
                               mHiiConfigRouting,
                               Configuration,
                               (VOID *) &CfgData,
                               &BufferSize,
                               Progress
                               );
  DEBUG ((EFI_D_INFO, "%s: mHiiConfigRouting->ConfigToBlock() returns: %r.\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CopyMem (&(mSetupData.PchSetup), &CfgData, sizeof (PCH_SETUP));
  Status = SetSpecificConfigGuid (&gPchSetupVariableGuid, &CfgData);
  return Status;
}


/**

    GC_TODO: add routine description

    @param This          - GC_TODO: add arg description
    @param Configuration - GC_TODO: add arg description
    @param Progress      - GC_TODO: add arg description

    @retval EFI_INVALID_PARAMETER - GC_TODO: add retval description
    @retval EFI_NOT_FOUND         - GC_TODO: add retval description
    @retval Status                - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
DefaultRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_STATUS                                 Status;
  UINTN                                      BufferSize;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // It's Fake varstore
  // to handle Extract/RouteConfig callbacks on multi platform code
  //
  if (HiiIsConfigHdrMatch (Configuration, &mFakeVarstoreGuid, mFakeVarstoreName)) {
    //
    // Add platform W/A here for multi platform mode
    //
    DEBUG ((EFI_D_ERROR,"DefaultRouteConfig: it's fake varstore\n"));
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
    //
    // Special handling for ME configuration.
    //
    Status = MeRcRouteConfig ();
#endif // ME_SUPPORT_FLAG
  }


  //
  // Special handling for PCH Rc configuration.
  //
  if (HiiIsConfigHdrMatch (Configuration, &gPchSetupVariableGuid, PCH_SETUP_NAME)) {
    return PchRcRouteConfig (Configuration, Progress);
  }

#if ME_WS_SUPPORT
  //
  // special handling for the ME setup storage
  //
  if (MeTypeIsClient ()) {
    if (HiiIsConfigHdrMatch (Configuration, &gEfiMeRcVariableGuid, mMeSetupStorageName)) {
      return MeRouteConfigClientWrapper (Configuration, Progress);
    }
    if (HiiIsConfigHdrMatch (Configuration, &gEfiMeRcVariableGuid, ICC_SETUP_DATA_C_NAME)) {
      return IccRouteConfigWrapper (Configuration, Progress);
    }
  }
#endif // ME_WS_SUPPORT

  *Progress = Configuration;

  if (!HiiIsConfigHdrMatch (Configuration, &mSystemConfigurationGuid, mVarStoreName)) {
    //
    // Note: these are for the config variables in other than platform config, changes to values for them will be ignored,
    // as the proper place to change their values are in the pkg setup where the config exist.
    // We return success, otherwise Hii will pop an error message that values were not saved.
    //
    DEBUG ((EFI_D_INFO,"Configuration not found-> %s \n",Configuration ));
    return EFI_SUCCESS;
  }

  //
  // Get Buffer Storage data from EFI variable
  //
  CopyMem (&mSystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = sizeof (SYSTEM_CONFIGURATION);
  Status = mHiiConfigRouting->ConfigToBlock (
                               mHiiConfigRouting,
                               Configuration,
                               (UINT8 *) &mSystemConfiguration,
                               &BufferSize,
                               Progress
                               );

  //
  // Store Buffer Storage back to EFI variable
  //
  CopyMem (&(mSetupData.SystemConfig), &mSystemConfiguration, sizeof(SYSTEM_CONFIGURATION));
  Status = SetSpecificConfigGuid (&gEfiSetupVariableGuid, &mSystemConfiguration);

  return Status;
}

/**

    GC_TODO: add routine description

    @param This       - GC_TODO: add arg description
    @param Action     - GC_TODO: add arg description
    @param QuestionId - GC_TODO: add arg description
    @param Type       - GC_TODO: add arg description
    @param Value      - GC_TODO: add arg description
    @param OPTIONAL   - GC_TODO: add arg description

    @retval Status - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
DefaultCallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *FileSystemProtocol;
  EFI_FILE_PROTOCOL                 *Root;
  EFI_FILE_PROTOCOL                 *NewFile;
  UINTN                             Status = EFI_UNSUPPORTED;
  UINTN                             VarSize;
  UINT32                            VersionNumber;
  SYSTEM_CONFIGURATION              SystemConfiguration;

  if (Action == EFI_BROWSER_ACTION_CHANGED) {
    switch(QuestionId) {
      case MAIN_PAGE_KEY_SAVE:
        //
        // Save setup data to file
        //
        Status = gBS->LocateProtocol(
            &mEfiSimpleFileSystemProtocolGuid,
            NULL,
            &FileSystemProtocol
        );
        ASSERT_EFI_ERROR(Status);

        //
        // Create new file on the first file system encountered
        // If file already exists, it is overwritten
        //
        FileSystemProtocol->OpenVolume(FileSystemProtocol, &Root);
        Root->Open(
            Root,
            &NewFile,
            L"SetupSave.txt",
            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
        );

        //
        // Access setup variables and write the firmware version followed by the setup data
        //
        CopyMem (&SystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));

        VarSize = sizeof(UINT32);
        NewFile->Write(NewFile, &VarSize, &(gST->FirmwareRevision));

        VarSize = sizeof (SYSTEM_CONFIGURATION);
        Status = NewFile->Write(NewFile, &VarSize, &SystemConfiguration);

        NewFile->Close(NewFile);
        Root->Close(Root);

        Status = EFI_SUCCESS;
        break;

      case MAIN_PAGE_KEY_RESTORE:
        //
        // Restore setup data from file
        //
        Status = gBS->LocateProtocol(
            &mEfiSimpleFileSystemProtocolGuid,
            NULL,
            &FileSystemProtocol
        );
        ASSERT_EFI_ERROR(Status);

        FileSystemProtocol->OpenVolume(FileSystemProtocol, &Root);

        Status = Root->Open(
            Root,
            &NewFile,
            L"SetupSave.txt",
            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
        );

        if(Status != EFI_SUCCESS) {
          DEBUG((EFI_D_INFO, "File SetupSave.txt could not be found.\n", Status));
          break;
        }

        //
        // Access setup variables
        //
        CopyMem (&SystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));

        //
        // Verify the file was created from the same firmware version
        // Clean up and exit if it isn't
        //
        VarSize = sizeof(UINT32);
        NewFile->Read(NewFile, &VarSize, &VersionNumber);

        if(VersionNumber != gST->FirmwareRevision) {
          DEBUG((EFI_D_INFO, "Setup data save file is for a different firmware version\n", Status));
          NewFile->Close(NewFile);
          Root->Close(Root);
          break;
        }

        VarSize = sizeof (SYSTEM_CONFIGURATION);
        NewFile->Read(NewFile, &VarSize, &SystemConfiguration);
        CopyMem (&(mSetupData.SystemConfig), &SystemConfiguration, sizeof(SYSTEM_CONFIGURATION));
        Status = SetSpecificConfigGuid (&gEfiSetupVariableGuid, &SystemConfiguration);

        NewFile->Close(NewFile);
        Root->Close(Root);

        Status = EFI_SUCCESS;
        break;
      default:
        Status = EFI_UNSUPPORTED;
    break;
    }
  }
  return Status;
}

/**

    GC_TODO: add routine description

    @param DriverHandle - GC_TODO: add arg description
    @param ConfigAccess - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
InstallDefaultCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  )
{
  EFI_STATUS          Status;

  if (ConfigAccess == NULL) {
    DEBUG ((EFI_D_ERROR, "Error!! InstallDefaultCallbackRoutine () - Invalid Parameters\n"));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  ConfigAccess->ExtractConfig = DefaultExtractConfig;
  ConfigAccess->RouteConfig   = DefaultRouteConfig;
  ConfigAccess->Callback      = DefaultCallbackRoutine;

  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  ConfigAccess
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}


/**

    GC_TODO: add routine description

    @param This       - GC_TODO: add arg description
    @param Action     - GC_TODO: add arg description
    @param QuestionId - GC_TODO: add arg description
    @param Type       - GC_TODO: add arg description
    @param Value      - GC_TODO: add arg description
    @param OPTIONAL   - GC_TODO: add arg description

    @retval Status - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
AdvancedCallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
  EFI_STATUS                                 Status = EFI_SUCCESS;
  SYSTEM_CONFIGURATION                       SystemConfiguration;
  SOCKET_MP_LINK_CONFIGURATION               SocketMpLinkConfiguration;
  SOCKET_IIO_CONFIGURATION                   SocketIioConfiguration;
  SOCKET_PROCESSORCORE_CONFIGURATION         SocketProcessorCoreConfiguration;
  SOCKET_COMMONRC_CONFIGURATION              SocketCommonRcConfiguration;
  SOCKET_MEMORY_CONFIGURATION                SocketMemoryConfiguration;
  BOOLEAN                                    UpdateSetupVar = FALSE;
  BOOLEAN                                    UpdatePchRcVar = FALSE;
// APTIOV_SERVER_OVERRIDE_RC_START: Defines dummy control and update value from NVRAM or HOB in order to avoid load defaults issue.
  SYSTEM_MEMORY_MAP_HOB                      *SystemMemoryMap;
  EFI_HOB_GUID_TYPE                          *GuidHob;
  UINT32                                     Attributes;
  UINTN                                      Size;
// APTIOV_SERVER_OVERRIDE_RC_END: Defines dummy control and update value from NVRAM or HOB in order to avoid load defaults issue.
//APTIOV_SERVER_OVERRIDE_RC_START : For executing SetVariableOnCallback only once
  static BOOLEAN                             EnableSetVariableOnCallback = TRUE;
//APTIOV_SERVER_OVERRIDE_RC_END : For executing SetVariableOnCallback only once

  switch(Action)
  {
  case EFI_BROWSER_ACTION_FORM_CLOSE:
    //
    // Do nothing for UEFI CLOSE/OPEN Action
    //
  break;
  case EFI_BROWSER_ACTION_CHANGING:

    if (PcdGetBool (PcdMrcXmpSupport)) {

      if ((QuestionId == KEY_XMP_PROFILE) || (QuestionId == KEY_XMP_PROFILE1)) {
        XmpCallback( This, Action, QuestionId, Type, Value, ActionRequest);
        break;
      }
    }
#if ME_WS_SUPPORT
    if (MeTypeIsClient ()) {
      MeFormCallBackFunction (
        This,
        Action,
        QuestionId,
        Type,
        Value,
        ActionRequest
        );
#if defined(WS_ICC_SUPPORT) && WS_ICC_SUPPORT
      switch (QuestionId) {
      case KEY_ICC_PROFILE:
        IccProfileCallback (This, Action, QuestionId, Type, Value, ActionRequest);
        break;
      default:
        break;
      }
#endif // WS_ICC_SUPPORT
    }
#endif // ME_WS_SUPPORT
  break;
  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  {
    ME_RC_CONFIGURATION                        MeRcConfig;
    // APTIOV_SERVER_OVERRIDE_RC_START : Use proper return type
    BOOLEAN     BrowserDataStatus;
    BrowserDataStatus = HiiGetBrowserData (
          &gEfiMeRcVariableGuid,
          ME_RC_CONFIGURATION_NAME,
          sizeof(ME_RC_CONFIGURATION),
          (UINT8*)&MeRcConfig
        );
    if (BrowserDataStatus) {  
    // APTIOV_SERVER_OVERRIDE_RC_END : Use proper return type
      UINT8 MeType;

      MeType = (UINT8) MeTypeGet ();
      if ((MeRcConfig.MeType == METYPE_UNDEF || MeRcConfig.MeType == METYPE_DISABLED)
        && MeType != MeRcConfig.MeType) {
        MeRcConfig.MeType = MeType;

        HiiSetBrowserData(
          &gEfiMeRcVariableGuid,
          ME_RC_CONFIGURATION_NAME,
          sizeof(ME_RC_CONFIGURATION),
          (UINT8*)&MeRcConfig,
          NULL
        );
      }
    }
  }
#endif // ME_SUPPORT_FLAG
// APTIOV_SERVER_OVERRIDE_RC_START: Defines dummy control and update value from NVRAM or HOB in order to avoid load defaults issue.
    Status = EFI_UNSUPPORTED;
    if (QuestionId == 0xDFF1){
        GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
             if (GuidHob != NULL){
                 SystemMemoryMap = (struct SystemMemoryMapHob*) GET_GUID_HOB_DATA (GuidHob);
                 //Assigning System Ras type value to the setup option(Value->u8) to avoid loading optimal default in SOCKET_COMMONRC_CONFIGURATION.SystemRasType variable when doing "Restore default"
                 Value->u8 = SystemMemoryMap->SystemRasType;
                 Status = EFI_SUCCESS;
             } else {
                 Status = EFI_NOT_FOUND;
                 DEBUG ((EFI_D_ERROR, "\nGet SystemMemoryMap HOB data, Status = %r\n", Status));
             }
    } else if((QuestionId == 0xDFF2) || (QuestionId == 0xDFF3) || (QuestionId == 0xDFF4) || (QuestionId == 0xDFF5) || (QuestionId == 0xDFF7) || (QuestionId == 0xDFF8)) {
        Size = sizeof(SYSTEM_CONFIGURATION);
        Status = gRT->GetVariable (
                        L"IntelSetup",
                        &mSystemConfigurationGuid,
                        &Attributes,
                        &Size,
                        &SystemConfiguration );
        if (EFI_ERROR (Status)) {
            Status = EFI_NOT_FOUND;
            DEBUG ((EFI_D_ERROR, "\nIntelSetup GetVariable Status = %r\n", Status));
        } else {
            switch (QuestionId) {
                case 0xDFF2:
                    // Assigning PlatformOCSupport value to the setup option(Value->u8) to avoid loading optimal default in SYSTEM_CONFIGURATION.PlatformOCSupport variable when doing "Restore default".
                    Value->u8 = SystemConfiguration.PlatformOCSupport;
                    break;
                case 0xDFF3:
                    // Assigning BiosGuardPlatformSupported value to the setup option(Value->u8) to avoid loading optimal default in SYSTEM_CONFIGURATION.BiosGuardPlatformSupported variable when doing "Restore default".
                    Value->u8 = SystemConfiguration.BiosGuardPlatformSupported;
                    break;
                case 0xDFF4:
                    // Assigning AntiFlashWearoutSupported value to the setup option(Value->u8) to avoid loading optimal default in SYSTEM_CONFIGURATION.AntiFlashWearoutSupported variable when doing "Restore default".
                    Value->u8 = SystemConfiguration.AntiFlashWearoutSupported;
                    break;
            }
        }
    }
// APTIOV_SERVER_OVERRIDE_RC_END: Defines dummy control and update value from NVRAM or HOB in order to avoid load defaults issue.
  break;
  case EFI_BROWSER_ACTION_FORM_OPEN:
    if (IsSimicsEnvironment() == FALSE)
    {
      switch (QuestionId)
      {
        case 0x2007:
          HiiGetBrowserData (
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration
          );
          SystemConfiguration.GbePciePortNum  = mPlatformType->PchData.GbePciePortNum;
          mSystemConfiguration.GbePciePortNum = mPlatformType->PchData.GbePciePortNum;
          HiiSetBrowserData(
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration,
            NULL
          );
          UpdateSetupVar = TRUE;
        break;
        case 0x200A:
          HiiGetBrowserData (
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration
          );
          SystemConfiguration.GbeEnabled      = (UINT8) mPlatformType->PchData.GbeEnabled;
          mSystemConfiguration.GbeEnabled     = (UINT8) mPlatformType->PchData.GbeEnabled;
          HiiSetBrowserData(
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration,
            NULL
          );
          UpdateSetupVar = TRUE;
        break;
        case 0x200B:
          HiiGetBrowserData (
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration
          );
          SystemConfiguration.PchStepping  =  mPlatformType->PchData.PchStepping;
          mSystemConfiguration.PchStepping =  mPlatformType->PchData.PchStepping;
          HiiSetBrowserData(
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration,
            NULL
          );
          UpdateSetupVar = TRUE;
        break;
         // Update PchPciePortConfig according to softstraps info
        case 0x2001:
        case 0x2002:
        case 0x2003:
        case 0x2004:
        case 0x2005:
          HiiGetBrowserData (
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&mSystemConfiguration
          );
          mSystemConfiguration.PciePortConfig1 = mPlatformType->PchData.PchPciePortCfg1;
          mSystemConfiguration.PciePortConfig2 = mPlatformType->PchData.PchPciePortCfg2;
          mSystemConfiguration.PciePortConfig3 = mPlatformType->PchData.PchPciePortCfg3;
          mSystemConfiguration.PciePortConfig4 = mPlatformType->PchData.PchPciePortCfg4;
          mSystemConfiguration.PciePortConfig5 = mPlatformType->PchData.PchPciePortCfg5;
          HiiSetBrowserData(
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&mSystemConfiguration,
            NULL
          );
          UpdateSetupVar = TRUE;
        break;
        case 0x2006:
          HiiGetBrowserData (
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&mSystemConfiguration
          );
          mSystemConfiguration.PcieSBDE = mPlatformType->PchData.PchPcieSBDE;
          HiiSetBrowserData(
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&mSystemConfiguration,
            NULL
          );
          UpdateSetupVar = TRUE;
        break;
        case 0x2008:
          HiiGetBrowserData (
            &gPchSetupVariableGuid,
            PCH_SETUP_NAME,
            sizeof (PCH_SETUP),
            (UINT8*)&mPchSetup
          );
          mPchSetup.GbeRegionInvalid = mPlatformType->PchData.GbeRegionInvalid;
          HiiSetBrowserData (
            &gPchSetupVariableGuid,
            PCH_SETUP_NAME,
            sizeof (PCH_SETUP),
            (UINT8*)&mPchSetup,
            NULL
          );
          UpdatePchRcVar = TRUE;
        break;
        case 0x2009:
          HiiGetBrowserData (
            &gPchSetupVariableGuid,
            PCH_SETUP_NAME,
            sizeof (PCH_SETUP),
            (UINT8*)&mPchSetup
          );
          mPchSetup.LomLanSupported = mPlatformType->PchData.LomLanSupported;
          HiiSetBrowserData (
            &gPchSetupVariableGuid,
            PCH_SETUP_NAME,
            sizeof (PCH_SETUP),
            (UINT8*)&mPchSetup,
            NULL
          );
          UpdatePchRcVar = TRUE;
        break;
        default:
          DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
          break;
      }
      if (UpdateSetupVar) {
        CopyMem(&(mSetupData.SystemConfig), &mSystemConfiguration, sizeof(SYSTEM_CONFIGURATION));
        Status = SetSpecificConfigGuid(&gEfiSetupVariableGuid, &mSystemConfiguration);
      }
      if (UpdatePchRcVar) {
        CopyMem (&(mSetupData.PchSetup), &mPchSetup, sizeof (PCH_SETUP));
        Status = SetSpecificConfigGuid (&gPchSetupVariableGuid, &mPchSetup);
      }
    }
  break;
  case EFI_BROWSER_ACTION_RETRIEVE:
  break;
  case EFI_BROWSER_ACTION_CHANGED:
    switch (QuestionId) {
      case KEY_SERIAL_DEBUG_MSGLVL:
      if (Value->u8 == 0) {
        HiiGetBrowserData (
          &mSystemConfigurationGuid,
          mVariableName,
          sizeof(SYSTEM_CONFIGURATION),
          (UINT8*)&SystemConfiguration
          );

        SystemConfiguration.serialDebugTrace = Value->u8;
        SystemConfiguration.serialDebugMsgLvlTrainResults = Value->u8;

        HiiSetBrowserData(
          &mSystemConfigurationGuid,
          mVariableName,
          sizeof(SYSTEM_CONFIGURATION),
          (UINT8*)&SystemConfiguration,
          NULL
          );
      }
      break;

      case KEY_CLOUD_PROFILE:
        if (Value->u8 == 1) {
          HiiGetBrowserData (
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration
            );
          HiiGetBrowserData (
            &gEfiSocketMpLinkVariableGuid,
            SOCKET_MP_LINK_CONFIGURATION_NAME,
            sizeof(SOCKET_MP_LINK_CONFIGURATION),
            (UINT8*)&SocketMpLinkConfiguration
            );
          HiiGetBrowserData (
            &gEfiSocketIioVariableGuid,
            SOCKET_IIO_CONFIGURATION_NAME,
            sizeof(SOCKET_IIO_CONFIGURATION),
            (UINT8*)&SocketIioConfiguration
            );
          HiiGetBrowserData (
            &gEfiSocketProcessorCoreVarGuid,
            SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
            sizeof(SOCKET_PROCESSORCORE_CONFIGURATION),
            (UINT8*)&SocketProcessorCoreConfiguration
            );
          HiiGetBrowserData (
            &gEfiSocketMemoryVariableGuid,
            SOCKET_MEMORY_CONFIGURATION_NAME,
            sizeof(SOCKET_MEMORY_CONFIGURATION),
            (UINT8*)&SocketMemoryConfiguration
            );
          HiiGetBrowserData (
            &gEfiSocketCommonRcVariableGuid,
            SOCKET_COMMONRC_CONFIGURATION_NAME,
            sizeof(SOCKET_COMMONRC_CONFIGURATION),
            (UINT8*)&SocketCommonRcConfiguration
            );

          SocketMpLinkConfiguration.SncEn = 1;
          SocketCommonRcConfiguration.IsocEn = 0;
          SocketProcessorCoreConfiguration.DCUIPPrefetcherEnable = 0;
          SocketProcessorCoreConfiguration.DCUStreamerPrefetcherEnable = 0;
          SocketProcessorCoreConfiguration.MlcSpatialPrefetcherEnable = 0;
          SocketProcessorCoreConfiguration.MlcStreamerPrefetcherEnable = 0;
          SocketProcessorCoreConfiguration.CpuL1NextPagePrefetcherDisable = 1;
          SocketIioConfiguration.PcieGlobalAspm = 0;
          SystemConfiguration.SystemErrorEn = 1;
          SystemConfiguration.IioErrorEn = 1;
          SystemConfiguration.KTIFailoverSmiEn = 1;
          SystemConfiguration.WheaSupportEn = 1;
          SystemConfiguration.WheaErrInjEn = 1;
          SystemConfiguration.WheaLogMemoryEn = 1;
          SystemConfiguration.WheaLogPciEn = 1;
          SystemConfiguration.WheaLogProcEn = 1;
          SocketMemoryConfiguration.allowCorrectableError = ALLOW_CORRECTABLE_ERROR_ENABLE;
          SocketMemoryConfiguration.allowCorrectableMemTestError = ALLOW_CORRECTABLE_MEM_TEST_ERROR_ENABLE;
          HiiSetBrowserData(
            &mSystemConfigurationGuid,
            mVariableName,
            sizeof(SYSTEM_CONFIGURATION),
            (UINT8*)&SystemConfiguration,
            NULL
            );
          HiiSetBrowserData(
            &gEfiSocketMpLinkVariableGuid,
            SOCKET_MP_LINK_CONFIGURATION_NAME,
            sizeof(SOCKET_MP_LINK_CONFIGURATION),
            (UINT8*)&SocketMpLinkConfiguration,
            NULL
            );
          HiiSetBrowserData(
            &gEfiSocketIioVariableGuid,
            SOCKET_IIO_CONFIGURATION_NAME,
            sizeof(SOCKET_IIO_CONFIGURATION),
            (UINT8*)&SocketIioConfiguration,
            NULL
            );
          HiiSetBrowserData(
            &gEfiSocketProcessorCoreVarGuid,
            SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
            sizeof(SOCKET_PROCESSORCORE_CONFIGURATION),
            (UINT8*)&SocketProcessorCoreConfiguration,
            NULL
            );
          HiiSetBrowserData(
            &gEfiSocketMemoryVariableGuid,
            SOCKET_MEMORY_CONFIGURATION_NAME,
            sizeof(SOCKET_MEMORY_CONFIGURATION),
            (UINT8*)&SocketMemoryConfiguration,
            NULL
            );
          HiiSetBrowserData(
            &gEfiSocketCommonRcVariableGuid,
            SOCKET_COMMONRC_CONFIGURATION_NAME,
            sizeof(SOCKET_COMMONRC_CONFIGURATION),
            (UINT8*)&SocketCommonRcConfiguration,
            NULL
            );
        }
        break;
    //
    // Benchmark profiles
    //
    case KEY_PLATFORM_APPLICATION_PROFILE:
        SetupApplicationProfile((Value->u8));
        break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
    }
    break;
  default:
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

/**
  Setup callback executed

  @retval always SUCCESS

**/
EFI_STATUS
EFIAPI
XmpCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION Action,
  IN EFI_QUESTION_ID KeyValue,
  IN UINT8 Type,
  IN EFI_IFR_TYPE_VALUE *Value,
  OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest OPTIONAL
  )
{
  UINT8       profile;
  SOCKET_MEMORY_CONFIGURATION      SocketMemoryConfiguration;
  EFI_STATUS  Status;
  struct SystemMemoryMapHob             *systemMemoryMap;

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  systemMemoryMap = GetSystemMemoryMapData ();
  if (systemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  HiiGetBrowserData (&gEfiSocketMemoryVariableGuid,SOCKET_MEMORY_CONFIGURATION_NAME, sizeof(SOCKET_MEMORY_CONFIGURATION), (UINT8*)&SocketMemoryConfiguration);

  if (( (SocketMemoryConfiguration.XMPMode == XMP_PROFILE_1) || (SocketMemoryConfiguration.XMPMode == XMP_PROFILE_2)) && systemMemoryMap != NULL) {

    // Get the profile to use
    profile = SocketMemoryConfiguration.XMPMode - XMP_PROFILE_1;

    SocketMemoryConfiguration.tCAS          = systemMemoryMap->profileMemTime[profile].nCL;
    SocketMemoryConfiguration.tRP           = systemMemoryMap->profileMemTime[profile].nRP;
    SocketMemoryConfiguration.tRCD          = systemMemoryMap->profileMemTime[profile].nRCD;
    SocketMemoryConfiguration.tRAS          = systemMemoryMap->profileMemTime[profile].nRAS;
    SocketMemoryConfiguration.tWR           = systemMemoryMap->profileMemTime[profile].nWR;
    SocketMemoryConfiguration.tRFC          = systemMemoryMap->profileMemTime[profile].nRFC;
    SocketMemoryConfiguration.tRRD          = systemMemoryMap->profileMemTime[profile].nRRD;
    SocketMemoryConfiguration.tRTP          = systemMemoryMap->profileMemTime[profile].nRTP;
    SocketMemoryConfiguration.tWTR          = systemMemoryMap->profileMemTime[profile].nWTR;
    SocketMemoryConfiguration.tFAW          = systemMemoryMap->profileMemTime[profile].nFAW;
    SocketMemoryConfiguration.tCWL          = systemMemoryMap->profileMemTime[profile].nCWL;
    SocketMemoryConfiguration.tRC           = systemMemoryMap->profileMemTime[profile].nRC;
    SocketMemoryConfiguration.commandTiming = systemMemoryMap->profileMemTime[profile].nCMDRate;
    SocketMemoryConfiguration.DdrFreqLimit  = systemMemoryMap->profileMemTime[profile].ddrFreqLimit;
    SocketMemoryConfiguration.tREFI         = systemMemoryMap->profileMemTime[profile].tREFI;
    SocketMemoryConfiguration.Vdd           = systemMemoryMap->profileMemTime[profile].vdd;
  } else if (SocketMemoryConfiguration.XMPMode == XMP_AUTO) {
    SocketMemoryConfiguration.DdrFreqLimit  = DDR_FREQ_AUTO;
  }

  HiiSetBrowserData(&gEfiSocketMemoryVariableGuid,SOCKET_MEMORY_CONFIGURATION_NAME, sizeof(SOCKET_MEMORY_CONFIGURATION), (UINT8*)&SocketMemoryConfiguration, NULL);

  return EFI_SUCCESS;
} // XmpCallback

/**

    GC_TODO: add routine description

    @param DriverHandle - GC_TODO: add arg description
    @param ConfigAccess - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
InstallAdvancedCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  )
{
  EFI_STATUS          Status;

  if (ConfigAccess == NULL) {
    DEBUG ((EFI_D_ERROR, "Error!! InstallAdvancedCallbackRoutine () - Invalid Parameters\n"));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  ConfigAccess->ExtractConfig = DefaultExtractConfig;
  ConfigAccess->RouteConfig   = DefaultRouteConfig;
  ConfigAccess->Callback      = AdvancedCallbackRoutine;

  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  ConfigAccess
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

// APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
#if 0
/**

    GC_TODO: add routine description

    @param Event   - GC_TODO: add arg description
    @param Context - GC_TODO: add arg description

    @retval None

**/
VOID
EFIAPI
BackupSetupDataAndCheckForPassword (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINTN                    VariableSize;
  SYSTEM_CONFIGURATION     SystemConfiguration;
  SOCKET_MP_LINK_CONFIGURATION SocketMpLinkSetupData;
  SOCKET_COMMONRC_CONFIGURATION SocketCommonRcData;
  EFI_STATUS               Status;
  CHAR16                   Buffer[100];
  SOCKET_POWERMANAGEMENT_CONFIGURATION  SocketPowermanagementConfiguration;
  SOCKET_PROCESSORCORE_CONFIGURATION    SocketProcessorCoreConfiguration;

  //
  // We don't need this to be save into flash, just in memory is enough
  //
  CheckForPassword ();

  //
  // Get Buffer Storage data from EFI variable
  //
  CopyMem (&SystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));

  VariableSize = sizeof(SYSTEM_CONFIGURATION);
  Status = gRT->SetVariable(
                  mTempVariableName,
                  &mSystemConfigurationGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  VariableSize,
                  &SystemConfiguration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "%s Set Variable Status = %r\n", mTempVariableName, Status));
    return;
  }

  VariableSize = sizeof(SOCKET_MP_LINK_CONFIGURATION);
  CopyMem (&SocketMpLinkSetupData, &(mSetupData.SocketConfig.UpiConfig), sizeof(SOCKET_MP_LINK_CONFIGURATION));

  StrCpyS(Buffer, STRING_WIDTH_100, SOCKET_MP_LINK_CONFIGURATION_NAME );
  StrCatS(Buffer, STRING_WIDTH_100, L"Temp");

  Status = gRT->SetVariable(
                  Buffer,
                  &gEfiSocketMpLinkVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  VariableSize,
                  &SocketMpLinkSetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "%s Set Variable Status = %r\n", Buffer, Status));
    return;
  }

  VariableSize = sizeof(SOCKET_COMMONRC_CONFIGURATION);
  CopyMem (&SocketCommonRcData, &(mSetupData.SocketConfig.CommonRcConfig), sizeof(SOCKET_COMMONRC_CONFIGURATION));

  StrCpyS(Buffer, STRING_WIDTH_100, SOCKET_COMMONRC_CONFIGURATION_NAME );
  StrCatS(Buffer, STRING_WIDTH_100, L"Temp");

  Status = gRT->SetVariable(
                  Buffer,
                  &gEfiSocketCommonRcVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  VariableSize,
                  &SocketCommonRcData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "%s Set Variable Status = %r\n", Buffer, Status));
    return;
  }
  VariableSize = sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION);
  CopyMem (&SocketPowermanagementConfiguration, &(mSetupData.SocketConfig.PowerManagementConfig), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));

  StrCpyS (Buffer, STRING_WIDTH_100, SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME);
  StrCatS (Buffer, STRING_WIDTH_100, L"Temp");

  Status = gRT->SetVariable (
    Buffer,
    &gEfiSocketPowermanagementVarGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS,
    VariableSize,
    &SocketPowermanagementConfiguration
    );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_INFO, "%s Set Variable Status = %r\n", Buffer, Status));
    return;
  }

  VariableSize = sizeof(SOCKET_PROCESSORCORE_CONFIGURATION);
  CopyMem (&SocketProcessorCoreConfiguration, &(mSetupData.SocketConfig.SocketProcessorCoreConfiguration), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));

  StrCpyS (Buffer, STRING_WIDTH_100, SOCKET_PROCESSORCORE_CONFIGURATION_NAME);
  StrCatS (Buffer, STRING_WIDTH_100, L"Temp");

  Status = gRT->SetVariable (
    Buffer,
    &gEfiSocketProcessorCoreVarGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS,
    VariableSize,
    &SocketProcessorCoreConfiguration
    );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_INFO, "%s Set Variable Status = %r\n", Buffer, Status));
    return;
  }

  gBS->CloseEvent (Event);

}
#endif

/**
    This method is used to save Intel Setup options which 
    is used in PlatformReset callback to set proper reset flag.

    @param  VOID

    @retval VOID

**/

VOID
BackupIntelSetupOptions (
  VOID
  )
{

  if (gOldSystemConfiguration == NULL) {
      gOldSystemConfiguration = AllocateZeroPool (sizeof (SYSTEM_CONFIGURATION));
      CopyMem ((VOID *)gOldSystemConfiguration, (VOID *)&mSetupData.SystemConfig, sizeof (SYSTEM_CONFIGURATION));
  }
  
  if (gOldSocketMpLinkConfiguration == NULL) {
      gOldSocketMpLinkConfiguration = AllocateZeroPool (sizeof (SOCKET_MP_LINK_CONFIGURATION));
      CopyMem ((VOID *)gOldSocketMpLinkConfiguration, (VOID *)&mSetupData.SocketConfig.UpiConfig, sizeof (SOCKET_MP_LINK_CONFIGURATION));
  }
  
  if (gOldSocketCommonRcConfiguration == NULL) {
      gOldSocketCommonRcConfiguration = AllocateZeroPool (sizeof (SOCKET_COMMONRC_CONFIGURATION));
      CopyMem ((VOID *)gOldSocketCommonRcConfiguration, (VOID *)&mSetupData.SocketConfig.CommonRcConfig, sizeof (SOCKET_COMMONRC_CONFIGURATION));
  }
  
  if (gOldSocketPowermanagementConfiguration == NULL) {
      gOldSocketPowermanagementConfiguration = AllocateZeroPool (sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
      CopyMem ((VOID *)gOldSocketPowermanagementConfiguration, (VOID *)&mSetupData.SocketConfig.PowerManagementConfig, sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION));
  }

  if (gOldSocketProcessorCoreConfiguration == NULL) {
      gOldSocketProcessorCoreConfiguration = AllocateZeroPool (sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
      CopyMem ((VOID *)gOldSocketProcessorCoreConfiguration, (VOID *)&mSetupData.SocketConfig.SocketProcessorCoreConfiguration, sizeof (SOCKET_PROCESSORCORE_CONFIGURATION));
  }

#if !defined(SNR_HOST) && !defined(ICXDE_HOST) && !defined(SPR_HOST)
  if (gOldFpgaSocketConfiguration == NULL) {
      gOldFpgaSocketConfiguration = AllocateZeroPool (sizeof(FPGA_SOCKET_CONFIGURATION));
      CopyMem ((VOID *)gOldFpgaSocketConfiguration, (VOID *)&mSetupData.SocketFpgaConfig, sizeof (FPGA_SOCKET_CONFIGURATION));
  }
#endif // !defined(SNR_HOST) && !defined(ICXDE_HOST) && !defined(SPR_HOST)
  
  if (gOldSocketIioConfiguration == NULL) {
      gOldSocketIioConfiguration = AllocateZeroPool (sizeof(SOCKET_IIO_CONFIGURATION));
      CopyMem ((VOID *)gOldSocketIioConfiguration, (VOID *)&mSetupData.SocketConfig.IioConfig, sizeof (SOCKET_IIO_CONFIGURATION));
  }
}
// APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
CheckForPassword (
  VOID
  )
{
  EFI_STATUS          Status;
  CHAR16             *ErrorMsgBuf1;
  CHAR16             *ErrorMsgBuf2;
  UINT8               ReTries;
  EFI_INPUT_KEY       Key;
  UINTN               Index;

  ErrorMsgBuf1   = NULL;
  ErrorMsgBuf2   = NULL;
  Status         = EFI_SUCCESS;

  ErrorMsgBuf1 = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));
  ASSERT (ErrorMsgBuf1 != NULL);

  ErrorMsgBuf2 = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));
  ASSERT (ErrorMsgBuf2 != NULL);
  //
  // Check if we need to validate at this point
  //
  gPwFlag = CheckIfPasswordInstalled();

  if (gPwFlag != VALIDATE_NONE ) {
    //
    // User has to be validated
    //
    for (ReTries=0;ReTries <3;) {
      Status = PromptForPassword ();
      if (Status == AUTHENTICATION_PASSED){
        //
        // Passed Authentication done , You are allowed to Enter BIOS Setup
        //
        break;
    }
      else {
         do {

         CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE, &Key, L"Invalid Password", NULL, NULL);
        } while ((Key.UnicodeChar != CHAR_CARRIAGE_RETURN));

     }
      //
      // Maximum of 3 retries are allowed to enter Wrong password
      //
      ReTries++;
    }
  if (Status == AUTHENTICATION_NOT_PASSED){
      //
      // Popup a menu to notice user
      //
    do {
        //
        // Retry Limit is over, Halt the Machine
        //
      StrCpyS (ErrorMsgBuf1, MAX_STRING_LEN, L" System Halted ! ");
      StrCpyS (ErrorMsgBuf2, MAX_STRING_LEN, L" Press CTRL+ALT+DEL to reset Machine");
      CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE, &Key, ErrorMsgBuf1, ErrorMsgBuf2, NULL);
      gBS->WaitForEvent( 1, gST->ConIn->WaitForKey, &Index );
      gST->ConIn->ReadKeyStroke( gST->ConIn, &Key );
    } while (TRUE);
  }
 }

  FreePool (ErrorMsgBuf1);
  FreePool (ErrorMsgBuf2);
  //gBS->CloseEvent (Event);

}

EFI_STATUS
ReadStrings(
  IN CHAR16      *StringPtr
){
/**

    GC_TODO: add routine description

    @param StringPtr - GC_TODO: add arg description

    @retval EFI_DEVICE_ERROR - GC_TODO: add retval description
    @retval EFI_SUCCESS      - GC_TODO: add retval description
    @retval Status           - GC_TODO: add retval description

**/
  UINTN                            Index;
  UINTN                            Count;
  CHAR16                           *TempString;
  CHAR16                           KeyPad[2];
  UINTN                            Maximum;
  EFI_INPUT_KEY                    Key;
  EFI_STATUS                       Status;
  CHAR16                           *BufferedString;

  Maximum = PASSWORD_MAX_SIZE;

  BufferedString = AllocateZeroPool((Maximum + 1)* sizeof (CHAR16));
  ASSERT (BufferedString);
  TempString = AllocateZeroPool ((Maximum + 1)* sizeof (CHAR16));
  ASSERT (TempString);

  if (BufferedString == NULL || TempString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  do{

   Status = ReadKeyStrokes( &Key);
   ASSERT_EFI_ERROR (Status);

   switch (Key.UnicodeChar) {
   case CHAR_NULL:
     switch (Key.ScanCode) {
     case SCAN_LEFT:
       break;

      case SCAN_RIGHT:
        break;

      case SCAN_ESC:
        FreePool (TempString);
        FreePool(BufferedString);
        return EFI_DEVICE_ERROR;
        break;

      default:
        break;
  }

     break;

   case CHAR_CARRIAGE_RETURN:
     FreePool (TempString);
     FreePool(BufferedString);
     return EFI_SUCCESS;

       break;

   case CHAR_BACKSPACE:
     if (StringPtr[0] != CHAR_NULL) {
      for (Index = 0; StringPtr[Index] != CHAR_NULL; Index++) {
        TempString[Index] = StringPtr[Index];
      }
        //
        // Effectively truncate string by 1 character
        //
      TempString[Index - 1] = CHAR_NULL;
      StrCpyS (StringPtr, PASSWORD_MAX_SIZE +1, TempString);
     }

   default:
      //
      // If it is the beginning of the string, don't worry about checking maximum limits
      //
    if ((StringPtr[0] == CHAR_NULL)&&(Key.UnicodeChar!= CHAR_BACKSPACE)) {
      Status = StrnCpyS (StringPtr, (PASSWORD_MAX_SIZE + 1), &Key.UnicodeChar, 1);
      ASSERT_EFI_ERROR(Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
      Status = StrnCpyS (TempString, (PASSWORD_MAX_SIZE + 1), &Key.UnicodeChar, 1);
      ASSERT_EFI_ERROR(Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
     } else if (((StrLen(StringPtr)) < Maximum) &&(Key.UnicodeChar!= CHAR_BACKSPACE)) {

        KeyPad[0] = Key.UnicodeChar;
        KeyPad[1] = CHAR_NULL;
        StrCatS (StringPtr, PASSWORD_MAX_SIZE +1, KeyPad);
        StrCatS (TempString, Maximum + 1, KeyPad);
     }

    for(Count =0; Count<Maximum; Count++)
      BufferedString[Count] = L' ';
   gST->ConOut->SetCursorPosition (gST->ConOut, gColumn, gRow);

    for(Count =0; Count<StrLen(StringPtr); Count++)
      BufferedString[Count] = L'*';
      gST->ConOut->OutputString (gST->ConOut, BufferedString);
    break;
   }


   gST->ConOut->SetCursorPosition (gST->ConOut, gColumn + StrLen(StringPtr), gRow);
  } while(TRUE);

  return Status;

 }


/**

    GC_TODO: add routine description

    @param Key - GC_TODO: add arg description

    @retval Status - GC_TODO: add retval description

**/
EFI_STATUS
ReadKeyStrokes(
  OUT  EFI_INPUT_KEY            *Key
)
{
  EFI_STATUS                    Status;
  EFI_EVENT                     TimerEvent;
  EFI_EVENT                     WaitList[2];
  UINTN                         Index;

  do{
    do {
     Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);

    //
    // Set a timer event of 1 second expiration
    //
     gBS->SetTimer (
            TimerEvent,
            TimerRelative,
            ONE_SECOND
            );

    //
    // Wait for the keystroke event or the timer
    //
    WaitList[0] = gST->ConIn->WaitForKey;
    WaitList[1] = TimerEvent;
    Status      = gBS->WaitForEvent (2, WaitList, &Index);

    //
    // Check for the timer expiration
    //
      if (!EFI_ERROR (Status) && Index == 1) {
        Status = EFI_TIMEOUT;
      }

    gBS->CloseEvent (TimerEvent);
   } while (Status == EFI_TIMEOUT);

  Status = gST->ConIn->ReadKeyStroke (gST->ConIn, Key);
  }while (EFI_ERROR(Status));
  return Status;
 }

VOID
EFIAPI
DrawPopUp (
 ){
/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
  EFI_SIMPLE_TEXT_OUTPUT_MODE      SavedConsoleMode;
  UINTN                            Columns;
  UINTN                            Rows;
  UINTN                            Column;
  UINTN                            Row;
  UINTN                            NumberOfLines;
  UINTN                            MaxLength;
  UINTN                            Length;
  CHAR16                           *Line;
  CHAR16                           *Line1;
  CHAR16                           *StringBuffer;


  MaxLength     = 20;
  NumberOfLines = 3;

  //
  // If the total number of lines in the popup is zero, then ASSERT()
  //
  ASSERT (NumberOfLines != 0);

  //
  // If the maximum length of all the strings is zero, then ASSERT()
  //
  ASSERT (MaxLength != 0);

  //
  // Cache a pointer to the Simple Text Output Protocol in the EFI System Table
  //
  ConOut = gST->ConOut;

  //
  // Save the current console cursor position and attributes
  //
  CopyMem (&SavedConsoleMode, ConOut->Mode, sizeof (SavedConsoleMode));

  //
  // Retrieve the number of columns and rows in the current console mode
  //
  ConOut->QueryMode (ConOut, SavedConsoleMode.Mode, &Columns, &Rows);

  ConOut->ClearScreen (gST->ConOut);

  //
  // Disable cursor and set the foreground and background colors specified by Attribute
  //
  ConOut->EnableCursor (ConOut, TRUE);
  ConOut->SetAttribute (ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE);

  //
  // Compute the starting row and starting column for the popup
  //
  Row    = (Rows - (NumberOfLines + 3)) / 2;
  Column = (Columns - (MaxLength + 2)) / 2;


  //
  // Allocate a buffer for a single line of the popup with borders and a Null-terminator
  //
  Line = AllocateZeroPool ((MaxLength + 3) * sizeof (CHAR16));
  ASSERT (Line != NULL);

  if (Line == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  //
  // Draw top of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_DOWN_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_DOWN_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);


  // Draw middle of the popup with strings
  StringBuffer = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));

  if (StringBuffer == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  StrCpyS (StringBuffer, MAX_STRING_LEN, L"Enter Password ");
  Length = StrLen (StringBuffer);


  SetMem16 (Line, (MaxLength + 2) * 2, L' ' );
  Line[0]             = BOXDRAW_VERTICAL;
  Line[MaxLength + 1] = BOXDRAW_VERTICAL;
  Line[MaxLength + 2] = L'\0';
  CopyMem (Line + 1 + (MaxLength - Length) / 2, StringBuffer , Length * sizeof (CHAR16));
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //draw vertical lines for popup

  while ( NumberOfLines > 0) {
  SetMem16 (Line, (MaxLength + 2) * 2, L' ');
  Line[0]             = BOXDRAW_VERTICAL;
  Line[MaxLength + 1] = BOXDRAW_VERTICAL;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);
  NumberOfLines--;
  }

  //
  // Draw bottom of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_UP_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_UP_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Free the allocated line buffer
  //
  FreePool (Line);

 Line1 = AllocateZeroPool ((PASSWORD_MAX_SIZE + 3) * sizeof (CHAR16));
  ASSERT (Line1 != NULL);

  if (Line1 == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  ConOut->SetAttribute (ConOut,EFI_WHITE | EFI_BACKGROUND_BLACK);

//these are for inside popup
  Row=Row-3;
  Column=Column+5;

  gRow=Row;
  gColumn=Column;


//Draw top of inside popup box
  SetMem16 (Line1, (PASSWORD_MAX_SIZE + 2) * 2, L' ');
  Line1[0]              = L' ';
  Line1[PASSWORD_MAX_SIZE + 1] = L' ';
  Line[PASSWORD_MAX_SIZE + 2]  = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line1);

  ConOut->EnableCursor (ConOut, TRUE);

  FreePool (Line1);
  FreePool (StringBuffer);

  //
  // Restore the cursor visibility, position, and attributes
  //
  ConOut->EnableCursor      (ConOut, SavedConsoleMode.CursorVisible);
  ConOut->SetCursorPosition (ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
  ConOut->SetAttribute      (ConOut, SavedConsoleMode.Attribute);


 }
/**

    GC_TODO: add routine description

    @param None

    @retval Status - GC_TODO: add retval description

**/
EFI_STATUS
PromptForPassword(
  VOID
)
{
  EFI_STATUS                      Status;
  CHAR16                          *Password;
  UINT8                           AccessLevel;
  SYSTEM_CONFIGURATION            SystemConfiguration;
  UINT8                           EncodedPassword[SHA256_DIGEST_LENGTH];
  UINTN                           EncodedPasswordBufferSize = SHA256_DIGEST_LENGTH;

  AccessLevel  = SYSTEM_PASSWORD_ADMIN ;
  Status       = AUTHENTICATION_NOT_PASSED;

  CopyMem (&SystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));

  //
  // Draw Popup for password prompt
  //
  DrawPopUp ();
  Password = AllocateZeroPool ((PASSWORD_MAX_SIZE +1)* sizeof (CHAR16));

  if (Password == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status  = ReadStrings(Password);

  if (*Password == L' ' || Status != EFI_SUCCESS) {
    Status = AUTHENTICATION_NOT_PASSED;
  }
  // APTIOV_SERVER_OVERRIDE_RC_START : To resolve build error
  //EncodePassword(Password, EncodedPassword, &EncodedPasswordBufferSize);
  // APTIOV_SERVER_OVERRIDE_RC_END : To resolve build error

  switch (gPwFlag) {

   case VALIDATE_ADMIN:
    if (CompareMem (EncodedPassword, SystemConfiguration.AdminPassword,SHA256_DIGEST_LENGTH) == 0) {
      Status = AUTHENTICATION_PASSED;
      AccessLevel = SYSTEM_PASSWORD_ADMIN;

    } else {
      Status = AUTHENTICATION_NOT_PASSED;
    }

    break;
   case VALIDATE_USER:
    if (CompareMem (EncodedPassword, SystemConfiguration.UserPassword,SHA256_DIGEST_LENGTH) == 0) {
      Status = AUTHENTICATION_PASSED;
      AccessLevel = SYSTEM_PASSWORD_USER;
     } else {
       Status = AUTHENTICATION_NOT_PASSED;
     }

    break;
   case VALIDATE_BOTH:
    if (CompareMem (EncodedPassword, SystemConfiguration.AdminPassword,SHA256_DIGEST_LENGTH) == 0){
      Status = AUTHENTICATION_PASSED;
      AccessLevel = SYSTEM_PASSWORD_ADMIN;
     } else if (CompareMem (EncodedPassword, SystemConfiguration.UserPassword,SHA256_DIGEST_LENGTH) == 0){
       Status = AUTHENTICATION_PASSED;
       AccessLevel = SYSTEM_PASSWORD_USER;
     } else {
       Status = AUTHENTICATION_NOT_PASSED;
     }

   break;
    default:
      DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
      break;
  }
  if(Status == AUTHENTICATION_PASSED ) {
   SetPasswordType ( AccessLevel );
  }

  FreePool (Password);
  return Status;
 }


/**

    GC_TODO: add routine description

    @param None

    @retval gPwFlag - GC_TODO: add retval description

**/
UINT8
CheckIfPasswordInstalled(
  VOID
)
{
  UINT8                   State;
  SYSTEM_CONFIGURATION    SystemConfiguration;
  UINT8                   ZeroArray[SHA256_DIGEST_LENGTH];
  gPwFlag            = VALIDATE_NONE;

  CopyMem (&SystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));
  ZeroMem (&ZeroArray, SHA256_DIGEST_LENGTH);
  //
  // User trying to enter setup , Validate the password here if it is set
  //
  // Verify if Admin Password is Installed
  State = (UINT8) (CompareMem(SystemConfiguration.AdminPassword, ZeroArray, SHA256_DIGEST_LENGTH) ? PW_SET : PW_NOT_SET);
  if (State == PW_SET ){
    //
    // Admin P/W installed
    //
    gPwFlag |= VALIDATE_ADMIN;
  }
  State = 0;
  State = (UINT8) (CompareMem(SystemConfiguration.UserPassword, ZeroArray, SHA256_DIGEST_LENGTH) ? PW_SET : PW_NOT_SET);
  if (State == PW_SET ){
    //
    // Yes User P/W installed
    //
    gPwFlag |= VALIDATE_USER;
  }
  return gPwFlag;
 }

/**

    GC_TODO: add routine description

    @param Check - GC_TODO: add arg description
    @param Key   - GC_TODO: add arg description

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
BOOLEAN
IsPasswordCheck (
 IN  BOOLEAN         Check,
 IN  CHAR16          Key
)
{
  if(Check){
    if(CharIsAlphaNumeric(Key)) {
      return TRUE;
    }
      else
       return FALSE;
  }
  else
    return TRUE;
 }

/**

    GC_TODO: add routine description

    @param Type - GC_TODO: add arg description

    @retval None

**/
VOID
SetPasswordType(
 IN UINT8         Type
)
{

  EFI_STATUS              Status;

  Status = SetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, Access), &Type, sizeof(UINT8));
  ASSERT_EFI_ERROR(Status);
  return;
 }

/**
  Platform Reset function

  @param[in] PlatformResetType          Reset type aligned with EFI_RESET_TYPE

  @retval EFI_RESET_TYPE                final reset type aligned with EFI_RESET_TYPE
**/
EFI_RESET_TYPE
PlatformReset (
  IN  EFI_RESET_TYPE              PlatformResetType
  )
{
  EFI_STATUS                      Status;
  SYSTEM_CONFIGURATION            OldVarData;
  SOCKET_MP_LINK_CONFIGURATION    OldSocketMpLinkData;
  SOCKET_COMMONRC_CONFIGURATION   OldSocketCommonRcData;
  SOCKET_POWERMANAGEMENT_CONFIGURATION OldSocketPowermanagementConfiguration;
  SOCKET_PROCESSORCORE_CONFIGURATION   OldSocketProcessorCoreConfiguration;
  UINTN                           VariableSize;
  BOOLEAN                         GlobalResetRequired;
  BOOLEAN                         PowerCycleResetRequired;
  UINT8                           *MemPtr1, *MemPtr2;
  UINT32                          MemSize;
  // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
  //CHAR16                        Buffer[100];
  // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support

  GlobalResetRequired = FALSE;
  PowerCycleResetRequired = FALSE;

  DEBUG((EFI_D_INFO, "Platform reset callback entered!\n"));

  Status = GetSpecificConfigGuid (&gEfiSetupVariableGuid, &mSystemConfiguration);
  if (!EFI_ERROR(Status)) {
    CopyMem (&(mSetupData.SystemConfig), &mSystemConfiguration, sizeof(SYSTEM_CONFIGURATION));
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    VariableSize = sizeof (SYSTEM_CONFIGURATION);
    Status = gRT->GetVariable (
                  mTempVariableName,
                  &mSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  &OldVarData
                  );
    #else
    Status = EFI_SUCCESS;
    CopyMem ((VOID *)&OldVarData, (VOID *)gOldSystemConfiguration, sizeof (SYSTEM_CONFIGURATION));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    //
    // ASSERT_EFI_ERROR(Status);
    //
    if (!EFI_ERROR (Status)) {
      // APTIOV_SERVER_OVERRIDE_RC_START : #SerialDebugMsg - Restrict serialDebugMsgLvl to MRC messages only
      #if 0
      if (mSystemConfiguration.serialDebugMsgLvl != OldVarData.serialDebugMsgLvl) {
        if (mSystemConfiguration.serialDebugMsgLvl == 0) {
          IoWrite8(R_IOPORT_CMOS_STANDARD_INDEX, PcdGet8(PcdCmosDebugPrintLevelReg));
          IoWrite8(R_IOPORT_CMOS_STANDARD_DATA, 0x00);
          IoWrite8(R_IOPORT_CMOS_STANDARD_INDEX, PcdGet8(PcdCmosDebugPrintLevelReg) + 1);
          IoWrite8(R_IOPORT_CMOS_STANDARD_DATA, 0x00);
          IoWrite8(R_IOPORT_CMOS_STANDARD_INDEX, PcdGet8(PcdCmosDebugPrintLevelReg) + 2);
          IoWrite8(R_IOPORT_CMOS_STANDARD_DATA, 0x00);
          IoWrite8(R_IOPORT_CMOS_STANDARD_INDEX, PcdGet8(PcdCmosDebugPrintLevelReg) + 3);
          IoWrite8(R_IOPORT_CMOS_STANDARD_DATA, 0x00);
          DEBUG((EFI_D_ERROR, "Serial debug messages disabled and cmos cleared\n"));
        }
      }
      #endif
      // APTIOV_SERVER_OVERRIDE_RC_END : #SerialDebugMsg - Restrict serialDebugMsgLvl to MRC messages only
      //
      // If this setup variable changed, a complete power cycle is needed to flush previous page tables
      //
      if (mSystemConfiguration.Use1GPageTable != OldVarData.Use1GPageTable){
        PowerCycleResetRequired = TRUE;
      }
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    } else {
      DEBUG((EFI_D_INFO, "GetVariable %s return Status %r\n", mTempVariableName, Status));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    }
  } else {
    DEBUG((EFI_D_ERROR, "Failed Get SYSTEM_CONFIGURATION Variable return Status %r\n", Status));
  }

  VariableSize = sizeof (SOCKET_MP_LINK_CONFIGURATION);
  Status = GetSpecificConfigGuid (&gEfiSocketMpLinkVariableGuid, &mSocketMpLinkConfiguration);
  if (!EFI_ERROR(Status)) {
    CopyMem (&(mSetupData.SocketConfig.UpiConfig), &mSocketMpLinkConfiguration, sizeof(SOCKET_MP_LINK_CONFIGURATION));
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    StrCpyS(Buffer, STRING_WIDTH_100, SOCKET_MP_LINK_CONFIGURATION_NAME );
    StrCatS(Buffer, STRING_WIDTH_100, L"Temp");
    Status = gRT->GetVariable (
              Buffer,
              &gEfiSocketMpLinkVariableGuid,
              NULL,
              &VariableSize,
              &OldSocketMpLinkData
              );
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    if (!EFI_ERROR (Status)) {
      //
      // If QPI has changed any setup items, make sure that setup exit reset gets power good reset. (cold reset)
      // The tags are not included for testing QPI setup changes.
      //
      // SKX_TODO remove the tags and just use the struct
      MemPtr1 = &mSocketMpLinkConfiguration.QpiSetupNvVariableStartTag;
      MemPtr2 = &OldSocketMpLinkData.QpiSetupNvVariableStartTag;
      MemSize = (UINT32)(&mSocketMpLinkConfiguration.QpiSetupNvVariableEndTag - &mSocketMpLinkConfiguration.QpiSetupNvVariableStartTag);
      while (--MemSize) {
        if (*(++MemPtr1) != *(++MemPtr2)) {
          PowerCycleResetRequired = TRUE;
          break;
        }
      }
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    } else {
      DEBUG((EFI_D_INFO, "GetVariable %s return Status %r\n", Buffer, Status));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    }
  } else {
    DEBUG((EFI_D_ERROR, "Failed Get SOCKET_MP_LINK_CONFIGURATION Variable return Status %r\n", Status));
  }

  VariableSize = sizeof (SOCKET_COMMONRC_CONFIGURATION);
  Status = GetSpecificConfigGuid (&gEfiSocketCommonRcVariableGuid, &mSocketCommonRcConfiguration);
  if (!EFI_ERROR(Status)) {
    CopyMem (&(mSetupData.SocketConfig.CommonRcConfig), &mSocketCommonRcConfiguration, sizeof(SOCKET_COMMONRC_CONFIGURATION));
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    StrCpyS(Buffer, STRING_WIDTH_100, SOCKET_COMMONRC_CONFIGURATION_NAME );
    StrCatS(Buffer, STRING_WIDTH_100, L"Temp");
    Status = gRT->GetVariable (
              Buffer,
              &gEfiSocketCommonRcVariableGuid,
              NULL,
              &VariableSize,
              &OldSocketCommonRcData
              );
    #else
    Status = EFI_SUCCESS;
    CopyMem ((VOID *)&OldSocketCommonRcData, (VOID *)gOldSocketCommonRcConfiguration, sizeof (SOCKET_COMMONRC_CONFIGURATION));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support

    if (!EFI_ERROR (Status)) {
      //
      // Change of any option from common ref. code requires power good reset
      //
      if ((mSocketCommonRcConfiguration.MmiohBase != OldSocketCommonRcData.MmiohBase) ||
      (mSocketCommonRcConfiguration.MmiohSize != OldSocketCommonRcData.MmiohSize) ||
      (mSocketCommonRcConfiguration.IsocEn != OldSocketCommonRcData.IsocEn) ||
      (mSocketCommonRcConfiguration.NumaEn != OldSocketCommonRcData.NumaEn) ||
      // APTIOV_SERVER_OVERRIDE_RC_START : Dynamic mmcfg base address Support change
      #ifdef DYNAMIC_MMCFG_BASE_FLAG 
      (mSocketCommonRcConfiguration.UmaBasedClustering != OldSocketCommonRcData.UmaBasedClustering)) {
        PowerCycleResetRequired = TRUE;
      }
      #endif
      // APTIOV_SERVER_OVERRIDE_RC_END : Dynamic mmcfg base address Support change

      //
      // Change of RasMode setup option requires power good reset
      //
      if ((mSocketCommonRcConfiguration.MirrorMode != OldSocketCommonRcData.MirrorMode)) {
        PowerCycleResetRequired = TRUE;
      }

    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    } else {
      DEBUG((EFI_D_INFO, "GetVariable %s return Status %r\n", Buffer, Status));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    }
  } else {
    DEBUG((EFI_D_ERROR, "Failed Get SOCKET_COMMONRC_CONFIGURATION Variable return Status %r\n", Status));
  }

  VariableSize = sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION);
  Status = GetSpecificConfigGuid (&gEfiSocketPowermanagementVarGuid, &mSocketPowermanagementConfiguration);
  if (!EFI_ERROR(Status)) {
    CopyMem (&(mSetupData.SocketConfig.PowerManagementConfig), &mSocketPowermanagementConfiguration, sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    StrCpyS(Buffer, STRING_WIDTH_100, SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME );
    StrCatS(Buffer, STRING_WIDTH_100, L"Temp");
    Status = gRT->GetVariable (
              Buffer,
              &gEfiSocketPowermanagementVarGuid,
              NULL,
              &VariableSize,
              &OldSocketPowermanagementConfiguration
              );
    #else
    Status = EFI_SUCCESS;
    CopyMem ((VOID *)&OldSocketPowermanagementConfiguration, (VOID *)gOldSocketPowermanagementConfiguration, sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    if (!EFI_ERROR (Status)) {
      if ((mSocketPowermanagementConfiguration.C6Enable != OldSocketPowermanagementConfiguration.C6Enable) &&
      (mSocketPowermanagementConfiguration.C6Enable == 0)) {
        PowerCycleResetRequired = TRUE;
      }
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    } else {
      DEBUG((EFI_D_INFO, "GetVariable %s return Status %r\n", Buffer, Status));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    }
  } else {
    DEBUG((EFI_D_ERROR, "Failed Get SOCKET_POWERMANAGEMENT_CONFIGURATION Variable return Status %r\n", Status));
  }

  VariableSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = GetSpecificConfigGuid (&gEfiSocketProcessorCoreVarGuid, &mSocketProcessorCoreConfiguration);
  if (!EFI_ERROR(Status)) {
    CopyMem (&(mSetupData.SocketConfig.SocketProcessorCoreConfiguration), &mSocketProcessorCoreConfiguration, sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
    // APTIOV_SERVER_OVERRIDE_RC_START : Setup power good reset support
    #if 0
    StrCpyS(Buffer, STRING_WIDTH_100, SOCKET_PROCESSORCORE_CONFIGURATION_NAME );
    StrCatS(Buffer, STRING_WIDTH_100, L"Temp");
    Status = gRT->GetVariable (
              SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
              &gEfiSocketProcessorCoreVarGuid,
              NULL,
              &VariableSize,
              &OldSocketProcessorCoreConfiguration
              );
    #else
    Status = EFI_SUCCESS;
    CopyMem ((VOID *)&OldSocketProcessorCoreConfiguration, (VOID *)gOldSocketProcessorCoreConfiguration, sizeof (SOCKET_PROCESSORCORE_CONFIGURATION));
    #endif
    // APTIOV_SERVER_OVERRIDE_RC_END : Setup power good reset support
    if (!EFI_ERROR (Status)) {
      //
      // Set eSmm PCDs to new value if changed
      //
      if (mSocketProcessorCoreConfiguration.eSmmSaveState != OldSocketProcessorCoreConfiguration.eSmmSaveState) {
        Status = PcdSetBoolS(PcdCpuSmmMsrSaveStateEnable, mSocketProcessorCoreConfiguration.eSmmSaveState ? TRUE : FALSE);
        ASSERT_EFI_ERROR(Status);
      }

      if (mSocketProcessorCoreConfiguration.SmmBlockedDelayed != OldSocketProcessorCoreConfiguration.SmmBlockedDelayed) {
        Status = PcdSetBoolS(PcdCpuSmmUseDelayIndication, mSocketProcessorCoreConfiguration.SmmBlockedDelayed ? TRUE : FALSE);
        ASSERT_EFI_ERROR(Status);

        Status = PcdSetBoolS(PcdCpuSmmUseBlockIndication, mSocketProcessorCoreConfiguration.SmmBlockedDelayed ? TRUE : FALSE);
        ASSERT_EFI_ERROR(Status);
      }

      //
      // SMX/VMX setup option changes require power good reset
      //
      if ((mSocketProcessorCoreConfiguration.ProcessorVmxEnable != OldSocketProcessorCoreConfiguration.ProcessorVmxEnable) ||
        (mSocketProcessorCoreConfiguration.ProcessorSmxEnable != OldSocketProcessorCoreConfiguration.ProcessorSmxEnable) ||
        (mSocketProcessorCoreConfiguration.ProcessorMsrLockControl != OldSocketProcessorCoreConfiguration.ProcessorMsrLockControl) ||
        (mSocketProcessorCoreConfiguration.ProcessorMsrPkgCstConfigControlLock != OldSocketProcessorCoreConfiguration.ProcessorMsrPkgCstConfigControlLock)) {
        PowerCycleResetRequired = TRUE;
      }

      Status = GetSpecificConfigGuid (&gEfiSocketIioVariableGuid, &mSocketIioConfiguration);
      ASSERT_EFI_ERROR(Status);

      //
      // Make sure enable all 4 of them if ProcessorTxtEnable is "enabled"
      //
      if (mSocketProcessorCoreConfiguration.ProcessorLtsxEnable) {
        DEBUG((EFI_D_INFO, "Setting up Smx, Vmx, LockChipset, DebugInterface, and VTdSupport\n"));
        mSocketProcessorCoreConfiguration.ProcessorSmxEnable = 1;
        mSocketProcessorCoreConfiguration.ProcessorVmxEnable = 1;
        mSocketProcessorCoreConfiguration.LockChipset = 1;
        DEBUG((EFI_D_INFO, "mSocketProcessorCoreConfiguration.AcmType %r\n", mSocketProcessorCoreConfiguration.AcmType));
        if (mSocketProcessorCoreConfiguration.AcmType == 0) {     // If it is a PW Acm, we need to disable DebugInterface
          DEBUG((EFI_D_INFO, "disable DebugInterfacet\n"));
          mSocketProcessorCoreConfiguration.DebugInterface = 0;
        }
        mSocketIioConfiguration.VTdSupport = 1;
      }

      Status = SetSpecificConfigGuid (&gEfiSocketProcessorCoreVarGuid, &mSocketProcessorCoreConfiguration);
      ASSERT_EFI_ERROR(Status);

      Status = SetSpecificConfigGuid (&gEfiSocketIioVariableGuid, &mSocketIioConfiguration);
      ASSERT_EFI_ERROR(Status);
    //APTIOV_SERVER_OVERRIDE_RC_START: Setup power good reset support	  
    #if 0
    } else {
      DEBUG((EFI_D_INFO, "GetVariable %s return Status %r\n", Buffer, Status));
    #endif
    //APTIOV_SERVER_OVERRIDE_RC_END: Setup power good reset support
    }
  } else {
    DEBUG((EFI_D_ERROR, "Failed Get SOCKET_PROCESSORCORE_CONFIGURATION Variable return Status %r\n", Status));
  }

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  if (gMeResetRequests) {
    GlobalResetRequired = TRUE;
  }
#if defined(ME_WS_SUPPORT) && ME_WS_SUPPORT
  if (gMeClientReset) {
    GlobalResetRequired = TRUE;
  }
#endif // ME_WS_SUPPORT
#if defined(WS_ICC_SUPPORT) && WS_ICC_SUPPORT
  if (gIccReset) {
    PowerCycleResetRequired = TRUE;
  }
#endif // WS_ICC_SUPPORT
#endif // ME_SUPPORT_FLAG

  //
  // Update Use1GPageTable PCD with setup value on exit
  //
  Status = PcdSetBoolS (PcdUse1GPageTable, mSystemConfiguration.Use1GPageTable ?  TRUE : FALSE);
  ASSERT_EFI_ERROR(Status);

  //
  // Doesn't need to do platform special reset
  //
  if (GlobalResetRequired) {
    return EfiResetPlatformSpecific;
  } else if (PowerCycleResetRequired) {
    return EfiResetCold;
  }

  return PlatformResetType;
}

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
UpdatePcds(
  VOID
)
{
  SYSTEM_CONFIGURATION                 SystemConfiguration;
  SOCKET_PROCESSORCORE_CONFIGURATION   SocketProcessorCoreConfiguration;
  EFI_STATUS                           Status;

  CopyMem (&SystemConfiguration, &(mSetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));

  //
  // Set PCDs for RTC driver
  //
  if (SystemConfiguration.WakeOnLanS5){
    Status = PcdSetBoolS (PcdWakeOnRTCS5, TRUE);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return;
    }
    Status = PcdSet8S (PcdRtcWakeupTimeHour,   SystemConfiguration.RTCWakeupTimeHour);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return;
    }
    Status = PcdSet8S (PcdRtcWakeupTimeMinute, SystemConfiguration.RTCWakeupTimeMinute);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return;
    }
    Status = PcdSet8S (PcdRtcWakeupTimeSecond, SystemConfiguration.RTCWakeupTimeSecond);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return;
    }
  }

  Status = PcdSetBoolS (PcdUse1GPageTable, SystemConfiguration.Use1GPageTable ?  TRUE : FALSE);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return;
  }

  CopyMem (&SocketProcessorCoreConfiguration, &(mSetupData.SocketConfig.SocketProcessorCoreConfiguration), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));

  Status = PcdSetBoolS (PcdCpuSmmUseDelayIndication, SocketProcessorCoreConfiguration.SmmBlockedDelayed ? TRUE : FALSE);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return;
  }
  Status = PcdSetBoolS (PcdCpuSmmUseBlockIndication, SocketProcessorCoreConfiguration.SmmBlockedDelayed ? TRUE : FALSE);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return;
  }

  return;
 }

/**

  Routine will suppress power management policy setup option based upon far memory controller type is EKV or BWV.

  @param [in]  VOID

  @retval None

**/
VOID
SuppressSetupOptionsBasedOnFmcType (
  VOID
  )
{
  EFI_STATUS                      Status;
  SOCKET_MEMORY_CONFIGURATION     Buffer;
  SYSTEM_MEMORY_MAP_HOB           *SystemMemoryMap;
  UINT8                           EkvOnSystem = 0;
  UINT8                           BwvOnSystem = 0;

  Status = GetSpecificConfigGuid (&gEfiSocketMemoryVariableGuid, &Buffer);

  //
  // Search for the Memory Configuration GUID HOB
  //
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    ASSERT_EFI_ERROR (Status);
  }
#ifdef DDRT_SUPPORT
  if (SystemMemoryMap != NULL) {
    if (SystemMemoryMap->DcpmmPresent != 0) { // check if DCPMM (Data Center Persistent Memory Module) is present on the system
      if (SystemMemoryMap->EkvPresent && SystemMemoryMap->BwvPresent) {
        DEBUG ((EFI_D_INFO,"EKV and BWV present on System, do not suppress either of power management options\n"));
      } else if (SystemMemoryMap->EkvPresent && !(SystemMemoryMap->BwvPresent)) {
        EkvOnSystem = 1;
        DEBUG ((EFI_D_INFO,"EKV on system suppress power management options for BWV\n"));
      } else if (!(SystemMemoryMap->EkvPresent) && SystemMemoryMap->BwvPresent) {
        BwvOnSystem = 1;
        DEBUG ((EFI_D_INFO,"BWV on system suppress power management options for EKV\n"));
      }
    }
  }
#endif // DDRT_SUPPORT

  Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF (SOCKET_MEMORY_CONFIGURATION, EkvOnSystem), &EkvOnSystem, sizeof (UINT8));

  if (Status == EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO,"\nEkvOnSystem flag is updated successfully\n"));
  } else {
    DEBUG ((EFI_D_INFO,"\nFailed to update EkvOnSystem flag!\n"));
  }

  Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF (SOCKET_MEMORY_CONFIGURATION, BwvOnSystem), &BwvOnSystem, sizeof (UINT8));

  if (Status == EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO,"\nBwvOnSystem flag is updated successfully\n"));
  } else {
    DEBUG ((EFI_D_INFO,"\nFailed to update BwvOnSystem flag!\n"));
  }

}

/*
  Grayout if the Secure Erase option for Intel DCPMM modules if
  the DCPMM is opted-in or Master passphrase enabled.
  Pharse the system memory hob and check check for SetSecureEraseSktChHob variable
  to see if Master passphrase is enabled and then grayout
*/
VOID
GrayoutSecureEraseIntelDcpmm (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT8                           SetSecureEraseSktChHob[MAX_DCPMMS]; //[MAX_SOCKET * MAX_CH * MAX_DCPMMS]
  UINT8                           Socket = 0;
  UINT8                           Dimm = 0;
  UINT8                           Ch = 0;
  SYSTEM_MEMORY_MAP_HOB           *SystemMemoryMap;
  SOCKET_MEMORY_CONFIGURATION     Buffer;

  DEBUG ((EFI_D_INFO, "\n Grayout SecureErase Intel PMem Module Entry \n"));

  Status = GetSpecificConfigGuid (&gEfiSocketMemoryVariableGuid, &Buffer);

  //
  // Search for the Memory Configuration GUID HOB
  //
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    ASSERT_EFI_ERROR (Status);
  } else {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      for (Ch = 0; Ch < MAX_CH; Ch++) {
        DEBUG ((EFI_D_INFO, "\n SystemMemoryMap->SetSecureEraseSktChHob[Socket][ch] = %d \n", SystemMemoryMap->SetSecureEraseSktChHob[Socket][Ch]));

        if (SystemMemoryMap->SetSecureEraseSktChHob[Socket][Ch] == 1) {
          SetSecureEraseSktChHob[Dimm] = 1;
          Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF (SOCKET_MEMORY_CONFIGURATION, SetSecureEraseSktChHob[Dimm]), &(SetSecureEraseSktChHob[Dimm]), sizeof (UINT8));
          if (Status == EFI_SUCCESS) {
            DEBUG ((EFI_D_INFO, "Successfully grayed out\n"));
          } else {
            DEBUG ((EFI_D_INFO, "Failed to gray out!\n"));
          }
        }
        Dimm++;
      }
    }
  }
  DEBUG ((EFI_D_INFO, "\n Grayout Secure Erase Intel PMem Module Exit \n"));
}
