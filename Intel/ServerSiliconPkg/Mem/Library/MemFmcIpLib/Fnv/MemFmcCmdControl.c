/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <SysHostChip.h>
#include <Library/MemoryCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include "Include/MemFmcRegs.h"
#include "Chip/Include/FnvAccessCommon.h"
#include <Library/MemFmcIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>

#include "Include/MemFmcIpLibInternal.h"

//-----------------------------------------------------------------------------------
//
// This const array use as DDRIO convert table. Each index will be convert to real address.
//

// Define maximums for CMD, CTL and CK for both processors
#define MAX_CMD_DELAY   (3*64+63)   // Logic value of 3 and Pi value of 63
#define MAX_CTL_DELAY   (3*64+63)   // Logic value of 3 and Pi value of 63
#define MAX_CLK_DELAY   127
#define MAX_CK_DELAY    128
#define MAX_CMD_DELAY_FNV   (3*64+63)   // Logic value of 3 and Pi value of 63

#define MIN_CMD_VREF_FMC        0
#define MAX_CMD_VREF_BKSD_FMC   63
#define MAX_CMD_VREF_FMC        63
//-----------------------------------------------------------------------------------

//
// Local Prototypes
//


/**

  Get min/max limits for Command, Control and Clock signals

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - DIMM number
  @param[in]  Level       - IO level to access
  @param[in]  Group       - Command, clock or control group to access
  @param[out] *MinLimit   - Minimum delay value allowed
  @param[out] *MaxLimit   - Maximum delay value allowed

  @retval EFI_SUCESS if signal max/mins are returned
          EFI_NOT_FOUND otherwise

**/
EFI_STATUS
GetCmdGroupLimitsFmc (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    MRC_LT    Level,
  IN    MRC_GT    Group,
  OUT   UINT16    *MinLimit,
  OUT   UINT16    *MaxLimit
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;

  switch (Group) {
    case CmdVref:
      *MinLimit = MIN_CMD_VREF_FMC;
      *MaxLimit = MAX_CMD_VREF_FMC;
      break;
    default:
      Status = EFI_NOT_FOUND;
      break;
  }
  return Status;
}

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param dimm:     DIMM number
  @param ioGroup:  Group number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset
  @param *minVal:  Current minimum control delay
  @param *maxVal:  Current maximum control delay

  @retval Pi delay value

**/
MRC_STATUS
GetSetCtlDelayFnv (
               PSYSHOST Host,
               UINT8    socket,
               UINT8    ch,
               UINT8    dimm,
               UINT8    mode,
               INT16    *value,
               UINT16   *minDelay,
               UINT16   *maxDelay
               )
{
  UINT8                                       logicVal;
  UINT8                                       piVal;
  UINT16                                      curVal;
  UINT16                                      piDelay;
  UINT16                                      maxVal;
  INT16                                       tmp;
  struct socketNvram                          *socketNvram;
  DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_STRUCT ddrCrCmdPiCodingFnv2;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY_FNV;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv2.Data, 0, FNV_IO_READ, 0xF);
  } else {
    ddrCrCmdPiCodingFnv2.Data = socketNvram->channelList[ch].ddrCrCmdPiCodingFnv2;
  }

  logicVal = (UINT8)ddrCrCmdPiCodingFnv2.Bits.cmdpilogicdelay4;
  piVal = (UINT8)ddrCrCmdPiCodingFnv2.Bits.cmdpicode4;

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    // Ensure we do not exceed maximums (caller should enforce this)
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    // ODT and CKE
    ddrCrCmdPiCodingFnv2.Bits.cmdpilogicdelay4 = logicVal;
    ddrCrCmdPiCodingFnv2.Bits.cmdpicode4 = piVal;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].ddrCrCmdPiCodingFnv2 != ddrCrCmdPiCodingFnv2.Data) || (mode & GSM_FORCE_WRITE)) {
      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv2.Data, 0, FNV_IO_WRITE, 0xF);
      IODdrtResetPerCh (Host, socket, ch, dimm);
    }

  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->channelList[ch].ddrCrCmdPiCodingFnv2 = ddrCrCmdPiCodingFnv2.Data;
  }

  // Hack for conformity
  *minDelay = *value;
  *maxDelay = *value;

  return MRC_STATUS_SUCCESS;
} // GetSetCtlDelayFnv


