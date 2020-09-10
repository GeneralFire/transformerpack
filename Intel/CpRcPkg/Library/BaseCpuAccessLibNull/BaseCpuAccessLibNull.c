/** @file
  Base implementation of the Cpu Access library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/CpuAccessLib.h>
#include <Library/BaseMemoryLib.h>

//
// For the UINT64_STRUCT definition only
//

#include <ReferenceCodeDataTypes.h>

/*++

Reads 64-BIT Machine status register

@param msrAddr - Address of MSR to read

@retval UINT64_STRUCT - MSR value as read from CPU

--*/

UINT64_STRUCT
EFIAPI
ReadMsr (
  UINT32 Index
  )
{
  UINT64_STRUCT Value;

  ZeroMem (&Value, sizeof (Value));
  
  return Value;

} // ReadMsr

/*++

Writes 64-BIT Machine status register

@param msrAddr - Address of MSR to write
@param data    - 64 bit data to write to MSR

@retval N/A

--*/

VOID
EFIAPI
WriteMsr (
  UINT32        Index,
  UINT64_STRUCT ValueStruct
  )
{
  return;

} // WriteMsr

/*++

Reads 8-bit APIC ID from the CPU executing this code

@retval UINT32 - Returns APIC ID

--*/

UINT8
EFIAPI
GetProcApicId (
  VOID
  )
{

  return 0;

} // GetProcApicId

/*++

  C-wrapper of CPUID instruction

  @param RegEax - ptr to EAX register
  @param RegEbx - ptr to EBX register
  @param RegEcx - ptr to ECX register
  @param RegEdx - ptr to EDX register

  @retval N/A: EAX, EBX, ECX, EDX contains return values of CPUID instruction

--*/

VOID
EFIAPI
GetCpuId  (
  UINT32*   RegEax,
  UINT32*   RegEbx,
  UINT32*   RegEcx,
  UINT32*   RegEdx
  )
{

  return;

} // GetCpuId

