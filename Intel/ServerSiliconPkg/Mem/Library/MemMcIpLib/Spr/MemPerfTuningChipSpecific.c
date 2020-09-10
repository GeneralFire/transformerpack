/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <SysHostChip.h>
#include "Include/MemMcRegs.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include "Include/MemMcIpLibInternal.h"

/**
  Program scheduler registers, chip specific

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval N/A
**/
VOID
DdrSchedulerConfigLateChip (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  PSYSHOST                                      Host;
  SCHEDULER_RID_MCDDC_CTL_STRUCT                Rid;
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT            SchedulerEnables;

  Host = GetSysHostPointer ();

  //RidMrr needs to be enabled in normal mode
  ConfigRidMrrEntry (Host, Socket, Channel, TRUE);

  Rid.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG);
  SchedulerEnables.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_ENABLES_MCDDC_CTL_REG);

  //
  // Scheduler RID
  //
  Rid.Bits.enable_rid_vc0 = 1;
  Rid.Bits.enable_rid_vc1 = 0; //DDR5 Only default
  Rid.Bits.enable_rid_vc2 = 0; //DDR5 Only default
  Rid.Bits.enable_rid_vc3 = 0;
  if (IsDcpmmPresentChannel (Socket, Channel)) {
    Rid.Bits.enable_rid_vc1 = IsTwoWayNmCacheEnabled ();
    Rid.Bits.enable_rid_vc2 = 1;
  }

  //
  // Scheduler Enables
  //
  SchedulerEnables.Bits.rpq_vc1_llt_en = IsTwoWayNmCacheEnabled ();

  MemWritePciCfgEp (Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG, Rid.Data);
  MemWritePciCfgEp (Socket, Channel, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);

} // DdrSchedulerConfigLateChip

/**
  Configure Rid Vc before Training

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval N/A
**/
VOID
ConfigureRidVcbeforeTraining (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  PSYSHOST                                      Host;
  SCHEDULER_RID_MCDDC_CTL_STRUCT                Rid;
  MC0_DP_CHKN_BIT2_MCDDC_DP_STRUCT              Mc0DpChenBit2;

  Host = GetSysHostPointer ();

  Rid.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG);

  //
  // Scheduler RID
  //
  Rid.Bits.enable_rid_vc0 = 1;
  Rid.Bits.enable_rid_vc1 = 0;
  Rid.Bits.enable_rid_vc2 = 0;
  Rid.Bits.enable_rid_vc3 = 0;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    Rid.Bits.enable_rid_underfill = 0;
  } else {
    Rid.Bits.enable_rid_underfill = 1;
  }

  Rid.Bits.enable_rid = 0xffffff;
  Rid.Bits.rid_vc2_low_threshold = 3;
  MemWritePciCfgEp (Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG, Rid.Data);

  Mc0DpChenBit2.Data = MemReadPciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT2_MCDDC_DP_REG);
  Mc0DpChenBit2.Bits.dis_rdb_byp = 0;
  MemWritePciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT2_MCDDC_DP_REG, Mc0DpChenBit2.Data);

} // ConfigureRidVcbeforeTraining


