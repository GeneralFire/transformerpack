/** @file
  S3M library.

  This file implements the S3mLib APIs.

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

//
// Statements that include other files
//
#include <KtiMisc.h>
#include "Include/S3mLibInternal.h"
#include <Library/S3mLib.h>
#include <Library/S3mIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/CpuPreMemPolicyLib.h>
#include <CpuDataStruct.h>
#include <Library/PcuIpLib.h>
#include <Library/ChaLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/MemRcLib.h>
#include <KtiSetupDefinitions.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Memory/MemDefaults.h>
#include <S3mMailBox.h>

CHAR8 S3M_PROVISIONING_MESSAGE[] = "S3M Provisioning SoftStrap %a";

/**
  Dump S3M Configuration and Softstrap contents.

  @param[in]      SocketId          The ID of the Socket.
  @param[in out]  Buffer            Pointer of the buffer that contains the contents to be dumped.
  @param[in out]  BufSize           Buffer size(Unit: Number of DWs).
  @param[in]      IsAllContents     TRUE: The contents in Buffer contains all S3M CPLD registers
                                    FALSE:The contents in Buffer contains SoftStrap Data only

  @retval KTI_SUCCESS               Dump S3M CPLD register contents successfully.
  @retval KTI_FAILURE               Dump S3M CPLD register contents failed.
**/
KTI_STATUS
EFIAPI
DumpS3mConfigAndSoftstrapSet (
  IN  UINT8     SocketId,
  IN  UINT32    *Buffer,
  IN  UINT32    BuffSize,
  IN  BOOLEAN   IsAllContents
  )
{
  UINT32             Index;
  UINT32             StrapLength;
  UINT32             *StrapBaseAddr;
  S3M_DATA_FORMAT    *S3mDataPtr;

  if (Buffer == NULL) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a () - Buffer pointer is NULL\n", __FUNCTION__));
    return KTI_FAILURE;
  }

  if (IsAllContents) {
    S3mDataPtr = (S3M_DATA_FORMAT *) Buffer;

    StrapBaseAddr = (UINT32 *) &S3mDataPtr->StrapSet;
    StrapLength = S3mDataPtr->StrapLength;

    if (BuffSize < (sizeof (S3M_DATA_FORMAT) / sizeof (UINT32))) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nFatal Error: Data Incomplete!\n"));
      return KTI_FAILURE;
    }

    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  Configuration Length: 0x%08X\n", S3mDataPtr->ConfigLength));
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  Config: TSC_OFFSET  : 0x%08X\n", S3mDataPtr->Config));
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  CPU SoftStrap Length: 0x%08X\n", S3mDataPtr->StrapLength));
  } else {
    StrapBaseAddr = Buffer;
    StrapLength = BuffSize;
  }

  for (Index = 0; Index < StrapLength; Index++) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  SoftStrap DWORD[%02X] : 0x%08X\n", Index + 1, *(StrapBaseAddr + Index)));
  }

  return KTI_SUCCESS;
}

/**
  Collect CPU strap contents from all sockets.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS        Collect S3M CPU Strap contents successfully
  @retval KTI_FAILURE        Failed to collect S3M CPU Strap contents
  @retval KTI_IGNORE         Not supported
**/
KTI_STATUS
EFIAPI
CollectS3mCpuStrapContents (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  EFI_STATUS              Status;
  UINT8                   SocketId, RetryCnt;
  UINT32                  BufSize;
  UINT32                  Buffer[S3M_CPLD_DATA_BLOCK_SIZE];
  BOOLEAN                 IsGlobalReset;
  UINT8                   ResetRequired;

  ResetRequired = GetSysResetRequired ();
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Current ResetRequired: 0x%02X\n", ResetRequired));
  if ((ResetRequired & POST_RESET_WARM) != 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  There is pending warm reset required, skip S3M SoftStrap read\n"));
    return KTI_SUCCESS;
  }

  //
  // ToDo: Get the Global Reset indicator
  //
  IsGlobalReset = FALSE;
  //
  // S3M CPU Strap contents will be cleared if global reset, so no need to read.
  //
  if (IsGlobalReset) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a () - Skip the read of S3M CPU Strap Contents since Global Reset is detected.\n"));
    return KTI_SUCCESS;
  }

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (SocketData->Cpu[SocketId].Valid == FALSE) {
      continue;
    }
    if (SocketData->Cpu[SocketId].SocType != SOCKET_TYPE_CPU) {
      continue;
    }

    RetryCnt = RETRY_COUNT;
    while (RetryCnt > 0) {
      ZeroMem ((VOID *) Buffer, sizeof (Buffer));
      BufSize = S3M_CPLD_DATA_BLOCK_SIZE;

      Status = ReadS3mCpuStrap (SocketId, Buffer, &BufSize);
      if (Status == EFI_SUCCESS) {
        break;
      }
      RetryCnt--;
    }

    if ((EFI_ERROR (Status)) && (RetryCnt == 0)) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Retry %X times to read S3M CPU Strap contents from Socket[%X], all failed!\n", RETRY_COUNT, SocketId));
      return KTI_FAILURE;
    }

    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nDump S3M Configuration and Softstrap begin - Socket[%X]\n", SocketId));
    DumpS3mConfigAndSoftstrapSet (SocketId, Buffer, BufSize, TRUE);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Dump S3M Configuration and Softstrap end\n\n"));

    CopyMem ((VOID *) KTIVAR->S3mInfo[SocketId], (VOID *) Buffer, sizeof (Buffer));
  }

  return KTI_SUCCESS;
}

