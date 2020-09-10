/** @file
  This is an implementation of the BootScript at run time.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2010 Intel Corporation. <BR>

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

#ifndef _SMM_SCRIPT_SAVE_H_
#define _SMM_SCRIPT_SAVE_H_

#include <PiDxe.h>
#include <Tbd/EfiBootScript.h>

typedef EFI_PHYSICAL_ADDRESS     EFI_SMM_SCRIPT_TABLE;

/**

  Intialize Boot Script table.

  @param SystemTable         -  Pointer to the EFI sytem table
  @param SmmScriptTablePages -  The expected ScriptTable page number
  SmmScriptTableBase  -  The returned ScriptTable base address

  @retval EFI_OUT_OF_RESOURCES   -  No resource to do the initialization.
  @retval EFI_SUCCESS            -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
InitializeSmmScriptLib (
  IN  EFI_SYSTEM_TABLE     *SystemTable,
  IN  UINTN                SmmScriptTablePages,
  OUT EFI_PHYSICAL_ADDRESS *SmmScriptTableBase
  )
;

/**

  Create Boot Script table.

  @param ScriptTable  -  Pointer to the boot script table to create.
  @param Type         -  The type of table to creat.


  @retval EFI_INVALID_PARAMETER  -  Invalid parameter detected.
  @retval EFI_SUCCESS            -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
SmmBootScriptCreateTable (
  IN OUT   EFI_SMM_SCRIPT_TABLE    *ScriptTable,
  IN       UINTN                   Type
  )
;

EFI_STATUS
EFIAPI
SmmBootScriptWrite (
  IN OUT   EFI_SMM_SCRIPT_TABLE    *ScriptTable,
  IN       UINTN                   Type,
  IN       UINT16                  OpCode,
  ...
  )
/**

  Write to boot script table. Format is defined in AcpiScritSave Protocol.
  Currently it supports MemWrite, IoWrite, PciCfgWrite.

  @param ScriptTable  -  Pointer to the script table to write to.
  @param Type         -  Not used.
  @param OpCode       -  Table write's Opcode.
  @param ...          -  Parameter of the OpCode.

  @retval EFI_INVALID_PARAMETER  -  Invalid parameter passed in detected.
  @retval EFI_SUCCESS            -  Function completed successfully.
  @retval Other                  -  Error occurred during execution.

**/
;

/**

  Close Boot Script table.

  @param ScriptTableBase  -  Pointer to the boot script table to create.
  @param ScriptTablePtr   -  Pointer to the script table to write to.
  @param Type             -  The type of table to creat.


  @retval EFI_SUCCESS            -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
SmmBootScriptCloseTable (
  IN EFI_SMM_SCRIPT_TABLE        ScriptTableBase,
  IN EFI_SMM_SCRIPT_TABLE        ScriptTablePtr,
  IN UINTN                       Type
  )
;

#endif
