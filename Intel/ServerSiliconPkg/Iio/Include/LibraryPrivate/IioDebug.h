/** @file
This file expose IIO debug facility interface.

@copyright
INTEL CONFIDENTIAL
Copyright 2019 Intel Corporation. <BR>

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

#ifndef _IIO_DEBUG_H_
#define _IIO_DEBUG_H_

#if AFTER_IIO_CLEANUP
//
// Generic IIO traces, not bound to any specific subsystem.
//
#define IIO_D_LOG(...)     { DEBUG((DEBUG_ERROR, "[IIO] " __VA_ARGS__)); }          // Important log always printed
#define IIO_D_DBG(...)     { DEBUG((DEBUG_INFO,  "[IIO] " __VA_ARGS__)); }           // Debug log, printed with BIOS debug jumper
#define IIO_D_WARN(...)    { DEBUG((DEBUG_ERROR, "[IIO] WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_ERR(...)     { DEBUG((DEBUG_ERROR, "[IIO] ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem
//
// DMI traces.
//
#define IIO_D_DMILOG(...)  { DEBUG((DEBUG_ERROR, "[IIO](DMI) " __VA_ARGS__)); }          // Important log always printed
#define IIO_D_DMIDBG(...)  { DEBUG((DEBUG_INFO,  "[IIO](DMI) " __VA_ARGS__)); }           // Debug log, printed with BIOS debug jumper
#define IIO_D_DMIWARN(...) { DEBUG((DEBUG_ERROR, "[IIO](DMI) WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_DMIERR(...)  { DEBUG((DEBUG_ERROR, "[IIO](DMI) ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem
//
// PCI traces.
//
#define IIO_D_PCILOG(...)  { DEBUG((DEBUG_ERROR, "[IIO](PCI) " __VA_ARGS__)); }           // Important log always printed
#define IIO_D_PCIDBG(...)  { DEBUG((DEBUG_INFO,  "[IIO](PCI) " __VA_ARGS__)); }           // Debug log, printed with BIOS debug jumper
#define IIO_D_PCIWARN(...) { DEBUG((DEBUG_ERROR, "[IIO](PCI) WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_PCIERR(...)  { DEBUG((DEBUG_ERROR, "[IIO](PCI) ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem
//
// VTD traces.
//
#define IIO_D_VTDLOG(...)  { DEBUG((DEBUG_ERROR, "[IIO](VTD) " __VA_ARGS__)); }          // Important log always printed
#define IIO_D_VTDDBG(...)  { DEBUG((DEBUG_INFO,  "[IIO](VTD) " __VA_ARGS__)); }          // Debug log, printed with BIOS debug jumper
#define IIO_D_VTDWARN(...) { DEBUG((DEBUG_ERROR, "[IIO](VTD) WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_VTDERR(...)  { DEBUG((DEBUG_ERROR, "[IIO](VTD) ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem

#else
//
// Generic IIO traces, not bound to any specific subsystem.
//
#define IIO_D_LOG(...)     { DEBUG((DEBUG_ERROR, "" __VA_ARGS__)); }          // Important log always printed
#define IIO_D_DBG(...)     { DEBUG((DEBUG_INFO,  "" __VA_ARGS__)); }           // Debug log, printed with BIOS debug jumper
#define IIO_D_WARN(...)    { DEBUG((DEBUG_ERROR, "WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_ERR(...)     { DEBUG((DEBUG_ERROR, "ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem
//
// DMI traces.
//
#define IIO_D_DMILOG(...)  { DEBUG((DEBUG_ERROR, "" __VA_ARGS__)); }          // Important log always printed
#define IIO_D_DMIDBG(...)  { DEBUG((DEBUG_INFO,  "" __VA_ARGS__)); }           // Debug log, printed with BIOS debug jumper
#define IIO_D_DMIWARN(...) { DEBUG((DEBUG_ERROR, "WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_DMIERR(...)  { DEBUG((DEBUG_ERROR, "ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem
//
// PCI traces.
//
#define IIO_D_PCILOG(...)  { DEBUG((DEBUG_ERROR, "" __VA_ARGS__)); }           // Important log always printed
#define IIO_D_PCIDBG(...)  { DEBUG((DEBUG_INFO,  "" __VA_ARGS__)); }           // Debug log, printed with BIOS debug jumper
#define IIO_D_PCIWARN(...) { DEBUG((DEBUG_ERROR, "WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_PCIERR(...)  { DEBUG((DEBUG_ERROR, "ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem
//
// VTD traces.
//
#define IIO_D_VTDLOG(...)  { DEBUG((DEBUG_ERROR, "" __VA_ARGS__)); }          // Important log always printed
#define IIO_D_VTDDBG(...)  { DEBUG((DEBUG_INFO,  "" __VA_ARGS__)); }          // Debug log, printed with BIOS debug jumper
#define IIO_D_VTDWARN(...) { DEBUG((DEBUG_ERROR, "WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_VTDERR(...)  { DEBUG((DEBUG_ERROR, "ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem

#endif // AFTER_IIO_CLEANUP

//
// SPK traces.
//
#define IIO_D_SPKLOG(...)  { DEBUG((DEBUG_ERROR, "[IIO](SPK) " __VA_ARGS__)); }          // Important log always printed
#define IIO_D_SPKDBG(...)  { DEBUG((DEBUG_INFO,  "[IIO](SPK) " __VA_ARGS__)); }          // Debug log, printed with BIOS debug jumper
#define IIO_D_SPKWARN(...) { DEBUG((DEBUG_ERROR, "[IIO](SPK) WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_SPKERR(...)  { DEBUG((DEBUG_ERROR, "[IIO](SPK) ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem
//
// Trace Hub (NPK) traces.
//
#define IIO_D_THLOG(...)  { DEBUG((DEBUG_ERROR, "[IIO](TH) " __VA_ARGS__)); }          // Important log always printed
#define IIO_D_THDBG(...)  { DEBUG((DEBUG_INFO,  "[IIO](TH) " __VA_ARGS__)); }          // Debug log, printed with BIOS debug jumper
#define IIO_D_THWARN(...) { DEBUG((DEBUG_ERROR, "[IIO](TH) WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_THERR(...)  { DEBUG((DEBUG_ERROR, "[IIO](TH) ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem

//
// VMD traces
//
#define IIO_D_VMDLOG(...)  { DEBUG((DEBUG_ERROR, "[IIO](VMD) " __VA_ARGS__)); }          // Important log always printed
#define IIO_D_VMDDBG(...)  { DEBUG((DEBUG_INFO,  "[IIO](VMD) " __VA_ARGS__)); }          // Debug log, printed with BIOS debug jumper
#define IIO_D_VMDWARN(...) { DEBUG((DEBUG_ERROR, "[IIO](VMD) WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_VMDERR(...)  { DEBUG((DEBUG_ERROR, "[IIO](VMD) ERROR: " __VA_ARGS__)); }   // Errorneous sytuation, probably bug or hw problem

//
// The following format is used for IIO ports and stacks identification in the log:
// [<iio>]              – e.g. [1]     for IIO instance index (currently socket), IIO/socket 1 in this case
// [<iio>.<stk>]        – e.g. [1.0]   for stack in IIO instance, stack 0 in IIO/socket 1 in this case
// [<iio>.<stk>.<port>] – e.g. [1.0.0] for port index in a stack in IIO instance, port 0 in stack 0 in IIO/socket 1 in this case
// [<iio> p<port>]      – e.g. [1 p5]  for IIO/socket wide PortIndex in IIO instance, port 5 in IIO 1 in this case
//
// And for PCI device location:
// %02X:%02X:%02X.%X  – <segment>:<bus>:<device>.<function>
//

#endif  _IIO_DEBUG_H_

