/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#ifndef _DXE_OOB_ONE_TOUCH_H_
#define _DXE_OOB_ONE_TOUCH_H_

#include <IndustryStandard/Tpm20.h>
#include <Protocol/Tcg2Protocol.h>

//-----------------------------------------------------------------------------------------------------
#pragma pack (push, 1)
//-----------------------------------------------------------------------------------------------------
// ********************	OOB Specification *************************************************************
//-----------------------------------------------------------------------------------------------------
// Implementation Version Number
//   Bug Solve should update the Minor Version.
//   Ehancement/New Feature MUST update the Major Version.
//   History:
//   01.02  Added TME/MKTME Handler support
//   02.00  Major version of implementation is changed to 02
//          Added SGX/PFR information in OXP_HEADER and in Task-0x01 OXP_DISCOVERY_INFO
//          Added PFR Handler support
//          Added support for Task Status (Bit11-8) in Attribute field of OXP_MK_TME_INFO
//
#define OXP_MAJOR_VERSION_IMP  0x02    // Major Version of Implementation
#define OXP_MINOR_VERSION_IMP  0x00    // Minor Version of Implementation
//
// OTA Specification Version Number
//   Any change in OXP_HEADER and/or OOB_TXT_TPM should update the OTA Specification Version.
#define OXP_SPECIFICATION_VERSION	0x02 // OTA Specification Version
//
// OXP_HEADER
//
typedef    struct  {                   // Offset Length   Description
    UINT32 Signature;                  //  00h   04 byte  Signature
    UINT16 TotalLength;                //  04h   02 byte  Length of OOB_TXT_TPM structure in bytes
    UINT16 HeaderLength;               //  06h   02 byte  Length of header in bytes
    UINT8  Version;                    //  08h   01 byte  Version
    UINT8  Checksum;                   //  09h   01 byte  Checksum
    UINT8  TaskAndResult;              //  0Ah   01 byte  Bit-mapped info of task and result
    UINT8  Status;                     //  0Bh   01 byte  Status
    UINT16 PasswordAttribute;          //  0Ch   02 byte  Bit-mapped info of supported password
    UINT16 FeatureSupported;           //  0Eh   02 byte  Bit-mapped info of supported TXT/TPM features
    UINT16 FeatureEnabled;             //  10h   02 byte  Bit-mapped info of enabled TXT/TPM features
    UINT32 State;                      //  12h   04 byte  Bit-mapped info of state of task
    UINT8  MajorVersionImp;            //  16h   01 byte  Major Version of Implementation
    UINT8  MinorVersionImp;            //  17h   01 byte  Minor Version of Implementation
    UINT16 TpmUsage;                   //  18h   02 byte  Bit-mapped info of TPM usage
    UINT8  Reserved [0x06];            //  1Ah   06 byte  Reserved for future use
} OXP_HEADER;
//
// Signature: Indicates the signature of the OOB TXT/TPM information.
//   On input, signature is ‘$OXP’. Offset-0 contains ‘$’, Offset-1 contains ‘O’, Offset-2 contains ‘X’,
//           and Offset-3 contains ‘P’. Caller must supply this signature while requesting an OOB task.
//   On output, signature is ‘PXO$’. Offset-0 contains ‘P’, Offset-1 contains ‘X’, Offset-2 contains ‘O’,
//           and Offset-3 contains ‘$’. BIOS must output this signature after performing the requested task.
#define OXP_SIGNATURE_INPUT   0x50584F24 // $OXP signature on Input
#define OXP_SIGNATURE_OUTPUT  0x244F5850 // PXO$ signature on Output
//
// TotalLength: Indicates the total length of all information in bytes i.e. length of OOB_TXT_TPM structure which
//   includes header and all additional information. The content of this field depends on the task.
//
// HeaderLength: Indicates the length of the OXP_HEADER in bytes.
//   The header length is 0x20 for this specification and is not changed after it is initialized.
//
// Version: Indicates the OTA Specification version number.
//   The version number is not changed after it is initialized. However, if the supplied version number is
//   different from the supported version number, BIOS will update this field to the supported version number.
//
// Checksum: Provides the checksum of OOB_TXT_TPM structure. The checksum is calculated in a manner such
//   that byte-addition of all bytes (including this field) in OOB_TXT_TPM structure is 0.
//   The content of this field is changed whenever any information in OOB_TXT_TPM is modified.
//
// TaskAndResult: Bit-mapped information of the task and the result.
#define   OXP_TASK_BIT_MASK    0x7F  // Bit6-0: Task# or Use-Case#
#define   OXP_TASK_BIT_N       0x00              //              #of bit shift for alignment
                                                 // Task#        Description
                                                         // 00   Reserved for historical reasons
#define   OXP_TASK_FEATURE_SUPPORT_DISCOVERY        0x01 // 01   Feature Support Discovery
#define   OXP_TASK_FEATURE_ENABLE_STATUS_DISCOVERY  0x02 // 02   Feature Enable Status Discovery
#define   OXP_TASK_ENABLE_TXT_DTPM                  0x03 // 03   Enable TXT/dTPM
#define   OXP_TASK_DTPM_OWNER_CLEAR_ONLY            0x04 // 04   dTPM Owner Clear Only
#define   OXP_TASK_DTPM_CLEAR_DTPM_ACTIVATION       0x05 // 05   dTPM Clear and dTPM Activation
#define   OXP_TASK_DTPM_CLEAR_ENABLE_TXT_DTPM       0x06 // 06   dTPM Clear and Enable TXT/dTPM
                                                    // 07 - 0A   NOT USED
