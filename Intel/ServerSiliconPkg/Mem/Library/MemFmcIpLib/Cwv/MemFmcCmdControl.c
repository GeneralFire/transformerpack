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
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>

#include "Include/MemFmcIpLibInternal.h"

//-----------------------------------------------------------------------------------
//
// This const array use as DDRIO convert table. Each index will be convert to real address.
//

#define MAX_CLOCK_MOVEMENT      127
#define CLOCK_MOVEMENT_STEP     32

// Define maximums for CMD, CTL and CK for both processors
#define MAX_CMD_DELAY   (3*64+63)   // Logic value of 3 and Pi value of 63
#define MAX_CTL_DELAY   (3*64+63)   // Logic value of 3 and Pi value of 63
#define MAX_CLK_DELAY   127
#define MAX_CK_DELAY    128
#define MAX_CMD_DELAY_FNV   (1*64+63)   // Logic value of 1 and Pi value of 63

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
//CWV_TODO  UINT16                                      curVal;
  UINT16                                      piDelay;
  UINT16                                      maxVal;
  INT16                                       tmp = 0;
  struct socketNvram                          *socketNvram;
//CWV_TODO  DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_STRUCT ddrCrCmdPiCodingFnv;


  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY_FNV;

  //
  // Get register data
  //
//CWV_TODO  if (mode & GSM_READ_CSR) {
//CWV_TODO    GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO  } else {
//CWV_TODO    ddrCrCmdPiCodingFnv.Data = socketNvram->channelList[ch].ddrCrCmdPiCodingFnv;
//CWV_TODO  }

//CWV_TODO  logicVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay0;
//CWV_TODO  piVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpicode0;

  // Combine into delay
//CWV_TODO  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
//CWV_TODO    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
//CWV_TODO      tmp = curVal + *value;
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

    // BCOM[1:0]
//CWV_TODO    ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay0 = logicVal;
//CWV_TODO    ddrCrCmdPiCodingFnv.Bits.cmdpicode0 = piVal;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
//CWV_TODO    if ((socketNvram->channelList[ch].ddrCrCmdPiCodingFnv != ddrCrCmdPiCodingFnv.Data) || (mode & GSM_FORCE_WRITE)) {
//CWV_TODO      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_WRITE, 0xF);
//CWV_TODO      IODdrtResetPerCh (Host, socket, ch, dimm);
//CWV_TODO    }

  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
//CWV_TODO    socketNvram->channelList[ch].ddrCrCmdPiCodingFnv = ddrCrCmdPiCodingFnv.Data;
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
  UINT8                                       logicVal = 0;
  UINT8                                       piVal = 0;
  UINT16                                      curVal;
  UINT16                                      piDelay;
  UINT16                                      maxVal = 0;
  INT16                                       tmp;
  struct socketNvram                          *socketNvram;
//CWV_TODO  DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_STRUCT ddrCrCmdPiCoding2;


  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY_FNV;

  //
  // Get register data
  //
//CWV_TODO  if (mode & GSM_READ_CSR) {
//CWV_TODO    GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCoding2.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO  } else {
//CWV_TODO    ddrCrCmdPiCoding2.Data = socketNvram->channelList[ch].ddrCrCmdPiCodingFnv2;
//CWV_TODO  }

  switch (group) {
  case CmdGrp3: // BCOM[1:0]
//CWV_TODO    logicVal = (UINT8)ddrCrCmdPiCoding2.Bits.cmdpilogicdelay3;
//CWV_TODO    piVal = (UINT8)ddrCrCmdPiCoding2.Bits.cmdpicode3;
    break;
  case CmdGrp4: // BCOM[3:2]
//CWV_TODO    logicVal = (UINT8)ddrCrCmdPiCoding2.Bits.cmdpilogicdelay4;
//CWV_TODO    piVal = (UINT8)ddrCrCmdPiCoding2.Bits.cmdpicode4;
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
//CWV_TODO      ddrCrCmdPiCoding2.Bits.cmdpilogicdelay3 = logicVal;
//CWV_TODO      ddrCrCmdPiCoding2.Bits.cmdpicode3 = piVal;
      break;
    case CmdGrp4: // BCOM[3:2]
//CWV_TODO      ddrCrCmdPiCoding2.Bits.cmdpilogicdelay4 = logicVal;
//CWV_TODO      ddrCrCmdPiCoding2.Bits.cmdpicode4 = piVal;
      break;
    default:
      break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
//CWV_TODO    if ((socketNvram->channelList[ch].ddrCrCmdPiCodingFnv2 != ddrCrCmdPiCoding2.Data) || (mode & GSM_FORCE_WRITE)) {
//CWV_TODO      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCoding2.Data, 0, FNV_IO_WRITE, 0xF);
//CWV_TODO      IODdrtResetPerCh (Host, socket, ch, dimm);
//CWV_TODO    }

  }

  //
  // Save to cache
  //
