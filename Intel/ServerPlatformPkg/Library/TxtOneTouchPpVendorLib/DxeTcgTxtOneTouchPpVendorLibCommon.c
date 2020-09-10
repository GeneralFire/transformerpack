/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Protocol/VariableLock.h>

#include "TxtOneTouchData.h"
#include "PlatformSetup.h"
#include "DxeTcgTxtOneTouchPpVendorLibCommon.h"

#define FLAG_RESET_TRACK                                  BIT31

#define PP_OPERATION_RESPONSE_SUCCESS              0x0
#define PP_OPERATION_RESPONSE_USER_ABORT           0xFFFFFFF0
#define PP_OPERATION_RESPONSE_BIOS_FAILURE         0xFFFFFFF1

#define CONFIRM_BUFFER_SIZE         4096

EFI_GUID gTxtOneTouchFlagsGuid = TXT_ONE_TOUCH_FLAGS_GUID;
EFI_GUID gSetupVariableGuid    = SETUP_VARIABLE_GUID;

EFI_HII_HANDLE mTxtOneTouchStringPackHandle;

//
// Bit definitions for MSR_IA32_FEATURE_CONTROL (ECX = 3Ah)
//
#define EFI_MSR_IA32_FEATURE_CONTROL          0x0000003A
#define B_MSR_FEATURE_CONTROL_LOCK            BIT0
#define B_MSR_ENABLE_VMX_INSIDE_SMX           BIT1
#define B_MSR_ENABLE_VMX_OUTSIDE_SMX          BIT2
#define B_MSR_SENTER_ENABLE                   (BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9|BIT8)
#define B_MSR_ALL_MASK                        (B_MSR_ENABLE_VMX_INSIDE_SMX | B_MSR_ENABLE_VMX_OUTSIDE_SMX | B_MSR_SENTER_ENABLE)

typedef struct {
  UINT8  ClearMask;
  UINT8  SetMask;
} MASK_TABLE;
// Bit definition in MASK_TABLE
#define B_TPM_ENABLE        BIT0
#define B_VMXINSMX_ENABLE   BIT1
#define B_VMXOUTSMX_ENABLE  BIT2
#define B_SENTER_ENABLE     BIT3
#define B_VTD_ENABLE        BIT4
#define B_TXT_ENABLE        BIT5
#define B_STM_ENABLE        BIT6

UINT64 mPPRegisterExpectedFeatureControlMsr[8] = {
  0,                                                                                // SENTER_0__VMXINSMX_0__VMXOUTSMX_0            150
  B_MSR_ENABLE_VMX_INSIDE_SMX,                                                      // SENTER_0__VMXINSMX_1__VMXOUTSMX_0            151
  B_MSR_ENABLE_VMX_OUTSIDE_SMX,                                                     // SENTER_0__VMXINSMX_0__VMXOUTSMX_1            152
  B_MSR_ENABLE_VMX_INSIDE_SMX | B_MSR_ENABLE_VMX_OUTSIDE_SMX,                       // SENTER_0__VMXINSMX_1__VMXOUTSMX_1            153
  B_MSR_SENTER_ENABLE,                                                              // SENTER_1__VMXINSMX_0__VMXOUTSMX_0            154
  B_MSR_SENTER_ENABLE | B_MSR_ENABLE_VMX_INSIDE_SMX,                                // SENTER_1__VMXINSMX_1__VMXOUTSMX_0            155
  B_MSR_SENTER_ENABLE | B_MSR_ENABLE_VMX_OUTSIDE_SMX,                               // SENTER_1__VMXINSMX_0__VMXOUTSMX_1            156
  B_MSR_SENTER_ENABLE | B_MSR_ENABLE_VMX_INSIDE_SMX | B_MSR_ENABLE_VMX_OUTSIDE_SMX, // SENTER_1__VMXINSMX_1__VMXOUTSMX_1            157
};

