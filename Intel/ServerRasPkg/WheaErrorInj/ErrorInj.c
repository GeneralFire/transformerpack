/** @file
  This is an implementation of the Core Whea error injection method.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/RasDebugLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Protocol/RasPolicy.h>
#include <Library/PlatPolicyLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/WheaSupportLib.h>
#include <Library/WorkaroundLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/CrystalRidgeLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/ImcRoutingLib.h>

#include <Guid/MemoryMapData.h>
#include <RcRegs.h>
#include <SiliconSetting.h>

#include "WheaErrorInj.h"

#include <Cpu/Msr/CommonMsr.h>

BOOLEAN                                       mAcpi5Support = FALSE;
BOOLEAN                                       mPcieErrInjActionTable = FALSE;
BOOLEAN                                       mMESegInjEn = FALSE;
BOOLEAN                                       mMESegIcvErrEn = FALSE;
UINT8                                         mErrorExpected = WHEA_NO_CONSUME; // Memory Error consume
UINT8                                         *MemoryLlcErrInjAddr;
UINT64                                        mBiosRsvdInjAddr;
UINT64                                        mUmaErrorInjectionType;
WHEA_EINJ_PARAM_BUFFER                        *mEinjParam = NULL;
VOID                                          *mEinjEsts = NULL;
WHEA_EINJ_TRIGGER_ACTION_TABLE                *mEinjAction = NULL;
UINT64                                        EinjIioErrInjLockSwSmi;
MEM_TOPOLOGY                                  *mMemTopologyData;
RAS_POLICY                                    *mPolicyData;
extern   UINT32                               mMESegInjAddr;
BOOLEAN                                       Is10nmFam;

//
// WHEA Serialization Action Table
//
EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY mSimEinj[9] = {
  { // Entry 0
    EFI_ACPI_6_2_EINJ_BEGIN_INJECTION_OPERATION,      // InjectionAction
    EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE,           // Instruction
    EFI_ACPI_6_2_EINJ_PRESERVE_REGISTER,              // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    EINJ_BEGIN_OPERATION,                             // Value
    0xffffffff                                        // Mask
  },
  { // Entry 1
    EFI_ACPI_6_2_EINJ_GET_TRIGGER_ERROR_ACTION_TABLE, // InjectionAction
    EFI_ACPI_6_2_EINJ_READ_REGISTER,                  // Instruction
    0,                                                // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffffffffffff                                // Mask
  },
  { // Entry 2
    EFI_ACPI_6_2_EINJ_SET_ERROR_TYPE,                 // InjectionAction
    EFI_ACPI_6_2_EINJ_WRITE_REGISTER,                 // Instruction
    EFI_ACPI_6_2_EINJ_PRESERVE_REGISTER,              // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffff                                        // Mask
  },
  { // Entry 3
    EFI_ACPI_6_2_EINJ_GET_ERROR_TYPE,                 // InjectionAction
    EFI_ACPI_6_2_EINJ_READ_REGISTER,                  // Instruction
    0,                                                // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffff                                        // Mask
  },
  { // Entry 4
    EFI_ACPI_6_2_EINJ_END_OPERATION,                  // InjectionAction
    EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE,           // Instruction
    EFI_ACPI_6_2_EINJ_PRESERVE_REGISTER,              // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    EINJ_END_OPERATION,                               // Value
    0xffffffff                                        // Mask
  },
  { // Entry 5
    EFI_ACPI_6_2_EINJ_EXECUTE_OPERATION,              // InjectionAction
    EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE,           // Instruction
    EFI_ACPI_6_2_EINJ_PRESERVE_REGISTER,              // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_EXECUTE,                            // RegisterRegion (GAS BYTE IO)
    RAS_SW_SMI_EINJ_EXECUTE,                          // Value
    0xff                                              // Mask
  },
  { // Entry 6
    EFI_ACPI_6_2_EINJ_CHECK_BUSY_STATUS,              // InjectionAction
    EFI_ACPI_6_2_EINJ_READ_REGISTER_VALUE,            // Instruction
    0,                                                // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0x00000001,                                       // Value
    0x00000001                                        // Mask
  },
  { // Entry 7
    EFI_ACPI_6_2_EINJ_GET_COMMAND_STATUS,             // InjectionAction
    EFI_ACPI_6_2_EINJ_READ_REGISTER,                  // Instruction
    EFI_ACPI_6_2_EINJ_PRESERVE_REGISTER,              // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0x000001fe                                        // Mask
  },
  { // Entry 8
    WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRSS,             // InjectionAction
    EFI_ACPI_6_2_EINJ_WRITE_REGISTER,                 // Instruction
    EFI_ACPI_6_2_EINJ_PRESERVE_REGISTER,              // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffff                                        // Mask
  },
};

WHEA_EINJ_TRIGGER_ACTION_TABLE mSimEinjAction = {
  { // Header
    sizeof (EFI_ACPI_6_2_EINJ_TRIGGER_ACTION_TABLE),  // HeaderSize
    0,                                                // Revision
    sizeof (WHEA_EINJ_TRIGGER_ACTION_TABLE),          // TableSize
    4                                                 // EntryCount
  },
  { // Trigger 0
    WHEA_EINJ_TRIGGER_ERROR_ACTION,                   // InjectionAction
    EFI_ACPI_6_2_EINJ_NOOP,                           // Instruction (filled by runtime injection code)
    0,                                                // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffffffffffff                                // Mask
  },
  { // Trigger 1
    WHEA_EINJ_TRIGGER_ERROR_ACTION,                   // InjectionAction
    EFI_ACPI_6_2_EINJ_NOOP,                           // Instruction (filled by runtime injection code)
    0,                                                // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffff                                        // Mask
  },
  { // Trigger 2
    WHEA_EINJ_TRIGGER_ERROR_ACTION,                   // InjectionAction
    EFI_ACPI_6_2_EINJ_NOOP,                           // Instruction (filled by runtime injection code)
    0,                                                // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffff                                        // Mask
  },
  { // Trigger 3
    WHEA_EINJ_TRIGGER_ERROR_ACTION,                   // InjectionAction
    EFI_ACPI_6_2_EINJ_NOOP,                           // Instruction (filled by runtime injection code)
    0,                                                // Flags
    0x00,                                             // Reserved0
    EINJ_REGISTER_FILLER,                             // RegisterRegion (GAS QWORD Memory, address filled during boot)
    0,                                                // Value
    0xffffffff                                        // Mask
  }
};

/**

    By default error injection mechanism comes up disabled.
    This function will write to Internal MSR (ERR_INJ_LCK_UNLOCK_CTL_MSR) to unlock the mechanism.
    See Machine Check Architecture HAS for details.

    @param node - Node Id

    @retval None

**/
VOID
EFIAPI
UnLockInjLogic(
  UINT8 *node
  )
{
  MSR_ERR_INJ_LCK_UNLOCK_CTL_REGISTER ErrInjLckUnlockCtlReg;

  ErrInjLckUnlockCtlReg.Uint64 = AsmReadMsr64 (MSR_ERR_INJ_LCK_UNLOCK_CTL);
  ErrInjLckUnlockCtlReg.Bits.ErrInjLckUnlock = 0;

  AsmWriteMsr64 (MSR_ERR_INJ_LCK_UNLOCK_CTL, ErrInjLckUnlockCtlReg.Uint64);
}

