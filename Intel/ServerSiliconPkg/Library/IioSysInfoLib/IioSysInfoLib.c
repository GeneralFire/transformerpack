/** @file

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
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <IioRegs.h>
#include <IioSysInfoTable.h>
#include <Cpu/CpuIds.h>


#define Is14nmCpu() (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL))


/**

  Get max port number per socket

  @retval    Max ports per socket
**/
UINT8
EFIAPI
GetMaxPortNumPerSocket (
  VOID
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0;
  } else {
    return IioSysInfoTable->MaxPortNumPerSocket;
  }
}

/**

  Get max IOU number per socket

  @retval    Max IOU number per socket
**/
UINT8
EFIAPI
GetMaxIouNumPerSocket (
  VOID
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0;
  } else {
    return IioSysInfoTable->MaxIouNumPerSocket;
  }
}


/**
  Get max port number per IOU

  @retval    Max ports per IOU
**/
UINT8
EFIAPI
GetMaxPortNumPerIou (
  VOID
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0;
  } else {
    return IioSysInfoTable->MaxPortNumPerIou;
  }
}

/**

  Get max NTB port number per socket

  @retval    Max NTB ports per socket
**/
UINT8
EFIAPI
GetMaxNtbNumPerSocket (
  VOID
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0;
  } else {
    return IioSysInfoTable->MaxNtbNumPerSocket;
  }
}

/**

  Get max stack number per socket

  @retval    Max stacks per socket
**/
UINT8
EFIAPI
GetMaxStackNumPerSocket (
  VOID
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0;
  } else {
    return IioSysInfoTable->MaxStackNumPerSocket;
  }
}

/**

  Get cluster port index by NTB index

  @param     NtbIndex - NTB index
  @retval    Cluster port Index
**/
UINT8
EFIAPI
GetClusterPortByNtbIndex (
  IN UINT8    NtbIndex
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0xFF;
  } else {
    return IioSysInfoTable->ClusterPortByNtbIndex[NtbIndex];
  }
}

/**

  Get stack index by IOU index

  @param     IouIndex - IOU index
  @retval    Stack Index
**/
UINT8
EFIAPI
GetStackIndexByIouIndex (
  IN UINT8    IouIndex
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0xFF;
  } else {
    return IioSysInfoTable->StackIndexByIouIndex[IouIndex];
  }
}

/**

  Get max port number per stack

  @param     StackIndex - Stack index
  @retval    Max port number
**/
UINT8
EFIAPI
GetMaxPortNumPerStack (
  IN UINT8    StackIndex
  )
{
  EFI_STATUS       Status;
  IIO_SYS_INFO    *IioSysInfoTable;

  Status = GetIioSysInfoTable (&IioSysInfoTable);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return 0;
  } else {
    return IioSysInfoTable->MaxPortNumByStackIndex[StackIndex];
  }
}


/**
    Convert port index in a stack to index in a socket

    @param StackIndex        - index of the stack
    @param PortIndexInStack  - index of the port in stack

    @retval port index in a socket
**/
UINT8
EFIAPI
GetPortIndexbyStack (
  IN UINT8    StackIndex,
  IN UINT8    PortIndexInStack
  )
{
  UINT8  Port = 0;

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    PCIESTACKPORTINDEXSPR (PortIndexInStack, StackIndex, Port);
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    PCIESTACKPORTINDEXSNR(PortIndexInStack, StackIndex, Port);
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    PCIESTACKPORTINDEXICXD(PortIndexInStack, StackIndex, Port);
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    PCIESTACKPORTINDEX10NMR (PortIndexInStack, StackIndex, Port);
  } else if (Is14nmCpu ()) {
    PCIESTACKPORTINDEX (PortIndexInStack, StackIndex, Port);
  } else {
    PCIESTACKPORTINDEX10NM (PortIndexInStack, StackIndex, Port);
  }
  return Port;
}