MASK_TABLE mPPRegisterExpectSetupBitmap[11] = {
  {
    B_SENTER_ENABLE | B_VMXINSMX_ENABLE | B_VMXOUTSMX_ENABLE,
    0
  }, // SENTER_0__VMXINSMX_0__VMXOUTSMX_0            150
  {
    B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE,
    B_VMXINSMX_ENABLE
  }, // SENTER_0__VMXINSMX_1__VMXOUTSMX_0            151
  {
    B_SENTER_ENABLE | B_VMXINSMX_ENABLE,
    B_VMXOUTSMX_ENABLE
  }, // SENTER_0__VMXINSMX_0__VMXOUTSMX_1            152
  {
    B_SENTER_ENABLE,
    B_VMXINSMX_ENABLE | B_VMXOUTSMX_ENABLE
  }, // SENTER_0__VMXINSMX_1__VMXOUTSMX_1            153
  {
    B_VMXINSMX_ENABLE | B_VMXOUTSMX_ENABLE,
    B_SENTER_ENABLE
  }, // SENTER_1__VMXINSMX_0__VMXOUTSMX_0            154
  {
    B_VMXOUTSMX_ENABLE,
    B_SENTER_ENABLE | B_VMXINSMX_ENABLE,
  }, // SENTER_1__VMXINSMX_1__VMXOUTSMX_0            155
  {
    B_VMXINSMX_ENABLE,
    B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE
  }, // SENTER_1__VMXINSMX_0__VMXOUTSMX_1            156
  {
    0,
    B_SENTER_ENABLE | B_VMXINSMX_ENABLE | B_VMXOUTSMX_ENABLE
  }, // SENTER_1__VMXINSMX_1__VMXOUTSMX_1            157
};

MASK_TABLE mPPVariableExpectSetupBitmap[11] = {
  {
    0,
    B_VMXINSMX_ENABLE | B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE
  }, // TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X 130
  {
    B_VMXINSMX_ENABLE | B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE | B_TXT_ENABLE | B_STM_ENABLE,
    0
  }, // TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0 131
  {
    0,
    B_VTD_ENABLE
  }, // TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X 132
  {
    B_VTD_ENABLE | B_TXT_ENABLE | B_STM_ENABLE,
    0
  }, // TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0 133
  {
    0,
    B_VMXINSMX_ENABLE | B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE | B_VTD_ENABLE
  }, // TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X 134
  {
    B_VMXINSMX_ENABLE | B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE | B_VTD_ENABLE | B_TXT_ENABLE | B_STM_ENABLE,
    0,
  }, // TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0 135
  {
    B_VMXOUTSMX_ENABLE | B_STM_ENABLE,
    B_TPM_ENABLE | B_VMXINSMX_ENABLE | B_SENTER_ENABLE | B_VTD_ENABLE | B_TXT_ENABLE,
  }, // TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0 136
  {
    B_STM_ENABLE,
    B_TPM_ENABLE | B_VMXINSMX_ENABLE | B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE | B_VTD_ENABLE | B_TXT_ENABLE,
  }, // TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0 137
  {
    0,
    B_TPM_ENABLE | B_VMXINSMX_ENABLE | B_SENTER_ENABLE | B_VMXOUTSMX_ENABLE | B_VTD_ENABLE | B_TXT_ENABLE | B_STM_ENABLE,
  }, // TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1 138
  {
    B_STM_ENABLE,
    0
  }, // TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0 139
  {
    B_TXT_ENABLE | B_STM_ENABLE,
    0
  }, // TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0 140
};

// The order must match bit definition in MASK_TABLE
UINT32 mOffsetTable[7] = {
  OFFSET_OF(SYSTEM_CONFIGURATION,TpmEnable),
  OFFSET_OF(SYSTEM_CONFIGURATION,VmxInSmxEnable),
  OFFSET_OF(SYSTEM_CONFIGURATION,VmxOutSmxEnable),
  OFFSET_OF(SYSTEM_CONFIGURATION,SenterEnable),
  OFFSET_OF(SYSTEM_CONFIGURATION,VTdEnable),
  OFFSET_OF(SYSTEM_CONFIGURATION,TxtEnable),
  OFFSET_OF(SYSTEM_CONFIGURATION,StmEnable),
};

/**
  Execute Vendor physical presence operation requested by the OS.

  @param[in]      CommandCode      Physical presence operation value.
  @param[in, out] PpiFlags         The physical presence interface flags.
  @param[out]     ResetRequired    If reset is required to vendor settings in effect.
                                   True, it indicates the reset is required.
                                   False, it indicates the reset is not required.
  @param[in, out] TxtOneTouchFlags The TXT one touch flags.

  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Unknown physical presence operation.
  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Error occurred during sending command to TPM or
                                                     receiving response from TPM.
  @retval Others                                     Return code from the TPM device after command execution.

**/
UINT32
TxtOneTouchExecutePhysicalPresence (
  IN UINT32                     CommandCode,
  IN OUT UINT32                 *PpiFlags,
  OUT BOOLEAN                   *ResetRequired,
  IN OUT TXT_ONE_TOUCH_FLAGS    *TxtOneTouchFlags
  );

