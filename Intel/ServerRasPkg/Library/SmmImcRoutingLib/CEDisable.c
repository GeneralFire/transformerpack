/** @file
  Implementation of disable CE function for SMI storm.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/RasDebugLib.h>
#include <IioUniversalData.h>
#include "ImcRoutingPrivate.h"

#define BCD2DEC(x)  ((x & 0x0F) + ((x >> 4) * 10))

/**
  This function reads the CMOS location.

  @param[in]  Address        - cmos address to read

  @retval cmos address value
**/

UINT8
EFIAPI
RtcRead (
  IN        UINT8   Address
  )
{
  UINT8             Data8;

  //
  // Port 0x74 is alias of port 0x70 which is write only
  //
  Data8 = IoRead8 (0x74);
  Data8 &= 0x80;
  Data8 |= Address;
  IoWrite8 (0x70, Data8);

  //
  // Data is at port 0x71
  //
  Data8 = IoRead8 (0x71);
  return Data8;
}


/**
  Waits until RTC register A and D show data is valid.

  @param[in]  Timeout        - Maximum time to wait

  @retval EFI_DEVICE_ERROR   RTC device error.
          EFI_SUCCESS        RTC is updated and ready.
**/
EFI_STATUS
EFIAPI
RtcWaitToUpdate (
  IN        UINTN   Timeout
  )
{
  RTC_REGISTER_A    RegisterA;
  RTC_REGISTER_D    RegisterD;

  //
  // See if the RTC is functioning correctly
  //
  RegisterD.Data = RtcRead (RTC_ADDRESS_REGISTER_D);

  if (RegisterD.Bits.Vrt == 0) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Wait for up to 0.1 seconds for the RTC to be ready.
  //
  Timeout = (Timeout / 10) + 1;
  RegisterA.Data = RtcRead (RTC_ADDRESS_REGISTER_A);
  while (RegisterA.Bits.Uip == 1 && Timeout > 0) {
    MicroSecondDelay (10);
    RegisterA.Data = RtcRead (RTC_ADDRESS_REGISTER_A);
    Timeout--;
  }

  RegisterD.Data = RtcRead (RTC_ADDRESS_REGISTER_D);
  if (Timeout == 0 || RegisterD.Bits.Vrt == 0) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  This function provides the time stamp information.

  @param[in]  Time        - The time stamp

  @retval The time
**/
UINT32
EFIAPI
EfiSmmGetTime (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT8       Data8;
  UINT32      CurHours = 0;

  //
  // Wait for up to 0.1 seconds for the RTC to be updated
  //
  Status = RtcWaitToUpdate (100000);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"Wait RTC turnover fail \n"));
  }

  Data8 = RtcRead (RTC_ADDRESS_YEAR);
  Data8 = BCD2DEC (Data8);
  CurHours += Data8 * 365 * 24;

  Data8 = RtcRead (RTC_ADDRESS_MONTH);
  Data8 = BCD2DEC (Data8);
  CurHours += Data8 * 30 * 24;

  Data8 = RtcRead (RTC_ADDRESS_DAY_OF_THE_MONTH);
  Data8 = BCD2DEC (Data8);
  CurHours += Data8  * 24;

  Data8 = RtcRead (RTC_ADDRESS_HOURS);
  Data8 = BCD2DEC (Data8);
  CurHours +=  Data8;

  return CurHours;
}

/**
  This function gets the number of hours passed.

  @param[in]  PrevTime  - The previous time stamp

  @retval The time
**/
UINT32
EfiSmmGetNoofHoursPast (
  IN     UINT32     PrevHours
  )
{
  UINT32      CurHours;
  UINT32      HoursPast;

  CurHours = EfiSmmGetTime ();
  HoursPast = 0;

  if (CurHours > PrevHours) {
    HoursPast = (CurHours - PrevHours);
  }
  return HoursPast;
}

/**
  Disable Rank SMI depend on Memory Correctable Error Flood policy

  @param[in]  MemoryErrRecord      - Pointer to the memory error record.

  @retval None

**/
VOID
EFIAPI
CorrectedErrorSMIThrottling (
  IN        UINT8    Node,
  IN        UINT8    ChOnMc,
  IN        UINT8    Rank,
  IN        UINT8    MemCeFloodPolicy
  )
{
  IMC_RAS_STRUCTURE         *ImcRas;
  CE_INTERVAL_ERROR_STRUCT  *RankIntervalError;
  ImcRas = GetImcRasData ();

  //
  //  if Disable, nothing to do
  //
  if(!MemCeFloodPolicy){
    return;
  }
  //
  //  if only allowed Memory Ce once, disable smi
  //
  if(MemCeFloodPolicy == 1){
    DisableRankError (Node,ChOnMc,Rank);
    return;
  }

  RankIntervalError = &ImcRas->RankIntervalError;

  //
  // Compute rank error rate and disable corrected SMIs if rate > threshold
  //

  //
  // Init timestamp, if first time
  //
  if (RankIntervalError->LastErrorTime[Node][ChOnMc][Rank] == 0) {
    RankIntervalError->LastErrorTime[Node][ChOnMc][Rank] = EfiSmmGetTime ();
    RankIntervalError->RankErrorCounter[Node][ChOnMc][Rank] ++;
    return;
  }

  if (EfiSmmGetNoofHoursPast (RankIntervalError->LastErrorTime[Node][ChOnMc][Rank])
      <= RankIntervalError->ErrorInterval) {
    RankIntervalError->RankErrorCounter[Node][ChOnMc][Rank] ++;

    //
    // Disable SMI when number of rank failures per TIME_DURATION hours exceeds threshold
    //
    if (RankIntervalError->RankErrorCounter[Node][ChOnMc][Rank] >= RankIntervalError->RankIntervalErrorrCnt) {
      DisableRankError (Node,ChOnMc,Rank);
    }
  } else {
    //
    // Reset beginning of time period for rank
    //
    RankIntervalError->LastErrorTime[Node][ChOnMc][Rank] = EfiSmmGetTime ();
  }

  return;
}
