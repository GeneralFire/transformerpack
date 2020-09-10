/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Library/PerformanceTrackerLib.h>
#include <Library/RcDebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/HobLib.h>

#include <ReferenceCodeFatalErrors.h>
#include <Memory/MemoryCheckpointCodes.h>

#include "PerformanceTrackerLibInternal.h"

/**
  Copy performance tracker data to storage

  @param[in]      Data                  - Pointer to data to store
  @param[in]      NumTrackers           - Pointer to number of trackers to move to storage
                                          Will be updated with actual number of trackers stored
  @param[in,out]  Storage               - Pointer to storage pointer
                                          Will be updated to point to the next available storage location
  @param[in,out]  AvailableStorageSize  - Pointer storage size
                                          Will be updated to reflect the amount of storage available

  @retval MRC_STATUS_SUCCESS if data is copied to storage
          MRC_STATUS_INVALID_PARAMETER if an NULL pointer is passed in
          MRC_STATUS_BUFFER_TOO_SMALL if insufficient storage

**/

MRC_STATUS
LoadPerfTrackerStorage (
  IN      PERFORMANCE_TRACKER_DESC      *Data,
  IN      UINT16                        *NumTrackers,
  IN OUT  PERFORMANCE_TRACKER_DATA      **Storage,
  IN OUT  UINT32                        *AvailableStorageSize
  )
{
  UINT16          Index;
  UINT32          DataSize;
  MRC_STATUS      Status = MRC_STATUS_SUCCESS;

  if ((*Storage == NULL) || (NumTrackers == NULL)) {
    return MRC_STATUS_INVALID_PARAMETER;
  }

  //
  // Verify there is enough storage for the data
  // If not, use what is available.
  //
  DataSize = *NumTrackers * sizeof (PERFORMANCE_TRACKER_DATA);
  if (DataSize > *AvailableStorageSize) {
    RcDebugPrint (SDBG_DEFAULT, "WARNING: Insufficient performance tracker storage - need: %d, have: %d\n", DataSize, *AvailableStorageSize);

    *NumTrackers = (UINT16)(*AvailableStorageSize / sizeof (PERFORMANCE_TRACKER_DATA));
    DataSize = *NumTrackers * sizeof (PERFORMANCE_TRACKER_DATA);
    Status = MRC_STATUS_BUFFER_TOO_SMALL;

    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_141);
  }

  //
  // Copy caller's data into storage
  //
  ZeroMem (*Storage, DataSize);
  for (Index = 0; Index < *NumTrackers; Index++) {
    (*Storage)->MemPhase  = Data->MemPhase;
    (*Storage)->String    = Data->String;
    Data      = Data + 1;
    *Storage  = *Storage + 1;
  }

  *AvailableStorageSize = *AvailableStorageSize - DataSize;

  return Status;

} // LoadPerfTrackerStorage

