/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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

#ifndef _SMBUS_RECOVERY_
#define _SMBUS_RECOVERY_

#define MAX_SMBUS_SPD_SEGMENTS            2

#define SMBUS_RECOVERY_SEMAPHORE_PERIOD   50000000  // The minimum period of time in 100 nanosecond units.
#define SMBUS_RECOVERY_SEMAPHORE_INTERVAL 640000    // The period of time interval between SMIs.
#define SMBUS_RECOVERY_TIME_PERIOD        350000    // The minimum period of time in 100 nanosecond units.
#define SMBUS_RECOVERY_TIME_INTERVAL      160000    // The period of time interval between SMIs.

/*
The following is the data structure used for doing the error recovery on the SMBUS
Some of the state of the SMBUS is saved  before doing the error recovery and
when it is done, we restore the state.  And also, an 'Attribute' filed is used to
indicate the status of the recovery process
*/

typedef union {
  struct {
    UINT8                Attribute;        // State of the data structure, whether recovery needed/started/completed
    UINT8                TsodSA:3;         // Slave address of the last SMbus transaction
    UINT8                TsodPollEn:1;     // smb_tsod_poll_en
    UINT8                SmbSbeEn:1;       // smb_sbe_en
    UINT8                ClttSetting:1;    // ddr_disable_cltt
    UINT8                Reserved:2;
  } Bits;
  UINT16 Data;
} SMBUS_RECOVERY_DATA_STRUCT;

#define UNDER_RECOVERY    BIT0


#define BOOTTIME_RECOVERY                   1
#define RUNTIME_RECOVERY                    0
#define SMBUS_RECOVERY_TIME_35_MS           35
#define SMBUS_RECOVERY_CALLBACK_COUNT       5

VOID
EFIAPI
EnableSmbusRecovery (
  );

VOID
EFIAPI
SmbusRecoverySmiHandler (
  VOID
  );



#endif
