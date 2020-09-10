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

#include "SocketSetup.h"
#include "MemorySetup.h"

#include <BackCompatible.h>
#include <UncoreCommonIncludes.h>
#include <MemDecodeCommonIncludes.h>
#include <Memory/JedecDefinitions.h>

#include <Guid/MemBootHealthGuid.h>
#include <Library/RngLib.h>
#include <Library/KtiApi.h>
#include <Library/SetupLib.h>
#include <Library/IioApiLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuEarlyDataLib.h>
#include <Library/X2ApicIdDetectLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SecurityPolicyDefinitions.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/PchRtcLib.h>
#include <Library/PmemMiscLib.h>

#include <Library/PcuIpLib.h>
typedef UINT16 STRING_REF;
#define MEM_TOPOLOGY_STRING_SIZE 0x250
#define PROC_VER_ARRAY_SIZE  24
#define NEWSTRING_SIZE 0x200

/*
  Function to map channels specific to SOC
  Cases when a channel is fused, we need a proper mapping
  so that memory topology shows information properly, so that
  customers are not confused.

  @param[in] Socket - 0 based socket number
  @param[in] Ch     - 0 based channel number (0 to maximum channel per socket)

  @retval New Mapped Channel value

*/
UINT8
GetSocSpecificChannel (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/*
  Function to map Sockets specific to SOC
  Cases when a dual die processor is populated, we need a proper mapping
  so that memory topology shows information properly, so that
  customers are not confused. Currently used to display memory topology
  in setup

  @param[in] Socket - 0 based socket number

  @retval New Mapped Socket value

*/
UINT8
GetSocketForSetupDisplay (
  IN UINT8 Socket
  );

/*
  In dual die sockets, Map the channel continuity so that
  Memory Topology is shown properly

  @param[in] Socket - 0 based socket number
  @param[in] Ch     - 0 based channel number (0 to maximum channel per socket)

  @retval New Channel number for continous display

*/
UINT8
GetChannelForSetupDisplay (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

SOCKET_POWERMANAGEMENT_CONFIGURATION      mSocketPowermanagementConfiguration;
SOCKET_PROCESSORCORE_CONFIGURATION        mSocketProcessorCoreConfiguration;

//
// static Socket Structures
//
SOCKET_IIO_CONFIGURATION                  mIioConfig;
// APTIOV_SERVER_OVERRIDE_RC_START : Isolate the AMI PCI Hotplug resource parameters out of SOCKET_IIO_CONFIGURATION (EIP455745)
#ifdef HOTPLUG_FLAG
SOCKET_IIO_HP_RESOURCE         mIioHpResConfig;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Isolate the AMI PCI Hotplug resource parameters out of SOCKET_IIO_CONFIGURATION (EIP455745)
SOCKET_COMMONRC_CONFIGURATION             mCommonRcConfig;
SOCKET_MP_LINK_CONFIGURATION              mUpiConfig;
SOCKET_MEMORY_CONFIGURATION               mMemoryConfig;
SOCKET_POWERMANAGEMENT_CONFIGURATION      mSocketPMConfiguration;
MEM_BOOT_HEALTH_CONFIG                    MemBootHealthConfig;

UINTN                                     mSocketPMUpdate = 0;

CPU_SETUP_INFO                            mCpuSetupInfo[MAX_SOCKET];
EFI_IIO_UDS_PROTOCOL                      *mIioUds;
EFI_CPU_PPM_PROTOCOL                      *mCpuPpmProtocol;
CPU_VAR_DATA                              *mCpuVarDataPtr;
//
// SGX variable
//
UINT8                                     mSgxEpochUpdate       = SECURITY_POLICY_SGX_USER_MANUAL_EPOCH;

#define SECURITY_POLICY_DISABLE                        0

#define SOCKET_SETUP_SIGNATURE SIGNATURE_32 ('S', 'V', 'O', 'S')

extern EFI_GUID gSocketPkgListGuid;
EFI_STRING_ID MemTypeStrIdTab [] = {
  (STRING_REF) STR_MEMORY_TRAINING_DISPLAY_FORM_HELP,
  (STRING_REF) STR_ENFORCE_POR_HELP,
};
#define MEMORY_TYPE_DDR_STR L"DDR4"

MEMORY_MODULE_MANUFACTURER_LIST MemoryModuleManufacturerList[] = {MEMORY_MODULE_MANUFACTURERS};

typedef struct _INTEL_SOCKET_SETUP_INSTANCE {
  UINTN                            Signature;
  EFI_HANDLE                       ImageHandle;
  EFI_HANDLE                       DeviceHandle;
  EFI_HII_CONFIG_ACCESS_PROTOCOL   ConfigAccess;
  VOID                            *StartOpCodeHandle;
  VOID                            *EndOpCodeHandle;

  EFI_HII_HANDLE                   HiiHandle;

} INTEL_SOCKET_SETUP_INSTANCE;

INTEL_SOCKET_SETUP_INSTANCE      mPrivateData;
EFI_HII_CONFIG_ROUTING_PROTOCOL  *mHiiConfigRouting = NULL;
EFI_HII_HANDLE                   mSocketStringsHiiHandle = NULL;

typedef struct {
  EFI_GUID     *Guid;
  UINTN        BufferSize;
  CHAR16       *VariableName;
} SOCKET_VARIABLE_INFO;

VOID    *SocketPtrs[] = {
    &(mIioConfig           ),
// APTIOV_SERVER_OVERRIDE_RC_START : Isolate the AMI PCI Hotplug resource parameters out of SOCKET_IIO_CONFIGURATION (EIP455745)
#ifdef HOTPLUG_FLAG
    &(mIioHpResConfig      ),
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Isolate the AMI PCI Hotplug resource parameters out of SOCKET_IIO_CONFIGURATION (EIP455745)
    &(mCommonRcConfig      ),
    &(mUpiConfig           ),
    &(mMemoryConfig        ),
    &(mSocketPowermanagementConfiguration),
    &(mSocketProcessorCoreConfiguration),
    &(MemBootHealthConfig)
};

SOCKET_VARIABLE_INFO SocketVariableInfo [] = {
  {&gEfiSocketIioVariableGuid,        sizeof (SOCKET_IIO_CONFIGURATION),              (CHAR16 *)(SOCKET_IIO_CONFIGURATION_NAME     )},
// APTIOV_SERVER_OVERRIDE_RC_START : Isolate the AMI PCI Hotplug resource parameters out of SOCKET_IIO_CONFIGURATION (EIP455745)
#ifdef HOTPLUG_FLAG
  {&gEfiSocketIioVariableGuid     ,sizeof (SOCKET_IIO_HP_RESOURCE)       , (CHAR16 *)(SOCKET_IIO_HP_RESOURCE_NAME       )},
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Isolate the AMI PCI Hotplug resource parameters out of SOCKET_IIO_CONFIGURATION (EIP455745)
  {&gEfiSocketCommonRcVariableGuid,   sizeof (SOCKET_COMMONRC_CONFIGURATION),         (CHAR16 *)(SOCKET_COMMONRC_CONFIGURATION_NAME)},
  {&gEfiSocketMpLinkVariableGuid,     sizeof (SOCKET_MP_LINK_CONFIGURATION),          (CHAR16 *)(SOCKET_MP_LINK_CONFIGURATION_NAME )},
  {&gEfiSocketMemoryVariableGuid,     sizeof (SOCKET_MEMORY_CONFIGURATION),           (CHAR16 *)(SOCKET_MEMORY_CONFIGURATION_NAME  )},
  {&gEfiSocketPowermanagementVarGuid, sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION),  (CHAR16 *)(SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME)},
  {&gEfiSocketProcessorCoreVarGuid,   sizeof (SOCKET_PROCESSORCORE_CONFIGURATION),    (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME)},
  {&gMemBootHealthGuid,               sizeof (MEM_BOOT_HEALTH_CONFIG),                (CHAR16 *)PcdGetPtr (PcdMemBootHealthConfigString)}
};
#define MAX_SOCKET_VAR_INFO ARRAY_SIZE (SocketVariableInfo)

typedef struct {
  VENDOR_DEVICE_PATH        Node;
  EFI_DEVICE_PATH_PROTOCOL  End;
} HII_VENDOR_DEVICE_PATH;

HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    SOCKET_FORMSET_GUID
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

// APTIOV_SERVER_OVERRIDE_RC_START
//extern UINT8  SocketSetupFormsBin[];
// APTIOV_SERVER_OVERRIDE_RC_END

typedef struct {
  EFI_MP_SERVICES_PROTOCOL  *MpService;
  EFI_PROCESSOR_INFORMATION *ProcessorInfo;
} GET_CPU_INFO_CONTEXT;


/**
  Construct Request String (L"&OFFSET=%x&WIDTH=%x") base on the input Offset and Width.
  If the input RequestString is not NULL, new request will be cat at the end of it. The full
  request string will be constructed and return. Caller is responsible to free it.

  @param RequestString   Current request string.
  @param Offset          Offset of data in Storage.
  @param Width           Width of data.

  @return String         Request string with input Offset and Width.
**/
EFI_STRING
EFIAPI
HiiConstructRequestString (
  IN EFI_STRING      RequestString, OPTIONAL
  IN UINTN           Offset,
  IN UINTN           Width
  )
{
  CHAR16             RequestElement[30];
  UINTN              StringLength = 0;
  EFI_STRING         NewString;

  StringLength = UnicodeSPrint (
                   RequestElement,
                   sizeof (RequestElement),
                   L"&OFFSET=%x&WIDTH=%x",
                   Offset,
                   Width
                   );

  if (RequestString != NULL) {
    StringLength = StringLength + StrLen (RequestString);
  }

  NewString = AllocateZeroPool ((StringLength + 1) * sizeof (CHAR16));

  if (NewString == NULL) {
    if (RequestString != NULL) {
      FreePool (RequestString);
    }
    return NULL;
  }

  if (RequestString != NULL) {
    StrCatS (NewString, StringLength + 1, RequestString);
    FreePool (RequestString);
  }

  StrCatS (NewString, StringLength + 1, RequestElement);
  return NewString;
}

/**
  Get value of option EpochUpdate and save it in module global variable: mSgxEpochUpdate

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @retval[out] EFI_SUCCESS
               EFI_NOT_FOUND
**/
EFI_STATUS
EFIAPI
GetOwnerEpochUpdate (
  IN EFI_QUESTION_ID          QuestionId,
  IN EFI_IFR_TYPE_VALUE       *Value
  )
{
  BOOLEAN                             HiiStatus;
  SOCKET_PROCESSORCORE_CONFIGURATION  *CurrentUserSetupData;
  UINTN                               VarSize  = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);

  CurrentUserSetupData = AllocatePool (VarSize);
  if (CurrentUserSetupData == NULL) {
    return EFI_NOT_FOUND;
  }

  HiiStatus = HiiGetBrowserData (&gEfiSocketProcessorCoreVarGuid, (CHAR16 *)
                              (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                              VarSize,
                              (UINT8 *) CurrentUserSetupData);
  if (HiiStatus != TRUE) {
    FreePool (CurrentUserSetupData);
    return EFI_NOT_FOUND;
  }

  mSgxEpochUpdate = CurrentUserSetupData->EpochUpdate;
  FreePool (CurrentUserSetupData);

  return EFI_SUCCESS;
}

/**
  Change visibility of fields SgxEpoch in ProcessorSetup form after close this form
  also update field EpochUpdate to SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @retval[out] EFI_SUCCESS
               EFI_NOT_FOUND
               EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
HandleSgxOwnerEpochValueVisibility (
  IN EFI_QUESTION_ID          QuestionId,
  IN EFI_IFR_TYPE_VALUE       *Value
  )
{
  SOCKET_PROCESSORCORE_CONFIGURATION *CurrentUserSetupData;
  UINTN                     VarSize = 0;
  BOOLEAN                   HiiStatus;
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_STRING                RequestString = NULL;

  VarSize              = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  CurrentUserSetupData = AllocatePool (VarSize);
  if (CurrentUserSetupData == NULL) {
    return EFI_NOT_FOUND;
  }

  HiiStatus = HiiGetBrowserData (
    &gEfiSocketProcessorCoreVarGuid,
    (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
    VarSize,
    (UINT8 *) CurrentUserSetupData
    );
  if (HiiStatus != TRUE) {
    Status = EFI_NOT_FOUND;
    goto FreeMemory;
  }

  /**
    If callback was called with value that is not supported by setup options
    This may happen if EPOCH was changed -> setup change was not saved by F10
    and user selected Continue and 'N', so the changes would not take effect
    In this case the callback will be called again, but EpochUpdate value would be
    invalid
  **/
  if (CurrentUserSetupData->EpochUpdate != SECURITY_POLICY_SGX_CHANGE_TO_NEW_RANDOM_EPOCH &&
      CurrentUserSetupData->EpochUpdate != SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED &&
      CurrentUserSetupData->EpochUpdate != SECURITY_POLICY_SGX_USER_MANUAL_EPOCH) {
    Status = EFI_UNSUPPORTED;
    goto FreeMemory;
  }

  if (CurrentUserSetupData->ShowEpoch == SECURITY_POLICY_HIDE &&
      CurrentUserSetupData->EpochUpdate != SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED) {
    Status = EFI_UNSUPPORTED;
    goto FreeMemory;
  }

  CurrentUserSetupData->ShowEpoch = SECURITY_POLICY_HIDE;
  mSgxEpochUpdate = SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED;
  CurrentUserSetupData->EpochUpdate = SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED;

  RequestString = HiiConstructRequestString (
    RequestString,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ShowEpoch),
    sizeof (UINT8)
    );

  RequestString = HiiConstructRequestString (
    RequestString,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, EpochUpdate),
    sizeof (UINT8)
    );

  HiiStatus = HiiSetBrowserData (
    &gEfiSocketProcessorCoreVarGuid,
    (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
    VarSize,
    (UINT8 *) CurrentUserSetupData,
    RequestString
    );
  if (HiiStatus != TRUE) {
    Status = EFI_NOT_FOUND;
    goto FreeMemory;
  }

FreeMemory:
  if (RequestString != NULL) {
    FreePool (RequestString);
  }
  FreePool (CurrentUserSetupData);
  return Status;
}

/**
  Handle SGX Owner Epoch option

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
**/
EFI_STATUS
EFIAPI
HandleSgxOwnerEpoch (
  IN  EFI_QUESTION_ID       QuestionId,
  IN  EFI_IFR_TYPE_VALUE    *Value
  )
{
  SOCKET_PROCESSORCORE_CONFIGURATION *CurrentUserSetupData;
  UINTN                     VarSize;
  EFI_STATUS                Status;
  BOOLEAN                   HiiStatus;
  EFI_STRING                RequestString;
  CHAR16                    *StringBuffer1;
  CHAR16                    *StringBuffer2;
  CHAR16                    *StringBuffer3;
  EFI_INPUT_KEY             Key;
  UINTN                     StringBuffSize;
  BOOLEAN                   RngSuccess;
  UINT64                    RandomEpoch;
  UINT64                    SgxEpoch0;
  UINT64                    SgxEpoch1;

  RequestString = NULL;
  RngSuccess    = FALSE;
  RandomEpoch   = 0;

  Status = GetOptionData (
    &gEfiSocketProcessorCoreVarGuid,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxEpoch0),
    &SgxEpoch0,
    sizeof (UINT64)
    );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetOptionData (
    &gEfiSocketProcessorCoreVarGuid,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxEpoch1),
    &SgxEpoch1,
    sizeof (UINT64)
    );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  VarSize               = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  CurrentUserSetupData  = AllocatePool (VarSize);
  if (CurrentUserSetupData == NULL) {
    return EFI_NOT_FOUND;
  }

  HiiStatus = HiiGetBrowserData (
    &gEfiSocketProcessorCoreVarGuid,
    (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
    VarSize,
    (UINT8 *) CurrentUserSetupData
    );
  if (HiiStatus != TRUE) {
    Status = EFI_NOT_FOUND;
    goto FreeMemory;
  }

  /**
    If callback was called with value that is not supported by setup options
    This may happen if EPOCH was changed -> setup change was not saved by F10
    and user selected Continue and 'N', so the changes would not take effect
    In this case the callback will be called again, but EpochUpdate value would be
    invalid
  **/
  if (CurrentUserSetupData->EpochUpdate != SECURITY_POLICY_SGX_CHANGE_TO_NEW_RANDOM_EPOCH &&
      CurrentUserSetupData->EpochUpdate != SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED &&
      CurrentUserSetupData->EpochUpdate != SECURITY_POLICY_SGX_USER_MANUAL_EPOCH) {
    Status = EFI_UNSUPPORTED;
    goto FreeMemory;
  }

  if (mSgxEpochUpdate == SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED) {
    ///
    /// Initialize strings for pop-up menu
    ///
    StringBuffer1 = AllocateZeroPool (200 * sizeof (CHAR16));
    if (StringBuffer1 == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeMemory;
    }
    StringBuffer2 = AllocateZeroPool (200 * sizeof (CHAR16));
    if (StringBuffer2 == NULL) {
      FreePool (StringBuffer1);
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeMemory;
    }
    StringBuffer3 = AllocateZeroPool (200 * sizeof (CHAR16));
    if (StringBuffer3 == NULL) {
      FreePool (StringBuffer1);
      FreePool (StringBuffer2);
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeMemory;
    }

    StringBuffSize = (200 * sizeof (CHAR16)) / sizeof (CHAR16);
    StrCpyS (StringBuffer1, StringBuffSize, L"Warning: EPOCH change!");
    StrCpyS (StringBuffer2, StringBuffSize, L"All persistent data protected by Intel(R) SGX will be lost");
    StrCpyS (StringBuffer3, StringBuffSize, L"Press 'Y' if you wish to continue or press any other key otherwise");

    CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, StringBuffer1, StringBuffer2, StringBuffer3, NULL);

    FreePool (StringBuffer1);
    FreePool (StringBuffer2);
    FreePool (StringBuffer3);

    ///
    /// If user selected not to change EPOCH value
    ///
    if (Key.UnicodeChar != L'Y' && Key.UnicodeChar != L'y') {
      ///
      /// If User changed the EPOCH type selection setup option but decided that he doesn't want to
      /// continue with changing the current EPOCH values
      ///
      if (CurrentUserSetupData->EpochUpdate != mSgxEpochUpdate) {
        ///
        /// Need to return the EPOCH type selection setup option to its previous state
        ///
        CurrentUserSetupData->EpochUpdate = mSgxEpochUpdate;
        VarSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);

        RequestString = HiiConstructRequestString (
          RequestString,
          OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, EpochUpdate),
          sizeof (UINT8)
          );

        HiiStatus = HiiSetBrowserData (
          &gEfiSocketProcessorCoreVarGuid,
          (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
          VarSize,
          (UINT8 *) CurrentUserSetupData,
          RequestString
          );
        if (HiiStatus != TRUE) {
          Status = EFI_NOT_FOUND;
          goto FreeMemory;
        }
      }
      Status = EFI_UNSUPPORTED;
      goto FreeMemory;
    }
  }
  ///
  /// User will manually enter Epoch Values
  ///
  if (CurrentUserSetupData->EpochUpdate == SECURITY_POLICY_SGX_USER_MANUAL_EPOCH) {
    SgxEpoch0 = CurrentUserSetupData->SgxEpoch0 = 0;
    SgxEpoch1 = CurrentUserSetupData->SgxEpoch1 = 0;
    mSgxEpochUpdate = CurrentUserSetupData->EpochUpdate;
  }

  ///
  /// It reports that EPOCH key has been changed
  /// Make the EPOCHs available for SECURITY_POLICY_SGX_CHANGE_TO_NEW_RANDOM_EPOCH and SECURITY_POLICY_SGX_USER_MANUAL_EPOCH
  ///
  if (CurrentUserSetupData->EpochUpdate == SECURITY_POLICY_SGX_CHANGE_TO_NEW_RANDOM_EPOCH) {
    ///
    /// Generate two new 64bit random numbers and replace the previous Epoch values.
    ///
    RngSuccess = GetRandomNumber64 (&RandomEpoch);
    if (RngSuccess) {
      SgxEpoch0 = CurrentUserSetupData->SgxEpoch0 = RandomEpoch;
      DEBUG ((DEBUG_INFO, "SgxEpoch0: %016llx\n", CurrentUserSetupData->SgxEpoch0));
    } else {
      DEBUG ((DEBUG_ERROR, "SgxEpoch0 - Failed to get a new 64-bit random number!\n"));
      ASSERT (!RngSuccess);
      Status = EFI_NOT_FOUND;
      goto FreeMemory;
    }

    RngSuccess = GetRandomNumber64 (&RandomEpoch);
    if (RngSuccess) {
      SgxEpoch1 = CurrentUserSetupData->SgxEpoch1 = RandomEpoch;
      DEBUG ((DEBUG_INFO, "SgxEpoch1: %016llx\n", CurrentUserSetupData->SgxEpoch1));
    } else {
      DEBUG ((DEBUG_ERROR, "SgxEpoch1 - Failed to get a new 64-bit random number!\n"));
      ASSERT (!RngSuccess);
      Status = EFI_NOT_FOUND;
      goto FreeMemory;
    }
    mSgxEpochUpdate = CurrentUserSetupData->EpochUpdate = SECURITY_POLICY_SGX_USER_MANUAL_EPOCH;
  }

  ///
  /// After generating two new Epoch values, set "Select EPOCH input type" switch to 'SGX User Manual Owner EPOCH'
  ///
  CurrentUserSetupData->ShowEpoch = SECURITY_POLICY_SHOW;
  RequestString = HiiConstructRequestString (
    RequestString,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ShowEpoch),
    sizeof (UINT8)
    );

  RequestString = HiiConstructRequestString (
    RequestString,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, EpochUpdate),
    sizeof (UINT8)
    );

  RequestString = HiiConstructRequestString (
    RequestString,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxEpoch0),
    sizeof (UINT64)
    );

  RequestString = HiiConstructRequestString (
    RequestString,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxEpoch1),
    sizeof (UINT64)
    );

  if (RequestString != NULL) {
    VarSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
    HiiStatus = HiiSetBrowserData (
      &gEfiSocketProcessorCoreVarGuid,
      (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
      VarSize,
      (UINT8 *) CurrentUserSetupData,
      RequestString
      );

    if (HiiStatus != TRUE) {
      Status =  EFI_NOT_FOUND;
    }
  }

  Status = EFI_SUCCESS;

  FreePool (RequestString);

FreeMemory:
  FreePool (CurrentUserSetupData);
  return Status;
}

/**
  Handle changing EnableSGX value

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @return EFI_STATUS         EnableSgx value change has been handled.
**/
EFI_STATUS
EFIAPI
HandleEnableSgx (
  IN  EFI_QUESTION_ID       QuestionId,
  IN  EFI_IFR_TYPE_VALUE    *Value
  )
{
  SOCKET_PROCESSORCORE_CONFIGURATION *CurrentUserSetupData;
  UINTN                     VarSize;
  EFI_STATUS                Status;
  EFI_STRING                RequestString;
  CHAR16                    *StringBuffer1;
  CHAR16                    *StringBuffer2;
  CHAR16                    *StringBuffer3;
  EFI_INPUT_KEY             Key;
  UINTN                     StringBuffSize;

  RequestString = NULL;

  VarSize               = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  CurrentUserSetupData  = AllocatePool (VarSize);
  if (CurrentUserSetupData == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = HiiGetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                              (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                              VarSize,
                              (UINT8 *) CurrentUserSetupData);
  if (Status != TRUE) {
    FreePool (CurrentUserSetupData);
    return EFI_NOT_FOUND;
  }

  ///
  /// Warning message should not be displayed when CR dimms are not present
  /// or when user is disabling SGX
  ///
  if (!CurrentUserSetupData->CrDimmsPresent || Value->u8 == SECURITY_POLICY_DISABLE) {
    FreePool (CurrentUserSetupData);
    return EFI_SUCCESS;
  }

  ///
  /// Initialize strings for pop-up menu
  ///
  StringBuffer1 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer1 == NULL) {
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }
  StringBuffer2 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer2 == NULL) {
    FreePool (StringBuffer1);
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }
  StringBuffer3 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer3 == NULL) {
    FreePool (StringBuffer1);
    FreePool (StringBuffer2);
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }

  StringBuffSize = (200 * sizeof (CHAR16)) / sizeof (CHAR16);
  StrCpyS (StringBuffer1, StringBuffSize, L"Warning: Enabling Intel(R) SGX with Intel(R) Optane(TM) DC persistent memory modules present!");
  StrCpyS (StringBuffer2, StringBuffSize, L"This will cause Intel(R) Optane(TM) DC persistent memory modules to disable!");
  StrCpyS (StringBuffer3, StringBuffSize, L"Press 'Y' if you wish to continue or press any other key otherwise");

  CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, StringBuffer1, StringBuffer2, StringBuffer3, NULL);

  FreePool (StringBuffer1);
  FreePool (StringBuffer2);
  FreePool (StringBuffer3);

  ///
  /// If user selected that he does not want CR to disable then we shall not enable SGX
  ///
  if (Key.UnicodeChar != L'Y' && Key.UnicodeChar != L'y') {
    CurrentUserSetupData->EnableSgx = SECURITY_POLICY_DISABLE;
    VarSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, EnableSgx), sizeof (UINT8));
    Status = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                                (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                                VarSize,
                                (UINT8 *) CurrentUserSetupData,
                                RequestString);
    if (Status != TRUE) {
      FreePool (CurrentUserSetupData);
      return EFI_NOT_FOUND;
    }
  }


  FreePool (CurrentUserSetupData);
  return EFI_SUCCESS;
}

/**
  Handle changing Mktme Integrity value

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @return EFI_STATUS         Mktme Integrity value change has been handled.
**/
EFI_STATUS
EFIAPI
HandleMktmeIntegrity (
  IN  EFI_QUESTION_ID       QuestionId,
  IN  EFI_IFR_TYPE_VALUE    *Value
  )
{
  SOCKET_PROCESSORCORE_CONFIGURATION *CurrentUserSetupData;
  UINTN                     VarSize;
  EFI_STATUS                Status;
  EFI_STRING                RequestString;
  CHAR16                    *StringBuffer1;
  CHAR16                    *StringBuffer2;
  CHAR16                    *StringBuffer3;
  CHAR16                    *StringBuffer4;
  EFI_INPUT_KEY             Key;
  UINTN                     StringBuffSize;

  RequestString = NULL;

  VarSize               = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  CurrentUserSetupData  = AllocatePool (VarSize);
  if (CurrentUserSetupData == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = HiiGetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                              (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                              VarSize,
                              (UINT8 *) CurrentUserSetupData);
  if (Status != TRUE) {
    FreePool (CurrentUserSetupData);
    return EFI_NOT_FOUND;
  }

  ///
  /// Warning message should not be displayed when SGX is disabled
  ///
  if (CurrentUserSetupData->EnableSgx != SECURITY_POLICY_ENABLE) {
    FreePool (CurrentUserSetupData);
    return EFI_SUCCESS;
  }

  ///
  /// Initialize strings for pop-up menu
  ///
  StringBuffer1 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer1 == NULL) {
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }
  StringBuffer2 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer2 == NULL) {
    FreePool (StringBuffer1);
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }
  StringBuffer3 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer3 == NULL) {
    FreePool (StringBuffer1);
    FreePool (StringBuffer2);
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }
  StringBuffer4 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer4 == NULL) {
    FreePool (StringBuffer1);
    FreePool (StringBuffer2);
    FreePool (StringBuffer3);
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }

  StringBuffSize = (200 * sizeof (CHAR16)) / sizeof (CHAR16);
  StrCpyS (StringBuffer1, StringBuffSize, L"Warning: Changing MKTME Integrity while SGX is enabled!");
  StrCpyS (StringBuffer2, StringBuffSize, L"This will generate new set of keys for SGX and reset registration state");
  StrCpyS (StringBuffer3, StringBuffSize, L"Previous SGX keys will be archived");
  StrCpyS (StringBuffer4, StringBuffSize, L"Press 'Y' if you wish to continue or press any other key otherwise");

  CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, StringBuffer1, StringBuffer2, StringBuffer3, StringBuffer4, NULL);

  FreePool (StringBuffer1);
  FreePool (StringBuffer2);
  FreePool (StringBuffer3);
  FreePool (StringBuffer4);

  ///
  /// If user does not want to lose SGX keys then we don't change MKTME Integrity knob
  ///
  if (Key.UnicodeChar != L'Y' && Key.UnicodeChar != L'y') {
    CurrentUserSetupData->MktmeIntegrity = (Value->u8 == SECURITY_POLICY_ENABLE) ? SECURITY_POLICY_DFX_DISABLE : SECURITY_POLICY_ENABLE;
    VarSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, MktmeIntegrity), sizeof (UINT8));
    Status = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                                (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                                VarSize,
                                (UINT8 *) CurrentUserSetupData,
                                RequestString);
    if (Status != TRUE) {
      FreePool (CurrentUserSetupData);
      return EFI_NOT_FOUND;
    }
  }


  FreePool (CurrentUserSetupData);
  return EFI_SUCCESS;
}

