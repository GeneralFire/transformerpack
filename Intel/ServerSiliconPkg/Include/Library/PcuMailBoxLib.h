/** @file
  Interface header for Bios to Pcode Mailbox library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _BIOS_TO_PCODE_MAILBOX_LIB_H_
#define _BIOS_TO_PCODE_MAILBOX_LIB_H_

#if defined (SKX_HOST) || defined (CPX_CPU) || defined (CPX_HOST) || defined (CLX_HOST)
#include "PcuMailBoxSkx.h"
#elif defined (ICX_HOST) || defined (CPX_PCIE_GEN4)                        // 10nm wave 1
#include "PcuMailBox10nmWave1.h"
#elif defined (SNR_HOST) || defined (ICXD_HOST) || defined (ICXDE_HOST)    // 10nm wave 2
#include "PcuMailBox10nmWave1.h"
#elif defined (SPR_HOST)                                                   // 10nm wave 3
#include "PcuMailBox10nmWave3.h"
#elif defined (GNRSRF_HOST)
#include "PcuMailBox10nmWave3.h"                                           // using wave3 version for now until Wave4 header is ready
#endif

#define SNC_INDEX_CONFIG        MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_SNC_CONFIG
#define SNC_INDEX_BASE_1        MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_SNC_BASE_1
#define SNC_INDEX_BASE_2        MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_SNC_BASE_2
#define SNC_INDEX_BASE_3        MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_SNC_BASE_3
#define SNC_INDEX_BASE_4        MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_SNC_BASE_4
#define SNC_INDEX_BASE_5        MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_SNC_BASE_5
#define SNC_INDEX_UPPER_BASE    MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_SNC_UPPER_BASE

//
// Define for invalid frequency. Used when unsupported\invalid frequency value is returned
//
#define PCU_INVALID_FREQUENCY 0xFF

//
// BIOS-to-PCODE Mailbox Encodings not yet included in auto-generated B2P_Mailbox.h by default.
//
// Used with B2P cmd 0x04
//
#define MAILBOX_BIOS_DATA_VR_REG_PROTOCOL_ID        0x05  // Protocol ID
#define MAILBOX_BIOS_VR_CMD_GET_REG                 0x07  // GetRegs VR Command
//
// Used with B2P cmd 0xA6 (MAILBOX_BIOS_CMD_SET_MC_FREQ)
//
#define MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MC_RATIO_NO_DIMMS         0xA // Avoids a false UNSUPPORTED_MC_FREQ error
#define MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MC_REF_CLK_TYPE_NO_DIMMS  0x0
#define MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_NONE          0x0
#define MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MC_INSTANCE_NONE          0x0
//
// Used with B2P cmd 0x7C (MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE).
//
#define MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_IOMT_WORKAROUND_ENABLE 4
#define MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_WATCHDOG_TIMER_DISABLE 2
#define MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_CFC_GV_DISABLE         1

//
// BIOS to PCode Mailbox CSR Registers
//
#ifdef GNRSRF_HOST
#define BIOS_MAILBOX_DATA       BIOS_MAILBOX_DATA_CFG_PCU_FUN0_REG
#define BIOS_MAILBOX_INTERFACE  BIOS_MAILBOX_INTERFACE_CFG_PCU_FUN0_REG
#else // GNRSRF_HOST
#define BIOS_MAILBOX_DATA       BIOS_MAILBOX_DATA_PCU_FUN1_REG
#define BIOS_MAILBOX_INTERFACE  BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG
#endif // GNRSRF_HOST

//
// DfxB2PErrAssertAtCommand:
//  0xFF: Assert for all the failed B2P Mailbox Commands
//  0x0: Do not assert on any failed B2P Mailbox command
//  The Other Command: Only assert when B2P Mailbox command (DfxB2PErrAssertAtCommand) is failed
//
#define B2P_ERROR_ASSERT_ON_ALL_COMMAND   0xFF
#define B2P_ERROR_ASSERT_ON_NO_COMMAND    0x0

//
//
//
typedef enum {
  SPD_SMBUS_ACCESS_LOCK = 0, ///> Enum for locking smbus access
  SPD_SMBUS_ACCESS_UNLOCK    ///> Enum value for unlocking smbus access
} SPD_SMBUS_ACCESS_TYPE;

//
// Memory Thermal Throttling Type
//
typedef enum {
  PCU_THROTTLE_CLTT,
  PCU_THROTTLE_CLTT_PECI,
  PCU_THROTTLE_MAX = 0xff
  } PCU_THROTTLE_TYPE;

//
// Memory Type enum for PCU Commands
//
typedef enum {
  PCU_MEM_NONE,
  PCU_MEM_DDR4,
  PCU_MEM_DDR5,
  PCU_MEM_HBM,
  PCU_MEM_DDRT,
  PCU_MEM_MAX = 0xff
  } PCU_MEM_TYPE;

//
// Surprise reset type
//
typedef enum {
  PCU_LEGACY_RST,
  PCU_ADR_RST,
  PCU_VIRAL_RST,
  PCU_BOTH_RST,
  PCU_RST_MAX = 0xff
  } PCU_RST_TYPE;

#define WARM_RESET_SURPRIZRST_MODE_LEGACY 0  // Legacy surprise reset mode
#define WARM_RESET_SURPRIZRST_MODE_ADR    1  // Treat surprise reset as an ADR equivalent reset
#define WARM_RESET_SURPRIZRST_MODE_VIRAL  2  // Treat surprise reset as a viral type reset to attempt to contain errors
#define WARM_RESET_SURPRIZRST_MODE_BOTH   3  // Best effort to do both ADR and viral behaviors for surprise reset
#define SurprizRst2Adr_Counter         0x14  // Each unit corresponds to 5.12uSec

#define BW_POWER_SCALE_FACTOR 0   // Bandwidth power scale factor selector value
#define BW_POWER_MINIMUM_POWER 1  // Bandwidth Minimum power selector value

#define DDR_POWER_SCALE_CONVERSION_FACTOR 0   // Conversion Factor
#define DDR_POWER_SCALE_IDLE_POWER 1          // Idle Power

//
// Bitmask definition to indicate which BGF pointer separation need to be programmed
//
#define PTRSEP_OVERRIDE_BGF_M3KTI_KTI     0x1
#define PTRSEP_OVERRIDE_BGF_M2PCIE        0x2
#define PTRSEP_OVERRIDE_BGF_M2M           0x4

//
// PI5 IP disable config field specifies.
//
#define PI5_IP_DISABLE_CONFIG_DMI_PCIEGEN4_ONLY_VALUE         0x00  // Both disabled (neither IAL nor Gen5) i.e. DMI or Gen4 only
#define PI5_IP_DISABLE_CONFIG_SINGLE_PROTOCOL_VALUE           0x01  // DMI, PCIE_gen4/gen5 or IAL.IO single protocol
#define PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE             0x02  // All capabilities i.e. DMI, Gen4 or Gen5 or IAL mutiprotocol
#define PI5_IP_DISABLE_CONFIG_INVALID_VALUE                   0xFF  // Invalid data

//
// B2P Mailbox command SET_UPI_FREQ
// Interface [11:11] - Trigger Bit (0 - don't trigger GV, 1 - Trigger GV)
//           [12:12] - Mode (0 - Slow mode, 1 - Fast Mode)
//
#define FAST_MODE_OF_SET_UPI_FREQ         0x1
#define SLOW_MODE_OF_SET_UPI_FREQ         0
#define TRIGGER_BIT_OF_SET_UPI_FREQ       0x1

//
// define the UPI ratio for pcode command
//
#define UPI_RATIO_FOR_SPEED_REC_96GT   12       // 9.6G / 0.8G = 12
#define UPI_RATIO_FOR_SPEED_REC_104GT  13       // 10.4G / 0.8G = 13
#define UPI_RATIO_FOR_SPEED_REC_112GT  14       // 11.2G / 0.8G = 14
#define UPI_RATIO_FOR_SPEED_REC_128GT  16       // 12.8G / 0.8G = 16
#define UPI_RATIO_FOR_SPEED_REC_144GT  18       // 14.4G / 0.8G = 18
#define UPI_RATIO_FOR_SPEED_REC_160GT  20       // 16.0G / 0.8G = 20

/**
  This function provides capability of either reading BIOS-to-PCode command,
  writing BIOS-to-PCode command or doing read-modify-write BIOS-to-Pcode
  commands.

  For read capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  2. Read and return the data from BIOS_MAILBOX_DATA CSR

  For write capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "write-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR

  For read-modify-write capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  2. Read the data from BIOS_MAILBOX_DATA CSR
  3. Modify the said returned data with the mask provided by the user
  4. Write the data back to BIOS_MAILBOX DATA CSR and
     write "write-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  5. Write the data to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  6. Read and return the data from BIOS_MAILBOX_DATA CSR

  @param[in]      Socket                CPU Socket Node number (Socket ID)
  @param[in]      DwordReadCommand      "Read-type" BIOS-to-PCode mailbox command to write into
                                        BIOS_MAILBOX_INTERFACE CSR
  @param[in]      DwordWriteCommand     "Write-type" BIOS-to-PCode mailbox command to write into
                                        BIOS_MAILBOX_INTERFACE CSR
  @param[in, out] Dworddata             As input, the data to Pcode by writing to BIOS_MAILBOX_DATA
                                        CSR. As output, the data returned from Pcode by reading
                                        from BIOS_MAILBOX_DATA CSR.
  @param[in]      DwordMaskValue        To clear specific bits in the data returned from Pcode
  @param[in]      DwordOrdata           To set specific bits in the data returned from Pcode

  @retval Error code from the PCU mailbox (0 = NO ERROR)

**/
UINT32
EFIAPI
ReadModifyWritePcuMailbox (
  IN     UINT8      Socket,
  IN     UINT32     DwordReadCommand,
  IN     UINT32     DwordWriteCommand,
  IN OUT UINT32     *DwordData,
  IN     UINT32     DwordMaskValue,
  IN     UINT32     DwordOrValue
  );

