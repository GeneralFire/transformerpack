
/** @file
  MDFIS_MAIN.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>
  
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
  
  This file contains Silicon register definitions.
  
  This is a generated file; please do not modify it directly.
  
**/

/* The following security policy groups are used by registers in this file:     */

/* SPRA0 Security Policy Groups:                                                */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */

/* SPRUCC Security Policy Groups:                                               */
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _MDFIS_MAIN_h
#define _MDFIS_MAIN_h
#include <Base.h>

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_90_GV_CTRL_BIN0_1_8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac800)                                                      */
/*      SPRB0 (0x200ac800)                                                      */
/*      SPRHBM (0x2007c800)                                                     */
/*      SPRC0 (0x200ac800)                                                      */
/*      SPRUCC (0x200dd000)                                                     */
/* Register default value on SPRA0: 0x208DD85F                                  */
/* Register default value on SPRB0: 0x208DD85F                                  */
/* Register default value on SPRHBM: 0x208DD85F                                 */
/* Register default value on SPRC0: 0x208DD85F                                  */
/* Register default value on SPRUCC: 0x208DD85F                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_90_GV_CTRL_BIN2_3_9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac804)                                                      */
/*      SPRB0 (0x200ac804)                                                      */
/*      SPRHBM (0x2007c804)                                                     */
/*      SPRC0 (0x200ac804)                                                      */
/*      SPRUCC (0x200dd004)                                                     */
/* Register default value on SPRA0: 0x1D88193E                                  */
/* Register default value on SPRB0: 0x1D88193E                                  */
/* Register default value on SPRHBM: 0x1D88193E                                 */
/* Register default value on SPRC0: 0x1D88193E                                  */
/* Register default value on SPRUCC: 0x1D88193E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_90_GV_CTRL_BIN4_5_10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac808)                                                      */
/*      SPRB0 (0x200ac808)                                                      */
/*      SPRHBM (0x2007c808)                                                     */
/*      SPRC0 (0x200ac808)                                                      */
/*      SPRUCC (0x200dd008)                                                     */
/* Register default value on SPRA0: 0x1A8578D4                                  */
/* Register default value on SPRB0: 0x1A8578D4                                  */
/* Register default value on SPRHBM: 0x1A8578D4                                 */
/* Register default value on SPRC0: 0x1A8578D4                                  */
/* Register default value on SPRUCC: 0x1A8578D4                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_90_GV_CTRL_BIN6_7_11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac80c)                                                      */
/*      SPRB0 (0x200ac80c)                                                      */
/*      SPRHBM (0x2007c80c)                                                     */
/*      SPRC0 (0x200ac80c)                                                      */
/*      SPRUCC (0x200dd00c)                                                     */
/* Register default value on SPRA0: 0x17C4809E                                  */
/* Register default value on SPRB0: 0x17C4809E                                  */
/* Register default value on SPRHBM: 0x17C4809E                                 */
/* Register default value on SPRC0: 0x17C4809E                                  */
/* Register default value on SPRUCC: 0x17C4809E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_90_GV_CTRL_BIN1_2_3_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac810)                                                      */
/*      SPRB0 (0x200ac810)                                                      */
/*      SPRHBM (0x2007c810)                                                     */
/*      SPRC0 (0x200ac810)                                                      */
/*      SPRUCC (0x200dd010)                                                     */
/* Register default value on SPRA0: 0x01008040                                  */
/* Register default value on SPRB0: 0x01008040                                  */
/* Register default value on SPRHBM: 0x01008040                                 */
/* Register default value on SPRC0: 0x01008040                                  */
/* Register default value on SPRUCC: 0x01008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_90_GV_CTRL_BIN4_5_6_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac814)                                                      */
/*      SPRB0 (0x200ac814)                                                      */
/*      SPRHBM (0x2007c814)                                                     */
/*      SPRC0 (0x200ac814)                                                      */
/*      SPRUCC (0x200dd014)                                                     */
/* Register default value on SPRA0: 0x11008040                                  */
/* Register default value on SPRB0: 0x11008040                                  */
/* Register default value on SPRHBM: 0x11008040                                 */
/* Register default value on SPRC0: 0x11008040                                  */
/* Register default value on SPRUCC: 0x11008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac818)                                                      */
/*      SPRB0 (0x200ac818)                                                      */
/*      SPRHBM (0x2007c818)                                                     */
/*      SPRC0 (0x200ac818)                                                      */
/*      SPRUCC (0x200dd018)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac818)                                                      */
/*      SPRB0 (0x200ac818)                                                      */
/*      SPRHBM (0x2007c818)                                                     */
/*      SPRC0 (0x200ac818)                                                      */
/*      SPRUCC (0x200dd018)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac818)                                                      */
/*      SPRB0 (0x200ac818)                                                      */
/*      SPRHBM (0x2007c818)                                                     */
/*      SPRC0 (0x200ac818)                                                      */
/*      SPRUCC (0x200dd018)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac81c)                                                      */
/*      SPRB0 (0x200ac81c)                                                      */
/*      SPRHBM (0x2007c81c)                                                     */
/*      SPRC0 (0x200ac81c)                                                      */
/*      SPRUCC (0x200dd01c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac81c)                                                      */
/*      SPRB0 (0x200ac81c)                                                      */
/*      SPRHBM (0x2007c81c)                                                     */
/*      SPRC0 (0x200ac81c)                                                      */
/*      SPRUCC (0x200dd01c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac81c)                                                      */
/*      SPRB0 (0x200ac81c)                                                      */
/*      SPRHBM (0x2007c81c)                                                     */
/*      SPRC0 (0x200ac81c)                                                      */
/*      SPRUCC (0x200dd01c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac820)                                                      */
/*      SPRB0 (0x200ac820)                                                      */
/*      SPRHBM (0x2007c820)                                                     */
/*      SPRC0 (0x200ac820)                                                      */
/*      SPRUCC (0x200dd020)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac820)                                                      */
/*      SPRB0 (0x200ac820)                                                      */
/*      SPRHBM (0x2007c820)                                                     */
/*      SPRC0 (0x200ac820)                                                      */
/*      SPRUCC (0x200dd020)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac820)                                                      */
/*      SPRB0 (0x200ac820)                                                      */
/*      SPRHBM (0x2007c820)                                                     */
/*      SPRC0 (0x200ac820)                                                      */
/*      SPRUCC (0x200dd020)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac824)                                                      */
/*      SPRB0 (0x200ac824)                                                      */
/*      SPRHBM (0x2007c824)                                                     */
/*      SPRC0 (0x200ac824)                                                      */
/*      SPRUCC (0x200dd024)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac824)                                                      */
/*      SPRB0 (0x200ac824)                                                      */
/*      SPRHBM (0x2007c824)                                                     */
/*      SPRC0 (0x200ac824)                                                      */
/*      SPRUCC (0x200dd024)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac824)                                                      */
/*      SPRB0 (0x200ac824)                                                      */
/*      SPRHBM (0x2007c824)                                                     */
/*      SPRC0 (0x200ac824)                                                      */
/*      SPRUCC (0x200dd024)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac828)                                                      */
/*      SPRB0 (0x200ac828)                                                      */
/*      SPRHBM (0x2007c828)                                                     */
/*      SPRC0 (0x200ac828)                                                      */
/*      SPRUCC (0x200dd028)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac828)                                                      */
/*      SPRB0 (0x200ac828)                                                      */
/*      SPRHBM (0x2007c828)                                                     */
/*      SPRC0 (0x200ac828)                                                      */
/*      SPRUCC (0x200dd028)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_90_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac828)                                                      */
/*      SPRB0 (0x200ac828)                                                      */
/*      SPRHBM (0x2007c828)                                                     */
/*      SPRC0 (0x200ac828)                                                      */
/*      SPRUCC (0x200dd028)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDLLQLS45_90_GVT_OFFSET_CREG_MDFIS_MAIN_REG supported on:                    */
/*      SPRA0 (0x200ac82c)                                                      */
/*      SPRB0 (0x200ac82c)                                                      */
/*      SPRHBM (0x2007c82c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* MDFIS DLLQLS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QDS_DCC45_BINARY_SEARCH_CTRL0_CREG_MDFIS_MAIN_REG supported on:              */
/*      SPRC0 (0x200ac82c)                                                      */
/*      SPRUCC (0x200dd02c)                                                     */
/* Register default value on SPRC0: 0xADBFE09C                                  */
/* Register default value on SPRUCC: 0xADBFE09C                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS DCC BINARY SEARCH CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_90_CONTD2C_DIFF_GV_CTRL_CREG_MDFIS_MAIN_REG supported on:              */
/*      SPRA0 (0x200ac830)                                                      */
/*      SPRB0 (0x200ac830)                                                      */
/*      SPRHBM (0x2007c830)                                                     */
/*      SPRC0 (0x200ac830)                                                      */
/*      SPRUCC (0x200dd030)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS MDLL D2C DIFF GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_90_GV_CTRL_BIN0_1_8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac834)                                                      */
/*      SPRB0 (0x200ac834)                                                      */
/*      SPRHBM (0x2007c834)                                                     */
/*      SPRC0 (0x200ac834)                                                      */
/*      SPRUCC (0x200dd034)                                                     */
/* Register default value on SPRA0: 0x208DD85F                                  */
/* Register default value on SPRB0: 0x208DD85F                                  */
/* Register default value on SPRHBM: 0x208DD85F                                 */
/* Register default value on SPRC0: 0x208DD85F                                  */
/* Register default value on SPRUCC: 0x208DD85F                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_90_GV_CTRL_BIN2_3_9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac838)                                                      */
/*      SPRB0 (0x200ac838)                                                      */
/*      SPRHBM (0x2007c838)                                                     */
/*      SPRC0 (0x200ac838)                                                      */
/*      SPRUCC (0x200dd038)                                                     */
/* Register default value on SPRA0: 0x1D88193E                                  */
/* Register default value on SPRB0: 0x1D88193E                                  */
/* Register default value on SPRHBM: 0x1D88193E                                 */
/* Register default value on SPRC0: 0x1D88193E                                  */
/* Register default value on SPRUCC: 0x1D88193E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_90_GV_CTRL_BIN4_5_10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac83c)                                                      */
/*      SPRB0 (0x200ac83c)                                                      */
/*      SPRHBM (0x2007c83c)                                                     */
/*      SPRC0 (0x200ac83c)                                                      */
/*      SPRUCC (0x200dd03c)                                                     */
/* Register default value on SPRA0: 0x1A8578D4                                  */
/* Register default value on SPRB0: 0x1A8578D4                                  */
/* Register default value on SPRHBM: 0x1A8578D4                                 */
/* Register default value on SPRC0: 0x1A8578D4                                  */
/* Register default value on SPRUCC: 0x1A8578D4                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_90_GV_CTRL_BIN6_7_11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac840)                                                      */
/*      SPRB0 (0x200ac840)                                                      */
/*      SPRHBM (0x2007c840)                                                     */
/*      SPRC0 (0x200ac840)                                                      */
/*      SPRUCC (0x200dd040)                                                     */
/* Register default value on SPRA0: 0x17C4809E                                  */
/* Register default value on SPRB0: 0x17C4809E                                  */
/* Register default value on SPRHBM: 0x17C4809E                                 */
/* Register default value on SPRC0: 0x17C4809E                                  */
/* Register default value on SPRUCC: 0x17C4809E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_90_GV_CTRL_BIN1_2_3_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac844)                                                      */
/*      SPRB0 (0x200ac844)                                                      */
/*      SPRHBM (0x2007c844)                                                     */
/*      SPRC0 (0x200ac844)                                                      */
/*      SPRUCC (0x200dd044)                                                     */
/* Register default value on SPRA0: 0x01008040                                  */
/* Register default value on SPRB0: 0x01008040                                  */
/* Register default value on SPRHBM: 0x01008040                                 */
/* Register default value on SPRC0: 0x01008040                                  */
/* Register default value on SPRUCC: 0x01008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_90_GV_CTRL_BIN4_5_6_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac848)                                                      */
/*      SPRB0 (0x200ac848)                                                      */
/*      SPRHBM (0x2007c848)                                                     */
/*      SPRC0 (0x200ac848)                                                      */
/*      SPRUCC (0x200dd048)                                                     */
/* Register default value on SPRA0: 0x11008040                                  */
/* Register default value on SPRB0: 0x11008040                                  */
/* Register default value on SPRHBM: 0x11008040                                 */
/* Register default value on SPRC0: 0x11008040                                  */
/* Register default value on SPRUCC: 0x11008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac84c)                                                      */
/*      SPRB0 (0x200ac84c)                                                      */
/*      SPRHBM (0x2007c84c)                                                     */
/*      SPRC0 (0x200ac84c)                                                      */
/*      SPRUCC (0x200dd04c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac84c)                                                      */
/*      SPRB0 (0x200ac84c)                                                      */
/*      SPRHBM (0x2007c84c)                                                     */
/*      SPRC0 (0x200ac84c)                                                      */
/*      SPRUCC (0x200dd04c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac84c)                                                      */
/*      SPRB0 (0x200ac84c)                                                      */
/*      SPRHBM (0x2007c84c)                                                     */
/*      SPRC0 (0x200ac84c)                                                      */
/*      SPRUCC (0x200dd04c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac850)                                                      */
/*      SPRB0 (0x200ac850)                                                      */
/*      SPRHBM (0x2007c850)                                                     */
/*      SPRC0 (0x200ac850)                                                      */
/*      SPRUCC (0x200dd050)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac850)                                                      */
/*      SPRB0 (0x200ac850)                                                      */
/*      SPRHBM (0x2007c850)                                                     */
/*      SPRC0 (0x200ac850)                                                      */
/*      SPRUCC (0x200dd050)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac850)                                                      */
/*      SPRB0 (0x200ac850)                                                      */
/*      SPRHBM (0x2007c850)                                                     */
/*      SPRC0 (0x200ac850)                                                      */
/*      SPRUCC (0x200dd050)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac854)                                                      */
/*      SPRB0 (0x200ac854)                                                      */
/*      SPRHBM (0x2007c854)                                                     */
/*      SPRC0 (0x200ac854)                                                      */
/*      SPRUCC (0x200dd054)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac854)                                                      */
/*      SPRB0 (0x200ac854)                                                      */
/*      SPRHBM (0x2007c854)                                                     */
/*      SPRC0 (0x200ac854)                                                      */
/*      SPRUCC (0x200dd054)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac854)                                                      */
/*      SPRB0 (0x200ac854)                                                      */
/*      SPRHBM (0x2007c854)                                                     */
/*      SPRC0 (0x200ac854)                                                      */
/*      SPRUCC (0x200dd054)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac858)                                                      */
/*      SPRB0 (0x200ac858)                                                      */
/*      SPRHBM (0x2007c858)                                                     */
/*      SPRC0 (0x200ac858)                                                      */
/*      SPRUCC (0x200dd058)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac858)                                                      */
/*      SPRB0 (0x200ac858)                                                      */
/*      SPRHBM (0x2007c858)                                                     */
/*      SPRC0 (0x200ac858)                                                      */
/*      SPRUCC (0x200dd058)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac858)                                                      */
/*      SPRB0 (0x200ac858)                                                      */
/*      SPRHBM (0x2007c858)                                                     */
/*      SPRC0 (0x200ac858)                                                      */
/*      SPRUCC (0x200dd058)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac85c)                                                      */
/*      SPRB0 (0x200ac85c)                                                      */
/*      SPRHBM (0x2007c85c)                                                     */
/*      SPRC0 (0x200ac85c)                                                      */
/*      SPRUCC (0x200dd05c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac85c)                                                      */
/*      SPRB0 (0x200ac85c)                                                      */
/*      SPRHBM (0x2007c85c)                                                     */
/*      SPRC0 (0x200ac85c)                                                      */
/*      SPRUCC (0x200dd05c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_90_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac85c)                                                      */
/*      SPRB0 (0x200ac85c)                                                      */
/*      SPRHBM (0x2007c85c)                                                     */
/*      SPRC0 (0x200ac85c)                                                      */
/*      SPRUCC (0x200dd05c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_90_CONTD2C_DIFF_GV_CTRL_CREG_MDFIS_MAIN_REG supported on:              */
/*      SPRA0 (0x200ac860)                                                      */
/*      SPRB0 (0x200ac860)                                                      */
/*      SPRHBM (0x2007c860)                                                     */
/*      SPRC0 (0x200ac860)                                                      */
/*      SPRUCC (0x200dd060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS MDLL D2C DIFF GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_270_GV_CTRL_BIN0_1_8_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac864)                                                      */
/*      SPRB0 (0x200ac864)                                                      */
/*      SPRHBM (0x2007c864)                                                     */
/*      SPRC0 (0x200ac864)                                                      */
/*      SPRUCC (0x200dd064)                                                     */
/* Register default value on SPRA0: 0x208DD85F                                  */
/* Register default value on SPRB0: 0x208DD85F                                  */
/* Register default value on SPRHBM: 0x208DD85F                                 */
/* Register default value on SPRC0: 0x208DD85F                                  */
/* Register default value on SPRUCC: 0x208DD85F                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_270_GV_CTRL_BIN2_3_9_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac868)                                                      */
/*      SPRB0 (0x200ac868)                                                      */
/*      SPRHBM (0x2007c868)                                                     */
/*      SPRC0 (0x200ac868)                                                      */
/*      SPRUCC (0x200dd068)                                                     */
/* Register default value on SPRA0: 0x1D88193E                                  */
/* Register default value on SPRB0: 0x1D88193E                                  */
/* Register default value on SPRHBM: 0x1D88193E                                 */
/* Register default value on SPRC0: 0x1D88193E                                  */
/* Register default value on SPRUCC: 0x1D88193E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_270_GV_CTRL_BIN4_5_10_CREG_MDFIS_MAIN_REG supported on:                */
/*      SPRA0 (0x200ac86c)                                                      */
/*      SPRB0 (0x200ac86c)                                                      */
/*      SPRHBM (0x2007c86c)                                                     */
/*      SPRC0 (0x200ac86c)                                                      */
/*      SPRUCC (0x200dd06c)                                                     */
/* Register default value on SPRA0: 0x1A8578D4                                  */
/* Register default value on SPRB0: 0x1A8578D4                                  */
/* Register default value on SPRHBM: 0x1A8578D4                                 */
/* Register default value on SPRC0: 0x1A8578D4                                  */
/* Register default value on SPRUCC: 0x1A8578D4                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_270_GV_CTRL_BIN6_7_11_CREG_MDFIS_MAIN_REG supported on:                */
/*      SPRA0 (0x200ac870)                                                      */
/*      SPRB0 (0x200ac870)                                                      */
/*      SPRHBM (0x2007c870)                                                     */
/*      SPRC0 (0x200ac870)                                                      */
/*      SPRUCC (0x200dd070)                                                     */
/* Register default value on SPRA0: 0x17C4809E                                  */
/* Register default value on SPRB0: 0x17C4809E                                  */
/* Register default value on SPRHBM: 0x17C4809E                                 */
/* Register default value on SPRC0: 0x17C4809E                                  */
/* Register default value on SPRUCC: 0x17C4809E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_270_GV_CTRL_BIN1_2_3_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac874)                                                      */
/*      SPRB0 (0x200ac874)                                                      */
/*      SPRHBM (0x2007c874)                                                     */
/*      SPRC0 (0x200ac874)                                                      */
/*      SPRUCC (0x200dd074)                                                     */
/* Register default value on SPRA0: 0x01008040                                  */
/* Register default value on SPRB0: 0x01008040                                  */
/* Register default value on SPRHBM: 0x01008040                                 */
/* Register default value on SPRC0: 0x01008040                                  */
/* Register default value on SPRUCC: 0x01008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_270_GV_CTRL_BIN4_5_6_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac878)                                                      */
/*      SPRB0 (0x200ac878)                                                      */
/*      SPRHBM (0x2007c878)                                                     */
/*      SPRC0 (0x200ac878)                                                      */
/*      SPRUCC (0x200dd078)                                                     */
/* Register default value on SPRA0: 0x11008040                                  */
/* Register default value on SPRB0: 0x11008040                                  */
/* Register default value on SPRHBM: 0x11008040                                 */
/* Register default value on SPRC0: 0x11008040                                  */
/* Register default value on SPRUCC: 0x11008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac87c)                                                      */
/*      SPRB0 (0x200ac87c)                                                      */
/*      SPRHBM (0x2007c87c)                                                     */
/*      SPRC0 (0x200ac87c)                                                      */
/*      SPRUCC (0x200dd07c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac87c)                                                      */
/*      SPRB0 (0x200ac87c)                                                      */
/*      SPRHBM (0x2007c87c)                                                     */
/*      SPRC0 (0x200ac87c)                                                      */
/*      SPRUCC (0x200dd07c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac87c)                                                      */
/*      SPRB0 (0x200ac87c)                                                      */
/*      SPRHBM (0x2007c87c)                                                     */
/*      SPRC0 (0x200ac87c)                                                      */
/*      SPRUCC (0x200dd07c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac880)                                                      */
/*      SPRB0 (0x200ac880)                                                      */
/*      SPRHBM (0x2007c880)                                                     */
/*      SPRC0 (0x200ac880)                                                      */
/*      SPRUCC (0x200dd080)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac880)                                                      */
/*      SPRB0 (0x200ac880)                                                      */
/*      SPRHBM (0x2007c880)                                                     */
/*      SPRC0 (0x200ac880)                                                      */
/*      SPRUCC (0x200dd080)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac880)                                                      */
/*      SPRB0 (0x200ac880)                                                      */
/*      SPRHBM (0x2007c880)                                                     */
/*      SPRC0 (0x200ac880)                                                      */
/*      SPRUCC (0x200dd080)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac884)                                                      */
/*      SPRB0 (0x200ac884)                                                      */
/*      SPRHBM (0x2007c884)                                                     */
/*      SPRC0 (0x200ac884)                                                      */
/*      SPRUCC (0x200dd084)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac884)                                                      */
/*      SPRB0 (0x200ac884)                                                      */
/*      SPRHBM (0x2007c884)                                                     */
/*      SPRC0 (0x200ac884)                                                      */
/*      SPRUCC (0x200dd084)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac884)                                                      */
/*      SPRB0 (0x200ac884)                                                      */
/*      SPRHBM (0x2007c884)                                                     */
/*      SPRC0 (0x200ac884)                                                      */
/*      SPRUCC (0x200dd084)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac888)                                                      */
/*      SPRB0 (0x200ac888)                                                      */
/*      SPRHBM (0x2007c888)                                                     */
/*      SPRC0 (0x200ac888)                                                      */
/*      SPRUCC (0x200dd088)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac888)                                                      */
/*      SPRB0 (0x200ac888)                                                      */
/*      SPRHBM (0x2007c888)                                                     */
/*      SPRC0 (0x200ac888)                                                      */
/*      SPRUCC (0x200dd088)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac888)                                                      */
/*      SPRB0 (0x200ac888)                                                      */
/*      SPRHBM (0x2007c888)                                                     */
/*      SPRC0 (0x200ac888)                                                      */
/*      SPRUCC (0x200dd088)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac88c)                                                      */
/*      SPRB0 (0x200ac88c)                                                      */
/*      SPRHBM (0x2007c88c)                                                     */
/*      SPRC0 (0x200ac88c)                                                      */
/*      SPRUCC (0x200dd08c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac88c)                                                      */
/*      SPRB0 (0x200ac88c)                                                      */
/*      SPRHBM (0x2007c88c)                                                     */
/*      SPRC0 (0x200ac88c)                                                      */
/*      SPRUCC (0x200dd08c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS4_DCC_270_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac88c)                                                      */
/*      SPRB0 (0x200ac88c)                                                      */
/*      SPRHBM (0x2007c88c)                                                     */
/*      SPRC0 (0x200ac88c)                                                      */
/*      SPRUCC (0x200dd08c)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDLLQLS45_270_GVT_OFFSET_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac890)                                                      */
/*      SPRB0 (0x200ac890)                                                      */
/*      SPRHBM (0x2007c890)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* MDFIS DLLQLS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QDS_DCC45_BINARY_SEARCH_CTRL1_CREG_MDFIS_MAIN_REG supported on:              */
/*      SPRC0 (0x200ac890)                                                      */
/*      SPRUCC (0x200dd090)                                                     */
/* Register default value on SPRC0: 0xA8DB9F66                                  */
/* Register default value on SPRUCC: 0xA8DB9F66                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS DCC BINARY SEARCH CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL4_270_CONTD2C_DIFF_GV_CTRL_CREG_MDFIS_MAIN_REG supported on:             */
/*      SPRA0 (0x200ac894)                                                      */
/*      SPRB0 (0x200ac894)                                                      */
/*      SPRHBM (0x2007c894)                                                     */
/*      SPRC0 (0x200ac894)                                                      */
/*      SPRUCC (0x200dd094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS MDLL D2C DIFF GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_270_GV_CTRL_BIN0_1_8_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac898)                                                      */
/*      SPRB0 (0x200ac898)                                                      */
/*      SPRHBM (0x2007c898)                                                     */
/*      SPRC0 (0x200ac898)                                                      */
/*      SPRUCC (0x200dd098)                                                     */
/* Register default value on SPRA0: 0x208DD85F                                  */
/* Register default value on SPRB0: 0x208DD85F                                  */
/* Register default value on SPRHBM: 0x208DD85F                                 */
/* Register default value on SPRC0: 0x208DD85F                                  */
/* Register default value on SPRUCC: 0x208DD85F                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_270_GV_CTRL_BIN2_3_9_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac89c)                                                      */
/*      SPRB0 (0x200ac89c)                                                      */
/*      SPRHBM (0x2007c89c)                                                     */
/*      SPRC0 (0x200ac89c)                                                      */
/*      SPRUCC (0x200dd09c)                                                     */
/* Register default value on SPRA0: 0x1D88193E                                  */
/* Register default value on SPRB0: 0x1D88193E                                  */
/* Register default value on SPRHBM: 0x1D88193E                                 */
/* Register default value on SPRC0: 0x1D88193E                                  */
/* Register default value on SPRUCC: 0x1D88193E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_270_GV_CTRL_BIN4_5_10_CREG_MDFIS_MAIN_REG supported on:                */
/*      SPRA0 (0x200ac8a0)                                                      */
/*      SPRB0 (0x200ac8a0)                                                      */
/*      SPRHBM (0x2007c8a0)                                                     */
/*      SPRC0 (0x200ac8a0)                                                      */
/*      SPRUCC (0x200dd0a0)                                                     */
/* Register default value on SPRA0: 0x1A8578D4                                  */
/* Register default value on SPRB0: 0x1A8578D4                                  */
/* Register default value on SPRHBM: 0x1A8578D4                                 */
/* Register default value on SPRC0: 0x1A8578D4                                  */
/* Register default value on SPRUCC: 0x1A8578D4                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_270_GV_CTRL_BIN6_7_11_CREG_MDFIS_MAIN_REG supported on:                */
/*      SPRA0 (0x200ac8a4)                                                      */
/*      SPRB0 (0x200ac8a4)                                                      */
/*      SPRHBM (0x2007c8a4)                                                     */
/*      SPRC0 (0x200ac8a4)                                                      */
/*      SPRUCC (0x200dd0a4)                                                     */
/* Register default value on SPRA0: 0x17C4809E                                  */
/* Register default value on SPRB0: 0x17C4809E                                  */
/* Register default value on SPRHBM: 0x17C4809E                                 */
/* Register default value on SPRC0: 0x17C4809E                                  */
/* Register default value on SPRUCC: 0x17C4809E                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS MDLL GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_270_GV_CTRL_BIN1_2_3_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8a8)                                                      */
/*      SPRB0 (0x200ac8a8)                                                      */
/*      SPRHBM (0x2007c8a8)                                                     */
/*      SPRC0 (0x200ac8a8)                                                      */
/*      SPRUCC (0x200dd0a8)                                                     */
/* Register default value on SPRA0: 0x01008040                                  */
/* Register default value on SPRB0: 0x01008040                                  */
/* Register default value on SPRHBM: 0x01008040                                 */
/* Register default value on SPRC0: 0x01008040                                  */
/* Register default value on SPRUCC: 0x01008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_270_GV_CTRL_BIN4_5_6_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8ac)                                                      */
/*      SPRB0 (0x200ac8ac)                                                      */
/*      SPRHBM (0x2007c8ac)                                                     */
/*      SPRC0 (0x200ac8ac)                                                      */
/*      SPRUCC (0x200dd0ac)                                                     */
/* Register default value on SPRA0: 0x11008040                                  */
/* Register default value on SPRB0: 0x11008040                                  */
/* Register default value on SPRHBM: 0x11008040                                 */
/* Register default value on SPRC0: 0x11008040                                  */
/* Register default value on SPRUCC: 0x11008040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b0)                                                      */
/*      SPRB0 (0x200ac8b0)                                                      */
/*      SPRHBM (0x2007c8b0)                                                     */
/*      SPRC0 (0x200ac8b0)                                                      */
/*      SPRUCC (0x200dd0b0)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b0)                                                      */
/*      SPRB0 (0x200ac8b0)                                                      */
/*      SPRHBM (0x2007c8b0)                                                     */
/*      SPRC0 (0x200ac8b0)                                                      */
/*      SPRUCC (0x200dd0b0)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN7_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b0)                                                      */
/*      SPRB0 (0x200ac8b0)                                                      */
/*      SPRHBM (0x2007c8b0)                                                     */
/*      SPRC0 (0x200ac8b0)                                                      */
/*      SPRUCC (0x200dd0b0)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b4)                                                      */
/*      SPRB0 (0x200ac8b4)                                                      */
/*      SPRHBM (0x2007c8b4)                                                     */
/*      SPRC0 (0x200ac8b4)                                                      */
/*      SPRUCC (0x200dd0b4)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b4)                                                      */
/*      SPRB0 (0x200ac8b4)                                                      */
/*      SPRHBM (0x2007c8b4)                                                     */
/*      SPRC0 (0x200ac8b4)                                                      */
/*      SPRUCC (0x200dd0b4)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN8_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b4)                                                      */
/*      SPRB0 (0x200ac8b4)                                                      */
/*      SPRHBM (0x2007c8b4)                                                     */
/*      SPRC0 (0x200ac8b4)                                                      */
/*      SPRUCC (0x200dd0b4)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b8)                                                      */
/*      SPRB0 (0x200ac8b8)                                                      */
/*      SPRHBM (0x2007c8b8)                                                     */
/*      SPRC0 (0x200ac8b8)                                                      */
/*      SPRUCC (0x200dd0b8)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b8)                                                      */
/*      SPRB0 (0x200ac8b8)                                                      */
/*      SPRHBM (0x2007c8b8)                                                     */
/*      SPRC0 (0x200ac8b8)                                                      */
/*      SPRUCC (0x200dd0b8)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN9_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8b8)                                                      */
/*      SPRB0 (0x200ac8b8)                                                      */
/*      SPRHBM (0x2007c8b8)                                                     */
/*      SPRC0 (0x200ac8b8)                                                      */
/*      SPRUCC (0x200dd0b8)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac8bc)                                                      */
/*      SPRB0 (0x200ac8bc)                                                      */
/*      SPRHBM (0x2007c8bc)                                                     */
/*      SPRC0 (0x200ac8bc)                                                      */
/*      SPRUCC (0x200dd0bc)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac8bc)                                                      */
/*      SPRB0 (0x200ac8bc)                                                      */
/*      SPRHBM (0x2007c8bc)                                                     */
/*      SPRC0 (0x200ac8bc)                                                      */
/*      SPRUCC (0x200dd0bc)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN10_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac8bc)                                                      */
/*      SPRB0 (0x200ac8bc)                                                      */
/*      SPRHBM (0x2007c8bc)                                                     */
/*      SPRC0 (0x200ac8bc)                                                      */
/*      SPRUCC (0x200dd0bc)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac8c0)                                                      */
/*      SPRB0 (0x200ac8c0)                                                      */
/*      SPRHBM (0x2007c8c0)                                                     */
/*      SPRC0 (0x200ac8c0)                                                      */
/*      SPRUCC (0x200dd0c0)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac8c0)                                                      */
/*      SPRB0 (0x200ac8c0)                                                      */
/*      SPRHBM (0x2007c8c0)                                                     */
/*      SPRC0 (0x200ac8c0)                                                      */
/*      SPRUCC (0x200dd0c0)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS5_DCC_270_GV_CTRL_BIN11_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac8c0)                                                      */
/*      SPRB0 (0x200ac8c0)                                                      */
/*      SPRHBM (0x2007c8c0)                                                     */
/*      SPRC0 (0x200ac8c0)                                                      */
/*      SPRUCC (0x200dd0c0)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x01800040                                  */
/* Register default value on SPRUCC: 0x01800040                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QDS GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDLL5_270_CONTD2C_DIFF_GV_CTRL_CREG_MDFIS_MAIN_REG supported on:             */
/*      SPRA0 (0x200ac8c4)                                                      */
/*      SPRB0 (0x200ac8c4)                                                      */
/*      SPRHBM (0x2007c8c4)                                                     */
/*      SPRC0 (0x200ac8c4)                                                      */
/*      SPRUCC (0x200dd0c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS MDLL D2C DIFF GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* CHNL2_DFX_VIEW_CREG_MDFIS_MAIN_REG supported on:                             */
/*      SPRA0 (0x200ac8c8)                                                      */
/*      SPRB0 (0x200ac8c8)                                                      */
/*      SPRHBM (0x2007c8c8)                                                     */
/*      SPRC0 (0x200ac8c8)                                                      */
/*      SPRUCC (0x200dd0c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS CHNL 2 DFX VIEW
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* DQS_OFFSETCODE_CLSTR4_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac8cc)                                                      */
/*      SPRB0 (0x200ac8cc)                                                      */
/*      SPRHBM (0x2007c8cc)                                                     */
/*      SPRC0 (0x200ac8cc)                                                      */
/*      SPRUCC (0x200dd0cc)                                                     */
/* Register default value on SPRA0: 0x00820820                                  */
/* Register default value on SPRB0: 0x00820820                                  */
/* Register default value on SPRHBM: 0x00820820                                 */
/* Register default value on SPRC0: 0x00820820                                  */
/* Register default value on SPRUCC: 0x00820820                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS DQS OFFSETCODE STICKY REG (PER CLSTR)
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* DQS_OFFSETCODE_CLSTR5_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac8d0)                                                      */
/*      SPRB0 (0x200ac8d0)                                                      */
/*      SPRHBM (0x2007c8d0)                                                     */
/*      SPRC0 (0x200ac8d0)                                                      */
/*      SPRUCC (0x200dd0d0)                                                     */
/* Register default value on SPRA0: 0x00820820                                  */
/* Register default value on SPRB0: 0x00820820                                  */
/* Register default value on SPRHBM: 0x00820820                                 */
/* Register default value on SPRC0: 0x00820820                                  */
/* Register default value on SPRUCC: 0x00820820                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS DQS OFFSETCODE STICKY REG (PER CLSTR)
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* AFEA_PERCHNL_CTRL_CHNL2_CREG_MDFIS_MAIN_REG supported on:                    */
/*      SPRA0 (0x200ac8d4)                                                      */
/*      SPRB0 (0x200ac8d4)                                                      */
/*      SPRHBM (0x2007c8d4)                                                     */
/*      SPRC0 (0x200ac8d4)                                                      */
/*      SPRUCC (0x200dd0d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS AFEA PERCHNL CTRL
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* AFEA_PERCHNL_CTRL1_CHNL2_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac8d8)                                                      */
/*      SPRB0 (0x200ac8d8)                                                      */
/*      SPRHBM (0x2007c8d8)                                                     */
/*      SPRC0 (0x200ac8d8)                                                      */
/*      SPRUCC (0x200dd0d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS AFEA PERCHNL CTRL1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* ACCUM_ERRORS_STATUS_CHNL2_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac8dc)                                                      */
/*      SPRB0 (0x200ac8dc)                                                      */
/*      SPRHBM (0x2007c8dc)                                                     */
/*      SPRC0 (0x200ac8dc)                                                      */
/*      SPRUCC (0x200dd0dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS ACCUM ERRORS STATUS REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* AFEA_TXRX_COMMON_CTRL_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac8e0)                                                      */
/*      SPRB0 (0x200ac8e0)                                                      */
/*      SPRHBM (0x2007c8e0)                                                     */
/*      SPRC0 (0x200ac8e0)                                                      */
/*      SPRUCC (0x200dd0e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS AFEA TXRX COMMON CTRL
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* AFEA_TXRX_COMMON_CTRL1_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac8e4)                                                      */
/*      SPRB0 (0x200ac8e4)                                                      */
/*      SPRHBM (0x2007c8e4)                                                     */
/*      SPRC0 (0x200ac8e4)                                                      */
/*      SPRUCC (0x200dd0e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS AFEA TXRX COMMON CTRL1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* QLS_FIXED_OFFSET_GV_CTRL_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac8e8)                                                      */
/*      SPRB0 (0x200ac8e8)                                                      */
/*      SPRHBM (0x2007c8e8)                                                     */
/*      SPRC0 (0x200ac8e8)                                                      */
/*      SPRUCC (0x200dd0e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS QLS FIXED OFFSET GV CTRL
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_GLOBAL_CREG_MDFIS_MAIN_REG supported on:                               */
/*      SPRA0 (0x200ac8ec)                                                      */
/*      SPRB0 (0x200ac8ec)                                                      */
/*      SPRHBM (0x2007c8ec)                                                     */
/*      SPRC0 (0x200ac8ec)                                                      */
/*      SPRUCC (0x200dd0ec)                                                     */
/* Register default value on SPRA0: 0x000002E8                                  */
/* Register default value on SPRB0: 0x000002E8                                  */
/* Register default value on SPRHBM: 0x000002E8                                 */
/* Register default value on SPRC0: 0x000002E8                                  */
/* Register default value on SPRUCC: 0x000002E8                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS GLOBAL register
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* COLDBOOT_XRATIOS_CTRL0_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac8f0)                                                      */
/*      SPRB0 (0x200ac8f0)                                                      */
/*      SPRHBM (0x2007c8f0)                                                     */
/*      SPRC0 (0x200ac8f0)                                                      */
/*      SPRUCC (0x200dd0f0)                                                     */
/* Register default value on SPRA0: 0x2B38BD89                                  */
/* Register default value on SPRB0: 0x2B38BD89                                  */
/* Register default value on SPRHBM: 0x2B38BD89                                 */
/* Register default value on SPRC0: 0x2B38BD89                                  */
/* Register default value on SPRUCC: 0x2B38BD89                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS SHARED BINS COLDBOOT XRATIO
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* COLDBOOT_XRATIOS_CTRL1_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac8f4)                                                      */
/*      SPRB0 (0x200ac8f4)                                                      */
/*      SPRHBM (0x2007c8f4)                                                     */
/*      SPRC0 (0x200ac8f4)                                                      */
/*      SPRUCC (0x200dd0f4)                                                     */
/* Register default value on SPRA0: 0x37ACE2F6                                  */
/* Register default value on SPRB0: 0x37ACE2F6                                  */
/* Register default value on SPRHBM: 0x37ACE2F6                                 */
/* Register default value on SPRC0: 0x37ACE2F6                                  */
/* Register default value on SPRUCC: 0x37ACE2F6                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS SHARED BINS COLDBOOT XRATIO
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_RCOMP_CODE_CREG_MDFIS_MAIN_REG supported on:                           */
/*      SPRA0 (0x200ac8f8)                                                      */
/*      SPRB0 (0x200ac8f8)                                                      */
/*      SPRHBM (0x2007c8f8)                                                     */
/*      SPRC0 (0x200ac8f8)                                                      */
/*      SPRUCC (0x200dd0f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS QLS FIXED OFFSET GV CTRL
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA_UISEL_CTRL_CHNL2_CREG_MDFIS_MAIN_REG supported on:                        */
/*      SPRA0 (0x200ac8fc)                                                      */
/*      SPRB0 (0x200ac8fc)                                                      */
/*      SPRHBM (0x2007c8fc)                                                     */
/*      SPRC0 (0x200ac8fc)                                                      */
/*      SPRUCC (0x200dd0fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS_PA UISEL CTRL
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA4_GV_CTRL_BIN0_1_8_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac900)                                                      */
/*      SPRB0 (0x200ac900)                                                      */
/*      SPRHBM (0x2007c900)                                                     */
/*      SPRC0 (0x200ac900)                                                      */
/*      SPRUCC (0x200dd100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA4_GV_CTRL_BIN2_3_9_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac904)                                                      */
/*      SPRB0 (0x200ac904)                                                      */
/*      SPRHBM (0x2007c904)                                                     */
/*      SPRC0 (0x200ac904)                                                      */
/*      SPRUCC (0x200dd104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA4_GV_CTRL_BIN4_5_10_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac908)                                                      */
/*      SPRB0 (0x200ac908)                                                      */
/*      SPRHBM (0x2007c908)                                                     */
/*      SPRC0 (0x200ac908)                                                      */
/*      SPRUCC (0x200dd108)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA4_GV_CTRL_BIN6_7_11_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac90c)                                                      */
/*      SPRB0 (0x200ac90c)                                                      */
/*      SPRHBM (0x2007c90c)                                                     */
/*      SPRC0 (0x200ac90c)                                                      */
/*      SPRUCC (0x200dd10c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA4_DCC_GV_CTRL_BIN6_7_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac910)                                                      */
/*      SPRB0 (0x200ac910)                                                      */
/*      SPRHBM (0x2007c910)                                                     */
/*      SPRC0 (0x200ac910)                                                      */
/*      SPRUCC (0x200dd110)                                                     */
/* Register default value on SPRA0: 0x10281028                                  */
/* Register default value on SPRB0: 0x10281028                                  */
/* Register default value on SPRHBM: 0x10281028                                 */
/* Register default value on SPRC0: 0x10201020                                  */
/* Register default value on SPRUCC: 0x10201020                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA DCC DCA GV CTRL REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA4_DCC_GV_CTRL_BIN8_9_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac914)                                                      */
/*      SPRB0 (0x200ac914)                                                      */
/*      SPRHBM (0x2007c914)                                                     */
/*      SPRC0 (0x200ac914)                                                      */
/*      SPRUCC (0x200dd114)                                                     */
/* Register default value on SPRA0: 0x10281028                                  */
/* Register default value on SPRB0: 0x10281028                                  */
/* Register default value on SPRHBM: 0x10281028                                 */
/* Register default value on SPRC0: 0x10201020                                  */
/* Register default value on SPRUCC: 0x10201020                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA DCC DCA GV CTRL REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA4_DCC_GV_CTRL_BIN10_11_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac918)                                                      */
/*      SPRB0 (0x200ac918)                                                      */
/*      SPRHBM (0x2007c918)                                                     */
/*      SPRC0 (0x200ac918)                                                      */
/*      SPRUCC (0x200dd118)                                                     */
/* Register default value on SPRA0: 0x10281028                                  */
/* Register default value on SPRB0: 0x10281028                                  */
/* Register default value on SPRHBM: 0x10281028                                 */
/* Register default value on SPRC0: 0x10201020                                  */
/* Register default value on SPRUCC: 0x10201020                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA DCC DCA GV CTRL REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA5_GV_CTRL_BIN0_1_8_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac91c)                                                      */
/*      SPRB0 (0x200ac91c)                                                      */
/*      SPRHBM (0x2007c91c)                                                     */
/*      SPRC0 (0x200ac91c)                                                      */
/*      SPRUCC (0x200dd11c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA5_GV_CTRL_BIN2_3_9_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac920)                                                      */
/*      SPRB0 (0x200ac920)                                                      */
/*      SPRHBM (0x2007c920)                                                     */
/*      SPRC0 (0x200ac920)                                                      */
/*      SPRUCC (0x200dd120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA5_GV_CTRL_BIN4_5_10_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac924)                                                      */
/*      SPRB0 (0x200ac924)                                                      */
/*      SPRHBM (0x2007c924)                                                     */
/*      SPRC0 (0x200ac924)                                                      */
/*      SPRUCC (0x200dd124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA5_GV_CTRL_BIN6_7_11_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac928)                                                      */
/*      SPRB0 (0x200ac928)                                                      */
/*      SPRHBM (0x2007c928)                                                     */
/*      SPRC0 (0x200ac928)                                                      */
/*      SPRUCC (0x200dd128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA GV CTRL REG
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA5_DCC_GV_CTRL_BIN6_7_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac92c)                                                      */
/*      SPRB0 (0x200ac92c)                                                      */
/*      SPRHBM (0x2007c92c)                                                     */
/*      SPRC0 (0x200ac92c)                                                      */
/*      SPRUCC (0x200dd12c)                                                     */
/* Register default value on SPRA0: 0x10281028                                  */
/* Register default value on SPRB0: 0x10281028                                  */
/* Register default value on SPRHBM: 0x10281028                                 */
/* Register default value on SPRC0: 0x10201020                                  */
/* Register default value on SPRUCC: 0x10201020                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA DCC DCA GV CTRL REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA5_DCC_GV_CTRL_BIN8_9_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac930)                                                      */
/*      SPRB0 (0x200ac930)                                                      */
/*      SPRHBM (0x2007c930)                                                     */
/*      SPRC0 (0x200ac930)                                                      */
/*      SPRUCC (0x200dd130)                                                     */
/* Register default value on SPRA0: 0x10281028                                  */
/* Register default value on SPRB0: 0x10281028                                  */
/* Register default value on SPRHBM: 0x10281028                                 */
/* Register default value on SPRC0: 0x10201020                                  */
/* Register default value on SPRUCC: 0x10201020                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA DCC DCA GV CTRL REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PA5_DCC_GV_CTRL_BIN10_11_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac934)                                                      */
/*      SPRB0 (0x200ac934)                                                      */
/*      SPRHBM (0x2007c934)                                                     */
/*      SPRC0 (0x200ac934)                                                      */
/*      SPRUCC (0x200dd134)                                                     */
/* Register default value on SPRA0: 0x10281028                                  */
/* Register default value on SPRB0: 0x10281028                                  */
/* Register default value on SPRHBM: 0x10281028                                 */
/* Register default value on SPRC0: 0x10201020                                  */
/* Register default value on SPRUCC: 0x10201020                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS PA DCC DCA GV CTRL REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_SCRATCHPAD_CREG_MDFIS_MAIN_REG supported on:                           */
/*      SPRA0 (0x200ac938)                                                      */
/*      SPRB0 (0x200ac938)                                                      */
/*      SPRHBM (0x2007c938)                                                     */
/*      SPRC0 (0x200ac938)                                                      */
/*      SPRUCC (0x200dd138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFI SCRATCHPAD
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_FSM_CTRL0_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac93c)                                                      */
/*      SPRB0 (0x200ac93c)                                                      */
/*      SPRHBM (0x2007c93c)                                                     */
/*      SPRC0 (0x200ac93c)                                                      */
/*      SPRUCC (0x200dd13c)                                                     */
/* Register default value on SPRA0: 0x00800008                                  */
/* Register default value on SPRB0: 0x00800008                                  */
/* Register default value on SPRHBM: 0x00800008                                 */
/* Register default value on SPRC0: 0x00800008                                  */
/* Register default value on SPRUCC: 0x00800008                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS TRAINING FSM CTRL0
*/


