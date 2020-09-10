//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
   AMI_FV_DIGEST_PPI definition
*/
#ifndef __AMI_FV_DIGEST_PPI__H__
#define __AMI_FV_DIGEST_PPI__H__

//
// TDXE - Trusted DXE
//
typedef enum {AMI_TDXE_SHA1, AMI_TDXE_SHA256, AMI_TDXE_SHA384, AMI_TDXE_SHA512, AMI_TDXE_MAX} AMI_TDXE_HASH_ALG;
typedef UINT16 AMI_TDXE_ALG_ID;

// 565DBCBC-814D-4d68-AA43-9F838EBF5DD3
#define AMI_FV_DIGEST_PPI_GUID \
  { \
    0x565dbcbc, 0x814d, 0x4d68, {0xaa, 0x43, 0x9f, 0x83, 0x8e, 0xbf, 0x5d, 0xd3 } \
  } 
typedef struct _AMI_FV_DIGEST_PPI AMI_FV_DIGEST_PPI;

struct _AMI_FV_DIGEST_PPI {
  UINTN                HashElemCnt;
  UINTN                *AddrList; // List of mem ranges used to calculate final FvHash digest
  UINTN                *LenList;
  UINTN                HashSize;
  AMI_TDXE_ALG_ID      HashAlgId;
  UINT8                FvHash[64]; // max digest size SHA512
};

#endif