//
// Function pointer definiton for ReadModifyWritePcuMailbox()
//
typedef
UINT32
(EFIAPI *READ_MODIFY_WRITE_PCU_MAILBOX) (
  IN  UINT8     Socket,
  IN  UINT32    DwordReadCommand,
  IN  UINT32    DwordWriteCommand,
  OUT UINT32    *DwordData,
  IN  UINT32    DwordMaskValue,
  IN  UINT32    DwordOrValue
  );

/**
  This function returns BOOLEAN to indicate if the given BIOS - to - Pcode mailbox command
  is enabled or disabled in DFX setup knob.

  @param[in]  CommandByte         8 - bit BIOS - to - Pcode mailbox command(see B2P_Mailbox.h)

  @retval TRUE    The given BIOS - to - Pcode mailbox command is enabled in the setup knob.
  @retval FALSE   The given BIOS - to - Pcode mailbox command is disabled in the setup knob.
**/

BOOLEAN
EFIAPI
IsBios2PcuMailboxCommandEnabled (
  IN UINT8       CommandByte
  );

/**
  Clear the RUN_BUSY flag by performing a UsraCsrRead() poll with
  a timeout of 5 Milli Seconds.

  @param socket        - CPU Socket Node number (Socket ID)
  @param BoxInst       - Box Instance, 0 based
  @param RegOffset     - Register offset

  @retval If error return the value of the last read. If success return 0;
**/

UINT32
EFIAPI
ClearRunBusyRegFlag (
  UINT8    socket,
  UINT8    BoxInst,
  UINT32   RegOffset
  );

//
// Function pointer definiton for ClearRunBusyRegFlag()
//
typedef
UINT32
(EFIAPI *CLEAR_RUN_BUSY_REG_FLAG) (
  UINT8    socket,
  UINT8    BoxInst,
  UINT32   RegOffset
  );

/**
  Set Core or Mesh Ratio in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  CoreRatio     Core Ratio value
  @param[in]  MeshRatio     Mesh Ratio Value

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetCoreMeshRatio (
  IN UINT8 Socket,
  IN UINT8 CoreRatio,
  IN UINT8 MeshRatio
  );

//
// Function pointer definiton for PcuSetCoreMeshRatio()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_CORE_MESH_RATIO) (
  IN UINT8 Socket,
  IN UINT8 CoreRatio,
  IN UINT8 MeshRatio
  );

/**
  Set Pmax disable in PCU

  @param[in]  Socket        0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetPmaxDisable (
  IN UINT8 Socket
  );

//
// Function pointer definiton for PcuSetPmaxDisable()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_PMAX_DISABLE) (
  IN UINT8 Socket
  );

/**
  Enable DRAM Power Management in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  ImcMask       ImcMask to enable DRAM Power Management
                            Bit 0 => McId0, Bit 1 => McId1 and so on
                            If bit is set, corresponding power management is enabled

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuEnableDramPm (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  );

//
// Function pointer definiton for PcuEnableDramPm()
//
typedef
EFI_STATUS
(EFIAPI *PCU_ENABLE_DRAM_PM) (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  );

/**
  Disable DRAM Power Management in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  ImcMask       ImcMask to enable DRAM Power Management
                            Bit 0 => McId0, Bit 1 => McId1 and so on
                            If bit is set, corresponding power management is disabled

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuDisableDramPm (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  );
//
// Function pointer definiton for PcuDisableDramPm()
//
typedef
EFI_STATUS
(EFIAPI *PCU_DISABLE_DRAM_PM) (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  );

/**
  Program VR Sensor Mapping in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  McId          Memory Controller number
  @param[in]  VrSvid        SVID of the Voltage Regulator

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramVrSensorMapping (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 VrSvid
  );
//
// Function pointer definiton for PcuProgramVrSensorMapping()
//
typedef
EFI_STATUS
(EFIAPI *PCU_PROGRAM_VR_SENSOR_MAPPING) (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 VrSvid
  );

/**
  Program VR memory controller Mapping in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  McId          Memory Controller number
  @param[in]  VrSvid        SVID of the Voltage Regulator

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramVrMcMapping (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 VrSvid
  );
//
// Function pointer definiton for PcuProgramVrMcMapping()
//
typedef
EFI_STATUS
(EFIAPI *PCU_PROGRAM_VR_MC_MAPPING) (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 VrSvid
  );

/**
  Program VR DIMM Mapping in PCU

  @param[in]  Socket            0 based Socket number
  @param[in]  DimmMask12V       Mask of Dimms attached to 12V. BIT0=>Dimm0...etc
  @param[in]  DimmMaskVr        Mask of Dimms attached to VR. BIT0=>Dimm0...etc
  @param[in]  McId              Memory Controller number
  @param[in]  ChId              Channel number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramVrDimmMapping (
  IN UINT8 Socket,
  IN UINT8 DimmMask12V,
  IN UINT8 DimmMaskVr,
  IN UINT8 McId,
  IN UINT8 ChId
  );
//
// Function pointer definiton for PcuProgramVrDimmMapping()
//
typedef
EFI_STATUS
(EFIAPI *PCU_PROGRAM_VR_DIMM_MAPPING) (
  IN UINT8 Socket,
  IN UINT8 DimmMask12V,
  IN UINT8 DimmMaskVr,
  IN UINT8 McId,
  IN UINT8 ChId
  );

/**
  Disable DRAM Thermal Management

  @param[in]  Socket        0 based Socket number
  @param[in]  ImcMask       ImcMask to enable DRAM Power Management
                            Bit 0 => McId0, Bit 1 => McId1 and so on
                            If bit is set, corresponding thermal management is disabled

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuDisableDramThermal (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  );
//
// Function pointer definiton for PcuDisableDramThermal()
//
typedef
EFI_STATUS
(EFIAPI *PCU_DISABLE_DRAM_THERMAL) (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  );

/**

  This command is used to request that pcode re-poll the DRAM VRs. Once triggered, pcode is expected
  to probe the status of any of the DRAM VRs that were previously listed as disabled/invalid.  Pcode
  is expected to output the updated bit mask of active DRAM VRs in the DATA register.

  After probing for new VRs, BIOS must call these other mailbox commands to update the Pcode configuration:
  VDD_RAMP           - to set the desired VID and VOFFSET for the newly added VR(s)
  DIMM_VR_PHASE_SHED - to initialize the phase shedding settings for the newly added VR(s)
  DDR_RANKS_PRESENT  - to update the DRAM RAPL rank population bitmaps to reflect the newly added VR(s)

  @param[in]     Socket                 Zero based socket number.
  @param[out]    ActiveDramVrMask       Bit mask of active DRAM VRs returned from PCU via DATA register.

  @retval EFI_SUCCESS           Function execution successful.
  @retval EFI_DEVICE_ERROR      Error on command execution.
  @retval EFI_INVALID_PARAMETER Parameter is invalid.

**/
EFI_STATUS
EFIAPI
PcuProbeDramVr (
 IN  UINT8    Socket,
 OUT UINT32   *ActiveDramVrMask
);

