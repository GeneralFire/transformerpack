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
#include <SysHost.h>
#include <MemMapHost.h>
#include <IioUniversalData.h>
#include <IioPlatformData.h>
#include <Fpga.h>
#include <Platform.h>
#include <RcRegs.h>
#include <Cpu/Msr/IcxMsr.h>
#include <Register/ArchitecturalMsr.h>

#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/HobLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/CpuEarlyDataLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PeiIioInitLib.h>
#include <Library/FpgaConfigurationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <Guid/MrcPlatformHooksGuid.h>
#include <Guid/PlatformInfo.h>
#include <Guid/PlatformTxt.h>

#include <Ppi/MemoryPolicyPpi.h>

#include "MrcThermalInclude.h"
//APTIOV_SERVER_OVERRIDE_RC_START : 
#include <AmiMrcHooksWrapperLib/AmiMrcHooksWrapperLib.h>
//APTIOV_SERVER_OVERRIDE_RC_END :

extern EFI_GUID gEfiSiliconRcHobsReadyPpi;

static EFI_PEI_PPI_DESCRIPTOR mSiliconRcHobsReadyPpi[] = {
    {(EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiSiliconRcHobsReadyPpi,
    NULL}
};

EFI_STATUS
EFIAPI
PublishHobDataCallbackPeiMemoryDiscoveredPpi (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList = {
   EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
   &gEfiPeiMemoryDiscoveredPpiGuid,
   PublishHobDataCallbackPeiMemoryDiscoveredPpi
};

/**

  This function creates the NGN NVDIMM Configuration Records HOB

  @param Host - Pointer to System Host data structure

  @retval None

**/
VOID
SaveNgnCfgRecords (
  IN PSYSHOST Host
  )
{
  UINT8                        Index;
  UINT8                        Socket;
  UINT8                        Channel;
  UINT8                        Dimm;
  UINT8                        CfgRecord;
  MEMORY_MAP_HOST              *MemMapHost;
  MEM_NVRAM_STRUCT             *NvramMem;
  MEM_VAR_STRUCT               *Mem;
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  NGN_CFG_ENTRY                *Entry;
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  NvramMem = &Host->nvram.mem;
  Mem = &(Host->var.mem);
  MemMapHost = GetMemMapHostPointer ();

  ZeroMem ((UINT8*)&MemMapHost->CfgRecords, sizeof (MemMapHost->CfgRecords));
  ZeroMem ((UINT8*)&MemMapHost->NumCfgRecords, sizeof (MemMapHost->NumCfgRecords));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets
    //
    if (!NvramMem->socket[Socket].enabled) {
      continue;
    }

    for (Channel = 0; Channel < MAX_CH; Channel++) {
      //
      // Skip disabled channels
      //
      if (!NvramMem->socket[Socket].channelList[Channel].enabled) {
        continue;
      }

      DimmNvList = &NvramMem->socket[Socket].channelList[Channel].dimmList;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if not NVDIMM or DIMM is not present
        //
        if (!((*DimmNvList)[Dimm].dimmPresent && (*DimmNvList)[Dimm].DcpmmPresent)) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Socket, Channel, Dimm);

        for (CfgRecord = 0; CfgRecord < NGN_DIMM_CFG_REC_MAX; CfgRecord++) {
          //
          // No CCUR record to process
          //
          if ((NGN_DIMM_CFG_CUR_REC == CfgRecord) && (!DdrtDimmInfo->NgnCfgCurPresent)) {
            continue;
          }

          //
          // No COUT record to process
          //
          if ((NGN_DIMM_CFG_OUT_REC == CfgRecord) && (!DdrtDimmInfo->NgnCfgOutPresent)) {
            continue;
          }

          //
          // No configuration error record to process
          //
          if ((NGN_DIMM_CFG_ERROR_REC == CfgRecord) && (!DdrtDimmInfo->NgnCfgErrorPresent)) {
            continue;
          }

          Entry = &MemMapHost->CfgRecords[CfgRecord][MemMapHost->NumCfgRecords[CfgRecord]++];
          Entry->Socket = Socket;
          Entry->Channel = Channel;
          Entry->Dimm = Dimm;

          //
          // Fill in the DIMM unique ID
          //
          Entry->DimmUniqueId.MfgIdCode = (*DimmNvList)[Dimm].VendorID;
          Entry->DimmUniqueId.MfgLocation = (*DimmNvList)[Dimm].SPDMMfLoc;
          Entry->DimmUniqueId.MfgDate.Year = (UINT8)((*DimmNvList)[Dimm].SPDModDate & 0x00FF);
          Entry->DimmUniqueId.MfgDate.Week = (UINT8)((*DimmNvList)[Dimm].SPDModDate >> 0x8);

          for( Index = 0; Index < NGN_MAX_SERIALNUMBER_STRLEN; Index++ ) {
            Entry->DimmUniqueId.SerialNumber[Index] = (*DimmNvList)[Dimm].SPDModSN[Index];
          }
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "PMem Module Configuration - %d CCUR Records created\n", MemMapHost->NumCfgRecords[NGN_DIMM_CFG_CUR_REC]));
  DEBUG ((DEBUG_INFO, "PMem Module Configuration - %d COUT Records created\n", MemMapHost->NumCfgRecords[NGN_DIMM_CFG_OUT_REC]));
  DEBUG ((DEBUG_INFO, "PMem Module Configuration - %d Error Records created\n", MemMapHost->NumCfgRecords[NGN_DIMM_CFG_ERROR_REC]));
}

#ifdef LT_FLAG
VOID
SavePlatformTxtDataToHob (
  IIO_UDS           *IioUds,
  EFI_PLATFORM_INFO *PlatformInfo
  )
{
  EFI_PLATFORM_TXT_DEVICE_MEMORY        PlatformTxtDeviceMemory;
  EFI_PHYSICAL_ADDRESS                  TxtDprAddress;
  EFI_PHYSICAL_ADDRESS                  TxtDevAddress;
  UINT32                                LtMem = 0;
  UINT32                                LtMemSize = 0;
  UINT32                                DprSize = 0;

    DEBUG ((DEBUG_INFO, "Reserve LT Mem!Should equal (MemTolm - MemTsegSize - LtMemSize) = 0x%08x\n", IioUds->PlatformData.MemTolm - PlatformInfo->MemData.MemTsegSize - PlatformInfo->MemData.MemLtMemSize));
    DprSize = PlatformInfo->MemData.MemDprMemSize;
    LtMem = PlatformInfo->MemData.MemLtMemAddress;
    DEBUG ((DEBUG_INFO, "LtMem: 0x%x\n",    LtMem));
    LtMem &= (~(LT_DEV_MEM_ALIGNMENT-1));
    LtMemSize = PlatformInfo->MemData.MemLtMemSize;
    if (PcdGetBool(PcdDprSizeFeatureSupport)) {
      DprSize   = PcdGet32 (PcdDprMemSize);
      LtMemSize = PcdGet32 (PcdLtMemSize);
    }
    TxtDprAddress = LtMem + LtMemSize - DprSize;
    TxtDevAddress = LtMem;
    PlatformTxtDeviceMemory.NoDMATableAddress                = TxtDevAddress;
    PlatformTxtDeviceMemory.NoDMATableSize                   = LtMemSize - DprSize;
    PlatformTxtDeviceMemory.SINITMemoryAddress               = TxtDprAddress + DprSize - LT_HEAP_SIZE - SINIT_MAX_SIZE;
    PlatformTxtDeviceMemory.SINITMemorySize                  = SINIT_MAX_SIZE;
    PlatformTxtDeviceMemory.TXTHeapMemoryAddress             = TxtDprAddress + DprSize - LT_HEAP_SIZE;
    PlatformTxtDeviceMemory.TXTHeapMemorySize                = LT_HEAP_SIZE;
    PlatformTxtDeviceMemory.DMAProtectionMemoryRegionAddress = LtMem - (UINT32)LShiftU64 (PlatformInfo->MemData.BiosGuardMemSize, 20); //TxtDprAddress;
    PlatformTxtDeviceMemory.DMAProtectionMemoryRegionSize    = (UINT32)LShiftU64 (PlatformInfo->MemData.BiosGuardMemSize, 20) + LtMemSize;
    DEBUG ((DEBUG_INFO, "========== LT Memory space ========== \n"));
    DEBUG ((DEBUG_INFO, "NoDMATableAddress: 0x%x\n",    PlatformTxtDeviceMemory.NoDMATableAddress));
    DEBUG ((DEBUG_INFO, "NoDMATableSize: 0x%x\n",       PlatformTxtDeviceMemory.NoDMATableSize));
    DEBUG ((DEBUG_INFO, "SINITMemoryAddress: 0x%x\n",   PlatformTxtDeviceMemory.SINITMemoryAddress));
    DEBUG ((DEBUG_INFO, "SINITMemorySize: 0x%x\n",      PlatformTxtDeviceMemory.SINITMemorySize));
    DEBUG ((DEBUG_INFO, "TXTHeapMemoryAddress: 0x%x\n", PlatformTxtDeviceMemory.TXTHeapMemoryAddress));
    DEBUG ((DEBUG_INFO, "TXTHeapMemorySize: 0x%x\n",    PlatformTxtDeviceMemory.TXTHeapMemorySize));
    DEBUG ((DEBUG_INFO, "DMAProtectionMemoryRegionAddress: 0x%x\n", PlatformTxtDeviceMemory.DMAProtectionMemoryRegionAddress));
    DEBUG ((DEBUG_INFO, "DMAProtectionMemoryRegionSize: 0x%x\n\n",  PlatformTxtDeviceMemory.DMAProtectionMemoryRegionSize));
    BuildGuidDataHob (&gEfiPlatformTxtDeviceMemoryGuid, &PlatformTxtDeviceMemory, sizeof(PlatformTxtDeviceMemory));

  return;
}
#endif //LT_FLAG

/**
  Get the IO resource size reserved for M2IOSF devices use.

  @param[in]        Socket     -  Socket number
  @param[in]        Stack      -  Stack number

  @retval           IO resource size (Unit: in Byte)
**/
UINT16
EFIAPI
GetM2iosfDeviceReservedIoSize (
  IN  UINT8       Socket,
  IN  UINT8       Stack
  )
{
  if ((Socket < MAX_SOCKET) && (Stack < MAX_CXL_PER_SOCKET)) {
    return 0x1000;  // 4KB
  } else {
    return 0;
  }
}

/**
  Get the MMIO32 resource size reserved for M2IOSF devices use.

  @param[in]        Socket     -  Socket number
  @param[in]        Stack      -  Stack number

  @retval           MMIO32 resource size (Unit: in Byte)
**/
UINT32
EFIAPI
GetM2iosfDeviceReservedMmio32Size (
  IN  UINT8       Socket,
  IN  UINT8       Stack
  )
{
  if ((Socket < MAX_SOCKET) && (Stack < MAX_CXL_PER_SOCKET)) {
    return 0x100000;  // 1MB
  } else {
    return 0;
  }
}

/**
  Get the MMIO64 resource size reserved for M2IOSF devices use.

  @param[in]        Socket     -  Socket number
  @param[in]        Stack      -  Stack number

  @retval           MMIO64 resource size (Unit: in Byte)
**/
UINT64
EFIAPI
GetM2iosfDeviceReservedMmio64Size (
  IN  UINT8       Socket,
  IN  UINT8       Stack
  )
{
  if ((Socket < MAX_SOCKET) && (Stack < MAX_CXL_PER_SOCKET)) {
    return 0x100000000;  // 4GB
  } else {
    return 0;
  }
}

/**
  This function builds UDS HOB of IIO_UNIVERSAL_DATA_GUID using KTI data.

  @param[in] Host  Pointer to System Host structure.
 **/
VOID
SaveIioUdsDataToHob (
  IN PSYSHOST  Host
  )
{
  IIO_UDS                               IioUds;         // Module Universal Data Store!
  EFI_GUID                              UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  UINT8                                 Socket, Stack, Port, SbspSktId;
  UINT8                                 ch;
  UINT64                                mmiohLimit;
  EFI_STATUS                            Status;
  UINT32                                IoApicPerSocket;
  KTI_HOST_OUT                          *KtiHostOutPtr;
  INTEL_DMI_PCIE_INFO                   *PcieInfoDefaults;
  ICX_MSR_TURBO_RATIO_LIMIT_REGISTER    TurboRatioLimitMsr;
  UINT32                                VtdDmiBarWidth = 0;
  IIO_GLOBALS                           *IioGlobalData;

  static CONST CHAR8 *StkPers2Name[] = {"UBOX", "IIO", "MCP", "FPGA", "HFI", "NAC", "GRAPHICS", "DINO", "RSVD"};

  IioGlobalData = NULL;

  //
  // Update the platform info hob with system PCI resource info based on CSI output structure
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  SbspSktId     = GetSysSbspSocketIdNv ();
  ASSERT (GuidHob != NULL);
  if(GuidHob == NULL) {
    return;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);
  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  // Initialize UDS stack variables to zero
  ZeroMem (&IioUds, sizeof (IioUds));

  // report CPU 0 resource into PlatformInfo
  IioUds.PlatformData.PlatGlobalIoBase      = 0x1000;//KtiHostOutPtr->CpuInfo[0].CpuRes.IoBase;
  IioUds.PlatformData.PlatGlobalIoLimit     = KtiHostOutPtr->CpuInfo[0].CpuRes.IoLimit;
  //
  // Get the highest IO limit in the system
  //
  for (Socket = 1; Socket < MAX_SOCKET; Socket++) {

    if (IioUds.PlatformData.PlatGlobalIoLimit < KtiHostOutPtr->CpuInfo[Socket].CpuRes.IoLimit) {
      IioUds.PlatformData.PlatGlobalIoLimit = KtiHostOutPtr->CpuInfo[Socket].CpuRes.IoLimit;
    }
  }
  DEBUG ((DEBUG_ERROR, "[UDS] System I/O   : 0x%04X..0x%04X\n",
          IioUds.PlatformData.PlatGlobalIoBase, IioUds.PlatformData.PlatGlobalIoLimit));
  IioUds.PlatformData.PlatGlobalMmio32Base = KtiHostOutPtr->CpuInfo[0].CpuRes.MmiolBase;
  IioUds.PlatformData.PlatGlobalMmio32Limit = KtiHostOutPtr->CpuInfo[0].CpuRes.MmiolLimit;
  //
  // Get the highest MMIOL limit in the system
  //
  for (Socket = 1; Socket < MAX_SOCKET; Socket++) {

    if (IioUds.PlatformData.PlatGlobalMmio32Limit < KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiolLimit) {
      IioUds.PlatformData.PlatGlobalMmio32Limit = KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiolLimit;
    }
  }
  DEBUG ((DEBUG_ERROR, "[UDS] System MMIO32: 0x%08X..0x%08X\n",
          IioUds.PlatformData.PlatGlobalMmio32Base, IioUds.PlatformData.PlatGlobalMmio32Limit));

  IioUds.PlatformData.PlatGlobalMmio64Base = LShiftU64((UINT64)(KtiHostOutPtr->CpuInfo[0].CpuRes.MmiohBase.hi), 32);
  IioUds.PlatformData.PlatGlobalMmio64Base |= (UINT64)KtiHostOutPtr->CpuInfo[0].CpuRes.MmiohBase.lo;
  IioUds.PlatformData.PlatGlobalMmio64Limit = LShiftU64((UINT64)(KtiHostOutPtr->CpuInfo[0].CpuRes.MmiohLimit.hi), 32);
  IioUds.PlatformData.PlatGlobalMmio64Limit |= (UINT64)KtiHostOutPtr->CpuInfo[0].CpuRes.MmiohLimit.lo;
  //
  // Get the highest MMIOH limit in the system
  //
  for (Socket = 1; Socket < MAX_SOCKET; Socket++) {

    mmiohLimit = LShiftU64((UINT64)(KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiohLimit.hi), 32);
    mmiohLimit |= (UINT64)KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiohLimit.lo;
    if (IioUds.PlatformData.PlatGlobalMmio64Limit < mmiohLimit) {
      IioUds.PlatformData.PlatGlobalMmio64Limit = mmiohLimit;
    }
  }
  DEBUG ((DEBUG_ERROR, "[UDS] System MMIO64: 0x%012llX..0x%012llX\n",
          IioUds.PlatformData.PlatGlobalMmio64Base, IioUds.PlatformData.PlatGlobalMmio64Limit));
  IioUds.PlatformData.IoGranularity    = KtiHostOutPtr->OutIoGranularity;
  IioUds.PlatformData.MmiolGranularity = KtiHostOutPtr->OutMmiolGranularity;
  IioUds.PlatformData.MmiohGranularity = KtiHostOutPtr->OutMmiohGranularity;
  IioUds.PlatformData.UboxMmioSize     = KtiHostOutPtr->OutUboxMmioSize;
  IioUds.PlatformData.MaxAddressBits   = KtiHostOutPtr->MaxAddress;

  IioUds.PlatformData.PciExpressBase         = KtiHostOutPtr->mmCfgBase;
  IioUds.PlatformData.PciExpressSize         = KtiHostOutPtr->mmCfgSize;
  DEBUG ((DEBUG_INFO, "[UDS] mmCfgBase     : 0x%08X\n", KtiHostOutPtr->mmCfgBase));
  IioUds.PlatformData.MemTolm                = Host->var.mem.lowMemSize * CONVERT_64MB_TO_BYTE;

  GuidHob = GetFirstGuidHob (&gIioSiPolicyHobGuid);
  ASSERT (GuidHob != NULL);
  IioGlobalData = (IIO_GLOBALS *) GET_GUID_HOB_DATA (GuidHob);
  if (IioGlobalData != NULL) {
    IioUds.PlatformData.Pci64BitResourceAllocation = IioGlobalData->SetupData.Pci64BitResourceAllocation;
  }
  DEBUG ((DEBUG_ERROR, "[UDS] IioUds.PlatformData.Pci64BitResourceAllocation: %x\n", IioUds.PlatformData.Pci64BitResourceAllocation));

  // Init UDS data here
  IioUds.PlatformData.MemTsegSize             = PlatformInfo->MemData.MemTsegSize;
  IioUds.PlatformData.MemIedSize              = PlatformInfo->MemData.MemIedSize;
  IioUds.PlatformData.numofIIO                = 0;
  IioUds.PlatformData.MaxBusNumber            = 0xFF;

  IoApicPerSocket = MAX_IIO_STACK;

  DEBUG ((DEBUG_INFO, "[UDS] Initial system ACPI IOAPICs enabled (bitmask) = 0x%llX\n", PlatformInfo->SysData.SysIoApicEnable));

  for (Socket = 0; Socket < MAX_SOCKET; ++Socket) {

    if (SocketPresent (Socket)) {

      IioUds.PlatformData.packageBspApicID[Socket] = GetCpuVarData()->PackageBspApicID[Socket];  // for CpuSv <AS>
      IioUds.PlatformData.CpuQpiInfo[Socket].Valid = 1;
      IioUds.PlatformData.CpuQpiInfo[Socket].SocId = Socket;
      for (Port = 0; Port < GetKtiPortCnt(); ++Port) {

        IioUds.PlatformData.CpuQpiInfo[Socket].PeerInfo[Port].Valid = (UINT8)KtiHostOutPtr->CpuInfo[Socket].LepInfo[Port].Valid;
        IioUds.PlatformData.CpuQpiInfo[Socket].PeerInfo[Port].PeerSocId = (UINT8)KtiHostOutPtr->CpuInfo[Socket].LepInfo[Port].PeerSocId;
        IioUds.PlatformData.CpuQpiInfo[Socket].PeerInfo[Port].PeerSocType = (UINT8)KtiHostOutPtr->CpuInfo[Socket].LepInfo[Port].PeerSocType;
        IioUds.PlatformData.CpuQpiInfo[Socket].PeerInfo[Port].PeerPort = (UINT8)KtiHostOutPtr->CpuInfo[Socket].LepInfo[Port].PeerPort;
      }

      IioUds.PlatformData.CpuQpiInfo[Socket].TotM3Kti = (UINT8)KtiHostOutPtr->CpuInfo[Socket].TotM3Kti;
      IioUds.PlatformData.CpuQpiInfo[Socket].ChaList = (UINT8)KtiHostOutPtr->CpuInfo[Socket].ChaList[0];
      IioUds.PlatformData.CpuQpiInfo[Socket].TotCha = (UINT8)KtiHostOutPtr->CpuInfo[Socket].TotCha;

      IioUds.PlatformData.CpuQpiInfo[Socket].M2PciePresentBitmap = (UINT8)KtiHostOutPtr->M2PciePresentBitmap[Socket];
      IioUds.PlatformData.CpuQpiInfo[Socket].stackPresentBitmap = KtiHostOutPtr->stackPresentBitmap[Socket];
      DEBUG ((DEBUG_INFO, "[UDS] IIO[%d] stackPresentBitmap: 0x%04X\n", Socket,
              IioUds.PlatformData.CpuQpiInfo[Socket].stackPresentBitmap));
      IioUds.PlatformData.CpuQpiInfo[Socket].CxlPresentBitmap = KtiHostOutPtr->CxlPresentBitmap[Socket];
      DEBUG ((EFI_D_INFO, "[UDS] IIO[%d] CxlBitMap: 0x%04X\n", Socket,
              IioUds.PlatformData.CpuQpiInfo[Socket].CxlPresentBitmap));
      IioUds.PlatformData.CpuQpiInfo[Socket].PcieSegment = KtiHostOutPtr->CpuInfo[Socket].CpuRes.PciSegment;
      IioUds.PlatformData.CpuQpiInfo[Socket].SegMmcfgBase = KtiHostOutPtr->CpuInfo[Socket].CpuRes.SegMmcfgBase;
      DEBUG ((DEBUG_INFO, "[UDS] IIO[%d] Segment: %d  MmCfgBase: 0x%X%08X\n", Socket,
              IioUds.PlatformData.CpuQpiInfo[Socket].PcieSegment,
              IioUds.PlatformData.CpuQpiInfo[Socket].SegMmcfgBase.hi,
              IioUds.PlatformData.CpuQpiInfo[Socket].SegMmcfgBase.lo));

      IioUds.PlatformData.IioQpiInfo[Socket].Valid = 1;
      IioUds.PlatformData.IioQpiInfo[Socket].SocId = Socket;

      IioUds.PlatformData.numofIIO++;
      IioUds.PlatformData.IIO_resource[Socket].Valid       = 1;
      IioUds.PlatformData.IIO_resource[Socket].SocketID    = Socket;
      IioUds.PlatformData.IIO_resource[Socket].BusBase     = KtiHostOutPtr->CpuInfo[Socket].CpuRes.BusBase;
      IioUds.PlatformData.IIO_resource[Socket].BusLimit    = KtiHostOutPtr->CpuInfo[Socket].CpuRes.BusLimit;
      DEBUG ((DEBUG_INFO, "[UDS] IIO[%d] Bus: 0x%02X..0x%02X\n", Socket,
              IioUds.PlatformData.IIO_resource[Socket].BusBase, IioUds.PlatformData.IIO_resource[Socket].BusLimit));

      //
      // TODO : Hardcode this to 0x1000 if we're the legacy IIO to make sure that PCI root ports are
      // not trying to decode legacy IO ports (ACPI, GPIO, etc).
      //
      IioUds.PlatformData.IIO_resource[Socket].PciResourceIoBase     = KtiHostOutPtr->CpuInfo[Socket].CpuRes.IoBase;
      if (IioUds.PlatformData.IIO_resource[Socket].PciResourceIoBase == 0) {
        IioUds.PlatformData.IIO_resource[Socket].PciResourceIoBase = 0x1000;
      } //Iobase 0 was set to 0 from RC, need to reserve legacy IO
      IioUds.PlatformData.IIO_resource[Socket].PciResourceIoLimit    = KtiHostOutPtr->CpuInfo[Socket].CpuRes.IoLimit;
      DEBUG ((DEBUG_INFO, "[UDS] IIO[%d] I/O: 0x%04X..0x%04X\n", Socket,
              IioUds.PlatformData.IIO_resource[Socket].PciResourceIoBase, IioUds.PlatformData.IIO_resource[Socket].PciResourceIoLimit));

      IioUds.PlatformData.IIO_resource[Socket].IoApicBase            = KtiHostOutPtr->CpuInfo[Socket].CpuRes.IoApicBase;
      IioUds.PlatformData.IIO_resource[Socket].IoApicLimit           = KtiHostOutPtr->CpuInfo[Socket].CpuRes.IoApicLimit;
      if ((IioUds.PlatformData.IIO_resource[Socket].IoApicBase < IOAPIC_BASE) ||
          (IioUds.PlatformData.IIO_resource[Socket].IoApicBase >= (IOAPIC_BASE + 0x100000))) {

         DEBUG ((DEBUG_INFO, "[UDS] IOAPIC 0x%X -> 0x%X !!!\n",
                 IioUds.PlatformData.IIO_resource[Socket].IoApicBase, IOAPIC_BASE + Socket * 0x40000));
         IioUds.PlatformData.IIO_resource[Socket].IoApicBase = IOAPIC_BASE + Socket * 0x40000;
      }
      DEBUG ((DEBUG_INFO, "[UDS] IIO[%d] IOAPIC: 0x%08X..0x%08X\n", Socket,
              IioUds.PlatformData.IIO_resource[Socket].IoApicBase, IioUds.PlatformData.IIO_resource[Socket].IoApicLimit));
      IioUds.PlatformData.IIO_resource[Socket].Mmio32Base  = KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiolBase;
      IioUds.PlatformData.IIO_resource[Socket].Mmio32Limit = KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiolLimit;
      IioUds.PlatformData.IIO_resource[Socket].Mmio64Base  = LShiftU64((UINT64)(KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiohBase.hi), 32);
      IioUds.PlatformData.IIO_resource[Socket].Mmio64Base  |= (UINT64)KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiohBase.lo;
      IioUds.PlatformData.IIO_resource[Socket].Mmio64Limit = LShiftU64((UINT64)(KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiohLimit.hi), 32);
      IioUds.PlatformData.IIO_resource[Socket].Mmio64Limit |= (UINT64)KtiHostOutPtr->CpuInfo[Socket].CpuRes.MmiohLimit.lo;
      DEBUG ((DEBUG_INFO, "[UDS] IIO[%d] MMIO32: 0x%08X..0x%08X\n", Socket,
              IioUds.PlatformData.IIO_resource[Socket].Mmio32Base,
              IioUds.PlatformData.IIO_resource[Socket].Mmio32Limit));
      DEBUG ((DEBUG_INFO, "[UDS] IIO[%d] MMIO64: 0x%012llX..0x%012llX\n", Socket,
              IioUds.PlatformData.IIO_resource[Socket].Mmio64Base,
              IioUds.PlatformData.IIO_resource[Socket].Mmio64Limit));

      for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        //
        // Update BusBase and BusLimit for stack even if it is not enabled in stackPresentBitmap.
        // UBOX is not present in stackPresentBitmap but has BusBase and BusLimit assigned.
        //
        IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Personality = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality;
        IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].BusBase;
        IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].BusLimit;

        DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] Personality: %a (%d)\n", Socket, Stack,
                IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Personality >= NELEMENTS (StkPers2Name) ?
                              "DIS" : StkPers2Name[IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Personality],
                IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Personality));
        DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] Bus: 0x%02X..0x%02X\n", Socket, Stack,
                IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase,
                IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit));
        //if (KtiHostOutPtr->stackPresentBitmap[Socket] & (1 << Stack)) {
        if  (IioUds.PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) {
          //
          // Update IoBase and IoLimit per Stack
          // Hardcode this to 0x1000 if we're the legacy IIO to make sure that PCI root ports are
          // not trying to decode legacy IO ports (ACPI, GPIO, etc).
          //
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoBase     = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].IoBase;
          if (IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoBase == 0) {
            IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoBase = 0x1000;
          } //Iobase 0 was set to 0 from RC, need to reserve legacy IO
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoLimit    = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].IoLimit;
          DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] I/O: 0x%04X..0x%04X\n", Socket, Stack,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoBase,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoLimit));
          //
          // Update IoApicBase and IoApicLimit per Stack
          //
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].IoApicBase;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicLimit = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].IoApicLimit;
          if ((IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase < IOAPIC_BASE) ||
             (IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase >= (IOAPIC_BASE + 0x100000))) {

             DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] IOAPIC 0x%X -> 0x%X !!!\n", Socket, Stack,
                     IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase, IOAPIC_BASE + Socket * 0x40000));
             IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase = IOAPIC_BASE + Socket * 0x40000;
          }
          DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] IOAPIC: 0x%08X..0x%08X\n", Socket, Stack,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicLimit));
          //
          // Update Mmiol limit\base, Mmioh limit\base per Stack
          //
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Base  = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Base  = LShiftU64(KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohBase.hi, 32);
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Base  |= (UINT64)KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohBase.lo;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit = LShiftU64(KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohLimit.hi, 32);
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit |= (UINT64)KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohLimit.lo;

          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceBusBase    = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceBusLimit   = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase     = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoBase;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoLimit    = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoLimit;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base  = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Base;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base  = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Base;
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Limit = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit;

          if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
            if (Stack == 0 && ((BIT0 << Socket) & GetPchPresentBitMap())) {
              //
              // Reserve the mmio used by DMI (RCRBBAR + MemBar)
              // TODO: May KtiHostOutPtr->DmiMemBarBase[Socket] be different than KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase ?
              // TODO: Can merge SPR RCRB with RcBaseAddress below in one universal solution?
              //
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base = KtiHostOutPtr->DmiMemBarBase[Socket] + KtiHostOutPtr->DmiMemBarSize[Socket];
            }

            if (GetCxlStatus (Socket, Stack) == AlreadyInCxlMode) {  // This stack works as CXL
              //
              // Bus resource
              //
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceBusBase = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase;
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceBusLimit = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase;

              //
              // Io resource
              //
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoLimit - \
                                                                                           GetM2iosfDeviceReservedIoSize (Socket, Stack) + 1;
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoLimit = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].IoLimit;

              //
              // Mmio32 resource
              //
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit - \
                                                                                              GetM2iosfDeviceReservedMmio32Size (Socket, Stack) + 1;
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit;

              //
              // Mmio64 resource
              //
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit - \
                                                                                              GetM2iosfDeviceReservedMmio64Size (Socket, Stack) + 1;
              IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Limit = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit;
            }
          }

          //
          // Reserve Vt-d BAR Address of size V_VT_D_IOH_BAR_SIZE, naturally aligned on 8KB boundary as required by the chipset
          // TODO: Reserve VTd region by moving PciResourceMem32Limit down, then remove VtdBarAddress.
          //
          if ((Socket == 0) && (Stack == 0) && (IsCpuAndRevision (CPU_ICXD, REV_ALL) || (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)))){
            VtdDmiBarWidth = (V_VT_D_IIO_BAR_SIZE + V_RLINKEXPPTMBAR_SIZE)-1; // 40KB reserved in total, 8KB for VTd and 32KB for EXPPTM bar
          } else {
            VtdDmiBarWidth = 0x3FFF; // 16KB reserved in total, 8KB for VTd and 8KB for DMI bar
          }
          IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].VtdBarAddress = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit & (~VtdDmiBarWidth);
          DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] VT-d BAR: 0x%08X\n", Socket, Stack,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].VtdBarAddress));
          // If DMI calculate DMIRCBAR Address
          if ((Socket == 0) && (Stack == 0)) {
            IioUds.PlatformData.IIO_resource[Socket].RcBaseAddress   = IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].VtdBarAddress + V_VT_D_IIO_BAR_SIZE;
            DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] RC BAR: 0x%08X\n", Socket, Stack,
                    IioUds.PlatformData.IIO_resource[Socket].RcBaseAddress));
          }
          DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] MMIO32: 0x%08X..0x%08X\n", Socket, Stack,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Base,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit));
          DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] MMIO32: 0x%08X..0x%08X for PCI devices\n", Socket, Stack,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit));
          DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] MMIO64: 0x%012llX..0x%012llX\n", Socket, Stack,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Base,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit));
          DEBUG ((DEBUG_INFO, "[UDS] IIO[%d.%d] MMIO64: 0x%012llX..0x%012llX for PCI devices\n", Socket, Stack,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base,
                  IioUds.PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Limit));

        } else {

          if ((IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) && Stack < IoApicPerSocket) {

            ASSERT (Stack < IoApicPerSocket);
            PlatformInfo->SysData.SysIoApicEnable &= ~LShiftU64 ((UINT64)1, (Socket * IoApicPerSocket + Stack + PCH_IOAPIC));
            DEBUG ((DEBUG_INFO, "[UDS] System ACPI IOAPICs enabled updated to %llx\n", PlatformInfo->SysData.SysIoApicEnable));
            DEBUG ((DEBUG_INFO, "[UDS] IoApic[%d][%d][%d] is invalid\n", KtiHostOutPtr->numCpus, Socket, Stack));
          }
        }
      }
    } else if (KtiHostOutPtr->FpgaPresentBitMap & (1 << Socket)) {
      //
      // Update common structures w/ updated bus numbers for FPGA to be used in CSR access routines after DXE
      //
      DEBUG ((DEBUG_INFO, "[UDS] FPGA = %x\n", Socket));
      IioUds.PlatformData.CpuQpiInfo[Socket].SegMmcfgBase = KtiHostOutPtr->CpuInfo[Socket].CpuRes.SegMmcfgBase;
      DEBUG ((DEBUG_INFO, "[UDS] Socket %2d SegMmcfgBase = %x %x\n", Socket, IioUds.PlatformData.CpuQpiInfo[Socket].SegMmcfgBase.hi, IioUds.PlatformData.CpuQpiInfo[Socket].SegMmcfgBase.lo));
    } else {
      IioUds.PlatformData.CpuQpiInfo[Socket].Valid      = 0;
      IioUds.PlatformData.CpuQpiInfo[Socket].SocId      = Socket;
      IioUds.PlatformData.IioQpiInfo[Socket].Valid      = 0;
      IioUds.PlatformData.IioQpiInfo[Socket].SocId      = Socket;
      IioUds.PlatformData.IIO_resource[Socket].Valid    = 0;
      IioUds.PlatformData.IIO_resource[Socket].SocketID = Socket;
      for (Stack = 0; Stack < IoApicPerSocket; Stack++) {
        if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
          ASSERT (Stack < IoApicPerSocket);
          PlatformInfo->SysData.SysIoApicEnable &= ~LShiftU64 (1, Socket * IoApicPerSocket + Stack + PCH_IOAPIC);
          DEBUG ((DEBUG_INFO, "[UDS] System ACPI IOAPICs enabled = 0x%016llX\n", PlatformInfo->SysData.SysIoApicEnable));
        }
      }
      DEBUG ((DEBUG_INFO, "[UDS] IoApic[%d] is invalid\n", Socket));
      DEBUG ((DEBUG_INFO, "[UDS] CPU[%d] is invalid\n", Socket));
    }
    //
    // Set PCIE port information for later usage
    //
    Status = GetPcieInfoDefaultsTable (&PcieInfoDefaults);
    ASSERT_EFI_ERROR (Status);
    for (Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
      IioUds.PlatformData.IIO_resource[Socket].PcieInfo.PortInfo[Port].Device = PcieInfoDefaults[Port].PortInfo->Device;
      IioUds.PlatformData.IIO_resource[Socket].PcieInfo.PortInfo[Port].Function = PcieInfoDefaults[Port].PortInfo->Function;
    }
  } // for (Socket...)

  DEBUG ((DEBUG_INFO, "[UDS] System ACPI IOAPICs enabled = 0x%012llX\n", PlatformInfo->SysData.SysIoApicEnable));

  IioUds.SystemStatus.numCpus                  = KtiHostOutPtr->numCpus;
  DEBUG ((DEBUG_INFO, "[UDS] num of Cpus = %d\n",IioUds.SystemStatus.numCpus));
  DEBUG ((DEBUG_INFO, "[UDS] num of IIOs = %d\n",IioUds.PlatformData.numofIIO));
  IioUds.SystemStatus.SystemRasType            = KtiHostOutPtr->SystemRasType;
  IioUds.SystemStatus.tolmLimit                = KtiHostOutPtr->tolmLimit;
  IioUds.SystemStatus.tohmLimit                = Host->var.common.tohmLimit;
  IioUds.SystemStatus.DfxRstCplBitsEn          = PcdGet8 (PcdDfxRstCplBitsEn);
  DEBUG ((DEBUG_INFO, "Common: DFX RST_CPL bits Programming Mask = 0x%x\n",IioUds.SystemStatus.DfxRstCplBitsEn));

  IioUds.SystemStatus.OutKtiCpuSktHotPlugEn    = KtiHostOutPtr->OutKtiCpuSktHotPlugEn;
  IioUds.SystemStatus.CurrentUpiiLinkSpeed     = KtiHostOutPtr->KtiCurrentLinkSpeedMode;
  IioUds.SystemStatus.CurrentUpiLinkFrequency  = KtiHostOutPtr->OutKtiLinkSpeed;

  TurboRatioLimitMsr.Uint64 = AsmReadMsr64 (ICX_MSR_TURBO_RATIO_LIMIT);
  DEBUG((DEBUG_INFO, "\n  Max Turbo Ratios : MaxTurbo1Core : MaxTurbo2Cores : MaxTurbo3Cores : MaxTurbo4Cores\n"));
  DEBUG((DEBUG_INFO,   "                          %2d              %2d               %2d               %2d\n",
        TurboRatioLimitMsr.Bits.MaxTurbo1Core,
        TurboRatioLimitMsr.Bits.MaxTurbo2Cores,
        TurboRatioLimitMsr.Bits.MaxTurbo3Cores,
        TurboRatioLimitMsr.Bits.MaxTurbo4Cores));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    IioUds.SystemStatus.OutKtiPerLinkL1En[Socket] = 0;
    for (Port = 0; Port < KtiHostOutPtr->KtiPortCnt; Port++) {
      IioUds.SystemStatus.OutKtiPerLinkL1En[Socket] |= (KtiHostOutPtr->OutKtiPerLinkL1En[Socket][Port] << Port);
    }
  }

  CopyMem (&IioUds.SystemStatus.RcVersion, &Host->nvram.common.RcVersion, sizeof (RC_VERSION));
  IioUds.SystemStatus.IsocEnable               = IfFeatureEnabled (TypeIsoc);
  IioUds.SystemStatus.meRequestedSize          = GetMeRequestedSize ();
  IioUds.SystemStatus.ieRequestedSize          = GetIeRequestedSize ();
  // RAS
  IioUds.SystemStatus.bootMode                 = GetSysBootMode ();
  IioUds.SystemStatus.LlcSizeReg               = (UINT16)((Host->var.common.procCom[0].capid0 >> 9) & 0x7);    //CAPID0.Bits[11:9]  - LLC size
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    IioUds.SystemStatus.memNode[Socket] = Host->nvram.mem.socket[Socket].enabled;
    for (ch = 0; ch < MAX_CH; ch++) {
      IioUds.SystemStatus.chEnabled[Socket][ch] = Host->nvram.mem.socket[Socket].channelList[ch].enabled;
    }
  } // Socket

  IioUds.PlatformData.EVMode           = PcdGet8 (PcdEvMode);
  // Fill in SNC Enabled bit
  IioUds.SystemStatus.OutSncEn = IfFeatureEnabled (TypeSnc);
  IioUds.SystemStatus.OutNumOfCluster = KtiHostOutPtr->OutNumOfClusterPerSystem;
  IioUds.SystemStatus.IoDcMode        = GetIoDcMode ();
  IioUds.PlatformData.RemoteRequestThreshold = GetRequestThreshold ();

