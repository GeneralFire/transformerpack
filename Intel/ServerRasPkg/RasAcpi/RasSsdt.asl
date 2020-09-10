/** @file
  ASL implementation for PCIe handshake between OS and firmware.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

DefinitionBlock (
  "RasSsdt.aml",            // Output Filename
  "SSDT",                   // Signature
  2,                        // SSDT Revision
  "INTEL",                  // OEM ID
  "RAS_ACPI",               // OEM Table ID
  0x0001                    // OEM Revision
  )
{

  Scope (\_SB) {
    //
    // The address and length of the operation region are patched at runtime.
    // They need to be dword-sized here; if zero is used, the ASL compiler
    // may encode them as a type too small to fit the patched values.
    // This is RAS ACPI BIOS Parameters
    OperationRegion (RASP, SystemMemory, 0xA5A5A5A5, 0xA5A5A5A5)
    Field (RASP, ByteAcc, NoLock, Preserve) {
      //
      // OS native PCIE AER Support
      //
      OSNP, 16,
      //
      // Present PCIE Root Port Count
      //
      PRPC, 16,
      //
      //IIO Topology - Root Port Info
      //64 bit x 400 = 25600 bits.
      //it could cover 400 root ports.
      //
      RPSX, 25600,
      //
      // EDPC Device Infomation, please refer to RasAcpi.h for details
      //
      EDVD, 8,    // EDPC Infomation Valid Flag
      EDTS, 8,    // EDPC Trigger Segment
      EDTB, 8,    // EDPC Trigger Bus
      EDTD, 8,    // EDPC Trigger Dev
      EDTF, 8,    // EDPC Trigger Function
      EDRS, 8,    // EDPC Root Port Segment
      EDRB, 8,    // EDPC Root Port Bus
      EDRD, 8,    // EDPC Root Port Device
      EDRF, 8,    // EDPC Root Port Function
      EDNT, 8,    // EDPC Notification Type
      LDIR, 64,    // L1 Directory Address
      EMCA, 8,    // EMCA Enable
      WHEN, 8,
      WHEA, 8,
    }

    //
    // Device specific method
    //
    Method (_DSM, 4, Serialized) {
      If (LEqual(Arg0,ToUUID("663E35AF-CC10-41A4-88EA-5470AF055295"))){

        // L1 DIR POINTER
        Switch (ToInteger(Arg2)) {
          //
          //Function 0: Return supported functions, based on revision
          //
          Case(0)
          {
            Switch (ToInteger(Arg1)) {
              Case(0) {
                If (Lequal(EMCA,1))
                {
                  Return ( Buffer() {0x3} )
                }
                Else
                {
                  Return (Buffer() {0})
                }
              }
            }
          }
          //
          // Function 1:
          //
          Case(1) {Return (LDIR) }
          Default { }
        }
      }

      Return (Buffer() {0})
    }

    //
    // Read PCIe Dword via MMIO
    // Arg0 : Offset
    //
    Method (PERD, 1) {
      OperationRegion (PCFG, SystemMemory, Arg0, 0x4)
      Field (PCFG, DWordAcc, Lock, Preserve){
        ACFG, 32 ,
      }
      Return (ACFG)
    }

    //
    //  Write PCIe Dword via MMIO
    //  Arg0 : Offset (Zero based),
    //  Arg1 : Value (Dword)
    //
    Method (PEWD, 2) {
      OperationRegion (PCFG, SystemMemory, Arg0, 0x4)
      Field (PCFG, DWordAcc, Lock, Preserve){
        ACFG, 32 ,
      }
      Store (Arg1, ACFG)
    }

    //
    // Read PCIe Word via MMIO
    // Arg0 : Offset
    //
    Method (PERW, 1) {
      OperationRegion (PCFG, SystemMemory, Arg0, 0x2)
      Field (PCFG, WordAcc, Lock, Preserve){
        ACFG, 16 ,
      }
      Return (ACFG)
    }

    //
    //  Write PCIe Word via MMIO
    //  Arg0 : Offset (Zero based),
    //  Arg1 : Value (Dword)
    //
    Method (PEWW, 2) {
      OperationRegion (PCFG, SystemMemory, Arg0, 0x2)
      Field (PCFG, WordAcc, Lock, Preserve){
        ACFG, 16 ,
      }
      Store (Arg1, ACFG)
    }

    //
    // Read PCIe Byte via MMIO
    // Arg0 : Offset
    //
    Method (PERB, 1) {
      OperationRegion (PCFG, SystemMemory, Arg0, 0x1)
      Field (PCFG, ByteAcc, Lock, Preserve){
        ACFG, 8 ,
      }
      Return (ACFG)
    }

    //
    //  Write PCIe Byte via MMIO
    //  Arg0 : Offset (Zero based),
    //  Arg1 : Value (Dword)
    //
    Method (PEWB, 2) {
      OperationRegion (PCFG, SystemMemory, Arg0, 0x1)
      Field (PCFG, ByteAcc, Lock, Preserve){
        ACFG, 8 ,
      }
      Store (Arg1, ACFG)
    }
    #include "PcieAer.asi"
    #include "PcieEdpc.asi"
    #include "Whea.asi"
  }
}