#define   OXP_TASK_ENABLE_TXT_FTPM                  0x0B // 0B   Enable TXT/fTPM
#define   OXP_TASK_FTPM_OWNER_CLEAR_ONLY            0x0C // 0C   fTPM Owner Clear Only
#define   OXP_TASK_FTPM_CLEAR_FTPM_ACTIVATION       0x0D // 0D   fTPM Clear and TPM Activation
#define   OXP_TASK_FTPM_CLEAR_ENABLE_TXT_FTPM       0x0E // 0E   fTPM Clear and Enable TXT/fTPM
#define   OXP_TASK_DISABLE_TXT_ONLY                 0x0F // 0F   Disable TXT only
#define   OXP_TASK_DISABLE_DTPM_ONLY                0x10 // 10   Disable DTPM only
#define   OXP_TASK_DISABLE_FTPM_ONLY                0x11 // 11   Disable FTPM only
#define   OXP_TASK_PROVISION_DTPM                   0x12 // 12   Reserved -> Provision dTPM
#define   OXP_TASK_PROVISION_FTPM                   0x13 // 13   Reserved -> Provision fTPM
#define   OXP_TASK_HANDLE_TPM_NV_INDEX              0x14 // 14   Reserved -> Handle TPM NV Index
#define   OXP_TASK_DTPM_RESET_AUX_INDEX             0x15 // 15   Reserved -> dTPM Reset AUX Index
#define   OXP_TASK_FTPM_RESET_AUX_INDEX             0x16 // 16   Reserved -> fTPM Reset AUX Index
#define   OXP_TASK_HANDLE_MKTME                     0x17 // 17   TME/MKTME Handler
#define   OXP_TASK_HANDLE_SGX                       0x18 // 18   Reserved -> SGX Handler
#define   OXP_TASK_HANDLE_PFR                       0x19 // 19   PFR Handler
                                                    // 1A - 5E   Reserved for future use
                                                    // 60 - 6F   Reserved for OEM/Vendor Specific use
                                                    // 70 - 7E   Reserved for Intel Internal Use
#define   OXP_TASK_MAX_NUMBER                       0x7F // 7F   Reserved for historical reasons
#define   OXP_TASK_PERFORMED_BIT                    0x80 //Bit-7 : 0/1, Task Not/Yes performed
                                                         //      A value of 1 indicates that task is performed and
                                                         //      the Status field indicates success/error.
                                                         //      The caller must set Bit-7 = 0 while requesting
                                                         //      a task to be performed.
// Status: Indicates the status of the performed task.
//   This field is valid only when Bit-7 of TaskAndResult field is set to 1.
//   A zero value indicate Success and non-zero value indicates Error.
//   The status codes 0x00-0xBF are defined by the specification.
//   The status codes 0xC0-0xDF are reserved for Vendor-specific use.
//   The status codes 0xE0-0xFF are reserved for Intel internal use (e.g. debugging, etc.)
#define OXP_SUCCESS                                 0x00  // Success
#define OXP_INVALID_TASK                            0x01  // Invalid task (more information may be available in OXP_ADDITIONAL_INFO)
#define OXP_TPM_LOCKED                              0x02  // TPM Locked (valid for TPM1.2)
#define OXP_TPM_PHYSICAL_PRESENCE_CMD_ENABLE_ERROR  0x03  // TPM Physical Presence command enable error (valid for TPM1.2)
#define OXP_TPM_PHYSICAL_PRESENCE_ERROR             0x04  // TPM Physical Presence set error (valid for TPM1.2)
#define OXP_ADDITIONAL_INFO_NOT_PRESENT             0x05  // OXP_ADDITIONAL_INFO not present
#define OXP_ADDITIONAL_INFO_MALFORMED               0x06  // OXP_ADDITIONAL_INFO mal-formed
                                               // 0x07-0x0F: Reserved for generic error codes

#define OXP_TXT_NOT_SUPPORTED                       0x10  // TXT not supported
#define OXP_TXT_ENABLE_ERROR                        0x11  // TXT enable error
                                               // 0x12-0x1F: Reserved for TXT related error codes

#define OXP_DTPM_NOT_SUPPORTED                      0x20  // dTPM not supported (and requested task cannot be performed)
#define OXP_DTPM_ENABLE_ERROR                       0x21  // dTPM enable error
#define OXP_DTPM_CLEAR_ERROR                        0x22  // dTPM clear error
#define OXP_DTPM_DISABLE_ERROR                      0x23  // dTPM disable error
#define OXP_DTPM_NOT_ENABLED                        0x24  // dTPM not enabled (and requested task cannot be performed)
#define OXP_DTPM_NOT_ACTIVATED                      0x25  // dTPM not activated (and requested task cannot be performed)
#define OXP_DTPM_ACTIVATE_ERROR                     0x26  // dTPM activate error
#define OXP_DTPM_DEACTIVATE_ERROR                   0x27  // dTPM deactivate error
                                               // 0x28-0x2F: Reserved for dTPM related error codes

#define OXP_AUTHENTICATION_ERROR                    0x30  // Authentication failure (supplied password not match)
#define OXP_PASSWORD_TYPE_NOT_SUPPORTED             0x31  // OXP_PASSWORD_INFO password type not supported
#define OXP_PASSWORD_INFO_NOT_FOUND                 0x32  // OXP_PASSWORD_INFO not found
#define OXP_PASSWORD_TOO_LARGE                      0x33  // OXP_PASSWORD_INFO password larger than internal area, increase internal size
#define OXP_PASSWORD_EMPTY                          0x34  // OXP_PASSWORD_INFO password empty
#define OXP_PASSWORD_NOT_NULL_TERMINATED            0x35  // OXP_PASSWORD_INFO password string not NULL terminated
#define OXP_PASSWORD_BIOS_ADMIN_NOT_INSTALLED       0x36  // OXP_PASSWORD_INFO BIOS Admin password not installed
                                               // 0x37-0x3D: Reserved for future use

#define OXP_INFO_LENGTH_NOT_VALID                   0x3E  // OXP info length not valid
#define OXP_INFO_VALID_NOT_SET                      0x3F  // OXP info valid bit not set in Attribute