/**
  Read the specified key for user confirmation.

  @param[in]  CautionKey  If true,  F12 is used as confirm key;
                          If false, F10 is used as confirm key.

  @retval     TRUE        User confirmed the changes by input.
  @retval     FALSE       User discarded the changes or device error.

**/
BOOLEAN
TxtOneTouchReadUserKey (
  IN     BOOLEAN                    CautionKey
  )
{
  EFI_STATUS                        Status;
  EFI_INPUT_KEY                     Key;
  UINT16                            InputKey;
  UINTN                             Index;

  InputKey = 0;
  do {
    Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
    if (Status == EFI_NOT_READY) {
      gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
      continue;
    }

    if (Status == EFI_DEVICE_ERROR) {
      return FALSE;
    }

    if (Key.ScanCode == SCAN_ESC) {
      InputKey = Key.ScanCode;
    }
    if ((Key.ScanCode == SCAN_F10) && !CautionKey) {
      InputKey = Key.ScanCode;
    }
    if ((Key.ScanCode == SCAN_F12) && CautionKey) {
      InputKey = Key.ScanCode;
    }
  } while (InputKey == 0);

  if (InputKey != SCAN_ESC) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get string by string id from HII Interface.

  @param[in] Id          String ID.

  @retval    CHAR16 *    String from ID.
  @retval    NULL        If error occurs.

**/
CHAR16 *
TxtOneTouchPhysicalPresenceGetStringById (
  IN  EFI_STRING_ID   Id
  )
{
  return HiiGetString (mTxtOneTouchStringPackHandle, Id, NULL);
}

/**
  Convert setup data to bitmap.

  @param[in] SetupData   Setup variable data.

  @retval    Bitmap data
**/
UINT8
SetupToBitmap (
  IN SYSTEM_CONFIGURATION *SetupData
  )
{
  UINT8 Bitmap;
  UINTN Index;
  UINT8 *Buffer;

  Bitmap = 0;
  Buffer = (UINT8 *)SetupData;
  for (Index = 0; Index < sizeof(mOffsetTable)/sizeof(mOffsetTable[0]); Index++) {
    if (Buffer[mOffsetTable[Index]] != 0) {
      Bitmap |= (1 << Index);
    }
  }
  return Bitmap;
}

/**
  Convert bitmap to setup data.

  @param[in]      Bitmap      Bitmap data.
  @param[in, out] SetupData   Setup variable data.
**/
VOID
BitmapToSetup (
  IN     UINT8                Bitmap,
  IN OUT SYSTEM_CONFIGURATION *SetupData
  )
{
  UINTN Index;
  UINT8 *Buffer;

  Buffer = (UINT8 *)SetupData;
  for (Index = 0; Index < sizeof(mOffsetTable)/sizeof(mOffsetTable[0]); Index++) {
    if ((Bitmap & (1 << Index)) != 0) {
      Buffer[mOffsetTable[Index]] = 1;
    } else {
      Buffer[mOffsetTable[Index]] = 0;
    }
  }
}

/**
  Execute Vendor physical presence variable operation requested by the OS.

  @param[in]      CommandCode      Physical presence operation value.
  @param[in, out] PpiFlags         The physical presence interface flags.
  @param[out]     ResetRequired    If reset is required to vendor settings in effect.
                                   True, it indicates the reset is required.
                                   False, it indicates the reset is not required.
  @param[in, out] TxtOneTouchFlags The TXT one touch flags.

  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Unknown physical presence operation.
  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Error occurred during sending command to TPM or
                                                     receiving response from TPM.
  @retval Others                                     Return code from the TPM device after command execution.
**/
UINT32
TxtOneTouchExecutePhysicalPresenceRegister (
  IN UINT32                     CommandCode,
  IN OUT UINT32                 *PpiFlags,
  OUT BOOLEAN                   *ResetRequired,
  IN OUT TXT_ONE_TOUCH_FLAGS    *TxtOneTouchFlags
  )
{
  UINT64               MsrValue;
  UINT64               ExpectedMsrValue;
  EFI_STATUS           Status;
  SYSTEM_CONFIGURATION SetupData;
  UINTN                SetupVariableSize;
  UINT32               SetupVariableAttributes;
  MASK_TABLE           ExpectedSetupBitmap;
  UINT8                Bitmap;

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] TxtOneTouchExecutePhysicalPresenceRegister\n"));

  ASSERT ((CommandCode >= SENTER_0__VMXINSMX_0__VMXOUTSMX_0) && (CommandCode <= SENTER_1__VMXINSMX_1__VMXOUTSMX_1));

  MsrValue = AsmReadMsr64 (EFI_MSR_IA32_FEATURE_CONTROL);
  DEBUG ((EFI_D_INFO, "[TxtOneTouch] MsrValue - 0x%016lx\n", MsrValue));

  ExpectedMsrValue = mPPRegisterExpectedFeatureControlMsr[CommandCode - SENTER_0__VMXINSMX_0__VMXOUTSMX_0];
  DEBUG ((EFI_D_INFO, "[TxtOneTouch] ExpectedMsrValue - 0x%016lx\n", ExpectedMsrValue));

  if ((MsrValue & B_MSR_FEATURE_CONTROL_LOCK) == 0) {
    MsrValue &= ~((UINT64) B_MSR_ALL_MASK);
    MsrValue |= ExpectedMsrValue | B_MSR_FEATURE_CONTROL_LOCK;
    AsmWriteMsr64 (EFI_MSR_IA32_FEATURE_CONTROL, MsrValue);
    *ResetRequired = FALSE;
    return PP_OPERATION_RESPONSE_SUCCESS;
  }

  if ((MsrValue & B_MSR_ALL_MASK) == ExpectedMsrValue) {
    *ResetRequired = FALSE;
    return PP_OPERATION_RESPONSE_SUCCESS;
  }

  ExpectedSetupBitmap = mPPRegisterExpectSetupBitmap[CommandCode - SENTER_0__VMXINSMX_0__VMXOUTSMX_0];

  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                  SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[TxtOneTouch] Variable - %s not found!\n", SETUP_VARIABLE_NAME));
    return PP_OPERATION_RESPONSE_BIOS_FAILURE;
  }

  Bitmap = SetupToBitmap (&SetupData);
  DEBUG ((EFI_D_INFO, "[TxtOneTouch] Bitmap - 0x%02x\n", Bitmap));
  Bitmap = (Bitmap & (~ExpectedSetupBitmap.ClearMask)) | ExpectedSetupBitmap.SetMask;
  DEBUG ((EFI_D_INFO, "[TxtOneTouch] Updated Bitmap - 0x%02x\n", Bitmap));
  BitmapToSetup (Bitmap, &SetupData);
  Status = gRT->SetVariable (
                  SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  SetupVariableAttributes,
                  SetupVariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    return PP_OPERATION_RESPONSE_BIOS_FAILURE;
  }
  *ResetRequired = TRUE;

  return PP_OPERATION_RESPONSE_SUCCESS;
}