#ifdef LT_FLAG
  DEBUG ((DEBUG_INFO, "[UDS] Var EVmode = %d\n",
          IioUds.PlatformData.EVMode));
  SavePlatformTxtDataToHob(&IioUds, PlatformInfo);
#endif

  // Build HOB data to forward to DXE phase now that all initialization is complete!
  BuildGuidDataHob (&UniversalDataGuid, (VOID *) &IioUds, sizeof(IIO_UDS));

  return;
} // SaveIioUdsDataToHob()


/**

  Build RAS Host Structure and save it into a HOB.

  @param Host - pointer to sysHost structure on stack.

**/
VOID
SaveRasHostStructureToHob (
  IN PSYSHOST  Host
  )
{
  struct sysRasHost rasHost;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  CopyMem ((UINT8 *)&(rasHost.var.common), (UINT8 *)&Host->var.common, sizeof (struct commonVar));
  CopyMem ((UINT8 *)&(rasHost.var.memChipPolicy), (UINT8 *)&Host->var.mem.memChipPolicy, sizeof (MEM_CHIP_POLICY));

  CopyMem ((UINT8 *)&(rasHost.setup.common), (UINT8 *)&(Setup->common), sizeof (struct commonSetup));

  BuildGuidDataHob (
               &gEfiRasHostGuid,
               &rasHost,
               sizeof (struct sysRasHost)
           );
}

