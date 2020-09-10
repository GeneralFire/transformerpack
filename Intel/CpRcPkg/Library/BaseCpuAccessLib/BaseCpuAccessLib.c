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

#include <Library/CpuAccessLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

//
// For the UINT64_STRUCT definition only
//

#include <ReferenceCodeDataTypes.h>

/*++

  Reads 64-BIT Machine status register (MSR)

  @param[in] Index - Address of MSR to read

  @retval UINT64_STRUCT - MSR value as read from CPU

--*/

UINT64_STRUCT
EFIAPI
ReadMsr (
  IN  UINT32 Index
  )
{
  UINT64_STRUCT ValueStruct;
  UINT64 Value;

  ZeroMem (&ValueStruct, sizeof (ValueStruct));

  Value = AsmReadMsr64 (Index);
  ValueStruct.lo = (UINT32)Value;
  ValueStruct.hi = (UINT32)RShiftU64 (Value, 32);

  return ValueStruct;

} // ReadMsr

/*++

  Writes 64-BIT Machine status register (MSR)

  @param[in] Index         - Address of MSR to write
  @param[in] ValueStruct   - 64 bit data to write to MSR

  @retval N/A

--*/

VOID
EFIAPI
WriteMsr (
  IN  UINT32        Index,
  IN  UINT64_STRUCT ValueStruct
  )
{
  UINT64 Value = 0;

  Value = LShiftU64(ValueStruct.hi, 32) | ValueStruct.lo;

  AsmWriteMsr64 (Index, Value);

} // WriteMsr

/*++

  Reads 8-bit APIC ID from the CPU executing this code

  @retval Returns APIC ID

--*/

UINT8
EFIAPI
GetProcApicId (
  VOID
  )
{
  UINT32 ApicId = 0;

  AsmCpuid (1, NULL, &ApicId, NULL, NULL);

  return (UINT8)((ApicId >> 24) & 0xFF);

} // GetProcApicId

/*++

  C-wrapper of CPUID instruction EAX, EBX, ECX, EDX contains
  the return values of CPUID instruction.

  @param[out]  RegEax - ptr to EAX register
  @param[out]  RegEbx - ptr to EBX register
  @param[out]  RegEcx - ptr to ECX register
  @param[out]  RegEdx - ptr to EDX register

  @retval N/A

--*/

VOID
EFIAPI
GetCpuId  (
  OUT  UINT32*  RegEax,
  OUT  UINT32*  RegEbx,
  OUT  UINT32*  RegEcx,
  OUT  UINT32*  RegEdx
  )
{
  AsmCpuid (*RegEax, RegEax, RegEbx, RegEcx, RegEdx);

  return;

} // GetCpuId

