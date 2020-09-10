/** @file
  Interface of S3 and address decode data  library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef __MEM_RAS_S3_SAVE_LIB_H__
#define __MEM_RAS_S3_SAVE_LIB_H__

#include <SiliconSetting.h>
#include <Library/PlatMemTopologyLib.h>

#include <Memory/RasNvramStruct.h>

/**

  Saves Patrol Scrub settings that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number

  @retval None.

**/
VOID
EFIAPI
MemS3SavePatrolScrub (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                 Socket,
  IN UINT8                 Imc
  );

/**

  Saves Leaky Bucket settings that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveLeakyBucket (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                Socket,
  IN UINT8                Imc
  );

/**

  Saves Retry read register values per channel to be restored at end of S3 operation

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveRetryRd (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                Socket,
  IN UINT8                Imc,
  IN UINT8                SktCh
  );

/**

  Saves Per Rank Error Setting that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SavePerRankErrorSetting (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                Socket,
  IN UINT8                Imc,
  IN UINT8                SktCh
  );

/**

  Saves Link Fail Setting that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveLinkFailSetting (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                Socket,
  IN UINT8                Imc,
  IN UINT8                SktCh
  );


/**

  Saves  Leaky Bucket 2ND Setting that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveLeakyBucket2nd (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                Socket,
  IN UINT8                Imc,
  IN UINT8                SktCh
  );

/**

  Saves SMI Setting that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveSmiSetting (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                Socket,
  IN UINT8                Imc,
  IN UINT8                SktCh
  );

#endif
