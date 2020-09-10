/** @file
  Register names for EBG PCH LPC/eSPI device

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
    Register that is specific to -N denoted by "_PCH_N_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#ifndef _PCH_REGS_LPC_EBG_H_
#define _PCH_REGS_LPC_EBG_H_

#define V_LPC_CFG_DID_EBG                         0x1B80

#define EBG_PCH_MIN_SUPPORTED_STEPPING            PCH_A0

//
// Ebg LPC Device ID's
//
#define V_EBG_LPC_CFG_DEVICE_ID_1B80              0x1B80          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B81              0x1B81          ///< EBG A0 LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B82              0x1B82          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B83              0x1B83          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B84              0x1B84          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B85              0x1B85          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B86              0x1B86          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B87              0x1B87          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B88              0x1B88          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B89              0x1B89          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B8A              0x1B8A          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B8B              0x1B8B          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B8C              0x1B8C          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B8D              0x1B8D          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B8E              0x1B8E          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B8F              0x1B8F          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B90              0x1B90          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B91              0x1B91          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B92              0x1B92          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B93              0x1B93          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B94              0x1B94          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B95              0x1B95          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B96              0x1B96          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B97              0x1B97          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B98              0x1B98          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B99              0x1B99          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B9A              0x1B9A          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B9B              0x1B9B          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B9C              0x1B9C          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B9D              0x1B9D          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B9E              0x1B9E          ///< EBG LPC Device ID
#define V_EBG_LPC_CFG_DEVICE_ID_1B9F              0x1B9F          ///< EBG LPC Device ID

#endif //_PCH_REGS_LPC_EBG_H_