#define MDFIS_TRAINING_FSM_CTRL0_CREG_MDFIS_MAIN_REG 0x290A004D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cold_boot_done : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Set to indicate whenever Cold Boot is done for
                               however many GV bins.[br] Auto-VT counter is
                               enabled after this is set and Training clock
                               gating is allowed.
                            */
    UINT32 sequence_reg_sel : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Sequence Register Select to indicate the flow of
                               training wanted. [br] 00=seq_reg0 (Full
                               Training) [br] 01=seq_reg1 (Partial Training)
                               [br] 10=seq_reg2 (BIOS_Assisted) [br]
                               11=seq_reg3 (spare)
                            */
    UINT32 step_size : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               Step size to increment sweep code iterations by.
                               Must be 1 or greater.
                            */
    UINT32 idle_st_cnt : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of additional x4clk cycles to extend the
                               'IDLE' state of the STATE_FSM by.
                            */
    UINT32 init_st_cnt : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of additional x4clk cycles to extend the
                               'INIT' state of the STATE_FSM by.
                            */
    UINT32 log_st_cnt : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of additional x4clk cycles to extend the
                               'LOG' state of the STATE_FSM by.
                            */
    UINT32 end_st_cnt : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of additional x4clk cycles to extend the
                               'END' state of the STATE_FSM by.
                            */
    UINT32 done_st_cnt : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of additional x4clk cycles to extend the
                               'DONE' state of the STATE_FSM by.
                            */
    UINT32 force_training_start : 1;

                            /* Bits[16:16], Access Type=RW/V, default=0x00000000*/

                            /*
                               Manually kicks off entire Training flow vs.
                               relying on subsystem dll_lock_req trigger.
                               Initial setting to 1 creates a trigger pulse
                               within the design.[br] This bit is automatically
                               self clearing by HW after assertion.[br]. To
                               ensure assertion performed as intended, can
                               check the Training FSM status bits.
                            */
    UINT32 enable_graceful_abort : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, allows for a more Graceful Abort out
                               of an Auto-VT Training Flow upon an incoming
                               dll_lock_req at the cost of requiring more time.
                               [br] This means that the STATE_FSM will be
                               allowed to naturally finish a current Phase to
                               completion, vs. immediately aborting out. [br]
                               NOTE: USE OF THIS FEATURE IS RISKY. There are
                               cornercase conditions in Abort that allow two
                               dies Training FSMs to get wildly out of sync
                               with each other.
                            */
    UINT32 phase_halt_exit : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               Exits out of a HALT for the PHASE_FSM when
                               programmed into a sequence_reg. This bit is
                               automatically self clearing by HW after
                               assertion.[br] To ensure assertion performed as
                               intended, can check the PHASE_FSM state status
                               bits.
                            */
    UINT32 halt_init_st : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Halts at the start of the 'INIT' state of the
                               STATE_FSM.
                            */
    UINT32 halt_run_st : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Halts at the start of the 'RUN' state of the
                               STATE_FSM.
                            */
    UINT32 halt_log_st : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Halts at the start of the 'LOG' state of the
                               STATE_FSM.
                            */
    UINT32 halt_end_st : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Halts at the start of the 'END' state of the
                               STATE_FSM.
                            */
    UINT32 halt_nxt_state_ptr : 4;

                            /* Bits[26:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               Pointer to what the next state should be in
                               STATE_FSM when exiting from a halt. Some valid
                               values for initial debug are:[br] '0001' = INIT
                               [br] '0011' = RUN for non-LFSR based phases [br]
                               '0100' = RUN for LFSR based phases [br] '0101' =
                               LOG [br] '0110' = END [br]
                            */
    UINT32 state_halt_exit : 1;

                            /* Bits[27:27], Access Type=RW/V, default=0x00000000*/

                            /*
                               Exits out of a HALT for the STATE_FSM and goes
                               to the next state specified by
                               halt_nxt_state_ptr.[br] This bit is
                               automatically self clearing by HW after
                               assertion.[br] To ensure assertion performed as
                               intended, can check the STATE_FSM state status
                               bits.
                            */
    UINT32 force_die2die_handshake : 1;

                            /* Bits[28:28], Access Type=RW, default=None*/

                            /*
                               Force the Training FSMs to adhere to die2die
                               req/ack handshake and ignore any bypassing.[br]
                               Used in debug scenario where rest of IP is in a
                               type of SORT mode, but still want FSMs to
                               perform the handshake.
                            */
    UINT32 disable_new_code_fifo_rst : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, disables the Training resetting of
                               FIFOs upon every new sweep code.
                            */
    UINT32 reserved : 2;

                            /* Bits[31:30], Access Type=RW, default=None*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} MDFIS_TRAINING_FSM_CTRL0_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac940)                                                      */
