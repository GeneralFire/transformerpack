/** @file
  ACPI DSDT table

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

//
// @NOTE: This should be read from hardware to reflect
// real PCI exress base. Currently we do not have such
// capability so hardcode is used instead.
//
#define PCI_EXPRESS_BASE 0x80000000

Scope(\_SB.PC00) {

  //
  // PCI-specific method's GUID
  //
  Name(PCIG, ToUUID("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))
  //
  // PCI's _DSM - an attempt at modular _DSM implementation
  // When writing your own _DSM function that needs to include PCI-specific methods, do this:
  //
  // Method(_YOUR_DSM,4){
  //   if(Lequal(Arg0,PCIG)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }
  //   ...continue your _DSM by checking different GUIDs...
  //   else { return(0) }
  // }
  //
  Method(PCID, 4, Serialized) {
    If(LEqual(Arg0, PCIG)) {         // PCIE capabilities UUID
      If(LGreaterEqual(Arg1,3)) {                                              // revision at least 3
        If(LEqual(Arg2,0)) { Return (Buffer(2){0x01,0x03}) }                   // function 0: list of supported functions
        If(LEqual(Arg2,8)) { Return (1) }                                      // function 8: Avoiding Power-On Reset Delay Duplication on Sx Resume
        If(LEqual(Arg2,9)) { Return (Package(5){50000,Ones,Ones,50000,Ones}) } // function 9: Specifying Device Readiness Durations
      }
    }
    return (Buffer(1){0})
  }

  Method(PCIC,1,Serialized) {
#ifndef CDF_SC_FLAG
    If(LEqual(ECR1,1)) {
      If(LEqual(Arg0, PCIG)) {
        return (1)
      }
    }
#else
// TODO: C-S Synch
#endif //CDF_SC_FLAG
    return (0)
  }
#ifdef PCH_ADL
  Method (PC2M, 1, Serialized) {  // Convert _ADR to PCI MMIO address.
    Store (\_SB.PC00.GPCB(), Local0)
    Return (Local0)
  }
#endif // PCH_ADL
  //
  // Get the PCI express base address.
  // Currently hardcoded.
  //
  Method (GPCB) {
    Return (PCI_EXPRESS_BASE)
  }
}