/**
  Execute Vendor physical presence variable operation requested by the OS.

  @param[in]      CommandCode      Physical presence operation value.
  @param[in, out] PpiFlags         The physical presence interface flags.
  @param[out]     ResetRequired    If reset is required to vendor settings in effect.
                                   True, it indicates the reset is required.
                                   False, it indicates the reset is not required.
  @param[in, out] TxtOneTouchFlags The TXT one touch flags.

  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Unknown physical presence operation.
  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Error occurred during sending command to TPM or
                                                     receiving response from TPM.
  @retval Others                                     Return code from the TPM device after command execution.
**/
UINT32
TxtOneTouchExecutePhysicalPresenceVariable (
  IN UINT32                     CommandCode,
  IN OUT UINT32                 *PpiFlags,
  OUT BOOLEAN                   *ResetRequired,
  IN OUT TXT_ONE_TOUCH_FLAGS    *TxtOneTouchFlags
  )
{
  EFI_STATUS           Status;
  SYSTEM_CONFIGURATION SetupData;
  UINTN                SetupVariableSize;
  UINT32               SetupVariableAttributes;
  MASK_TABLE           ExpectedSetupBitmap;
  UINT8                Bitmap;

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] TxtOneTouchExecutePhysicalPresenceVariable\n"));

  ASSERT ((CommandCode >= TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X) && (CommandCode <= TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0));
  ExpectedSetupBitmap = mPPVariableExpectSetupBitmap[CommandCode - TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X];

  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                  SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[TxtOneTouch] Variable - %s not found!\n", SETUP_VARIABLE_NAME));
    return PP_OPERATION_RESPONSE_BIOS_FAILURE;
  }

  Bitmap = SetupToBitmap (&SetupData);
  DEBUG ((EFI_D_INFO, "[TxtOneTouch] Bitmap - 0x%02x\n", Bitmap));
  Bitmap = (Bitmap & (~ExpectedSetupBitmap.ClearMask)) | ExpectedSetupBitmap.SetMask;
  DEBUG ((EFI_D_INFO, "[TxtOneTouch] Updated Bitmap - 0x%02x\n", Bitmap));
  BitmapToSetup (Bitmap, &SetupData);
  Status = gRT->SetVariable (
                  SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  SetupVariableAttributes,
                  SetupVariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    return PP_OPERATION_RESPONSE_BIOS_FAILURE;
  }
  *ResetRequired = TRUE;

  return PP_OPERATION_RESPONSE_SUCCESS;
}

