/** @file
  Definition of the NVDIMM SMBUS SMM area protocol.  This protocol
  publishes the address and format of a NVDIMM Inteface used as a communications
  buffer between SMM code and NON SMM code.
  Note:  Data structures defined in this protocol are not naturally aligned.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2018 Intel Corporation. <BR>

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

#ifndef _EFI_NVDIMM_SMBUS_SMM_PROTOCOL_H_
#define _EFI_NVDIMM_SMBUS_SMM_PROTOCOL_H_

#ifdef ICX_HOST
#ifndef NUM_PAYLOAD_REG
#define NUM_PAYLOAD_REG 32
#endif
#endif

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiNvdimmSmbusSmmInterfaceProtocolGuid;

//
// NVDIMM SMBUS SMM Interface Protocol Revisions
//
#ifdef ICX_HOST
#define EFI_NVDIMM_SMBUS_SMM_INTERFACE_VERSION_1_0 0x2
#else
#define EFI_NVDIMM_SMBUS_SMM_INTERFACE_VERSION_1_0 0x1
#endif
//
// NVDIMM SMBUS SMM Interface Protocol Request types
//
typedef enum {
  SET_TSOD_POLLING_STATE = 0,
#ifdef ICX_HOST
  FIS_REQUEST            = 1,
  GET_BSR                = 2,
#endif
  REQ_TYPE_MAX           = 0xFF
} NVDIMM_SMBUS_SMM_REQ_TYPE;

#pragma pack (1)
typedef union _NVDIMM_SMBUS_SMM_REQ_PARAMS {
  struct {
    UINT8            Socket;
    UINT8            Imc;
  } SetTsodPollState;
#ifdef ICX_HOST
  struct {
    UINT8     Socket;
    UINT8     Ch;
    UINT8     Dimm;
  } FisRequest;
  struct {
    UINT8     Socket;
    UINT8     Ch;
    UINT8     Dimm;
  } GetBsr;
#endif
} NVDIMM_SMBUS_SMM_REQ_PARAMS;

typedef union _NVDIMM_SMBUS_SMM_REQ_DATA {
  struct {
    UINT8     PollState;
  } SetTsodPollState;
#ifdef ICX_HOST
  struct {
    UINT32    InputPayload[NUM_PAYLOAD_REG];
    UINT32    OutputPayload[NUM_PAYLOAD_REG];
    UINT32    Opcode;
    UINT64    Timeout;
    UINT8     MbStatus;
  } FisRequest;
  struct {
    UINT64    *BootStatusRegister;
  } GetBsr;
  UINT16      Reserved;
#endif
} NVDIMM_SMBUS_SMM_REQ_DATA;

typedef struct _NVDIMM_SMBUS_SMM_INTERFACE {
  NVDIMM_SMBUS_SMM_REQ_TYPE   RequestType;
  NVDIMM_SMBUS_SMM_REQ_PARAMS RequestParams;
  NVDIMM_SMBUS_SMM_REQ_DATA   RequestData;
  EFI_STATUS                  RequestStatus;
} NVDIMM_SMBUS_SMM_INTERFACE, *PNVDIMM_SMBUS_SMM_INTERFACE;
#pragma pack ()

typedef struct _EFI_NVDIMM_SMBUS_SMM_INTERFACE_PROTOCOL {
  UINT8                     Revision;
  UINT8                     Reserved[7];
  NVDIMM_SMBUS_SMM_INTERFACE *Area;
} EFI_NVDIMM_SMBUS_SMM_INTERFACE_PROTOCOL;

#endif // _EFI_NVDIMM_SMBUS_SMM_PROTOCOL_H_
