/** @file
  Oem Hooks file for UncoreInit Module

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _ODT_TABLE_LIB_H_
#define _ODT_TABLE_LIB_H_

//
// This factor is how much Vref percentages are multiplied by on return. To get
// the correct percentage, the caller should divide the value returned by this.
//
// Functions in the library which use this factor are:
//  * GetDramVrefValueOdt
//  * GetMcVrefValueOdt
//  * GetDcaVrefValueOdt
//  * GetDcsVrefValueOdt
//
#define ODT_TABLE_LIB_VREF_PERCENT_FACTOR           10

//
// Convert to units of 1% and round to the nearest whole percent for Vref
// percentages (the values that use ODT_TABLE_LIB_VREF_PERCENT_FACTOR).
//
#define ODT_TABLE_LIB_ROUND_VREF_PERCENT(v)         \
  (((v) + (ODT_TABLE_LIB_VREF_PERCENT_FACTOR / 2))  \
  / ODT_TABLE_LIB_VREF_PERCENT_FACTOR)

//
// Defines the device position that will be considered as STRAP device
//
#define STRAP_POSITION      5

/*++

  Returns the  MC ODT values in Ohms (0-255)

  @param [IN    ] Socket - Socket number
  @param [IN    ] Ch     - Channel number (0-based)
  @param [   OUT] Mcodt  - MC ODT values in Ohms (0-255)

  @retval The MCODT value between 0 and 255 ohms

--*/
UINT8
GetMcOdtValue (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
     OUT UINT16 *Mcodt
  );

/*++

  Looks for the MC-RON value for Socket in the ODT table.
  If found, it returns a value in Ohms (0-255).

  @param [IN    ] Socket - Socket number
  @param [IN    ] Ch     - Channel number (0-based)
  @param [   OUT] McRon  - MC-RON value in Ohms (0-255)

  @retval EFI_UNSUPPORTED if called from a BRS build
  @retval EFI_NOT_FOUND if McRon for the current config is not found
  @retval EFI_SUCCESS if McRon for the current config is found

--*/
EFI_STATUS
EFIAPI
GetMcRonValue (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
     OUT UINT16 *McRon
  );

/**

  This function copies the DRAM RTT & Activation values, to RankNvList (i.e. to the Host structure)
  These values are later programmed to the DRAMs by the "JEDEC init" functions.
  This function also copies backside Vref and ODT values for LR-DIMMs to ChannelNvList (again, to Host)

  Emphasis: the above means that a subset of ODT values can then be used by **any piece of code** that fetches a pointer
  to RankNvList or ChannelNvList (indeed, there are pieces of code that do ODT programming without calling a single OdtTableLib API)

  Initial values for many other ODT parameters (like McOdt) are programmed by the MMRC (BaseMspChipLib), not by this function.

  The set of ODT Impedance and Activation values for multiple system configurations
  (that is, for multiple DIMM populations and frequencies) are sometimes collectively called "the ODT Matrix"

  @param [IN] Socket - Socket number
  @param [IN] Ch     - Channel number (0-based)

  @retval N/A

**/
VOID
BufferDramAndLrdimmBacksideOdtToHost (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/*++

  Returns the  DRAM Vref Value

  @param [IN    ]Socket    - Socket number
  @param [IN    ]Ch        - Channel number (0-based)
  @param [IN    ]Dimm      - Dimm number (0-based)
  @param [   OUT]VrefValue - Pointer to store the Vref value

  @retval True or False, depending if the Vref value is found in the ODT Value table
--*/
BOOLEAN
GetDramVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT16  *VrefValue
  );

/*++

  Returns the Mc Vref value

  @param [IN    ] Socket - Socket number
  @param [IN    ] Ch     - Channel number (0-based)
  @param [   OUT] McVref - Pointer to store the Mc Vref value

  @retval True or False, depending if the pointer in the ODT Value table is found
--*/
BOOLEAN
GetMcVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  OUT   UINT16  *McVref
  );

/**
  Determine if this is a Termination Rank

  @param [IN] Socket - Socket number
  @param [IN] Ch     - Channel number
  @param [IN] Dimm   - DIMM number
  @param [IN] Rank   - Rank number

  @retval 0: This is not a Termination Rank
  @retval 1: This is a Termination Rank
**/
UINT32
IsTermRank (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Rank
  );

/*++

  Returns the Read and Write activation DRAM Vref values

  @param [IN    ] Socket   - Socket number
  @param [IN    ] Ch       - Channel number (0-based)
  @param [IN    ] Dimm     - Dimm number (0-based)
  @param [IN    ] Rank     - Rank number (0-based)
  @param [   OUT] ReadAct  - Pointer to store the read activation value
  @param [   OUT] WriteAct - Pointer to store the write activation value

  @retval True or False, depending if the pointer in the ODT Value table is found
--*/
BOOLEAN
GetReadWriteActivationValuesOdt (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  Dimm,
  IN     UINT8  Rank,
     OUT UINT16 *ReadAct,
     OUT UINT16 *WriteAct
  );