#define OXP_FTPM_NOT_SUPPORTED                      0x40  // fTPM not supported (and requested task cannot be performed)
#define OXP_FTPM_ENABLE_ERROR                       0x41  // fTPM enable error
#define OXP_FTPM_CLEAR_ERROR                        0x42  // fTPM clear error
#define OXP_FTPM_DISABLE_ERROR                      0x43  // fTPM disable error
#define OXP_FTPM_NOT_ENABLED                        0x44  // fTPM not enabled (and requested task cannot be performed)
//
#define OXP_FTPM_SET_PCR_ERROR                      0x48  // fTPM Set PCR Banks error
                                               // 0x45-0x4F: Reserved for fTPM related error codes
                                               // 0x50-0xBF: Reserved for future use
                                               // 0xC0-0xDF: Reserved for Vendor specific use
                                               // 0xE0-0xFE: Reserved for Intel internal use
#define OXP_UNKNOWN_ERROR                           0xFF  // Unknown error (cause cannot be determined --> BUG)
//
// PasswordAttribute: Bit-mapped information of password usage in the platform.
//   If SHA value of password string is supported, Bit-14 indicates whether the SHA value of password string
//   must be calculated by including or not including the NULL-termination character.
#define OXP_PASSWORD_ASCII_BIT                      0x01  // Bit-0:   0/1, Password ASCII string Not/Yes supported
#define OXP_PASSWORD_UNICODE_BIT                    0x02  // Bit-1:   0/1, Password Unicode string Not/Yes supported
#define OXP_PASSWORD_SHA1_BIT                       0x04  // Bit-2:   0/1, SHA1 value of password Not/Yes supported
                                                          // Bit12-3: Reserved for future use (set to 0)
#define OXP_PASSWORD_IGNORE_BIT                     0x2000// Bit-13:  0/1, Use/Ignore BIOS Administrative password during authentication
#define OXP_PASSWORD_SHA_INCLUDE_NULL_BIT           0x4000// Bit-14:  0/1, SHA value Not/Yes include NULL-termination character of password string
#define OXP_PASSWORD_ADMIN_INSTALLED_BIT            0x8000// Bit-15:  0/1, Administrative password Not/Yes installed
//
#define OXP_PASSWORD_TYPE_BIT_MASK  (OXP_PASSWORD_ASCII_BIT | OXP_PASSWORD_UNICODE_BIT | OXP_PASSWORD_SHA1_BIT)
//
// FeatureSupported: Bit-mapped information of TXT/TPM features supported in the platform.
#define OXP_TXT_SUPPORT_BIT                         0x01  // Bit-0:   0/1, TXT Not/Yes supported
#define OXP_TPM_SUPPORT_BIT_MASK                    0x06  // Bit2-1:  TPM support information
#define   OXP_DTPM_SUPPORT_BIT                      0x02  // Bit-1: 0/1, dTPM Not/Yes supported
#define   OXP_FTPM_SUPPORT_BIT                      0x04  // Bit-2: 0/1, fTPM Not/Yes supported
//#define   OXP_TPM_SUPPORT_BIT_N                   0x01  // #of bit shift for alignment
//#define   OXP_TPM_NOT_SUPPORT                     0x00  // 00b: TPM not supported
//#define   OXP_DTPM_SUPPORT                        0x01  // 01b: dTPM supported
//#define   OXP_FTPM_SUPPORT                        0x02  // 10b: fTPM supported
//#define   OXP_DTPM_FTPM_SUPPORT                   0x03  // 11b: dTPM, fTPM supported
                                                 // Bit11-3: Reserved for future use (set to 0)
#define   OXP_SGX_SUPPORT_BIT                       0x1000// Bit-12:  0/1, SGX Not/Yes supported
#define   OXP_PFR_SUPPORT_BIT                       0x2000// Bit-13:  0/1, PFR Not/Yes supported
#define   OXP_MKTME_SUPPORT_BIT                     0x4000// Bit-14:  0/1, MKTME Not/Yes supported
#define   OXP_TME_SUPPORT_BIT                       0x8000// Bit-15:  0/1, TME Not/Yes supported
//
// FeatureEnabled: Bit-mapped information of TXT/TPM features that are currently enabled in the platform.
//   Since this information may change from boot to boot, it represents the features that are enabled
//   in the most recent boot.
#define OXP_TXT_ENABLED_BIT                         0x0001// Bit-0:   0/1, TXT Not/Yes enabled
#define OXP_TPM_ENABLED_BIT_MASK                    0x0006// Bit2-1:  TPM enabled information
#define   OXP_DTPM_ENABLED_BIT                      0x0002// Bit-1: 0/1, dTPM Not/Yes enabled
#define   OXP_FTPM_ENABLED_BIT                      0x0004// Bit-2: 0/1, fTPM Not/Yes enabled
//#define   OXP_TPM_ENABLED_BIT_N                   0x01  // #of bit shift for alignment
//#define   OXP_TPM_NOT_ENABLED                     0x00  // 00b: TPM not enabled
//#define   OXP_DTPM_ENABLED                        0x01  // 01b: dTPM enabled
//#define   OXP_FTPM_ENABLED                        0x02  // 10b: fTPM enabled
//#define   OXP_DTPM_FTPM_ENABLED_INVALID           0x03  // 11b: Invalid, since either dTPM or fTPM (not both) can be enabled
                                                 // Bit11-3: Reserved for future use (set to 0)
