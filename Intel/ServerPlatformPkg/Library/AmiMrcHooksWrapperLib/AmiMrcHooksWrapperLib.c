//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiMrcHooksWrapperLib.c
    MRC Hooks wrapper implementation
**/

//---------------------------------------------------------------------------
 #include <AmiMrcHooksWrapperLib.h>
// APTIOV_SERVER_OVERRIDE_START : Bus OOR Implementation
#include <Guid/AmiBusResourceMap.h>
// APTIOV_SERVER_OVERRIDE_END : Bus OOR Implementation
//---------------------------------------------------------------------------

 PLATFORM_LOG_WARNING_ELINK* PlatformLogWarningElink[] = { OVERRIDE_PLATFORM_LOG_WARNING_HOOK NULL };
 MODIFY_MRC_INPUT_PARAMETERS_ELINK* ModifyMrcInputParametersElink[] = {OVERRIDE_MRC_INPUT_PARAMETERS_HOOK NULL};
 UPDATE_PLATFORM_DATA_ELINK* UpdatePlatformDataElink[] = {OVERRIDE_PLATFORM_DATA_HOOK NULL};
 MODIFY_MRC_SPEED_ELINK* ModifyMrcSpeedElink[] = {OVERRIDE_MRC_SPEED_FREQUENCY_HOOK NULL};
 MODIFY_KTI_PARAMETER_ELINK* ModifyKtiParameterElink[] = {OVERRIDE_KTI_PARAMETER_HOOK NULL};
 PLATFORM_LOG_FATAL_ERROR_ELINK* PlatformFatalErrorElink[] = {OVERRIDE_PLATFORM_LOG_FATAL_ERROR_HOOK NULL};
 KTI_LOG_WARNING_ELINK* KtiLogWarningElink[] = {OVERRIDE_KTI_LOG_WARNING_HOOK NULL};
 KTI_CHECKPOINT_ELINK*  KtiCheckPointElink[] = {OVERRIDE_KTI_CHECKPOINT_HOOK NULL};
 GET_BOARD_TYPE_ELINK*  OemGetBoardTypeBitmaskElink[] = {OVERRIDE_GET_BOARD_TYPE_HOOK NULL};
 KTI_GET_EPARAMS_ELINK* OemKtiGetEParamsElink[] = {OVERRIDE_KTI_GET_EPARAMS_HOOK NULL};
 PUBLISH_HOB_DATA_ELINK* OemPublishHobDataElink[] = {OVERRIDE_PUBLISH_HOB_DATA_HOOK NULL};
 PLATFORM_OUTPUT_ERROR_ELINK* PlatformOutputErrorElink[] = {OVERRIDE_PLATFORM_OUTPUT_ERR_HOOK NULL};
 PLATFORM_OUTPUT_WARNING_ELINK* PlatformOutputWarningElink[] = {OVERRIDE_PLATFORM_OUTPUT_WAR_HOOK NULL};
 GET_RESOURCEMAP_UPDATE_ELINK* OemGetResourceMapUpdateElink[] = {OVERRIDE_GET_RESOURCEMAP_UPDATE_HOOK NULL};
 TURN_AROUND_TIMINGS_ELINK* TurnAroundTimingsElink[] = {OVERRIDE_TURN_AROUND_TIMINGS_HOOK NULL};
 
 /**

   AmiPlatformFatalErrorWrapper - OEM hook wrapper to provide fatal error output to the
   user interface and to override the halt on error policy.

   @param host - pointer to sysHost structure
   @param majorCode - major error/warning code
   @param minorCode - minor error/warning code
   @param haltOnError - pointer to variable that selects policy

 **/
 VOID
 AmiPlatformFatalErrorWrapper (
   IN UINT8    majorCode,
   IN UINT8    minorCode,
   IN UINT8    *haltOnError
   ) {
   
    UINT8 ElinkCount;
	
	
    for( ElinkCount = 0; PlatformFatalErrorElink[ElinkCount]; ElinkCount++ ) {
      PlatformFatalErrorElink[ElinkCount](majorCode,minorCode,haltOnError);
    }
 }

/**

  AmiPlatformLogWarningWrapper - OEM hook wrapper to provide common warning output to the
  user interface

  @param host      - pointer to sysHost structure
  @param majorCode - major error/warning code
  @param minorCode - minor error/warning code
  @param logData   - data log

  @retval N/A
**/
VOID
AmiPlatformLogWarningWrapper (
  IN UINT8    majorCode,
  IN UINT8    minorCode,
  IN UINT32   logData
  )
{

    UINT8 ElinkCount;
	
	
    for( ElinkCount = 0; PlatformLogWarningElink[ElinkCount]; ElinkCount++ ) {
         PlatformLogWarningElink[ElinkCount](majorCode,minorCode,logData);
    }
}