/**
  Handle changing SgxFactoryReset value
  This resets knobs to default values, but does not change EnableSgx value

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @return EFI_STATUS         EnableSgx value change has been handled.
**/
EFI_STATUS
EFIAPI
HandleFactoryResetSgx (
  IN  EFI_QUESTION_ID       QuestionId,
  IN  EFI_IFR_TYPE_VALUE    *Value
  )
{
  SOCKET_PROCESSORCORE_CONFIGURATION *CurrentUserSetupData = NULL;
  UINTN                     VarSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  EFI_STATUS                Status;
  EFI_STRING                RequestString  = NULL;
  CHAR16                    *StringBuffer1 = NULL;
  CHAR16                    *StringBuffer2 = NULL;
  CHAR16                    *StringBuffer3 = NULL;
  EFI_INPUT_KEY             Key;
  UINTN                     StringBuffSize;

  if (Value->u8 == SECURITY_POLICY_DISABLE) {
    return EFI_SUCCESS;
  }

  CurrentUserSetupData = AllocatePool (VarSize);
  if (CurrentUserSetupData == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = HiiGetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                              (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                              VarSize,
                              (UINT8 *) CurrentUserSetupData);
  if (Status != TRUE) {
    FreePool (CurrentUserSetupData);
    return EFI_NOT_FOUND;
  }

  ///
  /// Initialize strings for pop-up menu
  ///
  StringBuffer1 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer1 == NULL) {
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }
  StringBuffer2 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer2 == NULL) {
    FreePool (StringBuffer1);
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }
  StringBuffer3 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer3 == NULL) {
    FreePool (StringBuffer1);
    FreePool (StringBuffer2);
    FreePool (CurrentUserSetupData);
    return EFI_OUT_OF_RESOURCES;
  }

  StringBuffSize = (200 * sizeof (CHAR16)) / sizeof (CHAR16);
  StrCpyS (StringBuffer1, StringBuffSize, L"Warning: About to perform Intel(R) SGX factory reset!");
  StrCpyS (StringBuffer2, StringBuffSize, L"This will reset all SGX bios knobs to default values!");
  StrCpyS (StringBuffer3, StringBuffSize, L"Press 'Y' if you wish to continue or press any other key otherwise");

  CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, StringBuffer1, StringBuffer2, StringBuffer3, NULL);

  FreePool (StringBuffer1);
  FreePool (StringBuffer2);
  FreePool (StringBuffer3);

  if (Key.UnicodeChar != L'Y' && Key.UnicodeChar != L'y') {
    CurrentUserSetupData->SgxFactoryReset = SECURITY_POLICY_DISABLE;
    VarSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxFactoryReset), sizeof (UINT8));
    Status = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                                (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                                VarSize,
                                (UINT8 *) CurrentUserSetupData,
                                RequestString);
    if (Status != TRUE) {
      FreePool (CurrentUserSetupData);
      return EFI_NOT_FOUND;
    }
    FreePool (CurrentUserSetupData);
    return EFI_SUCCESS;
  }


  // Reset knobs to default values
  CurrentUserSetupData->SgxQoS            = SECURITY_POLICY_ENABLE;
  CurrentUserSetupData->EpochUpdate       = SECURITY_POLICY_SGX_USER_MANUAL_EPOCH;
  CurrentUserSetupData->SgxEpoch0         = 0;
  CurrentUserSetupData->SgxEpoch1         = 0;
  CurrentUserSetupData->SgxLeWr           = SECURITY_POLICY_ENABLE;
  CurrentUserSetupData->SgxLePubKeyHash0  = 0;
  CurrentUserSetupData->SgxLePubKeyHash1  = 0;
  CurrentUserSetupData->SgxLePubKeyHash2  = 0;
  CurrentUserSetupData->SgxLePubKeyHash3  = 0;
  CurrentUserSetupData->SgxDebugMode      = SECURITY_POLICY_DISABLE;
  CurrentUserSetupData->SgxAutoRegistrationAgent    = SECURITY_POLICY_ENABLE;
  CurrentUserSetupData->SgxPackageInfoInBandAccess  = SECURITY_POLICY_DISABLE;

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    CurrentUserSetupData->PrmrrSize = SECURITY_POLICY_PRMRR_SIZE_2GB;
  } else {
    CurrentUserSetupData->SprspOrLaterPrmSize = SECURITY_POLICY_PRMRR_SIZE_1GB;
  }



  VarSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxQoS), sizeof (UINT8));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxFactoryReset), sizeof (UINT8));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, EpochUpdate), sizeof (UINT8));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxEpoch0), sizeof (UINT64));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxEpoch1), sizeof (UINT64));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxLePubKeyHash0), sizeof (UINT64));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxLePubKeyHash1), sizeof (UINT64));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxLePubKeyHash2), sizeof (UINT64));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxLePubKeyHash3), sizeof (UINT64));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxDebugMode), sizeof (UINT8));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxAutoRegistrationAgent), sizeof (UINT8));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SgxPackageInfoInBandAccess), sizeof (UINT8));
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, PrmrrSize), sizeof (UINT64));
  } else {
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, SprspOrLaterPrmSize), sizeof (UINT64));
  }

  Status = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                              (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                              VarSize,
                              (UINT8 *) CurrentUserSetupData,
                              RequestString);
  if (Status != TRUE) {
    FreePool (CurrentUserSetupData);
    return EFI_NOT_FOUND;
  }

  FreePool (CurrentUserSetupData);
  return EFI_SUCCESS;

}
UINT64
CalculateBitMapFromRange (
  UINT64 MaxValue,
  UINT64 MinValue
  )
{
  return (UINT64)(MaxValue | (MaxValue - MinValue));
}

/**
  Handle changing SNC Enable value

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.

  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed
  @retval EFI_NOT_FOUND         Hii data read/write failed
  @retval EFI_SUCCESS           SNC enabling handled succesfully
**/
EFI_STATUS
EFIAPI
HandleEnableSnc (
  IN  EFI_QUESTION_ID       QuestionId,
  IN  EFI_IFR_TYPE_VALUE    *Value
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_STRING    RequestString = NULL;
  BOOLEAN       HiiStatus;
  INT8          ShiftValue;
  //
  // Popup related data
  //
  CHAR16        *StringBuffer1 = NULL;
  CHAR16        *StringBuffer2 = NULL;
  CHAR16        *StringBuffer3 = NULL;
  UINTN         StringBuffSize;
  EFI_INPUT_KEY Key;
  //
  // Setup configs
  //
  UINT8         CurrentSncConfig;
  UINT8         IncomingSncConfig;
  UINT64        CurrentPrmrrBitmap;
  UINT64        SmallestPrmrrInBitmap = 0;
  SOCKET_PROCESSORCORE_CONFIGURATION  *CurrentProcessorConfigData = NULL;
  SOCKET_MP_LINK_CONFIGURATION        *CurrentMpLinkConfigData = NULL;

  //
  // Exit if unsupported CPU
  //
  if (!IsCpuAndRevision (CPU_ICXSP, REV_ALL) &&
      !IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    goto FreeMem_HandleEnableSnc;
  }

  //
  // Retrieve setup configs
  //
  CurrentProcessorConfigData = AllocateZeroPool (sizeof (SOCKET_PROCESSORCORE_CONFIGURATION));
  if (CurrentProcessorConfigData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeMem_HandleEnableSnc;
  }

  CurrentMpLinkConfigData = AllocateZeroPool (sizeof (SOCKET_MP_LINK_CONFIGURATION));
  if (CurrentMpLinkConfigData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeMem_HandleEnableSnc;
  }

  Status = GetOptionData (
    &gEfiSocketProcessorCoreVarGuid,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ValidPrmrrBitMap),
    &CurrentPrmrrBitmap,
    sizeof (UINT64)
    );
  if (EFI_ERROR (Status)) {
    Status = EFI_NOT_FOUND;
    goto FreeMem_HandleEnableSnc;
  } else if ((CurrentPrmrrBitmap & CalculateBitMapFromRange (SECURITY_POLICY_PRMRR_SIZE_512GB, SECURITY_POLICY_PRMRR_SIZE_1GB)) == 0) {
    Status = EFI_UNSUPPORTED;
    goto FreeMem_HandleEnableSnc;
  }

  HiiStatus = HiiGetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                                 (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                                 sizeof (SOCKET_PROCESSORCORE_CONFIGURATION),
                                 (UINT8 *)CurrentProcessorConfigData);
  if (HiiStatus != TRUE) {
    Status = EFI_NOT_FOUND;
    goto FreeMem_HandleEnableSnc;
  }

  HiiStatus = HiiGetBrowserData (&gEfiSocketMpLinkVariableGuid,
                                 (CHAR16 *)(SOCKET_MP_LINK_CONFIGURATION_NAME),
                                 sizeof (SOCKET_MP_LINK_CONFIGURATION),
                                 (UINT8 *)CurrentMpLinkConfigData);
  if (HiiStatus != TRUE) {
    Status = EFI_NOT_FOUND;
    goto FreeMem_HandleEnableSnc;
  }

  //
  // Warning message should be displayed only when SGX is enabled
  //
  if (CurrentProcessorConfigData->EnableSgx == SECURITY_POLICY_ENABLE) {
    //
    // Initialize strings for pop-up menu
    //
    StringBuffer1 = AllocateZeroPool (200 * sizeof (CHAR16));
    if (StringBuffer1 == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeMem_HandleEnableSnc;
    }
    StringBuffer2 = AllocateZeroPool (200 * sizeof (CHAR16));
    if (StringBuffer2 == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeMem_HandleEnableSnc;
    }
    StringBuffer3 = AllocateZeroPool (200 * sizeof (CHAR16));
    if (StringBuffer3 == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto FreeMem_HandleEnableSnc;
    }

    StringBuffSize = (200 * sizeof (CHAR16)) / sizeof (CHAR16);
    StrCpyS (StringBuffer1, StringBuffSize, L"Warning: Changing SNC configuration with Intel(R) SGX enabled");
    StrCpyS (StringBuffer2, StringBuffSize, L"This will alter SGX PRMRR size configuration!");
    StrCpyS (StringBuffer3, StringBuffSize, L"Press 'Y' if you wish to continue or press any other key otherwise");

    CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, StringBuffer1, StringBuffer2, StringBuffer3, NULL);

    if (Key.UnicodeChar != L'Y' && Key.UnicodeChar != L'y') {
      //
      // User chose not to change SNC config
      // Change the incoming option value (Value->u8) to current option value
      //
      Value->u8 = CurrentMpLinkConfigData->SncEn;
      Status = EFI_SUCCESS;
      goto FreeMem_HandleEnableSnc;
    }
  }

  //
  // Extract current and incoming config values for SNC
  // Possible values are:
  //  0 - SNC0 (disabled)
  //  2 - SNC2
  //  4 - SNC4
  //
  CurrentSncConfig = CurrentMpLinkConfigData->SncEn;
  IncomingSncConfig = Value->u8;

  //
  // Clustering configuration (SNC/NUMA) changes affect available PRMRR sizes
  // For example when changing clustering config from SNC4 to SNC2, max PRMRR size is twice as large
  // and when changing from SNC2 to SNC4, two times smaller
  // ShiftValue represents value of binary shift to be applied to ValidPrmrrBitmap in order to display
  // valid PRMRR sizes in SocketSetup menu
  //
  ShiftValue = (CurrentSncConfig - IncomingSncConfig) / 2;

  if (ShiftValue == 0) {
    //
    // exit function when Shift Value is zero, there is no Snc change pending
    //
    goto FreeMem_HandleEnableSnc;
  } else if (ShiftValue > 0) {
    //
    // If calcucated ShiftValue is positive, shift left
    //
    CurrentPrmrrBitmap = LShiftU64 (CurrentPrmrrBitmap, (UINT8)ShiftValue);
    CurrentProcessorConfigData->PrmrrSize = LShiftU64 (CurrentProcessorConfigData->PrmrrSize, (UINT8)ShiftValue);
  } else {
    //
    // If calcucated ShiftValue is negative, shift right.
    // This step is necessary, since binary shift operator doesn't support negative values.
    //
    CurrentPrmrrBitmap = RShiftU64 (CurrentPrmrrBitmap, (-ShiftValue));
    CurrentProcessorConfigData->PrmrrSize = RShiftU64 (CurrentProcessorConfigData->PrmrrSize, (-ShiftValue));
  }
  if (IncomingSncConfig == KTI_DISABLE) {
    // PRMRR size 1 GB is not supported in NUMA (SNC0)
    CurrentPrmrrBitmap &= (~SECURITY_POLICY_PRMRR_SIZE_1GB);
  }

  CurrentProcessorConfigData->ValidPrmrrBitMap = CurrentPrmrrBitmap;
  //
  // Fallback PrmrrSize to supported size before save changes
  //
  // create bit map which have only supported PRMRR sizes in SGX (1GB in Mirroring Mode)
  CurrentPrmrrBitmap &= CalculateBitMapFromRange (SECURITY_POLICY_PRMRR_SIZE_512GB, SECURITY_POLICY_PRMRR_SIZE_1GB);

  //
  // if PrmrrSize is not supported in CurrentPrmrrBitMap override it value with:
  //  - smallest supported size in CurrentPrmrrBitMap when PrmrrSize is smaller than support sizes,
  //  - largest supported size in CurrentPrmrrBitMap when PrmrrSize is bigger than support sizes.
  //
  if ((CurrentPrmrrBitmap != 0) && ((CurrentProcessorConfigData->PrmrrSize & CurrentPrmrrBitmap) == 0)) {
    SmallestPrmrrInBitmap = LShiftU64 (1, LowBitSet64 (CurrentPrmrrBitmap));
    if (CurrentProcessorConfigData->PrmrrSize < SmallestPrmrrInBitmap) {
      CurrentProcessorConfigData->PrmrrSize = SmallestPrmrrInBitmap;
    } else {
      CurrentProcessorConfigData->PrmrrSize = LShiftU64 (1, HighBitSet64 (CurrentPrmrrBitmap));
    }
  }

  RequestString = HiiConstructRequestString (RequestString,
                              OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ValidPrmrrBitMap),
                              sizeof (UINT64));
  RequestString = HiiConstructRequestString (RequestString,
                              OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, PrmrrSize),
                              sizeof (UINT64));
  HiiStatus = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                              (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                              sizeof (SOCKET_PROCESSORCORE_CONFIGURATION),
                              (UINT8 *)CurrentProcessorConfigData,
                              RequestString);

  if (HiiStatus != TRUE) {
    Status = EFI_NOT_FOUND;
    goto FreeMem_HandleEnableSnc;
  }

  Status = SetOptionData (
    &gEfiSocketProcessorCoreVarGuid,
    OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ValidPrmrrBitMap),
    &(CurrentProcessorConfigData->ValidPrmrrBitMap),
    sizeof (UINT64));
  if (EFI_ERROR (Status)) {
    Status = EFI_NOT_FOUND;
  }

FreeMem_HandleEnableSnc:
  if (CurrentProcessorConfigData != NULL) {
    FreePool (CurrentProcessorConfigData);
  }
  if (CurrentMpLinkConfigData != NULL) {
    FreePool (CurrentMpLinkConfigData);
  }
  if (StringBuffer1 != NULL) {
    FreePool (StringBuffer1);
  }
  if (StringBuffer2 != NULL) {
    FreePool (StringBuffer2);
  }
  if (StringBuffer3 != NULL) {
    FreePool (StringBuffer3);
  }
  return Status;
}

/**
  Handle changing UMA based clustering value

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.

  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed
  @retval EFI_NOT_FOUND         Hii data read/write failed
  @retval EFI_SUCCESS           UMA based clustering enabling handled succesfully
**/
EFI_STATUS
EFIAPI
HandleUmaBasedClustering (
  IN  EFI_QUESTION_ID       QuestionId,
  IN  EFI_IFR_TYPE_VALUE    *Value
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  BOOLEAN       HiiStatus;
  //
  // Popup related data
  //
  CHAR16        *StringBuffer1 = NULL;
  CHAR16        *StringBuffer2 = NULL;
  CHAR16        *StringBuffer3 = NULL;
  UINTN         StringBuffSize;
  EFI_INPUT_KEY Key;

  SOCKET_PROCESSORCORE_CONFIGURATION  *CurrentProcessorConfigData = NULL;

  if (Value == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Retrieve setup configs
  //
  CurrentProcessorConfigData = AllocateZeroPool (sizeof (*CurrentProcessorConfigData));
  if (CurrentProcessorConfigData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeMem_HandleUma;
  }
  HiiStatus = HiiGetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                                 (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                                 sizeof (*CurrentProcessorConfigData),
                                 (UINT8 *)CurrentProcessorConfigData);
  if (HiiStatus != TRUE) {
    Status = EFI_NOT_FOUND;
    goto FreeMem_HandleUma;
  }

  //
  // Exit if platform supports SGX + UmaBasedClustering
  //
  if (!IsCpuAndRevision (CPU_ICXSP, REV_ALL) &&
      !IsCpuAndRevision (CPU_ICXD, REV_ALL) &&
      !IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
    goto FreeMem_HandleUma;
  }

  //
  // Exit if UMA Based Clustering or SGX disabled
  //
  if ((Value->u8 == UMABASEDCLUSTERING_DISABLED) ||
      (CurrentProcessorConfigData->EnableSgx == SECURITY_POLICY_DISABLE))
  {
    goto FreeMem_HandleUma;
  }

  //
  // Initialize strings for pop-up menu
  //
  StringBuffer1 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer1 == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeMem_HandleUma;
  }
  StringBuffer2 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer2 == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeMem_HandleUma;
  }
  StringBuffer3 = AllocateZeroPool (200 * sizeof (CHAR16));
  if (StringBuffer3 == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeMem_HandleUma;
  }

  StringBuffSize = 200;
  StrCpyS (StringBuffer1, StringBuffSize, L"Warning: Changing UMA based clustering configuration with Intel(R) SGX enabled");
  StrCpyS (StringBuffer2, StringBuffSize, L"This will disable SGX!");
  StrCpyS (StringBuffer3, StringBuffSize, L"Press 'Y' if you wish to continue or press any other key otherwise");

  CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, StringBuffer1, StringBuffer2, StringBuffer3, NULL);

  if (Key.UnicodeChar == L'Y' || Key.UnicodeChar == L'y') {
    CurrentProcessorConfigData->EnableSgx = SECURITY_POLICY_DISABLE;
    HiiStatus = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid,
                                   (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                                   sizeof (*CurrentProcessorConfigData),
                                   (UINT8 *)CurrentProcessorConfigData,
                                   NULL);
    if (HiiStatus != TRUE) {
      Status = EFI_NOT_FOUND;
    }
  }

FreeMem_HandleUma:
  if (CurrentProcessorConfigData != NULL) {
    FreePool (CurrentProcessorConfigData);
  }
  if (StringBuffer1 != NULL) {
    FreePool (StringBuffer1);
  }
  if (StringBuffer2 != NULL) {
    FreePool (StringBuffer2);
  }
  if (StringBuffer3 != NULL) {
    FreePool (StringBuffer3);
  }
  return Status;
}