#define   OXP_SGX_ENABLED_BIT                       0x1000// Bit-12:  0/1, SGX Not/Yes enabled
#define   OXP_PFR_ENABLED_BIT                       0x2000// Bit-13:  0/1, PFR Not/Yes enabled
#define   OXP_MKTME_ENABLED_BIT                     0x4000// Bit-14:  0/1, MKTME Not/Yes enabled
#define   OXP_TME_ENABLED_BIT                       0x8000// Bit-15:  0/1, TME Not/Yes enabled
//
// State: Bit-mapped information of state of task (BIOS INTERNAL USE ONLY, caller may not change this field).
#define OXP_STATE_SIGNATURE_INVALID_BIT                 0x00000001 // Bit-0:   0/1, Valid/Invalid Signature in OXP_HEADER
#define OXP_STATE_TOTAL_LENGTH_INVALID_BIT              0x00000002 // Bit-1:   0/1, Valid/Invalid Total Length in OXP_HEADER
#define OXP_STATE_HEADER_LENGTH_INVALID_BIT             0x00000004 // Bit-2:   0/1, Valid/Invalid Header Length OXP_HEADER
#define OXP_STATE_TOTAL_LENGTH_LESS_HEADER_LENGTH_BIT   0x00000008 // Bit-3:   0/1, Total Length (>=)/(<) Header Length in OXP_HEADER
#define OXP_STATE_VERSION_INVALID_BIT                   0x00000010 // Bit-4:   0/1, Valid/Invalid Version in OXP_HEADER
#define OXP_STATE_CHECKSUM_INVALID_BIT                  0x00000020 // Bit-5:   0/1, Valid/Invalid Checksum in OXP_HEADER
#define OXP_STATE_ALL_BYTES_SAME_BIT                    0x00000040 // Bit-6:   0/1, All bytes Not/Yes same in OOB data
                                                     // Bit-7.........Reserved for future use
#define OXP_STATE_VERISON_UPDATED_BIT                   0x00000100 // Bit-8:   0/1, Specification (and/or Implementation) Version Number not/yes updated in OXP_HEADER
#define OXP_STATE_DATA_DELETED_OUTPUT_BIT               0x00000200 // Bit-9:   0/1, OXP_PASSWORD_DATA not/yes deleted from OOB output data
#define OXP_STATE_PASSWORD_IGNORED_BIT                  0x00000400 // Bit-10:  0/1, Supplied Password checked/ignored during authentication
#define OXP_STATE_FEATURE_ENABLED_UPDATED_BIT           0x00000800 // Bit-11:  0/1, Feature Enabled not/yes updated in OXP_HEADER
#define OXP_STATE_FEATURE_SUPPORTED_UPDATED_BIT         0x00001000 // Bit-12:  0/1, Feature Supported not/yes updated in OXP_HEADER
#define OXP_STATE_PASSWORD_ATTRIBUTE_UPDATED_BIT        0x00002000 // Bit-13:  0/1, Password Attribute not/yes updated in OXP_HEADER
#define OXP_STATE_TPM_USAGE_UPDATED_BIT                 0x00004000 // Bit-14:  0/1, TPM USage not/yes updated in OXP_HEADER
                                                     // Bit-15........Reserved for future use
#define OXP_STATE_TPM_PREPARATION_BEFORE_TASK_ERROR_BIT 0x00010000 // Bit-16:  0/1, TPM preparation before task not/yes error
#define OXP_STATE_TPM_PREPARATION_AFTER_TASK_ERROR_BIT  0x00020000 // Bit-17:  0/1, TPM preparation after task not/yes error
#define OXP_STATE_PLATFORM_DATA_WRITE_ERROR_BIT         0x00040000 // Bit-18:  0/1, Write BIOS Setup Option data Success/Error
#define OXP_STATE_OOB_DEFAULT_TASK_USED_BIT             0x00080000 // Bit-19:  0/1, OOB default task not/yes used
#define OXP_STATE_OOB_ME_DATA_USED_BIT                  0x00100000 // Bit-20:  0/1, OOB ME Data not/yes used
#define OXP_STATE_OOB_EFI_DATA_USED_BIT                 0x00200000 // Bit-21:  0/1, OOB EFI Data not/yes used
#define OXP_STATE_OOB_ME_DATA_UPDATE_BIT                0x00400000 // Bit-22:  0/1, OOB ME Data not/yes to be updated
#define OXP_STATE_OOB_EFI_DATA_UPDATE_BIT               0x00800000 // Bit-23:  0/1, OOB EFI Data not/yes to be updated
//
#define OXP_STATE_OOB_ME_DATA_INVALID_BIT               0x01000000 // Bit-24:  0/1, OOB ME Data not/yes valid
#define OXP_STATE_OOB_EFI_DATA_INVALID_BIT              0x02000000 // Bit-25:  0/1, OOB EFI Data not/yes valid
#define OXP_STATE_OOB_ME_DATA_NO_TASK_BIT               0x04000000 // Bit-26:  0/1, Not/Yes new task in OOB ME Data
#define OXP_STATE_OOB_EFI_DATA_NO_TASK_BIT              0x08000000 // Bit-27:  0/1, Not/Yes new task in OOB EFI Data
                                                     // Bit29-28......Reserved for future use
#define OXP_STATE_TASK_PENDING_BIT                      0x40000000 // Bit-30:  0/1, Task not/yes pending
                                                                   //               1: Task is only accepted but not yet performed (to be performed following the reset)
#define OXP_STATE_RESET_BIT                             0x80000000 // Bit-31:  0/1, Reset Not/Yes resulted from last performed task
//
// MajorVersionImp, MinorVersionImp: Indicates the major and minor version of the implementation.
//   The version number must be updated with any change (bug solve, enhancememnt, etc.) in the implementation.
//
// TpmUsage: Indicates the bit-mapped information of TPM usage. New field added in OXP_VERSION 0x02.
//   This field is valid only if TPM is enabled (as indicated in FeatureEnabled field).
//   Since this information may change from boot to boot, it represents the information of the most recent boot.
#define OXP_TPM_PROVISION_BIT                      0x0001 // Bit-0:   0/1, TPM Not/Yes provisioned (valid for both TPM1.2 and TPM2.0)
#define OXP_TPM_OWNERSHIP_BIT                      0x0002 // Bit-1:   0/1, TPM Ownership Not/Yes claimed (valid for both TPM1.2 and TPM2.0)
#define OXP_TPM_VERSION_BIT                        0x0004 // Bit-2:   0/1, TPM Version TPM1.2/TPM2.0
#define   OXP_TPM_VERSION_BIT_N                    0x02   // #of bit shift for alignment
#define   OXP_TPM_VERSION_TPM12                    0x00   // 0: TPM1.2
#define   OXP_TPM_VERSION_TPM20                    0x01   // 1: TPM2.0
                                                          // Bit7-3........Reserved for future use (set to 0)