/**
  Collect current CPU strap configurations from all sockets.

  @param[in]                KtiHostOutPtr  - A pointer to the KTI_HOST_OUT data structure
  @param[in][out]           CpuStrapSet    - A pointer to the CPU_STRAP_SET variable that contains the current CPU Strap settings

  @retval                   VOID
**/
VOID
EFIAPI
CollectCurrentCpuStrapConfigurations (
  IN      KTI_HOST_OUT   *KtiHostOutPtr,
  IN OUT  CPU_STRAP_SET  *CpuStrapSet
  )
{
  UINT8                SocketId, Index;
  UINT8                NumOfCluster, SncPrefetchEn;
  UINT8                KtiPerPortDisable;
  UINT8                PcieGen4SpeedLimit;
  UINT8                UmaBasedClustring;
  UINT8                SncEn;
  CPU_PRE_MEM_POLICY   *CpuPolicy;
  KTI_HOST_IN          *KtiHostInPtr;
  STRAP_SET_LIST       *StrapSetListInS3m;
  BOOLEAN              TwoLMEnabled;

  KtiHostInPtr = KTI_HOST_IN_PTR;
  CpuPolicy = GetCpuPreMemPolicy();
  SncEn = KtiHostOutPtr->OutSncEn;
  UmaBasedClustring = KtiHostOutPtr->OutUmaClustering;

  if (SncEn == KTI_SNC2 ||
      SncEn == KTI_SNC4) {
    NumOfCluster = SncEn;
  } else if (UmaBasedClustring == UMABASEDCLUSTERING_HEMISPHERE ||
             UmaBasedClustring == UMABASEDCLUSTERING_QUADRANT) {
    NumOfCluster = UmaBasedClustring;
  } else {
    NumOfCluster = 1;
  }
  SncPrefetchEn = KtiHostOutPtr->OutSncPrefetchEn;

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    //
    // For DW1.IioStackDis and DW4.IioStackDisHigh, each bit is used to disable
    // a given MS2IOSF by overriding the fuse enable. Technical speaking, BIOS should
    // use runtime discovered IIO_DISABLE status to update IIO_STACK_DIS bits, however
    // 1) current bios implementation, both of PcuGetMs2IosfStacksEn() and
    //    GetStackPresentBitMap () are from CAPID10 register, they have the same values
    //    - PcuGetMs2IosfStacksEn returns CAPID value, which indicates fused enabled stack,
    //    - GetStackPresentBitMap returns runtime value
    // 2) Current SPR BIOS doesn't have option for user to override fuse enabled stacks.
    //    There is no need for BIOS to update IIO_STACK_DIS bits in S3M.
    // So bios is using S3M data to update IIO_STACK_DIS bits, that's to say,
    // in function IsS3mProvisioningRequired(), bios doesn't need to compare the 12 bits
    // IIO_STACK_DIS to determine whether need to do S3M provisioning or not.
    // In the future, if need to support override fuse enabled IIO stack(s), bios may need
    // to make change here.
    //
    StrapSetListInS3m = &(((S3M_CONFIG_SOFTSTRAP_PAYLOAD *) KtiHostOutPtr->S3mInfo[SocketId])->DataBlob.StrapSet);
    KtiPerPortDisable = 0;
    for (Index = 0; Index < KtiHostOutPtr->KtiPortCnt; Index++) {
      if ((KtiHostOutPtr->KtiPortPresentBitmap[SocketId] & (1 << Index)) == 0) {
        continue;
      }
      KtiPerPortDisable |= (KtiHostInPtr->PhyLinkPerPortSetting[SocketId].Link[Index].KtiPortDisable << Index);
    }

    PcieGen4SpeedLimit = GetPcieGen4SpeedLimit (SocketId);
    ChaCheck2LMEnabled (SocketId, 0, &TwoLMEnabled);
    //
    // SoftStrap - DW1
    //
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.ConfigTdpLevel = CpuPolicy->ConfigTdpLevel;
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.AdrResumeCoreCount = 0;  //Bits[27:26]: Leave it to default setting 0
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.AdrResume = 0; //Bits[25]: Leave it to default setting 0
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.IioStackDis = StrapSetListInS3m->Dword1.Bits.IioStackDis;
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.TwoLmEn = TwoLMEnabled;
    if (GetSocketMemSize (SocketId) == 0) {
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.SncCluster = 0;
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.SncIndEn = 0;
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.SncEn = 0;
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.UmaEn = 0;
    } else {
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.SncCluster = (NumOfCluster == 4) ? 1 : 0;
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.SncIndEn = (SncPrefetchEn != 0) ? 1 : 0;
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.SncEn = (SncEn > 0) ? 1 : 0;
      CpuStrapSet->StrapSet[SocketId].Dword1.Bits.UmaEn = (UmaBasedClustring > 0) ? 1 : 0;
    }
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.NonCoreHighVoltageMode = 0; //Bits[10]: Leave it to default setting 0b
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.CoreHighVoltageMode = 0; //Bits[09]: Leave it to default setting 0b
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.FlexRatio = CpuPolicy->flexRatioNext;
    CpuStrapSet->StrapSet[SocketId].Dword1.Bits.SmtDisable = (CpuPolicy->smtEnable == 1) ? 0 : 1;

    //
    // SoftStrap - DW2
    //
    CpuStrapSet->StrapSet[SocketId].Dword2.Data = (UINT32)CpuPolicy->CoreDisableMask[SocketId];

    //
    // SoftStrap - DW3
    //
    CpuStrapSet->StrapSet[SocketId].Dword3.Data = (UINT32)(RShiftU64(CpuPolicy->CoreDisableMask[SocketId],32));

    //
    // SoftStrap - DW4
    //
    CpuStrapSet->StrapSet[SocketId].Dword4.Bits.UpiDisable = (KtiPerPortDisable & 0xF);
    CpuStrapSet->StrapSet[SocketId].Dword4.Bits.IioStackDisHigh = StrapSetListInS3m->Dword4.Bits.IioStackDisHigh;
    CpuStrapSet->StrapSet[SocketId].Dword4.Bits.LimitPaTo46 = CpuPolicy->CpuPaLimit;
    CpuStrapSet->StrapSet[SocketId].Dword4.Bits.PcieGen4SpeedLimit = (PcieGen4SpeedLimit & 0x3F);
    CpuStrapSet->StrapSet[SocketId].Dword4.Bits.NoC = 0;  //Bits[15:00]: Leave it to be default 0. CSR_DESIRED_CORES_MASK2_PCU_FUN1_REG is reserved.

    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nDump current Softstrap begin - Socket[%X]\n", SocketId));
    DumpS3mConfigAndSoftstrapSet (SocketId, (UINT32 *) &CpuStrapSet->StrapSet[SocketId].Dword1.Data, (sizeof (STRAP_SET_LIST) / sizeof (UINT32)), FALSE);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Dump current Softstrap end\n\n"));
  }

  return;
}