//
// Function pointer definiton for PcuProbeDramVr()
//
typedef
EFI_STATUS
(EFIAPI * PCU_PROBE_DRAM_VR) (
 IN  UINT8    Socket,
 OUT UINT32   *ActiveDramVrMask
);


/**
  Enable DRAM Thermal Management

  @param[in]  Socket          0 based Socket number
  @param[in]  ImcMask         ImcMask to enable DRAM Power Management
                              Bit 0 => McId0, Bit 1 => McId1 and so on
                              If bit is set, corresponding thermal management is enabled.
  @param[in]  ThrottlingType  Throttling type to enable

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuEnableDramThermal (
  IN UINT8 Socket,
  IN UINT8 ImcMask,
  IN PCU_THROTTLE_TYPE ThrottlingType
  );
//
// Function pointer definiton for PcuEnableDramThermal()
//
typedef
EFI_STATUS
(EFIAPI *PCU_ENABLE_DRAM_THERMAL) (
  IN UINT8 Socket,
  IN UINT8 ImcMask,
  IN PCU_THROTTLE_TYPE ThrottlingType
  );

/**
  Get CPU SKU Limit. (Memory Capacity)

  @param[in]   Socket              0 based Socket number
  @param[out]  *CpuSkuLimit        Return value of CPU SKU Limit (Memory capacity in 64GB Units)

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetCpuSkuLimit (
  IN UINT8 Socket,
  OUT UINT32 *CpuSkuLimit
  );
//
// Function pointer definiton for PcuGetCpuSkuLimit()
//
typedef
EFI_STATUS
(EFIAPI *PCU_GET_CPU_SKU_LIMIT) (
  IN UINT8 Socket,
  OUT UINT32 *CpuSkuLimit
  );

/**
  Set Memory Controller Frequency

  @param[in]   Socket         0 based Socket number
  @param[in]   RefClk         Reference Clock type
  @param[in]   QclkRatio      Qclk Ratio
  @param[in]   MemType        Memory Type (HBM, DDR4, DDR5)
  @param[in]   MemSsEnabled   Memory Subsystem enable mask
  @param[in]   RampBitEnable  Ramp Bit Enable, if its last call to set MC

  @retval EFI_SUCCESS     Function execution successful
  @retval EFI_UNSUPPORTED Frequency given is unsupported
  @retval !EFI_SUCCESS    Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetMCFreq (
  IN UINT8 Socket,
  IN UINT8 RefClk,
  IN UINT8 QclkRatio,
  IN PCU_MEM_TYPE MemType,
  IN UINT8 MemSsEnabled,
  IN BOOLEAN RampBitEnable
  );
//
// Function pointer definiton for PcuSetMCFreq()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_MC_FREQ) (
  IN UINT8 Socket,
  IN UINT8 RefClk,
  IN UINT8 QclkRatio,
  IN PCU_MEM_TYPE MemType,
  IN UINT8 MemSsEnabled,
  IN BOOLEAN RampBitEnable
  );

/**
  Disable MC timeout W/A

  @param[in]   Socket  0 based Socket number

  @return      EFI_STATUS as reflected by ReadModifyWritePcuMailbox
**/
EFI_STATUS
EFIAPI
PcuMcTimeoutWaDisable (
  IN UINT8 Socket
  );
//
// Function pointer definiton for PcuMcaTimeoutWA()
//
typedef
EFI_STATUS
(EFIAPI *PCU_MC_TIMEOUT_WA_DISABLE) (
  IN  UINT8 Socket
  );

/**
  Send command to PUNIT when MC is operating in 2N mode.

  This function MUST only be called when 2N mode operation is required

  @param[in]    Socket      Current socket under test
  @param[in]    MemType     Memory Type (DDR4, HBM, DDR5)
  @param[in]    McBitMap    Memory Controller supported (bitwise)
  @param[in]    ChBitMap    Memory Channels supported (bitwise)

  @retval       EFI_DEVICE_ERROR  Command did not executed properly
  @retval       EFI_SUCCESS       Command was executed properly

**/
EFI_STATUS
EFIAPI
PcuEnable2nMode (
  IN        UINT8           Socket,
  IN        PCU_MEM_TYPE    MemType,
  IN        UINT8           McBitMap,
  IN        UINT8           ChBitMap
  );
//
// Function pointer definiton for PcuEnable2nMode()
//
typedef
EFI_STATUS
(EFIAPI *PCU_ENABLE_2N_MODE) (
  IN UINT8           Socket,
  IN PCU_MEM_TYPE    MemType,
  IN UINT8           McBitMap,
  IN UINT8           ChBitMap
  );

