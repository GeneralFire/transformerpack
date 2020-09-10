/** @file
  The SPD Decode Library API provides the decode functions which connects the core
  and hardware access to do the transaction work.

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

#ifndef _SPD_DECODE_LIB_H_
#define _SPD_DECODE_LIB_H_

#include <ReferenceCodeDataTypes.h>
#include <Memory/SpdFieldIndex.h>
#include <SysHost.h>
#include <MemCommon.h>
#include <Library/Ddr5CoreLib.h>


#define SPD_BANK_ADDRESS_BASE         2     // Base for sdram actual bank adress bits number for DDR4
#define SPD_COL_BASE                  9     // Base for sdram actual column bits number for DDR4
#define SPD_ROW_BASE                  12    // Base for sdram actual row bits number for DDR4
#define SPD_DIE_BASE                  1     // Base for actual sdram die count for both DDR4 and DDR5
#define SPD_REVISION_09_DDR5          9     // The SPD encoded value mapping to DDR5 SPD revision 0.9

//
// Applicable for SPD Spec lower than 0.9 version
//
#define SPD_COL_10_DDR5               0     // The SPD encoded value mapping to 10 column bits
#define SPD_COL_11_DDR5               2     // The SPD encoded value mapping to 11 column bits
#define SPD_ROW_16_DDR5               0     // The SPD encoded value mapping to 16 row bits
#define SPD_ROW_17_DDR5               2     // The SPD encoded value mapping to 17 row bits
#define SPD_ROW_18_DDR5               3     // The SPD encoded value mapping to 18 row bits
#define SPD_ACTUAL_COL_10_DDR5        10    // The actual column bits number is 10
#define SPD_ACTUAL_COL_11_DDR5        11    // The actual column bits number is 11
#define SPD_ACTUAL_ROW_16_DDR5        16    // The actual row bits number is 16
#define SPD_ACTUAL_ROW_17_DDR5        17    // The actual row bits number is 17
#define SPD_ACTUAL_ROW_18_DDR5        18    // The actual row bits number is 18
//
// Applicable for SPD Spec not lower than 0.9 version
//
#define SPD_ROW_BASE_DDR5             16    // Base for sdram actual row bits number
#define SPD_COL_BASE_DDR5             10    // Base for sdram actual column bits number


#define SPD_1DIE_DDR5                 0     // The SPD encoded value mapping to actual 1 die; Monolithic SDRAM
#define SPD_2DIE_DDR5                 1     // The SPD encoded value mapping to actual 2 die; DDP with multiple loads on inputs & outputs
#define SPD_2DIE_3DS_DDR5             2     // The SPD encoded value mapping to actual 2 die; 2H 3DS, single load on inputs & outputs
#define SPD_4DIE_DDR5                 3     // The SPD encoded value mapping to actual 4 die; 4H 3DS
#define SPD_8DIE_DDR5                 4     // The SPD encoded value mapping to actual 8 die; 8H 3DS
#define SPD_16DIE_DDR5                5     // The SPD encoded value mapping to actual 16 die; 16H 3DS
#define SPD_RANK_MIX_DDR5             1     // The SPD encoded value indicating that the target DIMM is asymmetrical (has rank mix)
//
// Macros defining the SPD encoded value mapping to the corresponding actual sdram capacity per die
// Based on DDR4 encoding rule, applicable for both DDR4 and DDR5
//
#define SPD_1Gb                       2     // The SPD encoded value mapping to sdram capacity 1 Gb per die, based on DDR4 encoding rule
#define SPD_2Gb                       3     // The SPD encoded value mapping to sdram capacity 2 Gb per die, based on DDR4 encoding rule
#define SPD_4Gb                       4     // The SPD encoded value mapping to sdram capacity 4 Gb per die, based on DDR4 encoding rule
#define SPD_8Gb                       5     // The SPD encoded value mapping to sdram capacity 8 Gb per die, based on DDR4 encoding rule
#define SPD_16Gb                      6     // The SPD encoded value mapping to sdram capacity 16 Gb per die, based on DDR4 encoding rule
#define SPD_32Gb                      7     // The SPD encoded value mapping to sdram capacity 32 Gb per die, based on DDR4 encoding rule
#define SPD_12Gb                      8     // The SPD encoded value mapping to sdram capacity 12 Gb per die, based on DDR4 encoding rule
#define SPD_24Gb                      9     // The SPD encoded value mapping to sdram capacity 24 Gb per die, based on DDR4 encoding rule
#define SPD_48Gb                      10    // The SPD encoded value mapping to sdram capacity 48 Gb per die, just valid for DDR5
#define SPD_64Gb                      11    // The SPD encoded value mapping to sdram capacity 64 Gb per die, just valid for DDR5

//
// The device width macros are applicable for both DDR4 and DDR5, because he SPD encoding rules for DDR4 and DDR5 are the same
//
#define SPD_DRAM_IO_WIDTH_X4           0     // The SPD encoded value mapping to SDRAM device width = 4 bits
#define SPD_DRAM_IO_WIDTH_X8           1     // The SPD encoded value mapping to SDRAM device width = 8 bits
#define SPD_DRAM_IO_WIDTH_X16          2     // The SPD encoded value mapping to SDRAM device width = 16 bits
#define SPD_DRAM_IO_WIDTH_X32          3     // The SPD encoded value mapping to SDRAM device width = 32 bits


#define SPD_CHNUMBER_PER_DIMM_FIELD_OFFSET                   5 // The field offset (in byte level) corresponding to channel number per DIMM
#define SPD_TO_ACTUAL_CHNUMBER_PER_DIMM_SHIFT_BASE           1 // The shift base for channel number per DIMM to convert SPD encoded value to actual value
#define SPD_TO_ACTUAL_PRIMARY_BUS_WIDTH_PER_CH_SHIFT_BASE    8 // The shift base for primary bus width per channel to convert SPD encoded value to actual value
#define SPD_TO_ACTUAL_DRAM_IO_WIDTH_SHIFT_BASE               4 // The shift base for DRAM IO width to convert SPD encoded value to actual value
//
// Structure used to map SPD encoded DRAM die count to actual die count
//
typedef struct {
  UINT8   SpdEncodedDie;
  UINT8   DramDieCount;
  UINT8   DeviceType;
} SPD_DIECOUNT_MAP;

//
// SDRAM number
//
typedef enum {
  PrimarySdramNumber,      //Indicate the Primary SDRAM
  SecondarySdramNumber,    //Indicate the Secondary SDRAM
  MaxSdramNumber
} SDRAM_NUM;

typedef enum {
  PprSoft,
  PprOneRowPerBankGroup,
  MaxPprTypeList
} PPR_TYPE_LIST;


//
// Function descriptions follows SPD spec:
//   DDR4 SPD Spec 4.0 Register Definitions
//   DDR5 SPD Spec 0.88 Register Definitions
//

/**
  Get the SPD version.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[out] Version     - SPD Version

  @retval EFI_SUCCESS       Successfully initialized SPD related functionality
  @retval EFI_DEVICE_ERROR  Error initializing devices
**/
EFI_STATUS
EFIAPI
SpdGetVersion (
  IN     UINT8      Socket,
  IN     UINT8      Chan,
  IN     UINT8      Dimm,
     OUT UINT8      *Version
  );