#define OXP_TPM_NVINDEX_PS_WRITE_PROTECT_BIT       0x0100 // Bit-8:   0/1, TPM PS NV Index Not/Yes write-protected (valid for both TPM1.2 and TPM2.0)
#define OXP_TPM_NVINDEX_AUX_WRITE_PROTECT_BIT      0x0200 // Bit-9:   0/1, TPM AUX NV Index Not/Yes write-protected (valid for both TPM1.2 and TPM2.0)
#define OXP_TPM_NVINDEX_PO_WRITE_PROTECT_BIT       0x0400 // Bit-10:  0/1, TPM PO NV Index Not/Yes write-protected (valid for both TPM1.2 and TPM2.0)
                                                          // Bit14-11......Reserved for future use (set to 0)
#define OXP_TPM_LOCK_BIT                           0x8000 // Bit-15:  0/1, TPM Lock Not/Yes set (valid for only TPM1.2)
                                                          //               For TPM1.2: It represents the settings of TPM_PERMANENT_FLAGS.nvLocked field
                                                          //               For TPM2.0: It does not have any meaning (set to 0)
//
//-----------------------------------------------------------------------------------------------------
// ******************** OXP_ADDITIONAL_INFO ***********************************************************
//-----------------------------------------------------------------------------------------------------
// 1. OXP_ADDITIONAL_INFO describes one or more additional information if it is required for the requested task.
// 2. OXP_ADDITIONAL_INFO is not required for all tasks. Hence OXP_ADDITIONAL_INFO may or may not be present.
// 3. If OXP_ADDITIONAL_INFO is not required for a task but is supplied, then OXP_ADDITIONAL_INFO must be correct.
// 4. First 5 bytes of each additional information in OXP_ADDITIONAL_INFO is of the following format:
//      Offset-0: BYTE, Identifier of this additional information
//      Offset-1: WORD, Length of this additional information in bytes
//      Offset-3: WORD, Attribute of this additional information
#define OXP_ADDITIONAL_INFO_VALID_BIT              0x8000 // Bit-15: 0/1, This additional information not/yes valid
//                                                        // Bit14-0......Depends on the additional information
//      Offset-5: VARIABLE, N BYTES, depends on the additional information
//
// Identifier of different additional information that are implemented
//                                                        // Identifier	Description
                                                          //   00       Reserved for historical reason
#define OXP_ID_PASSWORD                              0x01 //   01       Password Information (see OXP_PASSWORD_INFO for details)
#define OXP_ID_DISCOVERY                             0x02 //   02       Discovery Information (see OXP_DISCOVERY_INFO for details)
#define OXP_ID_TPM_NV                                0x03 //   03       TPM NV Information (see OXP_TPM_NV_INFO for details)
#define OXP_ID_MK_TME                                0x04 //   04       TME/MKTME Information (see OXP_MK_TME_INFO for details)
#define OXP_ID_SGX                                   0x05 //   05       SGX Information (see OXP_SGX_INFO for details)
#define OXP_ID_PFR                                   0x06 //   06       PFR Information (see OXP_PFR_INFO for details)
                                                          //   07-FE    Reserved for future use
                                                          //   0xFF     Reserved for historical reason
