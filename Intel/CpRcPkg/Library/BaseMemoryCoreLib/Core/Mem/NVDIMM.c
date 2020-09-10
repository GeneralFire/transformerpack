/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/TimerLib.h>
#include <Library/SysHostPointerLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/SpdAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/NvdimmnDefinitions.h>

//Local Prototypes
/**
  Sends SMBUS commands to switch to a given page

  @param[in]  Socket      Socket number
  @param[in]  Channel     Channel number
  @param[in]  Dimm        Dimm number
  @param[in]  SmbusDev    Structure containing information about
                          the SMBUS device to be accessed
  @param[in]  Page        Desired open page

  @retval EFI_DEVICE_ERROR if current page is not desired page after
          switch.

**/
EFI_STATUS
NvdimmNSwitchSmbusPage (
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm,
  IN SMB_DEVICE_STRUCT SmbusDev,
  IN UINT8             Page
  );

/**
  If page switch latency is not known, read from PAGE_SWITCH_LATENCY from SMBUS,
  else return page switch latency

  @param[in]  Socket              Socket number
  @param[in]  Channel             Channel number
  @param[in]  Dimm                Dimm number
  @param[in]  SmbusDev            Structure containing information about the
                                  SMBUS device to be accessed
  @param[out] *PageSwitchLatency  Pointer to UINT32 with the PageSwitchLatency for the NVDIMM

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
GetPageSwitchLatency (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  IN  SMB_DEVICE_STRUCT SmbusDev,
  OUT UINT32            *PageSwitchLatency
  );

/**
  NvdimmNGetValueInMilliseconds
  Returns a 32bit unsigned value in milliseconds based on 2 bytes of input:

  @param[in]     LsbValue      the least significant byte of the input value
  @param[in]     MsbValue      Bits 6:0 are the most significant bits of the input value
                               Bit 7 - 0 indicates the value is in milliseconds
                                     - 1 indicates the value is in seconds
  @param[out]    *ReturnValue  pointer to 32 bit value in milliseconds

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
NvdimmNGetValueInMilliseconds (
  IN  UINT8   LsbValue,
  IN  UINT8   MsbValue,
  OUT UINT32  *ReturnValue
  );

#define NVDIMM_INIT_TIMEOUT 203

//
// Values from:
// JEDEC Byte Addressable Energy Backed Interface,
// December 2015
//
// Page 0 Register Map
//
#define OPEN_PAGE                  0x0

#define HOST_MAX_OPERATION_RETRY   0x15

#define ES_CHARGE_TIMEOUT0 0x10
#define ES_CHARGE_TIMEOUT1 0x11

//
// Empirically derived minimum required time for NVDIMM to complete initialization
// in Milliseconds
//
#define MIN_NVDIMM_INIT_TIME_MS 10000

typedef enum {
  MaxInitTimeout,
  RestoreTimeout,
  ArmTimeout,
  EraseTimeout,
  AbortCmdTimeout,
  FirmwareOpsTimeout,
  CSaveTimeout,
  EsChargeTimeout,
  PageSwitchLatencyTimeout
} NvdimmTimeoutType;

/**
  NvdimmNGetTimeout
  Returns a 32bit unsigned value in milliseconds based for an input timeout type:

  @param[in]  Socket            Socket number
  @param[in]  Ch                DDR channel number within the processor socket (0-based)
  @param[in]  Dimm              DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param[in]  NvdimmTimeoutType Type of Nvdimm Timeout (i.e. Max init timeout, restore timeout, etc...)
  @param[out] *MaxTime          pointer to a 32 bit value in milliseconds

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
NvdimmNGetTimeout (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  IN  NvdimmTimeoutType TimeoutType,
  OUT UINT32            *MaxTime
  );

/**
  Initializes the SMB_DEVICE_STRUCT for a given Nvdimm I2C device

  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Device  - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved SMBus address data
**/
EFI_STATUS
EFIAPI
NvdimmNInitSmbusDeviceStructure (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  OUT SMB_DEVICE_STRUCT  *Device
  );


/*

  Determines if the Dimm is a DDR4 Jedec specification defined NVDIMM-N type


  @param[in]                Socket              Socket under test (Based-0)
  @param[in]                Channel             Channel under test (Based-0)
  @param[in]                Dimm                Dimm under test (Based-0)

  @retval                   TRUE                Device is an NVDIMM-N type
  @retval                   FALSE               Any other Dimm type

*/
BOOLEAN
EFIAPI
DetectJedecDdr4NvdimmN (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm
  )
{
  UINT8   SPDReg;
  UINT16  Offset;
  UINT16  FuncDesc;
  BOOLEAN ReturnValue = FALSE;

  SpdReadByte (Socket, Channel, Dimm, SPD_KEY_BYTE2, &SPDReg);
  if ((SPDReg & (SPD_HYBRID_MASK + SPD_HYBRID_TYPE_MASK)) == (SPD_HYBRID_TYPE + SPD_HYBRID_TYPE_NVDIMM)) {
    for (Offset = SPD_NVDIMM_FN0_FID_LSB; Offset <= SPD_NVDIMM_FN7_FID_MSB; Offset++) {
      SpdReadByte (Socket, Channel, Dimm, Offset + 1, &SPDReg);
      FuncDesc = (UINT16)(SPDReg << 8);
      SpdReadByte (Socket, Channel, Dimm, Offset, &SPDReg);
      FuncDesc |= (UINT16)(SPDReg);

      if (((FuncDesc & SPD_NVDIMM_FI_MASK) == SPD_NVDIMM_STANDARD_INTERFACE_1) &&
         ((FuncDesc & SPD_NVDIMM_FC_MASK) == SPD_NVDIMM_BYTE_ADDR_ENERGY_BACKED)) {
        ReturnValue = TRUE;
        break;
      }
    }
  }
  return ReturnValue;
}


