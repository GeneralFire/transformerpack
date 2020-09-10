/** @file Nvdr.asl
  Common implementation of NVDIMM ACPI interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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
**/

DefinitionBlock (
    "Nvdr.aml",
    "SSDT",
    2,
    "INTEL",
    "SSDTNVDR",
    0x1000
)
{
    Scope (\_SB)
    {
        #include "MaxSocket.h"
        #define DSM_ROOT_UUID      "2F10E7A4-9E91-11E4-89D3-123B93F75CBA"
        #define DSM_INPB_SIZE      0x10000  // NVDIMM_MAX_INPUT_BUF_SIZE in NvdimmAcpiSmmInterface.h
        #define DSM_OUTB_SIZE      0x10000  // NVDIMM_MAX_OUTPUT_BUF_SIZE in NvdimmAcpiSmmInterface.h
        #define DSM_INPB_SIZE_BITS 0x80000  // DSM_INPB_SIZE * 8, but must be number, ASL compiler will not evaluate
        #define DSM_OUTB_SIZE_BITS 0x80000  // DSM_OUTB_SIZE * 8, but must be number, ASL compiler will not evaluate
        #define DSM_ARS_STATUS_BUFFER_SIZE_BITS 0x80000

        #define LABEL_STORAGE_SUCCESS       0
        #define LABEL_STORAGE_FAILURE       1
        #define LABEL_STORAGE_INVALID_INPUT 2
        #define LABEL_STORAGE_LOCKED        3
        #define LABEL_STORAGE_HW_FAILURE    4

        #define _NGN_ACPI_MUTEX_ENABLED

        #if defined(_NGN_ACPI_MUTEX_ENABLED)
        #define ACQUIRE(_M, _T) Acquire(_M, _T)
        #define RELEASE(_M)     Release(_M)
        Mutex (NMTX, 0)
        #else
        #define ACQUIRE(_M, _T)
        #define RELEASE(_M)
        #endif


        #define ROOT_STATUS_SUCCESS                  0
        #define ROOT_STATUS_FUNCTION_NOT_SUPPORTED   1
        #define ROOT_STATUS_INVALID_INPUT_PARAMETERS 2
        #define ROOT_STATUS_HARDWARE_ERROR           3
        #define ROOT_STATUS_RETRY_SUGGESTED          4
        #define ROOT_STATUS_UNKNOWN_ERROR            5
        #define ROOT_STATUS_FUNCTION_SPECIFIC        6

        #define INP_ENTRY(_S, _C, _D, _REG, _POS) \
        Offset (0x0 ## _POS ## AA ## _S ## _C ## _D), \
        IN ## _REG, 64,
        #define OUP_ENTRY(_S, _C, _D, _REG, _POS) \
        Offset (0x0 ## _POS ## BB ## _S ## _C ## _D), \
        OU ## _REG, 64,
        #define FLSH_ENTRY(_S, _C, _D, _REG) \
        Offset (0x0 ## _REG ## 83 ## _S ## _C ## _D), \
        FL ## _REG, 64,

        #define DIMM_ID(_S, _C, _D) S ## _S.C ## _C.D ## _D:

        //
        // NVD_ENTRY macro is used to generate NVD entry for each DIMM
        //
        #define  NVD_ENTRY(socket, channel, dimm)                        \
        Device (N##socket##channel##dimm) {                            \
            Name (FXCD, 0) \
            Field (CCTX, QwordAcc, NoLock, Preserve) { \
            Offset (0x01 ## 80 ## socket ## channel ## dimm), \
            Offset (0x02 ## 80 ## socket ## channel ## dimm), \
            Offset (0x03 ## 80 ## socket ## channel ## dimm), \
            Offset (0x04 ## 80 ## socket ## channel ## dimm), \
            Offset (0x05 ## 80 ## socket ## channel ## dimm), \
            Offset (0x06 ## 80 ## socket ## channel ## dimm), \
            Offset (0x07 ## 80 ## socket ## channel ## dimm), \
            Offset (0x08 ## 80 ## socket ## channel ## dimm), \
            XCMD, 64 \
            } \
            Name (FXST, 0) \
            Field (CCTX, QwordAcc, NoLock, Preserve) { \
            Offset (0x01 ## 81 ## socket ## channel ## dimm), \
            Offset (0x02 ## 81 ## socket ## channel ## dimm), \
            Offset (0x03 ## 81 ## socket ## channel ## dimm), \
            Offset (0x04 ## 81 ## socket ## channel ## dimm), \
            Offset (0x05 ## 81 ## socket ## channel ## dimm), \
            Offset (0x06 ## 81 ## socket ## channel ## dimm), \
            Offset (0x07 ## 81 ## socket ## channel ## dimm), \
            Offset (0x08 ## 81 ## socket ## channel ## dimm), \
            XSTA, 64 \
            } \
            Name (FXIN, 0) \
            Field (CCTX, QwordAcc, NoLock, Preserve) { \
            Offset (0x01 ## AA ## socket ## channel ## dimm), \
            Offset (0x02 ## AA ## socket ## channel ## dimm), \
            Offset (0x03 ## AA ## socket ## channel ## dimm), \
            Offset (0x04 ## AA ## socket ## channel ## dimm), \
            Offset (0x05 ## AA ## socket ## channel ## dimm), \
            Offset (0x06 ## AA ## socket ## channel ## dimm), \
            Offset (0x07 ## AA ## socket ## channel ## dimm), \
            INP_ENTRY(socket, channel, dimm, 01, 08) \
            INP_ENTRY(socket, channel, dimm, 02, 09) \
            INP_ENTRY(socket, channel, dimm, 03, 0A) \
            INP_ENTRY(socket, channel, dimm, 04, 0B) \
            INP_ENTRY(socket, channel, dimm, 05, 0C) \
            INP_ENTRY(socket, channel, dimm, 06, 0D) \
            INP_ENTRY(socket, channel, dimm, 07, 0E) \
            INP_ENTRY(socket, channel, dimm, 08, 0F) \
            INP_ENTRY(socket, channel, dimm, 09, 10) \
            INP_ENTRY(socket, channel, dimm, 0A, 11) \
            INP_ENTRY(socket, channel, dimm, 0B, 12) \
            INP_ENTRY(socket, channel, dimm, 0C, 13) \
            INP_ENTRY(socket, channel, dimm, 0D, 14) \
            INP_ENTRY(socket, channel, dimm, 0E, 15) \
            INP_ENTRY(socket, channel, dimm, 0F, 16) \
            INP_ENTRY(socket, channel, dimm, 10, 17) \
            } \
            Name (FXOU, 0) \
            Field (CCTX, QwordAcc, NoLock, Preserve) { \
            Offset (0x01 ## BB ## socket ## channel ## dimm), \
            Offset (0x02 ## BB ## socket ## channel ## dimm), \
            Offset (0x03 ## BB ## socket ## channel ## dimm), \
            Offset (0x04 ## BB ## socket ## channel ## dimm), \
            Offset (0x05 ## BB ## socket ## channel ## dimm), \
            Offset (0x06 ## BB ## socket ## channel ## dimm), \
            Offset (0x07 ## BB ## socket ## channel ## dimm), \
            OUP_ENTRY(socket, channel, dimm, 01, 08) \
            OUP_ENTRY(socket, channel, dimm, 02, 09) \
            OUP_ENTRY(socket, channel, dimm, 03, 0A) \
            OUP_ENTRY(socket, channel, dimm, 04, 0B) \
            OUP_ENTRY(socket, channel, dimm, 05, 0C) \
            OUP_ENTRY(socket, channel, dimm, 06, 0D) \
            OUP_ENTRY(socket, channel, dimm, 07, 0E) \
            OUP_ENTRY(socket, channel, dimm, 08, 0F) \
            OUP_ENTRY(socket, channel, dimm, 09, 10) \
            OUP_ENTRY(socket, channel, dimm, 0A, 11) \
            OUP_ENTRY(socket, channel, dimm, 0B, 12) \
            OUP_ENTRY(socket, channel, dimm, 0C, 13) \
            OUP_ENTRY(socket, channel, dimm, 0D, 14) \
            OUP_ENTRY(socket, channel, dimm, 0E, 15) \
            OUP_ENTRY(socket, channel, dimm, 0F, 16) \
            OUP_ENTRY(socket, channel, dimm, 10, 17) \
            } \
            Name (FXBS, 0) \
            Field (CCTX, QwordAcc, NoLock, Preserve) { \
            Offset (0x01 ## 82 ## socket ## channel ## dimm), \
            Offset (0x02 ## 82 ## socket ## channel ## dimm), \
            Offset (0x03 ## 82 ## socket ## channel ## dimm), \
            Offset (0x04 ## 82 ## socket ## channel ## dimm), \
            Offset (0x05 ## 82 ## socket ## channel ## dimm), \
            Offset (0x06 ## 82 ## socket ## channel ## dimm), \
            Offset (0x07 ## 82 ## socket ## channel ## dimm), \
            Offset (0x08 ## 82 ## socket ## channel ## dimm), \
            XBSR, 64 \
            } \
            Name (FXFH, 0) \
            Field (CFHX, QwordAcc, NoLock, Preserve) { \
            FLSH_ENTRY(socket, channel, dimm, 01) \
            FLSH_ENTRY(socket, channel, dimm, 02) \
            FLSH_ENTRY(socket, channel, dimm, 03) \
            FLSH_ENTRY(socket, channel, dimm, 04) \
            FLSH_ENTRY(socket, channel, dimm, 05) \
            FLSH_ENTRY(socket, channel, dimm, 06) \
            FLSH_ENTRY(socket, channel, dimm, 07) \
            FLSH_ENTRY(socket, channel, dimm, 08) \
            FLSH_ENTRY(socket, channel, dimm, 09) \
            FLSH_ENTRY(socket, channel, dimm, 0A) \
            FLSH_ENTRY(socket, channel, dimm, 0B) \
            FLSH_ENTRY(socket, channel, dimm, 0C) \
            FLSH_ENTRY(socket, channel, dimm, 0D) \
            FLSH_ENTRY(socket, channel, dimm, 0E) \
            FLSH_ENTRY(socket, channel, dimm, 0F) \
            FLSH_ENTRY(socket, channel, dimm, 10) \
            FLSH_ENTRY(socket, channel, dimm, 11) \
            FLSH_ENTRY(socket, channel, dimm, 12) \
            FLSH_ENTRY(socket, channel, dimm, 13) \
            FLSH_ENTRY(socket, channel, dimm, 14) \
            FLSH_ENTRY(socket, channel, dimm, 15) \
            FLSH_ENTRY(socket, channel, dimm, 16) \
            FLSH_ENTRY(socket, channel, dimm, 17) \
            FLSH_ENTRY(socket, channel, dimm, 18) \
            FLSH_ENTRY(socket, channel, dimm, 19) \
            FLSH_ENTRY(socket, channel, dimm, 1A) \
            FLSH_ENTRY(socket, channel, dimm, 1B) \
            FLSH_ENTRY(socket, channel, dimm, 1C) \
            FLSH_ENTRY(socket, channel, dimm, 1D) \
            FLSH_ENTRY(socket, channel, dimm, 1E) \
            FLSH_ENTRY(socket, channel, dimm, 1F) \
            FLSH_ENTRY(socket, channel, dimm, 20) \
            } \
            Name (CENA, 0x8000 ## 0 ## socket ## channel ## dimm ## 59584946) \
            Name (CFIS, 0x8000 ## 1 ## socket ## channel ## dimm ## 57584946) \
            Method (_ADR, 0) {                                           \
            Store (Zero, Local0)                                       \
            if (LEqual (MCCH, 2)) {                                    \
                Store (0x##socket##00##dimm, Local0)                     \
                And (channel, 0x1, Local1)                               \
                ShiftRight (channel, 1, Local2)                          \
                ShiftLeft (Local1, 4, Local1)                            \
                ShiftLeft (Local2, 8, Local2)                            \
                Or (Local0, Local1, Local0)                              \
                Or (Local0, Local2, Local0)                              \
            } elseif (LEqual (MCCH, 3)) {                              \
                Store (0x##socket##0##channel##dimm, Local0)             \
                if (LGreater (channel, 2)) {                             \
                Store (channel, Local1)                                \
                Subtract (Local1, 3, Local1)                           \
                ShiftLeft (Local1, 4, Local1)                          \
                And (Local0, 0xFF0F, Local0)                           \
                Or (Local0, Local1, Local0)                            \
                Or (Local0, 0x100, Local0)                             \
                }                                                        \
            }                                                          \
            Return (Local0)                                            \
            }                                                            \
            Name (NTFY, 0)                                               \
                                                                        \
            DEFINE_CMBC(DIMM_ID(socket, channel, dimm), socket, channel) \
            DEFINE_CR01(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR02(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR03(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR04(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR05(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR06(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR07(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR08(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR09(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR10(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR11(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR17(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR18(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR19(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR20(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR21(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR22(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR23(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR24(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR26(DIMM_ID(socket, channel, dimm), socket, channel) \
            DEFINE_CR27(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CR28(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CLSI(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CLSR(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CLSW(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CNCH(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CNBS(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_CRLD(DIMM_ID(socket, channel, dimm))                  \
            DEFINE_PCDS()                                                \
            DEFINE_PCDR()                                                \
            DEFINE_PCDW()                                                \
                                                                        \
            Method (_DSM, 4)                                                 \
            {                                                            \
                Return (CRLD (Arg0, Arg1, Arg2, Arg3, _ADR))             \
            }                                                            \
            Method (_LSI, 0)                                             \
            {                                                            \
                Return (CLSI ())                                         \
            }                                                            \
            Method (_LSR, 2)                                             \
            {                                                            \
                Return (CLSR (Arg0, Arg1))                               \
            }                                                            \
            Method (_LSW, 3, Serialized)                                 \
            {                                                            \
                Return (CLSW (Arg0, Arg1, Arg2))                         \
            }                                                            \
            Method (_NCH, 0)                                             \
            {                                                            \
                Return (CNCH ())                                         \
            }                                                            \
            Method (_NBS, 0)                                             \
            {                                                            \
                Return (CNBS ())                                         \
            }                                                            \
        }

        //
        // Generates an NVD entry for each DIMM on the particular channel
        // MAX_DIMM = 2
        #define  DIMM_LIST(socket, channel) \
        NVD_ENTRY (socket, channel, 0)    \
        NVD_ENTRY (socket, channel, 1)

        // Generates entries for each channel on the socket
        // MAX_CH = 8
        #define  CHANNEL_LIST(socket) \
        DIMM_LIST (socket, 0)       \
        DIMM_LIST (socket, 1)       \
        DIMM_LIST (socket, 2)       \
        DIMM_LIST (socket, 3)       \
        DIMM_LIST (socket, 4)       \
        DIMM_LIST (socket, 5)       \
        DIMM_LIST (socket, 6)       \
        DIMM_LIST (socket, 7)

        #define NVDIMM_NOTIFY(socket, channel, dimm, _NBM0, _NBM1)         \
        If(LNotEqual(NVDP(socket, channel, dimm, _NBM0, _NBM1), 0x00)) { \
            Increment (NOTC)                                               \
            Store (socket, NOLS)                                           \
            Store (channel, NOLC)                                          \
            Store (dimm, NOLD)                                             \
            Notify(\_SB.NVDR.N##socket##channel##dimm, 0x81)               \
        }

        #define DIMM_NOTIFY_LIST(socket, channel, _NBM0, _NBM1) \
        NVDIMM_NOTIFY (socket, channel, 0, _NBM0, _NBM1)      \
        NVDIMM_NOTIFY (socket, channel, 1, _NBM0, _NBM1)

        #define CHANNEL_NOTIFY_LIST(socket, _NBM0, _NBM1)   \
        DIMM_NOTIFY_LIST (socket, 0, _NBM0, _NBM1)        \
        DIMM_NOTIFY_LIST (socket, 1, _NBM0, _NBM1)        \
        DIMM_NOTIFY_LIST (socket, 2, _NBM0, _NBM1)        \
        DIMM_NOTIFY_LIST (socket, 3, _NBM0, _NBM1)        \
        DIMM_NOTIFY_LIST (socket, 4, _NBM0, _NBM1)        \
        DIMM_NOTIFY_LIST (socket, 5, _NBM0, _NBM1)        \
        DIMM_NOTIFY_LIST (socket, 6, _NBM0, _NBM1)        \
        DIMM_NOTIFY_LIST (socket, 7, _NBM0, _NBM1)


        //
        // This method verifies whether DIMM for which it is called is present in specified bitmap
        // Arg0: socket  0-3
        // Arg1: channel 0-7
        // Arg2: dimm    0-1
        // Arg3: sockets 0-3 bitmask
        // Arg4: sockets 4-7 bitmask
        // DIMM encoding in abitmask is as follows:
        // [0,1] - DIMMs 0,1 in socket 0, channel 0
        // [2,3] - DIMMs 0,1 in socket 0, channel 1
        // ...
        // [10,11] - DIMMs 0,1 in socket 0, channel 5
        // [12..15] - spare for future channels 6,7 if used
        // [16,17] - DIMMs 0,1 in socket 1, channel 0
        //
        Method (NVDP, 5, Serialized)
        {
            Add (Multiply (ToInteger (Arg0), 16), Multiply (ToInteger (Arg1), 2), Local0)
            Add (Local0, ToInteger (Arg2), Local0)
            if (LGreater (ToInteger (Arg0), 3)) {
            ShiftRight (Arg4, Subtract (Local0, 64), Local0)
            } else {
            ShiftRight (Arg3, Local0, Local0)
            }
            if (LEqual (And (Local0, 0x01), 0x00)) {
            Return(0x00)
            } else {
            Return(0x0F)
            }
        }

        //
        // Perform notification of leaf devices, per bitmap in Arg0
        //
        Method (NVLN, 1)
        {
            Store (DeRefOf (Index (Arg0, 0)), Local0)
            Store (DeRefOf (Index (Arg0, 1)), Local1)
            CHANNEL_NOTIFY_LIST (0, Local0, Local1)
        #if MAX_SOCKET > 1
            CHANNEL_NOTIFY_LIST (1, Local0, Local1)
        #endif
        #if MAX_SOCKET > 2
            CHANNEL_NOTIFY_LIST (2, Local0, Local1)
            CHANNEL_NOTIFY_LIST (3, Local0, Local1)
        #endif
        #if MAX_SOCKET > 4
            CHANNEL_NOTIFY_LIST (4, Local0, Local1)
            CHANNEL_NOTIFY_LIST (5, Local0, Local1)
            CHANNEL_NOTIFY_LIST (6, Local0, Local1)
            CHANNEL_NOTIFY_LIST (7, Local0, Local1)
        #endif
        }

        //
        // Perform notification of root device per value in Arg0
        //
        Method (NVRN, 1) {
            if (LNotEqual (Arg0, Zero)) {
            Increment (RNTC)
            Notify(\_SB.NVDR, 0x81)
            }
        }

        //
        // number of control regions here must be MAX_CONTROL_REGIONS + 1,
        // last region is always a dummy control region (captures all unpopulated DIMMs)
        //
        OperationRegion (CCT0, SystemMemory, 0x0000000158584946, 0xdeadbeef00000000)
        OperationRegion (CCT1, SystemMemory, 0x0000010158584946, 0xdeadbeef00000000)
        OperationRegion (CCT2, SystemMemory, 0x0000020158584946, 0xdeadbeef00000000)
        OperationRegion (CCT3, SystemMemory, 0x0000030158584946, 0xdeadbeef00000000)
        OperationRegion (CCT4, SystemMemory, 0x0000040158584946, 0xdeadbeef00000000)
        OperationRegion (CCT5, SystemMemory, 0x0000050158584946, 0xdeadbeef00000000)
        OperationRegion (CCT6, SystemMemory, 0x0000060158584946, 0xdeadbeef00000000)
        OperationRegion (CCT7, SystemMemory, 0x0000070158584946, 0xdeadbeef00000000)
        OperationRegion (CCT8, SystemMemory, 0x0000080158584946, 0xdeadbeef00000000)

        OperationRegion (CFH0, SystemMemory, 0x0000000256584946, 0xdeadbeef00000000)
        OperationRegion (CFH1, SystemMemory, 0x0000010256584946, 0xdeadbeef00000000)
        OperationRegion (CFH2, SystemMemory, 0x0000020256584946, 0xdeadbeef00000000)
        OperationRegion (CFH3, SystemMemory, 0x0000030256584946, 0xdeadbeef00000000)
        OperationRegion (CFH4, SystemMemory, 0x0000040256584946, 0xdeadbeef00000000)
        OperationRegion (CFH5, SystemMemory, 0x0000050256584946, 0xdeadbeef00000000)
        OperationRegion (CFH6, SystemMemory, 0x0000060256584946, 0xdeadbeef00000000)
        OperationRegion (CFH7, SystemMemory, 0x0000070256584946, 0xdeadbeef00000000)
        OperationRegion (CFH8, SystemMemory, 0x0000080256584946, 0xdeadbeef00000000)

        #define CCTX CCT8
        #define CFHX CFH8

        Device (NVDR) {
            Name (_HID, "ACPI0012")
            External (SMIC)
            #include "CrystalRidge.asi"

            Method (_STA) {
            if (LAnd(LEqual(DBM0, 0), LEqual(DBM1, 0))) {
                Return (0x00)
            } else {
                Return (0x0F)
            }
            }

            //
            // NVDIMM root device specific method.
            //
            Method (_DSM, 4)
            {
                Return (CRRD (Arg0, Arg1, Arg2, Arg3))
            }

            //
            // Process leaf notifications pre SWGPE disable
            //
            Method (LPRE, 0)
            {
                Return (CLPR ())
            }

            //
            // Process root notifications pre SWGPE disable
            //
            Method (RPRE, 0)
            {
                Return (CRPR ())
            }

            //
            // Process leaf notifications post SWGPE disable
            //
            Method (LPST, 1)
            {
                Return (CLPT (Arg0))
            }

            //
            // Process root notifications post SWGPE disable
            //
            Method (RPST, 1)
            {
                Return (CRPT (Arg0))
            }

            Method (NTPR, 0, Serialized) {
                Store (LPRE (), Local0)
                Store (RPRE (), Local1)
                Store (Package (2) {}, Local2)
                Store (Local0, Index (Local2, 0))
                Store (Local1, Index (Local2, 1))

                Return (Local2)
            }

            Method (NTDO , 1, Serialized){
                Store (DeRefOf (Index (Arg0, 0)), Local0)
                Store (DeRefOf (Index (Arg0, 1)), Local1)
                NVLN (LPST (Local0))
                NVRN (RPST (Local1))
            }

            // supports Max_Socket = 8
            CHANNEL_LIST (0)
        #if MAX_SOCKET > 1
            CHANNEL_LIST (1)
        #endif
        #if MAX_SOCKET > 2
            CHANNEL_LIST (2)
            CHANNEL_LIST (3)
        #endif
        #if MAX_SOCKET > 4
            CHANNEL_LIST (4)
            CHANNEL_LIST (5)
            CHANNEL_LIST (6)
            CHANNEL_LIST (7)
        #endif
        }

        //
        // IMPORTANT NOTE: When updating this structure please update NVDIMM_ACPI_SMM_INTERFACE in ServerSiliconPkg/Universal/Dxe/CrystalRidge/NvdimmAcpiSmmInterface.h.
        // This structure is shared between ACPI and SMM.
        //
        OperationRegion (CRCM, SystemMemory, 0x41584946, 0x40130) // (FIXA - Patched by ACPI Platform Driver during POST)
        Field (CRCM, ByteAcc, NoLock, Preserve) {
            DBM0   ,    64,    // Device Bitmap for sockets 0 - 3.  Bit position = (socket * 16) + (channel * 2) + dimm
            DBM1   ,    64,    // Device Bitmap for sockets 4 - 7.  Bit position = (socket * 16) + (channel * 2) + dimm - 64

            NBM0   ,    64,
            NBM1   ,    64,

            FUNI   ,    32,    // Function Index
            NFHD   ,    32,    // NFIT Device Handle
            INPL   ,    32,    // Input Length
            OUTL   ,    32,    // Output Length

            NFST   ,    32,    // Status
            DINT   ,     8,    // Debug Interface
            NOTC   ,    16,    // Notification Counter
            NOTP   ,     4,    // Notification Padding
            NOLS   ,     4,    // Last notified socket
            NOLC   ,     4,    // Last notified channel
            NOLD   ,     4,    // Last notified dimm
            NPSN   ,    32,    // Notification's Produced Sequence Number

            NCSN   ,    32,    // Notification's Consumed Sequence Number
            RNTC   ,    16,    // Root notification counter
            RNTP   ,     8,    // Root notification padding
            RNST   ,     8,    // Root notification Status
            RPSN   ,    32,    // Root notification's Produced Sequence Number
            RCSN   ,    32,    // Root notification's Consumed Sequence Number

            DLVL   ,    32,
            ICTL   ,    32,
            OS00, 8, OS01, 8, OS02, 8, OS03, 8, OS04, 8, OS05, 8, OS06, 8, OS07, 8, // Overwrite status for S0.C0..C7

            OS10, 8, OS11, 8, OS12, 8, OS13, 8, OS14, 8, OS15, 8, OS16, 8, OS17, 8, // Overwrite status for S1.C0..C7
            OS20, 8, OS21, 8, OS22, 8, OS23, 8, OS24, 8, OS25, 8, OS26, 8, OS27, 8, // Overwrite status for S2.C0..C7

            OS30, 8, OS31, 8, OS32, 8, OS33, 8, OS34, 8, OS35, 8, OS36, 8, OS37, 8, // Overwrite status for S3.C0..C7

            OS40, 8, OS41, 8, OS42, 8, OS43, 8, OS44, 8, OS45, 8, OS46, 8, OS47, 8, // Overwrite status for S4.C0..C7

            OS50, 8, OS51, 8, OS52, 8, OS53, 8, OS54, 8, OS55, 8, OS56, 8, OS57, 8, // Overwrite status for S5.C0..C7

            OS60, 8, OS61, 8, OS62, 8, OS63, 8, OS64, 8, OS65, 8, OS66, 8, OS67, 8, // Overwrite status for S6.C0..C7

            OS70, 8, OS71, 8, OS72, 8, OS73, 8, OS74, 8, OS75, 8, OS76, 8, OS77, 8, // Overwrite status for S7.C0..C7

            LO00, 16, LO01, 16, LO02, 16, LO03, 16, // Long Op for S0.C0..C3

            LO04, 16, LO05, 16, LO06, 16, LO07, 16, // Long Op for S0.C4..C7
            LO10, 16, LO11, 16, LO12, 16, LO13, 16, // Long Op for S1.C0..C3

            LO14, 16, LO15, 16, LO16, 16, LO17, 16, // Long Op for S1.C4..C7
            LO20, 16, LO21, 16, LO22, 16, LO23, 16, // Long Op for S2.C0..C3

            LO24, 16, LO25, 16, LO26, 16, LO27, 16, // Long Op for S2.C4..C7
            LO30, 16, LO31, 16, LO32, 16, LO33, 16, // Long Op for S3.C0..C3

            LO34, 16, LO35, 16, LO36, 16, LO37, 16, // Long Op for S3.C4..C7
            LO40, 16, LO41, 16, LO42, 16, LO43, 16, // Long Op for S4.C0..C3

            LO44, 16, LO45, 16, LO46, 16, LO47, 16, // Long Op for S4.C4..C7
            LO50, 16, LO51, 16, LO52, 16, LO53, 16, // Long Op for S5.C0..C3

            LO54, 16, LO55, 16, LO56, 16, LO57, 16, // Long Op for S5.C4..C7
            LO60, 16, LO61, 16, LO62, 16, LO63, 16, // Long Op for S6.C0..C3

            LO64, 16, LO65, 16, LO66, 16, LO67, 16, // Long Op for S6.C4..C7
            LO70, 16, LO71, 16, LO72, 16, LO73, 16, // Long Op for S7.C0..C3

            LO74, 16, LO75, 16, LO76, 16, LO77, 16, // Long Op for S7.C4..C7
            ASBL   ,     8,    // ARS Status Buffer Lock (0/1/0xff) -- set by ASL before consuming a buffer
            ASPB   ,     8,    // ARS Status Produced Buffer (0/1) -- set by SMM
            INJE   ,     8,    // Error Injection Enabled on one or more DCPMM DIMMs
            MCCH   ,     8,    // Channels per IMC
            SAWO   ,     8,    // Stop ARS with Overflow indicator to SMM
            BARS   ,     8,    // Background ARS status
            PAD0   ,    40,

            AST0   ,    32,
            ASL0   ,    32,
            ASB0   , DSM_ARS_STATUS_BUFFER_SIZE_BITS,
            AST1   ,    32,
            ASL1   ,    32,
            ASB1   , DSM_ARS_STATUS_BUFFER_SIZE_BITS,

            // for optimal performance of CR Large Payload BIOS-emulated commands, these must be aligned on an 8-byte boundary
            INPB   , DSM_INPB_SIZE_BITS,// Input Buffer in bits (not bytes)
            OUTB   , DSM_OUTB_SIZE_BITS // Output Buffer in bits
        }
    }
}
