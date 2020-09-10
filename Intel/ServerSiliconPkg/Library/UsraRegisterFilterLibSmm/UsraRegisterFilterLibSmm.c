/** @file
  Implementation of UsraRegisterFilterLib class library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include "UsraRegisterFilterLibInternal.h"
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SmmCpu.h>

//#define SOFT_SKU_DEBUG   0  // This macro is used to switch on or off extra DEBUG message.

//
// Global variable to cache pointer to SMM CPU Service protocol.
//
EFI_SMM_CPU_SERVICE_PROTOCOL      *mSmmCpuServiceProtocolInterface = NULL;
EFI_SMM_CPU_PROTOCOL              *mSmmCpuProtocolInterface;

///
/// Global spin lock used to serialize AP CSR access
///
SPIN_LOCK  *mApCompleteCsrAccessSpinLock = NULL;

//
//  This variable indicates which function pointer for Register Filter Write should be selected.
//
UINT8  mFuncPtrIndex = 0;

REGISTER_FILTER_WRITE mRegisterFilterWritePtr[] = {
  &RegisterFilterWriteMmio
  };

//
// Below table is created to provide stack information.
//

STATIC UINT8 mStackNumberTable[MAX_BOX_TYPES] = {
  IIO_PSTACK0, //BOX_CHA_MISC        0
  IIO_PSTACK0, //BOX_CHA_PMA         1
  IIO_PSTACK0, //BOX_CHA_CMS         2
  IIO_PSTACK0, //BOX_CHABC           3
  IIO_PSTACK0, //BOX_PCU             4
  IIO_PSTACK0, //BOX_VCU             5
  IIO_PSTACK1, //BOX_M2MEM           6
  IIO_PSTACK1, //BOX_MC              7
  IIO_PSTACK1, //BOX_MCIO            8
  IIO_PSTACK2, //BOX_KTI             9
  IIO_PSTACK2, //BOX_M3KTI           10
  IIO_PSTACK1, //BOX_MCDDC           11
  IIO_PSTACK2, //BOX_M2UPCIE         12
  IIO_CSTACK,  //BOX_IIO_PCIE_DMI    13
  0xFF,        //BOX_IIO_PCIE        14
  0xFF,        //BOX_IIO_PCIE_NTB    15
  IIO_CSTACK,  //BOX_IIO_CB          16
  0xFF,        //BOX_IIO_VTD         17
  0xFF,        //BOX_IIO_DFX         18
  IIO_CSTACK, //BOX_UBOX            19
  IIO_CSTACK  //BOX_FPGA            20
  //MAX_BOX_TYPES       21
};

/**
  This function returns Bus Stack Number

  @param[in] USRA_ADDRESS         The pointer to USRA Address

  @retval UINT8                   Return Bus stack Number.
**/

