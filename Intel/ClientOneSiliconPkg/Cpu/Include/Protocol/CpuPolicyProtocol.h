/** @file
  Protocol used for specifying platform related CPU information and policy setting.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

@par Specification
**/
#ifndef _CPU_POLICY_PROTOCOL_H_
#define _CPU_POLICY_PROTOCOL_H_

//
// DXE_CPU_POLICY_PROTOCOL revisions
//
#define DXE_CPU_POLICY_PROTOCOL_REVISION 5

extern EFI_GUID gDxeCpuPolicyProtocolGuid;

#pragma pack (push,1)

/**
  The protocol allows the platform code to publish a set of configuration information that the
  CPU drivers will use to configure the processor in the DXE phase.
  This Policy Protocol needs to be initialized for CPU configuration.
  @note The Protocol has to be published before processor DXE drivers are dispatched.

  This member specifies the revision of the Cpu Policy protocol. This field is used to indicate backward
  compatible changes to the protocol. Any such changes to this protocol will result in an update in the revision number.

  <b>Revision 1</b>:
   - Initial version
  <b>Revision 2</b>:
   - Added DgrEnable to obtain DGR status from setup option.
  <b>Revision 3</b>:
   - Add SmmMsrSaveStateEnable, SmmUseDelayIndication, SmmUseBlockIndication, SmmUseSmmEnableIndication, SmmProcTraceEnable.
  <b>Revision 4</b>:
   - Added DgrSpaEnable to obtain DGR-SPA status from setup option.
  <b>Revision 5</b>:
   - Added DgrStateSaveProtect to obtain DGR State Save status from setup option.
   **/
typedef struct {
  /**
  Policies to obtain CPU temperature.
   - <b>0: ACPI thermal management uses EC reported temperature values</b>.
   - 1: ACPI thermal management uses DTS SMM mechanism to obtain CPU temperature values.
   - 2: ACPI Thermal Management uses EC reported temperature values and DTS SMM is used to handle Out of Spec condition.
  **/
  UINT32                         EnableDts           : 2;
  /**
  Policy to obtain DGR status from setup option.
   - <b>0: DGR is disabled</b>.
   - 1: DGR is enabled.
  **/
  UINT32                         DgrEnable           : 1;
  /**
  Policy to obtain DGR-SPA status from setup option.
   - <b>0: DGR-SPA is disabled</b>.
   - 1: DGR-SPA is enabled.
  **/
  UINT32                         DgrSpaEnable        : 1;
  /**
  Policy to obtain DGR State Save support status from setup option.
   - <b>0: SMM can have Read/Write access to SMM State Save region </b>.
   - 1: SMM State Save region access is protected.
  **/
  UINT32                         DgrStateSaveProtect  : 1;
  UINT32                         RsvdBit              : 27;  ///< Reserved bits, align to multiple 32;

  UINT8                          Revision;                  ///< Current revision of policy.
  /**
  Indidates if SMM Save State saved in MSRs.
  if enabled, SMM Save State will use the MSRs instead of the memory.
   - <b>0: FALSE</b> - SMM Save State will use the memory.
   - 1: TRUE - SMM Save State will use the MSRs.
  **/
  UINT8                          SmmMsrSaveStateEnable;
  /**
  Indidates if SMM Delay feature is supported.
   - 0: FALSE - SMM Delay feature is not supported.
   - <b>1: TRUE</b> - SMM Delay feature is supported.
  **/
  UINT8                          SmmUseDelayIndication;
  /**
  Indidates if SMM Block feature is supported.
   - 0: FALSE - SMM Block feature is not supported.
   - <b>1: TRUE</b> - SMM Block feature is supported.
  **/
  UINT8                          SmmUseBlockIndication;
  /**
  Indidates if SMM Enable/Disable feature is supported.
   - 0: FALSE - SMM Enable/Disable feature is not supported.
   - <b>1: TRUE</b> - SMM Enable/Disable feature is supported.
  **/
  UINT8                          SmmUseSmmEnableIndication;
  /**
  Specifies if SMM Proccessor Trace will be Enabled.
   - <b>0: FALSE</b> - SMM Proccessor Trace will be disabled.<BR>
   - 1: TRUE - SMM Proccessor Trace will be enabled.<BR>
  **/
  UINT8                          SmmProcTraceEnable;
  UINT8                          ReservedByte[6];           ///< Reserved bytes, align to multiple 8.
} DXE_CPU_POLICY_PROTOCOL;

#pragma pack (pop)

#endif