/**

  Build EWL Host Structure and save it into a HOB.

  @param Host - pointer to sysHost structure on stack.

**/
VOID
SaveEWLHostStructureToHob (
  IN PSYSHOST  Host
  )
{
  EWL_PRIVATE_DATA          ewlPrivateData;
  EFI_GUID                  EWLDataGuid = EWL_ID_GUID;

  DEBUG ((DEBUG_INFO, "SaveEWLHostStructureToHob - HOB\n"));

  ZeroMem ((UINT8 *) &ewlPrivateData, sizeof (ewlPrivateData));

  CopyMem( &(ewlPrivateData), &(Host->var.common.ewlPrivateData), sizeof(ewlPrivateData));

  BuildGuidDataHob (
               &EWLDataGuid,
               &ewlPrivateData,
               sizeof (ewlPrivateData)
           );
}


/**

  Build FPGA Host Structure and save it into a HOB.

  @param Host - pointer to sysHost structure on stack.

**/
VOID
SaveFpgaDataToHob (
  IN PSYSHOST  Host
  )
{
  FPGA_CONFIGURATION FpgaConfiguration;
  UINT8              Soc;
  EFI_STATUS         Status;

  //
  // Get the FpgaConfiguration
  //
  Status = FpgaConfigurationGetValues(&FpgaConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "FpgaConfigurationGetValues returns error :%r\n", Status));
    return;
  } else {
    for (Soc = 0; Soc < MAX_SOCKET; Soc ++) {
      if (KTI_HOST_OUT_PTR->OutKtiFpgaPresent[Soc]) {
        FpgaConfiguration.FpgaSktPresent |= (1 << Soc);
      }
      if (IfFpgaActive (Soc)) {
        FpgaConfiguration.FpgaSktActive |= (1 << Soc);
      }
    }

    DEBUG ((DEBUG_INFO, "Create or Update FpgaConfiguration HOB about Kti Output\n"));
    DEBUG ((DEBUG_INFO, "FpgaSktPresent: %x; FpgaSktActive: %x; ", FpgaConfiguration.FpgaSktPresent, FpgaConfiguration.FpgaSktActive));
    FpgaConfigurationSetValues (&FpgaConfiguration);
  }
}