/**
  Multi-use function to either get or set command delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param dimm:     DIMM number
  @param group:    Group number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program
  @param *minVal:  Current minimum command delay
  @param *maxVal:  Current maximum command delay


  @retval minVal and maxVal

**/
MRC_STATUS
GetSetCmdGroupDelayFnv (
                    PSYSHOST  Host,
                    UINT8     socket,
                    UINT8     ch,
                    UINT8     dimm,
                    MRC_GT    group,
                    UINT8     mode,
                    INT16     *value,
                    UINT16    *minVal,
                    UINT16    *maxVal
                    )
{
  MRC_GT groups[2];
  UINT8  groupIdx;
  UINT8  numGroups = 0;
  MRC_STATUS      status = MRC_STATUS_SUCCESS;

  switch (group) {
    case CmdGrp3:
    case CmdGrp4:
      groups[0] = group;
      numGroups = 1;
      break;
    case CmdAll:
      groups[0] = CmdGrp3;
      groups[1] = CmdGrp4;
      numGroups = 2;
      break;
    default:
      groups[0] = CmdGrp3;
      groups[1] = CmdGrp4;
      numGroups = 2;
      status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
    }

  if (status == MRC_STATUS_SUCCESS) {
    for (groupIdx = 0; groupIdx < numGroups; groupIdx++) {

      status = GetSetCmdDelayFnv (Host, socket, ch, dimm, groups[groupIdx], mode, value);

      // Update min/max values
      UpdateMinMaxUint (*value, minVal, maxVal);
    }
  } // status
  return status;
} // GetSetCmdGroupDelayFnv


/**
  Multi-use function to either get or set command delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param dimm:     DIMM number
  @param ioGroup:  Group number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset
  @param *minVal:  Current minimum control delay
  @param *maxVal:  Current maximum control delay

  @retval Pi delay value

**/
MRC_STATUS
GetSetCmdDelayFnv (
               PSYSHOST Host,
               UINT8    socket,
               UINT8    ch,
               UINT8    dimm,
               MRC_GT   group,
               UINT8    mode,
               INT16    *value
               )
{
  UINT8                                       logicVal;
  UINT8                                       piVal;
  UINT16                                      curVal;
  UINT16                                      piDelay;
  UINT16                                      maxVal = 0;
  INT16                                       tmp;
  struct socketNvram                          *socketNvram;
  DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_STRUCT ddrCrCmdPiCodingFnv;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY_FNV;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_READ, 0xF);
  } else {
    ddrCrCmdPiCodingFnv.Data = socketNvram->channelList[ch].ddrCrCmdPiCodingFnv;
  }

  switch (group) {
  case CmdGrp3: // BCOM[1:0]
    logicVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay1;
    piVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpicode1;
    break;
  case CmdGrp4: // BCOM[3:2]
    logicVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay0;
    piVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpicode0;
    break;
  default:
    logicVal = 0;
    piVal = 0;
    break;
    // Assert error
  }

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    // Ensure we do not exceed maximums (caller should enforce this)
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    switch (group) {
    case CmdGrp3: // BCOM[1:0]
      ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay1 = logicVal;
      ddrCrCmdPiCodingFnv.Bits.cmdpicode1 = piVal;
      break;
    case CmdGrp4: // BCOM[3:2]
      ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay0 = logicVal;
      ddrCrCmdPiCodingFnv.Bits.cmdpicode0 = piVal;
      break;
    default:
      break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].ddrCrCmdPiCodingFnv != ddrCrCmdPiCodingFnv.Data) || (mode & GSM_FORCE_WRITE)) {
      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_WRITE, 0xF);
      IODdrtResetPerCh (Host, socket, ch, dimm);
    }

  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->channelList[ch].ddrCrCmdPiCodingFnv = ddrCrCmdPiCodingFnv.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdDelayFnv

/**
  Multi-use function to either get or set command vref

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

**/
MRC_STATUS
GetSetCmdVrefFnv (
              PSYSHOST  Host,
              UINT8     socket,
              UINT8     ch,
              UINT8     dimm,
              UINT8     mode,
              INT16     *value
              )
{
  INT16                                         curVal;
  INT16                                         vref;
  UINT16                                        maxVal;
  INT16                                         tmp;
  struct socketNvram                            *socketNvram;
  DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_STRUCT  ddrCrDimmVrefControl1;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_VREF_FMC;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    // Read only 1 FUB to save time. All 3 should be programmed to be the same
    GetSetFnvIO (Host, socket, ch, dimm, DDRCRCMDCONTROLS_NA_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_READ, 0xF);
  } else {
    ddrCrDimmVrefControl1.Data = socketNvram->channelList[ch].dimmVrefControlFnv1;
  }

  //
  // Get the CA Vref current setting
  //
  curVal = (INT16)(ddrCrDimmVrefControl1.Bits.rxvref);

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      vref = tmp;
    } else {
      vref = 0;
    }
    // Ensure we do not exceed maximums
    if (vref > maxVal) {
      vref = maxVal;
    }

    //
    // Program the new value
    //
    ddrCrDimmVrefControl1.Bits.rxvref = vref;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].dimmVrefControlFnv1 != ddrCrDimmVrefControl1.Data) || (mode & GSM_FORCE_WRITE)) {
      //SKX change
      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_NA_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_NB_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
      if ((mode & GSM_SKIP_CMD_FUB) == 0) {
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SB_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
      }
      IODdrtResetPerCh (Host, socket, ch, dimm);
    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->channelList[ch].dimmVrefControlFnv1 = ddrCrDimmVrefControl1.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdVrefFnv

/**
  Multi-use function to either get or set command vref

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

**/
MRC_STATUS
GetSetCmdVrefFnvBackside(
              PSYSHOST  Host,
              UINT8     socket,
              UINT8     ch,
              UINT8     dimm,
              UINT8     mode,
              INT16     *value
              )
{
  INT16                                         curVal;
  INT16                                         vref;
  UINT16                                        maxVal = 0;
  INT16                                         tmp;
  struct socketNvram                            *socketNvram;
  DDRCRDIMMVREFCONTROL1_CH_FNV_DDRIO_COMP_STRUCT ddrCrDimmVrefControl1;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_VREF_BKSD_FMC;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    GetSetFnvIO(Host, socket, ch, dimm, DDRCRDIMMVREFCONTROL1_CH_FNV_DDRIO_COMP_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_READ, 0xF);
  } else {
    ddrCrDimmVrefControl1.Data = socketNvram->channelList[ch].dimmVrefControlFnv1Sa;
  }

  //
  // Get the CA Vref current setting
  //
  curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch0cavrefctl >> 1);

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      vref = tmp;
    } else {
      vref = 0;
    }
    // Ensure we do not exceed maximums
    if (vref > maxVal) {
      vref = maxVal;
    }

    //
    // Program the new value
    //
    ddrCrDimmVrefControl1.Bits.ch0cavrefctl = vref << 1;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].dimmVrefControlFnv1Sa != ddrCrDimmVrefControl1.Data) || (mode & GSM_FORCE_WRITE)) {
      GetSetFnvIO(Host, socket, ch, dimm, DDRCRDIMMVREFCONTROL1_CH_FNV_DDRIO_COMP_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
      IODdrtResetPerCh(Host, socket, ch, dimm);
    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->channelList[ch].dimmVrefControlFnv1Sa = ddrCrDimmVrefControl1.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdVrefFnvBackside


/**
  Multi-use function to either get or set control delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param dimm:     DIMM number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program

  @retval Pi delay value

**/
MRC_STATUS
GetSetClkDelayFnv (
               PSYSHOST Host,
               UINT8    socket,
               UINT8    ch,
               UINT8    dimm,
               UINT8    mode,
               INT16    *value
               )
{
  UINT8                                 logicVal;
  UINT8                                 piVal;
  UINT16                                curVal;
  INT16                                 tmp;
  UINT16                                piDelay;
  UINT16                                maxVal;
  struct socketNvram                    *socketNvram;
  DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_STRUCT   ddrCrCmdPiCodingFnv;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY_FNV;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_READ, 0xF);
  } else {
    ddrCrCmdPiCodingFnv.Data = socketNvram->channelList[ch].ddrCrCmdPiCodingFnv;
  }

  logicVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay2;
  piVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpicode2;

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      // Get the new offset (note: can be negative)
      tmp = curVal + *value;
    } else {
      // FORCE_WRITE
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    // Ensure we do not exceed maximums (caller should enforce this)
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay2 = logicVal;
    ddrCrCmdPiCodingFnv.Bits.cmdpicode2 = piVal;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].ddrCrCmdPiCodingFnv != ddrCrCmdPiCodingFnv.Data) || (mode & GSM_FORCE_WRITE)) {
      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_WRITE, 0xF);
      IODdrtResetPerCh(Host, socket, ch, dimm);
    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->channelList[ch].ddrCrCmdPiCodingFnv = ddrCrCmdPiCodingFnv.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetClkDelayFnv


VOID
Resync(
        PSYSHOST Host,
        UINT8    socket,
        UINT8    ch,
        UINT8    clk
)
{
  UINT8                                           dimm;
  struct dimmNvram                                (*dimmNvList)[MAX_DIMM];
  MSC_PLL_CTRL_FNV_DFX_MISC_0_STRUCT              pllCtrlMisc;
  MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_STRUCT  fnvioInitCtl;

  dimmNvList = GetDimmNvList(Host, socket, ch);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }

    //
    // Avoid clock movement for BPS A0
    //
    if (((*dimmNvList)[dimm].fmcType == FMC_BWV_TYPE) && (GetFmcStepping (Host, socket, ch, dimm) < FmcStepA1)) {
      continue;
    }

    if ((clk == 1) || (clk == 3)) {
      continue;
    }

    fnvioInitCtl.Data = ReadFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_REG);

    //
    // 1. Clear ddrtio and daunit pmreset masks
    //
    pllCtrlMisc.Data = 0;
    GetSetProtectedDcpmmRegister (Host, socket, ch, dimm, GSM_FORCE_WRITE, MSC_PLL_CTRL_FNV_DFX_MISC_0_REG,
      (PROT_REG_MASK_DAUNIT_PMRESET | PROT_REG_MASK_DDRTIO_PMRESET), &pllCtrlMisc.Data);

    //
    // 2. Wait 2 usec
    //
    FixedDelayMicroSecond (2);

    //
    // 3. Set ddrt clk idle mask
    //
    pllCtrlMisc.Bits.ddrtio_ckidle_mask = 1;
    GetSetProtectedDcpmmRegister (Host, socket, ch, dimm, GSM_FORCE_WRITE, MSC_PLL_CTRL_FNV_DFX_MISC_0_REG,
      PROT_REG_MASK_DDRTIO_CKIDLE, &pllCtrlMisc.Data);

    //
    // 4. Kick off Ddrt Resync FSM
    //
    fnvioInitCtl.Bits.ddrt_resync = 1;
    WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_REG, fnvioInitCtl.Data);

    //
    // 5. Wait 2 us
    //
    FixedDelayMicroSecond (2);

    //
    // 6. Clear DDRT Resync Bit
    //
    fnvioInitCtl.Bits.ddrt_resync = 0;
    WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_REG, fnvioInitCtl.Data);

  } // dimm loop
}

