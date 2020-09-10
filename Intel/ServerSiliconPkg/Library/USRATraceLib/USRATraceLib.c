/** @file
  USRA trace library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#include <Library/UsraAccessApi.h>
#include <Library/IoLib.h>
#include <Library/USRATrace.h>
#include <Library/MemoryCoreLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/SystemInfoLib.h>
#include <Library/PcdLib.h>
#include <Library/MemRcLib.h>

VOID
EFIAPI
DumpCpuCsrAccessVar (
  IN USRA_ADDRESS             *Address
  )
{
  UINT32                      Socket;
  UINT32                      IioStack;
  UINT32                      Imc;
  UINT32                      Ch;
  UINT32                      TypeMmioBar;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVar;
  UINT8                       MaxImc;


  Socket                 = 0;
  IioStack               = 0;
  Imc                    = 0;
  Ch                     = 0;
  TypeMmioBar            = 0;
  CpuCsrAccessVar        = NULL;
  MaxImc = GetMaxImc ();

  if (CheckIfCpuCsrAccessVarInfoDumped() == TRUE) {
    return;
  }
  //
  // Dump struct CPU_CSR_ACCESS_VAR for the first time of tracing CSR register.
  //
  if (Address->Attribute.AddrType == AddrTypeCSR) {

    CpuCsrAccessVar = GetSysCpuCsrAccessVar ();
    DEBUG ((EFI_D_ERROR, "\n"));
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->stackPresentBitmap[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->stackPresentBitmap[Socket]));
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
        DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->StackBus[%x][%x]= 0x%x: \n",Socket,IioStack,CpuCsrAccessVar->StackBus[Socket][IioStack]));
      }
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->SocketFirstBus[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->SocketFirstBus[Socket]));
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->Socket10nmUboxBus0[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->Socket10nmUboxBus0[Socket]));
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->SocketLastBus[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->SocketLastBus[Socket]));
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->segmentSocket[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->segmentSocket[Socket]));
    }
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->socketPresentBitMap = 0x%x: \n",CpuCsrAccessVar->socketPresentBitMap));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->FpgaPresentBitMap = 0x%x: \n",CpuCsrAccessVar->FpgaPresentBitMap));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->mmCfgBase = 0x%x: \n",CpuCsrAccessVar->mmCfgBase));
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->mmCfgBaseH[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->mmCfgBaseH[Socket]));
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->mmCfgBaseL[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->mmCfgBaseL[Socket]));
    }
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->DdrMaxCh = 0x%x: \n",CpuCsrAccessVar->DdrMaxCh));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->DdrMaxImc = 0x%x: \n",CpuCsrAccessVar->DdrMaxImc));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->DdrNumChPerMc = 0x%x: \n",CpuCsrAccessVar->DdrNumChPerMc));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->HbmMaxIoInst = 0x%x: \n",CpuCsrAccessVar->HbmMaxIoInst));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->HbmMaxCh = 0x%x: \n",CpuCsrAccessVar->HbmMaxCh));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->HbmNumChPerMc = 0x%x: \n",CpuCsrAccessVar->HbmNumChPerMc));
    DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->HbmNumChPerIo = 0x%x: \n",CpuCsrAccessVar->HbmNumChPerIo));
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      for (Imc = 0; Imc < MaxImc; Imc++) {
        DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->imcEnabled[%x][%x]= 0x%x: \n",Socket,Imc,CpuCsrAccessVar->imcEnabled[Socket][Imc]));
      }
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      for (Ch = 0; Ch < MAX_CH; Ch++) {
        DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->mcId[%x][%x]= 0x%x: \n",Socket,Ch,CpuCsrAccessVar->mcId[Socket][Ch]));
      }
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      for (TypeMmioBar = 0; TypeMmioBar < TYPE_MAX_MMIO_BAR; TypeMmioBar++) {
        DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->MmioBar[%x][%x]= 0x%x: \n",Socket,TypeMmioBar,CpuCsrAccessVar->MmioBar[Socket][TypeMmioBar]));
      }
    }
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      DEBUG ((EFI_D_ERROR, "CpuCsrAccessVar->HcxType[%x]= 0x%x: \n",Socket,CpuCsrAccessVar->HcxType[Socket]));
    }
  }
  DisableCpuCsrAccessVarInfoDump();

}

/**
  This routine is for print register info to serial port.

  @param  Address: A pointer of the address of the Common Address Structure to write to.
  @param  AlignedAddress: physical address converted from Address.
  @param  ReadFlag: TRUE -- read; FALSE -- Write.
  @param  Buffer A pointer of buffer for the value write to the CSR.

  @retval VOID

**/
VOID
USRATrace (
  IN USRA_ADDRESS             *Address,
  IN UINTN                    AlignedAddress,
  IN UINT8                    Operation,
  IN VOID                     *Buffer1,
  IN VOID                     *Buffer2
  )
{
  UINT8                       UsraTraceConfigurationL;
  UINT8                       UsraTraceConfigurationH;
  UINT16                      UsraTraceConfiguration;

  UsraTraceConfigurationL = 0;
  UsraTraceConfigurationH = 0;
  UsraTraceConfiguration = GetUsraTraceConfiguration();

  // UsraTraceConfigurationL
  // Bit               7       6        5           4                     3:0
  //               Write, Modify, GetAddr, Long/Short, Trace 0x5 as signature
  // UsraTraceConfigurationH
  // Bit               7                                 6:3    2     1     0
  // DumpCpuCsrAccessVar,                                TBD, CSR, PCIE, Read

  if (PcdGetBool (PcdUsraTraceConfigurationPortEnable ) == TRUE) {
    UsraTraceConfigurationL = IoRead8((UINTN)PcdGet8 (PcdUsraTraceConfigurationPortLow));
    UsraTraceConfigurationH = IoRead8((UINTN)PcdGet8 (PcdUsraTraceConfigurationPortHigh));
  } else {
    if (CheckUsraTraceContolFlag() == TRUE) {
      UsraTraceConfigurationL = (UINT8)(UsraTraceConfiguration & 0xFF);
      UsraTraceConfigurationH = (UINT8)(UsraTraceConfiguration >> 8);
    } else {
      UsraTraceConfigurationL = 0;
      UsraTraceConfigurationH = 0;
    }
  }
  if ((UsraTraceConfigurationL & 0x0F) != 0x5) {
    return;
  }

  switch (Operation) {
  case TraceRead:
    if ((UsraTraceConfigurationH & 0x01) == 0 ) {
      return;
    }
    break;
  case TraceWrite:
    if ((UsraTraceConfigurationL & 0x80) == 0 ) {
      return;
    }
    break;
  case TraceModify:
    if ((UsraTraceConfigurationL & 0x40) == 0 ) {
      return;
    }
    break;
  case TraceGetAddr:
    if ((UsraTraceConfigurationL & 0x20) == 0 ) {
      return;
    }
    break;
  default:
    DEBUG ((EFI_D_ERROR, "[USRA] Invalid  Operation value: 0x%x \n", Operation));
    return;
  }

  if (((Address->Attribute.AddrType == AddrTypePCIE) || (Address->Attribute.AddrType == AddrTypePCIEBLK)) && ((UsraTraceConfigurationH & BIT1) == 0)) {
    return;
  }
  if ((Address->Attribute.AddrType == AddrTypeCSR) && ((UsraTraceConfigurationH & BIT2) == 0)) {
    return;
  }

  if ((UsraTraceConfigurationH & BIT7) != 0) {
    DumpCpuCsrAccessVar(Address);
  }

  DEBUG ((EFI_D_ERROR, "\n"));
  switch (Operation) {
  case TraceRead:
    DEBUG ((EFI_D_ERROR, "[USRA] R  ")); //R - Read
    break;
  case TraceWrite:
    DEBUG ((EFI_D_ERROR, "[USRA] W  ")); //W - Write
    break;
  case TraceModify:
    DEBUG ((EFI_D_ERROR, "[USRA] M  ")); //M - Modify
    break;
  case TraceGetAddr:
    DEBUG ((EFI_D_ERROR, "[USRA] GA ")); //GA - GetAddr
    break;
  default:
    break;
  }

  switch ((UINT32)Address->Attribute.AccessWidth) {
  case  UsraWidth8:
    DEBUG ((EFI_D_ERROR, "B  ")); //B - Byte
    break;
  case  UsraWidth16:
    DEBUG ((EFI_D_ERROR, "W  ")); //W - Word
    break;
  case  UsraWidth32:
    DEBUG ((EFI_D_ERROR, "DW ")); //DW - DWord
    break;
  default:
    DEBUG ((EFI_D_ERROR, "QW ")); //QW - QWord
    break;
  }

  if ((UsraTraceConfigurationL & 0x10) != 0) {   // check if bit4 =1, to Display long trace
    if (Address->Attribute.AddrType == AddrTypeCSR) {
      DEBUG ((EFI_D_ERROR, " Skt:%02x  BI:%02x  ", Address->Csr.SocketId, Address->Csr.InstId)); // Skt - Socket   BI - BoxInst
      switch (((CSR_OFFSET *)&Address->Csr.Offset)->Bits.boxtype) {
      case  BOX_CHA_MISC:
        DEBUG ((EFI_D_ERROR, "CHA_MISC      "));
        break;
      case  BOX_CHA_PMA:
        DEBUG ((EFI_D_ERROR, "CHA_PMA       "));
        break;
      case  BOX_CHA_CMS:
        DEBUG ((EFI_D_ERROR, "CHA CMS       "));
        break;
      case  BOX_CHABC:
        DEBUG ((EFI_D_ERROR, "CHABC         "));
        break;
      case  BOX_PCU:
        DEBUG ((EFI_D_ERROR, "PCU           "));
        break;
      case  BOX_VCU:
        DEBUG ((EFI_D_ERROR, "VCU           "));
        break;
      case  BOX_M2MEM:
        DEBUG ((EFI_D_ERROR, "M2MEM         "));
        break;
      case  BOX_MC:
        DEBUG ((EFI_D_ERROR, "MC            "));
        break;
      case  BOX_MCIO:
        DEBUG ((EFI_D_ERROR, "MCIO          "));
        break;
      case  BOX_KTI:
        DEBUG ((EFI_D_ERROR, "KTI           "));
        break;
      case  BOX_M3KTI:
        DEBUG ((EFI_D_ERROR, "M3KTI         "));
        break;
      case  BOX_MCDDC:
        DEBUG ((EFI_D_ERROR, "MCDDC         "));
        break;
      case  BOX_M2UPCIE:
        DEBUG ((EFI_D_ERROR, "M2UPCIE       "));
        break;
      case  BOX_IIO_PCIE_DMI:
        DEBUG ((EFI_D_ERROR, "IIO PCIE DMI  "));
        break;
      case  BOX_IIO_PCIE:
        DEBUG ((EFI_D_ERROR, "IIO PCIE      "));
        break;
      case  BOX_IIO_PCIE_NTB:
        DEBUG ((EFI_D_ERROR, "IIO PCIE NTB  "));
        break;
      case  BOX_IIO_CB:
        DEBUG ((EFI_D_ERROR, "IIO CB        "));
        break;
      case  BOX_IIO_VTD:
        DEBUG ((EFI_D_ERROR, "IIO VTD       "));
        break;
      case  BOX_IIO_DFX:
        DEBUG ((EFI_D_ERROR, "IIO DFX       "));
        break;
      case  BOX_UBOX:
        DEBUG ((EFI_D_ERROR, "UBOX          "));
        break;
      case  BOX_MS2IDI:
        DEBUG ((EFI_D_ERROR, "MS2IDI        "));
        break;
      case  BOX_FPGA:
        DEBUG ((EFI_D_ERROR, "FPGA          "));
        break;
      case  BOX_HBM:
        DEBUG ((EFI_D_ERROR, "HBM           "));
        break;
      case  BOX_HBM2E_MC:
        DEBUG ((EFI_D_ERROR, "HBM MC        "));
        break;
      case  BOX_SPD:
        DEBUG ((EFI_D_ERROR, "SPD           "));
        break;
      case  BOX_MSM:
        DEBUG ((EFI_D_ERROR, "MSM           "));
        break;
      case  BOX_RLINK:
        DEBUG ((EFI_D_ERROR, "RLINK         "));
        break;
      case  BOX_SB:
        DEBUG ((EFI_D_ERROR, "SB            "));
        break;
      case  BOX_FNV:
        DEBUG ((EFI_D_ERROR, "FNV           "));
        break;
      case  BOX_SMEE:
        DEBUG ((EFI_D_ERROR, "SMEE          "));
        break;
      case  BOX_SB_I3C:
        DEBUG ((EFI_D_ERROR, "I3C           "));
        break;
      default:
        break;
      }
      DEBUG ((EFI_D_ERROR, "FB:%x  CO:%04x ", ((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.funcblk, (UINT16)((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.offset)); //FB - FuncBlk   CO - CsrOffset
    } else if(Address->Attribute.AddrType == AddrTypePCIE){
      DEBUG ((EFI_D_ERROR, "S:%x  B:%02x  D:%02x  F:%x  O:%03x ", Address->Pcie.Seg, Address->Pcie.Bus, Address->Pcie.Dev, Address->Pcie.Func, Address->Pcie.Offset)); //O - Offset
    }
  }

  if (Operation == TraceRead || Operation == TraceWrite) {
    DEBUG ((EFI_D_ERROR, " (0x%x)  ", AlignedAddress));
    switch ((UINT32)Address->Attribute.AccessWidth) {
    case  UsraWidth8:
      DEBUG ((EFI_D_ERROR, "V: %x \n", *((UINT8*)Buffer1))); //V - Value
      break;
    case  UsraWidth16:
      DEBUG ((EFI_D_ERROR, "V: %x \n", *((UINT16*)Buffer1)));
      break;
    case  UsraWidth32:
      DEBUG ((EFI_D_ERROR, "V: %x \n", *((UINT32*)Buffer1)));
      break;
    default:
      DEBUG ((EFI_D_ERROR, "V: %lx \n",  *((UINT64*)Buffer1)));
      break;
    }
  } else if (Operation == TraceModify) {
    switch ((UINT32)Address->Attribute.AccessWidth) {
    case  UsraWidth8:
      DEBUG ((EFI_D_ERROR, "AM: %x", *((UINT8*)Buffer1)));  //AM - AndMask
      DEBUG ((EFI_D_ERROR, "  OM: %x \n", *((UINT8*)Buffer2))); //OM - OrMask
      break;
    case  UsraWidth16:
      DEBUG ((EFI_D_ERROR, "AM: %x", *((UINT16*)Buffer1)));
      DEBUG ((EFI_D_ERROR, "  OM: %x \n", *((UINT16*)Buffer2)));
      break;
    case  UsraWidth32:
      DEBUG ((EFI_D_ERROR, "AM: %x", *((UINT32*)Buffer1)));
      DEBUG ((EFI_D_ERROR, "  OM: %x \n", *((UINT32*)Buffer2)));
      break;
    default:
      DEBUG ((EFI_D_ERROR, "AM: %x", *((UINT64*)Buffer1)));
      DEBUG ((EFI_D_ERROR, "  OM: %x \n", *((UINT64*)Buffer2)));
      break;
    }
  } else {
    DEBUG ((EFI_D_ERROR, "PA: 0x%x \n", AlignedAddress)); //PA - Physical Address
  }
}
