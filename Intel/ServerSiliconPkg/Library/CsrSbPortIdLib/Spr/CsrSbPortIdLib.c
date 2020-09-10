/** @file
  Implementation of CsrSbPortId class library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <Library/IoLib.h>
#include <CpuPciAccess.h>
#include <UncoreCommonIncludes.h>
#include <Library/DebugLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CsrSbPortIdLib.h>

UINT32 mSBReg_Pcie_PortID_FID_SPR_Tbl[] = {
  567 ,// PCIEGen4_RPA_DMI.PI5.0 S0) DMI_IAL.PI5.0 DMI has no FID

  587 + (0x8  << 24),// PCIEGen4_RPA_DMI.PI5.1
  588 + (0x10 << 24),// PCIEGen4_RPB.PI5.1
  589 + (0x18 << 24),// PCIEGen4_RPC.PI5.1
  590 + (0x20 << 24),// PCIEGen4_RPD.PI5.1
  592 + (0x28 << 24),// PCIEGen5_RPA_IAL.PI5.1
  593 + (0x30 << 24),// PCIEGen5_RPB.PI5.1
  594 + (0x38 << 24),// PCIEGen5_RPC.PI5.1
  595 + (0x40 << 24),// PCIEGen5_RPD.PI5.1

  607 + (0x8  << 24),// PCIEGen4_RPA_DMI.PI5.3
  608 + (0x10 << 24),// PCIEGen4_RPB.PI5.3
  609 + (0x18 << 24),// PCIEGen4_RPC.PI5.3
  610 + (0x20 << 24),// PCIEGen4_RPD.PI5.3
  612 + (0x28 << 24),// PCIEGen5_RPA_IAL.PI5.3
  613 + (0x30 << 24),// PCIEGen5_RPB.PI5.3
  614 + (0x38 << 24),// PCIEGen5_RPC.PI5.3
  615 + (0x40 << 24),// PCIEGen5_RPD.PI5.3

  627 + (0x8  << 24),// PCIEGen4_RPA_DMI.PI5.5
  628 + (0x10 << 24),// PCIEGen4_RPB.PI5.5
  629 + (0x18 << 24),// PCIEGen4_RPC.PI5.5
  630 + (0x20 << 24),// PCIEGen4_RPD.PI5.5
  632 + (0x28 << 24),// PCIEGen5_RPA_IAL.PI5.5
  633 + (0x30 << 24),// PCIEGen5_RPB.PI5.5
  634 + (0x38 << 24),// PCIEGen5_RPC.PI5.5
  635 + (0x40 << 24),// PCIEGen5_RPD.PI5.5

  647 + (0x8  << 24),// PCIEGen4_RPA_DMI.PI5.4
  648 + (0x10 << 24),// PCIEGen4_RPB.PI5.4
  649 + (0x18 << 24),// PCIEGen4_RPC.PI5.4
  650 + (0x20 << 24),// PCIEGen4_RPD.PI5.4
  652 + (0x28 << 24),// PCIEGen5_RPA_IAL.PI5.4
  653 + (0x30 << 24),// PCIEGen5_RPB.PI5.4
  654 + (0x38 << 24),// PCIEGen5_RPC.PI5.4
  655 + (0x40 << 24),// PCIEGen5_RPD.PI5.4

  667 + (0x8  << 24),// PCIEGen4_RPA_DMI.PI5.7
  668 + (0x10 << 24),// PCIEGen4_RPB.PI5.7
  669 + (0x18 << 24),// PCIEGen4_RPC.PI5.7
  670 + (0x20 << 24),// PCIEGen4_RPD.PI5.7
  672 + (0x28 << 24),// PCIEGen5_RPA_IAL.PI5.7
  673 + (0x30 << 24),// PCIEGen5_RPB.PI5.7
  674 + (0x38 << 24),// PCIEGen5_RPC.PI5.7
  675 + (0x40 << 24) // PCIEGen5_RPD.PI5.7
};

UINT8 mSBReg_Pcie_PortID_FID_SPR_Tbl_length = sizeof (mSBReg_Pcie_PortID_FID_SPR_Tbl)/sizeof (mSBReg_Pcie_PortID_FID_SPR_Tbl[0]);

UINT32 mSBReg_M2pcie_PortID_FID_SPR_Tbl[] = {
  486, // S0: MS2IOSF_SCF_IOCOH1
  487, // S1: MS2IOSF_SCF_IOCOH2
  490, // S2: MS2IOSF_SCF_IOCOH5
  492, // S3: MS2IOSF_SCF_IOCOH7
  491, // S4: MS2IOSF_SCF_IOCOH8
  494, // S5: MS2IOSF_SCF_IOCOH11
  //
  // For SPR BDF table, S6/S7 marked as N/A, currently use M2IOSF instance 4/10 instead
  //
  489, // S6: MS2IOSF_SCF_IOCOH4
  495, // S7: MS2IOSF_SCF_IOCOH10
  485, // S8: MS2IOSF_SCF_IOCOH0
  488, // S9: MS2IOSF_SCF_IOCOH3
  493, // S10:MS2IOSF_SCF_IOCOH6
  496, // S11:MS2IOSF_SCF_IOCOH9
};

UINT8 mSBReg_M2pcie_PortID_FID_SPR_Tbl_length = sizeof (mSBReg_M2pcie_PortID_FID_SPR_Tbl)/sizeof (mSBReg_M2pcie_PortID_FID_SPR_Tbl[0]);

UINT16 mSBReg_PSF_PortID_SPR_Tbl[] = {
  560, // (S0) PSF.PI5.0
  580, // (S1) PSF.PI5.1
  600, // (S2) PSF.PI5.3
  640, // (S3) PSF.PI5.4
  620, // (S4) PSF.PI5.5
  660, // (S5) PSF.PI5.7
  531, // (S6) PSF.PI5.2
  767  // (S7) PSF.PI5.6
};

UINT8 mSBReg_PSF_PortID_SPR_Tbl_length = sizeof (mSBReg_PSF_PortID_SPR_Tbl)/sizeof (mSBReg_PSF_PortID_SPR_Tbl[0]);


UINT16 mSBReg_CXPSMBUS_PortID_SPR_Tbl[] = {
  471 // CXPSMBUS.CXPSMBUS.0
};

UINT8 mSBReg_CXPSMBUS_PortID_SPR_Tbl_length = sizeof (mSBReg_CXPSMBUS_PortID_SPR_Tbl)/sizeof (mSBReg_CXPSMBUS_PortID_SPR_Tbl[0]);


UINT16 mSBReg_I3C_PortID_SPR_Tbl[] = {
  466, // SPDI3CBUS.SPDI3CBUS.0
  467  // SPDI3CBUS.SPDI3CBUS.1
};

UINT8 mSBReg_I3C_PortID_SPR_Tbl_length = sizeof (mSBReg_I3C_PortID_SPR_Tbl)/sizeof (mSBReg_I3C_PortID_SPR_Tbl[0]);

UINT16 mSBReg_PSF_DINO_PortID_FID_SPR_Tbl[] = {
  534, // (S8) PSF.DINO.0
  539, // (S9) PSF.DINO.1
  544, // (S10) PSF.DINO.2
  549  // (S11) PSF.DINO.3
};

UINT8 mSBReg_PSF_DINO_PortID_FID_SPR_Tbl_length = sizeof (mSBReg_PSF_DINO_PortID_FID_SPR_Tbl)/sizeof (mSBReg_PSF_DINO_PortID_FID_SPR_Tbl[0]);

UINT16 mSBReg_PSF_HCX_PortID_FID_SPR_Tbl[] = {
  514, // (S8) PSF.HCx.0
  518, // (S9) PSF.HCx.1
  522, // (S10) PSF.HCx.2
  526  // (S11) PSF.HCx.3
};

UINT8 mSBReg_PSF_HCX_PortID_FID_SPR_Tbl_length = sizeof (mSBReg_PSF_HCX_PortID_FID_SPR_Tbl)/sizeof (mSBReg_PSF_HCX_PortID_FID_SPR_Tbl[0]);

UINT32 mSBReg_PCIE_GEN5_PortID_FID_SPR_Tbl[] = {
  572, // (S0) PCIEGen5_RPA_IAL.PI5.0, UBOX PLA only decode CR_SB FID = 0
  592, // (S1) PCIEGen5_RPA_IAL.PI5.1
  612, // (S2) PCIEGen5_RPA_IAL.PI5.3
  652, // (S3) PCIEGen5_RPA_IAL.PI5.4
  632, // (S4) PCIEGen5_RPA_IAL.PI5.5
  672, // (S5) PCIEGen5_RPA_IAL.PI5.7
  683, // (S6) PCIEGen5_RPA_IAL.PI5.2
  779  // (S7) PCIEGen5_RPA_IAL.PI5.6
};

UINT8 mSBReg_PCIE_GEN5_PortID_FID_SPR_Tbl_length = sizeof (mSBReg_PCIE_GEN5_PortID_FID_SPR_Tbl)/sizeof (mSBReg_PCIE_GEN5_PortID_FID_SPR_Tbl[0]);

UINT32 mSBReg_PCIE_GEN4_PortID_FID_SPR_Tbl[] = {
  567, // (S0) PCIEGen4_RPA_DMI.PI5.0, UBOX PLA only decode CR_SB FID = 0
  587, // (S1) PCIEGen4_RPA_DMI.PI5.1
  607, // (S2) PCIEGen4_RPA_DMI.PI5.3
  647, // (S3) PCIEGen4_RPA_DMI.PI5.4
  627, // (S4) PCIEGen4_RPA_DMI.PI5.5
  667, // (S5) PCIEGen4_RPA_DMI.PI5.7
  678, // (S6) PCIEGen4_RPA_DMI.PI5.2
  774  // (S7) PCIEGen4_RPA_DMI.PI5.6
};

UINT8 mSBReg_PCIE_GEN4_PortID_FID_SPR_Tbl_length = sizeof (mSBReg_PCIE_GEN4_PortID_FID_SPR_Tbl)/sizeof (mSBReg_PCIE_GEN4_PortID_FID_SPR_Tbl[0]);

UINT32 mSBReg_FBLP_PortID_SPR_Tbl[] = {
  559,                    // (S0) Flexbus.PI5.0

  579 + (0 << 24),        // (S1) Flexbus.PI5.1 Port0
  SB_REG_PORT_ID_INVALID,
  579 + (1 << 24),        // (S1) Flexbus.PI5.1 Port1
  SB_REG_PORT_ID_INVALID,
  579 + (2 << 24),        // (S1) Flexbus.PI5.1 Port2
  SB_REG_PORT_ID_INVALID,
  579 + (3 << 24),        // (S1) Flexbus.PI5.1 Port3
  SB_REG_PORT_ID_INVALID,

  599 + (0 << 24),        // (S2) Flexbus.PI5.3 Port0
  SB_REG_PORT_ID_INVALID,
  599 + (1 << 24),        // (S2) Flexbus.PI5.3 Port1
  SB_REG_PORT_ID_INVALID,
  599 + (2 << 24),        // (S2) Flexbus.PI5.3 Port2
  SB_REG_PORT_ID_INVALID,
  599 + (3 << 24),        // (S2) Flexbus.PI5.3 Port3
  SB_REG_PORT_ID_INVALID,

  639 + (0 << 24),        // (S3) Flexbus.PI5.4 Port0
  SB_REG_PORT_ID_INVALID,
  639 + (1 << 24),        // (S3) Flexbus.PI5.4 Port1
  SB_REG_PORT_ID_INVALID,
  639 + (2 << 24),        // (S3) Flexbus.PI5.4 Port2
  SB_REG_PORT_ID_INVALID,
  639 + (3 << 24),        // (S3) Flexbus.PI5.4 Port3
  SB_REG_PORT_ID_INVALID,

  619 + (0 << 24),        // (S4) Flexbus.PI5.5 Port0
  SB_REG_PORT_ID_INVALID,
  619 + (1 << 24),        // (S4) Flexbus.PI5.5 Port1
  SB_REG_PORT_ID_INVALID,
  619 + (2 << 24),        // (S4) Flexbus.PI5.5 Port2
  SB_REG_PORT_ID_INVALID,
  619 + (3 << 24),        // (S4) Flexbus.PI5.5 Port3
  SB_REG_PORT_ID_INVALID,

  659 + (0 << 24),        // (S5) Flexbus.PI5.7 Port0
  SB_REG_PORT_ID_INVALID,
  659 + (1 << 24),        // (S5) Flexbus.PI5.7 Port1
  SB_REG_PORT_ID_INVALID,
  659 + (2 << 24),        // (S5) Flexbus.PI5.7 Port2
  SB_REG_PORT_ID_INVALID,
  659 + (3 << 24),        // (S5) Flexbus.PI5.7 Port3
  SB_REG_PORT_ID_INVALID,

  530 + (0 << 24),        // (S6) Flexbus.PI5.2 Port0
  SB_REG_PORT_ID_INVALID,
  530 + (1 << 24),        // (S6) Flexbus.PI5.2 Port1
  SB_REG_PORT_ID_INVALID,
  530 + (2 << 24),        // (S6) Flexbus.PI5.2 Port2
  SB_REG_PORT_ID_INVALID,
  530 + (3 << 24),        // (S6) Flexbus.PI5.2 Port3
  SB_REG_PORT_ID_INVALID,

  752 + (0 << 24),        // (S7) Flexbus.PI5.6 Port0
  SB_REG_PORT_ID_INVALID,
  752 + (1 << 24),        // (S7) Flexbus.PI5.6 Port1
  SB_REG_PORT_ID_INVALID,
  752 + (2 << 24),        // (S7) Flexbus.PI5.6 Port2
  SB_REG_PORT_ID_INVALID,
  752 + (3 << 24),        // (S7) Flexbus.PI5.6 Port3
  SB_REG_PORT_ID_INVALID
};

UINT8 mSBReg_FBLP_PortID_SPR_Tbl_length = sizeof (mSBReg_FBLP_PortID_SPR_Tbl)/sizeof (mSBReg_FBLP_PortID_SPR_Tbl[0]);

UINT32 mSBReg_FBLP_PRVT_PortID_SPR_Tbl[] = {
  559  + (16  << 24), // (S0) Flexbus.PI5.0 FID = 16
  579  + (16  << 24), // (S1) Flexbus.PI5.1 FID = 16
  599  + (16  << 24), // (S2) Flexbus.PI5.3 FID = 16
  639  + (16  << 24), // (S3) Flexbus.PI5.4 FID = 16
  619  + (16  << 24), // (S4) Flexbus.PI5.5 FID = 16
  659  + (16  << 24), // (S5) Flexbus.PI5.7 FID = 16
  530  + (16  << 24), // (S6) Flexbus.PI5.2 FID = 16
  752  + (16  << 24)  // (S7) Flexbus.PI5.6 FID = 16
};

UINT8 mSBReg_FBLP_PRVT_PortID_SPR_Tbl_length = sizeof (mSBReg_FBLP_PRVT_PortID_SPR_Tbl)/sizeof (mSBReg_FBLP_PRVT_PortID_SPR_Tbl[0]);

UINT32 mSBReg_CXL_AM_PortID_SPR_Tbl[] = {
  562, // (S0) Arb_Mux.PI5.0
  582, // (S1) Arb_Mux.PI5.1
  602, // (S2) Arb_Mux.PI5.3
  642, // (S3) Arb_Mux.PI5.4
  622, // (S4) Arb_Mux.PI5.5
  662, // (S5) Arb_Mux.PI5.7
  553, // (S6) Arb_Mux.PI5.2
  769  // (S7) Arb_Mux.PI5.6
};

UINT8 mSBReg_CXL_AM_PortID_SPR_Tbl_length = sizeof (mSBReg_CXL_AM_PortID_SPR_Tbl)/sizeof (mSBReg_CXL_AM_PortID_SPR_Tbl[0]);

/**
  This API gets the side band port ID based on the Box instance and function block info.

  @param[in]  UINT8             BoxInst
  @param[in]  UINT32            CsrOffset

  @retval PortId                The sideband port id as requested. If invalid arguments, returns SB_REG_PORT_ID_INVALID.
**/
UINT32
EFIAPI
GetSbRegPortId (
  IN UINT8    BoxInst,
  IN UINT32   CsrOffset
  )
{
  UINT32        SbRegPortId;
  CSR_OFFSET    RegOffset;

  RegOffset.Data = CsrOffset;
  SbRegPortId = SB_REG_PORT_ID_INVALID;

  if (RegOffset.Bits.boxtype == BOX_SB) {

    switch (RegOffset.Bits.funcblk) {
    case BOX_FUNC_SB_PCIE:                                      //func blk is pcie
      ASSERT (BoxInst < mSBReg_Pcie_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_Pcie_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_Pcie_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    case BOX_FUNC_SB_M2PCIE:                                    //func blk is m2pcie
      ASSERT (BoxInst < mSBReg_M2pcie_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_M2pcie_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_M2pcie_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    case BOX_FUNC_SB_PCIE_G5:                                   //func blk is pciegen5
      ASSERT (BoxInst < mSBReg_Pcie_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_Pcie_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_Pcie_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    case BOX_FUNC_SB_PSF:                                       //func blk is PSF
      ASSERT (BoxInst < mSBReg_PSF_PortID_SPR_Tbl_length);
      if (BoxInst < mSBReg_PSF_PortID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_PSF_PortID_SPR_Tbl[BoxInst];
      }
      break;
    case BOX_FUNC_SB_MCSMBUS:                                    //func blk is CXPSMB
      ASSERT (BoxInst < mSBReg_CXPSMBUS_PortID_SPR_Tbl_length);
      if (BoxInst < mSBReg_CXPSMBUS_PortID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_CXPSMBUS_PortID_SPR_Tbl[BoxInst];
      }
      break;

    case BOX_FUNC_SB_DMI:                                       //func blk is DMI
      ASSERT (BoxInst < mSBReg_Pcie_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_Pcie_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_Pcie_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    default:
      DEBUG ((DEBUG_ERROR, "[SB Access] Invalid funcblk: 0x%x \n", RegOffset.Bits.funcblk));
      break;
    }

  } else if (RegOffset.Bits.boxtype == BOX_SB_I3C)  {  //due to the limitation of func blk number, use new BOX_SB_I3C for I3C

    ASSERT (BoxInst < mSBReg_I3C_PortID_SPR_Tbl_length);
    if (BoxInst < mSBReg_I3C_PortID_SPR_Tbl_length) {
      SbRegPortId = (UINT32) mSBReg_I3C_PortID_SPR_Tbl[BoxInst];
    }

  } else if (RegOffset.Bits.boxtype == BOX_SB_EX) {
    switch (RegOffset.Bits.funcblk) {
    case BOX_FUNC_SB_PSF_DINO:
      ASSERT (BoxInst < mSBReg_PSF_DINO_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_PSF_DINO_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_PSF_DINO_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    case BOX_FUNC_SB_PSF_HCX:
    case BOX_FUNC_SB_PSF_HCB:
      ASSERT (BoxInst < mSBReg_PSF_HCX_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_PSF_HCX_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_PSF_HCX_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    case BOX_FUNC_SB_PRVT_PCIE_G5:
      ASSERT (BoxInst < mSBReg_PCIE_GEN5_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_PCIE_GEN5_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_PCIE_GEN5_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    case BOX_FUNC_SB_PRVT_PCIE_G4:
      ASSERT (BoxInst < mSBReg_PCIE_GEN4_PortID_FID_SPR_Tbl_length);
      if (BoxInst < mSBReg_PCIE_GEN4_PortID_FID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_PCIE_GEN4_PortID_FID_SPR_Tbl[BoxInst];
      }
      break;

    default:
      DEBUG ((DEBUG_ERROR, "[SB Access] Invalid funcblk: 0x%x \n", RegOffset.Bits.funcblk));
      break;
    }

  } else if (RegOffset.Bits.boxtype == BOX_SB_FBLP) {
    switch (RegOffset.Bits.funcblk) {
    case BOX_FUNC_SB_FBLP:                                      //func blk is FBLP
      if (BoxInst < mSBReg_FBLP_PortID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_FBLP_PortID_SPR_Tbl[BoxInst];
        ASSERT (SB_REG_PORT_ID_INVALID != SbRegPortId);
      } else {
        DEBUG ((DEBUG_ERROR, "[SB Access] Invalid BoxInst %d for funcblk %d in RegOffset %X\n",
                BoxInst, RegOffset.Bits.funcblk, RegOffset.Data));
        ASSERT (BoxInst < mSBReg_FBLP_PortID_SPR_Tbl_length);
      }
      break;

    case BOX_FUNC_SB_FBLP_PRVT:                                 //func blk is FBLP_PRVT
      if (BoxInst < mSBReg_FBLP_PRVT_PortID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_FBLP_PRVT_PortID_SPR_Tbl[BoxInst];
      } else {
        DEBUG ((DEBUG_ERROR, "[SB Access] Invalid BoxInst %d for funcblk %d in RegOffset %X\n",
                BoxInst, RegOffset.Bits.funcblk, RegOffset.Data));
        ASSERT (BoxInst < mSBReg_FBLP_PRVT_PortID_SPR_Tbl_length);
      }
      break;
    case BOX_FUNC_SB_CXL_AM:
      if (BoxInst < mSBReg_CXL_AM_PortID_SPR_Tbl_length) {
        SbRegPortId = (UINT32) mSBReg_CXL_AM_PortID_SPR_Tbl[BoxInst];
      } else {
        DEBUG ((DEBUG_ERROR, "[SB Access] Invalid BoxInst %d for funcblk %d in RegOffset %X\n",
                BoxInst, RegOffset.Bits.funcblk, RegOffset.Data));
        ASSERT (BoxInst < mSBReg_CXL_AM_PortID_SPR_Tbl_length);
      }
      break;
    default:
      DEBUG ((DEBUG_ERROR, "[SB Access] Invalid funcblk: 0x%x \n", RegOffset.Bits.funcblk));
      break;
    }
  } else {

    DEBUG ((DEBUG_ERROR, "[SB Access] Invalid boxtype: 0x%x \n", RegOffset.Bits.boxtype));
  }
  return SbRegPortId;
}

