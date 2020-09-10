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

#include <PiDxe.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Library/NvdimmCommonLib.h>
#include <Library/MemTypeLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

#define BITS_64MB_TO_BYTES          26

//
// Globals
//
static struct SystemMemoryMapHob    *mNvdimmCommonSystemMemoryMap = NULL;
BOOLEAN                             mNvdimmCommonInSmm = FALSE;

/******************************************************************************
 * Functions
 ******************************************************************************/


/**
  @brief Compute the number of ways Imc has been interleaved for a particular SAD in both the NUMA and UMA cases.

  @param[in] Socket      - socket number (0 based)
  @param[in] SadIndex    - SAD index

  @return Imc interleave Ways
**/
UINT8
GetImcWays (
  IN UINT8  Socket,
  IN UINT8  SadIndex
  )
{
  UINT8     Imc;
  UINT8     ImcWays;

  ImcWays = 0;
  for (Imc = 0; Imc < MC_MAX_NODE; Imc++) {
    if (mNvdimmCommonSystemMemoryMap->Socket[Socket].SadIntList[SadIndex][Imc]) {
      ImcWays++;
    }
  }
  return ImcWays;
}

/**
  @brief Find the MC index to use for calculating channel ways.

  In practice this function finds first bit set in the 'ImcInterBitmap'.

  @param[in] ImcInterBitmap  - bitmap of IMCs in the interleave.
                               ImcInterBitmap must be a non-zero value in input.
  @return IMC number to use for calculating channel ways.
**/
UINT8
GetMcIndexFromBitmap (
  IN UINT8 ImcInterBitmap
  )
{
  UINT8 Imc;

  for (Imc = 0; Imc < MAX_IMC; Imc++, ImcInterBitmap >>= 1) {

    if (ImcInterBitmap & 1) {

      return Imc;
    }
  }
  ASSERT (FALSE);
  return 0;
}

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
  )
{
  INT32            Index = -1;
  UINT32           i;

  ASSERT (NvDimmInfoPtr->NumPMEMRgns <= ARRAY_SIZE (NvDimmInfoPtr->PMEMInfo));
  for (i = 0; (i < NvDimmInfoPtr->NumPMEMRgns) && (i < ARRAY_SIZE (NvDimmInfoPtr->PMEMInfo)); i++) {
    if (NvDimmInfoPtr->PMEMInfo[i].SADPMemBase == SpaBaseAddr) {
      Index = (INT32)i;
      return Index;
    }
  }
  return Index;
}

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
  UINT64 SpaBaseAddr
  )
{
  INT32 Index;

  Index = GetPmemIndex (NvDimmInfoPtr, SpaBaseAddr);

  if (Index >= 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

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
  UINT64      SpaBaseAddr
  )
{
  UINT8            i = 0;
  UINT8            DimmCount = 0;
  PMEM_INFO        *Pmem = NULL;

  for (i = 0; (i < NvDimmInfoPtr->NumPMEMRgns) && (i < ARRAY_SIZE (NvDimmInfoPtr->PMEMInfo)); i++) {
    Pmem =  &NvDimmInfoPtr->PMEMInfo[i];
    if (Pmem->SADPMemBase == SpaBaseAddr) {
      DimmCount++;
    }
  }
  return DimmCount;
}



/**

  Routine Description: This helper function returns index of the PMEMInfo array
  based on the SAD base address, Socket and Channel info passed in.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct
  @param[in] SpaBaseAddr  - Spa Start Address of the SAD Rule
  @param[in] Socket       - Socket Number
  @param[in] Ch           - Channel

  @return Index of the array, -1 if no match found

**/
INT32
GetPmemIndexBasedOnSocChDimm (
  NVDIMM_COMMON_INFO *NvDimmInfoPtr,
  UINT64      SpaBaseAddr,
  UINT8       Socket,
  UINT8       Ch,
  UINT8       Dimm
  )
{
  INT32            Index = -1;
  UINT32           i;

  ASSERT (NvDimmInfoPtr->NumPMEMRgns <= ARRAY_SIZE (NvDimmInfoPtr->PMEMInfo));
  for (i = 0; (i < NvDimmInfoPtr->NumPMEMRgns) && (i < ARRAY_SIZE (NvDimmInfoPtr->PMEMInfo)); i++) {
    if ((NvDimmInfoPtr->PMEMInfo[i].SADPMemBase == SpaBaseAddr) &&
        (NvDimmInfoPtr->PMEMInfo[i].Socket      == Socket) &&
        (NvDimmInfoPtr->PMEMInfo[i].Channel     == Ch) &&
        (NvDimmInfoPtr->PMEMInfo[i].Dimm        == Dimm)) {
      Index = (INT32)i;
      return Index;
    }
  }
  return Index;
}

/**

  Routine Description: This function returns socket granularity
  (which is really iMC Granularity) basing on granularity encoding.

  @param[in] GranEncoding      - granularity encoding

  @return Socket Granularity

**/
UINT32
GetSocketGranularityFromEncoding (
  IN UINT8 GranEncoding
  )
{
  UINT32   SktGran;

  switch (GranEncoding) {
    case 0:
      SktGran = 64;
      break;
    case 1:
      SktGran = 256;
      break;
    case 2:
      SktGran = 4096;
      break;
    case 3:
      SktGran = 0x40000000;
      break;
    default:
      SktGran = 4096;
      break;
  }
  return SktGran;
} // GetSocketGranularityFromEncoding()

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
  )
{
  UINT32   SktGran, SetupOptions;
  UINT8    GranEncoding = 2;

  SetupOptions = mNvdimmCommonSystemMemoryMap->MemSetup.options;
  if (SadPtr->type == MemType1lmDdr) {
    switch (SadPtr->granularity) {
      case MEM_INT_GRAN_1LM_256B_256B:
        GranEncoding = 1;
        break;
      case MEM_INT_GRAN_1LM_64B_64B:
        GranEncoding = 0;
        break;
    }
  }

  if (IsMemTypeAppDirect (SadPtr->type)) {
      GranEncoding = 2;
  } else if (SadPtr->type == MemType1lmCtrl) {
    // No Setup Option for Block Window or Control Region, just fixed size of 4k
    GranEncoding = 2;
  }

  SktGran = GetSocketGranularityFromEncoding (GranEncoding);
  return SktGran;
} // GetSocketGranularity()

