/** @file
  Prototypes for IIO System information

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_SYS_INFO_LIB_H_
#define _IIO_SYS_INFO_LIB_H_
/**

  Get max port number per socket

  @retval    Max vmd ports per socket
**/
UINT8
EFIAPI
GetMaxPortNumPerSocket (
  VOID
  );

/**

  Get max IOU number per socket

  @retval    Max IOU number per socket
**/
UINT8
EFIAPI
GetMaxIouNumPerSocket (
  VOID
  );

/**

  Get Max NTB ports per socket

  @retval    Max NTB ports per socket
**/
UINT8
EFIAPI
GetMaxNtbNumPerSocket (
  VOID
 );



/**

  Get max port number per IOU

  @retval    Max ports per IOU
**/
UINT8
EFIAPI
GetMaxPortNumPerIou (
  VOID
  );



/**

  Get max stack number per socket

  @retval    Max vmd ports per socket
**/
UINT8
EFIAPI
GetMaxStackNumPerSocket (
  VOID
  );

/**

  Get cluster port index by NTB index

  @param     NtbIndex - NTB index
  @retval    Cluster port Index
**/
UINT8
EFIAPI
GetClusterPortByNtbIndex (
  IN UINT8    NtbIndex
  );

/**

  Get stack index by IOU index

  @param     IouIndex - IOU index
  @retval    Stack Index
**/
UINT8
EFIAPI
GetStackIndexByIouIndex (
  IN UINT8    IouIndex
  );

/**

  GetPortPerStack: Returns the maximum number of ports per
  stack

  @param Stack - Stack number

  @retval PortNumber - Number of ports defined by stack.

**/
UINT8
EFIAPI
GetMaxPortNumPerStack (
  IN  UINT8   Stack
  );


/**
  Check if Port will be configured as NTB

  @param[in]   PortIndex         - Port Index
  @param[out]  NtbPortIndex      - pointer to NtbPortPerPortIndex

  @retval                        - EFI_UNSUPPORTED: Ntb is not supported on given PortIndex
                                   EFI_SUCCESS:     function is excuted successfully
**/
EFI_STATUS
GetNtbPortPerPortIndex (
  IN     UINT8                   PortIndex,
     OUT UINT8                   *NtbPortIndex
  );

/**
  Check if Device's function 0 is VMD dummy function

  Specific Device ID (0x09AB in VMD specification v3.0) indicates that function is VMD dummy function.

  @param DevId           - Device ID

  @retval TRUE           - Function is VMD dummy function
  @retval FALSE          - Function is not VMD dummy function

**/
BOOLEAN
IsVmdDummyFunction (
  IN  UINT16             DeviceId
  );


/**

    Convert port index in a stack to index in a socket
    @param PortIndex     - Stack Index
    @param PortIndex     - Port Index

    @retval port index in a socket

**/
UINT8
EFIAPI
GetPortIndexbyStack (
  IN UINT8    StackIndex,
  IN UINT8    PortIndex
  );

/**
 @brief Convert IIO port index to a stack and port in the stack

 @param[in]  PortIdx  - Port index in IIO instance (usually socket)
 @param[out] StackPtr - Optinal pointer to buffer for stack index.

 @return Index of port in its stack is returned, or -1 if illegal PortIndex was provided.
**/
UINT8
EFIAPI
IioPortIndexToStackPort (
  IN UINT8   PortIdx,
  IN UINT8  *StackPtr
  );

/**
  Returns default value of Max Payload Size for different projects.

  @return 8-bit default value indicating Max Payload Size
**/
UINT8
GetDefaultMaxPayloadSize (VOID);

#endif // _IIO_SYS_INFO_LIB_H_

