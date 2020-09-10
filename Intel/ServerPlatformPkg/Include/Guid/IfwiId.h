/** @file
  GUID and definitions for IFWI ID.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef _IFWI_ID_GUID_H_
#define _IFWI_ID_GUID_H_

#include <Pi/PiHob.h>

#define IFWI_ID_GUID \
  { \
    0x8FFA88FB, 0x4395, 0xF6B4, { 0x05, 0x25, 0x93, 0xFF, 0x27, 0x7A, 0x62, 0x15 } \
  }
extern EFI_GUID gIfwiIdGuid;

//
// IFWI ID string format conforms to "IFWI Revision Identification Specification", Rev. 0.9, 05/30/2015.
//
// IFWI_ID = BoardID.BuildType.ReleaseType.Arc.YYYY.MajorV.MinV.BuildNum.Time.Suffix
//
// Example: "GRRFCRB.X86.OR.64.2015.24.3.99.1200.Suffix"
//
#pragma pack(1)

typedef struct {
  CHAR16    BoardId[7];              // "GRRFCRB"
  CHAR16    Dot1;                    // "."
  CHAR16    BuildType[3];            // "PRE"
  CHAR16    Dot2;                    // "."
  CHAR16    ReleaseType[2];          // "64"
  CHAR16    Dot3;                    // "."
  CHAR16    Arc[2];                  // "64"
  CHAR16    Dot4;                    // "."
  CHAR16    YYYY[4];                 // "YYYY"
  CHAR16    Dot5;                    // "."
  CHAR16    MajorV[2];               //Major version
  CHAR16    Dot6;                     // "."
  CHAR16    MinV;                    //Minor version
  CHAR16    Dot7;                    // "."
  CHAR16    BuildNum[2];             //Build Number
  CHAR16    Dot8;                    // "."
  CHAR16    BuildTime[4];            //"HHMM"
  CHAR16    Dot9;                    // "."
  CHAR16    Suffix[8];               //Suffix
  CHAR16    NullTerminator;          // 0x0000
} IFWI_ID_STRING;

//
// A signature precedes the IFWI ID string in the FV to enable search by external tools.
//
typedef struct {
  UINT8             Signature[8];   // "$IIFWII$"
  IFWI_ID_STRING    IfwiIdString;   // "GRRFCRB.X86.OR.64.2015.24.3.99.1200.Suffix"
} IFWI_ID_IMAGE;

#pragma pack()

typedef struct {
  EFI_HOB_GUID_TYPE GuidType;
  IFWI_ID_IMAGE     IfwiIdImage;
} IFWI_ID_HOB;

#endif

