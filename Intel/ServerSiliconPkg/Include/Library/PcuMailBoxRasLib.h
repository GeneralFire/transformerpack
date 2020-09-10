/** @file
  Interface header for Bios to Pcode Mailbox RAS library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2019  Intel Corporation. <BR>

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

#ifndef _BIOS_TO_PCODE_MAILBOX_RAS_LIB_H_
#define _BIOS_TO_PCODE_MAILBOX_RAS_LIB_H_


#include "PcuMailBoxLib.h"

/**
  Writes the given command to BIOS2PCU Mailbox and read back the data from PCU

  @param Socket       - CPU Socket number (Socket ID)
  @param Command      - Pcu mailbox command to write
  @param Data         - Pcu mailbox data

  @retval error code from the Pcu mailbox (0 = NO ERROR)

**/
UINT32
EFIAPI
Bios2PcodeMailBoxRead (
  IN UINT8  Socket,
  IN UINT32 Command,
  IN UINT32 *Data
  );

/**
  Writes the given command to BIOS to PCU Mailbox Interface CSR register

  @param Socket       - CPU Socket number (Socket ID)
  @param Command      - Pcu mailbox command to write
  @param Data         - Pcu mailbox data

  @retval error code from the Pcu mailbox (0 = NO ERROR)

**/
UINT32
EFIAPI
Bios2PcodeMailBoxWrite (
  IN UINT8  Socket,
  IN UINT32 Command,
  IN UINT32 Data
  );

/**
  This function provides capability of either reading BIOS-to-PCode command,
  writing BIOS-to-PCode command or doing read-modify-write BIOS-to-Pcode
  commands.

  For read capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  2. Read and return the data from BIOS_MAILBOX_DATA CSR

  For write capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "write-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR

  For read-modify-write capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  2. Read the data from BIOS_MAILBOX_DATA CSR
  3. Modify the said returned data with the mask provided by the user
  4. Write the data back to BIOS_MAILBOX DATA CSR and
     write "write-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  5. Write the data to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  6. Read and return the data from BIOS_MAILBOX_DATA CSR

  @param[in]      Socket                CPU Socket Node number (Socket ID)
  @param[in]      DwordReadCommand      "Read-type" BIOS-to-PCode mailbox command to write into
                                        BIOS_MAILBOX_INTERFACE CSR
  @param[in]      DwordWriteCommand     "Write-type" BIOS-to-PCode mailbox command to write into
                                        BIOS_MAILBOX_INTERFACE CSR
  @param[in, out] Dworddata             As input, the data to Pcode by writing to BIOS_MAILBOX_DATA
                                        CSR. As output, the data returned from Pcode by reading
                                        from BIOS_MAILBOX_DATA CSR.
  @param[in]      DwordMaskValue        To clear specific bits in the data returned from Pcode
  @param[in]      DwordOrdata           To set specific bits in the data returned from Pcode

  @retval Error code from the PCU mailbox (0 = NO ERROR)

**/
UINT32
EFIAPI
ReadWritePcuMailbox (
  IN     UINT8      Socket,
  IN     UINT32     DwordReadCommand,
  IN     UINT32     DwordWriteCommand,
  IN OUT UINT32     *DwordData,
  IN     UINT32     DwordMaskValue,
  IN     UINT32     DwordOrValue
  );


#endif  // _BIOS_TO_PCODE_MAILBOX_LIB_H_