/**

  This function saves the S3 Restore Data to Hob for later use.

  @param Host - pointer to sysHost structure on stack

  @retval 0 - Successful
  @retval Other - Failure

**/
EFI_STATUS
MrcSaveS3RestoreDataToHob (
  IN PSYSHOST  Host
  )
{
  VOID                  *CurrentHob;
  UINTN                 RemainingHobSize;
  UINTN                 CurrentHobSize;

  RemainingHobSize = sizeof(struct sysNvram);
  CurrentHob = &(Host->nvram);
  DEBUG ((EFI_D_INFO, "MrcSaveS3RestoreDataToHob(): RemainingHobSize Size(bytes): %d\n", RemainingHobSize));
  DEBUG ((EFI_D_INFO, "    CurrentHob - address of nvram: 0x%x\n", CurrentHob));

  while (RemainingHobSize) {

    if (RemainingHobSize >= MAX_HOB_ENTRY_SIZE){
      CurrentHobSize = MAX_HOB_ENTRY_SIZE;
    } else {
      CurrentHobSize = RemainingHobSize;
    }

    BuildGuidDataHob (&gEfiMemoryConfigDataHobGuid, CurrentHob, CurrentHobSize);

    RemainingHobSize -= CurrentHobSize;
    CurrentHob = (VOID *) ((UINT32) CurrentHob + CurrentHobSize);
  }

  return EFI_SUCCESS;
}


