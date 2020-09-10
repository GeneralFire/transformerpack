/** @file
  Mesh2Mem RAS support Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

//
// Include files
//

#include "UncoreCommonIncludes.h"
#include <Library/Mesh2MemIpLib.h>
#include "Include/Mesh2MemRegs.h"
#include <Library/SysHostPointerLib.h>
#include <Library/KtiApi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CteNetLib.h>
#include <Library/PcuMailBoxRasLib.h>
#include <Library/CpuAndRevisionLib.h>

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cfgmcasmioncorr : 1;

                            /* Bits[0:0], Access Type=RW/P, default=None*/

                            /*
                               Configures SMI signaling for memory correctable
                               error (retry corrected or ECC/SDDC/DDDC
                               corrected) . 0: No SMI (default). 1: Enable SMI
                               signaling.
                            */
    UINT32 cfgmcacmcioncorr : 1;

                            /* Bits[1:1], Access Type=RW/P, default=None*/

                            /*
                               Configures CMCI signaling for memory correctable
                               error (retry corrected or ECC/SDDC/DDDC
                               corrected) . 0: No CMCI (default). 1: Enable
                               CMCI signaling.
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cfgmcasmioncorrcountthr : 1;

                            /* Bits[3:3], Access Type=RW/P, default=None*/

                            /*
                               Configures SMI signaling of correctable error
                               count reaching threshold. 0: No SMI (default).
                               1: Enable SMI signaling.
                            */
    UINT32 cfgmcacmcioncorrcountthr : 1;

                            /* Bits[4:4], Access Type=RW/P, default=None*/

                            /*
                               Configures CMCI signaling of correctable error
                               count reaching threshold. 0: No CMCI (default).
                               1: Enable CMCI signaling.
                            */
    UINT32 cfgmcasmionmirrorcorr : 1;

                            /* Bits[5:5], Access Type=RW/P, default=None*/

                            /*
                               Configures SMI signaling for channel mirror-
                               correctable error. 0: No SMI (default). 1:
                               Enable SMI signaling. A mirror-correctable error
                               is an error on a mirror channel that is
                               corrected by fetching data from the alternate
                               mirror channel followed by a succesful write
                               back to and reread from the erroneous channel.
                            */
    UINT32 cfgmcacmcionmirrorcorr : 1;

                            /* Bits[6:6], Access Type=RW/P, default=None*/

                            /*
                               Configures CMCI signaling for channel mirror-
                               correctable error. 0: No CMCI (default). 1:
                               Enable CMCI signaling. A mirror-correctable
                               error is an error on a mirror channel that is
                               corrected by fetching data from the alternate
                               mirror channel followed by a succesful write
                               back to and reread from the erroneous channel.
                            */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cfgmcasmionfailover : 1;

                            /* Bits[8:8], Access Type=RW/P, default=None*/

                            /*
                               Configures SMI signaling for mirror scrub reread
                               failure. 0: No SMI (default). 1: Enable SMI
                               signaling. Note, if
                               SysFeatures0[ImmediateFailoverActionEna] is set
                               then hardware will take care that a mirror scub
                               reread fail will result in a permanent channel
                               failover. Otherwise, it is BIOS responsibility
                               to interject permanent channel failover.
                            */
    UINT32 cfgmcacmcionfailover : 1;

                            /* Bits[9:9], Access Type=RW/P, default=None*/

                            /*
                               Configures CMCI signaling for mirror scrub
                               reread failure. 0: No CMCI (default). 1: Enable
                               CMCI signaling. Note, if
                               SysFeatures0[ImmediateFailoverActionEna] is set
                               then hardware will take care that a mirror scub
                               reread fail will result in a permanent channel
                               failover. Otherwise, it is BIOS responsibility
                               to interject permanent channel failover.
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cfgmcasmionrecov : 1;

                            /* Bits[11:11], Access Type=RW/P, default=None*/

                            /*
                               Configures SMI signaling for recoverable error.
                               0: No SMI (default). 1: Enable SMI signaling.
                            */
    UINT32 cfgmcacmcionrecov : 1;

                            /* Bits[12:12], Access Type=RW/P, default=None*/

                            /*
                               Recoverable error (UC=1,PCC=0) generates a CMCI
                               type interrupt. 0: No interrupt; 1: Enable
                               interrupt generation (default).
                            */
    UINT32 cfgmcacorrondatauc : 1;

                            /* Bits[13:13], Access Type=RW/P, default=None*/

                            /*
                               Correctable error(UC=0, PCC=0) for data uc
                               errors corrected by FM fill 0: Log them as UC=1,
                               PCC=0 1: Log them as UC=0, PCC=0 signaling
                               behavior is synonymous to logging
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mcabankmergedis : 1;

                            /* Bits[15:15], Access Type=RW/P, default=None*/

                            /*
                               Disable M2M MCA bank merging support: M2M
                               supports MCA bank merging for products that have
                               multiple instantiations of the memory controller
                               tile on the system and are required to merge
                               their machine check banks due to the limited MSR
                               space for MCA. 1: Disable MCA bank merging
                               support 0: Enable MCA bank merging (default)
                            */
    UINT32 mcabankidoverride : 1;

                            /* Bits[16:16], Access Type=RW/P, default=None*/

                            /*
                               Override this M2M MCA Bank ID: When set, this
                               M2M instance will override its MCA BankID and
                               BankIndex values with the values specified in
                               the EXRAS_CONFIG[McaBankId] and
                               EXRAS_CONFIG[McaBankIndex] registers. These
                               BankID/BankIndex values are used by SW to help
                               identify the source of an MCA and which Machine
                               Check banks in the system look for logged
                               errors. This override is only needed in case
                               there is a need to override the values provided
                               in hardware. 1: Use the override values for MCA
                               BankID and BankIndex 0: Use the values provided
                               by the hardware (default)
                            */
    UINT32 mcabankid : 5;

                            /* Bits[21:17], Access Type=RW/P, default=None*/

                            /*
                               MCA Bank ID: Specifies the MCA BankID value for
                               this instance of M2M. Only used when
                               EXRAS_CONFIG[McaBankIdOverride]=0x1.
                            */
    UINT32 mcabankindex : 5;

                            /* Bits[26:22], Access Type=RW/P, default=None*/

                            /*
                               MCA Bank Index: Specifies the MCA BankIndex
                               value for this instance of M2M. Only used when
                               EXRAS_CONFIG[McaBankIdOverride]=0x1.
                            */
    UINT32 rsvd_27 : 3;

                            /* Bits[29:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 corr2corroverwritedis : 1;

                            /* Bits[30:30], Access Type=RW/P, default=None*/

                            /*
                               Correctable-correctable Overwrite Disable
                               (Corr2CorrOverwriteDis): 0: Correctable
                               overwrites correctable in Machine Check bank and
                               shadow register. 1: First correctable logged in
                               Machine Check bank and also the shadow register
                               will not be overwritten by another correctable
                               error. (default) If this bit is set, the
                               intention is that (i) also iMC
                               retry_rd_err_log[NO_OVERRIDE] should be set so
                               that the iMC error logging of correctables in
                               retry_rd_err_log, is aligned with the M2M error
                               log, and (ii) also iMCs
                               dp_chkn_bit[ign_mca_ovrd] should be set so that
                               iMC error logging of correctables in the iMC
                               machine check banks is aligned with the M2M
                               error logging. It should be noted that the M2M
                               and iMC error logging can still get out of sync
                               in case of iMC patrol scrub (or iMC rank
                               sparing) errors because M2M has no observability
                               into these error types.
                            */
    UINT32 cfgmcalogmemcorr : 1;

                            /* Bits[31:31], Access Type=RW/P, default=None*/

                            /*
                               Enable logging and signaling of memory
                               correctable errors. A correctable error is an
                               error that is corrected either by retrying the
                               transaction or through ECC/SDDC/DDDC. 0: The
                               error is ignored. It is not logged nor signaled;
                               1: The error is passed on to the MM MCA bank
                               (and the shadow register) to be logged and
                               signaled if appropriate (default);
                            */

  } Bits;
  UINT32 Data;

} EXRAS_CONFIG_M2MEM_MAIN_STRUCT;