/**
  Get SDRAM base module type.

  Spec section:
    Byte 3 (0x003): Key Byte / Module Type

  Description:
    Base module type identifies the SDRAM memory module type, such as SPD_RDIMM or SPD_LRDIMM.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Type       - Pointer to base module type

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetBaseModuleType (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Type
  );

/**
  Get the minimum cycle time for the SDRAM module.

  Spec section:
    DDR4:
    Byte 18 (0x012): SDRAM Minimum Cycle Time (tCKAVGmin)
    DDR5:
    SDRAM Minimum Cycle Time (tCKAVGmin)
    Byte 20 (0x014): Least Significant Byte
    Byte 21 (0x015): Most Significant Byte

  Description:
    Minimum cycle time applies to all applicable components on the module, in picoseconds (ps).
  It applies to SDRAM and support components as well as the overall capability of the DIMM.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  Socket     - Socket ID
  @param[in]  Chan       - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Val        - Pointer to minimum cycle time value in picosecond

  @retval MRC_STATUS_SUCCESS         Read minimum cycle time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetMiniCycleTime (
  IN     PSYSHOST   Host,
  IN     UINT8      Socket,
  IN     UINT8      Chan,
  IN     UINT8      Dimm,
     OUT INT32      *Val
  );

/**
  Get device type of SDRAM.

  Spec section:
    DDR4:
    Byte 6 (0x006): Primary SDRAM Package Type
    Byte 10 (0x00A): Secondary SDRAM Package Type
    DDR5:
    Byte 4 (0x004): First SDRAM Density and Package
    Byte 8 (0x008): Second SDRAM Density and Package

  Description:
    Dram device type refers to signal loading type, 3ds type, and monolithic device type.

  @param[in]     Socket      - Socket ID
  @param[in]     Ch          - Channel on socket
  @param[in]     Dimm        - Dimm number
  @param[in]     SdramNum    - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    DeviceType  - The pointer to the encoding of device type
                               BIT0: SPD_MULTI_LOAD_TYPE
                               BIT1: SPD_3DS_TYPE
                               BIT7: SPD_NON_MONOLITHIC_TYPE

  @retval MRC_STATUS_SUCCESS  - DeviceType can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid encoding or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetDramDeviceType (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *DeviceType
  );

/**
  Performs SPD field read operations.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  FieldIndex  - SPD Field Index to be read
  @param[out] Buffer      - Pointer to buffer to store read value
  @param[out] BufLength   - Pointer to the size, in bytes, of Buffer

  @retval RETURN_SUCCESS         The read completed successful
  @retval EFI_DEVICE_ERROR       Error configuring or during lower level library transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_UNSUPPORTED        This Field Index is not supported
  @retval EFI_BUFFER_TOO_SMALL   Buffer Length not big enough
**/
EFI_STATUS
EFIAPI
SpdReadField (
  IN     UINT8  Socket,
  IN     UINT8  Chan,
  IN     UINT8  Dimm,
  IN     UINT16 FieldIndex,
  IN OUT VOID   *Buffer,
  IN OUT UINT8  *BufLength
  );