/*++

  Gets the RCD QCK R-On ODT Value

  @param [IN    ] Socket            - Socket number
  @param [IN    ] Ch                - Channel number (0-based)
  @param [IN    ] Dimm              - Dimm number (0-based)
  @param [   OUT] RcdQckRonOdtValue - Pointer to store the encoded DRAM QCK ODT value

  @retval True or False, depending if the RCD QCK R-On ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetRcdQckRonValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT8   *RcdQckRonOdtValue
  );

/*++

  Gets the RCD QCA R-On ODT Value

  @param [IN    ] Socket            - Socket number
  @param [IN    ] Ch                - Channel number (0-based)
  @param [IN    ] Dimm              - Dimm number (0-based)
  @param [   OUT] RcdQcaRonOdtValue - Pointer to store the encoded DRAM QCA ODT value

  @retval True or False, depending if the RCD QCA R-On ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetRcdQcaRonValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT8   *RcdQcaRonOdtValue
  );

/*++

  Gets the RCD QCS R-On ODT Value

  @param [IN    ] Socket            - Socket number
  @param [IN    ] Ch                - Channel number (0-based)
  @param [IN    ] Dimm              - Dimm number (0-based)
  @param [   OUT] RcdQcsRonOdtValue - Pointer to store the encoded DRAM QCS ODT value

  @retval True or False, depending if the RCD QSA R-On ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetRcdQcsRonValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT8   *RcdQcsRonOdtValue
  );

/**
  Converts a strobe number to a relative DRAM position value
  The strobe number corresponds to the DQS# signal from the CPU
  The relative position number relates to the position of the DRAM
  relative to the RCD, such that 1 is closest to the RCD and 5 is
  furthest (STRAP).

  @param [IN    ] Strobe          - Strobe (0-based)
**/
UINT8
GetPositionFromStrobe (
  IN    UINT8   Strobe
);

/*++

  Gets the DRAM QCK ODT Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [IN    ] Strobe          - Strobe (0-based)
  @param [   OUT] QckOdtValue     - Pointer to store the encoded DRAM QCK ODT value

  @retval True or False, depending if the DRAM QCK ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetDramQckValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  IN    UINT8   Strobe,
  OUT   UINT16  *QckOdtValue
  );

/*++

  Gets the DRAM QCS ODT Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [IN    ] Strobe          - Strobe (0-based)
  @param [   OUT] QcsOdtValue     - Pointer to store the encoded DRAM QCS ODT value

  @retval True or False, depending if the DRAM QCS ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetDramQcsValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  IN    UINT8   Strobe,
  OUT   UINT16  *QcsOdtValue
  );

/*++

  Gets the DRAM QCA ODT Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [IN    ] Strobe          - Strobe (0-based)
  @param [   OUT] QcaOdtValue     - Pointer to store the encoded DRAM QCA ODT value

  @retval True or False, depending if the DRAM QCA ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetDramQcaValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  IN    UINT8   Strobe,
  OUT   UINT16  *QcaOdtValue
  );

/*++

  Gets the DRAM Ibt CA Value

  @param [IN    ] Socket     - Socket number
  @param [IN    ] Ch         - Channel number (0-based)
  @param [IN    ] Dimm       - Dimm number (0-based)
  @param [   OUT] IbtCaValue - Pointer to store the Ibt CA value

  @retval True or False, depending if the Ibt CA value is found in the ODT Value table
--*/
BOOLEAN
GetIbtCaValueOdt (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *IbtCaValue
  );

/*++

  Gets the DRAM DCA Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] DcaVrefValue - Pointer to store the DRAM DCA Vref value

  @retval True or False, depending if the DRAM DCA Vref value is found in the ODT Value table
--*/
BOOLEAN
GetDcaVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT16  *DcaVrefValue
  );

/*++

  Gets the DRAM QCA Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] QcaVrefValue    - Pointer to store the DRAM QCA Vref value

  @retval True or False, depending if the DRAM QCA Vref value is found in the backside ODT Value table
--*/
BOOLEAN
GetQcaVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   INT8    *QcaVrefValue
  );

/*++

  Gets the DRAM Ibt CS Value

  @param [IN    ] Socket     - Socket number
  @param [IN    ] Ch         - Channel number (0-based)
  @param [IN    ] Dimm       - Dimm number (0-based)
  @param [   OUT] IbtCsValue - Pointer to store the Ibt CS value

  @retval True or False, depending if the Ibt CS value is found in the ODT Value table
--*/
BOOLEAN
GetIbtCsValueOdt (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *IbtCsValue
  );

/*++

  Gets the DRAM DCS Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] DcsVrefValue - Pointer to store the DRAM CS Vref value

  @retval True or False, depending if the DRAM CS Vref value is found in the ODT Value table
--*/
BOOLEAN
GetDcsVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT16  *DcsVrefValue
  );

/*++

  Gets the DRAM QCS Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] QcsVrefValue    - Pointer to store the encoded DRAM QCS Vref value

  @retval True or False, depending if the DRAM CS Vref value is found in the ODT Value table
--*/
BOOLEAN
GetQcsVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   INT8    *QcsVrefValue
  );

/*++

  Gets the DRAM Ibt CLK Value

  @param [IN    ] Socket      - Socket number
  @param [IN    ] Ch          - Channel number (0-based)
  @param [IN    ] Dimm        - Dimm number (0-based)
  @param [   OUT] IbtClkValue - Pointer to store the Ibt CLK value

  @retval True or False, depending if the Ibt CLK value is found in the ODT Value table
--*/
BOOLEAN
GetIbtClkValueOdt (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *IbtClkValue
  );

/*++

  Gets the RDIMM DRAM RON or LRDIMM DB HOST RON

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] DramOrDbHostRon - RDIMM DRAM RON or LRDIMM DB Host RON

  @retval True or False, depending if the RDIMM DRAM RON or LRDIMM DB HOST RON is found in the ODT Value table
--*/
BOOLEAN
GetDramOrDbHostRon (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *DramOrDbHostRon
  );

#endif //_ODT_TABLE_LIB_H_
