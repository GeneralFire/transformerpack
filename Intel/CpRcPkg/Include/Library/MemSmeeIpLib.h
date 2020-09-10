/** @file
  MemSmeeIpLib.h

  Api library to support SMEE related access.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef  _MEM_SMEE_IP_LIB_H_
#define  _MEM_SMEE_IP_LIB_H_

typedef struct {
  UINT32   Pmem1Base;            // Offset to be subtracted from persistent memory 1 system address.
  UINT32   Pmem1Limit;           // Persistent memory 1 range limit in system addresses.
  UINT32   Pmem2Base;            // Offset to be subtracted from persistent memory 2 system address.
  UINT32   Pmem2Limit;           // Persistent memory 2 range limit in system addresses.
} MKTME_PMEM_DATA;

/**
  Program TME CMI registers

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] ProgramableValue  - Programmable CMI credit value

  @retval N/A
**/
VOID
EFIAPI
ProgramTmeCmiRegisters (
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   ProgramableValue
  );

/**
  Check if TME enabled in given Socket

  @param[in] Socket       - Socket number, 0 based

  @retval TRUE -  TME is enabled
  @retval FALSE - TME is disabled
**/
BOOLEAN
EFIAPI
IsTmeEnabled (
  IN UINT8 Socket
  );

/**
  Program config done bit in TME CMI registers

  @param[in] Socket       - Socket number
  @param[in] McId         - Memory Controller number

  @retval N/A
**/
VOID
EFIAPI
ProgramTmeCmiConfigDoneAllCh (
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**
  Wait for Initialized bit set for TME CMI registers. The CMI initialized
  bit will be polled till CATCHALL_TIMEOUT = 100 milliseconds. If bit dint get
  set then its a fatal condition and fatal error will be reported by this function.

  @param[in] Socket       - Socket number
  @param[in] McId         - Memory Controller number

  @retval N/A
**/
VOID
EFIAPI
WaitForTmeCmiConfigCompleteAllCh (
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**
  Enable PRMRR Security

  @param[in] Socket       - Socket number
  @param[in] ChId         - Channel Number

  @retval N/A
**/
VOID
EFIAPI
EnablePrmrrSecurity (
  IN UINT8 Socket,
  IN UINT8 ChId
  );

/**
  This function programs the MK-TME related CSRs with the range of AppDirect and/or AppDirect-WB regions.

  @param[in] ScktId             Socket index.
  @param[in] McId               Memory controller index.
  @param[in] ChId               Channel index on memory controller.
  @param[in] MktmePmemData      The base and limit of the AppDirect and/or AppDirect-WB regions.

  @retval N/A
**/
VOID
EFIAPI
SmeeSetMktmePmemData (
  IN UINT8            ScktId,
  IN UINT8            McId,
  IN UINT8            ChId,
  IN MKTME_PMEM_DATA  *MktmePmemData
  );

#endif // _MEM_SMEE_IP_LIB_H_
