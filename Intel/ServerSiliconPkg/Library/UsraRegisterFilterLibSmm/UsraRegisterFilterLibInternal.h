/** @file
  Implementation of UsraRegisterFilterLib class library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/SynchronizationLib.h>
#include <Protocol/IioUds.h>
#include <Library/UsraRegisterFilterLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmCpuService.h>
#include <UncoreCommonIncludes.h>
#include <CpuPciAccess.h>
#include "Library/SystemInfoLib.h"

#define CPUID_TOPO_INFO                       0x0b
#define IIO_DFX             0
#define  IIO_DFX_DMI         1
#define  IIO_DFX_GLOBAL      2
#define  IIO_DFX_GLOBAL_DMI  3
#define  IIO_DFX_VTD         4
#define  IIO_DFX_VTD_DMI     5
//
// CSR MSR ACCESS Info structure definition
//
typedef struct {
  UINT32       CsrMsrAddr;
  UINT32       CsrMsrData;
  } CSR_MSR_ACCESS_INFO;


typedef
  RETURN_STATUS
  (EFIAPI *REGISTER_FILTER_WRITE) (
    IN  USRA_ADDRESS*,
    IN  UINTN,
    OUT VOID*
   );
/**

  This function to write CSR via MSR mechanism.

  @param  CSR_MSR_ACCESS_INFO*       Pointer to the access information needed when access CSR.

  @return VOID

**/
VOID
EFIAPI
WriteCsrViaMsr (
  IN  CSR_MSR_ACCESS_INFO*     CsrMsrAccessInfo
  );

/**

  This function to write CSR via MSR mechanism from AP.

  @param  CSR_MSR_ACCESS_INFO*       Pointer to the access information needed when access CSR.

  @return VOID

**/
VOID
EFIAPI
WriteCsrViaMsrBlocking (
  IN  CSR_MSR_ACCESS_INFO*     CsrMsrAccessInfo
  );

/**
  This function returns Bus Stack Number

  @param[in] USRA_ADDRESS         USRA Address

  @retval UINT8                   Return Bus stack Number.
**/
UINT8
GetBusStackNumber (
  IN  USRA_ADDRESS             *Address
  );

/**

  This function to get the CPU index based on the package and Local ApicId info.

  @param  Package                Specific Package number.
  @param  Package                Local ApidId number.

  @return the CPU index ready to run.

**/
UINTN
GetCpuIndexValue (
  IN  UINT32                      Package
  );

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
  );

/**
  Perform register write via MSR

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be written.
  @param[in]  AlignedAddress       An address to be written.
  @param[in]  Buffer               A pointer of buffer contains the data to be written.

  @retval RETURN_SUCCESS           Register access is issued.
**/
RETURN_STATUS
EFIAPI
RegisterFilterWriteMsr (
  IN  USRA_ADDRESS             *Address,
  IN  UINTN                    AlignedAddress,
  IN  VOID                     *Buffer
  );

/**
Get SocketID for currently executing thread

@param[in out] LocalSocket          Pointer to LocalSocket Buffer

@return VOID
**/
VOID
GetLocalSocketId(
    OUT UINT8 *LocalSocket
);

/**
Finding Local Socket is equal to Target Socket
Local Socket = Currently executing Socket
Target Socket = Socket that CSR write is targeting

@param [in] TargetSocket           Socket that CSR write is targeting

@return TRUE                       If executing thread is part of the socket
**/

BOOLEAN
IsExecutingOnLocalSocket(
    UINT8 TargetSocket
);