/**
  Get Vccp Value

  @param[in]   Socket               0 based Socket number
  @param[in]   McId                 Memory Controller number
  @param[out]  *VccpValue           Return value of Vccp Voltage in milli voltage

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetVccpValue (
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT16 *VccpValue
  );
//
// Function pointer definiton for PcuGetVccpValue()
//
typedef
EFI_STATUS
(EFIAPI *PCU_GET_VCCP_VALUE) (
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT16 *VccpValue
  );

/**
  Read active VR info from PCU

  @param[in]   Socket              0 based Socket number
  @param[out]  *ActiveVrMask        Return value of Active VR

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuReadActiveVr (
  IN UINT8 Socket,
  OUT UINT32 *ActiveVrMask
  );
//
// Function pointer definiton for PcuReadActiveVr()
//
typedef
EFI_STATUS
(EFIAPI *PCU_READ_ACTIVE_VR) (
  IN UINT8 Socket,
  OUT UINT32 *ActiveVrMask
  );

/**
  Read VR mode for given VrAddress from PCU

  @param[in]   Socket             0 based Socket number
  @param[in]   VrAddress          Address to read VR Mode
  @param[out]  *VrMode            VR Mode information for given VR address

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcodeReadVrMode (
  IN UINT8  Socket,
  IN UINT32 VrAddress,
  OUT UINT32 *VrMode
  );
//
// Function pointer definiton for PcodeReadVrMode()
//
typedef
EFI_STATUS
(EFIAPI *PCODE_READ_VR_MODE) (
  IN UINT8  Socket,
  IN UINT32 VrAddress,
  OUT UINT32 *VrMode
  );

/**
  Set Vdd voltage through PCU

  @param[in]   Socket             0 based Socket number
  @param[in]   VrAddress          Address to read VR Mode
  @param[in]   VrMode             Voltage Regulator mode
  @param[in]   Uservdd            UserVdd value from policy\setup

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetVdd (
  IN UINT8 Socket,
  IN UINT32 VrAddress,
  IN UINT32 VrMode,
  IN UINT16 UserVdd
  );
//
// Function pointer definiton for PcuSetVdd()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_VDD) (
  IN UINT8 Socket,
  IN UINT32 VrAddress,
  IN UINT32 VrMode,
  IN UINT16 UserVdd
  );

/**
  Set warm reset data to PCU

  @param[in]   Socket                      0 based Socket number
  @param[in]   RstMode                     Surprize reset mode requested
  @param[in]   SurprizeRstCounter          Surprize reset counter.

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetWarmResetCtrl (
  IN UINT8 Socket,
  IN PCU_RST_TYPE RstMode,
  IN UINT32 SurprizeRstCounter
  );
//
// Function pointer definiton for PcuSetWarmResetCtrl()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_WARM_RESET_CTRL) (
  IN UINT8 Socket,
  IN PCU_RST_TYPE RstMode,
  IN UINT32 SurprizeRstCounter
  );

/**
  Read MC frequency information

  @param[in]    Socket                 0 based Socket number
  @param[in]    MemType                PCU Memory type, not used in SKX
  @param[in]    McIdMask               McID mask to read, not used in SKX
                                       Bit 0 => McId0, Bit 1 => McId1 and so on
                                       If bit is set, corresponding MC info is read
  @param[out]   *RefClkType            Reference clock type
  @param[out]   *McQclkRatio           MC QCLK Ratio Encoding.
  @param[out]   *MaxFreq               Max Frequency Supported. 0xFF for this API returning invalid value

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuReadMcFreq (
  IN UINT8 Socket,
  IN PCU_MEM_TYPE MemType,
  IN UINT8 McIdMask,
  OUT UINT8 *RefClkType,
  OUT UINT8 *McQclkRatio,
  OUT UINT8 *MaxFreq
  );
//
// Function pointer definiton for PcuReadMcFreq()
//
typedef
EFI_STATUS
(EFIAPI *PCU_READ_MC_FREQ) (
  IN UINT8 Socket,
  IN PCU_MEM_TYPE MemType,
  IN UINT8 McIdMask,
  OUT UINT8 *RefClkType,
  OUT UINT8 *McQclkRatio,
  OUT UINT8 *MaxFreq
  );

/**
  Set Scrub interval in PCU

  @param[in]    Socket                0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   ScrubInterval          Value of the Scrub Interval

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetScrubInterval (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 ScrubInterval
  );
//
// Function pointer definiton for PcuSetScrubInterval()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_SCRUB_INTERVAL) (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 ScrubInterval
  );

/**
  Get Scrub interval in PCU

  @param[in]    Socket                0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   ScrubInterval          Value of the Scrub Interval

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetScrubInterval (
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT32 *ScrubInterval
  );
//
// Function pointer definiton for PcuGetScrubInterval()
//
typedef
EFI_STATUS
(EFIAPI *PCU_GET_SCRUB_INTERVAL) (
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT32 *ScrubInterval
  );

/**
  Set Missed Threshold for Scrub in PCU

  @param[in]   Socket                 0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   MissedThreshold        Value of the threshold for Scrub miss

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetMissedThreshold (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 MissedThreshold
  );
//
// Function pointer definiton for PcuSetMissedThreshold()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_MISSED_THRESHOLD) (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 MissedThreshold
  );

/**
  Set Advanced Threshold for Scrub in PCU

  @param[in]   Socket                 0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   AdvancedThreshold      Value of Advanced threshold for Scrub

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetAdvancedThreshold (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 AdvancedThreshold
  );
//
// Function pointer definiton for PcuSetAdvancedThreshold()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_ADVANCED_THRESHOLD) (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 AdvancedThreshold
  );

/**
  Check Fuse override enabled

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuFuseOverride (
  UINT8 Socket
  );
//
// Function pointer definiton for PcuFuseOverride()
//
typedef
EFI_STATUS
(EFIAPI *PCU_FUSE_OVERRIDE) (
  UINT8 Socket
  );

/**
  Set miscellanous workaround enable

  @param[in]    Socket                 0 based Socket number
  @param[in]    CmdData                 Workaround Data

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuSetMiscWorkaroundEnable (
  UINT8 socket,
  UINT32 CmdData
  );
//
// Function pointer definiton for PcuSetMiscWorkaroundEnable()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_MISC_WA_ENABLE) (
  UINT8 socket,
  UINT32 CmdData
  );

/**
  Disable rcomp for given MCID mask

  @param[in]    Socket                 0 based Socket number
  @param[in]    McIdMask               McId mask value, Bit0=>Mc0..etc

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuDisableRcomp (
  UINT8 Socket,
  UINT8 McIdMask
  );
//
// Function pointer definiton for PcuDisableRcomp()
//
typedef
EFI_STATUS
(EFIAPI *PCU_DISABLE_RCOMP) (
  UINT8 Socket,
  UINT8 McIdMask
  );

/**
  Enable Surprise reset in PCU

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuEnableSurpriseReset (
  UINT8 Socket
  );
//
// Function pointer definiton for PcuEnableSurpriseReset()
//
typedef
EFI_STATUS
(EFIAPI *PCU_ENABLE_SURPRISE_RESET) (
  UINT8 Socket
  );

/**
  Program SREF PKGC register in PCU

  @param[in]    Socket                 0 based Socket number
  @param[in]    McId                    Memory Controller number
  @param[in]    ChId                    Channel Number
  @param[in]    SrefData                SREF Data

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetSrefPkgc (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 ChId,
  IN UINT32 SrefData
  );
//
// Function pointer definiton for PcuSetSrefPkgc()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_SREF_PKGC) (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 ChId,
  IN UINT32 SrefData
  );

/**
  Set E2E parity bit in PCU Misc

  @param[in]    Socket      0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetE2EParity (
  UINT8 Socket
  );
//
// Function pointer definiton for PcuSetE2EParity()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_E2E_PARITY) (
  UINT8 Socket
  );

/**
  Program Minimum OPS value for DRAM PBM\RAPL

  @param[in]    Socket                  0 based Socket number
  @param[in]    MinOpsValue             Minimum operation value

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramMinOpsValue (
  IN UINT8 Socket,
  IN UINT32 MinOpsValue
  );
//
// Function pointer definiton for PcuProgramMinOpsValue()
//
typedef
EFI_STATUS
(EFIAPI *PCU_PROGRAM_MIN_OPS_VALUE) (
  IN UINT8 Socket,
  IN UINT32 MinOpsValue
  );

/**
  Program DRAM RAPL Mode 0=>Estimated,01=>VR Measurement

  @param[in]    Socket                  0 based Socket number
  @param[in]    RaplMode                Set Rapl Mode

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetDramRaplMode (
  IN UINT8 Socket,
  IN UINT8 RaplMode
  );
//
// Function pointer definiton for PcuSetDramRaplMode()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_DRAM_RAPL_MODE) (
  IN UINT8 Socket,
  IN UINT8 RaplMode
  );

/**
  Program Bandwidth power scaling factor

  @param[in]    Socket                  0 based Socket number
  @param[in]    Channel                 Channel Number
  @param[in]    SelectorType            Selector type, 0 => Scale Factor, 1=> Minimum Power
  @param[in]    Value                   Value to program

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetBWPowerScale (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SelectorType,
  IN UINT16 Value
  );
//
// Function pointer definiton for PcuSetBWPowerScale()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_BW_POWER_SCALE) (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SelectorType,
  IN UINT16 Value
  );

/**
  Program DDR power scaling factor

  @param[in]    Socket                  0 based Socket number
  @param[in]    Channel                 Channel Number
  @param[in]    Dimm                    Dimm Number
  @param[in]    SelectorType            Selector type, 0 => Scale Factor, 1=> Minimum Power
  @param[in]    Value                   Value to program

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramDDRPowerScale (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 SelectorType,
  IN UINT16 Value
  );
//
// Function pointer definiton for PcuProgramDDRPowerScale()
//
typedef
EFI_STATUS
(EFIAPI *PCU_PROGRAM_DDR_POWER_SCALE) (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 SelectorType,
  IN UINT16 Value
  );

/*
  Program PBM Throttle Adjust factor

  @param[in]    Socket                  0 based Socket number
  @param[in]    Channel                 Channel Number
  @param[in]    Value                   Value to program

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramPbmThrotAdj (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Value
  );
//
// Function pointer definiton for PcuProgramPbmThrotAdj()
//
typedef
EFI_STATUS
(EFIAPI *PCU_PROGRAM_PBM_THROT_ADJ) (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Value
  );

/**
  Lock\Open Spd Smbus access

  @param[in]    Socket              0 based Socket number
  @param[in]    SpdSmbusAccessType  Lock\UnLock the access

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSpdSmbusAccess (
  IN UINT8 Socket,
  IN SPD_SMBUS_ACCESS_TYPE SpdSmbusAccessType
  );
//
// Function pointer definiton for PcuSpdSmbusAccess()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SPD_SMBUS_ACCESS) (
  IN UINT8 Socket,
  IN SPD_SMBUS_ACCESS_TYPE SpdSmbusAccessType
  );

/*
  Function will do a Bios 2 Pcu mailbox write for the command and data passed

  @param  [in]        Socket,
  @retval EFI_STATUS  Status  The status of the Bios 2 Pcode call
*/
EFI_STATUS
EFIAPI
PcuSetSmmSaiPolicyWa (
  IN UINT8  Socket
  );