/**
  Routine to check if need to do S3M Strap provisioning for each socket.

  @param[in]    KtiHostOutPtr  - A pointer to the KTI_HOST_OUT data structure
  @param[in]    CpuStrapSet    - A pointer to the CPU_STRAP_SET variable that contains the current CPU Strap settings
  @param[in]    IsGlobalReset  - TRUE: Global Reset detected; FALSE: Not Global Reset

  @retval       TRUE           - Need to do S3M Strap provisioning
  @retval       FALSE          - No need
**/
BOOLEAN
EFIAPI
IsS3mProvisioningRequired (
  IN  KTI_HOST_OUT   *KtiHostOutPtr,
  IN  CPU_STRAP_SET  *CpuStrapSet,
  IN  BOOLEAN        IsGlobalReset
  )
{
  UINT8                         SocketId;
  INTN                          CmpResult;
  S3M_CONFIG_SOFTSTRAP_PAYLOAD  *S3mPayloadPtr;

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    if (IsGlobalReset) {
      KtiHostOutPtr->OverrideStrapSetCpuMask |= (1 << SocketId);
    } else {
      S3mPayloadPtr = (S3M_CONFIG_SOFTSTRAP_PAYLOAD *) KtiHostOutPtr->S3mInfo[SocketId];

      CmpResult = CompareMem ((VOID *) &S3mPayloadPtr->DataBlob.StrapSet.Dword1.Data,
                              (VOID *) &CpuStrapSet->StrapSet[SocketId].Dword1.Data,
                              sizeof (STRAP_SET_LIST)
                             );
      if (CmpResult != 0) {
        KtiHostOutPtr->OverrideStrapSetCpuMask |= (1 << SocketId);
      }
    }
  }

  return ((KtiHostOutPtr->OverrideStrapSetCpuMask != 0) ? TRUE : FALSE);
}