/*      SPRB0 (0x200ac940)                                                      */
/*      SPRHBM (0x2007c940)                                                     */
/*      SPRC0 (0x200ac940)                                                      */
/*      SPRUCC (0x200dd140)                                                     */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRHBM: 0x00000005                                 */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS STANDALONE SEQUENCE REG2
*/


#define MDFIS_STANDALONE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_REG 0x290A004E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 standalone_sequence_reg2 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000005*/

                            /*
                               Intended for BIOS_Assisted valid standalone
                               phases. Phases run in order from LSB to MSB,
                               with each nibble a specific phase. Illegal
                               phases decoded to NULL. [br] 0x0 = NULL [br] 0x1
                               = RX_OFFSET_EVEN [br] 0x2 = RX_OFFSET_ODD [br]
                               0x3 = TX_RCOMP [br] 0x4 = TX_PA [br] 0x5 =
                               TX_DCC [br] 0x6 = RX_OFFSET_DQS [br] 0x7 =
                               TX_NELB [br] 0x8 = RX_NELB [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_STANDALONE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac944)                                                      */
/*      SPRB0 (0x200ac944)                                                      */
/*      SPRHBM (0x2007c944)                                                     */
/*      SPRC0 (0x200ac944)                                                      */
/*      SPRUCC (0x200dd144)                                                     */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRHBM: 0x00000005                                 */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS STANDALONE SEQUENCE REG2
*/


