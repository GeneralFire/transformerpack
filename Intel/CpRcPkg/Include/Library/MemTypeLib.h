/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _MEMTYPELIB_H_
#define _MEMTYPELIB_H_

typedef enum {
  MemTypeNone = 0,
  MemType1lmDdr,
  MemType1lmAppDirect,
  MemType1lmAppDirectReserved,
  MemType1lmCtrl,
  MemType1lmHbm,
  MemTypeNxm,
  MemType2lmDdrCacheMemoryMode,
  MemType2lmDdrWbCacheAppDirect,
  MemType2lmHbmCacheDdr,
  MemType2lmHbmCacheMemoryMode,
  MemTypeCxlAccVolatileMem,
  MemTypeCxlAccPersistentMem,
  MemTypeFpga,
  MemTypeCxlExpVolatileMem,
  MemTypeCxlExpPersistentMem,
  MemTypeMax
} MEM_TYPE;

typedef BOOLEAN (*MEM_TYPE_CHECK) (MEM_TYPE);

/**

  Check if the input MemType is classified as "Reserved"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "Reserved"
  @retval FALSE - The input MemType is not classified as "Reserved"

**/
BOOLEAN
IsMemTypeReserved (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType is classified as "AppDirect"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "AppDirect"
  @retval FALSE - The input MemType is not classified as "AppDirect"

**/
BOOLEAN
IsMemTypeAppDirect (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType is classified as "2lm DDR"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "2lm DDR"
  @retval FALSE - The input MemType is not classified as "2lm DDR"

**/
BOOLEAN
IsMemType2lmDdr (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType is classified as "2lm"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "2lm"
  @retval FALSE - The input MemType is not classified as "2lm"

**/
BOOLEAN
IsMemType2lm (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType is classified as "Hbm"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "Hbm"
  @retval FALSE - The input MemType is not classified as "Hbm"

**/
BOOLEAN
IsMemTypeHbm (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType is classified as "2lm HBM"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "2lm HBM"
  @retval FALSE - The input MemType is not classified as "2lm HBM"

**/
BOOLEAN
IsMemType2lmHbm (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType is classified as "1lm"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "1lm"
  @retval FALSE - The input MemType is not classified as "1lm"

**/
BOOLEAN
IsMemType1lm (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType involves DDR Volatile Memory or DDRT Crystal Ridge media
  configured for volatile MemoryMode access.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves DDRT "MemoryMode" or DDR Volatile memory
  @retval FALSE - The input MemType does not involve DDRT "MemoryMode" or DDR Volatile memory

**/
BOOLEAN
IsMemTypeVolatile (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType involves DDR volatile memory.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves DDR memory type.
  @retval FALSE - The input MemType does not involve DDR memory type.

**/
BOOLEAN
IsMemTypeDdr (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType involves DDRT Crystal Ridge media.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves DDRT memory type.
  @retval FALSE - The input MemType does not involve DDRT memory type.

**/
BOOLEAN
IsMemTypeDdrt (
  MEM_TYPE MemType
  );

/**

  Check if the input MemType involves CXL memory device.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves CXL memory device.
  @retval FALSE - The input MemType does not involve CXL memory device.

**/
BOOLEAN
IsMemTypeCxl (
  MEM_TYPE MemType
  );

/**

  Check if the input channel contains a SAD table entry with a type that matches the IsMemType condition.

  @param[in] SocketId        Index of the socket for the input channel to check.
  @param[in] ChId            Index of the channel to check within socket.
  @param[in] IsMemType       Pointer to the function that will be used to check the SAD table entry types.

  @retval TRUE   The channel has one or more SAD table entries with the type matching the IsMemType condition.
  @retval FALSE  The channel does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
EFIAPI
IsMemTypePresentChFunction (
  IN UINT8          SocketId,
  IN UINT8          ChId,
  IN MEM_TYPE_CHECK IsMemType
  );

/**

  Check if the input channel contains a SAD table entry that matches the MemType element.

  @param[in] SocketId        Index of the Socket for the input channel to check.
  @param[in] ChId            Index of the channel to check within socket.
  @param[in] MemType         MemType element that will be used to check the SAD table entry types

  @retval TRUE   The channel has one or more SAD table entries with the type matching the MemType element.
  @retval FALSE  The channel does not have any SAD table entries with the type matching the MemType element.

**/
BOOLEAN
EFIAPI
IsMemTypePresentChEnum (
  IN UINT8    SocketId,
  IN UINT8    ChId,
  IN MEM_TYPE MemType
  );

/**

  Check if the input Mc contains a SAD table entry with a type that matches the IsMemType condition

  @param[in] Sckt      - Index of the Socket for the input MC to check
  @param[in] Mc        - Index of the Mc to check
  @param[in] IsMemType - A pointer to the function that will be used to check the SAD table entry types

  @retval TRUE  - The MC has one or more SAD table entries present with a type that matches the IsMemType condition.
  @retval FALSE - The MC does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
IsMemTypePresentMcFunction (
  IN UINT8          Sckt,
  IN UINT8          Mc,
  IN MEM_TYPE_CHECK IsMemType
  );

/**

  Check if the input Mc contains a SAD table entry that matches the MemType element

  @param[in] Sckt      - Index of the Socket for the input MC to check
  @param[in] Mc        - Index of the Mc to check
  @param[in] MemType   - MemType element that will be used to check the SAD table entry types

  @retval TRUE  - The MC has one or more memory modes present that match the MemType element
  @retval FALSE - The MC does not have memory modes matching the MemType element

**/
BOOLEAN
IsMemTypePresentMcEnum (
  IN UINT8    Sckt,
  IN UINT8    Mc,
  IN MEM_TYPE MemType
  );

/**

  Check if the input Mc contains no memory types.

  @param[in] Sckt      - Index of the Socket for the input MC to check
  @param[in] Mc        - Index of the Mc to check

  @retval TRUE  - The MC has no memory types present
  @retval FALSE - The MC has one or more memory types present

**/
BOOLEAN
IsMemTypeEmptyMc (
  IN UINT8    Sckt,
  IN UINT8    Mc
  );

/**

  Check if the input Socket contains a SAD table entry with a type that matches the IsMemType condition

  @param[in] Sckt      - Index of the Socket to check
  @param[in] IsMemType - A pointer to the function that will be used to check the SAD table entry types

  @retval TRUE  - The Socket has one or more SAD table entries present with a type that matches the IsMemType condition.
  @retval FALSE - The Socket does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
IsMemTypePresentSocketFunction (
  IN UINT8          Sckt,
  IN MEM_TYPE_CHECK IsMemType
  );

/**

  Check if the input Socket contains a SAD table entry that matches the MemType element

  @param[in] Sckt      - Index of the Socket to check
  @param[in] MemType   - MemType element that will be used to check the SAD table entry types

  @retval TRUE  - The Socket has one or more memory modes present that match the MemType element
  @retval FALSE - The Socket does not have memory modes matching the MemType element

**/
BOOLEAN
IsMemTypePresentSocketEnum (
  IN UINT8    Sckt,
  IN MEM_TYPE MemType
  );

/**

  Check if the System contains a SAD table entry with a type that matches the IsMemType condition

  @param[in] IsMemType - A pointer to the function that will be used to check the SAD table entry types

  @retval TRUE  - The System has one or more SAD table entries present with a type that matches the IsMemType condition.
  @retval FALSE - The System does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
IsMemTypePresentSystemFunction (
  IN MEM_TYPE_CHECK IsMemType
  );

/**

  Check if the input System contains a SAD table entry that matches the MemType element

  @param[in] MemType   - MemType element that will be used to check the SAD table entry types

  @retval TRUE  - The System has one or more memory modes present that match the MemType element
  @retval FALSE - The System does not have memory modes matching the MemType element

**/
BOOLEAN
IsMemTypePresentSystemEnum (
  IN MEM_TYPE MemType
  );

/**

  Gets the printable memory type string for the given memory type.

  @param[in] MemType  Memory type element

  @retval Memory type string

**/
CONST
CHAR8*
EFIAPI
GetMemTypeString (
  IN MEM_TYPE  MemType
  );
#endif // _MEMTYPELIB_H_
