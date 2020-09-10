/** @file
    Register definitions for McSmb device.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification
**/


#ifndef _MCSMB_REGS_H_
#define _MCSMB_REGS_H_

#define R_PCH_MCSMB_PCR_SPGT                                                 0x214  // SMBus PHY Global Timing Register
#define B_PCH_MCSMB_PCR_SPGT_SPD                                        0xC0000000  //> SMBus Speed
#define N_PCH_MCSMB_PCR_SPGT_SPD                                                30
#define V_PCH_MCSMB_PCR_SPGT_SPD_STANDARD0                                       0  //>  80 kHz
#define V_PCH_MCSMB_PCR_SPGT_SPD_STANDARD1                                       1  //> 100 kHz
#define V_PCH_MCSMB_PCR_SPGT_SPD_FAST                                            2  //> 400 kHz
#define V_PCH_MCSMB_PCR_SPGT_SPD_FASTPLUS                                        3  //>   1 MHz

#define B_PCH_MCSMB_PCR_SPGT_THDDAT                                        0xF0000  //> T-Hold Data
#define N_PCH_MCSMB_PCR_SPGT_THDDAT                                             16
#define B_PCH_MCSMB_PCR_SPGT_TSUDAT                                          0xF00  //> T-Setup Data
#define N_PCH_MCSMB_PCR_SPGT_TSUDAT                                              8
#define B_PCH_MCSMB_PCR_SPGT_DG                                               0xFF  //> De-Glitch
#define N_PCH_MCSMB_PCR_SPGT_DG                                                  0
#define V_PCH_MCSMB_PCR_SPGT_DG_PULSE_370NS                                   0x25  //> 370ns De-Glich pulse

#define R_PCH_MCSMB_PCR_SPMT                                                 0x218  // SMBus PHY Master Timing Register
#define B_PCH_MCSMB_PCR_SPMT_THIGH                                      0xFF000000  //> Time high
#define N_PCH_MCSMB_PCR_SPMT_THIGH                                              24
#define B_PCH_MCSMB_PCR_SPMT_TLOW                                         0xFF0000  //> Time low
#define N_PCH_MCSMB_PCR_SPMT_TLOW                                               16

#define B_PCH_MCSMB_PCR_SPMT_THDSTA                                         0xF000  //> Thdsta - Hold time after (Repeated) Start Condition
#define N_PCH_MCSMB_PCR_SPMT_THDSTA                                             12
#define B_PCH_MCSMB_PCR_SPMT_TSUSTA                                          0xF00  //> Tsusta - Repeated Start Condition setup time
#define N_PCH_MCSMB_PCR_SPMT_TSUSTA                                              8
#define B_PCH_MCSMB_PCR_SPMT_TBUF                                             0xF0  //> Tbuf   - Bus free time between Stop and Start Condition
#define N_PCH_MCSMB_PCR_SPMT_TBUF                                                4
#define B_PCH_MCSMB_PCR_SPMT_TSUSTO                                            0xF  //> Tsusto - Stop Condition setup time
#define N_PCH_MCSMB_PCR_SPMT_TSUSTO                                              0
#define V_PCH_MCSMB_PCR_SPMT_TIMING_70NS                                       0x7  //> +70ns

#endif