#define MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_REG 0x290A004F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 standalone_sequence_reg2 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000005*/

                            /*
                               Intended for BIOS_Assisted valid standalone
                               phases. Phases run in order from LSB to MSB,
                               with each nibble a specific phase. Illegal
                               phases decoded to NULL. [br] 0x0 = NULL [br] 0x1
                               = RX_OFFSET_EVEN [br] 0x2 = RX_OFFSET_ODD [br]
                               0x3 = TX_RCOMP [br] 0x4 = TX_PA [br] 0x5 =
                               TX_DCC [br] 0x6 = RX_OFFSET_DQS [br] 0x7 =
                               TX_NELB [br] 0x8 = RX_NELB [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG3_LOWER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac948)                                                      */
/*      SPRB0 (0x200ac948)                                                      */
/*      SPRHBM (0x2007c948)                                                     */
/*      SPRC0 (0x200ac948)                                                      */
/*      SPRUCC (0x200dd148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS STANDALONE SEQUENCE REG3
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG3_UPPER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac94c)                                                      */
/*      SPRB0 (0x200ac94c)                                                      */
/*      SPRHBM (0x2007c94c)                                                     */
/*      SPRC0 (0x200ac94c)                                                      */
/*      SPRUCC (0x200dd14c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS STANDALONE SEQUENCE REG3
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac950)                                                      */
/*      SPRB0 (0x200ac950)                                                      */
/*      SPRHBM (0x2007c950)                                                     */
/*      SPRC0 (0x200ac950)                                                      */
/*      SPRUCC (0x200dd150)                                                     */
/* Register default value on SPRA0: 0x00CCCE8C                                  */
/* Register default value on SPRB0: 0x00CCCE8C                                  */
/* Register default value on SPRHBM: 0x00CCCE8C                                 */
/* Register default value on SPRC0: 0x00CCCE8C                                  */
/* Register default value on SPRUCC: 0x00CCCE8C                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS DIE2DIE SEQUENCE REG2
*/