/**

    This function will write to Internal MSR (ERR_INJ_LCK_UNLOCK_CTL_MSR) to lock error injection mechanism.
    See Machine Check Architecture HAS for details.

    @param node - Node Id

    @retval None

**/
VOID
EFIAPI
LockInjLogic(
  UINT8 *node
  )
{
  MSR_ERR_INJ_LCK_UNLOCK_CTL_REGISTER ErrInjLckUnlockCtlReg;

  ErrInjLckUnlockCtlReg.Uint64 = AsmReadMsr64 (MSR_ERR_INJ_LCK_UNLOCK_CTL);
  ErrInjLckUnlockCtlReg.Bits.ErrInjLckUnlock = 1;

  AsmWriteMsr64 (MSR_ERR_INJ_LCK_UNLOCK_CTL, ErrInjLckUnlockCtlReg.Uint64);
}

EFI_STATUS
EFIAPI
SetBiosRsvdInjAddress(
  VOID
  )
{
  UINT64      BiosRsvdStartAddr;
  UINT64      BiosRsvdEndAddr;

  BiosRsvdStartAddr  = (UINT64) (UINTN) MemoryLlcErrInjAddr + 0x100;
  BiosRsvdStartAddr &= ~(0xff);
  BiosRsvdEndAddr    = (UINT64) (UINTN) MemoryLlcErrInjAddr + WHEA_EINJ_ADDR_RANGE - 0x400;  // Safe side reduced 200h bytes from the limit.
  mBiosRsvdInjAddr  += 0x400;
  if (mBiosRsvdInjAddr >= BiosRsvdEndAddr) {
      return EFI_OUT_OF_RESOURCES;
  }
  mEinjParam->EinjAddr = mBiosRsvdInjAddr;
  mEinjParam->AddrMask = 0xfffffffffffff000;

  return EFI_SUCCESS;
}

