/** @file

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

#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Guid/IioIpInterface.h>
#include <Library/BaseLib.h>
#include <Upi/KtiSi.h>

/**
  Write to ACS register

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param AcsCtrl       - Value to be written to ACS Register

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioWriteACS (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT16                      AcsCtrl
)
{
#if !defined(TNR_HOST)
  UsraCsrWrite (SocId, CtrId, ACSCTRL_IIO_PCIE_REG, AcsCtrl);
#endif
  return EFI_SUCCESS;
}

/**
  Set MC Base Address and MC Index Position

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param McastIndxPos         - MultiCast Index
  @param McastBaseAddress     - MultiCast Base Adress

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioSetMcBaseAdressAndIndexPosition (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT8                       McastIndxPos,
IN  EFI_PHYSICAL_ADDRESS        McastBaseAddress
)
{
#if !defined(TNR_HOST)
  MCAST_BASE_N0_IIO_PCIE_STRUCT          McastBaseN0;
  MCAST_BASE_N1_IIO_PCIE_STRUCT          McastBaseN1;


  McastBaseN0.Bits.mc_index_position = McastIndxPos;
  McastBaseN0.Bits.mc_base_address = (UINT32) (McastBaseAddress>>12);
  UsraCsrWrite (SocId, CtrId, MCAST_BASE_N0_IIO_PCIE_REG, McastBaseN0.Data);

  McastBaseN1.Bits.mc_base_address = (UINT32) (McastBaseAddress >> 32);
  UsraCsrWrite (SocId, CtrId, MCAST_BASE_N1_IIO_PCIE_REG, McastBaseN1.Data);
#endif
  return EFI_SUCCESS;
}

/**
  Clear MCAST block all

  @param SocId                - Socket ID
  @param CtrId                - Controller ID

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioClearMcBlockAll (
IN  UINT8                       SocId,
IN  UINT8                       CtrId
)
{
#if !defined(TNR_HOST)
  MCAST_BLK_ALL_N0_IIO_PCIE_STRUCT       McastBlkAllN0;

  McastBlkAllN0.Data = UsraCsrRead (SocId, CtrId, MCAST_BLK_ALL_N0_IIO_PCIE_REG);
  McastBlkAllN0.Bits.mc_blk_all = 0;
  UsraCsrWrite (SocId, CtrId, MCAST_BLK_ALL_N0_IIO_PCIE_REG, McastBlkAllN0.Data);
#endif
  return EFI_SUCCESS;
}

/**
  Set MC Number of groups MC_NUM_GROUP and MC_ENABLE

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param McastNumGrp          - MultiCast number of groups

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioSetMcNumberOfGroupsAndMcEnable (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT8                       McastNumGrp
)
{
#if !defined(TNR_HOST)
  MCAST_CTRL_IIO_PCIE_STRUCT             McastCtrl;

  McastCtrl.Bits.mc_num_group = (McastNumGrp == 0) ? (McastNumGrp) : (McastNumGrp -1);
  McastCtrl.Bits.mc_enable = 1;
  UsraCsrWrite (SocId, CtrId, MCAST_CTRL_IIO_PCIE_REG, McastCtrl.Data);
#endif

  return EFI_SUCCESS;
}

/**
  MMCAST Receive should be set for NTB

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param McastNumGrp          - MultiCast number of groups

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioSetMmCastForNtb (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT8                       McastNumGrp
)
{
#if !defined(TNR_HOST)
  UINT32                        Data32 = 0;

  for( UINT8 i = 0 ; i < McastNumGrp; i++ ) {
    Data32 |= 1 << i;
  }
  UsraCsrWrite (SocId, CtrId, MCAST_RCV_N0_IIO_PCIE_REG, Data32);
#endif
  return EFI_SUCCESS;
}

/**
  Set RCRBBar and initialize the MemBar for DMI Port --- Don't support it, except SPR

  @param[in]  SocketId             - Socket Id
  @param[in]  DmiRcrbBarAddr       - DMI RCRB Bar Address
  @param[in]  MemResBase           - The start address of available MMIO resource
  @param[out] *DmiMemBarBase       - DMI MEMBAR Base in its RCRB region
  @param[out] *DmiMemBarSize       - DMI MEMBAR Size in its RCRB region

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
DmiRcrbInitialize (
IN  UINT8                       SocketId,
IN  UINT32                      DmiRcrbBarAddr,
IN  UINT32                      MemResBase,
OUT UINT32                      *DmiMemBarBase,
OUT UINT32                      *DmiMemBarSize
)
{
  return EFI_SUCCESS;
}