/**
  Execute Vendor physical presence variable operation requested by the OS.

  @param[in]      CommandCode      Physical presence operation value.
  @param[in, out] PpiFlags         The physical presence interface flags.
  @param[out]     ResetRequired    If reset is required to vendor settings in effect.
                                   True, it indicates the reset is required.
                                   False, it indicates the reset is not required.
  @param[in, out] TxtOneTouchFlags The TXT one touch flags.

  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Unknown physical presence operation.
  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Error occurred during sending command to TPM or
                                                     receiving response from TPM.
  @retval Others                                     Return code from the TPM device after command execution.
**/
UINT32
TxtOneTouchExecutePhysicalPresenceBiosAc (
  IN UINT32                     CommandCode,
  IN OUT UINT32                 *PpiFlags,
  OUT BOOLEAN                   *ResetRequired,
  IN OUT TXT_ONE_TOUCH_FLAGS    *TxtOneTouchFlags
  )
{
  UINT32                        TpmResponse;

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] TxtOneTouchExecutePhysicalPresenceBiosAc\n"));

  //
  // TBD: Need call BIOS AC to below operation
  //
  switch (CommandCode) {
  case EXEC_BOISAC_RESET_ESTABLISHMENT_BIT:
    TpmResponse = PP_OPERATION_RESPONSE_BIOS_FAILURE;
    break;
  case EXEC_BOISAC_RESET_AUX_INDEX:
    TpmResponse = PP_OPERATION_RESPONSE_BIOS_FAILURE;
    break;
  default:
    TpmResponse = PP_OPERATION_RESPONSE_BIOS_FAILURE;
    break;
  }

  return TpmResponse;
}

/**
  Execute Vendor physical presence operation requested by the OS.

  @param[in]      CommandCode      Physical presence operation value.
  @param[in, out] PpiFlags         The physical presence interface flags.
  @param[out]     ResetRequired    If reset is required to vendor settings in effect.
                                   True, it indicates the reset is required.
                                   False, it indicates the reset is not required.
  @param[in, out] TxtOneTouchFlags The TXT one touch flags.

  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Unknown physical presence operation.
  @retval PP_OPERATION_RESPONSE_BIOS_FAILURE         Error occurred during sending command to TPM or
                                                     receiving response from TPM.
  @retval Others                                     Return code from the TPM device after command execution.

**/
UINT32
TxtOneTouchExecutePhysicalPresence (
  IN UINT32                     CommandCode,
  IN OUT UINT32                 *PpiFlags,
  OUT BOOLEAN                   *ResetRequired,
  IN OUT TXT_ONE_TOUCH_FLAGS    *TxtOneTouchFlags
  )
{
  switch (CommandCode) {
  case SET_NO_TXT_MAINTENANCE_FALSE:
    TxtOneTouchFlags->Flags &= ~TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_MAINTENANCE;
    break;
  case SET_NO_TXT_MAINTENANCE_TRUE:
    TxtOneTouchFlags->Flags |= TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_MAINTENANCE;
    break;

  case SET_NO_TXT_REFURBISHING_FALSE:
    TxtOneTouchFlags->Flags &= ~TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_REFURBISHING;
    break;
  case SET_NO_TXT_REFURBISHING_TRUE:
    TxtOneTouchFlags->Flags |= TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_REFURBISHING;
    break;

  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0:
  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0:
    return TxtOneTouchExecutePhysicalPresenceVariable (CommandCode, PpiFlags, ResetRequired, TxtOneTouchFlags);
    break;

  case SENTER_0__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_1:
    return TxtOneTouchExecutePhysicalPresenceRegister (CommandCode, PpiFlags, ResetRequired, TxtOneTouchFlags);
    break;

  case EXEC_BOISAC_RESET_ESTABLISHMENT_BIT:
  case EXEC_BOISAC_RESET_AUX_INDEX:
    return TxtOneTouchExecutePhysicalPresenceBiosAc (CommandCode, PpiFlags, ResetRequired, TxtOneTouchFlags);
    break;

  default:
    return PP_OPERATION_RESPONSE_BIOS_FAILURE;
    break;
  }

  *ResetRequired = TRUE;
  return PP_OPERATION_RESPONSE_SUCCESS;
}

