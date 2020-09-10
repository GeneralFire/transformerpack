/** @file
  NGNDimmPlatformCfgData.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation. <BR>

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

#ifndef _NGN_DIMM_PLATFORM_CONFIG_DATA_H_
#define _NGN_DIMM_PLATFORM_CONFIG_DATA_H_



#define NGN_PLATFORM_CAPABILITY_SIG       SIGNATURE_32('P', 'C', 'A', 'P')
#define NGN_DIMM_CONFIGURATION_HEADER_SIG SIGNATURE_32('D', 'M', 'H', 'D')
#define NGN_DIMM_CURRENT_CONFIG_SIG       SIGNATURE_32('C', 'C', 'U', 'R')
#define NGN_DIMM_CONFIGURATION_INPUT_SIG  SIGNATURE_32('C', 'I', 'N', '_')
#define NGN_DIMM_OUTPUT_CONFIG_SIG        SIGNATURE_32('C', 'O', 'U', 'T')


#define NGN_DIMM_MEMORY_VOLATILE_TYPE   1
#define NGN_DIMM_MEMORY_PERSISTENT_TYPE 2

#define INTEL_PARTITION_SIZE 0x10000

#define NGN_CURRENT_CONFIGURATION_SIGNATURE 0x52554343
#define NGN_CONFIGURATION_HEADER_SIGNATURE  0x44484D44
#define NGN_CONFIGURATION_INPUT_SIGNATURE   0x5F4E4943
#define NGN_CONFIGURATION_OUTPUT_SIGNATURE  0x54554F43

#define CFG_CUR 0x00
#define CFG_IN  0x01
#define CFG_OUT 0x02
#define CFG_OEM 0x03

#define BIOS_PARTITION 0x00
#define OS_PARTITION   0x01
#define FNV_SIZE       0x01
#define FNV_DATA       0x00

#define BYTES_128 0x80

//Error codes
#define NOT_PROCESSED                      0x00
#define PARTITION_TOO_BIG                  0x01
#define INTERLEAVE_PARTICIPANT_UNAVAILABLE 0x02
#define INTERLEAVE_PARAMETER_MISMATCH      0x03
#define FNV_FW_ERROR                       0x04
#define INSUFFICIENT_SILICON_RESOURCES     0x05
#define SPA_UNAVIALABLE                    0x06
#define MISSING_CFG_REQUEST                0x07
#define PARTITIONING_REQUEST_FAILED        0x08
#define PARTITIONING_SUCCESS               0x09
#define CHANNEL_INTERLEAVE_MISMATCH        0x0A
#define NM_POPULATION_MISMATCH             0x0B
#define INTERLEAVE_FORMAT_NOT_VALID        0x0C
#define MIRROR_MAPPING_FAILED              0x0D
#define INTERLEAVE_NOT_ALIGNED             0x0E
#define PMEM_REGIONS_NOT_PROCESSED         0x0F
#define SECURITY_STATE_LOCKED              0x10

#define VALIDATION_SUCCESS 0xFF
#define OPERATION_SUCCESS  0xFF

#define NGN_CFGCUR_UNDEFINED                            0
#define NGN_CFGCUR_RESPONSE_SUCCESS                     1
#define NGN_CFGCUR_RESPONSE_ALL_DIMM_NOT_FOUND          3
#define NGN_CFGCUR_RESPONSE_PM_NOT_MAPPED               4
#define NGN_CFGCUR_RESPONSE_CFGIN_ERROR_CCUR_USED       6
#define NGN_CFGCUR_RESPONSE_CFGIN_ERROR_DIMM_NOT_MAPPED 7
#define NGN_CFGCUR_RESPONSE_CFGIN_CHECKSUM_ERROR        8
#define NGN_CFGCUR_RESPONSE_CFGIN_REVISION_ERROR        9
#define NGN_CFGCUR_RESPONSE_CCUR_CHECKSUM_NOT_VALID     10
#define NGN_CFGCUR_RESPONSE_HEALTH_ISSUE_OR_CFG_CHANGE  11
#define NGN_CFGCUR_RESPONSE_POPULATION_ISSUE            12
#define NGN_CFGCUR_RESPONSE_NM_FM_RATIO_UNSUPPORTED     13
#define NGN_CFGCUR_RESPONSE_SKU_LIMIT_VIOLATION         14
#define NGN_CFGCUR_RESPONSE_VOL_MEM_NOT_MAPPED          15

#define NGN_CFGOUT_VLD_STAT_UNDEFINED               0
#define NGN_CFGOUT_VLD_STAT_SUCCESS                 1
#define NGN_CFGOUT_VLD_STAT_ERROR                   2
#define NGN_CFGOUT_VLD_STAT_NM_FM_RATIO_UNSUPPORTED 6
#define NGN_CFGOUT_VLD_STAT_SKU_LIMIT_VIOLATION     7
#define NGN_CFGOUT_VLD_STAT_POPULATION_ISSUE        8

#define NGN_PART_SIZE_STAT_UNDEFINED                      0
#define NGN_PART_SIZE_STAT_SUCCESS                        1
#define NGN_PART_SIZE_STAT_INTERLEAVE_PARTICIPANT_MISSING 3
#define NGN_PART_SIZE_STAT_INTERLEAVE_SET_NOT_FOUND       4
#define NGN_PART_SIZE_STAT_PARTITION_SIZE_TOO_BIG         5
#define NGN_PART_SIZE_STAT_AEP_FW_ERROR                   6
#define NGN_PART_SIZE_STAT_INSUFFICIENT_SILICON_RESOURCES 7

#define NGN_INT_CH_STAT_UNDEFINED                      0
#define NGN_INT_CH_STAT_SUCCESS                        1
#define NGN_INT_CH_STAT_NOT_PROCESSED                  2
#define NGN_INT_CH_STAT_MATCHING_DIMM_MISSING          3
#define NGN_INT_CH_STAT_INTERLEAVE_PARAMETERS_MISMATCH 4
#define NGN_INT_CH_STAT_INSUFFICIENT_SILICON_RESOURCES 5
#define NGN_INT_CH_STAT_SYSTEM_ADDRESS_UNAVAILABLE     6
#define NGN_INT_CH_STAT_MIRROR_MAPPING_FAILED          7
#define NGN_INT_CH_STAT_PARTITIONING_FAILED            8
#define NGN_INT_CH_STAT_CFG_REQ_MISSING                9
#define NGN_INT_CH_STAT_CHANNEL_INTERLEAVE_MISMATCH    10
#define NGN_INT_CH_STAT_PARTITION_OFFSET_NOT_ALIGNED   11
#define NGN_INT_CH_STAT_UNSUPPORTED                    12
#define NGN_INT_CH_STAT_CLUSTERING_SETTING_MISMATCH    14

#define NGN_PCAT_TYPE_PLATFORM_CAPABILITY_INFORMATION        0
#define NGN_PCAT_TYPE_INTERLEAVE_CAPABILITY_INFORMATION      1
#define NGN_PCAT_TYPE_CONFIG_MANAGEMENT_ATTRIBUTES_EXTENSION 3
#define NGN_PCAT_TYPE_DIMM_PARTITION_SIZE_CHANGE             4
#define NGN_PCAT_TYPE_DIMM_INTERLEAVE_INFORMATION            5
#define NGN_PCAT_TYPE_DIE_SKU_INFORMATION                    6

#define NGN_MAX_MANUFACTURER_STRLEN      2
#define NGN_MAX_SERIALNUMBER_STRLEN      4
#ifdef DDR5_SUPPORT
#define NGN_MAX_PARTNUMBER_STRLEN        30
#else
#define NGN_MAX_PARTNUMBER_STRLEN        20
#endif //#ifdef DDR5_SUPPORT
#define NGN_MAX_UID_STRLEN               9
#define NGN_FW_VER_LEN                   4
#define NGN_MAX_MANUFACTURER_DATE_STRLEN 2
#define NGN_MAX_MANUFACTURER_LOC_STRLEN  1

#define NGN_BYTE0 0
#define NGN_BYTE1 1
#define NGN_BYTE2 2
#define NGN_BYTE3 3

#define NGN_LOG_TYPE_NUM      2
#define NGN_LOG_LEVEL_NUM     2
#define NGN_RETRIEVE_LOG_DATA 0
#define NGN_RETRIEVE_LOG_INFO 1

#define INTERLEAVE_FORMAT_64B  BIT0
#define INTERLEAVE_FORMAT_128B BIT1
#define INTERLEAVE_FORMAT_256B BIT2
#define INTERLEAVE_FORMAT_4KB  BIT6
#define INTERLEAVE_FORMAT_1GB  BIT7

#define  INTERLEAVE_ALIGNMENT_SIZE 30 //Interleave alignment size in 2^n bytes, n = 26 for 64MB, n = 27 for 128MB

#define INTERLEAVE_TYPE_VOLATILE           1
#define INTERLEAVE_TYPE_PERSISTENT         2
#define INTERLEAVE_TYPE_WBCACHE_PERSISTENT 3

#define NGN_DIMM_NO_ERROR               0x00
#define NGN_DIMM_COMM_FAILED            0x01
#define NGN_DIMM_MEDIA_ERROR            0x02
#define NGN_DIMM_SIGNATURE_ERROR        0x03
#define NGN_DIMM_HDR_REVISION_ERROR     0x04
#define NGN_DIMM_CCUR_REVISION_ERROR    0x05
#define NGN_DIMM_CIN_REVISION_ERROR     0x06
#define NGN_DIMM_COUT_REVISION_ERROR    0x07
#define NGN_DIMM_CHECKSUM_ERROR         0x08
#define NGN_DIMM_DATA_OFFSET_ERROR      0x09
#define NGN_DIMM_DATA_SIZE_ERROR        0x0A
#define NGN_DIMM_DATA_ERROR             0x0B
#define NGN_DIMM_UNKNOWN_ERROR          0xFF

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Following structures are used to parse the data defined in the the CR Software-Firmware Management Interface Spec
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// NGN DIMM Config Header
//
typedef struct {
  UINT32 Signature;       //!< Signature for this table
  UINT32 Length;          //!< Length in bytes for entire table. It implies the number of Entry fields at the end of the table.
  /**
    Bits [7:4] - Major Revision
    Bits [3:0] - Minor Revision
  **/
  UINT8  Revision;
  UINT8  Checksum;        //!< Entire table must sum to zero
  UINT8  OemId[6];
  UINT64 OemTableId;      //!< Manufacturer model ID
  UINT32 OemRevision;     //!< Revision of OEM Table ID
  UINT32 CreatorId;       //!< Vendor ID of utility that created the table
  UINT32 CreatorRevision; //!< Revision of utility that created the table
} NGN_DIMM_PLATFORM_CFG_HEADER;

