/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>

#include <SysHost.h>
#include <RcRegs.h>
#include <PttPtpRegs.h>
#include <Platform.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/OemProcMemInitLib.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PmcLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <Library/SetupLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Changes done to resolve TPM (not found in setup) issue during S3 resume when TXT is enabled.
//#include <Library/Tpm2CommandLib.h>
//#include <Library/Tpm12CommandLib.h>
#include <Token.h>  // Use SDL token for PM base address.
#include <AmiTcg/TcgPc.h>
#include <AmiTcg/Tpm20.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done to resolve TPM (not found in setup) issue during S3 resume when TXT is enabled.
#include <Library/MmPciBaseLib.h>
#include <Library/HobLib.h>
#include <Library/SysHostPointerLib.h>
#include <UbaKti.h>
#include <Library/MeUtilsLib.h>

#include <Library/BiosGuardInitLib/BiosGuardInit.h>

#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/UpiPolicyPpi.h>
#include <Ppi/Stall.h>
#include <Ppi/PchReset.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PcuIpLib.h>
#include <Library/NmLib.h>
#include <Library/CpuAndRevisionLib.h>


//APTIOV_SERVER_OVERRIDE_RC_START:
#include <AmiMrcHooksWrapperLib/AmiMrcHooksWrapperLib.h>
#include <IioRegs.h>
#include <Library/TimerLib.h>
//APTIOV_SERVER_OVERRIDE_RC_END:

// APTIOV_SERVER_OVERRIDE_RC_START : Installs PPI to signal the TCG module to skip TPM Startup if
//                          			the TXT policy specified in the FIT (CMOS BITS) is set to enabled.

#define TXT_ACM_STATUS_HI  0xFED300A4
#define AMI_SKIP_TPM_STARTUP_GUID \
    { 0x6ee1b483, 0xa9b8, 0x4eaf, 0x9a, 0xe1, 0x3b, 0x28, 0xc5, 0xcf, 0xf3,\
      0x6b}

static EFI_GUID gAmiSkipTpmStartupGuid = AMI_SKIP_TPM_STARTUP_GUID;

static EFI_PEI_PPI_DESCRIPTOR gPpiList[] = {
    EFI_PEI_PPI_DESCRIPTOR_PPI
    | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gAmiSkipTpmStartupGuid,
    NULL
};