/**
  Set VTd to enable if x2APIC is enabled.

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @retval[out] EFI_SUCCESS
               EFI_NOT_FOUND
**/
EFI_STATUS
EFIAPI
UpdateVtdOnx2ApicEnabled (
  IN EFI_QUESTION_ID          QuestionId,
  IN EFI_IFR_TYPE_VALUE       *Value
  )
{
  BOOLEAN                             HiiStatus;
  SOCKET_IIO_CONFIGURATION            *IioConfig;
  UINTN                               VarSize  = sizeof (SOCKET_IIO_CONFIGURATION);

  if (Value->u8 != 1) {
    return EFI_NOT_FOUND;
  }

  IioConfig = AllocatePool (VarSize);
  if (IioConfig == NULL) {
    return EFI_NOT_FOUND;
  }

  HiiStatus = HiiGetBrowserData (
                    &gEfiSocketIioVariableGuid,
                    (CHAR16 *)(SOCKET_IIO_CONFIGURATION_NAME),
                    VarSize,
                    (UINT8 *) IioConfig);
  if (HiiStatus != TRUE) {
    FreePool (IioConfig);
    return EFI_NOT_FOUND;
  }

  IioConfig->VTdSupport = 1;
  HiiStatus = HiiSetBrowserData(
                    &gEfiSocketIioVariableGuid,
                    SOCKET_IIO_CONFIGURATION_NAME,
                    VarSize,
                    (UINT8*)IioConfig,
                    NULL
                    );
  FreePool (IioConfig);

  if (HiiStatus != TRUE) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
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
  EFI_STRING                       ConfigRequestHdr;
  EFI_STRING                       ConfigRequest;
  UINTN                            Size;
  BOOLEAN                          AllocatedRequest;
  UINTN                            Index=0;


  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Request;

  if (Request != NULL) {
    for (Index = 0; Index < MAX_SOCKET_VAR_INFO; Index ++) {
      if (HiiIsConfigHdrMatch (Request, SocketVariableInfo[Index].Guid, SocketVariableInfo[Index].VariableName)){
        break;
      }
    }

    if (Index == MAX_SOCKET_VAR_INFO) {
      return EFI_NOT_FOUND;
    }
  }

  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  Size             = 0;
  AllocatedRequest = FALSE;

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = SocketVariableInfo[Index].BufferSize;

  Status = gRT->GetVariable (
                  SocketVariableInfo[Index].VariableName,
                  SocketVariableInfo[Index].Guid,
                  NULL,
                  &BufferSize,
                  SocketPtrs[Index]
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ConfigRequest = Request;
  if (Request == NULL || (StrStr (Request, L"OFFSET") == NULL)) {

  //
    // Request is set to NULL, construct full request string.
    //

    //
    // First Set ConfigRequestHdr string.
    //
    ConfigRequestHdr = HiiConstructConfigHdr (SocketVariableInfo[Index].Guid, SocketVariableInfo[Index].VariableName,  mPrivateData.DeviceHandle);
    if (ConfigRequestHdr == NULL) {
      DEBUG ((EFI_D_ERROR, "\n ERROR!!! HiiConstructConfigHdr () - Returned NULL Pointer for ConfigRequestHdr\n"));
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return EFI_INVALID_PARAMETER;
    }

    //
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      DEBUG ((EFI_D_ERROR, "\n ERROR!!! AllocateZeroPool () - Returned NULL Pointer for ConfigRequest\n"));
      ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
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
                                (UINT8 *) SocketPtrs[Index],
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
//

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
  EFI_STATUS          Status;
  UINTN               BufferSize;
  UINTN               Index;
  UINT32              Attributes;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;
  for (Index = 0; Index < MAX_SOCKET_VAR_INFO; Index ++) {
    if (HiiIsConfigHdrMatch (Configuration, SocketVariableInfo[Index].Guid, SocketVariableInfo[Index].VariableName)){
      break;
    }
  }


  if (Index == MAX_SOCKET_VAR_INFO) {
  return EFI_NOT_FOUND;
}


  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = SocketVariableInfo[Index].BufferSize;
  Status = gRT->GetVariable (
                  SocketVariableInfo[Index].VariableName,
                  SocketVariableInfo[Index].Guid,
                  &Attributes,
                  &BufferSize,
                  SocketPtrs[Index]
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }


  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = SocketVariableInfo[Index].BufferSize;
  Status = mHiiConfigRouting->ConfigToBlock (
                               mHiiConfigRouting,
                               Configuration,
                               (UINT8 *) SocketPtrs[Index],
                               &BufferSize,
                               Progress
                               );


  //
  // Store Buffer Storage back to EFI variable
  //

  Status = gRT->SetVariable (
                  SocketVariableInfo[Index].VariableName,
                  SocketVariableInfo[Index].Guid,
                  Attributes,
                  SocketVariableInfo[Index].BufferSize,
                  SocketPtrs[Index]
                  );

  return Status;
}

/**
  Set AveragePowerLimit to Max value if user set value exceeds the value read from device characteristics

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @retval[out] EFI_SUCCESS
               EFI_NOT_FOUND
               EFI_DEVICE_ERROR
               EFI_OUT_OF_RESOURCES
**/
EFI_STATUS
EFIAPI
UpdateAveragePowerLimit (
  IN EFI_QUESTION_ID          QuestionId,
  IN EFI_IFR_TYPE_VALUE       *Value
  )
{
  EFI_INPUT_KEY                         Key;
  CHAR16                                *StringBuffer = NULL;
  SYSTEM_MEMORY_MAP_HOB                 *SystemMemoryMap;
  EFI_STATUS                            Status = EFI_SUCCESS;
  BOOLEAN                               HiiBrowserStatus;
  SOCKET_MEMORY_CONFIGURATION           SocketMemoryConfiguration;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  HiiBrowserStatus = HiiGetBrowserData (
    &gEfiSocketMemoryVariableGuid,
    SOCKET_MEMORY_CONFIGURATION_NAME,
    sizeof (SOCKET_MEMORY_CONFIGURATION),
    (UINT8*)&SocketMemoryConfiguration
    );

  if (!HiiBrowserStatus) {
    DEBUG ((EFI_D_INFO, "\nFailed to get browser data\n"));
    return EFI_NOT_FOUND;
  }

  if (SocketMemoryConfiguration.DcpmmAveragePowerLimit > SystemMemoryMap->MaxAveragePowerLimit) {
    StringBuffer = AllocateZeroPool (NEWSTRING_SIZE);
    if (StringBuffer == NULL) {
      DEBUG ((EFI_D_INFO, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
      return EFI_OUT_OF_RESOURCES;
    }
    UnicodeValueToStringS (StringBuffer, NEWSTRING_SIZE, 0, SystemMemoryMap->MaxAveragePowerLimit, 10);
    StrCatS (StringBuffer, NEWSTRING_SIZE / sizeof (CHAR16), L" mW");
    CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, L"Requested value exceeds system capabilities, setting to max supported value", StringBuffer, NULL);
    FreePool (StringBuffer);
    SocketMemoryConfiguration.DcpmmAveragePowerLimit = SystemMemoryMap->MaxAveragePowerLimit;
  }

  HiiBrowserStatus = HiiSetBrowserData (
    &gEfiSocketMemoryVariableGuid,
    SOCKET_MEMORY_CONFIGURATION_NAME,
    sizeof (SOCKET_MEMORY_CONFIGURATION),
    (UINT8*)&SocketMemoryConfiguration,
    NULL
    );

  if (!HiiBrowserStatus) {
    DEBUG ((EFI_D_INFO, "\nFailed to set browser data\n"));
    return EFI_ABORTED;
  }

  return Status;
}

/**
  Set MbbAveragePowerTimeConstant to Max value if user set value exceeds the value read from device characteristics

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @retval[out] EFI_SUCCESS
               EFI_NOT_FOUND
               EFI_DEVICE_ERROR
               EFI_OUT_OF_RESOURCES
**/
EFI_STATUS
EFIAPI
UpdateMbbAveragePowerTimeConstant (
  IN EFI_QUESTION_ID          QuestionId,
  IN EFI_IFR_TYPE_VALUE       *Value
  )
{
  EFI_INPUT_KEY                         Key;
  CHAR16                                *StringBuffer = NULL;
  SYSTEM_MEMORY_MAP_HOB                 *SystemMemoryMap;
  EFI_STATUS                            Status = EFI_SUCCESS;
  BOOLEAN                               HiiBrowserStatus;
  SOCKET_MEMORY_CONFIGURATION           SocketMemoryConfiguration;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  HiiBrowserStatus = HiiGetBrowserData (
    &gEfiSocketMemoryVariableGuid,
    SOCKET_MEMORY_CONFIGURATION_NAME,
    sizeof (SOCKET_MEMORY_CONFIGURATION),
    (UINT8*)&SocketMemoryConfiguration
    );

  if (!HiiBrowserStatus) {
    DEBUG ((EFI_D_INFO, "\nFailed to get browser data\n"));
    return EFI_NOT_FOUND;
  }

  if (SocketMemoryConfiguration.DcpmmMbbAveragePowerTimeConstant > SystemMemoryMap->MaxMbbAveragePowerTimeConstant) {
    StringBuffer = AllocateZeroPool (NEWSTRING_SIZE);
    if (StringBuffer == NULL) {
      DEBUG ((EFI_D_INFO, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
      return EFI_OUT_OF_RESOURCES;
    }
    UnicodeValueToStringS (StringBuffer, NEWSTRING_SIZE, 0, SystemMemoryMap->MaxMbbAveragePowerTimeConstant, 10);
    StrCatS (StringBuffer, NEWSTRING_SIZE / sizeof (CHAR16), L" ms");
    CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, L"Requested value exceeds system capabilities, setting to max supported value", StringBuffer, NULL);
    FreePool (StringBuffer);
    SocketMemoryConfiguration.DcpmmMbbAveragePowerTimeConstant = SystemMemoryMap->MaxMbbAveragePowerTimeConstant;
  }

  HiiBrowserStatus = HiiSetBrowserData (
    &gEfiSocketMemoryVariableGuid,
    SOCKET_MEMORY_CONFIGURATION_NAME,
    sizeof (SOCKET_MEMORY_CONFIGURATION),
    (UINT8*)&SocketMemoryConfiguration,
    NULL
    );

  if (!HiiBrowserStatus) {
    DEBUG ((EFI_D_INFO, "\nFailed to set browser data\n"));
    return EFI_ABORTED;
  }

  return Status;
}

/**
  Set MbbMaxPowerLimit to Max value if user set value exceeds the value read from device characteristics

  @param[in] QuestionId      A unique value which is sent to the original exporting
                             driver so that it can identify the type of data to expect.
                             The format of the data tends to vary based on the opcode
                             that generated the callback.
  @param[in] Value           A pointer to the data being sent to the original exporting
                             driver. The type is specified by Type. Type EFI_IFR_TYPE_VALUE
                             is defined in EFI_IFR_ONE_OF_OPTION.
  @retval[out] EFI_SUCCESS
               EFI_NOT_FOUND
               EFI_DEVICE_ERROR
               EFI_OUT_OF_RESOURCES
**/
EFI_STATUS
EFIAPI
UpdateMbbMaxPowerLimit (
  IN EFI_QUESTION_ID          QuestionId,
  IN EFI_IFR_TYPE_VALUE       *Value
  )
{
  EFI_INPUT_KEY                         Key;
  CHAR16                                *StringBuffer = NULL;
  SYSTEM_MEMORY_MAP_HOB                 *SystemMemoryMap;
  EFI_STATUS                            Status = EFI_SUCCESS;
  BOOLEAN                               HiiBrowserStatus;
  SOCKET_MEMORY_CONFIGURATION           SocketMemoryConfiguration;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  HiiBrowserStatus = HiiGetBrowserData (
    &gEfiSocketMemoryVariableGuid,
    SOCKET_MEMORY_CONFIGURATION_NAME,
    sizeof (SOCKET_MEMORY_CONFIGURATION),
    (UINT8*)&SocketMemoryConfiguration
    );

  if (!HiiBrowserStatus) {
    DEBUG ((EFI_D_INFO, "\nFailed to get browser data\n"));
    return EFI_NOT_FOUND;
  }

  if (SocketMemoryConfiguration.DcpmmMbbMaxPowerLimit > SystemMemoryMap->MaxMbbPowerLimit) {
    StringBuffer = AllocateZeroPool (NEWSTRING_SIZE);
    if (StringBuffer == NULL) {
      DEBUG ((EFI_D_INFO, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
      return EFI_OUT_OF_RESOURCES;
    }
    UnicodeValueToStringS (StringBuffer, NEWSTRING_SIZE, 0, SystemMemoryMap->MaxMbbPowerLimit, 10);
    StrCatS (StringBuffer, NEWSTRING_SIZE / sizeof (CHAR16), L" mW");
    CreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_RED, &Key, L"Requested value exceeds system capabilities, setting to max supported value", StringBuffer, NULL);
    FreePool (StringBuffer);
    SocketMemoryConfiguration.DcpmmMbbMaxPowerLimit = SystemMemoryMap->MaxMbbPowerLimit;
  }

  HiiBrowserStatus = HiiSetBrowserData (
    &gEfiSocketMemoryVariableGuid,
    SOCKET_MEMORY_CONFIGURATION_NAME,
    sizeof (SOCKET_MEMORY_CONFIGURATION),
    (UINT8*)&SocketMemoryConfiguration,
    NULL
    );

  if (!HiiBrowserStatus) {
    DEBUG ((EFI_D_INFO, "\nFailed to set browser data\n"));
    return EFI_ABORTED;
  }

  return Status;
}
/**

  This function is called to provide results data to the driver.
  This data consists of a unique key that is used to identify
  which data is either being passed back or being asked for.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect. The format of the data tends to
                                 vary based on the opcode that generated the callback.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
EFI_STATUS
EFIAPI
CallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  SOCKET_MEMORY_CONFIGURATION           SocketMemoryConfiguration;
  SOCKET_MP_LINK_CONFIGURATION          UpiConfig;
  UINT32                                RegEax = 0;
  UINT8                                 CpuStepping;
  SYSTEM_MEMORY_MAP_HOB                 *SystemMemoryMap;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  UINT32                                Attributes;
  SOCKET_IIO_CONFIGURATION              IioConfig;
  UINTN                                 Size;
  SOCKET_PROCESSORCORE_CONFIGURATION    ProcessorConfig;
  SOCKET_POWERMANAGEMENT_CONFIGURATION  SocketPMConfiguration;
  BOOLEAN                               HiiBrowserStatus;

  switch (Action) {
  case EFI_BROWSER_ACTION_FORM_CLOSE:
    if (QuestionId == KEY_SGX_OWNER_EPOCH) {
      Status = HandleSgxOwnerEpochValueVisibility (QuestionId, Value);
    }
    break;

  case EFI_BROWSER_ACTION_FORM_OPEN:
    if (QuestionId == KEY_SGX_OWNER_EPOCH) {
      Status = GetOwnerEpochUpdate (QuestionId, Value);
    }
    break;

  case EFI_BROWSER_ACTION_CHANGING:
    switch (QuestionId) {
      case KEY_SNC_EN:
        Status = HandleEnableSnc (QuestionId, Value);
        break;

      case KEY_NGN_FACTORY_CLEAN:
        //
        // Reset NVDIMMs power setting values default ones (base on controller type)
        //
        HiiBrowserStatus = HiiGetBrowserData (
          &gEfiSocketMemoryVariableGuid,
          SOCKET_MEMORY_CONFIGURATION_NAME,
          sizeof(SOCKET_MEMORY_CONFIGURATION),
          (UINT8*)&SocketMemoryConfiguration
          );

        if (!HiiBrowserStatus) {
          DEBUG ((EFI_D_ERROR, "\nFailed to get browser data\n"));
          return EFI_NOT_FOUND;
        }

        if (SocketMemoryConfiguration.EkvOnSystem) {
          DEBUG ((EFI_D_INFO, "\nReset Gen 1 DCPMM Average Power Budget to default %d [mW]\n", NGN_AVERAGE_POWER_DEFAULT));
          SocketMemoryConfiguration.NgnAveragePower = NGN_AVERAGE_POWER_DEFAULT;
        }

        if (SocketMemoryConfiguration.BwvOnSystem) {
          DEBUG ((EFI_D_INFO, "\nReset Gen 2 DCPMM Average Power Budget to default %d [mW]\n", AVERAGE_POWER_LIMIT_DEFAULT));
          SocketMemoryConfiguration.DcpmmAveragePowerLimit = AVERAGE_POWER_LIMIT_DEFAULT;

          DEBUG ((EFI_D_INFO, "\nSet default state of Gen 2 DCPMM Turbo/Memory Bandwidth Boost(MBB)\n"));
          SocketMemoryConfiguration.DcpmmMbbFeature = TURBO_MODE_STATE_DEFAULT;

          DEBUG ((EFI_D_INFO, "\nReset Gen 2 DCPMM Turbo/Memory Bandwidth Boost Feature(MBB) Power Limit to default %d [mW]\n", TURBO_POWER_LIMIT_DEFAULT));
          SocketMemoryConfiguration.DcpmmMbbMaxPowerLimit = TURBO_POWER_LIMIT_DEFAULT;

          if (SocketMemoryConfiguration.DcpmmApiVersion200OnSystem) {
            DEBUG ((EFI_D_INFO, "\nReset Gen 2 DCPMM Turbo Average Power Time Constant to default 96 [mS]\n"));
            SocketMemoryConfiguration.DcpmmAveragePowerTimeConstant = AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_0;
          }

          if (SocketMemoryConfiguration.DcpmmApiVersion201OnSystem) {
            DEBUG ((EFI_D_INFO, "\nReset Gen 2 DCPMM Memory Bandwidth Boost(MBB) Average Power Time Constant (in mSec) to default %d [mS]\n", AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_1_OR_LATER));
            SocketMemoryConfiguration.DcpmmMbbAveragePowerTimeConstant = AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_1_OR_LATER;
          }
        }

        HiiBrowserStatus = HiiSetBrowserData(
            &gEfiSocketMemoryVariableGuid,
            SOCKET_MEMORY_CONFIGURATION_NAME,
            sizeof(SOCKET_MEMORY_CONFIGURATION),
            (UINT8*)&SocketMemoryConfiguration,
            NULL
            );

        if (!HiiBrowserStatus) {
          DEBUG ((EFI_D_ERROR, "\nFailed to set browser data\n"));
          return EFI_ABORTED;
        }
        break;
    }
    break;

  case EFI_BROWSER_ACTION_CHANGED:
    switch (QuestionId) {
    case KEY_CPU_ONLINE:
      HiiGetBrowserData (
        &gEfiSocketMpLinkVariableGuid,
        SOCKET_MP_LINK_CONFIGURATION_NAME,
        sizeof(SOCKET_MP_LINK_CONFIGURATION),
        (UINT8*)&UpiConfig
        );
      UpiConfig.QpiCpuSktHotPlugEn = Value->u8;
      HiiSetBrowserData(
        &gEfiSocketMpLinkVariableGuid,
        SOCKET_MP_LINK_CONFIGURATION_NAME,
        sizeof(SOCKET_MP_LINK_CONFIGURATION),
        (UINT8*)&UpiConfig,
        NULL
        );

      HiiGetBrowserData (
        &gEfiSocketMemoryVariableGuid,
        SOCKET_MEMORY_CONFIGURATION_NAME,
        sizeof(SOCKET_MEMORY_CONFIGURATION),
        (UINT8*)&SocketMemoryConfiguration
        );
      SocketMemoryConfiguration.Srat = Value->u8;
      SocketMemoryConfiguration.SratCpuHotPlug= Value->u8;
      SocketMemoryConfiguration.SratMemoryHotPlug= Value->u8;
      HiiSetBrowserData(
        &gEfiSocketMemoryVariableGuid,
        SOCKET_MEMORY_CONFIGURATION_NAME,
        sizeof(SOCKET_MEMORY_CONFIGURATION),
        (UINT8*)&SocketMemoryConfiguration,
        NULL
        );
      break;

    case KEY_EIST_ENABLE:
      if (Value->u8 == 0) {
        HiiGetBrowserData (
          &gEfiSocketPowermanagementVarGuid,
          SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME,
          sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION),
          (UINT8*)&SocketPMConfiguration
          );
        CopyMem(&mSocketPMConfiguration, &SocketPMConfiguration, sizeof(SocketPMConfiguration));
        mSocketPMUpdate = 1;
        SocketPMConfiguration.ProcessorEistPsdFunc = 0;
        SocketPMConfiguration.EETurboDisable = 1;
        SocketPMConfiguration.BootPState = 0;
        SocketPMConfiguration.TurboMode = 0;
        SocketPMConfiguration.EnableXe = 0;
        HiiSetBrowserData(
          &gEfiSocketPowermanagementVarGuid,
          SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME,
          sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION),
          (UINT8*)&SocketPMConfiguration,
          NULL
          );
      } else {
        if (mSocketPMUpdate == 1) {
          HiiGetBrowserData (
            &gEfiSocketPowermanagementVarGuid,
            SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME,
            sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION),
            (UINT8*)&SocketPMConfiguration
            );
          SocketPMConfiguration.ProcessorEistPsdFunc = mSocketPMConfiguration.ProcessorEistPsdFunc;
          SocketPMConfiguration.EETurboDisable = mSocketPMConfiguration.EETurboDisable;
          SocketPMConfiguration.BootPState = mSocketPMConfiguration.BootPState;
          SocketPMConfiguration.TurboMode = mSocketPMConfiguration.TurboMode;
          SocketPMConfiguration.EnableXe = mSocketPMConfiguration.EnableXe;
          mSocketPMUpdate = 0;
          HiiSetBrowserData (
            &gEfiSocketPowermanagementVarGuid,
            SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME,
            sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION),
            (UINT8*)&SocketPMConfiguration,
            NULL
            );
        }
      }
      break;
    case KEY_MKTME_INTEGRITY:
      Status = HandleMktmeIntegrity  (QuestionId, Value);
      break;

    case KEY_SGX_OWNER_EPOCH:
      Status = HandleSgxOwnerEpoch (QuestionId, Value);
      break;

    case KEY_SGX_ENABLE:
      Status = HandleEnableSgx (QuestionId, Value);
      break;
    case KEY_SGX_FACTORY_RESET:
      Status = HandleFactoryResetSgx (QuestionId, Value);
      break;

    case KEY_UMA_BASED_CLUSTERING:
      Status = HandleUmaBasedClustering (QuestionId, Value);
      break;
    case KEY_X2APIC_ENABLE:
      Status = UpdateVtdOnx2ApicEnabled (QuestionId, Value);
      break;

    case KEY_AVG_POWER_CHANGE:
      Status = UpdateAveragePowerLimit (QuestionId, Value);
      break;

    case KEY_AVG_POWER_TIME_CONST_CHANGE:
      Status = UpdateMbbAveragePowerTimeConstant (QuestionId, Value);
      break;

    case KEY_MBB_MAX_POWER_CHANGE:
      Status = UpdateMbbMaxPowerLimit (QuestionId, Value);
      break;

    default:
      break;
    }
    break;

  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    // APTIOV_SERVER_OVERRIDE_RC_START: Fixed that setup items that have QuestionId and not get processed here would fail to load the default value.
    Status = EFI_UNSUPPORTED;
    // APTIOV_SERVER_OVERRIDE_RC_END: Fixed that setup items that have QuestionId and not get processed here would fail to load the default value.
    if ((QuestionId == 0xDFF1) || (QuestionId == 0xDFF8)) {
        GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
        if (GuidHob != NULL) {
            SystemMemoryMap = (struct SystemMemoryMapHob*) GET_GUID_HOB_DATA (GuidHob);
            Status = EFI_SUCCESS;
            switch (QuestionId) {
                case 0xDFF1:
                    //Assigning System Ras type value to the setup option(Value->u8) to avoid loading optimal default in SOCKET_COMMONRC_CONFIGURATION.SystemRasType variable when doing "Restore default"
                    Value->u8 = SystemMemoryMap->SystemRasType;
                    break;
                case 0xDFF8:
                    // Assigning XMPProfilesSup value to the setup option(Value->u8) to avoid loading optimal default in SOCKET_MEMORY_CONFIGURATION.XMPProfilesSup variable when doing "Restore default".
                    Value->u8 = SystemMemoryMap->XMPProfilesSup;
                    break;
            }
        } else {
            Status = EFI_NOT_FOUND;
            DEBUG ((EFI_D_ERROR, "\nGet SystemMemoryMap HOB data, Status = %r\n", Status));
        }
    } else if (QuestionId == 0xDFF2) {
        AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

        CpuStepping = (UINT8) (RegEax & 0x0F);

        // Assigning CpuStepping value to the setup option(Value->u8) to avoid loading optimal default in SOCKET_IIO_CONFIGURATION.PcieSlotItemCtrl variable when doing "Restore default".
        Value->u8 = CpuStepping;

    } else if (QuestionId == 0xDFF7) {
        Size = sizeof (SOCKET_IIO_CONFIGURATION);
        Status = gRT->GetVariable (
                        L"SocketIioConfig",
                        &gEfiSocketIioVariableGuid,
                        &Attributes,
                        &Size,
                        &IioConfig );
        if (EFI_ERROR (Status)) {
            Status = EFI_NOT_FOUND;
        } else {
            // Assigning PcieSlotItemCtrl value to the setup option(Value->u8) to avoid loading optimal default in SOCKET_IIO_CONFIGURATION.PcieSlotItemCtrl variable when doing "Restore default".
            Value->u8 = IioConfig.PcieSlotItemCtrl;
        }
    } else if (QuestionId == 0xDFF5) {
        Size = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
        Status = gRT->GetVariable (
                        L"SocketProcessorCoreConfig",
                        &gEfiSocketProcessorCoreVarGuid,
                        &Attributes,
                        &Size,
                        &ProcessorConfig );
        if (EFI_ERROR (Status)) {
            Status = EFI_NOT_FOUND;
        } else {
            // Assigning AcmType value to the setup option(Value->u8) to avoid loading optimal default in SOCKET_PROCESSORCORE_CONFIGURATION.AcmType variable when doing "Restore default".
            Value->u8 = ProcessorConfig.AcmType;
            Status = EFI_SUCCESS;
        }
    } else if (QuestionId == KEY_EIST_ENABLE){
        Status = EFI_SUCCESS;
        DEBUG((EFI_D_INFO, "\n EFI_BROWSER_ACTION_DEFAULT_STANDARD KEY_EIST_ENABLE : \n\n"));
        Value->u8 = 1;
    } else if (QuestionId == KEY_SGX_FACTORY_RESET) {
      if (FixedPcdGetBool(PcdBindSgxResetWithF9GlobalReset)) {
        Value->u8 = SECURITY_POLICY_ENABLE;
        //
        // notify caller (Setup.c : GetQuestionDefault) that default value has been determined by this callback
        //
        Status = EFI_SUCCESS;
      } else {
        //
        // notify caller to use different source for defalut question value
        //
        Status = EFI_UNSUPPORTED;
      }
    } else if (SGX_SUBOPTION(QuestionId)) {
      if (FixedPcdGetBool(PcdBindSgxResetWithF9GlobalReset)) {
        //
        // notify caller (Setup.c : GetQuestionDefault) to use different source for defalut question value
        //
        Status = EFI_UNSUPPORTED;
      } else {
        //
        // notify caller that default value has been determined by this callback
        // (the question value remains unchanged)
        //
        Status = EFI_SUCCESS;
      }
    } else {
        //
        // as the hfr files have different default values for different processor configurations ,
        // let's notify caller (Setup.c : GetQuestionDefault) to use different source for default question value
        //
        Status = EFI_UNSUPPORTED;
    }
    break;

  case EFI_BROWSER_ACTION_RETRIEVE:
    break;

  default:
    //
    // Do nothing for other UEFI Action.
    //
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
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

  Nmi = (ReadNmiEn () & 0x80);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  if (IoRead8 (R_IOPORT_CMOS_STANDARD_DATA) & (BIT6 + BIT7)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
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

  Nmi = (ReadNmiEn () & 0x80);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  Value = IoRead8 (R_IOPORT_CMOS_STANDARD_DATA);
  IoWrite8 (R_IOPORT_CMOS_STANDARD_DATA, Value & 0x3F);
}

/**

  Acquire the string associated with the Index from SMBIOS structure and return it.
  The caller is responsible for freeing the string buffer.

  @param    OptionalStrStart  The start position to search the string
  @param    Index             The index of the string to extract
  @param    String            The string that is extracted

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS if successful.
  @retval   EFI_NOT_FOUND     The function returns EFI_NOT_FOUND if unsuccessful.

**/
EFI_STATUS
SmbiosGetOptionalStringByIndex (
  IN      CHAR8                   *OptionalStrStart,
  IN      UINT8                   Index,
  OUT     CHAR16                  **String,
  IN      UINTN                   StringSize
  )
{
  UINTN          StrSize;

  if (Index == 0) {
    return EFI_INVALID_PARAMETER;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart); // size includes null terminator
  // SMBIOS strings are NULL terminated, and end of all strings is indicated with NULL
  // loop until at end of all SMBIOS strings (found NULL terminator at string index past string's NULL), and Index != 0
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  if ((Index != 0) || (StrSize == 1)) {
    //
    // Meet the end of strings set but Index is non-zero
    return EFI_INVALID_PARAMETER;
  } else {
    AsciiStrToUnicodeStrS (OptionalStrStart, *String, StringSize);
  }

  return EFI_SUCCESS;
}

/**

  Find the SMBIOS structure with the specified Type and Handle.
  If Type is NULL then only Handle will be matched.

  @param    Handle            The Handle to search for.
  @param    Type              The SMBIOS Type to search for.
  @param    Record            The SMBIOS record if a match is found

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS if successful.
  @retval   EFI_NOT_FOUND     The function returns EFI_NOT_FOUND if unsuccessful.

**/
EFI_STATUS
SmbiosGetStructure (
  IN      UINT16  Handle,
  IN      EFI_SMBIOS_TYPE         Type,
  OUT     EFI_SMBIOS_TABLE_HEADER **Record
  )
{
  EFI_SMBIOS_HANDLE                 SmbiosHandle;
  EFI_SMBIOS_PROTOCOL               *Smbios;
  SMBIOS_HEADER                     *SmbiosHeader;
  EFI_STATUS                        Status;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);

  SmbiosHandle = 0;
  while (1) {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, &Type, Record, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }
    SmbiosHeader = (SMBIOS_HEADER *) *Record;
    if (SmbiosHeader->Handle == Handle) {
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
GetCpuInformation (
  VOID                         *Context
  )
{
  EFI_STATUS                   Status;
  GET_CPU_INFO_CONTEXT         *GetCpuInfoContext;
  UINTN                        Index;
  UINTN                        Package;

  GetCpuInfoContext = (GET_CPU_INFO_CONTEXT*)Context;
  Status = GetCpuInfoContext->MpService->WhoAmI (GetCpuInfoContext->MpService, &Index);
  if (EFI_ERROR (Status)) {
    CpuDeadLoop ();
  }

  //
  // Get microcode revision from one thread in the socket
  //
  Package = GetCpuInfoContext->ProcessorInfo[Index].ExtendedInformation.Location2.Package;
  mCpuSetupInfo[Package].Ucode = (UINT32) RShiftU64 (AsmReadMsr64 (MSR_BIOS_SIGN_ID), 32);
}

/**

    This function update the cpu socket HII information.

    @param[in]  HiiHandle        A handle that was previously registered in the HII Database.
    @param[in]  StringId         The identifier of the string to retrieved from the string
                                 package associated with HiiHandle.
    @param[in]  SocketIndex      CPU socket index.
    @param[in]  UpdateInfoField  Socket field information to update.
    @param[in]  BspSocketIndex   BSP socket index.

    @retval     None

**/
VOID
UpdateCpuHii (
  IN EFI_HII_HANDLE            HiiHandle,
  IN EFI_STRING_ID             StringId,
  IN UINTN                     SocketIndex,
  IN SOCKET_UPDATE_INFO_FIELDS UpdateInfoField,
  IN UINTN                     BspSocketIndex
  )
{
  UINTN                StrSize;
  CHAR16               *NewString;
  CHAR16               *OldString;
  UINTN                NaLen;
  CHAR16               *NotAvailable;

  StrSize          = NEWSTRING_SIZE;
  NewString        = AllocateZeroPool (StrSize);
  if (NewString   == NULL) {
    return;
  }
  NotAvailable     = L"   N/A   ";
  NaLen            = StrLen (NotAvailable) * 2;

  // Get string from HII
  OldString        = HiiGetString (HiiHandle, StringId, NULL);
  if (OldString   == NULL) {
    FreePool (NewString);
    return;
  }

  StrCpyS (NewString, NEWSTRING_SIZE / sizeof(CHAR16), OldString);
  gBS->FreePool (OldString);

  if ((SocketIndex % 2) != 0) {
    // Append separation bar
    StrCatS (NewString, NEWSTRING_SIZE / sizeof(CHAR16), L" |  ");
  }

  switch (UpdateInfoField) {
    case ProcIdVal:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s%08x%c",
        NewString,
        (UINTN) mCpuSetupInfo[SocketIndex].Id,
        (SocketIndex == BspSocketIndex) ? '*' : ' '
        );
      break;
    case ProcFreqVal:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s%1d.%03dGHz ",
        NewString,
        (UINTN) (mCpuSetupInfo[SocketIndex].Freq / 1000),
        (UINTN) (mCpuSetupInfo[SocketIndex].Freq % 1000)
        );
      break;
    case ProcMaxRatioVal:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s     %02xH ",
        NewString,
        (UINTN) (mCpuSetupInfo[SocketIndex].MaxRatio)
        );
      break;
    case ProcMinRatioVal:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s     %02xH ",
        NewString,
        (UINTN) (mCpuSetupInfo[SocketIndex].MinRatio)
        );
      break;
    case ProcUcodeVal:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s%08x ",
        NewString,
        (UINTN) mCpuSetupInfo[SocketIndex].Ucode
        );
      break;
    case ProcL1Val:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s%6dKB ",
        NewString,
        (UINTN) mCpuSetupInfo[SocketIndex].L1Cache
        );
      break;
    case ProcL2Val:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s%6dKB ",
        NewString,
        (UINTN) mCpuSetupInfo[SocketIndex].L2Cache
        );
      break;
    case ProcL3Val:
      UnicodeSPrint (
        NewString,
        StrSize,
        L"%s%6dKB ",
        NewString,
        (UINTN) mCpuSetupInfo[SocketIndex].L3Cache
        );
      break;
    default:
      break;
  }

  if ((mCpuSetupInfo[SocketIndex].Id == 0) ||
      ((mCpuSetupInfo[SocketIndex].Ucode == 0)&& (UpdateInfoField == ProcUcodeVal))) {
    if ((SocketIndex % 2) == 0) {
      CopyMem (&NewString[0], NotAvailable, NaLen);
    } else {
      CopyMem (&NewString[13], NotAvailable, NaLen);
    }
  }
  HiiSetString (HiiHandle, StringId, NewString, NULL);

  gBS->FreePool (NewString);
  return;
}

