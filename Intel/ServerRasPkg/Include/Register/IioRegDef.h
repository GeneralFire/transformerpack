/** @file
  Register definitions for machine check registers defined in
  IA32 Software Developer's Manual.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef __IIO_REG_DEF_H__
#define __IIO_REG_DEF_H__

#define R_MIERRSV                                   0x90                        // Misc Error Severity Register
#define R_PCIERRSV                                  0x94                        // PCI-E Error Severity Map Register
#define   V_PCIERRSV0                               00                          // Error Severity 0
#define   V_PCIERRSV1                               01                          // Error Severity 1
#define   V_PCIERRSV2                               02                          // Error Severity 2
#define R_SYSMAP                                    0x9C                        // System Error Event Map Register
#define   V_SYSMAP_NoInband                         000                         // No inband message
#define   V_SYSMAP_SMI_PMI                          001                         // Generate SMI/PMI
#define   V_SYSMAP_NMI                              002                         // Generate NMI
#define   V_SYSMAP_CPEI                             003                         // Generate CPEI
#define   V_SYSMAP_MCA                              004                         // Generate MCA
#define R_ERRPINCTL                                 0xA4
#define   V_ERRPINCTL_DIS                           00
#define   V_ERRPINCTL_DATA                          01
#define   V_ERRPINCTL_SEV                           02
#define   V_ERRPINCTL_RSVD                          03

//DMI settings
#define   R_DMI_DEVICE_CONTROL_OFFSET               0x60
#define   R_DMI_DEVICE_STATUS_OFFSET                0x62
#define   V_DMI_AER_CAPID                           0xB
#define   V_DMI_AER_VSEC_CAPID                      0x4

#define R_RPPIOERRHF                                0x08
#define R_RPPIOERRSTS                               0x0C
#define R_RPPIOERRMSK                               0x10

#define ONLY_REGISTER_OFFSET(x)                     (x & 0xFFF)

#define PCIE_EXT_CAP_HEADER_RPPIOERR_CAPID          0x000b
#define PCIE_EXT_CAP_HEADER_RPPIOERR_VSECID         0x0007

#define IIO_CORRECTABLE                             0x0
#define IIO_RECOVERABLE                             0x1
#define IIO_FATAL                                   0x2

// Bitmap mask
#define IIO_CORE_ERROR_MSK                          0x00003ff1      //IIOERRCTL
#define IIO_CORE_ERROR_SEVMSK                       0x00003f03      //IIOERRSV
#define IIO_COH_INFACE_MSK                          0x0000647e      //IRPP0ERRCTL     IRPP1ERRCTL  for BDX and HSX
#define IIO_COH_INFACE_IVTMSK                       0x0000646e      //IRPP0ERRCTL     IRPP1ERRCTL  for IVT
#define IIO_COH_SEV_MSK                             0x0c3003fc      // IRPPERRSV
#define IIO_MISC_ERROR_MSK                          0x0000001f      // MIERRCTL
#define IIO_MISC_ERROR_SEVMSK                       0x000003ff      // MIERRSV
#define IIO_VTD_MSK                                 0x800001ff      // VTUNCERRMSK   VTUNCERRSEV
#define IIO_ITC_MSK                                 0x0fffffff      // ITCERRSEV
#define IIO_OTC_MSK                                 0x0fffffff      // OTCERRSEV
#define IIO_DMA_MSK                                 0x00073fff      // CHANERRMSK    CHANERRSEV
#define IIO_PCIEADD_UNCORR_MSK                      0x0000037a      // XPUNCEDMASK   XPUNCERRSEV   XPUNCERRMSK

// Used to override the AER mask registers
// Set bits that need to be masked due to known issues
#define IIO_AER_CEM_OVERRIDE                        0x00000000
#define IIO_AER_UEM_OVERRIDE                        0x00000000


#define   LNERR_MASK_DATA                           0x3F0FFEF;
#define   LFERR_MASK_DATA                           0x3F0FFEF;
#define   LCERR_MASK_DATA                           0x7F0FFEF;
#define   LERRCTL_MASK_DATA                         0x7F0FFEF;

#define   IIO_XPCORERR_MASK_DATA                    0x1;
#define   IIO_XPUNCERRD_MASK_DATA                   0x7f;
#define   IIO_AERCORERR_MASK_DATA                   0x31C1;
//
// Defines
//
#define PCIE_EXT_CAP_HEADER_LERC                    0x000b
#define PCIE_EXT_CAP_HEADER_LERC_REVISION           0x0002
#define PCIE_EXT_CAP_HEADER_LER_VSECID              0x0005

#define PCIE_EXT_CAP_HEADER_EINJ                    0x000b
#define PCIE_EXT_CAP_HEADER_EINJ_REVISION           0x0001
#define PCIE_EXT_CAP_HEADER_EINJ_VSECID             0x0003
#define PCIE_EXT_CAP_EINJ_HDR_REG_OFFSET            0x0004
#define PCIE_EXT_CAP_EINJ_CTL_REG_OFFSET            0x0008
#define PCIE_EXT_CAP_EINJ_CTL_REG_OFFSET            0x0008
#define   B_PCIE_EXT_CAP_EINJ_CTL_EINJDIS             BIT0
#define   B_PCIE_EXT_CAP_EINJ_CTL_INJRCVERR           BIT1
#define   B_PCIE_EXT_CAP_EINJ_CTL_INJCTOERR           BIT2

#define IIO_GxERRST_PCIE_MASK                       0x10ffe0
#define IIO_GLBL_ERR_BITMASK                        0x0000003F

#define SKX_CBDMA_DEVICE_NUM                        0x7            // On stack 0, function 0-7
#define SKX_CBDMA_FUNCTION_NUM                      0x0
#define SKX_VTD_DEVICE_NUM                          0x5            // on Stack 0-5
#define SKX_VTD_FUNCTION_NUM                        0x0
#define SKX_RAS_DEVICE_NUM                          0x5            // on Stack 0-5
#define SKX_RAS_FUNCTION_NUM                        0x2

/*
   Version of the IEH 0: Wave1/2 IEH (legacy IEH ?
   used in ICX-SP, ICX-D, SNR) 1: Emmitsburg PCH
   IEH 2: Tigerlake PCH IEH 3: IEH with RCEC
   Enabled 4: IEH with RCEC Disabled Others:
   Reserved
*/

#define IEH_VER_LEGACY                            0x0
#define IEH_VER_EBG                               0x1
#define IEH_VER_TGL                               0x2
#define IEH_VER_RCEC                              0x3
#define IEH_VER_RCEC_DIS                          0x4

#endif
