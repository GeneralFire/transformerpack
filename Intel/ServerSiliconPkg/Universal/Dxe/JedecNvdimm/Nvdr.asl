/** @file Nvdr.asl
  Common implementation of NVDIMM ACPI interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation. <BR>

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
        #define DSM_ROOT_UUID      "2F10E7A4-9E91-11E4-89D3-123B93F75CBA"
        #define DSM_INPB_SIZE      0x10000  // NVDIMM_MAX_INPUT_BUF_SIZE in NvdimmAcpiSmmInterface.h
        #define DSM_OUTB_SIZE      0x10000  // NVDIMM_MAX_OUTPUT_BUF_SIZE in NvdimmAcpiSmmInterface.h
        #define DSM_INPB_SIZE_BITS 0x80000  // DSM_INPB_SIZE * 8, but must be number, ASL compiler will not evaluate
        #define DSM_OUTB_SIZE_BITS 0x80000  // DSM_OUTB_SIZE * 8, but must be number, ASL compiler will not evaluate
        #define DSM_ARS_STATUS_BUFFER_SIZE_BITS 0x80000

        #define ACPI_STS_NOT_IMPLEMENTED    0x01

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

        //
        // NVD_ENTRY macro is used to generate NVD entry for each DIMM
        //
        #define  NVD_ENTRY(socket, channel, dimm)                        \
        Device (N##socket##channel##dimm) {                              \
            Method (_ADR, 0) {                                           \
                Store (Zero, Local0)                                     \
                if (LEqual (MCCH, 2)) {                                  \
                    Store (0x##socket##00##dimm, Local0)                 \
                    And (channel, 0x1, Local1)                           \
                    ShiftRight (channel, 1, Local2)                      \
                    ShiftLeft (Local1, 4, Local1)                        \
                    ShiftLeft (Local2, 8, Local2)                        \
                    Or (Local0, Local1, Local0)                          \
                    Or (Local0, Local2, Local0)                          \
                } elseif (LEqual (MCCH, 3)) {                            \
                    Store (0x##socket##0##channel##dimm, Local0)         \
                    if (LGreater (channel, 2)) {                         \
                    Store (channel, Local1)                              \
                    Subtract (Local1, 3, Local1)                         \
                    ShiftLeft (Local1, 4, Local1)                        \
                    And (Local0, 0xFF0F, Local0)                         \
                    Or (Local0, Local1, Local0)                          \
                    Or (Local0, 0x100, Local0)                           \
                    }                                                    \
                }                                                        \
                Return (Local0)                                          \
            }                                                            \
            Method (_DSM, 4)                                             \
            {                                                            \
                Return (JNLD (Arg0, Arg1, Arg2, Arg3, _ADR))             \
            }                                                            \
            Method (_LSI, 0)                                             \
            {                                                            \
                Return (JLSI (_ADR))                                     \
            }                                                            \
            Method (_LSR, 2)                                             \
            {                                                            \
                Return (JLSR (Arg0, Arg1, _ADR))                         \
            }                                                            \
            Method (_LSW, 3, Serialized)                                 \
            {                                                            \
                Return (JLSW (Arg0, Arg1, Arg2, _ADR))                   \
            }                                                            \
            Method (_NCH, 0)                                             \
            {                                                            \
                Return (JNCH (_ADR))                                     \
            }                                                            \
            Method (_NBS, 0)                                             \
            {                                                            \
                Return (JNBS (_ADR))                                     \
            }                                                            \
            Method (_NIC, 0)                                             \
            {                                                            \
                Return (JNIC (_ADR))                                     \
            }                                                            \
            Method (_NIG, 0)                                             \
            {                                                            \
                Return (JNIG (_ADR))                                     \
            }                                                            \
            Method (_NIH, 1)                                             \
            {                                                            \
                Return (JNIH (Arg0, _ADR))                               \
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


        Device (NVDR) {
            Name (_HID, "ACPI0012")
            External (SMIC)
            #include "JedecNvdimm.asi"

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
                Return (JNRD (Arg0, Arg1, Arg2, Arg3))
            }

            //
            // Process leaf notifications pre SWGPE disable
            //
            Method (LPRE, 0)
            {
                Return (JLPR ())
            }

            //
            // Process root notifications pre SWGPE disable
            //
            Method (RPRE, 0)
            {
                Return (JRPR ())
            }

            //
            // Process leaf notifications post SWGPE disable
            //
            Method (LPST, 1)
            {
                Return (JLPT (Arg0))
            }

            //
            // Process root notifications post SWGPE disable
            //
            Method (RPST, 1)
            {
                Return (JRPT (Arg0))
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
        // IMPORTANT NOTE: When updating this structure please update NVDIMM_ACPI_SMM_INTERFACE in ServerSiliconPkg/Universal/Dxe/JedecNvdimm/NvdimmAcpiSmmInterface.h
        // This structure is shared between ACPI and SMM.
        //
        OperationRegion (CRCM, SystemMemory, 0x41584946, 0x20051) // (FIXA - Patched by ACPI Platform Driver during POST)
        Field (CRCM, ByteAcc, NoLock, Preserve) {
            DBM0   ,    64,    // Device Bitmap for sockets 0 - 3.  Bit position = (socket * 16) + (channel * 2) + dimm
            DBM1   ,    64,    // Device Bitmap for sockets 4 - 7.  Bit position = (socket * 16) + (channel * 2) + dimm - 64

            ESP0   ,    64,    // Energy Source Policy in sockets 0 thru 3
            ESP1   ,    64,    // Energy Source Policy in sockets 4 thru 7

            FUNI   ,    32,    // Function Index
            NFHD   ,    32,    // NFIT Device Handle
            INPL   ,    32,    // Input Length
            OUTL   ,    32,    // Output Length

            ARSA   ,    64,    // ARS Start Address
            ARSL   ,    64,    // ARS Length

            ARST   ,    16,    // ARS Start Type
            ARSF   ,     8,    // ARS Start Flags
            DINT   ,     8,    // Debug Interface
            NFST   ,    32,    // Status

            DLVL   ,    32,
            ICTL   ,    32,

            MCCH   ,     8,    // Channels per IMC

            INPB   , DSM_INPB_SIZE_BITS,// Input Buffer in bits (not bytes)
            OUTB   , DSM_OUTB_SIZE_BITS // Output Buffer in bits
        }

    }
}