/**

    Update dynamic data in CPU setup menus

    @param None

    @retval VOID

**/
VOID
UpdateCpuInformation (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 StrSize;
  EFI_STRING_ID                         PromptTokenToUpdate;
  EFI_STRING_ID                         TokenToUpdate;
  CHAR16                                *NewString;
  CHAR16                                *VersionString[MAX_SOCKET];
  UINTN                                 SocketIndex;
  UINTN                                 RealSocketIndex;
  UINTN                                 DieCount;
  UINTN                                 BspPackageNumber;
  UINT32                                BspCpuidSignature;
  UINT8                                 StrIndex;
  UINT8                                 ConfigTdpLevel;
  EFI_SMBIOS_HANDLE                     SmbiosHandle;
  EFI_SMBIOS_PROTOCOL                   *Smbios;
  SMBIOS_TABLE_TYPE4                    *SmbiosType4Record;
  SMBIOS_TABLE_TYPE7                    *SmbiosType7Record;
  EFI_SMBIOS_TYPE                       SmbiosType;
  EFI_SMBIOS_TABLE_HEADER               *SmbiosRecord;
  EFI_SMBIOS_TABLE_HEADER               *SmbiosRecord2;
  UINT32                                RegEax, RegEbx, RegEcx, RegEdx;
  GET_CPU_INFO_CONTEXT                  GetCpuInfoContext;
  UINTN                                 NumberOfProcessors;
  UINTN                                 NumberOfEnabledProcessors;
  UINTN                                 Index;
  UINT64                                CoreBitMap;
  CHAR16                                ProcessorVersion[PROC_VER_ARRAY_SIZE];
  CHAR16                                ProcessorVersion2[PROC_VER_ARRAY_SIZE];
  CONST EFI_STRING_ID                   TokensToUpdate[MAX_SOCKETS_UPDATE_FIELDS][MAX_SOCKETS_UPDATE_INFO] = {
                                          {STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET0_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET1_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET2_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET3_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET4_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET5_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET6_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_VERSION_SOCKET7_VALUE)
                                          },  /*0 STR_PROCESSOR_VERSION_SOCKETX_VALUE */
                                          {STRING_TOKEN (STR_PROCESSOR_ID_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_ID_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_ID_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_ID_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_ID_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_ID_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_ID_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_ID_VALUE)
                                          },
                                          {STRING_TOKEN (STR_PROCESSOR_FREQ_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_FREQ_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_FREQ_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_FREQ_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_FREQ_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_FREQ_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_FREQ_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_FREQ_VALUE)
                                          },
                                          {STRING_TOKEN (STR_PROCESSOR_MAX_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_MAX_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_MAX_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_MAX_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_MAX_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_MAX_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_MAX_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_MAX_RATIO_VALUE)
                                          },
                                          {STRING_TOKEN (STR_PROCESSOR_MIN_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_MIN_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_MIN_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_MIN_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_MIN_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_MIN_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_MIN_RATIO_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_MIN_RATIO_VALUE)
                                          },
                                          {STRING_TOKEN (STR_PROCESSOR_MICROCODE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_MICROCODE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_MICROCODE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_MICROCODE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_MICROCODE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_MICROCODE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_MICROCODE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_MICROCODE_VALUE)
                                          },
                                          {STRING_TOKEN (STR_PROCESSOR_L1_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_L1_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_L1_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_L1_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_L1_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_L1_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_L1_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_L1_CACHE_VALUE)
                                          },
                                          {STRING_TOKEN (STR_PROCESSOR_L2_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_L2_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_L2_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_L2_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_L2_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_L2_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_L2_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_L2_CACHE_VALUE)
                                          },
                                          {STRING_TOKEN (STR_PROCESSOR_L3_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR_L3_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_L3_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR2_3_L3_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_L3_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR4_5_L3_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_L3_CACHE_VALUE),
                                           STRING_TOKEN (STR_PROCESSOR6_7_L3_CACHE_VALUE)
                                          },
                                        };

  StrSize      = NEWSTRING_SIZE;
  NewString    = AllocateZeroPool (StrSize);
  ZeroMem (mCpuSetupInfo, sizeof (CPU_SETUP_INFO) * MAX_SOCKET);
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    VersionString[SocketIndex] = AllocateZeroPool (StrSize);
  }

  //
  // Get BSP CPU ID
  //
  AsmCpuid (CPUID_VERSION_INFO, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  BspCpuidSignature = RegEax;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  ASSERT_EFI_ERROR (Status);

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
  while (1) {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }

    SmbiosType4Record = (SMBIOS_TABLE_TYPE4 *) SmbiosRecord;
    StrIndex = SmbiosType4Record->Socket;
    if (SmbiosGetOptionalStringByIndex ((CHAR8*)((UINT8*)SmbiosType4Record + SmbiosType4Record->Hdr.Length), StrIndex,
                                        &NewString, StrSize / sizeof (CHAR16)) == EFI_SUCCESS) {
      // string should be 'CPUxx'
      if (StrnCmp(NewString, L"CPU", 3) != 0) {
        continue;
      }

      SocketIndex = StrDecimalToUintn(NewString + StrLen(L"CPU"));
      // if Status [6] is set then socket is populated
      if (SocketIndex < MAX_SOCKET && (SmbiosType4Record->Status & 0x40)) {
        mCpuSetupInfo[SocketIndex].Id = *(UINT32 *) &SmbiosType4Record->ProcessorId.Signature;
        mCpuSetupInfo[SocketIndex].Freq = SmbiosType4Record->CurrentSpeed;

        //
        //------------Begin: Setting cache information. Note that the cache level is 0 BASED!!!!------------
        //
        if (SmbiosGetStructure (SmbiosType4Record->L1CacheHandle, EFI_SMBIOS_TYPE_CACHE_INFORMATION, &SmbiosRecord2) == EFI_SUCCESS) {
          SmbiosType7Record = (SMBIOS_TABLE_TYPE7 *) SmbiosRecord2;
          if ((SmbiosType7Record->CacheConfiguration & 0x7) == 0) { // [2:0] is Cache Level; verify it matches
            // [15] indicates granularity; 0=1 KB, 1=64 KB
            // set cache size in bytes by setting the value to 1024*size
            mCpuSetupInfo[SocketIndex].L1Cache = (SmbiosType7Record->InstalledSize & 0x7FFF) / SmbiosType4Record->CoreCount;
            // if granularity is 64 KB, then multiply by 64
            if (SmbiosType7Record->InstalledSize & 0x8000) {
              mCpuSetupInfo[SocketIndex].L1Cache *= 64;
            }
          }
        }

        if (SmbiosGetStructure (SmbiosType4Record->L2CacheHandle, EFI_SMBIOS_TYPE_CACHE_INFORMATION, &SmbiosRecord2) == EFI_SUCCESS) {
          SmbiosType7Record = (SMBIOS_TABLE_TYPE7 *) SmbiosRecord2;
          if ((SmbiosType7Record->CacheConfiguration & 0x7) == 1) { // [2:0] is Cache Level; verify it matches
            // [15] indicates granularity; 0=1 KB, 1=64 KB
            // set cache size in bytes by setting the value to 1024*size
            if ( (BspCpuidSignature & 0xFFFF0) == (CPU_FAMILY_SNR << 4)) {
              // SNR - L2 cache is per CPU Module (TMT Max - 4 cores) not per core
              mCpuSetupInfo[SocketIndex].L2Cache = (SmbiosType7Record->InstalledSize) & 0x7FFF;
            } else {
              mCpuSetupInfo[SocketIndex].L2Cache = (SmbiosType7Record->InstalledSize & 0x7FFF) / SmbiosType4Record->CoreCount;
            }
            // if granularity is 64 KB, then multiply by 64
            if (SmbiosType7Record->InstalledSize & 0x8000) {
              mCpuSetupInfo[SocketIndex].L2Cache *= 64;
            }
          }
        }

        if (SmbiosGetStructure (SmbiosType4Record->L3CacheHandle, EFI_SMBIOS_TYPE_CACHE_INFORMATION, &SmbiosRecord2) == EFI_SUCCESS) {
          SmbiosType7Record = (SMBIOS_TABLE_TYPE7 *) SmbiosRecord2;
          if ((SmbiosType7Record->CacheConfiguration & 0x7) == 2) { // [2:0] is Cache Level; verify it matches
            // [15] indicates granularity; 0=1 KB, 1=64 KB
            // set cache size in bytes by setting the value to 1024*size
            mCpuSetupInfo[SocketIndex].L3Cache = SmbiosType7Record->InstalledSize & 0x7FFF;
            // if granularity is 64 KB, then multiply by 64
            if (SmbiosType7Record->InstalledSize & 0x8000) {
              mCpuSetupInfo[SocketIndex].L3Cache *= 64;
            }
          }
        }
        //
        //------------End: Setting cache information. Note that the cache level is 0 BASED!!!!------------
        //
        StrIndex = SmbiosType4Record->ProcessorVersion;
        if (SmbiosGetOptionalStringByIndex ((CHAR8*)((UINT8*)SmbiosType4Record + SmbiosType4Record->Hdr.Length), StrIndex, &VersionString[SocketIndex], (StrSize/sizeof(CHAR16))) == EFI_SUCCESS) {
          mCpuSetupInfo[SocketIndex].Version = VersionString[SocketIndex];
        }
      }
    }
  }

  //
  // Get Microcode version from each socket (package)
  //
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &GetCpuInfoContext.MpService);
  if (!EFI_ERROR (Status)) {
    Status = GetCpuInfoContext.MpService->GetNumberOfProcessors (GetCpuInfoContext.MpService, &NumberOfProcessors, &NumberOfEnabledProcessors);
    if (!EFI_ERROR (Status)) {
      GetCpuInfoContext.ProcessorInfo = AllocatePool (sizeof (*GetCpuInfoContext.ProcessorInfo) * NumberOfProcessors);
      if (GetCpuInfoContext.ProcessorInfo != NULL) {
        for (Index = 0; Index < NumberOfProcessors; Index++) {
          Status = GetCpuInfoContext.MpService->GetProcessorInfo (GetCpuInfoContext.MpService, Index | CPU_V2_EXTENDED_TOPOLOGY, &GetCpuInfoContext.ProcessorInfo[Index]);
          DEBUG ((DEBUG_INFO, "CPU[%03d]: P=%d C=%d T=%d, P2=%d D2=%d C2=%d T2=%d %r\n", Index,
            GetCpuInfoContext.ProcessorInfo[Index].Location.Package, GetCpuInfoContext.ProcessorInfo[Index].Location.Core, GetCpuInfoContext.ProcessorInfo[Index].Location.Thread,
            GetCpuInfoContext.ProcessorInfo[Index].ExtendedInformation.Location2.Package, GetCpuInfoContext.ProcessorInfo[Index].ExtendedInformation.Location2.Die,
            GetCpuInfoContext.ProcessorInfo[Index].ExtendedInformation.Location2.Core, GetCpuInfoContext.ProcessorInfo[Index].ExtendedInformation.Location2.Thread,
            Status
            ));
        }
        GetCpuInformation (&GetCpuInfoContext);
        GetCpuInfoContext.MpService->StartupAllAPs (
          GetCpuInfoContext.MpService,
          (EFI_AP_PROCEDURE)GetCpuInformation,
          FALSE,
          NULL,
          0,
          &GetCpuInfoContext,
          NULL
        );
        FreePool (GetCpuInfoContext.ProcessorInfo);
      }
    }
  }

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    mCpuSetupInfo[SocketIndex].MaxRatio = (UINT32) mCpuVarDataPtr->MaxCoreToBusRatio[SocketIndex];
    mCpuSetupInfo[SocketIndex].MinRatio = (UINT32) mCpuVarDataPtr->MinCoreToBusRatio[SocketIndex];
    mCpuSetupInfo[SocketIndex].FusedCores = mCpuVarDataPtr->FusedCores[SocketIndex];
    mCpuSetupInfo[SocketIndex].ActiveCores = mCpuVarDataPtr->ActiveCores[SocketIndex];

    //Populate the Setup structure for use in overclockinginterface driver
    mSocketPowermanagementConfiguration.MaxEfficiencyRatio[SocketIndex] = (UINT32) mCpuVarDataPtr->MinCoreToBusRatio[SocketIndex];
    mSocketPowermanagementConfiguration.MaxNonTurboRatio[SocketIndex] = (UINT32) mCpuVarDataPtr->MaxCoreToBusRatio[SocketIndex];
  }

  BspPackageNumber  = 0;

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    if (mCpuSetupInfo[SocketIndex].Version) {
      StrnCpyS (ProcessorVersion, PROC_VER_ARRAY_SIZE, mCpuSetupInfo[SocketIndex].Version, (PROC_VER_ARRAY_SIZE-1));
      StrnCpyS (ProcessorVersion2, PROC_VER_ARRAY_SIZE, ((mCpuSetupInfo[SocketIndex].Version) + (PROC_VER_ARRAY_SIZE-1)), (PROC_VER_ARRAY_SIZE-1));
      UnicodeSPrint (NewString, StrSize, L"%s\n%s", ProcessorVersion, ProcessorVersion2);
      TokenToUpdate = TokensToUpdate[ProcVersionVal][SocketIndex];//STR_PROCESSOR_VERSION_SOCKET0_VALUE;
      HiiSetString (mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);
      SetMem (ProcessorVersion, sizeof (ProcessorVersion), 0);
      SetMem (ProcessorVersion2, sizeof (ProcessorVersion2), 0);
    }
    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcIdVal][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcIdVal, BspPackageNumber);

    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcFreqVal][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcFreqVal, BspPackageNumber);

    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcMaxRatioVal][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcMaxRatioVal, BspPackageNumber);

    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcMinRatioVal][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcMinRatioVal, BspPackageNumber);

    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcUcodeVal][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcUcodeVal, BspPackageNumber);

    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcL1Val][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcL1Val, BspPackageNumber);

    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcL2Val][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcL2Val, BspPackageNumber);

    TokenToUpdate = (STRING_REF) TokensToUpdate[ProcL3Val][SocketIndex];
    UpdateCpuHii (mSocketStringsHiiHandle, TokenToUpdate, SocketIndex, ProcL3Val, BspPackageNumber);
  }
  //
  // Display Processor Revision in the Main page
  //
  TokenToUpdate = STR_CPU_BSP_REVISION_VALUE;
  switch (BspCpuidSignature) {
    case ((CPU_FAMILY_SKX << 4) + SKX_A0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SKX A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + SKX_A2_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SKX A2", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + SKX_B0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SKX B0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + SKX_B1_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SKX B1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + SKX_H0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SKX H0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + CLX_A0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - CLX A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + CLX_B0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - CLX B0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + CPX_A0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - CPX A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SKX << 4) + CPX_A1_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - CPX A1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_ICX << 4) + ICXSP_R0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - ICX R0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_ICX << 4) + ICXSP_L0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - ICX L0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_ICXD << 4) + ICXD_A0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - ICX-D A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SPRSP << 4) + SPRSP_A0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SPR-SP A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SPRSP << 4) + SPRSP_B0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SPR-SP B0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SPRSP << 4) + SPRSP_C0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SPR-SP C0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SPRSP << 4) + SPRSP_D0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SPR-SP D0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SNR << 4) + SNR_A0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SNR A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SNR << 4) + SNR_A1_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SNR A1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SNR << 4) + SNR_A2_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SNR A2", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SNR << 4) + SNR_A3_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SNR A3", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SNR << 4) + SNR_B0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SNR B0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SNR << 4) + SNR_B1_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SNR B1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case ((CPU_FAMILY_SNR << 4) + SNR_C0_CPU_STEP):
      UnicodeSPrint (NewString, StrSize, L"%5X - SNR C0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    default:
      UnicodeSPrint (NewString, StrSize, L"%5X - Unknown", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
  }
  HiiSetString (mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    CoreBitMap = mCpuSetupInfo[SocketIndex].ActiveCores;

    switch (SocketIndex) {
       case 0:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_0;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_0;
         break;
       case 1:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_1;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_1;
         break;
       case 2:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_2;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_2;
         break;
       case 3:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_3;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_3;
         break;
       case 4:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_4;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_4;
         break;
       case 5:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_5;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_5;
         break;
       case 6:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_6;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_6;
         break;
       default:
         TokenToUpdate = STR_AVAILABLE_CORE_BITMAP_VALUE_7;
         PromptTokenToUpdate = STR_SOCKET_CONFIG_FORM_TITLE_7;
         break;
    }

    UnicodeSPrint (NewString, StrSize, L"%16lX", CoreBitMap);
    HiiSetString (mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);
    DieCount = PcuGetDieCount ((UINT8) SocketIndex);
    if (DieCount > 1) {
      RealSocketIndex = GetSocketForSetupDisplay ((UINT8) SocketIndex);
      UnicodeSPrint (NewString, StrSize, L"CPU Socket %d (Die %d) Configuration", RealSocketIndex, SocketIndex % DieCount);
      HiiSetString (mSocketStringsHiiHandle, PromptTokenToUpdate, NewString, NULL);
    }
  }

  //
  // Populate the config TDP Setup info on "CPU P State Control" page of "Advanced Power Management Configuration" form
  // First get current TDP level
  //
  ConfigTdpLevel = (UINT8)mCpuVarDataPtr->IssConfigTdpCurrentLevel;

  //
  // Print core count for all levels
  //
  TokenToUpdate = STR_ISS_CONFIG_TDP_CORE_VALUE;

  UnicodeSPrint (
    NewString,
    StrSize,
    L"   Core Count            %02d  |    %02d    |    %02d",
    (UINTN) mCpuVarDataPtr->IssConfigTdpCoreCount[0][CONFIG_TDP_LEVEL_0],
    (UINTN) mCpuVarDataPtr->IssConfigTdpCoreCount[0][CONFIG_TDP_LEVEL_3],
    (UINTN) mCpuVarDataPtr->IssConfigTdpCoreCount[0][CONFIG_TDP_LEVEL_4]
    );
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // If current TDP level higher than 0
  //
  TokenToUpdate = STR_ISS_CONFIG_TDP_RATIO_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"   Current P1 Ratio [%d]  %02d  |    %02d    |    %02d",
    (UINTN) ConfigTdpLevel,
    (UINTN) mCpuVarDataPtr->IssConfigTdpRatio[0][CONFIG_TDP_LEVEL_0],
    (UINTN) mCpuVarDataPtr->IssConfigTdpRatio[0][CONFIG_TDP_LEVEL_3],
    (UINTN) mCpuVarDataPtr->IssConfigTdpRatio[0][CONFIG_TDP_LEVEL_4]
    );
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = STR_ISS_CONFIG_TDP_POWER_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"   Package TDP (W)      %03d  |   %03d    |   %03d",
    (UINTN) (mCpuVarDataPtr->IssConfigTdpPower[0][CONFIG_TDP_LEVEL_0]),
    (UINTN) mCpuVarDataPtr->IssConfigTdpPower[0][CONFIG_TDP_LEVEL_3],
    (UINTN) mCpuVarDataPtr->IssConfigTdpPower[0][CONFIG_TDP_LEVEL_4]
    );
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = STR_ISS_CONFIG_TDP_TJMAX_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"   Tjmax                %03d  |   %03d    |   %03d",
    (UINTN) mCpuVarDataPtr->IssConfigTdpTjmax[0][CONFIG_TDP_LEVEL_0],
    (UINTN) mCpuVarDataPtr->IssConfigTdpTjmax[0][CONFIG_TDP_LEVEL_3],
    (UINTN) mCpuVarDataPtr->IssConfigTdpTjmax[0][CONFIG_TDP_LEVEL_4]
    );
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  gBS->FreePool (NewString);
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    gBS->FreePool (VersionString[SocketIndex]);
  }

  return;
}

/**
  Function to update value of max MK-TME keys.
  Value is displayed in setup.

  @param  NONE

  @retval VOID
**/
VOID
UpdateMktmeSetupInformation(
)
{
  UINTN                                 StrSize = 0x200;
  EFI_STATUS                            Status;
  EFI_STRING_ID                         TokenToUpdate;
  CHAR16                                *NewString = NULL;
  UINT64                                MaxKeys;
  struct EFI_HOB_GUID_TYPE              *InitDataGuidHob = NULL;
  struct MKTME_INIT_DATA_HOB            *MktmeInitData = NULL;
  SOCKET_PROCESSORCORE_CONFIGURATION    SocketProcessorcoreConfiguration;
  UINTN                                 BufferSize;
  UINT32                                Attributes;

  InitDataGuidHob = GetFirstGuidHob (&gMktmeInitDataGuid);
  if (InitDataGuidHob != NULL) {
    MktmeInitData = (MKTME_INIT_DATA_HOB*) GET_GUID_HOB_DATA (InitDataGuidHob);
  } else {
    DEBUG ((EFI_D_ERROR, "gMktmeInitDataGuid is NULL! \n"));
    return;
  }

  BufferSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);

  Status = gRT->GetVariable (
                  (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                  &gEfiSocketProcessorCoreVarGuid,
                  &Attributes,
                  &BufferSize,
                  &SocketProcessorcoreConfiguration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "TME: Can not detect SOCKET_PROCESSORCORE_CONFIGURATION data.\n"));
    return;
  }

  SocketProcessorcoreConfiguration.TmeCapability   = MktmeInitData->TmeCapability;
  SocketProcessorcoreConfiguration.MktmeCapability = MktmeInitData->MktmeCapability;

  //No need to check MkTmeMaxKeys if TME or MK-TME is not supported
  if ((MktmeInitData->TmeCapability) && (MktmeInitData->MktmeCapability)) {
    SocketProcessorcoreConfiguration.MkTmeKeyIdBits = MktmeInitData->KeyIdBits;
    MaxKeys = (UINT64) (LShiftU64 (1, MktmeInitData->KeyIdBits) - 1);
    SocketProcessorcoreConfiguration.MktmeMaxKeys = (UINT16) MaxKeys;

    //Update setup string
    NewString = AllocateZeroPool (StrSize);
    if (NewString == NULL) {
      DEBUG ((EFI_D_ERROR, "ERROR: Unable to allocate TME setup string.\n"));
      ASSERT (FALSE);
      return;
    }
    UnicodeSPrint (NewString, StrSize, L"0x%lX", MaxKeys);
    TokenToUpdate = STRING_TOKEN (STR_MKTME_MAX_KEYS_VALUE);
    HiiSetString (mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);
    FreePool (NewString);
  }

  Status = gRT->SetVariable (
                  (CHAR16 *) (SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                  &gEfiSocketProcessorCoreVarGuid,
                  Attributes,
                  BufferSize,
                  &SocketProcessorcoreConfiguration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "TME: Can not set SOCKET_PROCESSORCORE_CONFIGURATION data.\n"));
    return;
  }
}

/**

  Function to update TDX capability setup information.

  @retval None

**/

VOID
UpdateTdxCapability (
  VOID
  )
{
  struct EFI_HOB_GUID_TYPE           *GuidHob;
  UINTN                              BufferSize;
  UINT32                             Attributes;
  EFI_STATUS                         Status;
  SOCKET_PROCESSORCORE_CONFIGURATION SocketProcessorcoreConfiguration;
  TDX_DATA_HOB                       *pTdxDataHob;

  GuidHob = GetFirstGuidHob(&gTdxDataHobGuid);
  if (GuidHob != NULL) {
    pTdxDataHob = (TDX_DATA_HOB*)GET_GUID_HOB_DATA(GuidHob);
    if (pTdxDataHob == NULL) {
      DEBUG((EFI_D_ERROR, "TDX: Error, Tdx_DATA_HOB is NULL, Exiting..\n"));
      return;
    }
  } else {
    DEBUG((EFI_D_ERROR, "TDX: GuidHob pointer is NULL \n"));
    return;
  }

  BufferSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);

  Status = gRT->GetVariable (
                  (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                  &gEfiSocketProcessorCoreVarGuid,
                  &Attributes,
                  &BufferSize,
                  &SocketProcessorcoreConfiguration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "TDX: Can't detect SOCKET_PROCESSORCORE_CONFIGURATION data.\n"));
    return;
  }

  SocketProcessorcoreConfiguration.TdxCapability = pTdxDataHob->TdxCapability;

  Status = gRT->SetVariable (
                  (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                  &gEfiSocketProcessorCoreVarGuid,
                  Attributes,
                  BufferSize,
                  &SocketProcessorcoreConfiguration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "TDX: Can't set SOCKET_PROCESSORCORE_CONFIGURATION data.\n"));
    return;
  }
}

/**

  Function to update CFR capability setup information.
  The CFRManualCommit only be executed one time. So, when setup startup,
  need set the CFRManualCommit back to 0.

  @retval None

**/
VOID
UpdateCFRConfig (
  VOID
  )
{
  UINTN                                     BufferSize;
  UINT32                                    Attributes;
  EFI_STATUS                                Status;
  SOCKET_PROCESSORCORE_CONFIGURATION        SocketProcessorcoreConfiguration;

  BufferSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);

  Status = gRT->GetVariable (
                  (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                  &gEfiSocketProcessorCoreVarGuid,
                  &Attributes,
                  &BufferSize,
                  &SocketProcessorcoreConfiguration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "CFR: Can't detect SOCKET_PROCESSORCORE_CONFIGURATION data.\n"));
    return;
  }
  if (SocketProcessorcoreConfiguration.CFRS3mManualCommit == 0 && SocketProcessorcoreConfiguration.CFRPucodeManualCommit == 0) {
    return;
  }

  SocketProcessorcoreConfiguration.CFRS3mManualCommit = 0;
  SocketProcessorcoreConfiguration.CFRPucodeManualCommit = 0;
  Status = gRT->SetVariable (
                  (CHAR16 *)(SOCKET_PROCESSORCORE_CONFIGURATION_NAME),
                  &gEfiSocketProcessorCoreVarGuid,
                  Attributes,
                  BufferSize,
                  &SocketProcessorcoreConfiguration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "CFR: Can't set SOCKET_PROCESSORCORE_CONFIGURATION data.\n"));
    return;
  }
}

/**
  Function to control setup question visiblity.
  Use this for disable setup questions related to NULL library vs Real Library

  @retval None
**/
VOID
SetMrcKnobVisiblity  (
  VOID
  )
{
  UINT8                             SsaLoaderKnob = 0;
  UINT8                             LegacyRmtKnob = 0;
  UINT8                             SsaRmtKnob = 0;
  UINT8                             MemBootHealthFeatureKnob = 0;
  UINT8                             OpportunisticSelfRefreshKnob = 0;
  EFI_STATUS                        Status;
  struct SystemMemoryMapHob         *SystemMemoryMap;
   //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return;
  }


  if (FeaturePcdGet (PcdSsaBuiltInRmtFeatureSupported)) {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Enable SSA Rmt Setup question Visiblity\n"));
    SsaRmtKnob = BIOS_SSA_RMT_KNOB_ENABLE;
  } else {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Disable SSA Rmt Setup question Visiblity\n"));
    SsaRmtKnob = BIOS_SSA_RMT_KNOB_DISABLE;
  }

  Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF(SOCKET_MEMORY_CONFIGURATION,SsaBuiltInRmtKnobVisible), &SsaRmtKnob, sizeof(UINT8));
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,"\nSetMrcKnobVisiblity: SSA Rmt knob is Successfuly Changed\n"));
  } else {
    DEBUG ((EFI_D_INFO,"\nSetMrcKnobVisiblity: Failed to Change SSA Rmt knob!!!\n"));
  }

  if (FeaturePcdGet (PcdSsaLoaderMethodFeatureSupported)) {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Enable SSA Loader Setup question Visiblity\n"));
    SsaLoaderKnob = BIOS_SSA_LOADER_KNOB_ENABLE;
  } else {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Disable SSA Loader Setup question Visiblity\n"));
    SsaLoaderKnob = BIOS_SSA_LOADER_KNOB_DISABLE;
  }


  Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF(SOCKET_MEMORY_CONFIGURATION,SsaLoaderKnobVisible), &SsaLoaderKnob, sizeof(UINT8));
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,"\nSetMrcKnobVisiblity: SSA Loader knob is Successfuly Changed\n"));
  } else {
    DEBUG ((EFI_D_INFO,"\nSetMrcKnobVisiblity: Failed to Change SSA Loader knob!!!\n"));
  }

  if (FeaturePcdGet (PcdLegacyRmtFeatureSupported)) {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Enable Legacy RMT Setup question Visiblity\n"));
    LegacyRmtKnob = LEGACY_RMT_KNOB_VISIBLITY_ENABLE;
  } else {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Disable Legacy RMT Setup question Visiblity\n"));
    LegacyRmtKnob = LEGACY_RMT_KNOB_VISIBLITY_DISABLE;
  }

  Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF(SOCKET_MEMORY_CONFIGURATION, LegacyRmtKnobVisible), &LegacyRmtKnob, sizeof(UINT8));
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,"\nSetMrcKnobVisiblity: Legacy RMT knob is Successfuly Changed\n"));
  } else {
    DEBUG ((EFI_D_INFO,"\nSetMrcKnobVisiblity: Failed to Change Legacy RMT knob!!!\n"));
  }

  if (FeaturePcdGet (PcdMemBootHealthFeatureSupported)) {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Enable Memory Health Check question Visiblity\n"));
    MemBootHealthFeatureKnob = MEM_BOOT_HEALTH_ENABLE;
  } else {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Disable Memory Health Check question Visiblity\n"));
    MemBootHealthFeatureKnob = MEM_BOOT_HEALTH_DISABLE;
  }

  Status = SetOptionData (&gMemBootHealthGuid, OFFSET_OF(MEM_BOOT_HEALTH_CONFIG, MemBootHealthVisible), &MemBootHealthFeatureKnob, sizeof(UINT8));
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,"\nSetMrcKnobVisiblity:Memory Health Check knob is Successfuly Changed\n"));
  } else {
    DEBUG ((EFI_D_ERROR,"\nSetMrcKnobVisiblity: Failed to Change Memory Health Check knob!!!\n"));
  }

  if (FeaturePcdGet (PcdOpportunisticSelfRefreshSupported)) {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Enable Opportunistic Self refresh question Visiblity\n"));
    OpportunisticSelfRefreshKnob = OPP_SREF_ENABLE;
  } else {
    DEBUG ((DEBUG_INFO, "SetMrcKnobVisiblity: Disable Opportunistic Self refresh question Visiblity\n"));
    OpportunisticSelfRefreshKnob = OPP_SREF_DISABLE;
  }

  Status = SetOptionData (&gEfiSocketMemoryVariableGuid, OFFSET_OF(SOCKET_MEMORY_CONFIGURATION, OppSrefVisible), &OpportunisticSelfRefreshKnob, sizeof(UINT8));
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,"\nSetMrcKnobVisiblity: Opportunistic Self refresh knob is Successfuly Changed\n"));
  } else {
    DEBUG ((EFI_D_ERROR,"\nSetMrcKnobVisiblity: Failed to Change Opportunistic Self refresh knob!!!\n"));
  }

}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
VOID
UpdateMemType (
  VOID
)
{
  EFI_STATUS                              Status;
  EFI_SMBIOS_HANDLE                       SmbiosHandle;
  EFI_SMBIOS_PROTOCOL                    *Smbios;
  EFI_SMBIOS_TYPE                         SmbiosType;
  EFI_STRING                              AdvancedString;
  EFI_STRING                              MemoryTypeString;
  SMBIOS_TABLE_TYPE17                    *SmbiosType17Record;
  CHAR16                                  MemoryTypeChar;
  UINTN                                   i;

  // Get memory type from SMBIOS
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  ASSERT_EFI_ERROR (Status);

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
  Status = Smbios->GetNext (
                    Smbios,
                    &SmbiosHandle,
                    &SmbiosType,
                    (EFI_SMBIOS_TABLE_HEADER **) (&SmbiosType17Record),
                    NULL);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Can not find SMBIOS information.\n"));
    goto Done;
  }

  //DEBUG((EFI_D_INFO, "SmbiosType17Record->MemoryType:0x%x\n", SmbiosType17Record->MemoryType));

  switch (SmbiosType17Record->MemoryType) {
    case MemoryTypeDdr3:
      MemoryTypeChar = L'3';
      break;
    default:
      MemoryTypeChar = L'4';
      break;
  }

  //DEBUG((EFI_D_INFO, "Memory type is DDR%c\n", MemoryTypeChar));

  // Get string from Advanced menu.
  for (i = 0; i < sizeof (MemTypeStrIdTab) / sizeof (EFI_STRING_ID); ++i) {
    AdvancedString = HiiGetString (mSocketStringsHiiHandle, MemTypeStrIdTab [i], NULL);
    if (NULL == AdvancedString) {
      DEBUG((EFI_D_ERROR, "Can not found memory type string.\n"));
      continue;
    }

    // Search the DDR# string
    MemoryTypeString = StrStr (AdvancedString, MEMORY_TYPE_DDR_STR);
    if (MemoryTypeString != NULL) {
      MemoryTypeString [3] = MemoryTypeChar;
      HiiSetString(mSocketStringsHiiHandle, MemTypeStrIdTab [i], AdvancedString, NULL);
      //DEBUG((EFI_D_INFO, "Updated memory type string:%s. \n", AdvancedString));
    }

    FreePool (AdvancedString);
    AdvancedString = NULL;
  }
Done:
  return;
}

/**

  CreateStringForInstalledDimm:
  Takes the input information and creates a string
  that is used to update the token value on a setup page.
  The HII Handle that is used is the global mSocketStringsHiiHandle.

  @param[in]     Socket          - Socket number
  @param[in]     Channel         - Channel number
  @param[in]     Dimm            - Dimm slot
  @param[in,out] DimmString1     - Final string value to be sent back to the caller, allocate/free by the caller
  @param[in]     DimmString2     - Used to build up the string value, allocate/free by the caller.
  @param[in]     SystemMemoryMap - System Memory Map to determine the topology.
  @param[in]     StrSize         - Size of the string buffer.

  @retval None

**/
VOID
CreateStringForInstalledDimm (
  IN      UINT8  Socket,
  IN      UINT8  Channel,
  IN      UINT8  Dimm,
  IN OUT  CHAR16 *DimmString1,
  IN      CHAR16 *DimmString2,
  IN      struct SystemMemoryMapHob *SystemMemoryMap,
  IN      UINTN  StrSize
  )
{
  UINT8  MfdIndex;
  UINT16 DimmSize;
  UINT8  MfdSize;

  MfdSize = sizeof (MemoryModuleManufacturerList) / sizeof (MemoryModuleManufacturerList[0]);

  for (MfdIndex = 0; MfdIndex < MfdSize; MfdIndex++) {
    if (MemoryModuleManufacturerList[MfdIndex].MfgId == SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].SPDMMfgId) {
      break;
    }
  }
  //
  // Initialize the string with Socket.Channel.Dimm Frequency and Manufacturer
  //
  if (MfdIndex < MfdSize) {
    UnicodeSPrint (DimmString1, StrSize, L"Socket%d.Ch%c.Dimm%d: %dMT/s %s ", GetSocketForSetupDisplay (Socket), 'A' + GetChannelForSetupDisplay (Socket, Channel), Dimm,
      SystemMemoryMap->memFreq, MemoryModuleManufacturerList[MfdIndex].String);
  } else {
    UnicodeSPrint (DimmString1, StrSize, L"Socket%d.Ch%c.Dimm%d: %dMT/s UNKNOWN ", GetSocketForSetupDisplay (Socket), 'A' + GetChannelForSetupDisplay (Socket, Channel), Dimm,
      SystemMemoryMap->memFreq);
  }

  //
  // Add number of ranks to the string
  //
  switch (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].NumRanks) {
    case SINGLE_RANK:
      UnicodeSPrint (DimmString2, StrSize, L"SR");
      break;
    case DUAL_RANK:
      UnicodeSPrint (DimmString2, StrSize, L"DR");
      break;
    case QUAD_RANK:
      UnicodeSPrint (DimmString2, StrSize, L"QR");
      break;
    case RANK_8R:
      UnicodeSPrint (DimmString2, StrSize, L"8R");
      break;
  }
  StrCatS (DimmString1, StrSize / sizeof (CHAR16), DimmString2);

  //
  // Add the Device Width to the string
  //
  if (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].Features & X4_PRESENT) {
    UnicodeSPrint (DimmString2, StrSize, L"x4 ");
  } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].Features & X8_PRESENT) {
    UnicodeSPrint (DimmString2, StrSize, L"x8 ");
  } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].Features & X16_PRESENT) {
    UnicodeSPrint (DimmString2, StrSize, L"x16 ");
  }
  StrCatS (DimmString1, StrSize / sizeof (CHAR16), DimmString2);

  //
  // Add Dimm Size to the string
  //
  DimmSize = SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].DimmSize >> 4;
  UnicodeSPrint (DimmString2, StrSize, L"%dGB", DimmSize);
  StrCatS (DimmString1, StrSize / sizeof (CHAR16), DimmString2);

  //
  // Add the DIMM Type to the string
  //
  if ((SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].keyByte == SPD_TYPE_DDR4) ||
      (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].keyByte == SPD_TYPE_DDR5)) {
    switch (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].actKeyByte2) {
      case SPD_RDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" RDIMM");
        break;
      case SPD_UDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" UDIMM");
        break;
      case SPD_SODIMM:
        UnicodeSPrint (DimmString2, StrSize, L" SODIMM");
        break;
      case SPD_MINI_RDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" MINI-RDIMM");
        break;
      case SPD_MINI_UDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" MINI-UDIMM");
        break;
      case SPD_MINI_CDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" MINI-CDIMM");
        break;
      case SPD_ECC_SO_UDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" ECC-SO-UDIMM");
        break;
      case SPD_ECC_SO_RDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" ECC-SO-RDIMM");
        break;
      case SPD_ECC_SO_CDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" ECC-SO-CDIMM");
        break;
      case SPD_LRDIMM_DDR4:
        UnicodeSPrint (DimmString2, StrSize, L" LRDIMM-DDR4");
        break;
      case SPD_LRDIMM:
        UnicodeSPrint (DimmString2, StrSize, L" LRDIMM");
        break;
      case SPD_NVM_DIMM:
        UnicodeSPrint (DimmString2, StrSize, L" NVDIMM");
        break;
      case SPD_UDIMM_ECC:
        UnicodeSPrint (DimmString2, StrSize, L" ECC-UDIMM");
        break;
      default:
        UnicodeSPrint (DimmString2, StrSize, L"N/A");
        break;
    }
  } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].keyByte == SPD_TYPE_AEP) {
    switch (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].DeviceID) {
    case FNV:
      UnicodeSPrint (DimmString2, StrSize, L" AEP-DIMM");
      break;
    case BWV:
      UnicodeSPrint (DimmString2, StrSize, L" BPS-DIMM");
      break;
    case CWV:
      UnicodeSPrint (DimmString2, StrSize, L" CPS-DIMM");
      break;
    default:
      UnicodeSPrint (DimmString2, StrSize, L" DCPMM-DIMM");
      break;
    }
  }
  StrCatS (DimmString1, StrSize / sizeof (CHAR16), DimmString2);
}