#define MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_REG 0x290A0052

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 die2die_sequence_reg2 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00CCCE8C*/

                            /*
                               Intended for BIOS_Assisted valid die2die phases.
                               Phases run in order from LSB to MSB, with each
                               nibble a specific phase. Illegal phases decoded
                               to NULL. [br] 0x0 = NULL [br] 0x8 = RX_VREF [br]
                               0xB = IN_LOCK_WINDOW [br] 0xC = QDS_SLOOP [br]
                               0xE = D2C_LLOOP [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac954)                                                      */
/*      SPRB0 (0x200ac954)                                                      */
/*      SPRHBM (0x2007c954)                                                     */
/*      SPRC0 (0x200ac954)                                                      */
/*      SPRUCC (0x200dd154)                                                     */
/* Register default value on SPRA0: 0x00CCCE8C                                  */
/* Register default value on SPRB0: 0x00CCCE8C                                  */
/* Register default value on SPRHBM: 0x00CCCE8C                                 */
/* Register default value on SPRC0: 0x00CCCE8C                                  */
/* Register default value on SPRUCC: 0x00CCCE8C                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS DIE2DIE SEQUENCE REG2
*/


#define MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_REG 0x290A0053

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 die2die_sequence_reg2 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00CCCE8C*/

                            /*
                               Intended for BIOS_Assisted valid die2die phases.
                               Phases run in order from LSB to MSB, with each
                               nibble a specific phase. Illegal phases decoded
                               to NULL. [br] 0x0 = NULL [br] 0x8 = RX_VREF [br]
                               0xB = IN_LOCK_WINDOW [br] 0xC = QDS_SLOOP [br]
                               0xE = D2C_LLOOP [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG3_LOWER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac958)                                                      */
/*      SPRB0 (0x200ac958)                                                      */
/*      SPRHBM (0x2007c958)                                                     */
/*      SPRC0 (0x200ac958)                                                      */
/*      SPRUCC (0x200dd158)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS DIE2DIE SEQUENCE REG3
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG3_UPPER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac95c)                                                      */
/*      SPRB0 (0x200ac95c)                                                      */
/*      SPRHBM (0x2007c95c)                                                     */
/*      SPRC0 (0x200ac95c)                                                      */
/*      SPRUCC (0x200dd15c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MDFIS DIE2DIE SEQUENCE REG3
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_REG supported on: */
/*      SPRA0 (0x200ac960)                                                      */
/*      SPRB0 (0x200ac960)                                                      */
/*      SPRHBM (0x2007c960)                                                     */
/*      SPRC0 (0x200ac960)                                                      */
/*      SPRUCC (0x200dd160)                                                     */
/* Register default value on SPRA0: 0x00053216                                  */
/* Register default value on SPRB0: 0x00053216                                  */
/* Register default value on SPRHBM: 0x00053216                                 */
/* Register default value on SPRC0: 0x00053216                                  */
/* Register default value on SPRUCC: 0x00053216                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS STANDALONE SEQUENCE HI GV REG0
*/


#define MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_REG 0x290A0056

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 standalone_sequence_hi_gv_reg0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00053216*/

                            /*
                               Intended for High GV Full_Training valid
                               standalone phases. Phases run in order from LSB
                               to MSB, with each nibble a specific phase.
                               Illegal phases decoded to NULL. [br] 0x0 = NULL
                               [br] 0x1 = RX_OFFSET_EVEN [br] 0x2 =
                               RX_OFFSET_ODD [br] 0x3 = TX_RCOMP [br] 0x4 =
                               TX_PA [br] 0x5 = TX_DCC [br] 0x6 = RX_OFFSET_DQS
                               [br] 0x7 = TX_NELB [br] 0x8 = RX_NELB [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_REG supported on: */
/*      SPRA0 (0x200ac964)                                                      */
/*      SPRB0 (0x200ac964)                                                      */
/*      SPRHBM (0x2007c964)                                                     */
/*      SPRC0 (0x200ac964)                                                      */
/*      SPRUCC (0x200dd164)                                                     */
/* Register default value on SPRA0: 0x00053216                                  */
/* Register default value on SPRB0: 0x00053216                                  */
/* Register default value on SPRHBM: 0x00053216                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS STANDALONE SEQUENCE HI GV REG0
*/


#define MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_REG 0x290A0057

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 standalone_sequence_hi_gv_reg0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00053216*/

                            /*
                               Intended for High GV Full_Training valid
                               standalone phases. Phases run in order from LSB
                               to MSB, with each nibble a specific phase.
                               Illegal phases decoded to NULL. [br] 0x0 = NULL
                               [br] 0x1 = RX_OFFSET_EVEN [br] 0x2 =
                               RX_OFFSET_ODD [br] 0x3 = TX_RCOMP [br] 0x4 =
                               TX_PA [br] 0x5 = TX_DCC [br] 0x6 = RX_OFFSET_DQS
                               [br] 0x7 = TX_NELB [br] 0x8 = RX_NELB [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_REG supported on: */
/*      SPRA0 (0x200ac968)                                                      */
/*      SPRB0 (0x200ac968)                                                      */
/*      SPRHBM (0x2007c968)                                                     */
/*      SPRC0 (0x200ac968)                                                      */
/*      SPRUCC (0x200dd168)                                                     */
/* Register default value on SPRA0: 0x00000053                                  */
/* Register default value on SPRB0: 0x00000053                                  */
/* Register default value on SPRHBM: 0x00000053                                 */
/* Register default value on SPRC0: 0x00000053                                  */
/* Register default value on SPRUCC: 0x00000053                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS STANDALONE SEQUENCE HI GV REG1
*/


#define MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_REG 0x290A0058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 standalone_sequence_hi_gv_reg1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000053*/

                            /*
                               Intended for High GV Partial_Training valid
                               standalone phases. Phases run in order from LSB
                               to MSB, with each nibble a specific phase.
                               Illegal phases decoded to NULL. [br] 0x0 = NULL
                               [br] 0x1 = RX_OFFSET_EVEN [br] 0x2 =
                               RX_OFFSET_ODD [br] 0x3 = TX_RCOMP [br] 0x4 =
                               TX_PA [br] 0x5 = TX_DCC [br] 0x6 = RX_OFFSET_DQS
                               [br] 0x7 = TX_NELB [br] 0x8 = RX_NELB [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_REG supported on: */
/*      SPRA0 (0x200ac96c)                                                      */
/*      SPRB0 (0x200ac96c)                                                      */
/*      SPRHBM (0x2007c96c)                                                     */
/*      SPRC0 (0x200ac96c)                                                      */
/*      SPRUCC (0x200dd16c)                                                     */
/* Register default value on SPRA0: 0x00000053                                  */
/* Register default value on SPRB0: 0x00000053                                  */
/* Register default value on SPRHBM: 0x00000053                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS STANDALONE SEQUENCE HI GV REG1
*/


#define MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_REG 0x290A0059

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 standalone_sequence_hi_gv_reg1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000053*/

                            /*
                               Intended for High GV Partial_Training valid
                               standalone phases. Phases run in order from LSB
                               to MSB, with each nibble a specific phase.
                               Illegal phases decoded to NULL. [br] 0x0 = NULL
                               [br] 0x1 = RX_OFFSET_EVEN [br] 0x2 =
                               RX_OFFSET_ODD [br] 0x3 = TX_RCOMP [br] 0x4 =
                               TX_PA [br] 0x5 = TX_DCC [br] 0x6 = RX_OFFSET_DQS
                               [br] 0x7 = TX_NELB [br] 0x8 = RX_NELB [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_REG supported on:    */
/*      SPRA0 (0x200ac970)                                                      */
/*      SPRB0 (0x200ac970)                                                      */
/*      SPRHBM (0x2007c970)                                                     */
/*      SPRC0 (0x200ac970)                                                      */
/*      SPRUCC (0x200dd170)                                                     */
/* Register default value on SPRA0: 0x0CCCCECB                                  */
/* Register default value on SPRB0: 0x0CCCCECB                                  */
/* Register default value on SPRHBM: 0x0CCCCECB                                 */
/* Register default value on SPRC0: 0x0CCCCECB                                  */
/* Register default value on SPRUCC: 0x0CCCCECB                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS DIE2DIE SEQUENCE HI GV REG0
*/


#define MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_REG 0x290A005A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 die2die_sequence_hi_gv_reg0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x0CCCCECB*/

                            /*
                               Intended for High GV Full_Training valid die2die
                               phases. Phases run in order from LSB to MSB,
                               with each nibble a specific phase. Illegal
                               phases decoded to NULL. [br] 0x0 = NULL [br] 0x8
                               = RX_VREF [br] 0xB = IN_LOCK_WINDOW [br] 0xC =
                               QDS_SLOOP [br] 0xE = D2C_LLOOP [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_REG supported on:    */
/*      SPRA0 (0x200ac974)                                                      */
/*      SPRB0 (0x200ac974)                                                      */
/*      SPRHBM (0x2007c974)                                                     */
/*      SPRC0 (0x200ac974)                                                      */
/*      SPRUCC (0x200dd174)                                                     */
/* Register default value on SPRA0: 0x0CCCCECB                                  */
/* Register default value on SPRB0: 0x0CCCCECB                                  */
/* Register default value on SPRHBM: 0x0CCCCECB                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS DIE2DIE SEQUENCE HI GV REG0
*/


#define MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_REG 0x290A005B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 die2die_sequence_hi_gv_reg0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x0CCCCECB*/

                            /*
                               Intended for High GV Full_Training valid die2die
                               phases. Phases run in order from LSB to MSB,
                               with each nibble a specific phase. Illegal
                               phases decoded to NULL. [br] 0x0 = NULL [br] 0x8
                               = RX_VREF [br] 0xB = IN_LOCK_WINDOW [br] 0xC =
                               QDS_SLOOP [br] 0xE = D2C_LLOOP [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_REG supported on:    */
/*      SPRA0 (0x200ac978)                                                      */
/*      SPRB0 (0x200ac978)                                                      */
/*      SPRHBM (0x2007c978)                                                     */
/*      SPRC0 (0x200ac978)                                                      */
/*      SPRUCC (0x200dd178)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS DIE2DIE SEQUENCE HI GV REG1
*/


