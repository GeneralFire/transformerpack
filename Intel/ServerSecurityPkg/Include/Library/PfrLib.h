/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _PFR_LIB_H_
#define _PFR_LIB_H_

#include "PfrHw.h"

//-----------------------------------------------------------------------------------------------------
#pragma pack (push, 1)
//-----------------------------------------------------------------------------------------------------
//-------------	FUNCTION PROTOTYPES -------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
/**
  ReadCpldRegister(): Read the requested CPLD register and store the data byte in given area.

  @param  Socket    INPUT: UINT8, Processor Socket ID
          Register  INPUT: UINT8, CPLD register to read
          Data      INPUT/OUTPUT: UINT8*, Ptr to area to store the data

  @retval Status    EFI_SUCCESS  Success
                    Other        Error
**/
EFI_STATUS
ReadCpldRegister(
  IN     UINT8 Socket,
  IN     UINT8 Register,
  IN OUT UINT8 *Data
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  WriteCpldRegister(): Write the given data byte to the requested CPLD register.

  @param  Socket    INPUT: UINT8, Processor Socket ID
          Register  INPUT: UINT8, CPLD register to write
          Data      INPUT: UINT8, Data byte to be written

  @retval Status    EFI_SUCCESS  Success
                    Other        Error
**/
EFI_STATUS
WriteCpldRegister(
  IN  UINT8    Socket,
  IN  UINT8    Register,
  IN  UINT8    Data
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  UfmCommandDataReadFifo(): Execute given UFM command that receives some data via UFM Read FIFO.

  @param  Socket       INPUT: UINT8, Processor Socket ID
          Command      INPUT: UINT8, UFM Command
	  Buffer       INPUT/OUTPUT: PFR_DATA_BUFFER*, Ptr to buffer containing data
                                     Buffer->Length indicates the size of data in bytes to read from FIFO

  @retval Status       EFI_SUCCESS  Success, UFM command executed successfully and data are stored in supplied buffer
                       Other        Error, UFM command execution not successful or CPLD Access Error
**/
EFI_STATUS
UfmCommandDataReadFifo (
  IN     UINT8             Socket,
  IN     UINT8             Command,
  IN OUT PFR_DATA_BUFFER   *Buffer
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  UfmCommandDataWriteFifo(): Execute given UFM command that requires some data to be supplied via UFM Write FIFO.

  @param  Socket     INPUT: UINT8, Processor Socket ID
          Command    INPUT: UINT8, UFM Command
          Buffer     INPUT: PFR_DATA_BUFFER*, Ptr to buffer containing data
                            Buffer->Length indicates the size of data in bytes to be written to FIFO

  @retval Status     EFI_SUCCESS  Success, UFM command executed successfully
                     Other        Error, UFM command execution not successful or CPLD Access Error
**/
EFI_STATUS
UfmCommandDataWriteFifo (
  IN  UINT8              Socket,
  IN  UINT8              Command,
  IN  PFR_DATA_BUFFER    *Buffer
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  UfmCommandNonData(): Execute given UFM command that does not have any data associated with it.

  @param  Socket     INPUT: UINT8, Processor Socket ID
          Command    INPUT: UINT8, UFM Command

  @retval Status     EFI_SUCCESS  Success, UFM command executed successfully
                     Other        Error, UFM command execution not successful or CPLD Access Error
**/
EFI_STATUS
UfmCommandNonData (
  IN  UINT8    Socket,
  IN  UINT8    Command
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  PfrGetPlatformInfo(): Get ptr to platform information PFR_LIB_DATA.
                This function is called ONLY if PFR is supported.

  @param  Info          INPUT/OUTPUT: PFR_LIB_DATA**, Ptr to PFR_LIB_DATA

  @retval EFI_STATUS    EFI_SUCCESS, Success
                        Other, Error
**/
EFI_STATUS
PfrGetPlatformInfo (
  IN OUT  PFR_LIB_DATA  **Info
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  PfrSupported(): Determine whether PFR is supported in the platform.

  @param  None

  @retval Status    EFI_SUCCESS  Success, PFR supported
                    Other        Error, PFR not supported

**/
EFI_STATUS
PfrSupported (
  );
//
//-----------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------
#pragma pack (pop)
//-----------------------------------------------------------------------------------------------------

#endif
