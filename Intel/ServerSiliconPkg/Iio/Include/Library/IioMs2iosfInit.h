/** @file
  IIO MS2IOSF misc initialization header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _IIOMS2IOSFINIT_H_
#define _IIOMS2IOSFINIT_H_

/**
 @brief This function disables decoding bank assigned to a root port.

 @param[in] IioGlobalData - Pointer to global IIO data structure
 @param[in] IioIndex - Index of IIO instance
 @param[in] IioPort  - IIO port index

 @return Standard EFI status is returned.
**/
EFI_STATUS
Ms2iosfBankDecoderDisablePciePort (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        IioPort);

/**
 This function updates bank decoder to decode as CXL.

 @param[in] IioGlobalData - Pointer to global IIO data structure
 @param[in] IioIndex      - Index of IIO instance
 @param[in] StackIndex    - IIO stack index

 @return Standard EFI status is returned.
**/
EFI_STATUS
Ms2iosfBankDecoderEnableCxl (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        StackIndex
  );

/**

  After resource allocation configure MS2IOSF for routing transactions.

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
Ms2iosfPostResources (
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex
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
ProgramMs2IosfBankDecoder (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

OverrideMs2iosfBankDecoder: This function configures overrides for bank decoders of MS2IOSF

@param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
@param IioIndex           - Index of CPU/IIO

@retval None

**/
VOID
OverrideMs2iosfBankDecoder (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

    ProgramMs2IosfCredits: Main function to program and override all MS2IOSF ITC/OTC credits

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramMs2IosfCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

    Trigger hw_init for credit programming to take effect.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
TriggerCreditsHwInit (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

  IIO Stack Initialize

  @param Phase         - Initialization phase (pre, init, post).
  @param IioGlobalData - Pointer to IIO global data/methods
  @param IioIndex      - Current socket.
  @param StackIndex    - Stack number being initialized.

**/
VOID
IioStackInit (
  IN  IIO_INIT_PHASE                 Phase,
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          StackIndex
  );

/**
  Update Trace Hub FW_BAR shadow register. Additional specific programming for 10nm wave 1/2.

  @param[in]  IioGlobalData       Pointer to IIO Universal Data Structure (UDS)
  @param[in]  IioIndex            Index with the changed IIO (Socket)
  @param[in]  StackIndex          Trace Hub stack number
  @param[in]  FwBar               Value of Trace Hub FW_BAR
**/
VOID
IioMs2iosfUpdateFwBarShadowReg (
  IN  IIO_GLOBALS        *IioGlobalData,
  IN  UINT8               IioIndex,
  IN  UINT8               StackIndex,
  IN  UINT64              FwBar
  );

/**
  Update OOB MSM BAR shadow register. Additional specific programming for 10nm wave 3.

  @param[in]  IioGlobalData       Pointer to IIO Universal Data Structure (UDS)
  @param[in]  IioIndex            Index with the changed IIO (Socket)
**/
VOID
IioMs2iosfUpdateOobMsmBarShadowReg (
  IN  IIO_GLOBALS        *IioGlobalData,
  IN  UINT8               IioIndex
  );

 /**
  @brief This function updates DMI Max Payload Size in MS2IOSF DMI bank DevCtl register.

  @param[in] IioGlobalPtr   - Pointer to global IIO data structure.
  @param[in] IioIndex       - Index of IIO instance.
  @param[in] MaxPayloadSize - Max Payload Size as defined for PCIe Device Control register.

  @return Standard EFI status is returned.
 **/
 EFI_STATUS
 IioMs2iosfUpdateDmiMps (
   IN IIO_GLOBALS              *IioGlobalPtr,
   IN UINT8                     IioIndex,
   IN UINT8                     MaxPayloadSize);

#endif /* _IIOMS2IOSFINIT_H_*/