/**
  Performs SPD field UINT32 read operations.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  FieldIndex  - SPD Field Index to be read
  @param[out] Buffer      - Pointer to buffer to store read value

  @retval RETURN_SUCCESS         The read completed successful
  @retval EFI_DEVICE_ERROR       Error configuring or during lower level library transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_UNSUPPORTED        This Field Index is not supported
  @retval EFI_BUFFER_TOO_SMALL   Buffer Length not big enough
**/
EFI_STATUS
EFIAPI
SpdReadField32 (
  IN     UINT8  Socket,
  IN     UINT8  Chan,
  IN     UINT8  Dimm,
  IN     UINT16 FieldIndex,
  IN OUT UINT32 *Buffer
  );

/**
  Performs SPD field write operations.

  @param[in] Socket      - Socket ID
  @param[in] Chan        - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[in] FieldIndex  - SPD Field Index to be written
  @param[in] Buffer      - Pointer to buffer to write
  @param[in] BufLength   - The size, in bytes, of Buffer

  @retval RETURN_SUCCESS         The write completed successful.
  @retval EFI_DEVICE_ERROR       Error configuring or during lower level library transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_UNSUPPORTED        This Field Index is not supported
**/
EFI_STATUS
EFIAPI
SpdWriteField (
  IN UINT8  Socket,
  IN UINT8  Chan,
  IN UINT8  Dimm,
  IN UINT16 FieldIndex,
  IN VOID   *Buffer,
  IN UINT8  BufLength
  );