/**
  NvdimmNGetValueInMilliseconds
  Returns a 32bit unsigned value in milliseconds based on 2 bytes of input:

  @param[in]     LsbValue      the least significant byte of the input value
  @param[in]     MsbValue      Bits 6:0 are the most significant bits of the input value
                               Bit 7 - 0 indicates the value is in milliseconds
                                     - 1 indicates the value is in seconds
  @param[out]    *ReturnValue  pointer to 32 bit value in milliseconds

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
NvdimmNGetValueInMilliseconds (
  IN  UINT8   LsbValue,
  IN  UINT8   MsbValue,
  OUT UINT32  *ReturnValue
  )
{
  *ReturnValue = ((MsbValue & 0x7F) << 8) | LsbValue;
  if ((MsbValue & BIT7) != 0) {
    *ReturnValue = *ReturnValue * 1000;
  }
  return EFI_SUCCESS;
}

/**
  NvdimmNGetTimeout
  Returns a 32bit unsigned value in milliseconds based for an input timeout type:

  @param[in]  Socket            Socket number
  @param[in]  Ch                DDR channel number within the processor socket (0-based)
  @param[in]  Dimm              DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param[in]  NvdimmTimeoutType Type of Nvdimm Timeout (i.e. Max init timeout, restore timeout, etc...)
  @param[out] *MaxTime          pointer to a 32 bit value in milliseconds

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
NvdimmNGetTimeout (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  IN  NvdimmTimeoutType TimeoutType,
  OUT UINT32            *MaxTime
  )
{
  UINT8               SpdInitTimeValue = 0;
  EFI_STATUS          Status = EFI_SUCCESS;
  UINT8               TimeoutLsb;
  UINT8               TimeoutMsb;
  SMB_DEVICE_STRUCT   SmbusDevice;


  NvdimmNInitSmbusDeviceStructure (Socket, Channel, Dimm, &SmbusDevice);

  switch (TimeoutType) {
  case MaxInitTimeout:
    //
    // First get the maximum time to wait for an NVDIMM to be ready
    //
    Status = SpdReadByte (Socket, Channel, Dimm, SPD_NVDIMM_MAX_INIT_TIME, &SpdInitTimeValue);
    //
    //SPD byte is in seconds, but we need to convert to milliseconds
    //
    *MaxTime = SpdInitTimeValue * 1000;
    if (*MaxTime < MIN_NVDIMM_INIT_TIME_MS) {
      *MaxTime = MIN_NVDIMM_INIT_TIME_MS;
    }
    break;

  case PageSwitchLatencyTimeout:
    ReadProcSmb (SmbusDevice, PAGE_SWITCH_LATENCY0, &TimeoutLsb);
    ReadProcSmb (SmbusDevice, PAGE_SWITCH_LATENCY1, &TimeoutMsb);
    NvdimmNGetValueInMilliseconds (TimeoutLsb, TimeoutMsb, MaxTime);
    //
    // Put in a floor of at least 100 milliseconds if the value is less than that
    //
    if (*MaxTime < 100) {
      *MaxTime = 100;
    }
    break;

  case CSaveTimeout:
    ReadProcSmb (SmbusDevice, CSAVE_TIMEOUT0, &TimeoutLsb);
    ReadProcSmb (SmbusDevice, CSAVE_TIMEOUT1, &TimeoutMsb);
    NvdimmNGetValueInMilliseconds (TimeoutLsb, TimeoutMsb, MaxTime);
    break;

  case RestoreTimeout:
    ReadProcSmb (SmbusDevice, RESTORE_TIMEOUT0, &TimeoutLsb);
    ReadProcSmb (SmbusDevice, RESTORE_TIMEOUT1, &TimeoutMsb);
    NvdimmNGetValueInMilliseconds (TimeoutLsb, TimeoutMsb, MaxTime);
    break;

  case EsChargeTimeout:
    //
    //The ES_CHARGE_TIMEOUT value is in page 1.  Most NVDIMM code assumes page 0,
    //therefore this code will switch to page 1 and then restore page 0.
    //
    NvdimmNSwitchSmbusPage (Socket, Channel, Dimm, SmbusDevice, 1);
    ReadProcSmb (SmbusDevice, ES_CHARGE_TIMEOUT0, &TimeoutLsb);
    ReadProcSmb (SmbusDevice, ES_CHARGE_TIMEOUT1, &TimeoutMsb);
    //
    // The ES Charge Timeout always returns the value in seconds but some
    // SPD data may not have the MSB set to indicate such, so this code just
    // forces the MSB set such that the following code will always return
    // appropriate data in seconds
    //
    TimeoutMsb |= BIT7;
    NvdimmNGetValueInMilliseconds (TimeoutLsb, TimeoutMsb, MaxTime);
    NvdimmNSwitchSmbusPage (Socket, Channel, Dimm, SmbusDevice, 0);
    break;

  case EraseTimeout:
    ReadProcSmb (SmbusDevice, ERASE_TIMEOUT0, &TimeoutLsb);
    ReadProcSmb (SmbusDevice, ERASE_TIMEOUT1, &TimeoutMsb);
    NvdimmNGetValueInMilliseconds (TimeoutLsb, TimeoutMsb, MaxTime);
    break;

  case ArmTimeout:
    ReadProcSmb (SmbusDevice, ARM_TIMEOUT0, &TimeoutLsb);
    ReadProcSmb (SmbusDevice, ARM_TIMEOUT1, &TimeoutMsb);
    NvdimmNGetValueInMilliseconds (TimeoutLsb, TimeoutMsb, MaxTime);
    break;

  default:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "NvdimmNGetTimeout called for unsupported Timeout type, returning fixed default\n");
    *MaxTime = MIN_NVDIMM_INIT_TIME_MS;
  }

  return Status;
}

/**
  Initializes the SMB_DEVICE_STRUCT for a given Nvdimm I2C device

  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Device  - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved SMBus address data
**/
EFI_STATUS
EFIAPI
NvdimmNInitSmbusDeviceStructure (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  OUT SMB_DEVICE_STRUCT  *Device
  )
{
  //
  // Initialize structure for NVDIMM-N SMBUS device
  //
  Device->compId = SPD;
  Device->address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  Device->address.deviceType = DTI_NVDIMM_I2C;

  if (EFI_ERROR (GetSmbAddress (Socket, Channel, Dimm, Device))) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error retrieving DIMM SMBus address\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

/**
  Reads an SMBUS offset and stalls until the value is equal to the value provided
  on input, or until the input timeout is exceeded

  @param[in]     Socket        Socket number
  @param[in]     SmbusDev      Structure containing information about the
                               SMBUS device to be accessed
  @param[in]     ByteOffset    Smbus offset to read
  @param[in/out] *Data         Pointer to a location for reading the smbus data
  @param[in]     Mask          Byte to mask off bits to ignore in the compare
  @param[in]     ExpectedValue Value to compare
  @param[in]     Timeout       Time in milliseconds to wait for the desired value

  @retval EFI_DEVICE_ERROR if timeout occurs, else EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
PollForReadSmbValueWithTimeout (
  IN     UINT8             Socket,
  IN     SMB_DEVICE_STRUCT SmbusDev,
  IN     UINT8             ByteOffset,
  IN OUT UINT8             *Data,
  IN     UINT8             Mask,
  IN     UINT8             ExpectedValue,
  IN     UINT32            Timeout
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  PSYSHOST      Host;
  BOOLEAN       FirstPass = TRUE;
  UINT64        StartTsc;
  UINT64        CurrentTsc;
  BOOLEAN       TimedOut = FALSE;

  Host = GetSysHostPointer ();
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "    Starting a Poll Reg = 0x%x, Mask = 0x%x, Expected Value = 0x%x\n", ByteOffset, Mask, ExpectedValue);

  StartTsc = GetPerformanceCounter ();
  do {
    Status = ReadProcSmb (SmbusDev, ByteOffset, Data);

    if (FirstPass) {
      FirstPass = FALSE;
    } else {
      //
      // wait 1ms
      //
      FixedDelayMicroSecond (1000);
    }

    CurrentTsc = GetPerformanceCounter ();;
    if (TimeDiff (StartTsc, CurrentTsc, TDIFF_UNIT_MS) > Timeout) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " PollForReadSmbValueWithTimeout Timed out after %d ms, Reg = 0x%x, Mask = 0x%x, Expected Value = 0x%x, last read = 0x%x, last status = 0x%x\n", Timeout, ByteOffset, Mask, ExpectedValue, *Data, Status);
      TimedOut = TRUE;
      Status = EFI_DEVICE_ERROR;
    }
  } while ((((*Data & Mask) != ExpectedValue) || (Status != EFI_SUCCESS)) && (!TimedOut));
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "      Final Data=0x%x\n", *Data);
  return Status;
} // PollForReadSmbValueWithTimeout

/**
  Prints status registers for a JEDEC NVDIMM-N type DIMM

  @param[in]  Socket      Socket number
  @param[in]  Ch          Channel number
  @param[in]  Dimm        Dimm number
  @param[in]  SmbusDev    Structure containing information about
                          the SMBUS device to be accessed

  @retval EFI_SUCCESS

**/
EFI_STATUS
JEDECDumpStatusRegs (
  UINT8             Socket,
  UINT8             Channel,
  UINT8             Dimm,
  SMB_DEVICE_STRUCT SmbusDevice
  )
{
  EFI_STATUS  Status;
  UINT8       Data;

  Status = ReadProcSmb (SmbusDevice, NVDIMM_READY, &Data);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if (0xA5 != Data) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "NVDIMM_READY = 0x%x != 0xA5\n", Data);
  }
  Status = ReadProcSmb (SmbusDevice, NVDIMM_CMD_STATUS, &Data);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "NVDIMM_CMD_STATUS\n");
  }
  if ((Data & CONTROLLER_BUSY) == CONTROLLER_BUSY) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - Controller Busy\n");
  }
  if ((Data & FACTORY_DEFAULT_IN_PROGRESS) == FACTORY_DEFAULT_IN_PROGRESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - Factory Default In Progress\n");
  }
  if ((Data & SAVE_IN_PROGRESS) == SAVE_IN_PROGRESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - Save In Progress\n");
  }
  if ((Data & RESTORE_IN_PROGRESS) == RESTORE_IN_PROGRESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - Restore In Progress\n");
  }
  if ((Data & ERASE_IN_PROGRESS) == ERASE_IN_PROGRESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - Erase In Progress\n");
  }
  if ((Data & ABORT_IN_PROGRESS) == ABORT_IN_PROGRESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - Abort In Progress\n");
  }
  if ((Data & ARM_IN_PROGRESS) == ARM_IN_PROGRESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [6] - Arm In Progress\n");
  }
  if ((Data & FIRMWARE_OPS_IN_PROGRESS) == FIRMWARE_OPS_IN_PROGRESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [7] - FIRMWARE_OPS In Progress\n");
  }

  Status = ReadProcSmb (SmbusDevice, SAVE_STATUS, &Data);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SAVE_STATUS\n");
  }
  if ((Data & SAVE_SUCCESS) == SAVE_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - SAVE_SUCCESS\n");
  }
  if ((Data & SAVE_ERROR) == SAVE_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - SAVE_ERROR\n");
  }
  if ((Data & ABORT_SUCCESS) == ABORT_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - ABORT_SUCCESS\n");
  }
  if ((Data & ABORT_ERROR) == ABORT_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - ABORT_ERROR\n");
  }
  if ((Data & ((1 << 2) | (1 << 3) | (1 << 6) | (1 << 7))) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  0x%x[3:2,7:6] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, CSAVE_INFO, &Data);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "CSAVE_INFO\n");
  }
  if ((Data & NVM_DATA_VALID) == NVM_DATA_VALID) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - NVM_Data_Valid\n");
  }
  if ((Data & START_SAVE_CMD) == START_SAVE_CMD) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - START_SAVE_CMD\n");
  }
  if ((Data & SAVE_N) == SAVE_N) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - SAVE_N\n");
  }
  if ((Data & 0xF8) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:3] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, CSAVE_FAIL_INFO0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "CSAVE_FAIL_INFO0\n");
  }
  if ((Data & VOLTAGE_REGULATOR_FAILED) == VOLTAGE_REGULATOR_FAILED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - VOLTAGE_REGULATOR_FAILED\n");
  }
  if ((Data & VDD_LOST) == VDD_LOST) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - VDD_LOST\n");
  }
  if ((Data & VPP_LOST) == VPP_LOST) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - VPP_LOST\n");
  }
  if ((Data & VTT_LOST) == VTT_LOST) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - VTT_LOST\n");
  }
  if ((Data & DRAM_NOT_SELF_REFRESH) == DRAM_NOT_SELF_REFRESH) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - DRAM_NOT_SELF_REFRESH\n");
  }
  if ((Data & CONTROLLER_HARDWARE_ERROR) == CONTROLLER_HARDWARE_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - CONTROLLER_HARDWARE_ERROR\n");
  }
  if ((Data & NVM_CONTROLLER_ERROR) == NVM_CONTROLLER_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [6] - NVM_CONTROLLER_ERROR\n");
  }
  if ((Data & NVM_MEDIA_ERROR) == NVM_MEDIA_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [7] - NVM_MEDIA_ERROR\n");
  }

  Status = ReadProcSmb (SmbusDevice, CSAVE_FAIL_INFO1, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "CSAVE_FAIL_INFO1\n");
  }
  if ((Data & NOT_ENOUGH_ENERGY_FOR_CSAVE) == NOT_ENOUGH_ENERGY_FOR_CSAVE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - NOT_ENOUGH_ENERGY_FOR_CSAVE\n");
  }
  if ((Data & PARIAL_DATA_SAVED) == PARIAL_DATA_SAVED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - PARIAL_DATA_SAVED\n");
  }
  if ((Data & 0xFC) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:2] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, RESTORE_STATUS0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RESTORE_STATUS0\n");
  }
  if ((Data & RESTORE_SUCCESS) == RESTORE_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - RESTORE_SUCCESS\n");
  }
  if ((Data & RESTORE_ERROR) == RESTORE_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - RESTORE_ERROR\n");
  }
  if ((Data & ABORT_SUCCESS) == ABORT_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - ABORT_SUCCESS\n");
  }
  if ((Data & ABORT_ERROR) == ABORT_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - ABORT_ERROR\n");
  }
  if ((Data & 0xCC) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[3:2,7:6] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, ERASE_STATUS0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ERASE_STATUS0\n");
  }
  if ((Data & ERASE_SUCCESS) == ERASE_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - ERASE_SUCCESS\n");
  }
  if ((Data & ERASE_ERROR) == ERASE_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - ERASE_ERROR\n");
  }
  if ((Data & ABORT_SUCCESS) == ABORT_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - ABORT_SUCCESS\n");
  }
  if ((Data & ABORT_ERROR) == ABORT_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - ABORT_ERROR\n");
  }
  if ((Data & 0xCC) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[3:2,7:6] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, ARM_STATUS0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ARM_STATUS0\n");
  }
  if ((Data & ARM_SUCCESS) == ARM_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - ARM_SUCCESS\n");
  }
  if ((Data & ARM_ERROR) == ARM_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - ARM_ERROR\n");
  }
  if ((Data & SAVE_N_ARMED) == SAVE_N_ARMED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - SAVE_N_ARMED\n");
  }
  if ((Data & 0xF8) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:3] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, FACTORY_DEFAULT_STATUS0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "FACTORY_DEFAULT_STATUS0\n");
  }
  if ((Data & FACTORY_DEFAULT_SUCCESS) == FACTORY_DEFAULT_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - FACTORY_DEFAULT_SUCCESS\n");
  }
  if ((Data & FACTORY_DEFAULT_ERROR) == FACTORY_DEFAULT_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - FACTORY_DEFAULT_ERROR\n");
  }
  if ((Data & 0xFC) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:2] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, SET_EVENT_NOTIFICATION_STATUS0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SET_EVENT_NOTIFICATION_STATUS0\n");
  }
  if ((Data & SET_EVENT_NOTIFICATION_SUCCESS) == SET_EVENT_NOTIFICATION_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - SET_EVENT_NOTIFICATION_SUCCESS\n");
  }
  if ((Data & SET_EVENT_NOTIFICATION_ERROR) == SET_EVENT_NOTIFICATION_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - SET_EVENT_NOTIFICATION_ERROR\n");
  }
  if ((Data & PERSISTENCY_ENABLED) == PERSISTENCY_ENABLED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - PERSISTENCY_ENABLED\n");
  }
  if ((Data & WARNING_THRESHOLD_ENABLED) == WARNING_THRESHOLD_ENABLED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - WARNING_THRESHOLD_ENABLED\n");
  }
  if ((Data & VOLTAGE_REGULATOR_FAIL_ENABLED) == VOLTAGE_REGULATOR_FAIL_ENABLED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - VOLTAGE_REGULATOR_FAIL_ENABLED\n");
  }
  if ((Data & 0xE0) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:5] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, SET_ES_POLICY_STATUS, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SET_ES_POLICY_STATUS\n");
  }
  if ((Data & SET_ES_POLICY_SUCCESS) == SET_ES_POLICY_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - SET_ES_POLICY_SUCCESS\n");
  }
  if ((Data & SET_ES_POLICY_ERROR) == SET_ES_POLICY_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - SET_ES_POLICY_ERROR\n");
  }
  if ((Data & DEVICE_MANAGED_POLICY_ENABLED) == DEVICE_MANAGED_POLICY_ENABLED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - DEVICE_MANAGED_POLICY_ENABLED\n");
  }
  if ((Data & HOST_MANAGED_POLICY_ENABLED) == HOST_MANAGED_POLICY_ENABLED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - HOST_MANAGED_POLICY_ENABLED\n");
  }
  if ((Data & 0xF0) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:4] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, FIRMWARE_OPS_STATUS0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "FIRMWARE_OPS_STATUS0\n");
  }
  if ((Data & FIRMWARE_OPS_SUCCESS) == FIRMWARE_OPS_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - FIRMWARE_OPS_SUCCESS\n");
  }
  if ((Data & FIRMWARE_OPS_ERROR) == FIRMWARE_OPS_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - FIRMWARE_OPS_ERROR\n");
  }
  if ((Data & FIRMWARE_UPDATE_MODE) == FIRMWARE_UPDATE_MODE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - FIRMWARE_UPDATE_MODE\n");
  }
  if ((Data & ABORT_SUCCESS) == ABORT_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - ABORT_SUCCESS\n");
  }
  if ((Data & ABORT_ERROR) == ABORT_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - ABORT_ERROR\n");
  }
  if ((Data & 0xC8) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:6, 3] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, MODULE_HEALTH, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "MODULE_HEALTH\n");
  }
  if ((Data & PERSISTENCY_LOST_ERROR) == PERSISTENCY_LOST_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - PERSISTENCY_LOST_ERROR\n");
  }
  if ((Data & WARNING_THRESHOLD_EXCEEDED) == WARNING_THRESHOLD_EXCEEDED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - WARNING_THRESHOLD_EXCEEDED\n");
  }
  if ((Data & PERSISTENCY_RESTORED) == PERSISTENCY_RESTORED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - PERSISTENCY_RESTORED\n");
  }
  if ((Data & BELOW_WARNING_THRESHOLD) == BELOW_WARNING_THRESHOLD) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - BELOW_WARNING_THRESHOLD\n");
  }
  if ((Data & PERMANENT_HARDWARE_FAILURE) == PERMANENT_HARDWARE_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - PERMANENT_HARDWARE_FAILURE\n");
  }
  if ((Data & 0xF0) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7:4] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, MODULE_HEALTH_STATUS0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "MODULE_HEALTH_STATUS0\n");
  }
  if ((Data & VOLTAGE_REGULATOR_FAILED) == VOLTAGE_REGULATOR_FAILED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - VOLTAGE_REGULATOR_FAILED\n");
  }
  if ((Data & VDD_LOST) == VDD_LOST) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - VDD_LOST\n");
  }
  if ((Data & VPP_LOST) == VPP_LOST) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - VPP_LOST\n");
  }
  if ((Data & VTT_LOST) == VTT_LOST) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - VTT_LOST\n");
  }
  if ((Data & DRAM_NOT_SELF_REFRESH) == DRAM_NOT_SELF_REFRESH) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - DRAM_NOT_SELF_REFRESH\n");
  }
  if ((Data & CONTROLLER_HARDWARE_ERROR) == CONTROLLER_HARDWARE_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - CONTROLLER_HARDWARE_ERROR\n");
  }
  if ((Data & NVM_CONTROLLER_ERROR) == NVM_CONTROLLER_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [6] - NVM_CONTROLLER_ERROR\n");
  }
  if ((Data & NVM_LIFETIME_ERROR) == NVM_LIFETIME_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [7] - NVM_LIFETIME_ERROR\n");
  }

  Status = ReadProcSmb (SmbusDevice, MODULE_HEALTH_STATUS1, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "MODULE_HEALTH_STATUS1\n");
  }
  if ((Data & NOT_ENOUGH_ENERGY_FOR_CSAVE) == NOT_ENOUGH_ENERGY_FOR_CSAVE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - NOT_ENOUGH_ENERGY_FOR_CSAVE\n");
  }
  if ((Data & INVALID_FIRMWARE_ERROR) == INVALID_FIRMWARE_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - INVALID_FIRMWARE_ERROR\n");
  }
  if ((Data & CONFIG_DATA_ERROR) == CONFIG_DATA_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - CONFIG_DATA_ERROR\n");
  }
  if ((Data & NO_ES_PRESENT) == NO_ES_PRESENT) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - NO_ES_PRESENT\n");
  }
  if ((Data & ES_POLICY_NOT_SET) == ES_POLICY_NOT_SET) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - ES_POLICY_NOT_SET\n");
  }
  if ((Data & ES_HARDWARE_FAILURE) == ES_HARDWARE_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - ES_HARDWARE_FAILURE\n");
  }
  if ((Data & ES_HEALTH_ASSESSMENT_ERROR) == ES_HEALTH_ASSESSMENT_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [6] - ES_HEALTH_ASSESSMENT_ERROR\n");
  }
  if ((Data & 0x80) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  =0x%x[7] - Reserved\n", Data);
  }

  Status = ReadProcSmb (SmbusDevice, ERROR_THRESHOLD_STATUS, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ERROR_THRESHOLD_STATUS\n");
  }
  if ((Data & NVM_LIFETIME_ERROR1) == NVM_LIFETIME_ERROR1) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - NVM_LIFETIME_ERROR1\n");
  }
  if ((Data & ES_LIFETIME_ERROR) == ES_LIFETIME_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - ES_LIFETIME_ERROR\n");
  }
  if ((Data & ES_TEMP_ERROR) == ES_TEMP_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - ES_TEMP_ERROR\n");
  }

  Status = ReadProcSmb (SmbusDevice, WARNING_THRESHOLD_STATUS, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING_THRESHOLD_STATUS\n");
  }
  if ((Data & NVM_LIFETIME_WARNING) == NVM_LIFETIME_WARNING) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - NVM_LIFETIME_WARNING\n");
  }
  if ((Data & ES_LIFETIME_WARNING) == ES_LIFETIME_WARNING) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - ES_LIFETIME_WARNING\n");
  }
  if ((Data & ES_TEMP_WARNING) == ES_TEMP_WARNING) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - ES_TEMP_WARNING\n");
  }

  Status = ReadProcSmb (SmbusDevice, INJECT_OPS_FAILURES0, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "INJECT_OPS_FAILURES0\n");
  }
  if ((Data & INJECT_CSAVE_FAILURE) == INJECT_CSAVE_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - INJECT_CSAVE_FAILURE\n");
  }
  if ((Data & INJECT_RESTORE_FAILURE) == INJECT_RESTORE_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - INJECT_RESTORE_FAILURE\n");
  }
  if ((Data & INJECT_ERASE_FAILURE) == INJECT_ERASE_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - INJECT_ERASE_FAILURE\n");
  }
  if ((Data & INJECT_ARM_FAILURE) == INJECT_ARM_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - INJECT_ARM_FAILURE\n");
  }
  if ((Data & INJECT_INTERNAL_CONTROLLER_FAILURE) == INJECT_INTERNAL_CONTROLLER_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - INJECT_INTERNAL_CONTROLLER_FAILURE\n");
  }
  if ((Data & INJECT_NVM_LIFETIME_WARNING) == INJECT_NVM_LIFETIME_WARNING) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - INJECT_NVM_LIFETIME_WARNING\n");
  }
  if ((Data & INJECT_NVM_LIFETIME_ERROR) == INJECT_NVM_LIFETIME_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [6] - INJECT_NVM_LIFETIME_ERROR\n");
  }
  if ((Data & INJECT_BAD_BLOCKS) == INJECT_BAD_BLOCKS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [7] - INJECT_BAD_BLOCKS\n");
  }

  Status = ReadProcSmb (SmbusDevice, INJECT_ES_FAILURES, &Data);
  if (Data != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "INJECT_ES_FAILURES\n");
  }
  if ((Data & INJECT_ES_FAILURE) == INJECT_ES_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [0] - INJECT_ES_FAILURE\n");
  }
  if ((Data & INJECT_ES_ASSESEMENT_FAILURE) == INJECT_ES_ASSESEMENT_FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [1] - INJECT_ES_ASSESEMENT_FAILURE\n");
  }
  if ((Data & INJECT_ES_LIFETIME_WARNING) == INJECT_ES_LIFETIME_WARNING) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [2] - INJECT_ES_LIFETIME_WARNING\n");
  }
  if ((Data & INJECT_ES_LIFETIME_ERROR) == INJECT_ES_LIFETIME_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [3] - INJECT_ES_LIFETIME_ERROR\n");
  }
  if ((Data & INJECT_ES_TEMP_WARNING) == INJECT_ES_TEMP_WARNING) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [4] - INJECT_ES_TEMP_WARNING\n");
  }
  if ((Data & INJECT_ES_TEMP_ERROR) == INJECT_ES_TEMP_ERROR) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "  [5] - INJECT_ES_TEMP_ERROR\n");
  }

  return Status;
}