// Max value of Identifier used in the implementation
#define MAX_OXP_ADDITIONAL_INFO_ID             OXP_ID_PFR // ===> MUST BE CHANGED if any ID is added/removed
#define MIN_OXP_ADDITIONAL_LENGTH              0x05       // Min Length of Additional Info
//
//
//---------------------------------------------------------
// OXP_PASSWORD_INFO: For the task (Use-Case) that requires user authentication, the caller must supply
//                    the administrative password in OXP_PASSWORD_INFO format.
//
#define PASSWORD_DATA_SIZE_B                         0x14 // Size of Password field in bytes for SHA1
//
typedef    struct  {                                // Offset Description
    UINT8  Identifier;                              // 0x00   Identifier
    UINT16 Length;                                  // 0x01   Length of OXP_PASSWORD_INFO
    UINT16 Attribute;                               // 0x03   Bit-mapped Attribute
    UINT8  Password [PASSWORD_DATA_SIZE_B];         // 0x05   Password
} OXP_PASSWORD_INFO;
//
#define OXP_PASSWORD_INFO_MIN_LENGTH           0x05       // Min Length of OXP_PASSWORD_INFO (i.e. length of the structure without Password field)
//
// Identifier: Identifies the type of additional information, 0x01 for Password Information
//
// Length: Provides the length D of OXP_PASSWORD_INFO in bytes.
//
// Attribute: Indicates the nature of password information present in Password field.
//   Only one bit (either Bit-1 or Bit-0) must be set to 1 indicating ASCII or Unicode string.
//   If Bit-2 is 0, it indicates that Password information is supplied as string and the string type
//     (ASCII or Unicode) is indicated by Bit1-0 setting.
//   If Bit-2 is 1, it indicates that Password information is supplied as SHA1 value and Bit1-0 setting
//     indicates whether the SHA1 value is calculated on ASCII string or Unicode string.
//   If SHA value of password string is supplied, Bit-14 indicates whether the SHA value of password
//     string is calculated by including or not including the NULL-termination character.
//
//                                                                    // Bit12-0: Identical to Bit12-0 of OXP_HEADER.PasswordAttribute field
//                                                                    // Bit-13:  NOT USED (IGNORED)
//                                                                    // Bit-14:  Identical to Bit-14 of OXP_HEADER.PasswordAttribute field
#define OXP_PASSWORD_INFO_VALID_BIT    OXP_ADDITIONAL_INFO_VALID_BIT  // Bit-15:  0/1, Password info Not/Yes valid
//
// Password: Provides the password as indicated by Attribute field. The length P of this
//   field is P = (D – 5) bytes, where D = Length of OXP_PASSWORD_INFO (i.e. the value in Length field).
//
//--------------------------------------------------------
// OXP_DISCOVERY_INFO: For the task# OXP_TASK_TXT_TPM_SUPPORT_DISCOVERY, the caller must supply OXP_DISCOVERY_INFO.
//
typedef    struct  {                        // Offset  Description
    UINT8  Identifier;                      // 0x00    Identifier
    UINT16 Length;                          // 0x01    Length of OXP_DISCOVERY_INFO
    UINT16 Attribute;                       // 0x03    Bit-mapped Attribute
    UINT8  TaskSupported [0x10];            // 0x05    Bit-mapped information of supported tasks
    UINT16 MaxMkTmeKeys;                    // 0x15    Maximum #of MK-TME Keys available for use
    UINT8  MaxMkTmeKeyIdBits;               // 0x17    Maximum #of Bits allowed for use as Key Identifiers for MK-TME
    UINT8  MkTmeKeyIdBits;                  // 0x18    #of Bits allocated for use as Key Identifiers for MK-TME
    UINT16 PfrState;                        // 0x19    Bit-mapped information of PFR State
    UINT8  PfrRecoveryCount;                // 0x1B    #of Recovery since last AC power cycle
    UINT8  PfrLastRecoveryReason;           // 0x1C    Last Recovery Reason
    UINT8  PfrPanicEventCount;              // 0x1D    #of Panic events since last AC power cycle
    UINT8  PfrLastPanicReason;              // 0x1E    Last Panic Reason
} OXP_DISCOVERY_INFO;
//
#define OXP_DISCOVERY_INFO_MIN_LENGTH  sizeof (OXP_DISCOVERY_INFO)    // Min Length of OXP_DISCOVERY_INFO
//
// Identifier: Identifies the type of additional information, 0x02 for Discovery Information
//
// Length: Provides the length D of OXP_DISCOVERY_INFO in bytes.
//
// Attribute: Indicates the nature of discovery information.
//                                                                  // Bit12-0: Reserved for future use
#define OXP_DISCOVERY_INFO_PFR_VALID_BIT     0x2000                 // Bit-13 : 0/1, PFR related fields Not/Yes valid
#define OXP_DISCOVERY_INFO_MK_TME_VALID_BIT  0x4000                 // Bit-14 : 0/1, MKTME related fields Not/Yes valid
#define OXP_DISCOVERY_INFO_VALID_BIT  OXP_ADDITIONAL_INFO_VALID_BIT // Bit-15 : 0/1, Discovery info Not/Yes valid
//
// TaskSupported: Provides the bit-mapped information of the supported tasks.
//   Bit-N = 0/1, Task-N Not/Yes supported
//   Since maximum number of tasks that can be supported is 0x80 (128 decimal),
//   the length of this field is 128bits i.e. 0x10 (16 decimal) bytes.
//
// MaxMkTmeKeys: Provides the maximum number of MK-TME Keys available for use.
//   This value of this field may not be a power of 2.
//   The maximum value of this field is (2^MaxMkTmeKeyIdBits) – 1.
//   A value of 0 in this field indicates that MK-TME is not supported.
//
// MaxMkTmeKeyIdBits: Provides the maximum number of bits allowed for use as Key Identifiers for MK-TME.
//   A value of 0 in this field indicates that MK-TME is not supported.
//
// MkTmeKeyIdBits: Provides the number of bits allocated for use as Key Identifiers for MK-TME.
//   This field is valid only if MK-TME is enabled.
//
// PfrState: Provides bit-mapped information of current state of PFR.
//   This field is valid only if PFR is supported in the platform.
//                                                  // Bit3-0  : Reserved (set to 0)
#define OXP_DI_PFR_LOCKED_BIT               0x0010  // Bit-4   : 0/1, PFR Not/Yes locked
#define OXP_DI_PFR_PROVISIONED_BIT          0x0020  // Bit-5   : 0/1, PFR Not/Yes provisioned
#define OXP_DI_PFR_PIT_L1_ENABLED_BIT       0x0040  // Bit-6   : 0/1, PIT Level-1 Protection Not/Yes enabled
#define OXP_DI_PFR_PIT_L2_ENABLED_BIT       0x0080  // Bit-7   : 0/1, PIT Level-2 Protection Not/Yes enabled
#define OXP_DI_PFR_NUM_CPLD_BIT_MASK        0x0F00  // Bit11-8 : #of (1-based) CPLD present
#define   OXP_DI_PFR_NUM_CPLD_BIT_N           0x08  //           #of bit shift for alignment
                                                    // Bit13-12: Reserved (set to 0)
