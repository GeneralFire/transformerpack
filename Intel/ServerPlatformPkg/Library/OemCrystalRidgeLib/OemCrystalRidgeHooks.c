/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2019 Intel Corporation. <BR>

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
// Include files
//
#include "CrystalRidge.h"
#include "NvdimmSmbusAccess.h"
#include "Nfit.h"
#include "Pcat.h"
#include <Protocol/PcatProtocol.h>
#include <GpioPinsSklH.h>
#include <PchAccess.h>
#include <BackCompatible.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/OemCrystalRidgeHooks.h>
//APTIOV_SERVER_OVERRIDE_RC_START : Removed the library instance that is not present in AMI Source
//#include <Library/EfiServerMgmtRtLib.h>
//APTIOV_SERVER_OVERRIDE_RC_END : Removed the library instance that is not present in AMI Source


/*
//
// Externals
//
extern struct SystemMemoryMapHob       *mSystemMemoryMap;
extern CR_INFO                         CrInfo;
extern BLK_CTRL_WND_INFO               BlkAndCtrlWinInfo;
extern SAD_CONTROL_REGION              SadControlRegion;
extern EFI_SMM_BASE2_PROTOCOL          *mSmmBase;
extern EFI_SMM_SYSTEM_TABLE2           *mSmst;
extern EFI_MP_SERVICES_PROTOCOL        *mMpService;
extern EFI_CPU_CSR_ACCESS_PROTOCOL     *mCpuCsrAccess;
*/

#pragma warning(disable : 4100)
#pragma optimize("", off)


/**

Routine Description:   OEM hook to modify/rebuild PCD data in buffer, before BIOS updates BIOS partition of PCD on the given AEP DIMM

  @param socket      - Socket ID
  @param ch            - Channel ID
  @param dimm       - DIMM number
  @param *cfgCurRecBuffer   - (Input/Output) Pointer to pointer to buffer containing Current Config record data
  @param cfgCurRecLength    - (Input/Output) Length of the Current Config record data

  @retval VOID          - None

**/
VOID
OemGetMemTopologyInfo(
    UINT8 *skt,
    UINT8 *ch,
    UINT8 *dimm)
{
  *skt  = (UINT8) PcdGet32(PcdOemSkuBoardSocketCount);
  *ch   = (UINT8) PcdGet32(PcdOemSkuMaxChannel);
  *dimm = (UINT8) PcdGet32(PcdOemSkuMaxDimmPerChannel);
  return;
}


VOID
OemBeforeUpdatePCDBIOSPartition(
    UINT8 skt,
    UINT8 ch,
    UINT8 dimm,
    UINT8 **cfgCurRecBuffer,
    UINT32 *cfgCurRecLength )
{
    NGN_DIMM_PLATFORM_CFG_HEADER        *headerCfgCur;
    NGN_DIMM_PLATFORM_CFG_CURRENT_BODY  *bodyCfgCur;

    //
    // Create pointers to Current Config record in buffer
    //
    headerCfgCur = (NGN_DIMM_PLATFORM_CFG_HEADER*)*cfgCurRecBuffer;
    bodyCfgCur = (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)&headerCfgCur[1];

    //
    // Place OEM specific code here
    // If you need to modify the buffer address and/or length, make sure you return the new values via the input/output pointers
    // If you make any change to the data in buffer, make sure to update the checksum field in the header
    //

    //
    //  Example below for modifying OEM ID fields
    //

    //CopyMem (headerCfgCur->OemId, "OEM_ID", 6);
    //CopyMem (&(headerCfgCur->OemTableId), "OEMTBLID", 8);
    //headerCfgCur->OemRevision = EFI_ACPI_OEM_REVISION;
    //headerCfgCur->CreatorId = EFI_ACPI_CREATOR_ID;
    //headerCfgCur->CreatorRevision = EFI_ACPI_CREATOR_REVISION;


    // Update checksum if data record in buffer has been changed
    //ComputeChecksum( (*cfgCurRecBuffer), headerCfgCur->Length, &(headerCfgCur->Checksum) ) ;

    // Update return values only if they have been changed
    //*cfgCurRecBuffer = (UINT8*) headerCfgCur;
    //*cfgCurRecLength = headerCfgCur->Length;

    return;             //  Default implementation.  No OEM-specific code
}