/**
  If page switch latency is not known, read from PAGE_SWITCH_LATENCY from SMBUS,
  else return page switch latency

  @param[in]  Socket              Socket number
  @param[in]  Channel             Channel number
  @param[in]  Dimm                Dimm number
  @param[in]  SmbusDev            Structure containing information about the
                                  SMBUS device to be accessed
  @param[out] *PageSwitchLatency  Pointer to UINT32 with the PageSwitchLatency for the NVDIMM

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
GetPageSwitchLatency (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  IN  SMB_DEVICE_STRUCT SmbusDev,
  OUT UINT32            *PageSwitchLatency
  )
{
  UINT8             CurrentPage;
  UINT8             Page = 0;

  //
  //Make sure we are on Page 0
  //
  ReadProcSmb (SmbusDev, OPEN_PAGE, &CurrentPage);

  // Avoid the Page Switch if we are already on Page 0
  if (CurrentPage != 0) {

    WriteProcSmb (SmbusDev, OPEN_PAGE, &Page);
    //
    // Here we don't know the page switch latency and we have to switch to page 0 to get it...so assume
    // a maximum of 5 seconds
    //
    PollForReadSmbValueWithTimeout (Socket, SmbusDev, OPEN_PAGE, &CurrentPage, Page, Page, 5000);
  }

  NvdimmNGetTimeout (Socket, Channel, Dimm, PageSwitchLatencyTimeout, PageSwitchLatency);

  return EFI_SUCCESS;
}


/**
  Sends SMBUS commands to switch to a given page.

  @param[in]  Socket      Socket number
  @param[in]  Channel     Channel number
  @param[in]  Dimm        Dimm number
  @param[in]  SmbusDev    Structure containing information about
                          the SMBUS device to be accessed
  @param[in]  Page        Desired open page

  @retval EFI_DEVICE_ERROR if current page is not desired page after
          switch.

**/
EFI_STATUS
NvdimmNSwitchSmbusPage (
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm,
  IN SMB_DEVICE_STRUCT SmbusDev,
  IN UINT8             Page
  )
{
  EFI_STATUS Status;
  UINT8      CurrentPage;
  UINT32     PageSwitchLatency;

  Status = ReadProcSmb (SmbusDev, OPEN_PAGE, &CurrentPage);

  // Avoid the Page Switch if we are already on the new Page
  if (CurrentPage != Page) {

    GetPageSwitchLatency (Socket, Channel, Dimm, SmbusDev, &PageSwitchLatency);

    WriteProcSmb (SmbusDev, OPEN_PAGE, &Page);

    Status = PollForReadSmbValueWithTimeout (Socket, SmbusDev, OPEN_PAGE, &CurrentPage, Page, Page, PageSwitchLatency);
    if (CurrentPage != Page) {
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
}

/**
  Checks status for a JEDEC NVDIMM-N type DIMM

  @param[in]  Socket      Socket number
  @param[in]  Ch          Channel number
  @param[in]  Dimm        Dimm number

  @retval NvdimmN specific status data

**/
UINT32
JEDECStatus (
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm
  )
{
  EFI_STATUS          Status;
  UINT32              InitTimeout;
  UINT32              SaveTimeout;
  UINT8               Data = 0;
  SMB_DEVICE_STRUCT   SmbusDevice;

  NvdimmNGetTimeout (Socket, Channel, Dimm, MaxInitTimeout, &InitTimeout);

  NvdimmNInitSmbusDeviceStructure (Socket, Channel, Dimm, &SmbusDevice);

  NvdimmNSwitchSmbusPage (Socket, Channel, Dimm, SmbusDevice, 0);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Waiting for NVDIMM to be ready\n");
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_READY, &Data, 0xA5, 0xA5, InitTimeout);
  if (Status != EFI_SUCCESS) {
    return NVDIMM_ERROR;
  }

  NvdimmNGetTimeout (Socket, Channel, Dimm, CSaveTimeout, &SaveTimeout);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Save timeout=%d ms\n", SaveTimeout);

  // Make sure there isn't a Backup in Progress
  Data = 0;
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, 0xFF, 0x0, SaveTimeout);
  if (Status != EFI_SUCCESS) {
    return NVDIMM_ERROR;
  }

  // Check to see if there is a backup to restore from
  Data = 0;
  Status = ReadProcSmb (SmbusDevice, SAVE_STATUS, &Data);
  if (Status != EFI_SUCCESS) {
    return NVDIMM_ERROR;
  }

  // SAVE_STATUS[0] - SAVE SUCCESS
  // SAVE_STATUS[1] - SAVE ERROR
  // SAVE_STATUS[4] - ABORT SUCESS
  // SAVE_STATUS[5] - ABORT ERROR
  // If there was a SAVE ERROR return NVDIMM_ERROR
  if ((Data & SAVE_ERROR) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Save Error. Status=0x%x\n", Data);
    JEDECDumpStatusRegs (Socket, Channel, Dimm, SmbusDevice);
    return NVDIMM_ERROR | ERROR_SAVE;
  }

  //  Check if there was a SAVE operation. Bit0 is set to 1 if prevous save operation completed successfully
  if ((Data & SAVE_SUCCESS) == 0) {
    return NVDIMM_NODATA;
  }

  // Verify there is a valid save image
  Data = 0;
  Status = ReadProcSmb (SmbusDevice, CSAVE_INFO, &Data);
  if (Status != EFI_SUCCESS) {
    return NVDIMM_ERROR;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CSAVE_INFO=0x%x\n", Data);

  // CSAVE_INFO[0]-NVM_Data_Valid
  // CSAVE_INFO[1]-START_SAVE_CMD
  // CSAVE_INFO[2]-SAVE_N
  // CSAVE_INFO = 0x % x[7:3] - reserved
  if ((Data & NVM_DATA_VALID) != 0) {
    return NVDIMM_RESTORE;
  } else {
    JEDECDumpStatusRegs (Socket, Channel, Dimm, SmbusDevice);
    Data = CL_SAVE_STAT;
    Status = WriteProcSmb (SmbusDevice, NVDIMM_MGT_CMD0, &Data);
    return NVDIMM_ERROR | ERROR_RESTORE;
  }
}

/**
  Start a RESTORE operation for a JEDEC NVDIMM-N type DIMM

  @param[in]  Socket      Socket number
  @param[in]  Ch          Channel number
  @param[in]  Dimm        Dimm number

  @retval EFI_DEVICE_ERROR if an error occurs, else EFI_SUCCESS

**/
EFI_STATUS
JEDECRestoreStart (
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm
  )
{
  EFI_STATUS          Status;
  UINT8               Data = 0;
  UINT32              Timeout;
  SMB_DEVICE_STRUCT   SmbusDevice;

  //
  // The NVDIMM-N Jedec Spec does not specify the maximum time to wait for the controller to
  // be not busy, but only that firmware must wait.  This code assumes that the maximum
  // initialization time in SPD is a worst case time to wait.
  //
  NvdimmNGetTimeout (Socket, Channel, Dimm, MaxInitTimeout, &Timeout);

  NvdimmNInitSmbusDeviceStructure (Socket, Channel, Dimm, &SmbusDevice);

  //1.  NVDIMM_CMD_STATUS0 : wait for Controller NOT busy.
  // Wait for the NVDIMM to be Ready
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Wait for Nvdimm-N to be idle\n");
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, CONTROLLER_BUSY, 0x0, Timeout);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  //2.  NVDIMM_MGT_CMD0 : set CL_RESTORE_STAT to clear restore status register.
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Clearing Restore Status\n");
  Data = CL_RESTORE_STAT;
  Status = WriteProcSmb (SmbusDevice, NVDIMM_MGT_CMD0, &Data);

  // 3.  NVDIMM_FUNC_CMD0 : START_RESTORE
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Starting Restore\n");
  Data = START_RESTORE;
  Status = WriteProcSmb (SmbusDevice, NVDIMM_FUNC_CMD0, &Data);

  //4.  NVDIMM_CMD_STATUS0 : check for RESTORE in progress bit to be set.If not set, check for RESTORE to be done in RESTORE_STATUS0.If restore not done, retry step 3.
  Data = 0;
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, 0xFF, (CONTROLLER_BUSY | RESTORE_IN_PROGRESS), Timeout); // Controller BUSY and Restore in Progress bits
  if ((Data & RESTORE_IN_PROGRESS) == 0) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RESTORE in progress bit NOT set after %d ms\n", Timeout);
  }

  return Status;
}


