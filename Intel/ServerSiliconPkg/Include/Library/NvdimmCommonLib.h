/** @file

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
#ifndef _NVDIMM_COMMON_LIB_H_
#define _NVDIMM_COMMON_LIB_H_

#include <Library/UefiLib.h>
#include <Library/MemFmcIpLib.h>
#include <Guid/MemoryMapData.h>
#include <UncoreCommonIncludes.h>

#define NVDIMM_INFO_STR(module)  "[NVDIMM] (" module ") "
#define NVDIMM_ERROR_STR(module) "[NVDIMM] (" module ") ERROR: "

#define SPA_NOT_MAPPED           0x0
#define MAX_PMEM_RGN_PER_DIMM      2

#define NVDIMM_FLAG_ENABLED               0x00
#define NVDIMM_FLAG_INCLUDE_DISABLED      0x01
#define NVDIMM_FLAG_ALL                   (NVDIMM_FLAG_INCLUDE_DISABLED)
#define NVDIMM_FLAG_DUMMY                 0x02
#define NVDIMM_FLAG_ANY_DIMM              0x04

#define MAX_PMEM_INTERLEAVE_SET   (MAX_SOCKET * MAX_MC_CH * MAX_IMC * MAX_PMEM_RGN_PER_DIMM)

typedef struct {
  UINT64 pCommand;        // SPA of the command register for the NVMDIMM
  UINT64 pNonce0;         // SPA of the nonce0 register for the NVMDIMM
  UINT64 pNonce1;         // SPA of the nonce1 register for the NVMDIMM
  UINT64 pInPayload[NUM_PAYLOAD_REG];  // SPA of the 32 input payload registers (write only)
  UINT64 pStatus;         // SPA of the status register for the NVMDIMM
  UINT64 pOutPayload[NUM_PAYLOAD_REG]; // SPA of the 32 input payload registers (read only)
  UINT64 LargeInPayload;  //SPA of theLarge InPayload region for the NVMDIMM
  UINT64 LargeOutPayload; // SPA of the Large OutPayload region for the NVMDIMM
  UINT64 pInPayloadOrder[NUM_PAYLOAD_REG];
  UINT64 pOutPayloadOrder[NUM_PAYLOAD_REG];
  UINT8  InPayloadOrderLut[NUM_PAYLOAD_REG];
  UINT8  OutPayloadOrderLut[NUM_PAYLOAD_REG];
} CR_MAILBOX; //TODO JEDEC - needed only in Crystal Ridge

typedef struct {
  BOOLEAN IsSetPassphrase;
  UINT8 Passphrase[32]; //FIS_S3_PASSPHRASE_SIZE
} S3_INFO; //needed only in Crystal Ridge

typedef struct {
  UINT64      SADPMemBase;         // SAD Rule PMEM Base address
  UINT64      DPABase;             // DPA start address for this PMEM
  UINT8       IntDimmNum;          // Dimm Number in the interleave set (not to be confused with dimm # of channel)
  UINT8       Socket;              // Socket number
  UINT8       Channel;             // Channel number
  UINT8       Dimm;                // Dimm number on channel
  UINT8       iMCWays;             // Number of iMC interleave ways that this dimm is participating for PMEM Rgn
  UINT8       ChWays;              // Number of Channel interleave ways that this dimm is participating for PMEM Rgn
  EFI_STATUS  ArsStatus;           // Status of the Ars Command Sent

  // below JEDEC-specific fields
  UINT8       SecondPMemRgn;       // To tell if this a second PMem Region of a dimm
  UINT64      ARSDpaStart;         // Dpa Start address for ARS
  UINT64      ARSDpaEnd;           // Dpa End address for ARS
} PMEM_INFO;

typedef struct {
  BOOLEAN ActivationEnabled;
  UINT8 ActivationState;
  UINT16 LastActivationStatus;
} ACTIVATION_DATA;

typedef struct {
  UINT16      ImcId;               // Integrated memory controller number
  UINT16      SocketId;            // Socket Num
  UINT8       Dimm;                // Dimm #
  UINT8       IntDimmNum;          // Dimm Number in the interleave set (not to be confused with dimm # of channel)
  UINT8       Ch;                  // Channel # where this Dimm resides (per socket numbering)
  UINT8       iMCWays;             // Number of iMC interleave ways that this dimm is participating for Cntrl Rgn
  UINT8       ChWays;              // Number of Channel interleave ways that this dimm is participating for Cntrl Rgn
  UINT8       FMchannelInterBitmap;// FMchannelInterBitmap of the SAD Rule
  UINT64      SADSpaBase;          // SAD Rule Spa Base Addr for Control Region, if DIMM disabled set to SPA_NOT_MAPPED
  UINT64      CntrlBase;           // Control Region Base Address
  UINT16      DeviceID;            // Device ID of this dimm
  UINT16      VendorID;            // Vendor ID of the dimm
  UINT16      RevisionID;          // Revision ID of the dimm
  UINT16      SubsystemVendorID;   // Subsystem vendor ID
  UINT16      SubsystemDeviceID;   // Subsystem Device ID
  UINT16      SubsystemRevisionID; // Subsystem RID
  UINT8       ManufLocation;       // Manufacturing location for the NVDIMM
  UINT16      ManufDate;           // Date the NVDIMM was manufactured
  UINT16      FormatInterfaceCode; // FormatInterfaceCode
  UINT32      SerialNum;           // Serial Number of the Dimm

  // below CR-specific fields
  UINT16      FisVersion;          // Firmware Interface Specification version
  CR_MAILBOX  MailboxSPA;          // BIOS Control Region Mapped SPA regiter addresses
  CR_MAILBOX  MailboxDPA;          // BIOS Control Region DPA register addresses
  CR_MAILBOX  OSMailboxSPA;        // OS Mailbox region for _DSM Command Interface
  CR_MAILBOX  OSMailboxDPA;        // OS Mailbox region for _DSM Command Interface
  UINT64      Nonce;               // Holds security Nonce generated
  UINT8       NonceSet;            // Flag to tell if the Nonce has been set for the Mailbox of this dimm.
  UINT64      ControlRegionIndex;
  UINT64      BsrSpa;
  UINT64      FlushAddress[32];
  UINT64      FlushAddressOrder[32];
  UINT8       FlushAddressOrderLut[32];
  UINT64      FlushRegionIndex;
  S3_INFO     S3Info;
  ACTIVATION_DATA ActivationData;
  UINT64      SmbusCmdRegSpa;       //SPA of the SMBUS Command REG
} NVDIMM;

typedef struct {
  UINT32                  SktGran;            // Socket Granularity for the Control Region
  UINT32                  ChGran;             // Channel Granularity for the Control Region
  NVDIMM                  Nvdimms[MAX_SYS_DIMM + 1];
  PMEM_INFO               PMEMInfo[MAX_SYS_DIMM * MAX_PMEM_RGN_PER_DIMM]; // Information about 1st Persistent Memory Region
  UINT8                   NumPMEMRgns;        // Number of PMEM Regions total
  UINT8                   NumNvdimms;        // Number of Nvdimms Installed/Found
} NVDIMM_COMMON_INFO;

//
// Function declarations being exposed by this module
//

/**

  Routine Description: This function checks if the given start address
  of a SAD rule belongs to a PMEM Region.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct

  @param[in] SpaBaseAddr - Spa Start Address of the SAD Rule

  @return TRUE if yes, else FALSE

**/
BOOLEAN
IsPmemRgn (
  NVDIMM_COMMON_INFO *NvDimmInfoPtr,
  IN UINT64 SpaBaseAddr
  );