/**

Routine Description:   OEM hook to modify/rebuild PCD data in buffer, before BIOS updates OS partition of PCD on the given AEP DIMM

    @param socket       - Socket ID
    @param ch             - Channel ID
    @param dimm        - DIMM number
    @param *cfgOutRecBuffer   - (Input/Output) Pointer to pointer to buffer containing Config  Output record data
    @param cfgOutRecLength    - (Input/Output) Length of the Config Output record data

    @retval VOID          - None

**/
VOID
OemBeforeUpdatePCDOSPartition(
    UINT8 skt,
    UINT8 ch,
    UINT8 dimm,
    UINT8 **cfgOutRecBuffer,
    UINT32 *cfgOutRecLength )
{
    NGN_DIMM_PLATFORM_CFG_HEADER       *headerCfgOut;
    NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY  *bodyCfgOut;

    //
    // Create pointers to Config Output record in buffer
    //
    headerCfgOut = (NGN_DIMM_PLATFORM_CFG_HEADER*)*cfgOutRecBuffer;
    bodyCfgOut = (NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY*)&headerCfgOut[1];

    //
    // Place OEM specific code here
    // If you need to modify the buffer address and/or length, make sure you return the new values via the input/output pointers
    // If you make any change to the data in buffer, make sure to update the checksum field in the header
    //

    //
    //  Example below for modifying OEM ID fields
    //

    //CopyMem (headerCfgOut->OemId, "OEM_ID", 6);
    //CopyMem (&(headerCfgOut->OemTableId), "OEMTBID", 8);
    //headerCfgOut->OemRevision = EFI_ACPI_OEM_REVISION;
    //headerCfgOut->CreatorId = EFI_ACPI_CREATOR_ID;
    //headerCfgOut->CreatorRevision = EFI_ACPI_CREATOR_REVISION;

    // Update checksum if data record in buffer has been changed
    //ComputeChecksum( (*cfgOutRecBuffer), headerCfgOut->Length, &(headerCfgOut->Checksum) ) ;

    // Update return values only if they have been changed
    //*cfgOutRecBuffer = (UINT8*) headerCfgOut;
    //*cfgOutRecLength = headerCfgOut->Length;

    return;             //  Default implementation.  No OEM-specific code
}


/**

Routine Description:   OEM Hook for creation of NFIT table in the given buffer. Called before NFIT table is created by Intel code.

  @param *Table                  - Pointer to empty buffer for NFIT table
  @param  TableBufferSize    - Size of the buffer

  @retval EFI_STATUS             EFI_UNSUPPORTED - Nothing was done by OEM hook.
                                             EFI_SUCCESS  - Buffer contains the NFIT table ready for use
**/
EFI_STATUS
OemCreateNfitTable (
   UINT64  *Table,
   UINT32   TableBufferSize
   )
{
  NVDIMM_FW_INTERFACE_TABLE *NfitPtr;

  //
  // Create pointer to NFIT table in buffer
  //
  NfitPtr = (NVDIMM_FW_INTERFACE_TABLE*) Table;

  //
  //  Place OEM specific code here to create NFIT table in buffer,  and return EFI_SUCCESS
  //
   return  EFI_UNSUPPORTED;       // default - No OEM specific code
}