#define OXP_DI_PFR_FLASH_UPDATE_BIT         0x4000  // Bit-14  : 0/1, PFR Flash Update not/yes supported outside SMM
#define OXP_DI_PFR_SUPPORTED_BIT            0x8000  // Bit-15  : 0/1, PFR Not/Yes supported in platform
//
// PfrRecoveryCount: Provides the number of firmware recovery action since last AC Power Cycle.
//   This field is valid only (if PFR is supported) AND (if PFR is provisioned).
//   Please see PFR HAS 1.2 for details.
//
// PfrLastRecoveryReason: Provides the enumerated value of the last recovery reason.
//   This field is valid only (if PFR is supported) AND (if PFR is provisioned).
//   Please see PFR HAS 1.2 for details.
//
// PfrPanicEventCount: Provides the number of panic events (T0 mode to T-1 mode transitions) since last AC Power Cycle.
//   This field is valid only (if PFR is supported) AND (if PFR is provisioned).
//   Please see PFR HAS 1.2 for details.
//
// PfrLastPanicReason: Provides the enumerated value of the last panic reason.
//   This field is valid only (if PFR is supported) AND (if PFR is provisioned).
//   Please see PFR HAS 1.2 for details.
//
//--------------------------------------------------------
// OXP_MK_TME_INFO: For the TME/MKTME task (Use-Case), the caller must supply
//   the additional information in OXP_MK_TME_INFO format.
//
typedef    struct   {                          // Offset  Description
    UINT8  Identifier;                         // 0x00    Identifier: INPUT
    UINT16 Length;                             // 0x01    Length of OXP_MF_TME_INFO in bytes: INPUT/OUTPUT
    UINT16 Attribute;                          // 0x03    Bit-mapped Attribute: INPUT/OUTPUT
    UINT16 MaxKeys;                            // 0x05    Max #of Keys for use
    UINT8  MaxKeyIdBits;                       // 0x07    Max #of bits allowed for use as Key Identifiers for MK-TME
} OXP_MK_TME_INFO;
//
#define OXP_MK_TME_INFO_MIN_LENGTH  sizeof (OXP_MK_TME_INFO)  // Min length of OXP_MK_TME_INFO
//
// Identifier: Identifies the type of additional information, 0x04 for MK-TME Information
//
// Length: Provides the length D of OXP_MK_TME_INFO in bytes.
//
// Attribute: Indicates the nature of TMe/MK-TME operation.
#define OXP_MTI_ENABLE_TME_BIT                      0x0001    // Bit-0:   Whether to enable TME
                                                              //          INPUT : 0/1, Not/Yes enable TME
                                                              //          OUTPUT: Unchanged
#define OXP_MTI_DISABLE_TME_BIT                     0x0002    // Bit-1:   Whether to disable TME
                                                              //          INPUT : 0/1, Not/Yes disable TME
                                                              //          OUTPUT: Unchanged
#define OXP_MTI_ENABLE_MKTME_BIT                    0x0004    // Bit-2:   Whether to enable MK-TME
                                                              //          INPUT : 0/1, Not/Yes enable MK-TME
                                                              //          OUTPUT: Unchanged
#define OXP_MTI_DISABLE_MKTME_BIT                   0x0008    // Bit-3:   Whether to disable MK-TME
                                                              //          INPUT : 0/1, Not/Yes disable MK-TME
                                                              //          OUTPUT: Unchanged
#define OXP_MTI_PROGRAM_KEY_BIT                     0x0010    // Bit-4:   Whether to program Keys information supplied in MaxKeys, MaxKeyIdBits field
                                                              //          INPUT : 0/1, Not/Yes program of keys information
                                                              //          OUTPUT: Unchanged
#define   OXP_MTI_TASK_BIT_MASK                       0x1F    //     Bit-mask for tasks
                                                              // Bit7-5:  Reserved (set to 0)
#define OXP_MTI_STATUS_BIT_MASK                     0x0F00    // Bit11-8: Task status
#define   OXP_MTI_STATUS_BIT_N                        0x08    //          #of bit shift for alignment
                                                              //          INPUT : Don't care
                                                              //          OUTPUT: Task initiation status
#define   OXP_MTI_STATUS_TASK_INITIATED               0x00    //             00 = Task initiated
#define   OXP_MTI_STATUS_TASK_NONE                    0x01    //             01 = Task not requested
#define   OXP_MTI_STATUS_TASK_INVALID                 0x02    //             02 = Task invalid
#define   OXP_MTI_STATUS_TME_NOT_SUPPORTED            0x03    //             03 = TME not supported
#define   OXP_MTI_STATUS_MKTME_NOT_SUPPORTED          0x04    //             04 = MKTME not supported
#define   OXP_MTI_STATUS_TME_ALREADY_IN_STATE         0x05    //             05 = TME already in requested state (i.e. already en(dis)abled if requested to en(dis)able
#define   OXP_MTI_STATUS_MKTME_ALREADY_IN_STATE       0x06    //             06 = MKTME already in requested state (i.e. already en(dis)abled if requested to en(dis)able
#define   OXP_MTI_STATUS_PROGRAM_KEY_NOT_SUPPORTED    0x07    //             07 = Key programming not supported
                                                              //             08-0F....Reserved
                                                              // Bit14-12: Reserved (set to 0)
#define OXP_MTI_INFO_VALID_BIT  OXP_ADDITIONAL_INFO_VALID_BIT // Bit-15:  INPUT/OUTPUT : 0/1, OXP_MK_TME_INFO Not/Yes valid
//
//
// MaxKeys: Provides the #of Keys available for use.
//     INPUT : Number of keys to be set. This field is valid only if Bit-4 of Attribute field is set to 1.
//     OUTPUT: The maximum number of keys available for use. This field is always valid.
//
// MaxKeyIdBits: Provides the #of bits allocated for use as Key Identifiers for MK-TME.
//     INPUT : Number of bits to be allocated. This field is valid only if Bit-4 of Attribute field is set to 1.
//     OUTPUT: The maximum number of bits allowed for use as Key Identifiers for MK-TME. This field is always valid.
//
//--------------------------------------------------------
// OXP_PFR_INFO: For the PFR task (Use-Case), the caller must supply
//   the additional information in OXP_PFR_INFO format.
//
typedef    struct   {                          // Offset  Description
    UINT8  Identifier;                         // 0x00    Identifier: INPUT
    UINT16 Length;                             // 0x01    Length of OXP_PFR_INFO in bytes: INPUT/OUTPUT
    UINT16 Attribute;                          // 0x03    Bit-mapped Attribute: INPUT/OUTPUT
} OXP_PFR_INFO;
//
#define OXP_PFR_INFO_MIN_LENGTH  sizeof (OXP_PFR_INFO)  // Min length of OXP_PFR_INFO
//
// Identifier: Identifies the type of additional information, 0x06 for PFR Information
//
// Length: Provides the length D of OXP_PFR_INFO in bytes.
//
// Attribute: Indicates the nature of PFR operation.
#define OXP_PI_PROVISION_BIT                       0x0001    // Bit-0:   PFR Provision
                                                             //          INPUT : 0/1, Not/Yes perform Provision
                                                             //          OUTPUT: Unchanged