/**
  Wait for in progress RESTORE operations for a JEDEC NVDIMM-N type DIMM to complete

  @param[in]  Socket      Socket number
  @param[in]  Ch          Channel number
  @param[in]  Dimm        Dimm number

  @retval EFI_DEVICE_ERROR if an error occurs, else EFI_SUCCESS

**/
EFI_STATUS
JEDECRestoreDone (
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm
  )
{
  EFI_STATUS          Status;
  UINT8               Data = 0;
  UINT32              Timeout;
  UINT8               CurrentAttempt;
  UINT8               MaximumRetries;
  SMB_DEVICE_STRUCT   SmbusDevice;

  NvdimmNInitSmbusDeviceStructure (Socket, Channel, Dimm, &SmbusDevice);

  Status = ReadProcSmb (SmbusDevice, HOST_MAX_OPERATION_RETRY, &Data);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  //
  // Maximum Restore Retry Count is in bits 3:2 of HOST_MAX_OPERATION_RETRY
  //
  MaximumRetries = Data >> 2;
  MaximumRetries &= 0x3;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "NVDIMM allows for %d retries\n", MaximumRetries);

  NvdimmNGetTimeout (Socket, Channel, Dimm, RestoreTimeout, &Timeout);

  for (CurrentAttempt = 0; CurrentAttempt < MaximumRetries; CurrentAttempt++) {

    if (CurrentAttempt != 0) {
      JEDECRestoreStart (Socket, Channel, Dimm);
    }

    if (Timeout == 0) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Restore timeout=%d\n Wait for Restore to Complete\n", Timeout);

    // 5.  NVDIMM_CMD_STATUS0 : wait for RESTORE in progress bit to be clear.If RESTORE in progress bit is not clear after RESTORE_TIMEOUT, abort the RESTORE(see 9.6).
    Data = 0;
    Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, 0xFF, 0x0, Timeout);
    if (Status != EFI_SUCCESS) {
      continue;
    }

    //  6.  RESTORE_STATUS0 : check for either RESTORE successful or error.If error or RESTORE aborted, Host may retry up to HOST_MAX_OPERATION_RETRY.
    Data = 0;
    Status = ReadProcSmb (SmbusDevice, RESTORE_STATUS0, &Data);
    if (Status != EFI_SUCCESS) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Restore Status=0x%x\n", Data);

    if ((Data & RESTORE_SUCCESS) != 0) {
      return EFI_SUCCESS;
    } else {
      continue;
    }
  } //CurrentAttempt

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Restore Failed after %d attempts\n", CurrentAttempt);
  JEDECDumpStatusRegs (Socket, Channel, Dimm, SmbusDevice);
  return EFI_DEVICE_ERROR;
}