/**
  Intialize the performance tracker context structure

  @param[in]  None

  @retval MRC_STATUS
          MRC_STATUS_OUT_OF_RESOURCES otherwise

**/
MRC_STATUS
InitPerfTrackerData (
  VOID
  )
{
  MRC_STATUS                  Status = MRC_STATUS_SUCCESS;

  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

    PERF_TRACKER_CONTEXT        *PtContext;
    UINT16                      NumTrackers;
    UINT32                      AvailableStorageSize;
    PERFORMANCE_TRACKER_DATA    *PerfTrackerStorage;
    PERFORMANCE_TRACKER_DESC    PerfTrackerDesc[] = {
          {PTZ_PRE_MRC,                  "PreMrc"             },
          {PTZ_PIPE_SYNC,                "PipeSync"           },
          {PTZ_SELECT_BOOT_MODE,         "SelectBootMode"     },
          {PTZ_INIT_STRUCT_LATE,         "InitStructLate"     },
          {PTZ_DIMM_DETECT,              "DetectDimmConfig"   },
          {PTZ_CHECK_POR,                "CheckPor"           },
          {PTZ_HBM_PRE_TRAINING,         "HBM Pre-Training"   },
          {PTZ_CLOCK_INIT,               "Clock Init"         },
          {PTZ_UNLOCK_MEM_REGS,          "UnlockMemRegs"      },
          {PTZ_CHECK_STATUS,             "CheckStatus"        },
          {PTZ_CONFIG_XMP,               "ConfigXmp"          },
          {PTZ_SET_CLK_VDD,              "SetClkVdd"          },
          {PTZ_CHECK_DIMM_RANKS,         "CheckDimmRanks"     },
          {PTZ_EARLY_DDR_THERM,          "EarlyDdrTherm"      },
          {PTZ_EARLY_INIT_MEM,           "EarlyInitMem"       },
          {PTZ_HBM_TRAINING,             "HBM Training"       },
          {PTZ_GATHER_SPD,               "GatherSPDData"      },
          {PTZ_DIMM_INFO_DISPLAY,        "DisplayDimmInfo"    },
          {PTZ_NVDIMM_STATUS,            "NvDimmStatus"       },
          {PTZ_EARLY_CONFIG,             "ChannelEarlyConfig" },
          {PTZ_JEDEC_INIT,               "JedecInitSequence"  },
          {PTZ_DDRIO_INIT,               "InitDdrioInterface" },
          {PTZ_XOVER_CALIB,              "X-Over Calib"       },
          {PTZ_PACKAGE_DELAY,            "SetStartingCCC"     },
          {PTZ_RCOMP_STAT_LEG,           "RcompStaticLeg"     },
          {PTZ_SENSE_AMPLIFIER,          "Sense Amp"          },
          {PTZ_PRE_TRAINING_INIT,        "DdrPreTrainingInit" },
          {PTZ_CS_CLK_EARLY,             "CsClockEarly"       },
          {PTZ_CA_CLK_EARLY,             "CaClockEarly"       },
          {PTZ_CA_SLEW_RATE,             "DcaSlewRate"        },
          {PTZ_DCA_DCLK_DUTY_CYCLE,      "RcdDcaDckDutyCycle" },
          {PTZ_LRDIMM_BCOM,              "Lrdimm Bcom Train"  },
          {PTZ_DCA_DFE,                  "DcaDfeTraining"     },
          {PTZ_EARLY_CS_CLK_BACKSIDE,    "BsCsClockEarly"     },
          {PTZ_EARLY_CA_CLK_BACKSIDE,    "BsCaClockEarly"     },
          {PTZ_LRDIMM_PBA_ENUMERATE_ID,  "Lrdimm Pba Enu Id"  },
          {PTZ_EARLY_REQ_CLK,            "Early Req Clk Train"},
          {PTZ_CTL_CLK_EARLY,            "CtlClockEarly"      },
          {PTZ_CMD_CLK_EARLY,            "CmdClockEarly"      },
          {PTZ_QxCA_CLK,                 "BacksideCmdCtl"     },
          {PTZ_LRDIMM_RX,                "LRDIMM RX"          },
          {PTZ_REC_EN,                   "Receive Enable"     },
          {PTZ_RDDQDQS,                  "Rd Dq/Dqs"          },
          {PTZ_SWIZZLE_DISCOVERY,        "Swizzle Discovery"  },
          {PTZ_DRAM_DUTY_CYCLE_ADJ,      "Dram Duty Cycle Adj"},
          {PTZ_RDDQDQS_DFE,              "RdDqDqsDfeCentering"},
          {PTZ_TRAIN_RX_POST_DFE,        "Rx Dq/Dqs Post Dfe" },
          {PTZ_SWITCH_DDRT2_MODE,        "Switch DDRT2 Mode"  },
          {PTZ_LRDIMM_TX,                "LRDIMM TX"          },
          {PTZ_WR_LVL,                   "Write Leveling"     },
          {PTZ_WR_FLYBY,                 "Wr Fly-By"          },
          {PTZ_WRDQDQS,                  "Wr Dq/Dqs"          },
          {PTZ_DQ_SLEW_RATE,             "Tx DQ Slew Rate"    },
          {PTZ_WRDQDQS_DFE,              "WrDqDqsDfeCentering"},
          {PTZ_POST_PKG_RPR,             "PostPkgRepair"      },
          {PTZ_DDRIO_RCSIM_CACHE,        "DDRIORcsimCache"    },
          {PTZ_E_WR_VREF,                "Early Wr Vref"      },
          {PTZ_E_RD_VREF,                "Early Rd Vref"      },
          {PTZ_TRAIN_CMD_TX_EQ,          "Cmd Tx Eq"          },
          {PTZ_CMD_VREF_CEN,             "Cmd Vref"           },
          {PTZ_CMD_CLK,                  "CmdClockTraining"   },
          {PTZ_E_RID,                    "Early Rd ID"        },
          {PTZ_TX_DQ_CENTERING_LRDIMM,   "LRDIMM Tx DQ Cen"   },
          {PTZ_RX_DQ_CENTERING_LRDIMM,   "LRDIMM Rx DQ Cen"   },
          {PTZ_WR_VREF_LRDIMM,           "LRDIMM Wr Vref"     },
          {PTZ_RD_VREF_LRDIMM,           "LRDIMM Rd Vref"     },
          {PTZ_BS_WR_LVL_LRDIMM,         "LRDIMM Bs Write"    },
          {PTZ_TRAIN_RX_ODT,             "RXODT"              },
          {PTZ_TRAIN_DRAM_RON,           "DRAMRON"            },
          {PTZ_TRAIN_MC_RON,             "MCRON"              },
          {PTZ_TRAIN_RTT_WR,             "RTTWR"              },
          {PTZ_TRAIN_TX_EQ,              "TX EQ"              },
          {PTZ_TRAIN_DRAM_RX_EQ,         "Dram Rx Eq"         },
          {PTZ_TX_ECHO,                  "Dram Tx Echo"       },
          {PTZ_LEGACY_RXDFE,             "Legacy RxDfe"       },
          {PTZ_TRAIN_IMODE,              "IMODE"              },
          {PTZ_TRAIN_CTLE,               "CTLE"               },
          {PTZ_TRAIN_TX_RFSR,            "TX RFSR"            },
          {PTZ_TRAIN_TCO_COMP,           "TCO COMP"           },
          {PTZ_TRAIN_DCA_TCO,            "DCA TCO"            },
          {PTZ_TRAIN_TCO_DQDQS,          "TCO_DQDQS"          },
          {PTZ_TRAIN_TX_ODT,             "TX ODT"             },
          {PTZ_BIT_DESKEW_TX,            "Tx Per Bit Deskew"  },
          {PTZ_BIT_DESKEW_RX,            "Rx Per Bit Deskew"  },
          {PTZ_WR_VREF,                  "Wr Vref"            },
          {PTZ_RD_VREF,                  "Rd Vref"            },
          {PTZ_WRDQDQS_ADV,              "Tx DqDqs Adv"       },
          {PTZ_RDDQDQS_ADV,              "Rx DqDqs Adv"       },
          {PTZ_TRAIN_PXC,                "PXC"                },
          {PTZ_TRAIN_DDJC,               "DDJC"               },
          {PTZ_TRAIN_RX_DFE,             "RX DFE"             },
          {PTZ_TRAIN_READ_DFE,           "READ DFE"           },
          {PTZ_TRAIN_WRITE_DFE,          "WRITE DFE"          },
          {PTZ_TRAIN_TX_POST_DFE,        "Tx Dq/Dqs Post Dfe" },
          {PTZ_TRAIN_TX_RETRAIN,         "Periodic Tx Retrain"},
          {PTZ_RT_OPT,                   "Round Trip Opt"     },
          {PTZ_TURNAROUND,               "TurnaroundTrain"    },
          {PTZ_DISPLAY_RESULTS,          "DisplayResults"     },
          {PTZ_POST_TRAINING_INIT,       "PostTrainingInit"   },
          {PTZ_RANK_MARGIN_TOOL,         "Rk Margin Tool"     },
          {PTZ_SSA_API_INIT_ST,          "ExecuteSsaRmt"      },
          {PTZ_MEM_HEALTH_TEST,          "MemHealthTest"      },
          {PTZ_NVDIMM_RESTORE,           "NvDimmRestore"      },
          {PTZ_NVDIMM_ARM,               "NvDimmArm"          },
          {PTZ_HBM_POST_TRAINING,        "HBM Post-Training"  },
          {PTZ_LATE_CONFIG,              "LateConfig"         },
          {PTZ_MID_DDR_THERM,            "InitThrottling"     },
          {PTZ_LATE_DDR_THERM,           "InitThrottlingLate" },
          {PTZ_MEM_BIST,                 "MEMTEST"            },
          {PTZ_SVL_SCRAMBLE,             "SvlAndScrambling"   },
          {PTZ_MEM_INIT,                 "MEMINIT"            },
          {PTZ_CMI_CREDIT,               "CmiCreditProg"      },
          {PTZ_CHECK_RAS,                "CheckRasPostMrc"    },
          {PTZ_MEM_LATE,                 "MemLate"            },
          {PTZ_MEM_MAPPING,              "Memory Mapping"     },
          {PTZ_HBM_NORMAL_MODE,          "HBM Normal Mode"    },
          {PTZ_NORMAL_MODE,              "Normal Mode"        },
          {PTZ_INIT_ADR,                 "InitAdr"            },
          {PTZ_RAS_CONFIG,               "RAS Config"         },
          {PTZ_CALL_TABLE,               "CallTableOverhead"  },
          {PTZ_SET_CPL2,                 "Set Cpl2"           },
          {PTZ_LDRIMM_DB_DFE,            "DB DFE"             },
          {PTZ_NORMALIZE_CMD,            "Normalize CMD"      },
        };

    //
    // We allocate memory in this function via AllocatePages rather than RcAllocatePool
    // because we have a circular dependency when using the RcAllocatePool path in Sim
    // builds. RcAllocatePool generates a debug print, which causes a recursive call back
    // into this function when the debug data is retrieved from the static pointer table.
    //
    // Allocating this memory via AllocatePages avoids the whole recursion mess since
    // no debug prints are generated. We have plenty of space in the Heap assocated with
    // AllocatePages so this should present no problem.
    //

    PtContext = NULL;
    PtContext = AllocatePages (EFI_SIZE_TO_PAGES (sizeof (PERF_TRACKER_CONTEXT)));
    if (PtContext == NULL) {

      //
      // we ASSERT in this function rather than call RC_FATAL_ERROR for the
      // same reason we use AllocatePages rather than RcAllocatePool.
      //

      ASSERT (FALSE);
      return MRC_STATUS_OUT_OF_RESOURCES;
    }

    ZeroMem (PtContext, sizeof (PERF_TRACKER_CONTEXT));

    //
    // Wait until the context is fully initialized before attempting to count data
    //
    PtContext->PerfTrackerEnabled  = FALSE;

    PerfTrackerStorage = NULL;
    PerfTrackerStorage = AllocatePages (EFI_SIZE_TO_PAGES (sizeof (PERFORMANCE_TRACKER_DATA) *  PTZ_MAX_ZONE));
    if (PerfTrackerStorage == NULL) {
      ASSERT (FALSE);
      return MRC_STATUS_OUT_OF_RESOURCES;
    }

    ZeroMem (PerfTrackerStorage, sizeof (PERFORMANCE_TRACKER_DATA) *  PTZ_MAX_ZONE);

    PtContext->PerformanceData     = PerfTrackerStorage;
    AvailableStorageSize              = sizeof (PERFORMANCE_TRACKER_DATA) *  PTZ_MAX_ZONE;

    //
    // The performance tracker descriptions are stored as array of type PERFORMANCE_TRACKER_DESC.
    // The performace data is stored as array of type PERFORMANCE_TRACKER_DATA.
    // Find the number of performance trackers and multiply by the size of PEFORMANCE_TRACKER_DATA
    // to determine total amount of storage required, in bytes.
    //
    NumTrackers = sizeof (PerfTrackerDesc) / sizeof (PerfTrackerDesc[0]);
    Status = LoadPerfTrackerStorage (PerfTrackerDesc, &NumTrackers, &PerfTrackerStorage, &AvailableStorageSize);
    PtContext->ZoneCount = NumTrackers;

    //
    // We store the data in the static pointer table
    // so set it in there.
    //

    SetMrcPerformanceDataPointer ((VOID *)PtContext);

    PtContext->Signature = PERF_TRACKER_CONTEXT_SIGNATURE;

    PtContext->CurrentMemPhase     = PTZ_PRE_MRC;
    PtContext->PerfTrackerEnabled  = TRUE;

  }

  return Status;

} // InitPerfTrackerData

