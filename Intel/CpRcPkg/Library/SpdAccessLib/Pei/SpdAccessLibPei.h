/** @file
  Header file for the PEI implementation of the SpdAccessLib.

  This header file provides the definition of a structure used with the PEI
  implementation of the SpdAccessLib.  The structure is used to store relevant
  data needed by the implementation for its operation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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


#ifndef _SPD_ACCESS_LIB_PEI_H_
#define _SPD_ACCESS_LIB_PEI_H_

//
// SPD Cache Data Structure
//
#pragma pack (push,1)
typedef struct {
  UINT16        Offset;
  UINT8         Data;
} SPD_CACHE_DATA;
#pragma pack (pop)

#define INVALID_SPD_OFFSET   0xFFFF

//
// SPD Access structure used to store SPD related information
//
#pragma pack (push,1)
typedef struct {
  BOOLEAN               FirstPass[MAX_SOCKET];                     // Used to indicate the first pass through the initialize SPD call
  BOOLEAN               DimmDetectDone;                            // Used to indicate to the library that DIMM detection is done
  BOOLEAN               DimmPresent[MAX_SOCKET][MAX_CH][MAX_DIMM]; // Stores DIMM presence information
  BOOLEAN               Ddr5Caching;                               // Stores if DDR5 Page caching is allowed
  UINT32                Seg0Ddr5PageCache;                         // Stores DDR5 cached page for up to 8 DIMMS (4 bits each) on SMBUS Instance 0
  UINT32                Seg1Ddr5PageCache;                         // Stores DDR5 cacehd page for up to 8 DIMMS (4 bits each) on SMBUS Instance 1
  UINT16                PageSize;                                  // Stores page size for the DDR type present
  UINT16                MaxOffset;                                 // Stores maximum SPD byte offset for the DDR type present
  DDR_TECHNOLOGY_TYPE   DdrType;                                   // Stores type of DDR Technology present in system
  SPD_CACHE_DATA        CacheLastSpdByte[MAX_SOCKET][MAX_CH][MAX_DIMM]; // Stores the Last Spd Byte Data
} SPD_ACCESS_DATA;
#pragma pack (pop)

#endif // _SPD_ACCESS_LIB_PEI_H_

