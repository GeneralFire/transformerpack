/** @file
  Interface source file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include "Include/HbmIoIpInternal.h"
/**

  Get read and write latencies

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[out]               ReadLatency         Pointer to return read latency Value
  @param[out]               WriteLatency        Pointer to return write latency Value

  @retval                   EFI_SUCCESS             Function is able to retrieve data and populate pointers
  @retval                   EFI_INVALID_PARAMETER   If any output pointer is null

**/
EFI_STATUS
EFIAPI
GetReadWriteLatency (
  IN    UINT8       Socket,
  IN    UINT8       Channel,
  OUT   UINT16      *ReadLatency,
  OUT   UINT16      *WriteLatency
  )
{
#if 0 //SPR HBM TODO HSD   14010087379
  UINT32  OffsetPerInstance;
  AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_STRUCT    RdTimingCtrl;
  AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_STRUCT   WrTimingCtrl;

  if (ReadLatency == NULL || WriteLatency == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OffsetPerInstance = UpdateHbmIoRegister (Socket, Channel, 0, AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_REG);
  RdTimingCtrl.Data = MemReadPciCfgEp (Socket, Channel, OffsetPerInstance);

  OffsetPerInstance = UpdateHbmIoRegister (Socket, Channel, 0, AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_REG);
  WrTimingCtrl.Data = MemReadPciCfgEp (Socket, Channel, OffsetPerInstance);

  *ReadLatency  = (UINT16) RdTimingCtrl.Bits.rx_fifo_rd_en_dly;
  *WriteLatency = (UINT16) WrTimingCtrl.Bits.wr_dq_out_en_dly;
#endif

  *ReadLatency  = 0;
  *WriteLatency = 0;
  return EFI_SUCCESS;
}

/**

  Update Latency timings in HBM IO Phy.

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
HbmUpdateLatency (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  )
{

#if 0 //SPR HBM TODO HSD   14010087379
  UINT8                                         ChannelStart;
  UINT8                                         ChannelEnd;
  UINT8                                         ChannelIndex;
  UINT8                                         ReadLatency;
  UINT8                                         WriteLatency;
  UINT8                                         ParityLatency;
  UINT8                                         SaOnDuration;
  UINT8                                         RxSaEnDelay;
  UINT8                                         TxFifoWr;
  UINT32                                        Offset;
  HBM_MR2_STRUCT                                Mr2;
  HBM_MR4_STRUCT                                Mr4;
  AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_STRUCT  ReadTimingCtrl;
  AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_STRUCT WriteTimingCtrl;

  ChannelStart  = IoModule * HbmGetMaxChannelMemSs (Socket, IoModule);
  ChannelEnd    = ChannelStart + HbmGetMaxChannelMemSs (Socket, IoModule);

  for (ChannelIndex = ChannelStart; ChannelIndex < ChannelEnd; ChannelIndex++) {

    Offset = UpdateHbmIoRegister (
               Socket,
               ChannelIndex,
               0,
               AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_REG
               );
    ReadTimingCtrl.Data = MemReadPciCfgEp (
                            Socket,
                            ChannelIndex,
                            Offset
                            );

    Offset = UpdateHbmIoRegister (
               Socket,
               ChannelIndex,
               0,
               AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_REG
               );
    WriteTimingCtrl.Data = MemReadPciCfgEp (
                             Socket,
                             ChannelIndex,
                             Offset
                             );
    //
    // Check if RD/WR/PAR Latency needs to be updated
    //
    Mr2.Data = GetHbmCacheMr (Socket, IoModule, HbmCacheMr02);
    Mr4.Data = GetHbmCacheMr (Socket, IoModule, HbmCacheMr04);

    ReadLatency   = Mr2.Bits.rd_lat;
    WriteLatency  = Mr2.Bits.wr_lat;
    ParityLatency = Mr4.Bits.par_lat;

    //
    // Convert latency from MRS to nCK
    // For Parity latency, MRS Value is the same as nCK Value
    //
    ReadLatency   = HBM_RD_LATENCY_MRS2NCK (ReadLatency);
    WriteLatency  = HBM_WR_LATENCY_MRS2NCK (WriteLatency);

    if (ReadLatency < HBM_MIN_READ_LATENCY_NCK) {
      HbmDebugPrintDevice (
        HBM_SDBG_MINMAX,
        Socket,
        ChannelIndex,
        NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "M%d: WARNING UNSUPPORTED HBM Read Latency setting.  Current=%dnCK  New=%dnCK\n",
        IoModule,
        ReadLatency,
        (INT8)(ReadLatency - HBM_MIN_READ_LATENCY_NCK)
        );
      ReadLatency = HBM_MIN_READ_LATENCY_NCK;
      Mr2.Bits.rd_lat = HBM_RD_LATENCY_NCK2MRS (HBM_MIN_READ_LATENCY_NCK);
      SetHbmCacheMr (Socket, IoModule, HbmCacheMr02, Mr2.Data);
    }

    if (WriteLatency < HBM_MIN_WRITE_LATENCY_NCK) {
      HbmDebugPrintDevice (
        HBM_SDBG_MINMAX,
        Socket,
        ChannelIndex,
        NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "M%d: WARNING UNSUPPORTED HBM Write Latency setting.  Current=%dnCK  New=%dnCK\n",
        IoModule,
        WriteLatency,
        (INT8)(WriteLatency - HBM_MIN_WRITE_LATENCY_NCK)
        );
      WriteLatency = HBM_MIN_WRITE_LATENCY_NCK;
      Mr2.Bits.wr_lat = HBM_WR_LATENCY_NCK2MRS (HBM_MIN_WRITE_LATENCY_NCK);
      SetHbmCacheMr (Socket, IoModule, HbmCacheMr02, Mr2.Data);
    }

    GetChipIoLatency (ReadLatency, WriteLatency, &SaOnDuration, &RxSaEnDelay, &TxFifoWr);

    ReadTimingCtrl.Bits.sa_on_duration     = SaOnDuration;
    ReadTimingCtrl.Bits.rx_sa_en_dly       = RxSaEnDelay;

    ReadTimingCtrl.Bits.rdqs_en_duration   = 8;
    ReadTimingCtrl.Bits.rdqs_en_dly        = ReadLatency - 3;
    ReadTimingCtrl.Bits.rx_fifo_rd_en_dly  = ReadLatency + 3;
    ReadTimingCtrl.Bits.rd_data_valid_dly  = ReadTimingCtrl.Bits.rx_fifo_rd_en_dly - RT_DELAY_CONST;
    //
    // Broadcast new latency to All channels All Registers in the stack
    //
    Offset = UpdateHbmIoRegister (
               Socket,
               ChannelIndex,
               0,
               AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_REG
               );
    MemWritePciCfgEp (
      Socket,
      ChannelIndex,
      Offset,
      ReadTimingCtrl.Data
      );

    WriteTimingCtrl.Bits.tx_fifo_wr_en_dly = TxFifoWr;
    WriteTimingCtrl.Bits.wr_dq_out_en_dly  = WriteLatency - 4;
    WriteTimingCtrl.Bits.wr_dqs_en_dly     = WriteLatency - 4;
    WriteTimingCtrl.Bits.dq_parity_latency = ParityLatency;

    Offset = UpdateHbmIoRegister (
               Socket,
               ChannelIndex,
               0,
               AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_REG
               );

    MemWritePciCfgEp (
      Socket,
      ChannelIndex,
      Offset,
      WriteTimingCtrl.Data
      );
    HbmDebugPrintDevice (
      HBM_SDBG_MINMAX, Socket, ChannelIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RL=%d WL=%d HCLKs\n", ReadLatency, WriteLatency
      ));

    HbmDebugPrintDevice ((
      HBM_SDBG_MINMAX, Socket, ChannelIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "sa_on_duration=%d rx_sa_en_dly=%d rx_fifo_rd_en_dly=%d rd_data_valid_dly=%d \
      \nWriteTimingCtrl.Bits.dq_parity_latency=%d tx_fifo_wr_en_dly=%d wr_dq_out_en_dly=%d wr_dqs_en_dly=%d\n",
      ReadTimingCtrl.Bits.sa_on_duration,
      ReadTimingCtrl.Bits.rx_sa_en_dly,
      ReadTimingCtrl.Bits.rx_fifo_rd_en_dly,
      ReadTimingCtrl.Bits.rd_data_valid_dly,
      WriteTimingCtrl.Bits.dq_parity_latency,
      WriteTimingCtrl.Bits.tx_fifo_wr_en_dly,
      WriteTimingCtrl.Bits.wr_dq_out_en_dly,
      WriteTimingCtrl.Bits.wr_dqs_en_dly
      ));

  }
  #endif
} // HbmUpdateLatency