/**
  Provide a pointer to the performance tracker context structure

  @param[out] PtContext   - Pointer to caller's perf tracker context pointer

  @retval MRC_STATUS_SUCCESS if PtContext is updated to point to the perf tracker context
          MRC_STATUS_NOT_FOUND otherwise

**/
MRC_STATUS
GetPerfTrackerContext (
  OUT     PERF_TRACKER_CONTEXT      **PtContext
  )
{

  *PtContext = NULL;
  *PtContext = GetMrcPerformanceDataPointer ();

  if ((*PtContext == NULL) ||
      ((*PtContext)->Signature != PERF_TRACKER_CONTEXT_SIGNATURE)) {

    if (MRC_STATUS_SUCCESS != InitPerfTrackerData ()) {
      ASSERT (FALSE);
      return MRC_STATUS_NOT_FOUND;
    }

    *PtContext = GetMrcPerformanceDataPointer ();
    if ((*PtContext == NULL) ||
      ((*PtContext)->Signature != PERF_TRACKER_CONTEXT_SIGNATURE)) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_141);
    }

  }

  return MRC_STATUS_SUCCESS;

} // GetPerfTrackerContext

/**

  Counts this data access

  @param[in] PerfDataPtr  - Pointer to current performance pointer data structure
  @param[in] Counter      - The performance counter
  @param[in] Data         - Data to add to counter

  @retval N/A

**/
VOID
CountData (
  IN    PERF_DATA             *PerfDataPtr,
  IN    PERFORMANCE_COUNTER   Counter,
  IN    UINT32                Data
  )
{

  switch (Counter) {
  case PTC_PCI_ACCESS:
    (*PerfDataPtr).PciCount += Data;
    break;

  case PTC_JEDEC_COUNT:
    (*PerfDataPtr).JedecCount += Data;
    break;

  case PTC_FIXED_DELAY:
    (*PerfDataPtr).FixedDelay += Data;
    break;

  case PTC_POLLING_COUNT:
    (*PerfDataPtr).PollingCount += Data;
    break;

  case PTC_VREF_MOVE:
    (*PerfDataPtr).VrefCount += Data;
    break;

  case PTC_CPGC_COUNT:
    (*PerfDataPtr).CpgcCount += Data;
    break;

  case PTC_DURATION_TIME:
    (*PerfDataPtr).DurationTime += Data;
    break;

  case PTC_SMBUS_READ:
    (*PerfDataPtr).SmbusRead += Data;
    break;

  case PTC_SMBUS_WRITE:
    (*PerfDataPtr).SmbusWrite += Data;
    break;

  case PTC_MRS_WRITE:
    (*PerfDataPtr).MrsWrite += Data;
    break;

  case PTC_GETSYSHOSTPTR_CALLS:
    (*PerfDataPtr).GetSysHostPointerCalls += Data;
    break;

  default:
    break;
  }
} // CountData