/**
 @brief Convert IIO port index to a stack and port in the stack

 @param[in]  PortIdx  - Port index in IIO instance (usually socket)
 @param[out] StackPtr - Optional pointer to buffer for stack index.

 @return Index of port in its stack is returned, or 0xFF if illegal PortIndex was provided.
**/
UINT8
EFIAPI
IioPortIndexToStackPort (
  IN UINT8   PortIdx,
  IN UINT8  *StackPtr
  )
{
  UINT8  Stack = (UINT8)-1;
  UINT8  Port = (UINT8)-1;

  if (PortIdx < GetMaxPortNumPerSocket ()) {

    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {

      Stack = PORTINDEX_TO_STACK_SPR (PortIdx);
      Port = PORTINDEX_TO_PORT_SPR (PortIdx);

    } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
      Stack = PORTINDEX_TO_STACK_SNR (PortIdx);
      Port = PORTINDEX_TO_PORT_SNR (PortIdx);
    } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
      Stack = PORTINDEX_TO_STACK_ICXD (PortIdx);
      Port = PORTINDEX_TO_PORT_ICXD (PortIdx);
    } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      Stack = PORTINDEX_TO_STACK_10NMR (PortIdx);
      Port = PORTINDEX_TO_PORT_10NMR (PortIdx);
    } else if (Is14nmCpu ()) {
      Stack = PORTINDEX_TO_STACK (PortIdx);
      Port = PORTINDEX_TO_PORT (PortIdx);
    } else {
      Stack = PORTINDEX_TO_STACK_10NM (PortIdx);
      Port = PORTINDEX_TO_PORT_10NM (PortIdx);
    }
    if (StackPtr != NULL) {
      *StackPtr = Stack;
    }
  }
  return Port;
}


/**
  Check if Port may be configured as NTB on given port number and set Ntb Port Index

  @param[in]   PortIndex         - Port Index
  @param[out]  NtbPortIndex      - pointer to NtbPortPerPortIndex

  @retval                        - EFI_UNSUPPORTED: Ntb is not supported on given PortIndex
                                   EFI_SUCCESS:     function is excuted successfully
**/
EFI_STATUS
GetNtbPortPerPortIndex (
  IN     UINT8                   PortIndex,
     OUT UINT8                   *NtbPortIndex
  )
{
  *NtbPortIndex = 0;

  if (Is14nmCpu ()) {
    switch(PortIndex) {
      case PORT_1A_INDEX:
        *NtbPortIndex = 0;
        break;
      case PORT_2A_INDEX:
        *NtbPortIndex = 1;
        break;
      case PORT_3A_INDEX:
        if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
          return EFI_UNSUPPORTED;
        } else {
          *NtbPortIndex = 2;
        }
        break;
      case PORT_4A_INDEX:
        if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
          *NtbPortIndex = 2;
        } else {
          return EFI_UNSUPPORTED;
        }
        break;
      default:
        return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;

  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    switch(PortIndex) {
      case PORT_1A_INDEX:
        *NtbPortIndex = 0;
        break;
      case PORT_2A_INDEX:
        *NtbPortIndex = 1;
        break;
      case PORT_3A_INDEX:
        *NtbPortIndex = 2;
        break;
      case PORT_4A_INDEX:
        *NtbPortIndex = 3;
        break;
      case PORT_5A_INDEX:
        *NtbPortIndex = 4;
        break;
      default:
        return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;

  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    switch(PortIndex) {
      case PORT_1H_INDEX_SPR:
        *NtbPortIndex = 0;
        break;
      case PORT_2H_INDEX_SPR:
        *NtbPortIndex = 1;
        break;
      case PORT_3H_INDEX_SPR:
        *NtbPortIndex = 2;
        break;
      case PORT_4H_INDEX_SPR:
        *NtbPortIndex = 3;
        break;
      case PORT_5H_INDEX_SPR:
        *NtbPortIndex = 4;
        break;
      default:
        return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;

  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    switch (PortIndex) {
      case PORT_1A_INDEX:
        *NtbPortIndex = 0;
        break;
      case PORT_2A_INDEX:
        *NtbPortIndex = 1;
        break;
      default:
        return EFI_UNSUPPORTED;
    }
  return EFI_SUCCESS;

  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    switch (PortIndex) {
      case PORT_1A_INDEX:
        *NtbPortIndex = 0;
        break;
      default:
        return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

/**
  Check if Device's function 0 is VMD dummy function

  Specific Device ID (0x09AB in VMD specification v3.0) indicates that function is VMD dummy function.

  @param DevId           - Device ID

  @retval TRUE           - Function is VMD dummy function
  @retval FALSE          - Function is not VMD dummy function

**/
BOOLEAN
IsVmdDummyFunction (
  IN  UINT16       DevId
  )
{
  if (DevId == V_VMD_DUMMY_DID) {
    return TRUE;
  }
  return FALSE;
}

/**
  Returns default value of Max Payload Size for different projects.

  @retval 8-bit default value indicating Max Payload Size
**/
UINT8
GetDefaultMaxPayloadSize (
  VOID
  )
{
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) ||IsCpuAndRevision (CPU_TNR, REV_ALL) || (Is14nmCpu ())) {
    return PCIE_MAX_PAYLOAD_256B;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return PCIE_MAX_PAYLOAD_512B;
  } else {
    ASSERT (FALSE);
    return 0;
  }
}
