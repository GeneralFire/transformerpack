/** @file
  HECI Transport protocol driver core operations for use in PEI, DXE, and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/
#include <PiPei.h>
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/HeciTraceLib.h>
#include <Library/HeciConfigureLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/MeRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include "HeciTransportCore.h"
#include <Protocol/HeciAccessProtocol.h>


#define HECI_TRANSPORT_DEBUG   "[HECI Transport-%d %s]"
extern CHAR16      *mHeciModuleDebugPrefix;

/**
  Function gets initialization state flag

  Initialization state flag should be set to true after interface
  is successfully initialized

  @param[in]      This          Pointer to HECI Transport device context structure

  @retval TRUE      Heci interface is initialized
  @retval FALSE     Heci interface is not initialized
**/
BOOLEAN
GetInitializationState (
  IN HECI_TRANSPORT_PRIVATE *This
  )
{
  BOOLEAN      ReturnInitializationState;

  ReturnInitializationState = FALSE;
  if (This != NULL) {
    ReturnInitializationState = This->IsInitialized;
  }

  return ReturnInitializationState;
}

/**
  Get HECI Device Information Record

  @param[in]      This           Pointer to HECI Transport device context structure
  @param[in, out] HeciDeviceInfo Pointer to HECI device information structure

  @retval On success HECI Device Information record is received,
          with valid information
  @retval On error HECI Device Information record is received,
          with MAX_UINTN information for properly passed HeciDeviceInfo pointer
**/
HECI_DEVICE_INFO*
GetHeciDeviceInfo (
  IN      HECI_TRANSPORT_PRIVATE     *This,
  IN OUT  HECI_DEVICE_INFO           *HeciDeviceInfo
  )
{
  HECI_DEVICE_INFO   *HeciDeviceInfoReturn;
  HECI_ACCESS        *HeciAccess;
  EFI_STATUS         Status;

  HeciDeviceInfoReturn = NULL;
  if (HeciDeviceInfo != NULL) {
    HeciDeviceInfoReturn = HeciDeviceInfo;
    HeciDeviceInfoReturn->HeciPch = MAX_UINTN;
    HeciDeviceInfoReturn->HeciSegment = MAX_UINTN;
    HeciDeviceInfoReturn->HeciBus = MAX_UINTN;
    HeciDeviceInfoReturn->HeciDevice = MAX_UINTN;
    HeciDeviceInfoReturn->HeciFunction = MAX_UINTN;
    HeciDeviceInfoReturn->HeciNo = MAX_UINTN;
    if (This != NULL) {
      HeciDeviceInfoReturn->HeciSegment = This->PciSegment;
      HeciDeviceInfoReturn->HeciBus = This->PciBus;
      HeciDeviceInfoReturn->HeciDevice = This->PciDevice;
      HeciDeviceInfoReturn->HeciFunction = This->PciFunction;
      HeciAccess = GetHeciAccess (This);
      if (HeciAccess != NULL) {
        Status = HeciAccess->GetHeciPch (HeciAccess, &HeciDeviceInfoReturn->HeciPch);
        ASSERT_EFI_ERROR (Status);
        Status = GetHeciNumber (HeciDeviceInfoReturn->HeciFunction, &HeciDeviceInfoReturn->HeciNo);
        ASSERT_EFI_ERROR (Status);
      } else {
        ASSERT (HeciAccess != NULL);
      }
    }
  }
  return HeciDeviceInfoReturn;
}

/**
  Return Host Firmware status
  Corresponds to HECI HPS (part of) section 4.2.1

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in,out] FirmwareStatus Pointer to returned firmware status

  @retval EFI_SUCCESS           Returned valid firmware status
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      Error occurred
**/
EFI_STATUS
ReadHeciFirmwareStatus (
  IN      HECI_TRANSPORT_PRIVATE *HeciTransport,
  IN OUT UINT32                  *FirmwareStatus
  )
{
  HECI_ACCESS *HeciAccess;
  EFI_STATUS  Status;
  UINTN       HeciNumber;

  if (FirmwareStatus == NULL) {
    ASSERT (FirmwareStatus != NULL);
    return EFI_INVALID_PARAMETER;
  }
  *FirmwareStatus = MAX_UINT32;
  if (HeciTransport == NULL) {
    ASSERT (HeciTransport != NULL);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n", __FUNCTION__));

  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   R_ME_HFS,
                                   1,
                                   FirmwareStatus
                                   );

  return Status;
}

/**
  Return number of filled slots in HECI circular buffer.
  Corresponds to HECI HPS (part of) section 4.2.1

  @param[in]      Offset        Offset to Host/Me Control Status Register

  @retval FilledSlots    Number of filled slots in circular buffer
  @retval MAX_UINT32     Circular buffer overflow has occurred
**/
UINT32
GetFilledSlots (
  IN UINT32 ControlStatusRegister
  )
{
  UINT8 FilledSlots;

  //
  // Calculation documented in HECI HPS 0.68 section 4.2.1
  //
  FilledSlots = (INT8)GET_CB_WRITE_POINTER (ControlStatusRegister);
  FilledSlots -= (INT8)GET_CB_READ_POINTER (ControlStatusRegister);

  return (FilledSlots > (UINT8)GET_CB_DEPTH (ControlStatusRegister) ? MAX_UINT32 : FilledSlots);
}


