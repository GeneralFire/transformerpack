/** @file
  NUMA Library Definition Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef _NUMA_LIB_H_
#define _NUMA_LIB_H_

/**
  Determine the lowest APIC ID in NUMA cluster to be used as new BSP.

  This function confirms the BSP is in SNC cluster 0.
  If the BSP is not in SNC cluster 0, the logical processor
  with the lowest APIC ID in cluster 0 will take over as BSP.
  If the BSP is in SNC cluster 0, then no action is taken.
**/
VOID
SelectNumaBsp (
  VOID
  );

/**
  Determine the NUMA domain for use in the ACPI SRAT.

  Determine the NUMA domain used in the SRAT for the processor specified
  via the input parameters.

  @param[in]    ApicId  Processor's APIC ID.
  @param[in]    Socket  Processor's socket number.

  @return   NUMA cluster
**/
UINT32
GetCpuNumaDomain (
  IN UINT32 ApicId,
  IN UINT32 Socket
  );

#endif // _NUMA_LIB_H_