/**

  Get Mesh2Mem field Value for Mode register - NmCaching.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mModeNmCaching (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshCrMode;

  MeshCrMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  return (BOOLEAN)MeshCrMode.Bits.nmcaching;
}

/**

  Get Mesh2Mem field Value for SysFeatures0 - ImFailoverEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mSysFeatures0ImFailoverEn (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT   SysFeatures0;

  SysFeatures0.Data = UsraCsrRead ( Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);
  return (BOOLEAN)SysFeatures0.Bits.immediatefailoveractionena;
}


/**

  Get Mesh2Mem field Value TimeoutLock from register TimeOut.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: locked or FALSE: unlock

**/
BOOLEAN
EFIAPI
GetM2mTimeoutLock (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return FALSE;

}


/**

  Get Mesh2Mem field Value BadChRetry from register Defeatures0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: retry for bad channel or FALSE: not to retry

**/
BOOLEAN
EFIAPI
GetM2mDefeatures0BadChRetry (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return FALSE;
}


/**

  Get Mesh2Mem field Value ScrChkReadDis from register Defeatures0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: disabled or FALSE: enabled

**/
BOOLEAN
EFIAPI
GetM2mDefeatures0ScrChkReadDis (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  return (BOOLEAN)M2MemDefeatures0.Bits.scrubcheckrddis;
}