/**

  UpdateTokenValue: Takes the input information and creates a string
                    that is used to update the token value on a setup page.
                    The HII Handle that is used is the global mSocketStringsHiiHandle.

  @param[in] String            - string value to be written out to the setup page
  @param[in] StartOpCodeHandle - Start OP Code handle used to find where to write the string

  @retval None

**/
VOID
UpdateTokenValue (
  IN CHAR16              *String,
  IN VOID                *StartOpCodeHandle
  )
{
  EFI_STRING_ID   TokenToUpdate;

  TokenToUpdate = 0;
  TokenToUpdate = HiiSetString (
    mSocketStringsHiiHandle,
    0,
    String,
    NULL
  );

// APTIOV_SERVER_OVERRIDE_RC_END : Truncated DIMM information in setup.
#if 0
  HiiCreateTextOpCode (
    StartOpCodeHandle,
    TokenToUpdate,
    STRING_TOKEN (STR_NULL_STRING),
    STRING_TOKEN (STR_NULL_STRING)
  );
#endif

  HiiCreateSubTitleOpCode (
    StartOpCodeHandle,
    TokenToUpdate,
    0,
    0,
    0
    );
// APTIOV_SERVER_OVERRIDE_RC_END : Truncated DIMM information in setup.
}

/*
  Function to map Sockets specific to SOC
  Cases when a dual die processor is populated, we need a proper mapping
  so that CPU/memory topology shows information properly, so that
  customers are not confused. Currently used to display CPU/memory topology
  in setup

  @param[in] Socket - 0 based socket number

  @retval New Mapped Socket value

*/
UINT8
GetSocketForSetupDisplay (
  IN UINT8 Socket
  )
{
  UINT8 NewSocket = Socket;

  if (Socket < MAX_SOCKET) {
    if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      if (PcuGetDieCount (Socket) == TWO_DIE_IN_SOCKET) {
        NewSocket = Socket / 2; //Socket 0, 1 => 0 and socket 2, 3 => 1
      }
    }
  }
  return NewSocket;
}

//Fused Channels in each internal socket
#define CPX4_FUSED_CHANNELS_IN_DIE 2

/*
  CPX4 specific function
  In dual die sockets, Map the channel continuity so that
  Memory Topology is shown properly

  @param[in] Socket - 0 based socket number
  @param[in] Ch     - 0 based channel number (0 to maximum channel per socket)

  @retval New Channel number for continous display

*/
UINT8
GetChannelForSetupDisplay (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{

  UINT8 DisplayChannel = 0;
  UINT8 MaxAvailableChannel = 0;
  UINT8 InternalSocketNumber = 0;

// GetSocSpecificChannel (Socket, Ch) => 0, 1, 2, 3
  DisplayChannel = GetSocSpecificChannel (Socket, Ch);

  if (Ch < MAX_CH) {
    if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      if (PcuGetDieCount (Socket) == TWO_DIE_IN_SOCKET) {

        // Socket % 2 => Socket = 0 to 3 -> Result = 0, 1, 0, 1
        InternalSocketNumber = (Socket % 2);

        // (MAX_CH - CPX4_FUSED_CHANNELS_IN_DIE) => (6 - 2 ) = 4 Max Channels per internal Die
        MaxAvailableChannel = (MAX_CH - CPX4_FUSED_CHANNELS_IN_DIE);

        DisplayChannel = (DisplayChannel + (InternalSocketNumber * MaxAvailableChannel));
      } //if (PcuGetDieCount (Socket) == TWO_DIE_IN_SOCKET)
    } //if (IsCpuAndRevision (CPU_CPX, REV_ALL))
  } //if (Ch < MAX_CH)

  return DisplayChannel;
}

/*
  Function to map channels specific to SOC
  Cases when a channel is fused, we need a proper mapping
  so that memory topology shows information properly, so that
  customers are not confused.

  @param[in] Socket - 0 based socket number
  @param[in] Ch     - 0 based channel number (0 to maximum channel per socket)

  @retval New Mapped Channel value

*/
UINT8
GetSocSpecificChannel (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{

  UINT8 NewChannel = Ch;

  if (Ch < MAX_CH) {
    if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      if (PcuGetDieCount (Socket) == TWO_DIE_IN_SOCKET) {
        /*
        Channel mapping for Memory Topology
          Old    => New
         Channel => Channel
            0    => 0
            1    => 1
            2    => disconnected
            3    => 2
            4    => 3
            5    => disconnected
        */
        //For CPX Channel 2 and 5 are disconnected
        if (!(Ch == 2 || Ch == 5)) {
          if (Ch > 1) {
            NewChannel--;
          }  //if (Ch > 1)
        }  //if (!(Ch == 2 || Ch == 5))
      }  //if (PcuGetDieCount (Socket) == TWO_DIE_IN_SOCKET)
    }  //if (IsCpuAndRevision (CPU_CPX, REV_ALL))
  }  //if (Ch < MAX_CH)

  return NewChannel;
}

/**

  This is the function for displaying memory topology in Setup.

  @param VOID.

  @retval VOID.

**/
VOID
DisplayMemTopology (
  VOID
  )
{
  struct SystemMemoryMapHob *SystemMemoryMap;
  EFI_STATUS                Status;
  UINT8                     Socket;
  UINT8                     Channel;
  UINT8                     Dimm;
  UINTN                     StrSize;
  UINT8                     Cntr;
  CHAR16                    *DimmString1;
  CHAR16                    *DimmString2;
  VOID                      *HobList;
  EFI_IFR_GUID_LABEL        *StartLabel;
  EFI_IFR_GUID_LABEL        *EndLabel;
  VOID                      *StartOpCodeHandle;
  VOID                      *EndOpCodeHandle;
  EFI_GUID                  FormSetGuid = SOCKET_FORMSET_GUID;

//
// Initialize the container for dynamic opcodes
//
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (EndOpCodeHandle != NULL);

  //
  // If we failed to allocate handle, skip creating the opcodes
  //
  if ((StartOpCodeHandle == NULL) || (EndOpCodeHandle == NULL)) {
    if (StartOpCodeHandle != NULL) {
      HiiFreeOpCodeHandle (StartOpCodeHandle);
    }
    if (EndOpCodeHandle != NULL) {
      HiiFreeOpCodeHandle (EndOpCodeHandle);
    }
    return;
  }

  //
  // Create Hii Extend Label OpCode as the start opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number = MEM_TOPOLOGY_START_LABEL;

  //
  // Create Hii Extend Label OpCode as the end opcode
  //
  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number = MEM_TOPOLOGY_END_LABEL;

  //
  // Search for the Memory Map GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);


   SystemMemoryMap = GetSystemMemoryMapData ();
   if (SystemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return;
  }

  StrSize = MEM_TOPOLOGY_STRING_SIZE;
  DimmString1 = AllocateZeroPool (StrSize);
  Cntr = 0;

  if (DimmString1 == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  DimmString2 = AllocateZeroPool (StrSize);
  if (DimmString2 == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    gBS->FreePool (DimmString1);
    return;
  }

  //
  // Locate installed Dimm Modules and create a string to be displayed in setup.
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].Enabled == 1
              && SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].Present == 1 ) {
          CreateStringForInstalledDimm (Socket, Channel, Dimm, DimmString1, DimmString2, SystemMemoryMap, StrSize);
          UpdateTokenValue (DimmString1, StartOpCodeHandle);
        }

        if (SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].Enabled
            && SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].Present) {
            Cntr++;
        }
      } // dimm loop
    } // Channel loop
  } // Socket loop

  //
  // Update Memory Topology info page form
  //
  HiiUpdateForm (
    mSocketStringsHiiHandle,         // HII handle
    &FormSetGuid,                    // Formset GUID
    VFR_FORMID_MEMTOPOLOGY_DISPLAY,  // Form ID
    StartOpCodeHandle,               // Label for where to insert opcodes
    EndOpCodeHandle                  // Replace data
  );

  gBS->FreePool (DimmString1);
  gBS->FreePool (DimmString2);
}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
VOID
UpdateMemoryInformation (
  VOID
  )
{
  UpdateMemType();
  DisplayMemTopology();
  SetMrcKnobVisiblity ();
}

/**

  This function configures KTI settings in Setup.

  @param none

  @retval none

**/
VOID
UpdateKtiStatusLines (
  VOID
  )
{
  CHAR16                *NewString;
  STRING_REF            TokenToUpdate;
  UINTN                 StrSize;
  BOOLEAN               IsSprCpu = FALSE;

  StrSize   = 0x250;
  NewString = AllocateZeroPool (StrSize);

  if (NewString == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    IsSprCpu = TRUE;
  }

  //
  // Number of CPU
  //
  TokenToUpdate = (STRING_REF) STR_KTI_NUMBER_OF_CPU_VAL;
  UnicodeSPrint (NewString,StrSize,L"%d",(UINTN)mIioUds->IioUdsPtr->SystemStatus.numCpus);
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Number of IOH
  //
  TokenToUpdate = (STRING_REF) STR_KTI_NUMBER_OF_IIO_VAL;
  UnicodeSPrint (NewString,StrSize,L"%d",(UINTN)mIioUds->IioUdsPtr->PlatformData.numofIIO);
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Current MP Link Frequency, need to show up according to Cpu Type.
  // SKX: only support 0: 9.6 GT, 1: 10.4 GT
  // ICX: supports 0: 9.6 GT, 1: 10.4 GT and 2: 11.2 GT
  // SPR: supports 0: 12.8 GT, 1: 14.4 GT and 2: 16.0 GT
  //
  switch (mIioUds->IioUdsPtr->SystemStatus.CurrentUpiLinkFrequency) {
  case 0:
    if (!IsSprCpu) {
      StrCpyS (NewString, StrSize / sizeof (CHAR16), L"9.6 GT/s");
    } else {
      StrCpyS (NewString, StrSize / sizeof (CHAR16), L"12.8 GT/s");
    }
    break;

  case 1:
    if (!IsSprCpu) {
      StrCpyS (NewString, StrSize / sizeof (CHAR16), L"10.4 GT/s");
    } else {
      StrCpyS (NewString, StrSize / sizeof (CHAR16), L"14.4 GT/s");
    }
    break;

  case 2:
    if (!IsSprCpu) {
      StrCpyS (NewString, StrSize / sizeof (CHAR16), L"11.2 GT/s");
    } else {
      StrCpyS (NewString, StrSize / sizeof (CHAR16), L"16.0 GT/s");
    }
    break;

  case 4:
    StrCpyS (NewString, StrSize / sizeof (CHAR16), L"Per Link Speed is used");
    break;

  default:
    StrCpyS (NewString, StrSize / sizeof (CHAR16), L"Unknown or 1S configuration");
    break;
  }

  //
  // If Link Speed is set to slow and multiple CPUs are present,
  // The link frequency should be 50 MT/s for SKX and ICX; The link frequency should be 2.5 GT/s for SPR;
  //
  if((!mIioUds->IioUdsPtr->SystemStatus.CurrentUpiiLinkSpeed) && (mIioUds->IioUdsPtr->SystemStatus.numCpus > 1)){
    if (!IsSprCpu) {
      StrCpyS (NewString, StrSize/sizeof(CHAR16), L"50 MT/s");
    } else {
      StrCpyS (NewString, StrSize / sizeof (CHAR16), L"2.5 GT/s");
    }
  }

  TokenToUpdate = (STRING_REF) STR_KTI_LINK_FREQ_STATUS;
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Current MP Link Speed
  //
  if (mIioUds->IioUdsPtr->SystemStatus.CurrentUpiiLinkSpeed) {
    StrCpyS (NewString, StrSize/sizeof(CHAR16), L"Fast");
  }
  else
      StrCpyS (NewString, StrSize/sizeof(CHAR16), L"Slow or 1S Configuration");

  TokenToUpdate = (STRING_REF) STR_KTI_LINK_SPEED_STATUS;
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);
  //
  // Global MMIO Low Base / Limit
  //
  TokenToUpdate = (STRING_REF) STR_KTI_GMMIOL_BASE_LIMIT_VAL;
  UnicodeSPrint (NewString,StrSize,L"%08x / %08x", (UINTN)mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Base, (UINTN)mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Limit);
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Global MMIO High Base / Limit
  //
  TokenToUpdate = (STRING_REF) STR_KTI_GMMIOH_BASE_LIMIT_VAL;
  UnicodeSPrint (NewString,StrSize,L"%016x / %016x", mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Base, mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Limit);
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Pci-e Configuration Base
  //
  TokenToUpdate = (STRING_REF) STR_KTI_PCI_E_CFG_BASE_VAL;
  UnicodeSPrint (NewString,StrSize,L"%08x / %08x", (UINTN)mIioUds->IioUdsPtr->PlatformData.PciExpressBase, (UINTN)mIioUds->IioUdsPtr->PlatformData.PciExpressSize);
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  gBS->FreePool (NewString);
  return ;
}


/**

  This function configures KTI settings in Setup.

  @param none

  @retval none

**/
VOID
UpdateKtiResourceAllocationLines (
  VOID
  )
{
  return ;
}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
VOID
UpdateMpLinkInformation (
  VOID
  )
{
  UpdateKtiStatusLines();
  UpdateKtiResourceAllocationLines();
  return;
}


typedef struct {
  STRING_REF LinkStatusToken;
  STRING_REF MaxLinkStatusToken;
  STRING_REF PcieSpeedStatusToken;
} PCIE_STRING_TOKENS;

