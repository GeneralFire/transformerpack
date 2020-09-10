/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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


#include <Library/SysHostPointerLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/MemoryServicesLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5RcwRegs.h>
#if MAX_SOCKET <= 4
#include "MemDbCwCache.h"
#endif // MAX_SOCKET <= 4



/**

  Send MRW Command to DB and update the DB CW cache

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
DbControlWordWrite (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  UINT32            Status;
  UINT8             MaxChDdr;
#if MAX_SOCKET <= 4
  UINT8             SubCh;
  UINT8             CachedSelectId;
  UINT8             CachedPage;
  UINT8             SubChMSVx8 = BUFFERS_PER_SUBCH;
  UINT8             Strobe;
  UINT8             StartStrobe;
  UINT8             EndStrobe;
#endif // MAX_SOCKET <= 4

  MaxChDdr    = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Rank   >= MAX_RANK_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "WARNING - DbControlWordWrite called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_205);
    return MRC_STATUS_FAILURE;
  }

  Host        = GetSysHostPointer ();
  Status      = MRC_STATUS_FAILURE;

    //
    // 0x80  <=  DB Direct Cws <  0xE0
    // 0xE0  <=  DB Paged Cws  <= 0xFF
    //
   if (Address < DB_DIRECT_CONTROL_WORD_INITIAL_OFFSET) {
    //
    // Address is out of DB CW range
    //
    return MRC_STATUS_GROUP_NOT_SUPPORTED;

  }

  if (SendMrw (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, CW_RCD_DB, Data) == SUCCESS) {

#if MAX_SOCKET <= 4
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      if ((SubChMask & (1 << SubCh)) == 0) {
        continue;
      }

      //
      // Get PBA Buffer select ID from RW93 to choose apprpriate Data Buffer
      // Get RW93 from Strobe 0 because all strobe share same value for RW93
      //
      CachedSelectId = ALL_DATABUFFERS;

      ReadDbCwCache (Socket, Ch, SubCh, Dimm, 0, LRDIMM_DB_PAGE0, DDR5_DATA_BUFFER_RW93_REG, &CachedSelectId);


      if (CachedSelectId == ALL_DATABUFFERS || Address == DDR5_DATA_BUFFER_RW93_REG) {
        //
        // Mrw to Rw93 is always excuted for all data buffers, ALL_DATABUFFERS ID will make Mrw to all Data buffers
        //
        StartStrobe = 0;
        EndStrobe   = SubChMSVx8;

      } else {
        //
        // Program for Single Strobe
        // Map Pba select id to buffer nummber in cache
        // SUB_CHA: pba select id 0~4
        // SUB_CHB: pba select id 5~9
        //
        Strobe      = (CachedSelectId & PBA_SELECT_ID_BITS) % BUFFERS_PER_SUBCH;
        StartStrobe = CachedSelectId;
        EndStrobe   = CachedSelectId +1;
      }


        for (Strobe = StartStrobe; Strobe < EndStrobe; Strobe++) {

          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }

          CachedPage = LRDIMM_DB_PAGE0;

          if (Address >= DB_PAGED_CONTROL_WORD_INITIAL_OFFSET) {
            //
            // Get Cache Page number form RDIMM_RWDF
            // RDIMM_RWDF is Direct Cw, don't care the Page parameter:LRDIMM_DB_PAGE0 passed in to ReadDbCwCache
            //
            ReadDbCwCache (Socket, Ch, SubCh, Dimm, Strobe, LRDIMM_DB_PAGE0, RDIMM_RWDF, &CachedPage);
          }

          WriteDbCwCache (Socket, Ch, SubCh, Dimm, Strobe, CachedPage, Address, Data);

        } // Strobe

    } // SubCh
#endif // MAX_SOCKET <= 4

    Status = MRC_STATUS_SUCCESS;

  }

  return Status;
}