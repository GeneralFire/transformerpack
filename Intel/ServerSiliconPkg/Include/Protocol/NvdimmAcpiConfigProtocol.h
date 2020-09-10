/** @file NvdimmAcpiConfigProtocol.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _NVDIMM_ACPI_CONFIG_PROTOCOL_H_
#define _NVDIMM_ACPI_CONFIG_PROTOCOL_H_

#define REGION_TYPE_CONTROL_REGION 1
#define REGION_TYPE_FLUSH_REGION   2

#define MAGIC_COMMAND_REG        0x180
#define MAGIC_STATUS_REG         0x181
#define MAGIC_BOOT_STATUS_REG    0x182
#define MAGIC_FLUSH_ADDRESS      0x183
#define MAGIC_INPUT_PAYLOAD_REG  0x1AA
#define MAGIC_OUTPUT_PAYLOAD_REG 0x1BB
#define MAGIC_NEXT_OFFSETS       0x100
/**
  Populates Control Region Operation Region's offset and length properties (CCT#)

  @param[in,out]  TablePointer  Pointer to size prefix of Operation Region's offset

**/
typedef
VOID
(EFIAPI *POPULATE_CONTROL_REGION) (
  IN OUT UINT8 *TablePointer
  );

typedef
VOID
(EFIAPI *POPULATE_FLUSH_REGION) (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Mailbox Command Register Field (N###.XCMD)

  @param[in,out]  TablePointer  Pointer to XCMD Field Opcode

**/
typedef
VOID
(EFIAPI *POPULATE_COMMAND_REGISTER) (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Mailbox Status Register Field (N###.XSTA)

  @param[in,out]  TablePointer  Pointer to XSTA Field Opcode

**/
typedef
VOID
(EFIAPI *POPULATE_STATUS_REGISTER) (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Input Payload Register Fields (N###.IN##)

  @param[in,out]  TablePointer  Pointer to IN01 field opcode.

**/
typedef
VOID
(EFIAPI *POPULATE_INPUT_PAYLOAD_REGISTER) (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Output Payload Register Fields (N###.OU##)

  @param[in,out]  TablePointer  Pointer to OU01 field opcode.

**/
typedef
VOID
(EFIAPI *POPULATE_OUTPUT_PAYLOAD_REGISTER) (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Boot Status Register Field (N###.XBSR)

  @param[in,out]  TablePointer  Pointer to XBSR Field Opcode

**/
typedef
VOID
(EFIAPI *POPULATE_BOOT_STATUS_REGISTER) (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates DIMM Enable Status object (N###.CENA)

  @param[in,out]  TablePointer  Pointer to size prefix of object value

**/
typedef
VOID
(EFIAPI *POPULATE_DIMM_ENABLE_STATUS) (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates FIS Version object (N###.CFIS)

  @param[in,out]  TablePointer  Pointer to size prefix of object value

**/
typedef
VOID
(EFIAPI *POPULATE_FIS_VERSION) (
  IN OUT UINT8 *TablePointer
  );

typedef
VOID
(EFIAPI *POPULATE_FLUSH_ADDRESS) (
  IN OUT UINT8 *TablePointer
  );

typedef struct {
  POPULATE_CONTROL_REGION          PopulateControlRegion;
  POPULATE_FLUSH_REGION            PopulateFlushRegion;
  POPULATE_COMMAND_REGISTER        PopulateCommandRegister;
  POPULATE_STATUS_REGISTER         PopulateStatusRegister;
  POPULATE_INPUT_PAYLOAD_REGISTER  PopulateInputPayloadRegister;
  POPULATE_OUTPUT_PAYLOAD_REGISTER PopulateOutputPayloadRegister;
  POPULATE_BOOT_STATUS_REGISTER    PopulateBootStatusRegister;
  POPULATE_DIMM_ENABLE_STATUS      PopulateDimmEnableStatus;
  POPULATE_FIS_VERSION             PopulateFisVersion;
  POPULATE_FLUSH_ADDRESS           PopulateFlushAddress;
} EFI_NVDIMM_ACPI_CONFIG_PROTOCOL;

#endif