/**
  Routine to prepare S3M Config and Strapset payload and program into S3M.

  @param[in]    KtiHostOutPtr  - A pointer to the KTI_HOST_OUT data structure
  @param[in]    CpuStrapSet    - A pointer to the CPU_STRAP_SET variable that contains the current CPU Strap settings
  @param[in]    IsGlobalReset  - TRUE: Global Reset detected; FALSE: Not Global Reset

  @retval       KTI_SUCCESS    - S3M provisioning successfully
  @retval       KTI_FAILURE    - S3M provisioning failed
**/
KTI_STATUS
EFIAPI
ProgramS3mCpldRegisters (
  IN  KTI_HOST_OUT   *KtiHostOutPtr,
  IN  CPU_STRAP_SET  *CpuStrapSet,
  IN  BOOLEAN        IsGlobalReset
  )
{
  EFI_STATUS                      Status;
  UINT8                           SocketId;
  UINT8                           RetryCnt;
  S3M_CONFIG_SOFTSTRAP_PAYLOAD    S3mPayload;

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if ((KtiHostOutPtr->OverrideStrapSetCpuMask & (1 << SocketId)) == 0) {
      continue;
    }

    ZeroMem ((VOID *) &S3mPayload, sizeof (S3mPayload));

    S3mPayload.DataBlob.ConfigLength = sizeof (S3M_CONFIG) / sizeof (UINT32);  //Unit: in DWs
    //
    // ToDo: Need to check how to initialize the TSC_TRANSFER_OFFSET register?
    //
    S3mPayload.DataBlob.Config.TscOffset.Data = 0;
    S3mPayload.DataBlob.StrapLength = sizeof (STRAP_SET_LIST) / sizeof (UINT32);  //Unit: in DWs

    CopyMem ((VOID *) &S3mPayload.DataBlob.StrapSet.Dword1.Data,
             (VOID *) &CpuStrapSet->StrapSet[SocketId].Dword1.Data,
             sizeof (STRAP_SET_LIST)
            );

    RetryCnt = RETRY_COUNT;
    while (RetryCnt > 0) {
      Status = OverrideS3mCpuStrap (SocketId, (UINT32 *) &S3mPayload, (sizeof (S3M_DATA_FORMAT) / sizeof (UINT32)));
      if (Status == EFI_SUCCESS) {
        break;
      }
      RetryCnt--;
    }

    if ((EFI_ERROR (Status)) && (RetryCnt == 0)) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Retry %X times to program S3M CPLD register for Socket[%X], all failed!\n", RETRY_COUNT, SocketId));
      return KTI_FAILURE;
    }
  }

  return KTI_SUCCESS;
}