/**

  Get Mesh2Mem field Value egreco from Defeatures1

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mDefeatures1EgEcoMask (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
#ifndef GNRSRF_HOST
  DEFEATURES1_M2MEM_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT    M2MemDefeatures1A0B0HBM;
  DEFEATURES1_M2MEM_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT   M2MemDefeatures1C0;

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    M2MemDefeatures1A0B0HBM.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    return (UINT32)M2MemDefeatures1A0B0HBM.Bits.egreco;
  } else {
    M2MemDefeatures1C0.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    return (UINT32)M2MemDefeatures1C0.Bits.egreco;
  }
#else // !GNRSRF_HOST
  DEFEATURES1_M2MEM_MAIN_STRUCT                 M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
  return (UINT32)M2MemDefeatures1.Bits.egreco;
#endif // !GNRSRF_HOST
}



/**

  Get Mesh2Mem field Value akwrcmphyst from WrTrKrAlloc.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32  - high 16 bits: akwrcmphysthi
                     low  16 bits: akwrcmphystlo

**/
UINT32
EFIAPI
GetM2mWrTrKrAllocAkwrCmphyst (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  //
  // only NULL stub for ICX
  //

  return 0;
}


/**

  Get Mesh2Mem Register Value for Vid.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mVid (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, VID_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem Register Value for Did.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mDid (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, DID_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem Register Value for ExRasConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mExRasConfig (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  UINT32                              MailBoxCommand;
  UINT32                              MailboxData;
  UINT32                              MailboxStatus;

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                    | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);
  MailboxData = 0;

  MailboxStatus = Bios2PcodeMailBoxRead (Socket, MailBoxCommand, &MailboxData);

  if (MailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    return MailboxData;
  } else {
  return 0;
}
}

/**

  Get Mesh2Mem field Mirror from register TAD_WR_N0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: mirrored or FALSE: not mirror

**/
BOOLEAN
EFIAPI
GetM2mTadMirror (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  TAD_WR_N0_M2MEM_MAIN_STRUCT    MeshN0Tad;

  MeshN0Tad.Data = UsraCsrRead (Socket, McId, TAD_WR_N0_M2MEM_MAIN_REG);
  return (BOOLEAN) MeshN0Tad.Bits.mirror;
}

/**

  Get Mesh2Mem Register Value for ErrCountCtrl.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mErrCountCtrl (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, ERR_CNTR_CTL_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem Register Value for MirrorChDdr4.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch   - Channel ID

  @retval UINT8

**/
UINT8
EFIAPI
GetM2mMirrorChDdr4 (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Ch
  )
{
  MIRRORCHNLS_M2MEM_MAIN_STRUCT          MirrorChannels;
  UINT8                                  ChSec;

  ChSec = 0;
  MirrorChannels.Data = UsraCsrRead (Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG);
  if (Ch == 0)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddr4chnl0secondary;
  }
  if (Ch == 1)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddr4chnl1secondary;
  }
  if (Ch == 2)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddr4chnl2secondary;
  }
  return ChSec;
}

/**

  Get Mesh2Mem Register Value for MirrorChDdrt.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch   - Channel ID

  @retval UINT8

**/
UINT8
EFIAPI
GetM2mMirrorChDdrt (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Ch
  )
{
  MIRRORCHNLS_M2MEM_MAIN_STRUCT          MirrorChannels;
  UINT8                                  ChSec;

  ChSec = 0;
  MirrorChannels.Data = UsraCsrRead (Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG);
  if (Ch == 0)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddrtchnl0secondary;
  }
  if (Ch == 1)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddrtchnl1secondary;
  }
  if (Ch == 2)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddrtchnl2secondary;
  }
  return ChSec;
}