#define MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_REG 0x290A005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 die2die_sequence_hi_gv_reg1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Intended for High GV Partial_Training valid
                               die2die phases. Phases run in order from LSB to
                               MSB, with each nibble a specific phase. Illegal
                               phases decoded to NULL. [br] 0x0 = NULL [br] 0x8
                               = RX_VREF [br] 0xB = IN_LOCK_WINDOW [br] 0xC =
                               QDS_SLOOP [br] 0xE = D2C_LLOOP [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_REG supported on:    */
/*      SPRA0 (0x200ac97c)                                                      */
/*      SPRB0 (0x200ac97c)                                                      */
/*      SPRHBM (0x2007c97c)                                                     */
/*      SPRC0 (0x200ac97c)                                                      */
/*      SPRUCC (0x200dd17c)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFIS DIE2DIE SEQUENCE HI GV REG1
*/


#define MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_REG 0x290A005D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 die2die_sequence_hi_gv_reg1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Intended for High GV Partial_Training valid
                               die2die phases. Phases run in order from LSB to
                               MSB, with each nibble a specific phase. Illegal
                               phases decoded to NULL. [br] 0x0 = NULL [br] 0x8
                               = RX_VREF [br] 0xB = IN_LOCK_WINDOW [br] 0xC =
                               QDS_SLOOP [br] 0xE = D2C_LLOOP [br]
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_RXOFF_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac980)                                                      */
/*      SPRB0 (0x200ac980)                                                      */
/*      SPRHBM (0x2007c980)                                                     */
/*      SPRC0 (0x200ac980)                                                      */
/*      SPRUCC (0x200dd180)                                                     */
/* Register default value on SPRA0: 0x0C1F7F00                                  */
/* Register default value on SPRB0: 0x0C1F7F00                                  */
/* Register default value on SPRHBM: 0x0C1F7F00                                 */
/* Register default value on SPRC0: 0x0C1F7F00                                  */
/* Register default value on SPRUCC: 0x0C1F7F00                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI RXOFF PHASE CTRL
*/


#define MDFIS_RXOFF_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A005E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_code : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Start code of the sweep. Start code must be less
                               than end code.
                            */
    UINT32 end_code : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x0000007F*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[25:16], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */
    UINT32 partial_end_code : 6;

                            /* Bits[31:26], Access Type=RW/P, default=0x00000003*/

                            /*
                               Partial End code of the sweep. End code must be
                               greater than start code.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_RXOFF_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_RCOMP_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac984)                                                      */
/*      SPRB0 (0x200ac984)                                                      */
/*      SPRHBM (0x2007c984)                                                     */
/*      SPRC0 (0x200ac984)                                                      */
/*      SPRUCC (0x200dd184)                                                     */
/* Register default value on SPRA0: 0x001A8026                                  */
/* Register default value on SPRB0: 0x001A8026                                  */
/* Register default value on SPRHBM: 0x001A8026                                 */
/* Register default value on SPRC0: 0x001A8026                                  */
/* Register default value on SPRUCC: 0x001A8026                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI RCOMP PHASE CTRL
*/


#define MDFIS_RCOMP_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A005F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 test_length : 10;

                            /* Bits[9:0], Access Type=RW/P, default=0x00000026*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */
    UINT32 rcomp_code_override : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* Code update override */
    UINT32 rcomp_num_idle_cycles : 5;

                            /* Bits[15:11], Access Type=RW/P, default=0x00000010*/

                            /* Idle cycle between two rcomp sample */
    UINT32 rcomp_num_samples : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000002*/

                            /* Total number of sample at a time */
    UINT32 rcomp_trn_threshold : 5;

                            /* Bits[23:19], Access Type=RW/P, default=0x00000003*/

                            /* Threshold value for RCOMP training 1 detection */
    UINT32 reserved : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} MDFIS_RCOMP_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_ILW_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac988)                                                      */
/*      SPRB0 (0x200ac988)                                                      */
/*      SPRHBM (0x2007c988)                                                     */
/*      SPRC0 (0x200ac988)                                                      */
/*      SPRUCC (0x200dd188)                                                     */
/* Register default value on SPRA0: 0x0500F800                                  */
/* Register default value on SPRB0: 0x0500F800                                  */
/* Register default value on SPRHBM: 0x0500F800                                 */
/* Register default value on SPRC0: 0x0500F800                                  */
/* Register default value on SPRUCC: 0x0500F800                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI ILW PHASE CTRL
*/


#define MDFIS_ILW_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_code : 11;

                            /* Bits[10:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Start code of the sweep. Start code must be less
                               than end code.
                            */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x0000001F*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000014*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_ILW_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac98c)                                                      */
/*      SPRB0 (0x200ac98c)                                                      */
/*      SPRHBM (0x2007c98c)                                                     */
/*      SPRC0 (0x200ac98c)                                                      */
/*      SPRUCC (0x200dd18c)                                                     */
/* Register default value on SPRA0: 0x0647F83F                                  */
/* Register default value on SPRB0: 0x0647F83F                                  */
/* Register default value on SPRHBM: 0x0647F83F                                 */
/* Register default value on SPRC0: 0x0640A7CC                                  */
/* Register default value on SPRUCC: 0x0640A7CC                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI DCC PHASE CTRL
*/


#define MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0061

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 partial_end_code : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x0000003F*/

                            /*
                               Partial End code of the sweep. End code must be
                               less than end code.
                            */
    UINT32 reserved : 4;

                            /* Bits[10:7], Access Type=RW/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x000000FF*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000019*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac98c)                                                      */
/*      SPRB0 (0x200ac98c)                                                      */
/*      SPRHBM (0x2007c98c)                                                     */
/*      SPRC0 (0x200ac98c)                                                      */
/*      SPRUCC (0x200dd18c)                                                     */
/* Register default value on SPRA0: 0x0647F83F                                  */
/* Register default value on SPRB0: 0x0647F83F                                  */
/* Register default value on SPRHBM: 0x0647F83F                                 */
/* Register default value on SPRC0: 0x0640A7CC                                  */
/* Register default value on SPRUCC: 0x0640A7CC                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI DCC PHASE CTRL
*/


#define MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0061

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 partial_end_code : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Partial End code of the sweep. End code must be
                               less than end code.
                            */
    UINT32 autovt_partial_end_code : 5;

                            /* Bits[10:6], Access Type=RW/P, default=0x0000001F*/

                            /*
                               AutoVt End code appended as MSBs to Partial End
                               code to acheive higher AutoVt EndCode.
                            */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x00000014*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000019*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_SPRC0_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac98c)                                                      */
/*      SPRB0 (0x200ac98c)                                                      */
/*      SPRHBM (0x2007c98c)                                                     */
/*      SPRC0 (0x200ac98c)                                                      */
/*      SPRUCC (0x200dd18c)                                                     */
/* Register default value on SPRA0: 0x0647F83F                                  */
/* Register default value on SPRB0: 0x0647F83F                                  */
/* Register default value on SPRHBM: 0x0647F83F                                 */
/* Register default value on SPRC0: 0x0640A7CC                                  */
/* Register default value on SPRUCC: 0x0640A7CC                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI DCC PHASE CTRL
*/


#define MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0061

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x000000FF*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000019*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac990)                                                      */
/*      SPRB0 (0x200ac990)                                                      */
/*      SPRHBM (0x2007c990)                                                     */
/*      SPRC0 (0x200ac990)                                                      */
/*      SPRUCC (0x200dd190)                                                     */
/* Register default value on SPRA0: 0x064FF814                                  */
/* Register default value on SPRB0: 0x064FF814                                  */
/* Register default value on SPRHBM: 0x064FF814                                 */
/* Register default value on SPRC0: 0x0640A7CC                                  */
/* Register default value on SPRUCC: 0x0640A7CC                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI SLOOP PHASE CTRL
*/


#define MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0062

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 partial_end_code : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000014*/

                            /*
                               Partial End code of the sweep. End code must be
                               less than end code.
                            */
    UINT32 reserved : 5;

                            /* Bits[10:6], Access Type=RW/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x000001FF*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000019*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac990)                                                      */
/*      SPRB0 (0x200ac990)                                                      */
/*      SPRHBM (0x2007c990)                                                     */
/*      SPRC0 (0x200ac990)                                                      */
/*      SPRUCC (0x200dd190)                                                     */
/* Register default value on SPRA0: 0x064FF814                                  */
/* Register default value on SPRB0: 0x064FF814                                  */
/* Register default value on SPRHBM: 0x064FF814                                 */
/* Register default value on SPRC0: 0x0640A7CC                                  */
/* Register default value on SPRUCC: 0x0640A7CC                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI SLOOP PHASE CTRL
*/


#define MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0062

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 partial_end_code : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Partial End code of the sweep. End code must be
                               less than end code.
                            */
    UINT32 autovt_partial_end_code : 5;

                            /* Bits[10:6], Access Type=RW/P, default=0x0000001F*/

                            /*
                               AutoVt End code appended as MSBs to Partial End
                               code to acheive higher AutoVt EndCode.
                            */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x00000014*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000019*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_SPRC0_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac990)                                                      */
/*      SPRB0 (0x200ac990)                                                      */
/*      SPRHBM (0x2007c990)                                                     */
/*      SPRC0 (0x200ac990)                                                      */
/*      SPRUCC (0x200dd190)                                                     */
/* Register default value on SPRA0: 0x064FF814                                  */
/* Register default value on SPRB0: 0x064FF814                                  */
/* Register default value on SPRHBM: 0x064FF814                                 */
/* Register default value on SPRC0: 0x0640A7CC                                  */
/* Register default value on SPRUCC: 0x0640A7CC                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI SLOOP PHASE CTRL
*/


#define MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0062

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 partial_end_code : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000014*/

                            /*
                               Partial End code of the sweep. End code must be
                               less than end code.
                            */
    UINT32 rsvd : 5;

                            /* Bits[10:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x000001FF*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000019*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac994)                                                      */
/*      SPRB0 (0x200ac994)                                                      */
/*      SPRHBM (0x2007c994)                                                     */
/*      SPRC0 (0x200ac994)                                                      */
/*      SPRUCC (0x200dd194)                                                     */
/* Register default value on SPRA0: 0x003FF800                                  */
/* Register default value on SPRB0: 0x003FF800                                  */
/* Register default value on SPRHBM: 0x003FF800                                 */
/* Register default value on SPRC0: 0x00120000                                  */
/* Register default value on SPRUCC: 0x00120000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI LLOOP PHASE CTRL
*/


#define MDFIS_LLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0063

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_code : 11;

                            /* Bits[10:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Start code of the sweep. Start code must be less
                               than end code.
                            */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x000007FF*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code.
                            */
    UINT32 reserved : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} MDFIS_LLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_NELB_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                      */
/*      SPRA0 (0x200ac998)                                                      */
/*      SPRB0 (0x200ac998)                                                      */
/*      SPRHBM (0x2007c998)                                                     */
/*      SPRC0 (0x200ac998)                                                      */
/*      SPRUCC (0x200dd198)                                                     */
/* Register default value on SPRA0: 0x0003F800                                  */
/* Register default value on SPRB0: 0x0003F800                                  */
/* Register default value on SPRHBM: 0x0003F800                                 */
/* Register default value on SPRC0: 0x003FF800                                  */
/* Register default value on SPRUCC: 0x003FF800                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI NELB PHASE CTRL
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_PATGEN_CTRL_CREG_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200ac99c)                                                      */
/*      SPRB0 (0x200ac99c)                                                      */
/*      SPRHBM (0x2007c99c)                                                     */
/*      SPRC0 (0x200ac99c)                                                      */
/*      SPRUCC (0x200dd19c)                                                     */
/* Register default value on SPRA0: 0x0000F3CF                                  */
/* Register default value on SPRB0: 0x0000F3CF                                  */
/* Register default value on SPRHBM: 0x0000F3CF                                 */
/* Register default value on SPRC0: 0x00003083                                  */
/* Register default value on SPRUCC: 0x00003083                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI PATGEN CTRL.  Used in patgen/lfsr based phases like LLOOP, RXVREF, and NELB.
*/


#define MDFIS_PATGEN_CTRL_CREG_MDFIS_MAIN_REG 0x290A0065

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 burst_cnt : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x0000000F*/

                            /*
                               Burst length. How many x4clk cycles to keep LFSR
                               active for within a burst-idle sequence. Must be
                               minimum of 2.
                            */
    UINT32 idle_cnt : 6;

                            /* Bits[11:6], Access Type=RW/P, default=0x0000000F*/

                            /*
                               Idle length. How many x4clk cycles to disable
                               LFSR for within a burst-idle sequence
                            */
    UINT32 iter_cnt : 7;

                            /* Bits[18:12], Access Type=RW/P, default=0x0000000F*/

                            /*
                               Iteration count. How many sets of burst-idle
                               sequences to run.
                            */
    UINT32 mask_burst_sample : 2;

                            /* Bits[20:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Debug feature to mask out first X number of
                               samples of every burst in a burst-idle sequence.
                            */
    UINT32 leak_cnt : 6;

                            /* Bits[26:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Leak length. The burst_cnt minus leak_cnt is how
                               many actual x4clk cycles to disable TX DQ CBBs
                               within tail end of a burst window to aid with
                               leakage measurements. Must be less than Burst
                               length.
                            */
    UINT32 enable_leak : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable feature that allows for disabling TX DQ
                               CBBs within tail end of a burst window for
                               leakage measurements
                            */
    UINT32 reserved : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} MDFIS_PATGEN_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_RXVREF_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                    */
/*      SPRA0 (0x200ac9a0)                                                      */
/*      SPRB0 (0x200ac9a0)                                                      */
/*      SPRHBM (0x2007c9a0)                                                     */
/*      SPRC0 (0x200ac9a0)                                                      */
/*      SPRUCC (0x200dd1a0)                                                     */
/* Register default value on SPRA0: 0x00005458                                  */
/* Register default value on SPRB0: 0x00005458                                  */
/* Register default value on SPRHBM: 0x00005458                                 */
/* Register default value on SPRC0: 0x00005458                                  */
/* Register default value on SPRUCC: 0x00005458                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI RXVREF PHASE CTRL
*/


