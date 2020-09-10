//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
// @file
//  This file retrieves the system information and prints it to the screen
//
#include <Token.h>
#include <Timestamp.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/HobLib.h>

#include <Library/CpuPpmLib.h>
#include <Library/CpuConfigLib.h>
#include <Ppi/AmiPeiTextOut.h>
#include <Guid/MemoryMapData.h>
#include <Register/Cpuid.h>
#include <Cpu/CpuCoreRegs.h>
#include <AmiTextOutHob.h>
#include <IioUniversalData.h>
#include <Library/MemMapDataLib.h>
#include <Library/CpuEarlyDataLib.h>
#include <Library/CpuAndRevisionLib.h>

#define CORE_COUNT_OFFSET                   16

#define CALCULATED_2133HZ                   2132
#define ACTUAL_2133HZ                       2133
#define CALCULATED_2933HZ                   2934
#define ACTUAL_2933HZ                       2933

#define ___INTERNAL_CONVERT_TO_STRING___(a) #a
#define CONVERT_TO_STRING(a) ___INTERNAL_CONVERT_TO_STRING___(a)

typedef struct {
  UINT16        Value;
  CHAR8         *String;
} VALUE_TO_STRING_MAPPING;

/**
    Outputs system information to Pei Early Video

    @param[in] PeiServices        Describes the list of possible PEI Services.
    @param[in] NotifyDescriptor   
    @param[in] Ppi   

    @retval  EFI_STATUS
**/
EFI_STATUS
OutputCpuDimmInformation (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
  );

/**
    Outputs CPU information to early video.

    @param[in] PeiServices   - Pointer to Pei service table
    @param[in] EarlyVideoPpi - Pointer to PEI text out PPI

    @retval  VOID
**/
VOID
InitializeCpuData (
  IN  CONST EFI_PEI_SERVICES        **PeiServices,
  IN  AMI_PEI_TEXT_OUT              *EarlyVideoPpi
);

/**
    Gets the DIMM's data from the system memory HOB

    @param[in] PeiServices   - Pointer to Pei service table
    @param[in] EarlyVideoPpi - Pointer to PEI text out PPI

    @retval  VOID
**/
VOID
InitializeDimmData (
  IN  CONST EFI_PEI_SERVICES        **PeiServices,
  IN  AMI_PEI_TEXT_OUT              *EarlyVideoPpi
);
