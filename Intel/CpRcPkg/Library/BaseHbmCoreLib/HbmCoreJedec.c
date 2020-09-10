/** @file
  Interface source file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include "HbmCore.h"

HBM_DIMM_MTR_ENCODINGS mDimmMtrEncode[] = {
  //Density;    ca_width;       ra_width;    number_of_banks;     rsvd
  { Hbm1Gb,     {CA_WIDTH_HBM_6, RA_WIDTH_13, NUMBER_OF_BANKS_8,  0}}, // Only available for Legacy Mode
  { Hbm2Gb,     {CA_WIDTH_HBM_6, RA_WIDTH_14, NUMBER_OF_BANKS_8,  0}}, // available for Legacy Mode and Pseudo Channel Mode, different ca_width
  { Hbm4Gb,     {CA_WIDTH_HBM_6, RA_WIDTH_14, NUMBER_OF_BANKS_16, 0}}, // available for Legacy Mode and Pseudo Channel Mode, different ca_width
  { Hbm8Gb8H,   {CA_WIDTH_HBM_6, RA_WIDTH_14, NUMBER_OF_BANKS_32, 0}}, // This one and below only available for Pseudo Channel Mode
  { Hbm6Gb,     {CA_WIDTH_HBM_6, RA_WIDTH_15, NUMBER_OF_BANKS_16, 0}},
  { Hbm8Gb,     {CA_WIDTH_HBM_6, RA_WIDTH_15, NUMBER_OF_BANKS_16, 0}},
  { Hbm12Gb8H,  {CA_WIDTH_HBM_6, RA_WIDTH_15, NUMBER_OF_BANKS_32, 0}},
  { Hbm8Gb12H,  {CA_WIDTH_HBM_6, RA_WIDTH_14, NUMBER_OF_BANKS_48, 0}},
  { Hbm16Gb8H,  {CA_WIDTH_HBM_6, RA_WIDTH_15, NUMBER_OF_BANKS_32, 0}},
  { Hbm12Gb12H, {CA_WIDTH_HBM_6, RA_WIDTH_15, NUMBER_OF_BANKS_48, 0}},
  { Hbm16Gb12H, {CA_WIDTH_HBM_6, RA_WIDTH_15, NUMBER_OF_BANKS_48, 0}},
 };

/**

  This function returns the value of tCK with unit pico seconds

  @param[in] Socket  - Socket Id

  @retval UINT16 Value of tCK

**/
UINT16
HbmGettCK (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 714;  // HBM2e_2.8_GTPerSec
  } else {
    return 625;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tRCD_RD

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRCD_RD

**/
UINT32
HbmGettRCD_RD (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x15;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x18;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tRCD_WR

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRCD_WR

**/
UINT32
HbmGettRCD_WR (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0xF;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x11;  // HBM2e_3.2_GTPerSec
  }
}

UINT32
HbmGettRCD_ImpRD (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x29;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x2F;  // HBM2e_3.2_GTPerSec
  }
}


