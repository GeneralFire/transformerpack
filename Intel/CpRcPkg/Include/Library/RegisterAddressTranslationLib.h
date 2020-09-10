/** @file
  The Register Address Translation Library API provides the functions which
  convert SoC neutral IP accesses into real silicon accesses.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef __REGISTER_ADDRESS_TRANSLATION_LIB_H__
#define __REGISTER_ADDRESS_TRANSLATION_LIB_H__

#include <RegisterAddressTranslation.h>

/**
  Returns number of IPs in system topology for given socket

  @param[in] Socket - Which socket to check

  @retval             IP count
**/
UINT16
EFIAPI
GetIpCount (
  IN UINT8     Socket
  );

/**
  Returns ID of IP at given index and socket

  @param[in] Socket - Which socket to check
  @param[in] Index  - Which index to check

  @retval             0 if index is out of bounds, valid ID otherwise
**/
UINT64
EFIAPI
GetIpId (
  IN UINT8       Socket,
  IN UINT16      Index
  );

/**
  Returns instance count of IP with given ID and socket

  @param[in] Socket - Which socket to check
  @param[in] Id     - Which ID to search for

  @retval             0 if IP not found, instance count 0 otherwise
**/
UINT16
EFIAPI
GetIpInstanceCount (
  IN UINT8     Socket,
  IN UINT64    Id
  );

/**
  Returns the address of the requested IP instance's requested accessor.
  This is the base function that IP drivers will use to access hardware,
  by locating the base of an IP and then issuing accesses to registers
  (offsets) from that base.

  @param[in] Socket   - Which socket to check
  @param[in] Id       - Which ID to search for
  @param[in] Instance - Which instance to decode
  @param[in] Accessor - Which access method to return

  @retval               NULL if register address could not be determined,
                        valid address otherwise
**/
VOID*
EFIAPI
GetIpBaseAddress (
  IN UINT8     Socket,
  IN UINT64    Id,
  IN UINT16    Instance,
  IN UINT8     Accessor
  );

/**
  Returns the address of the requested IP instance's first accessor

  @param[in] Socket   - Which socket to check
  @param[in] Id       - Which ID to search for
  @param[in] Instance - Which instance to decode

  @retval               NULL if register address could not be determined,
                        valid address otherwise
**/
VOID*
EFIAPI
GetIpBaseAddressDefault (
  IN UINT8     Socket,
  IN UINT64    Id,
  IN UINT16    Instance
  );

/**
  Returns the address of the requested IP instance's requested accessor

  @param[in] Id       - Which ID to search for
  @param[in] Instance - Which instance to decode
  @param[in] Accessor - Which access method to return

  @retval               NULL if register address could not be determined,
                        valid address otherwise
**/
VOID*
EFIAPI
GetIpBaseAddressNoSocket(
  IN UINT64    Id,
  IN UINT16    Instance,
  IN UINT8     Accessor
  );

/**
  Returns the address of the requested IP instance's first accessor

  @param[in] Id       - Which ID to search for
  @param[in] Instance - Which instance to decode

  @retval               NULL if register address could not be determined,
                        valid address otherwise
**/
VOID*
EFIAPI
GetIpBaseAddressDefaultNoSocket (
  IN UINT64    Id,
  IN UINT16    Instance
  );

#endif // #ifndef __REGISTER_ADDRESS_TRANSLATION_LIB_H__