//
// APTIOV_SERVER_OVERRIDE_RC_END : Installs PPI to signal the TCG module to skip TPM Startup if
//                          		the TXT policy specified in the FIT (CMOS BITS) is set to enabled.
/**

  Build KTI EParam Host Structure and save it into a HOB for SMM KTI online routine.

  @param Host - pointer to sysHost structure on stack.

**/
VOID
SaveKtiEParamDataToHob (
  VOID
  )
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_KTI_EPARAM_UPDATE_TABLE  KtiEparamTable;
  UINTN                             TableSize;
  KTI_EPARAM_LINK_INFO              KtiEparamLinkInfo;
  KTI_PRESET_COEFF_INFO             KtiPresetCoeffInfo;

  DEBUG ((EFI_D_INFO, " [%a] - HOB\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"[%a] Locate UbaConfigPPi failure - %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  TableSize = sizeof (KtiEparamTable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformKtiEparamUpdateDataGuid,
                                 &KtiEparamTable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR,"[%a] Get platform kti lane eparam data failure - %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  ASSERT (KtiEparamTable.Signature == PLATFORM_KTIEP_UPDATE_SIGNATURE);
  ASSERT (KtiEparamTable.Version == PLATFORM_KTIEP_UPDATE_VERSION);

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    //
    // SPR uses a different way for eparam programming
    //    Per lane eparam table --- Preset coeff for each lane (PH_TCL[x])
    //    All lanes eparam table --- Safe Preset coeff for the whole link (PH_TCC)
    //
    ZeroMem ((UINT8 *) &KtiPresetCoeffInfo, sizeof(KtiPresetCoeffInfo));

    KtiPresetCoeffInfo.CountOfAllLanesEparamLink = (UINT32) (KtiEparamTable.SizeOfAllLanesEparamTable / sizeof(ALL_LANES_PRESET_COEFF_LINK_INFO));
    KtiPresetCoeffInfo.CountOfPerLaneEparamLink = (UINT32) (KtiEparamTable.SizeOfPerLaneEparamTable / sizeof(PER_LANE_PRESET_COEFF_LINK_INFO));

    if (KtiPresetCoeffInfo.CountOfAllLanesEparamLink > 0) {
      CopyMem ((UINT8 *)&(KtiPresetCoeffInfo.AllLanesPresetCoeffLinkInfo), (UINT8 *)KtiEparamTable.AllLanesEparamTablePtr, KtiEparamTable.SizeOfAllLanesEparamTable);
      DEBUG ((EFI_D_INFO,"[%a] CountOfAllLanesEparamLink: 0x%04X\n", __FUNCTION__, KtiPresetCoeffInfo.CountOfAllLanesEparamLink));
    }

    if (KtiPresetCoeffInfo.CountOfPerLaneEparamLink > 0) {
      CopyMem ((UINT8 *)&(KtiPresetCoeffInfo.PerLanePresetCoeffLinkInfo), (UINT8 *)KtiEparamTable.PerLaneEparamTablePtr, KtiEparamTable.SizeOfPerLaneEparamTable);
      DEBUG ((EFI_D_INFO,"[%a] CountOfPerLaneEparamLink: 0x%04X\n", __FUNCTION__, KtiPresetCoeffInfo.CountOfPerLaneEparamLink));
    }

    BuildGuidDataHob (
                &gEfiKtiEparamInfoGuid,
                &KtiPresetCoeffInfo,
                sizeof(KTI_PRESET_COEFF_INFO)
            );
  } else {
    ZeroMem ((UINT8 *) &KtiEparamLinkInfo, sizeof(KtiEparamLinkInfo));

    KtiEparamLinkInfo.CountOfAllLanesEparamLink = (UINT32) (KtiEparamTable.SizeOfAllLanesEparamTable / sizeof(ALL_LANES_EPARAM_LINK_INFO));
    KtiEparamLinkInfo.CountOfPerLaneEparamLink = (UINT32) (KtiEparamTable.SizeOfPerLaneEparamTable / sizeof(PER_LANE_EPARAM_LINK_INFO));

    if (KtiEparamLinkInfo.CountOfAllLanesEparamLink > 0) {
      CopyMem ((UINT8 *)&(KtiEparamLinkInfo.AllLanesEparamLinkInfo), (UINT8 *)KtiEparamTable.AllLanesEparamTablePtr, KtiEparamTable.SizeOfAllLanesEparamTable);
      DEBUG ((EFI_D_INFO,"[%a] CountOfAllLanesEparamLink: 0x%04X\n", __FUNCTION__, KtiEparamLinkInfo.CountOfAllLanesEparamLink));
    }

    if (KtiEparamLinkInfo.CountOfPerLaneEparamLink > 0) {
      CopyMem ((UINT8 *)&(KtiEparamLinkInfo.PerLaneEparamLinkInfo), (UINT8 *)KtiEparamTable.PerLaneEparamTablePtr, KtiEparamTable.SizeOfPerLaneEparamTable);
      DEBUG ((EFI_D_INFO,"[%a] CountOfPerLaneEparamLink: 0x%04X\n", __FUNCTION__, KtiEparamLinkInfo.CountOfPerLaneEparamLink));
    }

    BuildGuidDataHob (
                &gEfiKtiEparamInfoGuid,
                &KtiEparamLinkInfo,
                sizeof(KTI_EPARAM_LINK_INFO)
            );
  }

  return;
}

/**

  Platform Hook for resetting the platform.

  @param[in] ResetType - Warm == 0x02, Cold == 0x04

  @retval MRC_STATUS

**/

MRC_STATUS
EFIAPI
PlatformIssueReset (
  IN UINT8 ResetType
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // PlatformIssueReset

VOID
OemSecurityPolicy (
  IN PSYSHOST       Host,
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  INTEL_SETUP_DATA                 *SetupData,
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  IN EFI_BOOT_MODE  BootMode
  )
{

#ifdef LT_FLAG
  EFI_PEI_CPU_IO_PPI             *CpuIo;
  UINT8                          Data8;
  UINT8                          Data8_Save;
  EFI_PEI_SERVICES               **PeiServices;
  EFI_STATUS                     Status;
  PeiServices  = (EFI_PEI_SERVICES **) Host->var.common.oemVariable;

  CpuIo = (*PeiServices)->CpuIo;
  CpuIo->IoWrite8 (PeiServices, CpuIo, 0x70, CMOS_LTSX_OFFSET);
  Data8 = CpuIo->IoRead8 (PeiServices, CpuIo, 0x71);
  Data8_Save = Data8 ;
  Data8 &= (~ (BIT4 | BIT5));

  DEBUG((EFI_D_INFO, "(Setup)ProcessorLtsxEnable = %d\n", SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorLtsxEnable));
  if ((SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorLtsxEnable)) {
      Status = PcdSetBoolS (PcdProcessorLtsxEnable, TRUE);
      Data8 |= BIT4 | BIT5;
  } else {
    Status = PcdSetBoolS (PcdProcessorLtsxEnable, FALSE);
  }
  DEBUG((EFI_D_INFO, "CMOS_LTSX_OFFSET(%02x)= 0x%02x: for LT/TPM policy\n", CMOS_LTSX_OFFSET, Data8));
  if( (Data8_Save & (BIT4+BIT5)) != (Data8 & (BIT4+BIT5)) ) {
    CpuIo->IoWrite8 (PeiServices, CpuIo, 0x70, CMOS_LTSX_OFFSET); // Added because DEBUG now checks CMOS and alters port 0x70
    CpuIo->IoWrite8 (PeiServices, CpuIo, 0x71, Data8);    // sync up FITEGN cmos with setup option
    //
    // Do this powergood reset only if we're not in HSX PO
    //  where a bootscript is needed to start the CPU
    //
    if ((SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorLtsxEnable)) {
      DEBUG((EFI_D_INFO, "Sync up LT Policy for : do powergood reset!\n"));
      SetSysResetRequired(POST_RESET_POWERGOOD);
    }
    //(*PeiServices)->ResetSystem(PeiServices);          // do powergood reset
  }
#endif //LT_FLAG

}

/**

  Platform hook to Initialize Memory data

    @param Host = pointer to sysHost structure

**/
VOID
PlatformInitializeMemoryData (
  IN PSYSHOST  Host
  )
{

  EFI_STATUS        Status;
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  INTEL_SETUP_DATA        SetupData;
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  EFI_BOOT_MODE     BootMode;
  BIOS_BOOTING_MODE RequestedBiosBootingMode;
  SYS_SETUP         *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Determine boot mode
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  ZeroMem(&SetupData, sizeof(INTEL_SETUP_DATA) );
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  Status = GetEntireConfig (&SetupData);
  ASSERT_EFI_ERROR (Status);

  Status = GetBootingModeRequestedBySpsNm (&RequestedBiosBootingMode);
  if (!EFI_ERROR (Status) && (Setup != NULL)) {
    if (RequestedBiosBootingMode == PowerOptimizedMode) {
      Setup->mem.options &= ~MULTI_THREAD_MRC_EN;
    }
  }

  OemSecurityPolicy (Host, &SetupData, BootMode);


  return;

} // PlatformInitializeMemoryData

/**

  Platform hook pre memory init

  @param[in] Host  - Pointer to SysHost

  @retval EFI_SUCCESS
          EFI_NOT_FOUND - The PeiVariable PPI is not in the database or
                          System Configuration variable was not found

**/
EFI_STATUS
EFIAPI
PlatformPreMemoryInit (
  IN PSYSHOST Host
  )
{
  SYSTEM_CONFIGURATION                  TotalSystemConfiguration;
  SYSTEM_CONFIGURATION                  *TotalSystemConfigurationPtr = &TotalSystemConfiguration;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  EFI_STATUS                            Status;
  EFI_PEI_SERVICES                      **PeiServices;

  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  //
  // Locate Variable PPI
  //
  Status = (**PeiServices).LocatePpi (
                           PeiServices,
                           &gEfiPeiReadOnlyVariable2PpiGuid,
                           0,
                           NULL,
                           &PeiVariable
                           );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"Unable to locate variable PPI - %r\n", Status));
    return Status;
  } else {

    VariableSize = sizeof (SYSTEM_CONFIGURATION);

    Status = PeiVariable->GetVariable (
                          PeiVariable,
                        // APTIOV_SERVER_OVERRIDE_RC_START
                        L"IntelSetup",
                        // APTIOV_SERVER_OVERRIDE_RC_END
                          &gEfiSetupVariableGuid,
                          NULL,
                          &VariableSize,
                          TotalSystemConfigurationPtr
                          );

    //
    // BIOS Guard Initialization
    //
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"Unable to get system config - %r\n", Status));
      return Status;
    } else {
      BiosGuardInit (PeiServices, TotalSystemConfigurationPtr);
    }
  }
  //
  // Platform specific Cleanup and settings required before memory init
  //

  //
  // Platform specific handling Errors from previous boot
  //

  return EFI_SUCCESS;
}

