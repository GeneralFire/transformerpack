//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file RtErrorLogBmc.h
    Runtime error log BMC includes and declarations 
**/

#ifndef _BMC_SEL_LOG_H
#define _BMC_SEL_LOG_H

#include "Token.h"
#include <RuntimeErrorlog/RtErrorlogBoard.h>
#include <Library/SmmServicesTableLib.h>
//#include <Library/mpsyncdatalib.h>
#include <Library/DebugLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/IehSiliconLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/MpService.h>
#include <IndustryStandard/PciExpress21.h>
#include <Guid/Cper.h>
#include <SiliconSetting.h>
#include <Emca.h>

//
// General IPMI Defines
//
#define ADD_SEL_ENTRY                   0x44
#define EFI_SM_STORAGE                  0xA

//
// General Info related to SEL Record
//
#define EFI_EVM_REVISION    0x04
#define EFI_BIOS_ID         0x18
#define EFI_FORMAT_REV      0x00
#define EFI_FORMAT_REV1     0x01
#define EFI_SOFTWARE_ID     0x01
#define EFI_SENSOR_TYPE_EVENT_CODE_DISCRETE 0x6F
#define SEL_SYSTEM_RECORD                   0x2  
#define EFI_MEMORY_SENSOR_TYPE              0x0C
#define EFI_PROCESSOR_SENSOR_TYPE           0x07


//
// SEL Sensor Types and Error Codes for PCI Errors
//
#define SEL_SENS_TYPE_CRIT_ERR      0x13
#define SEL_SENS_PCI_PERR           0x04      
#define SEL_SENS_PCI_SERR           0x05

#define MEMORY_SENSOR_CE_OFFSET     0x00
#define MEMORY_SENSOR_UCE_OFFSET    0x01
#define MEMORY_SENSOR_CEMAX_OFFSET  0x05

#define EFI_SOFTWARE_ID_SELECT          0x01
#define SMI_HANDLER_SOFTWARE_ID         0x10
#define EFI_GENERATOR_ID(SOFTWARE_ID)   ((SOFTWARE_ID << 1) | (EFI_SOFTWARE_ID_SELECT))


#define SOCKET_NUM_BITS                 0x05
#define CHANNEL_NUM_BITS                0x02
#define DIMM_NUM_BITS                   0x03

//
// Enum Definitions
//
typedef enum {
  EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE=0,
  EFI_MEMORY_SENSOR_OFFSET_ECC_UNCORRECTABLE,
  EFI_MEMORY_SENSOR_OFFSET_PARITY,
  EFI_MEMORY_SENSOR_OFFSET_SCRUB_FAILED,
  EFI_MEMORY_SENSOR_OFFSET_MEM_DEVICE_DISABLED,
  EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE_MAX,
}EFI_SEL_SENSOR_MEMORY_OFFSET;

typedef enum {
  EFI_PROCESSOR_SENSOR_OFFSET_CONFIGURATION_ERROR=5,
}EFI_SEL_SENSOR_PROCESSOR_OFFSET;

typedef enum {
   MCA_UNIT_NONE=0,
   MCA_UNIT_IFU,
   MCA_UNIT_DCU,
   MCA_UNIT_DTLB,
   MCA_UNIT_MLC,
   MCA_UNIT_PCU,
   MCA_UNIT_IIO,
   MCA_UNIT_CHA,
   MCA_UNIT_KTI
}EFI_SEL_PROCESSOR_BANK_TYPE;

typedef enum {
   SEL_PROC_ERROR_TYPE_UNKNOWN = 0, // EFI_GENERIC_ERROR_PROC_ERROR_TYPE_UNKNOWN
   SEL_PROC_ERROR_TYPE_CACHE,       // EFI_GENERIC_ERROR_PROC_ERROR_TYPE_CACHE
   SEL_PROC_ERROR_TYPE_TLB,         // EFI_GENERIC_ERROR_PROC_ERROR_TYPE_TLB
   SEL_PROC_ERROR_TYPE_BUS,         // EFI_GENERIC_ERROR_PROC_ERROR_TYPE_BUS
   SEL_PROC_ERROR_TYPE_MICRO_ARCH   // EFI_GENERIC_ERROR_PROC_ERROR_TYPE_MICRO_ARCH
} EFI_SEL_PROCESSOR_GENERIC_ERROR_TYPE;