/**
  Check and perform any required operations such that the Nvdimm-N dimm is ARMed

  @param[in]  Socket      Socket number
  @param[in]  Ch          Channel number
  @param[in]  Dimm        Dimm number

  @retval EFI_DEVICE_ERROR if an error occurs, else EFI_SUCCESS

**/
EFI_STATUS
JEDECArm (
  UINT8             Socket,
  UINT8             Channel,
  UINT8             Dimm
  )
{
  EFI_STATUS Status;
  UINT8               Data = 0;
  UINT8               PolicyEnabling = 0;
  UINT32              Timeout;
  SYS_SETUP           *Setup;
  SMB_DEVICE_STRUCT   SmbusDevice;

  Setup = GetSysSetupPointer ();

  NvdimmNInitSmbusDeviceStructure (Socket, Channel, Dimm, &SmbusDevice);
  //
  // Ensure page 0 is active
  //
  Status = NvdimmNSwitchSmbusPage (Socket, Channel, Dimm, SmbusDevice, 0);
  //
  //9.5 ERASE WORKFLOW
  //
  //To initiate an erase, the Host shall do the following :
  //1.  NVDIMM_CMD_STATUS0 : wait for Controller NOT busy.
  // Wait for the NVDIMM to be Ready
  //

  //
  // The NVDIMM-N Jedec Spec does not specify the maximum time to wait for the controller to
  // be not busy, but only that firmware must wait.  This code assumes that the maximum
  // initialization time in SPD is a worst case time to wait.
  //
  NvdimmNGetTimeout (Socket, Channel, Dimm, MaxInitTimeout, &Timeout);
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, CONTROLLER_BUSY, 0x0, Timeout);
  if (Status != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Setting Energy Policy to ");
  switch (Setup->mem.NvDimmEnergyPolicy) {
    case ENERGY_POLICY_DEVICE_MANAGED:
      Data = DEVICE_MANAGED_POLICY;
      PolicyEnabling = DEVICE_MANAGED_POLICY_ENABLED;
      RcDebugPrint (SDBG_MAX, "Device-Managed\n");
      break;
    case ENERGY_POLICY_HOST_MANAGED:
      Data = HOST_MANAGED_POLICY;
      PolicyEnabling = HOST_MANAGED_POLICY_ENABLED;
      RcDebugPrint (SDBG_MAX, "Host-Managed\n");
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_185);
  }
  Status = WriteProcSmb (SmbusDevice, SET_ES_POLICY_CMD, &Data);

  Data = 0;
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, 0xFF, 0x0, Timeout);

  Status = ReadProcSmb (SmbusDevice, SET_ES_POLICY_STATUS, &Data);
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, SET_ES_POLICY_STATUS, &Data, PolicyEnabling, PolicyEnabling, Timeout);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SET_ES_POLICY_STATUS=0x%x\n", Data);

  NvdimmNGetTimeout (Socket, Channel, Dimm, EsChargeTimeout, &Timeout);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Charge Timeout=0x%x\n", Timeout);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Waiting for the ES to charge\n");
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, MODULE_HEALTH_STATUS1, &Data, 1, 0, Timeout);

  NvdimmNGetTimeout (Socket, Channel, Dimm, EraseTimeout, &Timeout);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Erase Timeout=0x%x\n", Timeout);

  //2.  NVDIMM_MGT_CMD0 : set CL_ERASE_STAT to clear restore status register.
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Clearing Erase Status\n");
  Data = CL_ERASE_STAT;
  Status = WriteProcSmb (SmbusDevice, NVDIMM_MGT_CMD0, &Data);

  //3.  NVDIMM_FUNC_CMD0 : START_ERASE
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Start Erase Command\n");
  Data = START_ERASE;
  Status = WriteProcSmb (SmbusDevice, NVDIMM_FUNC_CMD0, &Data);

  //4.  NVDIMM_CMD_STATUS0 : check for ERASE in progress bit to be set.
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Waiting for Erase to Start\n");

  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, ERASE_IN_PROGRESS, ERASE_IN_PROGRESS, Timeout);

  //5.  NVDIMM_CMD_STATUS0 : wait for ERASE in progress bit to be clear.If the ERASE in progress bit is not clear after ERASE_TIMEOUT, abort the ERASE(see 9.6).
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Waiting for Erase to Finish\n");
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, 0xFF, 0x0, Timeout);
  if (Status != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }

  //6.  ERASE_STATUS0 : check for either ERASE successful or error.If error or ERASE aborted, Host may retry up to HOST_MAX_OPERATION_RETRY.
  Status = ReadProcSmb (SmbusDevice, ERASE_STATUS0, &Data);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ERASE_STATUS0=0x%x\n", Data);

  if ((Data & ERASE_SUCCESS) != ERASE_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Erase Operation Not Successful\n", Data);
    return EFI_DEVICE_ERROR;
  }

  //9.4 ARM WORKFLOW
  NvdimmNGetTimeout (Socket, Channel, Dimm, ArmTimeout, &Timeout);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ARM Timeout=0x%x\n", Timeout);

  //  To initiate an arm operation, the Host shall do the following :
  //1.  NVDIMM_CMD_STATUS0 : wait for Controller NOT busy.
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, CONTROLLER_BUSY, 0x0, Timeout);

  //2.  NVDIMM_MGT_CMD0 : set CL_ARM_STAT to clear arm status register.
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Clear ARM status\n", Timeout);
  Data = CL_ARM_STAT;
  WriteProcSmb (SmbusDevice, NVDIMM_MGT_CMD0, &Data);

  //3.  ARM_CMD : Set the appropriate bits to reflect the save triggers to be enabled.
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Start ARM operation\n");
  Data = ARM_FOR_SAVE_N;
  WriteProcSmb (SmbusDevice, ARM_CMD, &Data);

  //4.  ARM_STATUS0 : check for either ARM successful or error.If successful, the triggers that are enabled for catastrophic save shall be set.

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Waiting for Arm to Complete\n");
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_CMD_STATUS, &Data, 0xFF, 0x0, Timeout);
  if (Status != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }

  Status = ReadProcSmb (SmbusDevice, MODULE_HEALTH, &Data);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "MODULE_HEALTH=0x%x\n", Data);

  Status = ReadProcSmb (SmbusDevice, MODULE_HEALTH_STATUS0, &Data);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "MODULE_HEALTH_STATUS0=0x%x\n", Data);

  Status = ReadProcSmb (SmbusDevice, MODULE_HEALTH_STATUS1, &Data);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "MODULE_HEALTH_STATUS1=0x%x\n", Data);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Check Arm status\n");
  Status = PollForReadSmbValueWithTimeout (Socket, SmbusDevice, ARM_STATUS0, &Data, (ARM_SUCCESS | SAVE_N_ARMED), (ARM_SUCCESS | SAVE_N_ARMED), Timeout);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ARM_STATUS0=0x%x\n", Data);

  if ((Data & ARM_SUCCESS) == ARM_SUCCESS) {
    return EFI_SUCCESS;
  } else {
    return EFI_DEVICE_ERROR;
  }
}

