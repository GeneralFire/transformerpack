/** @file
  Header file for PeiAceInitLib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _PEI_ACE_INIT_LIB_H_
#define _PEI_ACE_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/PeiHdaLib.h>

/**
  Initialize the Intel High Definition Audio Codec(s) present in the system.
  For each codec, a predefined codec verb table should be programmed.
  The list contains 32-bit verbs to be sent to the corresponding codec.
  If it is not programmed, the codec uses the default verb table, which may or may not
  correspond to the platform jack information.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ABORTED             Controller in reset - unable to proceed with initialization
  @retval EFI_INVALID_PARAMETER   Provided VerbTableData is null
**/
EFI_STATUS
AceConfigureHdAudioCodec (
  IN HDA_HANDLE                *HdaHandle
  );

/**
  Triggers ACE initialization or disables ACE depending on HdaPreMem configuration.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure
  @param[in, out] XtalValue       Pointer to Xtal value
  @param[in] InterruptPin         Interrupt pin number

  @retval EFI_SUCCESS             Controller disabled successfully
**/
EFI_STATUS
AceConfigureHdAudio (
  IN HDA_HANDLE                *HdaHandle,
  IN OUT UINT32                *XtalValue,
  IN UINT8                     InterruptPin
  );

#endif
