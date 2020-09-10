/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2018 Intel Corporation. <BR>

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

#ifndef _SMM_VARIABLE_H
#define _SMM_VARIABLE_H
//
// Statements that include other header files
//
#include <Uefi.h>
#include <Protocol/Variable.h>
#include <Guid/VariableFormat.h>
#include <Guid/SystemNvDataGuid.h>
#include <Protocol/VariableWrite.h>
#include <Protocol/SmmSwDispatch2.h>

#define ALIGNMENT 1
#define R9_VARIABLE_COMP

#define VARIABLE_STORE_SIZE (64 * 1024)
#define SCRATCH_SIZE        (4 * 1024)

#define VARIABLE_RECLAIM_THRESHOLD (1024)
//
// Define GET_PAD_SIZE to optimize compiler
//
#if ((ALIGNMENT == 0) || (ALIGNMENT == 1))
#define GET_PAD_SIZE(a) (0)
#else
#define GET_PAD_SIZE(a) (((~a) + 1) & (ALIGNMENT - 1))
#endif

#define GET_VARIABLE_NAME_PTR(a)  (CHAR16 *) ((UINTN) (a) + sizeof (VARIABLE_HEADER))

#ifndef R9_VARIABLE_COMP
#define HEADER_ALIGN(Header)
#define SMM_VARIABLE_ADDED            0x7F
#define VARIABLE_STORE_HEADER_COMP    VARIABLE_STORE_HEADER
#else
//
// Alignment of Variable Data Header in Variable Store region
// Add this is to be compatiable to R9 variable driver.
//
#define HEADER_ALIGNMENT  4
#define HEADER_ALIGN(Header)  (((UINTN) (Header) + HEADER_ALIGNMENT - 1) & (~(HEADER_ALIGNMENT - 1)))
#define SMM_VARIABLE_ADDED  0x3F
#pragma pack(1)
///
/// Variable Store region header
///
typedef struct {
  ///
  /// Variable store region signature.
  ///
  EFI_GUID  Signature;
  ///
  /// Size of entire variable store,
  /// including size of variable store header but not including the size of FvHeader.
  ///
  UINT32  Size;
  ///
  /// variable region format state
  ///
  UINT8   Format;
  ///
  /// variable region healthy state
  ///
  UINT8   State;
  UINT16  Reserved;
  UINT32  Reserved1;
} VARIABLE_STORE_HEADER_COMP;
#pragma pack()
#endif

typedef enum {
  Physical,
  Virtual
} VARIABLE_POINTER_TYPE;

typedef struct {
  VARIABLE_HEADER *CurrPtr;
  VARIABLE_HEADER *EndPtr;
  VARIABLE_HEADER *StartPtr;
  BOOLEAN         Volatile;
} VARIABLE_POINTER_TRACK;

typedef struct {
  EFI_PHYSICAL_ADDRESS  VolatileVariableBase;
  EFI_PHYSICAL_ADDRESS  NonVolatileVariableBase;
} VARIABLE_GLOBAL;

typedef struct {
  VARIABLE_GLOBAL VariableBase[2];
  UINTN           VolatileLastVariableOffset;
  UINTN           NonVolatileLastVariableOffset;
  UINT32          FvbInstance;
} ESAL_VARIABLE_GLOBAL;

//
// Functions
//

EFI_STATUS
EFIAPI
SmmGetVariable (
  IN CHAR16        *VariableName,
  IN EFI_GUID      * VendorGuid,
  OUT UINT32       *Attributes OPTIONAL,
  IN OUT UINTN     *DataSize,
  OUT VOID         *Data
  );

EFI_STATUS
EFIAPI
SmmGetNextVariableName (
  IN OUT UINTN     *VariableNameSize,
  IN OUT CHAR16    *VariableName,
  IN OUT EFI_GUID  *VendorGuid
  );

EFI_STATUS
EFIAPI
SmmSetVariable (
  IN CHAR16        *VariableName,
  IN EFI_GUID      *VendorGuid,
  IN UINT32        Attributes,
  IN UINTN         DataSize,
  IN VOID          *Data
  );

#endif