/**

    SW SMI handler Registered to RAS_SW_SMI_EINJ_EXECUTE. This will execute error injection.
    This handler supports patrol scrub injection, EWB error injection and correctable error injections.

    @param DispatchHandle     - The unique handle assigned to this handler by SmiHandlerRegister().Type
                                            EFI_HANDLE is defined in InstallProtocolInterface() in the UEFI 2.1 Specification.
    @param DispatchContext   - Points to the optional handler context which was specified when the handler was registered.
    @param CommBuffer         - A pointer to a collection of data in memory that will be conveyed from a non-SMM
                                           environment into an SMM environment. The buffer must be contiguous, physically mapped, and be a physical address.
    @param CommBufferSize    - The size of the CommBuffer.

    @retval EFI_SUCCESS - The interrupt was handled and quiesced. No other handlers should still be called.

**/
EFI_STATUS
EFIAPI
WheaEinjHandler (
  IN  EFI_HANDLE                          DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN  OUT VOID                            *CommBuffer,     OPTIONAL
  IN  OUT UINTN                           *CommBufferSize  OPTIONAL
  )
{
  UINT32          PcieSBDF;
  UINT32          ErrorToInject;
  BOOLEAN         PoisonEnable = FALSE;
  BOOLEAN         Einj5ValidErr = FALSE;
  UINT32          ApicId;
  UINT8           SktId;
  EFI_STATUS      Status;
  UINTN           CoreNum;
  UINTN           ThreadNum;

  RAS_DEBUG ((LEVEL_REG, "Whea error injection enter! \n"));

  SktId = 0;    // assume socket 0 unless specified by EINJ

  mEinjParam->OpStatus = 0;
  mEinjParam->CmdStatus = 0;
  mEinjParam->EinjAddr = 0;   // Set address
  mEinjParam->AddrMask = 0;   // Address Mask

  mMESegInjEn = FALSE;
  mMESegIcvErrEn = FALSE;

  RAS_DEBUG ((
    LEVEL_REG,
    "[EINJ]Error with Address. Error Type:0x%x   Vendor:0x%x   Flags:0x%x  APIC ID:0x%x\n",
    mEinjParam->SetAddressTable.ErrType,
    mEinjParam->SetAddressTable.VendorErrTypeExtOffset,
    mEinjParam->SetAddressTable.Flags,
    mEinjParam->SetAddressTable.ApicId
    ));
  RAS_DEBUG ((
    LEVEL_REG,
    "[EINJ]Error with Address. MemAddress:0x%lx   MemAddressRange:0x%lx   PcieSBDF:0x%x\n",
    mEinjParam->SetAddressTable.MemAddress,
    mEinjParam->SetAddressTable.MemAddressRange,
    mEinjParam->SetAddressTable.PcieSBDF
    ));

  //
  // Check what type of error to be injected and create log accordingly.
  //
  // From SetError type field.
  //
  ErrorToInject = (UINT32) mEinjParam->ErrorToInject[0] & 0x7fffffff;
  RAS_DEBUG ((LEVEL_REG,"[EINJ]  Error Type from injection :0x%x\n", ErrorToInject));

  mErrorExpected = WHEA_NO_CONSUME;

  if (ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL) {
    PoisonEnable    = TRUE;
    mErrorExpected  = WHEA_REC_PATROLSCRUB_ERROR;
  }

  if(ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_MEMORY_CORRECTABLE) {
    mErrorExpected = WHEA_MEM_COR_ERROR;
  }

  if (mAcpi5Support) {
    //
    // Included Temp error type also for testing by SSG
    //
    if ((mEinjParam->SetAddressTable.ErrType & WHEA_TEMP_MEM_ERROR_TYPE )  ||
       ((mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) && (mEinjParam->VendorExt.OemDefType2 & WHEA_MEM_ERROR_REC_TYPE))) {
      ErrorToInject   = EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL;
      mErrorExpected  = WHEA_CORE_IFU_DCU_ERR;
      PoisonEnable    = TRUE;
    }

    if ((mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) && (mEinjParam->VendorExt.OemDefType2 & WHEA_PATROL_SCRUB_ERROR)) {
      ErrorToInject = EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL;
      mErrorExpected  = WHEA_REC_PATROLSCRUB_ERROR;
      PoisonEnable    = TRUE;
    }

    if ((mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) && (mEinjParam->VendorExt.OemDefType2 & WHEA_UMA_ERROR_TYPE)) {
      mMESegInjEn = TRUE;
      PoisonEnable = TRUE;
      mUmaErrorInjectionType = mEinjParam->VendorExt.OemDefType3;

      if ((mUmaErrorInjectionType & (WHEA_MESEG_COR_ERR_INJ_CONSUME_NOW |
                                     WHEA_MESEG_COR_ERR_INJ_CONSUME_1MS |
                                     WHEA_MESEG_COR_ERR_INJ_CONSUME_G0_S1_RW |
                                     WHEA_MESEG_COR_ERR_INJ_CONSUME_RESET_WARN)) != 0) {
        ErrorToInject = EFI_ACPI_6_2_EINJ_ERROR_MEMORY_CORRECTABLE;
        RAS_DEBUG ((LEVEL_REG, "Requesting UMA CE njection\n"));
      }

      if ((mUmaErrorInjectionType & (WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_NOW |
                                     WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_1MS |
                                     WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_G0_S1_RW |
                                     WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_RESET_WARN)) != 0) {
        ErrorToInject = EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL;
        RAS_DEBUG ((LEVEL_REG, "Requesting UMA UCE Injection\n"));
      }

      if ((mUmaErrorInjectionType & (WHEA_MESEG_ICV_ERR_INJ_CONSUME_NOW |
                                     WHEA_MESEG_ICV_ERR_INJ_CONSUME_1MS |
                                     WHEA_MESEG_ICV_ERR_INJ_CONSUME_G0_S1_RW |
                                     WHEA_MESEG_ICV_ERR_INJ_CONSUME_RESET_WARN)) != 0) {
        mMESegIcvErrEn = TRUE;
        ErrorToInject = EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL;
        RAS_DEBUG ((LEVEL_REG, "Requesting UMA ICV Error Injection\n"));
      }
    }

    if ((mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) != WHEA_VENDOR_EXT_TYPE) {
      if (mEinjParam->SetAddressTable.ErrType & (EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_CORRECTABLE |
                                                 EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_NONFATAL |
                                                 EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_FATAL |
                                                 EFI_ACPI_6_2_EINJ_ERROR_MEMORY_CORRECTABLE |
                                                 EFI_ACPI_6_2_EINJ_ERROR_PROCESSOR_UNCORRECTABLE_NONFATAL |
                                                 EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL |
                                                 EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL)) {
        ErrorToInject = mEinjParam->SetAddressTable.ErrType;
        if (mEinjParam->SetAddressTable.ErrType & EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL) {
          PoisonEnable    = TRUE;
          mErrorExpected  = WHEA_REC_PATROLSCRUB_ERROR;
        }
        if (mEinjParam->SetAddressTable.ErrType & EFI_ACPI_6_2_EINJ_ERROR_MEMORY_CORRECTABLE) {
          mErrorExpected = WHEA_MEM_COR_ERROR;
        }
      }
    }

    RAS_DEBUG ((LEVEL_REG, "mEinjParam->SetAddressTable.Flags : 0x%x \n", mEinjParam->SetAddressTable.Flags));
    if (mEinjParam->SetAddressTable.Flags & WHEA_SET_ERROR_TYPE_WITH_MEMORY_ADDRESS_VALID) {
      mEinjParam->EinjAddr = (UINT64) mEinjParam->SetAddressTable.MemAddress;  // Set address
      mEinjParam->AddrMask = (UINT64) mEinjParam->SetAddressTable.MemAddressRange;  // Address Mask
      Einj5ValidErr  = TRUE;
    } else {
      //
      //use old way to get data.
      //
      mEinjParam->EinjAddr = mEinjParam->ErrorToInject[3];  // Set address
      mEinjParam->AddrMask = mEinjParam->ErrorToInject[4];  // Address Mask
    }
    RAS_DEBUG ((LEVEL_REG, " Error inj address:0x%lx, mask:0x%lx \n", mEinjParam->EinjAddr, mEinjParam->AddrMask));

    if ((mEinjParam->SetAddressTable.Flags & WHEA_SET_ERROR_TYPE_WITH_PROC_APIC_VALID)) {
      ApicId = mEinjParam->SetAddressTable.ApicId;
      GetCoreAndSmtBits (&ThreadNum, &CoreNum);
      SktId = (UINT8) (ApicId >> (ThreadNum + CoreNum));
      RAS_DEBUG ((LEVEL_REG, " ApicId= 0x%x, \n", ApicId));
    }
    mEinjParam->SetAddressTable.ErrType = 0;  // Bios has to clear this field after we consumed it.
  }


  if ((ErrorToInject != EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_NONFATAL) &&
    (ErrorToInject != EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_CORRECTABLE)) {
    if ((!(mEinjParam->EinjAddr)) || (!(mEinjParam->AddrMask))) {
      Status = SetBiosRsvdInjAddress ();

      if (EFI_ERROR (Status)) {
        mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
        return EFI_INTERRUPT_PENDING;
      }
    }
  }

  switch (ErrorToInject) {
    case EFI_ACPI_6_2_EINJ_ERROR_PROCESSOR_UNCORRECTABLE_NONFATAL:           //Explicit Write Back Error
      if (IsSocketPresent (SktId) == FALSE ) {
        RAS_DEBUG ((LEVEL_REG, " - EWB injection Request, SktId passed= 0x%x, Socket not present\n", SktId));
        return EFI_SUCCESS;
      }

      SetViralPoison (EnablePoisonOnly);
      InjectLLCError (SktId);
      break;
    case EFI_ACPI_6_2_EINJ_ERROR_MEMORY_CORRECTABLE:
      RAS_DEBUG ((LEVEL_REG, "INJECT_ERROR_MEMORY_CE\n"));
      InjectMemoryError (EFI_ACPI_6_2_EINJ_ERROR_MEMORY_CORRECTABLE);
      break;
    case EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL:
      RAS_DEBUG ((LEVEL_REG, "INJECT_ERROR_MEMORY_UE_NON_FATAL\n"));

      SetViralPoison (EnablePoisonOnly);
      InjectMemoryError (EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL);
      break;
    case EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL:
      RAS_DEBUG ((LEVEL_REG, "INJECT_ERROR_MEMORY_UE_FATAL\n"));

      //
      // For WHQL testing , Disabling the poison and injecting uncorrected memeory error to cause system reset with blue screen.
      //
      SetViralPoison (DisablePoisonViral);
      InjectMemoryError (EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL);
      break;
    case EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_FATAL :
    case EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_CORRECTABLE :
    case EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_NONFATAL :
      //
      // Check Set addresstable has valid bus, dev, func numbers.
      //
      if ((mEinjParam->SetAddressTable.PcieSBDF) && (((mEinjParam->SetAddressTable.Flags & WHEA_SET_ERROR_TYPE_WITH_SBDF_VALID) == WHEA_SET_ERROR_TYPE_WITH_SBDF_VALID))) {
        PcieSBDF = mEinjParam->SetAddressTable.PcieSBDF;
        RAS_DEBUG ((LEVEL_REG, "PCIE error with specific root port:0x%x\n", PcieSBDF));
        if (ValidateSBDFForInjection (PcieSBDF) == TRUE) {
          Status = InjectPciExError (PcieSBDF, ErrorToInject, mEinjAction, mEinjParam);
        } else {
          Status = EFI_UNSUPPORTED;
        }
      } else {
        RAS_DEBUG ((LEVEL_REG, "PCIE error with default root port\n"));
        PcieSBDF = GetRootPortForInjection ();
        RAS_DEBUG ((LEVEL_REG, "root port address:0x%x\n", PcieSBDF));

        if (PcieSBDF != 0) {
          Status = InjectPciExError (PcieSBDF, ErrorToInject, mEinjAction, mEinjParam);
        } else {
          Status = EFI_UNSUPPORTED;
        }
      }
      if (EFI_ERROR (Status)) {
        mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
      }
      break;
    default:
      break;

  }
  return EFI_SUCCESS;
}