/**
  Routine Description: This function returns channel granularity for interleave
  as part of the setup options for the memory configuration.

  @param[in] SadPtr      - pointer to the SAD

  @return Channel granularity
**/
UINT32
GetChannelGranularity (
  SAD_TABLE *SadPtr
  )
{
  if (IsMemTypeAppDirect (SadPtr->type)) {

    switch (SadPtr->granularity)  {
      default:
        DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR("NVDIMMLIB") "Unknowns SAD granularity encoding (%d), assume channel granularity 4 KiB\n",
                SadPtr->granularity));
      case MEM_INT_GRAN_CH_4KB_TGT_4KB:
        return 4096;
      case MEM_INT_GRAN_CH_256B_TGT_4KB:
        return 256;
    }
  } else if (SadPtr->type == MemType1lmCtrl) {
    //
    // No setup options for Blk and Ctrl region, use 256 bytes for channel granularity
    //
    return 256;
  } else if (SadPtr->type == MemType1lmDdr) {
    switch (SadPtr->granularity) {
      case MEM_INT_GRAN_1LM_256B_256B:
        return 256;
      case MEM_INT_GRAN_1LM_64B_64B:
        return 64;
    }
  }
  return 4096;
} // GetChannelGranularity()




/**

  Routine Description: Given a SAD base address, function returns channel
  granularity based on the SAD Rule.

  @param[in] SadBaseAddr - SAD base address

  @return Channel granularity

**/
UINT32
GetChGranularityFromSadBaseAddr (
  UINT64 SadBaseAddr
  )
{
  UINT8      Skt;
  UINT8      Sad;
  UINT32     ChGran = 0;
  UINT64     SadBase;

  for (Skt = 0; Skt < ARRAY_SIZE (mNvdimmCommonSystemMemoryMap->Socket); Skt++) {

    for (Sad = 0; Sad < ARRAY_SIZE (mNvdimmCommonSystemMemoryMap->Socket[Skt].SAD); Sad++) {

      if (!mNvdimmCommonSystemMemoryMap->Socket[Skt].SAD[Sad].local) {
        continue;
      }
      SadBase = NvdimmGetSadBase (Skt, Sad);
      if (SadBase == SadBaseAddr) {
        ChGran = GetChannelGranularity (&mNvdimmCommonSystemMemoryMap->Socket[Skt].SAD[Sad]);
        break;
      }
    }
  }
  return ChGran;
} // GetChGranularityFromSadBaseAddr()

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
  )
{
  if (Skt < ARRAY_SIZE (mNvdimmCommonSystemMemoryMap->Socket) && Sad < ARRAY_SIZE (mNvdimmCommonSystemMemoryMap->Socket[0].SAD)) {

    return (UINT64)mNvdimmCommonSystemMemoryMap->Socket[Skt].SAD[Sad].Base << BITS_64MB_TO_BYTES;

  } else {

    ASSERT (FALSE);
  }
  return 0;
} // NvdimmGetSadBase()


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
  NVDIMM_COMMON_INFO *NvDimmInfoPtr,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Flags
  )
{
  NVDIMM      *NvmDimm = NULL;
  UINT8         i;

  for (i = 0; i < NvDimmInfoPtr->NumNvdimms; i++) {
    NvmDimm = &NvDimmInfoPtr->Nvdimms[i];
    if (NvmDimm->SocketId != Socket ||
        NvmDimm->Ch != Ch ||
        (NvmDimm->Dimm != Dimm && (Flags & NVDIMM_FLAG_ANY_DIMM) == 0)) {
      continue;
    }
    if ((Flags & NVDIMM_FLAG_INCLUDE_DISABLED) || NvmDimm->SADSpaBase != SPA_NOT_MAPPED) {
      return NvmDimm;
    }
  }

  if (Flags & NVDIMM_FLAG_DUMMY) {
    return &NvDimmInfoPtr->Nvdimms[MAX_SYS_DIMM];
  }
  DEBUG ((EFI_D_ERROR, NVDIMM_INFO_STR("NVDIMMLIB") "S%d.C%d.D%d: No Nvdimm found\n", Socket, Ch, Dimm));

  return NULL;
}