typedef enum {
    IIO_INTERNAL_OTC_BIT00 = 0x70,  // otc_crdt_uf, Bits[0:0]
    IIO_INTERNAL_OTC_BIT01 = 0x71,  // otc_crdt_of, Bits[1:1]
    IIO_INTERNAL_OTC_BIT02 = 0x72,  // otc_par_hdr_rf, Bits[2:2]
    IIO_INTERNAL_OTC_BIT03 = 0x73,  // otc_par_addr_rf, Bits[3:3]
    IIO_INTERNAL_OTC_BIT04 = 0x74,  // otc_ecc_uncor_rf, Bits[4:4]
    IIO_INTERNAL_OTC_BIT05 = 0x75,  // otc_cabort, Bits[5:5]
    IIO_INTERNAL_OTC_BIT06 = 0x76,  // otc_mabort, Bits[6:6]
    IIO_INTERNAL_OTC_BIT07 = 0x77,  // otc_mtc_tgt_err, Bits[7:7]
    IIO_INTERNAL_OTC_BIT08 = 0x78,  // otc_ecc_cor_rf, Bits[8:8]
    IIO_INTERNAL_OTC_BIT09 = 0x79,  // otc_mctp_bcast_to_dmi, Bits[9:9]
    IIO_INTERNAL_OTC_BIT10 = 0x7A,  // otc_par_rte, Bits[10:10]
    IIO_INTERNAL_OTC_BIT11 = 0x7B,  // otc_irp_dat_par, Bits[11:11]
    IIO_INTERNAL_OTC_BIT12 = 0x7C,  // otc_irp_addr_par, Bits[12:12]
    IIO_INTERNAL_OTC_BIT13 = 0x7D,  // otc_hw_assert, Bits[13:13]
    IIO_INTERNAL_OTC_BIT14 = 0x7E,  // otc_txn_dur_lock, Bits[14:14]
    IIO_INTERNAL_OTC_BIT15 = 0x7F,  // otc_gpsb_par, Bits[15:15]
    IIO_INTERNAL_OTC_BIT16 = 0x80,  // otc_sb_uf, Bits[16:16]
    IIO_INTERNAL_OTC_BIT17 = 0x81,  // otc_sb_of, Bits[17:17]
    IIO_INTERNAL_OTC_BIT18 = 0x82,  // otc_misc_snarf_fifo_of, Bits[18:18]
    IIO_INTERNAL_OTC_BIT19 = 0x83,  // otc_misc_snarf_fifo_uf, Bits[19:19]
    IIO_INTERNAL_OTC_BIT20 = 0x84,  // otc_misc_oobmsm_mult_pend, Bits[20:20]
    IIO_INTERNAL_OTC_RESERVED = 0x85 // Reserved / Others
} IIO_INTERNAL_OTC_ERROR_TYPE;