/**

  Issue ARM command of 0 to NVDIMM_N (disabling the ARM trigger)

  @param[in] Socket  Processor socket within the system (0-based)
  @param[in] Ch      DDR channel number within the processor socket (0-based)
  @param[in] Dimm    DIMM number within a DDR channel (0-based, farthest slot == 0)

**/
EFI_STATUS
EFIAPI
UnarmNvdimm (
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm
  )
{
  SMB_DEVICE_STRUCT SmbusDevice;
  UINT8             Data = 0;
  UINT32            Timeout;

  NvdimmNGetTimeout (Socket, Channel, Dimm, MaxInitTimeout, &Timeout);

  NvdimmNInitSmbusDeviceStructure (Socket, Channel, Dimm, &SmbusDevice);

  NvdimmNSwitchSmbusPage (Socket, Channel, Dimm, SmbusDevice, 0);
  // Wait for the NVDIMM to be Ready
  if (EFI_ERROR (PollForReadSmbValueWithTimeout (Socket, SmbusDevice, NVDIMM_READY, &Data, 0xA5, 0xA5, Timeout))) {
    return EFI_DEVICE_ERROR;
  }

  //Disable any previous NVDIMM arm command in ARM_CMD register
  Data = 0;
  WriteProcSmb (SmbusDevice, ARM_CMD, &Data);

  return EFI_SUCCESS;
}