/**

  Puts the Host structure into HOB variable

  @param Host - pointer to sysHost structure on stack

@retval VOID

**/
VOID
PublishHobData (
  IN PSYSHOST Host
   )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  if (Host == NULL) {
    ASSERT (FALSE);
    return;
  }

  SaveIioUdsDataToHob (Host);

  SaveRasHostStructureToHob (Host);

  SaveNgnCfgRecords (Host);

  SaveFpgaDataToHob (Host);

  SaveEWLHostStructureToHob (Host);

  SaveMrcThermalInfoToHob (Host);

  //
  // Save S3 restore Data into Hob
  //
  DEBUG ((DEBUG_INFO, "Save NVRAM restore data into Hob\n"));

  Status = MrcSaveS3RestoreDataToHob (Host);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Save NVRAM restore data into Hob failed with status %r\n", Status));
    ASSERT (FALSE);
    return;
  }

  //
  // Publish Platform specfic HOBs, if any
  //

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformPublishHobs ();
  }

  //
  // Signal RC HOBs are ready
  //
  PeiServicesInstallPpi (mSiliconRcHobsReadyPpi);
}

/**
  This function is called after PeiMemoryDiscoveredPpi is installed.

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_STATUS        Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PublishHobDataCallbackPeiMemoryDiscoveredPpi (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  )
{
  SYSHOST         *Host;
  MEMORY_MAP_HOST *MemMapHost;
  MSR_IA32_MISC_ENABLE_REGISTER         MiscEnable;
  VOID *GuidHob;

  //
  // Re-enable Fast Strings on BSP after CAR is disabled
  //
  MiscEnable.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);
  MiscEnable.Bits.FastStrings = 1;
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLE, MiscEnable.Uint64);

  Host = (SYSHOST *)(UINTN) PcdGet64 (PcdSyshostMemoryAddress);
  if (Host == NULL) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  MemMapHost = (MEMORY_MAP_HOST *)(UINTN) PcdGet64 (PcdMemMapHostMemoryAddress);
  if (MemMapHost == NULL) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  //
  //Get System info var hob pointer.
  //
  GuidHob = GetFirstGuidHob (&gSystemInfoVarHobGuid);
  if (GuidHob != NULL) {
    SetSysInfoVarPointer(GET_GUID_HOB_DATA(GuidHob));
  }

  SetSysHostPointer ((VOID *)Host);
  SetMemMapHostPointer ((VOID *)MemMapHost);

  //
  // Do an RcDebugPrint to force pointer to relocated HOB
  //
  RcDebugPrint (SDBG_MAX, "Move Debug Data pointer to relocated HOB...\n");
  
  // APTIOV_SERVER_OVERRIDE_RC_START
  Host->var.common.oemVariable = (UINT32) PeiServices; // Update PeiServices pointer in Host structure
  // APTIOV_SERVER_OVERRIDE_RC_END

  PublishHobData (Host);
  // APTIOV_SERVER_OVERRIDE_RC_START
  AmiOemPublishHobDataWrapper(Host); // OEM Hooks Support
  // APTIOV_SERVER_OVERRIDE_RC_END
  return EFI_SUCCESS;
}


/**
  This function is called to save MRC Host structure into memory.

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_SUCCESS           The function complete successfully.
  @retval EFI_INVALID_PARAMETER Type is not equal to AllocateAnyPages.
  @retval EFI_NOT_AVAILABLE_YET Called with permanent memory not available.
  @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated.
**/
EFI_STATUS
SaveHostToMemory(
  IN SYSHOST                       *Host
  )
{
  EFI_STATUS                       Status;
  EFI_PHYSICAL_ADDRESS             Buffer;

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES (sizeof(SYSHOST)),
             &Buffer
             );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((
    EFI_D_INFO,
    "[SaveHostToMemory]: Allocate pages size = %x, status = %r, Address = %x\n",
    EFI_SIZE_TO_PAGES (sizeof(SYSHOST)),
    Status,
    (UINT8 *)(UINTN) Buffer
    ));

  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "[SaveHostToMemory]: Failed to allocate %x pages\n",
      EFI_SIZE_TO_PAGES (sizeof(SYSHOST))
      ));
    return Status;
  }

  //
  // Copy Host structure to memory
  //
  CopyMem ((VOID *)(UINTN) Buffer, (VOID *) Host, sizeof(SYSHOST));

  //
  // Save Host memory address to PCD
  //
  Status = PcdSet64S (PcdSyshostMemoryAddress, (UINTN) Buffer);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES (sizeof(MEMORY_MAP_HOST)),
             &Buffer
             );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((
    EFI_D_INFO,
    "[SaveHostToMemory]: Allocate pages size = %x, status = %r, Address = %x\n",
    EFI_SIZE_TO_PAGES (sizeof(MEMORY_MAP_HOST)),
    Status,
    (UINT8 *)(UINTN) Buffer
    ));

  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "[SaveHostToMemory]: Failed to allocate %x pages\n",
      EFI_SIZE_TO_PAGES (sizeof(MEMORY_MAP_HOST))
      ));
    return Status;
  }

  //
  // Copy MemMapHost structure to memory
  //
  CopyMem ((VOID *)(UINTN) Buffer, GetMemMapHostPointer (), sizeof(MEMORY_MAP_HOST));

  //
  // Save MemMapHost memory address to PCD
  //
  Status = PcdSet64S (PcdMemMapHostMemoryAddress, (UINTN) Buffer);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

