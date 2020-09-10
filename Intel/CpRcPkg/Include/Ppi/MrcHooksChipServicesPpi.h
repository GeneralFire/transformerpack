/** @file
  MrcHooksChipServicesPpi.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _MRC_HOOKS_CHIP_SERVICES_PPI_H_
#define _MRC_HOOKS_CHIP_SERVICES_PPI_H_

#include <Library/Mesh2MemIpLib.h>
#include <Upi/KtiHost.h>
///
/// PPI revision information
/// This PPI will be extended in a backwards compatible manner over time
/// Added interfaces should be documented here with the revisions added
/// Revision 1:  Initial revision
///
#define MRC_HOOKS_CHIP_SERVICES_PPI_REVISION  1

typedef struct _MRC_HOOKS_CHIP_SERVICES_PPI MRC_HOOKS_CHIP_SERVICES_PPI;
extern EFI_GUID gMrcHooksChipServicesPpiGuid;

typedef
UINT32
(EFIAPI *USRA_CSR_READ) (
  UINT8    SocId,
  UINT8    BoxInstance,
  UINT32   Offset
  );

typedef
VOID
(EFIAPI *GET_SET_M2M_SAI_REGISTERS) (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN M2M_SAI_REG_ENUM    SaiRegIndex,
  IN BOOLEAN             Get,
  IN OUT UINT32          *Value
  );

typedef
VOID
(EFIAPI *SET_M2M_PRMRR_MASK) (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PrmrrMaskLo,
  IN UINT32              PrmrrMaskHi
  );

typedef
VOID
(EFIAPI *SET_M2M_PRMRR_BASE) (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PrmrrBaseLo,
  IN UINT32              PrmrrBaseHi
  );

typedef
VOID
(EFIAPI *USRA_CSR_WRITE) (
  UINT8    SocId,
  UINT8    BoxInstance,
  UINT32   Offset,
  UINT32   Data
  );

typedef
KTI_STATUS
(EFIAPI *SEND_MAIL_BOX_CMD_TO_PCODE) (
  UINT8               Cpu,
  UINT32              Command,
  UINT32              Data
  );

typedef
VOID
(EFIAPI *FATAL_ERROR) (
  UINT8           MajorCode,
  UINT8           MinorCode
  );

typedef
VOID
(EFIAPI *DEBUG_PRINT_MEM_FUNCTION) (
  SYSHOST    *Host,
  UINT32     DbgLevel,
  UINT8      Socket,
  UINT8      Channel,
  UINT8      Dimm,
  UINT8      Rank,
  UINT8      Strobe,
  UINT8      Bit,
  CHAR8      *Format,
  CHAR8      *Marker
  );

struct _MRC_HOOKS_CHIP_SERVICES_PPI {
  //
  // This member specifies the revision of the MRC_HOOKS_CHIP_SERVICES_PPI. This field is used to
  // indicate backwards compatible changes to the PPI. Platform code that produces
  // this PPI must fill with the correct revision value for MRC code
  // to correctly interpret the content of the PPI fields.
  //
  UINT32                                Revision;
  USRA_CSR_READ                         UsraCsrRead;
  USRA_CSR_WRITE                        UsraCsrWrite;
  SEND_MAIL_BOX_CMD_TO_PCODE            SendMailBoxCmdToPcode;
  FATAL_ERROR                           FatalError;
  DEBUG_PRINT_MEM_FUNCTION              DebugPrintMemFunction;
  GET_SET_M2M_SAI_REGISTERS             GetSetM2mSaiRegisters;
  SET_M2M_PRMRR_MASK                    SetM2mPrmrrMask;
  SET_M2M_PRMRR_BASE                    SetM2mPrmrrBase;
};

#endif  //_MRC_HOOKS_CHIP_SERVICES_PPI_
