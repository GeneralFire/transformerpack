/** @file
 Definitions for HECI Transport driver

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/
#ifndef _HECI_TRANSPORT_CORE_LIB_H_
#define _HECI_TRANSPORT_CORE_LIB_H_

#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciAccessProtocol.h>

#pragma pack (1)
/**
  HECI packet header.

  HECI packet header is one double word long. It identifies communication
  entities (subsystems) on engine and host side and provides the exact number
  of bytes in the packet body following this header. Several packets may
  establish one HECI message. In this case the last and only the last packet
  must have Complete bit set to 1.
**/
typedef union {
  UINT32   Data;
  struct {
    UINT32 EngineAddress   : 8;  ///< Address on engine (ME/IE) side
    UINT32 HostAddress     : 8;  ///< Address on host side, zero for BIOS
    UINT32 Length          : 9;  ///< Number of bytes following the header
    UINT32 Reserved        : 6;
    UINT32 MessageComplete : 1;  ///< Whether this is last fragment of a message
  } Fields;
} HECI_PACKET_HEADER;
#pragma pack ()

typedef struct {
  HECI_TRANSPORT      HeciTransport;
  // Related HeciAccess pointer, Heci Transport is based on
  HECI_ACCESS         *HeciAccess;           /// HeciAccess pointer to link Heci Transport and Access in PostMem
  UINT32              HeciAccessIndex;       /// HeciAccess index to link Heci Transport and Access in PreMem
  VOID                *HeciTransportRoot;    /// HeciTransport list root
  VOID                *HeciTransportNext;    /// Next HeciTransport protocol (used in Smm/Dxe))

  BOOLEAN             IsInitialized;         ///< Flag if interface is successfully initialized
  // By PchIndex & PciFunction we are able to match Transport and access during relink
  UINTN               PciSegment;            ///< PCI segment where this HECI instance is located
  UINTN               PciBus;                ///< PCI bus where this HECI instance is located
  UINTN               PciDevice;             ///< PCI device where this HECI instance is located
  UINTN               PciFunction;           ///< PCI function number of this HECI instance
  //
  // Default HECI timeouts
  //
  UINT32              SendTimeout;           ///< HECI default send timeout
  UINT32              ReceiveTimeout;        ///< HECI default receive timeout
  UINT32              ResetTimeout;          ///< HECI default reset timeout
} HECI_TRANSPORT_PRIVATE;

/**
  Initialize HECI_TRANSPORT protocol functions

  @param[in]     HeciTransport         HECI transport pointer the be initialized
**/
VOID
SetHeciTransportFunctions (
  IN OUT HECI_TRANSPORT_PRIVATE *Heci
  );

/**
  Initialize HECI_TRANSPORT protocol instance

  @param[in]     HeciTransport HECI transport pointer the be initialized
  @param[in]     HeciAccess    Associated HECI access instance

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_SUCCESS           Heci transport initialized
**/
EFI_STATUS
HeciInitializeNewTransportInstance (
  IN OUT HECI_TRANSPORT_PRIVATE *Heci,
  IN     HECI_ACCESS            *HeciAccess
  );

/**
  Function returns HeciAccess for given HeciTransport
  Function implementation is deliver by parent module to HeciTransportCore library

  @param[in] HeciTransport        HEciTransport for getting HeciAccess


  @retval HECI_ACCESS*            On success valid pointer to linked HeciAccess is returned
  @retval NULL                    If HEciAccess can't be finded NULL is returned
**/
HECI_ACCESS*
GetHeciAccess (
  HECI_TRANSPORT_PRIVATE         *HeciTransport
 );

/**
  Get Next HECI Transport

  @param[in]     This          Pointer to HECI Transport device context structure
  @param[in]     HeciTransport Pointer to HECI Transport device context structure
                               the next element should be relative to
                               Passing NULL requests first element

  @retval HECI_TRANSPORT Next HeciTransport object
**/
HECI_TRANSPORT*
GetNextHeciTransport (
  IN      HECI_TRANSPORT   *This,
  IN      HECI_TRANSPORT   *HeciTransport
  );

/**
  Get HECI_TRANSPORT instance for given PchIndex and HeciFunction in HeciAccess.
  (PEI only)

  @param[in]     HeciAccess           Heci Access that parameters HeciAccess Has to match
  @param[in,out] HeciCurrentInstance  Pointer to found instance id

  @retval HECI_TRANSPORT pointer
  @retval NULL pointer if no HECI_TRANSPORT_PRIVATE is found
**/
HECI_TRANSPORT*
GetHeciTransportForHeciAccess (
  IN     HECI_ACCESS      *HeciAccess,
  IN OUT UINTN            *HeciCurrentInstance
  );

#endif // _HECI_TRANSPORT_CORE_LIB_H_