#ifdef SPR_HOST
typedef enum {
    IIO_INTERNAL_VTD_UNC_BIT00 = 0x90,  // iommu_data_iotlb_par_err, Bits[0:0]
    IIO_INTERNAL_VTD_UNC_BIT01 = 0x91,  // pq_req_par_err, Bits[1:1]
    IIO_INTERNAL_VTD_UNC_BIT02 = 0x92,  // pwt_req_par_err, Bits[2:2]
    IIO_INTERNAL_VTD_UNC_BIT03 = 0x93,  // iommu_mem_resp_abort, Bits[3:3]
    IIO_INTERNAL_VTD_UNC_BIT16 = 0x94,  // pmr_check_abort, Bits[16:16]
    IIO_INTERNAL_VTD_UNC_BIT17 = 0x95,  // hpa_overflow, Bits[17:17]
    IIO_INTERNAL_VTD_UNC_BIT18 = 0x96,  // gpa_overflow, Bits[18:18]
    IIO_INTERNAL_VTD_UNC_BIT19 = 0x97,  // illegal_msi, Bits[19:19]
    IIO_INTERNAL_VTD_UNC_BIT20 = 0x98,  // at_translated_illegal_device, Bits[20:20]
    IIO_INTERNAL_VTD_UNC_BIT21 = 0x99,  // spare, Bits[21:21]
    IIO_INTERNAL_VTD_UNC_RESERVED = 0x9A // Reserved / Others
} IIO_INTERNAL_VTD_UNC_ERROR_TYPE;
#endif 

#ifdef ICX_HOST
typedef enum {
    IIO_INTERNAL_VTD_UNC_BIT00 = 0x90,  // iommu_data_iotlb_par_err, Bits[0:0]
    IIO_INTERNAL_VTD_UNC_BIT01 = 0x91,  // pq_req_par_err, Bits[1:1]
    IIO_INTERNAL_VTD_UNC_BIT02 = 0x92,  // pwt_req_par_err, Bits[2:2]
    IIO_INTERNAL_VTD_UNC_BIT03 = 0x93,  // iommu_mem_resp_abort, Bits[3:3]
    IIO_INTERNAL_VTD_UNC_BIT16 = 0x94,  // pmr_check_abort, Bits[16:16]
    IIO_INTERNAL_VTD_UNC_BIT17 = 0x95,  // hpa_overflow, Bits[17:17]
    IIO_INTERNAL_VTD_UNC_BIT18 = 0x96,  // gpa_overflow, Bits[18:18]
    IIO_INTERNAL_VTD_UNC_BIT19 = 0x97,  // illegal_msi, Bits[19:19]
    IIO_INTERNAL_VTD_UNC_BIT20 = 0x98,  // at_translated_illegal_device, Bits[20:20]
    IIO_INTERNAL_VTD_UNC_BIT21 = 0x99,  // spare, Bits[21:21]
    IIO_INTERNAL_VTD_UNC_RESERVED = 0x9A // Reserved / Others
} IIO_INTERNAL_VTD_UNC_ERROR_TYPE;
#endif 

#ifdef CPX_HOST
typedef enum {
    IIO_INTERNAL_VTD_UNC_BIT00 = 0x90,  // perr_context_cache, Bits[0:0]
    IIO_INTERNAL_VTD_UNC_BIT01 = 0x91,  // perr_l1_lookup, Bits[1:1]
    IIO_INTERNAL_VTD_UNC_BIT02 = 0x92,  // perr_l2_lookup, Bits[2:2]
    IIO_INTERNAL_VTD_UNC_BIT03 = 0x93,  // perr_l3_lookup, Bits[3:3]
    IIO_INTERNAL_VTD_UNC_BIT04 = 0x94,  // perr_tlb0, Bits[4:4]
    IIO_INTERNAL_VTD_UNC_BIT05 = 0x95,  // perr_tlb1, Bits[5:5]
    IIO_INTERNAL_VTD_UNC_BIT06 = 0x96,  // unsucc_ci_rdcp, Bits[6:6]
    IIO_INTERNAL_VTD_UNC_BIT07 = 0x97,  // miscerrs, Bits[7:7]
    IIO_INTERNAL_VTD_UNC_BIT08 = 0x98,  // protmemviol, Bits[8:8]
    IIO_INTERNAL_VTD_UNC_BIT31 = 0x99,  // vtderr, Bits[31:31]
    IIO_INTERNAL_VTD_UNC_RESERVED = 0x9A // Reserved / Others
} IIO_INTERNAL_VTD_UNC_ERROR_TYPE;
#endif 