/**
  Prepare Nvdimm-N (Jedec Standard) Dimms for training.  Disable Arm triggers and collect status.

  @param[in]Host             - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS
**/
UINT32
PrepareNvdimmNForTraining (
  IN PSYSHOST Host   // Pointer to Host structure
  )
{
  UINT8               Socket;
  UINT8               Channel;
  UINT8               Dimm;
  UINT32              Status;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  if (!(PcdGetBool (PcdNvDimmEn))) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Issue Unarm command to any present Jedec NVDIMM-N type dimms on a normal cold boot
  //
  if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if ((*ChannelNvList)[Channel].enabled == 0) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Channel);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        if ((*DimmNvList)[Dimm].NvDimmNPresent) {
          UnarmNvdimm (Socket, Channel, Dimm);
        }
      }//Dimm
    }//Channel
  }//

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((*DimmNvList)[Dimm].NvDimmNPresent) {

        (*DimmNvList)[Dimm].NvDimmStatus = 0;

        // Check if the NVDIMM has data to restore
        Status = JEDECStatus (Socket, Channel, Dimm);

        switch (Status & 0x0F) {
        case NVDIMM_RESTORE:
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM HAS data to restore\n");
          (*DimmNvList)[Dimm].NvDimmStatus |= STATUS_RESTORE_NEEDED;
          break;

        case NVDIMM_NODATA:
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM does NOT have data to restore\n");
          break;

        case NVDIMM_ERROR:
        default:
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM in ERROR condition (status = %08x)\n", Status);
          (*DimmNvList)[Dimm].NvDimmStatus |= ERROR_DETECT | (Status & ~(0x0F));
          break;
        }
      }
    } // Dimm loop
  } // Channel loop
  return SUCCESS;
}