/**

  Print statistics

  @param[in] PerfDataPtr  - Pointer to current performance pointer data structure

  @retval N/A

**/
VOID
EFIAPI
PrintStats (
  IN    PERF_DATA             *PerfDataPtr
  )
{

#ifdef DEBUG_CODE_BLOCK

  if (GetDebugLevel () & SDBG_MINMAX) {

    //
    // Number of PCI accesses
    //
    RcDebugPrint (SDBG_DEFAULT, "|  %9d", (*PerfDataPtr).PciCount);

    //
    // Number of PCI access while polling
    //
    RcDebugPrint (SDBG_DEFAULT, "| %9d ", (*PerfDataPtr).PollingCount);

    //
    // Number of JEDEC inits
    //
    RcDebugPrint (SDBG_DEFAULT, "| %5d", (*PerfDataPtr).JedecCount);

    //
    // Total time in the FixedDelay function
    //
    if ((*PerfDataPtr).FixedDelay > 5000) {
      RcDebugPrint (SDBG_DEFAULT, "|  %6dms  ", (*PerfDataPtr).FixedDelay / 1000);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "|  %6dus  ", (*PerfDataPtr).FixedDelay);
    }

    //
    // Number of Vref movements
    //
    RcDebugPrint (SDBG_DEFAULT, "| %6d ", (*PerfDataPtr).VrefCount);

    //
    // Number of CPGC tests
    //
    RcDebugPrint (SDBG_DEFAULT, "| %7d ", (*PerfDataPtr).CpgcCount);

    //
    // Number of SMBUS Reads
    //
    RcDebugPrint (SDBG_DEFAULT, "| %7d ", (*PerfDataPtr).SmbusRead);

    //
    // Number of SMBUS Writes
    //
    RcDebugPrint (SDBG_DEFAULT, "|%8d ", (*PerfDataPtr).SmbusWrite);

    //
    // Number of MRS Writes
    //
    RcDebugPrint (SDBG_DEFAULT, "| %8d ", (*PerfDataPtr).MrsWrite);

    //
    // Duration time
    //
    RcDebugPrint (SDBG_DEFAULT, "| %8dms ", (*PerfDataPtr).DurationTime);

    //
    // Number of GetSysHostPointer calls
    //
    RcDebugPrint (SDBG_DEFAULT, "| %10d ", (*PerfDataPtr).GetSysHostPointerCalls);

    //
    // End the table
    //
    RcDebugPrint (SDBG_DEFAULT, "|\n");
  }

#endif // #ifdef DEBUG_CODE_BLOCK

} // PrintStats

/**

  Check if the performance data has data

  @param[in] PerfDataPtr  - Pointer to current performance pointer data structure

  @retval TRUE  -> performance data present
          FALSE -> performance data is all clear

**/
BOOLEAN
PerfDataNonZero (
  IN    PERF_DATA             *PerfDataPtr
  )
{
  if(((*PerfDataPtr).TestCount       == 0) &&
     ((*PerfDataPtr).PciCount        == 0) &&
     ((*PerfDataPtr).JedecCount      == 0) &&
     ((*PerfDataPtr).FixedDelay      == 0) &&
     ((*PerfDataPtr).PollingCount    == 0) &&
     ((*PerfDataPtr).VrefCount       == 0) &&
     ((*PerfDataPtr).CpgcCount       == 0) &&
     ((*PerfDataPtr).SmbusRead       == 0) &&
     ((*PerfDataPtr).SmbusWrite      == 0) &&
     ((*PerfDataPtr).MrsWrite        == 0) &&
     ((*PerfDataPtr).DurationTime    == 0) &&
     ((*PerfDataPtr).GetSysHostPointerCalls == 0)
     ) {
    return FALSE;
  } else {
    return TRUE;
  }
} // PerfDataNonZero

/**

  Counts this PCI access

  @param[in] Counter      - The performance counter
  @param[in] Data         - Data to add to counter

  @retval N/A

**/
VOID
EFIAPI
CountTrackingData (
  IN    PERFORMANCE_COUNTER     Counter,
  IN    UINT32                  Data
  )
{
  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

    UINT16                      Index;
    UINT16                      LastIndex;
    UINT16                      IndexAdjust;
    UINT16                      TableSize;
    PERF_DATA                   *PerfDataPtr;
    MRC_STATUS                  Status;
    PERF_TRACKER_CONTEXT        *PtContext;

    GetPerfTrackerContext (&PtContext);

    if (PtContext->CurrentMemPhase == PTZ_PIPE_POLL) {
      return;
    }
    if (PtContext->CurrentMemPhase == PTZ_TRAIN_DDR) {
      return;
    }

    if (!PtContext->PerfTrackerEnabled) {
      //
      // Performance tracking has been disabled or suspended
      //
      return;
    }
    //
    // Prevent recursion
    //
    if (PtContext->PerfTrackerActive) {
      return;
    }
    PtContext->PerfTrackerActive = 1;

    //
    // Always count in the "Totals" structure
    //
    PerfDataPtr = &PtContext->Totals;
    CountData (PerfDataPtr, Counter, Data);

    //
    // Get a pointer to the performance tracker data for the current memory phase
    // For performance, keep track of which index in the list was referenced last.
    // It is likely that the same index will be referenced next time this routine is called
    //
    Status = MRC_STATUS_NOT_FOUND;
    TableSize = PtContext->ZoneCount;
    LastIndex = PtContext->LastIndex;
    for (Index = 0; Index < TableSize; Index++) {
      IndexAdjust = (Index + LastIndex) % TableSize;
      if (PtContext->CurrentMemPhase == PtContext->PerformanceData[IndexAdjust].MemPhase) {
        PerfDataPtr = &(PtContext->PerformanceData[IndexAdjust].PerfData);
        CountData (PerfDataPtr, Counter, Data);
        PtContext->LastIndex = IndexAdjust;
        Status = MRC_STATUS_SUCCESS;
        break;
      }
    }

    //
    // If no entry found above, attribute the counted data to a "nozone"
    //
    if (MRC_STATUS_SUCCESS != Status) {
      //
      // Go to the next "no zone" if no zone is defined
      //
      if (PtContext->NoZoneActive == 0) {
        PtContext->NoZoneActive = 1;
      }

      if (PtContext->NoZoneIndex < MAX_NOZONE) {
        PerfDataPtr = &PtContext->NoZone[PtContext->NoZoneIndex];
        CountData (PerfDataPtr, Counter, Data);
      }

    }

    PtContext->PerfTrackerActive = 0;
  }

  return;

} // CountTrackingData

