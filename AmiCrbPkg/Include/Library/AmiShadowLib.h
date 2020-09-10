//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmiShadowLib.h
  This file contains North Bridge chipset porting library shadow 
  functions and data structure definitions for both PEI & DXE stages.
  
*/

#ifndef _AMI_SHADOWLIB_LIB_H_
#define _AMI_SHADOWLIB_LIB_H_

#include "IioUniversalData.h"
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MmPciBaseLib.h>
#include <AmiPcieSegBusLib/AmiPcieSegBusLib.h>
#include <Library/PcdLib.h> 
#include "Token.h"

#include <Ppi/ReadOnlyVariable2.h>
#include <Uefi/UefiSpec.h>

#ifdef __cplusplus
extern "C" {
#endif


//----------------------------------------------------------------------------
// Constant, Macro and Type Definition(s)
//----------------------------------------------------------------------------
// Macro Definition(s)
#define PCI_SAD_ALL_DEVICE_NUMBER                   0x1D 
#define PCI_SAD_ALL_FUNC_NUMBER                     0x00

#define R_PCI_SAD_ALL_PAM0123                       0x40   //PAM0123 Register Offset
#define  B_PAM0_HIENABLE                            BIT4|BIT5   //DRAM RW enable for region 0F0000-0FFFFF
#define R_PCI_SAD_ALL_PAM456                        0x44   //PAM456 Register Offset
#define  B_PAM4_LOENABLE                            BIT0|BIT1   //DRAM RW enable for region 0D8000-0DBFFF
#define  B_PAM4_HIENABLE                            BIT4|BIT5   //DRAM RW enable for region 0DC000-0DFFFF
#define  B_PAM5_LOENABLE                            BIT8|BIT9   //DRAM RW enable for region 0E0000-0E3FFF
#define  B_PAM5_HIENABLE                            BIT12|BIT13   //DRAM RW enable for region 0E4000-0E7FFF
#define  B_PAM6_LOENABLE                            BIT16|BIT17   //DRAM RW enable for region 0E8000-0EBFFF
#define  B_PAM6_HIENABLE                            BIT20|BIT21   //DRAM RW enable for region 0EC000-0EFFFF


//Enable DRAM RW for 0F0000-0FFFFF region specified in PAM0123 register
#define PAM0123_DRAM_RW_ENABLE                      B_PAM0_HIENABLE
//Enable DRAM RW for all regions specified in PAM456 register
#define PAM456_DRAM_RW_ENABLE                       B_PAM4_LOENABLE|B_PAM4_HIENABLE|B_PAM5_LOENABLE|B_PAM5_HIENABLE|B_PAM6_LOENABLE|B_PAM6_HIENABLE


VOID NbRuntimeShadowRamWrite(IN BOOLEAN Enable);

VOID OemRuntimeShadowRamWrite(
    IN BOOLEAN Enable
);

#ifdef __cplusplus
}
#endif
#endif