/**
  Display the confirm text and get user confirmation.

  @param[in] TpmPpCommand  The requested TPM physical presence command.

  @retval    TRUE          The user has confirmed the changes.
  @retval    FALSE         The user doesn't confirm the changes.
**/
BOOLEAN
TxtOneTouchUserConfirm (
  IN      UINT32                    TpmPpCommand
  )
{
  CHAR16                            *ConfirmText;
  CHAR16                            *TmpStr1;
  CHAR16                            *TmpStr2;
  UINTN                             BufSize;
  BOOLEAN                           CautionKey;
  UINT16                            Index;
  CHAR16                            DstStr[81];

  TmpStr2     = NULL;
  CautionKey  = FALSE;
  BufSize     = CONFIRM_BUFFER_SIZE;
  ConfirmText = AllocateZeroPool (BufSize);
  ASSERT (ConfirmText != NULL);
  if (ConfirmText == NULL) {
    return FALSE;
  }

  TmpStr1 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_PPI_HEAD_STR));

  switch (TpmPpCommand) {
    case SET_NO_TXT_MAINTENANCE_FALSE:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SET_NO_TXT_MAINTENANCE_FALSE));
      break;
    case SET_NO_TXT_MAINTENANCE_TRUE:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SET_NO_TXT_MAINTENANCE_TRUE));
      break;
    case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X));
      break;
    case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0));
      break;
    case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X));
      break;
    case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0));
      break;
    case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X));
      break;
    case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0));
      break;
    case TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0));
      break;
    case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0));
      break;
    case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1));
      break;
    case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0));
      break;
    case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0));
      break;
    case SENTER_0__VMXINSMX_0__VMXOUTSMX_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_0__VMXINSMX_0__VMXOUTSMX_0));
      break;
    case SENTER_0__VMXINSMX_1__VMXOUTSMX_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_0__VMXINSMX_1__VMXOUTSMX_0));
      break;
    case SENTER_0__VMXINSMX_0__VMXOUTSMX_1:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_0__VMXINSMX_0__VMXOUTSMX_1));
      break;
    case SENTER_0__VMXINSMX_1__VMXOUTSMX_1:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_0__VMXINSMX_1__VMXOUTSMX_1));
        break;
    case SENTER_1__VMXINSMX_0__VMXOUTSMX_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_1__VMXINSMX_0__VMXOUTSMX_0));
      break;
    case SENTER_1__VMXINSMX_1__VMXOUTSMX_0:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_1__VMXINSMX_1__VMXOUTSMX_0));
      break;
    case SENTER_1__VMXINSMX_0__VMXOUTSMX_1:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_1__VMXINSMX_0__VMXOUTSMX_1));
      break;
    case SENTER_1__VMXINSMX_1__VMXOUTSMX_1:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SENTER_1__VMXINSMX_1__VMXOUTSMX_1));
      break;
    case SET_NO_TXT_REFURBISHING_FALSE:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SET_NO_TXT_REFURBISHING_FALSE));
      break;
    case SET_NO_TXT_REFURBISHING_TRUE:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_SET_NO_TXT_REFURBISHING_TRUE));
      break;
    case EXEC_BOISAC_RESET_ESTABLISHMENT_BIT:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_EXEC_BIOSAC_RESET_ESTABLISHMENT_BIT));
      break;
    case EXEC_BOISAC_RESET_AUX_INDEX:
      TmpStr2 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_EXEC_BIOSAC_RESET_AUX_INDEX));
      break;

    default:
      return FALSE;
  }

  TmpStr1 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_PPI_HEAD_STR));
  ASSERT (TmpStr1 != NULL);
  if (TmpStr1 == NULL) {
    return FALSE;
  }
  UnicodeSPrint (ConfirmText, BufSize, TmpStr1, TmpStr2);
  FreePool (TmpStr1);

  TmpStr1 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_ACCEPT_KEY));
  ASSERT (TmpStr1 != NULL);
  if (TmpStr1 == NULL) {
    return FALSE;
  }
  StrnCatS (
    ConfirmText,
    BufSize / sizeof (CHAR16),
    TmpStr1,
    BufSize / sizeof (CHAR16) - StrLen (ConfirmText) - 1
    );
  FreePool (TmpStr1);

  if (TmpStr2 == NULL) {
    FreePool (ConfirmText);
    return FALSE;
  }

  TmpStr1 = TxtOneTouchPhysicalPresenceGetStringById (STRING_TOKEN (VENDOR_TPM_REJECT_KEY));
  ASSERT (TmpStr1 != NULL);
  if (TmpStr1 == NULL) {
    return FALSE;
  }
  BufSize -= StrSize (ConfirmText);
  UnicodeSPrint (ConfirmText + StrLen (ConfirmText), BufSize, TmpStr1, TmpStr2);

  for (Index = 0; Index < StrLen (ConfirmText); Index += 80) {
    StrnCpyS (
      DstStr,
      sizeof (DstStr) / sizeof (DstStr[0]),
      ConfirmText + Index,
      80
      );
    Print (DstStr);
  }

  FreePool (TmpStr1);
  FreePool (TmpStr2);
  FreePool (ConfirmText);

  if (TxtOneTouchReadUserKey (CautionKey)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check and execute the requested physical presence command.

  This API should be invoked in BIOS boot phase to process pending request.

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in, out] ManagementFlags  BIOS TPM Management Flags.
  @param[out]     ResetRequired    If reset is required to vendor settings in effect.
                                   True, it indicates the reset is required.
                                   False, it indicates the reset is not required.

  @return TPM Operation Response to OS Environment.