/**

  Sets the current memory phase

  @param[in] Phase - Current memory phase

  @retval MRC_STATUS_SUCCESS if memory phase set
          MRC_STATUS_UNSUPPORTED otherwise

**/

MRC_STATUS
EFIAPI
SetMemPhase (
  IN      PT_ZONE   Phase
  )
{
  MRC_STATUS    Status = MRC_STATUS_UNSUPPORTED;

  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

    PERF_TRACKER_CONTEXT    *PtContext;

    GetPerfTrackerContext (&PtContext);

    PtContext->CurrentMemPhase = Phase;
    if (PtContext->NoZoneActive == 1) {
      PtContext->NoZoneActive = 0;
      if ((PtContext->NoZoneIndex + 1) < MAX_NOZONE) {
        PtContext->NoZoneIndex = PtContext->NoZoneIndex + 1;
      }
    }
    Status = MRC_STATUS_SUCCESS;
  }

  return Status;

} // SetMemPhase

/**

  Sets the current memory phase based on checkpoint code

  @param[in] MajorCheckPoint  - Major Check Point to set
  @param[in] MinorCheckPoint  - Minor Check Point to set

  @retval MRC_STATUS_SUCCESS if memory phase is set
          MRC_STATUS_NOT_FOUND if unable to set memory phase;
          MRC_STATUS_UNSUPPORTED if performance tracker is not supported

**/

MRC_STATUS
EFIAPI
SetMemPhaseCP (
  IN    UINT8     MajorCheckPoint,
  IN    UINT8     MinorCheckPoint
  )
{
  MRC_STATUS Status = MRC_STATUS_UNSUPPORTED;

  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

    UINT8                   Index;
    CONST CHECK_POINT_TO_PHASE MainPhaseTable[] = {
        { CHECKPOINT_MAJOR_INIT_STRUCT_LATE,    0,                                                PTZ_INIT_STRUCT_LATE       },
        { CHECKPOINT_MAJOR_SELECT_BOOT_MODE,    0,                                                PTZ_SELECT_BOOT_MODE       },
        { CHECKPOINT_MAJOR_DIMM_DETECT,         0,                                                PTZ_DIMM_DETECT            },
        { CHECKPOINT_MAJOR_CHECK_POR,           0,                                                PTZ_CHECK_POR              },
        { CHECKPOINT_MAJOR_UNLOCK_MEM_REGS,     0,                                                PTZ_UNLOCK_MEM_REGS        },
        { CHECKPOINT_MAJOR_CHECK_STATUS,        0,                                                PTZ_CHECK_STATUS           },
        { CHECKPOINT_MAJOR_CONFIG_XMP,          0,                                                PTZ_CONFIG_XMP             },
        { CHECKPOINT_MAJOR_CLOCK_INIT,          CHECKPOINT_MINOR_SET_CLK_VDD,                     PTZ_SET_CLK_VDD            },
        { CHECKPOINT_MAJOR_RANK_DETECT,         0,                                                PTZ_CHECK_DIMM_RANKS       },
        { CHECKPOINT_MAJOR_SPD_DATA,            0,                                                PTZ_GATHER_SPD             },
        { CHECKPOINT_MAJOR_DIMM_INFO,           CHECKPOINT_MINOR_DIMM_INFO_EARLY,                 PTZ_DIMM_INFO_DISPLAY      },
        { CHECKPOINT_MAJOR_NVDIMM,              CHECKPOINT_MINOR_NVDIMM_STS,                      PTZ_NVDIMM_STATUS          },
        { CHECKPOINT_MAJOR_INIT_THROTTLING,     CHECKPOINT_MINOR_EARLY_DDR_THERM,                 PTZ_EARLY_DDR_THERM        },
        { CHECKPOINT_MAJOR_EARLY_INIT_MEM,      0,                                                PTZ_EARLY_INIT_MEM         },
        { CHECKPOINT_MAJOR_GLOBAL_EARLY,        0,                                                PTZ_EARLY_CONFIG           },
        { CHECKPOINT_MAJOR_JEDEC_INIT,          0,                                                PTZ_JEDEC_INIT             },
        { CHECKPOINT_MAJOR_DDRIO_INIT,          0,                                                PTZ_DDRIO_INIT             },
        { CHECKPOINT_MAJOR_DDRIO_INIT,          CHECKPOINT_MINOR_EARLY_INIT,                      PTZ_DDRIO_INIT             },
        { CHECKPOINT_MAJOR_DDRIO_INIT,          CHECKPOINT_MINOR_LATE_INIT,                       PTZ_DDRIO_INIT             },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PRE_TRAINING_INIT,               PTZ_PRE_TRAINING_INIT      },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PACKAGE_DELAY_CCC,               PTZ_PACKAGE_DELAY          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_CMD_CLK,                   PTZ_CMD_CLK_EARLY          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_QX_CA_CLK,                       PTZ_QxCA_CLK               },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_CTL_CLK,                   PTZ_CTL_CLK_EARLY          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_LATE_CMD_CLK,                    PTZ_CMD_CLK                },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MAJOR_NO_CODE,                         PTZ_TRAIN_DDR              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_CROSSOVER_CALIBRATION,           PTZ_XOVER_CALIB            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RCOMP_STAT_LEG,                  PTZ_RCOMP_STAT_LEG         },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_SENSE_AMP,                       PTZ_SENSE_AMPLIFIER        },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC,            PTZ_REC_EN                 },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RX_DQ_DQS_BASIC,                 PTZ_RDDQDQS                },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DQ_SWIZZLE_DISCOVERY,            PTZ_SWIZZLE_DISCOVERY      },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DRAM_DUTY_CYCLE_ADJUSTER,        PTZ_DRAM_DUTY_CYCLE_ADJ    },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_READ_DQDQS_PRE_DFE_2D_CENTERING, PTZ_RDDQDQS_DFE            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_READ_DQDQS_POST_DFE_2D_CENTERING,PTZ_TRAIN_RX_POST_DFE      },
