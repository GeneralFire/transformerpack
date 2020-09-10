/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

// ITSS
// Define the needed ITSS registers used by ASL on Interrupt
// mapping.

#define R_ITSS_PCR_PIRQA_ROUT  0x3100  ///< PIRQA Routing Control register

scope(\_SB){
      // APTIOV_SERVER_OVERRIDE_RC_START : Hard code R_ITSS_PCR_PIRQA_ROUT macro from PchRegsItss.h since the header file in not included in AMI ASL building method
      //OperationRegion(ITSS, SystemMemory, Add(SBRG, Add(ShiftLeft(PID_ITSS, 16), R_ITSS_PCR_PIRQA_ROUT)), 0x208)
      OperationRegion(ITSS, SystemMemory, Add(SBRG, Add(ShiftLeft(0xC4, 16), 0x3100)), 0x208)
      // APTIOV_SERVER_OVERRIDE_RC_END : Hard code R_ITSS_PCR_PIRQA_ROUT macro from PchRegsItss.h since the header file in not included in AMI ASL building method
      Field(ITSS, ByteAcc, NoLock, Preserve)
      {
        PARC, 8,
        PBRC, 8,
        PCRC, 8,
        PDRC, 8,
        PERC, 8,
        PFRC, 8,
        PGRC, 8,
        PHRC, 8,
       Offset(0x200),   // Offset 3300h ITSSPRC - ITSS Power Reduction Control
            , 1,
            , 1,
        SCGE, 1,        // ITSSPRC[2]: 8254 Static Clock Gating Enable (8254CGE)

      }
}