/**

  Routine Description: This helper function returns first index of the PMEMInfo
  array found based on the SAD base address passed in.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct

  @param[in] SpaBaseAddr     - Spa Start Address of the SAD Rule

  @return Index of the array, -1 if no match found

**/
INT32
GetPmemIndex (
  NVDIMM_COMMON_INFO *NvDimmInfoPtr,
  UINT64      SpaBaseAddr
   );

/**

  Routine Description: This helper function returns index of the PMEMInfo array
  based on the SAD base address, Socket and Channel info passed in.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct
  @param[in] SpaBaseAddr - Spa Start Address of the SAD Rule
  @param[in] Socket       - Socket Number
  @param[in] Ch           - Channel

  @return Index of the array, -1 if no match found

**/
INT32
GetPmemIndexBasedOnSocChDimm (
  NVDIMM_COMMON_INFO *NvDimmInfoPtr,
  IN UINT64  SpaBaseAddr,
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   Dimm
  );

/**

Routine Description: InitAllDimmsFromHOB Initializes
all Nvdimms installed in the system using the HOB,
basically to gather VendorID & DeviceID of all these dimms and
stores in the pDimm Struct.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct

**/
VOID
InitAllDimmsFromHOB (
  NVDIMM_COMMON_INFO *NvDimmInfoPtr
  );

/**

  Routine Description: This helper function returns the total number of Dimms
  in the Interleave set of the SAD Rule for a PMEM Region.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct

  @param[in] SpaBaseAddr - Spa Start Address of the SAD Rule

  @return Total number of dimms in the Interleave set.

**/
UINT8
GetNumDimmsForPmemRgn (
  NVDIMM_COMMON_INFO *NvDimmInfoPtr,
  IN UINT64 SpaBaseAddr
  );

