/** @file

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

#include "Include/Pipe.h"

/**
 * For RcSim, send a single byte from a slave to master indicating that the
 * slave is ready to transfer data. The slave thread will sleep in between
 * attempts to send the data in order to avoid consuming a large amount of CPU
 * resources.
 *
 * This function does nothing on non-RcSim builds.
 *
 * @param[in]  Host              A pointer to the Host data.
 * @param[in]  Socket            The socket
 *
 * @retval None
 *
 **/
VOID
SendReadyToTransferData (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{

  return;

} // SendReadyToTransferData

/**
 * For RcSim, recieve a single byte from a slave to master indicating that the
 * slave is ready to transfer data.
 *
 * This function does nothing on non-RcSim builds
 *
 * @param[in]  Host              A pointer to the Host data.
 * @param[in]  Socket            The socket.
 *
 * @retval None
 *
 **/
VOID
GetReadyToTransferData (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{

  return;

} // GetReadyToTransferData

/**
 * Get socket's imc mem variable segment of the SYSHOST structure from the specified slave processor
 *
 * @param Host      - Pointer to the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
GetMemVarData (
  PSYSHOST Host,
  UINT8    socketId
  )
{

  GetPipePackage (socketId, (UINT8*)&Host->var.mem.socket[socketId], sizeof(struct Socket));

} // GetMemVarData

/**
 * Get socket's processorCommon variable segment of the SYSHOST structure from the specified slave processor
 *
 * @param[in] Host      - Pointer to sysHost, the system Host (root) structure
 * @param[in] SocketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
GetProcessorCommonVarData (
  IN PSYSHOST Host,
  IN UINT8 SocketId
  )
{

  GetPipePackage (SocketId, (UINT8*)&Host->var.common.procCom[SocketId], sizeof(struct processorCommon));

}

/**
 * Get socket's imc nvram segment of the SYSHOST structure from the specified slave processor
 *
 * @param Host      - Pointer to the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
GetMemNvramCommonData (
  PSYSHOST Host,
  UINT8    socketId
  )
{

  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.socket[socketId], sizeof(struct socketNvram));

} // GetMemNvramCommonData

/**
 * Get socket's imc nvram segment(not covered by socketNvram) of the SYSHOST structure from the specified slave processor
 *
 * @param Host      - Pointer to the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
GetMemNvramData (
                PSYSHOST Host,
                UINT8    socketId
                )
{

  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.RASmode, sizeof(UINT8));
  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.RASmodeEx, sizeof(UINT8));
  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.ratioIndex, sizeof(UINT8));
  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.eccEn, sizeof(UINT8));
  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.dimmTypePresent, sizeof(UINT8));
  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.DcpmmPresent, sizeof(UINT8));


  if (PcdGetBool (PcdMrcXmpSupport)) {
    GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.XMPProfilesSup, sizeof(UINT8));
  }

  GetPipePackage (socketId, (UINT8*)&Host->nvram.mem.Crc16, sizeof(UINT16));

} // GetMemNvramData

/**
 * Send BSP's socket imc mem variable segment of the SYSHOST structure to the specified slave processor
 *
 * @param Host      - Pointer to the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
SendMemVarData (
  PSYSHOST Host,
  UINT8    socketId
  )
{

  SendPipePackage (Host, socketId, (UINT8*)&Host->var.mem.socket[socketId], sizeof (struct Socket), RCSIM_NO_DELAY);

} // SendMemVarData


/**
 * Send socket's processorCommon variable segment of the SYSHOST structure to the specified processor
 *
 * @param[in] Host      - Pointer to sysHost, the system Host (root) structure
 * @param[in] SocketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
SendProcessorCommonVarData (
  IN PSYSHOST Host,
  IN UINT8 SocketId
  )
{

  SendPipePackage (Host, SocketId, (UINT8*)&Host->var.common.procCom[SocketId], sizeof (struct processorCommon), RCSIM_NO_DELAY);

}

/**
 * Send BSP socket imc nvram segment of the SYSHOST structure to the specified slave processor
 *
 * @param Host      - Pointer to the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
SendMemNvramCommonData (
  PSYSHOST Host,
  UINT8    socketId
  )
{

  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.socket[socketId], sizeof(struct socketNvram), RCSIM_NO_DELAY);

} // SendMemNvramCommonData

/**
 * Send BSP socket imc nvram (not covered by socketNvram) segment of the SYSHOST structure to the specified slave processor
 *
 * @param Host      - Pointer to the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
SendMemNvramData (
  PSYSHOST Host,
  UINT8    socketId
  )
{

  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.RASmode, sizeof(UINT8), RCSIM_NO_DELAY);
  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.RASmodeEx, sizeof(UINT8), RCSIM_NO_DELAY);
  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.ratioIndex, sizeof(UINT8), RCSIM_NO_DELAY);
  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.eccEn, sizeof(UINT8), RCSIM_NO_DELAY);
  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.dimmTypePresent, sizeof(UINT8), RCSIM_NO_DELAY);
  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.DcpmmPresent, sizeof(UINT8), RCSIM_NO_DELAY);

  if (PcdGetBool (PcdMrcXmpSupport)) {
    SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.XMPProfilesSup, sizeof(UINT8), RCSIM_NO_DELAY);
  }

  SendPipePackage (Host, socketId, (UINT8*)&Host->nvram.mem.Crc16, sizeof(UINT16), RCSIM_NO_DELAY);

} // SendMemNvramData