#ifdef DDRT_SUPPORT
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DDRT2_MODE,                      PTZ_SWITCH_DDRT2_MODE      },
#endif
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_WRITE_LEVELING_BASIC,            PTZ_WR_LVL                 },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TX_DQ_BASIC,                     PTZ_WRDQDQS                },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_WRITE_DQDQS_PRE_DFE_2D_CENTERING,PTZ_WRDQDQS_DFE            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_E_TRAIN_RD_VREF,                 PTZ_E_RD_VREF              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_E_TRAIN_WR_VREF,                 PTZ_E_WR_VREF              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_WR_FLY_BY,                       PTZ_WR_FLYBY               },
        { CHECKPOINT_MAJOR_CLOCK_INIT,          0,                                                PTZ_CLOCK_INIT             },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_RID_FINE,                  PTZ_E_RID                  },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TRAIN_RD_DQS,                    PTZ_RDDQDQS_ADV            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TRAIN_WR_DQS,                    PTZ_WRDQDQS_ADV            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RMT,                             PTZ_RANK_MARGIN_TOOL       },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_NORMAL_MODE,                     PTZ_NORMAL_MODE            },
        { CHECKPOINT_MAJOR_DDR_MEMMAP,          0,                                                PTZ_MEM_MAPPING            },
        { CHECKPOINT_MAJOR_RAS_CONFIG,          0,                                                PTZ_RAS_CONFIG             },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PER_BIT_DESKEW_RX,               PTZ_BIT_DESKEW_RX          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PER_BIT_DESKEW_TX,               PTZ_BIT_DESKEW_TX          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TRAIN_WR_VREF_LRDIMM,            PTZ_WR_VREF_LRDIMM         },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TRAIN_RD_VREF,                   PTZ_RD_VREF                },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TRAIN_WR_VREF,                   PTZ_WR_VREF                },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_CMD_TX_EQ,                     PTZ_TRAIN_CMD_TX_EQ        },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_CMD_VREF_CENTERING,              PTZ_CMD_VREF_CEN           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_ROUND_TRIP_OPTIMIZE,             PTZ_RT_OPT                 },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TX_EQ_TRAINING,                  PTZ_TRAIN_TX_EQ            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_TX_EQ,                         PTZ_TRAIN_TX_EQ            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_DRAM_RX_EQ,                    PTZ_TRAIN_DRAM_RX_EQ       },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_IMODE_TRAINING,                  PTZ_TRAIN_IMODE            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_IMODE,                         PTZ_TRAIN_IMODE            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_TX_ECHO,                       PTZ_TX_ECHO                },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_LEGACY_RXDFE,                    PTZ_LEGACY_RXDFE           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_CTLE,                          PTZ_TRAIN_CTLE             },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_MC_RON,                        PTZ_TRAIN_MC_RON           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_RTT_WR,                        PTZ_TRAIN_RTT_WR           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_MC_ODT,                        PTZ_TRAIN_RX_ODT           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_DRAM_RON,                      PTZ_TRAIN_DRAM_RON         },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_TX_RISE_FALL_SLEW_RATE,        PTZ_TRAIN_TX_RFSR          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_TCO_COMP,                      PTZ_TRAIN_TCO_COMP         },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DCA_TCO,                         PTZ_TRAIN_DCA_TCO          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TCO_DQ,                          PTZ_TRAIN_TCO_DQDQS        },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_WRITE_ODT_LATENCY,               PTZ_TRAIN_TX_ODT           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PXC_TRAINING,                    PTZ_TRAIN_PXC              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DDJC_TRAINING,                   PTZ_TRAIN_DDJC             },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_RX_DFE,                        PTZ_TRAIN_RX_DFE           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_READ_DFE,                        PTZ_TRAIN_READ_DFE         },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_WRITE_DFE,                       PTZ_TRAIN_WRITE_DFE        },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_WRITE_DQDQS_POST_DFE_2D_CENTERING, PTZ_TRAIN_TX_POST_DFE    },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PERIODIC_TX_RETRAIN,             PTZ_TRAIN_TX_RETRAIN       },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING,      PTZ_LRDIMM_RX              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RX_BACKSIDE_CYCLE_TRAINING,      PTZ_LRDIMM_RX              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RX_BACKSIDE_DELAY_TRAINING,      PTZ_LRDIMM_RX              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TX_BACKSIDE_FINE_WL_TRAINING,    PTZ_LRDIMM_TX              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TX_BACKSIDE_COARSE_WL_TRAINING,  PTZ_LRDIMM_TX              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TX_BACKSIDE_DELAY_TRAINING,      PTZ_LRDIMM_TX              },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DISPLAY_RESULTS,                 PTZ_DISPLAY_RESULTS        },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_POST_TRAINING_INIT,              PTZ_POST_TRAINING_INIT     },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_END_TRAINING,                    PTZ_LATE_CONFIG            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_T_LRDIMM_DB_DFE,                 PTZ_LDRIMM_DB_DFE          },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_DELAY,     PTZ_LRDIMM_TX              },
        { CHECKPOINT_MAJOR_INIT_THROTTLING,     CHECKPOINT_MINOR_MID_DDR_THERM,                   PTZ_MID_DDR_THERM          },
        { CHECKPOINT_MAJOR_INIT_THROTTLING,     CHECKPOINT_MINOR_LATE_DDR_THERM,                  PTZ_LATE_DDR_THERM         },
        { CHECKPOINT_MAJOR_MEMBIST,             0,                                                PTZ_MEM_BIST               },
        { CHECKPOINT_MAJOR_SVL_SCRAMBLE,        0,                                                PTZ_SVL_SCRAMBLE           },
        { CHECKPOINT_MAJOR_MEMINIT,             0,                                                PTZ_MEM_INIT               },
        { CHECKPOINT_MAJOR_CMI_CREDIT,          0,                                                PTZ_CMI_CREDIT             },
        { CHECKPOINT_MAJOR_CHECK_RAS,           0,                                                PTZ_CHECK_RAS              },
        { CHECKPOINT_MAJOR_MEM_LATE,            0,                                                PTZ_MEM_LATE               },
        { CHECKPOINT_MAJOR_SSA_API_INIT,        CHECKPOINT_MINOR_SSA_API_INIT_START,              PTZ_SSA_API_INIT_ST        },
        { CHECKPOINT_MAJOR_MEM_HEALTH_TEST,     0,                                                PTZ_MEM_HEALTH_TEST        },
        { CHECKPOINT_MAJOR_NVDIMM,              CHECKPOINT_MINOR_NVDIMM_RESTORE,                  PTZ_NVDIMM_RESTORE         },
        { CHECKPOINT_MAJOR_NVDIMM,              CHECKPOINT_MINOR_NVDIMM_ARM,                      PTZ_NVDIMM_ARM             },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TRAIN_RD_VREF_LRDIMM,            PTZ_RD_VREF_LRDIMM         },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TX_LRDIMM_DQ_CENTERING,          PTZ_TX_DQ_CENTERING_LRDIMM },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RX_LRDIMM_DQ_CENTERING,          PTZ_RX_DQ_CENTERING_LRDIMM },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_LEVEL_COARSE, PTZ_BS_WR_LVL_LRDIMM    },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PPR_FLOW,                        PTZ_POST_PKG_RPR           },
        { CHECKPOINT_MAJOR_DDRIO_INIT,          CHECKPOINT_MINOR_RCSIM_CACHE_DDRIO,               PTZ_DDRIO_RCSIM_CACHE      },
        { CHECKPOINT_MAJOR_DIMM_INFO,           CHECKPOINT_MINOR_DIMM_INFO_LATE,                  PTZ_DIMM_INFO_DISPLAY      },
        { CHECKPOINT_MAJOR_INIT_ADR,            0,                                                PTZ_INIT_ADR               },
        { CHECKPOINT_MAJOR_HBM,                 CHECKPOINT_MINOR_HBM_PRE_TRAINING,                PTZ_HBM_PRE_TRAINING       },
        { CHECKPOINT_MAJOR_HBM,                 CHECKPOINT_MINOR_HBM_TRAINING,                    PTZ_HBM_TRAINING           },
        { CHECKPOINT_MAJOR_HBM,                 CHECKPOINT_MINOR_HBM_POST_TRAINING,               PTZ_HBM_POST_TRAINING      },
        { CHECKPOINT_MAJOR_HBM,                 CHECKPOINT_MINOR_HBM_NORMAL_MODE,                 PTZ_HBM_NORMAL_MODE        },