//
// Function pointer definiton for PcuSetSmmSaiPolicyWa()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_SMM_SAI_POLICY_WA) (
  IN UINT8  Socket
  );

/**
  Set MS2IOSF HW_INIT for the given Mesh ID

  @param[in]    Socket      0 based Socket number
  @param[in]    MeshId      Mesh ID of the IIO stack to trigger HW_INIT

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetMs2iosfHwInit (
  IN UINT8 Socket,
  IN UINT8 MeshId
  );
//
// Function pointer definiton for PcuSetMs2iosfHwInit()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_MS2IOSF_HW_INIT) (
  IN UINT8 Socket,
  IN UINT8 MeshId
  );

/**
  Read MS2IOSF HW_INIT for the given Mesh ID

  @param[in]    Socket                    0 based Socket number
  @param[in]    MeshId                    Mesh ID of the IIO stack to trigger HW_INIT
  @param[OUT]   TcMiscUcrMailBoxData      Data from TC_MISC_UCR_SB_M2PCIE_REG

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetMs2iosfHwInit (
  IN UINT8 Socket,
  IN UINT8 MeshId,
  OUT UINT32 *TcMiscUcrMailBoxData
  );
//
// Function pointer definiton for PcuGetMs2iosfHwInit()
//
typedef
EFI_STATUS
(EFIAPI *PCU_GET_MS2IOSF_HW_INIT) (
  IN UINT8 Socket,
  IN UINT8 MeshId,
  OUT UINT32 *TcMiscUcrMailBoxData
  );

/**
  Set PCIe DFX registers
  @param[in]    Socket                    Socket number
  @param[in]    CrIndex                   Index of register type. Currently, supported three PCIe DFX Gen 3 registers
                                          {pxpprtgen3eqpriv = 0x80,gen3privtxeq = 0x81,tx_ph3_cursor = 0x82}
  @param[in]    CrInst                    Ip Instance supported. Currently only supported value {0}, representing PCIe Gen 3 for SNR.
  @param[in]    CrPortIdx                 PCIe Port Index to access. Currently supported four values {0, 1, 2, 3} corresponding to
                                          PCIe port A, port B, port C, portD, respectively.
  @param[in]    CrProxyMailBoxData        Data to PCIe DFX register
  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetPcieDfx (
  IN UINT8   Socket,
  IN UINT8   CrIndex,
  IN UINT8   CrInst,
  IN UINT8   CrPortIdx,
  IN UINT32  CrProxyMailBoxData
  );
//
// Function pointer definition for PcuSetPcieDfx()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_PCIE_DFX) (
  IN UINT8   Socket,
  IN UINT8   CrIndex,
  IN UINT8   CrInst,
  IN UINT8   CrPortIdx,
  IN UINT32  CrProxyMailBoxData
  );

/**
  Read PCIe DFX registers
  @param[in]    Socket                    Socket number
  @param[in]    CrIndex                   Index of register type. Currently, supported three PCIe DFX Gen 3 registers
                                          {pxpprtgen3eqpriv = 0x80,gen3privtxeq = 0x81,tx_ph3_cursor = 0x82}
  @param[in]    CrInst                    Ip Instance supported. Currently only supported value {0}, representing PCIe Gen 3 for SNR.
  @param[in]    CrPortIdx                 PCIe Port Index to access. Currently supported four values {0, 1, 2, 3} corresponding to
                                          PCIe port A, port B, port C, portD, respectively.

  @param[OUT]   CrProxyMailBoxData        Data from PCIe DFX reg

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetPcieDfx (
  IN  UINT8  Socket,
  IN  UINT8  CrIndex,
  IN  UINT8  CrInst,
  IN  UINT8  CrPortIdx,
  OUT UINT32 *CrProxyMailBoxData
  );
//
// Function pointer definition for PcuSetPcieDfx()
//
typedef
EFI_STATUS
(EFIAPI *PCU_GET_PCIE_DFX) (
  IN  UINT8   Socket,
  IN  UINT8   CrIndex,
  IN  UINT8   CrInst,
  IN  UINT8   CrPortIdx,
  OUT UINT32  *CrProxyMailBoxData
  );

/**
  Check Fuse override enabled

  @param[in]    Socket                 0 based Socket number
  @param[in]    AvoidRatio             Send avoid mesh range to Pcode

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetAvoidMeshRatio (
  IN UINT8  Socket,
  IN UINT32 AvoidRatio
  );
//
// Function pointer definiton for PcuSetAvoidMeshRatio()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_AVOID_MESH_RATIO) (
  IN UINT8 Socket,
  IN UINT32 AvoidRatio
  );

/**
  Trigger MDFIS advanced Training

  @param[in]    Socket                 0 based Socket number
  @param[in]    MinRatio               Min ratio
  @param[in]    MaxRatio               Max ratio

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI PcuTriggerMdfisTraining (
  IN UINT8 Socket,
  IN UINT8 MinRatio,
  IN UINT8 MaxRatio
  );
//
// Function pointer definiton for PcuTriggerMdfisTraining()
//
typedef
EFI_STATUS
(EFIAPI *PCU_TRIGGER_MDFIS_TRAINING) (
  IN UINT8 Socket,
  IN UINT8 MinRatio,
  IN UINT8 MaxRatio
  );

/**
  This command is used to override the default pointer separation values for BGFs in the GV
  domain

  @param[in]    Socket                 0 based Socket number
  @param[in]    BGFPtrOverrideMask     Mask to indicate which BGFs pointer separation need to be programmed

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuBGFPointerOverride (
  IN UINT8  Socket,
  IN UINT8  BGFPtrOverrideMask
  );
//
// Function pointer definiton for PcuBGFPointerOverride()
//
typedef
EFI_STATUS
(EFIAPI *PCU_BGF_POINTER_OVERRIDE) (
  IN UINT8  Socket,
  IN UINT8  BGFPtrOverrideMask
  );

/**
  This command is used to trigger mechanism for TSC synchronization of hot
  added/onlined sockets

  @param[in]    Socket                 0 based Socket number
  @param[in]    Value                  1: assert the CPU only reset pin on the local socket.
                                       0: de-assert the CPU only reset pin on the local socket.

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSynchronizeTsc (
  IN UINT8  Socket,
  IN UINT8  Value
  );
//
// Function pointer definiton for PcuSynchronizeTsc()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SYNCHRONIZE_TSC) (
  IN UINT8  Socket,
  IN UINT8  Value
  );

/**
  Set the UPI (f.k.a. KTI, f.k.a. QPI) frequency for the next reset.

  @param[in]    Socket                 0 based Socket number
  @param[in]    UpiPortId              UPI Port ID
  @param[in]    UpiSpeedRatio          UPI Ratio (the UPI fast clock frequency in 100MHz units)

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetUpiFrequency (
  IN UINT8  Socket,
  IN UINT8  UpiPortId,
  IN UINT32 UpiSpeedRatio
  );
//
// Function pointer definiton for PcuSetUpiFrequency()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_UPI_FREQUENCY) (
  IN UINT8  Socket,
  IN UINT8  UpiPortId,
  IN UINT32 UpiSpeedRatio
  );

/**
  Set the trigger bit to trigger the GV FLow for UPI (f.k.a. KTI, f.k.a. QPI) frequency switch.
  Set once for each socket

  @param[in]    Socket                 0 based Socket number
  @param[in]    UpiPortId              UPI Port ID
  @param[in]    UpiSpeedRatio          UPI Ratio (the UPI fast clock frequency in 100MHz units)

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetTriggerBit (
  IN UINT8  Socket,
  IN UINT8  UpiPortId,
  IN UINT32 UpiSpeedRatio
  );
//
// Function pointer definiton for PcuSetTriggerBit()
//
typedef
EFI_STATUS
(EFIAPI *PCU_SET_TRIGGER_BIT) (
  IN UINT8  Socket,
  IN UINT8  UpiPortId,
  IN UINT32 UpiSpeedRatio
  );

/**
  BIOS uses this command to clear the FW agent link layer attribute in the UPI (f.k.a. KTI,
  f.k.a. QPI) transmit parameters on the neighboring socket, corresponding to the UPI port that
  will be connected to the socket being added.

  @param[in]    Socket                 0 based Socket number
  @param[in]    UpiPortId              UPI Port ID

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuMaskFwAgent (
  IN UINT8  Socket,
  IN UINT8  UpiPortId
  );
//
// Function pointer definiton for PcuMaskFwAgent()
//
typedef
EFI_STATUS
(EFIAPI *PCU_MASK_FW_AGENT) (
  IN UINT8  Socket,
  IN UINT8  UpiPortId
  );

/**
  Used to configure SNC

  @param[in]    Socket                 0 based Socket number
  @Param[in]    SncCfgInterfaceIndex   Snc configure interface Index
  @param[in]    Value                  SNC configuration

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuCfgSnc (
  IN UINT8  Socket,
  IN UINT8  SncCfgInterfaceIndex,
  IN UINT32 Value
  );

//
// Function pointer definiton for PcuCfgSnc()
//
typedef
EFI_STATUS
(EFIAPI *PCU_CFG_SNC) (
  IN UINT8  Socket,
  IN UINT8  SncCfgInterfaceIndex,
  IN UINT32 Value
  );

/**
  Extract the link number from Mailbox Interface register

  @param[in]    Socket                 0 based Socket number
  @param[in]    Value                  Mailbox Interface register value
  @param[out]   UpiPort                UPI Port Number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuExtractLinkNum (
  IN UINT8  Socket,
  IN UINT32 Value,
  OUT UINT8 *UpiPort
  );
//
// Function pointer definiton for PcuExtractLinkNum()
//
typedef
EFI_STATUS
(EFIAPI *PCU_EXTRACT_LINK_NUM) (
  IN UINT8  Socket,
  IN UINT32 Value,
  OUT UINT8 *UpiPort
  );

/**
  Check whether PCU command is MAILBOX_BIOS_CMD_SET_UPI_FREQ

  @param[in]    Socket                 0 based Socket number
  @param[in]    Value                  Mailbox Interface register value
  @param[out]   Flag                   Indicate whether it's SET_UPI_FREQ command or not

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuCheckCmdIsSetUpiFreq (
  IN UINT8  Socket,
  IN UINT32 Value,
  OUT BOOLEAN *Flag
  );
//
// Function pointer definiton for PcuCheckCmdIsSetUpiFreq()
//
typedef
EFI_STATUS
(EFIAPI *PCU_CHECK_CMD_IS_SET_UPI_FREQ) (
  IN UINT8  Socket,
  IN UINT32 Value,
  OUT BOOLEAN *Flag
  );


/**
  Initial Dimm_temp to 0x54 for DCPMM

  @param[in]  Socket        0 based Socket number
  @param[in]  McId          Memory Controller number
  @param[in]  Ch            0 based Channel number
  @param[in]  Dimm          0 based Dimm number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuInitialDimmTemp (
  IN UINT8  Socket,
  IN UINT8  McId,
  IN UINT8  Ch,
  IN UINT8  Dimm
);
//
// Function pointer definiton for PcuInitialDimmTemp()
//
typedef
EFI_STATUS
(EFIAPI *PCU_INITIAL_DIMM_TEMP) (
  IN UINT8  Socket,
  IN UINT8  McId,
  IN UINT8  Ch,
  IN UINT8  Dimm
);

/**
  Enable IMON polling on given SVID
  @param[in]  Socket        0 based Socket number
  @param[in]  VrSvid        SVID of the Voltage Regulator

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuEnableImon (
  IN UINT8 Socket,
  IN UINT8 VrSvid
  );
//
// Function pointer definiton for PcuEnableImon()
//
typedef
EFI_STATUS
(EFIAPI *PCU_ENABLE_IMON) (
  IN UINT8 Socket,
  IN UINT8 VrSvid
  );

/**

  To write into the respective SMBus2 control registers given by the parameters:
    0: SMB2_CMD register
    1: SMB2_CMD_EXT register
    2: SMB2_DATA register

  Error codes:
  MAILBOX_BIOS_RETURN_CODE_NONE
  MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND

  Semaphore Arbitration
  An "access semaphore" is maintained by CPU to arbitrate between multiple competing requestors (PECI, BIOS, CPU itself).
  BIOS must request the semaphore (bit 17, see below) prior to performing any CR reads or writes.  When BIOS requests semaphore,
  CPU will return either of the following return codes:
    MAILBOX_BIOS_RETURN_CODE_NONE: semaphore is granted
    MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND: semaphore is not granted
  Once granted, BIOS will "own" the semaphore until explicitly released using bit 18.
  Semaphore request/release cannot be combined with a CR access; if bit 17 or 18 is set, the other bits will be ignored.
  If BIOS does not 'own' the semaphore, all CR access requests will return INVALID_COMMAND.
  CPU will filter addresses to prevent access to certain address ranges.  If an attempt to read or write one of those addresses,
  INVALID_COMMAND will be returned.

  @param[in]     Socket                 Zero based socket number.
  @param[in]     SmbusCommand           SMBUS command.
  @param[in]     SmbusCommandExt        SMBUS command extension.
  @param[in]     SmbusData              SMBUS data.

  @retval        EFI_SUCCESS            PCU executed command with no error.
  @retval        EFI_DEVICE_ERROR       PCU failed to execute command.
  @retval        EFI_INVALID_PARAMETER  Parameter error.

**/
EFI_STATUS
EFIAPI
PcuSmbAccessWrite (
  IN  UINT8   Socket,
  IN  UINT32  SmbusCommand,
  IN  UINT32  SmbusCommandExt,
  IN  UINT16  SmbusData
  );