/**

Routine Description:   OEM Hook for modification of NFIT table in the given buffer. Called after NFIT table is created by Intel code.

  @param *Table   - Pointer to buffer containing NFIT table

  @retval VOID        - None

**/
VOID
OemUpdateNfitTable (
   UINT64  *Table
   )
{
  NVDIMM_FW_INTERFACE_TABLE *NfitPtr;
  UINT32   TableLength;

  //
  // Create pointer to NFIT table in buffer
  //
  NfitPtr = (NVDIMM_FW_INTERFACE_TABLE*) Table;
  TableLength = NfitPtr->Length;

  //
  //  Place OEM specific code here to modify NFIT table in buffer
  //  If table length is changed, make sure to update the table length field
  //  Caller will update the checksum field of the table after this function returns
  //

  //
  //  Example below for modifying OEM ID fields in the table
  //
  //CopyMem(NfitPtr->OemID, "OEM_ID", 6);
  //CopyMem(NfitPtr->OemTblID, "OEMTBLID", 8);
  //NfitPtr->OemRevision = EFI_ACPI_OEM_REVISION;
  //NfitPtr->CreatorID   = EFI_ACPI_CREATOR_ID;
  //NfitPtr->CreatorRev  = EFI_ACPI_CREATOR_REVISION;

  return;        // default - No OEM specific code
}



/**

Routine Description:   OEM Hook for creation of PCAT table in the given buffer. Called before PCAT table is created by Intel code.

  @param *Table                  - Pointer to empty buffer for PCAT table
  @param  TableBufferSize    - Size of the buffer

  @retval EFI_STATUS             EFI_UNSUPPORTED - Nothing was done by OEM hook.
                                             EFI_SUCCESS  - Buffer contains the PCAT table ready for use
**/
EFI_STATUS
OemCreatePcatTable (
   UINT64  *Table,
   UINT64   TableBufferSize
   )
{
  NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE  *PcatPtr;

  //
  // Create pointer to PCAT  table in buffer
  //
  PcatPtr = (NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE*)Table;

  //
  //  Place OEM specific code here to create PCAT table in buffer,  and return EFI_SUCCESS
  //
  return  EFI_UNSUPPORTED;       // default - No OEM specific code
}


/**

Routine Description:   OEM Hook for modification of PCAT table in the given buffer. Called after PCAT table is created by Intel code.

  @param *Table   - Pointer to buffer containing PCAT table

  @retval VOID        - None

**/
VOID
OemUpdatePcatTable (
   UINT64  *Table
   )
{
    NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE *PcatPtr;
    UINT32   TableLength;

    //
    // Create pointer to PCAT table in buffer
    //
    PcatPtr = (NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE*)Table;
    TableLength = PcatPtr->Length;

    //
    //  Place OEM specific code here to modify PCAT table in buffer
    //  If table length is changed, make sure to update the table length field
    //  Caller will update the checksum field of the table after this function returns
    //

    //
    //  Example below for modifying OEM ID fields in the table
    //
    // CopyMem (PcatPtr->OemID, "OEM_ID", 6);
    // CopyMem (PcatPtr->OemTblID, "OEMTBLID", 8);
    // PcatPtr->OemRevision = EFI_ACPI_OEM_REVISION;
    // PcatPtr->CreatorID   = EFI_ACPI_CREATOR_ID;
    // PcatPtr->CreatorRev  = EFI_ACPI_CREATOR_REVISION;

    return;        // default - No OEM specific code
}


BOOLEAN
OemInitFlushNearMemoryCacheGPI(
  VOID
  )
{
#if defined(WB_AD_SUPPORTED)
  UINT32       Data32;
  UINT16       AcpiBaseAddr;
  GPIO_CONFIG  GpioPadConfig;
  ZeroMem (&GpioPadConfig, sizeof(GPIO_CONFIG));
  GpioGetPadConfig (FLUSH_NEAR_MEMORY_CACHE_GPP_PAD, &GpioPadConfig);
  GpioPadConfig.PadMode = GpioPadModeGpio;
  GpioPadConfig.Direction = GpioDirIn;//GpioDirInInv;
  GpioPadConfig.InterruptConfig = GpioIntSmi | GpioIntLevel;
  GpioPadConfig.HostSoftPadOwn = GpioHostOwnAcpi;
  GpioSetPadConfig (FLUSH_NEAR_MEMORY_CACHE_GPP_PAD, &GpioPadConfig);
  PchAcpiBaseGet (&AcpiBaseAddr);
  Data32 = IoRead32 ((UINTN) (AcpiBaseAddr + R_PCH_SMI_EN));
  Data32 |= B_PCH_SMI_EN_GBL_SMI;
  IoWrite32 ((AcpiBaseAddr + R_ACPI_SMI_EN), Data32);
  GpioClearGpiSmiSts(FLUSH_NEAR_MEMORY_CACHE_GPP_PAD);
  return TRUE;
#else
  return FALSE;
#endif
}