#ifdef SPR_HOST
STATIC PCIE_STRING_TOKENS PcieStringTokens[] = {
  //
  // Socket 0
  //
  { STR_LINK_STATUS_IIO0PORT0, STR_MAXLINK_STATUS_IIO0PORT0, STR_PCIESPEED_STATUS_IIO0PORT0 },
  { STR_LINK_STATUS_IIO0PORT1, STR_MAXLINK_STATUS_IIO0PORT1, STR_PCIESPEED_STATUS_IIO0PORT1 },
  { STR_LINK_STATUS_IIO0PORT2, STR_MAXLINK_STATUS_IIO0PORT2, STR_PCIESPEED_STATUS_IIO0PORT2 },
  { STR_LINK_STATUS_IIO0PORT3, STR_MAXLINK_STATUS_IIO0PORT3, STR_PCIESPEED_STATUS_IIO0PORT3 },
  { STR_LINK_STATUS_IIO0PORT4, STR_MAXLINK_STATUS_IIO0PORT4, STR_PCIESPEED_STATUS_IIO0PORT4 },
  { STR_LINK_STATUS_IIO0PORT5, STR_MAXLINK_STATUS_IIO0PORT5, STR_PCIESPEED_STATUS_IIO0PORT5 },
  { STR_LINK_STATUS_IIO0PORT6, STR_MAXLINK_STATUS_IIO0PORT6, STR_PCIESPEED_STATUS_IIO0PORT6 },
  { STR_LINK_STATUS_IIO0PORT7, STR_MAXLINK_STATUS_IIO0PORT7, STR_PCIESPEED_STATUS_IIO0PORT7 },
  { STR_LINK_STATUS_IIO0PORT8, STR_MAXLINK_STATUS_IIO0PORT8, STR_PCIESPEED_STATUS_IIO0PORT8 },
  { STR_LINK_STATUS_IIO0PORT9, STR_MAXLINK_STATUS_IIO0PORT9, STR_PCIESPEED_STATUS_IIO0PORT9 },
  { STR_LINK_STATUS_IIO0PORT10, STR_MAXLINK_STATUS_IIO0PORT10, STR_PCIESPEED_STATUS_IIO0PORT10 },
  { STR_LINK_STATUS_IIO0PORT11, STR_MAXLINK_STATUS_IIO0PORT11, STR_PCIESPEED_STATUS_IIO0PORT11 },
  { STR_LINK_STATUS_IIO0PORT12, STR_MAXLINK_STATUS_IIO0PORT12, STR_PCIESPEED_STATUS_IIO0PORT12 },
  { STR_LINK_STATUS_IIO0PORT13, STR_MAXLINK_STATUS_IIO0PORT13, STR_PCIESPEED_STATUS_IIO0PORT13 },
  { STR_LINK_STATUS_IIO0PORT14, STR_MAXLINK_STATUS_IIO0PORT14, STR_PCIESPEED_STATUS_IIO0PORT14 },
  { STR_LINK_STATUS_IIO0PORT15, STR_MAXLINK_STATUS_IIO0PORT15, STR_PCIESPEED_STATUS_IIO0PORT15 },
  { STR_LINK_STATUS_IIO0PORT16, STR_MAXLINK_STATUS_IIO0PORT16, STR_PCIESPEED_STATUS_IIO0PORT16 },
  { STR_LINK_STATUS_IIO0PORT17, STR_MAXLINK_STATUS_IIO0PORT17, STR_PCIESPEED_STATUS_IIO0PORT17 },
  { STR_LINK_STATUS_IIO0PORT18, STR_MAXLINK_STATUS_IIO0PORT18, STR_PCIESPEED_STATUS_IIO0PORT18 },
  { STR_LINK_STATUS_IIO0PORT19, STR_MAXLINK_STATUS_IIO0PORT19, STR_PCIESPEED_STATUS_IIO0PORT19 },
  { STR_LINK_STATUS_IIO0PORT20, STR_MAXLINK_STATUS_IIO0PORT20, STR_PCIESPEED_STATUS_IIO0PORT20 },
  { STR_LINK_STATUS_IIO0PORT21, STR_MAXLINK_STATUS_IIO0PORT21, STR_PCIESPEED_STATUS_IIO0PORT21 },
  { STR_LINK_STATUS_IIO0PORT22, STR_MAXLINK_STATUS_IIO0PORT22, STR_PCIESPEED_STATUS_IIO0PORT22 },
  { STR_LINK_STATUS_IIO0PORT23, STR_MAXLINK_STATUS_IIO0PORT23, STR_PCIESPEED_STATUS_IIO0PORT23 },
  { STR_LINK_STATUS_IIO0PORT24, STR_MAXLINK_STATUS_IIO0PORT24, STR_PCIESPEED_STATUS_IIO0PORT24 },
  { STR_LINK_STATUS_IIO0PORT25, STR_MAXLINK_STATUS_IIO0PORT25, STR_PCIESPEED_STATUS_IIO0PORT25 },
  { STR_LINK_STATUS_IIO0PORT26, STR_MAXLINK_STATUS_IIO0PORT26, STR_PCIESPEED_STATUS_IIO0PORT26 },
  { STR_LINK_STATUS_IIO0PORT27, STR_MAXLINK_STATUS_IIO0PORT27, STR_PCIESPEED_STATUS_IIO0PORT27 },
  { STR_LINK_STATUS_IIO0PORT28, STR_MAXLINK_STATUS_IIO0PORT28, STR_PCIESPEED_STATUS_IIO0PORT28 },
  { STR_LINK_STATUS_IIO0PORT29, STR_MAXLINK_STATUS_IIO0PORT29, STR_PCIESPEED_STATUS_IIO0PORT29 },
  { STR_LINK_STATUS_IIO0PORT30, STR_MAXLINK_STATUS_IIO0PORT30, STR_PCIESPEED_STATUS_IIO0PORT30 },
  { STR_LINK_STATUS_IIO0PORT31, STR_MAXLINK_STATUS_IIO0PORT31, STR_PCIESPEED_STATUS_IIO0PORT31 },
  { STR_LINK_STATUS_IIO0PORT32, STR_MAXLINK_STATUS_IIO0PORT32, STR_PCIESPEED_STATUS_IIO0PORT32 },
  { STR_LINK_STATUS_IIO0PORT33, STR_MAXLINK_STATUS_IIO0PORT33, STR_PCIESPEED_STATUS_IIO0PORT33 },
  { STR_LINK_STATUS_IIO0PORT34, STR_MAXLINK_STATUS_IIO0PORT34, STR_PCIESPEED_STATUS_IIO0PORT34 },
  { STR_LINK_STATUS_IIO0PORT35, STR_MAXLINK_STATUS_IIO0PORT35, STR_PCIESPEED_STATUS_IIO0PORT35 },
  { STR_LINK_STATUS_IIO0PORT36, STR_MAXLINK_STATUS_IIO0PORT36, STR_PCIESPEED_STATUS_IIO0PORT36 },
  { STR_LINK_STATUS_IIO0PORT37, STR_MAXLINK_STATUS_IIO0PORT37, STR_PCIESPEED_STATUS_IIO0PORT37 },
  { STR_LINK_STATUS_IIO0PORT38, STR_MAXLINK_STATUS_IIO0PORT38, STR_PCIESPEED_STATUS_IIO0PORT38 },
  { STR_LINK_STATUS_IIO0PORT39, STR_MAXLINK_STATUS_IIO0PORT39, STR_PCIESPEED_STATUS_IIO0PORT39 },
  { STR_LINK_STATUS_IIO0PORT40, STR_MAXLINK_STATUS_IIO0PORT40, STR_PCIESPEED_STATUS_IIO0PORT40 },
  //
  // Socket 1
  //
  { STR_LINK_STATUS_IIO1PORT0, STR_MAXLINK_STATUS_IIO1PORT0, STR_PCIESPEED_STATUS_IIO1PORT0 },
  { STR_LINK_STATUS_IIO1PORT1, STR_MAXLINK_STATUS_IIO1PORT1, STR_PCIESPEED_STATUS_IIO1PORT1 },
  { STR_LINK_STATUS_IIO1PORT2, STR_MAXLINK_STATUS_IIO1PORT2, STR_PCIESPEED_STATUS_IIO1PORT2 },
  { STR_LINK_STATUS_IIO1PORT3, STR_MAXLINK_STATUS_IIO1PORT3, STR_PCIESPEED_STATUS_IIO1PORT3 },
  { STR_LINK_STATUS_IIO1PORT4, STR_MAXLINK_STATUS_IIO1PORT4, STR_PCIESPEED_STATUS_IIO1PORT4 },
  { STR_LINK_STATUS_IIO1PORT5, STR_MAXLINK_STATUS_IIO1PORT5, STR_PCIESPEED_STATUS_IIO1PORT5 },
  { STR_LINK_STATUS_IIO1PORT6, STR_MAXLINK_STATUS_IIO1PORT6, STR_PCIESPEED_STATUS_IIO1PORT6 },
  { STR_LINK_STATUS_IIO1PORT7, STR_MAXLINK_STATUS_IIO1PORT7, STR_PCIESPEED_STATUS_IIO1PORT7 },
  { STR_LINK_STATUS_IIO1PORT8, STR_MAXLINK_STATUS_IIO1PORT8, STR_PCIESPEED_STATUS_IIO1PORT8 },
  { STR_LINK_STATUS_IIO1PORT9, STR_MAXLINK_STATUS_IIO1PORT9, STR_PCIESPEED_STATUS_IIO1PORT9 },
  { STR_LINK_STATUS_IIO1PORT10, STR_MAXLINK_STATUS_IIO1PORT10, STR_PCIESPEED_STATUS_IIO1PORT10 },
  { STR_LINK_STATUS_IIO1PORT11, STR_MAXLINK_STATUS_IIO1PORT11, STR_PCIESPEED_STATUS_IIO1PORT11 },
  { STR_LINK_STATUS_IIO1PORT12, STR_MAXLINK_STATUS_IIO1PORT12, STR_PCIESPEED_STATUS_IIO1PORT12 },
  { STR_LINK_STATUS_IIO1PORT13, STR_MAXLINK_STATUS_IIO1PORT13, STR_PCIESPEED_STATUS_IIO1PORT13 },
  { STR_LINK_STATUS_IIO1PORT14, STR_MAXLINK_STATUS_IIO1PORT14, STR_PCIESPEED_STATUS_IIO1PORT14 },
  { STR_LINK_STATUS_IIO1PORT15, STR_MAXLINK_STATUS_IIO1PORT15, STR_PCIESPEED_STATUS_IIO1PORT15 },
  { STR_LINK_STATUS_IIO1PORT16, STR_MAXLINK_STATUS_IIO1PORT16, STR_PCIESPEED_STATUS_IIO1PORT16 },
  { STR_LINK_STATUS_IIO1PORT17, STR_MAXLINK_STATUS_IIO1PORT17, STR_PCIESPEED_STATUS_IIO1PORT17 },
  { STR_LINK_STATUS_IIO1PORT18, STR_MAXLINK_STATUS_IIO1PORT18, STR_PCIESPEED_STATUS_IIO1PORT18 },
  { STR_LINK_STATUS_IIO1PORT19, STR_MAXLINK_STATUS_IIO1PORT19, STR_PCIESPEED_STATUS_IIO1PORT19 },
  { STR_LINK_STATUS_IIO1PORT20, STR_MAXLINK_STATUS_IIO1PORT20, STR_PCIESPEED_STATUS_IIO1PORT20 },
  { STR_LINK_STATUS_IIO1PORT21, STR_MAXLINK_STATUS_IIO1PORT21, STR_PCIESPEED_STATUS_IIO1PORT21 },
  { STR_LINK_STATUS_IIO1PORT22, STR_MAXLINK_STATUS_IIO1PORT22, STR_PCIESPEED_STATUS_IIO1PORT22 },
  { STR_LINK_STATUS_IIO1PORT23, STR_MAXLINK_STATUS_IIO1PORT23, STR_PCIESPEED_STATUS_IIO1PORT23 },
  { STR_LINK_STATUS_IIO1PORT24, STR_MAXLINK_STATUS_IIO1PORT24, STR_PCIESPEED_STATUS_IIO1PORT24 },
  { STR_LINK_STATUS_IIO1PORT25, STR_MAXLINK_STATUS_IIO1PORT25, STR_PCIESPEED_STATUS_IIO1PORT25 },
  { STR_LINK_STATUS_IIO1PORT26, STR_MAXLINK_STATUS_IIO1PORT26, STR_PCIESPEED_STATUS_IIO1PORT26 },
  { STR_LINK_STATUS_IIO1PORT27, STR_MAXLINK_STATUS_IIO1PORT27, STR_PCIESPEED_STATUS_IIO1PORT27 },
  { STR_LINK_STATUS_IIO1PORT28, STR_MAXLINK_STATUS_IIO1PORT28, STR_PCIESPEED_STATUS_IIO1PORT28 },
  { STR_LINK_STATUS_IIO1PORT29, STR_MAXLINK_STATUS_IIO1PORT29, STR_PCIESPEED_STATUS_IIO1PORT29 },
  { STR_LINK_STATUS_IIO1PORT30, STR_MAXLINK_STATUS_IIO1PORT30, STR_PCIESPEED_STATUS_IIO1PORT30 },
  { STR_LINK_STATUS_IIO1PORT31, STR_MAXLINK_STATUS_IIO1PORT31, STR_PCIESPEED_STATUS_IIO1PORT31 },
  { STR_LINK_STATUS_IIO1PORT32, STR_MAXLINK_STATUS_IIO1PORT32, STR_PCIESPEED_STATUS_IIO1PORT32 },
  { STR_LINK_STATUS_IIO1PORT33, STR_MAXLINK_STATUS_IIO1PORT33, STR_PCIESPEED_STATUS_IIO1PORT33 },
  { STR_LINK_STATUS_IIO1PORT34, STR_MAXLINK_STATUS_IIO1PORT34, STR_PCIESPEED_STATUS_IIO1PORT34 },
  { STR_LINK_STATUS_IIO1PORT35, STR_MAXLINK_STATUS_IIO1PORT35, STR_PCIESPEED_STATUS_IIO1PORT35 },
  { STR_LINK_STATUS_IIO1PORT36, STR_MAXLINK_STATUS_IIO1PORT36, STR_PCIESPEED_STATUS_IIO1PORT36 },
  { STR_LINK_STATUS_IIO1PORT37, STR_MAXLINK_STATUS_IIO1PORT37, STR_PCIESPEED_STATUS_IIO1PORT37 },
  { STR_LINK_STATUS_IIO1PORT38, STR_MAXLINK_STATUS_IIO1PORT38, STR_PCIESPEED_STATUS_IIO1PORT38 },
  { STR_LINK_STATUS_IIO1PORT39, STR_MAXLINK_STATUS_IIO1PORT39, STR_PCIESPEED_STATUS_IIO1PORT39 },
  { STR_LINK_STATUS_IIO1PORT40, STR_MAXLINK_STATUS_IIO1PORT40, STR_PCIESPEED_STATUS_IIO1PORT40 },
  //
  // Socket 2
  //
  { STR_LINK_STATUS_IIO2PORT0, STR_MAXLINK_STATUS_IIO2PORT0, STR_PCIESPEED_STATUS_IIO2PORT0 },
  { STR_LINK_STATUS_IIO2PORT1, STR_MAXLINK_STATUS_IIO2PORT1, STR_PCIESPEED_STATUS_IIO2PORT1 },
  { STR_LINK_STATUS_IIO2PORT2, STR_MAXLINK_STATUS_IIO2PORT2, STR_PCIESPEED_STATUS_IIO2PORT2 },
  { STR_LINK_STATUS_IIO2PORT3, STR_MAXLINK_STATUS_IIO2PORT3, STR_PCIESPEED_STATUS_IIO2PORT3 },
  { STR_LINK_STATUS_IIO2PORT4, STR_MAXLINK_STATUS_IIO2PORT4, STR_PCIESPEED_STATUS_IIO2PORT4 },
  { STR_LINK_STATUS_IIO2PORT5, STR_MAXLINK_STATUS_IIO2PORT5, STR_PCIESPEED_STATUS_IIO2PORT5 },
  { STR_LINK_STATUS_IIO2PORT6, STR_MAXLINK_STATUS_IIO2PORT6, STR_PCIESPEED_STATUS_IIO2PORT6 },
  { STR_LINK_STATUS_IIO2PORT7, STR_MAXLINK_STATUS_IIO2PORT7, STR_PCIESPEED_STATUS_IIO2PORT7 },
  { STR_LINK_STATUS_IIO2PORT8, STR_MAXLINK_STATUS_IIO2PORT8, STR_PCIESPEED_STATUS_IIO2PORT8 },
  { STR_LINK_STATUS_IIO2PORT9, STR_MAXLINK_STATUS_IIO2PORT9, STR_PCIESPEED_STATUS_IIO2PORT9 },
  { STR_LINK_STATUS_IIO2PORT10, STR_MAXLINK_STATUS_IIO2PORT10, STR_PCIESPEED_STATUS_IIO2PORT10 },
  { STR_LINK_STATUS_IIO2PORT11, STR_MAXLINK_STATUS_IIO2PORT11, STR_PCIESPEED_STATUS_IIO2PORT11 },
  { STR_LINK_STATUS_IIO2PORT12, STR_MAXLINK_STATUS_IIO2PORT12, STR_PCIESPEED_STATUS_IIO2PORT12 },
  { STR_LINK_STATUS_IIO2PORT13, STR_MAXLINK_STATUS_IIO2PORT13, STR_PCIESPEED_STATUS_IIO2PORT13 },
  { STR_LINK_STATUS_IIO2PORT14, STR_MAXLINK_STATUS_IIO2PORT14, STR_PCIESPEED_STATUS_IIO2PORT14 },
  { STR_LINK_STATUS_IIO2PORT15, STR_MAXLINK_STATUS_IIO2PORT15, STR_PCIESPEED_STATUS_IIO2PORT15 },
  { STR_LINK_STATUS_IIO2PORT16, STR_MAXLINK_STATUS_IIO2PORT16, STR_PCIESPEED_STATUS_IIO2PORT16 },
  { STR_LINK_STATUS_IIO2PORT17, STR_MAXLINK_STATUS_IIO2PORT17, STR_PCIESPEED_STATUS_IIO2PORT17 },
  { STR_LINK_STATUS_IIO2PORT18, STR_MAXLINK_STATUS_IIO2PORT18, STR_PCIESPEED_STATUS_IIO2PORT18 },
  { STR_LINK_STATUS_IIO2PORT19, STR_MAXLINK_STATUS_IIO2PORT19, STR_PCIESPEED_STATUS_IIO2PORT19 },
  { STR_LINK_STATUS_IIO2PORT20, STR_MAXLINK_STATUS_IIO2PORT20, STR_PCIESPEED_STATUS_IIO2PORT20 },
  { STR_LINK_STATUS_IIO2PORT21, STR_MAXLINK_STATUS_IIO2PORT21, STR_PCIESPEED_STATUS_IIO2PORT21 },
  { STR_LINK_STATUS_IIO2PORT22, STR_MAXLINK_STATUS_IIO2PORT22, STR_PCIESPEED_STATUS_IIO2PORT22 },
  { STR_LINK_STATUS_IIO2PORT23, STR_MAXLINK_STATUS_IIO2PORT23, STR_PCIESPEED_STATUS_IIO2PORT23 },
  { STR_LINK_STATUS_IIO2PORT24, STR_MAXLINK_STATUS_IIO2PORT24, STR_PCIESPEED_STATUS_IIO2PORT24 },
  { STR_LINK_STATUS_IIO2PORT25, STR_MAXLINK_STATUS_IIO2PORT25, STR_PCIESPEED_STATUS_IIO2PORT25 },
  { STR_LINK_STATUS_IIO2PORT26, STR_MAXLINK_STATUS_IIO2PORT26, STR_PCIESPEED_STATUS_IIO2PORT26 },
  { STR_LINK_STATUS_IIO2PORT27, STR_MAXLINK_STATUS_IIO2PORT27, STR_PCIESPEED_STATUS_IIO2PORT27 },
  { STR_LINK_STATUS_IIO2PORT28, STR_MAXLINK_STATUS_IIO2PORT28, STR_PCIESPEED_STATUS_IIO2PORT28 },
  { STR_LINK_STATUS_IIO2PORT29, STR_MAXLINK_STATUS_IIO2PORT29, STR_PCIESPEED_STATUS_IIO2PORT29 },
  { STR_LINK_STATUS_IIO2PORT30, STR_MAXLINK_STATUS_IIO2PORT30, STR_PCIESPEED_STATUS_IIO2PORT30 },
  { STR_LINK_STATUS_IIO2PORT31, STR_MAXLINK_STATUS_IIO2PORT31, STR_PCIESPEED_STATUS_IIO2PORT31 },
  { STR_LINK_STATUS_IIO2PORT32, STR_MAXLINK_STATUS_IIO2PORT32, STR_PCIESPEED_STATUS_IIO2PORT32 },
  { STR_LINK_STATUS_IIO2PORT33, STR_MAXLINK_STATUS_IIO2PORT33, STR_PCIESPEED_STATUS_IIO2PORT33 },
  { STR_LINK_STATUS_IIO2PORT34, STR_MAXLINK_STATUS_IIO2PORT34, STR_PCIESPEED_STATUS_IIO2PORT34 },
  { STR_LINK_STATUS_IIO2PORT35, STR_MAXLINK_STATUS_IIO2PORT35, STR_PCIESPEED_STATUS_IIO2PORT35 },
  { STR_LINK_STATUS_IIO2PORT36, STR_MAXLINK_STATUS_IIO2PORT36, STR_PCIESPEED_STATUS_IIO2PORT36 },
  { STR_LINK_STATUS_IIO2PORT37, STR_MAXLINK_STATUS_IIO2PORT37, STR_PCIESPEED_STATUS_IIO2PORT37 },
  { STR_LINK_STATUS_IIO2PORT38, STR_MAXLINK_STATUS_IIO2PORT38, STR_PCIESPEED_STATUS_IIO2PORT38 },
  { STR_LINK_STATUS_IIO2PORT39, STR_MAXLINK_STATUS_IIO2PORT39, STR_PCIESPEED_STATUS_IIO2PORT39 },
  { STR_LINK_STATUS_IIO2PORT40, STR_MAXLINK_STATUS_IIO2PORT40, STR_PCIESPEED_STATUS_IIO2PORT40 },
  //
  // Socket 3
  //
  { STR_LINK_STATUS_IIO3PORT0, STR_MAXLINK_STATUS_IIO3PORT0, STR_PCIESPEED_STATUS_IIO3PORT0 },
  { STR_LINK_STATUS_IIO3PORT1, STR_MAXLINK_STATUS_IIO3PORT1, STR_PCIESPEED_STATUS_IIO3PORT1 },
  { STR_LINK_STATUS_IIO3PORT2, STR_MAXLINK_STATUS_IIO3PORT2, STR_PCIESPEED_STATUS_IIO3PORT2 },
  { STR_LINK_STATUS_IIO3PORT3, STR_MAXLINK_STATUS_IIO3PORT3, STR_PCIESPEED_STATUS_IIO3PORT3 },
  { STR_LINK_STATUS_IIO3PORT4, STR_MAXLINK_STATUS_IIO3PORT4, STR_PCIESPEED_STATUS_IIO3PORT4 },
  { STR_LINK_STATUS_IIO3PORT5, STR_MAXLINK_STATUS_IIO3PORT5, STR_PCIESPEED_STATUS_IIO3PORT5 },
  { STR_LINK_STATUS_IIO3PORT6, STR_MAXLINK_STATUS_IIO3PORT6, STR_PCIESPEED_STATUS_IIO3PORT6 },
  { STR_LINK_STATUS_IIO3PORT7, STR_MAXLINK_STATUS_IIO3PORT7, STR_PCIESPEED_STATUS_IIO3PORT7 },
  { STR_LINK_STATUS_IIO3PORT8, STR_MAXLINK_STATUS_IIO3PORT8, STR_PCIESPEED_STATUS_IIO3PORT8 },
  { STR_LINK_STATUS_IIO3PORT9, STR_MAXLINK_STATUS_IIO3PORT9, STR_PCIESPEED_STATUS_IIO3PORT9 },
  { STR_LINK_STATUS_IIO3PORT10, STR_MAXLINK_STATUS_IIO3PORT10, STR_PCIESPEED_STATUS_IIO3PORT10 },
  { STR_LINK_STATUS_IIO3PORT11, STR_MAXLINK_STATUS_IIO3PORT11, STR_PCIESPEED_STATUS_IIO3PORT11 },
  { STR_LINK_STATUS_IIO3PORT12, STR_MAXLINK_STATUS_IIO3PORT12, STR_PCIESPEED_STATUS_IIO3PORT12 },
  { STR_LINK_STATUS_IIO3PORT13, STR_MAXLINK_STATUS_IIO3PORT13, STR_PCIESPEED_STATUS_IIO3PORT13 },
  { STR_LINK_STATUS_IIO3PORT14, STR_MAXLINK_STATUS_IIO3PORT14, STR_PCIESPEED_STATUS_IIO3PORT14 },
  { STR_LINK_STATUS_IIO3PORT15, STR_MAXLINK_STATUS_IIO3PORT15, STR_PCIESPEED_STATUS_IIO3PORT15 },
  { STR_LINK_STATUS_IIO3PORT16, STR_MAXLINK_STATUS_IIO3PORT16, STR_PCIESPEED_STATUS_IIO3PORT16 },
  { STR_LINK_STATUS_IIO3PORT17, STR_MAXLINK_STATUS_IIO3PORT17, STR_PCIESPEED_STATUS_IIO3PORT17 },
  { STR_LINK_STATUS_IIO3PORT18, STR_MAXLINK_STATUS_IIO3PORT18, STR_PCIESPEED_STATUS_IIO3PORT18 },
  { STR_LINK_STATUS_IIO3PORT19, STR_MAXLINK_STATUS_IIO3PORT19, STR_PCIESPEED_STATUS_IIO3PORT19 },
  { STR_LINK_STATUS_IIO3PORT20, STR_MAXLINK_STATUS_IIO3PORT20, STR_PCIESPEED_STATUS_IIO3PORT20 },
  { STR_LINK_STATUS_IIO3PORT21, STR_MAXLINK_STATUS_IIO3PORT21, STR_PCIESPEED_STATUS_IIO3PORT21 },
  { STR_LINK_STATUS_IIO3PORT22, STR_MAXLINK_STATUS_IIO3PORT22, STR_PCIESPEED_STATUS_IIO3PORT22 },
  { STR_LINK_STATUS_IIO3PORT23, STR_MAXLINK_STATUS_IIO3PORT23, STR_PCIESPEED_STATUS_IIO3PORT23 },
  { STR_LINK_STATUS_IIO3PORT24, STR_MAXLINK_STATUS_IIO3PORT24, STR_PCIESPEED_STATUS_IIO3PORT24 },
  { STR_LINK_STATUS_IIO3PORT25, STR_MAXLINK_STATUS_IIO3PORT25, STR_PCIESPEED_STATUS_IIO3PORT25 },
  { STR_LINK_STATUS_IIO3PORT26, STR_MAXLINK_STATUS_IIO3PORT26, STR_PCIESPEED_STATUS_IIO3PORT26 },
  { STR_LINK_STATUS_IIO3PORT27, STR_MAXLINK_STATUS_IIO3PORT27, STR_PCIESPEED_STATUS_IIO3PORT27 },
  { STR_LINK_STATUS_IIO3PORT28, STR_MAXLINK_STATUS_IIO3PORT28, STR_PCIESPEED_STATUS_IIO3PORT28 },
  { STR_LINK_STATUS_IIO3PORT29, STR_MAXLINK_STATUS_IIO3PORT29, STR_PCIESPEED_STATUS_IIO3PORT29 },
  { STR_LINK_STATUS_IIO3PORT30, STR_MAXLINK_STATUS_IIO3PORT30, STR_PCIESPEED_STATUS_IIO3PORT30 },
  { STR_LINK_STATUS_IIO3PORT31, STR_MAXLINK_STATUS_IIO3PORT31, STR_PCIESPEED_STATUS_IIO3PORT31 },
  { STR_LINK_STATUS_IIO3PORT32, STR_MAXLINK_STATUS_IIO3PORT32, STR_PCIESPEED_STATUS_IIO3PORT32 },
  { STR_LINK_STATUS_IIO3PORT33, STR_MAXLINK_STATUS_IIO3PORT33, STR_PCIESPEED_STATUS_IIO3PORT33 },
  { STR_LINK_STATUS_IIO3PORT34, STR_MAXLINK_STATUS_IIO3PORT34, STR_PCIESPEED_STATUS_IIO3PORT34 },
  { STR_LINK_STATUS_IIO3PORT35, STR_MAXLINK_STATUS_IIO3PORT35, STR_PCIESPEED_STATUS_IIO3PORT35 },
  { STR_LINK_STATUS_IIO3PORT36, STR_MAXLINK_STATUS_IIO3PORT36, STR_PCIESPEED_STATUS_IIO3PORT36 },
  { STR_LINK_STATUS_IIO3PORT37, STR_MAXLINK_STATUS_IIO3PORT37, STR_PCIESPEED_STATUS_IIO3PORT37 },
  { STR_LINK_STATUS_IIO3PORT38, STR_MAXLINK_STATUS_IIO3PORT38, STR_PCIESPEED_STATUS_IIO3PORT38 },
  { STR_LINK_STATUS_IIO3PORT39, STR_MAXLINK_STATUS_IIO3PORT39, STR_PCIESPEED_STATUS_IIO3PORT39 },
  { STR_LINK_STATUS_IIO3PORT40, STR_MAXLINK_STATUS_IIO3PORT40, STR_PCIESPEED_STATUS_IIO3PORT40 },
  //
  // Socket 4
  //
  { STR_LINK_STATUS_IIO4PORT0, STR_MAXLINK_STATUS_IIO4PORT0, STR_PCIESPEED_STATUS_IIO4PORT0 },
  { STR_LINK_STATUS_IIO4PORT1, STR_MAXLINK_STATUS_IIO4PORT1, STR_PCIESPEED_STATUS_IIO4PORT1 },
  { STR_LINK_STATUS_IIO4PORT2, STR_MAXLINK_STATUS_IIO4PORT2, STR_PCIESPEED_STATUS_IIO4PORT2 },
  { STR_LINK_STATUS_IIO4PORT3, STR_MAXLINK_STATUS_IIO4PORT3, STR_PCIESPEED_STATUS_IIO4PORT3 },
  { STR_LINK_STATUS_IIO4PORT4, STR_MAXLINK_STATUS_IIO4PORT4, STR_PCIESPEED_STATUS_IIO4PORT4 },
  { STR_LINK_STATUS_IIO4PORT5, STR_MAXLINK_STATUS_IIO4PORT5, STR_PCIESPEED_STATUS_IIO4PORT5 },
  { STR_LINK_STATUS_IIO4PORT6, STR_MAXLINK_STATUS_IIO4PORT6, STR_PCIESPEED_STATUS_IIO4PORT6 },
  { STR_LINK_STATUS_IIO4PORT7, STR_MAXLINK_STATUS_IIO4PORT7, STR_PCIESPEED_STATUS_IIO4PORT7 },
  { STR_LINK_STATUS_IIO4PORT8, STR_MAXLINK_STATUS_IIO4PORT8, STR_PCIESPEED_STATUS_IIO4PORT8 },
  { STR_LINK_STATUS_IIO4PORT9, STR_MAXLINK_STATUS_IIO4PORT9, STR_PCIESPEED_STATUS_IIO4PORT9 },
  { STR_LINK_STATUS_IIO4PORT10, STR_MAXLINK_STATUS_IIO4PORT10, STR_PCIESPEED_STATUS_IIO4PORT10 },
  { STR_LINK_STATUS_IIO4PORT11, STR_MAXLINK_STATUS_IIO4PORT11, STR_PCIESPEED_STATUS_IIO4PORT11 },
  { STR_LINK_STATUS_IIO4PORT12, STR_MAXLINK_STATUS_IIO4PORT12, STR_PCIESPEED_STATUS_IIO4PORT12 },
  { STR_LINK_STATUS_IIO4PORT13, STR_MAXLINK_STATUS_IIO4PORT13, STR_PCIESPEED_STATUS_IIO4PORT13 },
  { STR_LINK_STATUS_IIO4PORT14, STR_MAXLINK_STATUS_IIO4PORT14, STR_PCIESPEED_STATUS_IIO4PORT14 },
  { STR_LINK_STATUS_IIO4PORT15, STR_MAXLINK_STATUS_IIO4PORT15, STR_PCIESPEED_STATUS_IIO4PORT15 },
  { STR_LINK_STATUS_IIO4PORT16, STR_MAXLINK_STATUS_IIO4PORT16, STR_PCIESPEED_STATUS_IIO4PORT16 },
  { STR_LINK_STATUS_IIO4PORT17, STR_MAXLINK_STATUS_IIO4PORT17, STR_PCIESPEED_STATUS_IIO4PORT17 },
  { STR_LINK_STATUS_IIO4PORT18, STR_MAXLINK_STATUS_IIO4PORT18, STR_PCIESPEED_STATUS_IIO4PORT18 },
  { STR_LINK_STATUS_IIO4PORT19, STR_MAXLINK_STATUS_IIO4PORT19, STR_PCIESPEED_STATUS_IIO4PORT19 },
  { STR_LINK_STATUS_IIO4PORT20, STR_MAXLINK_STATUS_IIO4PORT20, STR_PCIESPEED_STATUS_IIO4PORT20 },
  { STR_LINK_STATUS_IIO4PORT21, STR_MAXLINK_STATUS_IIO4PORT21, STR_PCIESPEED_STATUS_IIO4PORT21 },
  { STR_LINK_STATUS_IIO4PORT22, STR_MAXLINK_STATUS_IIO4PORT22, STR_PCIESPEED_STATUS_IIO4PORT22 },
  { STR_LINK_STATUS_IIO4PORT23, STR_MAXLINK_STATUS_IIO4PORT23, STR_PCIESPEED_STATUS_IIO4PORT23 },
  { STR_LINK_STATUS_IIO4PORT24, STR_MAXLINK_STATUS_IIO4PORT24, STR_PCIESPEED_STATUS_IIO4PORT24 },
  { STR_LINK_STATUS_IIO4PORT25, STR_MAXLINK_STATUS_IIO4PORT25, STR_PCIESPEED_STATUS_IIO4PORT25 },
  { STR_LINK_STATUS_IIO4PORT26, STR_MAXLINK_STATUS_IIO4PORT26, STR_PCIESPEED_STATUS_IIO4PORT26 },
  { STR_LINK_STATUS_IIO4PORT27, STR_MAXLINK_STATUS_IIO4PORT27, STR_PCIESPEED_STATUS_IIO4PORT27 },
  { STR_LINK_STATUS_IIO4PORT28, STR_MAXLINK_STATUS_IIO4PORT28, STR_PCIESPEED_STATUS_IIO4PORT28 },
  { STR_LINK_STATUS_IIO4PORT29, STR_MAXLINK_STATUS_IIO4PORT29, STR_PCIESPEED_STATUS_IIO4PORT29 },
  { STR_LINK_STATUS_IIO4PORT30, STR_MAXLINK_STATUS_IIO4PORT30, STR_PCIESPEED_STATUS_IIO4PORT30 },
  { STR_LINK_STATUS_IIO4PORT31, STR_MAXLINK_STATUS_IIO4PORT31, STR_PCIESPEED_STATUS_IIO4PORT31 },
  { STR_LINK_STATUS_IIO4PORT32, STR_MAXLINK_STATUS_IIO4PORT32, STR_PCIESPEED_STATUS_IIO4PORT32 },
  { STR_LINK_STATUS_IIO4PORT33, STR_MAXLINK_STATUS_IIO4PORT33, STR_PCIESPEED_STATUS_IIO4PORT33 },
  { STR_LINK_STATUS_IIO4PORT34, STR_MAXLINK_STATUS_IIO4PORT34, STR_PCIESPEED_STATUS_IIO4PORT34 },
  { STR_LINK_STATUS_IIO4PORT35, STR_MAXLINK_STATUS_IIO4PORT35, STR_PCIESPEED_STATUS_IIO4PORT35 },
  { STR_LINK_STATUS_IIO4PORT36, STR_MAXLINK_STATUS_IIO4PORT36, STR_PCIESPEED_STATUS_IIO4PORT36 },
  { STR_LINK_STATUS_IIO4PORT37, STR_MAXLINK_STATUS_IIO4PORT37, STR_PCIESPEED_STATUS_IIO4PORT37 },
  { STR_LINK_STATUS_IIO4PORT38, STR_MAXLINK_STATUS_IIO4PORT38, STR_PCIESPEED_STATUS_IIO4PORT38 },
  { STR_LINK_STATUS_IIO4PORT39, STR_MAXLINK_STATUS_IIO4PORT39, STR_PCIESPEED_STATUS_IIO4PORT39 },
  { STR_LINK_STATUS_IIO4PORT40, STR_MAXLINK_STATUS_IIO4PORT40, STR_PCIESPEED_STATUS_IIO4PORT40 },
  //
  // Socket 5
  //
  { STR_LINK_STATUS_IIO5PORT0, STR_MAXLINK_STATUS_IIO5PORT0, STR_PCIESPEED_STATUS_IIO5PORT0 },
  { STR_LINK_STATUS_IIO5PORT1, STR_MAXLINK_STATUS_IIO5PORT1, STR_PCIESPEED_STATUS_IIO5PORT1 },
  { STR_LINK_STATUS_IIO5PORT2, STR_MAXLINK_STATUS_IIO5PORT2, STR_PCIESPEED_STATUS_IIO5PORT2 },
  { STR_LINK_STATUS_IIO5PORT3, STR_MAXLINK_STATUS_IIO5PORT3, STR_PCIESPEED_STATUS_IIO5PORT3 },
  { STR_LINK_STATUS_IIO5PORT4, STR_MAXLINK_STATUS_IIO5PORT4, STR_PCIESPEED_STATUS_IIO5PORT4 },
  { STR_LINK_STATUS_IIO5PORT5, STR_MAXLINK_STATUS_IIO5PORT5, STR_PCIESPEED_STATUS_IIO5PORT5 },
  { STR_LINK_STATUS_IIO5PORT6, STR_MAXLINK_STATUS_IIO5PORT6, STR_PCIESPEED_STATUS_IIO5PORT6 },
  { STR_LINK_STATUS_IIO5PORT7, STR_MAXLINK_STATUS_IIO5PORT7, STR_PCIESPEED_STATUS_IIO5PORT7 },
  { STR_LINK_STATUS_IIO5PORT8, STR_MAXLINK_STATUS_IIO5PORT8, STR_PCIESPEED_STATUS_IIO5PORT8 },
  { STR_LINK_STATUS_IIO5PORT9, STR_MAXLINK_STATUS_IIO5PORT9, STR_PCIESPEED_STATUS_IIO5PORT9 },
  { STR_LINK_STATUS_IIO5PORT10, STR_MAXLINK_STATUS_IIO5PORT10, STR_PCIESPEED_STATUS_IIO5PORT10 },
  { STR_LINK_STATUS_IIO5PORT11, STR_MAXLINK_STATUS_IIO5PORT11, STR_PCIESPEED_STATUS_IIO5PORT11 },
  { STR_LINK_STATUS_IIO5PORT12, STR_MAXLINK_STATUS_IIO5PORT12, STR_PCIESPEED_STATUS_IIO5PORT12 },
  { STR_LINK_STATUS_IIO5PORT13, STR_MAXLINK_STATUS_IIO5PORT13, STR_PCIESPEED_STATUS_IIO5PORT13 },
  { STR_LINK_STATUS_IIO5PORT14, STR_MAXLINK_STATUS_IIO5PORT14, STR_PCIESPEED_STATUS_IIO5PORT14 },
  { STR_LINK_STATUS_IIO5PORT15, STR_MAXLINK_STATUS_IIO5PORT15, STR_PCIESPEED_STATUS_IIO5PORT15 },
  { STR_LINK_STATUS_IIO5PORT16, STR_MAXLINK_STATUS_IIO5PORT16, STR_PCIESPEED_STATUS_IIO5PORT16 },
  { STR_LINK_STATUS_IIO5PORT17, STR_MAXLINK_STATUS_IIO5PORT17, STR_PCIESPEED_STATUS_IIO5PORT17 },
  { STR_LINK_STATUS_IIO5PORT18, STR_MAXLINK_STATUS_IIO5PORT18, STR_PCIESPEED_STATUS_IIO5PORT18 },
  { STR_LINK_STATUS_IIO5PORT19, STR_MAXLINK_STATUS_IIO5PORT19, STR_PCIESPEED_STATUS_IIO5PORT19 },
  { STR_LINK_STATUS_IIO5PORT20, STR_MAXLINK_STATUS_IIO5PORT20, STR_PCIESPEED_STATUS_IIO5PORT20 },
  { STR_LINK_STATUS_IIO5PORT21, STR_MAXLINK_STATUS_IIO5PORT21, STR_PCIESPEED_STATUS_IIO5PORT21 },
  { STR_LINK_STATUS_IIO5PORT22, STR_MAXLINK_STATUS_IIO5PORT22, STR_PCIESPEED_STATUS_IIO5PORT22 },
  { STR_LINK_STATUS_IIO5PORT23, STR_MAXLINK_STATUS_IIO5PORT23, STR_PCIESPEED_STATUS_IIO5PORT23 },
  { STR_LINK_STATUS_IIO5PORT24, STR_MAXLINK_STATUS_IIO5PORT24, STR_PCIESPEED_STATUS_IIO5PORT24 },
  { STR_LINK_STATUS_IIO5PORT25, STR_MAXLINK_STATUS_IIO5PORT25, STR_PCIESPEED_STATUS_IIO5PORT25 },
  { STR_LINK_STATUS_IIO5PORT26, STR_MAXLINK_STATUS_IIO5PORT26, STR_PCIESPEED_STATUS_IIO5PORT26 },
  { STR_LINK_STATUS_IIO5PORT27, STR_MAXLINK_STATUS_IIO5PORT27, STR_PCIESPEED_STATUS_IIO5PORT27 },
  { STR_LINK_STATUS_IIO5PORT28, STR_MAXLINK_STATUS_IIO5PORT28, STR_PCIESPEED_STATUS_IIO5PORT28 },
  { STR_LINK_STATUS_IIO5PORT29, STR_MAXLINK_STATUS_IIO5PORT29, STR_PCIESPEED_STATUS_IIO5PORT29 },
  { STR_LINK_STATUS_IIO5PORT30, STR_MAXLINK_STATUS_IIO5PORT30, STR_PCIESPEED_STATUS_IIO5PORT30 },
  { STR_LINK_STATUS_IIO5PORT31, STR_MAXLINK_STATUS_IIO5PORT31, STR_PCIESPEED_STATUS_IIO5PORT31 },
  { STR_LINK_STATUS_IIO5PORT32, STR_MAXLINK_STATUS_IIO5PORT32, STR_PCIESPEED_STATUS_IIO5PORT32 },
  { STR_LINK_STATUS_IIO5PORT33, STR_MAXLINK_STATUS_IIO5PORT33, STR_PCIESPEED_STATUS_IIO5PORT33 },
  { STR_LINK_STATUS_IIO5PORT34, STR_MAXLINK_STATUS_IIO5PORT34, STR_PCIESPEED_STATUS_IIO5PORT34 },
  { STR_LINK_STATUS_IIO5PORT35, STR_MAXLINK_STATUS_IIO5PORT35, STR_PCIESPEED_STATUS_IIO5PORT35 },
  { STR_LINK_STATUS_IIO5PORT36, STR_MAXLINK_STATUS_IIO5PORT36, STR_PCIESPEED_STATUS_IIO5PORT36 },
  { STR_LINK_STATUS_IIO5PORT37, STR_MAXLINK_STATUS_IIO5PORT37, STR_PCIESPEED_STATUS_IIO5PORT37 },
  { STR_LINK_STATUS_IIO5PORT38, STR_MAXLINK_STATUS_IIO5PORT38, STR_PCIESPEED_STATUS_IIO5PORT38 },
  { STR_LINK_STATUS_IIO5PORT39, STR_MAXLINK_STATUS_IIO5PORT39, STR_PCIESPEED_STATUS_IIO5PORT39 },
  { STR_LINK_STATUS_IIO5PORT40, STR_MAXLINK_STATUS_IIO5PORT40, STR_PCIESPEED_STATUS_IIO5PORT40 },
  //
  // Socket 6
  //
  { STR_LINK_STATUS_IIO6PORT0, STR_MAXLINK_STATUS_IIO6PORT0, STR_PCIESPEED_STATUS_IIO6PORT0 },
  { STR_LINK_STATUS_IIO6PORT1, STR_MAXLINK_STATUS_IIO6PORT1, STR_PCIESPEED_STATUS_IIO6PORT1 },
  { STR_LINK_STATUS_IIO6PORT2, STR_MAXLINK_STATUS_IIO6PORT2, STR_PCIESPEED_STATUS_IIO6PORT2 },
  { STR_LINK_STATUS_IIO6PORT3, STR_MAXLINK_STATUS_IIO6PORT3, STR_PCIESPEED_STATUS_IIO6PORT3 },
  { STR_LINK_STATUS_IIO6PORT4, STR_MAXLINK_STATUS_IIO6PORT4, STR_PCIESPEED_STATUS_IIO6PORT4 },
  { STR_LINK_STATUS_IIO6PORT5, STR_MAXLINK_STATUS_IIO6PORT5, STR_PCIESPEED_STATUS_IIO6PORT5 },
  { STR_LINK_STATUS_IIO6PORT6, STR_MAXLINK_STATUS_IIO6PORT6, STR_PCIESPEED_STATUS_IIO6PORT6 },
  { STR_LINK_STATUS_IIO6PORT7, STR_MAXLINK_STATUS_IIO6PORT7, STR_PCIESPEED_STATUS_IIO6PORT7 },
  { STR_LINK_STATUS_IIO6PORT8, STR_MAXLINK_STATUS_IIO6PORT8, STR_PCIESPEED_STATUS_IIO6PORT8 },
  { STR_LINK_STATUS_IIO6PORT9, STR_MAXLINK_STATUS_IIO6PORT9, STR_PCIESPEED_STATUS_IIO6PORT9 },
  { STR_LINK_STATUS_IIO6PORT10, STR_MAXLINK_STATUS_IIO6PORT10, STR_PCIESPEED_STATUS_IIO6PORT10 },
  { STR_LINK_STATUS_IIO6PORT11, STR_MAXLINK_STATUS_IIO6PORT11, STR_PCIESPEED_STATUS_IIO6PORT11 },
  { STR_LINK_STATUS_IIO6PORT12, STR_MAXLINK_STATUS_IIO6PORT12, STR_PCIESPEED_STATUS_IIO6PORT12 },
  { STR_LINK_STATUS_IIO6PORT13, STR_MAXLINK_STATUS_IIO6PORT13, STR_PCIESPEED_STATUS_IIO6PORT13 },
  { STR_LINK_STATUS_IIO6PORT14, STR_MAXLINK_STATUS_IIO6PORT14, STR_PCIESPEED_STATUS_IIO6PORT14 },
  { STR_LINK_STATUS_IIO6PORT15, STR_MAXLINK_STATUS_IIO6PORT15, STR_PCIESPEED_STATUS_IIO6PORT15 },
  { STR_LINK_STATUS_IIO6PORT16, STR_MAXLINK_STATUS_IIO6PORT16, STR_PCIESPEED_STATUS_IIO6PORT16 },
  { STR_LINK_STATUS_IIO6PORT17, STR_MAXLINK_STATUS_IIO6PORT17, STR_PCIESPEED_STATUS_IIO6PORT17 },
  { STR_LINK_STATUS_IIO6PORT18, STR_MAXLINK_STATUS_IIO6PORT18, STR_PCIESPEED_STATUS_IIO6PORT18 },
  { STR_LINK_STATUS_IIO6PORT19, STR_MAXLINK_STATUS_IIO6PORT19, STR_PCIESPEED_STATUS_IIO6PORT19 },
  { STR_LINK_STATUS_IIO6PORT20, STR_MAXLINK_STATUS_IIO6PORT20, STR_PCIESPEED_STATUS_IIO6PORT20 },
  { STR_LINK_STATUS_IIO6PORT21, STR_MAXLINK_STATUS_IIO6PORT21, STR_PCIESPEED_STATUS_IIO6PORT21 },
  { STR_LINK_STATUS_IIO6PORT22, STR_MAXLINK_STATUS_IIO6PORT22, STR_PCIESPEED_STATUS_IIO6PORT22 },
  { STR_LINK_STATUS_IIO6PORT23, STR_MAXLINK_STATUS_IIO6PORT23, STR_PCIESPEED_STATUS_IIO6PORT23 },
  { STR_LINK_STATUS_IIO6PORT24, STR_MAXLINK_STATUS_IIO6PORT24, STR_PCIESPEED_STATUS_IIO6PORT24 },
  { STR_LINK_STATUS_IIO6PORT25, STR_MAXLINK_STATUS_IIO6PORT25, STR_PCIESPEED_STATUS_IIO6PORT25 },
  { STR_LINK_STATUS_IIO6PORT26, STR_MAXLINK_STATUS_IIO6PORT26, STR_PCIESPEED_STATUS_IIO6PORT26 },
  { STR_LINK_STATUS_IIO6PORT27, STR_MAXLINK_STATUS_IIO6PORT27, STR_PCIESPEED_STATUS_IIO6PORT27 },
  { STR_LINK_STATUS_IIO6PORT28, STR_MAXLINK_STATUS_IIO6PORT28, STR_PCIESPEED_STATUS_IIO6PORT28 },
  { STR_LINK_STATUS_IIO6PORT29, STR_MAXLINK_STATUS_IIO6PORT29, STR_PCIESPEED_STATUS_IIO6PORT29 },
  { STR_LINK_STATUS_IIO6PORT30, STR_MAXLINK_STATUS_IIO6PORT30, STR_PCIESPEED_STATUS_IIO6PORT30 },
  { STR_LINK_STATUS_IIO6PORT31, STR_MAXLINK_STATUS_IIO6PORT31, STR_PCIESPEED_STATUS_IIO6PORT31 },
  { STR_LINK_STATUS_IIO6PORT32, STR_MAXLINK_STATUS_IIO6PORT32, STR_PCIESPEED_STATUS_IIO6PORT32 },
  { STR_LINK_STATUS_IIO6PORT33, STR_MAXLINK_STATUS_IIO6PORT33, STR_PCIESPEED_STATUS_IIO6PORT33 },
  { STR_LINK_STATUS_IIO6PORT34, STR_MAXLINK_STATUS_IIO6PORT34, STR_PCIESPEED_STATUS_IIO6PORT34 },
  { STR_LINK_STATUS_IIO6PORT35, STR_MAXLINK_STATUS_IIO6PORT35, STR_PCIESPEED_STATUS_IIO6PORT35 },
  { STR_LINK_STATUS_IIO6PORT36, STR_MAXLINK_STATUS_IIO6PORT36, STR_PCIESPEED_STATUS_IIO6PORT36 },
  { STR_LINK_STATUS_IIO6PORT37, STR_MAXLINK_STATUS_IIO6PORT37, STR_PCIESPEED_STATUS_IIO6PORT37 },
  { STR_LINK_STATUS_IIO6PORT38, STR_MAXLINK_STATUS_IIO6PORT38, STR_PCIESPEED_STATUS_IIO6PORT38 },
  { STR_LINK_STATUS_IIO6PORT39, STR_MAXLINK_STATUS_IIO6PORT39, STR_PCIESPEED_STATUS_IIO6PORT39 },
  { STR_LINK_STATUS_IIO6PORT40, STR_MAXLINK_STATUS_IIO6PORT40, STR_PCIESPEED_STATUS_IIO6PORT40 },
  //
  // Socket 7
  //
  { STR_LINK_STATUS_IIO7PORT0, STR_MAXLINK_STATUS_IIO7PORT0, STR_PCIESPEED_STATUS_IIO7PORT0 },
  { STR_LINK_STATUS_IIO7PORT1, STR_MAXLINK_STATUS_IIO7PORT1, STR_PCIESPEED_STATUS_IIO7PORT1 },
  { STR_LINK_STATUS_IIO7PORT2, STR_MAXLINK_STATUS_IIO7PORT2, STR_PCIESPEED_STATUS_IIO7PORT2 },
  { STR_LINK_STATUS_IIO7PORT3, STR_MAXLINK_STATUS_IIO7PORT3, STR_PCIESPEED_STATUS_IIO7PORT3 },
  { STR_LINK_STATUS_IIO7PORT4, STR_MAXLINK_STATUS_IIO7PORT4, STR_PCIESPEED_STATUS_IIO7PORT4 },
  { STR_LINK_STATUS_IIO7PORT5, STR_MAXLINK_STATUS_IIO7PORT5, STR_PCIESPEED_STATUS_IIO7PORT5 },
  { STR_LINK_STATUS_IIO7PORT6, STR_MAXLINK_STATUS_IIO7PORT6, STR_PCIESPEED_STATUS_IIO7PORT6 },
  { STR_LINK_STATUS_IIO7PORT7, STR_MAXLINK_STATUS_IIO7PORT7, STR_PCIESPEED_STATUS_IIO7PORT7 },
  { STR_LINK_STATUS_IIO7PORT8, STR_MAXLINK_STATUS_IIO7PORT8, STR_PCIESPEED_STATUS_IIO7PORT8 },
  { STR_LINK_STATUS_IIO7PORT9, STR_MAXLINK_STATUS_IIO7PORT9, STR_PCIESPEED_STATUS_IIO7PORT9 },
  { STR_LINK_STATUS_IIO7PORT10, STR_MAXLINK_STATUS_IIO7PORT10, STR_PCIESPEED_STATUS_IIO7PORT10 },
  { STR_LINK_STATUS_IIO7PORT11, STR_MAXLINK_STATUS_IIO7PORT11, STR_PCIESPEED_STATUS_IIO7PORT11 },
  { STR_LINK_STATUS_IIO7PORT12, STR_MAXLINK_STATUS_IIO7PORT12, STR_PCIESPEED_STATUS_IIO7PORT12 },
  { STR_LINK_STATUS_IIO7PORT13, STR_MAXLINK_STATUS_IIO7PORT13, STR_PCIESPEED_STATUS_IIO7PORT13 },
  { STR_LINK_STATUS_IIO7PORT14, STR_MAXLINK_STATUS_IIO7PORT14, STR_PCIESPEED_STATUS_IIO7PORT14 },
  { STR_LINK_STATUS_IIO7PORT15, STR_MAXLINK_STATUS_IIO7PORT15, STR_PCIESPEED_STATUS_IIO7PORT15 },
  { STR_LINK_STATUS_IIO7PORT16, STR_MAXLINK_STATUS_IIO7PORT16, STR_PCIESPEED_STATUS_IIO7PORT16 },
  { STR_LINK_STATUS_IIO7PORT17, STR_MAXLINK_STATUS_IIO7PORT17, STR_PCIESPEED_STATUS_IIO7PORT17 },
  { STR_LINK_STATUS_IIO7PORT18, STR_MAXLINK_STATUS_IIO7PORT18, STR_PCIESPEED_STATUS_IIO7PORT18 },
  { STR_LINK_STATUS_IIO7PORT19, STR_MAXLINK_STATUS_IIO7PORT19, STR_PCIESPEED_STATUS_IIO7PORT19 },
  { STR_LINK_STATUS_IIO7PORT20, STR_MAXLINK_STATUS_IIO7PORT20, STR_PCIESPEED_STATUS_IIO7PORT20 },
  { STR_LINK_STATUS_IIO7PORT21, STR_MAXLINK_STATUS_IIO7PORT21, STR_PCIESPEED_STATUS_IIO7PORT21 },
  { STR_LINK_STATUS_IIO7PORT22, STR_MAXLINK_STATUS_IIO7PORT22, STR_PCIESPEED_STATUS_IIO7PORT22 },
  { STR_LINK_STATUS_IIO7PORT23, STR_MAXLINK_STATUS_IIO7PORT23, STR_PCIESPEED_STATUS_IIO7PORT23 },
  { STR_LINK_STATUS_IIO7PORT24, STR_MAXLINK_STATUS_IIO7PORT24, STR_PCIESPEED_STATUS_IIO7PORT24 },
  { STR_LINK_STATUS_IIO7PORT25, STR_MAXLINK_STATUS_IIO7PORT25, STR_PCIESPEED_STATUS_IIO7PORT25 },
  { STR_LINK_STATUS_IIO7PORT26, STR_MAXLINK_STATUS_IIO7PORT26, STR_PCIESPEED_STATUS_IIO7PORT26 },
  { STR_LINK_STATUS_IIO7PORT27, STR_MAXLINK_STATUS_IIO7PORT27, STR_PCIESPEED_STATUS_IIO7PORT27 },
  { STR_LINK_STATUS_IIO7PORT28, STR_MAXLINK_STATUS_IIO7PORT28, STR_PCIESPEED_STATUS_IIO7PORT28 },
  { STR_LINK_STATUS_IIO7PORT29, STR_MAXLINK_STATUS_IIO7PORT29, STR_PCIESPEED_STATUS_IIO7PORT29 },
  { STR_LINK_STATUS_IIO7PORT30, STR_MAXLINK_STATUS_IIO7PORT30, STR_PCIESPEED_STATUS_IIO7PORT30 },
  { STR_LINK_STATUS_IIO7PORT31, STR_MAXLINK_STATUS_IIO7PORT31, STR_PCIESPEED_STATUS_IIO7PORT31 },
  { STR_LINK_STATUS_IIO7PORT32, STR_MAXLINK_STATUS_IIO7PORT32, STR_PCIESPEED_STATUS_IIO7PORT32 },
  { STR_LINK_STATUS_IIO7PORT33, STR_MAXLINK_STATUS_IIO7PORT33, STR_PCIESPEED_STATUS_IIO7PORT33 },
  { STR_LINK_STATUS_IIO7PORT34, STR_MAXLINK_STATUS_IIO7PORT34, STR_PCIESPEED_STATUS_IIO7PORT34 },
  { STR_LINK_STATUS_IIO7PORT35, STR_MAXLINK_STATUS_IIO7PORT35, STR_PCIESPEED_STATUS_IIO7PORT35 },
  { STR_LINK_STATUS_IIO7PORT36, STR_MAXLINK_STATUS_IIO7PORT36, STR_PCIESPEED_STATUS_IIO7PORT36 },
  { STR_LINK_STATUS_IIO7PORT37, STR_MAXLINK_STATUS_IIO7PORT37, STR_PCIESPEED_STATUS_IIO7PORT37 },
  { STR_LINK_STATUS_IIO7PORT38, STR_MAXLINK_STATUS_IIO7PORT38, STR_PCIESPEED_STATUS_IIO7PORT38 },
  { STR_LINK_STATUS_IIO7PORT39, STR_MAXLINK_STATUS_IIO7PORT39, STR_PCIESPEED_STATUS_IIO7PORT39 },
  { STR_LINK_STATUS_IIO7PORT40, STR_MAXLINK_STATUS_IIO7PORT40, STR_PCIESPEED_STATUS_IIO7PORT40 },
};