#define OXP_PI_UNPROVISION_BIT                     0x0002    // Bit-1:   PFR UnProvision
                                                             //          INPUT : 0/1, Not/Yes perform UnProvision
                                                             //          OUTPUT: Unchanged
#define OXP_PI_LOCK_BIT                            0x0004    // Bit-2:   PFR Lock
                                                             //          INPUT : 0/1, Not/Yes perform Lock
                                                             //          OUTPUT: Unchanged
#define   OXP_PI_TASK_BIT_MASK                       0x07    //     Bit-mask of PFR task
                                                             // Bit7-3:  Reserved (set to 0)
#define OXP_PI_STATUS_BIT_MASK                     0x0F00    // Bit11-8: Task status
#define   OXP_PI_STATUS_BIT_N                        0x08    //          #of bit shift for alignment
                                                             //          INPUT : Don't care
                                                             //          OUTPUT: Task initiation status
#define   OXP_PI_STATUS_TASK_INITIATED               0x00    //             00 = Task initiated
#define   OXP_PI_STATUS_TASK_NONE                    0x01    //             01 = Task not requested
#define   OXP_PI_STATUS_TASK_INVALID                 0x02    //             02 = Task invalid
#define   OXP_PI_STATUS_TASK_NOT_SUPPORTED           0x03    //             03 = Task not supported
#define   OXP_PI_STATUS_PFR_NOT_PROVISIONED          0x04    //             04 = PFR not yet provisioned
#define   OXP_PI_STATUS_PFR_ALREADY_PROVISIONED      0x05    //             05 = PFR already provisioned
#define   OXP_PI_STATUS_PFR_ALREADY_UNPROVISIONED    0x06    //             06 = PFR already unprovisioned
#define   OXP_PI_STATUS_PFR_ALREADY_LOCKED           0x07    //             07 = PFR already locked
                                                             //             08-0F....Reserved
                                                             // Bit14-12: Reserved (set to 0)
#define OXP_PI_INFO_VALID_BIT  OXP_ADDITIONAL_INFO_VALID_BIT // Bit-15:  INPUT/OUTPUT : 0/1, OXP_PFR_INFO Not/Yes valid
//
//--------------------------------------------------------
//
typedef union {
  OXP_PASSWORD_INFO   PasswordInfo;
  OXP_DISCOVERY_INFO  DiscoveryInfo;
//  OXP_TPM_NV_INFO   TpmNvInfo;
  OXP_MK_TME_INFO     MkTmeInfo;
//  OXP_SGX_INFO      SgxInfo;
  OXP_PFR_INFO        PfrInfo;
} OXP_ADDITIONAL_INFO;
//
//--------------------------------------------------------
// OOB_TXT_TPM structure: Data format stored in ME non-volatile space.
//
typedef    struct    {
  OXP_HEADER           Header;            // Valid for all tasks
  OXP_ADDITIONAL_INFO  AdditionalInfo;    // Valid for tasks requiring additional information
} OOB_TXT_TPM;
//
//-----------------------------------------------------------------------------------------------------
// ******************** OOB Implementation ************************************************************
//-----------------------------------------------------------------------------------------------------
//
#define EFI_OOB_TXT_TPM_DATA_GUID \
  { \
    0x2a8ec022, 0x11d0, 0x43bb, { 0xa7, 0x18, 0xf0, 0x08, 0x22, 0x39, 0xd8, 0xd0 } \
  }
//
#define EFI_OOB_TXT_TPM_DATA_VARIABLE  L"OobTxtTpmData"
//
//------------------------------------------------------------------------
// FATAL Error during OOB data processing (NOT stored anywhere, but displayed in DEBUG messages)
#define OOB_FATAL_MEMORY_NOT_AVAILABLE_BIT       0x00000001 // Bit-0:  0/1, Memory yes/not available for platform information
#define OOB_FATAL_NV_STORAGE_SIZE_ZERO_BIT       0x00000002 // Bit-1:  0/1, OOB Data Non-Volatile Storage Size not/yes Zero bytes
#define OOB_FATAL_NV_STORAGE_NOT_SUPPORTED_BIT   0x00000004 // Bit-2:  0/1, OOB Data Non-Volatile Storage yes/not supported
#define OOB_FATAL_PLATFORM_DATA_READ_ERROR_BIT   0x00000008 // Bit-3:  0/1, Platform Data Read Success/Error
#define OOB_FATAL_OOB_DATA_WRITE_ERROR_BIT       0x00000010 // Bit-4:  0/1, Write OOB Data to NV Storage Success/Error
                                                            // Bit31-5......Reserved for future use
//
//-----------------------------------------------------------------------------------------------------
// Debug macro: Can be used for debugging
#define JMP_$() {volatile int j; j = 1; while (j);}
//
//-----------------------------------------------------------------------------------------------------
/**
  Fill the bit-mapped information of supported tasks.

  @param  Buffer  INPUT: UINT8*, Ptr to buffer where bit-mapped information will be filled
  @param  Size    INPUT: UINTN,  Size of supplied buffer in bytes

  @retval	None
**/
VOID
FillOobSupportedTaskInfo (
  IN  UINT8  *Buffer,
  IN  UINTN  Size
  );
//
//-----------------------------------------------------------------------------------------------------
#pragma pack (pop)
//-----------------------------------------------------------------------------------------------------

#endif