/**

  AmiKtiLogWarningWrapper - Wrapper function that provides an interface to handle KTI warnings.

  @param host     - To be filled with the warning log
  @param SocId    - Bit mask, where bit x corresponds to CPU (CPU:0...3). Set to 0xFF if the Warning is system wide.
  @param SocType  - 0: CPU socket. Set to 0xFF if the Warning is system wide.
  @param KtiLink  - Bit mask, where bit x corresponds to CPU KTI port 0/1. Set to 0xFF if the Warning is system wide, 0 if Link Not Applicable.
  @param WarnCode - Warning Code

  @retval None.

**/
VOID
AmiKtiLogWarningWrapper (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      KtiLink,
  UINT8                      WarnCode
  ) {
  
    UINT8 ElinkCount;
	
		
    for( ElinkCount = 0; KtiLogWarningElink[ElinkCount]; ElinkCount++ ) {
         KtiLogWarningElink[ElinkCount](SocId,SocType,KtiLink,WarnCode);
    }
}


/**

  AmiPlatformInitializeDataWrapper - OEM hook wrapper to initialize Input structure data if required.

  @param host  - Pointer to sysHost
  @param socket

  @retval N/A

**/
VOID
AmiPlatformInitializeDataWrapper (
  IN OUT PSYSHOST host,
  IN     UINT8 socket
  )
{

    UINT8 ElinkCount;
	
	
    for( ElinkCount = 0; ModifyMrcInputParametersElink[ElinkCount]; ElinkCount++ ) {
        ModifyMrcInputParametersElink[ElinkCount](host, socket);
    }
}

VOID
 AmiOemInitializePlatformData (
	struct sysHost *host
    )
{
    UINT8 ElinkCount;
	
    for( ElinkCount = 0; UpdatePlatformDataElink[ElinkCount]; ElinkCount++ ) {
    	UpdatePlatformDataElink[ElinkCount](host);
    }
}

/**

  AmiOemLookupFreqTableWrapper - OEM hook wrapper for overriding the memory POR frequency table.

  @param host            - Pointer to PSYSHOST
  @param freqTablePtr    - MRC frequency table
  @param freqTableLength - Frequency table length

  @retval N/A

**/
VOID
AmiOemLookupFreqTableWrapper (
  IN     PSYSHOST host,
  IN OUT struct DimmPOREntryStruct **freqTablePtr,
  IN OUT UINT16 *freqTableLength
  )
{

    UINT8 ElinkCount;
	
		
    for( ElinkCount = 0; ModifyMrcSpeedElink[ElinkCount]; ElinkCount++ ) {
        ModifyMrcSpeedElink[ElinkCount](host, freqTablePtr,freqTableLength);
    }
}

/**

  AmiOemKtiChangeParameterWrapper - OEM hook wrapper for changing KTI related parameters.

  @param host   - Pointer to sysHost

  @retval N/A

**/
VOID
AmiOemKtiChangeParameterWrapper (
  IN KTI_SOCKET_DATA *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  ) {
  
    UINT8 ElinkCount;
	
		
    for( ElinkCount = 0; ModifyKtiParameterElink[ElinkCount]; ElinkCount++ ) {
        ModifyKtiParameterElink[ElinkCount](SocketData, KtiInternalGlobal);
    }
}
/**

  AmiOemPublishHobDataWrapper - OEM hook wrapper to put the host structure into HOB variable

  @param host - pointer to sysHost structure on stack

  @retval VOID

**/
VOID
AmiOemPublishHobDataWrapper(
   struct sysHost             *host
   ) {
   
    UINT8 ElinkCount;
		
		
    for( ElinkCount = 0; OemPublishHobDataElink[ElinkCount]; ElinkCount++ ) {
        OemPublishHobDataElink[ElinkCount](host);
    }
}

/**

  OemKtiGetEParamsOverride - OEM hook wrapper to override the EParams

  @param host - pointer to sysHost structure on stack
  @param Ptr - pointer to ALL_LANES_EPARAM_LINK_INFO structure
  @param SizeOfTable - Size of EParam Table
  @retval VOID

**/
VOID
OemKtiGetEParamsOverride(
   struct sysHost             *host,
   ALL_LANES_EPARAM_LINK_INFO **Ptr,
   UINT32                     *SizeOfTable
   ) {
   
    UINT8 ElinkCount;
	
	
    for( ElinkCount = 0; OemKtiGetEParamsElink[ElinkCount]; ElinkCount++ ) {
        OemKtiGetEParamsElink[ElinkCount](host, Ptr, SizeOfTable);
    }
}

/**

  OemGetBoardTypeBitmaskOverride - OEM hook wrapper to override the BoardTypeBitmask

  @param host - pointer to sysHost structure on stack
  @param BoardTypeBitmask
  @retval VOID

**/
VOID
OemGetBoardTypeBitmaskOverride(
   struct sysHost             *host,
   UINT32                     *BoardTypeBitmask
   ) {
   
    UINT8 ElinkCount;
	  
	  
    for( ElinkCount = 0; OemGetBoardTypeBitmaskElink[ElinkCount]; ElinkCount++ ) {
        OemGetBoardTypeBitmaskElink[ElinkCount](host, BoardTypeBitmask);
    }
}