/**

  Get Mesh2Mem Register Value for MirrorFailOverDdr4Failed.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMirrorFailOverDdr4Failed (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  return (BOOLEAN)MirrorFailover.Bits.ddr4chnlfailed;
}

/**

  Get Mesh2Mem Register Value for MirrorFailOver.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
UINT32
GetM2mMirrorFailOver (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  return MirrorFailover.Data;
}

/**

  Get Mesh2Mem Register Value for MirrorFailOverDdrtFailed.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMirrorFailOverDdrtFailed (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  return (BOOLEAN)MirrorFailover.Bits.ddrtchnlfailed;
}

/**

  Get Mesh2Mem Register Value for MciStatusShadowN0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciStatusShadowN0 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem field Value for McaCod from reg MciStatusShadowN0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciStatusShadowN0McaCod (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_STATUS_SHADOW_N0_M2MEM_MAIN_STRUCT   MciStatusShadow0;

  MciStatusShadow0.Data = UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG);
  return (UINT32)MciStatusShadow0.Bits.mcacod;
}

/**

  Get Mesh2Mem Register Value for MciStatusShadowN1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciStatusShadowN1 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
}

BOOLEAN
EFIAPI
GetM2mMciStatusShadowN1Valid (
  UINT8               Socket,
  UINT8               McId
  )
{
  MCI_STATUS_SHADOW_N1_M2MEM_MAIN_STRUCT   MciStatusShadow1;

  MciStatusShadow1.Data = UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciStatusShadow1.Bits.valid;
}

/**

  Get Mesh2Mem Register Value for MciStatusShadowN1AddValid.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMciStatusShadowN1AddValid (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_STATUS_SHADOW_N1_M2MEM_MAIN_STRUCT   MciStatusShadow1;

  MciStatusShadow1.Data = UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciStatusShadow1.Bits.addrv;
}

/**

  Get Mesh2Mem Register Value for MciAddrShadow0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciAddrShadow0 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_ADDR_SHADOW0_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem Register Value for MciAddrShadow1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciAddrShadow1 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_ADDR_SHADOW1_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem Register Value for MciMiscShadowN0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciMiscShadowN0 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N0_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem Register Value for MciMiscShadowN1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciMiscShadowN1 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem Register Value for MciMiscShadowN1MirrFailover.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMciMiscShadowN1MirrFailover (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_MISC_SHADOW_N1_M2MEM_MAIN_STRUCT MciMiscShadow1;

  MciMiscShadow1.Data = UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciMiscShadow1.Bits.mirrorfailover;
}

/**

  Get Mesh2Mem Register Value for MciMiscShadowN1McCmdVld.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMciMiscShadowN1McCmdVld (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_MISC_SHADOW_N1_M2MEM_MAIN_STRUCT MciMiscShadow1;

  MciMiscShadow1.Data = UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciMiscShadow1.Bits.mccmdvld;
}

/**

  Get Mesh2Mem Register Value for MciMiscShadowN1McCmdMemRegion.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciMiscShadowN1McCmdMemRegion (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_MISC_SHADOW_N1_M2MEM_MAIN_STRUCT MciMiscShadow1;

  MciMiscShadow1.Data = UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
  return (UINT32)MciMiscShadow1.Bits.mccmdmemregion;
}

/**

  Get Mesh2Mem Register bus dev fun in high bits 31~12, the lower 12bit offsets (bit 0~11) will be 0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mCsrBusDevFunAddress (
  IN UINT8    Socket,
  IN UINT8    McId
  )
{
  return ((UINT32)UsraGetCsrRegisterAddress (Socket, McId, VID_M2MEM_MAIN_REG) & 0xFFFF000);
}

/**

  SetM2mSysFeatures0ForMirror.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Pcommitforcebroadcast   - Pcommitforcebroadcast
  @param Immediatefailoveractionena   - Immediatefailoveractionena

  @retval N/A

**/
VOID
EFIAPI
SetM2mSysFeatures0ForMirror (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Pcommitforcebroadcast,
  IN BOOLEAN             Immediatefailoveractionena
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT   SysFeatures0;

  SysFeatures0.Data = UsraCsrRead ( Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);

  SysFeatures0.Bits.immediatefailoveractionena = Immediatefailoveractionena;
  UsraCsrWrite (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);

}

/**

  SetM2mModeMirrorType.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param MirrorDdr4   - MirrorDdr4
  @param MirrorDdrt   - MirrorDdrt

  @retval N/A

**/
VOID
EFIAPI
SetM2mModeMirrorType (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             MirrorDdr4,
  IN BOOLEAN             MirrorDdrt
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshMode;

  MeshMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  MeshMode.Bits.mirrorddr4 = MirrorDdr4;
  MeshMode.Bits.mirrorddrt = MirrorDdrt;
  UsraCsrWrite (Socket, McId, MODE_M2MEM_MAIN_REG, MeshMode.Data);
}

/**

  SetM2mModeAdddc.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Adddc   - Adddc

  @retval N/A

**/
VOID
EFIAPI
SetM2mModeAdddc (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Adddc
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshMode;

  MeshMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  MeshMode.Bits.adddc = Adddc;
  UsraCsrWrite (Socket, McId, MODE_M2MEM_MAIN_REG, MeshMode.Data);
}

/**

  SetM2mTimeoutLock.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param TimeoutLock   - TimeoutLock

  @retval N/A

**/
VOID
EFIAPI
SetM2mTimeoutLock (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             TimeoutLock
  )
{
  //
  // only NULL stub for ICX
  //
  return;
}