//
// Function pointer definiton for PcuSmbAccessWrite()
//
typedef
EFI_STATUS
(EFIAPI  *PCU_SMBUS_ACCESS_WRITE) (
  IN  UINT8   Socket,
  IN  UINT32  SmbusCommand,
  IN  UINT32  SmbusCommandExt,
  IN  UINT16  SmbusData
  );

/**
  Trigger late action credit update.

  In response to the mailbox command, pcode (on CPU-L) will inform the S3M to
  initiate a multi-socket update sequence where pcode sets the trigger control bit
  to cause a late-action update of the credits.

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution

**/
EFI_STATUS
EFIAPI
PcuTriggerLateActionRegUpdate (
  IN UINT8  Socket
  );
//
// Function pointer definiton for PcuTriggerLateActionRegUpdate()
//
typedef
EFI_STATUS
(EFIAPI *PCU_TRIGGER_LATE_ACTION_REG_UPDATE) (
  IN UINT8  Socket
  );

/**
  Set PI5 IP Disable for the given Stack ID

  @param[in]    Socket      0 based Socket number
  @param[in]    StackId     IIO stack to trigger PI5 IP Disable
  @param[in]    DisConf     PI5 IP Disable config

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_DEVICE_ERROR  Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetPi5IpDisable (
  IN UINT8  Socket,
  IN UINT8  StackId,
  IN UINT32 DisConf
  );

//
// Function pointer definiton for PcuSetPi5IpDisable()
//
typedef
EFI_STATUS
(EFIAPI  *PCU_SET_PI5_IP_DISABLE) (
  IN UINT8  Socket,
  IN UINT8  StackId,
  IN UINT32 DisConf
  );

/**
  Inform pCode if the result of Protocol negotiation during Link Training is CXL for the given Stack ID.

  @param[in]    Socket      0 based Socket number
  @param[in]    StackId     IIO stack to trigger PI5 IP Disable
  @param[in]    DisConf     PI5 IP Disable config

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_DEVICE_ERROR  Error on command execution
**/
EFI_STATUS
EFIAPI
PcuNotifyCxlInst (
  IN UINT8    Socket,
  IN UINT8    StackId,
  IN BOOLEAN  ConfComplete
  );