typedef enum {
    IIO_INTERNAL_VTD_COR_BIT00 = 0xA0,  // iommu_tag_iotlb_par_err, Bits[0:0]
    IIO_INTERNAL_VTD_COR_BIT01 = 0xA1,  // iommu_tag_sl_l2_par_err, Bits[1:1]
    IIO_INTERNAL_VTD_COR_BIT02 = 0xA2,  // iommu_tag_sl_l3_par_err, Bits[2:2]
    IIO_INTERNAL_VTD_COR_BIT03 = 0xA3,  // iommu_tag_sl_l4_par_err, Bits[3:3]
    IIO_INTERNAL_VTD_COR_BIT04 = 0xA4,  // iommu_tag_sl_l5_par_err, Bits[4:4]
    IIO_INTERNAL_VTD_COR_BIT05 = 0xA5,  // iommu_tag_fl_l2_par_err, Bits[5:5]
    IIO_INTERNAL_VTD_COR_BIT06 = 0xA6,  // iommu_tag_fl_l3_par_err, Bits[6:6]
    IIO_INTERNAL_VTD_COR_BIT07 = 0xA7,  // iommu_tag_fl_l4_par_err, Bits[7:7]
    IIO_INTERNAL_VTD_COR_BIT08 = 0xA8,  // iommu_tag_fl_l5_par_err, Bits[8:8]
    IIO_INTERNAL_VTD_COR_BIT09 = 0xA9,  // iommu_tag_pasid_par_err, Bits[9:9]
    IIO_INTERNAL_VTD_COR_BIT10 = 0xAA,  // iommu_tag_rcc_par_err, Bits[10:10]
    IIO_INTERNAL_VTD_COR_BIT11 = 0xAB,  // iommu_tag_ice_par_err, Bits[11:11]
    IIO_INTERNAL_VTD_COR_BIT12 = 0xAC,  // iommu_data_sl_l2_par_err, Bits[12:12]
    IIO_INTERNAL_VTD_COR_BIT13 = 0xAD,  // iommu_data_sl_l3_par_err, Bits[13:13]
    IIO_INTERNAL_VTD_COR_BIT14 = 0xAE,  // iommu_data_sl_l4_par_err, Bits[14:14]
    IIO_INTERNAL_VTD_COR_BIT15 = 0xAF,  // iommu_data_sl_l5_par_err, Bits[15:15]
    IIO_INTERNAL_VTD_COR_BIT16 = 0xB0,  // iommu_data_fl_l2_par_err, Bits[16:16]
    IIO_INTERNAL_VTD_COR_BIT17 = 0xB1,  // iommu_data_fl_l3_par_err, Bits[17:17]
    IIO_INTERNAL_VTD_COR_BIT18 = 0xB2,  // iommu_data_fl_l4_par_err, Bits[18:18]
    IIO_INTERNAL_VTD_COR_BIT19 = 0xB3,  // iommu_data_fl_l5_par_err, Bits[19:19]
    IIO_INTERNAL_VTD_COR_BIT20 = 0xB4,  // iommu_data_pasid_err, Bits[20:20]
    IIO_INTERNAL_VTD_COR_BIT21 = 0xB5,  // iommu_data_rcc_par_err, Bits[21:21]
    IIO_INTERNAL_VTD_COR_BIT22 = 0xB6,  // iommu_data_ice_par_err, Bits[22:22]
    IIO_INTERNAL_VTD_COR_BIT23 = 0xB7,  // iommu_data_pwt_other_par_err, Bits[23:23]
    IIO_INTERNAL_VTD_COR_RESERVED = 0xB8 // Reserved / Others
} IIO_INTERNAL_VTD_COR_ERROR_TYPE;