//CWV_TODO  if (mode & GSM_UPDATE_CACHE) {
//CWV_TODO    socketNvram->channelList[ch].ddrCrCmdPiCodingFnv2 = ddrCrCmdPiCoding2.Data;
//CWV_TODO  }

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
  INT16                                         curVal = 0; //CWV_TODO
  INT16                                         vref;
  UINT16                                        maxVal;
  INT16                                         tmp;
  struct socketNvram                            *socketNvram;
//CWV_TODO  DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_STRUCT  ddrCrDimmVrefControl1;

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
//CWV_TODO    GetSetFnvIO (Host, socket, ch, dimm, DDRCRCMDCONTROLS_NA_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_READ, 0xF);
  } else {
//CWV_TODO    ddrCrDimmVrefControl1.Data = socketNvram->channelList[ch].dimmVrefControlFnv1;
  }

  //
  // Get the CA Vref current setting
  //
//CWV_TODO  curVal = (INT16)(ddrCrDimmVrefControl1.Bits.rxvref);

  // Read only?
  if (mode & GSM_READ_ONLY) {
//CWV_TODO    *value = (INT16)curVal;
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
//CWV_TODO    ddrCrDimmVrefControl1.Bits.rxvref = vref;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
//CWV_TODO    if ((socketNvram->channelList[ch].dimmVrefControlFnv1 != ddrCrDimmVrefControl1.Data) || (mode & GSM_FORCE_WRITE)) {
//CWV_TODO      //SKX change
//CWV_TODO      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_NA_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
//CWV_TODO      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_NB_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
//CWV_TODO      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SB_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
//CWV_TODO      IODdrtResetPerCh (Host, socket, ch, dimm);
//CWV_TODO    }
  }

  //
  // Save to cache
  //
//CWV_TODO  if (mode & GSM_UPDATE_CACHE) {
//CWV_TODO    socketNvram->channelList[ch].dimmVrefControlFnv1 = ddrCrDimmVrefControl1.Data;
//CWV_TODO  }

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
  INT16                                         curVal = 0; //CWV_TODO
  INT16                                         vref;
  UINT16                                        maxVal = 0;
  INT16                                         tmp;
  struct socketNvram                            *socketNvram;
//CWV_TODO  DDRCRDIMMVREFCONTROL1_CH_FNV_DDRIO_COMP_STRUCT ddrCrDimmVrefControl1;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_VREF_BKSD_FMC;

  //
  // Get register data
  //
//CWV_TODO  if (mode & GSM_READ_CSR) {
//CWV_TODO    GetSetFnvIO(Host, socket, ch, dimm, DDRCRDIMMVREFCONTROL1_CH_FNV_DDRIO_COMP_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO  } else {
//CWV_TODO    ddrCrDimmVrefControl1.Data = socketNvram->channelList[ch].dimmVrefControlFnv1Sa;
//CWV_TODO  }

  //
  // Get the CA Vref current setting
  //
//CWV_TODO  curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch0cavrefctl >> 1);

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
//CWV_TODO    ddrCrDimmVrefControl1.Bits.ch0cavrefctl = vref << 1;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
//CWV_TODO    if ((socketNvram->channelList[ch].dimmVrefControlFnv1Sa != ddrCrDimmVrefControl1.Data) || (mode & GSM_FORCE_WRITE)) {
//CWV_TODO      GetSetFnvIO(Host, socket, ch, dimm, DDRCRDIMMVREFCONTROL1_CH_FNV_DDRIO_COMP_REG, &ddrCrDimmVrefControl1.Data, 0, FNV_IO_WRITE, 0xF);
//CWV_TODO      IODdrtResetPerCh(Host, socket, ch, dimm);
//CWV_TODO    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
//CWV_TODO    socketNvram->channelList[ch].dimmVrefControlFnv1Sa = ddrCrDimmVrefControl1.Data;
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
  UINT8                                 logicVal = 0; //CWV_TODO
  UINT8                                 piVal = 0; //CWV_TODO
  UINT16                                curVal;
  INT16                                 tmp;
  UINT16                                piDelay;
  UINT16                                maxVal;
  struct socketNvram                    *socketNvram;
