/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>

#include "CrystalRidge.h"
#include "Dsm.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsFlowsPrivate.h"

/**

  Routine description: dumps the contents of SPA_INT_SET structure. Uses
  DEBUG family of functions.

  @param[in] IntSet Pointer to the structure to dump the data

**/
VOID
DumpSpaIntSet (
  IN CONST SPA_INT_SET *IntSet
  )
{
  CRDEBUG ((DEBUG_INFO, "    StartSpa         : 0x%012llX\n", IntSet->StartSpa));
  CRDEBUG ((DEBUG_INFO, "    EndSpa           : 0x%012llX\n", IntSet->EndSpa));
  CRDEBUG ((DEBUG_INFO, "    SpaBase          : 0x%012llX\n", IntSet->SpaBase));
  CRDEBUG ((DEBUG_INFO, "    Enabled          : %s\n",     PRINT_BOOL (IntSet->Enabled)));
  CRDEBUG ((DEBUG_INFO, "    Running          : %s\n",     PRINT_BOOL (IntSet->Running)));
  CRDEBUG ((DEBUG_INFO, "    EndedPrematurely : %s\n",     PRINT_BOOL (IntSet->EndedPrematurely)));
}

/**

  Routine description: dumps the contents of ARS_DIMM structure. Uses
  DEBUG family of functions.

  @param[in] ArsDimm Pointer to the structure to dump the data

**/
VOID
DumpArsDimm (
  IN CONST ARS_DIMM *ArsDimm
  )
{
  CRDEBUG ((DEBUG_INFO, "    Socket           : %d\n",     ArsDimm->Socket));
  CRDEBUG ((DEBUG_INFO, "    Channel          : %d\n",     ArsDimm->Channel));
  CRDEBUG ((DEBUG_INFO, "    Dimm             : %d\n",     ArsDimm->Dimm));
  CRDEBUG ((DEBUG_INFO, "    NfitHandle       : 0x%04x\n", ConvertSocketChannelDimmToNfitHandle (ArsDimm->Socket,
                                                            ArsDimm->Channel,
                                                            ArsDimm->Dimm)));
  CRDEBUG ((DEBUG_INFO, "    StartDpa         : 0x%012llX\n", ArsDimm->StartDpa));
  CRDEBUG ((DEBUG_INFO, "    EndDpa           : 0x%012llX\n", ArsDimm->EndDpa));
  CRDEBUG ((DEBUG_INFO, "    CurrentSpa       : 0x%012llX\n", ArsDimm->CurrentSpa));
  CRDEBUG ((DEBUG_INFO, "    SpaBase          : 0x%012llX\n", ArsDimm->SpaBase));
  CRDEBUG ((DEBUG_INFO, "    State            : %d\n",     ArsDimm->State));
  CRDEBUG ((DEBUG_INFO, "    ErrorsFound      : %d\n",     ArsDimm->ErrorsFound));
}

/**

  Routine description: dumps the contents of ARS_ERROR_RECORD structure. Uses
  DEBUG family of functions.

  @param[in] ArsErrorRecord Pointer to the structure to dump the data

**/
VOID
DumpArsErrorRecord (
  IN CONST ARS_ERROR_RECORD *ArsErrorRecord
  )
{
  CRDEBUG ((DEBUG_INFO, "    NfitHandle        : 0x%04x\n", ArsErrorRecord->NfitHandle));
  CRDEBUG ((DEBUG_INFO, "    SpaOfErrLoc       : 0x%012llX\n", ArsErrorRecord->SpaOfErrLoc));
  CRDEBUG ((DEBUG_INFO, "    Length            : 0x%012llX\n", ArsErrorRecord->Length));
}

/**

  Routine descritption: dumps the data in ARS_DATA structure using DEBUG
  family of functions.

  @param[in] Data Pointer to the structure to dump the contents

**/
VOID
DumpArsData (
  IN CONST ARS_DATA *Data
  )
{
  UINT16 Index;

  CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "--- ARS data dump ---\n"));
  CRDEBUG ((DEBUG_INFO, "  SystemStartSpa            : 0x%012llX\n", Data->SystemStartSpa));
  CRDEBUG ((DEBUG_INFO, "  SystemEndSpa              : 0x%012llX\n", Data->SystemEndSpa));
  CRDEBUG ((DEBUG_INFO, "  SystemRestartSpa          : 0x%012llX\n", Data->SystemRestartSpa));
  CRDEBUG ((DEBUG_INFO, "  InProgress                : %s\n",     PRINT_BOOL (Data->InProgress)));
  CRDEBUG ((DEBUG_INFO, "  EndedPrematurely          : %s\n",     PRINT_BOOL (Data->EndedPrematurely)));
  CRDEBUG ((DEBUG_INFO, "  InterleaveSetsCount       : %d\n",     Data->InterleaveSetsCount));
  CRDEBUG ((DEBUG_INFO, "  CurrentInterleaveSetIndex : %d\n",     Data->CurrentInterleaveSetIndex));
  CRDEBUG ((DEBUG_INFO, "  DimmsInCurrentIntSetCount : %d\n",     Data->DimmsInCurrentIntSetCount));
  CRDEBUG ((DEBUG_INFO, "  NumberOfErrorRecords      : %d\n",     Data->NumberOfErrorRecords));
  CRDEBUG ((DEBUG_INFO, "  InterleaveSets:\n"));
  for (Index = 0; Index < Data->InterleaveSetsCount && Index < ARRAY_SIZE (Data->InterleaveSets); Index++) {
    CRDEBUG ((DEBUG_INFO, "    %d :\n", Index));
    DumpSpaIntSet (&Data->InterleaveSets[Index]);
  }
  CRDEBUG ((DEBUG_INFO, "  DimmsInCurrentIntSet:\n"));
  for (Index = 0; Index < Data->DimmsInCurrentIntSetCount && Index < ARRAY_SIZE (Data->DimmsInCurrentIntSetList); Index++) {
    CRDEBUG ((DEBUG_INFO, "    %d :\n", Index));
    DumpArsDimm (&Data->DimmsInCurrentIntSetList[Index]);
  }
  CRDEBUG ((DEBUG_INFO, "  Errors found:\n"));
  for (Index = 0; Index < Data->NumberOfErrorRecords && Index < ARRAY_SIZE (Data->ErrorRecords); Index++) {
    CRDEBUG ((DEBUG_INFO, "    %d :\n", Index));
    DumpArsErrorRecord (&Data->ErrorRecords[Index]);
  }
  CRDEBUG ((DEBUG_INFO, "---\n"));
}
