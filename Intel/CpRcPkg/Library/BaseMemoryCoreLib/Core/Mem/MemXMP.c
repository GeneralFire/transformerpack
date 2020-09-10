/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SysHostPointerLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/SpdAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>

extern  UINT32 preciseTCK[MAX_SUP_FREQ];

//
// Local Prototypes
//
VOID   GetXMPTimingsDDR4 (PSYSHOST Host);
UINT16 GetCycles (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT16 mtbOffset, UINT16 ftbOffset, UINT32 tCK);
UINT16 TimeBaseToDCLKtCK (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT16 mtbTime, UINT16 ftbTime, UINT32 tCK);




UINT32
WaitForConfigureXMP (
  PSYSHOST Host
  )
/*++

  Wait for BSP to send AP continue handshake

  @param Host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8               handShakeByte;

  if ((Host->nvram.mem.DataGood != 1) || (GetSysBootMode () == NormalBoot)) {
    //
    // if MT MRC, slave processor to inform BSP GatherSPDData is complete
    //

    SendPipePackage (Host, GetSysSocketId (), &handShakeByte, 1, RCSIM_NO_DELAY);

    //
    // now wait for the data back from the BSP to indicate AP should continue
    //
    GetPipePackage (GetSysSocketId (), &handShakeByte, 1);

  } // S3 or Warm boot

  return SUCCESS;
} // WaitForConfigureXMP

UINT32
ConfigureXMP (
  PSYSHOST Host
  )
/*++

  Checks if XMP is supported by all sockets

  @param Host  - Pointer to sysHost

  @retval N/A

--*/
{

#ifndef DDR5_SUPPORT

  UINT8   socket;
  UINT8               handShakeByte;
  UINT8   ch;
  UINT8   dimm;
  UINT8   SPDReg;
  UINT16  XMPId;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  SYS_SETUP *Setup;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if ((Host->nvram.mem.DataGood != 1) || (GetSysBootMode () == NormalBoot)) {
    //
    // if MT MRC, wait for slave processors to complete GatherSPDData
    //
    if (Setup->mem.options & MULTI_THREAD_MRC_EN) {
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        //
        // Continue if this socket is not present
        //
        if (!SocketPresent (socket)) {
          continue;
        }

        if (socket == GetSysSocketId ()) {
          continue;
        }

        //RcDebugPrint (SDBG_MAX,
        //                        "Wait for slave GatherSPDData complete (S%d)\n", socket);

        GetPipePackage (socket, &handShakeByte, 1);
      } // socket loop
    }

    // Loop for each CPU socket
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (Host->nvram.mem.socket[socket].enabled == 0 || (Host->nvram.mem.socket[socket].maxDimmPop == 0)) {
        continue;
      }

      Host->nvram.mem.XMPProfilesSup = 3;
      Host->nvram.mem.XMPProfilesRevision = 0xFF;

      channelNvList = GetChannelNvList (Host, socket);

      // Detect DIMMs on each channel
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        // Detect DIMMs in each slot
        for (dimm = 0; dimm < Host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {

          OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CLOCK_INIT, CHECKPOINT_MINOR_XMP_INIT, (UINT16) ((socket << 8) | (ch << 4) |dimm)));

          //
          // Read SPD device to detect DIMM presence and check for XMP support
          //
          if (SpdReadByte (socket, ch, dimm, SPD_XMP_ID_DDR4, &SPDReg) == EFI_SUCCESS) {

            XMPId = (UINT16) (SPDReg << 8);

            SpdReadByte (socket, ch, dimm, SPD_XMP_ID_DDR4 + 1, &SPDReg);
            XMPId |= (UINT16) SPDReg;

            //
            // Clear out the profiles supported if this DIMM does not support XMP
            //

            if (XMPId != XMP_ID) {
              Host->nvram.mem.XMPProfilesSup = 0;
            } else {

              SpdReadByte (socket, ch, dimm, SPD_XMP_REV_DDR4, &(*dimmNvList)[dimm].XMPRev);
              if (Host->nvram.mem.XMPProfilesRevision > (*dimmNvList)[dimm].XMPRev) {
                Host->nvram.mem.XMPProfilesRevision = (*dimmNvList)[dimm].XMPRev;
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                        "XMP Revision: %d.%d\n",
                                        ((*dimmNvList)[dimm].XMPRev >> 4) & 0xFF, (*dimmNvList)[dimm].XMPRev & 0xFF);
              }
              //
              // Get the profiles supported
              //
              SpdReadByte (socket, ch, dimm, SPD_XMP_ORG_DDR4, &(*dimmNvList)[dimm].XMPOrg);

              //
              // AND with the profiles supported by the other DIMMs
              //
              Host->nvram.mem.XMPProfilesSup &= ((*dimmNvList)[dimm].XMPOrg & 3);
            }
          } else {
            Host->nvram.mem.XMPProfilesSup = 0;
          }
        } // dimm loop
      } // ch loop
    } // socket loop

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MINMAX) {
      if (Host->nvram.mem.XMPProfilesSup) {
        RcDebugPrint (SDBG_DEFAULT, "Supported XMP Profile(s):  ");
        if (Host->nvram.mem.XMPProfilesSup & BIT0) {
          RcDebugPrint (SDBG_DEFAULT, "1");
        }

        if (Host->nvram.mem.XMPProfilesSup & BIT1) {
          RcDebugPrint (SDBG_DEFAULT, ", 2");
        }

        RcDebugPrint (SDBG_DEFAULT, "\n");

        RcDebugPrint (SDBG_DEFAULT, "Supported XMP Revision: %d.%d\n", (Host->nvram.mem.XMPProfilesRevision & 0xF0) >> 4, Host->nvram.mem.XMPProfilesRevision & 0x0F );
      }
    }