#ifdef LRDIMM_SUPPORT
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_BCOM_CLK,                  PTZ_LRDIMM_BCOM            },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_PBA_ENUMERATE_ID,                PTZ_LRDIMM_PBA_ENUMERATE_ID},
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_REQ_CLK,                         PTZ_EARLY_REQ_CLK          },
#endif
#ifdef DDR5_SUPPORT
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_CS_CLK,                    PTZ_CS_CLK_EARLY           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_CA_CLK,                    PTZ_CA_CLK_EARLY           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_CA_SLEW_RATE,                    PTZ_CA_SLEW_RATE           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_RCD_DCA_DCK_DUTY_CYCLE_TRAINING, PTZ_DCA_DCLK_DUTY_CYCLE    },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DCA_DFE,                         PTZ_DCA_DFE                },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_CS_CLK_BACKSIDE,           PTZ_EARLY_CS_CLK_BACKSIDE  },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_EARLY_CA_CLK_BACKSIDE,           PTZ_EARLY_CA_CLK_BACKSIDE  },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_WRITE_LEVELING_BASIC_DDR5,       PTZ_WR_LVL                 },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_DQ_SLEW_RATE,                    PTZ_DQ_SLEW_RATE           },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_TURNAROUND,                      PTZ_TURNAROUND             },
        { CHECKPOINT_MAJOR_CHANNEL_TRAINING,    CHECKPOINT_MINOR_NORMALIZE_CMD,                   PTZ_NORMALIZE_CMD          },
#endif // DDR5_SUPPORT
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_AP_BOOT_MODE,               PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_SBSP_BOOT_MODE,             PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_AP_DATA,                    PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_SBSP_STATUS,                PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_SBSP_DATA,                  PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_AP_SSA_DATA,                PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_SBSP_SSA_DATA,              PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_THERMAL_SYNC,               PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_AP_NVRAM_DATA,              PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_SBSP_VAR_DATA,              PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_SBSP_MEM_MODE,              PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC,        PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_PIPE_SYNC,           CHECKPOINT_MINOR_PIPE_CPL2,                       PTZ_PIPE_SYNC              },
        { CHECKPOINT_MAJOR_CPL2,                0,                                                PTZ_SET_CPL2               },
      };

    #define MAIN_PHASE_TABLE_SIZE sizeof(MainPhaseTable)/sizeof(MainPhaseTable[0])

    //
    // Try the main checkpoint list
    //
    for (Index = 0; Index < MAIN_PHASE_TABLE_SIZE; Index++) {
      if ((MainPhaseTable[Index].MajorCheckPoint == MajorCheckPoint) && (MainPhaseTable[Index].MinorCheckPoint == MinorCheckPoint)) {
        Status = SetMemPhase (MainPhaseTable[Index].Phase);
        return Status;
      }
    }
  }

  return Status;

} // SetMemPhaseCP

/**

  Clears the current memory phase

  @param[in] Phase - Current memory phase to clear

  @retval MRC_STATUS_SUCCESS if memory phase is cleared
          MRC_STATUS_UNSUPPORTED if performance tracker is not supported

**/

MRC_STATUS
EFIAPI
ClearMemPhase (
  IN      PT_ZONE   Phase
  )
{
  MRC_STATUS    Status = MRC_STATUS_UNSUPPORTED;

  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

    PERF_TRACKER_CONTEXT    *PtContext;

    GetPerfTrackerContext (&PtContext);

    PtContext->CurrentMemPhase = PTZ_NOZONE;
    Status = MRC_STATUS_SUCCESS;
  }

  return Status;

} // ClearMemPhase

/**

  Gets the current memory phase

  @param[out] MemPhase        - The current memory phase

  @retval MRC_STATUS_SUCCESS if memory phase is known
          MRC_NOT_FOUND if unable to determine memory phase

**/

MRC_STATUS
EFIAPI
GetMemPhase (
  OUT PT_ZONE *Phase
  )
{
  MRC_STATUS Status = MRC_STATUS_NOT_FOUND;

  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

    PERF_TRACKER_CONTEXT    *PtContext;

    GetPerfTrackerContext (&PtContext);

    if ((Phase != NULL) && (PtContext != NULL)) {
      *Phase = PtContext->CurrentMemPhase;
      Status = MRC_STATUS_SUCCESS;
    }
  }

  return Status;

} // GetMemPhase

