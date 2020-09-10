/** @file
  This file defines the PFR Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2015 Intel Corporation. <BR>

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

#ifndef _PFR_PROTOCOL_H_
#define _PFR_PROTOCOL_H_

//-----------------------------------------------------------------------------------------------------
#pragma pack (push, 1)
//-----------------------------------------------------------------------------------------------------
// Ref: The document "PFR Protocol in BIOS - Design and Implementation.pdf"
//----------------------------------------------------------------------------------------------
// Forward reference for ANSI compatability
typedef struct _PFR_PROTOCOL PFR_PROTOCOL;
//
//-----------------------------------------------------------------------------------------------------
// PFR_API_FUNC_INFO_02: Additional information for Function-02
//
typedef         struct {
  UINT8         Register;        // PFR Register# to be read
  UINT8         Data;            // Content of PFR Register after reading
} PFR_API_FUNC_INFO_02;
//
// Register: PFR Register Number to be read.
//      INPUT : The caller should initialize this field with the PFR Register to be read.
//      OUTPUT: Unchanged.
//
// Data: The content of PFR Register after reading.
//      INPUT : Don’t care. However, it is recommended that the caller should initialize
//              this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the content of PFR Register.
//              If not successful, the value in this field is undefined.
//
//-----------------------------------------------------------------------------------------------------
// PFR_API_FUNC_INFO_01: Additional information for Function-01
//
typedef         struct {
  UINT8         PchUpdateIntent;                         // Bit-mapped information of PCH Update Intent
} PFR_API_FUNC_INFO_01;
//
// PchUpdateIntent: Bit-mapped information of PCH Update Intent. One or more bit can be set to 1.
//      INPUT : The caller should initialize this field with the proper information.
#define PP01_PUI_UPDATE_DEFER_TILL_NEXT_RESET_BIT  0x80  // Bit-7 : 0/1, Not/Yes defer the update till next reset
#define PP01_PUI_UPDATE_DYNAMIC_REGION_BIT         0x40  // Bit-6 = 0/1, Not/Yes Update Dynamic Region (Valid for Update PCH SPI Active Image i.e. Bit-0 = 1)
                                                         // Bit5-3: Reserved for future use
#define PP01_PUI_UPDATE_CPLD_IMAGE_BIT             0x04  // Bit-2 : 0/1, Not/Yes update CPLD image
#define PP01_PUI_UPDATE_RECOVERY_IMAGE_BIT         0x02  // Bit-1 : 0/1, Not/Yes update PCH SPI Recovery Image
#define PP01_PUI_UPDATE_ACTIVE_IMAGE_BIT           0x01  // Bit-0 : 0/1, Not/Yes update PCH SPI Active Image
//      OUTPUT: Unchanged.
//
//-----------------------------------------------------------------------------------------------------
// PFR_API_FUNC_INFO_00: Additional information for Function-00
//
typedef         struct {
  UINT16        State;                         // Bit-mapped information of PFR State in the platform
  UINT8         MaxFunction;                   // Max Function# (0-based) supported by PFR Protocol
  UINT8         FlashRegion;                   // PFR Region# (0-based) in SPI Flash
  UINT32        FlashRegionBase;               // PFR Region Base Address
  UINT32        FlashRegionSize;               // PFR Region Size in bytes
  UINT32        StagingAreaBase;               // Staging Area Base Address
  UINT32        StagingAreaSize;               // Staging Area Size in bytes
  UINT32        RecoveryAreaBase;              // Recovery Area Base Address
  UINT32        RecoveryAreaSize;              // Recovery Area Size in bytes
  UINT32        PfmAreaBase;                   // Platform Firmware Manifest Area Base Address
  UINT32        PfmAreaSize;                   // Platform Firmware Manifest Area Size in bytes
} PFR_API_FUNC_INFO_00;
//
// State: Bit-mapped information of PFR State in the platform.
//      INPUT : Don’t care. However, it is recommended that the caller should initialize
//              this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper PFR State.
//              If not successful, the value in this field is undefined.
#define PP00_PS_PFR_SUPPORT_BIT        0x8000  // Bit-15  : 0/1, PFR not/yes supported in the platform
#define PP00_PS_PFR_FLASH_UPDATE_BIT   0x4000  // Bit-14  : 0/1, PFR Flash Update not/yes supported outside SMM
                                               // Bit13-12: Reserved for future use (set to 0)
#define PP00_PS_PFR_NUM_CPLD_BIT_MASK  0x0F00  // Bit11-8 : #of CPLD (1-based) present
#define   PP00_PS_PFR_NUM_CPLD_BIT_N     0x08  //           #of bit shift for alignment
                                               // Bit7-4  : Reserved for future use (set to 0)
#define PP00_PS_PFR_PIT_L2_ENABLED_BIT 0x0008  // Bit-3   : 0/1, PFR PIT Level-2 not/yes enabled
#define PP00_PS_PFR_PIT_L1_ENABLED_BIT 0x0004  // Bit-2   : 0/1, PFR PIT Level-1 not/yes enabled
#define PP00_PS_PFR_LOCKED_BIT         0x0002  // Bit-1   : 0/1, PFR not/yes locked
#define PP00_PS_PFR_PROVISIONED_BIT    0x0001  // Bit-0   : 0/1, PFR not/yes provisioned
//
// MaxFunction: Maximum Function (0-based) number supported by PFR Protocol implementation.
//      INPUT : Don’t care. However, it is recommended that the caller should initialize
//              this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the 0-based maximum function number
//              supported by PFR Protocol. If not successful, the value in this field is undefined.
//
// FlashRegion: Region number in SPI Flash used for PFR.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//              Note: On Output, a zero value in this field indicates PFR Region in Flash is not available.
//
// FlashRegionBase: Base address of the PFR Region in SPI Flash. This address is relative to the
//              start of Flash Device. The start of Flash Device is considered to be at address 0.
//              This field is valid only if FlashRegionSize field contains a non-zero value.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//
// FlashRegionSize: Size (in bytes) of the PFR Region in SPI Flash.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//              Note: On Output, a zero value in this field indicates PFR Region is not available.
//
// StagingAreaBase: Base address of the PFR Staging Area. This address is relative to the start
//              of Flash Device. The start of Flash Device is considered to be at address 0. The
//              Staging area is within the PFR Region in SPI Flash. This field is valid only if
//              StagingAreaSize field contains a non-zero value.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//
// StagingAreaSize: Size (in bytes) of the PFR Staging Area in SPI Flash.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//              Note: On Output, a zero value in this field indicates PFR Staging Area is not available.
//
// RecoveryAreaBase: Base address of the PFR Recovery Area. This address is relative to the start
//              of Flash Device. The start of Flash Device is considered to be at address 0. The
//              Recovery area is within the PFR Region in SPI Flash. This field is valid only if
//              RecoveryAreaSize field contains a non-zero value.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//
// RecoveryAreaSize: Size (in bytes) of the PFR Recovery Area in SPI Flash.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//              Note: On Output, a zero value in this field indicates PFR Recovery Area is not available.
//
// PfmAreaBase: Base address of the PFR Platform Firmware Manifest (PFM) Area. This address is
//              relative to the start of Flash Device. The start of Flash Device is considered
//              to be at address 0. The Platform Firmware Manifest area is within the PFR Region
//              in SPI Flash. This field is valid only if PfmAreaSize field contains a non-zero value.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//
// PfmAreaSize: Size (in bytes) of the PFR Platform Firmware Manifest Area in SPI Flash.
//      INPUT : Don’t care. However, the caller should initialize this field to zero.
//      OUTPUT: If successful, BIOS updates this field with the proper value.
//              If not successful, the value in this field is undefined.
//              Note: On Output, a zero value in this field indicates PFM Area is not available.
//
//-----------------------------------------------------------------------------------------------------
// PFR_API_HEADER
//
typedef         struct {
  PFR_PROTOCOL  *This;           // Pointer to PFR_PROTOCOL
  UINT16        HeaderSize;      // Size of PFR_API_HEADER in bytes
  UINT16        TotalSize;       // Total size of PFR_API_DATA
                                 //   = Size of (PFR_API_HEADER + PFR_API_FUNC_INFO_xx)
  UINT8         Checksum;        // Checksum
  UINT8         Function;        // Function# of the specific task
  UINT8         Status;          // Status of performed task
  UINT16        ExtendedStatus;  // Extended Status of performed task
} PFR_API_HEADER;
//
// This: Pointer to PFR_PROTOCOL.
//      INPUT : The caller MUST initialize this field to correct value. BIOS must validate this field
//              before performing the requested function.
//      OUTPUT: Unchanged.
//
// HeaderSize: Size of PFR_API_HEADER in bytes.
//      INPUT : The caller MUST initialize this field to correct value.BIOS must validate this field
//              before performing the requested function.
//      OUTPUT: Unchanged.
#define PFR_API_HEADER_SIZE_MIN    sizeof (PFR_API_HEADER)  // Minimum Header Size (in oldest PFR Protocol Specification Version 01.00)
//
// TotalSize: Total size of PFR_API_DATA in bytes i.e. size of PFR_API_HEADER + size of PFR_API_FUNC_INFO_XX.
//      INPUT : The caller MUST initialize this field to correct value. This value can be different
//              for different functions. BIOS must validate this field before performing the
//              requested function.
//      OUTPUT: Unchanged.
//
// Checksum: Checksum of PFR_API_DATA. The checksum is calculated in a manner so that the
//           byte-addition of all bytes (including this Checksum field) in PFR_API_DATA
//           is zero. Note that the size of PFR_API_DATA is indicated by TotalSize field.
//      INPUT : The caller MUST initialize this field to correct value. BIOS must validate this field
//              before performing the requested function.
//      OUTPUT: BIOS updates this field with correct value if any field in PFR_API_DATA is
//              modified on output. On return, the caller must validate this field before
//              using the information in PFR_API_DATA.
//
// Function: Function (0-based) number of the PFR task to be performed.
//      INPUT : The caller MUST initialize this field to proper value for the task to be performed.
//              The following PFR tasks are defined in this version.
//                                          Func#  PFR Task Description
#define PFR_API_FUNCTION_00        0x00	 // 00     Get PFR Protocol Information
#define PFR_API_FUNCTION_01        0x01  // 01     Write PCH Update Intent
#define PFR_API_FUNCTION_02        0x02  // 02     Read PFR Register
#define PFR_API_FUNCTION_03        0x03  // 03     Write PFR Register
#define PFR_API_FUNCTION_04        0x04  // 04     Provision PFR
#define PFR_API_FUNCTION_05        0x05  // 05     UnProvision PFR
#define PFR_API_FUNCTION_06        0x06  // 06     Lock PFR
#define PFR_API_FUNCTION_07        0x07  // 07     PIT Level-1 Protection
#define PFR_API_FUNCTION_08        0x08  // 08     PIT Level-2 Protection
                                         // 09-FF  Reserved for future use
#define PFR_API_FUNCTION_MAX       0x08  // Max Func# supported by PFR Protocol implementation
//      OUTPUT: Unchanged.
//
// Status: Status of the performed function. If not successful, the more error information may be
//         available in ExtendedStatus field.
//      INPUT : Don’t care. However, it is recommended that the caller should initialize this field
//              to any non-zero value.
//      OUTPUT: BIOS updates this field with the status of the performed function.
//              A Zero value indicates success and a non-zero value indicates an error.
//                                      // Status Code  Description
#define PFR_API_SUCCESS                  0x00  // 00    Success
#define PFR_API_FUNCTION_INVALID         0x01  // 01    Invalid function (requested function not supported)
#define PFR_API_HEADER_SIZE_INVALID      0x02  // 02    Invalid Header Size (Header Size in Header not correct)
#define PFR_API_TOTAL_SIZE_INVALID       0x03  // 03    Invalid Total Size (Total Size in Header not correct for the requested function)
#define PFR_API_CHECKSUM_INVALID         0x04  // 04    Invalid Checksum (Checksum in Header not correct)
#define PFR_API_PARAMETER_INVALID        0x05  // 05    Invalid parameter (one or more supplied parameter not valid)
#define PFR_API_REGISTER_READ_ERROR      0x06  // 06    PFR register read error
#define PFR_API_REGISTER_WRITE_ERROR     0x07  // 07    PFR register write error
#define PFR_API_PROVISION_ERROR          0x08  // 08    PFR Provision error
#define PFR_API_UNPROVISION_ERROR        0x09  // 09    PFR UnProvision error
#define PFR_API_LOCK_ERROR               0x0A  // 0A    PFR Lock error
#define PFR_API_PIT_LEVEL1_ERROR         0x0B  // 0B    PIT Level-1 Protection error
#define PFR_API_PIT_LEVEL2_ERROR         0x0C  // 0C    PIT Level-2 Protection error
                                               // 0D-FE Reserved for future use
#define PFR_API_UNKNOWN_ERROR            0xFF  // FF    Unknown error
//
// ExtendedStatus: Extended Status of the performed function.
//    a) This field is used to indicate the state of execution and more information about the error (if any)
//       which can be used to debug/solve a problem.
//    b) The following different information regarding the state of execution supported by this specification.
//       1) Whether BIOS updated any field in PFR_API_DATA: The caller should check this information determine
//          whether the BIOS has acted on the PFR_API_DATA at all.
//       2) Whether BIOS performed the requested function: The caller should check this information to determine
//          whether the requested function is performed. Note that this information does NOT indicate the
//          result (success or error) of the function. The result of the function is indicated by Status field.
//       INPUT : Don’t care. However, it is recommended that the caller should initialize this field to zero.
//       OUTPUT: BIOS updates this field is updated with proper value.
#define PFR_API_ES_DATA_UPDATED_BIT    0x8000 // Bit-15 : 0/1, BIOS Not/Yes updated some information in PFR_API_DATA
#define PFR_API_ES_FUNC_PERFORMED_BIT  0x4000 // Bit-14 : 0/1, BIOS Not/Yes performed the requested function
                                              // Bit13-0: Reserved for future use
//
//-----------------------------------------------------------------------------------------------------
// PFR_API_FUNC_INFO: Additional information specific to the function.
//
typedef union {
  PFR_API_FUNC_INFO_00     Function00;    // Additional Info for Function-00  
  PFR_API_FUNC_INFO_01     Function01;    // Additional Info for Function-01
  PFR_API_FUNC_INFO_02     Function02;    // Additional Info for Function-02
} PFR_API_FUNC_INFO;
//
//-----------------------------------------------------------------------------------------------------
// PFR_API_DATA: Structure passed to PFR API.
//
typedef         struct {
  PFR_API_HEADER       Header;   // PFR_API_HEADER
  PFR_API_FUNC_INFO    Info;     // PFR_API_FUNC_INFO
} PFR_API_DATA;
//
// Header: This field is valid for all functions.
//
// Info: The field provides the additional information for a particular function.
//       The content of this field depends on the specific function to be performed.
//
//-----------------------------------------------------------------------------------------------------
// PFR_API: Interface to PFR API Functions.
//
typedef
EFI_STATUS
(EFIAPI *PFR_API) (
  IN OUT PFR_API_DATA   *PfrApiData
  );
//
//-----------------------------------------------------------------------------------------------------
// PFR Protocol
//
struct _PFR_PROTOCOL {
  UINT8          MajorVersion;    // Major Version Number
  UINT8          MinorVersion;    // Minor Version Number
  PFR_API        PfrApi;          // Interface to PFR API Functions
};
//
// MajorVersion: Major Version number of PFR Protocol Interface.
//      Current major version number is 0x03.
//      The major version must be updated when a new function is supported via PFR_API.
#define PFR_PROTOCOL_MAJOR_VERSION      0x03  // Major version number
//
// MinorVersion: Minor Version number of PFR Protocol Interface.
//      Current minor version number is 0x00.
//      The minor version must be updated when any problem is solved in an existing function supported via PFR_API.
#define PFR_PROTOCOL_MINOR_VERSION      0x00  // Minor version number
//
// PfrApi: Interface to perform PFR tasks.
//
//-----------------------------------------------------------------------------------------------------
#pragma pack (pop)
//-----------------------------------------------------------------------------------------------------

#endif
