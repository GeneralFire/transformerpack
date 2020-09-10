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
#include "DxeOneTouchTpm20.h"

//-----------------------------------------------------------------------------------------------------
/**
  This function checks if TPM2.0 NV Index already exists.

  It performs this operation by sending the read public command with the index value.

  @param    Opd         INPUT: OT_PLATFORM_DATA*, ptr to OT_PLATFORM_DATA
  @param    NvIndex     INPUT: UINT32, NV index
  @param    NvAttribute INPUT/OUTPUT: UINT32*, Ptr to area to return NV attribute information
  @param    NvDataSize  INPUT/OUTPUT: UINT32*, ptr to area to return NV Data Size in bytes

  @retval TRUE      Index exists.
  @retval FALSE     Index does not exist.
**/
BOOLEAN
OT_NvIndexExistTpm20 (
  IN     OT_PLATFORM_DATA     *Opd,
  IN     UINT32               NvIndex,
  IN OUT UINT32               *NvAttribute,
  IN OUT UINT32               *NvDataSize
  )
{
  // Local data
  TPM2_NV_READPUBLIC_COMMAND  Send;
  TPM2_NV_READPUBLIC_RESPONSE Receive;
  EFI_STATUS                  s;
  UINTN                       n;
  UINT32                      *p, rc, SendSize, ReceiveSize;
  BOOLEAN                     b;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM20] OT_NvIndexExistTpm20()  { -> Check if NV Index 0x%08x exist\n", NvIndex));

  // Initialize command buffer
  SendSize = (UINT32) sizeof (Send);                             // Send buffer size
  Send.Header.tag = SwapBytes16 (TPM_ST_NO_SESSIONS);            // TAG
  Send.Header.paramSize = SwapBytes32 (SendSize);                // Command parameters size
  Send.Header.commandCode = SwapBytes32 (TPM_CC_NV_ReadPublic);  // Command
  Opd->TpmCommand = TPM_CC_NV_ReadPublic;                        // Last TPM Command issued
  Send.NvIndex = SwapBytes32 (NvIndex);                          // NV Index

  DEBUG ((EFI_D_ERROR, "    Command Buffer: Read NV Data Public\n"));
  DisplayBinaryData ((UINT8*)(UINTN) &Send, SendSize);

  ReceiveSize = sizeof (Receive);                                // Receive buffer size
  ZeroMem ((UINT8*)(UINTN) &Receive, ReceiveSize);               // Clear receive buffer
  s = Opd->Tcg2Protocol->SubmitCommand (Opd->Tcg2Protocol,       // Set command to TPM
               SendSize, (UINT8*)(UINTN) &Send, ReceiveSize, (UINT8*)(UINTN) &Receive);
  rc = SwapBytes32 (Receive.Header.responseCode);                // Response Code from TPM
  Opd->TpmCommand = SwapBytes32 (Send.Header.commandCode);       // Last TPM Command issued
  Opd->TpmRc = rc;                                               // TPM Response code of Last TPM Command issued
  if (s == EFI_SUCCESS) {                                        // If success
    ReceiveSize = SwapBytes32 (Receive.Header.paramSize);        //   TPM response size
  } else {                                                       // If not success
    ReceiveSize = sizeof (Receive.Header);                       //   TPM response size
  }

  DEBUG ((EFI_D_ERROR, "    TPM Response Buffer\n"));
  DisplayBinaryData ((UINT8*)(UINTN) &Receive, ReceiveSize);

  b = FALSE; *NvAttribute = 0; *NvDataSize = 0;                         // Assume not exist, Init return parameters
  if ((s == EFI_SUCCESS) && (rc == TPM_SUCCESS)) {                      // If success
    b = TRUE;                                                           //   NV Index Exist
    p = (UINT32*)(UINTN) &Receive.NvPublic.nvPublic.attributes;         //   Ptr NV Attribute
    *NvAttribute = SwapBytes32 ((UINT32) *p);                           //   Return NV Attribute
    n = (((UINTN) &Receive.NvPublic +                                   //   Address of dataSize field in TPMS_NV_PUBLIC
        (UINTN) (SwapBytes16 (Receive.NvPublic.size)) +
        (UINTN) sizeof (Receive.NvPublic.size)) -
        (UINTN) sizeof (Receive.NvPublic.nvPublic.dataSize));
    *NvDataSize = (UINT32)(UINTN) (SwapBytes16 (*((UINT16*)(UINTN) n)));//   Return NV Data size
  }

  ZeroMem ((UINT8*)(UINTN) &Send, SendSize);                            // Clear send buffer
  ZeroMem ((UINT8*)(UINTN) &Receive, ReceiveSize);                      // Clear receive buffer

  DEBUG ((EFI_D_ERROR, "[OOB-TPM20] OT_NvIndexExistTpm20()  } -> NV Index Exist? "));
  if (b) { DEBUG ((EFI_D_ERROR, "YES, Attribute 0x%08x, Data size 0x%08x bytes\n", *NvAttribute, *NvDataSize)); }
  else { DEBUG ((EFI_D_ERROR, "NO, EFI Status 0x%08x, TPM Response Code 0x%08x\n", s, rc)); }

  return b;                                                             // Result
}

