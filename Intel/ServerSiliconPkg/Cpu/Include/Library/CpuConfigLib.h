/** @file
  Public include file for the CPU Configuration Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_CONFIG_LIB_H_
#define _CPU_CONFIG_LIB_H_

#include <Protocol/MpService.h>
#include <AcpiCpuData.h>
#include <CpuDataStruct.h>

//
// PURLEY_OVERRIDE_BEGIN
//
// CPU C State Settings
#define C0_ENABLE                           0x00
#define C3_ENABLE                           0x02
#define C6_ENABLE                           0x03
#define C7_ENABLE                           0x04
#define C8_ENABLE                           0x05
#define C9_ENABLE                           0x06
#define C10_ENABLE                          0x07
//
// PURLEY_OVERRIDE_END
//

//
// Structure for collected CPUID data.
//
typedef struct {
  EFI_CPUID_REGISTER         *CpuIdLeaf;
  UINTN                      NumberOfBasicCpuidLeafs;
  UINTN                      NumberOfExtendedCpuidLeafs;
  UINTN                      NumberOfCacheAndTlbCpuidLeafs;
  UINTN                      NumberOfDeterministicCacheParametersCpuidLeafs;
  UINTN                      NumberOfExtendedTopologyEnumerationLeafs;
} CPU_CPUID_DATA;

typedef struct {
  UINTN    Ratio;
  UINTN    Vid;
  UINTN    Power;
  UINTN    TransitionLatency;
  UINTN    BusMasterLatency;
} FVID_ENTRY;

typedef struct {
  VOID     *MicrocodeData;
  UINTN    MicrocodeSize;
  UINT32   ProcessorId;
} MICROCODE_INFO;

//
// Miscellaneous processor data
//
typedef struct {
  //
  // Local Apic Data
  //
  UINT32                     InitialApicID;  ///< Initial APIC ID
  UINT32                     ApicID;         ///< Current APIC ID
  EFI_PHYSICAL_ADDRESS       ApicBase;
  UINT32                     ApicVersion;
  //
  // Frequency data
  //
  UINTN                      IntendedFsbFrequency;
  UINTN                      ActualFsbFrequency;
  UINTN                      MaxCoreToBusRatio;
  UINTN                      MinCoreToBusRatio;
  UINTN                      MaxTurboRatio;
  UINTN                      PackageTdp;
  UINTN                      NumberOfPStates;
  FVID_ENTRY                 *FvidTable;
  UINTN                      GreaterNumberOfPStates;  // Greater Than 16 p-state support
  FVID_ENTRY                 *GreaterFvidTable;       // Greater Than 16 p-state support
  //
  // Other data
  //
  UINT32                     MicrocodeRevision;
  UINT64                     EnabledThreadCountMsr; // PURLEY_OVERRIDE_5371439
  MICROCODE_INFO             MicrocodeInfo;
  UINT64                     MiscEnablesMsr;
} CPU_MISC_DATA;

//
// Structure for all collected processor data
//
typedef struct {
  CPU_CPUID_DATA             CpuidData;
  EFI_CPU_PHYSICAL_LOCATION  ProcessorLocation;
  CPU_MISC_DATA              CpuMiscData;
  UINT8                      PackageIdBitOffset;
  BOOLEAN                    PackageBsp;
} CPU_COLLECTED_DATA;

#define GET_CPU_MISC_DATA(ProcessorNumber, Item) \
  ((mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber]).CpuMiscData.Item)


//
// Definition of Processor Configuration Context Buffer
//
typedef struct {
  UINTN                    NumberOfProcessors;
  UINTN                    BspNumber;
  CPU_COLLECTED_DATA       *CollectedDataBuffer;
  CPU_REGISTER_TABLE       *PreSmmInitRegisterTable;
  CPU_REGISTER_TABLE       *RegisterTable;
  BOOLEAN                  RegisterTableSaved;
} CPU_CONFIG_CONTEXT_BUFFER;

//
// Structure conveying socket ID configuration information.
//
typedef struct {
  UINT32                    DefaultSocketId;
  UINT32                    NewSocketId;
} CPU_SOCKET_ID_INFO;

extern CPU_CONFIG_CONTEXT_BUFFER      *mCpuConfigLibConfigContextBuffer;

typedef enum {
  BasicCpuidLeaf,
  ExtendedCpuidLeaf,
  CacheAndTlbCpuidLeafs,
  DeterministicCacheParametersCpuidLeafs,
  ExtendedTopologyEnumerationCpuidLeafs
} CPUID_TYPE;

/**
  Get the number of CPUID leafs of various types.

  This function get the number of CPUID leafs of various types.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  CpuidType         The type of the CPU id.

  @return Maximal index of CPUID instruction for basic leafs.

**/
UINTN
EFIAPI
GetNumberOfCpuidLeafs (
  IN  UINTN               ProcessorNumber,
  IN  CPUID_TYPE          CpuidType
  );