/**

  Platform hook post memory init

  @param[in] Host  - Pointer to SysHost

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
PlatformPostMemoryInit (
  IN PSYSHOST Host
  )

{
  return EFI_SUCCESS;
}

/**

  Platform hook before checking and triggering the proper type of reset.
  Executed by System BSP only.

  @param Host = pointer to sysHost structure

  @retval None

**/
VOID
PlatformPreHandleResetRequests (
  IN PSYSHOST             Host
  )
{
  //
  // Perform Platform tasks before a reset might be triggered after control returns to the caller
  //

  if (GetSysResetRequired())
  {
    ClearSmiAndWake();
  }
  //APTIOV_SERVER_OVERRIDE_RC_START: Removed references to SecurityPkg
#if 0
  if ((GetSysBootMode () == S3Resume) || (GetSysResetRequired () != 0)) {
    EFI_STATUS                Status;
    EFI_PEI_SERVICES          **gPeiServices;
    VOID                      *TpmInitialize;
    UINT64                    IsFTPM = 0;

    gPeiServices = (EFI_PEI_SERVICES **) Host->var.common.oemVariable;

    //
    // Locate TPM Initialized Ppi to determine TPM is present and initialized properly.
    //
    Status = (**gPeiServices).LocatePpi(gPeiServices,
      &gPeiTpmInitializedPpiGuid,
      0,
      NULL,
      &TpmInitialize
    );
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "TpmInitialize: TPM is not present!!\n"));
      return;
    }
    if (CompareGuid(PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTpm12Guid)) {
      DEBUG((EFI_D_INFO, "Running TpmSaveStateRun....\n"));
      Status = Tpm12SaveState();
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "Fail to run TpmSaveStateRun....\n"));
      }
    }
    else if (CompareGuid(PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTpm20DtpmGuid)) {
      if ((GetSysBootMode () == S3Resume)) {
        DEBUG((EFI_D_INFO, "Running Tpm2SaveStateRun....\n"));
        Status = Tpm2Shutdown(TPM_SU_STATE);
        if (EFI_ERROR(Status))
        {
          DEBUG((EFI_D_ERROR, "Fail to run Tpm2CommSaveState....\n"));
        }
      }
      else {
       IsFTPM = (UINT64)(UINTN)*((UINTN*)(UINTN)(R_PTT_TXT_STS_FTIF));
        if (((RShiftU64(IsFTPM, 16)) & 0x07) != 0x07) {   //Only do below operation for DTPM
          DEBUG((EFI_D_INFO, "Running Tpm2Shutdown(TPM_SU_CLEAR)....\n"));
          Status = Tpm2Shutdown(TPM_SU_CLEAR);
          if (EFI_ERROR(Status))
          {
            DEBUG((EFI_D_ERROR, "Fail to run Tpm2Shutdown(TPM_SU_CLEAR)....\n"));
          }
        }
      }
    }
    else {
      DEBUG((EFI_D_ERROR, "No TPM12 and TPM2 DTPM instance is installed!\n"));
    }
  }
#endif
    //APTIOV_SERVER_OVERRIDE_RC_END: Removed references to SecurityPkg
  return;
}

/**

  Puts the Host structure variables for POST

  @param Host - pointer to sysHost structure on stack

@retval VOID

**/
VOID
PlatformPublishDataForPost (
  IN PSYSHOST  Host
  )
{
  return;
}

/**

  Platform hook for initializing Tcrit to 105 temp offset & the config register
  which is inside of initialization of memory throttling

  @param socket            - socket number
  @param smbDevice dev     - SMB Device
  @param byteOffset        - byte Offset
  @param data              - data

**/
VOID
PlatformHookMst (
  UINT8             socket,
  SMB_DEVICE_STRUCT dev,
  UINT8             byteOffset,
  UINT8             *data
  )
{
  return;
}

/**

  OEM hook for reacting to Board VS CPU conflict.

  If this function returns MRC_FUNCTION_NOT_IMPLEMENTED,
  the reference code will halt execution.

  @param[in] Host - pointer to sysHost

  @retval MRC_STATUS

**/

