/** @file
  Memory Jedec Definitions and declarations.

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

#ifndef _JEDEC_DEFINITIONS_H_
#define _JEDEC_DEFINITIONS_H_

//
// JEDEC manufacturer IDs from JEP-106
//

#define MFGID_AENEON          0x5705
#define MFGID_QIMONDA         0x5105
#define MFGID_NEC             0x1000
#define MFGID_IDT             0xB300
#define MFGID_TI              0x9700
#define MFGID_HYNIX           0xAD00
#define MFGID_MICRON          0x2C00
#define MFGID_INFINEON        0xC100
#define MFGID_SAMSUNG         0xCE00
#define MFGID_TEK             0x3D00
#define MFGID_KINGSTON        0x9801
#define MFGID_ELPIDA          0xFE02
#define MFGID_SMART           0x9401
#define MFGID_AGILENT         0xC802
#define MFGID_NANYA           0x0B03
#define MFGID_INPHI           0xB304
#define MFGID_MONTAGE         0x3206
#define MFGID_RAMBUS          0x9D06
#define MFGID_INTEL           0x8900
#define MFGID_VIKING          0x4001
#define MFGID_TRANSCEND       0x4f01
#define MFGID_ATP             0xE306
#define MFGID_CRUCIAL         0x9B05
#define MFGID_DATARAM         0x9101
#define MFGID_VENTURA         0x4305
#define MFGID_ADATA           0xCB04
#define MFGID_ACTICA          0xF186
#define MFGID_APACER          0x7A01
#define MFGID_INNODISK        0xF106
#define MFGID_UNIGEN          0xCE01
#define MFGID_WINTEC          0x6101
#define MFGID_V_COLOR         0x6D06
#define MFGID_VIRTIUM         0xBA05
#define MFGID_AVANT           0xF785
#define MFGID_LEGACY          0x4604
#define MFGID_CENTURY_MICRO   0xE904
#define MFGID_INTELIMEM       0x5D09
#define MFGID_NETLIST         0x1603

typedef struct {
  UINT16  MfgId;
  CHAR16  *String;
} MEMORY_MODULE_MANUFACTURER_LIST;

#define MEMORY_MODULE_MANUFACTURERS     \
  {MFGID_AENEON,        L"Aeneon"},     \
  {MFGID_AGILENT,       L"Agilent"},    \
  {MFGID_ELPIDA,        L"Elpida"},     \
  {0x02FE,              L"Elpida"},     \
  {MFGID_HYNIX,         L"Hynix"},      \
  {0x80AD,              L"Hynix"},      \
  {MFGID_IDT,           L"IDT"},        \
  {MFGID_INFINEON,      L"Infineon"},   \
  {MFGID_INPHI,         L"Inphi"},      \
  {MFGID_INTEL,         L"Intel"},      \
  {0x8006,              L"Intel"},      \
  {MFGID_KINGSTON,      L"Kingston"},   \
  {MFGID_MICRON,        L"Micron"},     \
  {0x802C,              L"Micron"},     \
  {MFGID_MONTAGE,       L"Montage"},    \
  {MFGID_NANYA,         L"Nanya"},      \
  {MFGID_NEC,           L"NEC"},        \
  {MFGID_QIMONDA,       L"Qimonda"},    \
  {0x8551,              L"Qimonda"},    \
  {MFGID_RAMBUS,        L"Rambus"},     \
  {MFGID_SAMSUNG,       L"Samsung"},    \
  {0x80CE,              L"Samsung"},    \
  {MFGID_SMART,         L"Smart"},      \
  {MFGID_TEK,           L"TEK"},        \
  {MFGID_TI,            L"TI"},         \
  {MFGID_TRANSCEND,     L"Transcend"},  \
  {0x014F,              L"Transcend"},  \
  {MFGID_VIKING,        L"Viking"},     \
  {MFGID_ATP,           L"ATP"},        \
  {MFGID_APACER,        L"Apacer"},     \
  {MFGID_CRUCIAL,       L"Crucial"},    \
  {MFGID_DATARAM,       L"DataRam"},    \
  {MFGID_VENTURA,       L"Ventura"},    \
  {MFGID_ADATA,         L"Adata"},      \
  {MFGID_ACTICA,        L"Actica"},     \
  {MFGID_INNODISK,      L"Innodisk"},   \
  {MFGID_UNIGEN,        L"Unigen"},     \
  {MFGID_WINTEC,        L"Wintec"},     \
  {MFGID_V_COLOR,       L"V_Color"},    \
  {MFGID_VIRTIUM,       L"Virtium"},    \
  {MFGID_AVANT,         L"Avant"},      \
  {MFGID_LEGACY,        L"Legacy"},     \
  {MFGID_CENTURY_MICRO, L"Century"},    \
  {MFGID_INTELIMEM,     L"Intelimem"},  \
  {MFGID_NETLIST,       L"Netlist"},    \
  {0xffff,              L"Unknown"}

//
// Revisions
//

#define SPD_MODPART_WILDCARD       '*'

#define IDT_SPD_REGREV_B0          0x20
#define IDT_SPD_REGREV_C0          0x30
#define MONTAGE_SPD_REGREV_B1      0xB1
#define INPHI_SPD_REGREV_B0        0x01
#define TI_SPD_REGREV_A6           0x0B

#define MICRON_SPD_DRAMREV_E       0x45
#define MICRON_SPD_DRAMREV_I       0x49

#define IDT_SPD_REGREV_GEN2_B0     0x50
#define INPHI_SPD_REGREV_GEN2_C0   0x10
#define RAMBUS_SPD_REGREV_GEN2_B1  0x21
#define MONTAGE_SPD_REGREV_GEN2_A0 0xA0
#define MONTAGE_SPD_REGREV_GEN2_H0 0x80

#define SAMSUNG_SPD_MODDATE_CTLE        0x1906
// The date at or after which Samsung C-die DRAMs have CTLE enabled. The encoding of 0x1906 indicates 2019ww06.
#define SAMSUNG_SPD_MODPART_DDR4_D_DIE  "M3**A*K4*DB2-CWE"
// The 'D' near the end indicates that this is a D die, but the rest are used to confirm that the part number is valid.

#define RAMBUS_SPD_REGREV_B0  0x01
#define RAMBUS_SPD_REGREV_C0  0x02
#define RAMBUS_SPD_REGREV_C1  0x03
#define RAMBUS_SPD_REGREV_D0  0x10
#define RAMBUS_SPD_REGREV_D1  0x11

#define LRBUF_INPHI_DVID      0x004018C7
#define LRBUF_INPHI_RID_A0    0x00
#define LRBUF_INPHI_RID_A1    0x01
#define LRBUF_INPHI_RID_B0    0x10

#define LRBUF_IDT_SPD_REGVEN    MFGID_IDT
#define LRBUF_IDT_SPD_REGREV_B0 IDT_SPD_REGREV_B0
#define LRBUF_IDT_DB_RID_A0     0x10
#define LRBUF_IDT_DB_RID_GPB0   0xB0
#define LRBUF_IDT_DB_RID_GPC0   0xC0
#define LRBUF_IDT_DB_RID_GPC1   0xC1

#define LRBUF_MONTAGE_RID_A1      0xA1
#define LRBUF_MONTAGE_RID_GEN2_A0 0x10
#define LRBUF_MONTAGE_RID_GEN2_A1 0x11
#define LRBUF_MONTAGE_DB_RID_B0   0x20

//
// JEDEC Mode Register Set (MRS) Command definitions
//

#define RDIMM_RC00     0x00
#define RDIMM_RC01     0x01
#define RDIMM_RC02     0x02
#define RDIMM_RC03     0x03
#define RDIMM_RC04     0x04
#define RDIMM_RC05     0x05
#define RDIMM_RC06     0x06
#define RDIMM_RC08     0x08
#define RDIMM_RC09     0x09
#define RDIMM_RC0A     0x0A
#define RDIMM_RC0B     0x0B
#define RDIMM_RC0C     0x0C
#define RDIMM_RC0D     0x0D
#define RDIMM_RC0E     0x0E
#define RDIMM_RC0F     0x0F
#define RDIMM_RC1x     0x10
#define RDIMM_RC2x     0x20
#define RDIMM_RC3x     0x30
#define RDIMM_RC4x     0x40
#define RDIMM_RC5x     0x50
#define RDIMM_RC6x     0x60
#define RDIMM_RC7x     0x70
#define RDIMM_RC8x     0x80
#define RDIMM_RC9x     0x90
#define RDIMM_RCAx     0xA0
#define RDIMM_RCBx     0xB0

#define LRDIMM_BC00    0x00
#define LRDIMM_BC01    0x01
#define LRDIMM_BC02    0x02
#define LRDIMM_BC03    0x03
#define LRDIMM_BC04    0x04
#define LRDIMM_BC05    0x05
#define LRDIMM_BC06    0x06
#define LRDIMM_BC07    0x07
#define LRDIMM_BC08    0x08
#define LRDIMM_BC09    0x09
#define LRDIMM_BC0A    0x0A
#define LRDIMM_BC0B    0x0B
#define LRDIMM_BC0C    0x0C
#define LRDIMM_BC0E    0x0E

#define LRDIMM_BC0x    0x00
#define LRDIMM_BC1x    0x10
#define LRDIMM_BC2x    0x20
#define LRDIMM_BC3x    0x30
#define LRDIMM_BC4x    0x40
#define LRDIMM_BC5x    0x50
#define LRDIMM_BC6x    0x60
#define LRDIMM_BC7x    0x70
#define LRDIMM_BC8x    0x80
#define LRDIMM_BC9x    0x90
#define LRDIMM_BCAx    0xA0
#define LRDIMM_BCBx    0xB0
#define LRDIMM_BCCx    0xC0
#define LRDIMM_BCDx    0xD0
#define LRDIMM_BCEx    0xE0
#define LRDIMM_BCFx    0xF0
#define LRDIMM_F0      0x0
#define LRDIMM_F1      0x1
#define LRDIMM_F2      0x2
#define LRDIMM_F3      0x3
#define LRDIMM_F4      0x4
#define LRDIMM_F5      0x5
#define LRDIMM_F6      0x6
#define LRDIMM_F7      0x7
#define LRDIMM_F8      0x8
#define LRDIMM_F9      0x9
#define LRDIMM_F12     0x0C

#define LRDIMM_BCAx_VALID   BIT0
#define LRDIMM_BCBx_VALID   BIT1
#define LRDIMM_BCDx_VALID   BIT2

#define RDIMM_RW00     0x00
#define RDIMM_RW01     0x01
#define RDIMM_RW02     0x02
#define RDIMM_RW03     0x03
#define RDIMM_RW04     0x04
#define RDIMM_RW05     0x05
#define RDIMM_RW06     0x06
#define RDIMM_RW07     0x07
#define RDIMM_RW08     0x08
#define RDIMM_RW09     0x09
#define RDIMM_RW0A     0x0A
#define RDIMM_RW0B     0x0B
#define RDIMM_RW0C     0x0C
#define RDIMM_RW0E     0x0E
#define RDIMM_RW0F     0x0F //Internal VrefCS Control Word.
#define RDIMM_RW10     0x10
#define RDIMM_RW12     0x12
#define RDIMM_RW13     0x13
#define RDIMM_RW14     0x14
#define RDIMM_RW15     0x15
#define RDIMM_RW16     0x16
#define RDIMM_RW17     0x17
#define RDIMM_RW18     0x18
#define RDIMM_RW19     0x19
#define RDIMM_RW1A     0x1A
#define RDIMM_RW1B     0x1B
#define RDIMM_RW1C     0x1C

#define RDIMM_RW25     0x25
#define RDIMM_RW26     0x26
#define RDIMM_RW27     0x27
#define RDIMM_RW28     0x28
#define RDIMM_RW29     0x29
#define RDIMM_RW2A     0x2A

#define RDIMM_RW30     0x30
#define RDIMM_RW31     0x31

#define RDIMM_RW3F     0x3F
#define RDIMM_RW40     0x40
#define RDIMM_RW41     0x41
#define RDIMM_RW42     0x42
#define RDIMM_RW43     0x43
#define RDIMM_RW44     0x44
#define RDIMM_RW45     0x45
#define RDIMM_RW46     0x46
#define RDIMM_RW47     0x47
#define RDIMM_RW48     0x48
#define RDIMM_RW49     0x49
#define RDIMM_RW4A     0x4A

#define RDIMM_RW60     0x60
#define RDIMM_RW62     0x62
#define RDIMM_RW66     0x66
#define RDIMM_RW68     0x68
#define RDIMM_RW6A     0x6A
#define RDIMM_RW6D     0x6D
#define RDIMM_RW6E     0x6E
#define RDIMM_RW72     0x72
#define RDIMM_RW76     0x76
#define RDIMM_RW78     0x78
#define RDIMM_RW7A     0x7A
#define RDIMM_RW7E     0x7E

#define RDIMM_RW83     0x83
#define RDIMM_RW97     0x97
#define RDIMM_RW98     0x98
#define RDIMM_RW99     0x99
#define RDIMM_RW9A     0x9A
#define RDIMM_RW9B     0x9B
#define RDIMM_RW9C     0x9C
#define RDIMM_RW9D     0x9D
#define RDIMM_RWDF     0xDF
#define LRDIMM_RWE0    0xE0
#define LRDIMM_RWE1    0xE1
#define LRDIMM_RWE2    0xE2
#define LRDIMM_RWE3    0xE3
#define LRDIMM_RWE4    0xE4
#define LRDIMM_RWE5    0xE5
#define LRDIMM_RWE6    0xE6
#define LRDIMM_RWE7    0xE7
#define LRDIMM_RWE8    0xE8
#define LRDIMM_RWE9    0xE9
#define LRDIMM_RWEA    0xEA
#define LRDIMM_RWEB    0xEB
#define LRDIMM_RWEC    0xEC
#define LRDIMM_RWED    0xED
#define LRDIMM_RWEE    0xEE
#define LRDIMM_RWEF    0xEF
#define LRDIMM_RWF0    0xF0
#define LRDIMM_RWF1    0xF1
#define LRDIMM_RWF2    0xF2
#define LRDIMM_RWF3    0xF3
#define LRDIMM_RWF4    0xF4
#define LRDIMM_RWF6    0xF6
#define LRDIMM_RWF7    0xF7
#define LRDIMM_RWF8    0xF8
#define LRDIMM_RWF9    0xF9
#define LRDIMM_RWFA    0xFA
#define LRDIMM_RWFB    0xFB
#define LRDIMM_RWFC    0xFC
#define LRDIMM_RWFE    0xFE
#define LRDIMM_RWFF    0xFF


// DDR5 LRDIMM DB Page numbers
#define LRDIMM_DB_PAGE0  0x0
#define LRDIMM_DB_PAGE1  0x1
#define LRDIMM_DB_PAGE2  0x2
#define LRDIMM_DB_PAGE3  0x3
#define LRDIMM_DB_PAGE4  0x4
#define LRDIMM_DB_PAGE5  0x5
#define LRDIMM_DB_PAGE6  0x6
#define LRDIMM_DB_PAGE7  0x7
#define LRDIMM_DB_PAGE8  0x8
#define LRDIMM_DB_PAGE9  0x9
#define LRDIMM_DB_PAGEB  0xB
#define LRDIMM_DB_PAGE83 0x83
#define LRDIMM_DB_PAGECC 0xCC

#define RDIMM_RW_INVALID    0xFF
#define RDIMM_CW_PAGE0      0x0
#define RDIMM_CW_PAGE1      0x1
#define RDIMM_CW_PAGE2      0x2
#define RDIMM_CW_PAGE9A     0x9A
#define RDIMM_CW_PAGE9B     0x9B
#define RDIMM_CW_PAGEFB     0xFB
#define RDIMM_CW_PAGEFE     0xFE

#if FixedPcdGetBool(PcdMcrSupport)
#define RDIMM_CW_PAGE70    0x70
#endif // FixedPcdGetBool(PcdMcrSupport)

//
// F0 BC03 bit definitions
//

#define LRDIMM_HOSTDQDQSDRIVERS_DISABLE BIT3

//
// F0 BC06 bit definitions
//

#define LRDIMM_ZQCL    0x01
#define LRDIMM_ZQCS    0x02

//
// F0 BC0A bit definitions
//

#define LRDIMM_BC0A_DDR4_1600  0x0
#define LRDIMM_BC0A_DDR4_1866  0x1
#define LRDIMM_BC0A_DDR4_2133  0x2
#define LRDIMM_BC0A_DDR4_2400  0x3
#define LRDIMM_BC0A_DDR4_2666  0x4
#define LRDIMM_BC0A_DDR4_2933  0x5
#define LRDIMM_BC0A_DDR4_3200  0x6

// RW84 LRDIMM Operating Speed bit definitions
#define LRDIMM_RW84_DDR5_3200  0x0
#define LRDIMM_RW84_DDR5_3600  0x1
#define LRDIMM_RW84_DDR5_4000  0x2
#define LRDIMM_RW84_DDR5_4400  0x3
#define LRDIMM_RW84_DDR5_4800  0x4

// RW85 LRDIMM Fine Granularity definitions
#define LRDIMM_RW85_MAX_GRAN   400   // Fine Granularity range is 0-400 MT/s
#define LRDIMM_RW85_GRAN_INC   20    // Fine Granularity incremental speed steps are 20 MT/s

// MR26/27 default training patterns
#define MR26_DEFAULT_RD_TRAIN   0x5A
#define MR27_DEFAULT_RD_TRAIN   0x3C

// RW83 bit definitions
#define LRDIMM_EXIT_TRAINING    0x0  // Exit any of the training mode
#define LRDIMM_RECEN_TRAINING   0x1  // DRAM Interface Receive Enable Training
#define LRDIMM_RDDL_TRAINING    0x2  // MDQS Read Delay Training
#define LRDIMM_WRLVL_TRAINING   0x3  // DRAM Write Leveling
#define LRDIMM_WRDL_TRAINING    0x4  // MDQ Write Delay Training
#define LRDIMM_HWRLVL_TRAINING  0x5  // Host Interface Write Leveling
#define LRDIMM_HRD_TRAINING     0x6  // Host Interface Read Training

//
// F0 BC0C bit definitions
//

#define LRDIMM_NORMAL_MODE            0x0
#define LRDIMM_MREP_TRAINING_MODE     0x1
#define LRDIMM_DWL_TRAINING_MODE      0x4
#define LRDIMM_HWL_TRAINING_MODE      0x5
#define LRDIMM_MRD_TRAINING_MODE      0x6
#define LRDIMM_MWD_TRAINING_MODE      0x7
#define LRDIMM_DWL_EXT_TRAINING_MODE  0x8
#define LRDIMM_DWL_INT_TRAINING_MODE  0x9

//
// F0BC1x bit definitions
//

#define LRDIMM_PBA_MODE 0x1
#define LRDIMM_MPR_OVERRIDE_MODE 0x2
#define LRDIMM_PDA_MODE 0x4
#define LRDIMM_ONE_RANK_TIMING_MODE 0x8
#define LRDIMM_RD_PREAMBLE_TRAINING_MODE 0x10

//
// F5BC0x..F5BC3x and F6BC0x..F6BC3x definitions
//
#define  MAX_BUFFER_MPR  8 // Number of MPR registers in the buffer

//
// RC08 - Input/Output Configuration Control Word
//

#define QxC_2_0_enabled                    0
#define QxC_1_0_enabled_QxC_2_disabled     BIT0
#define QxC_2_1_disabled_QxC_0_enabled     BIT1
#define QxC_2_0_disabled                   BIT0 | BIT1
#define QxPAR_disabled                     BIT2  // Feature is enabled when Bit 2 is clear

//
// RC0B - Operating Voltage Vdd and VrefCA Source Control
//

#define RC0B_VDD_1_2V                0
#define RC0B_VDD_RSVD                1
#define RC0B_VDD_MASK                0x1

#define RC0B_QVREFCA_BVREFCA_OPTION1      0 // Vdd/2 connected to QVrefCA and BVrefCA
#define RC0B_QVREFCA_BVREFCA_OPTION2      2 // Internally generated Vref connected to QVrefCa and Vdd/2 connected to BVrefCA
#define RC0B_QVREFCA_BVREFCA_OPTION3      4 // Internally generated Vref connected to BVrefCa and Vdd/2 connected to QVrefCA
#define RC0B_QVREFCA_BVREFCA_OPTION4      6 // External VrefCA input connected to QVrefCA and BVrefCA
#define RC0B_QVREFCA_BVREFCA_OPTION_MASK  0x6

#define RC0B_INPUT_VREF_SOURCE_INTERNAL  0 // Internally generated Vref
#define RC0B_INPUT_VREF_SOURCE_EXTERNAL  8 // External VrefCA input
#define RC0B_INPUT_VREF_SOURCE_MASK      0x8

//
// Below are some JEDEC-defined DDR4 and DDR5 constants used by ODT code/logic
//
#define CODE_ODT     0
#define DECODE_ODT   1

#define ENCODE_RTT   0 // Convert readable RTT value to RTT MR register bits.
#define DECODE_RTT   1 // Convert RTT MR register bits to readable RTT value.

#define ODT_40_OHMS   40
#define ODT_45_OHMS   45
#define ODT_50_OHMS   50
#define ODT_55_OHMS   55
#define ODT_100_OHMS  100
#define na            0

// DDR4 Rtt_wr
#define  DDR4_RTT_WR_DIS    0
#define  DDR4_RTT_WR_120    BIT0
#define  DDR4_RTT_WR_240    BIT1
#define  DDR4_RTT_WR_80     BIT2
#define  DDR4_RTT_WR_60     (BIT2 + BIT0) // Placeholder
#define  DDR4_RTT_WR_INF    (BIT1 | BIT0)

#define  RTTWR_DIS    0
#define  RTTWR_120    120
#define  RTTWR_240    240
#define  RTTWR_80     80
#define  RTTWR_60     60
#define  RTTWR_INF    999

// DDR4 Rtt_nom
#define  DDR4_RTT_NOM_DIS   0
#define  DDR4_RTT_NOM_INF   0
#define  DDR4_RTT_NOM_60    BIT0
#define  DDR4_RTT_NOM_120   BIT1
#define  DDR4_RTT_NOM_40    (BIT1 | BIT0)
#define  DDR4_RTT_NOM_240   BIT2
#define  DDR4_RTT_NOM_48    (BIT2 | BIT0)
#define  DDR4_RTT_NOM_80    (BIT2 | BIT1)
#define  DDR4_RTT_NOM_34    (BIT2 | BIT1 | BIT0)

// DDR4 Rtt_park
#define  DDR4_RTT_PARK_DIS   0
#define  DDR4_RTT_PARK_INF   0
#define  DDR4_RTT_PARK_60    BIT0
#define  DDR4_RTT_PARK_120   BIT1
#define  DDR4_RTT_PARK_40    (BIT1 | BIT0)
#define  DDR4_RTT_PARK_240   BIT2
#define  DDR4_RTT_PARK_48    (BIT2 | BIT0)
#define  DDR4_RTT_PARK_80    (BIT2 | BIT1)
#define  DDR4_RTT_PARK_34    (BIT2 | BIT1 | BIT0)

// ODT equates used to program mode registers
#define DDR4_RTT_WR_ODT_SHIFT 9
#define DDR4_RTT_WR_ODT_MASK  (BIT11 | BIT10 | BIT9)
#define DDR4_RTT_PRK_ODT_SHIFT 6
#define DDR4_RTT_PRK_ODT_MASK  (BIT8 | BIT7 | BIT6)
#define DDR4_RTT_NOM_ODT_SHIFT 8
#define DDR4_RTT_NOM_ODT_MASK  (BIT10 | BIT9 | BIT8)

#define DDR4_DRAM_RON_34 0
#define DDR4_DRAM_RON_48 1

#define DDR_RON_34 34
#define DDR_RON_40 40
#define DDR_RON_48 48

//
// DDR5 DQ/DQS RTT.
// Unlike DDR4 used to, DDR5 does use the same values for all:
// Dqs_rtt_park, Rtt_park, Rtt_wr, Rtt_nom_wr & Rtt_nom_rd.
//

#ifdef USE_LATEST_DRAM_SPEC
#define INITIAL_ODTLON_WR_OFFSET  1 // 001B: -4 Clocks
#define INITIAL_ODTLOFF_WR_OFFSET 1 // 001B: +4 Clocks
#else
#define INITIAL_ODTLON_WR_OFFSET  2 // 010B: -3 Clocks
#define INITIAL_ODTLOFF_WR_OFFSET 2 // 010B: +3 Clocks
#endif

#define  DDR5_RTT_NA  0x0
#define  DDR5_RTT_OFF 0x0
#define  DDR5_RTT_240 0x1
#define  DDR5_RTT_120 0x2
#define  DDR5_RTT_80  0x3
#define  DDR5_RTT_60  0x4
#define  DDR5_RTT_48  0x5
#define  DDR5_RTT_40  0x6
#define  DDR5_RTT_34  0x7

#define  DDR5_RTT_NA_OHMS  0x0
#define  DDR5_RTT_OFF_OHMS 0x0
#define  DDR5_RTT_240_OHMS 240
#define  DDR5_RTT_120_OHMS 120
#define  DDR5_RTT_80_OHMS  80
#define  DDR5_RTT_60_OHMS  60
#define  DDR5_RTT_48_OHMS  48
#define  DDR5_RTT_40_OHMS  40
#define  DDR5_RTT_34_OHMS  34

//
// Bits of offset within the corresponding Mode Register.
//
#define DDR5_CK_ODT_REG_OFFSET          0 // MR32
#define DDR5_CS_ODT_REG_OFFSET          3 // MR32
#define DDR5_CA_ODT_REG_OFFSET          0 // MR33
#define DDR5_DQS_RTT_PARK_REG_OFFSET    3 // MR33
#define DDR5_RTT_PARK_REG_OFFSET        0 // MR34
#define DDR5_RTT_WR_REG_OFFSET          3 // MR34
#define DDR5_RTT_NOM_WR_REG_OFFSET      0 // MR35
#define DDR5_RTT_NOM_RD_REG_OFFSET      3 // MR35
#define DDR5_DRAM_RON_PULL_UP_OFFSET    1             // MR5
#define DDR5_DRAM_RON_PULL_UP_MASK      (BIT2 | BIT1) // MR5
#define DDR5_DRAM_RON_PULL_DOWN_OFFSET  6             // MR5
#define DDR5_DRAM_RON_PULL_DOWN_MASK    (BIT7 | BIT6) // MR5

#define DDR5_DB_RON_PULL_UP_34     0  // RW8A 34 Ohms Pull up value
#define DDR5_DB_RON_PULL_DOWN_34   0  // RW8A 34 Ohms Pull down value

//
// DDR5 MR5 for Ron.
//
#define DDR5_DRAM_RON_34_BITS   0 //00B: RZQ/7 (34)
#define DDR5_DRAM_RON_40_BITS   1 //01B: RZQ/6 (40)
#define DDR5_DRAM_RON_48_BITS   2 //10B: RZQ/5 (48)

//
// DDR5 LRDIMM RW8A DB HOST Ron
//
#define DDR5_DB_HOST_RON_34_BITS   0 //00B: RZQ/7 (34)
#define DDR5_DB_HOST_RON_48_BITS   2 //10B: RZQ/5 (48)

//
// DDR5 LRDIMM RW8B MDQ RON
//
#define DDR5_MDQ_RON_34_BITS   0 //00B: RZQ/7 (34)
#define DDR5_MDQ_RON_40_BITS   1 //01B: RZQ/6 (40)
#define DDR5_MDQ_RON_48_BITS   2 //10B: RZQ/5 (48)

//
// DDR5 MR32 MR33 for CK, CS and CA RTT.
//
#define DDR5_RTT_CK_CS_CA_RTT_OFF_BITS  0
#define DDR5_RTT_CK_CS_CA_RTT_480_BITS  (BIT0)
#define DDR5_RTT_CK_CS_CA_RTT_240_BITS  (BIT1)
#define DDR5_RTT_CK_CS_CA_RTT_120_BITS  (BIT1 | BIT0)
#define DDR5_RTT_CK_CS_CA_RTT_80_BITS   (BIT2)
#define DDR5_RTT_CK_CS_CA_RTT_60_BITS   (BIT2 | BIT0)
#define DDR5_RTT_CK_CS_CA_RTT_40_BITS   (BIT2 | BIT1 | BIT0)

#define DDR5_RTT_CK_CS_CA_RTT_OFF_OHMS  0
#define DDR5_RTT_CK_CS_CA_RTT_480_OHMS  480
#define DDR5_RTT_CK_CS_CA_RTT_240_OHMS  240
#define DDR5_RTT_CK_CS_CA_RTT_120_OHMS  120
#define DDR5_RTT_CK_CS_CA_RTT_80_OHMS   80
#define DDR5_RTT_CK_CS_CA_RTT_60_OHMS   60
#define DDR5_RTT_CK_CS_CA_RTT_40_OHMS   40

//
// DDR5 MR13 for tCCD_L / tDLLK
//
#define DataRate_2001_3200              0
#define DataRate_3201_3600              1
#define DataRate_3601_4000              2
#define DataRate_4001_4400              3
#define DataRate_4401_4800              4
#define DataRate_4801_5200              5
#define DataRate_5201_5600              6
#define DataRate_5601_6000              7
#define DataRate_6001_6400              8

//
// DDR5 tXP should be max(7.5ns, 8nCK), tRAS should be greater than 32us
//
#define tXP_PS                          7500    // 7.5 ns
#define tXP_MIN_DCLK                    8       // 8 dclk
#define tRAS_MIN_NS                     32      // 32 ns
//
// DDR5 tCKSRKX should be max(3.5ns, 8nCK)
//
#define tCKSRKX_MIN_PS                  3500    // 3.5 ns
#define tCKSRKX_MIN_DCLK                8       // 8 dclk
//
// DDR5 tSRKX2SRX should be max(10ns, 16nCK)
//
#define tCKSRKX2SRX_MIN_NS              10      // 10 ns
#define tCKSRKX2SRX_MIN_DCLK            16      // 16 dclk
//
// DDR5 TCkAct should be 16nCK
//
#define tCKACT_MAX_DCLK                 16      // 16 dclk
//
// DDR5 tCSSR should be max(15ns, 24nCK)
//
#define tCSSR_MIN_NS                    15      // 15 ns
#define tCSSR_MIN_DCLK                  24      // 24 dclk

#endif // #ifndef _JEDEC_DEFINITIONS_H_