#else // #ifdef SPR_HOST

STATIC PCIE_STRING_TOKENS PcieStringTokens[] = {
  //
  // Socket 0
  //
  { STR_LINK_STATUS_IIO0PORT0, STR_MAXLINK_STATUS_IIO0PORT0, STR_PCIESPEED_STATUS_IIO0PORT0 },
  { STR_LINK_STATUS_IIO0PORT1, STR_MAXLINK_STATUS_IIO0PORT1, STR_PCIESPEED_STATUS_IIO0PORT1 },
  { STR_LINK_STATUS_IIO0PORT2, STR_MAXLINK_STATUS_IIO0PORT2, STR_PCIESPEED_STATUS_IIO0PORT2 },
  { STR_LINK_STATUS_IIO0PORT3, STR_MAXLINK_STATUS_IIO0PORT3, STR_PCIESPEED_STATUS_IIO0PORT3 },
  { STR_LINK_STATUS_IIO0PORT4, STR_MAXLINK_STATUS_IIO0PORT4, STR_PCIESPEED_STATUS_IIO0PORT4 },
  { STR_LINK_STATUS_IIO0PORT5, STR_MAXLINK_STATUS_IIO0PORT5, STR_PCIESPEED_STATUS_IIO0PORT5 },
  { STR_LINK_STATUS_IIO0PORT6, STR_MAXLINK_STATUS_IIO0PORT6, STR_PCIESPEED_STATUS_IIO0PORT6 },
  { STR_LINK_STATUS_IIO0PORT7, STR_MAXLINK_STATUS_IIO0PORT7, STR_PCIESPEED_STATUS_IIO0PORT7 },
  { STR_LINK_STATUS_IIO0PORT8, STR_MAXLINK_STATUS_IIO0PORT8, STR_PCIESPEED_STATUS_IIO0PORT8 },
  { STR_LINK_STATUS_IIO0PORT9, STR_MAXLINK_STATUS_IIO0PORT9, STR_PCIESPEED_STATUS_IIO0PORT9 },
  { STR_LINK_STATUS_IIO0PORT10, STR_MAXLINK_STATUS_IIO0PORT10, STR_PCIESPEED_STATUS_IIO0PORT10 },
  { STR_LINK_STATUS_IIO0PORT11, STR_MAXLINK_STATUS_IIO0PORT11, STR_PCIESPEED_STATUS_IIO0PORT11 },
  { STR_LINK_STATUS_IIO0PORT12, STR_MAXLINK_STATUS_IIO0PORT12, STR_PCIESPEED_STATUS_IIO0PORT12 },
  { STR_LINK_STATUS_IIO0PORT13, STR_MAXLINK_STATUS_IIO0PORT13, STR_PCIESPEED_STATUS_IIO0PORT13 },
  { STR_LINK_STATUS_IIO0PORT14, STR_MAXLINK_STATUS_IIO0PORT14, STR_PCIESPEED_STATUS_IIO0PORT14 },
  { STR_LINK_STATUS_IIO0PORT15, STR_MAXLINK_STATUS_IIO0PORT15, STR_PCIESPEED_STATUS_IIO0PORT15 },
  { STR_LINK_STATUS_IIO0PORT16, STR_MAXLINK_STATUS_IIO0PORT16, STR_PCIESPEED_STATUS_IIO0PORT16 },
  { STR_LINK_STATUS_IIO0PORT17, STR_MAXLINK_STATUS_IIO0PORT17, STR_PCIESPEED_STATUS_IIO0PORT17 },
  { STR_LINK_STATUS_IIO0PORT18, STR_MAXLINK_STATUS_IIO0PORT18, STR_PCIESPEED_STATUS_IIO0PORT18 },
  { STR_LINK_STATUS_IIO0PORT19, STR_MAXLINK_STATUS_IIO0PORT19, STR_PCIESPEED_STATUS_IIO0PORT19 },
  { STR_LINK_STATUS_IIO0PORT20, STR_MAXLINK_STATUS_IIO0PORT20, STR_PCIESPEED_STATUS_IIO0PORT20 },
  //
  // Socket 1
  //
  { STR_LINK_STATUS_IIO1PORT0, STR_MAXLINK_STATUS_IIO1PORT0, STR_PCIESPEED_STATUS_IIO1PORT0 },
  { STR_LINK_STATUS_IIO1PORT1, STR_MAXLINK_STATUS_IIO1PORT1, STR_PCIESPEED_STATUS_IIO1PORT1 },
  { STR_LINK_STATUS_IIO1PORT2, STR_MAXLINK_STATUS_IIO1PORT2, STR_PCIESPEED_STATUS_IIO1PORT2 },
  { STR_LINK_STATUS_IIO1PORT3, STR_MAXLINK_STATUS_IIO1PORT3, STR_PCIESPEED_STATUS_IIO1PORT3 },
  { STR_LINK_STATUS_IIO1PORT4, STR_MAXLINK_STATUS_IIO1PORT4, STR_PCIESPEED_STATUS_IIO1PORT4 },
  { STR_LINK_STATUS_IIO1PORT5, STR_MAXLINK_STATUS_IIO1PORT5, STR_PCIESPEED_STATUS_IIO1PORT5 },
  { STR_LINK_STATUS_IIO1PORT6, STR_MAXLINK_STATUS_IIO1PORT6, STR_PCIESPEED_STATUS_IIO1PORT6 },
  { STR_LINK_STATUS_IIO1PORT7, STR_MAXLINK_STATUS_IIO1PORT7, STR_PCIESPEED_STATUS_IIO1PORT7 },
  { STR_LINK_STATUS_IIO1PORT8, STR_MAXLINK_STATUS_IIO1PORT8, STR_PCIESPEED_STATUS_IIO1PORT8 },
  { STR_LINK_STATUS_IIO1PORT9, STR_MAXLINK_STATUS_IIO1PORT9, STR_PCIESPEED_STATUS_IIO1PORT9 },
  { STR_LINK_STATUS_IIO1PORT10, STR_MAXLINK_STATUS_IIO1PORT10, STR_PCIESPEED_STATUS_IIO1PORT10 },
  { STR_LINK_STATUS_IIO1PORT11, STR_MAXLINK_STATUS_IIO1PORT11, STR_PCIESPEED_STATUS_IIO1PORT11 },
  { STR_LINK_STATUS_IIO1PORT12, STR_MAXLINK_STATUS_IIO1PORT12, STR_PCIESPEED_STATUS_IIO1PORT12 },
  { STR_LINK_STATUS_IIO1PORT13, STR_MAXLINK_STATUS_IIO1PORT13, STR_PCIESPEED_STATUS_IIO1PORT13 },
  { STR_LINK_STATUS_IIO1PORT14, STR_MAXLINK_STATUS_IIO1PORT14, STR_PCIESPEED_STATUS_IIO1PORT14 },
  { STR_LINK_STATUS_IIO1PORT15, STR_MAXLINK_STATUS_IIO1PORT15, STR_PCIESPEED_STATUS_IIO1PORT15 },
  { STR_LINK_STATUS_IIO1PORT16, STR_MAXLINK_STATUS_IIO1PORT16, STR_PCIESPEED_STATUS_IIO1PORT16 },
  { STR_LINK_STATUS_IIO1PORT17, STR_MAXLINK_STATUS_IIO1PORT17, STR_PCIESPEED_STATUS_IIO1PORT17 },
  { STR_LINK_STATUS_IIO1PORT18, STR_MAXLINK_STATUS_IIO1PORT18, STR_PCIESPEED_STATUS_IIO1PORT18 },
  { STR_LINK_STATUS_IIO1PORT19, STR_MAXLINK_STATUS_IIO1PORT19, STR_PCIESPEED_STATUS_IIO1PORT19 },
  { STR_LINK_STATUS_IIO1PORT20, STR_MAXLINK_STATUS_IIO1PORT20, STR_PCIESPEED_STATUS_IIO1PORT20 },
  //
  // Socket 2
  //
  { STR_LINK_STATUS_IIO2PORT0, STR_MAXLINK_STATUS_IIO2PORT0, STR_PCIESPEED_STATUS_IIO2PORT0 },
  { STR_LINK_STATUS_IIO2PORT1, STR_MAXLINK_STATUS_IIO2PORT1, STR_PCIESPEED_STATUS_IIO2PORT1 },
  { STR_LINK_STATUS_IIO2PORT2, STR_MAXLINK_STATUS_IIO2PORT2, STR_PCIESPEED_STATUS_IIO2PORT2 },
  { STR_LINK_STATUS_IIO2PORT3, STR_MAXLINK_STATUS_IIO2PORT3, STR_PCIESPEED_STATUS_IIO2PORT3 },
  { STR_LINK_STATUS_IIO2PORT4, STR_MAXLINK_STATUS_IIO2PORT4, STR_PCIESPEED_STATUS_IIO2PORT4 },
  { STR_LINK_STATUS_IIO2PORT5, STR_MAXLINK_STATUS_IIO2PORT5, STR_PCIESPEED_STATUS_IIO2PORT5 },
  { STR_LINK_STATUS_IIO2PORT6, STR_MAXLINK_STATUS_IIO2PORT6, STR_PCIESPEED_STATUS_IIO2PORT6 },
  { STR_LINK_STATUS_IIO2PORT7, STR_MAXLINK_STATUS_IIO2PORT7, STR_PCIESPEED_STATUS_IIO2PORT7 },
  { STR_LINK_STATUS_IIO2PORT8, STR_MAXLINK_STATUS_IIO2PORT8, STR_PCIESPEED_STATUS_IIO2PORT8 },
  { STR_LINK_STATUS_IIO2PORT9, STR_MAXLINK_STATUS_IIO2PORT9, STR_PCIESPEED_STATUS_IIO2PORT9 },
  { STR_LINK_STATUS_IIO2PORT10, STR_MAXLINK_STATUS_IIO2PORT10, STR_PCIESPEED_STATUS_IIO2PORT10 },
  { STR_LINK_STATUS_IIO2PORT11, STR_MAXLINK_STATUS_IIO2PORT11, STR_PCIESPEED_STATUS_IIO2PORT11 },
  { STR_LINK_STATUS_IIO2PORT12, STR_MAXLINK_STATUS_IIO2PORT12, STR_PCIESPEED_STATUS_IIO2PORT12 },
  { STR_LINK_STATUS_IIO2PORT13, STR_MAXLINK_STATUS_IIO2PORT13, STR_PCIESPEED_STATUS_IIO2PORT13 },
  { STR_LINK_STATUS_IIO2PORT14, STR_MAXLINK_STATUS_IIO2PORT14, STR_PCIESPEED_STATUS_IIO2PORT14 },
  { STR_LINK_STATUS_IIO2PORT15, STR_MAXLINK_STATUS_IIO2PORT15, STR_PCIESPEED_STATUS_IIO2PORT15 },
  { STR_LINK_STATUS_IIO2PORT16, STR_MAXLINK_STATUS_IIO2PORT16, STR_PCIESPEED_STATUS_IIO2PORT16 },
  { STR_LINK_STATUS_IIO2PORT17, STR_MAXLINK_STATUS_IIO2PORT17, STR_PCIESPEED_STATUS_IIO2PORT17 },
  { STR_LINK_STATUS_IIO2PORT18, STR_MAXLINK_STATUS_IIO2PORT18, STR_PCIESPEED_STATUS_IIO2PORT18 },
  { STR_LINK_STATUS_IIO2PORT19, STR_MAXLINK_STATUS_IIO2PORT19, STR_PCIESPEED_STATUS_IIO2PORT19 },
  { STR_LINK_STATUS_IIO2PORT20, STR_MAXLINK_STATUS_IIO2PORT20, STR_PCIESPEED_STATUS_IIO2PORT20 },
  //
  // Socket 3
  //
  { STR_LINK_STATUS_IIO3PORT0, STR_MAXLINK_STATUS_IIO3PORT0, STR_PCIESPEED_STATUS_IIO3PORT0 },
  { STR_LINK_STATUS_IIO3PORT1, STR_MAXLINK_STATUS_IIO3PORT1, STR_PCIESPEED_STATUS_IIO3PORT1 },
  { STR_LINK_STATUS_IIO3PORT2, STR_MAXLINK_STATUS_IIO3PORT2, STR_PCIESPEED_STATUS_IIO3PORT2 },
  { STR_LINK_STATUS_IIO3PORT3, STR_MAXLINK_STATUS_IIO3PORT3, STR_PCIESPEED_STATUS_IIO3PORT3 },
  { STR_LINK_STATUS_IIO3PORT4, STR_MAXLINK_STATUS_IIO3PORT4, STR_PCIESPEED_STATUS_IIO3PORT4 },
  { STR_LINK_STATUS_IIO3PORT5, STR_MAXLINK_STATUS_IIO3PORT5, STR_PCIESPEED_STATUS_IIO3PORT5 },
  { STR_LINK_STATUS_IIO3PORT6, STR_MAXLINK_STATUS_IIO3PORT6, STR_PCIESPEED_STATUS_IIO3PORT6 },
  { STR_LINK_STATUS_IIO3PORT7, STR_MAXLINK_STATUS_IIO3PORT7, STR_PCIESPEED_STATUS_IIO3PORT7 },
  { STR_LINK_STATUS_IIO3PORT8, STR_MAXLINK_STATUS_IIO3PORT8, STR_PCIESPEED_STATUS_IIO3PORT8 },
  { STR_LINK_STATUS_IIO3PORT9, STR_MAXLINK_STATUS_IIO3PORT9, STR_PCIESPEED_STATUS_IIO3PORT9 },
  { STR_LINK_STATUS_IIO3PORT10, STR_MAXLINK_STATUS_IIO3PORT10, STR_PCIESPEED_STATUS_IIO3PORT10 },
  { STR_LINK_STATUS_IIO3PORT11, STR_MAXLINK_STATUS_IIO3PORT11, STR_PCIESPEED_STATUS_IIO3PORT11 },
  { STR_LINK_STATUS_IIO3PORT12, STR_MAXLINK_STATUS_IIO3PORT12, STR_PCIESPEED_STATUS_IIO3PORT12 },
  { STR_LINK_STATUS_IIO3PORT13, STR_MAXLINK_STATUS_IIO3PORT13, STR_PCIESPEED_STATUS_IIO3PORT13 },
  { STR_LINK_STATUS_IIO3PORT14, STR_MAXLINK_STATUS_IIO3PORT14, STR_PCIESPEED_STATUS_IIO3PORT14 },
  { STR_LINK_STATUS_IIO3PORT15, STR_MAXLINK_STATUS_IIO3PORT15, STR_PCIESPEED_STATUS_IIO3PORT15 },
  { STR_LINK_STATUS_IIO3PORT16, STR_MAXLINK_STATUS_IIO3PORT16, STR_PCIESPEED_STATUS_IIO3PORT16 },
  { STR_LINK_STATUS_IIO3PORT17, STR_MAXLINK_STATUS_IIO3PORT17, STR_PCIESPEED_STATUS_IIO3PORT17 },
  { STR_LINK_STATUS_IIO3PORT18, STR_MAXLINK_STATUS_IIO3PORT18, STR_PCIESPEED_STATUS_IIO3PORT18 },
  { STR_LINK_STATUS_IIO3PORT19, STR_MAXLINK_STATUS_IIO3PORT19, STR_PCIESPEED_STATUS_IIO3PORT19 },
  { STR_LINK_STATUS_IIO3PORT20, STR_MAXLINK_STATUS_IIO3PORT20, STR_PCIESPEED_STATUS_IIO3PORT20 },
  //
  // Socket 4
  //
  { STR_LINK_STATUS_IIO4PORT0, STR_MAXLINK_STATUS_IIO4PORT0, STR_PCIESPEED_STATUS_IIO4PORT0 },
  { STR_LINK_STATUS_IIO4PORT1, STR_MAXLINK_STATUS_IIO4PORT1, STR_PCIESPEED_STATUS_IIO4PORT1 },
  { STR_LINK_STATUS_IIO4PORT2, STR_MAXLINK_STATUS_IIO4PORT2, STR_PCIESPEED_STATUS_IIO4PORT2 },
  { STR_LINK_STATUS_IIO4PORT3, STR_MAXLINK_STATUS_IIO4PORT3, STR_PCIESPEED_STATUS_IIO4PORT3 },
  { STR_LINK_STATUS_IIO4PORT4, STR_MAXLINK_STATUS_IIO4PORT4, STR_PCIESPEED_STATUS_IIO4PORT4 },
  { STR_LINK_STATUS_IIO4PORT5, STR_MAXLINK_STATUS_IIO4PORT5, STR_PCIESPEED_STATUS_IIO4PORT5 },
  { STR_LINK_STATUS_IIO4PORT6, STR_MAXLINK_STATUS_IIO4PORT6, STR_PCIESPEED_STATUS_IIO4PORT6 },
  { STR_LINK_STATUS_IIO4PORT7, STR_MAXLINK_STATUS_IIO4PORT7, STR_PCIESPEED_STATUS_IIO4PORT7 },
  { STR_LINK_STATUS_IIO4PORT8, STR_MAXLINK_STATUS_IIO4PORT8, STR_PCIESPEED_STATUS_IIO4PORT8 },
  { STR_LINK_STATUS_IIO4PORT9, STR_MAXLINK_STATUS_IIO4PORT9, STR_PCIESPEED_STATUS_IIO4PORT9 },
  { STR_LINK_STATUS_IIO4PORT10, STR_MAXLINK_STATUS_IIO4PORT10, STR_PCIESPEED_STATUS_IIO4PORT10 },
  { STR_LINK_STATUS_IIO4PORT11, STR_MAXLINK_STATUS_IIO4PORT11, STR_PCIESPEED_STATUS_IIO4PORT11 },
  { STR_LINK_STATUS_IIO4PORT12, STR_MAXLINK_STATUS_IIO4PORT12, STR_PCIESPEED_STATUS_IIO4PORT12 },
  { STR_LINK_STATUS_IIO4PORT13, STR_MAXLINK_STATUS_IIO4PORT13, STR_PCIESPEED_STATUS_IIO4PORT13 },
  { STR_LINK_STATUS_IIO4PORT14, STR_MAXLINK_STATUS_IIO4PORT14, STR_PCIESPEED_STATUS_IIO4PORT14 },
  { STR_LINK_STATUS_IIO4PORT15, STR_MAXLINK_STATUS_IIO4PORT15, STR_PCIESPEED_STATUS_IIO4PORT15 },
  { STR_LINK_STATUS_IIO4PORT16, STR_MAXLINK_STATUS_IIO4PORT16, STR_PCIESPEED_STATUS_IIO4PORT16 },
  { STR_LINK_STATUS_IIO4PORT17, STR_MAXLINK_STATUS_IIO4PORT17, STR_PCIESPEED_STATUS_IIO4PORT17 },
  { STR_LINK_STATUS_IIO4PORT18, STR_MAXLINK_STATUS_IIO4PORT18, STR_PCIESPEED_STATUS_IIO4PORT18 },
  { STR_LINK_STATUS_IIO4PORT19, STR_MAXLINK_STATUS_IIO4PORT19, STR_PCIESPEED_STATUS_IIO4PORT19 },
  { STR_LINK_STATUS_IIO4PORT20, STR_MAXLINK_STATUS_IIO4PORT20, STR_PCIESPEED_STATUS_IIO4PORT20 },
  //
  // Socket 5
  //
  { STR_LINK_STATUS_IIO5PORT0, STR_MAXLINK_STATUS_IIO5PORT0, STR_PCIESPEED_STATUS_IIO5PORT0 },
  { STR_LINK_STATUS_IIO5PORT1, STR_MAXLINK_STATUS_IIO5PORT1, STR_PCIESPEED_STATUS_IIO5PORT1 },
  { STR_LINK_STATUS_IIO5PORT2, STR_MAXLINK_STATUS_IIO5PORT2, STR_PCIESPEED_STATUS_IIO5PORT2 },
  { STR_LINK_STATUS_IIO5PORT3, STR_MAXLINK_STATUS_IIO5PORT3, STR_PCIESPEED_STATUS_IIO5PORT3 },
  { STR_LINK_STATUS_IIO5PORT4, STR_MAXLINK_STATUS_IIO5PORT4, STR_PCIESPEED_STATUS_IIO5PORT4 },
  { STR_LINK_STATUS_IIO5PORT5, STR_MAXLINK_STATUS_IIO5PORT5, STR_PCIESPEED_STATUS_IIO5PORT5 },
  { STR_LINK_STATUS_IIO5PORT6, STR_MAXLINK_STATUS_IIO5PORT6, STR_PCIESPEED_STATUS_IIO5PORT6 },
  { STR_LINK_STATUS_IIO5PORT7, STR_MAXLINK_STATUS_IIO5PORT7, STR_PCIESPEED_STATUS_IIO5PORT7 },
  { STR_LINK_STATUS_IIO5PORT8, STR_MAXLINK_STATUS_IIO5PORT8, STR_PCIESPEED_STATUS_IIO5PORT8 },
  { STR_LINK_STATUS_IIO5PORT9, STR_MAXLINK_STATUS_IIO5PORT9, STR_PCIESPEED_STATUS_IIO5PORT9 },
  { STR_LINK_STATUS_IIO5PORT10, STR_MAXLINK_STATUS_IIO5PORT10, STR_PCIESPEED_STATUS_IIO5PORT10 },
  { STR_LINK_STATUS_IIO5PORT11, STR_MAXLINK_STATUS_IIO5PORT11, STR_PCIESPEED_STATUS_IIO5PORT11 },
  { STR_LINK_STATUS_IIO5PORT12, STR_MAXLINK_STATUS_IIO5PORT12, STR_PCIESPEED_STATUS_IIO5PORT12 },
  { STR_LINK_STATUS_IIO5PORT13, STR_MAXLINK_STATUS_IIO5PORT13, STR_PCIESPEED_STATUS_IIO5PORT13 },
  { STR_LINK_STATUS_IIO5PORT14, STR_MAXLINK_STATUS_IIO5PORT14, STR_PCIESPEED_STATUS_IIO5PORT14 },
  { STR_LINK_STATUS_IIO5PORT15, STR_MAXLINK_STATUS_IIO5PORT15, STR_PCIESPEED_STATUS_IIO5PORT15 },
  { STR_LINK_STATUS_IIO5PORT16, STR_MAXLINK_STATUS_IIO5PORT16, STR_PCIESPEED_STATUS_IIO5PORT16 },
  { STR_LINK_STATUS_IIO5PORT17, STR_MAXLINK_STATUS_IIO5PORT17, STR_PCIESPEED_STATUS_IIO5PORT17 },
  { STR_LINK_STATUS_IIO5PORT18, STR_MAXLINK_STATUS_IIO5PORT18, STR_PCIESPEED_STATUS_IIO5PORT18 },
  { STR_LINK_STATUS_IIO5PORT19, STR_MAXLINK_STATUS_IIO5PORT19, STR_PCIESPEED_STATUS_IIO5PORT19 },
  { STR_LINK_STATUS_IIO5PORT20, STR_MAXLINK_STATUS_IIO5PORT20, STR_PCIESPEED_STATUS_IIO5PORT20 },
  //
  // Socket 6
  //
  { STR_LINK_STATUS_IIO6PORT0, STR_MAXLINK_STATUS_IIO6PORT0, STR_PCIESPEED_STATUS_IIO6PORT0 },
  { STR_LINK_STATUS_IIO6PORT1, STR_MAXLINK_STATUS_IIO6PORT1, STR_PCIESPEED_STATUS_IIO6PORT1 },
  { STR_LINK_STATUS_IIO6PORT2, STR_MAXLINK_STATUS_IIO6PORT2, STR_PCIESPEED_STATUS_IIO6PORT2 },
  { STR_LINK_STATUS_IIO6PORT3, STR_MAXLINK_STATUS_IIO6PORT3, STR_PCIESPEED_STATUS_IIO6PORT3 },
  { STR_LINK_STATUS_IIO6PORT4, STR_MAXLINK_STATUS_IIO6PORT4, STR_PCIESPEED_STATUS_IIO6PORT4 },
  { STR_LINK_STATUS_IIO6PORT5, STR_MAXLINK_STATUS_IIO6PORT5, STR_PCIESPEED_STATUS_IIO6PORT5 },
  { STR_LINK_STATUS_IIO6PORT6, STR_MAXLINK_STATUS_IIO6PORT6, STR_PCIESPEED_STATUS_IIO6PORT6 },
  { STR_LINK_STATUS_IIO6PORT7, STR_MAXLINK_STATUS_IIO6PORT7, STR_PCIESPEED_STATUS_IIO6PORT7 },
  { STR_LINK_STATUS_IIO6PORT8, STR_MAXLINK_STATUS_IIO6PORT8, STR_PCIESPEED_STATUS_IIO6PORT8 },
  { STR_LINK_STATUS_IIO6PORT9, STR_MAXLINK_STATUS_IIO6PORT9, STR_PCIESPEED_STATUS_IIO6PORT9 },
  { STR_LINK_STATUS_IIO6PORT10, STR_MAXLINK_STATUS_IIO6PORT10, STR_PCIESPEED_STATUS_IIO6PORT10 },
  { STR_LINK_STATUS_IIO6PORT11, STR_MAXLINK_STATUS_IIO6PORT11, STR_PCIESPEED_STATUS_IIO6PORT11 },
  { STR_LINK_STATUS_IIO6PORT12, STR_MAXLINK_STATUS_IIO6PORT12, STR_PCIESPEED_STATUS_IIO6PORT12 },
  { STR_LINK_STATUS_IIO6PORT13, STR_MAXLINK_STATUS_IIO6PORT13, STR_PCIESPEED_STATUS_IIO6PORT13 },
  { STR_LINK_STATUS_IIO6PORT14, STR_MAXLINK_STATUS_IIO6PORT14, STR_PCIESPEED_STATUS_IIO6PORT14 },
  { STR_LINK_STATUS_IIO6PORT15, STR_MAXLINK_STATUS_IIO6PORT15, STR_PCIESPEED_STATUS_IIO6PORT15 },
  { STR_LINK_STATUS_IIO6PORT16, STR_MAXLINK_STATUS_IIO6PORT16, STR_PCIESPEED_STATUS_IIO6PORT16 },
  { STR_LINK_STATUS_IIO6PORT17, STR_MAXLINK_STATUS_IIO6PORT17, STR_PCIESPEED_STATUS_IIO6PORT17 },
  { STR_LINK_STATUS_IIO6PORT18, STR_MAXLINK_STATUS_IIO6PORT18, STR_PCIESPEED_STATUS_IIO6PORT18 },
  { STR_LINK_STATUS_IIO6PORT19, STR_MAXLINK_STATUS_IIO6PORT19, STR_PCIESPEED_STATUS_IIO6PORT19 },
  { STR_LINK_STATUS_IIO6PORT20, STR_MAXLINK_STATUS_IIO6PORT20, STR_PCIESPEED_STATUS_IIO6PORT20 },
  //
  // Socket 7
  //
  { STR_LINK_STATUS_IIO7PORT0, STR_MAXLINK_STATUS_IIO7PORT0, STR_PCIESPEED_STATUS_IIO7PORT0 },
  { STR_LINK_STATUS_IIO7PORT1, STR_MAXLINK_STATUS_IIO7PORT1, STR_PCIESPEED_STATUS_IIO7PORT1 },
  { STR_LINK_STATUS_IIO7PORT2, STR_MAXLINK_STATUS_IIO7PORT2, STR_PCIESPEED_STATUS_IIO7PORT2 },
  { STR_LINK_STATUS_IIO7PORT3, STR_MAXLINK_STATUS_IIO7PORT3, STR_PCIESPEED_STATUS_IIO7PORT3 },
  { STR_LINK_STATUS_IIO7PORT4, STR_MAXLINK_STATUS_IIO7PORT4, STR_PCIESPEED_STATUS_IIO7PORT4 },
  { STR_LINK_STATUS_IIO7PORT5, STR_MAXLINK_STATUS_IIO7PORT5, STR_PCIESPEED_STATUS_IIO7PORT5 },
  { STR_LINK_STATUS_IIO7PORT6, STR_MAXLINK_STATUS_IIO7PORT6, STR_PCIESPEED_STATUS_IIO7PORT6 },
  { STR_LINK_STATUS_IIO7PORT7, STR_MAXLINK_STATUS_IIO7PORT7, STR_PCIESPEED_STATUS_IIO7PORT7 },
  { STR_LINK_STATUS_IIO7PORT8, STR_MAXLINK_STATUS_IIO7PORT8, STR_PCIESPEED_STATUS_IIO7PORT8 },
  { STR_LINK_STATUS_IIO7PORT9, STR_MAXLINK_STATUS_IIO7PORT9, STR_PCIESPEED_STATUS_IIO7PORT9 },
  { STR_LINK_STATUS_IIO7PORT10, STR_MAXLINK_STATUS_IIO7PORT10, STR_PCIESPEED_STATUS_IIO7PORT10 },
  { STR_LINK_STATUS_IIO7PORT11, STR_MAXLINK_STATUS_IIO7PORT11, STR_PCIESPEED_STATUS_IIO7PORT11 },
  { STR_LINK_STATUS_IIO7PORT12, STR_MAXLINK_STATUS_IIO7PORT12, STR_PCIESPEED_STATUS_IIO7PORT12 },
  { STR_LINK_STATUS_IIO7PORT13, STR_MAXLINK_STATUS_IIO7PORT13, STR_PCIESPEED_STATUS_IIO7PORT13 },
  { STR_LINK_STATUS_IIO7PORT14, STR_MAXLINK_STATUS_IIO7PORT14, STR_PCIESPEED_STATUS_IIO7PORT14 },
  { STR_LINK_STATUS_IIO7PORT15, STR_MAXLINK_STATUS_IIO7PORT15, STR_PCIESPEED_STATUS_IIO7PORT15 },
  { STR_LINK_STATUS_IIO7PORT16, STR_MAXLINK_STATUS_IIO7PORT16, STR_PCIESPEED_STATUS_IIO7PORT16 },
  { STR_LINK_STATUS_IIO7PORT17, STR_MAXLINK_STATUS_IIO7PORT17, STR_PCIESPEED_STATUS_IIO7PORT17 },
  { STR_LINK_STATUS_IIO7PORT18, STR_MAXLINK_STATUS_IIO7PORT18, STR_PCIESPEED_STATUS_IIO7PORT18 },
  { STR_LINK_STATUS_IIO7PORT19, STR_MAXLINK_STATUS_IIO7PORT19, STR_PCIESPEED_STATUS_IIO7PORT19 },
  { STR_LINK_STATUS_IIO7PORT20, STR_MAXLINK_STATUS_IIO7PORT20, STR_PCIESPEED_STATUS_IIO7PORT20 },
};
#endif // #ifdef SPR_HOST

