/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#include <PiDxe.h>

#include <Protocol/Tcg2Protocol.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
//APTIOV_SERVER_OVERRIDE_START
//#include <Library/Tpm2CommandLib.h>
#include <AmiTcg/TcgPc.h>
#include <AmiTcg/Tpm20.h>
//APTIOV_SERVER_OVERRIDE_END
#include <DxeOneTouchPlatform.h>
#include <DxeOobOneTouch.h>
#include <SmbiosOTA.h>
#include <Library/MeStorageLib.h>

//-----------------------------------------------------------------------------------------------------
//
EFI_GUID      mEfiOobTxtTpmDataGuid = EFI_OOB_TXT_TPM_DATA_GUID; // EFI Variable GUID for OOB Data
//
VOID          *mAllocatedMemory = NULL;                          // Allocated memory for NV Storage (both ME and EFI)
UINTN         mAllocatedSize = 0;                                // Size of Allocated memory for NV Storage
UINTN         mNvStorageSize = 0;                                // Max NV Storage Size in bytes
BOOLEAN       mMeNvStorage = FALSE;                              // ME NV Storage Not Supported
BOOLEAN       mEfiNvStorage = FALSE;                             // EFI NV Storage Not Supported
UINTN         mMeDataSize = 0;                                   // OOB data size read from ME
UINTN         mEfiDataSize = 0;                                  // OOB data size read from EFI
OOB_TXT_TPM   *mOttMe = NULL;                                    // Ptr to OOB data read from ME Storage
OOB_TXT_TPM   *mOttEfi = NULL;                                   // Ptr to OOB data read from EFI Storage
//
//-----------------------------------------------------------------------------------------------------
// OOB Tasks that require authentication
//
UINT8  AuthenticateTaskTable[] = {
              OXP_TASK_ENABLE_TXT_DTPM,
              OXP_TASK_DTPM_OWNER_CLEAR_ONLY,
              OXP_TASK_DTPM_CLEAR_DTPM_ACTIVATION,
              OXP_TASK_DTPM_CLEAR_ENABLE_TXT_DTPM,
              OXP_TASK_ENABLE_TXT_FTPM,
              OXP_TASK_FTPM_OWNER_CLEAR_ONLY,
              OXP_TASK_FTPM_CLEAR_FTPM_ACTIVATION,
              OXP_TASK_FTPM_CLEAR_ENABLE_TXT_FTPM,
              OXP_TASK_DISABLE_TXT_ONLY,
              OXP_TASK_DISABLE_DTPM_ONLY,
              OXP_TASK_DISABLE_FTPM_ONLY,
              OXP_TASK_PROVISION_DTPM,
              OXP_TASK_PROVISION_FTPM,
              OXP_TASK_HANDLE_TPM_NV_INDEX,
              OXP_TASK_DTPM_RESET_AUX_INDEX,
              OXP_TASK_FTPM_RESET_AUX_INDEX,
              OXP_TASK_HANDLE_MKTME,
              OXP_TASK_HANDLE_SGX,
              OXP_TASK_HANDLE_PFR,
              };
//
//-----------------------------------------------------------------------------------------------------
// OOB Tasks that require TPM Preparation
//
UINT8  TpmPreparationTaskTable[] = {
              OXP_TASK_ENABLE_TXT_DTPM,
              OXP_TASK_DTPM_OWNER_CLEAR_ONLY,
              OXP_TASK_DTPM_CLEAR_DTPM_ACTIVATION,
              OXP_TASK_DTPM_CLEAR_ENABLE_TXT_DTPM,
              OXP_TASK_ENABLE_TXT_FTPM,
              OXP_TASK_FTPM_OWNER_CLEAR_ONLY,
              OXP_TASK_FTPM_CLEAR_FTPM_ACTIVATION,
              OXP_TASK_FTPM_CLEAR_ENABLE_TXT_FTPM,
              OXP_TASK_DISABLE_DTPM_ONLY,
              OXP_TASK_DISABLE_FTPM_ONLY,
              OXP_TASK_PROVISION_DTPM,
              OXP_TASK_PROVISION_FTPM,
              OXP_TASK_HANDLE_TPM_NV_INDEX,
              OXP_TASK_DTPM_RESET_AUX_INDEX,
              OXP_TASK_FTPM_RESET_AUX_INDEX,
              };
//
//-----------------------------------------------------------------------------------------------------
// OOB Tasks for which the additional information MUST be deleted from OOB Output Data
//
UINT8  AdditionalInfoDeleteTaskTable[] = {
              OXP_TASK_HANDLE_TPM_NV_INDEX,
              };
//
//-----------------------------------------------------------------------------------------------------
/**
  TaskPresentInTable(): Find if the given task is present in the given table.

  @param  Ott          INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM
  @param  Table        INPUT: UINT8*, Ptr to table where task will be searched
  @param  TableEntries INPUT: UINT8, #of entries in the table

  @retval Result OUTPUT: BOOLEAN, TRUE/FALSE: Task Yes/Not present in the table
**/
BOOLEAN
TaskPresentInTable (
  IN  OOB_TXT_TPM   *Ott,
  IN  UINT8         *Table,
  IN  UINT8         TableEntries
  )
{
  // Local data
  UINT8   i, t;
  BOOLEAN a;

  t = (Ott->Header.TaskAndResult & OXP_TASK_BIT_MASK)                // Requested Task
                                         >> OXP_TASK_BIT_N;
  a = FALSE;                                                         // FALSE: Task not present in the table
  if ((Table != NULL) && (TableEntries != 0)) {                      // If supplied table valid
    for (i = 0; i < TableEntries; i++) {                             //   Search for the task in the table
      if (Table[i] == t) { a = TRUE; break; }                        //     Task found in the table, Stop
    }
  }

  return a;                                                          // TRUE/FALSE: Task Yes/Not present in the table
}

//-----------------------------------------------------------------------------------------------------
/**
  DeleteOobAdditionalInfo(): Finds whether to delete the additional information (present beyond OXP_HEADER) from OOB Output data.

  @param  Ott   INPUT: Ptr to OOB_TXT_TPM data
  @param  Opd   INPUT: Ptr to OT_PLATFORM_DATA

  @retval Result  BOOLEAN, TRUE/FALSE: Yes/Not delete the additional information from output data

  NOTE:
 1. When this function is called, the password information was already deleted from output data.
 2. This function should determine whether to delete the other additional information from output data.
**/
BOOLEAN
DeleteOobAdditionalInfo (
  IN  OOB_TXT_TPM      *Ott,
  IN  OT_PLATFORM_DATA *Opd
  )
{
  BOOLEAN b;

  if (Opd->TaskHasOutputData) {                                      // If Task has output data
    b = FALSE;                                                       //   FALSE: Do not delete additional info from output data
  } else {                                                           // If Task has no output data
    // EITHER No task is performed during this OOB execution OR Task performed during this OOB execution does not have any output data
    b = TaskPresentInTable (Ott,
          (UINT8*)(UINTN) &AdditionalInfoDeleteTaskTable, sizeof (AdditionalInfoDeleteTaskTable));
  }

  return b;                                                          // TRUE/FALSE: Yes/Not delete additional info from output data
}


//-----------------------------------------------------------------------------------------------------
/**
  AuthenticationOobTaskRequired(): Find if authentication is required for the requested task.

  @param  Ott     INPUT:  Ptr to OOB_TXT_TPM data

  @retval Result  OUTPUT: BOOLEAN, TRUE/FALSE -> Authentication Yes/Not required
**/
BOOLEAN
AuthenticationOobTaskRequired (
  IN  OOB_TXT_TPM *Ott
  )
{
  return TaskPresentInTable (Ott,
           (UINT8*)(UINTN) &AuthenticateTaskTable, sizeof (AuthenticateTaskTable));
}

//-----------------------------------------------------------------------------------------------------
/**
  TpmPreparationOobTaskRequired(): Find if TPM preparation is required for the requested task.

  @param  Ott     INPUT:  Ptr to OOB_TXT_TPM data

  @retval Result  OUTPUT: BOOLEAN, TRUE/FALSE -> TPM preparation Yes/Not required
**/
BOOLEAN
TpmPreparationOobTaskRequired (
  IN  OOB_TXT_TPM *Ott
  )
{
  return TaskPresentInTable (Ott,
           (UINT8*)(UINTN) &TpmPreparationTaskTable, sizeof (TpmPreparationTaskTable));
}

//-----------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------
/**
  CheckSame8(): Check whether all bytes in the given buffer contain the same data.

  @param  Buffer  INPUT: UINT8*, Ptr to Buffer
          Size    INPUT: UINTN, Buffer Size in bytes

  @retval Result  OUTPUT: BOOLEAN, TRUE  -> All bytes same
                                   FALSE -> All bytes Not same
**/
BOOLEAN
CheckSame8 (
  IN  UINT8    *Buffer,
  IN  UINTN    Size
  )
{
  // Local data
  UINTN    i;

  if ((Buffer != NULL) && (Size != 0)) {    // If parameter valid
    for (i = 0; i < Size; i++) {            //   Check all bytes
      if (Buffer[i] != Buffer[0]) {         //   If not same
        return FALSE; break;                //     FALSE: All bytes not same
      }
    }
  }
  return TRUE;                              // TRUE: All bytes same
}

//-----------------------------------------------------------------------------------------------------
/**
  LocateOobAdditionalInfo(): Locate the requested additional information in OOB input data.

  @param  Ott    INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM data
  @param  Id     INPUT: UINT8, Identifier of requested Additional Information

  @retval Status OUTPUT: UINTN, Ptr to additional information
                                NULL   Not found
                                Other  Found
**/
UINTN
LocateOobAdditionalInfo (
  IN OOB_TXT_TPM  *Ott,
  IN UINT8        Id
  )
{
  // Local data
  UINTN     otta;
  UINT16    l, w;
  UINT8     i, n, *p;

  DEBUG ((EFI_D_ERROR, "[OOB] LocateOobAdditionalInfo() { Locate Additional Information ID: 0x%02x\n", Id));

  // Each additional information is of the following format:
  //   Offset-0: BYTE, Identifier of this additional information
  //   Offset-1: WORD, Length of this additional information in bytes
  //   Offset-3: WORD, Attribute of this additional information
  //   Offset-5: VARIABLE (N BYTES), depends on the additional information
  otta = (UINTN) Ott;                                                // OOB_TXT_TPM Address
  p = (UINT8*)(UINTN) (otta + Ott->Header.HeaderLength);             // Ptr to first additional info
  l = Ott->Header.TotalLength - Ott->Header.HeaderLength;            // Length of all additional info
  otta += ((UINTN) Ott->Header.TotalLength);                         // End Address of all additional info + 1

  for (n = 0; ; n++) {                                               // Search for requested additional information
    // l = length of all remaining additional info in bytes, p = ptr to next additional info, otta = end address of all additional info + 1
    if ((l == 0) || (l < MIN_OXP_ADDITIONAL_LENGTH) ||               // If no more additional info OR all additional info length not valid OR
	((UINTN) p >= otta)) { p = NULL; break; }                    //    ptr to next additional info not valid -> Not found, Stop
    i = *p; w = *((UINT16*)(UINTN) (p+1));                           //   ID, Length of this additional info
    DEBUG ((EFI_D_ERROR, "    Additional Info# 0x%02x: Identifier 0x%02x, Length 0x%04x bytes\n", n, i, w));
    if ((w == 0) || (l < w)) { p = NULL; break; }                    //   If current Length of all additional info < Length of this additional info -> Not found, Stop
    if (i == Id) { break; }                                          //   If requested ID found, Stop
    l -= w;                                                          //   Update length of all remaining additional info
    p = (UINT8*)(UINTN) ((UINTN) p + (UINTN) w);                     //   Ptr to next additional info
  }

  DEBUG ((EFI_D_ERROR, "[OOB] LocateOobAdditionalInfo() } Ptr to Additional Information ID 0x%02x: 0x%08x, ", Id, p));
  if (p != NULL) { DEBUG ((EFI_D_ERROR, "Found\n")); }
  else { DEBUG ((EFI_D_ERROR, "Not Found\n")); }

  return ((UINTN) p);                                                // Return ptr to requested additional information
}

//
//-----------------------------------------------------------------------------------------------------
//--------------------- OOB Data Read, Write, Integrity, Initialization Functions { -------------------
//-----------------------------------------------------------------------------------------------------
/**
  ReadOobDataFromMeStorage(): Read OOB Data from ME Non-Volatile Storage.

  @param    Buffer     INPUT: VOID*, Ptr to buffer where data to be read
  @param    Size       INPUT/OUTPUT: UINTN*, Ptr to area having Size of buffer in bytes
                         INPUT : Ptr to area having max size of supplied buffer in bytes
                         OUTPUT: Ptr to area having size of data read in bytes

  @retval   Result     BOOLEAN, TRUE/FALSE: Success/Error
**/
BOOLEAN
ReadOobDataFromMeStorage (
  IN     VOID  *Buffer,
  IN OUT UINTN *Size
  )
{
  // LOcal data
  EFI_STATUS  s;
  UINTN       MeStatus = 0;
  BOOLEAN     b;

  DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromMeStorage() { Read OOB Data from ME Storage\n"));

  b = FALSE;                                              // Assume Error
  if ((Buffer == NULL) || (Size == NULL)) {
    DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromMeStorage() } ERROR: Ptr NULL => BUG\n"));
    return b;                                             // Error
  }

  DEBUG ((EFI_D_ERROR, "      Read from ME Storage Index 0x00, Buffer Size 0x%08x bytes\n", *Size));
  s = MeStorageEntryRead (0, (VOID*)(UINTN) &MeStatus,    // Read from ME Storage: Index, Ptr to detail Error, Ptr to buffer, Ptr to buffer size
         Buffer, (UINT32*)(UINTN) Size);
  if (s == EFI_SUCCESS) {                                 // If success
    b = TRUE;                                             //   Success
    DEBUG ((EFI_D_ERROR, "      OOB data read from ME Storage, Size 0x%08x bytes\n", *Size));
    DisplayBinaryData ((UINT8*)(UINTN) Buffer, *Size);    //   Display OOB Data
  } else { *Size = 0; }                                   // If not success, return data size = 0

  DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromMeStorage() } "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success")); }
  else { DEBUG ((EFI_D_ERROR, "ERROR")); }
  DEBUG ((EFI_D_ERROR, ": OOB Data Size = 0x%08x bytes, EFI Status 0x%08x, ME Status 0x%08x\n", *Size, s, MeStatus));

  return b;                                               // Result
}

//-----------------------------------------------------------------------------------------------------
/**
  WriteOobDataToMeStorage(): Write OOB Data to ME Non-Volatile Storage.

  @param  Buffer  INPUT: VOID*, Ptr to buffer containing data to be written
  @param  Size    INPUT: UINTN*, Ptr to area having Size of buffer in bytes

  @retval Result  BOOLEAN, TRUE/FALSE: Success/Error
**/
BOOLEAN
WriteOobDataToMeStorage (
  IN     VOID  *Buffer,
  IN OUT UINTN *Size
  )
{
  // Local data
  EFI_STATUS  s;
  UINTN       MeStatus = 0;
  BOOLEAN     b;

  DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToMeStorage() { Write OOB Data to ME Storage\n"));

  if ((Buffer == NULL) || (Size == NULL)) {
    DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToMeStorage() } ERROR: Ptr NULL => BUG\n"));
    return FALSE;                                         // Error
  }
  DEBUG ((EFI_D_ERROR, "      OOB Data Size 0x%08x bytes\n", *Size));
  if (*Size == 0) {                                       // If size 0
    DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToMeStorage() } ERROR: Data Size 0 => BUG\n"));
    return FALSE;                                         // Error
  }

  // Determine whether to write OOB data
  b = TRUE;                                               // Write required
  if (mMeDataSize != 0) {                                 // If existing data valid
    if (*Size == mOttMe->Header.TotalLength) {            //   If OOB data size same
      if ((CompareMem ((UINT8*)(UINTN) mOttMe,            //     If OOB data content same
                       (UINT8*)(UINTN) Buffer, *Size)) == 0) {
        DEBUG ((EFI_D_ERROR, "      OOB Data not modified, Write not required\n"));
        b = FALSE;                                        //     Write not required
      } else {                                            //     If OOB data content changed
        DEBUG ((EFI_D_ERROR, "      OOB Data changed, Write OOB Data\n"));
      }
    } else {                                              //   If OOB data size changed
      DEBUG ((EFI_D_ERROR, "      OOB Data size changed, Write OOB Data\n"));
    }
  } else {                                                // If existing data not valid
    DEBUG ((EFI_D_ERROR, "      Existing Data not valid, Write OOB Data\n"));
  }

  if (b) {                                                // If write required
    DEBUG ((EFI_D_ERROR, "      Write to ME Storage Index 0x00\n"));
    s = MeStorageEntryWrite (                             // Write to ME Storage
               0, (VOID*)(UINTN) &MeStatus,               //   Index, Ptr to detail Error 
               Buffer, ((UINT32)(UINTN) *Size));          //   Ptr to buffer, Buffer size
    if (s != EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToMeStorage() -> ERROR: Write ME Storage error, EFI Status 0x%08x, ME Status 0x%08x\n", s, MeStatus));
      b = FALSE;                                          // Error
    }
  } else { b = TRUE; }                                    // If write not required -> success

  DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToMeStorage() } "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { b = FALSE; DEBUG ((EFI_D_ERROR, "ERROR\n")); }

  return b;                                               // Result
}