/**
  Checks if HECI Interrupt Delivery Mode (HIDM) register is locked
  (This means HECI is initialized)

  @param[in]     HeciAccess       HECI access pointer the message has to be sent via
  @param[in]     InterruptLocked  Pointer to returned InterruptLocked register

  @retval Operation EFI_STATUS
**/
EFI_STATUS
ReadInterruptsLocked (
  IN      HECI_ACCESS     *HeciAccess,
  IN OUT  UINT8           *InterruptLocked
  )
{
  EFI_STATUS               Status;
  UINT8                    InterruptLockedLocal;

  if (InterruptLocked == NULL) {
    InterruptLocked = &InterruptLockedLocal;
  }
  *InterruptLocked = MAX_UINT8;
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint8,
                                   R_ME_HIDM,
                                   1,
                                   InterruptLocked
                                   );
  if (EFI_ERROR (Status) || (*InterruptLocked == MAX_UINT8)) {
    // 0xFF value means device disabled
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Check if HECI timeouts expired
  Add one timeout tick.

  @retval TRUE             Timeout is expired
  @retval FALSE            Timeout is not expired
**/
BOOLEAN
IsHeciTimeout (
  IN UINT32 *Timeout
  )
{
  if ((Timeout == NULL) ||
      (*Timeout == 0) ||
      (!GetHeciTimeoutsEnabled ())) {
    return TRUE;
  }

  if (*Timeout < HECI_TIMEOUT_TICK) {
    *Timeout = 0;
  } else {
    *Timeout -= HECI_TIMEOUT_TICK;
  }
  MicroSecondDelay (HECI_TIMEOUT_TICK);

  return FALSE;
}

/**
  Checks if ME is ready for communication over the HECI interface.

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in]     Timeout       Timeout value

  @retval TRUE   ME is ready
  @retval FALSE  ME is not ready
**/
BOOLEAN
IsMeReady (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN     UINT32                   *Timeout
  )
{
  UINT32       ControlStatusRegister;
  UINT16       MeVendorId;
  EFI_STATUS   Status;
  HECI_ACCESS  *HeciAccess;
  UINTN        HeciNumber;

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return FALSE;
  }

  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    return FALSE;
  }

  do {
    ///
    /// Check for reset first and then for ME Ready
    ///
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_CSR_HA,
                                     1,
                                     &ControlStatusRegister
                                     );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return FALSE;
    }
    if (GET_RESET (ControlStatusRegister) == 1) {
      return FALSE;
    }
    if (GET_READY (ControlStatusRegister) == 1) {
      return TRUE;
    }
  } while (!IsHeciTimeout (Timeout));

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_ERROR, " ERROR: Timeout due to ME_RDY bit not set after %d seconds\n", *Timeout));

  return FALSE;
}

/**
  Public functions.
**/

/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_UNSUPPORTED       Device not supported for this operation
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_SUCCESS           Reset HECI queue succeed
**/
EFI_STATUS
EFIAPI
HeciReset (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout
  )
{
  EFI_STATUS         Status;
  UINT32             ResetTimeout;
  UINT32             ControlStatusRegister;
  UINT32             MeControlStatusRegister;
  UINT16             MeVendorId;
  HECI_ACCESS        *HeciAccess;
  UINTN              HeciNumber;

  if (HeciTransport == NULL) {
    ASSERT (HeciTransport != NULL);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n", __FUNCTION__));

  if (EFI_ERROR (ReadInterruptsLocked (HeciAccess, NULL))) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: HECI device not initialized\n"));
    return EFI_DEVICE_ERROR;
  }

  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: Reset failed (device disabled)\n"));
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  ResetTimeout = HeciTransport->ResetTimeout;
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_H_CSR,
                                   1,
                                   &ControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CSR_HA,
                                   1,
                                   &MeControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_WARN, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((
    DEBUG_WARN,
    " WARNING: Resetting HECI interface (CSR %08X/%08X)\n",
    ControlStatusRegister,
    MeControlStatusRegister
    ));

  if (GET_RESET (ControlStatusRegister) == 0) {
    SET_RESET (ControlStatusRegister, 1);
    SET_INT_GENERATE (ControlStatusRegister, 1);
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Wait for H_RDY cleared to make sure that the reset started.
  //
  Status = EFI_SUCCESS;
  while (TRUE) {
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
    ASSERT_EFI_ERROR (Status);
    if (GET_READY (ControlStatusRegister) == 0) {
      break;
    }
    if (IsHeciTimeout (&ResetTimeout)) {
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_CSR_HA,
                                       1,
                                       &MeControlStatusRegister
                                       );
      ASSERT_EFI_ERROR (Status);
      DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
      DEBUG ((
        DEBUG_ERROR,
        " ERROR: Reset failed (timeout)(CSR %08X/%08X)\n",
        ControlStatusRegister,
        MeControlStatusRegister
        ));
      Status = EFI_TIMEOUT;
      break;
    }
  }
  if (!EFI_ERROR (Status)) {
    //
    // Wait for ME to perform reset and signal it is ready.
    //
    while (1) {
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_CSR_HA,
                                       1,
                                       &MeControlStatusRegister
                                       );
      ASSERT_EFI_ERROR (Status);
      if (GET_READY (MeControlStatusRegister)) {
        break;
      }
      if (IsHeciTimeout (&ResetTimeout)) {
        Status = HeciAccess->PciIo.Mem.Read (
                                         &(HeciAccess->PciIo),
                                         EfiPciIoWidthUint32,
                                         0,
                                         R_H_CSR,
                                         1,
                                         &ControlStatusRegister
                                         );
        ASSERT_EFI_ERROR (Status);
        DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
        DEBUG ((
          DEBUG_ERROR,
          " ERROR: Reset failed (timeout)(CSR %08X/%08X)\n",
          ControlStatusRegister,
          MeControlStatusRegister
          ));
        Status = EFI_TIMEOUT;
        break;
      }
    }
    if (!EFI_ERROR (Status)) {
      //
      // ME side is ready, signal host side is ready too.
      //
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_H_CSR,
                                       1,
                                       &ControlStatusRegister
                                       );
      ASSERT_EFI_ERROR (Status);
      SET_RESET (ControlStatusRegister, 0);
      SET_READY (ControlStatusRegister, 1);
      SET_INT_GENERATE (ControlStatusRegister, 1);
      Status = HeciAccess->PciIo.Mem.Write (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_H_CSR,
                                       1,
                                       &ControlStatusRegister
                                       );
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (Timeout != NULL) {
    ResetTimeout = HeciTransport->ResetTimeout - ResetTimeout;
    if (*Timeout > ResetTimeout) {
      *Timeout -= ResetTimeout;
    } else {
      *Timeout = 0;
    }
  }

  return Status;
}

