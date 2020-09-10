/** @file
  Implementation of SemaphoreLib functions common to PEI and DXE/SMM for 10nm.

  This implementation for 10nm allows a calling agent to acquire a global (the
  SBSP) semaphore or a socket's semaphore.  It also provides functionality to
  release the semaphore and check if ownership has been obtained.  If a semaphore
  is desired, an agent should first attempt to acquire it, then check if it has
  ownership.  If ownership has not been obtained, the agent must wait until
  ownership has been obtained before proceeding.  Once the desired task is complete
  the semaphore must be released.  Semaphores should be used for when ensuring
  exclusive access to resoruces among CPU sockets is necessary.

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

#include <Uefi/UefiBaseType.h>
#include <RcRegs.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/BaseLib.h>
#include <Library/SemaphoreLib.h>       // *this* library
#include <Library/SystemInfoLib.h>
#include <Library/UsraCsrLib.h>
#include "HardwareSemaphoreLib10nm.h"

/**
  Get the socket requester ID.

  This is a 1-based ID produced by adding 1 to the socket's ID, so it
  can be compared to an owner ID where 0 means no owner.

  @return Requester ID.
**/
UINT8
GetSocketRequesterId (
  VOID
  )
{
  //
  // Zero indicates no ownership, so add one to make this 1-based.
  //
  return GetSysSocketId () + 1;
}

/**
  Get the thread requester ID.

  This is a 1-based ID produced by adding 1 to the thread's logical ID, so it
  can be compared to an owner ID where 0 means no owner.

  @return Requester ID.
**/
UINT8
GetThreadRequesterId (
  VOID
  )
{
  MSR_THD_LOGICAL_ID_REGISTER MsrThdLogicalId;

  MsrThdLogicalId.Uint32 = AsmReadMsr32 (MSR_THD_LOGICAL_ID);

  //
  // Zero indicates no ownership, so add one to make this 1-based.
  //
  return (UINT8) (MsrThdLogicalId.Bits.LogicalId + 1);
}

/**
  Set the owner of a semaphore.

  Also set the call depth to exactly 1, meaning it only has to be released
  to actually release using CSRs.

  @param[in]  Tracker           - semaphore tracking data
  @param[in]  Requester         - requester ID (1-based)
**/
VOID
TrackerSetOwnership (
  IN  PRIVATE_SEMAPHORE_TRACKER *Tracker,
  IN  UINT8                     Requester
  )
{
  Tracker->Owner = Requester;
  Tracker->Depth = 1;
}

