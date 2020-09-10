//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#ifndef _RT_ERRORLOG_BOARD_H_
#define _RT_ERRORLOG_BOARD_H_

#include "Token.h"
#include <RtErrorLogElinks.h>
#include <Guid/ErrorReportLibListener.h>
#include <Guid/RasNonStandardErrorSection.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <IehTypes.h>
#include <PCIeErrorTypes.h>
#include <Register/IioRegDef.h>
#include <RcRegs.h>


//
// PCI Error Status Bits
//
#define PCI_STS_MSTR_DATA_PERR              BIT8
#define PCI_STS_SIG_TRGT_ABRT               BIT11
#define PCI_STS_REC_TRGT_ABRT               BIT12
#define PCI_STS_SIG_SERR                    BIT14
#define PCI_STS_DET_PERR                    BIT15

#define CRITICAL_INTERRUPT_PCI_PERR         0x04
#define CRITICAL_INTERRUPT_PCI_SERR         0x05
#define CRITICAL_INTERRUPT_BUS_CORERR       0x07
#define CRITICAL_INTERRUPT_BUS_UNCERR       0x08
#define CRITICAL_INTERRUPT_BUS_FATERR       0x0A


#define LASTBOOT_PCIE_ERROR                 0x0F

#define RT_LOCAL_IEH_DEV                    0x00
#define RT_LOCAL_IEH_FUN                    0x04
#pragma pack(1)

typedef struct {
  UINT8   				Socket;
  UINT8   				Branch;
  UINT8   				MemoryController;
  UINT8   				FbdChannel;
  UINT8   				DdrChannel;
  UINT8   				Dimm;
} MEM_ERROR_LOCATION;

/*
typedef struct {
  ERROR_SOURCE       ErrorType;
  UINT8                           ErrorSeverity;
} ERROR_HEADER;

typedef struct {
  ERROR_HEADER   	Header;
  UINT8   			ErrorSubType;
  UINT8   			Socket;
  UINT8             Stack;
  UINT8   			Port;
  UINT8   			Data[1];
} ERROR_DATA_TYPE1;

typedef struct _IOH_QPI_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             IohQpiErrorRecord;
}IOH_QPI_COMBINED_ERROR_RECORD;

typedef struct _IOH_CORE_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             IohCoreErrorRecord;
}IOH_CORE_COMBINED_ERROR_RECORD;

typedef struct _IOH_VTD_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             IohVtdErrorRecord;
}IOH_VTD_COMBINED_ERROR_RECORD;

typedef struct _ITC_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             ItcErrorRecord;
} ITC_COMBINED_ERROR_RECORD;

typedef struct _OTC_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             OtcErrorRecord;
} OTC_COMBINED_ERROR_RECORD;

typedef struct _DMA_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             DmaErrorRecord;
} DMA_COMBINED_ERROR_RECORD;

typedef struct {
  //
  // Header-like information...
  //
  ERROR_HEADER    Header;
  //
  // Essential information...
  //
  UINT8                                   Socket;
  UINT8                                   UpiPort;
  BOOLEAN                            Valid;
  UINT8                                   CorrectedErrorCount;
  UINT64				  Address;
  UINT16                                 ModelSpecificCode; // Will go as extended data...
  UINT16                                 MachineCheckCode; // Will go as extended data...
} UPI_ERROR_RECORD;

typedef struct {
  //
  // Header-like information...
  //
ERROR_HEADER    Header;
  //
  // Essential information...
  //
  UINT8                                   Socket;
  UINT8                                   CoreMcaBank;
  BOOLEAN                            Valid;
  UINT8                                   CorrectedErrorCount;
  UINT16                                  ModelSpecificCode; // Will go as extended data...
  UINT16                                  MachineCheckCode; // Will go as extended data...
} CORE_ERROR_RECORD;

typedef struct {
  //
  // Header-like information...
  //
ERROR_HEADER    Header;
  //
  // Essential information...
  //
  UINT8                                   Socket;
  BOOLEAN                                 Valid;
  UINT8                                   CorrectedErrorCount;
  UINT16                                  ErrorAddress;
  UINT16                                  ModelSpecificCode; // Will go as extended data...
  UINT16                                  MachineCheckCode; // Will go as extended data...
} PCU_ERROR_RECORD;

typedef struct {
  //
  // Header-like information...
  //
  ERROR_HEADER    Header;
  //
  // Essential information...
  //
  UINT8                                   Socket;
  UINT8                                   CoreId;
  UINT8					 				  ThreadId;
  UINT8                                   ChaMcaBank;
  BOOLEAN                            Valid;
  UINT8                                   CorrectedErrorCount;
  UINT64				  				  Address;
  UINT16                                  ModelSpecificCode; // Will go as extended data...
  UINT16                                  MachineCheckCode; // Will go as extended data...
} CHA_ERROR_RECORD;
*/

#pragma pack()

#endif
