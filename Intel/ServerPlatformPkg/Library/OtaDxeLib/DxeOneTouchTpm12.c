/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/TimerLib.h>

#include "DxeOobOneTouch.h"
#include "DxeOneTouchPlatform.h"
#include "DxeOneTouchTpm12.h"

//-----------------------------------------------------------------------------------------------------
/**
  Issue a TPM command for which no additional output data will be returned.

  @param[in] TcgProtocol             EFI TCG Protocol instance
  @param[in] Ordinal                 TPM command code
  @param[in] AdditionalParameterSize Additional parameter size
  @param[in] AdditionalParameters    Pointer to the Additional paramaters

  @retval TPM_RESULT  TPM_SUCCESS, Success
    Other, Error
**/
TPM_RESULT
OT_Tpm12CommandNoReturnData (
  IN  EFI_TCG_PROTOCOL *TcgProtocol,
  IN  TPM_COMMAND_CODE Ordinal,
  IN  UINTN            AdditionalParameterSize,
  IN  VOID             *AdditionalParameters
  )
{
  EFI_STATUS          Status;
  TPM_RQU_COMMAND_HDR *TpmRqu;
  TPM_RSP_COMMAND_HDR TpmRsp;
  UINT32              Size, t;
  UINT16              w;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12CommandNoReturnData()  { -> Issue TPM command 0x%02x\n", Ordinal));

  t = TPM_SUCCESS; TpmRqu = NULL;
  if (TcgProtocol == NULL) {                                                  // Error
    DEBUG ((EFI_D_ERROR, "      Invalid ptr to EFI_TCG_PROTOCOL\n"));
    t = TPM_BAD_PARAMETER;
  } else {
    TpmRqu = (TPM_RQU_COMMAND_HDR*) AllocatePool (sizeof (*TpmRqu) + AdditionalParameterSize);
    if (TpmRqu == NULL) {                                                     // Error
      DEBUG ((EFI_D_ERROR, "      Memory not available\n"));
      t = TPM_FAIL;
    }
  }

  if (t == TPM_SUCCESS) {                                                     // If success so far
    TpmRqu->tag       = SwapBytes16 (TPM_TAG_RQU_COMMAND);
    Size              = (UINT32)(sizeof (*TpmRqu) + AdditionalParameterSize);
    TpmRqu->paramSize = SwapBytes32 (Size);
    TpmRqu->ordinal   = SwapBytes32 (Ordinal);
    CopyMem (TpmRqu + 1, AdditionalParameters, AdditionalParameterSize);

    Status = TcgProtocol->PassThroughToTpm (TcgProtocol,
                          Size, (UINT8*)TpmRqu,
                          (UINT32)sizeof (TpmRsp), (UINT8*)&TpmRsp);

    t = SwapBytes32 (TpmRsp.returnCode); w = SwapBytes16 (TpmRsp.tag);
    DEBUG ((EFI_D_ERROR, "      EFI Status 0x%08x, TPM Response Tag 0x%02x, TPM Return Code 0x%04x\n", Status, w, t));
    if ((Status != EFI_SUCCESS) || (w != TPM_TAG_RSP_COMMAND) || (t != TPM_SUCCESS)) {
      if (t == TPM_SUCCESS) { t = TPM_FAIL; }
    }
  }

  if (TpmRqu != NULL) { FreePool (TpmRqu); }                                 // Dellocate memory (if allocated)

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12CommandNoReturnData()  } -> TPM Result 0x%04x -> ", t));
  if (t == TPM_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return t;
}

//-----------------------------------------------------------------------------------------------------
/**
  This function checks if TPM1.2 NV Index already exists.

  @param  Opd            INPUT: OT_PLATFORM_DATA*, ptr to OT_PLATFORM_DATA
  @param  NvIndex        INPUT: UINT32, NV index
  @param  NvAttribute    INPUT/OUTPUT: UINT32*, ptr to area to return NV Attribute
  @param  NvWriteProtect INPUT/OUTPUT: BOOLEAN*, ptr to area to return NV Write-Protect info
  @param  NvDataSize     INPUT/OUTPUT: UINT32*, ptr to area to return NV Data Size in bytes

  @retval Result         BOOLEAN: TRUE/FALSE, NV Index yes/not exist
**/
BOOLEAN
OT_NvIndexExistTpm12 (
  IN     OT_PLATFORM_DATA *Opd,
  IN     UINT32           NvIndex,
  IN OUT UINT32           *NvAttribute,
  IN OUT BOOLEAN          *NvWriteProtect,
  IN OUT UINT32           *NvDataSize
  )
{
  // Local Data
  EFI_STATUS                   s;
  TPM_CMD_READ_NV_DATA_PUBLIC  Send;
  TPM_RESPONSE                 Receive;
  TPM_RSP_READ_NV_DATA_PUBLIC  *trrndp;
  TPM_NV_ATTRIBUTES            *tna;
  UINTN                        t;
  UINT32                       rc, SendSize, ReceiveSize;
  BOOLEAN                      b;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_NvIndexExistTpm12()  { -> Check if NV Index 0x%08x exist\n", NvIndex));

  // Intialize command buffer for Get Capability
  SendSize = sizeof (Send);                               // Size of command buffer
  ZeroMem ((UINT8*)(UINTN) &Send, SendSize);              // Clear send buffer
  Send.Hdr.tag = SwapBytes16 (TPM_TAG_RQU_COMMAND);       // Tag
  Send.Hdr.paramSize = SwapBytes32 (SendSize);            // Parameter size
  Send.Hdr.ordinal = SwapBytes32 (TPM_ORD_GetCapability); // Command ordinal
  Send.capArea = SwapBytes32 (TPM_CAP_NV_INDEX);          // Capability Area
  Send.subCapSize = SwapBytes32 (sizeof (TPM_NV_INDEX));  // Size of Sub Cap
  Send.nvIndex = SwapBytes32 (NvIndex);                   // NV Index

  DEBUG ((EFI_D_ERROR, "    Command Buffer: Read NV Data Public\n"));
  DisplayBinaryData ((UINT8*)(UINTN) &Send, SendSize);

  ReceiveSize = sizeof (Receive);                           // Size of receive buffer
  ZeroMem ((UINT8*)(UINTN) &Receive, sizeof (Receive));     // Clear receive buffer
  s = Opd->TcgProtocol->PassThroughToTpm (Opd->TcgProtocol, // Send command to TPM
    SendSize, (UINT8*)(UINTN) &Send, ReceiveSize, (UINT8*)(UINTN) &Receive);
  if (s == EFI_SUCCESS) {                                   // If success
    ReceiveSize = SwapBytes32 (Receive.Hdr.paramSize);      //   TPM response size
  } else {            // If not success
    ReceiveSize = sizeof (Receive.Hdr);                     //   TPM response size
  }

  DEBUG ((EFI_D_ERROR, "    TPM Response Buffer\n"));
  DisplayBinaryData ((UINT8*)(UINTN) &Receive, ReceiveSize);

  b = FALSE;                                                  // Assume NV Index not exist
  *NvAttribute = 0; *NvWriteProtect = FALSE; *NvDataSize = 0; // Init return parameters
  rc = SwapBytes32 (Receive.Hdr.returnCode);                  // Response Code from TPM
  if ((s == EFI_SUCCESS) && (rc == TPM_SUCCESS)) {  // If success
    // Calculate ptr to TPM_NV_ATTRIBUTES in TPM_NV_DATA_PUBLIC
    //   (note that size of TPM_NV_DATA_PUBLIC is variable and depends of #of PCRs supported)
    // Ptr to TPM_NV_ATTRIBUTES = Ptr to Response buffer + total response size - sizeof (TPM_NV_DATA_PUBLIC.dataSize) - sizeof (TPM_NV_DATA_PUBLIC.bWriteDefine) - sizeof (TPM_NV_DATA_PUBLIC.bWriteSTClear) - sizeof (TPM_NV_DATA_PUBLIC.bReadSTClear) - sizeof (TPM_NV_DATA_PUBLIC.permission)
    trrndp = (TPM_RSP_READ_NV_DATA_PUBLIC*)(UINTN) &Receive; // Ptr to TPM_RSP_READ_NV_DATA_PUBLIC
    t = (UINTN) trrndp;                                      // Address of TPM_RSP_READ_NV_DATA_PUBLIC
    t = (UINTN) ((t + SwapBytes32 (trrndp->Hdr.paramSize)) - // Address of TPM_NV_ATTRIBUTES
                 sizeof (trrndp->nvDataPublic.dataSize) -
                 sizeof (trrndp->nvDataPublic.bWriteDefine) -
                 sizeof (trrndp->nvDataPublic.bWriteSTClear) -
                 sizeof (trrndp->nvDataPublic.bReadSTClear) -
                 sizeof (trrndp->nvDataPublic.permission));
    tna = (TPM_NV_ATTRIBUTES*)(UINTN) t;                       // Ptr to TPM_NV_ATTRIBUTES
    if ((SwapBytes16 (tna->tag)) == TPM_TAG_NV_ATTRIBUTES) {   //   If NV Attribute TAG valid
      b = TRUE;                                                //     NV Index Exist
      *NvAttribute = SwapBytes32 (tna->attributes);            //     Return NV Attribute
      t = (UINTN) tna;                                         //     Address of TPM_NV_ATTRIBUTES
      t += (UINTN) (sizeof (trrndp->nvDataPublic.permission) + //     Address of bWriteDefine
                  sizeof (trrndp->nvDataPublic.bReadSTClear) +
                  sizeof (trrndp->nvDataPublic.bWriteSTClear));
      *NvWriteProtect = *((BOOLEAN*)(UINTN) t);                //     Return bWriteDefine
      t += sizeof (trrndp->nvDataPublic.bWriteDefine);
      *NvDataSize = SwapBytes32 (*((UINT32*)(UINTN) t));       //     Return NV Data size in bytes
    } else {                                                   //   If NV Attribute TAG not valid
      s = EFI_DEVICE_ERROR;                                    //     Error
      DEBUG ((EFI_D_ERROR, "    Invalid NV Attribute TAG -> Error\n"));
    }
  }

  ZeroMem ((UINT8*)(UINTN) &Send, sizeof (Send));            // Clear send buffer
  ZeroMem ((UINT8*)(UINTN) &Receive, sizeof (Receive));      // Clear receive buffer

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_NvIndexExistTpm12()  } -> NV Index Exist? "));
  if (b) { DEBUG ((EFI_D_ERROR, "YES, Attribute 0x%08x, Write-Protect 0x%02x, Data Size 0x%08x\n", *NvAttribute, *NvWriteProtect, *NvDataSize)); }
  else { DEBUG ((EFI_D_ERROR, "NO, EFI Status 0x%08x, TPM Response Code 0x%08x\n", s, rc)); }

  return b;                                                  // Result
}

//--------------------------------------------------------------------------------------------------------------
// NV Index for TPM1.2 that are protected (i.e. NOT allowed) by TPM NV Index Handler
//
UINT32 ProtectedNvIndexTableTpm12[] = { \
     TPM12_NV_INDEX_AUX,   // Aux Index
     TPM12_NV_INDEX_PS,    // PS Index
     TPM12_NV_INDEX_PO,    // PO Index
     TPM12_NV_INDEX_LOCK,  // Reserved by TPM Spec
     TPM12_NV_INDEX0,      // Reserved by TPM Spec
     TPM12_NV_INDEX_DIR,   // Reserved by TPM Spec
     };
//
//--------------------------------------------------------------------------------------------------------------
/**
  This function checks if given NV Index is allowed for TPM1.2 NV Index Handler.

  @param NvIndex  INPUT:  UINT32, Given NV Index

  @retval Result  OUTPUT: BOOLEAN, TRUE/FALSE, NV Index Yes/Not allowed
**/
BOOLEAN
OT_NvIndexAllowedTpm12 (
  IN  UINT32 NvIndex
  )
{
  // Local Data
  UINT32 i, n;

  // Check NV Index is protected (i.e. not allowed)
  n = (sizeof (ProtectedNvIndexTableTpm12))/ (sizeof (UINT32));     // #of entry in table
  for (i = 0; i < n; i++) {                                         // Search all entry
    if (ProtectedNvIndexTableTpm12[i] == NvIndex) { return FALSE; } //   If match, Return NV Index not allowed
  }

  // Check if NV Index is proper
  if ((NvIndex & (TPM12_NV_INDEX_T_BIT | TPM12_NV_INDEX_D_BIT)) != 0) { // If either T or D set in NV Index
    return FALSE;                                                       //   Return NV Index not allowed
  }

  return TRUE;                                                          // Return NV Index allowed
}

//--------------------------------------------------------------------------------------------------------------
/**
  This function validates the NV Index for TPM1.2.

  @param[in]  NcIndex  INPUT: UINT32, NV Index

  @retval     Result   OUTPUT: BOOLEAN, TRUE/FALSE, NV Index yes/not valid
**/
BOOLEAN
OT_NvIndexValidTpm12 (
  IN  UINT32 NvIndex
  )
{
  // Local Data
  BOOLEAN  b;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_NvIndexValidTpm12()  { -> Validate NV Index = 0x%08x\n", NvIndex));

  b = OT_NvIndexAllowedTpm12 (NvIndex);      // TRUE/FALSE: NV Index Yes/Not allowed

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_NvIndexValidTpm12()  } -> NV Index "));
  if (b) { DEBUG ((EFI_D_ERROR, "Valid\n")); }
  else { DEBUG ((EFI_D_ERROR, "Invalid\n")); }

  return b;                                  // Valid/Invalid
}

//----------------------------------------------------------------------------------------------
//--------------------- PUBLIC Functions -------------------------------------------------------
//----------------------------------------------------------------------------------------------
/**
  Get TPM1.2 Permanent Flags.

  @param[in] Opd         Ptr to OT_PLATFORM_DATA

  @retval    TPM_RESULT  TPM_SUCCESS, Success
                         Other, Error

**/
TPM_RESULT
OT_GetTpm12PermanentFlags (
  IN  OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  EFI_STATUS          Status;
  TPM_RSP_COMMAND_HDR *Rsp;
  UINT32              SendSize, t;
  UINT16              w;
  UINT8               CmdBuf[64], *ppf;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_GetTpm12PermanentFlags()  { -> Get TPM1.2 Permanent Flags\n"));

  // Validate input parameters
  t = TPM_SUCCESS;                                                   // Assume success
  if ((Opd == NULL) || (Opd->TcgProtocol == NULL)) {                 // Error
    DEBUG ((EFI_D_ERROR, "      Invalid ptr to OT_PLATFORM_DATA or EFI_TCG_PROTOCOL\n"));
    t = TPM_BAD_PARAMETER;
  } else {
    // Get TPM Permanent flags
    SendSize              = sizeof (TPM_RQU_COMMAND_HDR) + sizeof (UINT32) * 3;
    *(UINT16*)&CmdBuf[0]  = SwapBytes16 (TPM_TAG_RQU_COMMAND);
    *(UINT32*)&CmdBuf[2]  = SwapBytes32 (SendSize);
    *(UINT32*)&CmdBuf[6]  = SwapBytes32 (TPM_ORD_GetCapability);

    *(UINT32*)&CmdBuf[10] = SwapBytes32 (TPM_CAP_FLAG);
    *(UINT32*)&CmdBuf[14] = SwapBytes32 (sizeof (TPM_CAP_FLAG_PERMANENT));
    *(UINT32*)&CmdBuf[18] = SwapBytes32 (TPM_CAP_FLAG_PERMANENT);

    Status = Opd->TcgProtocol->PassThroughToTpm (Opd->TcgProtocol,   // Send command to TPM
       SendSize, CmdBuf, sizeof (CmdBuf), CmdBuf);

    Rsp = (TPM_RSP_COMMAND_HDR*) &CmdBuf[0];                         // Ptr to TPM response buffer
    t = SwapBytes32 (Rsp->returnCode); w = SwapBytes16 (Rsp->tag);
    DEBUG ((EFI_D_ERROR, "    EFI Status 0x%08x, TPM Response Tag 0x%02x, TPM Return Code 0x%04x -> ", Status, w, t));
    if ((Status != EFI_SUCCESS) || (w != TPM_TAG_RSP_COMMAND) || (t != TPM_SUCCESS)) { // Error
      t = TPM_FAIL;
      DEBUG ((EFI_D_ERROR, "Error\n"));
    } else {
      ppf = (UINT8*) &CmdBuf[sizeof (TPM_RSP_COMMAND_HDR) + sizeof (UINT32)];  // Ptr to TPM_PERMANENT_FLAGS
      CopyMem ((UINT8*) &Opd->Tpm12PermanentFlags, ppf, sizeof (TPM_PERMANENT_FLAGS)); // Save in OT_PLATFORM_DATA
      DEBUG ((EFI_D_ERROR, "Success\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_GetTpm12PermanentFlags()  } -> TPM Result: 0x%04x ", t));
  if (t == TPM_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return t;
}

//-----------------------------------------------------------------------------------------------------
/**
  Issue TSC_PhysicalPresence command to TPM.

  @param[in] TcgProtocol       EFI TCG Protocol instance.
  @param[in] PhysicalPresence  The state to set the TPM's Physical Presence flags.

  @retval    TPM_RESULT        TPM_SUCCESS, Success
                               Other, Error
**/
TPM_RESULT
OT_Tpm12PhysicalPresence (
  IN  EFI_TCG_PROTOCOL       *TcgProtocol,
  IN  TPM_PHYSICAL_PRESENCE  PhysicalPresence
  )
{
  EFI_STATUS Status;
  TPM_RQU_COMMAND_HDR    *TpmRqu;
  TPM_PHYSICAL_PRESENCE  *TpmPp;
  TPM_RSP_COMMAND_HDR    TpmRsp;
  UINT8                  Buffer[sizeof (*TpmRqu) + sizeof (*TpmPp)];
  UINT16                 w;
  UINT32                 t;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12PhysicalPresence()  { -> PhysicalPresence 0x%02x\n", (UINT16)(UINTN) PhysicalPresence));

  t = TPM_SUCCESS;                                                   // Assume success
  if (TcgProtocol == NULL) {                                         // Error
    DEBUG ((EFI_D_ERROR, "      Invalid ptr to EFI_TCG_PROTOCOL\n"));
    t = TPM_BAD_PARAMETER;
  } else {
    TpmRqu = (TPM_RQU_COMMAND_HDR*)Buffer;
    TpmPp = (TPM_PHYSICAL_PRESENCE*)(TpmRqu + 1);

    TpmRqu->tag       = SwapBytes16 (TPM_TAG_RQU_COMMAND);
    TpmRqu->paramSize = SwapBytes32 (sizeof (Buffer));
    TpmRqu->ordinal   = SwapBytes32 (TSC_ORD_PhysicalPresence);
    WriteUnaligned16 (TpmPp, (TPM_PHYSICAL_PRESENCE) SwapBytes16 (PhysicalPresence));

    Status = TcgProtocol->PassThroughToTpm (TcgProtocol,
                          sizeof (Buffer), (UINT8*)TpmRqu,
                          sizeof (TpmRsp), (UINT8*)&TpmRsp);

    t = SwapBytes32 (TpmRsp.returnCode); w = SwapBytes16 (TpmRsp.tag);
    DEBUG ((EFI_D_ERROR, "      EFI Status 0x%08x, TPM Response Tag 0x%02x, TPM Return Code 0x%04x\n", Status, w, t));
   if ((Status != EFI_SUCCESS) || (w != TPM_TAG_RSP_COMMAND) || (t != TPM_SUCCESS)) {
     if (t == TPM_SUCCESS) { t = TPM_FAIL; }
   }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12PhysicalPresence()  } -> TPM Result 0x%04x -> ", t));
  if (t == TPM_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return t;
}

//-----------------------------------------------------------------------------------------------------
/**
  OT_Tpm12DeactivateDisable(): Deactivate, Disable TPM.

  @param[in]  Opd     INPUT: Ptr to OT_PLATFORM_DATA
  @retval     Result  OUTPUT: UINT8, Status code

**/
UINT8
OT_Tpm12DeactivateDisable (
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  // Local Data
  UINT8   e;
  BOOLEAN b;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12DeactivateDisable()  { -> TPM1.2: Deactivate, Disable\n"));

  // Deactivate TPM1.2, if necessary
  e = OXP_SUCCESS;                                                         // Assume success
  if (!(Opd->Tpm12PermanentFlags.disable)) {                               // If currently enabled
    DEBUG ((EFI_D_ERROR, "      Deactivate TPM1.2:\n"));
    if (!(Opd->Tpm12PermanentFlags.deactivated)) {                         //   If currently activated
      b = TRUE;
      if ((OT_Tpm12CommandNoReturnData (Opd->TcgProtocol,                  //     Deactivate TPM
        TPM_ORD_PhysicalSetDeactivated, sizeof (b), &b)) == TPM_SUCCESS) {
        Opd->Tpm12PermanentFlags.deactivated = TRUE;                       //       Indicate Deactivated
        Opd->Tpm12ActivateStateChange = TRUE;                              //       Indicate TPM ctivate State Changed
        DEBUG ((EFI_D_ERROR, "          Success -> Activate State Changed, Reboot Required\n"));
      } else {                                                             //       If not success
        e = OXP_DTPM_DEACTIVATE_ERROR;                                     //         Error
      }
    } else {
      DEBUG ((EFI_D_ERROR, "          Already Deactivated\n"));
    }
  }

  // Disable TPM1.2, if necessary
  if (e == OXP_SUCCESS) {                                // If success so far
    DEBUG ((EFI_D_ERROR, "      Disable TPM1.2:\n"));
    if (!(Opd->Tpm12PermanentFlags.disable)) {           //   If currently enabled
      if ((OT_Tpm12CommandNoReturnData (Opd->TcgProtocol,//     Disable TPM
        TPM_ORD_PhysicalDisable, 0, NULL)) == TPM_SUCCESS) {
        Opd->Tpm12PermanentFlags.disable = TRUE;         //     Indicate Disabled
      } else {                                           //     If not success
        e = OXP_DTPM_DISABLE_ERROR;                      //       Error
      }
    } else {                                             //     if currently disabled
      DEBUG ((EFI_D_ERROR, "          Already Disabled\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12DeactivateDisable()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                              // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  OT_Tpm12EnableActivate(): Enable, Activate TPM.

  @param[in]  Opd     INPUT: Ptr to OT_PLATFORM_DATA
  @param[in]  Always  INPUT: BOOLEAN, TRUE  --> Issue Enable and Activate always
                                      FALSE --> Issue Enable and Activate if necessary

  @retval     Result OUTPUT: UINT8, Status code

**/
UINT8
OT_Tpm12EnableActivate (
  IN  OT_PLATFORM_DATA *Opd,
  IN  BOOLEAN          Always
  )
{
  // Local Data
  UINT8   e;
  BOOLEAN b;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12EnableActivate()  { -> TPM1.2: Enable, Activate\n"));

  // Enable TPM1.2, if necessary
  e = OXP_SUCCESS;                                       // Assume Success
  DEBUG ((EFI_D_ERROR, "      Enable TPM1.2:\n"));
  if ((Always) || (Opd->Tpm12PermanentFlags.disable)) {  // If always enable OR currently disabled
    if ((OT_Tpm12CommandNoReturnData (Opd->TcgProtocol,  //   Enable TPM
      TPM_ORD_PhysicalEnable, 0, NULL)) == TPM_SUCCESS) {//   If success
      Opd->Tpm12PermanentFlags.disable = FALSE;          //     Indicate Enabled
    } else {                                             //   If not success
      e = OXP_DTPM_ENABLE_ERROR;                         //   Error
    }
  } else {
    DEBUG ((EFI_D_ERROR, "          Already Enabled\n"));
  }


  // Activate TPM1.2, if necessary
  if (e == OXP_SUCCESS) {                                     // If success so far
    DEBUG ((EFI_D_ERROR, "      Activate TPM1.2:\n"));
    if ((Always) || (Opd->Tpm12PermanentFlags.deactivated)) { //   If always activate OR currently deactivated
      b = FALSE;
      if ((OT_Tpm12CommandNoReturnData (Opd->TcgProtocol,     //     Activate TPM
        TPM_ORD_PhysicalSetDeactivated, sizeof (b), &b)) == TPM_SUCCESS) {
        Opd->Tpm12PermanentFlags.deactivated = FALSE;         //     Indicate Activated
        Opd->Tpm12ActivateStateChange = TRUE;                 //     Indicate TPM Activate State Changed --> Reboot Required
        DEBUG ((EFI_D_ERROR, "          Success -> Activate State Changed, Reboot Required\n"));
      } else {                                                //     If not success
        e = OXP_DTPM_DEACTIVATE_ERROR;                        //       Error
      }
    } else {
      DEBUG ((EFI_D_ERROR, "          Already Activated\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12EnableActivate()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                   // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  OT_Tpm12ClearEnableActivate(): Clear, Enable, Activate TPM.

  @param[in]  Opd INPUT: Ptr to OT_PLATFORM_DATA

  @retval     Result OUTPUT: UINT8, Status code

**/
UINT8
OT_Tpm12ClearEnableActivate (
  IN  OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12ClearEnableActivate()  { -> TPM1.2: Clear, Enable, Activate\n"));

  e = OXP_SUCCESS;                                          // Assume auccess
  if (Opd->Tpm12PermanentFlags.disable) {                   // If disabled
    DEBUG ((EFI_D_ERROR, "      TPM not enabled\n"));
    e = OXP_DTPM_NOT_ENABLED;                               //   Error
  } else {
    if (Opd->Tpm12PermanentFlags.deactivated) {             //   If deactivated
      DEBUG ((EFI_D_ERROR, "      TPM not activated\n"));
      e = OXP_DTPM_NOT_ACTIVATED;                           //     Error
    }
  }

  if (e == OXP_SUCCESS) {                                   // If success so far
    DEBUG ((EFI_D_ERROR, "      Clear TPM1.2:\n"));
    if ((OT_Tpm12CommandNoReturnData (Opd->TcgProtocol,     //   Force Clear TPM
      TPM_ORD_ForceClear, 0, NULL)) != TPM_SUCCESS) {
      e = OXP_DTPM_CLEAR_ERROR;                             //     Error
    } else {                                                //   If Clear success
      e = OT_Tpm12EnableActivate (Opd, TRUE);               //     Enable, Activate (always)
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_Tpm12ClearEnableActivate()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                 // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  This function returns the information of different TPM1.2 NV Index.

  @param    Opd  INPUT: OT_PLATFORM_DATA*, ptr to OT_PLATFORM_DATA

  @retval   Info OUTPUT: UINT16, Bit-mapped information of different TPM NV Index
                         (as defined in OXP_HEADER.FeatureEnabled field)
**/
UINT16
OT_GetNvIndexInfoTpm12 (
  IN  OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  UINT32  auxa, poa, psa, psd, auxd, pod;
  UINT16  i;
  BOOLEAN aux, auxwp, po, powp, ps, pswp;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_GetNvIndexInfoTpm12()  { -> Get information of different TPM1.2 NV Index\n"));

  i = 0;                                                                      // Init information of different NV Index

  ps = OT_NvIndexExistTpm12 (Opd, TPM12_NV_INDEX_PS, &psa, &pswp, &psd);      // PS NV Index info
  aux = OT_NvIndexExistTpm12 (Opd, TPM12_NV_INDEX_AUX, &auxa, &auxwp, &auxd); // AUX NV Index info
  po = OT_NvIndexExistTpm12 (Opd, TPM12_NV_INDEX_PO, &poa, &powp, &pod);      // PO NV Index info

  // ps, psa, pswp = PS NV Index info; aux, auxa, auxwp = AUX NV Index info; po, poa, powp = PO NV INdex info
  DEBUG ((EFI_D_ERROR, "     TPM1.2 PS NV Index 0x%08x: ", TPM12_NV_INDEX_PS));
  if (ps) { DEBUG ((EFI_D_ERROR, "Defined, ")); }
  else { DEBUG ((EFI_D_ERROR, "Not Defined, ")); }
  if (pswp) {
    i |= OXP_TPM_NVINDEX_PS_WRITE_PROTECT_BIT;                                // PS NV INdex write-protected
    DEBUG ((EFI_D_ERROR, "Write-Protected\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "Not Write-Protected\n"));
  }

  DEBUG ((EFI_D_ERROR, "    TPM1.2 AUX NV Index 0x%08x: ", TPM12_NV_INDEX_AUX));
  if (aux) { DEBUG ((EFI_D_ERROR, "Defined, ")); }
  else { DEBUG ((EFI_D_ERROR, "Not Defined, "));  }
  if (auxwp) {
    i |= OXP_TPM_NVINDEX_AUX_WRITE_PROTECT_BIT;                              // AUX NV Index write-protected
    DEBUG ((EFI_D_ERROR, "Write-Protected\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "Not Write-Protected\n"));
  }

  DEBUG ((EFI_D_ERROR, "     TPM1.2 PO NV Index 0x%08x: ", TPM12_NV_INDEX_PO));
  if (po) { DEBUG ((EFI_D_ERROR, "Defined, ")); }
  else { DEBUG ((EFI_D_ERROR, "Not Defined, ")); }
  if (powp) {
    i |= OXP_TPM_NVINDEX_PO_WRITE_PROTECT_BIT;                               // PO NV INdex write-protected
    DEBUG ((EFI_D_ERROR, "Write-Protected\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "Not Write-Protected\n"));
  }


  DEBUG ((EFI_D_ERROR, "     TPM1.2 Provisioned? "));
  if ((ps) && (aux)) {                                                       // If both PS NV Index and AUX NV Index defined
    i |= OXP_TPM_PROVISION_BIT;                                              //   TPM Provisioned
    DEBUG ((EFI_D_ERROR, "Yes\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "No\n"));
  }

  DEBUG ((EFI_D_ERROR, "     TPM1.2 Ownership Claimed? "));
  if (po) {                                                                  // If PO NV Index defined
    i |= OXP_TPM_OWNERSHIP_BIT;                                              //   TPM Ownership claimed
    DEBUG ((EFI_D_ERROR, "Yes\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "No\n"));
  }

  DEBUG ((EFI_D_ERROR, "     TPM1.2 NvLocked? "));
  if (Opd->Tpm12PermanentFlags.nvLocked) {
    i |= OXP_TPM_LOCK_BIT;                                                   // TPM Lock info
    DEBUG ((EFI_D_ERROR, "Yes\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "No\n"));
  }

  DEBUG ((EFI_D_ERROR, "[OOB-TPM12] OT_GetNvIndexInfoTpm12()  } -> TPM1.2 NV Index Information = 0x%04x\n", i));

  return i;
}

//-----------------------------------------------------------------------------------------------------