/**
  S3M CPU strap provision flow main entry.

  @param            - VOID

  @retval           KTI_SUCCESS
  @retval           KTI_IGNORE   Feature not supported
**/
KTI_STATUS
EFIAPI
S3mCpuStrapProvision (
  VOID
  )
{
  EFI_STATUS       Status;
  UINT8            ResetRequired;
  BOOLEAN          IsGlobalReset;
  BOOLEAN          ProvisioningRequired;
  CPU_STRAP_SET    CpuStrapSet;
  KTI_HOST_OUT     *KtiHostOutPtr;

  IsGlobalReset = FALSE;
  //
  // ToDo: Get the Global Reset indicator
  //

  ResetRequired = GetSysResetRequired ();
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Current ResetRequired: 0x%02X\n", ResetRequired));
  if ((ResetRequired & POST_RESET_WARM) != 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  There is pending warm reset required, skip S3M SoftStrap provision\n"));
    return KTI_SUCCESS;
  }

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  ZeroMem ((VOID *) &CpuStrapSet, sizeof (CPU_STRAP_SET));

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, S3M_PROVISIONING_MESSAGE, "start...\n"));

  CollectCurrentCpuStrapConfigurations (KtiHostOutPtr, &CpuStrapSet);

  ProvisioningRequired = IsS3mProvisioningRequired (KtiHostOutPtr, &CpuStrapSet, IsGlobalReset);

  if (!ProvisioningRequired) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, S3M_PROVISIONING_MESSAGE, "not required.\n\n"));
    return KTI_SUCCESS;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, S3M_PROVISIONING_MESSAGE, "required for sockets in this bitmap: "));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "0x%02X\n", KtiHostOutPtr->OverrideStrapSetCpuMask));

  Status = ProgramS3mCpldRegisters (KtiHostOutPtr, &CpuStrapSet, IsGlobalReset);
  if (EFI_ERROR (Status)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, S3M_PROVISIONING_MESSAGE, "failed!\n\n"));
    return KTI_FAILURE;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, S3M_PROVISIONING_MESSAGE, "successfully completed - Set Cold Reset required.\n\n"));
  SetSysResetRequired (ResetRequired | POST_RESET_POWERGOOD);

  return KTI_SUCCESS;
}

/**
  Select the most suitable image in the image list.

  @param[in]         ImageList  - A pointer to the image list.
  @param[out]        Image    - A pointer to selected image.
  @param[out]        ImageSize - Selected image size.
  @param[in]         PatchType - Select S3m patch or Pcode patch.

  @retval           KTI_SUCCESS
**/
KTI_STATUS
GetTheBestImage (
  UINT8           **ImageList,
  UINT8           **Image,
  UINT32          *ImageSize,
  UINTN           PatchType
)
{
  UINTN                   Index;

  //
  // List all images in image list.
  //
  Index = 0;
  while (TRUE) {
    if (ImageList[Index * 2] == NULL) {
      break;
    }
    if (((PatchType == S3M_FW_PATCH) && S3M_IS_S3M_FW_CFR_IMAGE(ImageList[Index * 2])) ||
      ((PatchType == P_U_CODE_PATCH) && S3M_IS_PUCODE_CFR_IMAGE(ImageList[Index * 2]))
     ) {
      *Image = ImageList[Index * 2];
      *ImageSize = (UINT32)((UINT32*)ImageList)[Index * 2 + 1];
      return EFI_SUCCESS;
    }
    Index ++;
  }
  return EFI_NOT_FOUND;
}

/**
  Check if the image is acceptable image.

  @param[in]        SocketId  - The socket id will be provision.
  @param[in]        PatchImage    - A pointer to cfr image.

  @retval           TRUE  Accepteable.
  @retval           FALSE Unaccepteable.
**/
BOOLEAN
CheckValidCfrImage (
  UINT8         SocketId,
  UINT8        *Image
)
{
  UINT32      PatchRevID;
  UINT32      PatchSVN;
  UINT32      SVN;
  UINT32      RevId;
  BOOLEAN     IsCommitted;
  EFI_STATUS  Status;
  //
  //  Dump the patch image info for check.
  //
  PatchRevID = ((S3M_COMMON_CSS_HEADER*)Image)->UniqueID;
  PatchSVN = ((S3M_COMMON_CSS_HEADER*)Image)->SVN;

  SVN = 0;
  RevId = 0;
  IsCommitted = 0;
  Status = EFI_SUCCESS;
  if (S3M_IS_S3M_FW_CFR_IMAGE(Image)) {
    Status = GetS3mCfrValidVersionInfo(SocketId, &SVN, &RevId, &IsCommitted);
  } else if (S3M_IS_PUCODE_CFR_IMAGE(Image)) {
    Status = GetPucodeCfrValidVersionInfo(SocketId, &SVN, &RevId, &IsCommitted);
  }
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_INFO, "Can't Get Version Info: %r\n", Status));
    return FALSE;
  }
  if (PatchRevID > RevId && PatchSVN >= SVN) {
    return TRUE;
  }
  return FALSE;
}