/**
  Initialize HECI interface.

  This function initializes host side of HECI interface. If timeout is
  greater than zero it also waits until ME is ready to receive messages.

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_UNSUPPORTED       Device not supported for this operation
  @retval EFI_DEVICE_ERROR      Device not present or HECI MBar can't be initialized
  @retval EFI_SUCCESS           Initialization succeed
**/
EFI_STATUS
EFIAPI
HeciInit (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout
  )
{
  EFI_STATUS            Status;
  UINT32                InitTimeout;
  UINT32                ControlStatusRegister;
  UINT32                MeControlStatusRegister;
  UINT16                MeVendorId;
  UINTN                 InterruptMode;
  HECI_ACCESS           *HeciAccess;
  UINTN                 HeciNumber;
  UINTN                 HeciSegment;
  UINTN                 HeciBus;
  UINTN                 HeciDevice;
  UINTN                 HeciFunction;

  if (HeciTransport == NULL) {
    ASSERT (HeciTransport != NULL);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n", __FUNCTION__));

  Status = HeciAccess->PciIo.GetLocation (
                               &(HeciAccess->PciIo),
                               &HeciSegment,
                               &HeciBus,
                               &HeciDevice,
                               &HeciFunction
                               );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((
    DEBUG_INFO,
    " HECI Seg = %d Bus = %d Dev = %d Func = %d\n",
    HeciSegment,
    HeciBus,
    HeciDevice,
    HeciFunction
    ));

  if (Timeout != NULL) {
    InitTimeout = *Timeout;
  } else {
    InitTimeout = 0;
  }

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((
    DEBUG_INFO,
    " VID-DID: %2X-%2X\n",
    PCI_SEGMENT_LIB_ADDRESS (
      HeciSegment,
      HeciBus,
      HeciDevice,
      HeciFunction,
      PCI_VENDOR_ID_OFFSET),
    PCI_SEGMENT_LIB_ADDRESS (
      HeciSegment,
      HeciBus,
      HeciDevice,
      HeciFunction,
      PCI_DEVICE_ID_OFFSET)
    ));

  Status = EFI_SUCCESS;
  //
  // Check MBAR, whether it is configured. If not then check if user provided
  // some address to configure in such a case.
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: Init failed (device disabled)\n"));
    Status = EFI_DEVICE_ERROR;
  } else {
    //
    // Make sure HECI interrupts are disabled before configuring delivery mode.
    //
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
    ASSERT_EFI_ERROR (Status);
    SET_INT_ENABLE (ControlStatusRegister, 0);
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
    ASSERT_EFI_ERROR (Status);

    //
    // Set HECI interrupt delivery mode.
    //
    Status = HeciAccess->GetHeciInterruptMode (HeciAccess, &InterruptMode);
    ASSERT_EFI_ERROR (Status);
    HeciAccess->PciIo.Pci.Write (
                            &(HeciAccess->PciIo),
                            EfiPciIoWidthUint8,
                            R_ME_HIDM,
                            1,
                            &InterruptMode
                            );

    //
    // If HECI was in use reset it to clear queues.
    //
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_CSR_HA,
                                     1,
                                     &MeControlStatusRegister
                                     );
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status) &&
        (GET_READY (MeControlStatusRegister) == 1)) {
      if (GET_READY (ControlStatusRegister) == 0) {
        SET_INT_GENERATE (ControlStatusRegister, 1);
        SET_READY (ControlStatusRegister, 1);
        SET_RESET (ControlStatusRegister, 0);
        Status = HeciAccess->PciIo.Mem.Write (
                                         &(HeciAccess->PciIo),
                                         EfiPciIoWidthUint32,
                                         0,
                                         R_H_CSR,
                                         1,
                                         &ControlStatusRegister
                                         );
        ASSERT_EFI_ERROR (Status);
      }
    } else {
      if (InitTimeout > 0) {
        Status = HeciReset (HeciTransport, &InitTimeout);
      }
    }
  }

  if (Timeout != NULL) {
    *Timeout = InitTimeout;
  }

  if (!EFI_ERROR (Status)) {
    HeciTransport->IsInitialized = TRUE;
  }

  return Status;
}