**/
UINT32
EFIAPI
TxtOneTouchLibExecutePendingRequest (
  IN UINT32                 OperationRequest,
  IN OUT UINT32             *ManagementFlags,
  OUT BOOLEAN               *ResetRequired
  )
{
  UINT32                            PPResponse;
  BOOLEAN                           RequestConfirmed;
  UINTN                             DataSize;
  TXT_ONE_TOUCH_FLAGS               TxtOneTouchFlags;
  EFI_STATUS                        Status;
  EDKII_VARIABLE_LOCK_PROTOCOL      *VariableLockProtocol;

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] TxtOneTouchLibExecutePendingRequest\n"));

  DataSize = sizeof(TXT_ONE_TOUCH_FLAGS);
  Status = gRT->GetVariable (
                  TXT_ONE_TOUCH_FLAGS_VARIABLE,
                  &gTxtOneTouchFlagsGuid,
                  NULL,
                  &DataSize,
                  &TxtOneTouchFlags
                  );
  if (EFI_ERROR (Status)) {
    TxtOneTouchFlags.Flags = 0;
    Status = gRT->SetVariable (
                    TXT_ONE_TOUCH_FLAGS_VARIABLE,
                    &gTxtOneTouchFlagsGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    DataSize,
                    &TxtOneTouchFlags
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[TxtOneTouch] Set TxtOneTouch flag failed, Status = %r\n", Status));
      return PP_OPERATION_RESPONSE_BIOS_FAILURE;
    }
  }

  //
  // This flags variable controls whether physical presence is required for TPM command.
  // It should be protected from malicious software. We set it as read-only variable here.
  //
  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLockProtocol);
  if (!EFI_ERROR (Status)) {
    Status = VariableLockProtocol->RequestToLock (
                                     VariableLockProtocol,
                                     TXT_ONE_TOUCH_FLAGS_VARIABLE,
                                     &gTxtOneTouchFlagsGuid
                                     );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[TxtOneTouch] Error when lock variable %s, Status = %r\n", TXT_ONE_TOUCH_FLAGS_VARIABLE, Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] IN Request=%x, Flags=%x, TxtOneTouchFlags=%x\n", OperationRequest, *ManagementFlags, TxtOneTouchFlags.Flags));

  if (!TxtOneTouchLibHasValidRequest(OperationRequest, *ManagementFlags, &RequestConfirmed)) {
    return PP_OPERATION_RESPONSE_BIOS_FAILURE;
  }

  if (!RequestConfirmed) {
    //
    // Print confirm text and wait for approval.
    //
    RequestConfirmed = TxtOneTouchUserConfirm (OperationRequest);
  }
  PPResponse = PP_OPERATION_RESPONSE_USER_ABORT;
  if (RequestConfirmed) {
    PPResponse = TxtOneTouchExecutePhysicalPresence (OperationRequest, ManagementFlags, ResetRequired, &TxtOneTouchFlags);
  }

  Status = gRT->SetVariable (
                  TXT_ONE_TOUCH_FLAGS_VARIABLE,
                  &gTxtOneTouchFlagsGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  DataSize,
                  &TxtOneTouchFlags
                  );

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] OUT: Flags=%x, ResetRequired=%x, TxtOneTouchFlags=%x, \n", *ManagementFlags, ResetRequired, TxtOneTouchFlags.Flags));

  return PPResponse;
}

