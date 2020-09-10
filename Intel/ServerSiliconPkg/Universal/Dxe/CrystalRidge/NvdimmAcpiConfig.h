/** @file NvdimmAcpiConfig.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _NVDIMM_ACPI_CONFIG_H_
#define _NVDIMM_ACPI_CONFIG_H_

/**
  Populates Control Region Operation Region's offset and length properties (CCT#)

  Extracts region index from the template and populates offset and length properties accordingly
  Note: the function expects that both offset and length properties are encoded as quad-word,
        otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of Operation Region's offset

**/
VOID
EFIAPI
PopulateControlRegion (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Flush Region Operation Region's offset and length properties (CFH#)

  Extracts region index from the template and populates offset and length properties accordingly
  Note: the function expects that both offset and length properties are encoded as quad-word,
        otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of Operation Region's offset

**/
VOID
EFIAPI
PopulateFlushRegion (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Mailbox Command Register Field (N###.XCMD)

  Extracts DIMM location from the template and populates object's offset accordingly.

  @param[in,out]  TablePointer  Pointer to XCMD Field Opcode

**/
VOID
EFIAPI
PopulateCommandRegister (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Mailbox Status Register Field (N###.XSTA)

  Extracts DIMM location from the template and populates object's offset accordingly.

  @param[in,out]  TablePointer  Pointer to XSTA Field Opcode

**/
VOID
EFIAPI
PopulateStatusRegister (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Input Payload Register Fields (N###.IN##)

  Extracts DIMM location and register numbers from templates
  and populates objects' offset and name properties accordingly.

  @param[in,out]  TablePointer  Pointer to IN01 field opcode.

**/
VOID
EFIAPI
PopulateInputPayloadRegister (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Output Payload Register Fields (N###.OU##)

  Extracts DIMM location and register numbers from templates
  and populates objects' offset and name properties accordingly.

  @param[in,out]  TablePointer  Pointer to OU01 field opcode.

**/
VOID
EFIAPI
PopulateOutputPayloadRegister (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Boot Status Register Field (N###.XBSR)

  Extracts DIMM location from the template and populates object's offset property accordingly.

  @param[in,out]  TablePointer  Pointer to XBSR Field Opcode

**/
VOID
EFIAPI
PopulateBootStatusRegister (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates DIMM Enable Status object (N###.CENA)

  Extracts DIMM location from the template and checks if the DIMM is enabled,
  then populates object's value accordingly.
  Note: the function expects object's value to be encoded as quad-word, otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of object value

**/
VOID
EFIAPI
PopulateDimmEnableStatus (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates FIS version object (N###.CFIS)

  Note: the function expects object's value to be encoded as quad-word, otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of object value

**/
VOID
EFIAPI
PopulateFisVersion (
  IN OUT UINT8 *TablePointer
  );

/**
  Populates Flush Address Fields (N###.FL##)

  Extracts DIMM location and register numbers from templates
  and populates objects' offset and name properties accordingly.

  @param[in,out]  TablePointer  Pointer to FL01 field opcode.
**/
VOID
EFIAPI
PopulateFlushAddress (
  IN OUT UINT8 *TablePointer
  );

/**
  Installs NVDIMM ACPI Config Protocol

  @param[in]  Handle  Handle to install the protocol on.

  @retval     EFI_SUCCESS         Protocol installed.
  @retval     <EFI error status>  An error occurred. Protocol not installed.

**/
EFI_STATUS
InstallNvdimmAcpiConfigProtocol (
  IN EFI_HANDLE  *Handle
  );

#endif // _NVDIMM_ACPI_CONFIG_H_