MRC_STATUS
EFIAPI
OemCheckForBoardVsCpuConflicts (
  IN PSYSHOST Host
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // OemCheckForBoardVsCpuConflicts

// APTIOV_SERVER_OVERRIDE_RC_START : Changes done to resolve TPM (not found in setup) issue during S3 resume when TXT is enabled.
#define TPM_TIS             0
#define TPM_PTPCRB          1
#define TPM_DEVICE_ERROR    0xff

#define R_PTT_LOCALITY_STATE  0x00
#define R_TPM_INTERFACE_ID    0x30
#define RESPONSE_HEADER_SIZE  12

#define TPM_DRIVER_WAIT     0x5
#define TPM_DELAY_AMOUNT    0xa
#define ACCESS_WAITCOUNTA   (750 * 1000 / 100)    // 750MS (750 * 10000 /1000)
#define ACCESS_WAITCOUNTB   (  3 * 1000000/100)   // 3 seconds delay

#define TIS_TIMEOUT_A       ACCESS_WAITCOUNTA
#define TIS_TIMEOUT_B       ACCESS_WAITCOUNTB

#ifndef PORT_TPM_IOMEMBASE
#define PORT_TPM_IOMEMBASE  0xFED40000
#endif

#pragma pack(1)
typedef struct _GENERIC_RESP_HDR_ {
    UINT16 Tag;
    UINT32 RespSize;
    UINT32 RespCode;
} GENERIC_RESP_HDR;

typedef struct {
  TPM2_COMMAND_HEADER  Header;
  TPM_SU               ShutdownType;
} TPM2_SHUTDOWN_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER Header;
} TPM2_SHUTDOWN_RESPONSE;

typedef struct _TPM_CRB_ACCESS_REG_ {
    UINT32 TpmlocState; //00-03
    UINT32 Reserved;    //04-07
    UINT32 TpmlocCtrl;  //08-0B
    UINT32 TpmlocSts;   //0C-0F
    UINT32 Reserved1[0x8];    //10-2F
    UINT32 TpmCrbIntfId[2];   //30-37
    UINT32 TpmCrbCtrlExt[2];  //38-3F
    UINT32 TpmCrbCtrlReq;     //40-43
    UINT32 TpmCrbCtrlSts;     //44-47
    UINT32 TpmCrbCtrlCncl;    //48-4B
    UINT32 TpmCrbCtrlStrt;    //4C-4F
    UINT32 TpmCrbCtrlInt[2];  //50-57
    UINT32 TpmCrbCtrlCmdSize; //58-5B
    UINT32 TpmCrbCtrlCmdLAddr; //5C-5F
    UINT32 TpmCrbCtrlCmdHAddr; //60-63
    UINT32 TpmCrbCtrlRespSize; //64-67
    UINT32 TpmCrbCtrlResdLAddr; //68-6B
    UINT32 TpmCrbCtrlResdHAddr; //6C-6F
    UINT32 Reserved2[0x4];     //70-7F
} TPM_CRB_ACCESS_REG;

typedef volatile TPM_CRB_ACCESS_REG *TPM_CRB_ACCESS_REG_PTR;
#pragma pack()

/**
    Check if TPM device is Tis or PTPCRB.

    @retval BOOLEAN Return true if TPMdevice is Tis.
**/

UINT8 AuxChkIsTpmTisOrPTPCRB(
    VOID
  )
{
    UINT8                 TpmBase = *(UINT8*)TPM_BASE_ADDRESS;
    TPM_1_2_REGISTERS_PTR TpmReg = (TPM_1_2_REGISTERS_PTR)TPM_BASE_ADDRESS;
    UINT32                TpmInterfaceID;
    UINT8                 TpmLocState;
    
    if (TpmBase == 0xFF || TpmBase == 0x00)
      return TPM_DEVICE_ERROR;
    
    TpmInterfaceID = *(UINT32*)(TPM_BASE_ADDRESS +R_TPM_INTERFACE_ID);
    TpmLocState = *(UINT8*)(TPM_BASE_ADDRESS +R_PTT_LOCALITY_STATE);
    
    if ((TpmInterfaceID & 0x0f) == 1 && (TpmLocState & 0x80) == 0x80)
      return TPM_PTPCRB;
    else {
      if ((TpmReg->Access & 0x80) == 0) // Check the TpmReg Valids
        return TPM_DEVICE_ERROR;
      return TPM_TIS;
    }
}

void
TpmFixedDelay(
    UINT32 dCount
  )
{
  UINTN  MicroSDelay = TPM_DELAY_AMOUNT;

  while (dCount) {
    MicroSecondDelay(MicroSDelay);  
    dCount--;
  }
}

static
UINT8
CheckAccessBit (
  IN      volatile UINT8   *Sts,
  IN      UINT8            Bit,
  IN      UINT32           Timeout    
  )
/*++
Routine Description:
  Function to check bits in TPM access register
Arguments:
  *Sts      - A Pointer to Status register
  Bit       - Bit position
  Timeout    - Timeout amount to wait till the specified bit
Returns:
  EFI_Status
--*/
{
  UINT32  AccessCount = Timeout;            
  
  if (AccessCount) {
    do {
      TpmFixedDelay((UINT32)TPM_DRIVER_WAIT);  
      if ((*Sts & Bit) && (*Sts & TPM_STS_VALID))
        return *Sts & Bit;
      AccessCount--;
    } while (AccessCount);
  } else {
    while (!(*Sts & TPM_STS_VALID)) {
      TpmFixedDelay((UINT32)TPM_DRIVER_WAIT);
    }
    return *Sts & Bit;
  }

  return 0;    
}

static
UINT16
ReadBurstCount (
  IN      TPM_1_2_REGISTERS_PTR     TpmReg
)
/*++
Routine Description:
  Gets the number of bytes (burstCount) that the TPM can return
  on reads or accept on writes without inserting LPC long wait 
  states on the LPC bus.
  burstCount is TPM_STS_x register bits 8..23
Arguments:
  TpmReg           - A Pointer to the TPM Register Space
Returns:
  0                - Any Errors
  burstCount       - TPM_STS_x.burstCount
--*/  
{
  UINT16   burstCount;
  UINT64   Deadline = TIS_TIMEOUT_A;

  burstCount = 0;
  do {
    //
    // burstCount is little-endian bit ordering
    //
    TpmFixedDelay((UINT32)TPM_DRIVER_WAIT);
    Deadline--;
    burstCount = TpmReg->BurstCount;
  } while (!burstCount && (Deadline > 0));
  
  return burstCount;
}