/**
  Provision Cfr to each socket.

  @param[in]        Image  - The CFR image.
  @param[out]       NewPatchProvisioned - Indicate there is at least one socket be provisioned.

  @retval           KTI_SUCCESS.
**/
KTI_STATUS
S3mCfrPatchProvisionOnEachSocket (
  UINT8          *Image,
  UINT32         ImageSize,
  BOOLEAN        *NewPatchProvisioned
)
{
  UINT8            SocketId;
  EFI_STATUS       Status;
  UINT32           ProvisionCmd;

  Status = KTI_SUCCESS;
  *NewPatchProvisioned = FALSE;
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    if(!CheckValidCfrImage(SocketId, Image)) {
      DEBUG((EFI_D_INFO, "Socket %x Can't Provision, Skip.\n", SocketId));
      continue;
    }

    ProvisionCmd = 0;
    if (S3M_IS_S3M_FW_CFR_IMAGE(Image)) {    
      ProvisionCmd = S3M_FW_CFR_PROVISION_NO_SVN_COMMIT;
    } else if (S3M_IS_PUCODE_CFR_IMAGE(Image)) {
      ProvisionCmd = PUCODE_CFR_PROVISION_NO_SVN_COMMIT;
    } 
    Status = S3mProvision(SocketId, (UINT32*)Image, (ImageSize + sizeof(UINT32) - 1) / sizeof(UINT32), ProvisionCmd);
    if (EFI_ERROR(Status)) {
      break;
    }
    *NewPatchProvisioned = TRUE;
  }
  return KTI_SUCCESS;
}

/**
  Check if all socket has same SVN and REVID.

  @retval           TRUE - all same.
  @retval           FALSE - there at least one is not same.
**/
BOOLEAN
CheckCFRProvistionStatus (
  UINT8         SocketId,  
  UINT8         *Image  
)
{
  UINT32      PatchRevID;
  UINT32      PatchSVN;
  UINT32      SVN;
  UINT32      RevId;
  BOOLEAN     IsCommitted;
  EFI_STATUS  Status;
  //
  //  Dump the patch image info for check.
  //
  PatchRevID = ((S3M_COMMON_CSS_HEADER*)Image)->UniqueID;
  PatchSVN = ((S3M_COMMON_CSS_HEADER*)Image)->SVN;

  SVN = 0;
  RevId = 0;
  IsCommitted = 0;
  Status = EFI_SUCCESS;
  if (S3M_IS_S3M_FW_CFR_IMAGE(Image)) {
    Status = GetS3mCfrValidVersionInfo(SocketId, &SVN, &RevId, &IsCommitted);
  } else if (S3M_IS_PUCODE_CFR_IMAGE(Image)) {
    Status = GetPucodeCfrValidVersionInfo(SocketId, &SVN, &RevId, &IsCommitted);
  }
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_INFO, "Can't Get Version Info: %r\n", Status));
    return FALSE;
  }
  if (PatchRevID == RevId && PatchSVN == SVN && !IsCommitted) {
    return TRUE;
  }
  return FALSE;
}