UINT32
HbmGettRCD_ImpWR (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x23;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x28;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tRRD_L

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRRD_L

**/
UINT32
HbmGettRRD_L (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x4;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x7;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tRRD

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRRD

**/
UINT32
HbmGettRRD (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x4;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x7;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tFAW

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tFAW

**/
UINT32
HbmGettFAW (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x16;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x1C;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tCL

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCL

**/
UINT32
HbmGettCL (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x22;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x1E;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tCWL

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCWL

**/
UINT32
HbmGettCWL (
  IN UINT8 Socket
  )
{
  return 0xF;  // HBM2e_2.8_GTPerSec, 2_GTPerSec
}

/**

  This function returns the value of tRTP

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRTP

**/
UINT32
HbmGettRTP (
  IN UINT8 Socket
  )
{
  return 0x3;  // HBM2e_2.8_GTPerSec, HBM2e_3.2_GTPerSec
}

// t_wr - unused - replaced by t_wrpre

/**

  This function returns the value of tRP

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRP

**/
UINT32
HbmGettRP (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x12;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x15;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tRC

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRC

**/
UINT32
HbmGettRC (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x43;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x4C;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tRAS

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRAS

**/
UINT32
HbmGettRAS (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x36;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x30;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tPRPDEN

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tPRPDEN

**/
UINT32
HbmGettPRPDEN (
  IN UINT8 Socket
  )
{
  return 0x1;  // HBM2e_2.8_GTPerSec, HBM2e_3.2_GTPerSec
}

/**

  This function returns the value of tCKE

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCKE

**/
UINT32
HbmGettCKE (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0xB;  // HBM2e_2.8_GTPerSec
  } else {
    return 0xC;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tXP

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tXP

**/
UINT32
HbmGettXP (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0xB;  // HBM2e_2.8_GTPerSec
  } else {
    return 0xC;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tRDPDEN

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRDPDEN

**/
UINT32
HbmGettRDPDEN (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x21;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x25;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tWRPDEN

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRPDEN

**/
UINT32
HbmGettWRPDEN (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x24;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x27;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tRDA

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRDA

**/
UINT32
HbmGettRDA (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x17;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x1A;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tWR

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRA

**/
UINT32
HbmGettWR (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x16;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x19;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tWRA

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRA

**/
UINT32
HbmGettWRA (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x36;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x3C;  // HBM2e_3.2_GTPerSec
  }
}

/**

  This function returns the value of tWRPRE

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRPRE

**/
UINT32
HbmGettWRPRE (
  IN UINT8 Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x24;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x27;  // HBM2e_3.2_GTPerSec
  }
}


/**

  This function returns the value of tWRRDA

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRRDA

**/
UINT32
HbmGettWRRDA (
  IN UINT8 Socket
  )
{
  return 0x3C;  // HBM2e_2.8_GTPerSec, HBM2e_3.2_GTPerSec
}

/**

  Writes HBM Mode registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                StackId             Stack ID
  @param[in]                Mrs                 MRS register number (0-based)
  @param[in]                Data                Data for the MRS write
  @param[in]                DatAccessMethod     Read or Write

**/
VOID
HbmWriteMRS (
  IN        UINT8               Socket,
  IN        UINT8               IoModule,
  IN        UINT8               StackId,
  IN        UINT8               Mrs,
  IN        UINT8               Data,
  IN        UINT8               AccessMethod
  )
{

  SetHbmCacheMr (Socket, IoModule, Mrs, Data);

  switch (AccessMethod)  {
    case IEEE_1500:
      WriteMrs1500 (Socket, IoModule, Mrs, Data);
      break;
    default:
      HbmDebugPrint (HBM_SDBG_MAX, "IO_MOD%d: HbmWriteMRS HBM MRS Invalid access method = %d \n", IoModule, AccessMethod);
      break;
  } // switch accessMethod
  return;
} // HbmWriteMRS



/**

  Writes HBM Mode registers using IEEE1500


  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                Mrs                 MRS register number (0-based)
  @param[in]                Data                Data for the MRS write

**/
VOID
WriteMrs1500 (
  IN        UINT8           Socket,
  IN        UINT8           IoModule,
  IN        UINT8           Mrs,
  IN        UINT8           Data
  )
{
  UINT8   DataByte[HbmMaxCacheMr];

  if (!IsHbmMemSsEnabled (Socket, IoModule)) {
    return;
  }

  DataByte[0]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr00);
  DataByte[1]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr01);
  DataByte[2]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr02);
  DataByte[3]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr03);
  DataByte[4]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr04);
  DataByte[5]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr05);
  DataByte[6]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr06);
  DataByte[7]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr07);
  DataByte[8]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr08);
  DataByte[9]       = GetHbmCacheMr (Socket, IoModule, HbmCacheMr09);
  DataByte[10]      = GetHbmCacheMr (Socket, IoModule, HbmCacheMr10);
  DataByte[11]      = GetHbmCacheMr (Socket, IoModule, HbmCacheMr11);
  DataByte[12]      = GetHbmCacheMr (Socket, IoModule, HbmCacheMr12);
  DataByte[13]      = GetHbmCacheMr (Socket, IoModule, HbmCacheMr13);
  DataByte[14]      = GetHbmCacheMr (Socket, IoModule, HbmCacheMr14);
  DataByte[15]      = GetHbmCacheMr (Socket, IoModule, HbmCacheMr15);

  if (Mrs < HbmMaxCacheMr) {
    DataByte[Mrs]   = Data;
  }

  //
  // Initiate Write
  //
  Hbm1500PortCmd (Socket, IoModule, WIR_CHANNEL_ALL, MODE_REG_DUMP_SET, PAYLOAD_MODE_REGISTER_DUMP, (UINT8 *) DataByte);

  return;
} // WriteMrs1500

/**

  JEDEC initialization for HBM.

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
HbmJedecInit (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  )
{
  UINT8     Mrs              = 0;
  UINT8     Data[HbmMaxCacheMr] = {0};
  UINT8     AccessMethod     = IEEE_1500;

  HbmDebugPrint (HBM_SDBG_MAX, "M%d: HbmJedecInit -- Start \n", IoModule);

  Data[0]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr00);
  Data[1]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr01);
  Data[2]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr02);
  Data[3]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr03);
  Data[4]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr04);
  Data[5]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr05);
  Data[6]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr06);
  Data[7]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr07);
  Data[8]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr08);
  Data[9]  = GetHbmCacheMr (Socket, IoModule, HbmCacheMr09);
  Data[10] = GetHbmCacheMr (Socket, IoModule, HbmCacheMr10);
  Data[11] = GetHbmCacheMr (Socket, IoModule, HbmCacheMr11);
  Data[12] = GetHbmCacheMr (Socket, IoModule, HbmCacheMr12);
  Data[13] = GetHbmCacheMr (Socket, IoModule, HbmCacheMr13);
  Data[14] = GetHbmCacheMr (Socket, IoModule, HbmCacheMr14);
  Data[15] = GetHbmCacheMr (Socket, IoModule, HbmCacheMr15);

  for (Mrs = 0; Mrs < HbmMaxCacheMr; Mrs++) {
    //
    // Update MRS structures in case of changes
    //
    SetHbmCacheMr (Socket, IoModule, Mrs, Data[Mrs]);

    // if IEEE_1500 port is use then write all MRS 0-15
    if ((HbmCacheMr15 == Mrs) && (IEEE_1500 == AccessMethod)){
      HbmWriteMRS (Socket, IoModule, 0, Mrs, Data[Mrs], AccessMethod);
    }
  }

  HbmDebugPrint (HBM_SDBG_MAX, "M%d: HbmJedecInit -- End \n", IoModule);

} // HbmJedecInit

/**

  Generate HBM commands IEEE1500

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Command             IEEE1500 command
  @param[in]                Size                Size of data to Rd/Wr to/from IEEE1500
  @param[in]                Data                Data return path

**/
VOID
Hbm1500PortCmd (
  IN        UINT8           Socket,
  IN        UINT8           IoModule,
  IN        UINT8           Channel,
  IN        UINT8           Command,
  IN        UINT8           Size,
  IN        UINT8           *Data
  )
{
}

/**

  Get HBM MTR per Density

  @param[in]      HbmDensity     - HBM Density
  @param[in]      AddressingMode - Addressing Mode Support
  @param[in, out] DimmMtr        - Pointer to DIMM MTR

  @retval EFI_SUCCESS     if got
          EFI_UNSUPPORTED otherwise

**/
EFI_STATUS
EFIAPI
HbmGetDimmMtr (
  IN     HBM_DENSITY             HbmDensity,
  IN     UINT8                   AddressingMode,
  IN OUT DIMMMTR_DATA_STRUCT     *DimmMtr
  )
{
  EFI_STATUS     Status = EFI_UNSUPPORTED;
  UINT8          TableSize;
  UINT8          Index;

  TableSize = sizeof (mDimmMtrEncode) / sizeof (mDimmMtrEncode[0]);

  for (Index = 0; Index < TableSize; Index++) {
    if (HbmDensity == mDimmMtrEncode[Index].HbmDensity) {
      *DimmMtr = mDimmMtrEncode[Index].DimmMtr;
      Status = EFI_SUCCESS;
      break;
    }
  }

  return Status;
}
