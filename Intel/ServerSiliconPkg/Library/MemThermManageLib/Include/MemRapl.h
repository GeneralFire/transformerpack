/** @file
  Interface for Memory RAPL feature

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_RAPL_H_
#define _MEM_RAPL_H_

/**

  This routine will scale the calculated power values to be programmed for DRAM RAPL based upon the looked up values
  based on the refresh rate and a linear scaling factor from looked up values.

  @param[in] Socket         Socket number
  @param[in] Slope          Slope of the Power Input
  @param[in] PowerInput     Unscaled power value

  @retval Scaled power value

**/
UINT16
RefreshScaledPwr (
  IN UINT8    Socket,
  IN UINT32   Slope,
  IN UINT16   PowerInput
  );

/**

  This routine will progam registers for Initializing DRAM RAPL

  @param[in] Socket                      - Socket number
  @param[in] pointer DRamRaplDataStruct  - Structure containing the values to be should be Programmed

  @retval N/A

**/
VOID
DramRaplProg (
  IN UINT8              Socket,
  IN OUT DramRaplDataStruct *RaplData
  );

/**

  This routine will calculate the values to be programmed for DRAM RAPL based upon the looked up values

  @param[in] Socket        - Socket number
  @param[in] Channel            - channel
  @param[in] Dimm          - Dimm number
  @param[in][out] pointer DramRaplDataStruct  - Strucutre containing the values to be should be Programmed
  @param[in][out] pointer DramRaplLUTDataStruct     - Strucutre containing looked up which the calculations will be based upon

  @retval N/A

**/
VOID
DramRaplProgCalc (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN OUT DramRaplDataStruct *RaplData,
  IN OUT DramRaplLUTDataStruct *RaplLutData
  );

/**

  This routine will calculate the values to be programmed for DRAM RAPL based upon the looked up values

  @param[in] Socket        - Socket number
  @param[in] DimmInWeightTable    Indicates dimm is in WTT Table
  @param[in][out] PtrDimmWieghtTable          - Pointer to DimmWT
  @param[in] WeightTableCurrentDimm         Pointer to current weight table
  @param[in][out] pointer DramRaplDataStruct  - structure containing the values to be should be Programmed
  @param[in] Channel  - Current Channel to configure
  @param[in] Dimm     - Current Dimm to program

  @retval N/A

**/
VOID
ConfigureDramRaplFeature (
  IN UINT8 Socket,
  IN BOOLEAN  DimmInWeightTable,
  IN OUT VOID   *PtrDimmWieghtTable,
  IN VOID *WeightTableCurrentDimm,
  IN OUT DramRaplDataStruct *RaplData,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

#endif //#ifndef _MEM_RAPL_H_
