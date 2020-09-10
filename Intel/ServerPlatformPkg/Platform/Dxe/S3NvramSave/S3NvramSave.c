/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include "S3NvramSave.h"
#include <Protocol/PlatformType.h>
#include <Protocol/Decompress.h>
#include <Library/CompressLib.h>
#include <Protocol/VariableLock.h>
#include <Guid/CpuNvramData.h>
#include <Include/SystemInfoVar.h>
#include <Library/SystemInfoLib.h>
#include <Library/CompressedVariableLib.h>

/**
  Verify the SysHost structure size.

  Verifies that the size of the SysHost structure in PEI is
  the same as the size of the SysHost structure and DXE and
  ASSERT's is the size is not the same.

  This is typically caused by the use of pointers or UINTNs
  in the SysHost structure, neither of those datatypes are
  allowed in SysHost.

  @param  None

  @retval None

**/

VOID
VerifySysHostStructureSize (
  VOID
  )
{

  if (PcdGet32 (PcdPeiSyshostMemorySize) != sizeof (SYSHOST)) {

    DEBUG ((EFI_D_ERROR, "ERROR: In DXE sizeof SysHost = %d, in PEI sizeof SysHost = %d\n",
      sizeof (SYSHOST),
      PcdGet32 (PcdPeiSyshostMemorySize)
      ));

    DEBUG ((EFI_D_ERROR, "Size of SysHost must match in PEI and DXE\n"));
    ASSERT (FALSE);

  }

  return;

} // VerifySysHostStructureSize

/**

  This function will retrieve the S3 data from HOBs produced by MRC
  and will save it to NVRAM if the data is absent or different from
  the previously saved data.

  @param VOID

  @retval VOID

**/
VOID
SaveS3StructToNvram (
  VOID
  )
{
  EFI_STATUS Status;

  DEBUG ((EFI_D_INFO, "SaveS3StructToNvram: entered\n"));

  Status = SaveVariableFromHob (gEfiMemoryConfigDataHobGuid, L"MemoryConfig", gEfiMemoryConfigDataGuid);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "SaveS3StructToNvram: SaveVariableFromHob failed, Status = %r\n", Status));
    ASSERT (FALSE);
  }

  DEBUG ((EFI_D_INFO, "SaveS3StructToNvram: exiting\n"));

  return;

}

/**

  This function is used to compare the saved S3 structure from NVRAM
  with the S3 structure populated on the current boot.  This will
  determine if the structure needs to be saved to NVRAM again or not.

  @param IN VOID  *CurrentBootStruct - Pointer to current boot NVRAM structure
  @param IN VOID  *SavedBootStruct - Pointer to buffer of the saved NVRAM structure
                                     from a previous boot

  @retval BOOLEAN  -  TRUE  - The structures match
              @retval FALSE - There are differences

**/
BOOLEAN
S3NvramCompareStructs (
  IN VOID                               *CurrentBootStruct,
  IN VOID                               *SavedBootStruct
  )
{
  EFI_STATUS                    Status;
  struct sysNvram               *SavedS3    = SavedBootStruct;
  struct sysNvram               *CurrentS3  = CurrentBootStruct;
  UINTN                         CompareValue;
  EFI_PLATFORM_TYPE_PROTOCOL    *mPlatformType;
  BOOLEAN                       StructsMatch = TRUE;

  Status = gBS->LocateProtocol (
                &gEfiPlatformTypeProtocolGuid,
                NULL,
                &mPlatformType
                );
  DEBUG ((EFI_D_INFO, "S3NvramCompareStructs: Set variable status: %r \n",   Status));
  ASSERT_EFI_ERROR (Status);

  //
  // TODO:  This will be updated to check more vital parts of the
  // NVRAM structure instead of the entire structure.  For now, a compare of
  // the entire NVRAM structure is used
  //
  CompareValue = CompareMem ((VOID*)CurrentS3, (VOID*)SavedS3, sizeof (struct sysNvram));
  DEBUG((EFI_D_INFO, "S3NvramCompareStructs: Set variable CompareValue: %x \n",  CompareValue));

  //
  // SKX_TODO: Do we need to skip saving this structure a 2nd time to avoid garbage collection for STHI boards
  //
  if (CompareValue != 0) {
    StructsMatch = FALSE;
  }

  return StructsMatch;
}

/**

  This function will save the Cpu NvRam Data to NVRAM.

  @param VOID

  @retval VOID

**/
VOID
SaveCpuS3StructToNvram (
  VOID
  )
{
  EFI_HOB_GUID_TYPE               *GuidHob;
  VOID                            *HobData;
  UINTN                            HobSize;
  EFI_STATUS                       Status;
  GuidHob = GetFirstGuidHob (&gEfiCpuNvramDataGuid);
  if (GuidHob == NULL) {
    return;
  }
  HobData = GET_GUID_HOB_DATA (GuidHob);
  HobSize = GET_GUID_HOB_DATA_SIZE (GuidHob);
  Status = gRT->SetVariable (
                  EFI_CPU_NVRAM_DATA_VARIABLE_NAME,
                  &gEfiCpuNvramDataGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  HobSize,
                  HobData
                  );
}

/**

  This function will save the KTI HOST NVRAM data hob to Variable.

  @param VOID

  @retval VOID

**/
VOID
SaveKtiS3StructToNvram (
  VOID
  )
{
  EFI_STATUS          Status;
  UINTN               HobSize;
  EFI_HOB_GUID_TYPE   *GuidHob;
  VOID                *HobData;
  CHAR16              EfiKtiHostNvramVariableName[] = L"KtiNvramData";

  GuidHob = GetFirstGuidHob (&gEfiKtiHostNvramDataHobGuid);
  if (GuidHob == NULL) {
    DEBUG((EFI_D_ERROR, "\n KtiHostNvram hob is not found! \n"));
    return;
  }
  HobData = GET_GUID_HOB_DATA (GuidHob);
  HobSize = GET_GUID_HOB_DATA_SIZE (GuidHob);

  Status = gRT->SetVariable (
                  EfiKtiHostNvramVariableName,
                  &gEfiKtiHostNvramDataHobGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  HobSize,
                  HobData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Status of saving KtiNvramData to Variable: %r \n", Status));
  }
  return;
}

/**

  This function will save the Cpu NvRam Data to NVRAM.

  @param VOID

  @retval VOID

**/
VOID
SaveInfoVarS3StructToNvram (
  VOID
  )
{
  SYS_INFO_VAR_NVRAM *InfoVarNvramPtr;
  EFI_STATUS         Status;
  InfoVarNvramPtr = GetSysInfoVarNvramPtr ();

  Status = gRT->SetVariable (
                  SYS_INFO_NVRAM_VAR_NAME,
                  &gEfiSysInfoVarNvramDataGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (SYS_INFO_VAR_NVRAM),
                  InfoVarNvramPtr
                  );
}

EFI_STATUS
EFIAPI
S3NvramSaveEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
/**

  This is the main entry point of the S3 NVRAM Save module.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS  -  Module launched successfully.

**/
{
  EFI_STATUS Status = EFI_SUCCESS;

  //
  // Check that SysHost size in DXE is the same as PEI.
  //

  VerifySysHostStructureSize ();

  //
  // Save the s3 structure from MRC into NVRAM if needed
  //
  SaveS3StructToNvram ();
  SaveCpuS3StructToNvram ();
  SaveKtiS3StructToNvram ();
  SaveInfoVarS3StructToNvram ();
  return Status;

}