/**
  @brief OEM hook for retrieving of maximum amount of time allowed to use the SMBus

  @param[out] TimeMs            Maximum amount of time allowed to use the SMBus

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER TimeMs is NULL.
**/
EFI_STATUS
OemSmbusGetMaxAccessTime (
  OUT UINT32 *TimeMs
  )
{
  if (NULL == TimeMs) {
    return EFI_INVALID_PARAMETER;
  }

  //
  //  Place OEM specific code here to modify maximal SMBus access time
  //
  return EFI_SUCCESS;
}

/**
  @brief OEM hook for SMBus access acquiring

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level
  @param[out] TsodPollingState  TSOD polling state

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
OemAcquireSmbusCtrl (
  IN  UINT8 Socket,
  IN  UINT8 Imc,
  OUT UINT8 *TsodPollingState
  )
{
  if (TsodPollingState == NULL || Socket >= MAX_SOCKET || Imc >= MAX_IMC) {
    return EFI_INVALID_PARAMETER;
  }

  *TsodPollingState = NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN;

  //
  //  Place OEM specific code here for SMBus Control acquisition
  //
  return EFI_SUCCESS;
}

/**
  @brief OEM hook for SMBus access releasing

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level
  @param[in] TsodPollingState  TSOD polling state

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
OemReleaseSmbusCtrl (
  IN UINT8 Socket,
  IN UINT8 Imc,
  IN UINT8 TsodPollingState
  )
{
  if (Socket >= MAX_SOCKET || Imc >= MAX_IMC) {
    return EFI_INVALID_PARAMETER;
  }

  //
  //  Place OEM specific code here for SMBus Control acquisition
  //
  return EFI_SUCCESS;
}
/**
  Return Date and Time from RTC in Unix format which fits in 32 bit format.

  @param NumOfSeconds - pointer to return calculated time

  @retval EFI_SUCCESS
  @retval EFI status if error occurred
**/
EFI_STATUS
OemGetTimeStamp (
  OUT UINT32 *NumOfSeconds
  )
{
//APTIOV_SERVER_OVERRIDE_RC_START : Removed the library instance that is not present in AMI Source
//  return EfiSmGetTimeStamp (NumOfSeconds);
  return EFI_SUCCESS;
//APTIOV_SERVER_OVERRIDE_RC_END : Removed the library instance that is not present in AMI Source
}

/**
  Routine Description: OEM hook for preventing ADR trigger during power failure.

  This OEM hook disables the FM_ADR_TRIGGER_N receiver input to prevent ADR trigger by setting GPIORXDIS to 1.
  The eADR handler will re-enable and assert FM_ADR_TRIGGER_N to trigger the legacy ADR after eADR flush is complete.

**/
VOID
OemPreventAdrTrigger (
  VOID
  )
{
  GPIO_CONFIG  GpioPadConfig;
  ZeroMem (&GpioPadConfig, sizeof(GPIO_CONFIG));
  GpioGetPadConfig (FM_ADR_TRIGGER_N_GPIO_PAD, &GpioPadConfig);

  //
  // Disable the FM_ADR_TRIGGER_N receiver input to prevent ADR trigger (Set the GPIO pad for output only).
  //
  GpioPadConfig.Direction = GpioDirOut;
  GpioSetPadConfig (FLUSH_NEAR_MEMORY_CACHE_GPP_PAD, &GpioPadConfig);
}