//-----------------------------------------------------------------------------------------------------
/**
  ReadOobDataFromEfiVariable(): Read OOB Data from EFI Variable.

  @param  Buffer  INPUT: VOID*, Ptr to buffer where data to be read
  @param  Size    INPUT/OUTPUT: UINTN*, Ptr to area having Size of buffer in bytes
                                INPUT : Ptr to area having max size of supplied buffer in bytes
                                OUTPUT: Ptr to area having size of data read in bytes

  @retval Result  BOOLEAN, TRUE/FALSE: Success/Error
**/
BOOLEAN
ReadOobDataFromEfiVariable (
  IN     VOID  *Buffer,
  IN OUT UINTN *Size
  )
{
  // Local data
  BOOLEAN     b;

  DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromEfiVariable() { Read OOB Data from EFI Variable\n"));

  b = FALSE;                                              // Assume Error
  if ((Buffer == NULL) || (Size == NULL)) {
    DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromEfiVariable() } ERROR: Ptr NULL => BUG\n"));
    return b;                                             // Error
  }
  DEBUG ((EFI_D_ERROR, "      Read from EFI Variable: Buffer Size 0x%08x bytes\n", *Size));
  if ((gRT->GetVariable (EFI_OOB_TXT_TPM_DATA_VARIABLE,   // If Read OOB data success
            &mEfiOobTxtTpmDataGuid, NULL, Size, Buffer)) == EFI_SUCCESS) {
    b = TRUE;                                             //   Success
    DEBUG ((EFI_D_ERROR, "      OOB data read from EFI Storage, Size 0x%08x bytes\n", *Size));
    DisplayBinaryData ((UINT8*)(UINTN) Buffer, *Size);    //   Display OOB Data
  } else { *Size = 0; }                                   // If not success, Return data size = 0

  DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromEfiVariable() } "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success")); }
  else { DEBUG ((EFI_D_ERROR, "ERROR")); }
  DEBUG ((EFI_D_ERROR, ", OOB Data Size = 0x%08x bytes\n", *Size));

  return b;                                               // Result
}

//-----------------------------------------------------------------------------------------------------
/**
  WriteOobDataToEfiVariable(): Write OOB Data to EFI Variable.

  @param  Buffer  INPUT: VOID*,  Ptr to buffer where data to be read
  @param  Size    INPUT: UINTN*, Ptr to area having Size of buffer in bytes

  @retval Result  BOOLEAN, TRUE/FALSE: Success/Error

**/
BOOLEAN
WriteOobDataToEfiVariable (
  IN  VOID       *Buffer,
  IN  UINTN      *Size
  )
{
  // Local data
  EFI_STATUS   s;
  BOOLEAN      b;

  DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToEfiVariable() { Write OOB Data to EFI Variable\n"));

  if ((Buffer == NULL) || (Size == NULL)) {
    DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToEfiVariable() } ERROR: Ptr NULL => BUG\n"));
    return FALSE;                                         // Error
  }
  DEBUG ((EFI_D_ERROR, "      OOB Data Size = 0x%08x bytes\n", *Size));
  if (*Size == 0) {                                       // If size 0
    DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToEfiStorage() } ERROR: Data Size 0 => BUG\n"));
    return FALSE;                                         // Error
  }

  // Determine whether to write OOB data
  b = TRUE;                                               // Write required
  if (mEfiDataSize != 0) {                                // If existing data valid
    if (*Size == mOttEfi->Header.TotalLength) {           //   If OOB data size same
      if ((CompareMem ((UINT8*)(UINTN) mOttEfi,           //     If OOB data content same
                       (UINT8*)(UINTN) Buffer, *Size)) == 0) {
        DEBUG ((EFI_D_ERROR, "      OOB Data not modified, Write not required\n"));
        b = FALSE;                                        //     Write not required
      } else {                                            //     If OOB data content changed
        DEBUG ((EFI_D_ERROR, "      OOB Data changed, Write OOB Data\n"));
      }
    } else {                                              //   If OOB data size changed
      DEBUG ((EFI_D_ERROR, "      OOB Data size changed, Write OOB Data\n"));
    }
  } else {                                                // If existing data not valid
    DEBUG ((EFI_D_ERROR, "      Existing Data not valid, Write OOB Data\n"));
  }

  if (b) {                                                // If write required
    DEBUG ((EFI_D_ERROR, "      Write OOB Data to EFI Variable\n"));
    s = gRT->SetVariable (EFI_OOB_TXT_TPM_DATA_VARIABLE,  // Write OOB data
              &mEfiOobTxtTpmDataGuid,
              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
              *Size, Buffer);
    if (s != EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "      ERROR: Write EFI Variable error, EFI Status 0x%08x\n", s));
      b = FALSE;                                          // Error
    }
  } else { b = TRUE; }                                    // If write not required -> success

  DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToEfiVariable() } "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "ERROR\n")); }

  return b;                                               // Result
}

//-----------------------------------------------------------------------------------------------------
/**
  WriteOobDataToNvStorage(): Write OOB One-Touch Data to Non-Volatile Storages.

  @param  Buffer   INPUT: VOID*, Ptr to data buffer
          Size     INPUT: UINTN*, Ptr to data size
          State    INPUT: UINT32, Bit-mapped information of State

  @retval Result   BOOLEAN, TRUE -> Success (write to one or both storage successful)
                            FALSE-> Error (write to both storages not successful)
**/
BOOLEAN
WriteOobDataToNvStorage (
  IN  VOID      *Buffer,
  IN  UINTN     *Size,
  IN  UINT32    State
  )
{
  // Local data
  BOOLEAN  be, bm;

  DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToNvStorage() { Write OOB Data to Non-Volatile Storage\n"));

  be = FALSE; bm = FALSE;                                        // Assume OOB data not to write
  if ((Buffer == NULL) || (Size == NULL)) {                      // If ptr not valid
    DEBUG ((EFI_D_ERROR, "      ERROR: Ptr NULL => BUG\n"));
  } else {                                                       // If ptr valid
    DEBUG ((EFI_D_ERROR, "      OOB Data Size = 0x%08x bytes\n", *Size));
    if (*Size > mNvStorageSize) {                                //   If size not valid
      DEBUG ((EFI_D_ERROR, "      ERROR: OOB Data Not Written, Size too big\n"));
    } else { be = TRUE; }                                        //   If size valid, OOB data to write
  }

  if (be) {                                                      // If OOB data to write
    bm = TRUE;                                                   //   Assume Success
    if (*Size == 0) {                                            //   If size zero
      DEBUG ((EFI_D_ERROR, "      WARNING: OOB Data Not written, Data Size = 0\n"));
      DEBUG ((EFI_D_ERROR, "               ME Storage Not Updated, EFI Variable not updated\n"));
    } else {                                                     //   If size non-zero
      if (mMeNvStorage) {                                        //     If ME Storage supported
        if ((State & OXP_STATE_OOB_ME_DATA_UPDATE_BIT) != 0) {   //       If to be updated
          bm = WriteOobDataToMeStorage (Buffer, Size);           //         Write ME Storage
        } else {
          DEBUG ((EFI_D_ERROR, "      WARNING: ME Storage Not Supported -> Not Written\n"));
        }
      }
      if (mEfiNvStorage) {                                       //     If EFI Storage supported
        if ((State & OXP_STATE_OOB_EFI_DATA_UPDATE_BIT) != 0) {  //       If to be updated
          be = WriteOobDataToEfiVariable (Buffer, Size);         //         Write EFI Variable
        } else {
          DEBUG ((EFI_D_ERROR, "      WARNING: EFI Variable Not Supported -> Not Written\n"));
        }
      }
    }
    be = (be | bm);                                              // Result of Write
  }

  DEBUG ((EFI_D_ERROR, "[OOB] WriteOobDataToNvStorage() } "));
  if (be) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "ERROR\n")); }

  return be;                                                     // Result
}

//-----------------------------------------------------------------------------------------------------
/**
  ReadOobDataFromNvStorage(): Read OOB Data from Non-Volatile Storage.

  @param   None

  @retval  None
**/
VOID
ReadOobDataFromNvStorage (
  )
{
  DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromNvStorage() { Read OOB Data from Non-Volatile Storage\n"));

  mMeDataSize = 0; mEfiDataSize = 0;                             // Assume data not valid
  if (mMeNvStorage) {                                            // If ME Storage support
    mMeDataSize = mNvStorageSize;                                //   Max data size
    ReadOobDataFromMeStorage ((VOID*) mOttMe, &mMeDataSize);     //   Read from ME Storage
  }
  if (mEfiNvStorage) {                                           // If EFI Storage support
    mEfiDataSize = mNvStorageSize;                               //   Max data size
    ReadOobDataFromEfiVariable ((VOID*) mOttEfi, &mEfiDataSize); //   Read from EFI Variable
  }

  DEBUG ((EFI_D_ERROR, "[OOB] ReadOobDataFromNvStorage() }\n"));
}

//-----------------------------------------------------------------------------------------------------
/**
  FindOobNvStorageSize(): Find max size of OOB Non-Volatile Storage.

  @param  Size   INPUT/OUTPUT: UINTN*, Ptr to size

  @retval None
**/
VOID
FindOobNvStorageSize (
  IN OUT UINTN  *Size
  )
{
  // Local data
  UINT16   se, sm;

  se = 0; sm = 0;
  if (mMeNvStorage) { sm = ME_STORAGE_MAX_BUFFER; } // ME NV storage Max Size in bytes
  if (mEfiNvStorage) { se = sizeof (OOB_TXT_TPM); } // EFI Variable Max Size in bytes (rounded up to next para)
  if (se < sm) { se = sm; }                         // Use larger value
  se = (se + (UINT16)(0x000F)) & (UINT16)(0xFFF0);  // Max Size in bytes (rounded up to next para)

  if (Size != NULL) { *Size = (UINTN) se; }         // Return NV Storage Max Size in bytes
}

//-----------------------------------------------------------------------------------------------------
/**
  OobNvStorageSupported(): Check if OOB Non-Volatile Storage is supported.

  @param   None

  @retval  Result  BOOLEAN, TRUE/FALSE: Yes/Not supported

**/
BOOLEAN
OobNvStorageSupported (
  )
{
  mMeNvStorage = MeStorageIsSupported();            // TRUE/FALSE = ME NV storage yes/not Supported
  mEfiNvStorage = TRUE;                             // EFI Storage supported
  return TRUE;                                      // NV Storage supported
}

//-----------------------------------------------------------------------------------------------------
/**
  InitOobOneTouchData(): Initialize OOB data.

  @param  Ott  INPUT: Ptr to OOB_TXT_TPM data
  @param  Opd  INPUT: Ptr to OT_PLATFORM_DATA
  @param  Size INPUT/OUTPUT: UINTN*, Ptr to area containing OOB Data Size in bytes
                             INPUT : Ptr to area containing OOB Data Size in bytes as read from NV space
                             OUTPUT: Ptr to area containing OOB Data Size in bytes as generated or unchanged

  @retval None
**/
VOID
InitOobOneTouchData (
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINTN            *Size
  )
{
  // Local data
  OXP_DISCOVERY_INFO *odi;

  DEBUG ((EFI_D_ERROR, "[OOB] InitOobOneTouchData() {\n"));

  DEBUG ((EFI_D_ERROR, "      Use default Task 0x01: Feature Support Disovery\n"));
  ZeroMem ((VOID*) Ott, sizeof (OOB_TXT_TPM));                     // Init with 0
  Ott->Header.Signature = OXP_SIGNATURE_INPUT;                     // Signature
  Ott->Header.TotalLength = sizeof (OXP_HEADER);                   // Total Length in bytes
  Ott->Header.HeaderLength = sizeof (OXP_HEADER);                  // Header Length in bytes
  Ott->Header.Version = 0x01;                                      // Version
  Ott->Header.TaskAndResult = OXP_TASK_FEATURE_SUPPORT_DISCOVERY;  // Use-Case 1
  Ott->Header.Status = OXP_UNKNOWN_ERROR;                          // Status
  // Init OXP_DISCOVERY_INFO
  odi = (OXP_DISCOVERY_INFO*)(UINTN)                               // Ptr to OXP_DISCOVERY_INFO
           ((UINTN) Ott + Ott->Header.TotalLength);
  odi->Identifier = OXP_ID_DISCOVERY;                              // Identifier
  odi->Attribute = OXP_DISCOVERY_INFO_VALID_BIT;                   // Bit-mapped Attribute
  odi->Length = sizeof (OXP_DISCOVERY_INFO);                       // Length of OXP_DISCOVERY_NFO in bytes
  Ott->Header.TotalLength += odi->Length;                          // Total Length in bytes
  FillOobSupportedTaskInfo (                                       // Bit-mapped information of Task Supported
      (UINT8*)(UINTN) &odi->TaskSupported, sizeof (odi->TaskSupported));
  Ott->Header.Checksum = 0;                                        // Init Checksum field with 0
  Ott->Header.Checksum = CalculateCheckSum8 (                      // Update Checksum
                         (UINT8*)(UINTN) Ott, (UINTN) Ott->Header.TotalLength);
  *Size = (UINTN) Ott->Header.TotalLength;                         // OOB data size in bytes

  DEBUG ((EFI_D_ERROR, "[OOB] InitOobOneTouchData() } OOB data size 0x%08x bytes\n", *Size));
}

//-----------------------------------------------------------------------------------------------------
/**
  ValidateOobOneTouchInputData(): Validate OOB Input data (supplied by caller).

  @param  Ott     INPUT: Ptr to OOB_TXT_TPM data
  @param  Size    INPUT/OUTPUT: UINTN*, Ptr to area containing OOB Data Size in bytes
                                INPUT : Ptr to area containing OOB Data Size in bytes as read from NV space
                                OUTPUT: Ptr to area containing actual OOB Data Size in bytes (if valid) otherwise unchanged
  @param  State	  INPUT/OUTPUT: UINT32*, Ptr to state of task

  @retval Result  BOOLEAN: TRUE/FALSE -> Yes/Not Valid

  NOTE: This function validates OXP_HEADER. The OXP_ADDITIONAL_INFO is validated by the respective task.
**/
BOOLEAN
ValidateOobOneTouchInputData (
  IN     OOB_TXT_TPM  *Ott,
  IN OUT UINTN        *Size,
  IN OUT UINT32       *State
  )
{
  // Local data
  UINT32   s;

  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobOneTouchInputData() { Validate OOB Input Data\n"));
  s = 0;                                                           // State
  if ((Ott == NULL) || (Size == NULL) || (State == NULL)) {        // If ptr not valid
    DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobOneTouchInputData() } ERROR: Ptr NULL => BUG\n"));
    return FALSE;                                                  //   Error
  }
  DEBUG ((EFI_D_ERROR, "      Current State 0x%08x, OOB Data Size 0x%08x bytes\n", *State, *Size));

  // Validate OOB data integrity
  if ((Ott->Header.TaskAndResult & OXP_TASK_PERFORMED_BIT) == 0) { // If some task to be performed
    if (Ott->Header.Signature != OXP_SIGNATURE_INPUT)  {           //   If not input signature
      s |= OXP_STATE_SIGNATURE_INVALID_BIT;                        //     Indicate error
    }
  } else {                                                         // If no task to be performed
    if (Ott->Header.Signature != OXP_SIGNATURE_OUTPUT) {           //   If not output signature
      s |= OXP_STATE_SIGNATURE_INVALID_BIT;                        //     Indicate error
    }
  }
  if (Ott->Header.TotalLength == 0) {                              // If Total Length zero
    s |= OXP_STATE_TOTAL_LENGTH_INVALID_BIT;                       //   Indicate error
  } 
  if ((Ott->Header.HeaderLength == 0) ||                           // If Header Length zero OR
      (Ott->Header.HeaderLength != sizeof (OXP_HEADER))) {         //    Header Length invalid
    s |= OXP_STATE_HEADER_LENGTH_INVALID_BIT;                      //   Indicate error
  }
  if ((Ott->Header.TotalLength < Ott->Header.HeaderLength) ||      // If (Total Length < Header Length) OR
      (((UINTN) Ott->Header.TotalLength) > *Size)) {               //    (Total Length > Read OOB Data size)
    s |= OXP_STATE_TOTAL_LENGTH_LESS_HEADER_LENGTH_BIT;            //   Indicate error
  }
  if ((Ott->Header.Version == 0) ||                                // If Version not valid
      (Ott->Header.Version > OXP_SPECIFICATION_VERSION)) { 
    s |= OXP_STATE_VERSION_INVALID_BIT;                            //   Indicate error
  }
  if (s == 0) {                                                    // If success so far
    if ((CalculateSum8 ((UINT8*)(UINTN) Ott,                       // If Checksum not valid
                        (UINTN) Ott->Header.TotalLength)) != 0) { 
      s |= OXP_STATE_CHECKSUM_INVALID_BIT;                         //   Indicate error
    }
  }
  if (s != 0) {                                                    // If not success
    if (CheckSame8 ((UINT8*)(UINTN) Ott, *Size)) {                 //   If all bytes same
      s |= OXP_STATE_ALL_BYTES_SAME_BIT;                           //     Indicate error
    }
  }

  if (s == 0) { *Size = Ott->Header.TotalLength; }                 // If success, actual OOB data size in bytes
  *State |= s;                                                     // Return updated State

  DEBUG ((EFI_D_ERROR, "      Updated State 0x%08x, OOB Data Size 0x%08x bytes\n", *State, *Size));
  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobOneTouchInputData() } "));
  if (s != 0) {                                                    // If not success
    DEBUG ((EFI_D_ERROR, "ERROR (OOB Data Not Valid)\n"));
    return FALSE;                                                  //   Not Valid
  }
  DEBUG ((EFI_D_ERROR, "Success (OOB Data Valid)\n"));
  return TRUE;                                                     // Valid
}