/**

    Updates and install EINJ ACPI Tables

    @param None

    @retval Status - The update/install was successful

**/
EFI_STATUS
EFIAPI
UpdateEinj (
  VOID
  )
{
  UINT8                             *Ests;
  UINT32                            Offset;
  UINT8                             i;
  EFI_STATUS                        Status;
  UINT32                            VidM2m;

  VidM2m = 0;

  if (mPolicyData->WheaPolicy.WheaErrorInjSupportEn == 0) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, " WHEA Error Injection is not enabled in Setup\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate Error status block for Error Injection - we should change this to use
  // Error status block assigned for the error source
  //
  Status = gBS->AllocatePool (EfiReservedMemoryType, 0x900, (VOID **) &Ests);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (Ests, 0x900);

  //
  // Init buffer pointers and data for Einj parameter and Error log address range.
  //
  mEinjParam    = (WHEA_EINJ_PARAM_BUFFER *) Ests;
  mEinjAction   = (WHEA_EINJ_TRIGGER_ACTION_TABLE *) (Ests + ((sizeof (WHEA_EINJ_PARAM_BUFFER) + 0x3F) / 0x40) * 0x40);
  mEinjEsts     = ((UINT8 *) mEinjAction) + ((sizeof (WHEA_EINJ_TRIGGER_ACTION_TABLE) + 0x3F) / 0x40) * 0x40;

  //
  // Init EINJ action table in the allocated reserved memory
  // Update Error injection Action table - Static initialization is sufficient
  //
  CopyMem (mEinjAction, &mSimEinjAction, sizeof (WHEA_EINJ_TRIGGER_ACTION_TABLE));

  //
  // Allocate memory for 6000h bytes for Error injection structure.
  //
  Status = gBS->AllocatePool (EfiRuntimeServicesData, WHEA_EINJ_ADDR_RANGE, &MemoryLlcErrInjAddr);
  //
  // Make it cache line boundary - This is the incremantal injection address Bios uses for error injection
  //
  mBiosRsvdInjAddr  = (UINT64) (UINTN) MemoryLlcErrInjAddr + 0x800;
  mBiosRsvdInjAddr  &= ~(0xff);
  //
  // Init EINJ parameters
  //
  mEinjParam->TriggerActionTable    = (UINT64) (UINTN) mEinjAction;
  mEinjParam->ErrorInjectCap        = EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL |
                                      EFI_ACPI_6_2_EINJ_ERROR_MEMORY_CORRECTABLE |
                                      EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL;
  //
  // Check Acpi 5.0 support enabled.
  //

  if (mPolicyData->WheaPolicy.WheaPcieErrInjEn) {
    mEinjParam->ErrorInjectCap  |= EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_CORRECTABLE  |
                                   EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_NONFATAL |
                                   EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_FATAL ;
  }

  mEinjParam->PcieErrInjActionTable = mPcieErrInjActionTable;

  if (mAcpi5Support) {
    mEinjParam->ErrorInjectCap      |= WHEA_VENDOR_EXT_TYPE | WHEA_TEMP_MEM_ERROR_TYPE;
  }

  mEinjParam->ErrorToInject[3]      = 0;
  mEinjParam->ErrorToInject[4]      = 0;
  mEinjParam->OpState               = 0;
  mEinjParam->OpStatus              = 0;
  mEinjParam->CmdStatus             = 0;
  mEinjParam->ErrorToInject[0]      = 0;
  mEinjParam->EinjAddr              = 0;
  mEinjParam->AddrMask              = 0;

  //
  // Update Error injection serialization informations
  //
  mSimEinj[0].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->OpState;              // Update address for BEGIN_OPERATION
  mSimEinj[1].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->TriggerActionTable;   // Update address for GET_TRIGGER_TABLE
  mSimEinj[2].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->ErrorToInject[0];     // Update address for SET_ERROR
  mSimEinj[3].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->ErrorInjectCap;       // Update address for GET_ERROR_CAP
  mSimEinj[4].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->OpState;              // Update address for END_OPERATION
  mSimEinj[6].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->OpStatus;             // Update address for CHECK_BUSY_STATUS
  mSimEinj[7].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->CmdStatus;            // Update address for GET_CMD_STATUS

  if (mAcpi5Support)  {
    mSimEinj[8].RegisterRegion.Address = (UINT64) (UINTN) &mEinjParam->SetAddressTable;        // Update address for SET_ERROR_TYPE_WITH_ADDRESS

    Offset = (UINT32) ((UINTN) &mEinjParam->VendorExt - (UINTN) &mEinjParam->SetAddressTable);
    mEinjParam->SetAddressTable.VendorErrTypeExtOffset = Offset;
    mEinjParam->SetAddressTable.PcieSBDF  = 0;

    mEinjParam->VendorExt.Length    = sizeof (WHEA_EINJ_VENDOR_EXT);
    //
    //SBDF Byte 0 - reserved, Byte 1 - Device and Function, Byte 2 - Bus, Byte 3 PCIe segment
    //
    VidM2m = GetM2mVid (0, 0);
    mEinjParam->VendorExt.SBDF      = GetM2mCsrBusDevFunAddress(0, 0) >> 4;
    mEinjParam->VendorExt.VendorId  = (UINT16) VidM2m;
    mEinjParam->VendorExt.DeviceId  = (UINT16) GetM2mDid (0,0);
    mEinjParam->VendorExt.RevId     = (UINT8) VidM2m;
  }

  RAS_DEBUG((LEVEL_REG, " WHEA Error Injection driver is installed\n"));

  //
  // install WHEA EINJ table.
  //
  for (i = 0; i < 7; i ++) {
    AddEinjActionItem (&mSimEinj[i], FALSE);
  }

  if (mAcpi5Support) {
    AddEinjActionItem (&mSimEinj[7], FALSE);
    AddEinjActionItem (&mSimEinj[8], TRUE);
  } else {
    AddEinjActionItem (&mSimEinj[7], TRUE);
  }

  return Status;
}

