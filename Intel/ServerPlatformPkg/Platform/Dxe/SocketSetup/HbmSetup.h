/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _HBM_SETUP_H_
#define _HBM_SETUP_H_


#include "HbmDefaults.h"
#include "MemorySetup.h"

//
// Page Policy
//
#if HBM_CLOSED_PAGE_DEFAULT == CLOSED_PAGE_DIS
  #define HBM_CLOSED_PAGE_DIS_FLAGS     DEFAULT_MEMORY_SETUP
  #define HBM_CLOSED_PAGE_EN_FLAGS      RESET_REQUIRED
  #define HBM_OPEN_PAGE_ADAPTIVE_FLAGS  RESET_REQUIRED
#elif HBM_CLOSED_PAGE_DEFAULT == CLOSED_PAGE_EN
  #define HBM_CLOSED_PAGE_EN_FLAGS      DEFAULT_MEMORY_SETUP
  #define HBM_CLOSED_PAGE_DIS_FLAGS     RESET_REQUIRED
  #define HBM_OPEN_PAGE_ADAPTIVE_FLAGS  RESET_REQUIRED
#else // HBM_CLOSED_PAGE_DEFAULT == CLOSED_PAGE_DIS
  #define HBM_OPEN_PAGE_ADAPTIVE_FLAGS  DEFAULT_MEMORY_SETUP
  #define HBM_CLOSED_PAGE_DIS_FLAGS     RESET_REQUIRED
  #define HBM_CLOSED_PAGE_EN_FLAGS      RESET_REQUIRED
#endif // HBM_CLOSED_PAGE_DEFAULT == CLOSED_PAGE_DIS

//
// DBI Mode
//
#if HBM_DBI_DEFAULT == HBM_DBI_DIS
#define HBM_DBI_DIS_FLAGS DEFAULT_MEMORY_SETUP
#else // HBM_DBI_DEFAULT == HBM_DBI_DIS
#define HBM_DBI_DIS_FLAGS RESET_REQUIRED
#endif // HBM_DBI_DEFAULT == HBM_DBI_DIS

#if HBM_DBI_DEFAULT == HBM_DBI_EN
#define HBM_DBI_EN_FLAGS DEFAULT_MEMORY_SETUP
#else // HBM_DBI_DEFAULT == HBM_DBI_EN
#define HBM_DBI_EN_FLAGS RESET_REQUIRED
#endif // HBM_DBI_DEFAULT == HBM_DBI_EN

//
// Relaxed turnaround
//
#if HBM_RELAXED_TA_DEFAULT == HBM_RELAXED_TA_DIS
#define HBM_RELAXED_TA_DIS_FLAGS DEFAULT_MEMORY_SETUP
#else // HBM_RELAXED_TA_DEFAULT == HBM_RELAXED_TA_DIS
#define HBM_RELAXED_TA_DIS_FLAGS RESET_REQUIRED
#endif // HBM_RELAXED_TA_DEFAULT == HBM_RELAXED_TA_DIS

#if HBM_RELAXED_TA_DEFAULT == HBM_RELAXED_TA_EN
#define HBM_RELAXED_TA_EN_FLAGS DEFAULT_MEMORY_SETUP
#else // HBM_RELAXED_TA_DEFAULT == HBM_RELAXED_TA_EN
#define HBM_RELAXED_TA_EN_FLAGS RESET_REQUIRED
#endif // HBM_RELAXED_TA_DEFAULT == HBM_RELAXED_TA_EN

//
// Refresh mode
//
#if HBM_REFRESH_MODE_DEFAULT == HBM_ALL_BANK_REFRESH
#define HBM_ALL_BANK_REFRESH_FLAGS DEFAULT_MEMORY_SETUP
#else // HBM_REFRESH_MODE_DEFAULT == HBM_ALL_BANK_REFRESH
#define HBM_ALL_BANK_REFRESH_FLAGS RESET_REQUIRED
#endif // HBM_REFRESH_MODE_DEFAULT == HBM_ALL_BANK_REFRESH

#if HBM_REFRESH_MODE_DEFAULT == HBM_PER_BANK_REFRESH
#define HBM_PER_BANK_REFRESH_FLAGS DEFAULT_MEMORY_SETUP
#else // HBM_REFRESH_MODE_DEFAULT == HBM_PER_BANK_REFRESH
#define HBM_PER_BANK_REFRESH_FLAGS RESET_REQUIRED
#endif // HBM_REFRESH_MODE_DEFAULT == HBM_PER_BANK_REFRESH

//
// Self Refresh Programming
//
#if HBM_SREF_PROG_DEFAULT == SREF_PROG_AUTO
#define HBM_SREF_PROG_AUTO_FLAGS   DEFAULT_MEMORY_SETUP
#define HBM_SREF_PROG_MANUAL_FLAGS RESET_REQUIRED
#else // HBM_SREF_PROG_DEFAULT == SREF_PROG_AUTO
#define HBM_SREF_PROG_AUTO_FLAGS   RESET_REQUIRED
#define HBM_SREF_PROG_MANUAL_FLAGS DEFAULT_MEMORY_SETUP
#endif // HBM_SREF_PROG_DEFAULT == SREF_PROG_AUTO

//
// Opportunistic Self-Refresh
//
#if HBM_OPP_SREF_DEFAULT == OPP_SREF_DISABLE
#define HBM_OPP_SREF_DIS_FLAGS DEFAULT_MEMORY_SETUP
#define HBM_OPP_SREF_EN_FLAGS  RESET_REQUIRED
#else // HBM_OPP_SREF_DEFAULT == OPP_SREF_DISABLE
#define HBM_OPP_SREF_DIS_FLAGS RESET_REQUIRED
#define HBM_OPP_SREF_EN_FLAGS  DEFAULT_MEMORY_SETUP
#endif // HBM_OPP_SREF_DEFAULT == OPP_SREF_DISABLE

//
// PKGC_SREF
//
#if HBM_PKGC_SREF_DEFAULT == PKGC_SREF_DISABLE
#define HBM_PKGC_SREF_DIS_FLAGS DEFAULT_MEMORY_SETUP
#define HBM_PKGC_SREF_EN_FLAGS  RESET_REQUIRED
#else //HBM_PKGC_SREF_DEFAULT == PKGC_SREF_DISABLE
#define HBM_PKGC_SREF_DIS_FLAGS RESET_REQUIRED
#define HBM_PKGC_SREF_EN_FLAGS  DEFAULT_MEMORY_SETUP
#endif // HBM_PKGC_SREF_DEFAULT == PKGC_SREF_DISABLE


#endif // _HBM_SETUP_H_