VOID
PrintSADInfo(
  IN PSYSHOST  Host
  )
{
  UINT8                socket;
  UINT8                i;
  UINT64               SADStartAddr;
  UINT64               SADLimit;

  DEBUG ((DEBUG_INFO, "-------------SAD Info-------------\n"));
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (Host->var.mem.socket[socket].SAD[0].Enable == 0) {
      continue;
    }
    // Traverse thru all SAD entries to build the Nfit.
    for (i = 0; i < MAX_SAD_RULES; i++) {
      if (Host->var.mem.socket[socket].SAD[i].Enable == 0) {
        continue;
      }
      if (Host->var.mem.socket[socket].SAD[i].local == 0) {
        continue;
      }
      if (i == 0) {
        SADStartAddr = 0;
      } else {
        SADStartAddr = LShiftU64 (Host->var.mem.socket[socket].SAD[i - 1].Limit, 26);
      }
      SADLimit = LShiftU64 (Host->var.mem.socket[socket].SAD[i].Limit, 26);
      DEBUG ((DEBUG_INFO, "E820 - CR: SAD - Type: %d\n", Host->var.mem.socket[socket].SAD[i].type));
      DEBUG ((DEBUG_INFO, "E820 - CR: SAD - StartAddr: 0x%16lx,  Limit:0x%16lx\n", SADStartAddr, SADLimit));
    }
  }
}


/**

  Puts the Host structure variables for POST

  @param[in] Host - pointer to sysHost structure on stack

  @retval VOID

**/

VOID
EFIAPI
PublishDataForPost (
  IN PSYSHOST  Host
  )
{

  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  //
  // Save SYSHOST into persistent memory
  // Register memory discovered PPI call back to create RC HOBs
  //
  Status = SaveHostToMemory (Host);
  if (!EFI_ERROR(Status)) {
    Status = PeiServicesNotifyPpi (&mNotifyList);
  }
  ASSERT_EFI_ERROR (Status);

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformPublishDataForPost (Host);
  }


} // PublishDataForPost

