/** @file
  Provides data structure information used by ServerSecurity features in literally all products
  Header is flat and injected directly in SecurityPolicy sructuture and SOCKET_PROCESSORCORE_CONFIGURATION.

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

  // Header is flat and injected directly in SecurityPolicy sructuture and SOCKET_PROCESSORCORE_CONFIGURATION.
  // Put common definitons here either directly or via intermediate header file..

// SECURITY_IP_MKTME_1V0  MkTme;
#include "SecurityIp/SecurityIpMkTme1v0_Inputs.h"
#include "SecurityIp/SecurityIpMkTme1v0_Outputs.h"
// SECURITY_IP_SGXTEM_1V0 SgxTem;
#include "SecurityIp/SecurityIpSgxTem1v0_Inputs.h"
#include "SecurityIp/SecurityIpSgxTem1v0_Outputs.h"
// SECURITY_IP_TDX_1V0    Tdx;
#include "SecurityIp/SecurityIpTdx1v0_Inputs.h"
#include "SecurityIp/SecurityIpTdx1v0_Outputs.h"