#define MDFIS_RXVREF_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0066

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_code : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000058*/

                            /*
                               This is a signed 2s complement number. Start
                               code of the sweep. Start code must be less than
                               end code.
                            */
    UINT32 end_code : 7;

                            /* Bits[13:7], Access Type=RW/P, default=0x00000028*/

                            /*
                               This is a signed 2s complement number. End code
                               of the sweep. End code must be greater than
                               start code.
                            */
    UINT32 rxvref_step_size : 3;

                            /* Bits[16:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is an unsigned number. The hardware forces
                               this always positive for 2s complement math.
                               [br] Step size solely for the rxvref offsetcode
                               count.
                            */
    UINT32 reserved : 15;

                            /* Bits[31:17], Access Type=RW/P, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} MDFIS_RXVREF_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TX_PA_PHASE_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac9a4)                                                      */
/*      SPRB0 (0x200ac9a4)                                                      */
/*      SPRHBM (0x2007c9a4)                                                     */
/*      SPRC0 (0x200ac9a4)                                                      */
/*      SPRUCC (0x200dd1a4)                                                     */
/* Register default value on SPRA0: 0x07C13800                                  */
/* Register default value on SPRB0: 0x07C13800                                  */
/* Register default value on SPRHBM: 0x07C13800                                 */
/* Register default value on SPRC0: 0x07C13800                                  */
/* Register default value on SPRUCC: 0x07C13800                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI TX_PA PHASE CTRL0
*/


#define MDFIS_TX_PA_PHASE_CTRL_CREG_MDFIS_MAIN_REG 0x290A0067

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_code : 11;

                            /* Bits[10:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Start code of the sweep. Start code must be less
                               than end code.
                            */
    UINT32 end_code : 11;

                            /* Bits[21:11], Access Type=RW/P, default=0x00000027*/

                            /*
                               End code of the sweep. End code must be greater
                               than start code and less than max dline cband
                               steps(40),so cannot exceed 'd39
                            */
    UINT32 test_length : 10;

                            /* Bits[31:22], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Test Length for each code iteration to run in
                               x4clk cycles.
                            */

  } Bits;
  UINT32 Data;

} MDFIS_TX_PA_PHASE_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG0_LOWER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac9a8)                                                      */
/*      SPRB0 (0x200ac9a8)                                                      */
/*      SPRHBM (0x2007c9a8)                                                     */
/*      SPRC0 (0x200ac9a8)                                                      */
/*      SPRUCC (0x200dd1a8)                                                     */
/* Register default value on SPRA0: 0x00003216                                  */
/* Register default value on SPRB0: 0x00003216                                  */
/* Register default value on SPRHBM: 0x00003216                                 */
/* Register default value on SPRC0: 0x00003216                                  */
/* Register default value on SPRUCC: 0x00003216                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI STANDALONE SEQUENCE REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG0_UPPER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac9ac)                                                      */
/*      SPRB0 (0x200ac9ac)                                                      */
/*      SPRHBM (0x2007c9ac)                                                     */
/*      SPRC0 (0x200ac9ac)                                                      */
/*      SPRUCC (0x200dd1ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI STANDALONE SEQUENCE REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG1_LOWER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac9b0)                                                      */
/*      SPRB0 (0x200ac9b0)                                                      */
/*      SPRHBM (0x2007c9b0)                                                     */
/*      SPRC0 (0x200ac9b0)                                                      */
/*      SPRUCC (0x200dd1b0)                                                     */
/* Register default value on SPRA0: 0x00000003                                  */
/* Register default value on SPRB0: 0x00000003                                  */
/* Register default value on SPRHBM: 0x00000003                                 */
/* Register default value on SPRC0: 0x00000003                                  */
/* Register default value on SPRUCC: 0x00000003                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI STANDALONE SEQUENCE REG1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_STANDALONE_SEQUENCE_REG1_UPPER_CREG_MDFIS_MAIN_REG supported on:       */
/*      SPRA0 (0x200ac9b4)                                                      */
/*      SPRB0 (0x200ac9b4)                                                      */
/*      SPRHBM (0x2007c9b4)                                                     */
/*      SPRC0 (0x200ac9b4)                                                      */
/*      SPRUCC (0x200dd1b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI STANDALONE SEQUENCE REG1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG0_LOWER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac9b8)                                                      */
/*      SPRB0 (0x200ac9b8)                                                      */
/*      SPRHBM (0x2007c9b8)                                                     */
/*      SPRC0 (0x200ac9b8)                                                      */
/*      SPRUCC (0x200dd1b8)                                                     */
/* Register default value on SPRA0: 0x000000CB                                  */
/* Register default value on SPRB0: 0x000000CB                                  */
/* Register default value on SPRHBM: 0x000000CB                                 */
/* Register default value on SPRC0: 0x000000CB                                  */
/* Register default value on SPRUCC: 0x000000CB                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI DIE2DIE SEQUENCE REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG0_UPPER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac9bc)                                                      */
/*      SPRB0 (0x200ac9bc)                                                      */
/*      SPRHBM (0x2007c9bc)                                                     */
/*      SPRC0 (0x200ac9bc)                                                      */
/*      SPRUCC (0x200dd1bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI DIE2DIE SEQUENCE REG0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG1_LOWER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac9c0)                                                      */
/*      SPRB0 (0x200ac9c0)                                                      */
/*      SPRHBM (0x2007c9c0)                                                     */
/*      SPRC0 (0x200ac9c0)                                                      */
/*      SPRUCC (0x200dd1c0)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI DIE2DIE SEQUENCE REG1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DIE2DIE_SEQUENCE_REG1_UPPER_CREG_MDFIS_MAIN_REG supported on:          */
/*      SPRA0 (0x200ac9c4)                                                      */
/*      SPRB0 (0x200ac9c4)                                                      */
/*      SPRHBM (0x2007c9c4)                                                     */
/*      SPRC0 (0x200ac9c4)                                                      */
/*      SPRUCC (0x200dd1c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI DIE2DIE SEQUENCE REG1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_COUNTER_1MS_CTRL_CREG_MDFIS_MAIN_REG supported on:                     */
/*      SPRA0 (0x200ac9c8)                                                      */
/*      SPRB0 (0x200ac9c8)                                                      */
/*      SPRHBM (0x2007c9c8)                                                     */
/*      SPRC0 (0x200ac9c8)                                                      */
/*      SPRUCC (0x200dd1c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI COUNTER 1MS CTRL
*/


#define MDFIS_COUNTER_1MS_CTRL_CREG_MDFIS_MAIN_REG 0x290A0070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ovrd_val : 19;

                            /* Bits[18:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               1ms counter reset value. Do not program counter
                               less than time needed for a Partial Training
                               flow.
                            */
    UINT32 stop_or_start : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* start/stop the counter */
    UINT32 reserved : 12;

                            /* Bits[31:20], Access Type=RW/P, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} MDFIS_COUNTER_1MS_CTRL_CREG_MDFIS_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_FSM_STATUS0_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac9cc)                                                      */