/**
  Increment a semaphore's call depth.

  If the semaphore is owned by the requester, increment it call depth and
  return success. Otherwise, return an error. The requester is either a socket
  or a thread, depending on the scope of the semaphore. The Owned output
  parameter is not modified if the return value is an error.

  @param[in]  Tracker           - semaphore tracking data
  @param[in]  Requester         - requester ID (1-based)
  @param[out] Owned             - TRUE if already owned, FALSE if not

  @retval EFI_SUCCESS           - semaphore's call depth was incremented
  @retval EFI_DEVICE_ERROR      - call depth counter rolled over to zero
**/
EFI_STATUS
TrackerIncrementDepth (
  IN  PRIVATE_SEMAPHORE_TRACKER *Tracker,
  IN  UINT8                     Requester,
  OUT BOOLEAN                   *Owned
  )
{
  //
  // Check if requesting thread already owns the target semaphore.
  //
  if (Tracker->Owner == Requester) {
    //
    // Increment call depth.
    //
    Tracker->Depth++;
    //
    // If the value rolls over, log an error.
    //
    if (Tracker->Depth == 0) {
      SemaphoreDepthError ();
      return EFI_DEVICE_ERROR;
    }

    *Owned = TRUE;
  } else {
    *Owned = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  Decrement a semaphore's call depth.

  If the semaphore is owned by the requester, decrement it call depth and
  return success. Otherwise, return an error. The requester is either a socket
  or a thread, depending on the scope of the semaphore. The Owned output
  parameter is not modified if the return value is an error.

  @param[in]  Tracker           - semaphore tracking data
  @param[in]  Requester         - requester ID (1-based)
  @param[out] Owned             - TRUE if still owned, FALSE if not

  @retval EFI_SUCCESS           - semaphore's call depth was decremented
  @retval EFI_DEVICE_ERROR      - requester didn't own the semaphore
**/
EFI_STATUS
TrackerDecrementDepth (
  IN  PRIVATE_SEMAPHORE_TRACKER *Tracker,
  IN  UINT8                     Requester,
  OUT BOOLEAN                   *Owned
  )
{
  //
  // Make sure the requester actually owns the semaphore.
  //
  if (Tracker->Owner != Requester || Tracker->Depth == 0) {
    SemaphoreOwnerError ();
    return EFI_DEVICE_ERROR;
  }

  Tracker->Depth--;

  //
  // Only do the actual release when call depth reaches 0.
  //
  if (Tracker->Depth == 0) {
    Tracker->Owner = 0; // no one owns it now
    *Owned = FALSE;
  } else {
    *Owned = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  Acquire a global (BSP) semaphore for the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireGlobalSemaphore (SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsGlobalSemaphoreOwned (SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseGlobalSemaphore (SystemSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  AddToQueue        - specifices whether to add the requesting
                                  socket to the queue (TRUE or FALSE)
  @param[out] QueueNumber       - assigned place in line of semaphore request,
                                  if adding to queue

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
AcquireGlobalSemaphore (
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  BOOLEAN                   AddToQueue,
  OUT UINT32                    *QueueNumber      OPTIONAL
  )
{
  UINT8  BspSocket;

  //
  // Get BSP socket number.
  //
  BspSocket = GetSysSbspSocketIdNv ();

  return AcquireSocketSemaphore (BspSocket, SemaphoreNumber, AddToQueue, QueueNumber);
}

/**
  Checks if a global (BSP) semaphore is owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireGlobalSemaphore (SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsGlobalSemaphoreOwned (SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseGlobalSemaphore (SystemSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  QueueNumber       - assigned place in line of semaphore request
                                  that was returned by AcquireGlobalSemaphore

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
IsGlobalSemaphoreOwned (
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  UINT32                    QueueNumber
  )
{
  UINT8  BspSocket;

  //
  // Get BSP socket number.
  //
  BspSocket = GetSysSbspSocketIdNv ();

  return IsSocketSemaphoreOwned (BspSocket, SemaphoreNumber, QueueNumber);
}

/**
  Release a global (BSP) semaphore owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireGlobalSemaphore (SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsGlobalSemaphoreOwned (SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseGlobalSemaphore (SystemSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS           - successfully released semaphore
  @retval EFI_DEVICE_ERROR      - error releasing semaphore
**/
EFI_STATUS
EFIAPI
ReleaseGlobalSemaphore (
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber
  )
{
  UINT8  BspSocket;

  //
  // Get BSP socket number.
  //
  BspSocket = GetSysSbspSocketIdNv ();

  return ReleaseSocketSemaphore (BspSocket, SemaphoreNumber);
}

/**
  Acquire a socket semaphore for the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseSocketSemaphore (Socket, SystemSemaphore0);
      ......

  @param[in]  Socket            - Socket where the semaphore is located
  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  AddToQueue        - specifices whether to add the requesting
                                  socket to the queue (TRUE or FALSE)
  @param[out] QueueNumber       - assigned place in line of semaphore request,
                                  if adding to queue

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
AcquireSocketSemaphore (
  IN  UINT8                     Socket,
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  BOOLEAN                   AddToQueue,
  OUT UINT32                    *QueueNumber      OPTIONAL
  )
{
  EFI_STATUS                        Status;
  UINT32                            SemAcquireRegAddr;
  SYSTEMAQUSEMP_0_UBOX_MISC_STRUCT  SemAcquireReg;
  PRIVATE_SEMAPHORE_DATA            *Data;
  UINT8                             Requester;
  BOOLEAN                           Owned;

  if (SemaphoreNumber >= SystemSemaphoreMax) {
    return FALSE;
  }

  Status = GetSemaphoreData (&Data);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Requester = GetSocketRequesterId ();

  //
  // Attempt to increment the call depth if the requester already owns it.
  //
  Status = TrackerIncrementDepth (
      &Data->System[Socket][SemaphoreNumber],
      Requester,
      &Owned
      );
  if (EFI_ERROR (Status)) {
    return FALSE;
  } else if (Owned) {
    return TRUE;        // already owned (depth incremented)
  }

  //
  // Normal acquire registers put the requester into the queue.
  // Impatient ("IMP") acquire registers do not.
  //
  if (AddToQueue) {
    if (SemaphoreNumber == SystemSemaphore0) {
      SemAcquireRegAddr = SYSTEMAQUSEMP_0_UBOX_MISC_REG;
    } else {
      SemAcquireRegAddr = SYSTEMAQUSEMP_1_UBOX_MISC_REG;
    }
  } else {
    if (SemaphoreNumber == SystemSemaphore0) {
      SemAcquireRegAddr = SYSTEMIMPAQUSEMP_0_UBOX_MISC_REG;
    } else {
      SemAcquireRegAddr = SYSTEMIMPAQUSEMP_1_UBOX_MISC_REG;
    }
  }

  //
  // Read the CSR. The value returned tells us whether we acquired the
  // semaphore as a result of this read.
  //
  SemAcquireReg.Data = UsraCsrRead (Socket, 0, SemAcquireRegAddr);
  if (QueueNumber != NULL) {
    *QueueNumber = SemAcquireReg.Bits.sempcurrtail;
  }
  if (SemAcquireReg.Bits.sempowned == 1) {
    TrackerSetOwnership (
      &Data->System[Socket][SemaphoreNumber],
      Requester
      );
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if a socket semaphore is owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseSocketSemaphore (Socket, SystemSemaphore0);
      ......

  @param[in]  Socket            - Socket where the semaphore is located
  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  QueueNumber       - assigned place in line of semaphore request
                                  that was returned by AcquireSocketSemaphore

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
IsSocketSemaphoreOwned (
  IN  UINT8                     Socket,
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  UINT32                    QueueNumber
  )
{
  EFI_STATUS                        Status;
  UINT32                            SemHeadRegAddr;
  SYSTEMHEADSEMP_0_UBOX_MISC_STRUCT SemHeadReg;
  PRIVATE_SEMAPHORE_DATA            *Data;
  UINT8                             Requester;

  if (SemaphoreNumber >= SystemSemaphoreMax) {
    return FALSE;
  }

  Status = GetSemaphoreData (&Data);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Requester = GetSocketRequesterId ();

  if (SemaphoreNumber == SystemSemaphore0) {
    SemHeadRegAddr = SYSTEMHEADSEMP_0_UBOX_MISC_REG;
  } else {
    SemHeadRegAddr = SYSTEMHEADSEMP_1_UBOX_MISC_REG;
  }

  SemHeadReg.Data = UsraCsrRead (Socket, 0, SemHeadRegAddr);

  //
  // If semaphore acquired, update semaphore owner and call depth.
  //
  if (SemHeadReg.Bits.sempcurrhead == QueueNumber) {
    TrackerSetOwnership (
      &Data->System[Socket][SemaphoreNumber],
      Requester
      );
    return TRUE;
  }

  return FALSE;
}

/**
  Release a socket semaphore owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseSocketSemaphore (Socket, SystemSemaphore0);
      ......

  @param[in]  Socket            - Socket to release semaphore
  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS           - successfully released semaphore
  @retval EFI_DEVICE_ERROR      - error releasing semaphore
**/
EFI_STATUS
EFIAPI
ReleaseSocketSemaphore (
  IN  UINT8                     Socket,
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber
  )
{
  EFI_STATUS                            Status;
  UINT32                                SemHeadRegAddr;
  UINT32                                SemReleaseRegAddr;
  SYSTEMHEADSEMP_0_UBOX_MISC_STRUCT     SemHeadReg;
  SYSTEMRELEASESEMP_0_UBOX_MISC_STRUCT  SemReleaseReg;
  PRIVATE_SEMAPHORE_DATA                *Data;
  UINT8                                 Requester;
  BOOLEAN                               Owned;

  if (SemaphoreNumber >= SystemSemaphoreMax) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetSemaphoreData (&Data);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Requester = GetSocketRequesterId ();

  Status = TrackerDecrementDepth (
    &Data->System[Socket][SemaphoreNumber],
    Requester,
    &Owned
    );
  if (EFI_ERROR (Status)) {
    return Status;
  } else if (Owned) {
    return EFI_SUCCESS; // still owned (depth not zero)
  }

  if (SemaphoreNumber == SystemSemaphore0) {
    SemHeadRegAddr = SYSTEMHEADSEMP_0_UBOX_MISC_REG;
    SemReleaseRegAddr = SYSTEMRELEASESEMP_0_UBOX_MISC_REG;
  } else {
    SemHeadRegAddr = SYSTEMHEADSEMP_1_UBOX_MISC_REG;
    SemReleaseRegAddr = SYSTEMRELEASESEMP_1_UBOX_MISC_REG;
  }

  //
  // To release, write current head to the release CSR.
  //
  SemHeadReg.Data = UsraCsrRead (Socket, 0, SemHeadRegAddr);

  SemReleaseReg.Data = 0;
  SemReleaseReg.Bits.sempreleasevalue = SemHeadReg.Bits.sempcurrhead;

  UsraCsrWrite (Socket, 0, SemReleaseRegAddr, SemReleaseReg.Data);

  return EFI_SUCCESS;
}

/**
  Acquire a local semaphore for the calling thread.

  Used for ensuring exclusive access to resources among CPU threads.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireLocalSemaphore (LocalSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsLocalSemaphoreOwned (LocalSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseLocalSemaphore (LocalSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - LOCALSEMAPHORE register number (0 or 1)
  @param[in]  AddToQueue        - specifices whether to add the requesting
                                  thread to the queue (TRUE or FALSE)
  @param[out] QueueNumber       - assigned place in line of semaphore request,
                                  if adding to queue

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
AcquireLocalSemaphore (
  IN  LOCAL_SEMAPHORE_NUMBER    SemaphoreNumber,
  IN  BOOLEAN                   AddToQueue,
  OUT UINT32                    *QueueNumber      OPTIONAL
  )
{
  if (FeaturePcdGet (PcdHardwareLocalSemaphores)) {
    EFI_STATUS                        Status;
    UINT32                            SemAcquireRegAddr;
    LOCALAQUSEMP_0_UBOX_MISC_STRUCT   SemAcquireReg;
    PRIVATE_SEMAPHORE_DATA            *Data;
    UINT8                             Socket;
    UINT8                             Requester;
    BOOLEAN                           Owned;

    if (SemaphoreNumber >= LocalSemaphoreMax) {
      return FALSE;
    }

    Status = GetSemaphoreData (&Data);
    if (EFI_ERROR (Status)) {
      return FALSE;
    }

    Socket = GetSysSocketId ();
    Requester = GetThreadRequesterId ();

    //
    // Attempt to increment the call depth if the requester already owns it.
    //
    Status = TrackerIncrementDepth (
        &Data->Local[Socket][SemaphoreNumber],
        Requester,
        &Owned
        );
    if (EFI_ERROR (Status)) {
      return FALSE;
    } else if (Owned) {
      return TRUE;        // already owned (depth incremented)
    }

    //
    // Normal acquire registers put the requester into the queue.
    // Impatient ("IMP") acquire registers do not.
    //
    if (AddToQueue) {
      if (SemaphoreNumber == LocalSemaphore0) {
        SemAcquireRegAddr = LOCALAQUSEMP_0_UBOX_MISC_REG;
      } else {
        SemAcquireRegAddr = LOCALAQUSEMP_1_UBOX_MISC_REG;
      }
    } else {
      if (SemaphoreNumber == LocalSemaphore0) {
        SemAcquireRegAddr = LOCALIMPAQUSEMP_0_UBOX_MISC_REG;
      } else {
        SemAcquireRegAddr = LOCALIMPAQUSEMP_1_UBOX_MISC_REG;
      }
    }

    //
    // Read the CSR. The value returned tells us whether we acquired the
    // semaphore as a result of this read.
    //
    SemAcquireReg.Data = UsraCsrRead (Socket, 0, SemAcquireRegAddr);
    if (QueueNumber != NULL) {
      *QueueNumber = SemAcquireReg.Bits.sempcurrtail;
    }
    if (SemAcquireReg.Bits.sempowned == 1) {
      TrackerSetOwnership (
        &Data->Local[Socket][SemaphoreNumber],
        Requester
        );
      return TRUE;
    }

    return FALSE;
  } else {
    //
    // Local semaphore support is disabled.
    // Return in a way that will not block anyone.
    //
    return TRUE;
  }
}

/**
  Checks if a local semaphore is owned by the calling thread.

  Used for ensuring exclusive access to resources among CPU threads.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireLocalSemaphore (LocalSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsLocalSemaphoreOwned (LocalSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseLocalSemaphore (LocalSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - LOCALSEMAPHORE register number (0 or 1)
  @param[in]  QueueNumber       - assigned place in line of semaphore request
                                  that was returned by AcquireLocalSemaphore

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
IsLocalSemaphoreOwned (
  IN  LOCAL_SEMAPHORE_NUMBER    SemaphoreNumber,
  IN  UINT32                    QueueNumber
  )
{
  if (FeaturePcdGet (PcdHardwareLocalSemaphores)) {
    EFI_STATUS                        Status;
    UINT32                            SemHeadRegAddr;
    SYSTEMHEADSEMP_0_UBOX_MISC_STRUCT SemHeadReg;
    PRIVATE_SEMAPHORE_DATA            *Data;
    UINT8                             Socket;
    UINT8                             Requester;

    if (SemaphoreNumber >= LocalSemaphoreMax) {
      return FALSE;
    }

    Status = GetSemaphoreData (&Data);
    if (EFI_ERROR (Status)) {
      return FALSE;
    }

    Socket = GetSysSocketId ();
    Requester = GetThreadRequesterId ();

    if (SemaphoreNumber == LocalSemaphore0) {
      SemHeadRegAddr = LOCALHEADSEMP_0_UBOX_MISC_REG;
    } else {
      SemHeadRegAddr = LOCALHEADSEMP_1_UBOX_MISC_REG;
    }

    SemHeadReg.Data = UsraCsrRead (Socket, 0, SemHeadRegAddr);

    //
    // If semaphore acquired, update semaphore owner and call depth.
    //
    if (SemHeadReg.Bits.sempcurrhead == QueueNumber) {
      TrackerSetOwnership (
        &Data->Local[Socket][SemaphoreNumber],
        Requester
        );
      return TRUE;
    }

    return FALSE;
  } else {
    //
    // Local semaphore support is disabled.
    // Return in a way that will not block anyone.
    //
    return TRUE;
  }
}

/**
  Release a local semaphore owned by the calling thread.

  Used for ensuring exclusive access to resources among CPU threads.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireLocalSemaphore (LocalSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsLocalSemaphoreOwned (LocalSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseLocalSemaphore (LocalSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - LOCALSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS           - successfully released semaphore
  @retval EFI_INVALID_PARAMETER - semaphore number is out of range
  @retval EFI_DEVICE_ERROR      - error releasing semaphore
**/
EFI_STATUS
EFIAPI
ReleaseLocalSemaphore (
  IN  LOCAL_SEMAPHORE_NUMBER    SemaphoreNumber
  )
{
  if (FeaturePcdGet (PcdHardwareLocalSemaphores)) {
    EFI_STATUS                            Status;
    UINT32                                SemHeadRegAddr;
    UINT32                                SemReleaseRegAddr;
    LOCALHEADSEMP_0_UBOX_MISC_STRUCT      SemHeadReg;
    LOCALRELEASESEMP_0_UBOX_MISC_STRUCT   SemReleaseReg;
    PRIVATE_SEMAPHORE_DATA                *Data;
    UINT8                                 Socket;
    UINT8                                 Requester;
    BOOLEAN                               Owned;

    if (SemaphoreNumber >= LocalSemaphoreMax) {
      return EFI_INVALID_PARAMETER;
    }

    Status = GetSemaphoreData (&Data);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Socket = GetSysSocketId (); // always operates on the local socket
    Requester = GetThreadRequesterId ();

    Status = TrackerDecrementDepth (
      &Data->Local[Socket][SemaphoreNumber],
      Requester,
      &Owned
      );
    if (EFI_ERROR (Status)) {
      return Status;
    } else if (Owned) {
      return EFI_SUCCESS; // still owned (depth not zero)
    }

    if (SemaphoreNumber == LocalSemaphore0) {
      SemHeadRegAddr = LOCALHEADSEMP_0_UBOX_MISC_REG;
      SemReleaseRegAddr = LOCALRELEASESEMP_0_UBOX_MISC_REG;
    } else {
      SemHeadRegAddr = LOCALHEADSEMP_1_UBOX_MISC_REG;
      SemReleaseRegAddr = LOCALRELEASESEMP_1_UBOX_MISC_REG;
    }

    //
    // To release, write current head to the release CSR.
    //
    SemHeadReg.Data = UsraCsrRead (Socket, 0, SemHeadRegAddr);

    SemReleaseReg.Data = 0;
    SemReleaseReg.Bits.sempreleasevalue = SemHeadReg.Bits.sempcurrhead;

    UsraCsrWrite (Socket, 0, SemReleaseRegAddr, SemReleaseReg.Data);

    return EFI_SUCCESS;
  } else {
    //
    // Local semaphore support is disabled.
    // Return in a way that will not block anyone.
    //
    return EFI_SUCCESS;
  }
}