UINT8
GetBusStackNumber (
  IN  USRA_ADDRESS             *Address
  )
{
  UINT8  BoxType;
  UINT8  FuncBlk;
  UINT8  BoxInst;
  UINT8  StackNumber;

  BoxType = (UINT8)((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.boxtype;
  BoxInst = (UINT8)Address->Csr.InstId;
  FuncBlk = (UINT8)((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.funcblk;

  StackNumber = mStackNumberTable[BoxType];

  if (StackNumber == 0xFF) {
    switch (BoxType) {
    case BOX_IIO_PCIE:
      if (BoxInst == 0 ){
        StackNumber = IIO_CSTACK;
      } else {
        StackNumber = IIO_PSTACK0 + ((BoxInst-1) / 4);
      }
      break;
    case BOX_IIO_PCIE_NTB:
      if (BoxInst > 0) {
        StackNumber = IIO_PSTACK0 + ((BoxInst-1) / 4);
      }
      break;
    case BOX_IIO_VTD:
      StackNumber = IIO_CSTACK + BoxInst;
      break;
    case BOX_IIO_DFX:
      if (FuncBlk == IIO_DFX) {
        //
        // IIO_DFX is used to access all pcie ports in all stacks same as iio_pcie
        //
        if (BoxInst == 0) {
          StackNumber = IIO_CSTACK;
        } else {
          StackNumber = IIO_PSTACK0 + ((BoxInst-1) / 4);
        }
      } else if ((FuncBlk == IIO_DFX_GLOBAL) || (FuncBlk == IIO_DFX_VTD)) {
          //
          // IIO_DFX_GLOBAL and IIO_DFX_VTD maps 1 instance per c/p/m stack
          //
          if ((IIO_CSTACK + BoxInst) < MAX_IIO_STACK) {
            StackNumber = IIO_CSTACK + BoxInst;
          }
      } else if ((FuncBlk == IIO_DFX_VTD_DMI) ||
              (FuncBlk == IIO_DFX_DMI)      ||
              (FuncBlk == IIO_DFX_GLOBAL_DMI)) {
          StackNumber = IIO_CSTACK;
      }
      break;
    default:
      break;
    }

  }
  if (StackNumber == 0xFF) {
    DEBUG ((EFI_D_ERROR, "Soft SKU Support: Invalid Stack number returned ! BoxType: %x; FuncBlk: %x; BoxInst: %x\n", BoxType, FuncBlk, BoxInst ));
  }

  return StackNumber;
}

/**
  Perform MMIO read

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be read out
  @param[in] Buffer               A pointer of buffer contains the data to be read out

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
RegAlignedRead (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  switch (AccessWidth) {
  case  UsraWidth8:
    *((UINT8*)Buffer) = MmioRead8 (AlignedAddress);
    break;
  case  UsraWidth16:
    *((UINT16*)Buffer) = MmioRead16 (AlignedAddress);
    break;
  case  UsraWidth32:
    *((UINT32*)Buffer) = MmioRead32 (AlignedAddress);
    break;
  default:
    *((UINT64*)Buffer) = MmioRead64 (AlignedAddress);
    break;
  }

  return RETURN_SUCCESS;
};

/**
  Perform MMIO write

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be written
  @param[in] Buffer               A pointer of buffer contains the data to be written

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
RegAlignedWrite (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  switch (AccessWidth) {
  case  UsraWidth8:
    MmioWrite8 (AlignedAddress,*((UINT8*)Buffer));
    break;
  case  UsraWidth16:
    MmioWrite16 (AlignedAddress,*((UINT16*)Buffer));
    break;
  case  UsraWidth32:
    MmioWrite32 (AlignedAddress,*((UINT32*)Buffer));
    break;
  default:
    MmioWrite64 (AlignedAddress, *((UINT64*)Buffer));
    break;
  }
  return RETURN_SUCCESS;
}


/**

  Get SocketID for currently executing thread

  @param LocalSocket - Pointer to LocalSocket Buffer

  @return VOID

  **/
VOID
GetLocalSocketId(
  OUT UINT8 *LocalSocket
  )
{
  //
  // the APIC ID is composed of the Socket ID
  // and a combination of the Core ID and Thread ID, as illustrated
  // below:
  //
  //  ----------- -------------------
  // |  Bit 7-n  |      Bit n-0      |
  //  ----------- -------------------
  // | Socket ID | Core ID/Thread ID |
  //  ----------- -------------------
  // Bit "n" is Socket specific (defined via CPUID)
  //

  UINT8                PackageShift;
  UINT32               RegEax=0, RegEbx=0, RegEcx=0, RegEdx=0;

  //
  // Execute CPUID Leaf 0Bh, with ECX=1 (Core level), then
  // EAX[4:0]  = Number of bits to right-shift APIC ID to get SocketID (i.e., Packge-leval ID)
  // EDX[31:0] = Extended APIC ID -- Lower 8 bits identical to the legacy APIC ID
  //

  RegEax = CPUID_TOPO_INFO;
  RegEcx = 1;

  AsmCpuidEx (RegEax, RegEcx, &RegEax, &RegEbx, &RegEcx, &RegEdx);

  PackageShift = (UINT8)RegEax & 0x1f;
  *LocalSocket = (UINT8)(RegEdx >> PackageShift);

  return;
}
/**
  Finding Local Socket is equal to Target Socket
  Local Socket = Currently executing Socket
  Target Socket = Socket that CSR write is targeting

  @param TargetSocket - Socket that CSR write is targeting
  @return TRUE - if executing thread is part of the socket

**/

BOOLEAN
IsExecutingOnLocalSocket (
  UINT8 TargetSocket
  )
{
  UINT8 LocalSocketId = 0;

  GetLocalSocketId (&LocalSocketId);

  return (BOOLEAN)(TargetSocket == LocalSocketId);
}

/**

  Worker function to get the CPU index based on the package.

  @param  Package                Specific Package number.

  @return the CPU index ready to run.

**/
UINTN
GetCpuIndexValue (
  IN  UINT32                      Package
  )
{
  UINTN                          Index;
  EFI_STATUS                     Status;
  EFI_PROCESSOR_INFORMATION      ProcessorInfo;
  UINT64                         Data64;

  if (mSmmCpuServiceProtocolInterface == NULL) {
    Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, (VOID**) &mSmmCpuServiceProtocolInterface);
    ASSERT_EFI_ERROR (Status);
  }

  if (mSmmCpuProtocolInterface == NULL) {
    Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuProtocolGuid, NULL, (VOID **) &mSmmCpuProtocolInterface);
    ASSERT_EFI_ERROR (Status);
  }

  for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
    Status = mSmmCpuServiceProtocolInterface->GetProcessorInfo (mSmmCpuServiceProtocolInterface, Index, &ProcessorInfo);
    if (Status == EFI_NOT_FOUND) {
      continue;
    }
    ASSERT_EFI_ERROR (Status);

    Status = mSmmCpuProtocolInterface->ReadSaveState (mSmmCpuProtocolInterface, sizeof (UINT64),
                                                      EFI_SMM_SAVE_STATE_REGISTER_PROCESSOR_ID, Index, &Data64);
    if (Status == EFI_NOT_FOUND) {
      DEBUG ((EFI_D_ERROR, "UsraRegisterFilterLibSmm: skiping CPU index %d: not available for code execution in SMM", Index));
      continue;
    }
    ASSERT_EFI_ERROR (Status);

    if (ProcessorInfo.Location.Package == Package) {
        break;
    }
  }

  if (Index == gSmst->NumberOfCpus) {
    ASSERT (FALSE);
  }

  return Index;
}

/**

  This function to write CSR via MMIO mechanism.

  @param  CSR_MSR_ACCESS_INFO*       Pointer to the access information needed when access CSR.

  @return VOID

**/
RETURN_STATUS
EFIAPI
RegisterFilterWriteMmio (
  IN  USRA_ADDRESS            *Address,
  IN  UINTN                   AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  RegAlignedWrite((UINT32)Address->Attribute.AccessWidth, AlignedAddress, Buffer);
  return RETURN_SUCCESS;
}

/**
  Perform register filter read.

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be read out.
  @param[in]  AlignedAddress       An address to be read out.
  @param[out] Buffer               A pointer of buffer contains the data to be read out.

  @retval RETURN_SUCCESS           Register access is issued.
**/
RETURN_STATUS
EFIAPI
UsraRegisterFilterRead (
  IN  USRA_ADDRESS            *Address,
  IN  UINTN                   AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  if (Address->Attribute.AddrType == AddrTypePCIE || Address->Attribute.AddrType == AddrTypePCIEBLK) {
    return RETURN_UNSUPPORTED;
  }

  RegAlignedRead((UINT32)Address->Attribute.AccessWidth, AlignedAddress, Buffer);

  return RETURN_SUCCESS;
}
/**
  Perform register filter write.

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be read out.
  @param[in]  AlignedAddress       An address to be read out.
  @param[out] Buffer               A pointer of buffer contains the data to be read out.

  @retval RETURN_SUCCESS           Register access is issued.
**/
RETURN_STATUS
EFIAPI
UsraRegisterFilterWrite (
  IN  USRA_ADDRESS            *Address,
  IN  UINTN                   AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  if (Address->Attribute.AddrType == AddrTypeCSRMEM) {
    return RETURN_UNSUPPORTED;
  } else if (Address->Attribute.AddrType == AddrTypeCSRCFG) {
    return RETURN_UNSUPPORTED;
  } else if (Address->Attribute.AddrType == AddrTypePCIE || Address->Attribute.AddrType == AddrTypePCIEBLK) {
    return RETURN_UNSUPPORTED;
  }

  mRegisterFilterWritePtr[mFuncPtrIndex](Address, AlignedAddress, Buffer);

  return RETURN_SUCCESS;
}

/**
  The constructor function initializes the global variable.

  @param[in] ImageHandle To be added.
  @param[in] SystemTable To be added.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmmUsraRegisterFilterLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINTN   SpinLockSize;
  VOID    *SpinLockBuffer;

  //
  // Allocate page for SPIN LOCK buffer
  //
  SpinLockSize   = GetSpinLockProperties ();
  SpinLockBuffer = AllocatePages (EFI_SIZE_TO_PAGES (SpinLockSize * 2));
  mApCompleteCsrAccessSpinLock = (SPIN_LOCK *)((UINTN)SpinLockBuffer + SpinLockSize);

  InitializeSpinLock (mApCompleteCsrAccessSpinLock);

  return EFI_SUCCESS;
}
