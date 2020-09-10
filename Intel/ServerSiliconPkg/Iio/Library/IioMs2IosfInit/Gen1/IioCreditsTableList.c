/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include "IioCredits.h"
#include <Library/DebugLib.h>

extern CREDITS_ENTRY        ItcCreditDefaultTable[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie3x16Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie3x8x8Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie3x4x4x8Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie3x8x4x4Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie3x4x4x4x4Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie4x16Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie4x8x8Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie4x4x4x8Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie4x8x4x4Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie4x4x4x4x4Table[ITC_NUMBER_ENTRIES];

extern CREDITS_ENTRY        OtcCreditDefaultTable[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie3x16Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie3x8x8Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie3x4x4x8Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie3x8x4x4Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie3x4x4x4x4Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie4x16Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie4x8x8Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie4x4x4x8Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie4x8x4x4Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie4x4x4x4x4Table[OTC_NUMBER_ENTRIES];

SINGLE_PCIE_CREDIT_TABLE   ItcCreditSinglePcieTableList[] = {
  { IIO_BIFURCATE_x4x4x4x4, (CREDITS_ENTRY*) &ItcCreditSinglePcie4x4x4x4x4Table, (CREDITS_ENTRY*) &ItcCreditSinglePcie3x4x4x4x4Table },
  { IIO_BIFURCATE_x4x4xxx8, (CREDITS_ENTRY*) &ItcCreditSinglePcie4x4x4x8Table, (CREDITS_ENTRY*) &ItcCreditSinglePcie3x4x4x8Table },
  { IIO_BIFURCATE_xxx8x4x4, (CREDITS_ENTRY*) &ItcCreditSinglePcie4x8x4x4Table, (CREDITS_ENTRY*) &ItcCreditSinglePcie3x8x4x4Table },
  { IIO_BIFURCATE_xxx8xxx8, (CREDITS_ENTRY*) &ItcCreditSinglePcie4x8x8Table, (CREDITS_ENTRY*) &ItcCreditSinglePcie3x8x8Table },
  { IIO_BIFURCATE_xxxxxx16, (CREDITS_ENTRY*) &ItcCreditSinglePcie4x16Table, (CREDITS_ENTRY*) &ItcCreditSinglePcie3x16Table },
};

SINGLE_PCIE_CREDIT_TABLE   OtcCreditSinglePcieTableList[] = {
  { IIO_BIFURCATE_x4x4x4x4, (CREDITS_ENTRY*) &OtcCreditSinglePcie4x4x4x4x4Table, (CREDITS_ENTRY*) &OtcCreditSinglePcie3x4x4x4x4Table },
  { IIO_BIFURCATE_x4x4xxx8, (CREDITS_ENTRY*) &OtcCreditSinglePcie4x4x4x8Table, (CREDITS_ENTRY*) &OtcCreditSinglePcie3x4x4x8Table },
  { IIO_BIFURCATE_xxx8x4x4, (CREDITS_ENTRY*) &OtcCreditSinglePcie4x8x4x4Table, (CREDITS_ENTRY*) &OtcCreditSinglePcie3x8x4x4Table },
  { IIO_BIFURCATE_xxx8xxx8, (CREDITS_ENTRY*) &OtcCreditSinglePcie4x8x8Table, (CREDITS_ENTRY*) &OtcCreditSinglePcie3x8x8Table },
  { IIO_BIFURCATE_xxxxxx16, (CREDITS_ENTRY*) &OtcCreditSinglePcie4x16Table, (CREDITS_ENTRY*) &OtcCreditSinglePcie3x16Table },
};

CREDITS_ENTRY*
GetItcCreditSinglePcieTable (
  IN UINT8         Bifurcation,
  IN IIO_PORT_GEN  PcieGen
  )
{
  if (Bifurcation >= MAX_X4_BIFURCATION_TYPE) {
    return (CREDITS_ENTRY*) &ItcCreditDefaultTable;
  }

  ASSERT (Bifurcation == ItcCreditSinglePcieTableList[Bifurcation].Bifurcation);

  switch (PcieGen) {
  case IIO_GEN3:
    return ItcCreditSinglePcieTableList[Bifurcation].Pcie3Table;
  case IIO_GEN4:
    return ItcCreditSinglePcieTableList[Bifurcation].Pcie4Table;
  default:
    return (CREDITS_ENTRY*) &ItcCreditDefaultTable;
  }
}

CREDITS_ENTRY*
GetOtcCreditSinglePcieTable (
  IN UINT8         Bifurcation,
  IN IIO_PORT_GEN  PcieGen
  )
{
  if (Bifurcation >= MAX_X4_BIFURCATION_TYPE) {
    return (CREDITS_ENTRY*) &OtcCreditDefaultTable;
  }

  ASSERT (Bifurcation == OtcCreditSinglePcieTableList[Bifurcation].Bifurcation);

  switch (PcieGen) {
  case IIO_GEN3:
    return OtcCreditSinglePcieTableList[Bifurcation].Pcie3Table;
  case IIO_GEN4:
    return OtcCreditSinglePcieTableList[Bifurcation].Pcie4Table;
  default:
    return (CREDITS_ENTRY*) &OtcCreditDefaultTable;
  }
}