/**

  SetM2mDefeatures0MirrorMode.

  @param[in] Socket   - Socket ID
  @param[in] McId     - Mc Controller Id
  @param[in] Wait4BothHalves, bit field value
  @param[in] EgressBypassDis, bit field value
  @param[in] IngressBypassDis,
  @param[in] DemandScrubDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0MirrorMode (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT32   Wait4BothHalves,
  IN UINT32   EgrBypassDis,
  IN UINT32   IngBypassDis,
  IN UINT32   DemandScrubWrDis
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);

  if (Wait4BothHalves != 0xFF) {
    M2MemDefeatures0.Bits.wait4bothhalves    = Wait4BothHalves;
  }
  if (EgrBypassDis != 0xFF) {
    M2MemDefeatures0.Bits.egrbypdis          = EgrBypassDis;
  }
  if (IngBypassDis != 0xFF) {
    M2MemDefeatures0.Bits.ingbypdis         = IngBypassDis;
  }
  if (DemandScrubWrDis != 0xFF) {
    M2MemDefeatures0.Bits.demandscrubwrdis   = DemandScrubWrDis;
  }

  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}

/**
  Bit field not applicable for ICX
**/
VOID
EFIAPI
SetM2MDefeatures0BadChnlFirst (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT32   BadChnlFirst
  )
{
  return;
}

/**

  SetM2mDefeatures0BadChRetry.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param BadChRetryFirst   - BadChRetryFirst

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0BadChRetry (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             BadChRetryFirst
  )
{
  //
  // only NULL stub for ICX
  //

  return;
}

/**

  SetM2mDefeatures0Wait4BothHalves.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Wait4BothHalves   - Wait4BothHalves

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0Wait4BothHalves (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Wait4BothHalves
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.wait4bothhalves = Wait4BothHalves;
  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}


/**

  SetM2mDefeatures0ScrChkReadDis.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param ScrubChkReadDis   - ScrubChkReadDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0ScrChkReadDis (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             ScrubChkReadDis
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.scrubcheckrddis = ScrubChkReadDis;
  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}

/**

  SetM2mDefeatures0EgrBypDis.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param EgrBypDis   - EgrBypDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0EgrBypDis (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             EgrBypDis
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.egrbypdis = EgrBypDis;
  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}

/**

  SetM2mDefeatures1MirrorMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param DdrtMirrScrubWrDis   - DdrtMirrScrubWrDis
  @param SkipBadChnlForRd   - SkipBadChnlForRd
  @param SkipBadChnlForPwr   - SkipBadChnlForPwr
  @param EgEcoMask   - EgEcoMask

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1MirrorMode (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             DdrtMirrScrubWrDis,
  IN BOOLEAN             SkipBadChnlForRd,
  IN BOOLEAN             SkipBadChnlForPwr,
  IN UINT32              EgEcoMask
  )
{
#ifndef GNRSRF_HOST
  DEFEATURES1_M2MEM_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT    M2MemDefeatures1A0B0HBM;
  DEFEATURES1_M2MEM_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT   M2MemDefeatures1C0;

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    M2MemDefeatures1A0B0HBM.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    M2MemDefeatures1A0B0HBM.Bits.ddrtmirrscrubwrdis = DdrtMirrScrubWrDis;
    M2MemDefeatures1A0B0HBM.Bits.skipbadchnlforrd   = SkipBadChnlForRd;
    M2MemDefeatures1A0B0HBM.Bits.egreco             = EgEcoMask;
    UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1A0B0HBM.Data);
  } else {
    M2MemDefeatures1C0.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    M2MemDefeatures1C0.Bits.ddrtmirrscrubwrdis = DdrtMirrScrubWrDis;
    M2MemDefeatures1C0.Bits.skipbadchnlforrd   = SkipBadChnlForRd;
    M2MemDefeatures1C0.Bits.egreco             = EgEcoMask;
    UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1C0.Data);
  }
#else // !GNRSRF_HOST
  DEFEATURES1_M2MEM_MAIN_STRUCT                 M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
  M2MemDefeatures1.Bits.ddrtmirrscrubwrdis = DdrtMirrScrubWrDis;
  M2MemDefeatures1.Bits.egreco             = EgEcoMask;
  UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1.Data);
#endif // !GNRSRF_HOST
}

/**

  SetM2mDefeatures1Cfgfailover.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Scrubfailover   - Scrubfailover

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1Cfgfailover (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Scrubfailover
  )
{
  DEFEATURES1_M2MEM_MAIN_STRUCT   M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
  M2MemDefeatures1.Bits.cfgfailoveronscrubchkrdcorr = Scrubfailover;
  UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1.Data);
}

/**

  SetM2mWrTrKrAllocMirrorMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param *Mode   - *Mode

  @retval N/A

**/
VOID
EFIAPI
SetM2mWrTrKrAllocMirrorMode (
  IN UINT8                         Socket,
  IN UINT8                         McId,
  IN WRTRKRALLOC_M2MEM_MIRROR_MODE *Mode
  )
{
#ifndef GNRSRF_HOST
  //
  // To check if the following register has moved or obsolete for GNR/SRF
  //
  WRTRKRALLOC_M2MEM_MAIN_STRUCT WrTrKrAlloc;

  WrTrKrAlloc.Data = UsraCsrRead (Socket, McId,  WRTRKRALLOC_M2MEM_MAIN_REG);
  WrTrKrAlloc.Bits.mirrtrkrpwrthresh     = Mode->MirrTrkrPwrThresh;
  WrTrKrAlloc.Bits.mirrtrkrthresh        = Mode->MirrTrkrThresh;
  WrTrKrAlloc.Bits.mirrtrkrfaketgrthresh = Mode->MirrTrkrFakeTgrThresh;
  UsraCsrWrite (Socket, McId, WRTRKRALLOC_M2MEM_MAIN_REG, WrTrKrAlloc.Data);
#endif // !GNRSRF_HOST
}

