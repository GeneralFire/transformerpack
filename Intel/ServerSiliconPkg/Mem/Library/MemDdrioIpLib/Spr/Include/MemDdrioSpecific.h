/** @file
  MemDdrioSpecific.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_DDRIO_SPECIFIC_H_
#define _MEM_DDRIO_SPECIFIC_H_

#include <ReferenceCodeDataTypes.h>

#pragma pack(1)
typedef struct {
  UINT8 Signal;
  UINT8 RegisterIndex;
} PinRegister;
#pragma pack()


typedef enum {
  PinSubChA = SUB_CH_A,
  PinSubChB = SUB_CH_B,
  PinSubChMax,
} PinSubCh;

/**
  Gets the silicon register index from the signal name

  @param[in]     Host:      Pointer to SYSHOST
  @param[in]     Socket:    Socket number
  @param[in]     Ch:        Channel number
  @param[in]     Signal:    Enumerated signal name
  @param[in,out] SubCh:     Sub Channel Number
  @param[in,out] RegIndex:  Register Index

  @retval MRC_STATUS

**/
MRC_STATUS
SignalToRegIndex (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Ch,
  IN GSM_CSN               Signal,
  IN OUT UINT8             *SubCh,
  IN OUT UINT8             *RegIndex
  );

/**
  Get the Pin Register structure for given Side

  @param[in]  Host         Pointer to SYSHOST
  @param[in]  Socket       Socket number
  @param[in]  Ch           Channel number
  @param[in]  SubCh        Either SUB_CH_A or SUB_CH_B
  @param[out] Signal       Pointer to the Signal Structure (If SignalType is valid)
  @param[out] Size         Size of structure (If SignalType is valid)

  retval MRC_SUCCESS - Value returned successfully.
  retval MRC_STATUS_FAILURE - Failed to return structure pointer(Wrong Input parameter).
**/
MRC_STATUS
GetPinRegisterValue (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN  PinSubCh      SubCh,
  OUT PinRegister   **Signal,
  OUT UINT8         *Size
  );

/**

  This function updates sub-channel number,
  taking DDRIO lane reversal into consideration.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       SubCh       - Sub channel number

  @retval CC Register Address

**/
UINT8
EFIAPI
UpdateDdrIoCcRegisterSubCh (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh
  );
#endif //#define _MEM_DDRIO_SPECIFIC_H_