/*      SPRB0 (0x200ac9cc)                                                      */
/*      SPRHBM (0x2007c9cc)                                                     */
/*      SPRC0 (0x200ac9cc)                                                      */
/*      SPRUCC (0x200dd1cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING FSM STATUS
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_TRAINING_FSM_STATUS1_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac9d0)                                                      */
/*      SPRB0 (0x200ac9d0)                                                      */
/*      SPRHBM (0x2007c9d0)                                                     */
/*      SPRC0 (0x200ac9d0)                                                      */
/*      SPRUCC (0x200dd1d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING FSM STATUS
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_FSM_STATUS1_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac9d0)                                                      */
/*      SPRB0 (0x200ac9d0)                                                      */
/*      SPRHBM (0x2007c9d0)                                                     */
/*      SPRC0 (0x200ac9d0)                                                      */
/*      SPRUCC (0x200dd1d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING FSM STATUS
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_FSM_STATUS1_CREG_MDFIS_MAIN_REG supported on:                 */
/*      SPRA0 (0x200ac9d0)                                                      */
/*      SPRB0 (0x200ac9d0)                                                      */
/*      SPRHBM (0x2007c9d0)                                                     */
/*      SPRC0 (0x200ac9d0)                                                      */
/*      SPRUCC (0x200dd1d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING FSM STATUS
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_CTRL0_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac9d4)                                                      */
/*      SPRB0 (0x200ac9d4)                                                      */
/*      SPRHBM (0x2007c9d4)                                                     */
/*      SPRC0 (0x200ac9d4)                                                      */
/*      SPRUCC (0x200dd1d4)                                                     */
/* Register default value on SPRA0: 0x00000802                                  */
/* Register default value on SPRB0: 0x00000802                                  */
/* Register default value on SPRHBM: 0x00000802                                 */
/* Register default value on SPRC0: 0x00000802                                  */
/* Register default value on SPRUCC: 0x00000802                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI TRAINING CTRL0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_TRAINING_CTRL1_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac9d8)                                                      */
/*      SPRB0 (0x200ac9d8)                                                      */
/*      SPRHBM (0x2007c9d8)                                                     */
/*      SPRC0 (0x200ac9d8)                                                      */
/*      SPRUCC (0x200dd1d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x8B0D3900                                  */
/* Register default value on SPRUCC: 0x8B0D3900                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI TRAINING CTRL1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_CTRL1_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac9d8)                                                      */
/*      SPRB0 (0x200ac9d8)                                                      */
/*      SPRHBM (0x2007c9d8)                                                     */
/*      SPRC0 (0x200ac9d8)                                                      */
/*      SPRUCC (0x200dd1d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x8B0D3900                                  */
/* Register default value on SPRUCC: 0x8B0D3900                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI TRAINING CTRL1
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_CTRL1_CREG_MDFIS_MAIN_REG supported on:                       */
/*      SPRA0 (0x200ac9d8)                                                      */
/*      SPRB0 (0x200ac9d8)                                                      */
/*      SPRHBM (0x2007c9d8)                                                     */
/*      SPRC0 (0x200ac9d8)                                                      */
/*      SPRUCC (0x200dd1d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x8B0D3900                                  */
/* Register default value on SPRUCC: 0x8B0D3900                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI TRAINING CTRL1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_STATUS0_C4_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac9dc)                                                      */
/*      SPRB0 (0x200ac9dc)                                                      */
/*      SPRHBM (0x2007c9dc)                                                     */
/*      SPRC0 (0x200ac9dc)                                                      */
/*      SPRUCC (0x200dd1dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING STATUS0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_STATUS0_C5_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac9e0)                                                      */
/*      SPRB0 (0x200ac9e0)                                                      */
/*      SPRHBM (0x2007c9e0)                                                     */
/*      SPRC0 (0x200ac9e0)                                                      */
/*      SPRUCC (0x200dd1e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING STATUS0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_STATUS1_C4_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac9e4)                                                      */
/*      SPRB0 (0x200ac9e4)                                                      */
/*      SPRHBM (0x2007c9e4)                                                     */
/*      SPRC0 (0x200ac9e4)                                                      */
/*      SPRUCC (0x200dd1e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING STATUS1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_TRAINING_STATUS1_C5_CREG_MDFIS_MAIN_REG supported on:                  */
/*      SPRA0 (0x200ac9e8)                                                      */
/*      SPRB0 (0x200ac9e8)                                                      */
/*      SPRHBM (0x2007c9e8)                                                     */
/*      SPRC0 (0x200ac9e8)                                                      */
/*      SPRUCC (0x200dd1e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFI TRAINING STATUS1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_MASK_C4_CTRL0_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac9ec)                                                      */
/*      SPRB0 (0x200ac9ec)                                                      */
/*      SPRHBM (0x2007c9ec)                                                     */
/*      SPRC0 (0x200ac9ec)                                                      */
/*      SPRUCC (0x200dd1ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE MASK C4 CTRL0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_MASK_C4_CTRL1_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac9f0)                                                      */
/*      SPRB0 (0x200ac9f0)                                                      */
/*      SPRHBM (0x2007c9f0)                                                     */
/*      SPRC0 (0x200ac9f0)                                                      */
/*      SPRUCC (0x200dd1f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE MASK C4 CTRL1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_MASK_C4_CTRL2_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac9f4)                                                      */
/*      SPRB0 (0x200ac9f4)                                                      */
/*      SPRHBM (0x2007c9f4)                                                     */
/*      SPRC0 (0x200ac9f4)                                                      */
/*      SPRUCC (0x200dd1f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE MASK C4 CTRL2
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_MASK_C5_CTRL0_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac9f8)                                                      */
/*      SPRB0 (0x200ac9f8)                                                      */
/*      SPRHBM (0x2007c9f8)                                                     */
/*      SPRC0 (0x200ac9f8)                                                      */
/*      SPRUCC (0x200dd1f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE MASK C5 CTRL0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_MASK_C5_CTRL1_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200ac9fc)                                                      */
/*      SPRB0 (0x200ac9fc)                                                      */
/*      SPRHBM (0x2007c9fc)                                                     */
/*      SPRC0 (0x200ac9fc)                                                      */
/*      SPRUCC (0x200dd1fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE MASK C5 CTRL1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_MASK_C5_CTRL2_CREG_MDFIS_MAIN_REG supported on:                   */
/*      SPRA0 (0x200aca00)                                                      */
/*      SPRB0 (0x200aca00)                                                      */
/*      SPRHBM (0x2007ca00)                                                     */
/*      SPRC0 (0x200aca00)                                                      */
/*      SPRUCC (0x200dd200)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE MASK C5 CTRL2
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_NEVER_PASS_C4_STATUS0_CREG_MDFIS_MAIN_REG supported on:           */
/*      SPRA0 (0x200aca04)                                                      */
/*      SPRB0 (0x200aca04)                                                      */
/*      SPRHBM (0x2007ca04)                                                     */
/*      SPRC0 (0x200aca04)                                                      */
/*      SPRUCC (0x200dd204)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE NEVER PASS C4 STATUS0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_NEVER_PASS_C4_STATUS1_CREG_MDFIS_MAIN_REG supported on:           */
/*      SPRA0 (0x200aca08)                                                      */
/*      SPRB0 (0x200aca08)                                                      */
/*      SPRHBM (0x2007ca08)                                                     */
/*      SPRC0 (0x200aca08)                                                      */
/*      SPRUCC (0x200dd208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE NEVER PASS C4 STATUS1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_NEVER_PASS_C4_STATUS2_CREG_MDFIS_MAIN_REG supported on:           */
/*      SPRA0 (0x200aca0c)                                                      */
/*      SPRB0 (0x200aca0c)                                                      */
/*      SPRHBM (0x2007ca0c)                                                     */
/*      SPRC0 (0x200aca0c)                                                      */
/*      SPRUCC (0x200dd20c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE NEVER PASS C4 STATUS2
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_NEVER_PASS_C5_STATUS0_CREG_MDFIS_MAIN_REG supported on:           */
/*      SPRA0 (0x200aca10)                                                      */
/*      SPRB0 (0x200aca10)                                                      */
/*      SPRHBM (0x2007ca10)                                                     */
/*      SPRC0 (0x200aca10)                                                      */
/*      SPRUCC (0x200dd210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE NEVER PASS C5 STATUS0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_NEVER_PASS_C5_STATUS1_CREG_MDFIS_MAIN_REG supported on:           */
/*      SPRA0 (0x200aca14)                                                      */
/*      SPRB0 (0x200aca14)                                                      */
/*      SPRHBM (0x2007ca14)                                                     */
/*      SPRC0 (0x200aca14)                                                      */
/*      SPRUCC (0x200dd214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE NEVER PASS C5 STATUS1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LANE_NEVER_PASS_C5_STATUS2_CREG_MDFIS_MAIN_REG supported on:           */
/*      SPRA0 (0x200aca18)                                                      */
/*      SPRB0 (0x200aca18)                                                      */
/*      SPRHBM (0x2007ca18)                                                     */
/*      SPRC0 (0x200aca18)                                                      */
/*      SPRUCC (0x200dd218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* MDFIS LANE NEVER PASS C5 STATUS2
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LFSR_SEED_A_CREG_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca1c)                                                      */
/*      SPRB0 (0x200aca1c)                                                      */
/*      SPRHBM (0x2007ca1c)                                                     */
/*      SPRC0 (0x200aca1c)                                                      */
/*      SPRUCC (0x200dd21c)                                                     */
/* Register default value on SPRA0: 0xAAAAAAAA                                  */
/* Register default value on SPRB0: 0xAAAAAAAA                                  */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* Register default value on SPRC0: 0xAAAAAAAA                                  */
/* Register default value on SPRUCC: 0xAAAAAAAA                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI LFSR SEED
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LFSR_SEED_B_CREG_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca20)                                                      */
/*      SPRB0 (0x200aca20)                                                      */
/*      SPRHBM (0x2007ca20)                                                     */
/*      SPRC0 (0x200aca20)                                                      */
/*      SPRUCC (0x200dd220)                                                     */
/* Register default value on SPRA0: 0xBBBBBBBB                                  */
/* Register default value on SPRB0: 0xBBBBBBBB                                  */
/* Register default value on SPRHBM: 0xBBBBBBBB                                 */
/* Register default value on SPRC0: 0xBBBBBBBB                                  */
/* Register default value on SPRUCC: 0xBBBBBBBB                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI LFSR SEED
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LFSR_POLY_A_CREG_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca24)                                                      */
/*      SPRB0 (0x200aca24)                                                      */
/*      SPRHBM (0x2007ca24)                                                     */
/*      SPRC0 (0x200aca24)                                                      */
/*      SPRUCC (0x200dd224)                                                     */
/* Register default value on SPRA0: 0xEBDFAB00                                  */
/* Register default value on SPRB0: 0xEBDFAB00                                  */
/* Register default value on SPRHBM: 0xEBDFAB00                                 */
/* Register default value on SPRC0: 0xEBDFAB00                                  */
/* Register default value on SPRUCC: 0xEBDFAB00                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI LFSR POLY
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LFSR_POLY_B_CREG_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca28)                                                      */
/*      SPRB0 (0x200aca28)                                                      */
/*      SPRHBM (0x2007ca28)                                                     */
/*      SPRC0 (0x200aca28)                                                      */
/*      SPRUCC (0x200dd228)                                                     */
/* Register default value on SPRA0: 0xCFBCAFFE                                  */
/* Register default value on SPRB0: 0xCFBCAFFE                                  */
/* Register default value on SPRHBM: 0xCFBCAFFE                                 */
/* Register default value on SPRC0: 0xCFBCAFFE                                  */
/* Register default value on SPRUCC: 0xCFBCAFFE                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* MDFI LFSR POLY
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_SAI_HI_CP_POLICY_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca2c)                                                      */
/*      SPRB0 (0x200aca2c)                                                      */
/*      SPRHBM (0x2007ca2c)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access to RAC, WAC and CP registers for P_U_CODE security policy group. This group is intended for registers that were uCR in prior art.
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_P_U_CODE_CHNL2_SAI_HI_CP_POLICY_MDFIS_MAIN_REG supported on:           */
/*      SPRC0 (0x200aca2c)                                                      */
/*      SPRUCC (0x200dd22c)                                                     */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access to RAC, WAC and CP registers for P_U_CODE security policy group. This group is intended for registers that were uCR in prior art.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_SAI_LO_CP_POLICY_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca30)                                                      */
/*      SPRB0 (0x200aca30)                                                      */
/*      SPRHBM (0x2007ca30)                                                     */
/* Register default value on SPRA0: 0x01000208                                  */
/* Register default value on SPRB0: 0x01000208                                  */
/* Register default value on SPRHBM: 0x01000208                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access to RAC, WAC and CP registers for P_U_CODE security policy group. This group is intended for registers that were uCR in prior art.
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_P_U_CODE_CHNL2_SAI_LO_CP_POLICY_MDFIS_MAIN_REG supported on:           */
/*      SPRC0 (0x200aca30)                                                      */
/*      SPRUCC (0x200dd230)                                                     */
/* Register default value on SPRC0: 0x01000208                                  */
/* Register default value on SPRUCC: 0x01000208                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access to RAC, WAC and CP registers for P_U_CODE security policy group. This group is intended for registers that were uCR in prior art.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_SAI_HI_RD_POLICY_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca34)                                                      */
/*      SPRB0 (0x200aca34)                                                      */
/*      SPRHBM (0x2007ca34)                                                     */
/* Register default value on SPRA0: 0x20000500                                  */
/* Register default value on SPRB0: 0x20000500                                  */
/* Register default value on SPRHBM: 0x20000500                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* This register configures the read access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_P_U_CODE_CHNL2_SAI_HI_RD_POLICY_MDFIS_MAIN_REG supported on:           */
/*      SPRC0 (0x200aca34)                                                      */
/*      SPRUCC (0x200dd234)                                                     */
/* Register default value on SPRC0: 0x20000500                                  */
/* Register default value on SPRUCC: 0x20000500                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This register configures the read access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_SAI_LO_RD_POLICY_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca38)                                                      */
/*      SPRB0 (0x200aca38)                                                      */
/*      SPRHBM (0x2007ca38)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* This register configures the read access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_P_U_CODE_CHNL2_SAI_LO_RD_POLICY_MDFIS_MAIN_REG supported on:           */
/*      SPRC0 (0x200aca38)                                                      */
/*      SPRUCC (0x200dd238)                                                     */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This register configures the read access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_SAI_HI_WR_POLICY_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca3c)                                                      */
/*      SPRB0 (0x200aca3c)                                                      */
/*      SPRHBM (0x2007ca3c)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_P_U_CODE_CHNL2_SAI_HI_WR_POLICY_MDFIS_MAIN_REG supported on:           */
/*      SPRC0 (0x200aca3c)                                                      */
/*      SPRUCC (0x200dd23c)                                                     */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* MDFIS_SAI_LO_WR_POLICY_MDFIS_MAIN_REG supported on:                          */
/*      SPRA0 (0x200aca40)                                                      */
/*      SPRB0 (0x200aca40)                                                      */
/*      SPRHBM (0x2007ca40)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRB0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRHBM Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_P_U_CODE_CHNL2_SAI_LO_WR_POLICY_MDFIS_MAIN_REG supported on:           */
/*      SPRC0 (0x200aca40)                                                      */
/*      SPRUCC (0x200dd240)                                                     */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This register configures the write access for registers in P_U_CODE security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DFX_CHNL2_SAI_HI_CP_POLICY_MDFIS_MAIN_REG supported on:                */
/*      SPRC0 (0x200aca44)                                                      */
/*      SPRUCC (0x200dd244)                                                     */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register configures the write access to RAC, WAC and CP registers for DFX security policy group. This policy group is expected to be used for registers that log corrected error counts. BIOS is in the policy group for this register. Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DFX_CHNL2_SAI_LO_CP_POLICY_MDFIS_MAIN_REG supported on:                */
/*      SPRC0 (0x200aca48)                                                      */
/*      SPRUCC (0x200dd248)                                                     */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register configures the write access to RAC, WAC and CP registers for DFX security policy group. This policy group is expected to be used for registers that log corrected error counts. BIOS is in the policy group for this register. Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DFX_CHNL2_SAI_HI_RD_POLICY_MDFIS_MAIN_REG supported on:                */
/*      SPRC0 (0x200aca4c)                                                      */
/*      SPRUCC (0x200dd24c)                                                     */
/* Register default value on SPRC0: 0x20000500                                  */
/* Register default value on SPRUCC: 0x20000500                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register configures the read access to registers in DFX security policy group. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DFX_CHNL2_SAI_LO_RD_POLICY_MDFIS_MAIN_REG supported on:                */
/*      SPRC0 (0x200aca50)                                                      */
/*      SPRUCC (0x200dd250)                                                     */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register configures the read access to registers in DFX security policy group. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DFX_CHNL2_SAI_HI_WR_POLICY_MDFIS_MAIN_REG supported on:                */
/*      SPRC0 (0x200aca54)                                                      */
/*      SPRUCC (0x200dd254)                                                     */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register configures the write access to registers in DFX security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_DFX_CHNL2_SAI_LO_WR_POLICY_MDFIS_MAIN_REG supported on:                */
/*      SPRC0 (0x200aca58)                                                      */
/*      SPRUCC (0x200dd258)                                                     */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register configures the write access to registers in DFX security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LB_W_CHNL2_SAI_HI_CP_POLICY_MDFIS_MAIN_REG supported on:               */
/*      SPRC0 (0x200aca5c)                                                      */
/*      SPRUCC (0x200dd25c)                                                     */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for LB_W security policy group. This policy group covers registers that had the -LB attribute in prior art, but are not checked by MCHECK.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LB_W_CHNL2_SAI_LO_CP_POLICY_MDFIS_MAIN_REG supported on:               */
/*      SPRC0 (0x200aca60)                                                      */
/*      SPRUCC (0x200dd260)                                                     */
/* Register default value on SPRC0: 0x01000208                                  */
/* Register default value on SPRUCC: 0x01000208                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for LB_W security policy group. This policy group covers registers that had the -LB attribute in prior art, but are not checked by MCHECK.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LB_W_CHNL2_SAI_HI_RD_POLICY_MDFIS_MAIN_REG supported on:               */
/*      SPRC0 (0x200aca64)                                                      */
/*      SPRUCC (0x200dd264)                                                     */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the read access for registers in LB_W security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LB_W_CHNL2_SAI_LO_RD_POLICY_MDFIS_MAIN_REG supported on:               */
/*      SPRC0 (0x200aca68)                                                      */
/*      SPRUCC (0x200dd268)                                                     */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the read access for registers in LB_W security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LB_W_CHNL2_SAI_HI_WR_POLICY_MDFIS_MAIN_REG supported on:               */
/*      SPRC0 (0x200aca6c)                                                      */
/*      SPRUCC (0x200dd26c)                                                     */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the write access for registers in LB_W security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* MDFIS_LB_W_CHNL2_SAI_LO_WR_POLICY_MDFIS_MAIN_REG supported on:               */
/*      SPRC0 (0x200aca70)                                                      */
/*      SPRUCC (0x200dd270)                                                     */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRC0 Register File:    sprsp_top/mdfis[0]/chnl2_creg/mem_mdfis_chnl2_registers*/
/* SPRUCC Register File:    sprsp_top/mdi[0]/mdi_left_creg/mem_mdfis_chnl2_registers*/
/* Struct generated from SPRC0 BDF: 0_0_0                                       */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the write access for registers in LB_W security policy group.
*/

#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */
#endif /* _MDFIS_MAIN_h */
