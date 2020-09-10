/** @file
  ASL implementation of address translation DSM.

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
  "AddressTranslation.aml", // Output Filename
  "SSDT",                   // Signature
  2,                        // SSDT Revision
  "INTEL",                  // OEM ID
  "ADDRXLAT",               // OEM Table ID
  0x0001                    // OEM Revision
  )
{
  Scope (\_SB) {

    //
    // Address translation pseudo-device.
    //
    Device (ADXL) {
      Name (_HID, "INTL0000") // vendor-specific

      Method (_STA, 0) {
        Return (0x0B)         // hide the device in the UI
      }

      //
      // The address and length of the operation region are patched at runtime.
      // They need to be dword-sized here; if zero is used, the ASL compiler
      // may encode them as a type too small to fit the patched values.
      //
      OperationRegion (BUFF, SystemMemory, 0xA5A5A5A5, 0x5A5A5A5A)
      Field (BUFF, AnyAcc, NoLock, Preserve) {
        SWSM, 32,
        CMD,  32,
        STS,  32,

        SYSA, 64, // "SystemAddress"
        NMSA, 64, // "NmSystemAddress"
        SPSA, 64, // "SpareSystemAddress"
        DPA,  64, // "DevicePhysicalAddress"
        PSKT, 64, // "ProcessorSocketId"
        MCID, 64, // "MemoryControllerId"
        NMID, 64, // "NmMemoryControllerId"
        TID,  64, // "TargetId"
        LCID, 64, // "LogicalChannelId"
        CA,   64, // "ChannelAddress"
        NMCA, 64, // "NmChannelAddress"
        CID,  64, // "ChannelId"
        NCID, 64, // "NmChannelId"
        RA,   64, // "RankAddress"
        NMRA, 64, // "NmRankAddress"
        PRID, 64, // "PhysicalRankId"
        NPRI, 64, // "NmPhysicalRankId"
        SLOT, 64, // "DimmSlotId"
        NMSL, 64, // "NmDimmSlotId"
        DRID, 64, // "DimmRankId"
        ROW,  64, // "Row"
        NROW, 64, // "NmRow"
        COL,  64, // "Column"
        NCOL, 64, // "NmCol"
        BANK, 64, // "Bank"
        NBNK, 64, // "NmBank"
        BG,   64, // "BankGroup"
        NMBG, 64, // "NmBankGroup"
        LSR,  64, // "LockStepRank"
        LSPR, 64, // "LockStepPhysicalRank"
        LSB,  64, // "LockStepBank"
        LSBG, 64, // "LockStepBankGroup"
        CS,   64, // "ChipSelect"
        NMCS, 64, // "NmChipSelect"
        NODE, 64, // "Node"
        CHIP, 64, // "ChipId"
        NCHP, 64  // "NmChipId"
      }

      Name (PRMS, Package () {
        "SystemAddress",
        "NmSystemAddress",
        "SpareSystemAddress",
        "DevicePhysicalAddress",
        "ProcessorSocketId",
        "MemoryControllerId",
        "NmMemoryControllerId",
        "TargetId",
        "LogicalChannelId",
        "ChannelAddress",
        "NmChannelAddress",
        "ChannelId",
        "NmChannelId",
        "RankAddress",
        "NmRankAddress",
        "PhysicalRankId",
        "NmPhysicalRankId",
        "DimmSlotId",
        "NmDimmSlotId",
        "DimmRankId",
        "Row",
        "NmRow",
        "Column",
        "NmCol",
        "Bank",
        "NmBank",
        "BankGroup",
        "NmBankGroup",
        "LockStepRank",
        "LockStepPhysicalRank",
        "LockStepBank",
        "LockStepBankGroup",
        "ChipSelect",
        "NmChipSelect",
        "Node",
        "ChipId",
        "NmChipId"
      })

      //
      // Return values
      //
      Name (SUCC, Zero) // success
      Name (UNKN, One)  // unknown failure
      Name (INVC, 2)    // invalid command
      Name (INTE, 3)    // internal error

      //
      // Operation region for SMI port access.
      //
      OperationRegion (SMIP, SystemIO, 0xB2, 1)
      Field (SMIP, ByteAcc, NoLock, Preserve) {
        IOB2, 8
      }

      //
      // _DSM - Device Specific Method
      //
      // Arg0   Function UUID
      // Arg1   Revision ID
      // Arg2   Function index
      // Arg3   Package containing function-specific parameters
      //
      Method (_DSM, 4, Serialized, 0, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj}) {
        If (LAnd (
          LEqual (Arg0, ToUUID("AA3C050A-7EA4-4C1F-AFDA-1267DFD3D48D")),
          LEQual (Arg1, One)
          )) {

          Switch (ToInteger (Arg2)) {

            //
            // Function Index 0 - Get Function Bitmap
            //
            case (0) {
              Return (Buffer () {0xF})  // Function Indexes 0-3 supported
            }

            //
            // Function Index 1 - Get Address Parameters
            //
            case (1) {
              Return (Package () {SUCC, PRMS})
            }

            //
            // Function Index 2 - Forward Translate
            //
            case (2) {
              If (Not (LEqual (SizeOf (Arg3), 1))) {
                Return (Package () {UNKN, Package (37) {}})
              }
              Store (2, CMD)
              Store (DerefOf (Index (Arg3, 0)),  SYSA)  // "SystemAddress"
              Store (SWSM, IOB2)
              Return (Package () {
                STS,
                Package () {
                  SYSA,
                  NMSA,
                  SPSA,
                  DPA,
                  PSKT,
                  MCID,
                  NMID,
                  TID,
                  LCID,
                  CA,
                  NMCA,
                  CID,
                  NCID,
                  RA,
                  NMRA,
                  PRID,
                  NPRI,
                  SLOT,
                  NMSL,
                  DRID,
                  ROW,
                  NROW,
                  COL,
                  NCOL,
                  BANK,
                  NBNK,
                  BG,
                  NMBG,
                  LSR,
                  LSPR,
                  LSB,
                  LSBG,
                  CS,
                  NMCS,
                  NODE,
                  CHIP,
                  NCHP,
                }
              })
            }

            //
            // Function Index 3 - Reverse Translate
            //
            case (3) {
              If (Not (LEqual (SizeOf (Arg3), 37))) {
                Return (Package () {UNKN, Package (37) {}})
              }
              Store (3, CMD)
              Store (DerefOf (Index (Arg3, 0)),  SYSA)  // "SystemAddress"
              Store (DerefOf (Index (Arg3, 1)),  NMSA)  // "NmSystemAddress"
              Store (DerefOf (Index (Arg3, 2)),  SPSA)  // "SpareSystemAddress"
              Store (DerefOf (Index (Arg3, 3)),  DPA)   // "DevicePhysicalAddress"
              Store (DerefOf (Index (Arg3, 4)),  PSKT)  // "ProcessorSocketId"
              Store (DerefOf (Index (Arg3, 5)),  MCID)  // "MemoryControllerId"
              Store (DerefOf (Index (Arg3, 6)),  NMID)  // "NmMemoryControllerId"
              Store (DerefOf (Index (Arg3, 7)),  TID)   // "TargetId"
              Store (DerefOf (Index (Arg3, 8)),  LCID)  // "LogicalChannelId"
              Store (DerefOf (Index (Arg3, 9)),  CA)    // "ChannelAddress"
              Store (DerefOf (Index (Arg3, 10)), NMCA)  // "NmChannelAddress"
              Store (DerefOf (Index (Arg3, 11)), CID)   // "ChannelId"
              Store (DerefOf (Index (Arg3, 12)), NCID)  // "NmChannelId"
              Store (DerefOf (Index (Arg3, 13)), RA)    // "RankAddress"
              Store (DerefOf (Index (Arg3, 14)), NMRA)  // "NmRankAddress"
              Store (DerefOf (Index (Arg3, 15)), PRID)  // "PhysicalRankId"
              Store (DerefOf (Index (Arg3, 16)), NPRI)  // "NmPhysicalRankId"
              Store (DerefOf (Index (Arg3, 17)), SLOT)  // "DimmSlotId"
              Store (DerefOf (Index (Arg3, 18)), NMSL)  // "NmDimmSlotId"
              Store (DerefOf (Index (Arg3, 19)), DRID)  // "DimmRankId"
              Store (DerefOf (Index (Arg3, 20)), ROW)   // "Row"
              Store (DerefOf (Index (Arg3, 21)), NROW)  // "NmRow"
              Store (DerefOf (Index (Arg3, 22)), COL)   // "Column"
              Store (DerefOf (Index (Arg3, 23)), NCOL)  // "NmCol"
              Store (DerefOf (Index (Arg3, 24)), BANK)  // "Bank"
              Store (DerefOf (Index (Arg3, 25)), NBNK)  // "NmBank"
              Store (DerefOf (Index (Arg3, 26)), BG)    // "BankGroup"
              Store (DerefOf (Index (Arg3, 27)), NMBG)  // "NmBankGroup"
              Store (DerefOf (Index (Arg3, 28)), LSR)   // "LockStepRank"
              Store (DerefOf (Index (Arg3, 29)), LSPR)  // "LockStepPhysicalRank"
              Store (DerefOf (Index (Arg3, 30)), LSB)   // "LockStepBank"
              Store (DerefOf (Index (Arg3, 31)), LSBG)  // "LockStepBankGroup"
              Store (DerefOf (Index (Arg3, 32)), CS)    // "ChipSelect"
              Store (DerefOf (Index (Arg3, 33)), NMCS)  // "NmChipSelect"
              Store (DerefOf (Index (Arg3, 34)), NODE)  // "Node"
              Store (DerefOf (Index (Arg3, 35)), CHIP)  // "ChipId"
              Store (DerefOf (Index (Arg3, 36)), NCHP)  // "NmChipId"
              Store (SWSM, IOB2)
              Return (Package () {
                STS,
                Package () {
                  SYSA,
                  NMSA,
                  SPSA,
                  DPA,
                  PSKT,
                  MCID,
                  NMID,
                  TID,
                  LCID,
                  CA,
                  NMCA,
                  CID,
                  NCID,
                  RA,
                  NMRA,
                  PRID,
                  NPRI,
                  SLOT,
                  NMSL,
                  DRID,
                  ROW,
                  NROW,
                  COL,
                  NCOL,
                  BANK,
                  NBNK,
                  BG,
                  NMBG,
                  LSR,
                  LSPR,
                  LSB,
                  LSBG,
                  CS,
                  NMCS,
                  NODE,
                  CHIP,
                  NCHP,
                }
              })
            }

            default {
              BreakPoint
              Return (Package () {INVC, Package (37) {}})
            }
          }
        } else {
          BreakPoint
        }

        Return (Buffer () {Zero}) // no functions available
      }
    }
  }
}