/**
  Get the pointer to specified CPUID leaf.

  This function gets the pointer to specified CPUID leaf.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf

**/
EFI_CPUID_REGISTER*
EFIAPI
GetProcessorCpuid (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
  );

/**
  Get the pointer to specified CPUID leaf of cache and TLB parameters.

  This function gets the pointer to specified CPUID leaf of cache and TLB parameters.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetCacheAndTlbCpuidLeaf (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
  );

/**
  Get the pointer to specified CPUID leaf of deterministic cache parameters.

  This function gets the pointer to specified CPUID leaf of deterministic cache parameters.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetDeterministicCacheParametersCpuidLeaf (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
  );

/**
  Get the pointer to specified CPUID leaf of Extended Topology Enumeration.

  This function gets the pointer to specified CPUID leaf of Extended Topology Enumeration.

  @param  ProcessorNumber   Handle number of specified logical processor.
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetExtendedTopologyEnumerationCpuidLeafs (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
  );

/**
  Get the version information of specified logical processor.

  This function gets the version information of specified logical processor,
  including family ID, model ID, stepping ID and processor type.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  DisplayedFamily   Pointer to family ID for output
  @param  DisplayedModel    Pointer to model ID for output
  @param  SteppingId        Pointer to stepping ID for output
  @param  ProcessorType     Pointer to processor type for output

**/
VOID
EFIAPI
GetProcessorVersionInfo (
  IN  UINTN               ProcessorNumber,
  OUT UINT32              *DisplayedFamily OPTIONAL,
  OUT UINT32              *DisplayedModel  OPTIONAL,
  OUT UINT32              *SteppingId      OPTIONAL,
  OUT UINT32              *ProcessorType   OPTIONAL
  );

/**
  Get initial local APIC ID of specified logical processor

  This function gets initial local APIC ID of specified logical processor.

  @param  ProcessorNumber Handle number of specified logical processor

  @return Initial local APIC ID of specified logical processor

**/
UINT32
EFIAPI
GetInitialLocalApicId (
  UINTN    ProcessorNumber
  );

/**
  Get the location of specified processor.

  This function gets the location of specified processor, including
  package number, core number within package, thread number within core.

  @param  ProcessorNumber Handle number of specified logical processor.
  @param  PackageNumber   Pointer to the output package number.
  @param  CoreNumber      Pointer to the output core number.
  @param  ThreadNumber    Pointer to the output thread number.

**/
VOID
EFIAPI
GetProcessorLocation (
  IN    UINTN     ProcessorNumber,
  OUT   UINT32    *PackageNumber   OPTIONAL,
  OUT   UINT32    *CoreNumber      OPTIONAL,
  OUT   UINT32    *ThreadNumber    OPTIONAL
  );

/**
  Add an entry in the post-SMM-init register table.

  This function adds an entry in the post-SMM-init register table, with given register type,
  register index, bit section and value.

  No entry will be added after SaveRegisterTable is called.

  @param  ProcessorNumber Handle number of specified logical processor
  @param  RegisterType    Type of the register to program
  @param  Index           Index of the register to program
  @param  ValidBitStart   Start of the bit section
  @param  ValidBitLength  Length of the bit section
  @param  Value           Value to write

**/
VOID
EFIAPI
WriteRegisterTable (
  IN  UINTN               ProcessorNumber,
  IN  REGISTER_TYPE       RegisterType,
  IN  UINT32              Index,
  IN  UINT8               ValidBitStart,
  IN  UINT8               ValidBitLength,
  IN  UINT64              Value
  );

/**
  Add an entry in the pre-SMM-init register table.

  This function adds an entry in the pre-SMM-init register table, with given register type,
  register index, bit section and value.

  No entry will be added after SaveRegisterTable is called.

  @param  ProcessorNumber Handle number of specified logical processor
  @param  RegisterType    Type of the register to program
  @param  Index           Index of the register to program
  @param  ValidBitStart   Start of the bit section
  @param  ValidBitLength  Length of the bit section
  @param  Value           Value to write

**/
VOID
EFIAPI
WritePreSmmInitRegisterTable (
  IN  UINTN               ProcessorNumber,
  IN  REGISTER_TYPE       RegisterType,
  IN  UINT32              Index,
  IN  UINT8               ValidBitStart,
  IN  UINT8               ValidBitLength,
  IN  UINT64              Value
  );

/**
  Save RegisterTable and PreSmmInitRegisterTable in mCpuConfigLibConfigContextBuffer
  to RegisterTable and PreSmmInitRegisterTable in PcdCpuS3DataAddress.

  It will combine the table entries from mCpuConfigLibConfigContextBuffer
  to PcdCpuS3DataAddress.

  It should be called at DxeSmmReadyToLock event with TPL_NOTIFY to make sure
  that it is after all Write(PreSmmInit)RegisterTable callings and before
  PcdCpuS3DataAddress is consumed by PiSmmCpuDxeSmm for CPU S3.

**/
VOID
EFIAPI
SaveRegisterTable (
  VOID
  );

#endif