//-----------------------------------------------------------------------------------------------------
/**
  ValidateOobAdditionalInformation(): Validate all additional information (supplied by caller beyond OXP_HEADER).

  NOTE: When this function is invoked, OXP_HEADER is already found to be valid.

  @param  Ott    INPUT: Ptr to OOB_TXT_TPM

  @retval Result UINT8, Status Code
**/
UINT8
ValidateOobAdditionalInformation (
  IN  OOB_TXT_TPM *Ott
  )
{
  // Local data
  UINTN    otta;
  UINT16   l, w;
  UINT8    e, i, n, *p;

  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobAdditionalInformation() { Validate all additional info beyond OXP_HEADER\n"));

  // Each additional information is of the following format:
  //   Offset-0: BYTE, Identifier of this additional information
  //   Offset-1: WORD, Length of this additional information in bytes
  //   Offset-3: WORD, Attribute of this additional information
  //   Offset-5: VARIABLE (N BYTES), depends on the additional information
  otta = (UINTN) Ott;                                                // Address of OOB_TXT_TPM
  p = (UINT8*)(UINTN) (otta + Ott->Header.HeaderLength);             // Ptr to first additional info after OXP_HEADER
  l = Ott->Header.TotalLength - Ott->Header.HeaderLength;            // Length of all additional info (beyond OXP_HEADER) in bytes
  otta += ((UINTN) Ott->Header.TotalLength);                         // End Address of all OOB data + 1

  e = OXP_ADDITIONAL_INFO_MALFORMED;                                 // Assume Error
  for (n = 0; ; n++) {                                               // Search for all additional info
    // l = length of remaining all additional info in bytes, p = ptr to next additional info, otta = end address of all OOB data + 1
    if (l == 0) { e = OXP_SUCCESS; break; }                          // If no more additional info, Success, Stop
    if ((UINTN) p >= otta) { break; }                                // If ptr to next additional info not valid, Error, Stop
    if (l < MIN_OXP_ADDITIONAL_LENGTH) { break; }                    // If remaining length not valid, Error, Stop
    i = *p; w = *((UINT16*)(UINTN) (p+1));                           // i, w = ID, Length of this additional info
    DEBUG ((EFI_D_ERROR, "    Additional Info-0x%02x: ID 0x%02x, Length 0x%04x\n", n, i, w));
    if ((i == 0) || (i > MAX_OXP_ADDITIONAL_INFO_ID)) { break; }     // If ID not valid, Error, Stop
    if ((w == 0) || (l < w)) { break; }                              // If Length of remaining additional info < Length of this extra info, Error, Stop
    l -= w;                                                          // Update length of remaining all additional info to be searched
    p = (UINT8*)(UINTN) ((UINTN) p + (UINTN) w);                     // Ptr to next additional info
  }

  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobAdditionalInformation() } "));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  GenerateOobOneTouchOutputData(): Generate OOB Output data (to be written to Non-Volatile storage).

  This function generates the OOB output data but MUST NOT write the data to non-volatile storage.

  @param  Ott   INPUT: Ptr to OOB_TXT_TPM data
  @param  Opd   INPUT: Ptr to OT_PLATFORM_DATA
  @param  Size  INPUT/OUTPUT: UINTN*, Ptr to area containing OOB Data Size in bytes
                                INPUT : Ptr to area containing OOB Input Data Size in bytes
                                 OUTPUT: Ptr to area containing OOB Output Data Size in bytes
  @param  State INPUT/OUTPUT: UINT32*, Ptr to state of task

  @retval None

**/
VOID
GenerateOobOneTouchOutputData (
  IN     OOB_TXT_TPM       *Ott,
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINTN             *Size,
  IN OUT UINT32            *State
  )
{
  // Local data
  OXP_PASSWORD_INFO  *pwd;
  UINTN              en, ex;
  UINT32             s;
  UINT16             w;
  UINT8              *p;

  DEBUG ((EFI_D_ERROR, "[OOB] GenerateOobOneTouchOutputData() { Generate OOB Output Data\n"));
  s = *State;                                                        // State
  w = 0;                                                             // #of bytes deleted from OOB output data

  if ((s & OXP_STATE_TASK_PENDING_BIT) == 0) {                       // If Task not pending
    // Clear password info (if present) beyond OXP_HEADER from OOB input data  (to delete password info from non-volatile storage)
    pwd = (OXP_PASSWORD_INFO*)(UINTN)                                //   Locate OXP_PASSWORD_INFO
                             LocateOobAdditionalInfo (Ott, OXP_ID_PASSWORD);
    if (pwd != NULL) {                                               //   If OXP_PASSWORD_INFO found
      if ((pwd->Attribute & OXP_PASSWORD_INFO_VALID_BIT) != 0) {     //     If password info valid
        p = (UINT8*)(UINTN) (pwd); w = pwd->Length;                  //       Ptr to password information, Length of password information in bytes
        if (w != 0) {                                                //       If password info present
          DEBUG ((EFI_D_ERROR, "    Delete Password information (size 0x%04x bytes) from OOB Output Data\n", w));
          ZeroMem (p, w);                                            //         Clear information
          s |= OXP_STATE_DATA_DELETED_OUTPUT_BIT;                    //         Info deleted from output data
          // Move other information (if any) to make output data contiguous
          ex = ((UINTN) p + (UINTN) w);                              //         Ptr to start of any other info after Password info
          en = ((UINTN) Ott + (UINTN) Ott->Header.TotalLength);      //         Ptr to 1st byte after all OOB data
          if (ex < en) {                                             //         If any info present after Password info
            DEBUG ((EFI_D_ERROR, "    Move other info (size 0x%04x bytes) to make OOB data contiguous\n", (en - ex)));
            CopyMem (p, (UINT8*)(UINTN) ex, (en - ex));              //           Move other info to proper place to make OOB data contiguous
          }
          Ott->Header.TotalLength -= w;                              //         Update Total Length in bytes
        }
      }
    }
    // Find whether to delete other additional information from output OOB data
    if (DeleteOobAdditionalInfo (Ott, Opd)) {                        //   If Additional Info to be deleted from output data
      // Clear all additional info (if present) beyond OXP_HEADER (to delete all additional info from non-volatile storage)
      p = (UINT8*)(UINTN)                                            //     Ptr to additional information
                    ((UINTN) Ott + (UINTN) Ott->Header.HeaderLength);
      w = Ott->Header.TotalLength - Ott->Header.HeaderLength;        //     Length of additional information in bytes
      if (w != 0) {                                                  //     If extra info supplied
        DEBUG ((EFI_D_ERROR, "    Delete all extra information (size 0x%04x bytes) from OOB Output Data\n", w));
        ZeroMem (p, w);                                              //       Clear information
        s |= OXP_STATE_DATA_DELETED_OUTPUT_BIT;                      //       Info deleted from output data
        Ott->Header.TotalLength -= w;                                //       Update Total Length in bytes
      }
    }

    Ott->Header.Signature = OXP_SIGNATURE_OUTPUT;                    //   Output Signature
  }

  // Update different state, checksum in OXP_HEADER
  Ott->Header.State = s;                                             // Update state in OOB data
  Ott->Header.Checksum = 0;                                          // Init Checksum with 0
  Ott->Header.Checksum = CalculateCheckSum8 (                        // Update Checksum
             (UINT8*)(UINTN) Ott, (UINTN) Ott->Header.TotalLength);

  // Return necessary information
  *State = s;                                                        // Updated State
  *Size = Ott->Header.TotalLength;                                   // OOB Output data size in bytes
  DEBUG ((EFI_D_ERROR, "[OOB] GenerateOobOneTouchOutputData() } State 0x%08x, OOB Output Data Size 0x%04x bytes\n", s, *Size));
}

//-----------------------------------------------------------------------------------------------------
/**
  MakeOobAdditionalInformationContiguous(): Make OOB data contiguous if necessary.

  This function make OOB additional information contiguous if given additional information length is changed.

  @param  Ott    INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM
  @param  Ai     INPUT: UINT8*, Ptr to given aditional information, length of which may have been changed
  @param  Osize  INPUT: UINT16, Original size of given aditional information in bytes

  @retval Result UINT8, Status Code
**/
UINT8
MakeOobAdditionalInformationContiguous (
  IN  OOB_TXT_TPM  *Ott,
  IN  UINT8        *Ai,
  IN  UINT16       Osize
  )
{
  // Local data
  UINTN    en, ex, oe;
  UINT16   w;
  UINT8    e, i;

  DEBUG ((EFI_D_ERROR, "[OOB] MakeOobAdditionalInformationContiguous() { Make OOB Data contiguous, if necessary\n"));

  // Each additional information is of the following format:
  //   Offset-0: BYTE, Identifier of this additional information
  //   Offset-1: WORD, Length of this additional information in bytes
  //   Offset-3: WORD, Attribute of this additional information
  //   Offset-5: VARIABLE (N BYTES), depends on the additional information

  e = OXP_UNKNOWN_ERROR;                                             // Assume error
  if ((Ott != NULL) && (Ai != NULL)) {                               // If ptrs valid
    i = *Ai; w = *((UINT16*)(UINTN) (Ai + 1));                       //   i, w = ID, Length of this additional info
    if (i <= MAX_OXP_ADDITIONAL_INFO_ID) {                           //   If Identifier valid
      e = OXP_SUCCESS;                                               //     Indicate success
      if (w < Osize) {                                               //     If additional info length decreased
        DEBUG ((EFI_D_ERROR, "    Given Additional info Identifier 0x%02x: Length changed -> Original Length 0x%04x, Modified Length 0x%04x\n", i, Osize, w));
        en = (UINTN) Ott;                                            //       Address of OOB_TXT_TPM
        en += ((UINTN) Ott->Header.TotalLength);                     //       Address of (End of all OOB data + 1) as supplied
        // Move other information (if any) to make OOB data contiguous
        oe = ((UINTN) Ai + (UINTN) Osize);                           //       Address of start of any other info after this additional info before modification
        if (oe < en) {                                               //       If any other additional info present after this additional info
          ex = ((UINTN) Ai + (UINTN) w);                             //         Address of start of any other info after this modified additional info
          DEBUG ((EFI_D_ERROR, "    Move other additional info (size 0x%04x bytes) to make OOB data contiguous\n", (en - oe)));
          CopyMem ((UINT8*)(UINTN) ex, (UINT8*)(UINTN) oe, (en - oe));//         Move other info to proper place to make OOB data contiguous
          Ott->Header.TotalLength -= (Osize - w);                    //         Update Total Length in OXP_HEADER
        } else {                                                     //       If any other additional info not present after this additional info
          DEBUG ((EFI_D_ERROR, "    No more additional info after given additional info -> No data movenment necessary\n"));
        }
      } else {                                                       //     If additional info length not decreased
        if (w > Osize) {                                             //       If additional info length increased
          DEBUG ((EFI_D_ERROR, "    Given Additional info Length Increased ==> BUG\n", i));
        }
      }
    } else {                                                         //   If Identifier not valid
      DEBUG ((EFI_D_ERROR, "    Given Additional info Identifier 0x%02x -> Not Valid ==> BUG\n", i));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] MakeOobAdditionalInformationContiguous() } "));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//--------------------- OOB Data Read, Write, Integrity, Initialization Functions } -------------------

//-----------------------------------------------------------------------------------------------------
//--------------------- Password Authentication Functions { -------------------------------------------
//-----------------------------------------------------------------------------------------------------
/**
  MatchOobPasswordAscii(): Check if given ASCII password matches with BIOS password.

  @param   Oxpd   INPUT: OXP_PASSWORD_INFO*, Ptr to OXP_PASSWORD_INFO
  @param   Opd    INPUT: Ptr to OT_PLATFORM_DATA

  @retval  Status OUTPUT: UINT8, Status code

**/
UINT8
MatchOobPasswordAscii (
  IN OXP_PASSWORD_INFO *Oxpd,
  IN OT_PLATFORM_DATA  *Opd
  )
{
  // Local data
  UINT32  i, n;
  CHAR16  c[PASSWORD_MAX_SIZE+1];
  UINT8   b, e, *p;

  DEBUG ((EFI_D_ERROR, "[OOB] MatchOobPasswordAscii() { Match ASCII Password\n"));

  // Init local data
  e = OXP_SUCCESS;                                                   // Assume success
  p = (UINT8*)(UINTN) &Oxpd->Password;                               // Ptr to Password
  n = (UINT32)(UINTN) (((UINTN) Oxpd->Length) -                      // Length of suplied password in bytes
                        ((UINTN) p - (UINTN) Oxpd));
  i = 0;

  // Ensure internal data area of enough space to accommodate Unicode string generated from ASCII string
  DEBUG ((EFI_D_ERROR, "                Size of Internal Array to generate Unicode String: 0x%08x bytes\n", sizeof (c)));
  DEBUG ((EFI_D_ERROR, "      Size of Password ASCII String (including NULL terminator): 0x%08x bytes\n", n));
  if (n > (sizeof (c) / sizeof (CHAR16))) {                          // If password information array size > internal space size
    DEBUG ((EFI_D_ERROR, "    Password too large -> Error\n"));
    e = OXP_PASSWORD_TOO_LARGE;                                      //   Error
  } else {
  // Convert ASCII string to Unicode string
  ZeroMem ((VOID*) &c[0], sizeof (c));                               // Init unicode string area with 0
  b = 0xFF;                                                          // Assume NULL terminator not found
  for (i = 0; i < n; i++) {                                          // Search for NULL terminator
    b = p[i];                                                        //   Get next ASCII character
    c[i] = (UINT16)(UINTN) b;                                        //   Store as Unicode character
    if (b == 0) { break; }                                           //   NULL found, Stop
  }

  // b = 00, NULL terminator found, i = 1-based string length (in characters) excluding NULL terminator
  //   <>00, NULL terminator not found
  if (b != 0) {                                                      // If NULL terminator not found
      DEBUG ((EFI_D_ERROR, "    Supplied Password string not NULL terminated -> Error\n"));
      e = OXP_PASSWORD_NOT_NULL_TERMINATED;                          //   Error
  } else {                                                           // If NULL terminator found
    if (i == 0) {                                                    //   If Password empty
      DEBUG ((EFI_D_ERROR, "    Supplied Password string empty -> Error\n"));
      e = OXP_PASSWORD_EMPTY;                                        //     Error
      }
    }
  }

  // Match password: s = state, i = 1-based string length (in characters) excluding NULL terminator
  //   p = ptr to given ASCII string, c = Ptr to converted Unicode string
  if (e == OXP_SUCCESS) {                                            // If success so far
    // Display ASCII string and Display Unicode string
    DEBUG ((EFI_D_ERROR, "                Input ASCIIZ String, Length 0x%08x bytes (including NULL terminator):\n", (i+1)));
    DisplayBinaryData (p, (i+1));                                    //   Display ASCIIZ String
    DEBUG ((EFI_D_ERROR, "                Converted UnicodeZ String, Length 0x%08x bytes (including NULL terminator):\n", ((i+1)*2)));
    DisplayBinaryData ((UINT8*)(UINTN) &c[0], sizeof (c));           //   Display UnicodeZ String

    // Match given password with BIOS Administrative password
    if (!(OT_MatchAdminPassword (Opd->SystemConfig, &c[0]))) {       // If not match
      e = OXP_AUTHENTICATION_ERROR;                                  //   Error
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] MatchOobPasswordAscii() } "));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  MatchOobPasswordUnicode(): Check if given Unicode password matches with BIOS password.

  @param   Oxpd   INPUT: OXP_PASSWORD_INFO*, Ptr to OXP_PASSWORD_INFO
  @param   Opd    INPUT: Ptr to OT_PLATFORM_DATA

  @retval  Status OUTPUT: UINT8, Status code

**/
UINT8
MatchOobPasswordUnicode (
  IN OXP_PASSWORD_INFO *Oxpd,
  IN OT_PLATFORM_DATA  *Opd
  )
{
  // Local data
  UINT32 i, n;
  CHAR16 w, *p, c[PASSWORD_DATA_SIZE_B];
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] MatchOobPasswordUnicode() { Match Unicode Password\n"));

  // Init local data
  e = OXP_SUCCESS;                                                   // Assume success
  p = (CHAR16*)(UINTN) &Oxpd->Password;                              // Ptr to Password
  n = (UINT32)(UINTN) (((UINTN) Oxpd->Length) -                      // Length of suplied password in bytes
                        ((UINTN) p - (UINTN) Oxpd));
  i = 0;

  // Ensure internal data area of enough space to accommodate Unicode string
  DEBUG ((EFI_D_ERROR, "                Size of Internal Array for Unicode String: 0x%08x bytes\n", sizeof (c)));
  DEBUG ((EFI_D_ERROR, "      Size of Password Unicode String (including NULL terminator): 0x%08x bytes\n", n));
  if ((n > sizeof (c)) || (n > sizeof (p))) {                                              // If password information array size > internal space size
    DEBUG ((EFI_D_ERROR, "    Password too large -> Error\n"));
    e = OXP_PASSWORD_TOO_LARGE;                                      //   Error
  } else {
  // Copy given password to internal area: p = Ptr to Password information array
  //   n = Length of password information array in bytes
  ZeroMem ((VOID*) &c[0], sizeof (c));                               // Init unicode string area with 0
  n >>= 1;                                                           // Length of password information array in characters
  w = 0xFFFF;                                                        // Assume NULL terminator not found
  for (i = 0; i < n; i++) {                                          // Search for NULL terminator
    w = p[i]; c[i] = w;                                              // Copy next char to internal area
    if (w == 0x0000) { break; }                                      // NULL found, Stop
  }

  // w = 00, NULL terminator found, i = 1-based string length (in characters) excluding NULL terminator
  //   <>00, NULL terminator not found
  if (w != 0) {                                                      // If NULL terminator not found
      DEBUG ((EFI_D_ERROR, "    Supplied Password string not NULL terminated -> Error\n"));
      e = OXP_PASSWORD_NOT_NULL_TERMINATED;                          //   Error
  } else {                                                           // If NULL terminator found
    if (i == 0) {                                                    //   If Password empty
      DEBUG ((EFI_D_ERROR, "    Supplied Password string empty -> Error\n"));
      e = OXP_PASSWORD_EMPTY;                                        //     Error
      }
    }
  }

  // Match password: i = string length (in characters) without NULL terminator
  //   c = ptr to internal Unicode string
  if (e == OXP_SUCCESS) {                                            // If success so far
    // Display Unicode string
    DEBUG ((EFI_D_ERROR, "                Input UnicodeZ String, Length 0x%08x bytes (including NULL terminator):\n", ((i+1) * 2)));
    DisplayBinaryData ((UINT8*)(UINTN) &c[0], sizeof (c));           //   Display UnicodeZ String

    // Match given password with BIOS Administrative password
    if (!(OT_MatchAdminPassword (Opd->SystemConfig, &c[0]))) {       // If not match
      e = OXP_AUTHENTICATION_ERROR;                                  //   Error
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] MatchOobPasswordUnicode() } "));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  ValidateOobPasswordInfo(): Validate supplied password related information.

  @param[in]  Ott    INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM data
  @param[in]  Oxpd   INPUT: OXP_PASSWORD_INFO*, Ptr to OXP_PASSWORD_INFO

  @retval     Result OUTPUT: UINT8, OXP_SUCCESS  Success
                             Other  Error
**/
UINT8
ValidateOobPasswordInfo (
  IN     OOB_TXT_TPM    *Ott,
  IN  OXP_PASSWORD_INFO *Oxpd
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobPasswordInfo() { Validate Password related information\n"));

  e = OXP_SUCCESS;                                                   // Assume success
  if (Oxpd == NULL) {                                                // If OXP_PASSWORD_INFO not found
    e = OXP_PASSWORD_INFO_NOT_FOUND;                                 //   Indicate not found
    DEBUG ((EFI_D_ERROR, "    OXP_PASSWORD_INFO not found\n"));
  } else {                                                           // If OXP_PASSWORD_INFO found
    if ((Oxpd->Length == 0) || (Oxpd->Length < OXP_PASSWORD_INFO_MIN_LENGTH) ||   //   If length not valid
      (Ott->Header.TotalLength < (Ott->Header.HeaderLength + Oxpd->Length))) {
      e = OXP_INFO_LENGTH_NOT_VALID;                                 //   Indicate info length not valid
      DEBUG ((EFI_D_ERROR, "    OXP_PASSWORD_INFO length not valid\n"));
    } else {                                                         //   If length valid
      if ((Oxpd->Attribute & OXP_PASSWORD_INFO_VALID_BIT) == 0) {    // If password data not valid
        e = OXP_INFO_VALID_NOT_SET;                                  //   Indicate info not valid
        DEBUG ((EFI_D_ERROR, "    Supplied Password Data Not Valid, OXP_PASSWORD_INFO.Attribute = 0x%04x\n",Oxpd->Attribute));
      }
    }
  }

  if (e == OXP_SUCCESS) {                                            // If success so far
    if ((Ott->Header.PasswordAttribute & OXP_PASSWORD_ADMIN_INSTALLED_BIT) == 0) { // If BIOS Administrative password not installed
      e = OXP_PASSWORD_BIOS_ADMIN_NOT_INSTALLED;                       //   Error
      DEBUG ((EFI_D_ERROR, "      BIOS Administrative Password Not Installed\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobPasswordInfo() } Status 0x%02x, ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  IfCheckOobPassword(): Determine whether to check/ignore password during authentication.

  @param[in]  Ott    INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM data
  @param[in]  State  INPUT/OUTPUT: UINT32*, Ptr to state of task

  @retval     Result OUTPUT: BOOLEAN	TRUE/FALSE -> Check/Ignore password during authentication

**/
BOOLEAN
IfCheckOobPassword (
  IN     OOB_TXT_TPM *Ott,
  IN OUT UINT32      *State
  )
{
  // Local data
  UINT32  s;
  UINT16  w;
  BOOLEAN b;

  DEBUG ((EFI_D_ERROR, "[OOB] IfCheckOobPassword() { Find whether to check/ignore password\n"));

//  Algorithm:
//    Step-0: If the caller asks to ignore the password, return FALSE.
//    Step-1: If the caller asks to check the password, the following truth-table is used to
//      determine whether to check/ignore password during authentication
//              |---------------|---------------|------------------------|
//              | BIOS          | Caller supply | Action                 |
//              | Administrative| Password in   |                        |
//              | Password      | OOB data      |                        |
//              |---------------|---------------|------------------------|
//        Case-0| Not Installed |       No      | FALSE: Ignore Password |
//        Case-1| Not Installed |       Yes     | FALSE: Ignore Password |
//        Case-2| Installed     |       No      | FALSE: Ignore Password |
//        Case-3| Installed     |       Yes     | TRUE : Check Password  |
//              |---------------|---------------|------------------------|
//       Summary of Truth-Table: If BIOS Administrative Password is installed AND the caller
//       has supplied the password in OOB data, return TRUE else return FALSE.

  s = OXP_STATE_PASSWORD_IGNORED_BIT; b = FALSE;                     // State, Ignore password, Supplied Password Attribute
  w = Ott->Header.PasswordAttribute;                                 // Supplied Password Attribute

  if ((w & OXP_PASSWORD_IGNORE_BIT) == 0) {                          // If caller asks to check password
    if (((w & OXP_PASSWORD_ADMIN_INSTALLED_BIT) != 0) &&             //   If BIOS Administrative password installed AND
         ((Ott->Header.TotalLength - Ott->Header.HeaderLength) != 0)) {      //      Caller has supplied password
      s = 0; b = TRUE;                                               //     TRUE: Check Password
    }
  }

  *State |= s;                                                       // Return updated state

  DEBUG ((EFI_D_ERROR, "[OOB] IfCheckOobPassword() } Updated State 0x%08x, ", *State));
  if (b) { DEBUG ((EFI_D_ERROR, "Check password\n")); }
  else { DEBUG ((EFI_D_ERROR, "Ignore password\n")); }

  return b;                                                          // TRUE/FALSE: Check/Ignore Password
}

//-----------------------------------------------------------------------------------------------------
/**
  AuthenticateOobPassword(): Authenticate supplied password data, if necessary.

  @param[in]  Ott    INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT: OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: UINT32*, Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code
                             OXP_SUCCESS EITHER Authentication success OR Authentication not required
                             Other       Authentication error
**/
UINT8
AuthenticateOobPassword (
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  OXP_PASSWORD_INFO  *pwd;
  UINT32             s;
  UINT8              e;

  DEBUG ((EFI_D_ERROR, "[OOB] AuthenticateOobPassword() { Authenticate OOB Password\n"));

  s = 0; e = OXP_SUCCESS;                                            // Init local data
  pwd = (OXP_PASSWORD_INFO*)(UINTN)                                  // Locate OXP_PASSWORD_INFO
                 LocateOobAdditionalInfo (Ott, OXP_ID_PASSWORD);

  // Determine whether to check/ignore password during authentication
  if (IfCheckOobPassword (Ott, &s)) {                                // If password to be checked
    e = ValidateOobPasswordInfo (Ott, pwd);                          // Validate password related info in OOB data
  // Match Password: s = State, e = status
    if (e == OXP_SUCCESS) {                                          // If success so far
      e = OXP_PASSWORD_TYPE_NOT_SUPPORTED;                           //     Assume Error
      switch (pwd->Attribute & OXP_PASSWORD_TYPE_BIT_MASK) {
        case OXP_PASSWORD_ASCII_BIT:                                 // ASCII Password
          if ((Opd->PasswordAttribute & OXP_PASSWORD_ASCII_BIT) != 0) {// If password string type supported
            e = MatchOobPasswordAscii (pwd, Opd);                      //     Match password
          }
          break;

        case OXP_PASSWORD_UNICODE_BIT:                               // Unicode Password
          if ((Opd->PasswordAttribute & OXP_PASSWORD_UNICODE_BIT) != 0) {// If password string type supported
            e = MatchOobPasswordUnicode (pwd, Opd);                  //     Match password
          }
          break;

        default:                                                     // Invalid password type
          break;
      }
    }
  }

  *State |= s;                                                       // Return updated state
  DEBUG ((EFI_D_ERROR, "          Updated State 0x%08x, Status 0x%02x\n", *State, e));

  DEBUG ((EFI_D_ERROR, "[OOB] AuthenticateOobPassword() } "));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//--------------------- Password Authentication Functions } -------------------------------------------

//--------------------- TME/MK-TME Information Functions { --------------------------------------------
//-----------------------------------------------------------------------------------------------------
/**
  ValidateOobMkTmeInfo(): Validate supplied TME/MK-TME information.

  @param  Ott    INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM
  @param  Omti   INPUT: OXP_MK_TME_INFO*, Ptr to OXP_MK_TME_INFO
  @param  State  INPUT/OUTPUT: UINT32*, Ptr to state of task

  @retval Result OUTPUT: UINT8, OXP_SUCCESS Success
                                Other  Error
**/
UINT8
ValidateOobMkTmeInfo (
  IN     OOB_TXT_TPM     *Ott,
  IN     OXP_MK_TME_INFO *Omti,
  IN OUT UINT32          *State
  )
{
  // Local data
  UINT32 s;
  UINT8  e;

  e = OXP_SUCCESS;                                                   // Assume success
  s = *State;                                                        // Current state of task

  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobMkTmeInfo() { Validate TME/MK-TME Information, Current State 0x%08x\n", s));

  DEBUG ((EFI_D_ERROR, "    Input OOB data before validation of OXP_MK_TME_INFO\n"));
  DisplayBinaryData ((UINT8*)(UINTN) Ott, Ott->Header.TotalLength);

  if (Omti == NULL) {                                                // If OXP_MK_TME_INFO not found
    e = OXP_ADDITIONAL_INFO_NOT_PRESENT;                             //   Indicate not found
    DEBUG ((EFI_D_ERROR, "    OXP_MK_TME_INFO not found\n"));
  } else {                                                           // If OXP_TPM_NV_INFO found
    DEBUG ((EFI_D_ERROR, "    Supplied OXP_MK_TME_INFO: Length 0x%04x bytes, Attribute 0x%04x\n", Omti->Length, Omti->Attribute));
    if ((Omti->Length == 0) || (Omti->Length < OXP_MK_TME_INFO_MIN_LENGTH) || //   If length not valid
         (Ott->Header.TotalLength < (Ott->Header.HeaderLength + Omti->Length))) {
      e = OXP_INFO_LENGTH_NOT_VALID;                                 //     Indicate info length not valid
      DEBUG ((EFI_D_ERROR, "    OXP_MK_TME_INFO length not valid\n"));
    } else {                                                         //   If length valid
      if ((Omti->Attribute & OXP_MTI_INFO_VALID_BIT) == 0) {         //     If info not valid
        e = OXP_INFO_VALID_NOT_SET;                                  //       Indicate info not valid
        DEBUG ((EFI_D_ERROR, "    Supplied TME/MK-TME Information Not Valid, OXP_MK_TME_INFO.Attribute 0x%04x\n", Omti->Attribute));
      }
    }
  }

  *State = s;                                                        // Return updated state
  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobMkTmeInfo() } Status 0x%02x, ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  CheckHandleMkTme(): TME/MK-TME Handler.

  @param[in]  Auth   INPUT: Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Opd    INPUT: Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task
  @param[in]  Omti   INPUT: Ptr to OXP_MK_TME_INFO

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
CheckHandleMkTme (
  IN     TPM2B_AUTH       *Auth,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State,
  IN     OXP_MK_TME_INFO  *Omti
  )
{
  // Local data
  UINT16  a, ts;
  UINT8   e;
  BOOLEAN tme, mktme, *t, *m;

  DEBUG ((EFI_D_ERROR, "[OOB] CheckHandleMkTme() { Check and handle TME/MK-TME: Input Attribute 0x%04x\n", Omti->Attribute));

  t = NULL; m = NULL; tme = FALSE; mktme = FALSE;                    // Ptr to TME Operation, Ptr to MK-TME Operation, Disable TME, Disable MK-TME
  e = OXP_SUCCESS; ts = OXP_MTI_STATUS_TASK_INITIATED;               // Assume Success
  a = Omti->Attribute & (OXP_MTI_TASK_BIT_MASK);                     // Bit-mapped information of requested task

  if (a != 0) {                                                      // If some task requested
    if (a & OXP_MTI_PROGRAM_KEY_BIT) {                               //   If Key program requested
      ts = OXP_MTI_STATUS_PROGRAM_KEY_NOT_SUPPORTED;                 //     Key program not supported, Error
    }
  } else { ts = OXP_MTI_STATUS_TASK_NONE; }                          // No task requested

  if (ts == OXP_MTI_STATUS_TASK_INITIATED) {                         // If success so far
    switch (a) {                                                     // Handle TME/MK-TME Operation
      case OXP_MTI_ENABLE_TME_BIT:                                   // Enable TME
        DEBUG ((EFI_D_ERROR, "    Enable TME ->\n"));
        if ((Opd->FeatureSupported & OXP_TME_SUPPORT_BIT) != 0) {    // If TME Supported
          if ((Opd->FeatureEnabled & OXP_TME_ENABLED_BIT) == 0) {    //   If TME not Enabled
            tme = TRUE; t = &tme;                                    //       Enable TME, Ptr to TME Operation
          } else { ts = OXP_MTI_STATUS_TME_ALREADY_IN_STATE; }       //   If TME already enabled, Error
        } else { ts = OXP_MTI_STATUS_TME_NOT_SUPPORTED; }            // If TME not Supported, Error
        break;

      case OXP_MTI_ENABLE_MKTME_BIT:                                 // Enable MK-TME
      case (OXP_MTI_ENABLE_TME_BIT | OXP_MTI_ENABLE_MKTME_BIT):      // Enable TME and MK-TME
        DEBUG ((EFI_D_ERROR, "    Enable TME and MK-TME ->\n"));
        if ((Opd->FeatureSupported & OXP_MKTME_SUPPORT_BIT) != 0) {  // If MK-TME Supported
          if ((Opd->FeatureEnabled & OXP_MKTME_ENABLED_BIT) == 0) {  //   If MK-TME not Enabled
            tme = TRUE; t = &tme;                                    //     Enable TME, Ptr to TME Operation
            mktme = TRUE; m = &mktme;                                //     Enable MK-TME, Ptr to MK-TME Operation
          } else { ts = OXP_MTI_STATUS_MKTME_ALREADY_IN_STATE; }     //   If MK-TME already enabled, Error
        } else { ts = OXP_MTI_STATUS_MKTME_NOT_SUPPORTED; }          // If MK-TME not Supported, Error
        break;

      case OXP_MTI_DISABLE_TME_BIT:                                  // Disable TME
      case (OXP_MTI_DISABLE_TME_BIT | OXP_MTI_DISABLE_MKTME_BIT):    // Disable TME and MK-TME
        DEBUG ((EFI_D_ERROR, "    Disable TME and MK-TME ->\n"));
        if ((Opd->FeatureSupported & OXP_TME_SUPPORT_BIT) != 0) {    // If TME Supported
          if ((Opd->FeatureEnabled & OXP_TME_ENABLED_BIT) != 0) {    //   If TME Enabled
            tme = FALSE; t = &tme;                                   //     Disable TME, Ptr to TME Operation
            mktme = FALSE; m = &mktme;                               //     Disable MK-TME, Ptr to MK-TME Operation
          } else { ts = OXP_MTI_STATUS_TME_ALREADY_IN_STATE; }       //   If TME already disabled, Error
        } else { ts = OXP_MTI_STATUS_TME_NOT_SUPPORTED; }            // If TME not Supported, Error
        break;

      case OXP_MTI_DISABLE_MKTME_BIT:                                // Disable MK-TME
        DEBUG ((EFI_D_ERROR, "    Disable MK-TME ->\n"));
        if ((Opd->FeatureSupported & OXP_MKTME_SUPPORT_BIT) != 0) {  // If MK-TME Supported
          if ((Opd->FeatureEnabled & OXP_MKTME_ENABLED_BIT) != 0) {  //   If MK-TME Disabled
            mktme = FALSE; m = &mktme;                               //     Disable MK-TME, Ptr to MK-TME Operation
          } else { ts = OXP_MTI_STATUS_MKTME_ALREADY_IN_STATE; }     //   If MK-TME already disabled, Error
        } else { ts = OXP_MTI_STATUS_MKTME_NOT_SUPPORTED; }          // If MK-TME not Supported, Error
        break;

      default:                                                       // Invalid operation
        DEBUG ((EFI_D_ERROR, "    TME/MK-TME Task -> Invalid\n"));
        ts = OXP_MTI_STATUS_TASK_INVALID;                            // Error
        break;
    }
  }
  if (ts != OXP_MTI_STATUS_TASK_INITIATED) { e = OXP_INVALID_TASK; } // Error

  // t = Ptr to TME task (NULL = do not change TME settings), m = Ptr to MK-TME task (NULL = do not change MK-TME settings)
  // e = Status, ts = task status
  if ((e == OXP_SUCCESS) && (ts == OXP_MTI_STATUS_TASK_INITIATED)) { // If success so far
    e = OT_HandleMkTme (Opd, t, m);                                  //     Perform TME/MK-TME Operation
    *State |= OXP_STATE_RESET_BIT;                                   //     Indicate reset required
  }

  // e = status, ts = task status
  Omti->MaxKeys = Opd->MaxMkTmeKeys;                                 // Max #of MK-TME keys available for use
  Omti->MaxKeyIdBits = Opd->MaxMkTmeKeyIdBits;                       // Max #of bits allowed for use as Key Identifiers for MK-TME
  Omti->Attribute &= ~(OXP_MTI_STATUS_BIT_MASK);                     // Clear status bits
  Omti->Attribute |= ((ts << OXP_MTI_STATUS_BIT_N)                   // Set status bits
                          & (OXP_MTI_STATUS_BIT_MASK));

  DEBUG ((EFI_D_ERROR, "[OOB] CheckHandleMkTme() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//--------------------- TME/MK-TME Information Functions } --------------------------------------------

//--------------------- PFR Information Functions { ---------------------------------------------------
//-----------------------------------------------------------------------------------------------------
/**
  ValidateOobPfrInfo(): Validate supplied PFR information.

  @param  Ott    INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM
  @param  Opi    INPUT: OXP_PFR_INFO*, Ptr to OXP_PFR_INFO
  @param  State  INPUT/OUTPUT: UINT32*, Ptr to state of task

  @retval Result OUTPUT: UINT8, OXP_SUCCESS Success
                                Other  Error
**/
UINT8
ValidateOobPfrInfo (
  IN     OOB_TXT_TPM   *Ott,
  IN     OXP_PFR_INFO  *Opi,
  IN OUT UINT32        *State
  )
{
  // Local data
  UINT32 s;
  UINT8  e;

  e = OXP_SUCCESS;                                           // Assume success
  s = *State;                                                // Current state of task

  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobPfrInfo() { Validate PFR Information, Current State 0x%08x\n", s));

  DEBUG ((EFI_D_ERROR, "    Input OOB data before validation of OXP_PFR_INFO\n"));
  DisplayBinaryData ((UINT8*)(UINTN) Ott, Ott->Header.TotalLength);

  if (Opi == NULL) {                                         // If OXP_PFR_INFO not found
    e = OXP_ADDITIONAL_INFO_NOT_PRESENT;                     //   Indicate not found
    DEBUG ((EFI_D_ERROR, "    OXP_PFR_INFO not found\n"));
  } else {                                                   // If OXP_TPM_NV_INFO found
    DEBUG ((EFI_D_ERROR, "    Supplied OXP_PFR_INFO: Length 0x%04x bytes, Attribute 0x%04x\n", Opi->Length, Opi->Attribute));
    if ((Opi->Length == 0) ||                                //   If length not valid
        (Opi->Length < OXP_PFR_INFO_MIN_LENGTH) ||
        (Ott->Header.TotalLength < (Ott->Header.HeaderLength + Opi->Length))) {
      e = OXP_INFO_LENGTH_NOT_VALID;                         //     Indicate info length not valid
      DEBUG ((EFI_D_ERROR, "    OXP_PFR_INFO length not valid\n"));
    } else {                                                 //   If length valid
      if ((Opi->Attribute & OXP_PI_INFO_VALID_BIT) == 0) {   //     If info not valid
        e = OXP_INFO_VALID_NOT_SET;                          //       Indicate info not valid
        DEBUG ((EFI_D_ERROR, "    Supplied PFR Information Not Valid, OXP_PFR_INFO.Attribute 0x%04x\n", Opi->Attribute));
      }
    }
  }

  *State = s;                                                // Return updated state
  DEBUG ((EFI_D_ERROR, "[OOB] ValidateOobPfrInfo() } Status 0x%02x, ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                  // Status code
}

//-----------------------------------------------------------------------------------------------------
/**
  CheckHandlePfr(): PFR Handler.

  @param[in]  Auth   INPUT: Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Opd    INPUT: Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task
  @param[in]  Opi    INPUT: Ptr to OXP_PI_INFO

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
CheckHandlePfr (
  IN     TPM2B_AUTH       *Auth,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State,
  IN     OXP_PFR_INFO     *Opi
  )
{
  // Local data
  UINT16   a, ps;
  UINT8    e;

  DEBUG ((EFI_D_ERROR, "[OOB] CheckHandlePfr() { Check and handle PFR: Input Attribute 0x%04x\n", Opi->Attribute));

  e = OXP_SUCCESS; ps = OXP_PI_STATUS_TASK_INITIATED;                // Assume Success
  a = Opi->Attribute & (OXP_PI_TASK_BIT_MASK);                       // Bit-mapped information of requested PFR task
  if (a != 0) {                                                      // If any task requested
    if ((Opd->FeatureSupported & OXP_PFR_SUPPORT_BIT) == 0) {        //   If PFR not supported
      ps = OXP_PI_STATUS_TASK_NOT_SUPPORTED;                         //     Error
    } else {                                                         //   If PFR supported
      if ((Opd->PfrState & OXP_DI_PFR_LOCKED_BIT) != 0) {            //     If PFR already locked
        ps = OXP_PI_STATUS_PFR_ALREADY_LOCKED;                       //       Already locked
      }
    }
  } else { ps = OXP_PI_STATUS_TASK_NONE; }                           // No task

  if (ps == OXP_PI_STATUS_TASK_INITIATED) {                          // If success so far
    switch (a) {                                                     // Handle PFR task
      case OXP_PI_PROVISION_BIT:                                     // Provision
        DEBUG ((EFI_D_ERROR, "    PFR Provision ->\n"));
        if ((Opd->PfrState & OXP_DI_PFR_PROVISIONED_BIT) != 0) {     // If PFR already provisioned
          ps = OXP_PI_STATUS_PFR_ALREADY_PROVISIONED;                //   Already provisioned
        }
        break;

      case OXP_PI_UNPROVISION_BIT:                                   // UnProvision
        DEBUG ((EFI_D_ERROR, "    PFR UnProvision ->\n"));
        if ((Opd->PfrState & OXP_DI_PFR_PROVISIONED_BIT) == 0) {     // If PFR not yet provisioned
          ps = OXP_PI_STATUS_PFR_ALREADY_UNPROVISIONED;              //   Already unprovisioned
        }
        break;

      case OXP_PI_LOCK_BIT:                                          // Lock
        DEBUG ((EFI_D_ERROR, "    PFR Lock ->\n"));
        //if ((Opd->PfrState & OXP_DI_PFR_LOCKED_BIT) == 0) {        // If PFR already locked
        //  ps = OXP_PI_STATUS_PFR_ALREADY_LOCKED;                   //   Already locked
        //}
        break;

      default:                                                       // Invalid operation
        DEBUG ((EFI_D_ERROR, "    PFR Task -> Invalid\n"));
        ps = OXP_PI_STATUS_TASK_INVALID;                             // Error
        break;
    }
  }
  if (ps != OXP_PI_STATUS_TASK_INITIATED) { e = OXP_INVALID_TASK; }  // Error

  // e = Status, ps = task status, a = Bit-mapped information of requested PFR task
  if ((e == OXP_SUCCESS) && (ps == OXP_PI_STATUS_TASK_INITIATED)) {  // If success so far
    e = OT_HandlePfr (Opd, (UINT8)(UINTN) a);                        //   Perform PFR task
    *State |= OXP_STATE_RESET_BIT;                                   //   Indicate reset required
  }
  Opi->Attribute &= ~(OXP_PI_STATUS_BIT_MASK);                       // Clear status bits
  Opi->Attribute |= ((ps << OXP_PI_STATUS_BIT_N)                     // Set status bits
                         & (OXP_PI_STATUS_BIT_MASK));

  DEBUG ((EFI_D_ERROR, "[OOB] CheckHandlePfr() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//--------------------- PFR Information Functions } ---------------------------------------------------

//--------------------- SubFunctions for different sub-operations { -----------------------------------
//-----------------------------------------------------------------------------------------------------
/**
  CheckEnableFTpm(): Enable fTPM.

  @param[in]  Auth   INPUT: Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Opd    INPUT: Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
CheckEnableFTpm (
  IN     TPM2B_AUTH       *Auth,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8   e;
  BOOLEAN c;

  DEBUG ((EFI_D_ERROR, "[OOB] CheckEnableFTpm() { Check and Enable fTPM\n"));

  e = OXP_SUCCESS;                                                   // Assume success
  if ((Opd->FeatureSupported & OXP_FTPM_SUPPORT_BIT) != 0) {         // If supported
    if ((Opd->FeatureEnabled & OXP_DTPM_ENABLED_BIT) != 0) {         //   If dTPM enabled currently
      DEBUG ((EFI_D_ERROR, "      OT_DTpmDisable(): dTPM enabled currently, disable dTPM\n"));
      e = OT_DTpmDisable (Auth, Opd);                                //     Disable dTPM
    }
    if (e == OXP_SUCCESS) {                                          //   If successs so far
      if ((Opd->FeatureEnabled & OXP_FTPM_ENABLED_BIT) == 0) {       //     If fTPM not enabled currently
        e = OT_FTpmEnable (Auth, Opd);                               //       Enable fTPM
        DEBUG ((EFI_D_ERROR, "      fTPM currently not enabled, Some fTPM Task may be pending until next reboot\n"));
        *State |= (OXP_STATE_TASK_PENDING_BIT | OXP_STATE_RESET_BIT);//     Indicate reset required, Task Pending
      } else {                                                       //     If fTPM enabled currently
        e = OT_FTpmSetPcrBanks (Auth, Opd, &c);                        //       Set PCR Banks
        if (c) {                                                       //       If Set PCR Banks command issued
          DEBUG ((EFI_D_ERROR, "      fTPM currently enabled, Some fTPM Task may be pending until next reboot\n"));
          *State |= (OXP_STATE_TASK_PENDING_BIT | OXP_STATE_RESET_BIT);//         Indicate reset required, Task Pending
        }
      }
    }
  } else {                                                           // If not supported
    e = OXP_FTPM_NOT_SUPPORTED;                                      // Assume Error
  }

  DEBUG ((EFI_D_ERROR, "[OOB] CheckEnableFTpm() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  CheckEnableDTpm(): Enable dTPM.

  @param[in]  Auth   INPUT:         Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Opd    INPUT:         Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT:  Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
CheckEnableDTpm (
  IN     TPM2B_AUTH       *Auth,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] CheckEnableDTpm() { Check and Enable dTPM\n"));

  e = OXP_DTPM_NOT_SUPPORTED;                                        // Assume Error
  if ((Opd->FeatureSupported & OXP_DTPM_SUPPORT_BIT) != 0) {         // If supported
    if ((Opd->FeatureEnabled & OXP_FTPM_ENABLED_BIT) != 0) {         //   If fTPM enabled currently
      DEBUG ((EFI_D_ERROR, "      OT_FTpmDisable(): fTPM enabled currently, disable fTPM\n"));
      if ((e = OT_FTpmDisable (Auth, Opd)) == OXP_SUCCESS) {         //   Disable fTPM, If success
        DEBUG ((EFI_D_ERROR, "      Enable dTPM: Task pending until next reboot\n"));
        *State |= (OXP_STATE_TASK_PENDING_BIT | OXP_STATE_RESET_BIT);//   Indicate reset required, Task Pending
      }
    } else {                                                         // If fTPM not enabled currently
      e = OXP_SUCCESS;                                               //   Assume Success
      if ((Opd->FeatureEnabled & OXP_DTPM_ENABLED_BIT) == 0) {       //   If dTPM not enabled currently
        e = OT_DTpmEnable (Auth, Opd);                               //   Enable dTPM
        *State |= OXP_STATE_RESET_BIT;                               //   Indicate reset required
      }
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] CheckEnableDTpm() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  CheckEnableTxt(): Enable TXT.

  @param[in]  Opd	INPUT:	Ptr to OT_PLATFORM_DATA
  @param[in]  State	INPUT/OUTPUT: Ptr to state of task

  @retval     Result	OUTPUT: UINT8, Status code for performed task

**/
UINT8
CheckEnableTxt (
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] CheckEnableTxt() { Check and Enable TXT\n"));

  e = OXP_SUCCESS;                                                   // Assume Success
  if ((Opd->FeatureSupported & OXP_TXT_SUPPORT_BIT) != 0) {          // If supported
    if ((Opd->FeatureEnabled & OXP_TXT_ENABLED_BIT) == 0) {          //   If not enabled currently
    e = OT_TxtEnable (Opd);                                          // Enable TXT
    *State |= OXP_STATE_RESET_BIT;                                   //   Indicate reset required
  }
  } else {                                                           // If not supported
    e = OXP_TXT_NOT_SUPPORTED;                                       //   Error
  }

  DEBUG ((EFI_D_ERROR, "[OOB] CheckEnableTxt() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//--------------------- SubFunctions for different sub-operations } -----------------------------------


//--------------------- Main Functions for different operations { -------------------------------------
//-----------------------------------------------------------------------------------------------------
/**
  OobDTpmClear(): Perform the requested task: Clear dTPM.

  @param[in]  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:  Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobDTpmClear (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e = OXP_SUCCESS;                                            // Assume success

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmClear() { Clear dTPM\n"));

  if ((Opd->FeatureSupported & OXP_DTPM_SUPPORT_BIT) == 0) {         // If not supported
    e = OXP_DTPM_NOT_SUPPORTED;                                      //   Error
  } else {                                                           // If supported
    if ((Opd->FeatureEnabled & OXP_DTPM_ENABLED_BIT) == 0) {         // If not enabled
      e = OXP_DTPM_NOT_ENABLED;                                      //   Error
    }
  }

  if (e == OXP_SUCCESS) {                                            // If success so far
    e = OT_DTpmClear (Auth, Opd);                                    // Clear TPM
    *State |= OXP_STATE_RESET_BIT;                                   // Indicate reset required
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmClear() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobDTpmDisableOnly(): Perform the requested task: Disable dTPM Only.

  @param[in]  Auth   INPUT:        Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:        Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:        Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobDTpmDisableOnly (
  IN     TPM2B_AUTH        *Auth,
  IN     OOB_TXT_TPM       *Ott,
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT32            *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmDisableOnly() { Disable dTPM Only\n"));

  e = OXP_DTPM_NOT_SUPPORTED;                                        // Assume Error
  if ((Opd->FeatureSupported & OXP_DTPM_SUPPORT_BIT) != 0) {         // If supported
    e = OXP_SUCCESS;                                                 //   Assume Success
    if ((Opd->FeatureEnabled & OXP_DTPM_ENABLED_BIT) != 0) {         //   If dTPM enabled currently
      e = OT_DTpmDisable (Auth, Opd);                                //     Disable dTPM
      *State |= OXP_STATE_RESET_BIT;                                 //     Indicate reset required
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmDisableOnly() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobDTpmEnableTxtEnable(): Perform the requested task: Enable dTPM, Enable TXT.

  @param[in]  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:  Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobDTpmEnableTxtEnable (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmEnableTxtEnable() { Enable dTPM, Enable TXT\n"));

  e = CheckEnableDTpm (Auth, Opd, State);                            // Enable TPM
  if (e == OXP_SUCCESS) {                                            // If success
    if ((*State & OXP_STATE_TASK_PENDING_BIT) == 0) {                //   If Task not pending
      e = CheckEnableTxt (Opd, State);                               //     Enable TXT
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmEnableTxtEnable() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobDTpmEnableClearTxtEnable(): Perform the requested task: Enable dTPM, Clear dTPM, and Enable TXT.

  @param[in]  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:  Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobDTpmEnableClearTxtEnable (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmEnableClearTxtEnable() { Enable dTPM, Clear dTPM, Enable TXT\n"));

  e = CheckEnableDTpm (Auth, Opd, State);                            // Enable dTPM
  if ((e == OXP_SUCCESS) && (Opd->Tpm12ActivateStateChange)) {       // If Success AND TPM Activate State Changed
    *State |= OXP_STATE_TASK_PENDING_BIT;                            //   Indicate Task pending
  }

  if (e == OXP_SUCCESS) {                                            // If success
    if ((*State & OXP_STATE_TASK_PENDING_BIT) == 0) {                //   If Task not pending
      e = OobDTpmClear (Auth, Ott, Opd, State);                      //     Clear dTPM
      if (e == OXP_SUCCESS) {                                        //     If success
// if ((*State & OXP_STATE_TASK_PENDING_BIT) == 0) {                 //      If Task not pending
        e = CheckEnableTxt (Opd, State);                             //         Enable TXT
// }
      }
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmEnableClearTxtEnable() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobDTpmEnableClear(): Perform the requested task: Enable dTPM, Clear dTPM.

  @param[in]  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:  Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobDTpmEnableClear (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmEnableClear() { Enable dTPM, Clear dTPM\n"));

  e = CheckEnableDTpm (Auth, Opd, State);                            // Enable dTPM
  if ((e == OXP_SUCCESS) && (Opd->Tpm12ActivateStateChange)) {       // If Success AND TPM Activate State Changed
    *State |= OXP_STATE_TASK_PENDING_BIT;                            //   Indicate Task pending
  }

  if (e == OXP_SUCCESS) {                                            // If success
    if ((*State & OXP_STATE_TASK_PENDING_BIT) == 0) {                //   If Task not pending
      e = OobDTpmClear (Auth, Ott, Opd, State);                      //     Clear dTPM
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobDTpmEnableClear() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobFTpmClear(): Perform the requested task: Clear fTPM.

  @param[in]  Auth  INPUT:        Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott   INPUT:        Ptr to OOB_TXT_TPM data
  @param[in]  Opd   INPUT:        Ptr to OT_PLATFORM_DATA
  @param[in]  State INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobFTpmClear (
  IN     TPM2B_AUTH        *Auth,
  IN     OOB_TXT_TPM       *Ott,
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT32            *State
  )
{
  // Local data
  UINT8  e = OXP_SUCCESS;                                            // Assume success

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmClear() { Clear fTPM\n"));

  if ((Opd->FeatureSupported & OXP_FTPM_SUPPORT_BIT) == 0) {         // If not supported
    e = OXP_FTPM_NOT_SUPPORTED;                                      //   Error
  } else {                                                           // If supported
    if ((Opd->FeatureEnabled & OXP_FTPM_ENABLED_BIT) == 0) {         // If not enabled
      e = OXP_FTPM_NOT_ENABLED;                                      //   Error
    }
  }

  if (e == OXP_SUCCESS) {                                            // If success so far
    e = OT_FTpmClear (Auth, Opd);                                    // Clear TPM
    *State |= OXP_STATE_RESET_BIT;                                   // Indicate reset required
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmClear() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobFTpmDisableOnly(): Perform the requested task: Disable fTPM Only.

  @param[in]  Auth   INPUT:         Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:         Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:         Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT:  Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobFTpmDisableOnly (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmDisableOnly() { Disable fTPM Only\n"));

  e = OXP_FTPM_NOT_SUPPORTED;                                        // Assume Error
  if ((Opd->FeatureSupported & OXP_FTPM_SUPPORT_BIT) != 0) {         // If supported
    e = OXP_SUCCESS;                                                 //   Assume Success
    if ((Opd->FeatureEnabled & OXP_FTPM_ENABLED_BIT) != 0) {         //   If fTPM enabled currently
      e = OT_FTpmDisable (Auth, Opd);                                //     Disable fTPM
      *State |= OXP_STATE_RESET_BIT;                                 //     Indicate reset required
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmDisableOnly() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobFTpmEnableTxtEnable(): Perform the requested task: Enable fTPM, Enable TXT.

  @param[in]  Auth   INPUT:         Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:         Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:         Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT:  Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobFTpmEnableTxtEnable (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmEnableTxtEnable() { Enable fTPM, Enable TXT\n"));

  e = CheckEnableFTpm (Auth, Opd, State);                            // Enable TPM
  if ((e == OXP_SUCCESS) && ((*State & OXP_STATE_TASK_PENDING_BIT) == 0)) { // If success AND Task not pending
    e = CheckEnableTxt (Opd, State);                                 //   Enable TXT
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmEnableTxtEnable() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobFTpmEnableClearTxtEnable(): Perform the requested task: Enable fTPM, Clear fTPM, and Enable TXT.

  @param[in]  Auth   INPUT:         Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:         Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:         Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT:  Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobFTpmEnableClearTxtEnable (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmEnableClearTxtEnable() { Enable fTPM, Clear fTPM, Enable TXT\n"));

  e = CheckEnableFTpm (Auth, Opd, State);                            // Enable fTPM
  if ((e == OXP_SUCCESS) && ((*State & OXP_STATE_TASK_PENDING_BIT) == 0)) {   // If success AND Task not pending
    e = OobFTpmClear (Auth, Ott, Opd, State);                        //   Clear fTPM
    if ((e == OXP_SUCCESS) && ((*State & OXP_STATE_TASK_PENDING_BIT) == 0)) { //   If success AND Task not pending
      e = CheckEnableTxt (Opd, State);                               //     Enable TXT
    }
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmEnableClearTxtEnable() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobFTpmEnableClear(): Perform the requested task: Enable fTPM, Clear fTPM.

  @param[in]  Auth   INPUT:        Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:        Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:        Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobFTpmEnableClear (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmEnableClear() { Enable fTPM, Clear fTPM\n"));

  e = CheckEnableFTpm (Auth, Opd, State);                            // Enable fTPM
  if ((e == OXP_SUCCESS) && ((*State & OXP_STATE_TASK_PENDING_BIT) == 0)) { // If success AND Task not pending
    e = OobFTpmClear (Auth, Ott, Opd, State);                        //   Clear fTPM
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobFTpmEnableClear() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobTxtDisableOnly(): Perform the requested task: Disable TXT Only.

  @param[in]  Auth   INPUT:       Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:       Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:       Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobTxtDisableOnly (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  UINT8  e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobTxtDisableOnly() { Disable TXT Only\n"));

  e = OXP_TXT_NOT_SUPPORTED;                                         // Assume Error
  if ((Opd->FeatureSupported & OXP_TXT_SUPPORT_BIT) != 0) {          // If supported
    e = OT_TxtDisable (Opd);                                         // Enable TXT
    *State |= OXP_STATE_RESET_BIT;                                   //   Indicate reset required
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobTxtDisableOnly() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobSupportDiscovery(): Discover the support of different features.

  @param[in]  SpecVer INPUT:        UINT8, OTA Spec Version in OXP_HEADER.Version as supplied by caller
  @param[in]  Ott     INPUT:        Ptr to OOB_TXT_TPM data
  @param[in]  Opd     INPUT:        Ptr to OT_PLATFORM_DATA
  @param[in]  State   INPUT/OUTPUT: Ptr to state of task

  @retval     Result  OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobSupportDiscovery (
  IN     UINT8             SpecVer,
  IN     OOB_TXT_TPM       *Ott,
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT32            *State
  )
{
  // Local data
  OXP_DISCOVERY_INFO  *odi;
  UINT32              s;
  UINT8               e;

  DEBUG ((EFI_D_ERROR, "[OOB] OobSupportDiscovery() { Discover support of different features, State 0x%08x\n", *State));

  s = 0;                                                             // State
  e = OXP_SUCCESS;                                                   // Assume success
  odi = (OXP_DISCOVERY_INFO*)(UINTN)                                 // Ptr to OXP_DISCOVERY_INFO
              LocateOobAdditionalInfo (Ott, OXP_ID_DISCOVERY);

  // For Spec Version > 0x01, OXP_DISCOVERY_INFO must be supplied as input
  // For Spec Version <= 0x01, OXP_DISCOVERY_INFO may or may not be supplied as input
  if (SpecVer > 0x01) {                                              // If OTA Spec Version > 0x01
    if (odi == NULL) { e = OXP_ADDITIONAL_INFO_NOT_PRESENT; }        //   If OXP_DISCOVERY_INFO not found, Error
  }

  if (e == OXP_SUCCESS) {                                            // If success so far
    if (odi != NULL) {                                               //   If OXP_DISCOVERY_INFO supplied
      Opd->TaskHasOutputData = TRUE;                                 //     Task has output data beyond OXP_HEADER
      if ((odi->Length >= sizeof (OXP_DISCOVERY_INFO)) &&            //     If Length valid AND
              ((odi->Attribute & OXP_DISCOVERY_INFO_VALID_BIT) != 0)) { //        Discovery info valid
        if (SpecVer == 0x01) {
          Ott->Header.Version = OXP_SPECIFICATION_VERSION;           //       Update OTA Spec Version
          s |= OXP_STATE_VERISON_UPDATED_BIT;                        //       Update State
        }
        FillOobSupportedTaskInfo ((UINT8*)(UINTN) &odi->TaskSupported,//       Fill TaskSupported field
               sizeof (odi->TaskSupported));
        // Fill MK-TME info
        odi->MaxMkTmeKeys = Opd->MaxMkTmeKeys;                       //     Max #of MK-TME Keys available for use
        odi->MaxMkTmeKeyIdBits = Opd->MaxMkTmeKeyIdBits;             //     Max #of Bits allowed for use as Key Identifiers for MK-TME
        odi->MkTmeKeyIdBits = Opd->MkTmeKeyIdBits;                   //     #of Bits allocated for use as Key Identifiers for MK-TME
        odi->Attribute |= OXP_DISCOVERY_INFO_MK_TME_VALID_BIT;       //     Indicate MKTME info valid
        // Fill PFR info
        odi->PfrState = Opd->PfrState;                               //     Bit-mapped information PFR STate
        odi->PfrRecoveryCount = Opd->PfrRecoveryCount;               //     #of Recovery since last AC Power Cycle
        odi->PfrLastRecoveryReason = Opd->PfrLastRecoveryReason;     //     Last Recovery Reason
        odi->PfrPanicEventCount = Opd->PfrPanicEventCount;           //     #of Panic Event since last AC Power Cycle
        odi->PfrLastPanicReason = Opd->PfrLastPanicReason;           //     Last Panic Reason
        odi->Attribute |= OXP_DISCOVERY_INFO_PFR_VALID_BIT;          //     Indicate PFR info valid
      } else {                                                       //     If Length not valid OR Discovery info not valid
        e = OXP_ADDITIONAL_INFO_MALFORMED;                           //       Error
      }
    }
  }

  *State |= s;                                                       // Update State

  DEBUG ((EFI_D_ERROR, "[OOB] OobSupportDiscovery() } State 0x%08x, Status 0x%02x -> ", *State, e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobMkTmeHandler():  TME/MK-TME handler.

  @param[in]  Auth   INPUT:       Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:       Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:       Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobMkTmeHandler (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  OXP_MK_TME_INFO *Omti;
  UINT16          s;
  UINT8           e, m;

  DEBUG ((EFI_D_ERROR, "[OOB] OobMkTmeHandler() { Handle TME/MK-TME\n"));

  Opd->TaskHasOutputData = TRUE;                                     // Task has output data beyond OXP_HEADER
  Omti = (OXP_MK_TME_INFO*)(UINTN) LocateOobAdditionalInfo (Ott, OXP_ID_MK_TME); // Ptr to OXP_MK_TME_INFO
  s = 0;                                                             // Length of additional information as supplied by caller

  e = ValidateOobMkTmeInfo (Ott, Omti, State);                       // Validate TME/MK-TME info
  if (e == OXP_SUCCESS) {                                            // If success
    s = Omti->Length;                                                //   Length of additional information
    e = CheckHandleMkTme (Auth, Opd, State, Omti);                   //     Handle TME/MK-TME
    m = MakeOobAdditionalInformationContiguous (Ott, (UINT8*)(UINTN) Omti, s);  // Make OOB Data contiguous (if additional information length is changed)
    if (m != OXP_SUCCESS) { e = m; }                                 // Update status code, if necessary
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobMkTmeHandler() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                                          // Status
}

//-----------------------------------------------------------------------------------------------------
/**
  OobPfrHandler():  PFR handler.

  @param[in]  Auth   INPUT:       Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott    INPUT:       Ptr to OOB_TXT_TPM data
  @param[in]  Opd    INPUT:       Ptr to OT_PLATFORM_DATA
  @param[in]  State  INPUT/OUTPUT: Ptr to state of task

  @retval     Result OUTPUT: UINT8, Status code for performed task

**/
UINT8
OobPfrHandler (
  IN     TPM2B_AUTH       *Auth,
  IN     OOB_TXT_TPM      *Ott,
  IN     OT_PLATFORM_DATA *Opd,
  IN OUT UINT32           *State
  )
{
  // Local data
  OXP_PFR_INFO  *Opi;
  UINT16        s;
  UINT8         e, m;

  DEBUG ((EFI_D_ERROR, "[OOB] OobPfrHandler() { Handle PFR\n"));

  Opd->TaskHasOutputData = TRUE;                          // Task has output data beyond OXP_HEADER
  Opi = (OXP_PFR_INFO*)(UINTN) LocateOobAdditionalInfo (  // Ptr to OXP_PFR_INFO
                               Ott, OXP_ID_PFR);
  s = 0;                                                  // Length of additional information as supplied by caller
  e = ValidateOobPfrInfo (Ott, Opi, State);               // Validate PFR info
  if (e == OXP_SUCCESS) {                                 // If success
    s = Opi->Length;                                      //   Length of additional information
    e = CheckHandlePfr (Auth, Opd, State, Opi);           //   Handle PFR
    m = MakeOobAdditionalInformationContiguous (Ott,      // Make OOB Data contiguous (if additional information length is changed)
                                    (UINT8*)(UINTN) Opi, s);
    if (m != OXP_SUCCESS) { e = m; }                      // Update status code, if necessary
  }

  DEBUG ((EFI_D_ERROR, "[OOB] OobPfrHandler() } Status 0x%02x -> ", e));
  if (e == OXP_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return e;                                               // Status
}

//--------------------- Main Functions for different operations } -------------------------------------

//-----------------------------------------------------------------------------------------------------
/**
  AnalyzeOobNvStorageData(): Analyze OOB data read from NV Storage.

  @param  Buffer  INPUT: VOID*, Ptr to OOB data buffer
          Size    INPUT/OUTPUT: UINTN*, Ptr to Data size in bytes
          Valid   INPUT/OUTPUT: BOOLEAN*, Ptr to data validity
          Task    INPUT/OUTPUT: BOOLEAN*, Ptr to new task requested
          State   INPUT/OUTPUT: UINT32*, Ptr to State

  @retval None
**/
VOID
AnalyzeOobNvStorageData (
  IN     VOID      *Buffer,
  IN     UINTN     *Size,
  IN OUT BOOLEAN   *Valid,
  IN OUT BOOLEAN   *Task,
  IN OUT UINT32    *State
  )
{
  // Local data
  OOB_TXT_TPM  *ott;
  UINT32       st;

  if ((Buffer == NULL) || (Size == NULL) || (Valid == NULL) || (Task == NULL) || (State == NULL)) {
    DEBUG ((EFI_D_ERROR, "[OOB] AnalyzeOobNvStorageData() -> ERROR: Ptr NULL => BUG\n"));
    return;                                              // Error
  }

  *Valid = FALSE; *Task = FALSE; st = 0;                 // Assume not valid, no new task, State
  ott = (OOB_TXT_TPM*)(UINTN) Buffer;                    // Ptr to OOB data
  if (ValidateOobOneTouchInputData (ott, Size, &st)) {   // If valid
    *Valid = TRUE;                                       //   Indicate valid
    if ((ott->Header.TaskAndResult &                     // If Task to be performed
         OXP_TASK_PERFORMED_BIT) == 0) { *Task = TRUE; } //   Indicate new task
  }
  *State |= st;                                          // Return updated State
}

//-----------------------------------------------------------------------------------------------------
/**
  WhichOobNvStorageDataToUse(): Determine which NV Storage data to use and copy OOB data to supplied buffer.

  @param  Buffer  INPUT : VOID*, Ptr to buffer
          Size    INPUT/OUTPUT: UINTN*, Ptr to data size of buffer/OOB data
          State   INPUT/OUTPUT: UINT32*, Ptr to state

  @retval Result  BOOLEAN: TRUE  -> Success (Valid OOB data found in EITHER ME OR EFI storage)
                           FALSE -> Error (Invalid data in both ME and EFI storage)

  Note:
    1. This function is called after data is read from both NV Storage (ME and EFI).
    2. The following table describes the implementation: 0 indicates invalid data and 1 indicates valid data in respective storage.
       ME    EFI    Action
       0     0      Case-0: No valid data in BOTH storage => Error
       0     1      Case-1: Valid data in ONLY EFI Storage: Use EFI Variable data
       1     0      Case-2: Valid data in ONLY ME Storage : Use ME Storage data
       1     1      Case-3: Valid data in BOTH ME and EFI Storage
                            (a) No  task requested in BOTH storage     : Use ME Storage data
                            (b) New task requested in ONLY EFI Storage : Use EFI Variable data
                            (c) New task requested in ONLY ME Storage  : Use ME Storage data
                            (d) New task requested in BOTH storage     : Use ME Storage data (i.e. task requested in EFI Variable is lost)
**/
BOOLEAN
WhichOobNvStorageDataToUse (
  IN     VOID    *Buffer,
  IN OUT UINTN   *Size, 
  IN OUT UINT32  *State
  )
{
  // Local data
  VOID    *p;
  UINTN    s;
  UINT32   st, t;
  BOOLEAN  b, ed, et, md, mt;

  DEBUG ((EFI_D_ERROR, "[OOB] WhichOobNvStorageDataToUse() { Determine OOB data (ME or EFI) to use\n"));

  ed = FALSE; et = FALSE; md = FALSE; mt = FALSE; s = 0;               // EFI/ME data not valid, no task in EFI/ME data, data size, State
  b = FALSE; p = NULL;                                                 // Assume Error, Ptr to data to be used

  if ((Buffer == NULL) || (Size == NULL) || (State == NULL)) {         // If parameters not valid
    DEBUG ((EFI_D_ERROR, "[OOB] WhichOobNvStorageDataToUse() } ERROR: Ptr NULL => BUG\n"));
    return b;                                                          //   Error
  }

  // Analyze data in both Storage
  st = 0; t = 0;                                                       // Init State information

  if (mMeNvStorage) {                                                  // If storage supported
    DEBUG ((EFI_D_ERROR, "      Analyze ME Storage Data, Size 0x%08x bytes\n", mMeDataSize));
    if (mMeDataSize != 0) {                                            //   If data size valid
      //DisplayBinaryData ((UINT8*)(UINTN) mOttMe, mMeDataSize);
      AnalyzeOobNvStorageData (mOttMe, &mMeDataSize, &md, &mt, &t);    //     Analyze ME Storage data
      if (((t & OXP_STATE_SIGNATURE_INVALID_BIT) == 0) ||              //     If Signature valid OR
          ((t & OXP_STATE_ALL_BYTES_SAME_BIT) != 0)) {                 //        All bytes same
        st |= OXP_STATE_OOB_ME_DATA_UPDATE_BIT;                        //       Indicate ME data to be updated, if required
      }
    } else {                                                           //   If data size not valid
      DEBUG ((EFI_D_ERROR, "      WARNING: ME Storage Data Empty\n"));
      st |= OXP_STATE_OOB_ME_DATA_UPDATE_BIT;                          //     Indicate ME data to be updated, if required
    }
    DEBUG ((EFI_D_ERROR, "      ME Storage OOB Data: Size = 0x%08x bytes, ", mMeDataSize));
    if (md) { DEBUG ((EFI_D_ERROR, "Valid\n")); } else { DEBUG ((EFI_D_ERROR, "Not Valid\n")); }
    DEBUG ((EFI_D_ERROR, "                           New Task? "));
    if (mt) { DEBUG ((EFI_D_ERROR, "Yes")); } else { DEBUG ((EFI_D_ERROR, "No")); }
    DEBUG ((EFI_D_ERROR, ", Data to be Updated on Output? "));
    if ((st & OXP_STATE_OOB_ME_DATA_UPDATE_BIT) != 0) { DEBUG ((EFI_D_ERROR, "Yes\n")); } else { DEBUG ((EFI_D_ERROR, "No\n")); }
  } else {
    DEBUG ((EFI_D_ERROR, "      WARNING: ME Storage not supported\n"));
  }

  t = 0;                                                               // Init State

  if (mEfiNvStorage) {                                                 // If storage supported
    DEBUG ((EFI_D_ERROR, "      Analyze EFI Variable Data, Size 0x%08x bytes\n", mEfiDataSize));
    if (mEfiDataSize != 0) {                                           //   If data size valid
      //DisplayBinaryData ((UINT8*)(UINTN) mOttEfi, mEfiDataSize);
      AnalyzeOobNvStorageData (mOttEfi, &mEfiDataSize, &ed, &et, &t);  //     Analyze EFI Storage data
      if (((t & OXP_STATE_SIGNATURE_INVALID_BIT) == 0) ||              //     If Signature valid OR
          ((t & OXP_STATE_ALL_BYTES_SAME_BIT) != 0)) {                 //        All bytes same
        st |= OXP_STATE_OOB_EFI_DATA_UPDATE_BIT;                       //       Indicate EFI data to be updated, if required
      }
    } else {                                                           //   If data size not valid
      DEBUG ((EFI_D_ERROR, "      WARNING: EFI Variable Data Empty\n"));
      st |= OXP_STATE_OOB_EFI_DATA_UPDATE_BIT;                         //     Indicate EFI data to be updated, if required
    }
    DEBUG ((EFI_D_ERROR, "      EFI Storage OOB Data: Size = 0x%08x bytes, ", mEfiDataSize));
    if (ed) { DEBUG ((EFI_D_ERROR, "Valid\n")); } else { DEBUG ((EFI_D_ERROR, "Not Valid\n")); }
    DEBUG ((EFI_D_ERROR, "                            New Task? "));
    if (et) { DEBUG ((EFI_D_ERROR, "Yes")); } else { DEBUG ((EFI_D_ERROR, "No")); }
    DEBUG ((EFI_D_ERROR, ", Data to be Updated on Output? "));
    if ((st & OXP_STATE_OOB_EFI_DATA_UPDATE_BIT) != 0) { DEBUG ((EFI_D_ERROR, "Yes\n")); } else { DEBUG ((EFI_D_ERROR, "No\n")); }
  } else {
    DEBUG ((EFI_D_ERROR, "      WARNING: EFI Variable not supported\n"));
  }

  // Check data in both Storage
  if ((!(ed)) && (!(md))) {                                           // If no valid data in both EFI and ME
    // Case-0: No valid data in BOTH Storage => ERROR
    DEBUG ((EFI_D_ERROR, "      ERROR: No valid data in both (ME and EFI) storage\n"));
    st |= (OXP_STATE_OOB_ME_DATA_INVALID_BIT |                        //   Update State
           OXP_STATE_OOB_EFI_DATA_INVALID_BIT);
  } else {
    if ((ed) && (!(md))) {                                            // If valid data in EFI but no valid data in ME
      // Case-1: Valid data in ONLY EFI Storage : Use EFI Storage data for OOB
      DEBUG ((EFI_D_ERROR, "      Use EFI Storage Data (Valid data in ONLY EFI storage)\n"));
      p = (VOID*) mOttEfi; s = mEfiDataSize; b = TRUE;                //   Use EFI storage data, Success
      st |= OXP_STATE_OOB_ME_DATA_INVALID_BIT;                        //   Update State
    } else {
      if ((!(ed)) && (!(md))) {                                       // If no valid data in EFI but valid data in ME
        // Case-2: Valid data in ONLY ME Storage : Use ME Storage data for OOB
        DEBUG ((EFI_D_ERROR, "      Use ME Storage Data (ME data valid AND EFI data not valid)\n"));
        p = (VOID*) mOttMe; s = mMeDataSize; b = TRUE;                //   Use ME storage data, Success
        st |= OXP_STATE_OOB_EFI_DATA_INVALID_BIT;                     //   Update State
      } else {
        if ((ed) && (md)) {                                           // If valid data in both EFI and ME
          // Case-3: Valid data in BOTH ME and EFI Storage
          //         (a) No  task requested in both storage: Use ME Storage data for OOB
          //         (b) New task requested in EFI Storage : Use EFI Variable data for OOB
          //         (c) New task requested in ME Storage  : Use ME Storage data for OOB
          //         (d) New task requested in both storage: Use ME Storage data for OOB (i.e. task requested in EFI Variable is lost)
          b = TRUE;                                                   //   Success
          if ((et) && (!(mt))) {                                      //   If new task in EFI but no new task in ME
            DEBUG ((EFI_D_ERROR, "      Use EFI Storage Data (New Task in EFI but no new task in ME)\n"));
            p = (VOID*) mOttEfi; s = mEfiDataSize;                    //     Case 3(b) -> Use EFI storage data
          } else {                                                    //   Case 3(a), 3(c), 3(d)
            DEBUG ((EFI_D_ERROR, "      Use ME Storage Data (EITHER No Task in BOTH OR New Task in BOTH OR New task in Only ME)\n"));
            p = (VOID*) mOttMe; s = mMeDataSize;                      //     Use ME storage data
          }
        }
      }
    }
  }

  if (b) {                                                            // If success so far
    // p = ptr to data to use, s = data size in bytes
    if (s <= *Size) {                                                 //   If supplied buffer large enough
      CopyMem (Buffer, p, s);                                         //     Copy OOB data to use
      if (p == mOttMe) { st |= OXP_STATE_OOB_ME_DATA_USED_BIT; }      //     Update State
      else { st |= OXP_STATE_OOB_EFI_DATA_USED_BIT; }
    } else {                                                          //   If supplied buffer not large enough
      DEBUG ((EFI_D_ERROR, "      ERROR: Supplied buffer too small\n"));
      b = FALSE; s = 0;                                               //     Error, Data Size = 0
    }
  }

  *State |= st; *Size = s;                                            // Return State, Size

  DEBUG ((EFI_D_ERROR, "[OOB] WhichOobNvStorageDataToUse() } "));
  if (b) { DEBUG ((EFI_D_ERROR, "Success: ")); }
  else { DEBUG ((EFI_D_ERROR, "ERROR: ")); }
  DEBUG ((EFI_D_ERROR, "OOB Data Size = 0x%08x bytes, State = 0x%08x\n", *Size, *State));

  return b;                                                           // Result
}

//-----------------------------------------------------------------------------------------------------
/**
  PerformOobOneTouchTask(): Perform the requested OOB One-Touch Task.

  @param[in]  Auth  INPUT:                 Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param[in]  Ott   INPUT:                 Ptr to OOB_TXT_TPM data
  @param[in]  Opd   INPUT:                 Ptr to OT_PLATFORM_DATA
  @param[in]  State INPUT/OUTPUT: UINT32*, Ptr to state of task

  @retval     None

  NOTE:
     1. This function should perform the task and update all necessary fields in OOB Data (execept State field).
     2. This function MUST NOT update the State field of OOB Data.
        State field in OOB data is updated before writing updated OOB data, if necessary.
**/
VOID
PerformOobOneTouchTask (
  IN     TPM2B_AUTH        *Auth,
  IN     OOB_TXT_TPM       *Ott,
  IN     OT_PLATFORM_DATA  *Opd,
  IN OUT UINT32            *State
  )
{
  // Local data
  UINT32  s;
  UINT8   e, iv, t;
  BOOLEAN b;

  s = *State;                                                        // Current State
  b = Ott->Header.TaskAndResult & OXP_TASK_PERFORMED_BIT;            // = 0, Task to be performed
                                                                     // <>0, No Task to perform
  t = (Ott->Header.TaskAndResult & OXP_TASK_BIT_MASK) >> OXP_TASK_BIT_N;// Task#

  DEBUG ((EFI_D_ERROR, "[OOB] PerformOobOneTouchTask() { Perform requested OOB Task\n"));
  DEBUG ((EFI_D_ERROR, "      INPUT : TaskAndResult 0x%02x, Status 0x%02x, State 0x%08x\n", Ott->Header.TaskAndResult, Ott->Header.Status, s));

  // Update general info in OXP_HEADER, if necessary
  if ((Ott->Header.PasswordAttribute & (~(OXP_PASSWORD_IGNORE_BIT))) != // If Password Attribute changed
      Opd->PasswordAttribute) {
    Ott->Header.PasswordAttribute = (Opd->PasswordAttribute |        //   Update Password Attribute
                                    (Ott->Header.PasswordAttribute & OXP_PASSWORD_IGNORE_BIT));
    s |= OXP_STATE_PASSWORD_ATTRIBUTE_UPDATED_BIT;                   //   Update State
  }
  if (Ott->Header.FeatureSupported != Opd->FeatureSupported) {       // If Feature Supported changed
    Ott->Header.FeatureSupported = Opd->FeatureSupported;            //   Update Feature Supported
    s |= OXP_STATE_FEATURE_SUPPORTED_UPDATED_BIT;                    //   Update State
  }
  if (Ott->Header.FeatureEnabled != Opd->FeatureEnabled) {           // If Feature Enabled changed
    Ott->Header.FeatureEnabled = Opd->FeatureEnabled;                //   Update Feature Enabled
    s |= OXP_STATE_FEATURE_ENABLED_UPDATED_BIT;                      //   Update State
  }

  iv = Ott->Header.Version;                                          // OTA Spec Version supplied as Input
  if (iv != OXP_SPECIFICATION_VERSION) {                             // If Specification Version not match
    if (t !=  OXP_TASK_FEATURE_SUPPORT_DISCOVERY) {                  //   If Task not Feature Support Discovery
      Ott->Header.Version = OXP_SPECIFICATION_VERSION;               //     Update Specification Version
      s |= OXP_STATE_VERISON_UPDATED_BIT;                            //     Update State
    }
  }

  if (Ott->Header.MajorVersionImp != OXP_MAJOR_VERSION_IMP) {        // If Implementation Major Version not match
    Ott->Header.MajorVersionImp = OXP_MAJOR_VERSION_IMP;             //   Update Implementation Major Version
    s |= OXP_STATE_VERISON_UPDATED_BIT;                              //   Update State
  }

  if (Ott->Header.MinorVersionImp != OXP_MINOR_VERSION_IMP) {        // If Implementation Minor Version not match
    Ott->Header.MinorVersionImp = OXP_MINOR_VERSION_IMP;             //   Update Implementation Minor Version
    s |= OXP_STATE_VERISON_UPDATED_BIT;                              //   Update State
  }

  if (Ott->Header.TpmUsage != Opd->TpmUsage) {                       // If TPM Usage changed
    Ott->Header.TpmUsage = Opd->TpmUsage;                            //   Update TPM Usage
    s |= OXP_STATE_TPM_USAGE_UPDATED_BIT;                            //   Update State
  }

  // Check if any task to be performed: s = State, t = Task#, b = 0, Task to be performed
  //                                                            <>0, No Task to perform
  if (b != 0) {                                                      // If no task to be performed
    if (t != OXP_TASK_FEATURE_SUPPORT_DISCOVERY) {                   //   If prev task <> Task# 01
      DEBUG ((EFI_D_ERROR, "              No task to perform\n"));
      *State = s;                                                    //   Update State
      DEBUG ((EFI_D_ERROR, "      OUTPUT: TaskAndResult 0x%02x, Status 0x%02x, State 0x%08x\n", Ott->Header.TaskAndResult, Ott->Header.Status, s));
      DEBUG ((EFI_D_ERROR, "[OOB] PerformOobOneTouchTask() }\n"));
      return;
    } else {                                                         //   If prev task = Task# 01
      DEBUG ((EFI_D_ERROR, "              Task# 0x01 performed: Check for any information changed in this boot\n"));
    }
  }

  // Validate additional information beyond OXP_HEADER: s = State, t = Task# to be performed
  e = ValidateOobAdditionalInformation (Ott);                        // Validate additional information

  // Authenticate password data, if necessary: s = State, e = status (success so far), t = Task# to be performed
  if (e == OXP_SUCCESS) {                                            // If success so far
    if (AuthenticationOobTaskRequired (Ott)) {                       // If authentication required
      e = AuthenticateOobPassword (Ott, Opd, &s);                    //   Authenticate password
    }
  }

  // Prepare TPM, if necessary: s = State, e = status, t = Task# to be performed
  if (e == OXP_SUCCESS) {                                            // If success so far
    if (TpmPreparationOobTaskRequired (Ott)) {                       // If TPM preparation required
      e = OT_PrepareTpmBeforeOobTask (Opd);                          //   Prepare TPM for OOB Task
      if (e != OXP_SUCCESS) {                                        // If not success
        s |= OXP_STATE_TPM_PREPARATION_BEFORE_TASK_ERROR_BIT;        //   Update state
      }
    }
  }

  // A task to be performed: s = state, e = status, t = Task# to be performed, iv = OTA Spec Version supplied as Input
  //   =======> NOTE: If any task is added or removed, Update TaskSupportedTable accordingly.
  if (e == OXP_SUCCESS) {                                            // If success so far
    switch (t) {
      case OXP_TASK_FEATURE_SUPPORT_DISCOVERY:                       //   01 Feature Support Discovery
        e = OobSupportDiscovery (iv, Ott, Opd, &s);                  //      Support discovery
        break;

      case OXP_TASK_FEATURE_ENABLE_STATUS_DISCOVERY:                 //   02 Feature Enable Status Discovery
        //e = OXP_SUCCESS;	// e = OXP_SUCCESS already           //      Success
        break;

      case OXP_TASK_ENABLE_TXT_DTPM:                                 //   03 Enable TXT/dTPM
        e = OobDTpmEnableTxtEnable (Auth, Ott, Opd, &s);             //      Enable dTPM, Enable TXT
        break;

      case OXP_TASK_DTPM_OWNER_CLEAR_ONLY:                           //   04 dTPM Owner Clear Only
        e = OobDTpmClear (Auth, Ott, Opd, &s);                       //      Clear dTPM
        break;

      case OXP_TASK_DTPM_CLEAR_DTPM_ACTIVATION:                      //   05 dTPM Clear and dTPM Activation
        e = OobDTpmEnableClear (Auth, Ott, Opd, &s);                 //      Enable dTPM, Clear dTPM
        break;

      case OXP_TASK_DTPM_CLEAR_ENABLE_TXT_DTPM:                      //   06 dTPM Clear and Enable TXT/dTPM
        e = OobDTpmEnableClearTxtEnable (Auth, Ott, Opd, &s);        //      Enable dTPM, Clear dTPM, Enable TXT
        break;

      case OXP_TASK_ENABLE_TXT_FTPM:                                 //   0B Enable TXT/fTPM
        e = OobFTpmEnableTxtEnable (Auth, Ott, Opd, &s);             //      Enable fTPM, Enable TXT
        break;

      case OXP_TASK_FTPM_OWNER_CLEAR_ONLY:                           //   0C fTPM Owner Clear Only
        e = OobFTpmClear (Auth, Ott, Opd, &s);                       //      Clear fTPM
        break;

      case OXP_TASK_FTPM_CLEAR_FTPM_ACTIVATION:                      //   0D fTPM Clear and fTPM Activation
        e = OobFTpmEnableClear (Auth, Ott, Opd, &s);                 //      Enable fTPM, Clear fTPM
        break;

      case OXP_TASK_FTPM_CLEAR_ENABLE_TXT_FTPM:                      //   0E fTPM Clear and Enable TXT/fTPM
        e = OobFTpmEnableClearTxtEnable (Auth, Ott, Opd, &s);        //      Enable fTPM, Clear fTPM, Enable TXT
        break;

      case OXP_TASK_DISABLE_TXT_ONLY:                                //   0F Disable TXT Only
        e = OobTxtDisableOnly (Auth, Ott, Opd, &s);                  //      Disable TXT
        break;

      case OXP_TASK_DISABLE_DTPM_ONLY:                               //   10 Disable dTPM Only
        e = OobDTpmDisableOnly (Auth, Ott, Opd, &s);                 //      Disable dTPM
        break;

      case OXP_TASK_DISABLE_FTPM_ONLY:                               //   11 Disable fTPM Only
        e = OobFTpmDisableOnly (Auth, Ott, Opd, &s);                 //      Disable fTPM
        break;

      case OXP_TASK_HANDLE_MKTME:                                    //   17 Handle TME/MK-TME
        e = OobMkTmeHandler (Auth, Ott, Opd, &s);                    //      Handle TME/MK-TME
        break;

      case OXP_TASK_HANDLE_PFR:                                      //   19 Handle PFR
        e = OobPfrHandler (Auth, Ott, Opd, &s);                      //      Handle PFR
        break;

      default:                                                       // Invalid Task
        e = OXP_INVALID_TASK;                                        // Status
        break;
    }
  }

  // Prepare TPM, if necessary: s = State, e = status
  if ((OT_PrepareTpmAfterOobTask (Opd)) != OXP_SUCCESS) {            // Prepare TPM after task
    s |= OXP_STATE_TPM_PREPARATION_AFTER_TASK_ERROR_BIT;             //   If not success, Update state
  }

  // s = State, e = status
  if ((s & OXP_STATE_TASK_PENDING_BIT) != 0) {                       // If Task pending (i.e. not yet performed)
    Ott->Header.TaskAndResult &= ~(OXP_TASK_PERFORMED_BIT);          //   Indicate task not yet performed in OOB data
  } else {                                                           // If Task not pending (i.e. performed)
    Ott->Header.TaskAndResult |= OXP_TASK_PERFORMED_BIT;             //   Indicate task performed in OOB data
  }
  Ott->Header.Status = e;                                            // Update Status in OOB data
  *State = s;                                                        // Return State
  DEBUG ((EFI_D_ERROR, "      OUTPUT: TaskAndResult 0x%02x, Status 0x%02x, State 0x%08x\n", Ott->Header.TaskAndResult, Ott->Header.Status, s));
  DEBUG ((EFI_D_ERROR, "[OOB] PerformOobOneTouchTask() }\n"));

  return;
}

//-----------------------------------------------------------------------------------------------------
// OOB Tasks that are supported
//
UINT8  TaskSupportedTable[] = {                                      // Task	Description
              OXP_TASK_FEATURE_SUPPORT_DISCOVERY,                    //  01	Feature Support Discovery
              OXP_TASK_FEATURE_ENABLE_STATUS_DISCOVERY,              //  02	Feature Enable Status Discovery
              OXP_TASK_ENABLE_TXT_DTPM,                              //  03	Enable TXT/dTPM
              OXP_TASK_DTPM_OWNER_CLEAR_ONLY,                        //  04	dTPM Owner Clear Only
              OXP_TASK_DTPM_CLEAR_DTPM_ACTIVATION,                   //  05	dTPM Clear and dTPM Activation
              OXP_TASK_DTPM_CLEAR_ENABLE_TXT_DTPM,                   //  06	dTPM Clear and Enable TXT/dTPM
              OXP_TASK_ENABLE_TXT_FTPM,                              //  0B	Enable TXT/fTPM
              OXP_TASK_FTPM_OWNER_CLEAR_ONLY,                        //  0C	fTPM Owner Clear Only
              OXP_TASK_FTPM_CLEAR_FTPM_ACTIVATION,                   //  0D	fTPM Clear and TPM Activation
              OXP_TASK_FTPM_CLEAR_ENABLE_TXT_FTPM,                   //  0E	fTPM Clear and Enable TXT/fTPM
              OXP_TASK_DISABLE_TXT_ONLY,                             //  0F	Disable TXT only
              OXP_TASK_DISABLE_DTPM_ONLY,                            //  10	Disable DTPM only
              OXP_TASK_DISABLE_FTPM_ONLY,                            //  11	Disable FTPM only
//            OXP_TASK_PROVISION_DTPM,                               //  12	Provision dTPM
//            OXP_TASK_PROVISION_FTPM,                               //  13	Provision fTPM
//            OXP_TASK_HANDLE_TPM_NV_INDEX,                          //  14	Handle TPM NV Index
//            OXP_TASK_DTPM_RESET_AUX_INDEX,                         //  15	dTPM Reset AUX Index
//            OXP_TASK_FTPM_RESET_AUX_INDEX,                         //  16	fTPM Reset AUX Index
              OXP_TASK_HANDLE_MKTME,                                 //  17	TME/MK-TME Handler
//            OXP_TASK_HANDLE_SGX,                                   //  18	SGX Handler
              OXP_TASK_HANDLE_PFR,                                   //  19	PFR Handler
              };
//
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
// ******************** PUBLIC Functions **************************************************************
//-----------------------------------------------------------------------------------------------------
/**
  Fill the bit-mapped information of supported tasks.

  @param  Buffer INPUT: UINT8*, Ptr to buffer where bit-mapped information will be filled
  @param  Size   INPUT: UINTN,  Size of supplied buffer in bytes

  @retval None
**/
VOID
FillOobSupportedTaskInfo (
  IN  UINT8  *Buffer,
  IN  UINTN  Size
  )
{
  // Local data
  UINT8  i, j, n, t;

  if ((Buffer != NULL) && (Size != 0)) {                             // If buffer valid
    ZeroMem (Buffer, Size);                                          //   Init buffer with 0
    for (n = 0; n < sizeof (TaskSupportedTable); n++) {              //   Search task supported table
      t = TaskSupportedTable[n];                                     //   Supported Task#
      i = t >> 3; j = t & 0x07;                                      //   i, j = 0-based byte# of TaskSupported array, 0-based bit# within the byte of TaskSupported array
      if (((UINTN)i) < Size) { Buffer[i] |= (1 << j); }              //   If byte# is within the buffer THEN set the bit to 1 ELSE skip
    }
  }

}
//
//-----------------------------------------------------------------------------------------------------
/**
  Check and execute the requested OOB One-Touch Request.

  @param  PlatformAuth  INPUT:  Ptr to TPM2B_AUTH, Platform auth value
                                NULL means no platform auth change
  @param  State         OUTPUT: State of performed OOB Task

  NOTE:
     1. The control will return to the caller in the following cases:
        a) There is no OOB One-Touch request.
        b) There is an OOB One-Touch request which does not require a system reset.
     2. The control will NOT return to the caller in the following cases:
        a) There is an OOB One-Touch request which requires a system reset.
**/
UINT32
ExecuteOobOneTouchRequest (
  IN  TPM2B_AUTH  *PlatformAuth
  )
{
  // Local data
  OOB_TXT_TPM       *Ottm, *Ottp;
  OT_PLATFORM_DATA  *Opd;
  UINTN             d;
  UINT32            s, f;

  DEBUG ((EFI_D_ERROR, "[OOB] ExecuteOobOneTouchRequest() {\n"));

  // Init global data
  mMeNvStorage = FALSE; mEfiNvStorage = FALSE; mNvStorageSize = 0;       // ME/EFI NV Storage Not Supported, Max NV Storage Size in bytes
  mAllocatedMemory = NULL; mAllocatedSize = 0;                           // Allocated memory for NV Storage, Size of Allocated memory for NV Storage
  mOttMe = NULL; mOttEfi = NULL; mMeDataSize = 0; mEfiDataSize = 0;      // Ptr to OOB data for ME/EFI Storage, OOB data size for ME/EFI

  // Init local data
  s = 0; f = 0;                                                          // State of different operations, Fatal Error information

  // Check if S4 resume
  if (GetBootModeHob() == BOOT_ON_S4_RESUME) {                           // If S4 Resume, Skip processing OOB request
    DEBUG ((EFI_D_ERROR, "      S4 Resume: OOB request Not Processed, State 0x%08x\n", s));
    DEBUG ((EFI_D_ERROR, "[OOB] ExecuteOobOneTouchRequest() }\n"));
    return s;                                                            //   Assume success
  }

  // ------------------	Read Platform Information ----------------------------------------
  // Allocate memory for platform information
  Opd = OT_AllocatePlatformInformationArea();                            // Allocate memory
  if (Opd == NULL) {                                                     // If Memory not available
    f |= OOB_FATAL_MEMORY_NOT_AVAILABLE_BIT;                             //   Update fatal error info
  } else {                                                               // If memory available
    // Read platform information
    if (!(OT_ReadPlatformInformation (Opd))) {                           // If Read platform information not success
      f |= OOB_FATAL_PLATFORM_DATA_READ_ERROR_BIT;                       //   Update fatal error info
    }
  }

  // ------------------ Validate OOB Data Non-Volatile Storage Support -------------------
  // Validate OOB Non-Volatile Storage support
  d = 0; Ottm = NULL; Ottp = NULL;                                       // Init Non-volatile Storage Size in bytes, ptr to current/previous OOB data
  DEBUG ((EFI_D_ERROR, "[OOB] Non-Volatile Storage Supported? "));
  if (OobNvStorageSupported()) {                                         // If non-volatile Storage supported
    DEBUG ((EFI_D_ERROR, "Yes\n"));
    DEBUG ((EFI_D_ERROR, "      ME Non-Volatile Storage: "));
    if (mMeNvStorage) { DEBUG ((EFI_D_ERROR, "Supported\n")); }
    else { DEBUG ((EFI_D_ERROR, "Not Supported\n")); }
    DEBUG ((EFI_D_ERROR, "      EFI Non-Volatile Storage: "));
    if (mEfiNvStorage) { DEBUG ((EFI_D_ERROR, "Supported\n")); }
    else { DEBUG ((EFI_D_ERROR, "Not Supported\n")); }
    FindOobNvStorageSize (&mNvStorageSize);                              //   Max NV Storage size in bytes
    DEBUG ((EFI_D_ERROR, "      Maximum size of OOB Non-Volatile Storage: 0x%08x bytes\n", mNvStorageSize));
  } else {                                                               // If non-volatile Storage not supported
    DEBUG ((EFI_D_ERROR, "No\n"));
    f |= OOB_FATAL_NV_STORAGE_NOT_SUPPORTED_BIT;                         //   Update fatal error info
  }

  // ------------------ Allocate memory for reading OOB data from non-volatile storage ---
  // s = state, f = fatal error info
  d = mNvStorageSize;                                                    // Max size of NV Storage in bytes
  if (d != 0) {                                                          // If Non-volatile Storage size valid
    mAllocatedSize = (d << 2);                                           //   Required memory size
    mAllocatedMemory = ((OOB_TXT_TPM*)(UINTN)                            //   Allocate memory for current/previous OOB data
                                      AllocateZeroPool (mAllocatedSize));
    DEBUG ((EFI_D_ERROR, "      Allocate Memory for NV Storage Data (Total 0x%08x bytes): ", mAllocatedSize));
    if (mAllocatedMemory == NULL) {                                      //   If Memory not available
      DEBUG ((EFI_D_ERROR, "ERROR: Memory Not Available\n"));
      mAllocatedSize = 0;                                                //     Allocated memory size
      f |= OOB_FATAL_MEMORY_NOT_AVAILABLE_BIT;                           //     Update fatal error info
    } else {                                                             //   If Memory available
      DEBUG ((EFI_D_ERROR, "Success\n"));
      mOttMe = (OOB_TXT_TPM*)(UINTN) mAllocatedMemory;                   //     OOB data for ME
      mOttEfi = (OOB_TXT_TPM*)(UINTN) ((UINTN) mOttMe + d);              //     OOB data for EFI
      Ottm = (OOB_TXT_TPM*)(UINTN) ((UINTN) mOttEfi + d);                //     Cuurent OOB data
      Ottp = (OOB_TXT_TPM*)(UINTN) ((UINTN) Ottm + d);                   //     Previous OOB data
    }
  } else {                                                               // If Non-volatile storage size not valid
    f |= OOB_FATAL_NV_STORAGE_SIZE_ZERO_BIT;                             //   Update fatal error info
  }

  // ------------------ Check whether OOB request can be processed -----------------------
  // s = state, f = Fatal Error info (<> 0 -> FATAL Error), d = Max size of Non-volatile storage in bytes
  if (f != 0) {                                                          // If any Fatal Error
    DEBUG ((EFI_D_ERROR, "    OOB request cannot be processed -> State 0x%08x, Fatal Error 0x%08x\n", s, f));
    if (mAllocatedMemory != NULL) {                                      // If memory alloacted for NV storage
      // OOB data not yet read, so no need to clear OOB data area
      DEBUG ((EFI_D_ERROR, "      Deallocate Memory used for OOB Data ->\n"));
      gBS->FreePool (mAllocatedMemory);                                  //   Deallocate memory used for NV storage
    }
    DEBUG ((EFI_D_ERROR, "      Clear and Deallocate Memory used for Platform Information ->\n"));
    OT_ClearAndDeallocatePlatformInformationArea (Opd);                  // Clear and Deallocate memory used for Platform Info
    DEBUG ((EFI_D_ERROR, "[OOB] ExecuteOobOneTouchRequest() }\n"));
    return s;                                                            //   State
  }

  // ------------------ Read and validate OOB Data from Non-Volatile Storage -------------
  // Read OOB data from non-volatile storage: s = state, f = fatal error info, d = Max size of Non-volatile storage in bytes
  ReadOobDataFromNvStorage();                                            // Read OOB Data
  if (WhichOobNvStorageDataToUse ((VOID*) Ottm, &d, &s)) {               // If OOB data can be used
    DEBUG ((EFI_D_ERROR, "    Use OOB Data read from NV Storage\n"));
    CopyMem ((VOID*) Ottp, (VOID*) Ottm, d);                             //   Save original OOB Data
    //if ((d != 0) && (d <= mNvStorageSize)) {
      //DEBUG ((EFI_D_ERROR, "    Input OOB Data read from NV Storage: Size = 0x%08x bytes\n", d));
      //DisplayBinaryData ((UINT8*)(UINTN) Ottm, d);                       // Display OOB Input Data
    //}
  } else {                                                               // If OOB Data cannot be used
    DEBUG ((EFI_D_ERROR, "    Use Default Use-Case 0x01 (OOB Data read from NV Storage Not used)\n"));
    InitOobOneTouchData (Ottm, Opd, &d);                                 //   OOB Data for default Use-Case
    s |= OXP_STATE_OOB_DEFAULT_TASK_USED_BIT;                            //   Indicate default Use-Case
  }

  // s = state, d = OOB data size in bytes as read from NV storage, dm = Max size of NV storage in bytes
  ValidateOobOneTouchInputData (Ottm, &d, &s);                           // Validate OOB One-Touch Input Data

  // ------------------ Display OOB Data to be processed ---------------------------------
  if ((d != 0) && (d <= mNvStorageSize)) {
    DEBUG ((EFI_D_ERROR, "      Input OOB Data to be processed: Size = 0x%08x bytes\n", d));
    DisplayBinaryData ((UINT8*)(UINTN) Ottm, d);                         // Display OOB Input Data
  }

  // ------------------ Perform OOB task -------------------------------------------------
  // s = state, f = fatal error, d = OOB data size in bytes as read from NV storage
  DEBUG ((EFI_D_ERROR, "    Updated State 0x%08x, Fatal Error 0x%08x\n", s, f));
  if (Opd != NULL) {                                                     // If platform ptr valid
    if (f == 0) {                                                        //   If success so far
      PerformOobOneTouchTask (PlatformAuth, Ottm, Opd, &s);              //     Perform the task
      DEBUG ((EFI_D_ERROR, "      OOB Data after processing (before updating Checksum, State): Size = 0x%08x\n", Ottm->Header.TotalLength));
      DisplayBinaryData ((UINT8*)(UINTN) Ottm, Ottm->Header.TotalLength);//     Display OOB Data
    }
    // ------------------ Write Platform Info and write OOB Data -------------------------
    // s = state, f = fatal error, d = OOB data size in bytes as read from NV storage
    if (!(OT_WritePlatformInformation (Opd))) {                          //   If Write platform information not success
      DEBUG ((EFI_D_ERROR, "      Platform Information Write Error\n"));
      s |= OXP_STATE_PLATFORM_DATA_WRITE_ERROR_BIT;                      //     Error in writing platform information
    }
    // Write OOB One-Touch Data: s = updated state, d = OOB data size in bytes as read from NV storage
    if ((s != 0) || ((CompareMem ((UINT8*)(UINTN) Ottp,                  //   If state non-zero OR OOB Data modified
                                  (UINT8*)(UINTN) Ottm, d)) != 0)) {
      DEBUG ((EFI_D_ERROR, "      State Non-Zero OR OOB Data modified\n"));
      if ((f == 0) && ((s & OXP_STATE_SIGNATURE_INVALID_BIT) == 0) &&    //     If no fatal error AND valid signature present in OOB data AND OOB data size non-zero
          (d != 0)) {
        DEBUG ((EFI_D_ERROR, "    OOB input data size <> 0 AND valid input signature: Generate OOB Output Data\n"));
        GenerateOobOneTouchOutputData (Ottm, Opd, &d, &s);               //       Generate OOB Output Data
        DEBUG ((EFI_D_ERROR, "      OOB Output Data Size = 0x%08x bytes, State = 0x%08x\n", d, s));
        if ((d != 0) && (d <= mNvStorageSize)) {                         //       If valid data size
          DisplayBinaryData ((UINT8*)(UINTN) Ottm, d);                   //         Display OOB Data
          DEBUG ((EFI_D_ERROR, "      Write OOB Output Data ->\n"));
          if (!(WriteOobDataToNvStorage ((VOID*)(UINTN) Ottm, &d, s))) { //         If Write OOB Data to NV Storage not success
            f |= OOB_FATAL_OOB_DATA_WRITE_ERROR_BIT;                     //           OOB data write error
          }
        } else {
          DEBUG ((EFI_D_ERROR, "    OOB Output Data Size either = 0 or > Max Size of Non-Volatile Storage\n"));
        }
      } else {                                                           //       If OOB data read not success OR valid signature not present in OOB data
        DEBUG ((EFI_D_ERROR, "    OOB input data size either = 0 or Invalid signature: Do not write OOB Data\n"));
      }
    } else {                                                             //   If state zero AND OOB Data not modified
      DEBUG ((EFI_D_ERROR, "      State Zero AND OOB Data not modified: No need to write OOB Data\n"));
    }
    DEBUG ((EFI_D_ERROR, "      Updated State: 0x%08x, Fatal Error: 0x%08x\n", s, f));
    GenerateSmbiosStructuresOTA (Ottm, Opd);                             // Generate OTA SMBIOS Structures
  } else {
    DEBUG ((EFI_D_ERROR, "      Platform Information and OOB Data not written, SMBIOS Structures not generated\n"));
    DEBUG ((EFI_D_ERROR, "      FATAL ERROR: Ptr NULL => BUG\n"));
  }

  // ------------------ Deallocate memory used for Platform Info and OOB Data ------------
  if ((mAllocatedMemory != NULL) && (mAllocatedSize != 0)) {
    DEBUG ((EFI_D_ERROR, "    Clear Memory used for OOB Data ->\n"));
    ZeroMem ((UINT8*)(UINTN) mAllocatedMemory, mAllocatedSize);          // Clear data area
    DEBUG ((EFI_D_ERROR, "    Deallocate Memory used for OOB Data ->\n"));
    gBS->FreePool ((VOID*)(UINTN) mAllocatedMemory);                     // Deallocate memory used for OOB data
  }
  if (Opd != NULL) {
    DEBUG ((EFI_D_ERROR, "    Clear and Deallocate Memory used for Platform Information ->\n"));
    OT_ClearAndDeallocatePlatformInformationArea (Opd);                  // Clear and Deallocate memory used for Platform Info
  }

  // ------------------ Check if reset required: s = updated state -----------------------
  if ((s & OXP_STATE_RESET_BIT) != 0) {                                  // If Reset required
    DEBUG ((EFI_D_ERROR, "      Reset Required: Issue system reset....\n"));
    DEBUG ((EFI_D_ERROR, "[OOB] ExecuteOobOneTouchRequest() }\n"));
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);               //   Reset system
    CpuDeadLoop();
  }

  DEBUG ((EFI_D_ERROR, "      No task to perform OR Reset not required for the performed task\n"));
  DEBUG ((EFI_D_ERROR, "[OOB] ExecuteOobOneTouchRequest() }\n"));

  return s;                                                              // State
}

//-----------------------------------------------------------------------------------------------------
