/** @file
  Routes to register includes

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
#ifndef __REG_PATH_H__
#define __REG_PATH_H__

//
// stringizing helper macros
//
#define STRINGIFY2( x) #x
#define STRINGIFY(x) STRINGIFY2(x)


#if defined(ICXDE_HOST)
#if defined(ICX_HOST) || defined(SKX_HOST) || defined(SNR_HOST) || defined(SPR_HOST) || defined(TNR_HOST)
#error "Only one HOST flag can be defined per build"
#endif // defined(ICX_HOST) || defined(SKX_HOST) || defined(SNR_HOST) || defined(SPR_HOST)
#define REG_PATH(a) <Registers/Icxde/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Icxde/a)
#endif // defined(ICXDE_HOST)

#if defined(ICX_HOST)
#if defined(ICXDE_HOST) || defined(SKX_HOST) || defined(SNR_HOST) || defined(SPR_HOST) || defined(TNR_HOST)
#error "Only one HOST flag can be defined per build"
#endif // defined(ICXDE_HOST) || defined(SKX_HOST) || defined(SNR_HOST) || defined(SPR_HOST)
#define REG_PATH(a) <Registers/Icx/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Icx/a)
#endif // defined(ICX_HOST)

#if defined(SPR_HOST) || defined(GNRSRF_HOST)
#if defined(ICXDE_HOST) || defined(SKX_HOST) || defined(SNR_HOST) || defined(ICX_HOST) || defined(TNR_HOST)
#error "Only one HOST flag can be defined per build"
#endif // defined(ICXDE_HOST) || defined(SKX_HOST) || defined(SNR_HOST) || defined(ICX_HOST)
#if defined(GNRSRF_HOST)
#define REG_PATH(a) <Registers/GnrSrf/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Spr/a)  // Using SPR for now until we have all IP header files for GNR.
#define REG_PATH_PRIVATE_S3M(a) STRINGIFY(Registers/GnrSrf/a) // To change this back to REG_PATH_PRIVATE once all GNR header files are integrated.
#define REG_PATH_PRIVATE_MDFIS(a) STRINGIFY(Registers/GnrSrf/a)  // To change this back to REG_PATH_PRIVATE once all GNR header files are integrated.
#else
#define REG_PATH(a) <Registers/Spr/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Spr/a)
#define REG_PATH_PRIVATE_MDFIS(a) STRINGIFY(Registers/Spr/a)
#define REG_PATH_PRIVATE_S3M(a) STRINGIFY(Registers/Spr/a)
#endif
#define REG_PATH_HBM(a) STRINGIFY(Registers/Spr/a)
#define REG_PATH_PRIVATE_DDRIO(a) STRINGIFY(Soc/a)
#define REG_PATH_PRIVATE_CPGC(a) STRINGIFY(Soc/a)
#define REG_PATH_PRIVATE_MC(a) STRINGIFY(Soc/a)
#define REG_PATH_PRIVATE_RCSIM(a) STRINGIFY(Soc/a)
#if defined(SPR_MCDDR)
#define REG_PATH_PRIVATE_HBM(a) STRINGIFY(Hbm/SprHbmRegsDummy.h)
#else
#define REG_PATH_PRIVATE_HBM(a) STRINGIFY(Hbm/a)
#endif //SPR_MCDDR
#endif // defined(SPR_HOST)

#if defined(SKX_HOST) && !defined(CPX_HOST)
#if defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SNR_HOST) || defined(SPR_HOST) || defined(TNR_HOST)
#error "Only one HOST flag can be defined per build"
#endif // defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SNR_HOST) || defined(SPR_HOST)
#define REG_PATH(a) <Registers/Skx/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Skx/a)
#endif // defined(SKX_HOST)

//
// Using SKX header files for CPX_CPU, when register heade file for CPX_CPU is ready, will update it
//
#if defined(SKX_HOST) && defined(CPX_HOST)
#if defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SNR_HOST) || defined(SPR_HOST) || defined(TNR_HOST)
#error "Only one HOST flag can be defined per build"
#endif // defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SNR_HOST) || defined(SPR_HOST)
#define REG_PATH(a) <Registers/Cpx/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Cpx/a)
#endif // defined(SKX_HOST)

//
// Using ICX header files for CPX_PCIE_GEN4, when register heade file for CPX_PCIE_GEN4 is ready, will update it
//
#if defined(CPX_PCIE_GEN4)
#ifdef REG_PATH
#undef REG_PATH
#endif
#ifdef REG_PATH_PRIVATE
#undef REG_PATH_PRIVATE
#endif
#define REG_PATH(a) <Registers/Icx/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Icx/a)
#endif // defined(CPX_PCIE_GEN4)

#if defined(SNR_HOST) && !defined(TNR_HOST)
#if defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SKX_HOST) || defined(SPR_HOST) || defined(TNR_HOST)
#error "Only one HOST flag can be defined per build"
#endif // defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SKX_HOST) || defined(SPR_HOST)
#define REG_PATH(a) <Registers/Snr/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Snr/a)
#endif // defined(SNR_HOST)

#if defined(TNR_HOST) && defined(SNR_HOST)
#if defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SKX_HOST) || defined(SPR_HOST)
#error "Only one HOST flag can be defined per build"
#endif // defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SKX_HOST) || defined(SPR_HOST)
#define REG_PATH(a) <Registers/Tnr/a>
#define REG_PATH_PRIVATE(a) STRINGIFY(Registers/Tnr/a)
#endif // defined(TNR_HOST)

#if !defined(REG_PATH)
#error "Could not determine the REG_PATH! Perhaps a HOST is not selected."
#endif

#endif // __REG_PATH_H__