/**

  SetM2mTadMirror.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Mirror   - Mirror

  @retval N/A

**/
VOID
EFIAPI
SetM2mTadMirror (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Mirror
  )
{
  TAD_WR_N0_M2MEM_MAIN_STRUCT    MeshN0Tad;

  MeshN0Tad.Data = UsraCsrRead (Socket, McId, TAD_WR_N0_M2MEM_MAIN_REG);
  MeshN0Tad.Bits.mirror = Mirror;
  UsraCsrWrite (Socket, McId, TAD_WR_N0_M2MEM_MAIN_REG, MeshN0Tad.Data);
}

/**

  SetM2mBiosQuiesceRtTimer.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param TrainRtTimer   - TrainRtTimer

  @retval N/A

**/
VOID
EFIAPI
SetM2mBiosQuiesceRtTimer (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              TrainRtTimer
  )
{
  M2MBIOSQUIESCE_M2MEM_MAIN_STRUCT        M2mBiosQuiesce;
  M2mBiosQuiesce.Data = UsraCsrRead (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG);
  M2mBiosQuiesce.Bits.drainrttimer =  TrainRtTimer;
  UsraCsrWrite (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG, M2mBiosQuiesce.Data);
}

/**

  SetM2mBiosQuiesceusecmiidlefordrain.

  @param[in] Socket               - Socket ID
  @param[in] McId                 - Mc Controller Id
  @param[in] UseCmiIdleForDrain   - UseCmiIdleForDrain value to program

  @retval N/A

**/
VOID
EFIAPI
SetM2mBiosQuiesceUseCmiIdleForDrain (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              UseCmiIdleForDrain
  )
{
  M2MBIOSQUIESCE_M2MEM_MAIN_STRUCT        M2mBiosQuiesce;
  M2mBiosQuiesce.Data = UsraCsrRead (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG);
  M2mBiosQuiesce.Bits.usecmiidlefordrain =  UseCmiIdleForDrain;
  UsraCsrWrite (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG, M2mBiosQuiesce.Data);
}