//
// NGN DIMM Config Description
//
typedef struct {
  UINT32 CurrentConfDataSize;    //!< Size of data area allocated to the current configuration information in bytes
  UINT32 CurrentConfStartOffset; //!< Starting location of current configuration. Valid if the size is non-zero
  UINT32 ConfRequestDataSize;    //!< Size of data area allocated to the configuration request area in bytes
  UINT32 ConfRequestDataOffset;  //!< Starting location of configuration request data. Valid if the size is non-zero
  UINT32 ConfResponseDataSize;   //!< Size of data area allocated to the configuration response area in bytes
  UINT32 ConfResponseDataOffset; //!< Starting location of configuration response data. Valid if the size is non-zero
} NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY;

//
// NGN DIMM Current Config
//
typedef struct {
  UINT16 ConfigurationStatus;
  UINT8  Reserved[2];
  UINT64 VolatileMemSizeIntoSpa;   //!< Total amount of 2LM size from the NGN DIMM mapped into the SPA
  UINT64 PersistentMemSizeIntoSpa; //!< Total amount of PM size from NGN DIMM mapped into the SPA
} NGN_DIMM_PLATFORM_CFG_CURRENT_BODY;


typedef struct {
  NGN_DIMM_PLATFORM_CFG_HEADER       PlatformCfgHeader;
  NGN_DIMM_PLATFORM_CFG_CURRENT_BODY CurrentCfgBody;
} NGN_DIMM_PLATFORM_CURRENT_CFG;