#endif //DEBUG_CODE_BLOCK
    //
    // Collect the XMP timings if at least one XMP profile is supported
    //
    if (Host->nvram.mem.XMPProfilesSup) {
      GetXMPTimingsDDR4 (Host);
    }

    if (Setup->mem.options & MULTI_THREAD_MRC_EN) {
      //
      // In MT MRC mode, tell slave processors to continue
      //
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        //
        // Continue if this socket is not present
        //
        if (!SocketPresent (socket)) {
          continue;
        }

        if (socket == GetSysSocketId ()) {
          continue;
        }

        SendPipePackage (Host, socket, &handShakeByte, 1, RCSIM_NO_DELAY);
      } // socket loop
    }

  } // S3 or Warm boot

#endif // #ifndef DDR5_SUPPORT

  return SUCCESS;
} // ConfigureXMP

/**

  Check and configure XMP on all sockets

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CheckXMP (
  PSYSHOST  Host
  )
{
  if (!(PcdGetBool (PcdMrcXmpSupport))) {
    return SUCCESS;
  }

  if (GetSysSocketId () == GetSysSbspSocketIdNv ()) {
    return ConfigureXMP (Host);
  } else {
    return WaitForConfigureXMP (Host);
  }
}

/*++

  Gathers XMP timings for DDR4

  @param Host  - Pointer to sysHost

  @retval N/A

--*/
VOID
GetXMPTimingsDDR4 (
  PSYSHOST Host
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               profile;
  UINT8               SPDReg;
  UINT16              cycles;
  UINT8               xmpOffset;
  UINT8               minTCK;
  UINT8               ftbVal;
  UINT16              tempFreq;
  UINT32              casSup;
  UINT16              tRC;
  UINT16              tRAS;
  UINT16              tRFC;
  UINT16              tFAW;
  UINT32              checkSum[MAX_SOCKET][MAX_CH][MAX_DIMM];
  UINT32              tCK;
  INT32               tempTCK;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  //
  // Check for two possible profiles
  //
  for (profile = 0; profile < 2; profile++) {
    //
    // Continue if this profile is not supported
    //
    if ((Host->nvram.mem.XMPProfilesSup & (1 << profile)) == 0) {
      continue;
    }

    //
    // Initialize to large values
    //
    Host->nvram.mem.profileMemTime[profile].ddrFreq = 0xFFFF;
    Host->nvram.mem.profileMemTime[profile].casSup = 0xFFFFFFFF;
    Host->nvram.mem.profileMemTime[profile].tREFI = 0x7FFF;

    //
    // Offset into SPD
    //
    xmpOffset = profile * 47;

    // Loop for each CPU socket
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (Host->nvram.mem.socket[socket].enabled == 0) {
        continue;
      }

      channelNvList = GetChannelNvList (Host, socket);

      //
      // Loop for each channel
      //
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        //
        // Loop for each DIMM
        //
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          checkSum[socket][ch][dimm] = 0;

          if (!profile) {
            //
            // Get TimeBases
            //
            SpdReadByte (socket, ch, dimm, SPD_XMP_TB_PROFILE_1, &SPDReg);
            checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) SPDReg;

            (*dimmNvList)[dimm].SPDftb = 1;      // 1 picosecond
            (*dimmNvList)[dimm].SPDmtb = 125;    //125 picoseconds
          }

          //
          // Get Min TCK
          //
          SpdReadByte (socket, ch, dimm, SPD_XMP_TCK_DDR4 + xmpOffset, &minTCK);
          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) minTCK;

          // Get FTB Min TCK
          SpdReadByte (socket, ch, dimm, SPD_XMP_FTB_TCK_DDR4 + xmpOffset, &ftbVal);
          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) ftbVal;

          if (ftbVal & 0x80) {
            tempTCK = (UINT32) (minTCK * (*dimmNvList)[dimm].SPDmtb) + ((ftbVal - 256) *  (*dimmNvList)[dimm].SPDftb);
          } else {
            tempTCK = (UINT32) (minTCK * (*dimmNvList)[dimm].SPDmtb) + (ftbVal * (*dimmNvList)[dimm].SPDftb);
          }

          //
          // Determine transfer rate
          //
          if (tempTCK <= DDR_4800_TCK_MIN) {
            tempFreq = 4800;
          } else if (tempTCK <= DDR_4400_TCK_MIN) {
            tempFreq = 4400;
          } else if (tempTCK <= DDR_4266_TCK_MIN) {
            tempFreq = 4266;
          } else if (tempTCK <= DDR_4200_TCK_MIN) {
            tempFreq = 4200;
          } else if (tempTCK <= DDR_4000_TCK_MIN) {
            tempFreq = 4000;
          } else if (tempTCK <= DDR_3800_TCK_MIN) {
            tempFreq = 3800;
          } else if (tempTCK <= DDR_3733_TCK_MIN) {
            tempFreq = 3733;
          } else if (tempTCK <= DDR_3600_TCK_MIN) {
            tempFreq = 3600;
          } else if (tempTCK <= DDR_3466_TCK_MIN) {
            tempFreq = 3466;
          } else if (tempTCK <= DDR_3400_TCK_MIN) {
            tempFreq = 3400;
          } else if (tempTCK <= DDR_3200_TCK_MIN) {
            tempFreq = 3200;
          } else if (tempTCK <= DDR_3000_TCK_MIN) {
            tempFreq = 3000;
          } else if (tempTCK <= DDR_2933_TCK_MIN) {
            tempFreq = 2933;
          } else if (tempTCK <= DDR_2800_TCK_MIN) {
            tempFreq = 2800;
          } else if (tempTCK <= DDR_2666_TCK_MIN) {
            tempFreq = 2666;
          } else if (tempTCK <= DDR_2600_TCK_MIN) {
            tempFreq = 2600;
          } else if (tempTCK <= DDR_2400_TCK_MIN) {
            tempFreq = 2400;
          } else if (tempTCK <= DDR_2200_TCK_MIN) {
            tempFreq = 2200;
          } else if (tempTCK <= DDR_2133_TCK_MIN) {
            tempFreq = 2133;
          } else if (tempTCK <= DDR_2000_TCK_MIN) {
            tempFreq = 2000;
          } else if (tempTCK <= DDR_1866_TCK_MIN) {
            tempFreq = 1866;
          } else if (tempTCK <= DDR_1800_TCK_MIN) {
            tempFreq = 1800;
          } else if (tempTCK <= DDR_1600_TCK_MIN) {
            tempFreq = 1600;
          } else if (tempTCK <= DDR_1400_TCK_MIN) {
            tempFreq = 1400;
          } else {
            tempFreq = 1333;
          }

          //
          // Save the frequency supported by all DIMMs
          //
          if (Host->nvram.mem.profileMemTime[profile].ddrFreq > tempFreq) {
            Host->nvram.mem.profileMemTime[profile].ddrFreq = tempFreq;
          }
        } // dimm loop
      } // ch loop
    } // socket loop

    //
    // Determine DCLK Ratio
    //
    if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 8400) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_8400 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 6400) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_6400 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 6000) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_6000 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 5600) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_5600 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 5200) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_5200 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 4800) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_4800 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 4400) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_4400 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 4266) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_4266 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 4200) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_4200 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 4000) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_4000 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 3800) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_3800 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 3733) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_3733 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 3600) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_3600 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 3466) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_3466 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 3400) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_3400 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 3200) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_3200 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 3000) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_3000 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2933) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2933 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2800) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2800 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2666) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2666 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2600) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2600 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2400) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2400 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2200) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2200 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2133) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2133 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 2000) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_2000 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1866) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1866 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1800) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1800 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1600) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1600 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1400) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1400 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1333) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1333 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1200) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1200 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1066) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1066 + 1;
    } else if (Host->nvram.mem.profileMemTime[profile].ddrFreq >= 1000) {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_1000 + 1;
    } else {
      Host->nvram.mem.profileMemTime[profile].ddrFreqLimit = DDR_800 + 1;
    }

    tCK = preciseTCK[Host->nvram.mem.profileMemTime[profile].ddrFreqLimit - 1];

    // Loop for each CPU socket
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (Host->nvram.mem.socket[socket].enabled == 0) {
        continue;
      }

      channelNvList = GetChannelNvList (Host, socket);

      //
      // Check strap addresses on each channel moving near to far
      //
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {

          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          //
          // Get tFAW
          //
          SpdReadByte (socket, ch, dimm, SPD_XMP_TFAW_DDR4 + xmpOffset, &SPDReg);
          tFAW = (UINT16) SPDReg;

          SpdReadByte (socket, ch, dimm, SPD_XMP_UN_TFAW_DDR4 + xmpOffset, &SPDReg);
          tFAW = tFAW | ((UINT16) SPDReg << 8);
          //
          // Get lower byte of tFAW
          //
          cycles = TimeBaseToDCLKtCK (Host, socket, ch, dimm, tFAW, 0, tCK);
          if (Host->nvram.mem.profileMemTime[profile].nFAW < cycles) {
            Host->nvram.mem.profileMemTime[profile].nFAW = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + tFAW;

          //
          // Get tRRD_S
          //
          cycles = GetCycles (Host, socket, ch, dimm, SPD_XMP_TRRDS_DDR4 + xmpOffset, SPD_XMP_FTB_TRRDS + xmpOffset, tCK);
          if (Host->nvram.mem.profileMemTime[profile].nRRD < cycles) {
            Host->nvram.mem.profileMemTime[profile].nRRD = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) SPDReg;

          //
          // Get tRRD_L
          //
          cycles = GetCycles (Host, socket, ch, dimm, SPD_XMP_TRRDL_DDR4 + xmpOffset, SPD_XMP_FTB_TRRDL + xmpOffset, tCK);
          if (Host->nvram.mem.profileMemTime[profile].nRRD_L < cycles) {
            Host->nvram.mem.profileMemTime[profile].nRRD_L = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) SPDReg;

          //
          // Get tRP
          //
          cycles = GetCycles (Host, socket, ch, dimm, SPD_XMP_TRP_DDR4 + xmpOffset, SPD_XMP_FTB_TRP_DDR4 + xmpOffset, tCK);
          if (Host->nvram.mem.profileMemTime[profile].nRP < cycles) {
            Host->nvram.mem.profileMemTime[profile].nRP = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) SPDReg;

          //
          // Get tRC and tRAS
          //
          SpdReadByte (socket, ch, dimm, SPD_XMP_TRC_TRAS_DDR4 + xmpOffset, &SPDReg);
          tRC = (UINT16) (SPDReg & 0xF0) << 4;
          //
          // Get upper nibble of tRC
          //
          tRAS = (UINT16) (SPDReg & 0x0F) << 8;
          //
          // Get upper nibble of tRAS
          //
          SpdReadByte (socket, ch, dimm, SPD_XMP_TRAS_DDR4 + xmpOffset, &SPDReg);
          tRAS = tRAS | (UINT16) SPDReg;
          //
          // Get lower byte of tRAS
          //
          cycles = TimeBaseToDCLKtCK (Host, socket, ch, dimm, tRAS, 0, tCK);
          if (Host->nvram.mem.profileMemTime[profile].nRAS < cycles) {
            Host->nvram.mem.profileMemTime[profile].nRAS = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + tRAS;

          SpdReadByte (socket, ch, dimm, SPD_XMP_TRC_DDR4 + xmpOffset, &SPDReg);
          tRC = tRC | (UINT16) SPDReg;
          //
          // Get lower byte of tRC
          //
          cycles = TimeBaseToDCLKtCK (Host, socket, ch, dimm, tRC, 0, tCK);
          if (Host->nvram.mem.profileMemTime[profile].nRC < cycles) {
            Host->nvram.mem.profileMemTime[profile].nRC = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + tRC;

          //
          // Get tRCD
          //
          cycles = GetCycles (Host, socket, ch, dimm, SPD_XMP_TRCD_DDR4 + xmpOffset, SPD_XMP_FTB_TRCD_DDR4 + xmpOffset, tCK);

          //
          // Odd latencies greater than 16 are not supported
          //
          if ((cycles > 16) && (cycles & BIT0)) {
            cycles++;
          }

          if (Host->nvram.mem.profileMemTime[profile].nRCD < cycles) {
            Host->nvram.mem.profileMemTime[profile].nRCD = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) SPDReg;

          //
          // Get tRFC
          //
          SpdReadByte (socket, ch, dimm, SPD_XMP_TRFC1_LSB + xmpOffset, &SPDReg);
          tRFC = (UINT16) SPDReg;

          SpdReadByte (socket, ch, dimm, SPD_XMP_TRFC1_MSB + xmpOffset, &SPDReg);
          tRFC = tRFC | ((UINT16) SPDReg << 8);
          //
          // Get lower byte of tRFC
          //
          cycles = TimeBaseToDCLKtCK (Host, socket, ch, dimm, tRFC, 0, tCK);
          if (Host->nvram.mem.profileMemTime[profile].nRFC < cycles) {
            Host->nvram.mem.profileMemTime[profile].nRFC = cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + tRFC;

          //
          // Get tAA
          //
          cycles = GetCycles (Host, socket, ch, dimm, SPD_XMP_TAA_DDR4 + xmpOffset, SPD_XMP_FTB_TAA_DDR4 + xmpOffset, tCK);

          if (Host->nvram.mem.profileMemTime[profile].nCL < cycles) {
            Host->nvram.mem.profileMemTime[profile].nCL = (UINT8) cycles;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) SPDReg;

          //
          // Get supported CAS settings
          //
          SpdReadByte (socket, ch, dimm, SPD_XMP_CAS_SUP_4 + xmpOffset, &SPDReg);
          casSup = (UINT32) (SPDReg << 24);

          SpdReadByte (socket, ch, dimm, SPD_XMP_CAS_SUP_3 + xmpOffset, &SPDReg);
          casSup = casSup | (UINT32) (SPDReg << 16);

          SpdReadByte (socket, ch, dimm, SPD_XMP_CAS_SUP_2 + xmpOffset, &SPDReg);
          casSup = (UINT32) (SPDReg << 8);

          SpdReadByte (socket, ch, dimm, SPD_XMP_CAS_SUP_1 + xmpOffset, &SPDReg);
          casSup = casSup | (UINT32) SPDReg;

          Host->nvram.mem.profileMemTime[profile].casSup &= casSup;
          //
          // Mask off the unsupported CLs
          //
          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + casSup;

          //
          // Vdd
          //
          SpdReadByte (socket, ch, dimm, SPD_XMP_VDD_DDR4 + xmpOffset, &SPDReg);
          if (Host->nvram.mem.profileMemTime[profile].vdd < SPDReg) {
            Host->nvram.mem.profileMemTime[profile].vdd = SPDReg;
          }

          checkSum[socket][ch][dimm] = checkSum[socket][ch][dimm] + (UINT16) SPDReg;

          if (Setup->mem.options & MEM_OVERRIDE_EN) {
            if ((*dimmNvList)[dimm].XMPChecksum != checkSum[socket][ch][dimm]) {
              //
              // If timing overrides are enabled but the DIMM configuration has changed disable memory overrides
              // and save the new checksum
              //
#ifdef DEBUG_CODE_BLOCK
              if (GetDebugLevel () & SDBG_MINMAX) {
                AcquirePrintControl ();
                RcDebugPrint (SDBG_DEFAULT, "DIMM configuration changed!\n");
                RcDebugPrint (SDBG_DEFAULT, "Old checksum = 0x%x\n", (*dimmNvList)[dimm].XMPChecksum);
                RcDebugPrint (SDBG_DEFAULT, "New checksum = 0x%x\n", checkSum[socket][ch][dimm]);
                ReleasePrintControl ();
              }
#endif //DEBUG_CODE_BLOCK
              //debug...
              //OutputWarning (WARN_MEM_CONFIG_CHANGED, WARN_MEM_OVERRIDE_DISABLED, socket, ch, dimm, 0xFF);

              //
              // Disable overrides
              // Setup->mem.options &= ~MEM_OVERRIDE_EN;
              // Save new checksum
              //
              (*dimmNvList)[dimm].XMPChecksum = checkSum[socket][ch][dimm];

              //
              // Force frequency to auto
              //
              Host->var.mem.ddrFreqLimit = 0;
            }
          } else {
            //
            // Save the checksum
            //
            (*dimmNvList)[dimm].XMPChecksum = checkSum[socket][ch][dimm];
          }
        } // dimm loop
      } // ch loop
    } // socket loop

    if (Host->nvram.mem.profileMemTime[profile].vdd & BIT7) {
      Host->nvram.mem.profileMemTime[profile].vdd = (Host->nvram.mem.profileMemTime[profile].vdd & ~BIT7) + 100;
    }
#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      AcquirePrintControl ();
      RcDebugPrint (SDBG_DEFAULT, "XMP Profile %d\n", profile + 1);
      RcDebugPrint (SDBG_DEFAULT, "nCL = %d\n", Host->nvram.mem.profileMemTime[profile].nCL);
      RcDebugPrint (SDBG_DEFAULT, "nRP = %d\n", Host->nvram.mem.profileMemTime[profile].nRP);
      RcDebugPrint (SDBG_DEFAULT, "nRCD = %d\n", Host->nvram.mem.profileMemTime[profile].nRCD);
      RcDebugPrint (SDBG_DEFAULT, "nRRD = %d\n", Host->nvram.mem.profileMemTime[profile].nRRD);
      RcDebugPrint (SDBG_DEFAULT, "nRRD_L = %d\n", Host->nvram.mem.profileMemTime[profile].nRRD_L);
      RcDebugPrint (SDBG_DEFAULT, "nRAS = %d\n", Host->nvram.mem.profileMemTime[profile].nRAS);
      RcDebugPrint (SDBG_DEFAULT, "nFAW = %d\n", Host->nvram.mem.profileMemTime[profile].nFAW);
      RcDebugPrint (SDBG_DEFAULT, "nCWL = %d\n", Host->nvram.mem.profileMemTime[profile].nCWL);
      RcDebugPrint (SDBG_DEFAULT, "nRC = %d\n", Host->nvram.mem.profileMemTime[profile].nRC);
      RcDebugPrint (SDBG_DEFAULT, "nRFC = %d\n", Host->nvram.mem.profileMemTime[profile].nRFC);
      RcDebugPrint (SDBG_DEFAULT, "ddrFreqLimit = %d\n", Host->nvram.mem.profileMemTime[profile].ddrFreqLimit);
      RcDebugPrint (SDBG_DEFAULT, "vdd = 0x%x\n", Host->nvram.mem.profileMemTime[profile].vdd);
      RcDebugPrint (SDBG_DEFAULT, "casSup = 0x%x\n", Host->nvram.mem.profileMemTime[profile].casSup);
      RcDebugPrint (SDBG_DEFAULT, "ddrFreq = %d\n", Host->nvram.mem.profileMemTime[profile].ddrFreq);
      ReleasePrintControl ();
    }
#endif //DEBUG_CODE_BLOCK
  } // profile loop
} // GetXMPTimingsDDR4