/**

Routine Description: InitAllDimmsFromHOB Initializes
all Nvdimms installed in the system using the HOB,
basically to gather VendorID & DeviceID of all these dimms and
stores in the pDimm Struct.

  @param[in] NvDimmInfoPtr - Ptr to NVDIMM_COMMON_INFO struct

  @retval EFI_STATUS - success if command sent

**/
VOID
InitAllDimmsFromHOB (
  NVDIMM_COMMON_INFO *NvDimmInfoPtr
  )
{
  UINT8             i;
  UINT16            Socket;
  UINT8             Ch;
  UINT8             Dimm;
  NVDIMM          *DimmPtr;
  struct DimmDevice *DimmDevicePtr;

  for (i = 0; i < NvDimmInfoPtr->NumNvdimms; i++) {
    DimmPtr = &NvDimmInfoPtr->Nvdimms[i];

    Socket = DimmPtr->SocketId;
    Ch = DimmPtr->Ch;
    Dimm = DimmPtr->Dimm;

    DimmDevicePtr = &mNvdimmCommonSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm];

    DimmPtr->VendorID = DimmDevicePtr->VendorID;
    DimmPtr->DeviceID = DimmDevicePtr->DeviceID;
    DimmPtr->RevisionID = DimmDevicePtr->RevisionID;
    DimmPtr->SubsystemVendorID = DimmDevicePtr->SubsystemVendorID;
    DimmPtr->SubsystemDeviceID = DimmDevicePtr->SubsystemDeviceID;
    DimmPtr->SubsystemRevisionID = DimmDevicePtr->SubsystemRevisionID;
    DimmPtr->FisVersion = DimmDevicePtr->FisVersion;
    DimmPtr->ManufLocation = DimmDevicePtr->manufacturingLocation;
    DimmPtr->ManufDate = DimmDevicePtr->manufacturingDate;
    DimmPtr->FormatInterfaceCode = DimmDevicePtr->InterfaceFormatCode;

    DimmPtr->SerialNum = (((UINT32)DimmDevicePtr->serialNumber[3]) << 24 |
                          ((UINT32)DimmDevicePtr->serialNumber[2]) << 16 |
                          ((UINT32)DimmDevicePtr->serialNumber[1]) << 8 |
                          DimmDevicePtr->serialNumber[0]);
    if (!mNvdimmCommonInSmm) {
      //
      // Contact CLV before changing the trace below, test depends on it.
      //
      DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR("NVDIMMLIB") "S%d.C%d.D%d: SPD_320 0x%02X, SPD_321 0x%02X, DID 0x%X, RID 0x%X\n",
              Socket, Ch, Dimm, (UINT8)DimmPtr->VendorID, DimmPtr->VendorID >> 8, DimmPtr->DeviceID, DimmPtr->RevisionID));
      DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR("NVDIMMLIB") "S%d.C%d.D%d: SVID 0x%X, SDID 0x%X, SRID 0x%X\n",
              Socket, Ch, Dimm, DimmPtr->SubsystemVendorID, DimmPtr->SubsystemDeviceID, DimmPtr->SubsystemRevisionID));
      DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR("NVDIMMLIB") "S%d.C%d.D%d: IFC 0x%X\n",
              Socket, Ch, Dimm, DimmPtr->FormatInterfaceCode));
      DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR("NVDIMMLIB") "S%d.C%d.D%d: FIS %X.%02X\n",
              Socket, Ch, Dimm, DimmPtr->FisVersion >> 8, (UINT8)DimmPtr->FisVersion));
    }
  }
}

