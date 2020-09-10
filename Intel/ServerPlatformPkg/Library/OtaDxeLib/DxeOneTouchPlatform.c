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
#include <Library/PasswordEncodeLib.h>
#include <Library/PttHeciLib.h>
#include <Register/Intel/ArchitecturalMsr.h>

#include "../../../ServerSecurityPkg/Include/Library/PfrLib.h"          // #include <Library/PfrLib.h>
#include "../../../ServerSecurityPkg/Include/Protocol/PfrProtocol.h"    // #include <Protocol/PfrProtocol.h>

#include <Tcg2ConfigNvData.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Guid/SocketIioVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/MeRcVariable.h>

#include <DxeOobOneTouch.h>
#include <DxeOneTouchPlatform.h>
#include <DxeOneTouchTpm20.h>
#include <DxeOneTouchTpm12.h>

#include <Library/PlatformOobOneTouchLib.h>
//-----------------------------------------------------------------------------------------------------
//
EFI_GUID  gPfrProtocolGuid         = { 0x8B841B47, 0xC9B4, 0x46AC, { 0x82, 0x70, 0x84, 0x26, 0x29, 0x62, 0xE1, 0x56 } };
//
EFI_GUID  mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;  // GUID of SYSTEM_CONFIGURATION
UINTN     mSystemConfigurationSize = 0;                          // Size of SYSTEM_CONFIGURATION
//
//-----------------------------------------------------------------------------------------------------
/**
  DisplayBinaryData(): Display binary data.

  @param  Data INPUT: UINT8*, Ptr to data
  @param  Size INPUT: UINTN, Data size in bytes

  @retval None
**/
VOID
DisplayBinaryData (
  UINT8      *Data,
  UINTN     Size
  )
{
  // Local data
  UINTN  i, n;
  UINT8  b, *c, j, l;

  // Display 16bytes per line
  n = (Size >> 4);                            // #of lines (each line is 16bytes)
  l = (UINT8) (Size & 0x0F);                  // #of bytes in last line
  c = Data;                                   // Ptr to start of data bytes
  if ((n != 00) || (l != 00)) {               // If something to display
  DEBUG ((EFI_D_ERROR, "        ----------------------------------------------------------------------\n"));
  DEBUG ((EFI_D_ERROR, "        Offset  00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F\n"));
  DEBUG ((EFI_D_ERROR, "        ----------------------------------------------------------------------\n"));
  if (n != 0) {                               // If #of full 16byte lines valid
    for (i = 0; i < n; i++) {                 // Display all full 16byte lines
      DEBUG ((EFI_D_ERROR, "        %04X  ", (i << 4)));
      for (j = 0; j < 16; j++) {              //   Display all 16bytes in the line
        b = *c; c++;                          //     Get next data byte, Update ptr to data
        DEBUG ((EFI_D_ERROR, "  %02X", b));   //     Display next data byte
      }
      DEBUG ((EFI_D_ERROR, "\n"));            //     Display <CR><LF>
    }
  }
  if (l != 0) {                               // If last line valid
    DEBUG ((EFI_D_ERROR, "        %04X  ", (n << 4)));
    for (j = 0; j < l; j++) {                 //   Display all bytes in last line
      b = *c; c++;                            //     Get next data byte, Update ptr to data
      DEBUG ((EFI_D_ERROR, "  %02X", b));     //     Display next data byte
    }
    DEBUG ((EFI_D_ERROR, "\n"));
  }
  DEBUG ((EFI_D_ERROR, "        ----------------------------------------------------------------------\n"));
}
}
//
//-----------------------------------------------------------------------------------------------------
/**
  OT_PrepareTpmBeforeOobTask(): Prepare TPM before performing OOB Task.

  @param[in]  Opd      INPUT: Ptr to OT_PLATFORM_DATA

  @retval     Result   OUTPUT: UINT8, Status code

**/
UINT8
OT_PrepareTpmBeforeOobTask (
  IN     OT_PLATFORM_DATA  *Opd
  )
{
  // Local data
  UINT8 e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_PrepareTpmBeforeOobTask()  { -> Prepare TPM before performing OOB Task\n"));
  Opd->Tpm12PreparedForTask = FALSE;          // Indicate TPM preparation not performed before task
  e = OXP_SUCCESS;                            // Status code (assume success)

  // Check if TPM1.2
  if (Opd->TcgProtocol == NULL) {             // If not TPM1.2
    DEBUG ((EFI_D_ERROR, "      Not TPM1.2, no preparation required\n"));
  } else {                                    // If TPM1.2
    // Prepare TPM1.2: Ensure TPM can accept physical presence commands
    if (!Opd->Tpm12PermanentFlags.physicalPresenceCMDEnable) {    // If TPM not enabled for accepting commands
      if (Opd->Tpm12PermanentFlags.physicalPresenceLifetimeLock) {//   If TPM Locked
        DEBUG ((EFI_D_ERROR, "      TPM1.2 Locked, cannot accept physical presence command\n"));
        e = OXP_TPM_LOCKED;                                       //     Error
      } else {                                                    //   If TPM not locked
        DEBUG ((EFI_D_ERROR, "      Physical presence command enable:\n"));
        if ((OT_Tpm12PhysicalPresence (Opd->TcgProtocol,          //     Set physical presence command enable
          TPM_PHYSICAL_PRESENCE_CMD_ENABLE)) != TPM_SUCCESS) {
          e = OXP_TPM_PHYSICAL_PRESENCE_CMD_ENABLE_ERROR;         //     Error
        }
      }
    }
  }

  // Set Physical presence flag
  if ((e == OXP_SUCCESS) && (Opd->TcgProtocol != NULL)) {         // If success AND TPM1.2
    DEBUG ((EFI_D_ERROR, "      Set Physical presence as Present:\n"));
    if ((OT_Tpm12PhysicalPresence (Opd->TcgProtocol,              // Set physical presence flag
      TPM_PHYSICAL_PRESENCE_PRESENT)) == TPM_SUCCESS) {
      Opd->Tpm12PreparedForTask = TRUE;                           //   Indicate TPM preparation performed before task
    } else {
      e = OXP_TPM_PHYSICAL_PRESENCE_ERROR;                        //   Error
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_PrepareTpmBeforeOobTask()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                       // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  OT_PrepareTpmAfterOobTask(): Prepare TPM after performing OOB Task.

  @param[in]  Opd    INPUT: Ptr to OT_PLATFORM_DATA

  @retval     Result OUTPUT: UINT8, Status code

**/
UINT8
OT_PrepareTpmAfterOobTask (
  IN     OT_PLATFORM_DATA  *Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_PrepareTpmAfterOobTask()  { -> Prepare TPM after performing OOB Task\n"));
  e = OXP_SUCCESS;                                                // Status code (assume success)

  //--------------------------------------------------------------
  // Check if TPM preparation performed before task
  if (!Opd->Tpm12PreparedForTask) {                               // If TPM preparation not performed before task
    DEBUG ((EFI_D_ERROR, "      Nothing to do (TPM not prepared before task)\n"));
  } else {
    // Prepare TPM1.2
    if (Opd->TcgProtocol != NULL) {                               // If TCG protocol valid
      DEBUG ((EFI_D_ERROR, "      Set physical presence as Not-Present:\n"));
      if ((OT_Tpm12PhysicalPresence (Opd->TcgProtocol,            // Lock physical presence
        TPM_PHYSICAL_PRESENCE_NOTPRESENT)) != TPM_SUCCESS) {
        e = OXP_TPM_PHYSICAL_PRESENCE_ERROR;                      //   Error
      }
    } else {                                                      // If TCG protocol not valid
      e = OXP_UNKNOWN_ERROR;                                      // Error
      DEBUG ((EFI_D_ERROR, "      Implementation Error -> BUG\n"));
    }
  }

  //--------------------------------------------------------------

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_PrepareTpmAfterOobTask()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                       // Status code
}
//
//-----------------------------------------------------------------------------------------------------
/**
  Display BIOS Setup Options.

  @param[in]  Opd  *OT_PLATFORM_DATA: Ptr to OT_PLATFORM_DATA
  @param[in]  Info BOOLEAN: TRUE/FALSE: (Modified before Write)/(Current after Read)

  @retval     None

**/
VOID
OT_DisplayBiosSetupOption (
  IN OUT OT_PLATFORM_DATA  *Opd,
  IN BOOLEAN               Info
  )
{
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DisplayBiosSetupOption()  { -> BIOS Setup Options, "));
  if (Info) { DEBUG ((EFI_D_ERROR, "To be written if Modified\n")); }
  else { DEBUG ((EFI_D_ERROR, "Current as Read\n")); }

  DEBUG ((EFI_D_ERROR, "                                     LTSX: "));
  if (Opd->Spcc.ProcessorLtsxEnable == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "                             Lock Chipset: "));
  if (Opd->Spcc.LockChipset == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "                                      VMX: "));
  if (Opd->Spcc.ProcessorVmxEnable == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "                                      SMX: "));
  if (Opd->Spcc.ProcessorSmxEnable == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "                         MSR Lock Control: "));
  if (Opd->Spcc.ProcessorMsrLockControl == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "                                     VT-d: "));
  if (Opd->Sic.VTdSupport == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "               Detected TPM Device (0x%02x): ", Opd->T2dd.TpmDeviceDetected));
  switch (Opd->T2dd.TpmDeviceDetected) {
    case TPM_DEVICE_2_0_DTPM:
      DEBUG ((EFI_D_ERROR, "TPM 2.0\n")); break;

    case TPM_DEVICE_1_2:
      DEBUG ((EFI_D_ERROR, "TPM 1.2\n")); break;

    case TPM_DEVICE_NULL:
    default:
      DEBUG ((EFI_D_ERROR, "None\n")); break;
  }

  DEBUG ((EFI_D_ERROR, "             Configured TPM Device (0x%02x): ", Opd->T2c.TpmDevice));
  switch (Opd->T2c.TpmDevice) {
    case TPM_DEVICE_2_0_DTPM:
      DEBUG ((EFI_D_ERROR, "TPM 2.0\n")); break;

    case TPM_DEVICE_1_2:
      DEBUG ((EFI_D_ERROR, "TPM 1.2\n")); break;

    case TPM_DEVICE_NULL:
    default:
      DEBUG ((EFI_D_ERROR, "None\n")); break;
  }

//  DEBUG ((EFI_D_ERROR, "                            TCG TPM Device: "));
//  if (Opd->SystemConfig->TpmEnable != 0) { DEBUG ((EFI_D_ERROR, "Enable\n")); }
//  else { DEBUG ((EFI_D_ERROR, "Disable\n")); }

//  DEBUG ((EFI_D_ERROR, "                      TCG TPM Device State: "));
//  if (Opd->SystemConfig->TpmState != 0) { DEBUG ((EFI_D_ERROR, "Activate\n")); }
//  else { DEBUG ((EFI_D_ERROR, "Deactivate\n")); }

  DEBUG ((EFI_D_ERROR, "                               PTT (fTPM): "));
  if (Opd->Merc.MePttEnabled == 0) {
    DEBUG ((EFI_D_ERROR, "Disabled\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "Enabled\n"));
  }

  // TME/MK-TME
  DEBUG ((EFI_D_ERROR, "                                      TME: "));
  if (Opd->Spcc.EnableTme == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "                                   MK-TME: "));
  if (Opd->Spcc.EnableMktme == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }

  // PFR Status
  DEBUG ((EFI_D_ERROR, "                            PFR Supported: "));
  if (Opd->SystemConfig->PfrSupported == 0) { DEBUG ((EFI_D_ERROR, "No\n")); }
  else { DEBUG ((EFI_D_ERROR, "Yes\n")); }
  DEBUG ((EFI_D_ERROR, "                          PFR Lock Status: "));
  if (Opd->SystemConfig->PfrLockStatus == 0) { DEBUG ((EFI_D_ERROR, "Not Locked\n")); }
  else { DEBUG ((EFI_D_ERROR, "Locked\n")); }
  DEBUG ((EFI_D_ERROR, "                     PFR Provision Status: "));
  if (Opd->SystemConfig->PfrProvisionStatus == 0) { DEBUG ((EFI_D_ERROR, "Not Provisioned\n")); }
  else { DEBUG ((EFI_D_ERROR, "Provisioned\n")); }
  DEBUG ((EFI_D_ERROR, "                   PFR PIT Level-1 Status: "));
  if (Opd->SystemConfig->PfrPitL1Status == 0) { DEBUG ((EFI_D_ERROR, "Disabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enabled\n")); }
  DEBUG ((EFI_D_ERROR, "                                 PFR Lock: "));
  if (Opd->SystemConfig->PfrLock == 0) { DEBUG ((EFI_D_ERROR, "Disable\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enable\n")); }
  DEBUG ((EFI_D_ERROR, "                            PFR Provision: "));
  if (Opd->SystemConfig->PfrProvision == 0) { DEBUG ((EFI_D_ERROR, "Disable\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enable\n")); }
  DEBUG ((EFI_D_ERROR, "                          PFR UnProvision: "));
  if (Opd->SystemConfig->PfrUnProvision == 0) { DEBUG ((EFI_D_ERROR, "Disable\n")); }
  else { DEBUG ((EFI_D_ERROR, "Enable\n")); }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DisplayBiosSetupOption()  }\n"));
}