/**

  Copy performance data to HOB structure.

  @param[in] PerfDataPtr - Pointer to current performance pointer data structure
  @param[in] HobDataPtr  - Pointer to Hob structure

**/
VOID
CopyPerfData (
  IN PERF_DATA  *PerfDataPtr,
  IN PERF_DATA  *HobDataPtr
  )
{
  (*HobDataPtr).TestCount       = (*PerfDataPtr).TestCount;
  (*HobDataPtr).PciCount        = (*PerfDataPtr).PciCount;
  (*HobDataPtr).JedecCount      = (*PerfDataPtr).JedecCount;
  (*HobDataPtr).FixedDelay      = (*PerfDataPtr).FixedDelay;
  (*HobDataPtr).PollingCount    = (*PerfDataPtr).PollingCount;
  (*HobDataPtr).VrefCount       = (*PerfDataPtr).VrefCount;
  (*HobDataPtr).CpgcCount       = (*PerfDataPtr).CpgcCount;
  (*HobDataPtr).SmbusRead       = (*PerfDataPtr).SmbusRead;
  (*HobDataPtr).SmbusWrite      = (*PerfDataPtr).SmbusWrite;
  (*HobDataPtr).MrsWrite        = (*PerfDataPtr).MrsWrite;
  (*HobDataPtr).DurationTime    = (*PerfDataPtr).DurationTime;
  (*HobDataPtr).GetSysHostPointerCalls = (*PerfDataPtr).GetSysHostPointerCalls;

  //
  // Clear all tracking data
  //
  (*PerfDataPtr).TestCount        = 0;
  (*PerfDataPtr).PciCount         = 0;
  (*PerfDataPtr).JedecCount       = 0;
  (*PerfDataPtr).FixedDelay       = 0;
  (*PerfDataPtr).PollingCount     = 0;
  (*PerfDataPtr).VrefCount        = 0;
  (*PerfDataPtr).CpgcCount        = 0;
  (*PerfDataPtr).SmbusRead        = 0;
  (*PerfDataPtr).SmbusWrite       = 0;
  (*PerfDataPtr).MrsWrite         = 0;
  (*PerfDataPtr).DurationTime     = 0;
  (*PerfDataPtr).GetSysHostPointerCalls     = 0;

} // CopyPerfData

/**

  Build Hob to be consumed in DXE to print performance statistics.

  @param None

  @retval MRC_STATUS_SUCCESS   - successfully built Hob containing performance tracker statistics
  @retval MRC_STATUS_NOT_FOUND - unable to locate performance tracker statistics

**/

MRC_STATUS
EFIAPI
BuildPerfStatsHob (
  VOID
  )
{
  VOID                   *PerfHobPtr;
  PERF_TRACKER_CONTEXT   *PtContext;
  PERF_TRACKER_HOB_DATA  PerfHobData;
  PERF_TRACKER_HOB_DATA  *PerfHobDataPtr;
  UINT16                 Index;
  UINT16                 TableSize;

  GetPerfTrackerContext (&PtContext);

  PerfHobDataPtr = &PerfHobData;

  // Initialize Hob
  ZeroMem (&PerfHobData, sizeof (PERF_TRACKER_HOB_DATA));

  // Copy performance totals to Hob
  PerfHobData.ZoneCount = PtContext->ZoneCount;
  CopyPerfData (&PtContext->Totals, &PerfHobData.Totals);

  // Copy zone info to Hob
  TableSize = PtContext->ZoneCount;
  for (Index = 0; Index < TableSize; Index++) {
    PerfHobData.PerformanceData[Index].MemPhase = PtContext->PerformanceData[Index].MemPhase;
    CopyPerfData (&(PtContext->PerformanceData[Index].PerfData), &(PerfHobData.PerformanceData[Index].PerfData));
  }

  // Copy nozone info to Hob
  for (Index = 0; (Index <= PtContext->NoZoneIndex) && (Index < MAX_NOZONE); Index++) {
    CopyPerfData (&(PtContext->NoZone[Index]), &(PerfHobData.NoZone[Index]));
  }

  // Build Hob
  PerfHobPtr = BuildGuidDataHob (&gPerfStatsGuid, PerfHobDataPtr, sizeof (PERF_TRACKER_HOB_DATA));

  ASSERT (PerfHobPtr);

  return MRC_STATUS_SUCCESS;

} // BuildPerfStatsHob

/**

  Print statistics

  @param[in] Socket         - Socket
  @param[in] FmcMaxCached   - FMC Max Cached
  @param[in] FmcCachedReads - FMC Cached Reads

  @retval N/A

**/

VOID
EFIAPI
PrintPerformanceStats (
  IN UINT8 Socket,
  IN UINT8 FmcMaxCached,
  IN UINT32 FmcCachedReads
  )
{

  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

#ifdef DEBUG_CODE_BLOCK

    UINT8                       NoZoneIndex;
    UINT16                      Index;
    UINT16                      TableSize;
    PERF_TRACKER_CONTEXT        *PtContext;

    GetPerfTrackerContext (&PtContext);

    PtContext->PerfTrackerEnabled = FALSE;

    if (GetDebugLevel () & SDBG_MINMAX) {
      AcquirePrintControl ();

      RcDebugPrint (SDBG_DEFAULT, "Performance statistics for socket %d\n", Socket);
      RcDebugPrint (SDBG_DEFAULT, "FmcMaxCached   = %d\n", FmcMaxCached);
      RcDebugPrint (SDBG_DEFAULT, "FmcCachedReads = %d\n", FmcCachedReads);
      RcDebugPrint (SDBG_DEFAULT, "MRC Phase          |    PCI    |  Polling  |JEDEC | FixedDelay |  Vref  |  CPGC   |  SMBUS  |  SMBUS  |    MRS   |  Duration  | GetSysHostPointer\n");
      RcDebugPrint (SDBG_DEFAULT, "                   |   Count   |   Count   |      |    Time    |  Count |  Count  |  Read   |  Write  |   Write  |    Time    |   Calls          \n");
      RcDebugPrintLine (SDBG_MINMAX, 140, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

      RcDebugPrint (SDBG_DEFAULT, "Total Stats        ");
      PrintStats (&PtContext->Totals);

      TableSize = PtContext->ZoneCount;
      for (Index = 0; Index < TableSize; Index++) {

        if (PtContext->PerformanceData[Index].String != NULL) {

          if (PerfDataNonZero (&(PtContext->PerformanceData[Index].PerfData))) {

            RcDebugPrint (SDBG_DEFAULT, "%-19a", PtContext->PerformanceData[Index].String);
            PrintStats (&(PtContext->PerformanceData[Index].PerfData));
          }
        }
      }

      for (NoZoneIndex = 0; (NoZoneIndex <= PtContext->NoZoneIndex) && (NoZoneIndex < MAX_NOZONE); NoZoneIndex++) {

        if (PerfDataNonZero (&PtContext->NoZone[NoZoneIndex])) {

          RcDebugPrint (SDBG_DEFAULT, "No Zone %2d         ", NoZoneIndex);
          PrintStats (&PtContext->NoZone[NoZoneIndex]);
        }
      }

      RcDebugPrint (SDBG_DEFAULT, "\n");

      ReleasePrintControl ();
    }

    // Flush the debug buffer
    FlushDebugBuffer ();

#endif  // #ifdef DEBUG_CODE_BLOCK

    BuildPerfStatsHob ();

  }

  return;

} // PrintPerformanceStats