/**

  SetM2mExRasConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param RegValue   - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfig (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              RegValue
  )
{

  UINT32                              MailBoxCommand;
  UINT32                              MailboxData;
  UINT32  MailboxStatus;

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                    | MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_READ_WRITE_MASK
                    | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);
  MailboxData = RegValue;

  MailboxStatus = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, MailboxData);

}

/**

  SetM2mExRasConfigMcaSmiOnCorr.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CfgMcaSmiOnCorr   - CfgMcaSmiOnCorr

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigMcaSmiOnCorr (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CfgMcaSmiOnCorr
  )
{
  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;
  UINT32                              MailBoxCommand;
  UINT32                              MailboxStatus;

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                    | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);
  ExRasCfg.Data = 0;

  MailboxStatus = Bios2PcodeMailBoxRead (Socket, MailBoxCommand, &ExRasCfg.Data);

  if (MailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    ExRasCfg.Bits.cfgmcasmioncorr = CfgMcaSmiOnCorr;
  } else {
    return;
  }

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                  | MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_READ_WRITE_MASK
                  | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);

  MailboxStatus = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, ExRasCfg.Data);

}

/**

  SetM2mExRasConfigSmiEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param *Enables   - *Enables

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigSmiEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN EXRAS_CONFIG_SMI_EN *Enables
  )
{
  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;
  UINT32                              MailBoxCommand;
  UINT32                              MailboxStatus;

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                    | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);
  ExRasCfg.Data = 0;

  MailboxStatus = Bios2PcodeMailBoxRead (Socket, MailBoxCommand, &ExRasCfg.Data);

  if (MailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    ExRasCfg.Bits.cfgmcacmcionfailover     = Enables->CfgMcaCmciOnFailover;
    ExRasCfg.Bits.cfgmcacmcionmirrorcorr   = Enables->CfgMcaCmciOnMirrorcorr;
    ExRasCfg.Bits.cfgmcasmionmirrorcorr    = Enables->CfgMcaSmiOnMirrorcorr;
    ExRasCfg.Bits.cfgmcacmcioncorrcountthr = Enables->CfgMcaCmciOnCorrCountthr;
    ExRasCfg.Bits.cfgmcasmioncorrcountthr  = Enables->CfgMcaSmiOnCorrCountthr;
    ExRasCfg.Bits.cfgmcacmcioncorr         = Enables->CfgMcaCmciOnCorr;
    ExRasCfg.Bits.cfgmcasmioncorr          = Enables->CfgMcaSmiOnCorr;
    ExRasCfg.Bits.cfgmcacmcionrecov        = Enables->CfgMcaCmciOnRecov;
    ExRasCfg.Bits.cfgmcasmionrecov         = Enables->CfgMcaSmiOnRecov;
  } else {
    return;
  }

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                  | MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_READ_WRITE_MASK
                  | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);

  MailboxStatus = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, ExRasCfg.Data);
}

/**

  SetM2mExRasConfigFailOverAct.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CfgMcaCmciOnFailover   - CfgMcaCmciOnFailover
  @param CfgMcaSmiOnFailover   - CfgMcaSmiOnFailover

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigFailOverAct (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CfgMcaCmciOnFailover,
  IN BOOLEAN             CfgMcaSmiOnFailover
  )
{
  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;
  UINT32                              MailBoxCommand;
  UINT32                              MailboxStatus;

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                    | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);
  ExRasCfg.Data = 0;

  MailboxStatus = Bios2PcodeMailBoxRead (Socket, MailBoxCommand, &ExRasCfg.Data);

  if (MailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    ExRasCfg.Bits.cfgmcacmcionfailover     = CfgMcaCmciOnFailover;
    ExRasCfg.Bits.cfgmcasmionfailover      = CfgMcaSmiOnFailover;
  } else {
    return;
  }

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                  | MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_READ_WRITE_MASK
                  | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);

  MailboxStatus = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, ExRasCfg.Data);
}

/**

  SetM2mExRasConfigMcaLogMemCorr.

  @param Socket          - Socket ID
  @param McId            - Mc Controller Id
  @param McaLogMemCorr   - McaLogMemCorr

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigMcaLogMemCorr (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               McaLogMemCorr
  )
{
  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;
  UINT32                              MailBoxCommand;
  UINT32                              MailboxStatus;

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                    | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);
  ExRasCfg.Data = 0;

  MailboxStatus = Bios2PcodeMailBoxRead (Socket, MailBoxCommand, &ExRasCfg.Data);

  if (MailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    ExRasCfg.Bits.cfgmcalogmemcorr = McaLogMemCorr;
  } else {
    return;
  }

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                  | MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_READ_WRITE_MASK
                  | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);

  MailboxStatus = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, ExRasCfg.Data);
}

/**

  SetM2mExRasConfigMcaCorrOnDataUc

  @param Socket          - Socket ID
  @param McId            - Mc Controller Id
  @param McaLogMemCorr   - McaCorrOnDataUc

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigMcaCorrOnDataUc (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               McaCorrOnDataUc
  )
{
  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;
  UINT32                              MailBoxCommand;
  UINT32                              MailboxStatus;

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                    | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);
  ExRasCfg.Data = 0;

  MailboxStatus = Bios2PcodeMailBoxRead (Socket, MailBoxCommand, &ExRasCfg.Data);

  if (MailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    ExRasCfg.Bits.cfgmcacorrondatauc = McaCorrOnDataUc;
  } else {
    return;
  }

  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK
                  | MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_READ_WRITE_MASK
                  | (McId << MAILBOX_BIOS_CMD_CONFIGURE_M2MEM_M2M_MCA_BANK_INTERFACE_INSTANCE_LSB);

  MailboxStatus = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, ExRasCfg.Data);
}


/**

  SetM2mMirrorChs.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ddr4ChnlxSec   - Ddr4ChnlxSec // [0:7] ch0; [8:15] ch1; [16:23] ch2; [24: 31] rsv.
  @param DdrtChnlxSec   - DdrtChnlxSec // [0:7] ch0; [8:15] ch1; [16:23] ch2; [24: 31] rsv.

  @retval N/A

**/
VOID
EFIAPI
SetM2mMirrorChs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Ddr4ChnlxSec,
  IN UINT32              DdrtChnlxSec
  )
{
  MIRRORCHNLS_M2MEM_MAIN_STRUCT          MirrorChannels;


  MirrorChannels.Data = UsraCsrRead (Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG);

  MirrorChannels.Bits.ddr4chnl0secondary = (Ddr4ChnlxSec & CH0_SEC_MASK) >> CH0_SHIFT_COUNT;
  MirrorChannels.Bits.ddr4chnl1secondary = (Ddr4ChnlxSec & CH1_SEC_MASK) >> CH1_SHIFT_COUNT;
  MirrorChannels.Bits.ddr4chnl2secondary = (Ddr4ChnlxSec & CH2_SEC_MASK) >> CH2_SHIFT_COUNT;;
  MirrorChannels.Bits.ddrtchnl0secondary = (DdrtChnlxSec & CH0_SEC_MASK) >> CH0_SHIFT_COUNT;;
  MirrorChannels.Bits.ddrtchnl1secondary = (DdrtChnlxSec & CH1_SEC_MASK) >> CH1_SHIFT_COUNT;;
  MirrorChannels.Bits.ddrtchnl2secondary = (DdrtChnlxSec & CH2_SEC_MASK) >> CH2_SHIFT_COUNT;;

  UsraCsrWrite(Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG, MirrorChannels.Data);
}