EFI_STATUS
TisRequestLocality (
  IN      TPM_1_2_REGISTERS_PTR     TpmReg
  )
/*++
Routine Description:
  Requests TPM locality 0
Arguments:
  TpmReg           - A Pointer to the TPM Register Space
Returns:
  EFI_Status
--*/  
{
  if ((TpmReg->Access & TPM_ACC_ACTIVE_LOCALITY) == TPM_ACC_ACTIVE_LOCALITY) {
    return EFI_SUCCESS;
  }
  
  TpmReg->Access = TPM_ACC_REQUEST_USE;
  TpmFixedDelay((UINT32)0x14);
  if (CheckAccessBit(&TpmReg->Access, TPM_ACC_ACTIVE_LOCALITY, (UINT32)TIS_TIMEOUT_B)) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_READY;
  }
}

#pragma optimize("",off)
EFI_STATUS
TisReleaseLocality (
  IN      TPM_1_2_REGISTERS_PTR     TpmReg
  )
/*++
Routine Description:
  Release TPM locality 0
Arguments:
  TpmReg           - A Pointer to the TPM Register Space
Returns:
  EFI_Status
--*/
{
  UINT32    AccessCount = ACCESS_WAITCOUNTA;
  
  if (!CheckAccessBit (&TpmReg->Access, TPM_ACC_ACTIVE_LOCALITY, 0)) {
    return EFI_DEVICE_ERROR;
  }

  TpmReg->Access = TPM_ACC_ACTIVE_LOCALITY;
  TpmFixedDelay((UINT32)0x14);
  if (CheckAccessBit(&TpmReg->Access, TPM_ACC_ACTIVE_LOCALITY, 0)) {
    do{/*give locality time to be released*/
      TpmFixedDelay((UINT32)TPM_DRIVER_WAIT); 
      AccessCount--;
    } while (((CheckAccessBit(&TpmReg->Access, TPM_ACC_ACTIVE_LOCALITY, 0)) && AccessCount));
    if(CheckAccessBit(&TpmReg->Access, TPM_ACC_ACTIVE_LOCALITY, 0))
      return EFI_DEVICE_ERROR;
    else
      return EFI_SUCCESS;
  } else {
    return EFI_SUCCESS;
  }
}
#pragma optimize("",on)

EFI_STATUS
TisSendCommand (
  IN      TPM_1_2_REGISTERS_PTR     TpmReg,
  IN      const VOID                *CmdStream,
  IN      UINTN                     Size,
  IN      BOOLEAN                   Last
  )
