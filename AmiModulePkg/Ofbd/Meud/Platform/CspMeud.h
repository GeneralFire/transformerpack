//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#ifndef _EFI_CSP_MEUD_H_
#define _EFI_CSP_MEUD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <Token.h>

#define FLASH_VALID_SIGNATURE 0x0FF0A55A

#if defined(CSP_MEUD_SPS5_PLATFORM) && (CSP_MEUD_SPS5_PLATFORM == 0)
#define CSP_MEUD_PSF1_PCR_T0_SHDW_HECI1_REG_BASE R_CNL_PCH_H_PSF1_PCR_T0_SHDW_HECI1_REG_BASE
#endif

#define SPS_CLIENTID_BIOS 0x00
#define SPS_CLIENTID_ME_MKHI 0x07

#define I40E_SR_SIZE_IN_WORDS			            (0x10000 >> 0x01) //64K
#define I40E_SR_VPD_MODULE_MAX_SIZE_IN_WORDS        (0x400 >> 0x01) //1K
#define I40E_SR_PCIE_ALT_MODULE_MAX_SIZE_IN_WORDS   (0x400 >> 0x01) //1K
#define I40E_SR_PCIE_ALT_MODULE_WORD_OFFSET         (I40E_SR_SIZE_IN_WORDS - I40E_SR_PCIE_ALT_MODULE_MAX_SIZE_IN_WORDS)
#define I40E_SR_SW_CHECKSUM_WORD_OFFSET             0x3F
#define I40E_SR_SW_CHECKSUM_BASE                    0xBABA

VOID InitializeSpiBase( VOID );

#pragma pack(1)
typedef struct
{
    UINT16  Header;
    UINT16  MacAddress[3];
}PF_MAC_ADDRESS_STRUCTURE;
#pragma pack()

typedef enum
{
    UpdateNonMeRegion,
    UpdateFtprToOpr,
    UpdateMfsb,
    UpdateFactoryData,
    UpdateOpr
}CSP_MEUD_UPDATE_STEP;

typedef enum
{
    Save10GbeMac,
    Restore10GbeMac
}BEHAVIOR_OF_10GBE_MAC;

typedef
VOID
(ACCESS_10GBE)(
    IN  VOID    *Source,
    IN  VOID    *Destination,
    IN  UINTN   Length,
    IN  BOOLEAN Restore );

EFI_STATUS
Process10GbeMac(
    IN  VOID    *Region10Gbe,
    IN  UINT8   Behavior
);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
