/** @file
  MrcHooksServicesPpi.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _MRC_HOOKS_SERVICES_PPI_H_
#define _MRC_HOOKS_SERVICES_PPI_H_

#include <ReferenceCodeDataTypes.h>

///
/// PPI revision information
/// This PPI will be extended in a backwards compatible manner over time
/// Added interfaces should be documented here with the revisions added
/// Revision 1:  Initial revision
/// Revision 2:  Added SmbSocket field to SMB_ADDRESS_STRUCT (within SMB_DEVICE_STRUCT)
///              - decreased controller bit field to 1 bit
///              - SmbSocket field is 3 bits
///              - Removed Socket parameter from ReadProcSmb, WriteProcSmb (Socket number
///                  is now included in SMB_ADDRESS_STRUCT (SmbSocket)
///
#define MRC_HOOKS_SERVICES_PPI_REVISION  3

typedef struct _MRC_HOOKS_SERVICES_PPI MRC_HOOKS_SERVICES_PPI;
extern EFI_GUID gMrcHooksServicesPpiGuid;

typedef
UINT32
(EFIAPI *INITIALIZE_USB2_DEBUG_PORT) (
  SYSHOST  *Host
  );

typedef
UINT32
(EFIAPI *DISCOVER_USB2_DEBUG_PORT_API) (
  SYSHOST  *Host
  );

typedef
UINT32
(EFIAPI *RC_VPRINTF) (
  SYSHOST     *Host,
  CONST CHAR8 *Format,
  CHAR8       *Marker
  );

typedef
VOID
(EFIAPI *WRITE_MSR_PIPE) (
  UINT8          Socket,
  UINT32         Msr,
  UINT64_STRUCT  MsrReg
  );

typedef
UINT32
(EFIAPI *SET_PROMOTE_WARNING_EXCEPTION) (
  UINT8    MajorCode,
  UINT8    MinorCode
  );

typedef
UINT32
(EFIAPI *CLEAR_PROMOTE_WARNING_EXCEPTION) (
  UINT8    MajorCode,
  UINT8    MinorCode
  );
  
typedef
EFI_STATUS
(EFIAPI *READ_PROC_SMB) (
  SMB_DEVICE_STRUCT Device,
  UINT8             ByteOffset,
  volatile UINT8    *Data
  );

typedef
EFI_STATUS
(EFIAPI *WRITE_PROC_SMB) (
  SMB_DEVICE_STRUCT Device,
  UINT8             ByteOffset,
  UINT8             *Data
  );

typedef
VOID
(EFIAPI *OUTPUT_CHECKPOINT) (
  UINT8               MajorCode,
  UINT8               MinorCode,
  UINT16              Data
  );

typedef
VOID
(EFIAPI *LOG_WARNING) (
  UINT8           MajorCode,
  UINT8           MinorCode,
  UINT32          LogData
  );

typedef
VOID
(EFIAPI *OUTPUT_WARNING) (
  UINT8           MajorCode,
  UINT8           MinorCode,
  UINT8           socket,
  UINT8           ch,
  UINT8           dimm,
  UINT8           rank
  );

typedef
EFI_STATUS
(EFIAPI *READ_SPD) (
  UINT8            Socket,
  UINT8            Channel,
  UINT8            Dimm,
  UINT16           ByteOffset,
  UINT8            *Data
  );

struct _MRC_HOOKS_SERVICES_PPI {
  //
  // This member specifies the revision of the MRC_HOOKS_SERVICES_PPI. This field is used to
  // indicate backwards compatible changes to the PPI. Platform code that produces
  // this PPI must fill with the correct revision value for MRC code
  // to correctly interpret the content of the PPI fields.
  //
  UINT32                                Revision;
  INITIALIZE_USB2_DEBUG_PORT            InitializeUsb2DebugPort;
  DISCOVER_USB2_DEBUG_PORT_API          DiscoverUsb2DebugPort;
  RC_VPRINTF                            rcVprintf;
  WRITE_MSR_PIPE                        WriteMsrPipe;
  SET_PROMOTE_WARNING_EXCEPTION         SetPromoteWarningException;
  CLEAR_PROMOTE_WARNING_EXCEPTION       ClearPromoteWarningException;  
  READ_PROC_SMB                         ReadSmb;
  WRITE_PROC_SMB                        WriteSmb;
  OUTPUT_CHECKPOINT                     OutputCheckpoint;
  LOG_WARNING                           LogWarning;
  OUTPUT_WARNING                        OutputWarning;
  READ_SPD                              ReadSpd;
};

#endif  //_MRC_HOOKS_SERVICES_PPI_