/**
  Routine Description: This function computes the
  number of ways iMC has been interleaved for a particular SAD in
  both the Numa and Uma cases.

  @param[in] Socket      - socket number (0 based)
  @param[in] SadIndex    - SAD index

  @return iMC interleave Ways
**/
UINT8
GetImcWays (
  IN UINT8       Socket,
  IN UINT8       SadIndex
  );

/**
  Routine Description: Find the MC index to use for calculating channel ways.

  @param[in] ImcInterBitmap  - bitmap of IMCs in the interleave.
                               ImcInterBitmap must be a non-zero value in input.
                               Routine assumes value has BIT0 and/or BIT1 set.
  @return IMC number to use for calculating channel ways
**/
UINT8
EFIAPI
GetMcIndexFromBitmap (
  IN UINT8 ImcInterBitmap
  );

/**
  @brief Get the SAD address base of the requested SAD table entry.

  Use the local and remote SAD tables to calculate the requested SAD entry base address.

  NOTE: For SKX architecture without remote SAD rules the remote table is empty so
        this function can be called, it is backward compatible.

  @param[in] SocketNum - Socket ID.
  @param[in] SadIndex  - Index of SAD table entry within socket

  @retval Base address of the requested SAD table entry

**/
UINT64
NvdimmGetSadBase (
  IN UINT8       Skt,
  IN UINT8       Sad
  );

/**
  Routine Description: This function returns socket granularity for interleave
  (which is really iMC Granularity) as part of the setup options for the memory
  configuration.

  @param[in] SadPtr      - pointer to the SAD

  @return Socket Granularity
**/
UINT32
GetSocketGranularity (
  IN SAD_TABLE *SadPtr
  );

/**
  Routine Description: This function returns channel granularity for interleave
  as part of the setup options for the memory configuration.

  @param[in] SadPtr      - pointer to the SAD

  @return Channel granularity
**/
UINT32
GetChannelGranularity (
  IN SAD_TABLE *SadPtr
  );

/**

  Routine Description: Given a SAD base address, function returns channel
  granularity based on the SAD Rule.

  @param[in] SadPtr      - pointer to the SAD

  @return Channel granularity

**/
UINT32
GetChGranularityFromSadBaseAddr (
  IN UINT64 SadBaseAddr
  );

/**

  Routine Description: GetDimm will return the pointer to the Dimm
    structure if found based on socket, channel and dimm arguments.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct
  @param[in] Socket      - Socket Number
  @param[in] Ch          - DDR Channel ID
  @param[in] Dimm        - DIMM number

  @return NVDIMM structure for given Dimm or NULL if not found

**/
NVDIMM *
GetDimm (
  NVDIMM_COMMON_INFO *NvdimmInfoPtr,
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Type
  );

/**
  @brief Get the channel ways number for given SAD index and socket.

  @param[in] Socket   - socket number (0 based)
  @param[in] SadIndex - SAD index

  @return Channel ways
*/
UINT8
GetChWays (
  IN UINT8  Socket,
  IN UINT8  SadIndex
  );


/**

Routine Description: GetSystemMemoryMapHob returns initialized SystemMemoryMap.

  return SystemMemoryMap

**/
struct SystemMemoryMapHob *
EFIAPI
GetSystemMemoryMapHob (
  VOID
  );

/**
  Converts socket channel index to memory controller index.

  @param[in] SktCh - socket channel index

  @return Memory controller index
**/
UINT8
ConvertSktChToImc (
  IN UINT8 SktCh
  );

/**
  Converts socket channel index to memory controller channel index.

  @param[in] SktCh - socket channel index

  @return Memory controller channel index
**/
UINT8
ConvertSktChToImcCh (
  IN UINT8 SktCh
  );

/**
  Converts memory controller channel index to socket channel index.

  @param[in] Imc - memory controller index

  @param[in] Ch  - memory controller channel index

  @return Socket channel index
**/
UINT8
ConvertImcChToSktCh (
  IN UINT8 Imc,
  IN UINT8 Ch
  );

/**
  Converts socket, channel and dimm to NFIT handle.

  DIMM encoding is as follows:
  [0,1] - DIMMs 0,1 in socket 0, channel 0
  [2,3] - DIMMs 0,1 in socket 0, channel 1
  ...
  [10,11] - DIMMs 0,1 in socket 0, channel 5
  [12..15] - spare for future channels 6,7 if used
  [16,17] - DIMMs 0,1 in socket 1, channel 0

  @param[in] Socket - socket index

  @param[in] Channel - channel index

  @param[in] Dimm - DIMM index

  @return NFIT handle
**/
UINT32
ConvertSocketChannelDimmToNfitHandle (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

//extern struct SystemMemoryMapHob *mSystemMemoryMap;
//extern BOOLEAN                    mInSmm;

#endif // _NVDIMM_COMMON_LIB_H_