//--------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//---------------------	PUBLIC Functions -------------------------------------------------------
//----------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
/**
  OT_SetPcrBanksTpm20(): Set PCR Banks for TPM2.0.

  @param  Auth        INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd         INPUT:  Ptr to OT_PLATFORM_DATA
  @param  PcrBanks    INPUT:  UIN32, Bit-mapped information of PCR Banks to be set
  @param  Ci          INPUT:  BOOLEAN*, Ptr to Boolean Variable
                            OUTPUT: TRUE  -> Set PCR Banks command issued
                            OUTPUT: FALSE -> Set PCR Banks command NOT issued
  @param  EFI_STATUS  EFI_SUCCESS  Success
                      Other        Error
**/
EFI_STATUS
OT_SetPcrBanksTpm20 (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd,
  IN     UINT32           PcrBanks,
  IN OUT BOOLEAN          *Ci
  )
{
  // Local data
  EFI_TCG2_BOOT_SERVICE_CAPABILITY    pc;
  EFI_STATUS                          s;
  UINT32                              cp, sp;
  BOOLEAN                             c;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM20] OT_SetPcrBanksTpm20(): Detected TPM Device 0x%02x, Requested PCR Banks 0x%08x\n", Opd->T2dd.TpmDeviceDetected, PcrBanks));

  s = EFI_DEVICE_ERROR; c = FALSE;                    // Assume: Error,  Set PCR Banks Command not issued
  if (Opd->Tcg2Protocol != NULL) {                    // If TCG2 protocol found
    DEBUG ((EFI_D_ERROR, "    Issue Get Capability\n"));
    pc.Size = sizeof (pc);
    s = Opd->Tcg2Protocol->GetCapability (Opd->Tcg2Protocol, &pc);//   Get capability
    if (s == EFI_SUCCESS) {                                       //   If success
      sp = pc.HashAlgorithmBitmap;                                //     Bitmap of Supported PCR Banks
      cp = pc.ActivePcrBanks;                                     //     Bitmap of Current Active PCR Banks
      DEBUG ((EFI_D_ERROR, "    PCR Banks: Supported = 0x%08x, Currently Active = 0x%08x\n", sp, cp));

// For customers who want to activate PCR Banks according to their choice, activate the following lines of code
/*
      if ((cp & PcrBanks) != PcrBanks) {                          //     If requested PCR Banks not active
        DEBUG ((EFI_D_ERROR, "    Requested PCR Banks not currently active\n"));
        if ((sp & PcrBanks) == PcrBanks) {                        //       If requested PCR Banks supported
          s = Tpm2CommandAllocPcr (Auth, sp, PcrBanks);           //         Set PCR Banks
          c = TRUE;                                               //         Indicate command issued
        } else {                                                  //       If requested PCR Banks not supported
          DEBUG ((EFI_D_ERROR, "    Requested of PCR Banks Not supported\n"));
          s = EFI_DEVICE_ERROR;                                   //   Error
        }
      } else {                                                    //     If requested PCR Banks already active
        DEBUG ((EFI_D_ERROR, "    Requested PCR Banks already active\n"));
      }
*/


    }
  }

  *Ci = c;                                                        // Return whether command issued or not

  DEBUG ((EFI_D_ERROR, "[OOB-TPM20] OT_SetPcrBanksTpm20()  } -> Command Issued: "));
  if (c) { DEBUG ((EFI_D_ERROR, "Yes")); }
  else { DEBUG ((EFI_D_ERROR, "No")); }
  DEBUG ((EFI_D_ERROR, ", EFI Status: 0x%08x -> ", s));
  if (s == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return s;
}