/**
  Check whether the target rank has CA[13] supported or not.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket to train
  @param[in] Ch      - DDR channel to setup
  @param[in] Dimm    - Dimm number the rank is on
  @param[in] Rank    - Rank number

  @retval TRUE       - Target rank supports CA[13]
  @retval FALSE      - Target rank doesn't support CA[13]
**/
BOOLEAN
EFIAPI
Ca13Enabled (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**
  Get die count of SDRAM.

  Spec section:
    DDR4:
    Byte 6 (0x006): Primary SDRAM Package Type
    Byte 10 (0x00A): Secondary SDRAM Package Type
    DDR5:
    Byte 4 (0x004): First SDRAM Density and Package
    Byte 8 (0x008): Second SDRAM Density and Package

  Description:
    Die count counts from single die to 8 die.

  @param[in]     Socket      - Socket ID
  @param[in]     Ch          - Channel on socket
  @param[in]     Dimm        - Dimm number
  @param[in]     SdramNum    - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    DieCount    - The pointer to the variable indicating the actual die count number of SDRAM

  @retval MRC_STATUS_SUCCESS  - DieCount can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetDramDieCount (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *DieCount
  );

/**
  Get actual column bits number of SDRAM.

  Spec section:
    DDR4:
    Byte 5 (0x005): SDRAM Addressing
    DDR5:
    Byte 5 (0x005): First SDRAM Addressing
    Byte 9 (0x009): Second SDRAM Addressing

  Description:
    Column bits number describes the number of SDRAM column address bits.

  @param[in]     Socket        - Socket ID
  @param[in]     Ch            - Channel on socket
  @param[in]     Dimm          - Dimm number
  @param[in]     SdramNum      - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out] NumColBits    - The pointer to the variable indicating the actual column bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - NumColBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetNumColBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *NumColBits
  );

/**
  Get actual row bits number of SDRAM.

  Spec section:
    DDR4:
    Byte 5 (0x005): SDRAM Addressing
    DDR5:
    Byte 5 (0x005): First SDRAM Addressing
    Byte 9 (0x009): Second SDRAM Addressing

  Description:
    Row bits number describes the number of SDRAM row address bits.

  @param[in]     Socket        - Socket ID
  @param[in]     Ch            - Channel on socket
  @param[in]     Dimm          - Dimm number
  @param[in]     SdramNum      - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    NumRowBits    - The pointer to the variable indicating the actual row bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - NumRowBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetNumRowBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *NumRowBits
  );

/**
  Get bank group bits number of SDRAM.

  Spec section:
    DDR4:
    Byte 4 (0x004): SDRAM Density and Banks
    DDR5:
    Byte 7 (0x007): First SDRAM Bank Groups & Banks Per Bank Group
    Byte 11 (0x00B): Second SDRAM Bank Groups & Banks Per Bank Group

  Description:
    It describes the number of bank groups.

  @param[in]     Socket        - Socket ID
  @param[in]     Ch            - Channel on socket
  @param[in]     Dimm          - Dimm number
  @param[in]     SdramNum      - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    BankGroupBits - The pointer to the variable indicating the bank group bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - BankGroupBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetBankGroupBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *BankGroupBits
  );

/**
  Get bank address bits number of SDRAM.

  Spec section:
    DDR4:
    Byte 4 (0x004): SDRAM Density and Banks
    DDR5:
    Byte 7 (0x007): First SDRAM Bank Groups & Banks Per Bank Group
    Byte 11 (0x00B): Second SDRAM Bank Groups & Banks Per Bank Group

  Description:
    It describes the number of bank address bits.

  @param[in]     Socket          - Socket ID
  @param[in]     Ch              - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[in]     SdramNum        - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    BankAddressBits - The pointer to the variable indicating the bank address bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - BankAddressBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetBankAddressBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *BankAddressBits
  );

/**
  Get RCD Vendor information

  Spec section:
    DDR4:
    Byte 133 (0x085) (Registered): Register Manufacturer ID Code, LSB
    Byte 134 (0x086) (Registered): Register Manufacturer ID Code, MSB
    Byte 133 (0x085) (Load Reduced): Register and Data Buffer Manufacturer ID Code, LSB
    Byte 134 (0x086) (Load Reduced): Register and Data Buffer Manufacturer ID Code, MSB
    DDR5:
    (Registered): Module Specific Device Information
    (Load Reduced): Module Specific Device Information

  Description:
    RCD vendor information refers to RCD manufacturer ID.

  @param[in]     Socket          - Socket ID
  @param[in]     Channel         - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[out]    RcdVendor       - The pointer to the variable indicating to RCD Vendor

  @retval MRC_STATUS_SUCCESS  - RCD Vendor can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetRcdVendor (
  IN     UINT8          Socket,
  IN     UINT8          Channel,
  IN     UINT8          Dimm,
     OUT UINT16         *RcdVendor
  );

/**
  Get RCD Vendor Revision

  Spec section:
    DDR4:
    Byte 135 (0x087) (Registered): Register Revision Number
    Byte 135 (0x087) (Load Reduced): Register Revision Number
    DDR5:
    (Registered): Module Specific Device Information
    (Load Reduced): Module Specific Device Information

  Description:
    RCD vendor information refers to RCD revision number, expressed in BCD.

  @param[in]     Socket          - Socket ID
  @param[in]     Channel         - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[out]    RcdRevision     - The pointer to the variable indicating to RCD Revision

  @retval MRC_STATUS_SUCCESS  - RCD Revision can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetRcdRevision (
  IN     UINT8          Socket,
  IN     UINT8          Channel,
  IN     UINT8          Dimm,
     OUT UINT8          *RcdRevision
  );


/**
  Get the encoded value of field specifying the sdram capacity per die based on DDR4 encoding rule.

  Spec section:
    DDR4:
    Byte 4 (0x004): SDRAM Density and Banks
    DDR5:
    Byte 4 (0x004): First SDRAM Density and Package
    Byte 8 (0x008): Second SDRAM Density and Package

  Description:
    It describes device density, which refers to sdram capacity per die, in Gbits.

  @param[in]     Socket                      - Socket ID
  @param[in]     Ch                          - Channel on socket
  @param[in]     Dimm                        - Dimm number
  @param[in]     SdramNum                    - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    EncondedDramCapacityDdr4    - The pointer to the variable indicating the encoded value of field specifying the sdram capacity per die

  @retval MRC_STATUS_SUCCESS  - EncodedDramCapacityDdr4 can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetEncodedDramCapacity (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *EncodedDramCapacityDdr4
  );

/**
  Get encoded value of dram IO width bits.

  Spec section:
    DDR4:
    Byte 12 (0x00C): Module Organization
    DDR5:
    Byte 6 (0x006): First SDRAM I/O Width
    Byte 10 (0x00A): Secondary SDRAM I/O Width

  Description:
    It describes the number of SDRAM I/O bits (DQ).

  @param[in]     Socket                  - Socket ID
  @param[in]     Ch                      - Channel on socket
  @param[in]     Dimm                    - Dimm number
  @param[in]     SdramNum                - Targrt SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    EncodedDramIOWidth      - The pointer to the variable indicating SPD encoded value of dram IO width bits

  @retval MRC_STATUS_SUCCESS  - EncodedDramIOWidth can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetEncodedDramIOWidth (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *EncodedDramIOWidth
  );

/**
  Get rank mix indicator of DIMM.

  Spec section:
    DDR4:
    Byte 12 (0x00C): Module Organization
    DDR5:
    (Common): Module Organization Byte 234 (0x0EA)

  Description:
    It describes whether the assembly has the same SDRAM density on all ranks or has different SDRAM densities on even and odd ranks.

  @param[in]     Socket              - Socket ID
  @param[in]     Ch                  - Channel on socket
  @param[in]     Dimm                - Dimm number
  @param[out]    RankMixIndicator    - The pointer to the variable indicating whether the target DIMM is asymmetirc

  @retval MRC_STATUS_SUCCESS  - RankMixIndicator can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetRankMixIndicator (
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
     OUT  BOOLEAN     *RankMixIndicator
  );

/**
  Return tRFC and nRFC from SPD based on refresh mode policy

  Spec section:
    DDR4:
    Byte 30 (0x01E): Minimum Refresh Recovery Delay Time (tRFC1min), LSB
    Byte 31 (0x01F): Minimum Refresh Recovery Delay Time (tRFC1min), MSB
    Byte 32 (0x020): Minimum Refresh Recovery Delay Time (tRFC2min), LSB
    Byte 33 (0x021): Minimum Refresh Recovery Delay Time (tRFC2min), MSB
    Byte 34 (0x022): Minimum Refresh Recovery Delay Time (tRFC4min), LSB
    Byte 35 (0x023): Minimum Refresh Recovery Delay Time (tRFC4min), MSB
    DDR5:
    SDRAM Minimum Refresh Recovery Delay Time (tRFC1min, tRFC1_slr min)
    SDRAM Minimum Refresh Recovery Delay Time (tRFC2min, tRFC2_slr min)
    SDRAM Minimum Refresh Recovery Delay Time (tRFCsbmin, tRFCsb_slr min)

  Description:
    It defines the minimum SDRAM Refresh Recovery Time Delay in nanoseconds (ns).

  @param[in]  Socket      - Socket to train
  @param[in]  Ch          - DDR channel to setup
  @param[in]  Dimm        - Dimm number the rank is on
  @param[in]  RefreshMode - Refresh mode policy setting
  @param[out] tRFC        - tRFC returned in units of ns
  @param[out] nRFC        - nRFC returned in units of tCLK

  @retval MRC_STATUS_SUCCESS if tRFC and nRFC are valid
  @retval MRC_STATUS_FAILURE otherwise
**/
MRC_STATUS
EFIAPI
SpdGetTrfc (
  IN     UINT8         Socket,
  IN     UINT8         Ch,
  IN     UINT8         Dimm,
  IN     REFRESH_MODE  RefreshMode,
     OUT UINT16        *tRFC,
     OUT UINT16        *nRFC
  );

/**
  Get Operable and Endurant encoding VDD.

  Spec section:
    DDR4:
    Byte 11 (0x00B): Module Nominal Voltage, VDD
    DDR5:
    Byte 16 (0x010): SDRAM Nominal Voltage, VDD

  Description:
    Operable: VDD voltage at which module operation is allowed using the performance values porgrammed in the SPD.
    Endurant: VDD voltage at which the module may be powered without adversely affecting the life expectancy or reliability.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] OperableEndurant     - Pointer to the value of Operable and Endurant

  @retval MRC_STATUS_SUCCESS         Read Operable and Endurant successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetOperableEndurant (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
     OUT UINT8          *OperableEndurant
  );

/**
  Get Number of package ranks per channel.

  Spec section:
    DDR4:
    Byte 12 (0x00C): Module Organization
    DDR5:
    (Common): Module Organization Byte 234 (0x0EA)

  Description:
    Package ranks per channel refers to the collections of devices on the module sharing common chip select signals.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] PackageRanks     - Pointer to the number of package ranks

  @retval MRC_STATUS_SUCCESS         Read package ranks number successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetPackageRanks (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
     OUT UINT8          *PackageRanks
  );

/**
  Get raw card reference design.

  Spec section:
    DDR4:
    Byte 130 (0x082) (Unbuffered): Reference Raw Card Used
    Byte 130 (0x082) (Registered): Reference Raw Card Used
    Byte 130 (0x082) (Load Reduced): Reference Raw Card Used
    DDR5:
    (Common): Reference Raw Card Used Byte 232 (0x0E8)

  Description:

  RawCardRefDesign encoding example:
  0b00000 - A
  0b00001 - B
  ...
  0b10011 - Y
  0b10100 - AA
  ...
  0b11101 - AK
  0b11110 - Reserved
  0b11111 - no reference used.

  @param[in]  Socket             - Socket ID
  @param[in]  Ch                 - Channel on socket
  @param[in]  Dimm               - DIMM on channel
  @param[out] RawCardRefDesign   - Pointer to the raw card reference design.

  @retval MRC_STATUS_SUCCESS         Read raw card reference design successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetRawCardRefDesign (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *RawCardRefDesign
  );

/**
  Get raw card reference design Revision.

  Spec section:
    DDR4:
    Byte 130 (0x082) (Unbuffered): Reference Raw Card Used
    Byte 130 (0x082) (Registered): Reference Raw Card Used
    Byte 130 (0x082) (Load Reduced): Reference Raw Card Used
    Byte 128 (0x080) (Unbuffered): Raw Card Extension, Module Nominal Height
    Byte 128 (0x080) (Registered): Raw Card Extension, Module Nominal Height
    Byte 128 (0x080) (Load Reduced): Raw Card Extension, Module Nominal Height
    DDR5:
    (Common): Reference Raw Card Used Byte 232 (0x0E8)

  Description:
    RawCardRefDesignRev directly refers to revision number. For example, RawCardRefDesignRev = 6 stands for Revision 6.

  @param[in]  Socket               - Socket ID
  @param[in]  Ch                   - Channel on socket
  @param[in]  Dimm                 - DIMM on channel
  @param[out] RawCardRefDesignRev  - Pointer to the raw card reference design Revision.

  @retval MRC_STATUS_SUCCESS         Read raw card reference design revision successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetRawCardRefDesignRev (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *RawCardRefDesignRev
  );

/**
  Get primay bus width in bits.

  Spec Section:
    DDR4:
    Byte 13 (0x00D): Module Memory Bus Width
    DDR5:
    (Common): Memory Channel Bus Width Byte 235 (0x0F3)

  Descirption:
    It describes the primary bus width.

  @param[in]  Socket                - Socket ID
  @param[in]  Ch                    - Channel on socket
  @param[in]  Dimm                  - DIMM on channel
  @param[out] PrimaryBusWidth       - Pointer to the primary bus width, in bits

  @retval MRC_STATUS_SUCCESS         Read primary bus width successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetPrimaryBusWidth (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *PrimaryBusWidth
  );

/**
  Get bus width extension in bits.

  Spec Section:
    DDR4:
    Byte 13 (0x00D): Module Memory Bus Width
    DDR5:
    (Common): Memory Channel Bus Width Byte 235 (0x0F3)

  Descirption:
    It describes the bus width extention, such as parity or ECC.

  @param[in]  Socket                 - Socket ID
  @param[in]  Ch                     - Channel on socket
  @param[in]  Dimm                   - DIMM on channel
  @param[out] BusWidthExtension      - Pointer to the bus width extension, in bits

  @retval MRC_STATUS_SUCCESS         Read bus width extension successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetBusWidthExtension (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *BusWidthExtension
  );

/**
  Get channel number per dimm.

  Spec Section:
    DDR4:
    Not supported
    DDR5:
    (Common): Memory Channel Bus Width Byte 235 (0x0F3)

  Descirption:
    It describes the number of channels on each module.

  @param[in]  Socket                 - Socket ID
  @param[in]  Ch                     - Channel on socket
  @param[in]  Dimm                   - DIMM on channel
  @param[out] ChannelNumPerDimm      - Pointer to the number of channels per dimm

  @retval MRC_STATUS_SUCCESS         Read number of channels per dimm successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetChannelNumPerDimm (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *ChannelNumPerDimm
  );

/**
  Get SPD spec revision.

  Spec section:
    DDR4:
    Byte 1 (0x001): SPD Revision
    DDR5:
    Byte 1 (0x001): SPD Revision for Base Configuration Parameters

  Description:
    SpecRevision is encoded with upper/lower nibble.

  Example:
  0x00 - Revision 0.0
  0x01 - Revision 0.1
  0x09 - Revision 0.9
  0x10 - Revision 1.0
  0x11 - Revision 1.1
  0xFF - Undefined.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] SpecRevision     - Pointer to the value of spec revision

  @retval MRC_STATUS_SUCCESS         Read revision successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetSpecRevision (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *SpecRevision
  );

/**
  Get the minimum SDRAM Row Precharge Delay Time.

  Spec section:
    DDR4:
    Byte 26 (0x01A): Minimum Row Precharge Delay Time (tRPmin)
    Byte 121 (0x079): Fine Offset for Minimum Row Precharge Delay Time (tRPmin)
    DDR5:
    SDRAM Minimum Row Precharge Delay Time (tRPmin)
    Byte 34 (0x022): Least Significant Byte
    Byte 35 (0x023): Most Significant Byte

  Description:
    It defines the minimum SDRAM Row Precharge Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrpVal           - Pointer to the minimum SDRAM Row Precharge Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTrp (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrpVal
  );

/**
  Get the Minimum SDRAM Active to Precharge Delay Time.

  Spec section:
    DDR4:
    Byte 27 (0x01B): Upper Nibbles for tRASmin and tRCmin
    Byte 28 (0x01C): Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
    DDR5:
    SDRAM Minimum Active to Precharge Delay Time (tRASmin)
    Byte 36 (0x024): Least Significant Byte
    Byte 37 (0x025): Most Significant Byte

    Description:
    It defines the minimum SDRAM Active to Precharge Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrasVal          - Pointer to the Minimum SDRAM Active to Precharge Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTras (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrasVal
  );

/**
  Get the minimum SDRAM Active to Active/Refresh Delay Time.

  Spec section:
    DDR4:
    Byte 27 (0x01B): Upper Nibbles for tRASmin and tRCmin
    Byte 29 (0x01D): Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
    Byte 120 (0x078): Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
    DDR5:
    SDRAM Minimum Active to Active/Refresh Delay Time (tRCmin)
    Byte 38 (0x026): Least Significant Byte
    Byte 39 (0x027): Most Significant Byte

  Description:
    It defines the minimum SDRAM Active to Active/Refresh Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrcVal           - Pointer to the minimum SDRAM Active to Active/Refresh Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTrc (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrcVal
  );

/**
  Get the minimum SDRAM RAS to CAS Delay Time.

  Spec section:
    DDR4:
    Byte 25 (0x019): Minimum RAS to CAS Delay Time (tRCDmin)
    Byte 122 (0x07A): Fine Offset for Minimum RAS to CAS Delay Time (tRCDmin)
    DDR5:
    SDRAM Minimum RAS to CAS Delay Time (tRCDmin)
    Byte 32 (0x020): Least Significant Byte
    Byte 33 (0x021): Most Significant Byte

  Description:
    It defines the minimum SDRAM RAS to CAS Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrcdVal          - Pointer to the minimum SDRAM RAS to CAS Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTrcd (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrcdVal
  );

/**
  Get the the minimum CAS Latency.

  Spec section:
    DDR4:
    Byte 24 (0x018): Minimum CAS Latency Time (tAAmin)
    Byte 123 (0x07B): Fine Offset for Minimum CAS Latency Time (tAAmin)
    DDR5:
    SDRAM Minimum CAS Latency Time (tAAmin)
    Byte 30 (0x01E): Least Significant Byte
    Byte 31 (0x01F): Most Significant Byte

  Description:
    It defines the minimum CAS Latency in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TaaVal           - Pointer to the minimum CAS Latency in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read CAS Latency successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTaa (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TaaVal
  );

/**
  Get supported CAS Latency (CL) values with one bit per possible CAS Latency.

  Spec section:
    DDR4:
    Byte 20 (0x014): CAS Latencies Supported, First Byte
    Byte 21 (0x015): CAS Latencies Supported, Second Byte
    Byte 22 (0x016): CAS Latencies Supported, Third Byte
    Byte 23 (0x017): CAS Latencies Supported, Fourth Byte
    DDR5:
    SDRAM CAS Latencies Supported
    Byte 24 (0x018): First Byte
    Byte 25 (0x019): Second Byte
    Byte 26 (0x01A): Third Byte
    Byte 27 (0x01B): Fourth Byte
    Byte 28 (0x01C): Fifth Byte

  Description:
    It define which CAS Latency (CL) values are supported with one bit per possible CAS Latency.
    0 : means this CAS Latency is not supported.
    1 : means this CAS Latency is supported.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] CasSup           - Pointer to supported CAS Latency bitmap values

  @retval MRC_STATUS_SUCCESS         Read supported CAS Latency values successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetCasSupported (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT64   *CasSup
  );

/**
  Get whether thermal sensor is installed.

  Spec section:
    DDR4:
    Byte 14 (0x00E): Module Thermal Sensor
    DDR5:
    (Common): Module Device Information

  Description:
    SensorIndex refers to TS0 or TS1 on DDR5. Its value is ignored for DDR4.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[in]  SensorIndex      - 0: TS0; 1: TS1

  @retval TRUE         - installed
  @retval FALSE        - not installed
**/
BOOLEAN
EFIAPI
SpdIsThermalSensorInstalled (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
  IN     UINT8    SensorIndex
  );

/**
  Get whether a specific PPR feature is supported.

  Spec section:
    DDR4:
    Byte 9 (0x009): Other SDRAM Optional Features
    DDR5:
    Byte 12 (0x00C): SDRAM Optional Features

  Description:
    PPR: Post package repair
  PprType refers to either PprSoft or PprOneRowPerBankGroup.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[in]  PprType          - PPR type in PPR_TYPE_LIST

  @retval TRUE        - supported
  @retval FALSE       - not supported
**/
BOOLEAN
EFIAPI
SpdIsPprSupported (
  IN     UINT8         Socket,
  IN     UINT8         Ch,
  IN     UINT8         Dimm,
  IN     PPR_TYPE_LIST PprType
  );

/**
  Get module manufacturing location.

  Spec section:
    DDR4:
    Byte 322 (0x142): Module Manufacturing Location
    DDR5:
    Byte 514 (0x202): Module Manufacturing Location

  Description:
    The individual manufacturer keeps track of manufacturing location and its appropriate decode
  represented in ModLoc.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] ModLoc           - Pointer to module manufacturing location

  @retval MRC_STATUS_SUCCESS         Read module manufacturing location successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleManufacturingLocation (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *ModLoc
  );

/**
  Get module manufacturing date.

  Spec section:
    DDR4:
    Bytes 323~324 (0x143~0x144): Module Manufacturing Date
    DDR5:
    Bytes 515~516 (0x203~0x204): Module Manufacturing Date

  Description:
    ModDate is represented in Binary Coded Decimal (BCD).
  For example, 0x4714 stands for week47 in year 2014.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] ModDate          - Pointer to module manufacturing date

  @retval MRC_STATUS_SUCCESS         Read module manufacturing date successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleManufacturingDate (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *ModDate
  );

/**
  Get module serial number.

  Spec section:
    DDR4:
    Bytes 325~328 (0x145~0x148): Module Serial Number
    DDR5:
    Bytes 517~520 (0x205~0x208): Module Serial Number

  Description:
    ModSerNum refers to the unique serial number for the module.
  ModSerNum has to point to an array with size of SPD_MODULE_SERIAL.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] ModSerNum        - Pointer to module serial number

  @retval MRC_STATUS_SUCCESS         Read module serial number successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleSerialNumber (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *ModSerNum
  );

/**
  Get module manufacturer ID.

  Spec section:
    DDR4:
    Byte 320 (0x140): Module Manufacturer ID Code, LSB
    Byte 321 (0x141): Module Manufacturer ID Code, MSB
    DDR5:
    Byte 512 (0x200): Module Manufacturer ID Code, First Byte
    Byte 513 (0x201): Module Manufacturer ID Code, Second Byte

  Description:
    ManufacturerId consists of 2 bytes, following encoding format in JEP-106.
  For example, 0x0400 refers to Fujitsu.

  @param[in]  Socket          - Socket ID
  @param[in]  Ch              - Channel on socket
  @param[in]  Dimm            - DIMM on channel
  @param[out] ManufacturerId  - Pointer to manufacturer ID

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleManufacturerId (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT16       *ManufacturerId
  );

/**
  Get module manufacturer part number.

  Spec section:
    DDR4:
    Bytes 329~348 (0x149~15C): Module Part Number
    DDR5:
    Bytes 521~550 (0x209~0x226): Module Part Number

  Description:
    Part number is encoded in ASCII format.
  PartNum has to point to an array. Array size is defined by SPD_MODULE_PART_DDR4 for DDR4, SPD_MODULE_PART_DDR5 for DDR5.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] PartNum    - Pointer to manufacturer part number array

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModulePartNum (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *PartNum
  );

/**
  Get dram stepping info.

  Spec section:
    DDR4:
    Byte 352 (0x160): DRAM Stepping
    DDR5:
    Byte 554 (0x22A): DRAM Stepping

  Description:
    Byte defines the vendor die revision level of the DRAMs.

  Examples:
    0x00 - Stepping 0
    0x01 - Stepping 1
    0x31 - Stepping 3.1
    0xA3 - Stepping A3
    0xB1 - Stepping B1
    0xFF - Stepping information not provided

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Stepping   - Pointer to dram stepping info

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetDramStepping (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Stepping
  );

/**
  Get RCD device type.

  Spec section:
    DDR4:
    not supported
    DDR5:
    (Registered): Module Specific Device Information
    (Load Reduced): Module Specific Device Information

  Description:
    Function always returns 0x0000 - DDR5RCD01 (JESD82-511).

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] RcdType    - Pointer to RCD device type

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetRcdDeviceType (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *RcdType
  );

/**
  Get SDRAM module nominal height.

  Spec section:
    DDR4:
    Byte 128 (0x080) (Unbuffered): Raw Card Extension, Module Nominal Height
    Byte 128 (0x080): (Registered): Raw Card Extension, Module Nominal Height 
    Byte 128 (0x080) (Load Reduced): Raw Card Extension, Module Nominal Height
    DDR5:
    Byte 230 (0x0E6): (Common) Module Nominal Height

  Description:
    This byte defines the nominal height (A dimension) in millimeters of the fully assembled module
  including heat spreaders or other added components.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Height     - Pointer to nominal height in milimeters (mm)

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetNominalHeight (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Height
  );

/**
  Get SDRAM module type.

  Spec section:
    DDR4:
    Byte 2 (0x002): Key Byte / DRAM Device Type
    DDR5:
    Byte 2 (0x002): Key Byte / Host Bus Command Protocol Type

  Description:
    This byte is the key byte used by the system BIOS to determine how to interpret all other bytes in
  the SPD EEPROM.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Type       - Pointer to module type

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleType (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Type
  );

/**
  Get manufacturer ID of the DRAM.

  Spec section:
    DDR4:
    Byte 350 (0x15E): DRAM Manufacturer ID Code, LSB
    Byte 351 (0x15F): DRAM Manufacturer ID Code, MSB
    DDR5:
    Byte 552 (0x228): DRAM Manufacturer ID Code, First Byte
    Byte 553 (0x229): DRAM Manufacturer ID Code, Second Byte

  Description:
    This two-byte field indicates the manufacturer of the DRAM on the module, and shall be encoded
  as follows: the first byte is the number of continuation bytes indicated in JEP-106; the second byte
  is the last non-zero byte of the manufacturer ID code, again as indicated in JEP-106.

  @param[in]  Socket              - Socket ID
  @param[in]  Ch                  - Channel on socket
  @param[in]  Dimm                - DIMM on channel
  @param[out] DramManufacturerId  - Pointer to DRAM manufacturer ID

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetDramManufacturerId (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT16       *DramManufacturerId
  );

#endif // #ifndef _SPD_DECODE_LIB_H_