//
// Function pointer definiton for PcuNotifyCxlInst()
//
typedef
EFI_STATUS
(EFIAPI  *PCU_NOTIFY_CXL_INSTANCE) (
  IN UINT8    Socket,
  IN UINT8    StackId,
  IN BOOLEAN  ConfComplete
  );

/**
  Report voltage regulator on Dimm to PCU

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuReportVrod (
  IN UINT8 Socket
  );

//
// Function pointer definiton for PcuReportVrod()
//
typedef
EFI_STATUS
(EFIAPI  *PCU_REPORT_VROD) (
  IN UINT8    Socket
  );

/**
  Inform pCode the fADR config and core/mesh ratio.

  @param[in]  Socket        0 based Socket number
  @param[in]  fAdrEn        fADR status
  @param[in]  CoreRatio     Core Ratio value
  @param[in]  MeshRatio     Mesh Ratio Value

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetEadrRatio (
  IN UINT8    Socket,
  IN BOOLEAN  fAdrEn,
  IN UINT8    CoreRatio,
  IN UINT8    MeshRatio
  );

//
// Function pointer definiton for PcuSeteAdrRatio()
//
typedef
EFI_STATUS
(EFIAPI  *PCU_SET_EADR_RATIO) (
  IN UINT8    Socket,
  IN BOOLEAN  fAdrEn,
  IN UINT8    CoreRatio,
  IN UINT8    MeshRatio
  );

/**
  PCU writes the CR_PROXY supported register.

  This function is a general workaround for writing registers implemented in wrong
  SAI policy group which prevents direct BIOS access. Pcode will write the register
  on behalf of BIOS.

  @param[in] ScktId               Socket index.
  @param[in] InstId               CR instance index on socket.
  @param[in] ChId                 CR channel index on CR instance.
  @param[in] RegId                Register index in the CR whitelist.
  @param[in] PcuMailboxData       Data to be written through PCU mailbox.

  @retval EFI_SUCCESS             Function execution successful.
  @retval !EFI_SUCCESS            Error on command execution.

**/
EFI_STATUS
EFIAPI
PcuWriteCrProxyReg (
  IN UINT8      ScktId,
  IN UINT8      InstId,
  IN UINT8      ChId,
  IN UINT8      RegId,
  IN UINT32     PcuMailboxData
  );

//
// Function pointer definiton for PcuWriteCrProxyReg()
//
typedef
EFI_STATUS
(EFIAPI  *PCU_WRITE_CR_PROXY_REG) (
  IN UINT8      ScktId,
  IN UINT8      InstId,
  IN UINT8      ChId,
  IN UINT8      RegId,
  IN UINT32     PcuMailboxData
  );


/**
  PCU reads the CR_PROXY supported register.

  This function is a general workaround for reading registers implemented in wrong
  SAI policy group which prevents direct BIOS access. Pcode will read the register
  on behalf of BIOS.

  @param[in]  ScktId              Socket index.
  @param[in]  InstId              CR instance index on socket.
  @param[in]  ChId                CR channel index on CR instance.
  @param[in]  RegId               Register index in the CR whitelist.
  @param[out] RegValue            Register value retured by PCU mailbox.

  @retval EFI_SUCCESS             Function execution successful.
  @retval !EFI_SUCCESS            Error on command execution.

**/
EFI_STATUS
EFIAPI
PcuReadCrProxyReg (
  IN  UINT8      ScktId,
  IN  UINT8      InstId,
  IN  UINT8      ChId,
  IN  UINT8      RegId,
  OUT UINT32     *RegValue
  );

//
// Function pointer definiton for PcuReadCrProxyReg()
//
typedef
EFI_STATUS
(EFIAPI  *PCU_READ_CR_PROXY_REG) (
  IN  UINT8      ScktId,
  IN  UINT8      InstId,
  IN  UINT8      ChId,
  IN  UINT8      RegId,
  OUT UINT32     *RegValue
  );