/**
  AEP error injection callback for Crystal Ridge ready.

  Enable or disable AEP error injection, depending on the policy.
**/
VOID
EFIAPI
AepErrorInjectionCrystalRidgeCallback (
  VOID
  )
{
  if (mPolicyData->WheaPolicy.AEPErrorInjEn == 1) {
    EnableAepErrorInjection ();
  }
} // AepErrorInjectionCrystalRidgeCallback

/**

  Entry point of the Whea error injection driver.

  @param ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  @param SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES:     Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
InitWheaErrorInj (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        SwHandle;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch  = 0;
  UINT8                             Socket;

  mPolicyData = GetRasPolicy ();
  mMemTopologyData = GetMemTopology ();

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    //
    // Unlock error injection mechanism on socket basis
    //
    MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, UnLockInjLogic, NULL);

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      InitializeAdddcErrInj (Socket);
    }

    //
    // Lock the error injection mechanism
    //
    MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
  }

  //
  // Register to enable/disable AEP error injection when the library is ready.
  //
  RegisterCrystalRidgeReadyCallback (AepErrorInjectionCrystalRidgeCallback);

  if (mPolicyData->WheaPolicy.WheaErrorInjSupportEn == 0) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, " WHEA Error Injection is not enabled in Setup\n"));
    return EFI_SUCCESS;
  }

  if (mPolicyData->WheaPolicy.WheaACPI50ErrInjEn){
    mAcpi5Support  = TRUE;
  }

  if(mPolicyData->WheaPolicy.PcieErrInjActionTable == 1) {
    mPcieErrInjActionTable = TRUE;
  }

  mMESegInjAddr = 0;
  //
  // Update EINJ table entries.
  //
  Status = UpdateEinj ();
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
  ASSERT_EFI_ERROR (Status);

  Is10nmFam = IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) ||
             IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL);

  //
  // Install SW SMI handler to build error data for simulated error injection
  //
  SwContext.SwSmiInputValue = RAS_SW_SMI_EINJ_EXECUTE;
  Status = SwDispatch->Register (SwDispatch, WheaEinjHandler, &SwContext, &SwHandle);
  ASSERT_EFI_ERROR (Status);

  EinjIioErrInjLockSwSmi = RAS_SW_SMI_EINJ_IIO_ERR_INJ_LCK;
  SwContext.SwSmiInputValue = (UINTN) EinjIioErrInjLockSwSmi;
  Status = SwDispatch->Register (SwDispatch, IioErrorInjectionLockHandler, &SwContext, &SwHandle);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
