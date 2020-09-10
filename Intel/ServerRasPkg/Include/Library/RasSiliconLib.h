/** @file
  Interface of RAS silicon library.

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

#ifndef __RAS_SILICON_LIB_H__
#define __RAS_SILICON_LIB_H__

#include <Uefi/UefiBaseType.h>

/**
  Check current patrol scrub engine if it is System Address(SA) mode.

  @param Skt  -- socket index
  @param Mc   -- memory controller in socket
  @param ChOnSkt  --Channel on socket

  @return BOOLEAN
**/
VOID
EFIAPI
SetPcuSpare2Gv (
  UINT8      Skt,
  BOOLEAN    EnableGv
  );

/**
  check if iMC has SMI error interrupt

  @param  Skt -- socket index

  @return  TRUE -- error happens

**/
BOOLEAN
EFIAPI
IsUboxImcError (
  IN  UINT8   Skt
  );

/**
  check if M2MEM has SMI error interrupt

  @param  Skt -- socket index

  @return  TRUE -- error happens

**/
BOOLEAN
EFIAPI
IsUboxM2MemError (
  IN  UINT8   Skt
  );

/**
  check if Smbus has SMI error interrupt

  @param  Skt -- socket index

  @return  TRUE -- error happens

**/
BOOLEAN
EFIAPI
IsUboxSmbusError (
  IN  UINT8   Skt
  );

/**
  clear umc in UBOX

  @param  Socket -- socket index

  @return none

**/
VOID
EFIAPI
ClearUboxUmcError (
  IN   UINT8   Socket
  );


/**
  Enable SMI for memory domain in UBOX

  @param  socket -- socket index

  @return  none.
**/
VOID
EFIAPI
EnableUboxSmiSrc (
  IN   UINT8   Socket
  );


/**
  Enable Ubox Error ctrol register

  @param  Socket  -- socket index

  @return none
**/
VOID
EFIAPI
EnableUboxErrCtrl (
  IN   UINT8   Socket
  );

/**
  clear imc controller interrupt status in UBOX

  @param  Socket  -- socket index

  @return  none
**/
VOID
EFIAPI
ClearImcInterruptInUbox (
  IN   UINT8     Socket
  );

/**
  This function clears sticky error log register.

**/
VOID
EFIAPI
ClearStickyErrLogReg (
  VOID
  );

/**
  Enable Viral in  UBOX.

  @param[in] Skt      - The socket to enable

  @retval None

**/
VOID
EnableUBoxViralPerSkt(
  IN        UINT8   Skt
  );

/**
  check viral status in UBOX

  @param  Socket -- socket index

  @return none.
**/
BOOLEAN
EFIAPI
IsUboxViral (
  IN  UINT8   Socket
  );

/**
  enable/disable KTI error log

  @param Socket  -- socket index
  @param KtiLink -- Kti link index
  @param EnableFlag  -- Enable/disable flag

  @return none.
**/
VOID
EFIAPI
EnableDisableKtiElog (
  IN  UINT8    Socket,
  IN  UINT8    KtiLink,
  IN  BOOLEAN  EnableFlag
  );

/**
  get KTI port viral status

  @param Socket -- socket index
  @param LinkIndex -- kti port

  @retval none.
**/
BOOLEAN
EFIAPI
IsKtiViralState (
  IN  UINT8   Socket,
  IN  UINT8   LinkIndex
  );

/**
  Clear KTI viral status per link

  @param  Skt  -- socket index
  @param  LinkIndex  -- KTI link port

  @return none
**/
VOID
EFIAPI
ClearKtiViralStatusPerLink (
  IN  UINT8    Skt,
  IN  UINT8    LinkIndex
  );

/**
  enable KTI viral per link

  @param  Skt  -- socket index
  @param  LinkIndex  -- KTI link port
  @param  ClearFlag -- clear flag

  @return none
**/
VOID
EFIAPI
EnableKtiViralPerLink (
  IN  UINT8    Skt,
  IN  UINT8    LinkIndex,
  IN  BOOLEAN  ClearFlag
  );

/**
  Enable KTI common mesh stop viral

  @param  Socket -- socket index
  @param  LinkIndex -- Link index

  @return none.
**/
VOID
EFIAPI
EnableM3KTIViral (
  IN   UINT8     Socket,
  IN   UINT8     LinkIndex
  );

/**
  enable M2PCI common mesh stop viral

  @param  skt  -- socket index
  @param  Ctr  -- M2PCI ip index

  @return  none
**/
VOID
EFIAPI
EnableM2PciViral (
  IN   UINT8   Skt,
  IN   UINT8   Ctr
  );

/**
  MS2ISOF Ingress error configure

  @param  skt  -- socket index

  @return  none
**/
VOID
EFIAPI
Ms2IosfIngErrConfig (
  IN   UINT8   Skt
  );

/**
  Get LLC slice bit map

  @param Socket -- socket index

  @return LLC slice bit map.
**/
UINT64
EFIAPI
GetLlcSliceBitMap (
  IN   UINT8   Socket
  );

/**
  Get the bit map of enabled LLC IA Core from the Capability Register.

  @param[in] SocketId     Socket index.

  @retval Bit map of enabled LLC IA Core.
**/
UINT64
EFIAPI
GetLlcIaCoreEnBitMap (
  IN UINT8   SocketId
  );

/**
  set LLC error injection.

  @param Socket  -- socket id
  @param Cha  --  Cha index

  @return none
**/
VOID
EFIAPI
SetLlcErrorinjection (
  IN  UINT8    Socket,
  IN  UINT8    Cha,
  IN  UINT64   PhyAddr
  );