typedef struct {
  READ_MODIFY_WRITE_PCU_MAILBOX        ReadModifyWritePcuMailbox;
  CLEAR_RUN_BUSY_REG_FLAG              ClearRunBusyRegFlag;
  PCU_SET_CORE_MESH_RATIO              PcuSetCoreMeshRatio;
  PCU_SET_PMAX_DISABLE                 PcuSetPmaxDisable;
  PCU_ENABLE_DRAM_PM                   PcuEnableDramPm;
  PCU_DISABLE_DRAM_PM                  PcuDisableDramPm;
  PCU_PROGRAM_VR_SENSOR_MAPPING        PcuProgramVrSensorMapping;
  PCU_PROGRAM_VR_MC_MAPPING            PcuProgramVrMcMapping;
  PCU_PROGRAM_VR_DIMM_MAPPING          PcuProgramVrDimmMapping;
  PCU_DISABLE_DRAM_THERMAL             PcuDisableDramThermal;
  PCU_PROBE_DRAM_VR                    PcuProbeDramVr;
  PCU_ENABLE_DRAM_THERMAL              PcuEnableDramThermal;
  PCU_GET_CPU_SKU_LIMIT                PcuGetCpuSkuLimit;
  PCU_SET_MC_FREQ                      PcuSetMCFreq;
  PCU_ENABLE_2N_MODE                   PcuEnable2nMode;
  PCU_GET_VCCP_VALUE                   PcuGetVccpValue;
  PCU_READ_ACTIVE_VR                   PcuReadActiveVr;
  PCODE_READ_VR_MODE                   PcodeReadVrMode;
  PCU_SET_VDD                          PcuSetVdd;
  PCU_SET_WARM_RESET_CTRL              PcuSetWarmResetCtrl;
  PCU_READ_MC_FREQ                     PcuReadMcFreq;
  PCU_SET_SCRUB_INTERVAL               PcuSetScrubInterval;
  PCU_MC_TIMEOUT_WA_DISABLE            PcuMcTimeoutWaDisable;
  PCU_SET_MISSED_THRESHOLD             PcuSetMissedThreshold;
  PCU_SET_ADVANCED_THRESHOLD           PcuSetAdvancedThreshold;
  PCU_FUSE_OVERRIDE                    PcuFuseOverride;
  PCU_SET_MISC_WA_ENABLE               PcuSetMiscWorkaroundEnable;
  PCU_DISABLE_RCOMP                    PcuDisableRcomp;
  PCU_ENABLE_SURPRISE_RESET            PcuEnableSurpriseReset;
  PCU_SET_SREF_PKGC                    PcuSetSrefPkgc;
  PCU_SET_E2E_PARITY                   PcuSetE2EParity;
  PCU_PROGRAM_MIN_OPS_VALUE            PcuProgramMinOpsValue;
  PCU_SET_DRAM_RAPL_MODE               PcuSetDramRaplMode;
  PCU_SET_BW_POWER_SCALE               PcuSetBWPowerScale;
  PCU_PROGRAM_DDR_POWER_SCALE          PcuProgramDDRPowerScale;
  PCU_PROGRAM_PBM_THROT_ADJ            PcuProgramPbmThrotAdj;
  PCU_SPD_SMBUS_ACCESS                 PcuSpdSmbusAccess;
  PCU_SET_SMM_SAI_POLICY_WA            PcuSetSmmSaiPolicyWa;
  PCU_SET_MS2IOSF_HW_INIT              PcuSetMs2iosfHwInit;
  PCU_GET_MS2IOSF_HW_INIT              PcuGetMs2iosfHwInit;
  PCU_SET_PCIE_DFX                     PcuSetPcieDfx;
  PCU_GET_PCIE_DFX                     PcuGetPcieDfx;
  PCU_SET_AVOID_MESH_RATIO             PcuSetAvoidMeshRatio;
  PCU_TRIGGER_MDFIS_TRAINING           PcuTriggerMdfisTraining;
  PCU_BGF_POINTER_OVERRIDE             PcuBGFPointerOverride;
  PCU_SYNCHRONIZE_TSC                  PcuSynchronizeTsc;
  PCU_SET_UPI_FREQUENCY                PcuSetUpiFrequency;
  PCU_SET_TRIGGER_BIT                  PcuSetTriggerBit;
  PCU_MASK_FW_AGENT                    PcuMaskFwAgent;
  PCU_CFG_SNC                          PcuCfgSnc;
  PCU_EXTRACT_LINK_NUM                 PcuExtractLinkNum;
  PCU_CHECK_CMD_IS_SET_UPI_FREQ        PcuCheckCmdIsSetUpiFreq;
  PCU_INITIAL_DIMM_TEMP                PcuInitialDimmTemp;
  PCU_ENABLE_IMON                      PcuEnableImon;
  PCU_SMBUS_ACCESS_WRITE               PcuSmbAccessWrite;
  PCU_TRIGGER_LATE_ACTION_REG_UPDATE   PcuTriggerLateActionRegUpdate;
  PCU_SET_PI5_IP_DISABLE               PcuSetPi5IpDisable;
  PCU_NOTIFY_CXL_INSTANCE              PcuNotifyCxlInst;
  PCU_REPORT_VROD                      PcuReportVrod;
  PCU_GET_SCRUB_INTERVAL               PcuGetScrubInterval;
  PCU_SET_EADR_RATIO                   PcuSetEadrRatio;
  PCU_WRITE_CR_PROXY_REG               PcuWriteCrProxyReg;
  PCU_READ_CR_PROXY_REG                PcuReadCrProxyReg;
} PCU_MAILBOX_IP_INTERFACE;

//
// This API supports multiple instances with the GUID identifying the instance
//
#define PCU_MAILBOX_IP_INTERFACE_CPU_GUID \
  { 0x52ee6a0d, 0x2c64, 0x4865, { 0xa8, 0xf5, 0x2c, 0x75, 0x81, 0xf7, 0xf9, 0x2a }} // {52EE6A0D-2C64-4865-A8F5-2C7581F7F92A}
extern EFI_GUID gPcuMailBoxIpInterfaceCpuGuid;

#define PCU_MAILBOX_IP_INTERFACE_PCIE_GEN4_GUID \
  { 0xb096b416, 0x520a, 0x42f0, { 0xa4, 0xb3, 0x53, 0x1d, 0x7d, 0xd5, 0x06, 0xc5 }} // {B096B416-520A-42F0-A4B3-531D7DD506C5}
extern EFI_GUID gPcuMailBoxIpInterfacePcieGen4Guid;

#define MAILBOX_BIOS_CR_READ_CMD(Mc, Ch, RegIndex)  ((Mc << MAILBOX_BIOS_CMD_CR_PROXY_NODE_INDEX) | (Ch << MAILBOX_BIOS_CMD_CR_PROXY_CH_INDEX) | \
                                                    (RegIndex << MAILBOX_BIOS_CMD_CR_PROXY_REG_INDEX) | MAILBOX_BIOS_CMD_CR_PROXY_READ);

#define MAILBOX_BIOS_CR_WRITE_CMD(Mc, Ch, RegIndex)  ((Mc << MAILBOX_BIOS_CMD_CR_PROXY_NODE_INDEX) | (Ch << MAILBOX_BIOS_CMD_CR_PROXY_CH_INDEX) | \
                                                    (RegIndex << MAILBOX_BIOS_CMD_CR_PROXY_REG_INDEX) | MAILBOX_BIOS_CMD_CR_PROXY_WRITE);


#endif  // _BIOS_TO_PCODE_MAILBOX_LIB_H_