/**
  Commit Cfr to each socket.

  @param[in]        Image  - The CFR image.

  @retval           KTI_SUCCESS.
**/
KTI_STATUS
S3mCfrPatchCommitOnEachSocket (
  UINT8         *Image,
  UINT32         ImageSize
)
{
  UINT8            SocketId;
  EFI_STATUS       Status;
  BOOLEAN          CanCommit;
  UINT32           ProvisionCmd;
  
  Status = KTI_SUCCESS;
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    CanCommit = CheckCFRProvistionStatus (SocketId, Image);
    if (!CanCommit) {
      DEBUG((EFI_D_INFO, "Socket %x can't commit, skip.\n", SocketId));
      continue;
    }

    ProvisionCmd = 0;
    if (S3M_IS_S3M_FW_CFR_IMAGE(Image)) {
      ProvisionCmd = S3M_FW_CFR_PROVISION_SVN_COMMIT;
    } else if (S3M_IS_PUCODE_CFR_IMAGE(Image)) {
      ProvisionCmd = PUCODE_CFR_PROVISION_SVN_COMMIT;
    }
    Status = S3mProvision(SocketId, (UINT32*)Image, (ImageSize + sizeof(UINT32) - 1) / sizeof(UINT32), ProvisionCmd);
    if (EFI_ERROR (Status)) {
      break;
    }
  }
  return KTI_SUCCESS;
}

/**
  S3M CFR patch provision flow main entry.

  @param            - VOID

  @retval           KTI_SUCCESS
  @retval           KTI_IGNORE   Feature not supported
**/
KTI_STATUS
EFIAPI
CfrPatchProvision (
  UINT8       *Image,
  UINT32       ImageSize
  )
{
  KTI_HOST_IN      *KtiHostInPtr;
  BOOLEAN           NewPatchProvisioned;
  UINT8             ResetRequired;

  ResetRequired = GetSysResetRequired ();

  S3mCfrPatchProvisionOnEachSocket (Image, ImageSize, &NewPatchProvisioned);
  if (NewPatchProvisioned) {
    SetSysResetRequired (ResetRequired | POST_RESET_POWERGOOD);
    DEBUG((EFI_D_INFO, "New Cfr Image Provisioned, Need Cold reset.\n"));
    return KTI_SUCCESS;
  }

  KtiHostInPtr = KTI_HOST_IN_PTR;
  if (S3M_IS_PUCODE_CFR_IMAGE(Image)) {
    if (KtiHostInPtr->S3mCFRCommit != 2) {
      DEBUG((EFI_D_INFO, "S3m fw CFR commit Disabled, return.\n"));
      return KTI_SUCCESS;
    }
  } else {
    if (KtiHostInPtr->PucodeCFRCommit != 2) {
      DEBUG((EFI_D_INFO, "Pucode CFR commit Disabled, return.\n"));
      return KTI_SUCCESS;
    }    
  }

  S3mCfrPatchCommitOnEachSocket (Image, ImageSize);
  DEBUG((EFI_D_INFO, "Provision/Commit Cfr Image Completed.\n"));
  return KTI_SUCCESS;
}

/**
  S3M CFR patch provision flow main entry.

  @param PatchType-S3M_FW_PATCH: S3m Fw CFR
                    P_U_CODE_PATCH: Pcode/Ucode CFR

  @retval           KTI_SUCCESS
  @retval           KTI_IGNORE   Feature not supported
**/
KTI_STATUS
EFIAPI
S3mCfrPatchProvision (
  UINTN           PatchType
  )
{
  UINT8            **ImageList;
  KTI_HOST_IN      *KtiHostInPtr;
  UINT8            *Image;
  EFI_STATUS        Status;
  UINT32            ImageSize;

  KtiHostInPtr = KTI_HOST_IN_PTR;
  if (KtiHostInPtr->CFRImagePtr == 0) {
    DEBUG((EFI_D_INFO, "CfrPatchProvision Disabled, Exit.\n"));
    return KTI_SUCCESS;
  }

  if (PatchType == S3M_FW_PATCH) {
    if (KtiHostInPtr->S3mCFRCommit == 0) {
    DEBUG((EFI_D_INFO, "S3mCfrPatchProvision Disabled, Exit.\n"));
      return KTI_SUCCESS;
    }
  }
  if (PatchType == P_U_CODE_PATCH) {
    if (KtiHostInPtr->PucodeCFRCommit == 0) {
      DEBUG((EFI_D_INFO, "PcodeCfrPatchProvision Disabled, Exit.\n"));
      return KTI_SUCCESS;
    }
  }

  ImageList = (UINT8**)(UINTN)KtiHostInPtr->CFRImagePtr;

  Status = GetTheBestImage (ImageList, &Image, &ImageSize, PatchType);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "S3mCfrPatchProvision didn't find S3m Cfr image, Exit.\n"));
    return KTI_FAILURE;
  }

  return CfrPatchProvision(Image, ImageSize);
}

