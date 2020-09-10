//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file FrbProtocol.h
    Frb protocol structure, macros and function declarations.

*/

#ifndef _FRB_PROTOCOL_H_
#define _FRB_PROTOCOL_H_

//---------------------------------------------------------------------------

#include <AmiIpmiNetFnAppDefinitions.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_SM_FRB_PROTOCOL_GUID \
    { \
        0xbc5f861c, 0x86ed, 0x417e, { 0xbb, 0x7b, 0x6c, 0x2, 0x6b, 0xcd, 0x75, 0x5b } \
    }

typedef struct _EFI_SM_FRB_PROTOCOL EFI_SM_FRB_PROTOCOL;

/**
    FRB timer status.
*/
typedef struct {
    /// Frb is enabled or not.
    BOOLEAN                             FrbEnabled;
    /// Frb failed or expiration.
    BOOLEAN                             FrbFailed;
    /// Frb timeout.
    UINTN                               FrbTimeout;
    /// Frb current boot policy.
    AMI_WATCHDOG_TIMEOUT_ACTION         FrbCurrentBootPolicy;
    /// Frb timeout interrupt.
    AMI_WATCHDOG_PRE_TIMEOUT_INTERRUPT  FrbTimeoutInterrupt;
} EFI_FRB_STAT;

/**
    This API gets the FRB timer status.

    @param[in]  This            Pointer to the FRB protocol.
    @param[in]  FrbType         Type of FRB timer to get data on.
    @param[out] FrbStatData     Timer data is returned in this structure.

    @retval EFI_SUCCESS             Data successfully retrieved and copied
                                    to structure.
    @retval EFI_UNSUPPORTED         Not supported for current FRB type.
    @retval EFI_INVALID_PARAMETER   Invalid parameter passed in.
    @retval EFI_NOT_FOUND           Frb timer is not same as timer in use.
    @retval Others                  Error status from functions calls used.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRB_STAT) (
  IN  EFI_SM_FRB_PROTOCOL       *This,
  IN  AMI_WATCHDOG_TIMER_USE    FrbType,
  OUT EFI_FRB_STAT              *FrbStatData
);

/**
    This API sets the FRB timer status.

    @param[in] This     Pointer to the FRB protocol.
    @param[in] FrbType  Type of FRB timer to get data on.
    @param[in] Timeout  Timer timeout value in microseconds.

    @retval EFI_SUCCESS             Timer successfully set.
    @retval EFI_UNSUPPORTED         Not supported for current FRB type.
    @retval EFI_INVALID_PARAMETER   Invalid parameter passed in.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRB_TIMER) (
  IN EFI_SM_FRB_PROTOCOL    *This,
  IN AMI_WATCHDOG_TIMER_USE FrbType,
  IN UINTN                  *Timeout
);

/**
    This API set the FRB pre timeout policy.

    @param[in] This                 Pointer to the FRB protocol.
    @param[in] FrbType              Type of FRB timer to get data on.
    @param[in] PretimeoutInterval   Pre timeout interval in microseconds.
    @param[in] FrbTimeoutInterrupt  Type of FRB pre time out interrupt.

    @retval EFI_SUCCESS         Pre timeout interval successfully set.
    @retval EFI_UNSUPPORTED     Timer does not support pre timeout interval.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRB_PRE_TIMEOUT_POLICY) (
  IN EFI_SM_FRB_PROTOCOL                *This,
  IN AMI_WATCHDOG_TIMER_USE             FrbType,
  IN UINTN                              *PreTimeoutInterval,
  IN AMI_WATCHDOG_PRE_TIMEOUT_INTERRUPT FrbTimeoutInterrupt
);

/**
    This API sets the boot policy to take on an FRB watchdog timeout.

    @param[in] This             Pointer to the FRB protocol.
    @param[in] FrbType          Type of FRB timer to get data on.
    @param[in] FrbBootPolicy    Boot policy to take on FRB timeout.

    @retval EFI_SUCCESS         Boot policy set for selected FRB Type.
    @retval EFI_UNSUPPORTED     Boot policy or timer not supported.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRB_BOOT_POLICY) (
  IN EFI_SM_FRB_PROTOCOL            *This,
  IN AMI_WATCHDOG_TIMER_USE         FrbType,
  IN AMI_WATCHDOG_TIMEOUT_ACTION    FrbBootPolicy
);

/**
    This API enables and sets up the selected FRB timer.

    @param[in] This                 Pointer to the FRB protocol.
    @param[in] FrbType              Type of FRB timer to get data on.
    @param[in] ClearFrbGlobalStatus Clear global status of the FRB.

    @retval EFI_SUCCESS         FRB timer is enabled successfully.
    @retval EFI_DEVICE_ERROR    Couldn't Enable FRB-n because of device
                                failure.
    @retval EFI_UNSUPPORTED     FRB timer is not supported.
    @retval EFI_TIMEOUT         Residual count is already 0, timed out.
    @retval Others              Error status from functions calls used.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_FRB) (
  IN EFI_SM_FRB_PROTOCOL    *This,
  IN AMI_WATCHDOG_TIMER_USE FrbType,
  IN BOOLEAN                ClearFrbGlobalStatus
);

/**
    This API disables the specified FRB timer.

    @param[in] This     Pointer to the FRB protocol.
    @param[in] FrbType  Type of FRB timer to get data on.

    @retval EFI_SUCCESS         FRB timer is disabled successfully.
    @retval EFI_UNSUPPORTED     FRB timer is not supported.
    @retval EFI_ABORTED         Timer was already stopped.
    @retval EFI_NOT_FOUND       Timer running is not same as FrbType.
    @retval Others              Error status from functions calls used.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DISABLE_FRB) (
  IN EFI_SM_FRB_PROTOCOL    *This,
  IN AMI_WATCHDOG_TIMER_USE FrbType
);

/**
    Frb protocol.
*/
struct _EFI_SM_FRB_PROTOCOL {
    /// Gets the Frb timer status.
    EFI_GET_FRB_STAT                GetFrbStatData;

    /// Sets the Frb timer status.
    EFI_SET_FRB_TIMER               SetFrbTimerData;

    /// Sets the Frb Pre timout policy.
    EFI_SET_FRB_PRE_TIMEOUT_POLICY  SetFrbPreTimeoutPolicy;

    /// Sets Frb boot policy.
    EFI_SET_FRB_BOOT_POLICY         SetFrbBootPolicy;

    /// Enables Frb timer.
    EFI_ENABLE_FRB                  EnableFrb;

    /// Disables Frb timer.
    EFI_DISABLE_FRB                 DisableFrb;
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiSmFrbProtocolGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _FRB_PROTOCOL_H_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