/**

  AmiKtiCheckPointWrapper - OEM hook wrapper for KtiCheckPoint

  @param SocId - Socket Number
  @param SocType - Socket Type
  @param KtiLink - Kti Link Number
  @param MajorCode - Major Code
  @param MinorCode - Minor Code
  @param host - pointer to sysHost structure
  @retval VOID

**/
VOID
AmiKtiCheckPointWrapper (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      KtiLink,
  UINT8                      MajorCode,
  UINT8                      MinorCode
  )
{

     UINT8 ElinkCount;
	 
	 
     for( ElinkCount = 0; KtiCheckPointElink[ElinkCount]; ElinkCount++ ) {
        KtiCheckPointElink[ElinkCount](SocId, SocType, KtiLink,MajorCode,MinorCode);
     }
}

/**

  AmiPlatformOutputErrorWrapper - OEM hook wrapper for PlatformOutputError


  @retval VOID

**/
VOID
AmiPlatformOutputErrorWrapper (
  PSYSHOST host,
  UINT8    ErrorCode,
  UINT8    minorErrorCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  ) {
  
    UINT8 ElinkCount;
	
	
    for( ElinkCount = 0; PlatformOutputErrorElink[ElinkCount]; ElinkCount++ ) {
    	PlatformOutputErrorElink[ElinkCount](host, ErrorCode, minorErrorCode,socket,ch,dimm,rank);
    }
}

/**

  AmiPlatformOutputWarningWrapper - OEM hook wrapper for PlatformOutputWarning


  @retval VOID

**/
VOID
AmiPlatformOutputWarningWrapper (
  PSYSHOST host,
  UINT8    warningCode,
  UINT8    minorWarningCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  ) {
  
    UINT8 ElinkCount;
	
	
    for( ElinkCount = 0; PlatformOutputWarningElink[ElinkCount]; ElinkCount++ ) {
	    PlatformOutputWarningElink[ElinkCount](host,warningCode,minorWarningCode,socket,ch,dimm,rank);
    }
}

/**

  OemGetResourceMapUpdateOverride - OEM hook wrapper to override the CPU Resource

  @param Status - pointer to gEfiPeiReadOnlyVariable2PpiGuid status
  @param PeiVariable - pointer to PeiVariable
  @param CpuRes - pointer to CPU_RESOURCE structure
  @param CpuRes - pointer to CPU_RESOURCE structure
  @retval VOID

**/
VOID
OemGetResourceMapUpdateOverride(
   EFI_STATUS                    Status,
   IN struct sysHost             *host,
   EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable,
   IN OUT KTI_CPU_RESOURCE       *CpuRes,
   IN KTI_OEMCALLOUT             SystemParams
   ) {
   
    UINT8 ElinkCount;
	
    for( ElinkCount = 0; OemGetResourceMapUpdateElink[ElinkCount]; ElinkCount++ ) {
        OemGetResourceMapUpdateElink[ElinkCount](Status, host, PeiVariable, CpuRes, SystemParams);
    }
    
    // APTIOV_SERVER_OVERRIDE_START : Bus OOR Implementation
    {
        AMI_BUS_RESOURCE_MAP    AmiBusResourceMap;
        BUS_SOCKET_RESOURCE     *BusSocketRes;
        BUS_STACK_RESOURCE      *BusStackRes;
        UINT8                   Socket, Stack;
        UINTN                   VariableSize = sizeof(AMI_BUS_RESOURCE_MAP);

        Status = PeiVariable->GetVariable (
                              PeiVariable,
                              AMI_BUS_RESOURCE_MAP_NAME,
                              &gAmiBusResourceMapGuid,
                              NULL,
                              &VariableSize,
                              &AmiBusResourceMap
                              );
        if (EFI_ERROR(Status)) {
            return;
        }

        for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
            BusSocketRes = &(AmiBusResourceMap.BusResource.SocketRes[Socket]);
            if (!BusSocketRes->Valid)
              continue;
            CpuRes[Socket].BusBase = (UINT8)BusSocketRes->NewBusBase;
            CpuRes[Socket].BusLimit = (UINT8)BusSocketRes->NewBusBase+(UINT8)BusSocketRes->NewBusLength-1;

            for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
                BusStackRes = &(BusSocketRes->StackRes[Stack]);
                if (!BusStackRes->Valid)
                  continue;
                CpuRes[Socket].StackRes[Stack].BusBase = (UINT8)BusStackRes->NewBusBase;
                CpuRes[Socket].StackRes[Stack].BusLimit = (UINT8)BusStackRes->NewBusBase+(UINT8)BusStackRes->NewBusLength-1;
            }
        }
    }
    // APTIOV_SERVER_OVERRIDE_END : Bus OOR Implementation
}

/**

  AmiTurnAroundTimingsWrapper - OEM hook wrapper to override TurnAroundTimings

  @param host   - Pointer to sysHost
  @param socket - Socket number
  
  @retval VOID

**/

VOID
AmiTurnAroundTimingsWrapper(
    PSYSHOST host,
    UINT8    socket		
) {
	UINT8 ElinkCount;
		
	for( ElinkCount = 0; TurnAroundTimingsElink[ElinkCount]; ElinkCount++ ) {
		TurnAroundTimingsElink[ElinkCount]( host, socket);
	}
}
