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

#include "IioCreditsGen2.h"
#include <Library/DebugLib.h>

extern CREDITS_ENTRY        ItcCreditDefaultTableGen2[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen4x16Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen4x8x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen4x8x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen4x4x4x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen4x4x4x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen4x16Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen5x8x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen5x8x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen5x4x4x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen5x4x4x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen5x16Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x4x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x2x2x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x8x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x8x4x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x4x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x2x2x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x4x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x4x4x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x2x2x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x8x2x2x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x2x2x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x4x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x4x4x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x2x2x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x2x2x4x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x4x2x2x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x2x2x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x2x2x4x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x4x2x2x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x4x2x2x2x2x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54x2x2x2x2x2x2x2x2Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx8x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx8x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx4x4x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx4x4x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx16Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx2x2x4x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx4x2x2x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx8x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx2x2x4x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx4x2x2x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx4x4x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx2x2x2x2x8Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx2x2x2x2x4x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx2x2x4x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx4x2x2x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen54Ntx2x2x2x2x2x2x4Table[ITC_NUMBER_ENTRIES_GEN2];

extern CREDITS_ENTRY        OtcCreditDefaultTableGen2[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen4x16Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen4x8x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen4x8x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen4x4x4x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen4x4x4x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen4x16Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen5x8x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen5x8x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen5x4x4x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen5x4x4x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen5x16Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x4x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x2x2x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x8x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x8x4x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x4x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x2x2x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x4x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x4x4x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x2x2x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x8x2x2x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x2x2x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x4x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x4x4x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x2x2x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x2x2x4x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x4x2x2x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x2x2x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x2x2x4x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x4x2x2x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x4x2x2x2x2x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54x2x2x2x2x2x2x2x2Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx8x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx8x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx4x4x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx4x4x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx16Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx2x2x4x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx4x2x2x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx8x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx2x2x4x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx4x2x2x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx4x4x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx2x2x2x2x8Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx2x2x2x2x4x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx2x2x4x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx4x2x2x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen54Ntx2x2x2x2x2x2x4Table[OTC_NUMBER_ENTRIES_GEN2];

// PI5 Gen4 Credit table
PCIE_CREDIT_TABLE   Pi5Gen4CreditPcieTableList[] = {
  {IIO_BIFURCATE_x4x4x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen4x4x4x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen4x4x4x4x4Table},
  {IIO_BIFURCATE_x4x4xxx8, (CREDITS_ENTRY*) &ItcCreditPcieGen4x4x4x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen4x4x4x8Table},
  {IIO_BIFURCATE_xxx8x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen4x8x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen4x8x4x4Table},
  {IIO_BIFURCATE_xxx8xxx8, (CREDITS_ENTRY*) &ItcCreditPcieGen4x8x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen4x8x8Table},
  {IIO_BIFURCATE_xxxxxx16, (CREDITS_ENTRY*) &ItcCreditPcieGen4x16Table, (CREDITS_ENTRY*) &OtcCreditPcieGen4x16Table},
};

// PI5 Gen5 + Gen4 Credit table
PCIE_CREDIT_TABLE   Pi5Gen54CreditPcieTableList[] = {
  {IIO_BIFURCATE_x4x4x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen5x4x4x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen5x4x4x4x4Table},
  {IIO_BIFURCATE_x4x4xxx8, (CREDITS_ENTRY*) &ItcCreditPcieGen5x4x4x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen5x4x4x8Table},
  {IIO_BIFURCATE_xxx8x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen5x8x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen5x8x4x4Table},
  {IIO_BIFURCATE_xxx8xxx8, (CREDITS_ENTRY*) &ItcCreditPcieGen5x8x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen5x8x8Table},
  {IIO_BIFURCATE_xxxxxx16, (CREDITS_ENTRY*) &ItcCreditPcieGen5x16Table, (CREDITS_ENTRY*) &OtcCreditPcieGen5x16Table},
  {IIO_BIFURCATE_x2x2x4x8, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x4x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x4x8Table},
  {IIO_BIFURCATE_x4x2x2x8, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x2x2x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x2x2x8Table},
  {IIO_BIFURCATE_x8x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x8x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x8x2x2x4Table},
  {IIO_BIFURCATE_x8x4x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x8x4x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x8x4x2x2Table},
  {IIO_BIFURCATE_x2x2x4x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x4x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x4x4x4Table},
  {IIO_BIFURCATE_x4x2x2x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x2x2x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x2x2x4x4Table},
  {IIO_BIFURCATE_x4x4x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x4x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x4x2x2x4Table},
  {IIO_BIFURCATE_x4x4x4x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x4x4x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x4x4x2x2Table},
  {IIO_BIFURCATE_x2x2x2x2x8, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x2x2x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x2x2x8Table},
  {IIO_BIFURCATE_x8x2x2x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x8x2x2x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x8x2x2x2x2Table},
  {IIO_BIFURCATE_x2x2x2x2x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x2x2x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x2x2x4x4Table},
  {IIO_BIFURCATE_x2x2x4x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x4x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x4x2x2x4Table},
  {IIO_BIFURCATE_x2x2x4x4x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x4x4x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x4x4x2x2Table},
  {IIO_BIFURCATE_x4x2x2x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x2x2x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x2x2x2x2x4Table},
  {IIO_BIFURCATE_x4x2x2x4x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x2x2x4x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x2x2x4x2x2Table},
  {IIO_BIFURCATE_x4x4x2x2x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x4x2x2x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x4x2x2x2x2Table},
  {IIO_BIFURCATE_x2x2x2x2x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x2x2x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x2x2x2x2x4Table},
  {IIO_BIFURCATE_x2x2x2x2x4x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x2x2x4x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x2x2x4x2x2Table},
  {IIO_BIFURCATE_x2x2x4x2x2x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x4x2x2x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x4x2x2x2x2Table},
  {IIO_BIFURCATE_x4x2x2x2x2x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x4x2x2x2x2x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x4x2x2x2x2x2x2Table},
  {IIO_BIFURCATE_x2x2x2x2x2x2x2x2, (CREDITS_ENTRY*) &ItcCreditPcieGen54x2x2x2x2x2x2x2x2Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54x2x2x2x2x2x2x2x2Table},
};

// PI5 NTB Gen4 + Gen5 Credit table
PCIE_CREDIT_TABLE   Pi5Gen54NtbCreditPcieTableList[] = {
  {IIO_BIFURCATE_x4x4x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx4x4x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx4x4x4x4Table},
  {IIO_BIFURCATE_x4x4xxx8, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx4x4x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx4x4x8Table},
  {IIO_BIFURCATE_xxx8x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx8x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx8x4x4Table},
  {IIO_BIFURCATE_xxx8xxx8, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx8x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx8x8Table},
  {IIO_BIFURCATE_xxxxxx16, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx16Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx16Table},
  {IIO_BIFURCATE_x2x2x4x8, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx2x2x4x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx2x2x4x8Table},
  {IIO_BIFURCATE_x4x2x2x8, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx4x2x2x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx4x2x2x8Table},
  {IIO_BIFURCATE_x8x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx8x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx8x2x2x4Table},
  {IIO_BIFURCATE_x2x2x4x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx2x2x4x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx2x2x4x4x4Table},
  {IIO_BIFURCATE_x4x2x2x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx4x2x2x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx4x2x2x4x4Table},
  {IIO_BIFURCATE_x4x4x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx4x4x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx4x4x2x2x4Table},
  {IIO_BIFURCATE_x2x2x2x2x8, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx2x2x2x2x8Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx2x2x2x2x8Table},
  {IIO_BIFURCATE_x2x2x2x2x4x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx2x2x2x2x4x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx2x2x2x2x4x4Table},
  {IIO_BIFURCATE_x2x2x4x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx2x2x4x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx2x2x4x2x2x4Table},
  {IIO_BIFURCATE_x4x2x2x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx4x2x2x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx4x2x2x2x2x4Table},
  {IIO_BIFURCATE_x2x2x2x2x2x2x4, (CREDITS_ENTRY*) &ItcCreditPcieGen54Ntx2x2x2x2x2x2x4Table, (CREDITS_ENTRY*) &OtcCreditPcieGen54Ntx2x2x2x2x2x2x4Table},
};

CREDITS_ENTRY*
GetPi5CreditTable (
  IN UINT8         CreditType,
  IN UINT8         Bifurcation,
  IN PI5_MODES     PcieMode
  )
{
  UINT8              Index;
  UINT8              MaxTableSize;
  BOOLEAN            BifurcationFound = FALSE;
  CREDITS_ENTRY      *CreditsTablePtr = NULL;
  PCIE_CREDIT_TABLE  *PcieModeCreditsTablePtr = NULL;

  //
  // Initialize the credit pointer.
  // For error handling, unknown CreditType will default to ITC.
  //
  if (CreditType == CREDIT_TYPE_OTC) {
    CreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDefaultTableGen2;
  } else {
    CreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDefaultTableGen2;
  }

  if (CreditType > CREDIT_TYPE_OTC) {
    ASSERT (FALSE);
    return CreditsTablePtr;
  }

  switch (PcieMode) {
  case PI5_MODE_GEN4_ONLY:
  case PI5_MODE_NTB_GEN4:
    MaxTableSize = NELEMENTS(Pi5Gen4CreditPcieTableList);
    PcieModeCreditsTablePtr = (PCIE_CREDIT_TABLE*) &Pi5Gen4CreditPcieTableList;
    break;
  case PI5_MODE_IAL:
    if (CreditType == CREDIT_TYPE_OTC) {
      return (CREDITS_ENTRY*) &OtcCreditPcieGen5x16Table;
    } else {
      return (CREDITS_ENTRY*) &ItcCreditPcieGen5x16Table;
    }
    break;
  case PI5_MODE_GEN5_ONLY:
  case PI5_MODE_GEN5_AND_GEN4:
    MaxTableSize = NELEMENTS(Pi5Gen54CreditPcieTableList);
    PcieModeCreditsTablePtr = (PCIE_CREDIT_TABLE*) &Pi5Gen54CreditPcieTableList;
    break;
  case PI5_MODE_NTB:
    MaxTableSize = NELEMENTS(Pi5Gen54NtbCreditPcieTableList);
    PcieModeCreditsTablePtr = (PCIE_CREDIT_TABLE*) &Pi5Gen54NtbCreditPcieTableList;
    break;
  default:
    ASSERT (FALSE);
    return CreditsTablePtr;
    break;
  }

  //
  // Search for the bifurcation entry index, input bifurcation and the tables bifurcation should match
  //
  for (Index = 0; Index < MaxTableSize; Index++) {
    if (Bifurcation == PcieModeCreditsTablePtr[Index].Bifurcation) {
      BifurcationFound = TRUE;
      break;
    }
  }

  //
  // Bifurcation from input and table doesn't match, return default table.
  //
  ASSERT (BifurcationFound);
  if (!BifurcationFound) {
    return CreditsTablePtr;
  }

  if (CreditType == CREDIT_TYPE_ITC) {
    CreditsTablePtr = PcieModeCreditsTablePtr[Index].ItcPcieTable;
  } else if (CreditType == CREDIT_TYPE_OTC) {
    CreditsTablePtr = PcieModeCreditsTablePtr[Index].OtcPcieTable;
  }

  return CreditsTablePtr;
}
