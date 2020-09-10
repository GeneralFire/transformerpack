/** @file AcpiNotify.h
  ACPI Notification handling

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

/**
  Clears notification status from all NVDIMM leaf devices
**/
VOID
EFIAPI
AcpiClearLeafNotifications (
  VOID
  );


/**
  Adds notification of specified NVDIMM leaf device

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index

**/
VOID
EFIAPI
AcpiAddLeafNotification (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm);


/**
  Triggers ACPI notification of NVDIMM leaf devices
**/
VOID
EFIAPI
AcpiLeafNotify (
  VOID
  );


/**
  Clears NVDIMM root device notification status
**/
VOID
EFIAPI
AcpiClearRootNotification (
  VOID
  );


/**
  Adds to error list for specified DIMM and DPA

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index
  @param[in]   Dpa            DPA
  @param[in]   RangeExponent  DPA range exponent (base 2)

**/
VOID
EFIAPI
AcpiAddToErrorList (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT64 Dpa,
  IN UINT8  RangeExponent
  );


/**
  Adds NVDIMM root device notification for specified DIMM and DPA

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index
  @param[in]   Dpa            DPA
  @param[in]   RangeExponent  DPA range exponent (base 2)

**/
VOID
EFIAPI
AcpiAddRootNotification (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT64 Dpa,
  IN UINT8  RangeExponent
  );


/**
  Triggers ACPI notification of NVDIMM root device

  @param[in]   Force  TRUE:  notify even if no notification is pending
                      FALSE: notify only if a notification is pending
**/
VOID
EFIAPI
AcpiRootNotifyEx (
  IN BOOLEAN Force
  );

/**
  Triggers ACPI notification of NVDIMM root device if such notification is pending
**/
VOID
EFIAPI
AcpiRootNotify (
  VOID
  );