/**
  Write one packet to HECI queue.

  This function puts one packet to HECI queue. If timeout is greater than
  zero the function may wait for space in the queue. This function handles
  only packets shorter than HECI queue length.
  Fragmentation of large messages into packets can be done using HeciSend().

  @param[in]     HeciTransport  HECI transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     PacketHeader  Header of the packet to send
  @param[in]     PacketBody    Body of the packet to send of length specified in PacketHeader.Fields.Length

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Packet is bigger than queue length
  @retval EFI_SUCCESS           Write one packet to HECI queue succeed
**/
EFI_STATUS
HeciPacketSend (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
  IN     HECI_PACKET_HEADER       PacketHeader,
  IN     UINT32                   *PacketBody
  )
{
  UINT32             Index;
  UINT32             LengthInDwords;
  UINT32             HeciCsrHost;
  UINT16             MeVendorId;
  EFI_STATUS         Status;
  HECI_ACCESS        *HeciAccess;
  UINTN              HeciNumber;

  if (PacketBody == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  // To be sure HeciAccess is valid
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n", __FUNCTION__));

  HeciPktTrace (
    HeciTransport,
    "Send pkt: ",
    PacketHeader,
    (UINT8*)PacketBody,
    PacketHeader.Fields.Length
    );
  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: Send pkt failed (device disabled)\n"));
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Compute message length in DWORDS
  ///
  LengthInDwords = ((PacketHeader.Fields.Length + 3) / 4);

  ///
  /// Wait until there is sufficient room in the circular buffer
  /// Must have room for message and message header
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_H_CSR,
                                   1,
                                   &HeciCsrHost
                                   );
  ASSERT_EFI_ERROR (Status);
  while ((LengthInDwords + 1) >
    (GET_CB_DEPTH (HeciCsrHost) - GetFilledSlots (HeciCsrHost))) {
    if (IsHeciTimeout (Timeout)) {
      DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
      DEBUG ((
        DEBUG_ERROR,
        " ERROR: EFI_TIMEOUT in HeciPacketWrite () due"\
        " to circular buffer never emptied after waiting"
        ));
      return EFI_TIMEOUT;
    }
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_H_CSR,
                                     1,
                                     &HeciCsrHost
                                     );
    ASSERT_EFI_ERROR (Status);
  }
  ///
  /// Write Message Header
  ///
  Status = HeciAccess->PciIo.Mem.Write (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_H_CB_WW,
                                   1,
                                   &PacketHeader.Data
                                   );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Write Message Body
  ///
  for (Index = 0; Index < LengthInDwords; Index++) {
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_H_CB_WW,
                                     1,
                                     &PacketBody[Index]
                                     );
    ASSERT_EFI_ERROR (Status);
  }
  ///
  /// Set Interrupt Generate bit
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_H_CSR,
                                   1,
                                   &HeciCsrHost
                                   );
  ASSERT_EFI_ERROR (Status);
  SET_INT_GENERATE (HeciCsrHost, 1);
  Status = HeciAccess->PciIo.Mem.Write (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_H_CSR,
                                   1,
                                   &HeciCsrHost
                                   );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Test if ME Ready bit is set to 1, if set to 0 a fatal error occurred during
  /// the transmission of this message.
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CSR_HA,
                                   1,
                                   &HeciCsrHost
                                   );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status) &&
      (GET_READY (HeciCsrHost) == 0)) {
    return EFI_NOT_READY;
  }

  return EFI_SUCCESS;
}