//
// NGN DIMM Configuration Request
//
typedef struct {
  /**
    Request sequence number. The BIOS copies this sequence number to the response structure once the BIOS
    completes processing the request input
  **/
  UINT32 SequenceNumber;
  UINT8  Reserved[8];
} NGN_DIMM_PLATFORM_CFG_INPUT_BODY;

//
// NGN DIMM Configuration Output
//
typedef struct {
  UINT32 SequenceNumber;  //!< Copy of the request sequence number to indicate that the BIOS has processed the request
  UINT8  ValidationStatus;
  UINT8  Reserved[7];
} NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY;

//
// NGN DIMM PCAT Table Header
typedef struct {
  UINT16 Type;
  UINT16 Length;
} NGN_PCAT_HEADER;

//
// NGN DIMM Partition Size Change PCAT Table
//
typedef struct {
  /**
    Valid on config output structure

    Byte [1-0]:
    0 - Undefined
    1 - Success
    2 - Reserved
    3 - All the DIMMs in the interleave set not found
    4 - Matching Interleave set not found (Matching DIMMs found)
    5 - Total partition size defined in the interleave set exceeds the partition size input
    6 - NGN DIMM FW returned error
    7 - insufficient number of DRAM Decoders available to map all the DIMMs in the
        interleave set. Repartition not performed
    8 - Persistent memory partition size is not aligned to the Interleave Alignment
        size specified in the Interface Capability Information Table

    Byte [3-2]: Intel NVDIMM FW Error Response Code

  **/
  UINT32 ResponseStatus;
  /**
    Size of persistent memory partition on the request and actually allocated persistent memory size on the response
  **/
  UINT64 PartitionSize;
} NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT;