typedef enum {
    IIO_INTERNAL_DMA_UNC_BIT02 = 0xF5, // received_poisoned_data_from_dp_status, Bits[2:2]
    IIO_INTERNAL_DMA_UNC_BIT03 = 0xF6, // dma_internal_hw_parity_error_status, Bits[3:3]
    IIO_INTERNAL_DMA_UNC_BIT04 = 0xF7, // cfg_reg_parity_error_status, Bits[4:4]
    IIO_INTERNAL_DMA_UNC_BIT07 = 0xF8, // rd_cmpl_header_error_status, Bits[7:7]
    IIO_INTERNAL_DMA_UNC_BIT10 = 0xF9, // read_address_decode_error_status, Bits[10:10]
    IIO_INTERNAL_DMA_UNC_BIT12 = 0xFA, // syndrome Bits[12:12]
    IIO_INTERNAL_DMA_UNC_RESERVED = 0xFB // Reserved / Others
} IIO_INTERNAL_DMA_UNC_ERROR_TYPE;

typedef enum {
    IIO_INTERNAL_CHN_ERR_BIT00 = 0x50,// dma_trans_saddr_err, Bits[0:0]
    IIO_INTERNAL_CHN_ERR_BIT01 = 0x51,// dma_xfrer_daddr_err, Bits[1:1]
    IIO_INTERNAL_CHN_ERR_BIT02 = 0x52,// nxt_desc_addr_err, Bits[2:2]
    IIO_INTERNAL_CHN_ERR_BIT03 = 0x53,// descriptor_error, Bits[3:3]
    IIO_INTERNAL_CHN_ERR_BIT04 = 0x54,// chn_addr_valerr, Bits[4:4]
    IIO_INTERNAL_CHN_ERR_BIT05 = 0x55,// chancmd_err, Bits[5:5]
    IIO_INTERNAL_CHN_ERR_BIT06 = 0x56,// cdata_parerr, Bits[6:6]
    IIO_INTERNAL_CHN_ERR_BIT07 = 0x57,// dma_data_parerr, Bits[7:7]
    IIO_INTERNAL_CHN_ERR_BIT08 = 0x58,// rd_data_err, Bits[8:8]
    IIO_INTERNAL_CHN_ERR_BIT09 = 0x59,// wr_data_err, Bits[9:9]
    IIO_INTERNAL_CHN_ERR_BIT10 = 0x5A,// desc_ctrl_err, Bits[10:10]
    IIO_INTERNAL_CHN_ERR_BIT11 = 0x5B,// desc_len_err, Bits[11:11]
    IIO_INTERNAL_CHN_ERR_BIT12 = 0x5C,// mp_addr_err, Bits[12:12]
    IIO_INTERNAL_CHN_ERR_BIT13 = 0x5D,// int_cfg_err,  Bits[13:13]
    IIO_INTERNAL_CHN_ERR_BIT16 = 0x5E,// crc_xorp_err, Bits[16:16]
    IIO_INTERNAL_CHN_ERR_BIT17 = 0x5F,// xorqerr, Bits[17:17]
    IIO_INTERNAL_CHN_ERR_BIT18 = 0x60,// desccnterr, Bits[18:18]
    IIO_INTERNAL_CHN_ERR_BIT19 = 0x61,// dif_allftag_err, Bits[19:19]
    IIO_INTERNAL_CHN_ERR_BIT20 = 0x62,// dif_guardtag_err, Bits[20:20]
    IIO_INTERNAL_CHN_ERR_BIT21 = 0x63,// dif_apptag_err, Bits[21:21]
    IIO_INTERNAL_CHN_ERR_BIT22 = 0x64,// dif_reftag_err, Bits[22:22]
    IIO_INTERNAL_CHN_ERR_BIT23 = 0x65,// dif_bndl_err, Bits[23:23]
    IIO_INTERNAL_CHN_ERR_RESERVED = 0x66 // Reserved / Others
} IIO_INTERNAL_CHN_ERR_ERROR_TYPE;