/**

Routine Description: GetSystemMemoryMapHob returns initialized SystemMemoryMap.

  return SystemMemoryMap

**/
struct SystemMemoryMapHob *
EFIAPI
GetSystemMemoryMapHob (
  VOID
  )
{
  return mNvdimmCommonSystemMemoryMap;
}

/**
  Converts socket channel index to memory controller index.

  @param[in] SktCh - socket channel index

  @return Memory controller index
**/
UINT8
ConvertSktChToImc (
  IN UINT8 SktCh
  )
{
  UINT8 Imc;
  UINT8 NumChannelPerMc;

  NumChannelPerMc = GetNumChannelPerMc ();
  Imc = SktCh / NumChannelPerMc;

  return Imc;
}

/**
  Converts socket channel index to memory controller channel index.

  @param[in] SktCh - socket channel index

  @return Memory controller channel index
**/
UINT8
ConvertSktChToImcCh (
  IN UINT8 SktCh
  )
{
  UINT8 ImcCh;
  UINT8 NumChannelPerMc;

  NumChannelPerMc = GetNumChannelPerMc ();
  ImcCh = SktCh % NumChannelPerMc;

  return ImcCh;
}

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
  )
{
  UINT8 SktCh;
  UINT8 NumChannelPerMc;

  NumChannelPerMc = GetNumChannelPerMc ();
  SktCh = Imc * NumChannelPerMc + Ch;

  return SktCh;
}

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
  )
{
  UINT8  NumChannelPerMc;
  UINT32 NfitHandle;

  NumChannelPerMc = GetNumChannelPerMc ();
  NfitHandle = (UINT32)Socket << 4;
  NfitHandle = (NfitHandle | ((UINT32)Channel / NumChannelPerMc)) << 4;
  NfitHandle = (NfitHandle | ((UINT32)Channel % NumChannelPerMc)) << 4;
  NfitHandle = (NfitHandle | (UINT32)Dimm);

  return NfitHandle;
}

/**

  Routine Description: NvdimmCommonLibStart initializes SystemMemoryMap for
  later enablement of the read/write of the NVDIMM registers thru the
  SAD interface. This function needs to be called during init.

  @retval UINT32 - status

**/
EFI_STATUS
EFIAPI
NvdimmCommonLibStart (
  VOID
  )
{
  EFI_STATUS                          Status = EFI_SUCCESS;
  BOOLEAN                             InSmm = FALSE;
  struct SystemMemoryMapHob           *TempSystemMemoryMap;
  EFI_SMM_BASE2_PROTOCOL               *mSmmBase;
  EFI_SMM_SYSTEM_TABLE2               *mSmst = NULL;

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&mSmmBase);
  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase->InSmm (mSmmBase, &InSmm);
    mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
  }

  mNvdimmCommonInSmm = InSmm;

  if (InSmm) {
    //
    // Allocate memory for SystemMemoryHob
    //
    Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (struct SystemMemoryMapHob), (VOID**)&mNvdimmCommonSystemMemoryMap);
    ASSERT_EFI_ERROR (Status);

  } else {
    //
    // Allocate memory for SystemMemoryMap HOB
    //
    Status = gBS->AllocatePool (EfiBootServicesData, sizeof (struct SystemMemoryMapHob), (VOID**)&mNvdimmCommonSystemMemoryMap);
    ASSERT_EFI_ERROR (Status);
  }

  TempSystemMemoryMap = GetSystemMemoryMapData ();
  if (TempSystemMemoryMap == NULL) {
    ASSERT (TempSystemMemoryMap != NULL);
    return EFI_NOT_FOUND;
  }
  CopyMem (mNvdimmCommonSystemMemoryMap, TempSystemMemoryMap, sizeof (struct SystemMemoryMapHob));

  return Status;
}