//
// NGN DIMM Interleave Information PCAT Table
//
typedef struct {
  /**
    Logical index number
  **/
  UINT16 InterleaveSetIndex;
  /**
    Total number of DIMMs participating in this interleave set
  **/
  UINT8  NumOfDimmsInInterleaveSet;
  /**
    2 - App Direct Mode
    3 - App Direct WB Mode
  **/
  UINT8  InterleaveMemoryType;
  /**
    Interleave size to be used for this interleave set:
      Byte[0]: Supported channel interleave size:
        Bit[0-1]: Reserved
        Bit[2]: 256B
        Bit[3-5]: Reserved
        Bit[6]: 4KB
        Bit[7]: Reserved

      Byte[1]: Supported iMC interleave sizes:
        Bit[0-1]: Reserved
        Bit[2]: 256B
        Bit[3-5]: Reserved
        Bit[6]: 4KB
        Bit[7]: Reserved
  **/
  UINT16 InterleaveSize;
  UINT8  Reserved[3];
  /**
    Config Output:
    0 - Information not processed
    1 - Successfully interleaved the request
    2 - Unable to find matching DIMMs in the interleave set
    3 - Matching DIMMs found, but interleave information does not match
    4 - Insufficient numnber of DRAM decoders available to map all the DIMMs in the interleave set
    5 - Memory mapping failed due to unavailable system address space
    6 - Mirror mapping failed due to unavailable resources
  **/
  UINT8  InterleaveChangeResponseStatus;
  /**
    Clustering settings when the interleave set was created. Should the clustering settings change,
    the PM will be unreadable and an error will be returned. Only applies to CCUR, COUT:
      Bits[1-0] – Sub-NUMA Clustering Mode
        00b - None
        01b – SNC2
        10b – SNC4
        11b - Reserved

      Bits[3-2]: UMA Based Clustering Mode
        00b - None
        01b - Hemisphere
        10b - Quadrant
        11b - Reserved
  **/
  UINT8  ClusteringSettings;
  UINT8  Reserved1[9];
} NGN_DIMM_INTERLEAVE_INFORMATION_PCAT;

//
// SPD manufacturing date code byte definitions.
//
typedef struct {
  UINT8 Year;                             // Year represented in BCD (00h = 2000)
  UINT8 Week;                             // Year represented in BCD (47h = week 47)
} NGN_DIMM_SPD_MFG_DATE;

//
// SPD manufacturing information definitions.
//
typedef struct {
  UINT16                MfgIdCode;                                 // Module Manufacturer ID Code (byte 320, 321)
  UINT8                 MfgLocation;                               // Module Manufacturing Location (byte 322)
  NGN_DIMM_SPD_MFG_DATE MfgDate;                                   // Module Manufacturing Year, in BCD (range: 2000-2255) (byte 323, 324)
  UINT8                 SerialNumber[NGN_MAX_SERIALNUMBER_STRLEN]; // Module Serial Number (byte 325, 326, 327, 328)
} NGN_DIMM_SPD_UNIQUE_DIMM_ID;                                     // This format matches the order of SPD bytes 320 to 328 from low to high (i.e., showing the lowest or
                                                                   // first byte on the left).


//
// DIMM Location Interleave Information
// Only valid in CCUR.
// Populated by BIOS on successful request.
// Used by BIOS to determine if the DCPMM location has changed and if so, return error in CCUR.
// Used by management SW to provide guidance on how to correct issues related to DCPMM movement.
// Byte[0] - Socket ID
// Byte[1] - Die ID
// Byte[2] - iMC ID
// Byte[3] - Channel ID
// Byte[4] - Slot ID
// Byte[5-7] - Reserved
// Each ID is relative to the immediate parent.
//
typedef struct {
  UINT8 SocketId;
  UINT8 DieId;
  UINT8 ImcId;
  UINT8 ChannelId;
  UINT8 SlotId;
  UINT8 Reserved[3];
} NGN_DIMM_LOCATION;