//------------------------------------------------------------------------------------------------------
/**
  OT_GetTxtInfo(): Return TXT support and enabled information.

  @param  Opd       INPUT:  OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
  @param  Support   INPUT:  UINT16*, Ptr to FeatureSupported
                    OUTPUT: UINT16*, Ptr to FeatureSupported filled with proper
                                     bit-mapped info of TXT
  @param  Enabled   INPUT:  UINT16*, Ptr to FeatureEnabled
                    OUTPUT: UINT16*, Ptr to FeatureEnabled filled with proper
                                     bit-mapped info of TXT
**/
VOID
OT_GetTxtInfo (
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT16            *Support,
  IN OUT UINT16            *Enabled
  )
{
  // Local data
  UINT16  te, ts;                                            // TXT enabled, TXT support information
  UINT64  q;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetTxtInfo()  { -> TXT Support and Enabled Information\n"));
  ts = 0; te = 0;                                            // Init TXT support info, Init TXT enabled info

  // TXT Support information: Read LT register EXISTS at Physical Address FED30010h
  q = (UINT64)(UINTN) *((UINTN*)(UINTN)LTCS_EXISTS_REGISTER);// Bit-mapped info of LT capable threads that registered at reset
  if (q != 0) { ts = OXP_TXT_SUPPORT_BIT; }                  // TXT supported

  // TXT Enabled information: ts = bit-mapped TXT support information
  if (((ts & OXP_TXT_SUPPORT_BIT) != 0) &&                   // If TXT supported AND
      (Opd->Spcc.ProcessorLtsxEnable == 1)) {                //    TXT enabled
    te = OXP_TXT_ENABLED_BIT;                                //   Indicate TXT Enabled
  }

  // Display TXT support information: ts, te = bit-mapped TXT support, enabled information
  DEBUG ((EFI_D_ERROR, "      [LT EXISTS register at 0x%08x]: 0x%08x%08x\n", LTCS_EXISTS_REGISTER, (UINT32)(UINTN) (q >> 32), (UINT32)(UINTN) q));
  DEBUG ((EFI_D_ERROR, "      TXT Support Information Bitmap 0x%x -> TXT ", ts));
  if ((ts & OXP_TXT_SUPPORT_BIT) != 0) { DEBUG ((EFI_D_ERROR, "Supported\n")); }
  else { DEBUG ((EFI_D_ERROR, "Not Supported\n")); }

  // Display TXT enabled information: ts, te = bit-mapped TXT support, enabled information
  DEBUG ((EFI_D_ERROR, "      TXT Enabled Information Bitmap 0x%x -> TXT ", te));
  if ((te & OXP_TXT_ENABLED_BIT) != 0) { DEBUG ((EFI_D_ERROR, "Enabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "Not Enabled\n")); }

  // ts, te = bit-mapped TXT support, enabled information
  if ((Support == NULL) || (Enabled == NULL)) {              // If any parameter invalid
    DEBUG ((EFI_D_ERROR, "    ERROR: Invalid Input Parameter\n"));
  } else {                                                   // If all parameters valid
  *Support |= ts; *Enabled |= te;                            // Return Support and Enabled info
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetTxtInfo()  }\n"));
}

//------------------------------------------------------------------------------------------------------
/**
  OT_GetTpmInfo(): Return TPM support and enabled information.

  @param  Opd       INPUT:  OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
  @param  Support   INPUT:  UINT16*, Ptr to FeatureSupported
                    OUTPUT: UINT16*, Ptr to FeatureSupported filled with proper
                                     bit-mapped info of TPM
  @param  Enabled   INPUT:  UINT16*, Ptr to FeatureEnabled
                    OUTPUT: UINT16*, Ptr to FeatureEnabled filled with proper
                                     bit-mapped info of TPM
  @param  Usage     INPUT:  UINT16*, Ptr to TpmUsage
                    OUTPUT: UINT16*, Ptr to TpmUsage filled with proper
                                     bit-mapped info of TPM Usage
**/
VOID
OT_GetTpmInfo (
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT16            *Support,
  IN OUT UINT16            *Enabled,
  IN OUT UINT16            *Usage
  )
{
  // Local data
  UINT64	q;
  UINT32	d;
  UINT16	te, ts, tu;                                                 // TPM enabled, support, usage info
  BOOLEAN	pe, ps;                                                    // PTT enabled, support info

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetTpmInfo()  { -> TPM Support and Enabled Information\n"));

  //--------------------------------------------------------------
  // Find PTT (fTPM) support and enabled information from ME
  DEBUG ((EFI_D_ERROR, "      Find PTT (fTPM) information from ME: PTT -> "));
  ps = FALSE;                                                        // Indicate PTT not supported
  if ((PttHeciGetCapability (&ps)) != EFI_SUCCESS) { ps = FALSE; }   // Find if PTT supported
  if (ps) { DEBUG ((EFI_D_ERROR, "Supported, ")); }
  else { DEBUG ((EFI_D_ERROR, "Not Supported, ")); }

  pe = FALSE;                                                        // Indicate PTT not enabled
  if (ps) {
    if ((PttHeciGetState (&pe)) != EFI_SUCCESS) { pe = FALSE; }      //   Find if PTT enabled in ME
  }
  if (pe) {                                                          //   If PTT enabled in ME
    DEBUG ((EFI_D_ERROR, "Enabled\n"));
    if (Opd->Merc.MePttEnabled == 0) {                               //     If PTT disabled in Setup
      DEBUG ((EFI_D_ERROR, "      Ensure PTT enabled in ME Configuration Setup\n"));
      Opd->Merc.MePttEnabled = 1; Opd->Mercm = TRUE;                 //     Ensure PTT is enabled in Setup, Data modified
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Not Enabled\n"));
  }

  //--------------------------------------------------------------
  // TPM support information: ps = TRUE/FALSE PTT yes/not supported, pe = TRUE/FALSE PTT yes/not enabled
  DEBUG ((EFI_D_ERROR, "    Find TPM Support information: Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));
  ts = 0;                                                            // Init TPM support info

  // Read LT EMC Version Number register at Physical Address FED30200h
  // LTCS_EMIF_REGISTER   0x00000000FED30200
  d = (UINT32)(UINTN) *((UINTN*)(UINTN)LTCS_EMIF_REGISTER);          // [LT EMIF register]
  DEBUG ((EFI_D_ERROR, "        [LT EMIF register at 0x%08x]: 0x%08x", LTCS_EMIF_REGISTER, d));
  DEBUG ((EFI_D_ERROR, ", Bit28:27 = 0x%02x, Bit24:22 = 0x%02x\n", ((UINT8)(UINTN) ((d >> 27) & 0x03)), ((UINT8)(UINTN) ((d >> 22) & 0x07))));
  // Read TPM I/F Version Number register at Physical Address FED30800h
  // LTCS_FTIF_REGISTER 0x00000000FED30800
  q = (UINT64)(UINTN) *((UINTN*)(UINTN)LTCS_FTIF_REGISTER);          // [TPM I/F Version Number register]
  DEBUG ((EFI_D_ERROR, "        [LT FTIF register at 0x%08x]: 0x%08x%08x", LTCS_FTIF_REGISTER, (UINT32)(UINTN) (q >> 32), (UINT32)(UINTN) q));
  DEBUG ((EFI_D_ERROR, ", Bit18:16 = 0x%02x\n", ((UINT8)(UINTN) ((q >> 16) & 0x07))));

  if (Opd->T2dd.TpmDeviceDetected != TPM_DEVICE_NULL) { ts |= OXP_DTPM_SUPPORT_BIT; }// If any TPM device detected, indicate dTPM supported
  if (ps) { ts |= OXP_FTPM_SUPPORT_BIT; }                            // If PTT supported, indicate fTPM supported

  // Display TPM support information
  DEBUG ((EFI_D_ERROR, "    TPM Support Information Bitmap 0x%04x -> ", ts));
  if ((ts & OXP_DTPM_SUPPORT_BIT) != 0) { DEBUG ((EFI_D_ERROR, "dTPM Supported, ")); }
  else { DEBUG ((EFI_D_ERROR, "dTPM Not Supported, ")); }
  if ((ts & OXP_FTPM_SUPPORT_BIT) != 0) { DEBUG ((EFI_D_ERROR, "fTPM Supported\n")); }
  else { DEBUG ((EFI_D_ERROR, "fTPM Not Supported\n")); }

  //--------------------------------------------------------------
  // TPM enabled, usage information: ps = TRUE/FALSE PTT yes/not supported, pe = TRUE/FALSE PTT yes/not enabled
  // ts = bit-mapped TPM support information
  DEBUG ((EFI_D_ERROR, "    Find TPM Enabled, Usage information: Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));
  te = 0; tu = 0;                                                    // Init TPM enabled, usage info
  if ((ts & OXP_TPM_SUPPORT_BIT_MASK) != 0) {                        // If TPM supported
    switch (Opd->T2dd.TpmDeviceDetected) {
      case TPM_DEVICE_2_0_DTPM:                                      // TPM2.0
      DEBUG ((EFI_D_ERROR, "        TPM2.0: "));
        if ((ps) && (pe)) {                                          // If fTPM supported AND PTT enabled
          te = OXP_FTPM_ENABLED_BIT;                                 //   Indicate fTPM enabled
          DEBUG ((EFI_D_ERROR, "fTPM Enabled\n"));
        } else {
          te = OXP_DTPM_ENABLED_BIT;                                 // Indicate dTPM enabled
          DEBUG ((EFI_D_ERROR, "dTPM Enabled\n"));
        }
        tu = (OT_GetNvIndexInfoTpm20 (Opd)) |                        // TPM Usage information
                (OXP_TPM_VERSION_TPM20 << OXP_TPM_VERSION_BIT_N);
      break;

      case TPM_DEVICE_1_2:                                           // TPM1.2
        DEBUG ((EFI_D_ERROR, "        TPM1.2: Get TPM Permanent Flags\n"));
        if ((OT_GetTpm12PermanentFlags (Opd)) == TPM_SUCCESS) {
          DEBUG ((EFI_D_ERROR, "        dTPM: "));
          if (Opd->Tpm12PermanentFlags.disable) {                      // If disabled
            DEBUG ((EFI_D_ERROR, "Disabled/"));
          } else {                                                     // If enabled
            te = OXP_DTPM_ENABLED_BIT;                                 //   Indicate dTPM enabled
            DEBUG ((EFI_D_ERROR, "Enabled/"));
          }
          if (Opd->Tpm12PermanentFlags.deactivated) {                  // If deactivated
            DEBUG ((EFI_D_ERROR, "Deactivated, "));
          } else {                                                     // If activated
            DEBUG ((EFI_D_ERROR, "Activated, "));
          }
          DEBUG ((EFI_D_ERROR, "Life Time Lock: "));
          if (Opd->Tpm12PermanentFlags.physicalPresenceLifetimeLock) { // If Life Time Locked
            DEBUG ((EFI_D_ERROR, "Set, "));
          } else {
            DEBUG ((EFI_D_ERROR, "Not set, "));
          }
          DEBUG ((EFI_D_ERROR, "Physical Presence HW: "));
          if (Opd->Tpm12PermanentFlags.physicalPresenceHWEnable) {     // If Physical Presence HW Enabled
            DEBUG ((EFI_D_ERROR, "Enabled, "));
          } else {
            DEBUG ((EFI_D_ERROR, "Not-Enabled, "));
          }
          DEBUG ((EFI_D_ERROR, "Physical Presence Command: "));
          if (Opd->Tpm12PermanentFlags.physicalPresenceCMDEnable) {    // If Physical Presence Command Enabled
            DEBUG ((EFI_D_ERROR, "Enabled\n"));
          } else {
            DEBUG ((EFI_D_ERROR, "Not-Enabled\n"));
          }
        }
        if ((te & OXP_DTPM_ENABLED_BIT) != 0) {                        // If enabled
          tu = (OT_GetNvIndexInfoTpm12 (Opd)) |                        //   TPM Usage information
          (OXP_TPM_VERSION_TPM12 << OXP_TPM_VERSION_BIT_N);
        }
        break;

      //case TPM_DEVICE_NULL:                                          // No TPM
      default:                                                         // Invalid
      DEBUG ((EFI_D_ERROR, "        No/Invalid TPM device\n"));
      break;
    }
  }

  // Display TPM enabled, usage information
  DEBUG ((EFI_D_ERROR, "    TPM Usage Information Bitmap 0x%04x\n", tu));
  DEBUG ((EFI_D_ERROR, "    TPM Enabled Information Bitmap 0x%04x -> ", te));
  if ((te & OXP_DTPM_ENABLED_BIT) != 0) { DEBUG ((EFI_D_ERROR, "dTPM Enabled, ")); }
  else { DEBUG ((EFI_D_ERROR, "dTPM Not Enabled, ")); }
  if ((te & OXP_FTPM_ENABLED_BIT) != 0) { DEBUG ((EFI_D_ERROR, "fTPM Enabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "fTPM Not Enabled\n")); }

  //--------------------------------------------------------------
  // ts, te, tu = bit-mapped TPM support, enabled, usage information
  if ((Support == NULL) || (Enabled == NULL) || (Usage == NULL)) {    // If any parameter inavlid
    DEBUG ((EFI_D_ERROR, "    ERROR: Invalid Input Parameter\n"));
  } else {                                                            // If all parameters valid
    *Support |= ts; *Enabled |= te; *Usage |= tu;                     //   Return Support, Enabled, Usage info
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetTpmInfo()  }\n"));
}

//------------------------------------------------------------------------------------------------------
/**
  OT_GetMkTmeInfo(): Return TME/MK-TME support and enabled information.

  @param  Opd          INPUT:  OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
  @param  Support      INPUT:  UINT16*, Ptr to FeatureSupported
                       OUTPUT: UINT16*, Ptr to FeatureSupported filled with proper
                                    bit-mapped info of TME/MK-TME
  @param  Enabled      INPUT:  UINT16*, Ptr to FeatureEnabled
                       OUTPUT: UINT16*, Ptr to FeatureEnabled filled with proper
                                    bit-mapped info of TME/MK-TME
  @param  MaxKeys      INPUT:  UINT16*, Ptr to area
                       OUTPUT: UINT16*, Ptr to area filled with max #of keys available for use
  @param  MaxKeyIdBits INPUT:  UINT8*,  Ptr to area
                       OUTPUT: UINT8*,  Ptr to area filled with max #of Bits allowed for use as Key Identifiers for MK-TME
  @param  KeyIdBits    INPUT:  UINT8*,  Ptr to area
                       OUTPUT: UINT8*,  Ptr to area filled with #of Bits allocated for use as Key Identifiers for MK-TME
**/
VOID
OT_GetMkTmeInfo (
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT16            *Support,
  IN OUT UINT16            *Enabled,
  IN OUT UINT16            *MaxKeys,
  IN OUT UINT8             *MaxKeyIdBits,
  IN OUT UINT8             *KeyIdBits
  )
{
  // Local data
  TME_ACTIVATE     ta;
  TME_CAPABILITY   tc;
  UINT32           a, b, c, d;
  UINT16           e, k, s;                                      // Enabled, Keys, Support info
  UINT8            mkb, kba;                                     // Max #of Key ID Bits allowed, #of Key ID Bits allocated

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetMkTmeInfo() { TME/MK-TME Information\n"));
  a = 0; b = 0; c = 0; d = 0;                                    // Init output of CPUID
  k = 0; mkb = 0; kba = 0;                                       // Init keys info
  e = 0; s = 0;                                                  // Init enabled, supported info

  //----------------------------------------------------------------------
  // TME/MK-TME information
  DEBUG ((EFI_D_ERROR, "    Find TME Support information: CPUID Input EAX = 7, Input ECX = 0, Output ECX Bit-13\n"));
  AsmCpuidEx (CPUID_LEAF_EAX_EXTENDED_FEATURE_FLAG,              // c = Output value of ECX after executing CPUID
              CPUID_SUBLEAF_ECX_EXTENDED_FEATURE_FLAG, &a, &b, &c, &d);
  DEBUG ((EFI_D_ERROR, "        CPUID Output: EAX = 0x%08x, EBX = 0x%08x, ECX = 0x%08x, EDX = 0x%08x\n", a, b, c, d));
  if ((c & CPUID_OUTPUT_ECX_TME_SUPPORT_BIT) != 0) {             // If TME supported (i.e. TME MSRs are available)
    s |= OXP_TME_SUPPORT_BIT;                                    //   TME Supported

    tc.data = AsmReadMsr64 (IA32_TME_CAPABILITY_MSR);            //   Read MSR 981h
    DEBUG ((EFI_D_ERROR, "    [IA32_TME_CAPABILITY_MSR 981h] = 0x%08x %08x\n", (UINT32)(UINTN) (tc.data >> 32), (UINT32)(UINTN) (tc.data)));
    mkb = (UINT8)(UINTN) (tc.bits.MaxKeyIdBits);                 //   Max #of bits allowed for use as Key Identifiers for MK-TME
    k = (UINT16)(UINTN) (tc.bits.MaxNumKeys);                    //   Max #of keys available for use
    if ((mkb != 0) && (k != 0)) { s |= OXP_MKTME_SUPPORT_BIT; }  //   MK-TME supported

    ta.data = AsmReadMsr64 (IA32_TME_ACTIVATE_MSR);              //   Read MSR 982h
    DEBUG ((EFI_D_ERROR, "    [IA32_TME_ACTIVATE_MSR 982h] = 0x%08x %08x\n", (UINT32)(UINTN) (ta.data >> 32), (UINT32)(UINTN) (ta.data)));
    kba = (UINT8)(UINTN) (ta.bits.KeyIdBits);                    //   #of bits allocated for use as Key Identifiers for MK-TME
    if (ta.bits.TmeEnableRwl != 0) {                             //   If TME enabled
      e |= OXP_TME_ENABLED_BIT;                                  //     TME enabled
      if (kba != 0) { e |= OXP_MKTME_ENABLED_BIT; }              //     MK-TME Enabled
    }
  }
  //----------------------------------------------------------------------
  // Display TME/MK-TME information
  DEBUG ((EFI_D_ERROR, "    TME/MK-TME Supported Information Bitmap 0x%04x -> ", s));
  if ((s & OXP_TME_SUPPORT_BIT) != 0) { DEBUG ((EFI_D_ERROR, "TME Supported, ")); }
  else { DEBUG ((EFI_D_ERROR, "TME Not Supported, ")); }
  if ((s & OXP_MKTME_SUPPORT_BIT) != 0) { DEBUG ((EFI_D_ERROR, "MK-TME Supported\n")); }
  else { DEBUG ((EFI_D_ERROR, "MK-TME Not Supported\n")); }

  DEBUG ((EFI_D_ERROR, "    TME/MK-TME Enabled Information Bitmap 0x%04x -> ", e));
  if ((e & OXP_TME_ENABLED_BIT) != 0) { DEBUG ((EFI_D_ERROR, "TME Enabled, ")); }
  else { DEBUG ((EFI_D_ERROR, "TME Not Enabled, ")); }
  if ((e & OXP_MKTME_ENABLED_BIT) != 0) { DEBUG ((EFI_D_ERROR, "MK-TME Enabled\n")); }
  else { DEBUG ((EFI_D_ERROR, "MK-TME Not Enabled\n")); }

  DEBUG ((EFI_D_ERROR, "    Max number of MK-TME Keys available for use: 0x%04x\n", k));
  DEBUG ((EFI_D_ERROR, "    Max number of Bits allowed for use as Key Identifiers for MK-TME: 0x%02x\n", mkb));
  DEBUG ((EFI_D_ERROR, "    Number of Bits allocated for use as Key Identifiers for MK-TME: 0x%02x\n", kba));

  //--------------------------------------------------------------
  // s, e, k, kbp, kbc = support, enabled, keys information
  if ((Support == NULL) || (Enabled == NULL) ||                 // If any parameter invalid
      (MaxKeys == NULL) || (MaxKeyIdBits == NULL) || (KeyIdBits == NULL)) {
    DEBUG ((EFI_D_ERROR, "    ERROR: Invalid Input Parameter\n"));
  } else {                                                      // If any parameter valid
    *Support |= s; *Enabled |= e;                               //   Return Support and Enabled info
    *MaxKeys = k; *MaxKeyIdBits = mkb; *KeyIdBits = kba;        //   Return Keys info
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetMkTmeInfo() }\n"));
}

//------------------------------------------------------------------------------------------------------
/**
  OT_GetSgxInfo(): Return SGX support and enabled information.

  @param  Opd       INPUT:  OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
  @param  Support   INPUT:  UINT16*, Ptr to FeatureSupported
                    OUTPUT: UINT16*, Ptr to FeatureSupported filled with proper bit-mapped info of SGX
  @param  Enabled   INPUT:  UINT16*, Ptr to FeatureEnabled
                    OUTPUT: UINT16*, Ptr to FeatureEnabled filled with proper bit-mapped info of SGX
**/
VOID
OT_GetSgxInfo (
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT16            *Support,
  IN OUT UINT16            *Enabled
  )
{
  // Local data
  MSR_IA32_FEATURE_CONTROL_REGISTER  fcr;
  UINT32                             a, b, c, d;
  UINT16                             e, s;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetSgxInfo() { SGX Support and Enabled Information\n"));
  e = 0; s = 0; fcr.Uint64 = 0;                              // Init SGX enabled, support info

  //----------------------------------------------------------------------
  // SGX Support/Enable information
  DEBUG ((EFI_D_ERROR, "    CPUID Input EAX = 7, Input ECX = 0: Output EBX Bit-2\n"));
  AsmCpuidEx (0, 0, &a, &b, &c, &d);                         // a = Output value of EAX after executing CPUID
  if (a < CPUID_LEAF_EAX_EXTENDED_FEATURE_FLAG) {            // If CPUID (with EAX = 7) not supported
    DEBUG ((EFI_D_ERROR, "      ERROR: CPUID instruction not supported for Input EAX = 7\n"));
  } else {                                                   // If CPUID (with EAX = 7) supported
    AsmCpuidEx (CPUID_LEAF_EAX_EXTENDED_FEATURE_FLAG,        //   b = Output value of EBX after executing CPUID
                CPUID_SUBLEAF_ECX_EXTENDED_FEATURE_FLAG, &a, &b, &c, &d);
    DEBUG ((EFI_D_ERROR, "      CPUID Output: EAX = 0x%08x, EBX = 0x%08x, ECX = 0x%08x, EDX = 0x%08x\n", a, b, c, d));
    if (b & CPUID_OUTPUT_EBX_SGX_SUPPORT_BIT) {              //   If SGX supported
      s |= OXP_SGX_SUPPORT_BIT;                              //     Indicate SGX support
      fcr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);  //     Read IA32_FEATURE_CONTROL MSR
      DEBUG ((EFI_D_ERROR, "    [IA32_FEATURE_CONTROL MSR 3Ah] = 0x%08x %08x\n", (UINT32)(UINTN) (fcr.Uint64 >> 32), (UINT32)(UINTN) (fcr.Uint64)));
      if (fcr.Bits.SgxEnable == 1) {                         //     If SGX Global Enable (Bit-18) = 1?
        e |= OXP_SGX_ENABLED_BIT;                            //       Indicate SGX enabled
      }
    }
  }

  //----------------------------------------------------------------------
  // Display SGX information
  DEBUG ((EFI_D_ERROR, "    SGX Supported Bitmap 0x%04x -> SGX ", s));
  if ((s & OXP_SGX_SUPPORT_BIT) != 0) { DEBUG ((EFI_D_ERROR, "Supported, ")); }
  else { DEBUG ((EFI_D_ERROR, "Not Supported\n")); }
  DEBUG ((EFI_D_ERROR, "    SGX Enabled Bitmap 0x%04x -> SGX ", e));
  if ((e & OXP_TME_ENABLED_BIT) != 0) { DEBUG ((EFI_D_ERROR, "Enabled, ")); }
  else { DEBUG ((EFI_D_ERROR, "Not Enabled\n")); }

  //----------------------------------------------------------------------
  // s, e = support, enabled information
  if ((Support == NULL) || (Enabled == NULL)) {              // If any parameter invalid
    DEBUG ((EFI_D_ERROR, "    ERROR: Invalid Input Parameter\n"));
  } else {                                                   // If any parameter valid
    *Support |= s; *Enabled |= e;                            //   Return Support and Enabled info
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetSgxInfo() }\n"));
}

//------------------------------------------------------------------------------------------------------
// **************************** PFR functions { ********************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_PfrInitProtocolApiData(): Initialize PFR API Data.

  @param  Pad       INPUT/OUTPUT: PFR_API_DATA*, ptr to PFR_API_DATA
          Function  INPUT: PFR API Function#
          Pp        INPUT: PFR_PROTOCOL*, Ptr to PFR_PROTOCOL

  @retval None

**/
//----------------------------------------------------------------------------------------------
VOID
OT_PfrInitProtocolApiData (
  IN OUT PFR_API_DATA  *Pad,
  IN     UINT8          Function,
  IN     PFR_PROTOCOL  *Pp
  )
{
  // Local data
  UINT16   w;

  ZeroMem ((UINT8*)(UINTN) Pad, sizeof (PFR_API_DATA));    // Init with 00

  // Find PFR_API_FUNC_INFO size for requested Function
  switch (Function) {
    case PFR_API_FUNCTION_00: w = sizeof (PFR_API_FUNC_INFO_00); break;
    case PFR_API_FUNCTION_02: w = sizeof (PFR_API_FUNC_INFO_02); break;
    default: w = 0; break;
  }

  // Init Header
  Pad->Header.This = Pp;                                   // Ptr to Protocol
  Pad->Header.HeaderSize = sizeof (PFR_API_HEADER);        // Header Size in bytes
  Pad->Header.TotalSize = (sizeof (PFR_API_HEADER) + w);   // Total Size in bytes
  //Pad->Header.Checksum = 0;                              // Init checksum (already done)
  Pad->Header.Function = Function;                         // Function
  Pad->Header.Status = 0xFF;                               // Init Status to a non-zero value
  //Pad->Header.ExtendedStatus = 0x0000;                   // Init Extended Status to zero (already done)

}

//------------------------------------------------------------------------------------------------------
/**
  OT_PfrFunction(): Perform PFR PROTOCOL function.

  @param  PfrProtocol  INPUT: PFR_PROTOCOL*, Ptr to PFR_PROTOCOL
          Function     INPUT: UINT8, PFR Protocol Function# 00 or 02
          Register     INPUT: UINT8, PFR Register# (valid for Function# 02)
          Data         INPUT/OUTPUT: UINT8*, Ptr to data to return

  @retval Result       BOOLEAN, TRUE/FALSE -> Success/Error
**/
BOOLEAN
OT_PfrFunction (
  IN     PFR_PROTOCOL  *PfrProtocol,
  IN     UINT8         Function,
  IN     UINT8         Register,
  IN OUT UINT16        *Data
  )
{
  // Local data
  PFR_API_DATA  pad;
  BOOLEAN       b = FALSE;                                   // Assume Error

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_PfrFunction() { Function 0x%02x, Register 0x%02x\n", Function, Register));

  if ((PfrProtocol != NULL) && (Data != NULL)) {             // If valid
    *Data = 0;                                               //   Init return data
    OT_PfrInitProtocolApiData (&pad, Function, PfrProtocol); //   Init PFR API Data
    if (Function == PFR_API_FUNCTION_02) {                   //   If Function-02
      pad.Info.Function02.Register = Register;               //     PFR Register
    }
    pad.Header.Checksum = 0 -                                //   Update Checksum in Header
               CalculateSum8 ((UINT8*)(UINTN) &pad, pad.Header.TotalSize);
    if ((PfrProtocol->PfrApi (&pad)) == EFI_SUCCESS) {       //   If Function success
      if ((pad.Header.ExtendedStatus &                       // If PFR API data updated
                      PFR_API_ES_DATA_UPDATED_BIT) != 0) {
        if ((CalculateSum8 ((UINT8*)(UINTN) &pad,            // If output checksum valid
               pad.Header.TotalSize)) == 0) { b = TRUE; }    //   Success
      }
    }
    if (b) {                                                 //   If success
      if (Function == PFR_API_FUNCTION_00) {                 //     If Function-00
        *Data = pad.Info.Function00.State;                   //       PFR State
      } else {
        if (Function == PFR_API_FUNCTION_02) {               //     If Function-02
          *Data = (UINT16)(UINTN) pad.Info.Function02.Data;  //       Content of PFR Register
        }
      }
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_PfrFunction() } "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success, [Register 0x%02x] = 0x%02x\n", Function, Register, *Data)); }
  else { DEBUG ((EFI_D_ERROR, "ERROR\n")); }
  return b;

}

//------------------------------------------------------------------------------------------------------
/**
  OT_GetPfrInfo(): Return PFR support and enabled information.

  @param  Opd                INPUT:  OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
  @param  Support            INPUT:  UINT16*, Ptr to FeatureSupported
                             OUTPUT: UINT16*, Ptr to FeatureSupported filled with proper bit-mapped info of PFR
  @param  Enabled            INPUT:  UINT16*, Ptr to FeatureEnabled
                             OUTPUT: UINT16*, Ptr to FeatureEnabled filled with proper bit-mapped info of PFR
  @param  State              INPUT:  UINT16*, Ptr to area
                             OUTPUT: UINT16*, Ptr to area filled with PFR State
  @param  RecoveryCount      INPUT:  UINT8*,  Ptr to area
                             OUTPUT: UINT8*,  Ptr to area filled with Recovery Count
  @param  LastRecoveryReason INPUT:  UINT8*,  Ptr to area
                             OUTPUT: UINT8*,  Ptr to area filled with Last Recovery Reason
  @param  PanicEventCount    INPUT:  UINT8*,  Ptr to area
                             OUTPUT: UINT8*,  Ptr to area filled with Panic Event Count
  @param  LastPanicReason    INPUT:  UINT8*,  Ptr to area
                             OUTPUT: UINT8*,  Ptr to area filled with Last Panic Reason
**/
VOID
OT_GetPfrInfo (
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT16            *Support,
  IN OUT UINT16            *Enabled,
  IN OUT UINT16            *State,
  IN OUT UINT8             *RecoveryCount,
  IN OUT UINT8             *LastRecoveryReason,
  IN OUT UINT8             *PanicEventCount,
  IN OUT UINT8             *LastPanicReason
  )
{
  // Local data
  PFR_PROTOCOL  *pp;
  UINT16        e, s, ps, t;
  UINT8         rc, lrr, pec, lpr;
  BOOLEAN       b;
 
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() { PFR Information\n"));
  e = 0; s = 0;                                            // Enabled, Supported information
  t = 0; ps = 0; rc = 0; lrr = 0; pec = 0; lpr = 0;

  //--------------------------------------------------------------
  // Locate PFR_PROTOCOL
  b = FALSE; pp = NULL;                                    // Assume Error
  if ((gBS->LocateProtocol (&gPfrProtocolGuid, NULL,       // Locate PFR Protocol
            (VOID**)(UINTN) &pp)) == EFI_SUCCESS) {        // If success
    if (pp != NULL) { b = TRUE; }                          //   If ptr to prootocol valid, Success
  }
  if (!(b)) { DEBUG((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() -> ERROR: PFR Protocol Not Found\n")); }

  //--------------------------------------------------------------------
  // Find PFR State using Function-00: pp = ptr to PFR_PROTOCOL
  if (b) {                                                 // If success so far
    if (!(OT_PfrFunction (pp, PFR_API_FUNCTION_00,         //   Pfr State
                          0, &t))) {
      DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() -> ERROR: PFR_PROTOCOL Function-00 error\n"));
    } else {                                               //   If success
      // PFR Info: Support, Enabled, Lock, Provision, PIT Level-1, #of CPLD, Flash Update
      if (t & PP00_PS_PFR_SUPPORT_BIT) { s |= OXP_PFR_SUPPORT_BIT; ps |= OXP_DI_PFR_SUPPORTED_BIT; }
      if (t & PP00_PS_PFR_PROVISIONED_BIT) { e |= OXP_PFR_ENABLED_BIT; ps |= OXP_DI_PFR_PROVISIONED_BIT; }
      if (t & PP00_PS_PFR_LOCKED_BIT) { ps |= OXP_DI_PFR_LOCKED_BIT; }
      if (t & PP00_PS_PFR_PIT_L1_ENABLED_BIT) { ps |= OXP_DI_PFR_PIT_L1_ENABLED_BIT; }
      if (t & PP00_PS_PFR_PIT_L2_ENABLED_BIT) { ps |= OXP_DI_PFR_PIT_L2_ENABLED_BIT; }
      if (t & PP00_PS_PFR_FLASH_UPDATE_BIT) { ps |= OXP_DI_PFR_FLASH_UPDATE_BIT; }
      t = ((t & PP00_PS_PFR_NUM_CPLD_BIT_MASK) >> PP00_PS_PFR_NUM_CPLD_BIT_N);
      ps |= ((t << OXP_DI_PFR_NUM_CPLD_BIT_N) & OXP_DI_PFR_NUM_CPLD_BIT_MASK);
    }
  }

  //--------------------------------------------------------------------
  // Read PFR registers: pp = ptr to PFR_PROTOCOL, (s, e) = PFR (Support, Enabled) bitmap
  if ((b) && (s & OXP_PFR_SUPPORT_BIT)) {                  // If success so far AND PFR supported
    if (!(OT_PfrFunction (pp, PFR_API_FUNCTION_02,         //   Recovery Count
                          CPLD_RECOVERY_COUNT_REGISTER, &t))) {
      DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() -> ERROR: PFR Register 0x%02x Read error\n", CPLD_RECOVERY_COUNT_REGISTER));
    } else { rc = (UINT8)(UINTN) t; }
    if (!(OT_PfrFunction (pp, PFR_API_FUNCTION_02,         //   Last Recovery Reason
                          CPLD_LAST_RECOVERY_REASON_REGISTER, &t))) {
      DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() -> ERROR: PFR Register 0x%02x Read error\n", CPLD_LAST_RECOVERY_REASON_REGISTER));
    } else { lrr = (UINT8)(UINTN) t; }
    if (!(OT_PfrFunction (pp, PFR_API_FUNCTION_02,         //   Panic Event Count
                          CPLD_PANIC_EVENT_COUNT_REGISTER, &t))) {
      DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() -> ERROR: PFR Register 0x%02x Read error\n", CPLD_PANIC_EVENT_COUNT_REGISTER));
    } else { pec = (UINT8)(UINTN) t; }
    if (!(OT_PfrFunction (pp, PFR_API_FUNCTION_02,         //   Last Panic Reason
                          CPLD_LAST_PANIC_REASON_REGISTER, &t))) {
      DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() -> ERROR: PFR Register 0x%02x Read error\n", CPLD_LAST_PANIC_REASON_REGISTER));
    } else { lpr = (UINT8)(UINTN) t; }
  }

  //--------------------------------------------------------------
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() -> PFR Support Bitmap: 0x%04x, PFR Enabled Bitmap: 0x%04x\n", s, e));
  DEBUG ((EFI_D_ERROR, "               PFR State: 0x%04x, Recovery Count: 0x%02x, Last Recovery Reason 0x%02x\n", ps, rc, lrr));
  DEBUG ((EFI_D_ERROR, "               Panic Event Count: 0x%02x, Last Panic Reason 0x%02x\n", pec, lpr));

  *Support |= s; *Enabled |= e; *State = ps;               // Support, Enabled info, PFR State
  *RecoveryCount = rc; *LastRecoveryReason = lrr;          // Recovery info
  *PanicEventCount = pec; *LastPanicReason = lpr;          // Panic info

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_GetPfrInfo() }\n"));
}

//------------------------------------------------------------------------------------------------------
// **************************** PFR functions } ********************************************************
//------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------

//----------------------------- Password Functions { ---------------------------------------------------
//-------------------------------------------------------------------------------------------------------
/**
  Returns the information of different installed passwords.

  @param[in]       Data     SYSTEM_CONFIGURATION*: Ptr to System configuration

  @retval          p        UINT8:    Bit-mapped information of installed passwords
                                      OT_NO_PASSWORD	No password installed
                                      OT_ADMIN_PASSWORD	Administrative password installed
                                      OT_USER_PASSWORD	User password installed
**/
UINT8
OT_CheckIfPasswordInstalled (
  IN  SYSTEM_CONFIGURATION  *Data
  )
{
  // Local data
  UINT8    p, a[SHA256_DIGEST_LENGTH];

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_CheckIfPasswordInstalled()  { -> Check if Password installed in BIOS\n"));
  p = OT_NO_PASSWORD;                                                 // No password installed
  ZeroMem (&a, sizeof (a));                                           // Init with 0

  if (Data != NULL) {                                                 // If Setup Data valid
    if ((CompareMem (Data->AdminPassword, a, sizeof (a))) != 0) {
      p |= OT_ADMIN_PASSWORD;                                         //   Admin password installed
    }
    if ((CompareMem (Data->UserPassword, a, sizeof (a))) != 0) {
      p |= OT_USER_PASSWORD;                                          //   User password installed
      }
    }

  OT_CheckIfPasswordInstalledOEM(&p);
  DEBUG ((EFI_D_ERROR, "      Installed Password Bitmap 0x%x\n", p));

  if (p == OT_NO_PASSWORD) {                                          // If No password installed
    DEBUG ((EFI_D_ERROR, "  No Password Installed\n"));
  } else {                                                            // If some password installed
    if ((p & OT_ADMIN_PASSWORD) != 0) {                               //   If Admin password installed
      DEBUG ((EFI_D_ERROR, "  Administrative Password Installed\n"));
    }
    if ((p & OT_USER_PASSWORD) != 0) {                                //   If User password installed
      DEBUG ((EFI_D_ERROR, "  User Password Installed\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_CheckIfPasswordInstalled()  }\n"));

  return p;                                                           // Return password installed information
}

//-------------------------------------------------------------------------------------------------------
/**
  Match the given UnicodeZ password with the BIOS Administrative Password.

  @param[in] *Scd               SYSTEM_CONFIGURATION*: Ptr to SYSTEM_CONFIGURATION
  @param[in] *Password CHAR16*: Ptr to given UnicodeZ password

  @retval     BOOLEAN  TRUE/FALSE: Success/Error

**/
BOOLEAN
OT_MatchAdminPassword (
  IN  SYSTEM_CONFIGURATION  *Scd,
  IN  CHAR16                *Password
  )
{
  UINT8  ep[SHA256_DIGEST_LENGTH];     // Encoded password buffer
  UINTN  eps = SHA256_DIGEST_LENGTH;   // Encoded password size
  BOOLEAN b = FALSE;                   // Assume Error

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_MatchAdminPassword()  {\n"));
  DEBUG ((EFI_D_ERROR, "      Encode Supplied Password -> "));
  if ((EncodePassword(Password, &ep[0], &eps)) == EFI_SUCCESS) {     // If Encode given password success
    DEBUG ((EFI_D_ERROR, "Success\n"));
  DEBUG ((EFI_D_ERROR, "  Encoded Supplied Password: Size 0x%08x bytes\n", eps));
  DisplayBinaryData ((UINT8*)(UINTN) &ep[0], eps);                   // Display Encoded password
    if (eps == sizeof (Scd->AdminPassword)) {                        //   If encoded password size match
  DEBUG ((EFI_D_ERROR, "  Encoded BIOS Administrative Password: Size 0x%08x bytes\n", sizeof (Scd->AdminPassword)));
  DisplayBinaryData ((UINT8*)(UINTN) &Scd->AdminPassword, sizeof (Scd->AdminPassword)); // Display encoded BIOS Password
  if (CompareMem ((UINT8*)(UINTN) &ep[0],                                               // If Admin Password match
  (UINT8*)(UINTN) &Scd->AdminPassword, eps) == 0) {
  b = TRUE;                                                                             // Success, match
  }
    } else {
      DEBUG ((EFI_D_ERROR, "      Mismatch in size of Encoded Supplied Password and Encode BIOS Administrative Password\n"));
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Error\n"));
  }

  OT_MatchAdminPasswordOEM(Password, &b);
  DEBUG ((EFI_D_ERROR, "      Clear Encoded Password Buffer\n"));
  ZeroMem (&ep[0], eps);                                                                // Clear encoded password buffer
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_MatchAdminPassword()  } -> Password "));
  if (b) { DEBUG ((EFI_D_ERROR, "Match, Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "not match, Error\n")); }

  return b;                                                                             // Result
}

//-----------------------------	} Password Functions ---------------------------------------------------
//------------------------------------------------------------------------------------------------------
//----------------------------- Platform Information { -------------------------------------------------
/**
  OT_AllocatePlatformInformationArea(): Allocate platform information area.

  @param  None  INPUT: None
  @param  Opd   OUTPUT: Ptr to OT_PLATFORM_DATA, Success
                        NULL, Error
**/
OT_PLATFORM_DATA*
OT_AllocatePlatformInformationArea (
  )
{
  // Local data
  EFI_STATUS       Status;
  OT_PLATFORM_DATA *Opd;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_AllocatePlatformInformationArea()  { -> Allocate Memory for Platform Information\n"));

  // Init local data
  Opd = NULL; mSystemConfigurationSize = 0;

  // Allocate memory
  DEBUG ((EFI_D_ERROR, "      OT_PLATFORM_DATA: Allocate memory 0x%x bytes\n", (sizeof (OT_PLATFORM_DATA))));
  Opd = (OT_PLATFORM_DATA*)(UINTN) AllocateZeroPool (sizeof (OT_PLATFORM_DATA));// Allocate memory
  if (Opd != NULL) {                                                            // If Memory available
    DEBUG ((EFI_D_ERROR, "      SYSTEM_CONFIGURATION: Find required memory size\n"));
    Status = gRT->GetVariable (PLATFORM_SETUP_VARIABLE_NAME,			//   Find Setup data size
    &mSystemConfigurationGuid, NULL, &mSystemConfigurationSize, Opd);           // --> should return error EFI_BUFFER_TOO_SMALL
    if (Status == EFI_BUFFER_TOO_SMALL) {                                       //   If EFI_BUFFER_TOO_SMALL
      DEBUG ((EFI_D_ERROR, "      SYSTEM_CONFIGURATION: Allocate memory 0x%x bytes\n", mSystemConfigurationSize));
      Opd->SystemConfig = (SYSTEM_CONFIGURATION*)(UINTN)                        //     Allocate memory for System configuration, fill ptr to SYSTEM_CONFIGURATION in OT_PLATFORM_DATA
      AllocateZeroPool (mSystemConfigurationSize);
      if (Opd->SystemConfig != NULL) {                                          //     If Success
        DEBUG ((EFI_D_ERROR, "      OT_PLATFORM_DATA allocated at memory address 0x%x\n", (UINTN) Opd));
        DEBUG ((EFI_D_ERROR, "      SYSTEM_CONFIGURATION allocated at memory address 0x%x\n", (UINTN) Opd->SystemConfig));
        DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_AllocatePlatformInformationArea()  } -> Success\n"));
        return Opd;                                                                    //       Ptr to OT_PLATFORM_DATA
      }
    }
  }

  // Some error
  if (Opd != NULL) { gBS->FreePool ((VOID*)(UINTN) Opd); }                      // Deallocate memory
  DEBUG ((EFI_D_ERROR, "      Platform information data area not allocated\n"));
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_AllocatePlatformInformationArea()  } -> Error\n"));
  return NULL;                                                                  // Error
}

//------------------------------------------------------------------------------------------------------
/**
  OT_ClearAndDeallocatePlatformInformationArea(): Clear and Deallocate platform information area.

  @param  Opd   INPUT: Ptr to OT_PLATFORM_DATA
  @param  None  OUTPUT

  Note: Before deallocating, clear platform information area to prevent the data from being hacked.

**/
VOID
OT_ClearAndDeallocatePlatformInformationArea (
  IN  OT_PLATFORM_DATA *Opd
  )
{
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_ClearAndDeallocatePlatformInformationArea()  {\n"));
  if (Opd != NULL) {
    if (Opd->SystemConfig != NULL) {
      DEBUG ((EFI_D_ERROR, "         Clear System Configuration Area\n"));
      ZeroMem ((UINT8*)(UINTN) Opd->SystemConfig, sizeof (SYSTEM_CONFIGURATION));
      DEBUG ((EFI_D_ERROR, "         Deallocate System Configuration Area\n"));
      gBS->FreePool ((VOID*)(UINTN) Opd->SystemConfig);                         // Deallocate memory
    }
    DEBUG ((EFI_D_ERROR, "         Clear OT_PLATFORM_DATA Area\n"));
    ZeroMem ((UINT8*)(UINTN) Opd, sizeof (OT_PLATFORM_DATA));
    DEBUG ((EFI_D_ERROR, "         Deallocate OT_PLATFORM_DATA area\n"));
    gBS->FreePool ((VOID*)(UINTN) Opd);                                         // Deallocate memory
  }
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_ClearAndDeallocatePlatformInformationArea()  }\n"));
}

//------------------------------------------------------------------------------------------------------
/**
  OT_ReadPlatformInformation(): Read platform information.

  @param  Opd    INPUT/OUTPUT: Ptr to OT_PLATFORM_DATA where platform information to be read
  @param  Result OUTPUT: BOOLEAN, TRUE/FALSE: Success/Error

**/
BOOLEAN
OT_ReadPlatformInformation (
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  EFI_STATUS       Status;
  UINTN            Size;
  UINT16           pa, fs, fe, tu, mtk, ps;
  UINT8            mtkb, tkb, prc, plrr, ppec, plpr;
  BOOLEAN          b;

  // Init local data
  b = TRUE;                                                          // Assume Success
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_ReadPlatformInformation()  { -> Read Platform Information\n"));

  // Read Socket Processor Core Configuration data
  Size = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);                // Size of SOCKET_PROCESSORCORE_CONFIGURATION
  DEBUG ((EFI_D_ERROR, "      Read BIOS Setup Options: %s, 0x%x bytes --> ", SOCKET_PROCESSORCORE_CONFIGURATION_NAME, Size));
  Status = gRT->GetVariable (SOCKET_PROCESSORCORE_CONFIGURATION_NAME, &gEfiSocketProcessorCoreVarGuid,
                (UINT32*)(UINTN) &Opd->Spcca, &Size, (UINT32*)(UINTN) &Opd->Spcc);
  if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }              //   Error

  Size = sizeof (SOCKET_IIO_CONFIGURATION);                          // Size of SOCKET_IIO_CONFIGURATION
  DEBUG ((EFI_D_ERROR, "      Read BIOS Setup Options: %s, 0x%x bytes --> ", SOCKET_IIO_CONFIGURATION_NAME, Size));
  Status = gRT->GetVariable (SOCKET_IIO_CONFIGURATION_NAME, &gEfiSocketIioVariableGuid,
                (UINT32*)(UINTN) &Opd->Sica, &Size, (UINT32*)(UINTN) &Opd->Sic);
  if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }              //   Error

  Size = sizeof(TCG2_CONFIGURATION);
  DEBUG ((EFI_D_ERROR, "      Read BIOS Setup Options: %s, 0x%x bytes --> ", TCG2_STORAGE_NAME, Size));
  Status = gRT->GetVariable (TCG2_STORAGE_NAME, &gTcg2ConfigFormSetGuid,
                  (UINT32*)(UINTN) &Opd->T2ca, &Size, (UINT32*)(UINTN) &Opd->T2c);
  if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }              //   Error

  Size = sizeof (ME_RC_CONFIGURATION);
  DEBUG ((EFI_D_ERROR, "      Read BIOS Setup Options: %s, 0x%x bytes --> ", ME_RC_CONFIGURATION_NAME, Size));
  Status = gRT->GetVariable (ME_RC_CONFIGURATION_NAME, &gEfiMeRcVariableGuid,
                  (UINT32*)(UINTN) &Opd->Merca, &Size, (UINT32*)(UINTN) &Opd->Merc);
  if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }              //   Error

  DEBUG ((EFI_D_ERROR, "      Read BIOS Setup Options: %s, 0x%x bytes --> ", PLATFORM_SETUP_VARIABLE_NAME, mSystemConfigurationSize));
  Status = gRT->GetVariable (PLATFORM_SETUP_VARIABLE_NAME, &mSystemConfigurationGuid,
                  (UINT32*)(UINTN) &Opd->Sca, &mSystemConfigurationSize, (VOID*)(UINTN) Opd->SystemConfig);
  if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }              //   Error

  //--------------------------------------------------------------
  // The following code does NOT work when TPM is physically removed after BIOS is booted successfully
  // with TPM (because this detection variable is not initialized in the error path and is left with
  // the value found in the previous boot
  //Size = sizeof(TCG2_DEVICE_DETECTION);
  //DEBUG ((EFI_D_ERROR, "      Read BIOS Setup Options: %s, 0x%x bytes --> ", TCG2_DEVICE_DETECTION_NAME, Size));
  //Status = gRT->GetVariable (TCG2_DEVICE_DETECTION_NAME, &gTcg2ConfigFormSetGuid,
  //                (UINT32*)(UINTN) &Opd->T2dda, &Size, (UINT32*)(UINTN) &Opd->T2dd);
  //if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  //else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }            //   Error

  DEBUG ((EFI_D_ERROR, "         TPM Device Present: "));
  Opd->T2dd.TpmDeviceDetected = TPM_DEVICE_NULL;                     // No TPM present
  Opd->Tcg2Protocol = NULL; Opd->TcgProtocol = NULL;                 // TCG2 protocol and TCG protocol not found
  if (((gBS->LocateProtocol (&gEfiTcg2ProtocolGuid, NULL, (VOID**) &Opd->Tcg2Protocol)) == EFI_SUCCESS) &&
      (Opd->Tcg2Protocol != NULL)) {                                 // If TCG2 protocol found
  Opd->T2dd.TpmDeviceDetected = TPM_DEVICE_2_0_DTPM;                 // TPM2.0 present
  } else {                                                           // If TCG2 protocol not found
    Opd->Tcg2Protocol = NULL;                                        //   TCG2 protocol not found
    if (((gBS->LocateProtocol (&gEfiTcgProtocolGuid, NULL, (VOID**) &Opd->TcgProtocol)) == EFI_SUCCESS) &&
      (Opd->TcgProtocol != NULL)) {                                  //   If TCG protocol found
    Opd->T2dd.TpmDeviceDetected = TPM_DEVICE_1_2;                    //   TPM1.2 present
    } else {                                                         //   If TCG protocol not found
      Opd->TcgProtocol = NULL;                                       //     TCG protocol not found
    }
  }
  switch (Opd->T2dd.TpmDeviceDetected) {                             // Display Detected TPM device
    case TPM_DEVICE_2_0_DTPM:                                        // TPM 2.0
      DEBUG ((EFI_D_ERROR, "TPM 2.0\n"));
      break;

    case TPM_DEVICE_1_2:                                             // TPM 1.2
      DEBUG ((EFI_D_ERROR, "TPM 1.2\n"));
      break;

    //case TPM_DEVICE_NULL:                                          // No TPM
    default:                                                         // Invalid
      DEBUG ((EFI_D_ERROR, "None or Invalid TPM Device\n"));
      break;
  }

  //--------------------------------------------------------------
  // Init different fields in OT_PLATFORM_DATA
  Opd->Tpm12PreparedForTask = FALSE;                                 // TPM preparation not performed before task
  Opd->Tpm12ActivateStateChange = FALSE;                             // TPM activate state not changed
  Opd->Scm = FALSE; Opd->Spccm = FALSE; Opd->Sicm = FALSE;           // Data not modified
  Opd->T2cm = FALSE; Opd->Mercm = FALSE;

  Opd->TaskHasOutputData = FALSE;                                    // Task has no output data beyond OXP_HEADER
  Opd->TpmCommand = (UINT32) 0xFFFFFFFF;                             // Last TPM Command issued
  Opd->TpmRc = (UINT32) 0xFFFFFFFF;                                  // TPM Response code of last TPM Command

  pa = 0; fs = 0; fe = 0; tu = 0; mtk = 0; mtkb = 0, tkb = 0;        // PasswordAttribute, FeatureSupported, FeatureEnabled, TpmUsage, MK-TME Key info
  ps = 0; prc = 0; plrr = 0; ppec = 0; plpr = 0;                     // PFR information
  if (b) {                                                           // If read platform info success
    OT_DisplayBiosSetupOption (Opd, FALSE);                          //   Display current Setup Option
    OT_GetTxtInfo (Opd, &fs, &fe);                                   //   Fill TXT support and enabled info
    OT_GetTpmInfo (Opd, &fs, &fe, &tu);                              //   Fill TPM support and enabled info
    OT_GetMkTmeInfo (Opd, &fs, &fe, &mtk, &mtkb, &tkb);              //   Fill TME/MK-TME support and enabled info
    OT_GetSgxInfo (Opd, &fs, &fe);                                   //   Fill SGX support and enabled info
    OT_GetPfrInfo (Opd, &fs, &fe, &ps, &prc, &plrr, &ppec, &plpr);   //   Fill PFR support and enabled info
    pa = (OXP_PASSWORD_ASCII_BIT | OXP_PASSWORD_UNICODE_BIT);        //   Default PasswordAttribute
    if (((OT_CheckIfPasswordInstalled (Opd->SystemConfig))           // If Administrative Password installed
      & OT_ADMIN_PASSWORD) != 0) {
      pa |= OXP_PASSWORD_ADMIN_INSTALLED_BIT;                        //   Update Password Attribute
    }
  }
  Opd->PasswordAttribute = pa;                                       // Password Attribute
  Opd->FeatureSupported = fs; Opd->FeatureEnabled = fe;              // Features supported, Features enabled
  Opd->TpmUsage = tu;                                                // TPM Usage
  Opd->MaxMkTmeKeys = mtk;                                           // Max #of TME keys availaible for use
  Opd->MaxMkTmeKeyIdBits = mtkb;                                     // Max #of bits allowed for use as Key Identifiers for MK-TME
  Opd->MkTmeKeyIdBits = tkb;                                         // Max #of bits allocated for use as Key Identifiers for MK-TME
  Opd->PfrState = ps;                                                // PFR State
  Opd->PfrRecoveryCount = prc; Opd->PfrLastRecoveryReason = plrr;    // PFR: Recovery Count, Last Recovery Reason
  Opd->PfrPanicEventCount = ppec; Opd->PfrLastPanicReason = plpr;    // PFR: Panic Event Count, Last Panic Reason

  //--------------------------------------------------------------

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_ReadPlatformInformation()  } -> "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else {DEBUG ((EFI_D_ERROR, "Error\n")); }

  return b;                                                          // Result
}

//-----------------------------------------------------------------------------------------------------
/**
  OT_WritePlatformInformation(): Write platform information, if necessary.

  @param  Opd    INPUT: Ptr to OT_PLATFORM_DATA having platform information
  @param  Result OUTPUT: BOOLEAN, TRUE/FALSE: Success/Error

**/
BOOLEAN
OT_WritePlatformInformation (
  IN OUT OT_PLATFORM_DATA  *Opd
  )
{
  // Local data
  EFI_STATUS  Status;
  BOOLEAN     b;

  // Init local data
  b = TRUE;                                                          // Assume Success
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_WritePlatformInformation()  { -> Write Platform Information\n"));
  OT_DisplayBiosSetupOption (Opd, TRUE);                             // Display modified Setup Option

  // Write Setup Option, if necessary
  DEBUG ((EFI_D_ERROR, "      Write BIOS Setup Options: %s --> ", SOCKET_PROCESSORCORE_CONFIGURATION_NAME));
  if (Opd->Spccm) {                                                  // If modified
    Status = gRT->SetVariable (SOCKET_PROCESSORCORE_CONFIGURATION_NAME, &gEfiSocketProcessorCoreVarGuid,
                  Opd->Spcca, sizeof (SOCKET_PROCESSORCORE_CONFIGURATION), (UINT32*)(UINTN) &Opd->Spcc);
    if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
    else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }            //   Error
  } else {                                                           // If not changed
    DEBUG ((EFI_D_ERROR, "Not modified, No need to write\n"));
  }

  DEBUG ((EFI_D_ERROR, "      Write BIOS Setup Options: %s --> ", SOCKET_IIO_CONFIGURATION_NAME));
  if (Opd->Sicm) {                                                   // If modified
    Status = gRT->SetVariable (SOCKET_IIO_CONFIGURATION_NAME, &gEfiSocketIioVariableGuid,
                  Opd->Sica, sizeof (SOCKET_IIO_CONFIGURATION), (UINT32*)(UINTN) &Opd->Sic);
    if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
    else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }            //   Error
  } else {                                                           // If not changed
    DEBUG ((EFI_D_ERROR, "Not modified, No need to write\n"));
  }

  DEBUG ((EFI_D_ERROR, "      Write BIOS Setup Options: %s --> ", TCG2_STORAGE_NAME));
  if (Opd->T2cm) {                                                   // If modified
    Status = gRT->SetVariable (TCG2_STORAGE_NAME, &gTcg2ConfigFormSetGuid,
                  Opd->T2ca, sizeof (TCG2_CONFIGURATION), (UINT32*)(UINTN) &Opd->T2c);
    if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
    else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }            //   Error
  } else {                                                           // If not changed
    DEBUG ((EFI_D_ERROR, "Not modified, No need to write\n"));
  }

  DEBUG ((EFI_D_ERROR, "      Write BIOS Setup Options: %s --> ", ME_RC_CONFIGURATION_NAME));
  if (Opd->Mercm) {                                                  // If modified
    Status = gRT->SetVariable (ME_RC_CONFIGURATION_NAME, &gEfiMeRcVariableGuid,
                  Opd->Merca, sizeof(ME_RC_CONFIGURATION), (UINT32*)(UINTN) &Opd->Merc);
    if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
    else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }            //   Error
  } else {                                                           // If not changed
    DEBUG ((EFI_D_ERROR, "Not modified, No need to write\n"));
  }

  DEBUG ((EFI_D_ERROR, "      Write BIOS Setup Options: %s --> ", PLATFORM_SETUP_VARIABLE_NAME));
  if (Opd->Scm) {                                                    // If modified
    Status = gRT->SetVariable (PLATFORM_SETUP_VARIABLE_NAME, &mSystemConfigurationGuid,
                  Opd->Sca, mSystemConfigurationSize, (VOID*)(UINTN) Opd->SystemConfig);
    if (Status == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
    else { b = FALSE; DEBUG ((EFI_D_ERROR, "Error\n")); }            //   Error
  } else {                                                           // If not changed
    DEBUG ((EFI_D_ERROR, "Not modified, No need to write\n"));
  }

  OT_WritePlatformInformationOEMHook(&b);
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_WritePlatformInformation()  } -> "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else {DEBUG ((EFI_D_ERROR, "Error\n")); }

  return b;                                                          // Result
}

//------------------------------------------------------------------------------------------------------
// **************************** dTPM functions *********************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_DTpmClear(): Clear dTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_DTPM_CLEAR_ERROR
**/
UINT8
OT_DTpmClear (
  IN     TPM2B_AUTH        *Auth,
  IN OUT OT_PLATFORM_DATA  *Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DTpmClear() { -> Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));
  e = OXP_DTPM_CLEAR_ERROR;                                          // Assume Error
  switch (Opd->T2dd.TpmDeviceDetected) {                             // Detected TPM device
    case TPM_DEVICE_2_0_DTPM:                                        // TPM 2.0
      DEBUG ((EFI_D_ERROR, "      TPM2.0: Clear --> "));
      if (Opd->Tcg2Protocol != NULL) {                               // If TCG2 protocol found
        DEBUG ((EFI_D_ERROR, "Tpm2CommandClear(): "));
        if (Tpm2CommandClear (Auth) == EFI_SUCCESS) {                //   If success
          e = OXP_SUCCESS;                                           //     Indicate success
          DEBUG ((EFI_D_ERROR, "Success\n"));
        } else {                                                     //   If not sucess
        DEBUG ((EFI_D_ERROR, "Error\n"));
        }
      } else {                                                       //   If TCG2 protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    case TPM_DEVICE_1_2:                                             // TPM 1.2
      DEBUG ((EFI_D_ERROR, "      TPM1.2: "));
      if (Opd->TcgProtocol != NULL) {                                // If TCG protocol found
        DEBUG ((EFI_D_ERROR, "Clear, Enable, Activate -->\n"));
        e = OT_Tpm12ClearEnableActivate (Opd);			// Clear
      } else {                                                       // If TCG protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    //case TPM_DEVICE_NULL:                                          // No TPM
    default:                                                         // Invalid
      DEBUG ((EFI_D_ERROR, "      None/Invalid TPM Device: Implementation Error -> BUG\n"));
      break;
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DTpmClear() } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else {DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//------------------------------------------------------------------------------------------------------
/**
  OT_DTpmEnable(): Enable dTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_DTPM_ENABLE_ERROR, OXP_DTPM_ACTIVATE_ERROR
**/
UINT8
OT_DTpmEnable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DTpmEnable()  { -> Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));
  e = OXP_DTPM_ENABLE_ERROR;                                         // Assume Error
  switch (Opd->T2dd.TpmDeviceDetected) {                             // Detected TPM device
    case TPM_DEVICE_2_0_DTPM:                                        // TPM 2.0
      DEBUG ((EFI_D_ERROR, "      TPM2.0: Enable --> "));
      if (Opd->Tcg2Protocol != NULL) {                               // If TCG2 protocol found
        e = OXP_SUCCESS;                                             //   Indicate success
        DEBUG ((EFI_D_ERROR, "Success\n"));
      } else {                                                       // If TCG2 protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    case TPM_DEVICE_1_2:                                             // TPM 1.2
      DEBUG ((EFI_D_ERROR, "      TPM1.2: "));
      if (Opd->TcgProtocol != NULL) {                                // If TCG protocol found
        DEBUG ((EFI_D_ERROR, "Enable, Activate -->\n"));
        e = OT_Tpm12EnableActivate (Opd, FALSE);                     // Enable, Activate (if necessary)
      } else {                                                       // If TCG protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    //case TPM_DEVICE_NULL:                                          // No TPM
    default:                                                         // Invalid
      DEBUG ((EFI_D_ERROR, "      None/Invalid TPM Device: Error\n"));
      break;
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DTpmEnable()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) {                                            // If success
    if (Opd->T2c.TpmDevice != Opd->T2dd.TpmDeviceDetected) {         //   If configured TPM <> detected TPM
      Opd->T2c.TpmDevice = Opd->T2dd.TpmDeviceDetected; Opd->T2cm = TRUE;// Set configured TPM = detected TPM, Data modified
    }
    DEBUG ((EFI_D_ERROR, "Success\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "Error\n"));
  }

  return e;                                                          // Status
}

//------------------------------------------------------------------------------------------------------
/**
  OT_DTpmDisable(): Disable dTPM.

  @param  Auth    INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd     INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result  OUTPUT: UINT8, Status Code
                          OXP_SUCCESS, OXP_DTPM_DISABLE_ERROR
**/
UINT8
OT_DTpmDisable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DTpmDisable()  { -> Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));
  e = OXP_DTPM_DISABLE_ERROR;                                        // Assume Error
  switch (Opd->T2dd.TpmDeviceDetected) {                             // Detected TPM device
    case TPM_DEVICE_2_0_DTPM:                                        // TPM 2.0
      DEBUG ((EFI_D_ERROR, "      TPM2.0: Disable --> "));
      if (Opd->T2c.TpmDevice != TPM_DEVICE_NULL) {
        Opd->T2c.TpmDevice = TPM_DEVICE_NULL; Opd->T2cm = TRUE;      // Disable detected TPM, Data modified
      }
      if (Opd->Tcg2Protocol != NULL) {                               // If TCG2 protocol found
        e = OXP_SUCCESS;                                             //   Success
        DEBUG ((EFI_D_ERROR, "Success\n"));
      } else {                                                       // If TCG2 protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    case TPM_DEVICE_1_2:                                             // TPM 1.2
      DEBUG ((EFI_D_ERROR, "      TPM1.2: "));
      if (Opd->TcgProtocol != NULL) {                                // If TCG protocol found
        DEBUG ((EFI_D_ERROR, "Deactivate, Disable -->\n"));
        e = OT_Tpm12DeactivateDisable (Opd);                         // Deactivate, Disable
      } else {                                                       // If TCG protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    //case TPM_DEVICE_NULL:                                          // No TPM
    default:                                                         // Invalid
      DEBUG ((EFI_D_ERROR, "      None/Invalid TPM Device: Error\n"));
      break;
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DTpmDisable()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// **************************** fTPM functions *********************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmClear(): Clear fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_DTPM_CLEAR_ERROR
**/
UINT8
OT_FTpmClear (
  IN     TPM2B_AUTH		*Auth,
  IN OUT OT_PLATFORM_DATA	*Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmClear(): Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));
  e = OXP_FTPM_CLEAR_ERROR;                                          // Assume Error
  switch (Opd->T2dd.TpmDeviceDetected) {                             // Detected TPM device
    case TPM_DEVICE_2_0_DTPM:                                        // TPM 2.0
      DEBUG ((EFI_D_ERROR, "      Clear fTPM --> TPM2.0: "));
      if (Opd->Tcg2Protocol != NULL) {                               // If TCG2 protocol found
        DEBUG ((EFI_D_ERROR, "Tpm2CommandClear() --> "));
        if (Tpm2CommandClear (Auth) == EFI_SUCCESS) {                // If success
          e = OXP_SUCCESS;                                           //   Indicate success
          DEBUG ((EFI_D_ERROR, "Success\n"));
        } else {                                                     // If not sucess
          DEBUG ((EFI_D_ERROR, "Error\n"));
      }
      } else {                                                       // If TCG2 protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    //case TPM_DEVICE_1_2:                                           // TPM 1.2
    //case TPM_DEVICE_NULL:                                          // No TPM
    default:                                                         // Invalid
      DEBUG ((EFI_D_ERROR, "      TPM1.2/None/Invalid TPM Device: Implementation Error -> BUG\n"));
      break;
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmClear()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else {DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmEnable(): Enable fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_FTPM_ENABLE_ERROR
**/
UINT8
OT_FTpmEnable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmEnable()  { -> Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));

  e = OXP_SUCCESS;                                                   // Assume success
  if ((Opd->Merc.MePttEnabled == 0) ||                               // If PTT disabled
      ((Opd->FeatureEnabled & OXP_FTPM_ENABLED_BIT) == 0)) {
    DEBUG ((EFI_D_ERROR, "      PttHeciUpdateState(TRUE): Enable PTT in ME --> "));
    if ((PttHeciUpdateState (TRUE)) == EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "Success\n"));
      DEBUG ((EFI_D_ERROR, "      ME Configuration Setup: PTT --> Enable\n"));
      Opd->Merc.MePttEnabled = 1; Opd->Mercm = TRUE;                 //   Enable PTT, Data modified
    } else {
      e = OXP_FTPM_ENABLE_ERROR;                                     // Error
      DEBUG ((EFI_D_ERROR, "Error\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmEnable()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else {DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmDisable(): Disable fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_FTPM_DISABLE_ERROR
**/
UINT8
OT_FTpmDisable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmDisable(): Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));

  e = OXP_SUCCESS;                                                   // Assume success
  if ((Opd->Merc.MePttEnabled != 0) ||                               // If PTT enabled
      ((Opd->FeatureEnabled & OXP_FTPM_ENABLED_BIT) != 0)) {
    DEBUG ((EFI_D_ERROR, "      PttHeciUpdateState(FALSE): Disable PTT in ME --> "));
    if ((PttHeciUpdateState (FALSE)) == EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "Success\n"));
      DEBUG ((EFI_D_ERROR, "      ME Configuration Setup: PTT --> Disable\n"));
      Opd->Merc.MePttEnabled = 0; Opd->Mercm = TRUE;                 //   Disable PTT, Data modified
    } else {
      e = OXP_FTPM_DISABLE_ERROR;                                    // Error
      DEBUG ((EFI_D_ERROR, "Error\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmDisable()  } -> Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else {DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmSetPcrBanks(): Set PCR Bansk for fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Ci     INPUT:  BOOLEAN*, Ptr to Boolean Variable
                         OUTPUT: TRUE  -> Set PCR Banks command issued
                         OUTPUT: FALSE -> Set PCR Banks command NOT issued
  @param  Result OUTPUT: UINT8, Status Code

**/
UINT8
OT_FTpmSetPcrBanks (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd,
  IN OUT BOOLEAN          *Ci
  )
{
  // Local data
  UINT8   e;
  BOOLEAN c;

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmSetPcrBanks(): Detected TPM Device 0x%02x\n", Opd->T2dd.TpmDeviceDetected));

  e = OXP_FTPM_SET_PCR_ERROR; c = FALSE;                             // Assume: Error, Set PCR Banks Command not issued
  switch (Opd->T2dd.TpmDeviceDetected) {                             // Detected TPM device
    case TPM_DEVICE_2_0_DTPM:                                        // TPM 2.0
      DEBUG ((EFI_D_ERROR, "      fTPM, TPM2.0: "));
      if (Opd->Tcg2Protocol != NULL) {                               // If TCG2 protocol found
        if (OT_SetPcrBanksTpm20 (Auth, Opd,                          // If success
          (EFI_TCG2_BOOT_HASH_ALG_SHA1 | EFI_TCG2_BOOT_HASH_ALG_SHA256), &c) == EFI_SUCCESS) {
          e = OXP_SUCCESS;                                           //   Indicate success
        }
      } else {                                                       // If TCG2 protocol not found
        DEBUG ((EFI_D_ERROR, "Implementation Error -> BUG\n"));
      }
      break;

    //case TPM_DEVICE_1_2:                                           // TPM 1.2
    //case TPM_DEVICE_NULL:                                          // No TPM
    default:                                                         // Invalid
      DEBUG ((EFI_D_ERROR, "      TPM1.2/None/Invalid TPM Device: Implementation Error -> BUG\n"));
      break;
  }

  *Ci = c;                                                           // Return whether command issued or not

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_FTpmSetPcrBanks()  } -> Command Issued: "));
  if (c) { DEBUG ((EFI_D_ERROR, "Yes")); }
  else { DEBUG ((EFI_D_ERROR, "No")); }
  DEBUG ((EFI_D_ERROR, ", Status: 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else {DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//------------------------------------------------------------------------------------------------------
// ****************************	TXT functions **********************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_TxtEnable(): Enable TXT.

  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_TXT_ENABLE_ERROR
**/
UINT8
OT_TxtEnable (
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_EnableTxt()  { -> Enable all TXT related BIOS Setup Options\n"));
  if (Opd->Spcc.ProcessorVmxEnable == 0) { Opd->Spcc.ProcessorVmxEnable = 1; Opd->Spccm = TRUE; }       // Enable VMX, Data modified
  if (Opd->Spcc.ProcessorSmxEnable == 0) { Opd->Spcc.ProcessorSmxEnable = 1; Opd->Spcc.ProcessorMsrLockControl = 1; Opd->Spccm = TRUE; }// Enable SMX, Data modified
  if (Opd->Sic.VTdSupport == 0) { Opd->Sic.VTdSupport = 1; Opd->Sicm = TRUE; }                          // Enable VT-d, Data modified
  if (Opd->Spcc.ProcessorLtsxEnable == 0) { Opd->Spcc.ProcessorLtsxEnable = 1; Opd->Spcc.LockChipset = 1; Opd->Spccm = TRUE; }   // Enable LTSX, Data modified

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_EnableTxt()  } -> Status 0x00 -> Success\n"));

  return OXP_SUCCESS;                                                                 // Success
}

//------------------------------------------------------------------------------------------------------
/**
  OT_TxtDisable(): Disable TXT.

  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_TXT_ENABLE_ERROR
**/
UINT8
OT_TxtDisable (
  IN OUT OT_PLATFORM_DATA *Opd
  )
{
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DisableTxt()  { -> Disable all TXT related BIOS Setup Options\n"));
  if (Opd->Spcc.ProcessorVmxEnable != 0) { Opd->Spcc.ProcessorVmxEnable = 0; Opd->Spccm = TRUE; }   // Disable VMX, Data modified
  if (Opd->Spcc.ProcessorSmxEnable != 0) { Opd->Spcc.ProcessorSmxEnable = 0; Opd->Spccm = TRUE; }   // Disable SMX, Data modified
  if (Opd->Sic.VTdSupport != 0) { Opd->Sic.VTdSupport = 0; Opd->Sicm = TRUE; }                      // Disable VT-d, Data modified
  if (Opd->Spcc.ProcessorLtsxEnable != 0) { Opd->Spcc.ProcessorLtsxEnable = 0; Opd->Spccm = TRUE; } // Disable LTSX, Data modified

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_DisableTxt()  } -> Status 0x00 -> Success\n"));

  return OXP_SUCCESS;                                                                               // Success
}

//------------------------------------------------------------------------------------------------------
// **************************** TME/MK-TME functions ***************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_HandleMkTme(): Handle TME/MK-TME.

  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Tme    INPUT:  BOOLEAN*, Ptr to area indicating TME Operation
                         If NULL, Do not touch TME settings
                         If Non-Null, TRUE/FALSE: Enable/Disable TME
  @param  MkTme  INPUT:  BOOLEAN*, Ptr to area indicating MK-TME Operation
                         If NULL, Do not touch MK-TME settings
                         If Non-Null, TRUE/FALSE: Enable/Disable MK-TME
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS Success
                         Other  Error
**/
UINT8
OT_HandleMkTme (
  IN OUT OT_PLATFORM_DATA *Opd,
  IN     BOOLEAN          *Tme,
  IN     BOOLEAN          *MkTme
  )
{
  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_HandleMkTme() { Handle TME/MK-TME BIOS Setup Options\n"));
  if (Tme != NULL) {                                                                    // If TME Settings requested
    if (*Tme) {                                                                         //   If TME to be enabled
      DEBUG ((EFI_D_ERROR, "    Enable TME\n"));
      if (Opd->Spcc.EnableTme == 0) { Opd->Spcc.EnableTme = 1; Opd->Spccm = TRUE; }     //     Enable TME, Data modified
    } else {                                                                            //   If TME to be disabled
      DEBUG ((EFI_D_ERROR, "    Disable TME\n"));
      if (Opd->Spcc.EnableTme != 0) { Opd->Spcc.EnableTme = 0; Opd->Spccm = TRUE; }     //     Disable TME, Data modified
    }
  }
  if (MkTme != NULL) {                                                                  // If MK-TME Settings requested
    if (*MkTme) {                                                                       //   If MK-TME to be enabled
      DEBUG ((EFI_D_ERROR, "    Enable MK-TME\n"));
      if (Opd->Spcc.EnableMktme == 0) { Opd->Spcc.EnableMktme = 1; Opd->Spccm = TRUE; } //     Enable MK-TME, Data modified
    } else {                                                                            //   If MK-TME to be disabled
      DEBUG ((EFI_D_ERROR, "    Disable MK-TME\n"));
      if (Opd->Spcc.EnableMktme != 0) { Opd->Spcc.EnableMktme = 0; Opd->Spccm = TRUE; } //     Disable MK-TME, Data modified
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_HandleMkTme() } Status 0x00 -> Success\n"));

  return OXP_SUCCESS;                                                // Success
}

//------------------------------------------------------------------------------------------------------
// **************************** PFR functions **********************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_HandlePfr(): Handle PFR.

  @param  Opd    INPUT : OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
          Task   INPUT : UINT16, PFR Task (Bit-mapped task information defined in OXP_PFR_INFO.Attribute)

  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS Success
                         Other  Error
**/
UINT8
OT_HandlePfr (
  IN OUT OT_PLATFORM_DATA  *Opd,
  IN     UINT16            Task
  )
{
  // Local data
  UINT8    e = OXP_SUCCESS;                                      // Assume success

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_HandlePfr() { Handle PFR BIOS Setup Options\n"));
  switch (Task) {                                                // Handle PFR task
    case OXP_PI_PROVISION_BIT:                                   // Provision
      DEBUG ((EFI_D_ERROR, "    PFR Task -> Provision\n"));
      if (Opd->SystemConfig->PfrProvision == 0) {                // If not enabled
        Opd->SystemConfig->PfrProvision = 1; Opd->Scm = TRUE;    //   Enable, Setup data modified
      }
      break;

    case OXP_PI_UNPROVISION_BIT:                                 // UnProvision
      DEBUG ((EFI_D_ERROR, "    PFR Task -> UnProvision\n"));
      if (Opd->SystemConfig->PfrUnProvision == 0) {              // If not enabled
        Opd->SystemConfig->PfrUnProvision = 1; Opd->Scm = TRUE;  //   Enable, Setup data modified
      }
      break;

    case OXP_PI_LOCK_BIT:                                        // Lock
      DEBUG ((EFI_D_ERROR, "    PFR Task -> Lock\n"));
      if (Opd->SystemConfig->PfrLock == 0) {                     // If not enabled
        Opd->SystemConfig->PfrLock = 1; Opd->Scm = TRUE;         //   Enable, Setup data modified
      }
      break;

    default:                                                     // Invalid operation
      DEBUG ((EFI_D_ERROR, "    PFR Task -> Invalid\n"));
      e = OXP_INVALID_TASK;                                      // Error
      break;
  }

  DEBUG ((EFI_D_ERROR, "[OOB-Platform] OT_HandlePfr() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                      // Status
}

//----------------------------- } Platform Information -------------------------------------------------