/**

  SetM2mErrCountCtrl.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CntEna   - CntEna
  @param ClearCount   - ClearCount
  @param ThresholdWr   - ThresholdWr
  @param CountIndex   - CountIndex
  @param ThresholdIndex   - ThresholdIndex

  @retval N/A

**/
VOID
EFIAPI
SetM2mErrCountCtrl (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CntEna,
  IN BOOLEAN             ClearCount,
  IN BOOLEAN             ThresholdWr,
  IN UINT32              CountIndex,
  IN UINT32              ThresholdIndex
  )
{
  ERR_CNTR_CTL_M2MEM_MAIN_STRUCT ErrCtrlM2MCtl;

  ErrCtrlM2MCtl.Data = UsraCsrRead (Socket, McId, ERR_CNTR_CTL_M2MEM_MAIN_REG);
  ErrCtrlM2MCtl.Bits.cntena       = CntEna; // Enable counting by all the M2M internal error counters
  ErrCtrlM2MCtl.Bits.clrcntrs     = ClearCount; // Clear all the M2M internal error counters
  ErrCtrlM2MCtl.Bits.cntridx      = CountIndex;
  ErrCtrlM2MCtl.Bits.thresholdidx = ThresholdIndex;
  ErrCtrlM2MCtl.Bits.thresholdwr  = ThresholdWr; // Indicate that we want to trigger because of MIRRSCRUBRD_ERR_TYPE
  UsraCsrWrite (Socket, McId, ERR_CNTR_CTL_M2MEM_MAIN_REG, ErrCtrlM2MCtl.Data);
}

/**

  SetM2mMirrorFailOver.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ddr4ChnlFailed   - Ddr4ChnlFailed
  @param DdrtChnlFailed   - DdrtChnlFailed

  @retval N/A

**/
VOID
EFIAPI
SetM2mMirrorFailOver (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Ddr4ChnlFailed,
  IN UINT32              DdrtChnlFailed
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  MirrorFailover.Bits.ddr4chnlfailed = Ddr4ChnlFailed;
  MirrorFailover.Bits.ddrtchnlfailed = DdrtChnlFailed;
  UsraCsrWrite (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG, MirrorFailover.Data);
}

/**

  SetM2mMciStatusShadowN0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param RegValue - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetM2mMciStatusShadowN0 (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              RegValue
  )
{
  UsraCsrWrite(Socket, McId, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG, RegValue);
}

/**

  SetM2mMciStatusShadowN1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param RegValue  - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetM2mMciStatusShadowN1 (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              RegValue
  )
{
  UsraCsrWrite(Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG, RegValue);
}

/**

  SetM2mClearShadowRegs.

  @param Socket   - Socket ID
  @param Mc       - Mc

  @retval N/A

**/
VOID
EFIAPI
SetM2mClearShadowRegs (
   IN UINT8 Socket,
   IN UINT8 Mc
  )
{
  UsraCsrWrite (Socket, Mc, MCI_ADDR_SHADOW0_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_ADDR_SHADOW1_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_MISC_SHADOW_N0_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG, 0);
}

/**

  SetM2mExRasConfigCorr2CorroverwriteDis

  @param Socket                  - Socket ID
  @param McId                    - Mc Controller Id
  @param Corr2CorroverwriteDis   - Corr2CorroverwriteDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigCorr2CorroverwriteDis (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Corr2CorroverwriteDis
  )
{
  return;
}