typedef enum {
    IIO_INTERNAL_IRPERR_BIT01 = 0xC0,  // wrcache_correcc_error_cs0, Bits[1:1]
    IIO_INTERNAL_IRPERR_BIT02 = 0xC1,  // wrcache_correcc_error_cs1, Bits[2:2]
    IIO_INTERNAL_IRPERR_BIT03 = 0xC2,  // protocol_rcvd_poison, Bits[3:3]
    IIO_INTERNAL_IRPERR_BIT04 = 0xC3,  // wrcache_uncecc_error_cs0, Bits[4:4]
    IIO_INTERNAL_IRPERR_BIT05 = 0xC4,  // wrcache_uncecc_error_cs1, Bits[5:5]
    IIO_INTERNAL_IRPERR_BIT06 = 0xC5,  // csr_acc_32b_unaligned, Bits[6:6]
    IIO_INTERNAL_IRPERR_BIT10 = 0xC6,  // protocol_rcvd_unexprsp, Bits[10:10]
    IIO_INTERNAL_IRPERR_BIT13 = 0xC7,  // protocol_qt_overflow_underflow, Bits[13:13]
    IIO_INTERNAL_IRPERR_BIT14 = 0xC8,  // protocol_parity_error, Bits[14:14]
    IIO_INTERNAL_IRPERR_BIT16 = 0xC9,  // p2p_hdr_rf_addr_par_error, Bits[16:16]
    IIO_INTERNAL_IRPERR_BIT17 = 0xCA,  // faf_rf_addr_par_error, Bits[17:17]
    IIO_INTERNAL_IRPERR_BIT18 = 0xCB,  // addrcam0_addr_par_error, Bits[18:18]
    IIO_INTERNAL_IRPERR_BIT19 = 0xCC,  // addrcam1_addr_par_error, Bits[19:19]
    IIO_INTERNAL_IRPERR_BIT20 = 0xCD,  // pf_timeout_err_cs0, Bits[20:20]
    IIO_INTERNAL_IRPERR_BIT21 = 0xCE,  // pf_timeout_err_cs1, Bits[21:21]
    IIO_INTERNAL_IRPERR_RESERVED = 0xCF // Reserved / Others
} IIO_INTERNAL_IRPERR_ERROR_TYPE;

typedef enum {
    IIO_INTERNAL_IRPRING_BIT00 = 0xD0,  // bl_parity_error, Bits[0:0]
    IIO_INTERNAL_IRPRING_BIT01 = 0xD1,  // snp_addr_parity_error, Bits[1:1]
    IIO_INTERNAL_IRPRING_BIT02 = 0xD2,  // rxdata1_parity_error, Bits[2:2]
    IIO_INTERNAL_IRPRING_BIT03 = 0xD3,  // rxdata0_parity_error, Bits[3:3]
    IIO_INTERNAL_IRPRING_BIT04 = 0xD4,  // blq_parity_error, Bits[4:4]
    IIO_INTERNAL_IRPRING_BIT05 = 0xD5,  // bl_ecc_corrected_error, Bits[5:5]
    IIO_INTERNAL_IRPRING_BIT06 = 0xD6,  // bl_ecc_uncorrectable_error, Bits[6:6]
    IIO_INTERNAL_IRPRING_BIT07 = 0xD7,  // bl_cqid_parity_error, Bits[7:7]
    IIO_INTERNAL_IRPRING_BIT08 = 0xD8,  // ak0_cqid_parity_error, Bits[8:8]
    IIO_INTERNAL_IRPRING_BIT09 = 0xD9,  // ak1_cqid_parity_error, Bits[9:9]
    IIO_INTERNAL_IRPRING_BIT10 = 0xDA,  // txq0_addr_par_error, Bits[10:10]
    IIO_INTERNAL_IRPRING_BIT11 = 0xDB,  // txq1_addr_par_error, Bits[11:11]
    IIO_INTERNAL_IRPRING_BIT12 = 0xDC,  // snpq_rf_addr_parity_error, Bits[12:12]
    IIO_INTERNAL_IRPRING_BIT13 = 0xDD,  // unexpected_go_err_rcvd, Bits[13:13]
    IIO_INTERNAL_IRPRING_RESERVED = 0xDE // Reserved / Others
} IIO_INTERNAL_IRPRING_ERROR_TYPE;