/**

  This function will release the semaphore after the recovery process is done.

  @param [in]  SktId     SktId to release semaphore.

  @retval  None

**/
VOID
EFIAPI
ReleaseSemaphoreSmbus (
  IN  UINT8    SktId
  );

/**
  aquire SMBUS controller ownership

  @param  skt -- socket index

  @return none

**/

VOID
EFIAPI
AquireSemaphore (
  IN  UINT8   Skt
  );

/**
  check if aquired SMBUS controller ownership.

  @param  skt -- socket index

  @return TRUE -- ownership aquired;

**/
BOOLEAN
EFIAPI
IsAquireSemaphored (
  IN  UINT8   Skt
  );


/**
  Override SMBUS controller ownership

  @param  skt -- socket index

  @return none

**/
VOID
EFIAPI
OverrideSemaphore (
  IN  UINT8   Skt
  );

/**
 send wa mailbox to DDR4 mirror memory controller

 @param Socket -- socket index
 @param Mc  -- Memory controller index
 @param ChBitmap -- channel bit map

 @return mailbox status
**/
UINT32
EFIAPI
Ddr4FailoverMailbox (
  IN   UINT8        Socket,
  IN   UINT8        Mc,
  IN   UINT8        ChBitmap
  );

/**
 send wa mailbox to DDRT mirror memory controller

 @param Socket -- socket index
 @param Mc  -- Memory controller index
 @param ChBitmap -- channel bit map

 @return mailbox status
**/
UINT32
EFIAPI
DdrtFailoverMailbox (
  IN   UINT8        Socket,
  IN   UINT8        Mc,
  IN   UINT8        ChBitmap
  );

/**
 send VLS sparing mailbox command for callbackusage

 @param Socket -- socket index
 @param MailboxData -- mailbox data

 @return mailbox status
**/
UINT32
EFIAPI
SendVlsSparing4Callback (
  IN   UINT8    Socket,
  IN   UINT32   MailboxData
  );


/**
  get VLS sparing mailbox command.

 @param Mc -- Node index
 @param ChOnMc  -- channel on memory controller

 @return VLS sparing mailbox command
**/
UINT32
EFIAPI
GetRasQuiesceUnquiesceCmd (
  IN  UINT8    NodeId,
  IN  UINT8    ChOnMc
  );

/**
  get VLS sparing mailbox command data.

 @param Mc -- Node index
 @param ChOnMc  -- channel on memory controller
 @param IsPlusOneSparing  -- Is Sparing mode SDDC Plus One

 @return VLS sparing mailbox command data
**/
UINT32
EFIAPI
GetVlsSparingData (
  IN  UINT8    NodeId,
  IN  UINT8    ChOnMc,
  IN  BOOLEAN  IsPlusOneSparing
  );

/**
  Set scrub interval in PCU via mailbox command.

  @param[in]  Socket          Socket number.
  @param[in]  Mc              MC number on socket.
  @param[in]  ScrubInterval   Value of the scrub interval.

  @retval EFI_SUCCESS         Function execution successful.
  @retval EFI_DEVICE_ERROR    Error in command execution.
**/
EFI_STATUS
EFIAPI
SetScrubIntervalMailbox (
  IN UINT8  Socket,
  IN UINT8  Mc,
  IN UINT32 ScrubInterval
  );

/**
  Set UBOX CSMI threshold

  @param[in]  CsmiThreshold    UBOX CSMI threshold setting
  @param[in]  CloakingEn       Indicates whether cloaking is enabled or not

**/
VOID
EFIAPI
SetUboxCsmiThreshold (
  IN UINT16    CsmiThreshold,
  IN BOOLEAN   CloakingEn
  );

/**
  Set KTI CSMI threshold

  @param[in]  CsmiThreshold    KTI CSMI threshold setting
  @param[in]  CloakingEn       Indicates whether cloaking is enabled or not

**/
VOID
EFIAPI
SetKtiCsmiThreshold (
  IN UINT16    CsmiThreshold,
  IN BOOLEAN   CloakingEn
  );

/**
  Enable Posion for CXL.

  @param[in] Skt      - The socket to enable

  @retval None

**/
VOID
EFIAPI
EnableCxlPosionPerSkt (
  IN        UINT8   Skt
  );

/**
  This function configurate Ubox Error Mask.

  @param[in] UboxErrorMask  Ubox Error Mask in Ubox.

**/
VOID
ConfigUboxErrorMask (
  IN UINT8 UboxErrorMask
  );

/**

  Read CR Proxy register via B2P MailBox

  @param Socket          - Socket Id
  @param ChOnSkt         - Channel number
  @param RegisterIndex   - CR Register Index

  @retval                - Value read from Proxy Register

**/
UINT32
ReadCRProxyRegister (
  IN UINT8     Socket,
  IN UINT8     ChOnSkt,
  IN UINT8     RegisterIndex
  );

/**

  Write to CR Proxy register via B2P MailBox

  @param Socket          - Socket Id
  @param ChOnSkt         - Channel number
  @param RegisterIndex   - CR Register Index
  @param Data            - Data to write

  @retval               - None

**/
VOID
WriteCRProxyRegister (
  IN UINT8     Socket,
  IN UINT8     ChOnSkt,
  IN UINT8     RegisterIndex,
  IN UINT32    Data
  );

#endif  // #ifndef __RAS_SILICON_LIB_H__