/**
  Read one packet from HECI queue.

  This function reads one packet from HECI queue. If timeout is greater than
  zero the function may wait for the packet. Size of packet buffer is given
  in bytes in (*PacketLen) on input. On exit (*PacketLen) provides the number
  of bytes written to the packet body buffer. If buffer is too short the packet
  is truncated. This function does not do reassembly of fragmented HECI messages.
  To receive whole message reassembled use HeciReceive().

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[out]    PacketHeader  Buffer for the header of received packet
  @param[out]    PacketBody    Buffer for the body of received packet
  @param[in,out] PacketLen     On input buffer size, on exit packet body size in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message exceeds buffer size
  @retval EFI_SUCCESS           Read one packet from HECI queue succeed
**/
EFI_STATUS
HeciPacketReceive (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
     OUT HECI_PACKET_HEADER       *PacketHeader,
     OUT UINT32                   *PacketBody,
  IN OUT UINT32                   *PacketLen
  )
{
  EFI_STATUS                   Status;
  UINT32                       FilledSlots;
  UINT32                       Index;
  UINT32                       LengthInDwords;
  UINT32                       TempBuffer;
  UINT32                       BufferLength;
  UINT32                       BufferCopySize;
  UINT32                       ControlStatusRegister;
  UINT16                       MeVendorId;
  HECI_ACCESS                  *HeciAccess;
  UINTN                        HeciNumber;

  if (PacketHeader == NULL ||
      PacketBody == NULL ||
      PacketLen == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  // To be sure HeciAccess is valid
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n", __FUNCTION__));

  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: Receive failed (device disabled)\n"));
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  FilledSlots = GetFilledSlots (ControlStatusRegister);


  ///
  /// For BLOCKING read, wait until data appears in the CB or timeout occurs
  ///
  if ((Timeout != NULL) && (*Timeout != 0)) {
    while (FilledSlots == 0) {
      if (IsHeciTimeout (Timeout)) {
        return EFI_TIMEOUT;
      }
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_CSR_HA,
                                       1,
                                       &ControlStatusRegister
                                       );
      ASSERT_EFI_ERROR (Status);
      FilledSlots = GetFilledSlots (ControlStatusRegister);
    }
  }

  BufferLength = *PacketLen;
  *PacketLen = 0;

  ///
  /// Check for empty and overflowed CB
  ///
  if (FilledSlots == 0) {
    return EFI_NO_RESPONSE;
  } else if (FilledSlots == MAX_UINT32) {  // Check for HECI Circular buffer overflow
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Eat the HECI Message header
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CB_RW,
                                   1,
                                   &PacketHeader->Data
                                   );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Compute required message length in DWORDS
  ///
  LengthInDwords = ((PacketHeader->Fields.Length + 3) / 4);

  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  if (LengthInDwords > GetFilledSlots (ControlStatusRegister)) {
    ///
    /// Make sure that the message does not overflow the circular buffer.
    ///
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Wait until whole message appears in circular buffer.
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  while (GetFilledSlots (ControlStatusRegister < LengthInDwords)) {
    if (IsHeciTimeout (Timeout)) {
      return EFI_TIMEOUT;
    }
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_CSR_HA,
                                     1,
                                     &ControlStatusRegister
                                     );
    ASSERT_EFI_ERROR (Status);
  }

  ///
  /// Check if buffer can hold the message
  ///
  if ((PacketHeader->Fields.Length) <= BufferLength) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_BUFFER_TOO_SMALL;
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_ERROR,
      " ERROR: Receive buffer is too small (required: %d, have: %d)\n",
      PacketHeader->Fields.Length,
      BufferLength
      ));
    return Status;
  }

  ///
  /// Copy as much bytes as there is space left in the message buffer.
  /// Excessive bytes will be dismissed.
  ///
  for (Index = 0; Index < LengthInDwords; Index++) {
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_CB_RW,
                                     1,
                                     &TempBuffer
                                     );
    ASSERT_EFI_ERROR (Status);
    BufferCopySize = MIN (
                      sizeof (TempBuffer),
                      BufferLength
                      );
    CopyMem (
      &PacketBody[Index],
      &TempBuffer,
      BufferCopySize
      );
    BufferLength -= BufferCopySize;
    *PacketLen += BufferCopySize;
  }
  *PacketLen = PacketHeader->Fields.Length;

  ///
  /// Read R_ME_CSR_HA.  If the ME_RDY bit is 0, then an ME reset occurred during the
  /// transaction and the message should be discarded as bad data may have been retrieved
  /// from the host's circular buffer
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  if (GET_READY (ControlStatusRegister) == 0) {
    *PacketLen   = 0;
    return EFI_NOT_READY;
  }

  ///
  /// Set Interrupt Generate bit and clear Interrupt Status bit if there are no more messages in the buffer.
  ///
  if (GetFilledSlots (ControlStatusRegister) == 0) {
    SET_INT_GENERATE (ControlStatusRegister, 1);
    SET_INT_STATUS (ControlStatusRegister, 1);
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
    ASSERT_EFI_ERROR (Status);
    HeciPktTrace (
      HeciTransport,
      "Got pkt: ",
      *PacketHeader,
      (UINT8*)PacketBody,
      *PacketLen
      );
  }

  return Status;
}


