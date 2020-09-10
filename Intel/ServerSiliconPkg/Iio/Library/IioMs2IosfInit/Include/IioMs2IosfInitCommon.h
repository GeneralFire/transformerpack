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

#ifndef _MS2IOSF_INIT_COMMON_H_
#define _MS2IOSF_INIT_COMMON_H_

#include <IioRegs.h>

/**
 @brief Read register described by Bank Decoder Table entry.

 @param[in] IioGlobalPtr - Pointer to IIO global data structure
 @param[in] IioIndex     - Index of IIO instance
 @param[in] PciBus       - PCI bus (IIO stack) to ready from MS2IOSF device
 @param[in] RegSize      - Size of the register to read
 @param[in] RegAddr      - Address to read (offset in MS2IOSF device config space)

 @return Register value is returned.
**/
UINT32
Ms2iosfRegRead (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PciBus,
  IN UINT8        RegSize,
  IN UINT32       RegAddr
  );

/**
 @brief Write register described by Bank Decoder Table entry.

 @param[in] IioGlobalPtr - Pointer to IIO global data structure
 @param[in] IioIndex     - Index of IIO instance
 @param[in] PciBus       - PCI bus (IIO stack) to ready from MS2IOSF device
 @param[in] RegSize      - Size of the register to read
 @param[in] RegAddr      - Address to read (offset in MS2IOSF device config space)
**/
VOID
Ms2iosfRegWrite (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PciBus,
  IN UINT8        RegSize,
  IN UINT32       RegAddr,
  IN UINT32       Data
  );

/**

  Apply silicon permanent workaround for disabled MS2IOSF Type 1 bank decoder.

  @param IioIndex           - index to IIO (socket number)
  @param StackIndex         - Stack index value of CPU/IIO
  @param BusNumber          - Bus number for the given stack
  @param BankNumber         - Bank number being programmed
  @param RegisterAddress    - Bank register address being accessed
  @param DataPtr            - Current data value that will be programmed by ProgramMs2IosfBankDecoder function

**/
VOID
DisableMs2IosfType1BankDecoder (
  IN UINT8        IioIndex,
  IN UINT8        StackIndex,
  IN UINT8        BusNumber,
  IN UINT8        BankNumber,
  IN UINT32       RegisterAddress,
  IN OUT UINT32  *DataPtr
  );

/**

  Program MS2IOSF bank decoder shadowed BAR

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
OverrideMs2iosfShadowedBar (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

  Program MS2IOSF bank decoder Device Control

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
OverrideMs2iosfBankDevCtrl (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

  MS2IOSF bank decoder EOI broadcast message

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
OverrideMs2iosfEoiBroadcast (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

  Enable/Disable which MS2IOSF decoder banks should be the target of broadcast MCTP messages

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
OverrideMs2iosfMctpBroadcast (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

OverrideMs2iosfBarSzForNtb:      This function re-configures BAR size for banks for NTB ports

@param IioGlobalData           - Pointer to IIO Universal Data Structure (UDS)
@param IioIndex                - Index of CPU/IIO

@retval None

**/
VOID
OverrideMs2iosfBarSzForNTB (
  IIO_GLOBALS                    *IioGlobalData,
  UINT8                          IioIndex
  );

/**

    ProgramItcCredits: This function programs all MS2IOSF ITC credits
                       based on the StackID.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramItcCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

    ProgramOtcCredits: This function programs all MS2IOSF OTC credits
                       based on the StackID.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramOtcCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

    Program and override all MS2IOSF related registers needed before Hardware init trigger

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramMs2IosfCreditsMisc (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

  Program MS2IOSF bank decoder shadowed Bus number

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

**/
VOID
OverrideMs2iosfShadowedBus (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**
    ProgramMs2IosfBankDecoder: This function programs all MS2IOSF bank decoders
                               based on the StackID, so all the transactions can be
                               routed as required.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramMs2IosfBankDecoderBanks (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

#endif // _MS2IOSF_INIT_COMMON_H_
