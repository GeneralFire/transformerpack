/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020  Intel Corporation. <BR>

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

#ifndef _ME_UMA_PPI_H_
#define _ME_UMA_PPI_H_

extern EFI_GUID gMeUmaPpiGuid;
extern EFI_GUID gDramInitDoneSentPpiGuid;

///
/// PCI Register Definition(s)
///
#define R_MESEG_BASE  0xF0

///
/// Maximum UMA size
///
#define ME_UMA_SIZE_UPPER_LIMIT 0x40

///
/// ME FW communication timeout value definitions
///
#define CPURV_TIMEOUT_MULTIPLIER  0x32
#define STALL_1_MILLISECOND       1000
#define STALL_100_MICROSECONDS    100

//
// DRAM Initialization Response Codes.
//
#define CBM_DIR_NON_PCR            0x01
#define CBM_DIR_PCR                0x02
// ME12
#define CBM_DIR_GLOBAL_RESET       0x03
// values 4 and greater - continue POST
#define CBM_DIR_CONTINUE_POST      0x04
// Additional code for UMA support
#define CBM_DIR_DO_NOTHING         0x00
#define CBM_DIR_NON_PCR_CLEARDISB  0xFF

//
// Memory initialization status passed to ME using DramInitDoneMessage
//
#define ME_MEMORY_INITIALIZATION_SUCCESSS                        0x00
#define ME_MEMORY_INITIALIZATION_NO_MEMORY_IN_CHANNELS           0x01
#define ME_MEMORY_INITIALIZATION_ERROR                           0x02

///
/// Revision
///
#define ME_UMA_PPI_REVISION 1

/**
  This function reads configurations for UMA.

  @retval Return Status based on errors that occurred while waiting for time to expire.
**/
typedef
BOOLEAN
(EFIAPI *ME_UMA_IS_ENABLED) (
  VOID
  );

/**
  This procedure will send the information to CSME about the IMRs which need to be
  enabled and retrieve the information about total size of memory and alignment
  required for IMRs.

  @param[in]  FfsHeader           Pointer to the FFS file header
  @param[out] Alignment           Pointer to required address alignment

  @retval UINT32                  Return ME UMA Size
  @retval 0                       Do not check for ME UMA
**/
typedef
UINT32
(EFIAPI *ME_GET_UMA_SIZE) (
  IN EFI_FFS_FILE_HEADER * FfsHeader,
  OUT UINT32             * Alignment
  );

/**
  Save MeUma parameters used to send DramInitDone message on MemoryDiscovered signal

  @param [in] MrcStatus           MRC status for checking if MRC returns SUCCESS status
  @param [in] MemSize             Size of system memory discovered by MRC
  @param [in] PreviousBootError   Previous boot error
  @param [in] SubBootMode         Sub boot mode
  @param [in] MeNcMemLowBaseAddr  ME UMA low part of base address
  @param [in] MeNcMemHighBaseAddr ME UMA high part of base address
  @param [in] MeNcMemLowLimit     ME UMA low address part of area limit
  @param [in] MeNcMemHighLimit    ME UMA high address part of area limit

  @retval EFI_SUCCESS             Parameters successfully saved
**/
typedef
EFI_STATUS
(EFIAPI *ME_UMA_SAVE_DID_PARAMETERS) (
  IN UINT32                MrcStatus,
  IN UINT32                MemSize,
  IN UINT8                 PreviousBootError,
  IN UINT8                 SubBootMode,
  IN UINT32                MeNcMemLowBaseAddr,
  IN UINT32                MeNcMemHighBaseAddr,
  IN UINT32                MeNcMemLowLimit,
  IN UINT32                MeNcMemHighLimit
  );

/**
  This procedure will determine whether or not the CPU was replaced
  during system power loss or via dynamic fusing.
  Calling this procedure could result in a warm reset (if ME FW is requesting one).

  @param[in] FfsHeader            Not used.
  @param[in] ForceFullTraining    When set = 0x1, MRC will be forced to perform
                                  a full memory training cycle.
  @param[in] nextStep             returns information about required next action

  @retval EFI_SUCCESS             The function completed successfully.
**/
typedef
EFI_STATUS
(EFIAPI *ME_UMA_CPU_REPLACEMENT_CHECK) (
  IN EFI_FFS_FILE_HEADER   *FfsHeader,
  UINT8                    *ForceFullTraining,
  UINT8                    *nextStep
  );

/**
  This procedure will Host Reset Warning message over HECI to ME.
  Host Reset Warning message is used in server BIOS to warn ME about
  a reset of Host partition done on Sx resume path that will not break
  the Sx resume. I.e. after reset Host continues the same Sx flow,
  so ME should ignore this reset and sustaing previews flow.

  @param[out] pAction   Optional pointer to buffer for action requested by ME

  @retval EFI_SUCCESS   Always return EFI_SUCCESS
**/
typedef
EFI_STATUS
(EFIAPI *ME_UMA_SEND_HOST_RESET_WARNING) (
  OUT UINT8                *pAction
  );

/**
  This procedure will enforce the BIOS Action that was requested by ME FW
  as part of the DRAM Init Done message.

  @param[in] BiosAction           Me requests BIOS to act

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
typedef
EFI_STATUS
(EFIAPI *ME_UMA_HANDLE_ME_BIOS_ACTION) (
  UINT8                    BiosAction
  );

///
/// MeUma Info HOB
//
typedef struct {
  UINT32                MeUmaSize;
  UINT8                 MeUmaSizeError;              // Indicate that error during gathering UMA Size
  UINT8                 InitStat;
  UINT32                MeNcMemLowBaseAddr;
  UINT32                MeNcMemHighBaseAddr;
  UINT32                MeNcMemLowLimit;
  UINT32                MeNcMemHighLimit;
} ME_UMA_INFO;

/**
  Get pointer to ME_UMA_INFO stored in a HOB


  @retval ME_UMA_INFO*    Pointer to ME_UMA_INFO
  @retval NULL            Error, HOB cant be located
**/
typedef
ME_UMA_INFO*
(EFIAPI *ME_UMA_GET_INFO) (
  VOID
  );

///
/// PPI definition
///
typedef struct {
  ME_UMA_IS_ENABLED              MeUmaIsEnabled;
  ME_GET_UMA_SIZE                MeUmaGetSize;
  ME_UMA_SAVE_DID_PARAMETERS     MeUmaSaveDramInitDoneParameters;
  ME_UMA_CPU_REPLACEMENT_CHECK   MeUmaCpuReplacementCheck;
  ME_UMA_SEND_HOST_RESET_WARNING MeUmaSendHostResetWarning;
  ME_UMA_HANDLE_ME_BIOS_ACTION   MeUmaHandleBiosAction;
  ME_UMA_GET_INFO                MeUmaGetInfo;
} ME_UMA_PPI;

#endif // _ME_UMA_PPI_H_