/**
  Write a message to HECI queue, if necessary perform fragmentation.

  This function puts one message to HECI queue. If the message is longer than
  HECI queue size and timeout is greater than zero the function will split
  the message into several packets and wait for space in the queue to send all
  packets/fragments. Each packet has HECI packet header added.

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engined (ME/IE) address
  @param[in]     MessageBody   Body of the message to send
  @param[in]     MessageLength Length of the message to send in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_UNSUPPORTED       Device not supported for this operation
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Packet is bigger than queue length
  @retval EFI_SUCCESS           Message has been sent successfully
**/
EFI_STATUS
EFIAPI
HeciSend (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
  IN     UINT8                    HostAddress,
  IN     UINT8                    EngineAddress,
  IN     UINT32                   *MessageBody,
  IN     UINT32                   MessageLength
  )
{
  EFI_STATUS                   Status;
  HECI_PACKET_HEADER           PacketHeader;
  UINT32                       CircularBufferDepth;
  UINT32                       SendLength;
  UINT32                       BytesLeft;
  UINT32                       SendTimeout;
  UINT32                       ControlStatusRegister;
  UINT16                       MeVendorId;
  HECI_ACCESS                  *HeciAccess;
  UINTN                        HeciNumber;


  if (MessageBody == NULL) {
    ASSERT (MessageBody != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (HeciTransport == NULL) {
    ASSERT (HeciTransport != NULL);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n", __FUNCTION__));

  if (Timeout == NULL) {
    SendTimeout = HeciTransport->SendTimeout;
    Timeout = &SendTimeout;
  }

  if (EFI_ERROR (ReadInterruptsLocked (HeciAccess, NULL))) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: HECI device not initialized\n"));
    return EFI_DEVICE_ERROR;
  }

  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: Send pkt failed (device disabled)\n"));
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Make sure that HECI is ready for communication
  ///
  if (!IsMeReady (HeciTransport, Timeout)) {
    ///
    /// If reset successful, continue as the interface should be healthy now
    ///
    Status = HeciReset (
               HeciTransport,
               Timeout
               );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_READY;
    }
  }

  ///
  /// Grab Circular Buffer length and convert it from Dword to bytes
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_H_CSR,
                                   1,
                                   &ControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  CircularBufferDepth = 4 * GET_CB_DEPTH (ControlStatusRegister);

  if (IsSimicsEnvironment ()) {
    if (GET_CB_DEPTH (ControlStatusRegister) >= MAX_CB_DEPTH_SIMICS) {
      CircularBufferDepth = 4 * MAX_CB_DEPTH_SIMICS;
    }
  }


  ///
  /// Prepare message header
  ///
  PacketHeader.Data = 0;
  PacketHeader.Fields.EngineAddress = EngineAddress;
  PacketHeader.Fields.HostAddress = HostAddress;

  BytesLeft = MessageLength;
  ///
  /// Break message up into CB-sized packets and loop until completely sent
  ///
  while (BytesLeft) {
    ///
    /// Set the MessageBody Complete bit if this is our last packet in the message.
    /// Needs to be less or equal to CB depth minus one Dword for HECI header.
    ///
    if (BytesLeft <= CircularBufferDepth - sizeof (PacketHeader)) {
      PacketHeader.Fields.MessageComplete = 1;
    }
    ///
    /// Calculate length for MessageBody Header:
    /// It will be the smaller value of circular buffer or remaining message.
    ///
    SendLength = MIN (
                   CircularBufferDepth - sizeof (PacketHeader),
                   BytesLeft
                   );
    PacketHeader.Fields.Length = SendLength;
    ///
    /// Send the current packet
    ///
    Status = HeciPacketSend (
      HeciTransport,
      Timeout,
      PacketHeader,
      (UINT32*)((UINTN)MessageBody + (MessageLength - BytesLeft))
      );
    if (EFI_ERROR (Status)) {
      Status = HeciReset (HeciTransport, Timeout);
      break;
    }
    ///
    /// Update the length information
    ///
    BytesLeft -= SendLength;
  }
  return Status;
}

/**
  Read a message from HECI queue, if necessary perform reassembly.

  This function reads one message from HECI queue. If the message is
  fragmented and timeout is greater than zero the function will wait for
  subsequent packets (fragments) until full message is completed.

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[out]    HostAddress   Host address
  @param[out]    EngineAddress Engine (ME/IE) address
  @param[out]    MessageBody   Body of the message to receive
  @param[in,out] MessageLength On input buffer size, on exit message body size in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_UNSUPPORTED       Device not supported for this operation
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message exceeds buffer size
  @retval EFI_SUCCESS           Message has been received successfully
**/
EFI_STATUS
EFIAPI
HeciReceive (
  IN     HECI_TRANSPORT_PRIVATE  *HeciTransport,
  IN OUT UINT32                  *Timeout,
     OUT UINT8                   *HostAddress,
     OUT UINT8                   *EngineAddress,
     OUT UINT32                  *MessageBody,
  IN OUT UINT32                  *MessageLength
  )
{
  EFI_STATUS              Status;
  UINT8                   HAddr;
  UINT8                   EAddr;
  UINT32                  PacketLen;
  HECI_PACKET_HEADER      PacketHeader;
  UINT32                  *PacketBody;
  UINT32                  ReceiveTimeout;
  UINT32                  TotalLength;
  HECI_ACCESS             *HeciAccess;
  UINTN                   HeciNumber;

  if (MessageBody == NULL ||
      MessageLength == NULL ||
      HeciTransport == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n", __FUNCTION__));

  if (Timeout == NULL) {
    ReceiveTimeout = HeciTransport->ReceiveTimeout;
    Timeout = &ReceiveTimeout;
  }

  if (EFI_ERROR (ReadInterruptsLocked (HeciAccess, NULL))) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: HECI device not initialized\n"));
    return EFI_DEVICE_ERROR;
  }

  PacketHeader.Data = 0;
  TotalLength       = 0;
  HAddr             = 0;
  EAddr             = 0;
  PacketBody        = MessageBody;


  if (!IsMeReady (HeciTransport, Timeout)) {
    //
    // Return as CB will be empty after reset and CSME will not put any data
    //
    HeciReset (HeciTransport, Timeout);
    return EFI_NOT_READY;
  }

  //
  // Read until MessageComplete bit is set
  //
  do {
    if (*MessageLength > TotalLength) {
      PacketLen = *MessageLength - TotalLength;
    } else {
      ///
      /// Buffer is too small to receive data
      /// Reset HECI to clear data in buffer and exit with error
      ///
      HeciReset (HeciTransport, Timeout);
      Status = EFI_BUFFER_TOO_SMALL;
      break;
    }
    Status = HeciPacketReceive (
               HeciTransport,
               Timeout,
               &PacketHeader,
               PacketBody,
               &PacketLen
               );

    ///
    /// If timeout occurred we need to reset the interface to clear the data that could possibly come later.
    /// Also buffer overflow and transaction errors will require a reset.
    /// We need to continue read even if buffer too small to clear the data and signal the buffer size.
    ///
    if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
      if (Status != EFI_NO_RESPONSE) {
        HeciReset (HeciTransport, Timeout);
      }
      *MessageLength = TotalLength;
      break;
    }

    if (PacketBody == MessageBody) { // If it is first fragment/packet
      HAddr = (UINT8)PacketHeader.Fields.HostAddress;
      EAddr = (UINT8)PacketHeader.Fields.EngineAddress;
    } else if (PacketHeader.Fields.HostAddress != HAddr ||
               PacketHeader.Fields.EngineAddress != EAddr) {
      DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
      DEBUG ((
        DEBUG_ERROR,
        " ERROR: Got packet %X->%X, while waiting for %X->%X, dropping\n",
        PacketHeader.Fields.EngineAddress,
        PacketHeader.Fields.HostAddress,
        EAddr,
        HAddr
        ));
      PacketLen = 0;
    }

    ///
    /// Track the MessageLength of what we have read so far
    ///
    TotalLength += PacketLen;
    PacketBody = (UINT32*)((UINT8*)PacketBody + PacketLen);
  } while (PacketHeader.Fields.MessageComplete == 0);

  if (!EFI_ERROR (Status)) {
    *MessageLength = TotalLength;

    if (HostAddress != NULL) {
      *HostAddress = HAddr;
    }
    if (EngineAddress != NULL) {
      *EngineAddress = EAddr;
    }
  }

  return Status;
}

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the ReqMsg and RspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     Retries       The number of retries requested (0 => no retry)
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message
  @param[in]     ReqLen        Request message length in bytes
  @param[out]    RspMsg        Buffer for the response message
  @param[in,out] RspLen        On input buffer size, on exit message, in bytes
  @param[in,out] SendStatus    Send operation status
  @param[in,out] ReceiveStatus Receive operation status

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_UNSUPPORTED       Device not supported for this operation
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_SUCCESS           Message request has been completed successfully
**/
EFI_STATUS
EFIAPI
HeciSendAndReceive (
  IN     HECI_TRANSPORT_PRIVATE  *HeciTransport,
  IN OUT UINT32                  *Timeout,
  IN     UINT8                   Retries,
  IN     UINT8                   HostAddress,
  IN     UINT8                   EngineAddress,
  IN     UINT32                  *ReqMsg,
  IN     UINT32                  ReqLen,
     OUT UINT32                  *RspMsg,
  IN     UINT32                  *RspLen,
  IN OUT EFI_STATUS              *SendStatus,
  IN OUT EFI_STATUS              *ReceiveStatus
  )
{
  EFI_STATUS       Status;
  UINT32           RspLenTemp;
  UINT8            ReceiveHostAddress;
  UINT8            ReceiveEngineAddress;
  UINT32           SingleRequestTimeout;
  HECI_ACCESS      *HeciAccess;
  UINTN            HeciNumber;
  UINT32           ControlStatusRegister;

  if (ReqMsg == NULL ||
     (RspMsg != NULL && RspLen == NULL) ||
     (ReqMsg == RspMsg) ||
     (HeciTransport == NULL) ||
     (Retries == 0)) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    if (SendStatus != NULL) {
      *SendStatus = EFI_INVALID_PARAMETER;
    }
    if (ReceiveStatus != NULL) {
      *ReceiveStatus = EFI_INVALID_PARAMETER;
    }
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    if (SendStatus != NULL) {
      *SendStatus = EFI_DEVICE_ERROR;
    }
    if (ReceiveStatus != NULL) {
      *ReceiveStatus = EFI_DEVICE_ERROR;
    }
    return EFI_DEVICE_ERROR;
  }

  Status = GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a()\n",__FUNCTION__));

  //
  // If HECI buffer is not empty reset HECI to clean it up
  //
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    if (GetFilledSlots (ControlStatusRegister) > 0) {
      HeciReset (HeciTransport, Timeout);
    }
  }

  if (Timeout != NULL) {
    SingleRequestTimeout = *Timeout;
  }

  RspLenTemp = 0;
  ReceiveHostAddress = HostAddress;
  ReceiveEngineAddress = EngineAddress;

  while (Retries > 0)  {
    //
    // Reload timeout if provided
    //
    if (Timeout != NULL) {
      *Timeout = SingleRequestTimeout;
    }

    //
    // Send the request and wait for response if response expected
    //
    Status = HeciSend (
               HeciTransport,
               Timeout,
               HostAddress,
               EngineAddress,
               ReqMsg,
               ReqLen
               );
    if (SendStatus != NULL) {
      *SendStatus = Status;
    }
    while (!EFI_ERROR (Status) && RspMsg != NULL) {
      RspLenTemp = *RspLen;
      Status = HeciReceive (
                 HeciTransport,
                 Timeout,
                 &ReceiveHostAddress,
                 &ReceiveEngineAddress,
                 RspMsg,
                 &RspLenTemp
                 );
      if (ReceiveStatus != NULL) {
        *ReceiveStatus = Status;
      }

      if (!EFI_ERROR (Status)) {
        if (ReceiveHostAddress != HostAddress ||
            ReceiveEngineAddress != EngineAddress) {
          DEBUG ((DEBUG_WARN, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
          DEBUG ((
            DEBUG_WARN,
            " WARNING: Got message %X->%X, while waiting for %X->%X, skipping\n",
            ReceiveEngineAddress,
            ReceiveHostAddress,
            EngineAddress,
            HostAddress
            ));
          Status = EFI_DEVICE_ERROR;
        } else {
          Retries = 0;
          break;
        }
      } else {
        if (Status == EFI_DEVICE_ERROR) {
          DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
          DEBUG ((
            DEBUG_ERROR,
            " ERROR: HECI request failed because of device error, aborting\n"
            ));
          Retries = 0;
        }
        break;
      }
    }

    if (Retries == 0) {
      break;
    }

    Retries--;
    DEBUG ((DEBUG_WARN, HECI_TRANSPORT_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_WARN,
      " WARNING: HECI request failed (%r), %d retries left\n",
      Status,
      Retries
      ));
  }
  if (RspLen != NULL) {
    *RspLen = RspLenTemp;
  }

  return Status;
}

/**
  Initialize HECI_TRANSPORT protocol functions

  @param[out]  Heci  HECI transport pointer to be initialized
**/
VOID
SetHeciTransportFunctions (
  OUT HECI_TRANSPORT_PRIVATE *Heci
  )
{
  if (Heci == NULL) {
    ASSERT (Heci != NULL);
    return;
  }

  Heci->HeciTransport.GetInitializationState = (HECI_TRANSPORT_GET_INIT_STATE) GetInitializationState;
  Heci->HeciTransport.GetHeciDeviceInfo      = (HECI_TRANSPORT_GET_HECI_DEVICE_INFO) GetHeciDeviceInfo;
  Heci->HeciTransport.ReadHeciFirmwareStatus = (HECI_TRANSPORT_GET_FIRMWARE_STATUS) ReadHeciFirmwareStatus;

  Heci->HeciTransport.HeciInit           = (HECI_TRANSPORT_INIT) HeciInit;
  Heci->HeciTransport.HeciReset          = (HECI_TRANSPORT_RESET) HeciReset;
  Heci->HeciTransport.HeciSend           = (HECI_TRANSPORT_SEND) HeciSend;
  Heci->HeciTransport.HeciReceive        = (HECI_TRANSPORT_RECEIVE) HeciReceive;
  Heci->HeciTransport.HeciSendAndReceive = (HECI_TRANSPORT_SEND_AND_RECEIVE) HeciSendAndReceive;

  Heci->HeciTransport.GetNextHeciTransport = (HECI_TRANSPORT_GET_NEXT_HECI) GetNextHeciTransport;
}

/**
  Initialize HECI_TRANSPORT protocol instance

  @param[in]     HeciTransport         HECI transport pointer the be initialized
  @param[in]     HeciAccess            Associated HECI access instance

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_SUCCESS           Heci transport initialized
**/
EFI_STATUS
HeciInitializeNewTransportInstance (
  IN OUT HECI_TRANSPORT_PRIVATE *Heci,
  IN     HECI_ACCESS            *HeciAccess
  )
{
  EFI_STATUS    Status;

  if (Heci == NULL) {
    ASSERT (Heci != NULL);
    return EFI_INVALID_PARAMETER;
  }
  if (HeciAccess == NULL) {
    ASSERT (HeciAccess != NULL);
    return EFI_INVALID_PARAMETER;
  }

  SetHeciTransportFunctions (Heci);

  // Data
  if (sizeof (UINT32) == sizeof (UINTN)) {
    // for PEI HeciAccess should be initialized in PostMem
    // before PostMem we should use HeciAccessIndex
    Heci->HeciAccess = NULL;
  } else {
    Heci->HeciAccess = HeciAccess;
  }
  Heci->HeciAccessIndex = MAX_INT32;
  Heci->IsInitialized = FALSE;
  Heci->SendTimeout = HECI_SEND_TIMEOUT;
  Heci->ReceiveTimeout = HECI_READ_TIMEOUT;
  Heci->ResetTimeout = HECI_RESET_TIMEOUT;
  Status = HeciAccess->PciIo.GetLocation (
                               &(HeciAccess->PciIo),
                               &(Heci->PciSegment),
                               &(Heci->PciBus),
                               &(Heci->PciDevice),
                               &(Heci->PciFunction)
                               );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}