typedef enum {
    IIO_INTERNAL_ITC_BIT12 = 0xE0,  // itc_enq_data_overflow, Bits[12:12]
    IIO_INTERNAL_ITC_BIT13 = 0xE1,  // sw2iosf_mps_err, Bits[13:13]
    IIO_INTERNAL_ITC_BIT14 = 0xE2,  // sw2iosf_req_buf_uf, Bits[14:14]
    IIO_INTERNAL_ITC_BIT15 = 0xE3,  // sw2iosf_req_buf_ov, Bits[15:15]
    IIO_INTERNAL_ITC_BIT16 = 0xE4,  // itc_irp_cred_uf, Bits[16:16]
    IIO_INTERNAL_ITC_BIT17 = 0xE5,  // itc_irp_cred_of, Bits[17:17]
    
    IIO_INTERNAL_ITC_BIT18 = 0xE6,  // If host = ICX itc_par_iosf_dat, Bits[18:18]
                                    // If host = CPX itc_par_pcie_dat, Bits[18:18]
    IIO_INTERNAL_ITC_BIT19 = 0xE7,  // itc_par_hdr_rf, Bits[19:19]
    IIO_INTERNAL_ITC_BIT20 = 0xE8,  // itc_vtmisc_hdr_rf, Bits[20:20]
    IIO_INTERNAL_ITC_BIT21 = 0xE9,  // itc_par_addr_rf, Bits[21:21]
    IIO_INTERNAL_ITC_BIT22 = 0xEA,  // itc_ecc_cor_rf, Bits[22:22]
    IIO_INTERNAL_ITC_BIT23 = 0xEB,  // itc_ecc_uncor_rf, Bits[23:23]
    IIO_INTERNAL_ITC_BIT24 = 0xEC,  // itc_cabort, Bits[24:24]
    IIO_INTERNAL_ITC_BIT25 = 0xED,  // itc_mabort, Bits[25:25]
    
    IIO_INTERNAL_ITC_BIT26 = 0xEE,  // If host = ICX inb_unsuccessful_cmpl, Bits[26:26]
                                    // If host = CPX itc_mtc_tgt_err, Bits[26:26]
    
    IIO_INTERNAL_ITC_BIT27 = 0xEF,  // itc_enq_overflow, Bits[27:27]
    IIO_INTERNAL_ITC_BIT28 = 0xF0,  // itc_misc_prh_overflow, Bits[28:28]
    IIO_INTERNAL_ITC_BIT29 = 0xF1,  // itc_hw_assert, Bits[29:29]
    IIO_INTERNAL_ITC_BIT30 = 0xF2,  // itc_par_iosf_cmd, Bits[30:30]
    IIO_INTERNAL_ITC_BIT31 = 0xF3,  // itc_msgd_illegal_size, Bits[31:31]
    IIO_INTERNAL_ITC_RESERVED = 0xF4 // Reserved / Others
} IIO_INTERNAL_ITC_ERROR_TYPE;

#pragma pack(1)

typedef struct {
  UINT32      Year    : 4;
  UINT32      Month   : 4;
  UINT32      Day     : 5;
  UINT32      Hour    : 5;
  UINT32      Minute  : 6;
  UINT32      Second  : 6;
} TIME_STAMP;

typedef struct {
  UINT16  RecordId;
  UINT8   RecordType;
  TIME_STAMP  TimeStamp;
  UINT16  GeneratorId;
  UINT8   EvMRevision;
  UINT8   SensorType;
  UINT8   SensorNumber;
  UINT8   EventDirType;
  UINT8   OEMEvData1;
  UINT8   OEMEvData2;
  UINT8   OEMEvData3;
} EFI_SEL_RECORD_DATA;

#pragma pack()

#endif