/*++
Routine Description:
  Function to send TPM command
Arguments:
  TpmReg       - A Pointer to the TPM Register Space
  *CmdStream   - A Pointer to the command stream to be sent to TPM Fifo
  Size         - Size in bytes of the command stream
  Last         - Boolean to signify the last byte?
Returns:
  EFI_Status
--*/
{
  UINT8                             *Ptr;
  UINT16                            burstCount;

  if (Size == 0)
    return (Last?EFI_INVALID_PARAMETER:EFI_SUCCESS);

  if ((TpmReg->Access & TPM_ACC_ACTIVE_LOCALITY) != TPM_ACC_ACTIVE_LOCALITY)
    return EFI_ABORTED;

  Ptr = (UINT8*)CmdStream;

  if (Last)
    Size--;

  while (Size > 0) {
    burstCount = ReadBurstCount(TpmReg);
    if (burstCount == 0) {
      // Cannot get the correct burstCount value
      return EFI_TIMEOUT;
    }

    for (; burstCount > 0 && Size > 0; burstCount--) {
      *(UINT8*)&TpmReg->DataFifo = *Ptr;
      Ptr++;
      Size--;
    }
  }

  if (Last) {
    if (!CheckAccessBit (&TpmReg->Sts, TPM_STS_EXPECT, 0))
      return EFI_ABORTED;

    *(UINT8*)&TpmReg->DataFifo = *Ptr;

    if (CheckAccessBit (&TpmReg->Sts, TPM_STS_EXPECT, 0))
      return EFI_ABORTED;

    TpmReg->Sts = TPM_STS_GO;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
TisReceiveResponse (
  IN      TPM_1_2_REGISTERS_PTR     TpmReg,
  OUT     VOID                      *Buffer,
  OUT     UINTN                     *Size
  )
/*++
Routine Description:
  Function to recieve TPM command results
Arguments:
  TpmReg       - A Pointer to the TPM Register Space
  *Buffer      - A Pointer to buffer for recieving result data
  Size         - buffer size
Returns:
  EFI_Status
--*/
{
  UINT8                             *Ptr, *BufEnd;
  UINT16                             burstCount;

  if((TpmReg->Access & TPM_ACC_ACTIVE_LOCALITY) != TPM_ACC_ACTIVE_LOCALITY)
      return EFI_ABORTED;

  Ptr = (UINT8*)Buffer;
  BufEnd = Ptr + *Size;
  while (Ptr < BufEnd && CheckAccessBit (&TpmReg->Sts, TPM_STS_DATA_AVAILABLE, 0)) {
    burstCount = ReadBurstCount(TpmReg);
    if (burstCount == 0)
      return EFI_TIMEOUT;

    while (burstCount && Ptr < BufEnd && CheckAccessBit (&TpmReg->Sts, TPM_STS_DATA_AVAILABLE, 0)) {
      *Ptr++ = *(UINT8*)&TpmReg->DataFifo;
      burstCount--;
    }
  }

  *Size -= BufEnd - Ptr;
  if (CheckAccessBit (&TpmReg->Sts, TPM_STS_DATA_AVAILABLE, 0))
    return EFI_BUFFER_TOO_SMALL;
  else
    return EFI_SUCCESS;
}

EFI_STATUS
TpmLibPassThrough (
  IN      TPM_1_2_REGISTERS_PTR     TpmReg,
  IN      UINTN                     NoInputBuffers,
  IN      TPM_TRANSMIT_BUFFER       *InputBuffers,
  IN      UINTN                     NoOutputBuffers,
  IN OUT  TPM_TRANSMIT_BUFFER       *OutputBuffers
  )
/*++
Routine Description:
  Higher level function to send a recieve commands to the TPM
Arguments:
  TpmReg           - A Pointer to the TPM Register Space
  NoInputBuffers   - Number count of Input buffers
  *InputBuffers    - Pointer to InputBuffers[0]
  NoOutputBuffers  - Number count of Output buffers
  *OutputBuffers   - Pointer to OutputBuffers[0]
Returns:
  EFI_Status
--*/
{
  EFI_STATUS                        Status;
  UINTN                             i;

  if (NoInputBuffers == 0 || InputBuffers->Size < sizeof (TPM_1_2_CMD_HEADER))
    return EFI_INVALID_PARAMETER;

  do {
    NoInputBuffers--;
  } while (InputBuffers[NoInputBuffers].Size == 0 && NoInputBuffers > 0);

  if (InputBuffers[NoInputBuffers].Size == 0)
    return EFI_INVALID_PARAMETER;

  // Check Tis if prepare send command
  if ((TpmReg->Access & TPM_ACC_ACTIVE_LOCALITY) != TPM_ACC_ACTIVE_LOCALITY) {
    Status = EFI_NOT_STARTED;
  } else {
    do {
      TpmReg->Sts = TPM_STS_READY;
    } while (!(TpmReg->Sts & TPM_STS_READY));
    Status = EFI_SUCCESS;
  }

  for (i = 0; !EFI_ERROR (Status) && i < NoInputBuffers; i++) {
    Status = TisSendCommand (
      TpmReg,
      InputBuffers[i].Buffer,
      InputBuffers[i].Size,
      FALSE
      );
  }

  if (!EFI_ERROR (Status)) {
    Status = TisSendCommand (
      TpmReg,
      InputBuffers[i].Buffer,
      InputBuffers[i].Size,
      TRUE
      );
  }

  if (!EFI_ERROR (Status)) {
    if((TpmReg->Access & TPM_ACC_ACTIVE_LOCALITY) != TPM_ACC_ACTIVE_LOCALITY)
      goto Exit; //EFI_NOT_STARTED

    while (!CheckAccessBit(&TpmReg->Sts, TPM_STS_DATA_AVAILABLE, 0));
  }

  Status = EFI_BUFFER_TOO_SMALL;
  for (i = 0; Status == EFI_BUFFER_TOO_SMALL && i < NoOutputBuffers; i++) {
    Status = TisReceiveResponse (
      TpmReg,
      OutputBuffers[i].Buffer,
      &OutputBuffers[i].Size
      );
  }

Exit:
  // Check Tis if complete send command
  if ((TpmReg->Access & TPM_ACC_ACTIVE_LOCALITY) != TPM_ACC_ACTIVE_LOCALITY) {
    Status = EFI_NOT_STARTED;
  } else {
    do {
      TpmReg->Sts = TPM_STS_READY;
    } while (!(TpmReg->Sts & TPM_STS_READY));
    Status = EFI_SUCCESS;
  }
  return Status;
}

EFI_STATUS
EFIAPI
TisTcgPassThroughToTpm (
  IN UINT32           TpmInputParamterBlockSize,
  IN UINT8            *TpmInputParamterBlock,
  IN UINT32           TpmOutputParameterBlockSize,
  IN UINT8            *TpmOutputParameterBlock
  )
{
    TPM_TRANSMIT_BUFFER InBuffer[1], OutBuffer[1];
    EFI_STATUS Status;
 
    InBuffer[0].Buffer  = TpmInputParamterBlock;
    InBuffer[0].Size    = TpmInputParamterBlockSize;
    OutBuffer[0].Buffer = TpmOutputParameterBlock;
    OutBuffer[0].Size   = TpmOutputParameterBlockSize;

    // Init
    TisRequestLocality((TPM_1_2_REGISTERS_PTR)(UINTN)TPM_BASE_ADDRESS);

    Status = TpmLibPassThrough(
                        (TPM_1_2_REGISTERS_PTR)(UINTN)TPM_BASE_ADDRESS,
                        sizeof (InBuffer) / sizeof (*InBuffer),
                        (TPM_TRANSMIT_BUFFER*)(UINTN)InBuffer,
                        sizeof (OutBuffer) / sizeof (*OutBuffer),
                        (TPM_TRANSMIT_BUFFER*)(UINTN)OutBuffer
                         );

    // Close
    TisReleaseLocality( (TPM_1_2_REGISTERS_PTR)(UINTN)TPM_BASE_ADDRESS );

    return Status;
}

UINT8*
EFIAPI
CrbMmioRWBuffer8 (
  IN  UINTN       StartAddress,
  IN  UINTN       Length,
  IN OUT UINT8   *Buffer,
  IN  BOOLEAN     Write
  )
{
    VOID   *ReturnBuffer;

    ASSERT ((Length - 1) <=  (MAX_ADDRESS - StartAddress));
    ASSERT ((Length - 1) <=  (MAX_ADDRESS - (UINTN) Buffer));

    ReturnBuffer = (UINT8 *)Buffer;

    while (Length-- != 0) {
      if (Write)
        MmioWrite8(StartAddress++, *(Buffer++));
      else
        *(Buffer++) = MmioRead8(StartAddress++);
    }

    return ReturnBuffer;
}

BOOLEAN
dTPMCrbLocality0Granted (
  TPM_CRB_ACCESS_REG_PTR dCrbAccessRegPtr
  )
{
    UINTN       ActiveLocality = 0;

    ActiveLocality = dCrbAccessRegPtr->TpmlocState >> 2;

    if (((ActiveLocality & 0x7) == 0)           &&      // Locality is 0.
        (dCrbAccessRegPtr->TpmlocState & 0x02)  &&      // Locality is assign.
        (dCrbAccessRegPtr->TpmlocState & 0x80)  &&      // TPM is Valid.
        (dCrbAccessRegPtr->TpmlocSts & 0x01)            // Locality has been granted.
      )
      return TRUE;
    return FALSE;
}

UINT8
isTPMIdle (
  TPM_CRB_ACCESS_REG_PTR dCrbAccessRegPtr
  )
{
    if ((dCrbAccessRegPtr->TpmCrbCtrlSts & 1) == 1) {
      return 0xFF; //FATAL TPM ERROR
    }
    if ((dCrbAccessRegPtr->TpmCrbCtrlSts & 2) == 2) {
      return 1; //idle state
    }
    return 0;
}

#define dTPMCrbSetLocality       0
#define dTPMCrbWaitStrtClr       1
#define dTPMCrbSetReqReadyState  2
#define dTPMCrbSetReqIdleState   3

EFI_STATUS
dTPMCrbFunc (
  TPM_CRB_ACCESS_REG_PTR dCrbAccessRegPtr,
  UINT8                  Func
  )
{
  UINTN       DelayTime;
  UINTN       Timeout;
  UINTN       Count;
  
  Count = 0;
  do {
    switch(Func) {
      case dTPMCrbSetLocality:
        DelayTime = 50;
        Timeout = 50;
        if (dTPMCrbLocality0Granted(dCrbAccessRegPtr) == FALSE) {
          dCrbAccessRegPtr->TpmlocCtrl = 0x1;
          MicroSecondDelay(DelayTime); //delay 50us
          break;
        }
        else
          goto Func_Exit;
      case dTPMCrbWaitStrtClr:
        DelayTime = 50;
        Timeout = 100000;
        if (dCrbAccessRegPtr->TpmCrbCtrlStrt != 0x0) {
          MicroSecondDelay(DelayTime); //delay 50us
          break;
        }
        else
          goto Func_Exit;
      case dTPMCrbSetReqReadyState:
        DelayTime = 50;
        Timeout = 10000;
        dCrbAccessRegPtr->TpmCrbCtrlReq = 0x01;
        MicroSecondDelay(DelayTime); //delay 50us
        if ((isTPMIdle(dCrbAccessRegPtr)) == 1)
          break;
        else
          goto Func_Exit;
      case dTPMCrbSetReqIdleState:
        DelayTime = 100;
        Timeout = 50000;
        dCrbAccessRegPtr->TpmCrbCtrlReq = 0x2;
        MicroSecondDelay(DelayTime); //delay 100us
        if ((isTPMIdle(dCrbAccessRegPtr)) == 0)
          break;
        else
          goto Func_Exit;
      default:
        goto Func_Exit;
    }

    Count++;
  } while (Count != Timeout);
  return EFI_DEVICE_ERROR;
  
Func_Exit:
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
CrbSend(
  IN     UINT8      *InputBuffer,
  IN     UINT32     DataLength
)
{
    TPM_CRB_ACCESS_REG_PTR  dCrbAccessRegPtr = (TPM_CRB_ACCESS_REG_PTR)((UINTN)PORT_TPM_IOMEMBASE);
    UINTN                   CmdBase;
    EFI_STATUS              Status;

    Status = dTPMCrbFunc(dCrbAccessRegPtr, dTPMCrbSetLocality);
    if (EFI_ERROR(Status))
      return Status;

    Status = dTPMCrbFunc(dCrbAccessRegPtr, dTPMCrbSetReqReadyState);
    if (EFI_ERROR(Status))
      return Status;

    if (dCrbAccessRegPtr->TpmCrbCtrlCmdLAddr == 0) {
      dCrbAccessRegPtr->TpmCrbCtrlCmdLAddr = 0xFED40080;
      dCrbAccessRegPtr->TpmCrbCtrlCmdSize = 0x500;
      dCrbAccessRegPtr->TpmCrbCtrlResdLAddr = 0xFED40080;
      dCrbAccessRegPtr->TpmCrbCtrlRespSize = 0x500;
    }

    if (dCrbAccessRegPtr->TpmCrbCtrlCmdLAddr !=0) {
      CmdBase = (UINTN)dCrbAccessRegPtr->TpmCrbCtrlCmdLAddr;
      CrbMmioRWBuffer8(CmdBase, DataLength, InputBuffer, TRUE);

      Status = dTPMCrbFunc(dCrbAccessRegPtr, dTPMCrbWaitStrtClr);
      if (EFI_ERROR(Status))
        return Status;

      dCrbAccessRegPtr->TpmCrbCtrlStrt = 1;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
CrbReceive(
  OUT     UINT8     *OutBuffer,
  OUT     UINT32    *RespSize
)
{
    TPM_CRB_ACCESS_REG_PTR dCrbAccessRegPtr = (TPM_CRB_ACCESS_REG_PTR)((UINTN)PORT_TPM_IOMEMBASE);
    UINTN                  RspBase;
    UINT16                 Data16;
    UINT32                 Data32;
    EFI_STATUS Status;

    Status = dTPMCrbFunc(dCrbAccessRegPtr, dTPMCrbWaitStrtClr);
    if (EFI_ERROR(Status))
      return Status;

    if (dCrbAccessRegPtr->TpmCrbCtrlResdLAddr != 0) {
      RspBase = (UINTN) dCrbAccessRegPtr->TpmCrbCtrlResdLAddr;
      CrbMmioRWBuffer8(RspBase, RESPONSE_HEADER_SIZE, OutBuffer, FALSE);
      CopyMem (&Data16, OutBuffer, sizeof (UINT16));

      if (SwapBytes16(Data16) == TPM_ST_RSP_COMMAND)
        return EFI_DEVICE_ERROR;

      CopyMem(&Data32, (OutBuffer + 2), sizeof(UINT32));
      *RespSize = SwapBytes32(Data32);

      if (*RespSize > dCrbAccessRegPtr->TpmCrbCtrlRespSize)
        return EFI_BUFFER_TOO_SMALL;

      if (*RespSize < sizeof(GENERIC_RESP_HDR))
        return EFI_DEVICE_ERROR;

      CrbMmioRWBuffer8(RspBase, *RespSize,  OutBuffer, FALSE);
      
      Status = dTPMCrbFunc(dCrbAccessRegPtr, dTPMCrbSetReqIdleState);
      if (EFI_ERROR(Status))
        return Status;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
CrbSubmitCmd(
  IN      UINT32     InputBufferSize,
  IN      UINT8     *InputBuffer,
  OUT     UINT32    *OutputBufferSize,
  OUT     UINT8     *OutputBuffer
  )
{
    EFI_STATUS Status;

    Status = CrbSend(InputBuffer, InputBufferSize);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    ///
    /// Receive the response data from TPM
    ///
    Status = CrbReceive(OutputBuffer, OutputBufferSize);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    return Status;
}

/**
    Sent TPM submit command.

    @retval EFI_STATUS Return EFI status.
**/

EFI_STATUS
EFIAPI
TpmOemSubmitCommand (
  IN UINT32            InputParameterBlockSize,
  IN UINT8             *InputParameterBlock,
  IN OUT UINT32        *OutputParameterBlockSize,
  IN UINT8             *OutputParameterBlock
  )
{
    switch (AuxChkIsTpmTisOrPTPCRB()) {
    case TPM_TIS:
        return TisTcgPassThroughToTpm(
                    InputParameterBlockSize,
                    InputParameterBlock,
                    *OutputParameterBlockSize,
                    OutputParameterBlock
                );
        break;
    case TPM_PTPCRB:
        return CrbSubmitCmd(
                    InputParameterBlockSize,
                    InputParameterBlock,
                    OutputParameterBlockSize,
                    OutputParameterBlock
                );
        break;
    default:
        return EFI_DEVICE_ERROR;
        break;
    }
}

/**
  Send Shutdown command to TPM2.

  @param[in] ShutdownType           TPM_SU_CLEAR or TPM_SU_STATE.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
Tpm2Shutdown (
  IN      TPM_SU             ShutdownType
  )
{
  EFI_STATUS                        Status;
  TPM2_SHUTDOWN_COMMAND             Cmd;
  TPM2_SHUTDOWN_RESPONSE            Res;
  UINT32                            ResultBufSize;

  Cmd.Header.tag         = SwapBytes16(TPM_ST_NO_SESSIONS);
  Cmd.Header.paramSize   = SwapBytes32(sizeof(Cmd));
  Cmd.Header.commandCode = SwapBytes32(TPM_CC_Shutdown);
  Cmd.ShutdownType       = SwapBytes16(ShutdownType);

  ResultBufSize = sizeof(Res);
  Status = TpmOemSubmitCommand (sizeof(Cmd), (UINT8 *)&Cmd, &ResultBufSize, (UINT8 *)&Res);

  return Status;
}

EFI_STATUS Tpm12SaveState()
{
    EFI_STATUS                  Status;
    TPM_RQU_COMMAND_HDR         Cmd;
    TPM_RSP_COMMAND_HDR         Ret;
 
    Cmd.tag         = SwapBytes16( TPM_TAG_RQU_COMMAND );
    Cmd.paramSize   = SwapBytes32( sizeof (TPM_RQU_COMMAND_HDR));
    Cmd.ordinal     = SwapBytes32( TPM_ORD_SaveState );
 
    Status = TisTcgPassThroughToTpm(
                                sizeof (Cmd),
                                (UINT8*)&Cmd,
                                sizeof (Ret),
                                (UINT8*)&Ret );
 
    if ( EFI_ERROR(Status) || (Ret.returnCode != 0)){
   
        DEBUG ((EFI_D_ERROR, "(TPM Error) Status: %r; RetCode: %x.\n", \
                                    Status, \
                                    SwapBytes32(Ret.returnCode))); 
        return EFI_DEVICE_ERROR;
    }
    return Status;
}
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done to resolve TPM (not found in setup) issue during S3 resume when TXT is enabled.

/**

  Platform hook meant to be used by customers where they can use it to write their own jumper detection code
  and have it return FALSE when no jumper is present, thereby, blocking calling of the BSSA loader; else have it
  return TRUE if the concerned jumper is physically present. This check ensures that someone will have to be
  physically present by the platform to enable the BSSA Loader.

  @param[in] None

  @retval TRUE - This function returns TRUE by default as OEMs are meant to implement their own jumper detection
                 code(DetectPhysicalPresence).

**/
BOOLEAN
PlatformDetectPhysicalPresenceSSA (
  VOID
  )
{
  return TRUE;
}


/**
  Publish any Platform specfic HOBs.

  @param  None

  @retval EFI_SUCCESS

**/

EFI_STATUS
EFIAPI
PlatformPublishHobs (
  VOID
  )
{

  //
  // Transfer the EparamTable to Hob for LR board to do CPU OL in SMM if the CPU OL is enabled.
  // KTI Eparam data is retrieved via the Platform UBA interface,
  // which means that we cannot create this hob from any of the Silicon packages,
  // so create the HOB here.
  //

  if (KtiCpuSktHotPlugEnabled ()) {
    SaveKtiEParamDataToHob ();
  }

  return EFI_SUCCESS;

} // PlatformPublishHobs