//CWV_TODO  DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_STRUCT   ddrCrCmdPiCodingFnv;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY_FNV;

  //
  // Get register data
  //
//CWV_TODO  if (mode & GSM_READ_CSR) {
//CWV_TODO    GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO  } else {
//CWV_TODO    ddrCrCmdPiCodingFnv.Data = socketNvram->channelList[ch].ddrCrCmdPiCodingFnv;
//CWV_TODO  }

//CWV_TODO  logicVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay2;
//CWV_TODO  piVal = (UINT8)ddrCrCmdPiCodingFnv.Bits.cmdpicode2;

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

//CWV_TODO    ddrCrCmdPiCodingFnv.Bits.cmdpilogicdelay2 = logicVal;
//CWV_TODO    ddrCrCmdPiCodingFnv.Bits.cmdpicode2 = piVal;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
//CWV_TODO    if ((socketNvram->channelList[ch].ddrCrCmdPiCodingFnv != ddrCrCmdPiCodingFnv.Data) || (mode & GSM_FORCE_WRITE)) {
//CWV_TODO      GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCrCmdPiCodingFnv.Data, 0, FNV_IO_WRITE, 0xF);
//CWV_TODO      IODdrtResetPerCh(Host, socket, ch, dimm);
//CWV_TODO    }
  }

  //
  // Save to cache
  //
//CWV_TODO  if (mode & GSM_UPDATE_CACHE) {
//CWV_TODO    socketNvram->channelList[ch].ddrCrCmdPiCodingFnv = ddrCrCmdPiCodingFnv.Data;
//CWV_TODO  }

  return MRC_STATUS_SUCCESS;
} // GetSetClkDelayFnv

VOID
Resync (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    clk
  )
{
  return;
}

VOID
RelockPll (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Clk
  )
{
  return;
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
  BOOLEAN     Allowed = TRUE;

  if (IsDcpmmPresentChannel (Socket, Ch)) {

    Allowed = FALSE;

    *NewClk = OrgClk + CLOCK_MOVEMENT_STEP;
    if ((TargetClk >= OrgClk) && (TargetClk <= *NewClk) && (TargetClk <= MAX_CLOCK_MOVEMENT)) {
      // Allow to move to the target clk
      *NewClk = TargetClk;
    } else if (*NewClk > MAX_CLOCK_MOVEMENT) {
      // The new clk is more than max clock, set to max
      *NewClk = MAX_CLOCK_MOVEMENT;
    }

    // if the original clock is equal to max value, set the new clock to 0 for next cycle
    if (OrgClk == MAX_CLOCK_MOVEMENT) {
      *NewClk = 0;
    }

    //
    // Allow Clock Movement when
    // 1. Clock movement must only move forwards (no shrinking of clock period). Movement no more than Qclk (<=32 ticks)
    // 2. Clock can move from 127 -> 0
    //
    if ((*NewClk > OrgClk) && ((*NewClk - OrgClk) <= CLOCK_MOVEMENT_STEP) ||
        ((OrgClk == 127) && (*NewClk == 0))) {
      Allowed = TRUE;
    }
  }

  return Allowed;
}

/**
  Clear PllCtrlMisc.ddrtio_ckidle_mask to enable CLK Sync FSM on a given channel

  @param Socket   Socket number
  @param Channel  Channel number

  @retval         None

**/
VOID
ClearCkIdleMaskCh (
  UINT8    Socket,
  UINT8    Channel
  )
{
  UINT8                               Dimm;
  PSYSHOST                            Host;
  IO_INIT_CTRL_CWV_DFX_MISC_0_STRUCT  MscPllCtrl;

  Host = GetSysHostPointer ();

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }
    if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == FALSE) {
      continue;
    }

    if (Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] < FIS_300_VERSION) {
      continue;
    }

    MscPllCtrl.Data = 0;
    GetSetProtectedDcpmmRegister (Host, Socket, Channel, Dimm, GSM_FORCE_WRITE, IO_INIT_CTRL_CWV_DFX_MISC_0_REG,
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

  if ((GetSysBootMode () != NormalBoot) || (Host->var.mem.subBootMode != WarmBootFast)) {
    return;
  }

  OutputCheckpoint (CHECKPOINT_MAJOR_CLOCK_INIT, CHECKPOINT_MINOR_DCPMM_ENABLE_RESYNC_FSM, 0);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    ClearCkIdleMaskCh (Socket, Channel);
  }

  return;
}