#define STR_SIZE    0x50
/**
  This function configures PCIE settings in Setup.
**/
VOID
ConfigPciePort (
  VOID
  )
{
  UINT8  LinkWidth;
  UINT8  MaxLinkWidth;
  UINT8  LinkSpeed;

  UINT8  PortIndex;
  UINT16 TotalPortIdx;
  UINT8  IioIndex;

  CHAR16 ValueStr[STR_SIZE];
  CHAR16 FinalStr[STR_SIZE];

  //
  // Get the PCI-E Link Status
  //
  for (IioIndex = 0; IioIndex < mIioUds->IioUdsPtr->PlatformData.numofIIO; IioIndex++ ) {
    for (PortIndex = 0; PortIndex < GetMaxPortNumPerSocket (); PortIndex++) {

        TotalPortIdx = IioIndex * GetMaxPortNumPerSocket () + PortIndex;

        ASSERT (TotalPortIdx < ARRAY_SIZE (PcieStringTokens));
        if (TotalPortIdx >= ARRAY_SIZE (PcieStringTokens)) {
          break;
        }

        LinkWidth = 0;
        MaxLinkWidth = 0;
        LinkSpeed = 0;

        IioGetPciePortLinkStatus (IioIndex, PortIndex, &LinkWidth, &MaxLinkWidth, &LinkSpeed);

        //
        // Update Link Status
        //
        if (LinkWidth != 0) {
          UnicodeValueToStringS (ValueStr, STR_SIZE, LEFT_JUSTIFY, LinkWidth, 2);
          StrCpyS (FinalStr, STR_SIZE/sizeof(CHAR16), L"Linked as x");
          StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), ValueStr);
        } else {
          StrCpyS (FinalStr, STR_SIZE/sizeof(CHAR16), L"Link Did Not Train");
        }
        HiiSetString (mSocketStringsHiiHandle, PcieStringTokens[TotalPortIdx].LinkStatusToken, FinalStr, NULL);

        //
        // Update Max link width
        //
        if (MaxLinkWidth != 0) {
          UnicodeValueToStringS (ValueStr, STR_SIZE, LEFT_JUSTIFY, MaxLinkWidth, 2);
          StrCpyS (FinalStr, STR_SIZE/sizeof(CHAR16), L"Max Width x");
          StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), ValueStr);
        } else {
          StrCpyS (FinalStr, STR_SIZE/sizeof(CHAR16), L"Not Available");
        }
        HiiSetString (mSocketStringsHiiHandle, PcieStringTokens[TotalPortIdx].MaxLinkStatusToken, FinalStr, NULL);

        //
        // Update Link Speed
        //
        if (LinkWidth != 0) {
          UnicodeValueToStringS (ValueStr, STR_SIZE, LEFT_JUSTIFY, LinkSpeed, 2);
          StrCpyS (FinalStr, STR_SIZE/sizeof(CHAR16), L"Gen ");
          StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), ValueStr);

          switch (LinkSpeed) {
          //
          // Values for link speed come from PCI-E spec. Add new cases for future generations
          //
          case 1:
            StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), L" (2.5 GT/s)");
            break;
          case 2:
            StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), L" (5.0 GT/s)");
            break;
          case 3:
            StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), L" (8.0 GT/s)");
            break;
          case 4:
            StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), L" (16.0 GT/s)");
            break;
          default:
            StrCatS (FinalStr, STR_SIZE/sizeof(CHAR16), L" (Undefined)");
            break;
          }
        } else {
          StrCpyS (FinalStr, STR_SIZE/sizeof(CHAR16), L"Link Did Not Train");
        }
        HiiSetString (mSocketStringsHiiHandle, PcieStringTokens[TotalPortIdx].PcieSpeedStatusToken, FinalStr, NULL);

      } // for each port
  } // for each IioIndex
}

/**
  This function configures the IIO settings.
**/
VOID
ConfigIio (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       VarSize;
  UINT32      IioBitMap;
  UINT16      StackPresentBitmap;
  UINT8       Socket;
  UINT8       Stack;
  UINT8       PortIndex;
  struct SystemMemoryMapHob     *mSystemMemoryMap;
  SOCKET_IIO_CONFIGURATION      SocketIioConfiguration;
  SOCKET_MEMORY_CONFIGURATION   SocketMemoryConfiguration;
  SOCKET_COMMONRC_CONFIGURATION SocketCommonRcConfiguration;
  STATIC UINT32                 IioAttributes;
  STATIC UINT32                 CrcAttributes;
  STATIC UINT32                 SktMemAttributes;
  CPU_CSR_ACCESS_VAR            *CpuCsrAccessVarPtr;

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  mSystemMemoryMap = GetSystemMemoryMapData ();
  if (mSystemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return;
  }

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  VarSize = sizeof(SOCKET_IIO_CONFIGURATION);

  Status = gRT->GetVariable (
    SOCKET_IIO_CONFIGURATION_NAME,
    &gEfiSocketIioVariableGuid,
    &IioAttributes,
    &VarSize,
    &SocketIioConfiguration
    );
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof(SOCKET_COMMONRC_CONFIGURATION);

  Status = gRT->GetVariable (
    SOCKET_COMMONRC_CONFIGURATION_NAME,
    &gEfiSocketCommonRcVariableGuid,
    &CrcAttributes,
    &VarSize,
    &SocketCommonRcConfiguration
    );
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof(SOCKET_MEMORY_CONFIGURATION);

  Status = gRT->GetVariable (
    SOCKET_MEMORY_CONFIGURATION_NAME,
    &gEfiSocketMemoryVariableGuid,
    &SktMemAttributes,
    &VarSize,
    &SocketMemoryConfiguration
    );
  ASSERT_EFI_ERROR (Status);

  IioBitMap =  CpuCsrAccessVarPtr->socketPresentBitMap;  // storing value of IIO presence indicator byte in temporary variable
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((IioBitMap & (1 << Socket)) == 0) {
      SocketIioConfiguration.IioPresent[Socket] = 0;
    } else {
      SocketIioConfiguration.IioPresent[Socket] = 1;
    }

    //
    // Check which IIO stacks are present in the current sysytem
    //
    StackPresentBitmap =  CpuCsrAccessVarPtr->stackPresentBitmap[Socket];
    for (Stack = 0; Stack < MAX_STACKS_PER_SOCKET; Stack++) {
      if ((StackPresentBitmap & (1 << Stack)) == 0) {
        SocketIioConfiguration.IioStackPresent[Socket*MAX_STACKS_PER_SOCKET + Stack] = 0;
      } else {
        SocketIioConfiguration.IioStackPresent[Socket*MAX_STACKS_PER_SOCKET + Stack] = 1;
      }
    }

    if (Socket == 0) {
      // for now only recognize PCH on legacy socket
      SocketIioConfiguration.PchPresentOnStack[Socket] = 1;
      //
      // Check which PCH Rps can be assigned to VMD on present system
      //
      for (PortIndex = 0; PortIndex < MAX_VMD_ROOTPORTS_PER_PCH; PortIndex++) {
        if (IioVmdIsPchPortAllowed (Socket, PortIndex)) {
          SocketIioConfiguration.VMDPchPortAllowed[Socket*MAX_STACKS_PER_SOCKET + PortIndex] = 1;
        }
      }
    } else {
      SocketIioConfiguration.PchPresentOnStack[Socket] = 0;
    }
  } // for each socket

  SocketMemoryConfiguration.XMPProfilesSup     = mSystemMemoryMap->XMPProfilesSup;
  SocketCommonRcConfiguration.X2ApicForceEn    = X2ApicIdDetect (NULL);
  SocketCommonRcConfiguration.SystemRasType    = mIioUds->IioUdsPtr->SystemStatus.SystemRasType;
  SocketCommonRcConfiguration.NumCpus          = mIioUds->IioUdsPtr->SystemStatus.numCpus;
  SocketCommonRcConfiguration.OcCap            = (UINT8) mCpuPpmProtocol->CpuPpmStruct->Info->CapId4Csr.Bits.oc_enabled;
  SocketCommonRcConfiguration.IssMaxLevel      = mCpuVarDataPtr->IssConfigTdpMaxLevel;
  SocketCommonRcConfiguration.DcpmmEnable      = (UINT8) FeaturePcdGet (PcdCrystalRidgeEnable);
  SocketCommonRcConfiguration.HbmSku           = IsHbmSku () ? 1 : 0;
  SocketCommonRcConfiguration.PbfCapableSystem = mCpuVarDataPtr->PbfCapableSystem;
  SocketCommonRcConfiguration.DcuRtlWaEn       = 0;  // Default setting
  SocketCommonRcConfiguration.SstCpCapableSystem = (UINT8)(((B2P_READ_PM_CONFIG_OUTPUT_STRUCT *)(&mCpuVarDataPtr->SstCpSystemStatus))->Bits.SstCpCapability);

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_ICXSP;

    if (IsCpuAndRevision (CPU_ICXSP, REV_R0) || IsCpuAndRevision (CPU_ICXSP, REV_L0) || IsCpuAndRevision (CPU_ICXSP, REV_C0)) {
      SocketCommonRcConfiguration.DcuRtlWaEn = 1;  // Only enable for < ICX-SP D-stepping
    }
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_SNR;
  } else if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_CPX;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_SPRSP;
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_ICXD;
  } else if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_TNR;
  } else if (IsCpuAndRevision (CPU_CLX, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_CLX;
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_SKX;
  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_GNRSP;
  } else if (IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    SocketCommonRcConfiguration.CpuType = CPU_SRFSP;
  } else {
    DEBUG ((DEBUG_ERROR, "%a - Unknown Cpu Type!\n", __FUNCTION__));
    ASSERT (FALSE);
  }

  SocketCommonRcConfiguration.CpuChop = GetChopType (0);

  Status = gRT->SetVariable (
    SOCKET_IIO_CONFIGURATION_NAME,
    &gEfiSocketIioVariableGuid,
    IioAttributes,
    sizeof (SOCKET_IIO_CONFIGURATION),
    &SocketIioConfiguration
    );
  ASSERT_EFI_ERROR (Status);

  Status = gRT->SetVariable (
    SOCKET_MEMORY_CONFIGURATION_NAME,
    &gEfiSocketMemoryVariableGuid,
    SktMemAttributes,
    sizeof (SOCKET_MEMORY_CONFIGURATION),
    &SocketMemoryConfiguration
    );
  ASSERT_EFI_ERROR (Status);

  Status = gRT->SetVariable (
    SOCKET_COMMONRC_CONFIGURATION_NAME,
    &gEfiSocketCommonRcVariableGuid,
    CrcAttributes,
    sizeof (SOCKET_COMMONRC_CONFIGURATION),
    &SocketCommonRcConfiguration
    );
  ASSERT_EFI_ERROR (Status);
  return;
}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
VOID
UpdateIioPcieInformation (
  VOID
  )
{
  ConfigIio();
  ConfigPciePort();
}

/**

    Update Core Disable Bitmap in help

    @param None

    @retval None
    @retval None

**/
VOID
UpdateCoreDisableHelp (
  VOID
)
{
  EFI_STRING                              NewString;
  EFI_STRING                              CoreBitMaskString;
  EFI_STRING_ID                           TokenToUpdate;

  NewString = AllocateZeroPool (NEWSTRING_SIZE);
  CoreBitMaskString = AllocateZeroPool (NEWSTRING_SIZE);

  if((NewString == NULL) || (CoreBitMaskString == NULL)){
    if(NewString != NULL){
      gBS->FreePool(NewString);
    }
    if(CoreBitMaskString != NULL){
      gBS->FreePool (CoreBitMaskString);
    }
    return;
  }

  TokenToUpdate = STR_STR_CORE_DISABLE_BITMAP_HELP;
  StrCpyS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), L"0: Enable all cores. ");
  UnicodeValueToStringS (CoreBitMaskString, NEWSTRING_SIZE, RADIX_HEX, MAX_CORE_BITMASK, 0);
  StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), CoreBitMaskString);
  StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), L": Disable all cores");
//APTIOV_SERVER_OVERRIDE_RC_START: Notify user that disabling all cores is an invalid configuration.
  StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), L". NOTE: At least one core per CPU must be enabled. Disabling all cores is an invalid configuration.");
//APTIOV_SERVER_OVERRIDE_RC_END: Notify user that disabling all cores is an invalid configuration.
  HiiSetString(mSocketStringsHiiHandle, TokenToUpdate, NewString, NULL);

  gBS->FreePool (NewString);
  gBS->FreePool (CoreBitMaskString);
  return;
}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
VOID
EFIAPI
UpdateSocketDataEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS           Status;

  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiCpuPpmProtocolGuid, NULL, &mCpuPpmProtocol);
  ASSERT_EFI_ERROR (Status);

  mCpuVarDataPtr = GetCpuVarData ();

  UpdateCpuInformation();
  UpdateMemoryInformation();
  UpdateMktmeSetupInformation();
  UpdateTdxCapability();
  UpdateCFRConfig();
  UpdateIioPcieInformation();
  UpdateMpLinkInformation();
  UpdateCoreDisableHelp();
  return;
}

/**

    GC_TODO: add routine description

    @param None

    @retval TRUE  - GC_TODO: add retval description
    @retval FALSE - GC_TODO: add retval description

**/
VOID
UpdateSocketData (
  VOID
  )
{
  EFI_STATUS            Status;
  EFI_EVENT             DummyEvent;
  VOID                  *mSocketDummyRegistration;

  Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    UpdateSocketDataEvent,
                    NULL,
                    &DummyEvent
                    );
  ASSERT_EFI_ERROR (Status);
  //
  // Register protocol notifications on this event.
  //
  Status = gBS->RegisterProtocolNotify (
                    &gSignalBeforeEnterSetupGuid,
                    DummyEvent,
                    &mSocketDummyRegistration
                    );
  ASSERT_EFI_ERROR (Status);
  return;
}

/**

  Entry point of the SOCKET Embedded controller screen setup driver.

  @param ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  @param SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully

**/
EFI_STATUS
EFIAPI
InstallSocketSetupScreen (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                           Status;
  UINTN                                Index;
  EFI_IFR_GUID_LABEL                   *StartLabel;
  EFI_IFR_GUID_LABEL                   *EndLabel;
  BOOLEAN                              ActionFlag;
  EFI_STRING                           ConfigRequestHdr;
  UINT32                               RegEcx;
  UINTN                                BufferSize;
  UINT32                               Attributes;
  UINT8                                EistSupport;
  //
  //  APTIOV_SERVER_OVERRIDE_RC_START : Getting resources for Socket Configuration
  //
  EFI_HII_PACKAGE_LIST_HEADER *HiiPackageList = NULL;
  EFI_HII_DATABASE_PROTOCOL *HiiDatabase = NULL;
  //
  //  Get HiiPackageList header resource from handle.
  //
  Status = gBS->HandleProtocol(ImageHandle, &gEfiHiiPackageListProtocolGuid, (VOID**)&HiiPackageList);
  ASSERT_EFI_ERROR (Status);
  //
  //  APTIOV_SERVER_OVERRIDE_RC_END : Getting resources for Socket Configuration
  //

  //
  // Get socket PM config, check EIST support, and set result
  //
  BufferSize = sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION);

  Status = gRT->GetVariable (
                  (CHAR16 *)(SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME),
                  &gEfiSocketPowermanagementVarGuid,
                  &Attributes,
                  &BufferSize,
                  &mSocketPowermanagementConfiguration
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &RegEcx, NULL);
  EistSupport = (UINT8)((CPUID_VERSION_INFO_ECX*)&RegEcx)->Bits.EIST;
  mSocketPowermanagementConfiguration.ProcessorEistSupport = EistSupport;

  Status = gRT->SetVariable (
                  (CHAR16 *)(SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME),
                  &gEfiSocketPowermanagementVarGuid,
                  Attributes,
                  BufferSize,
                  &mSocketPowermanagementConfiguration
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mPrivateData.Signature = SOCKET_SETUP_SIGNATURE;
  mPrivateData.ImageHandle = ImageHandle;


  mPrivateData.DeviceHandle = NULL;

  mPrivateData.ConfigAccess.ExtractConfig = DefaultExtractConfig;
  mPrivateData.ConfigAccess.RouteConfig   = DefaultRouteConfig;
  mPrivateData.ConfigAccess.Callback      = CallbackRoutine;

  Status = SystemTable->BootServices->InstallMultipleProtocolInterfaces (
                  &mPrivateData.DeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mPrivateData.ConfigAccess,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);


  Status = SystemTable->BootServices->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID**)&mHiiConfigRouting);
  ASSERT_EFI_ERROR (Status);

  //
  //  APTIOV_SERVER_OVERRIDE_RC_START : Getting resources for Socket Configuration
  //
  #if 0
  mPrivateData.HiiHandle = HiiAddPackages (
                       &gSocketPkgListGuid,
                       mPrivateData.DeviceHandle,
                       SocketSetupStrings,
                       SocketSetupFormsBin,
                       NULL
                       );
  #endif
  //
  //  Locate HiiDataBase protocol
  //
  Status = gBS->LocateProtocol(&gEfiHiiDatabaseProtocolGuid, NULL, (VOID**)&HiiDatabase);
  ASSERT_EFI_ERROR (Status);

  //
  // Register SocketSetup package list with the HII Database
  //
  Status = HiiDatabase->NewPackageList (
                     HiiDatabase, 
                     HiiPackageList, 
                     mPrivateData.DeviceHandle, 
                     &mPrivateData.HiiHandle
                     );
  ASSERT_EFI_ERROR (Status);
  //
  //  APTIOV_SERVER_OVERRIDE_RC_END : Getting resources for Socket Configuration
  //

   ASSERT(mPrivateData.HiiHandle != NULL);

  //
  // Initialize the container for dynamic opcodes
  //
  mPrivateData.StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (mPrivateData.StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "\n ERROR!!! HiiAllocateOpCodeHandle () - Returned NULL Pointer for mPrivateData.StartOpCodeHandle\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  mPrivateData.EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (mPrivateData.EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "\n ERROR!!! HiiAllocateOpCodeHandle () - Returned NULL Pointer for mPrivateData.EndOpCodeHandle\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Create Hii Extend Label OpCode as the Top opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (mPrivateData.StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = VFR_FORMLABLE_SOCKET_TOP;

  //
  // Create Hii Extend Label OpCode as the Bottom opcode
  //
  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (mPrivateData.EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = VFR_FORMLABLE_SOCKET_BOTTOM;

  if (IsCMOSBad()) {
    for (Index = 0; Index < MAX_SOCKET_VAR_INFO; Index ++) {
      ConfigRequestHdr = HiiConstructConfigHdr (SocketVariableInfo[Index].Guid,  SocketVariableInfo[Index].VariableName, mPrivateData.DeviceHandle);
      ASSERT (ConfigRequestHdr != NULL);
      ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
      ASSERT (ActionFlag);

    }
  }

  mSocketStringsHiiHandle = mPrivateData.HiiHandle;
  UpdateSocketData();

  return EFI_SUCCESS;
}