//
// NGN DIMM Interleave Information
//
typedef struct {
  /**
    SPD manufacturing information - DIMM Unique Identifier
  **/
  NGN_DIMM_SPD_UNIQUE_DIMM_ID DimmUniqueId;
  /**
    DIMM Location Interleave Information
  **/
  NGN_DIMM_LOCATION           DimmLocation;
  UINT8                       Reserved[15];
  /**
    Logical offset from the base of the partition type
  **/
  UINT64                      PartitionOffset;
  /**
    Size in bytes contributed by this DIMM for this interleave set
  **/
  UINT64                      PartitionSize;
} NGN_DIMM_INTERLEAVE_ID;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Following structures are used to represent the data defined in the the CR Software-Firmware Management Interface Spec
// v0.6c in the Host structure.  These data structures remove some fields that are not required for the Host structure to
// optimize the space used by the Host structure.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// HOST NGN DIMM Current Config
//
typedef struct {
  UINT16 ConfigurationStatus;
  UINT16 VolatileMemSizeIntoSpa;   //!< Total amount of 2LM size from the NGN DIMM mapped into the SPA ; 64-bit field in interface specification ; Store in 64MB granularity ; 16-bits can store 2TB DIMM
  UINT16 PersistentMemSizeIntoSpa; //!< Total amount of PM size from NGN DIMM mapped into the SPA ; 64-bit field in interface specification ; Store in 64MB granularity ; 16-bits can store 2TB DIMM
} NGN_DIMM_PLATFORM_CFG_CURRENT_BODY_HOST;

//
// HOST NGN DIMM Configuration Request
//
typedef struct {
  /**
    Request sequence number. The BIOS copies this sequence number to the response structure once the BIOS
    completes processing the request input
  **/
  UINT32 SequenceNumber;
} NGN_DIMM_PLATFORM_CFG_INPUT_BODY_HOST;

//
// HOST NGN DIMM Configuration Output
//
typedef struct {
  UINT32 SequenceNumber;  //!< Copy of the request sequence number to indicate that the BIOS has processed the request
  UINT8  ValidationStatus;
} NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY_HOST;

//
// HOST NGN DIMM Partition Size Change PCAT Table
//
typedef struct {
  BOOLEAN RecPresent;
  BOOLEAN Valid;
  /**
    Size of persistent memory partition on the request and actually allocated persistent memory size on the response
  **/
  UINT16  PartitionSize;     // 64-bit field in interface specification ; Store in 64MB granularity ; 16-bits can store 2TB DIMM
  UINT16  ResponseStatus;
} NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT_HOST;

//
// HOST NGN DIMM Interleave Info
//

//
// NGN DIMM Interleave Information PCAT Table
//
typedef struct {
  BOOLEAN RecPresent;
  BOOLEAN Valid;
  BOOLEAN Processed;
  UINT32  SadLimit;
  /**
    Logical index number
  **/
  UINT16  InterleaveSetIndex;
  /**
    Total number of DIMMs participating in this interleave set
  **/
  UINT8   NumOfDimmsInInterleaveSet;
  /**
    1 - Volatile
    2 - Persistent
  **/
  UINT8   InterleaveMemoryType;
  /**
    Interleave size to be used for this interleave set:
      Byte[0]: Supported channel interleave size:
        Bit[0-1]: Reserved
        Bit[2]: 256B
        Bit[3-5]: Reserved
        Bit[6]: 4KB
        Bit[7]: Reserved

      Byte[1]: Supported iMC interleave sizes:
        Bit[0-1]: Reserved
        Bit[2]: 256B
        Bit[3-5]: Reserved
        Bit[6]: 4KB
        Bit[7]: Reserved
  **/
  UINT16  InterleaveSize;
  /**
    Config Output:
    0 - Information not processed
    1 - Successfully interleaved the request
    2 - Unable to find matching DIMMs in the interleave set
    3 - Matching DIMMs found, but interleave information does not match
    4 - Insufficient numnber of DRAM decoders available to map all the DIMMs in the interleave set
    5 - Memory mapping failed due to unavailable system address space
    6 - Mirror mapping failed due to unavailable resources
  **/
  UINT8   InterleaveChangeResponseStatus;
} NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST;

typedef struct {
  UINT8  Socket;
  UINT8  Channel;
  UINT8  Dimm;
  UINT16 PartitionOffset;     // 64-bit field in interface specification ; Store in 64MB granularity ; 16-bits can store 2TB DIMM
  UINT16 PartitionSize;       // 64-bit field in interface specification ; Store in 64MB granularity ; 16-bits can store 2TB DIMM
} NGN_DIMM_INTERLEAVE_ID_HOST;


#pragma pack()

#endif