UINT16
GetCycles (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mtbOffset,
  UINT16    ftbOffset,
  UINT32    tCK
  )
/*++

  Gathers XMP timings for DDR4

  @param Host      - Pointer to sysHost
  @param socket    - Socket Id
  @param ch        - Channel
  @param dimm      - DIMM
  @param mtbOffset - Time in MTB (Medium Time Base)
  @param ftbOffset - Time in FTB (Fine Time Base)
  @param tCK       - tCK

  @retval Time in DCLKs

--*/
{
  UINT8             mtbVal;
  UINT8             ftbVal = 0;

  SpdReadByte (socket, ch, dimm, mtbOffset, &mtbVal);
  if (ftbOffset) {
    SpdReadByte (socket, ch, dimm, ftbOffset, &ftbVal);
  }

  return TimeBaseToDCLKtCK (Host, socket, ch, dimm, (UINT16) mtbVal, (UINT16) ftbVal, tCK);
} // GetCycles

UINT16
TimeBaseToDCLKtCK (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mtbTime,
  UINT16    ftbTime,
  UINT32    tCK
  )
/*++

  Converts MTB/FTB to DCLK

  @param Host      - Pointer to sysHost
  @param socket    - Socket Id
  @param ch        - Channel
  @param dimm      - DIMM
  @param mtbTime   - Time in MTB (Medium Time Base)
  @param ftbTime   - Time in FTB (Fine Time Base)
  @param tCK       - tCK

  @retval Time in DCLKs

--*/
{
  UINT16              mtb;
  UINT16              ftb;
  UINT32              tempVal;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList (Host, socket, ch);

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    mtb = 1250;
    ftb = 10;
  } else {
    mtb = 10000 / (*dimmNvList)[dimm].mtbDiv;
    if ((*dimmNvList)[dimm].SPDftb) {
      ftb = ((((*dimmNvList)[dimm].SPDftb >> 4) & 0xF) * 10) / ((*dimmNvList)[dimm].SPDftb & 0xF);
    } else {
      ftb = 0;
    }
  }

  if (ftbTime & BIT7) {
    tempVal = (UINT32) ((mtbTime * mtb) + ((ftbTime - 256) * ftb));
  } else {
    tempVal = (UINT32) ((mtbTime * mtb) + ((ftbTime & 0x3F) * ftb));
  }

  //
  // Round up to next cycle
  //
  tempVal = (tempVal + tCK - 1) / tCK;

  return (UINT16) tempVal;
} // TimeBaseToDCLKtCK