/**
  Restore Nvdimm-N (Jedec Standard).  Check and perform any required restore operations for Nvdimm-N dimms present.

  @param[in]Host          - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS
**/
UINT32
CoreRestoreNVDIMMs (
  IN PSYSHOST Host
  )
{
  UINT8               Socket;
  UINT8               Channel;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Nv_ckeMask = 0;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  if (!(PcdGetBool (PcdNvDimmEn))) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if (!(Setup->mem.restoreNVDIMMS && (Setup->mem.ADRDataSaveMode == ADR_NVDIMM))) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // First Loop to start the restore on all NVDIMMs
  //
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++)    {
      Nv_ckeMask = 0;
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((*DimmNvList)[Dimm].NvDimmNPresent) {
        RankList = &(*DimmNvList)[Dimm].rankList;
        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (CheckRank(Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          Nv_ckeMask |= 1 << (*RankList)[Rank].CKEIndex;
        } // Rank loop

        if ((*DimmNvList)[Dimm].NvDimmStatus & STATUS_RESTORE_NEEDED) {

          (*DimmNvList)[Dimm].NvDimmStatus &= ~(STATUS_RESTORE_NEEDED);

          SetCkeMask (Host, Socket, Channel, Nv_ckeMask, 0);

          Host->var.mem.subBootMode = NvDimmResume;
          if (EFI_ERROR (JEDECRestoreStart (Socket, Channel, Dimm))) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    ERROR starting NVDIMM Recovery\n");
            (*DimmNvList)[Dimm].NvDimmStatus |= ERROR_RESTORE;
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    Started NVDIMM Recovery\n");
            (*DimmNvList)[Dimm].NvDimmStatus |= STATUS_RESTORE_INPROGRESS;
          }
        } // STATUS_RESTORE_NEEDED
      } //NvDimmNPresent
    } // Dimm loop
  } // Channel loop

  // Second Loop is to verify that all NVDIMMs have finished their Restore
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++)   {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if (((*DimmNvList)[Dimm].NvDimmNPresent) && ((*DimmNvList)[Dimm].NvDimmStatus & STATUS_RESTORE_INPROGRESS)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Calling JEDEC restoreDone\n");
        (*DimmNvList)[Dimm].NvDimmStatus &= ~(STATUS_RESTORE_INPROGRESS);
        if (EFI_ERROR (JEDECRestoreDone (Socket, Channel, Dimm))) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    ERROR during NVDIMM Recovery\n");
          (*DimmNvList)[Dimm].NvDimmStatus |= ERROR_RESTORE;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    Recovery Successful\n");
          (*DimmNvList)[Dimm].NvDimmStatus |= STATUS_RESTORE_SUCCESSFUL;
        }
      }
    } // Dimm loop
  } // Channel loop
  return SUCCESS;
} // PlatformRestoreNVDIMMs

/**

  Arm Nvdimm-N (Jedec Standard).  Check and perform any required operations
  for Nvdimm-N dimms present such that dimms are ARMed

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CoreArmNVDIMMs (
  IN PSYSHOST Host
  )
{
  UINT8               Socket;
  UINT8               Channel;
  UINT8               Dimm;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  SYS_SETUP           *Setup;
  UINT8     MaxChDdr;

  if (!(PcdGetBool (PcdNvDimmEn))) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if (!(Setup->mem.eraseArmNVDIMMS && (Setup->mem.ADRDataSaveMode == ADR_NVDIMM))) {
    return 0;
  }
  Socket = Host->var.mem.currentSocket;

  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((*DimmNvList)[Dimm].NvDimmNPresent) {
        // Check if the NVDIMM has data to restore
        if (EFI_ERROR (JEDECArm (Socket, Channel, Dimm))) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    ERROR Arming NVDIMM\n");
          (*DimmNvList)[Dimm].NvDimmStatus |= ERROR_ARM;
        } else {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    NVDIMM armed\n");
          (*DimmNvList)[Dimm].NvDimmStatus |= STATUS_ARMED;
        }
      }
    } // Dimm loop
  } // Channel loop
  return SUCCESS;
}