VOID
RelockPll (
           PSYSHOST Host,
           UINT8    socket,
           UINT8    ch,
           UINT8    clk
)
{
  UINT8                                           dimm;
  struct dimmNvram                                (*dimmNvList)[MAX_DIMM];
  MSC_PLL_CTRL_FNV_DFX_MISC_0_STRUCT              pllCtrlMisc;

  dimmNvList = GetDimmNvList(Host, socket, ch);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }

    //
    // Avoid clock movement for BPS A0
    //
    if (((*dimmNvList)[dimm].fmcType == FMC_BWV_TYPE) && (GetFmcStepping (Host, socket, ch, dimm) < FmcStepA1)) {
      continue;
    }

    if ((clk == 1) || (clk == 3)) {
      continue;
    }

    //
    // 1. Set ddrtio and daunit pmreset masks
    //
    pllCtrlMisc.Data = 0;
    pllCtrlMisc.Bits.daunit_pmreset_mask = 1;
    pllCtrlMisc.Bits.ddrtio_pmreset_mask = 1;
    GetSetProtectedDcpmmRegister (Host, socket, ch, dimm, GSM_FORCE_WRITE, MSC_PLL_CTRL_FNV_DFX_MISC_0_REG,
      (PROT_REG_MASK_DAUNIT_PMRESET | PROT_REG_MASK_DDRTIO_PMRESET), &pllCtrlMisc.Data);

    //
    // 2. Wait 10usec
    //
    FixedDelayMicroSecond (10);

    //
    // 3. Clear ddrt clk idle mask
    //
    pllCtrlMisc.Data = 0;
    GetSetProtectedDcpmmRegister (Host, socket, ch, dimm, GSM_FORCE_WRITE, MSC_PLL_CTRL_FNV_DFX_MISC_0_REG,
      PROT_REG_MASK_DDRTIO_CKIDLE, &pllCtrlMisc.Data);

    //
    // Apply 15 usec delay to avoid race condition
    //
    FixedDelayMicroSecond (15);
  } // dimm loop
}

/**
  DDRT Clock Movement

  @param[in]      Socket      - Socket number
  @param[in]      Ch          - Memory channel number
  @param[in]      OrgClk      - Original clk number with step size of clkpi/128
  @param[in]      TargetClk   - Target clk number with step size of clkpi/128
  @param[in, out] NewClk      - Pointer to New clk number with step size of clkpi/128

  @retval TRUE  - Allowed
          FALSE - Not allowed
*/
BOOLEAN
EFIAPI
IsDdrtClockMovementAllowed (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  OrgClk,
  IN     UINT8  TargetClk,
  IN OUT UINT8  *NewClk
  )
{
  return TRUE;
}

/**
  Clear PllCtrlMisc.ddrtio_ckidle_mask to enable CLK Sync FSM on a given channel

  @param Host     Pointer to SYSHOST
  @param Socket   Socket number
  @param Channel  Channel number

  @retval         None

**/
VOID
ClearCkIdleMaskCh (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Channel
  )
{
  UINT8                               Dimm;
  struct dimmNvram                    (*DimmNvList)[MAX_DIMM];
  MSC_PLL_CTRL_FNV_DFX_MISC_0_STRUCT  MscPllCtrl;

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
      continue;
    }

    if (Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] < FIS_203_VERSION) {
      continue;
    }

    //
    // Avoid clock movement for BPS A0
    //
    if (((*DimmNvList)[Dimm].fmcType != FMC_BWV_TYPE) || (GetFmcStepping (Host, Socket, Channel, Dimm) < FmcStepA1)) {
      continue;
    }

    MscPllCtrl.Data = 0;
    GetSetProtectedDcpmmRegister (Host, Socket, Channel, Dimm, GSM_FORCE_WRITE, MSC_PLL_CTRL_FNV_DFX_MISC_0_REG,
      PROT_REG_MASK_DDRTIO_CKIDLE, &MscPllCtrl.Data);
  }

  return;
}

/**
  Enable Resync FSM on all DDRT DIMMs present on the given socket

  @param Socket   Socket number

  @retval         None
**/
VOID
FnvEnableResyncFsm (
  UINT8    Socket
  )
{
  PSYSHOST              Host = GetSysHostPointer ();
  UINT8                 Channel;
  UINT8                 MaxChDdr = GetMaxChDdr ();
  struct channelNvram   (*ChannelNvList)[MAX_CH] = GetChannelNvList (Host, Socket);

  if ((GetSysBootMode () != NormalBoot) || (Host->var.mem.subBootMode != WarmBootFast)) {
    return;
  }

  if (IsSiliconWorkaroundEnabled ("S2209980002") == FALSE) {
    return;
  }

  OutputCheckpoint (CHECKPOINT_MAJOR_CLOCK_INIT, CHECKPOINT_MINOR_DCPMM_ENABLE_RESYNC_FSM, 0);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    ClearCkIdleMaskCh (Host, Socket, Channel);
  }

  return;
}

