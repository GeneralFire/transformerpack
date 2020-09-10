/** @file
  MemCmdControlTables.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef  __MEM_CMD_CONTROL_TABLES_H__
#define  __MEM_CMD_CONTROL_TABLES_H__

#include <ReferenceCodeDataTypes.h>
#include <MemCommon.h>
#include <MrcCommonTypes.h>

struct SignalPoint {
  UINT8               Channel;
  GSM_CSN             Signal;
};

struct signalMapping {
  struct SignalPoint  signalDdr5;
  struct SignalPoint  signalDdr4;
};

struct ioGroup {
  UINT8               num;
  UINT8               side;
};

struct signalIOGroup {
  GSM_CSN             sig;
  MRC_GT              platformGroup;
  struct ioGroup      group;
};

/*
  Function to obtain the signal mapping for DDR4 and DDR5

  @param [   out] Signals    - Pointer to the signals table
  @param [   out] MaxSignals - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetDdrSignalsMappingChip (
  OUT struct signalMapping **Signals,
  OUT        UINT8         *MaxSignals
  );

/*
  Function to obtain the DDR signals table

  @param [   out] Signals    - Pointer to the signals table
  @param [   out] MaxSignals - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetDdrSignalsChip (
  OUT struct signalIOGroup **Signals,
  OUT        UINT8         *MaxSignals
  );

/*
  Function to obtain all the Cmd group signals table

  @param [   out] IogPtr     - Pointer to the signals table
  @param [   out] MaxIoGroup - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetCmdGroupAllTable (
  OUT struct ioGroup **IogPtr,
  OUT        UINT8   *MaxIoGroup
  );

/*
  Function to obtain the Ctl group 0 signals table

  @param [   out] IogPtr     - Pointer to the signals table
  @param [   out] MaxIoGroup - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetCtlGroup0Table (
  OUT struct ioGroup **IogPtr,
  OUT        UINT8   *MaxIoGroup
  );

/*
  Function to obtain the Ctl group 1 signals table

  @param [   out] IogPtr     - Pointer to the signals table
  @param [   out] MaxIoGroup - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetCtlGroup1Table (
  OUT struct ioGroup **IogPtr,
  OUT        UINT8   *MaxIoGroup
  );

/*
  Function to obtain the Ctl group 2 signals table

  @param [   out] IogPtr     - Pointer to the signals table
  @param [   out] MaxIoGroup - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetCtlGroup2Table (
  OUT struct ioGroup **IogPtr,
  OUT        UINT8   *MaxIoGroup
  );

/*
  Function to obtain the Ctl group 3 signals table

  @param [   out] IogPtr     - Pointer to the signals table
  @param [   out] MaxIoGroup - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetCtlGroup3Table (
  OUT struct ioGroup **IogPtr,
  OUT        UINT8   *MaxIoGroup
  );

/*
  Function to obtain the all the Ctl group signals table

  @param [   out] IogPtr     - Pointer to the signals table
  @param [   out] MaxIoGroup - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetCtlGroupAllTable (
  OUT struct ioGroup **IogPtr,
  OUT        UINT8   *MaxIoGroup
  );

/*
  Function to obtain the Ctl and Cmd group signals table

  @param [   out] IogPtr     - Pointer to the signals table
  @param [   out] MaxIoGroup - Pointer to the signals table size

  @retval N/A
*/
MRC_STATUS
GetCmdCtlGroupAllTable (
  OUT struct ioGroup **IogPtr,
  OUT        UINT8   *MaxIoGroup
  );

#endif   // __MEM_CMD_CONTROL_TABLES_H__
