/** @file
  Silicon workaround library.
  Each SOC is required to provide an implementation of this
  library to handle silicon workarounds that are applicable to the
  said SOC.
  Different tables are created to enable specific workarounds that
  are depending on:
  Silicon environment (Silicon, Simics, CTE, emulation and etc.)
  Silicon stepping
  The array (a.k.a. table) must be ended with NULL to indicate the
  end of the list.
  For workaround that contains Sighting number, it is added to the
  table using the sighting number preceded with "S".

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

#include <Library/SiliconWorkaroundLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/CpuAndRevisionLib.h>

//
// This table lists workarounds that applied to BIOS running in Simics
//
CONST CHAR8 *SimicsWorkaroundTable[] = {
  "22010041180",
  "S2207989002", // Don't enable posted CSR writes under Simics
  "S1507757586",
  "S1507820821",
  "22010473374", // Allow in-band MRW under Simics
  NULL
};

//
// This table lists workarounds that applied to Hybrid SLE BIOS
//
CONST CHAR8 *HybridSleWorkaroundTable[] = {
  "Warm Reset",
  NULL
};

//
// This table lists workarounds that applied to Ubios SLE enabled BIOS
//
CONST CHAR8 *UbiosSleWorkaroundTable[] = {
  "S1406647183",
  "S1208984927",
  "S1706724385",
  NULL
};

//
// This table lists workarounds that applied to A0 stepping of SPR,
// Note: A1 is a silent stepping which means no stepping ID change, A0 and A1 are same to bios.
//
CONST CHAR8 *SprA0WorkaroundTable[] = {
  "S1409873794",
  "S2208918713",
  "S14010043192", // Implement WA for High phase followed by low phase write for DDR5 corrupts the scrambling logic
  "S1808609346",
  "S14010181268",
  "S14010043240", // ITC can reuse PrhPend pointers under prhpend array buffer full conditions
  "S14010073297", // Far channel ID calculation for in tile 2LM is incorrect in m2mem
  "S14010985526", // MC Assertion error for a valid address
  "S14010293656",
  "S14010086617",
  "S14010086544",
  "S2209596922",
  "S1409883828",
  "S1409873802",
  "S2208908676",
  "S14010101993",
  "S1409866138",
  "S1409866097",
  "S14010044313",
  "S14010455369", // 2LM with KTI prefetch in 2S is broken
  "S1409873816",
  "S2209684728",
  "S14010451750",
  "S2209870115",
  "S14010402958",
  "S2209312485", // override read/write policy register for I3C registers access due to a SAI STRAP issue
  "S22010953912",
  "S1409930097",
  "S2208908874",
  "S14010042217",
  "S14010299479",
  "S14010544275",
  "S14010689545",
  "S14010197754",
  "S16010697617",
  "S1409873823",
  "S22010113831",
  "S14010065547",
  "S1409517281",
  "S14010448801", // Disable early read completion for systems with DDRT2 DIMM
  "S14010197660",
  "S14010741732",
  "S1707090346",
  "S14010044298", // CPGC writes leave corrupted state around in mc_wdb when EnableCpgcInOrder=1
  "S14010687317",
  "S1707159028",
  "S14010738859",
  "S14010123163",
  "S14010996350",
  "S14011166558",
  "S14011009740",
  "S14011415986",
  "S14011367913",
  "S14011080476",
  "S14010882657", // VID reverse for retimer low latency mode
  "S14010021323",
  "S14011088832", // CHA[59].bl_vna_credit_config_cfg.bl_vna_port10 to 0
  "S14010263993",
  "S14011008707",
  "S14011103009", // Default value of L1/L2 Entry Hysteresis should be increased to 1024 clocks for A0 and B0
  "S14010996611",
  "S14011218130",
  "S14011241978",
  "S1507902816", // WA CMI recipe for memtest when refresh_enabled gets set
  "S14011240682",// Disable clock modulation to walk around CHA bug.
  "S14011307716",
  "S22010499709",// Set the Defeature0.D2K4CisgrDis=1 for SPR A0 and B0, as the bit controls the d2k return for cisgress requests on 2LM mode
  "S22010581486", // WA to bypass alternate-protocol-negotiation
  "S1507951535",
  "S14011408944",
  "S14011403994", // When WrCRC is enabled, MRC shall add one cycle bubble in back to back writes for turnaround timing registers
  "S14011213678",
  "S22010823763",
  "S14011768962", // WA to skip apnsts
  "S14011903038",
  "S22010732966",
  "S1707233823",
  "S22011038746",
  "S22010555690",
  "S22010462897", // WA disable DRAM 2x refresh
  "S22010986909",
  "S14012066447", // BIST may fail on A-step due to silicon bug causing MLC BIST failures.
  NULL
};

//
// This table lists workarounds that applied to B0 stepping of SPR
//
CONST CHAR8 *SprB0WorkaroundTable[] = {
  "S14010892610",
  "S2208908676",
  "S14010101993",
  "S14010451750",
  "S2208908874",
  "S16010697617",
  "S1409517281",
  "S1707159028",
  "S16010813320",
  "S14010950010",
  "S14011009740",
  "S14011367913",
  "S14011080476",
  "S14011166558",
  "S14011154764",
  "S14010882657", // VID reverse for retimer low latency mode
  "S14011088832", // CHA[59].bl_vna_credit_config_cfg.bl_vna_port10 to 0
  "S14011103009", // Default value of L1/L2 Entry Hysteresis should be increased to 1024 clocks for A0 and B0
  "S14011507287", // X-Tile 2way 2LM hung transactions due to entry not getting de-allocated in MC scoreboard
  "S14010985526", // MC Assertion error for a valid address
  "S14010996611",
  "S22010438890", // BIOS shall override OTCMAXTOTCRDTS_C1_1_0_0_CFG in every enabled M2IOSF before triggering hw_init.
  "S14011218130",
  "S1507902816", // WA CMI recipe for memtest when refresh_enabled gets set
  "S22010274542",
  "S14011156829",
  "S14011367585",
  "S22010499709",// Set the Defeature0.D2K4CisgrDis=1 for SPR A0 and B0, as the bit controls the d2k return for cisgress requests on 2LM mode
  "S22010581486", // WA to bypass alternate-protocol-negotiation
  "S14011347981",// Metadata correction doesn't use BIST result
  "S14011198324",
  "S14011408944",
  "S14011403994", // When WrCRC is enabled, MRC shall add one cycle bubble in back to back writes for turnaround timing registers
  "S14011213678",
  "S22010823763",
  "S14010043192", // Implement WA for High phase followed by low phase write for DDR5 corrupts the scrambling logic
  "S14011768962", // WA to skip apnsts
  "S14011156662", // WA to not enable PFD in TME active mode by setting mc_rrd_crnode_registers.ecc_corr_bist.bist_en to 0
  "S14011903038",
  "S22010732966",
  "S1707233823",
  "S22011038746",
  "S22010555690",
  "S22010462897", // WA disable DRAM 2x refresh
  "S22010986909",
  "S14011971908",
  NULL
};

//
// This table lists workarounds that applied to C0 stepping of SPR
//
CONST CHAR8 *SprC0WorkaroundTable[] = {
  "S2208908874",
  "S14011403994", // When WrCRC is enabled, MRC shall add one cycle bubble in back to back writes for turnaround timing registers
  "S14011736925",
  "S14011903038",
  "S1707233823",
  "S14011776702", // MDFIS Die2Die sequence programming, this si_wa should be applied to C0 and later stepping
  NULL
};

//
// This table lists workarounds that applied to D0 stepping of SPR
//
CONST CHAR8 *SprD0WorkaroundTable[] = {
  "S14011776702", // MDFIS Die2Die sequence programming, this si_wa should be applied to C0 and later stepping
  NULL
};


//
// This table lists workarounds that applied to R0 stepping of SPR MCC
//
CONST CHAR8 *SprR0WorkaroundTable[] = {
  "S2208908874",
  "S14011403994", // When WrCRC is enabled, MRC shall add one cycle bubble in back to back writes for turnaround timing registers
  "S14011776702", // MDFIS Die2Die sequence programming, this si_wa should be applied to C0 and later stepping
  NULL
};

//
// This table lists workarounds that applied to U0 stepping of SPR UCC
//
CONST CHAR8 *SprU0WorkaroundTable[] = {
  "S2208908874",
  "S14011403994", // When WrCRC is enabled, MRC shall add one cycle bubble in back to back writes for turnaround timing registers
  "S14011776702", // MDFIS Die2Die sequence programming, this si_wa should be applied to C0 and later stepping
  NULL
};

//
// This table lists workarounds that applied to all steppings of SPR
//
CONST CHAR8 *AllSteppingsWorkaroundTable[] = {
  "S2202697614",
  "S2206866694",
  "S1506869315",
  "S1408726938", // BSE/MSE not writeable in command register of OOB-MSM device on PCI
  "S1507340892",
  "S14011736159",// EarlyCmp feature needs to be disabled for all SPR steppings
  "S2209269055", // ADR - Control bit for NM flush actions in MC missing
  "S1409873101", // Enable metadata ECC correction by default
  "S1409773441",
  "S1507442372",
  "S1707089812",
  "S1707072739", //  Snoop Response Timer setting should be 0xa
  "S2207523533", // Peer to Peer transactions hang, same as S2206780810, no fix for SPR-* steppings
  "S14011491308",
  "S14011724136",
  NULL
};

/**
  Return whether the workaround is enabled.

  This function returns a boolean that determines whether the workaround is enabled
  given a workaround name.

  @param[in]      WorkaroundName         An ASCII string that represents the workaround name.
                                         This workaround name should correspond to an entry
                                         in the silicon workarounds table(s).

  @retval TRUE    The workaround is enabled.
  @retval FALSE   The workaround is not found in the table(s) and therefore disabled.

**/
BOOLEAN
EFIAPI
IsSiliconWorkaroundEnabled (
  IN CONST CHAR8    *WorkaroundName
  )
{
  BOOLEAN    WorkaroundEnabled;

  WorkaroundEnabled = FALSE;

  WorkaroundEnabled = FindWorkaround (WorkaroundName, SimicsWorkaroundTable);
  if (WorkaroundEnabled) {
    if (IsSimicsEnvironment ()) {
      return WorkaroundEnabled;
    } else {
      return FALSE;
    }
  }

  WorkaroundEnabled = FindWorkaround (WorkaroundName, HybridSleWorkaroundTable);
  if (WorkaroundEnabled) {
    if (HybridSystemLevelEmulationEnabled ()) {
      return WorkaroundEnabled;
    } else {
      return FALSE;
    }
  }

  WorkaroundEnabled = FindWorkaround (WorkaroundName, UbiosSleWorkaroundTable);
  if (WorkaroundEnabled) {
    if (UbiosGenerationEnabled ()) {
      return WorkaroundEnabled;
    } else {
      return FALSE;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
    WorkaroundEnabled = FindWorkaround (WorkaroundName, SprA0WorkaroundTable);
    if (WorkaroundEnabled) {
      return WorkaroundEnabled;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    WorkaroundEnabled = FindWorkaround (WorkaroundName, SprB0WorkaroundTable);
    if (WorkaroundEnabled) {
      return WorkaroundEnabled;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_C0)) {
    WorkaroundEnabled = FindWorkaround (WorkaroundName, SprC0WorkaroundTable);
    if (WorkaroundEnabled) {
      return WorkaroundEnabled;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_D0)) {
    WorkaroundEnabled = FindWorkaround (WorkaroundName, SprD0WorkaroundTable);
    if (WorkaroundEnabled) {
      return WorkaroundEnabled;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_R0)) {
    WorkaroundEnabled = FindWorkaround (WorkaroundName, SprR0WorkaroundTable);
    if (WorkaroundEnabled) {
      return WorkaroundEnabled;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_U0)) {
    WorkaroundEnabled = FindWorkaround (WorkaroundName, SprU0WorkaroundTable);
    if (WorkaroundEnabled) {
      return WorkaroundEnabled;
    }
  }

  WorkaroundEnabled = FindWorkaround (WorkaroundName, AllSteppingsWorkaroundTable);

  return WorkaroundEnabled;
}