/**
  Check if there is a valid physical presence command request.

  This API should be invoked in BIOS boot phase to process pending request.

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in]      ManagementFlags  BIOS TPM Management Flags.
  @param[out]     RequestConfirmed If the physical presence operation command required user confirm from UI.
                                   True, it indicates the command doesn't require user confirm.
                                   False, it indicates the command need user confirm from UI.

  @retval  TRUE        Physical Presence operation command is valid.
  @retval  FALSE       Physical Presence operation command is invalid.
**/
BOOLEAN
EFIAPI
TxtOneTouchLibHasValidRequest (
  IN UINT32                 OperationRequest,
  IN UINT32                 ManagementFlags,
  OUT BOOLEAN               *RequestConfirmed
  )
{
  UINTN                  DataSize;
  TXT_ONE_TOUCH_FLAGS    TxtOneTouchFlags;
  EFI_STATUS             Status;

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] TxtOneTouchLibHasValidRequest\n"));

  DataSize = sizeof(TXT_ONE_TOUCH_FLAGS);
  Status = gRT->GetVariable (
                  TXT_ONE_TOUCH_FLAGS_VARIABLE,
                  &gTxtOneTouchFlagsGuid,
                  NULL,
                  &DataSize,
                  &TxtOneTouchFlags
                  );
  if (EFI_ERROR (Status)) {
    TxtOneTouchFlags.Flags = 0;
  }

  DEBUG ((EFI_D_INFO, "[TxtOneTouch] Request=%x, Flags=%x, TxtOneTouchFlags=%x\n", OperationRequest, ManagementFlags, TxtOneTouchFlags.Flags));

  *RequestConfirmed = FALSE;

  switch (OperationRequest) {
  case SET_NO_TXT_MAINTENANCE_FALSE :
  case SET_NO_TXT_REFURBISHING_FALSE:
    *RequestConfirmed = TRUE;
    break;
  case SET_NO_TXT_MAINTENANCE_TRUE  :
  case SET_NO_TXT_REFURBISHING_TRUE:
    *RequestConfirmed = FALSE;
    break;

  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0:
  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0:
  case SENTER_0__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_1:
    if ((TxtOneTouchFlags.Flags & TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_MAINTENANCE) != 0) {
      *RequestConfirmed = TRUE;
    } else {
      *RequestConfirmed = FALSE;
    }
     break;

  case EXEC_BOISAC_RESET_ESTABLISHMENT_BIT:
  case EXEC_BOISAC_RESET_AUX_INDEX:
    if ((TxtOneTouchFlags.Flags & TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_REFURBISHING) == 0) {
      *RequestConfirmed = TRUE;
    } else {
      *RequestConfirmed = FALSE;
    }
    break;

  default:
    //
    // Wrong Physical Presence command
    //
    return FALSE;
  }

  if ((ManagementFlags & FLAG_RESET_TRACK) != 0) {
    //
    // It had been confirmed in last boot, it doesn't need confirm again.
    //
    *RequestConfirmed = TRUE;
  }
  DEBUG ((EFI_D_INFO, "[TxtOneTouch] RequestConfirmed=%x\n", *RequestConfirmed));

  //
  // Physical Presence command is correct
  //
  return TRUE;
}

