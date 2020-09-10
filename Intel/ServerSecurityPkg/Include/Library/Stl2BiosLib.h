/**@file
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

#define STL_2_BIOS_FUSA_SAF_DATA_VARIABLE_NAME L"Stl2BiosFusaSafVariable"

extern EFI_GUID             gStl2BiosFusaSafUefiVariableGuid;

#pragma pack(1)
typedef struct {
  // field containing address of Saf image test pattern
  UINT64 SafImagePathAddr;
  struct {
    UINT16 Enable  : 1;
    UINT16 ImgLoad : 1;
    UINT16 Rsvd    : 14;
  } Bits;
} STL_2_BIOS_FUSA_SAF_DATA_VARIABLE;
#pragma pack()

EFI_STATUS
SetStl2BiosFusaSafVariable (
  UINT8         *SafSupport,
  UINT8         *EnableSaf,
  UINT64        *DfxPathImageLoad
  );

EFI_STATUS
GetStl2BiosFusaSafVariable (
  OUT STL_2_BIOS_FUSA_SAF_DATA_VARIABLE *Stl2BiosFusaSafVariableData
  );