//-----------------------------------------------------------------------------------------------------
/**
  This function returns the information of different TPM2.0 NV Index.

  @param    Opd    INPUT: OT_PLATFORM_DATA*, ptr to OT_PLATFORM_DATA

  @retval   Info   OUTPUT: UINT16, Bit-mapped information of different TPM NV Index
                           (as defined in OXP_HEADER.FeatureEnabled field)
**/
UINT16
OT_GetNvIndexInfoTpm20 (
  IN  OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  TPMA_NV  auxa, poa, psa;
  UINT32   auxd, pod, psd;
  UINT16   i;
  BOOLEAN  aux, po, ps;

  DEBUG ((EFI_D_ERROR, "[OOB-TPM20] OT_GetNvIndexInfoTpm20()  { -> Get information of different TPM2.0 NV Index\n"));

  i = 0;                                                                        // Init information of different NV Index

  ps = OT_NvIndexExistTpm20 (Opd, TPM20_NV_INDEX_PS, (UINT32*) &psa, &psd);     // PS NV Index info
  aux = OT_NvIndexExistTpm20 (Opd, TPM20_NV_INDEX_AUX, (UINT32*) &auxa, &auxd); // AUX NV Index info
  po = OT_NvIndexExistTpm20 (Opd, TPM20_NV_INDEX_PO, (UINT32*) &poa, &pod);     // PO NV Index info

  // ps, psa = PS NV Index info; aux, auxa = AUX NV Index info; po, poa = PO NV INdex info
  DEBUG ((EFI_D_ERROR, "     TPM2.0 PS NV Index 0x%08x: ", TPM20_NV_INDEX_PS));
  if (ps) { DEBUG ((EFI_D_ERROR, "Defined")); }
  else { DEBUG ((EFI_D_ERROR, "Not Defined")); }
  if ((psa.TPMA_NV_WRITTEN) != 0)  { DEBUG ((EFI_D_ERROR, ", Written")); }
  else { DEBUG ((EFI_D_ERROR, ", Not Written")); }
  if ((psa.TPMA_NV_WRITELOCKED) != 0) {
    i |= OXP_TPM_NVINDEX_PS_WRITE_PROTECT_BIT;                                 // PS NV INdex write-protected
    DEBUG ((EFI_D_ERROR, ", Write-Protected\n"));
  } else {
    DEBUG ((EFI_D_ERROR, ", Not Write-Protected\n"));
  }

  DEBUG ((EFI_D_ERROR, "    TPM2.0 AUX NV Index 0x%08x: ", TPM20_NV_INDEX_AUX));
  if (aux) { DEBUG ((EFI_D_ERROR, "Defined")); }
  else { DEBUG ((EFI_D_ERROR, "Not Defined")); }
  if ((auxa.TPMA_NV_WRITTEN) != 0)  { DEBUG ((EFI_D_ERROR, ", Written")); }
  else { DEBUG ((EFI_D_ERROR, ", Not Written")); }
  if ((auxa.TPMA_NV_WRITELOCKED) != 0) {
    i |= OXP_TPM_NVINDEX_AUX_WRITE_PROTECT_BIT;                               // AUX NV Index write-protected
    DEBUG ((EFI_D_ERROR, ", Write-Protected\n"));
  } else {
    DEBUG ((EFI_D_ERROR, ", Not Write-Protected\n"));
  }

  DEBUG ((EFI_D_ERROR, "     TPM2.0 PO NV Index 0x%08x: ", TPM20_NV_INDEX_PO));
  if (po) { DEBUG ((EFI_D_ERROR, "Defined")); }
  else { DEBUG ((EFI_D_ERROR, "Not Defined")); }
  if ((poa.TPMA_NV_WRITTEN) != 0)  { DEBUG ((EFI_D_ERROR, ", Written")); }
  else { DEBUG ((EFI_D_ERROR, ", Not Written")); }
  if ((poa.TPMA_NV_WRITELOCKED) != 0) {
    i |= OXP_TPM_NVINDEX_PO_WRITE_PROTECT_BIT;                               // PO NV INdex write-protected
    DEBUG ((EFI_D_ERROR, ", Write-Protected\n"));
  } else {
    DEBUG ((EFI_D_ERROR, ", Not Write-Protected\n"));
  }

  // Determine whether TPM is provisioned: Use the proper condition as per customer's requirement
  //    Condition-1: Check if both PS NV Index and AUX NV Index are written
  //    Condition-2: Check if both PS NV Index and AUX NV Index are defined -> Default Implementation
  DEBUG ((EFI_D_ERROR, "     TPM2.0 Provisioned? "));
//  if (((psa.TPMA_NV_WRITTEN) != 0) && ((auxa.TPMA_NV_WRITTEN) != 0)) {     // If both PS NV Index and AUX NV Index written
  if ((ps) && (aux)) {                                                       // If both PS NV Index and AUX NV Index defined
    i |= OXP_TPM_PROVISION_BIT;                                              //   TPM Provisioned
    DEBUG ((EFI_D_ERROR, "Yes\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "No\n"));
  }

  // Determine whether TPM Ownership is claimed: Use the proper condition as per customer's requirement
  //    Condition-1: Check if PO NV Index is written
  //    Condition-2: Check if PO NV Index is defined -> Default Implementation
  DEBUG ((EFI_D_ERROR, "     TPM2.0 Ownership Claimed? "));
//  if ((poa.TPMA_NV_WRITTEN) != 0) {                                        // If PO NV Index written
  if (po) {                                                                  // If PO NV Index defined
    i |= OXP_TPM_OWNERSHIP_BIT;                                              //   TPM Ownership claimed
    DEBUG ((EFI_D_ERROR, "Yes\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "No\n"));
  }

  DEBUG ((EFI_D_ERROR, "[OOB-TPM20] OT_GetNvIndexInfoTpm20()  } -> TPM2.0 NV Index Information = 0x%04x\n", i));

  return i;
}

//-----------------------------------------------------------------------------------------------------


