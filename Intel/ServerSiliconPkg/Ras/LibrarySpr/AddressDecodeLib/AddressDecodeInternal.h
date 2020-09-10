/** @file
  provide address decode internal definition.

@copyright
INTEL CONFIDENTIAL
Copyright (c) 2006 - 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef  __ADDRESS_DECODE_INTERNAL_H__
#define  __ADDRESS_DECODE_INTERNAL_H__

#include <Uefi/UefiBaseType.h>

#define  MAX_CLUSTER           4
#define  MAX_LOCAL_DRAM_RULE   16
#define  MAX_REMOTE_DRAM_RULE  21
#define  DRAM_SCOPE_NUMBER     8
#define  MAX_TARGET            8
#define  MAX_ROUTE_TABLE       8

#define  SAD_UNIT              26


#define  INT_ONE_WAY           1
#define  INT_TWO_WAY           2
#define  INT_THREE_WAY         3
#define  INT_FOUR_WAY          4
#define  INT_SIX_WAY           6
#define  INT_EIGHT_WAY         8



#define  MAX_SYS_IMC           20
#define  MAX_SAD2TAD           20
#define  MAX_CH_PER_MC         2
#define  MAX_TAD               12

#define  MAX_MC_DRAM_RULE      8


#define  MAX_RIR               4
#define  MAX_ILV               8
#define  RIR_UINT              29
#define  CLOSE_PG              6
#define  OPEN_PG               13

#define  MAX_DDRT_RIR          4
#define  MAX_DDRT_ILV          2

#define  X4_WIDTH              0
#define  X8_WIDTH              1
#define  X16_WIDTH             2
#define  Density_8Gb           3

#define  MAX_DIMM_NUM          2


#pragma pack(1)

typedef struct {
  UINT8     ValidFlag;      // 1 -- valid
  UINT8     InclusiveFlag;  // 1 -- include   0 -- exclude
  UINT64    DramBase;
  UINT64    DramLimit;
} DRAM_SCOPE;

typedef struct {
  BOOLEAN   LocalDramRuleValid;   //local
  UINT64    StartAddress;
  UINT64    EndAddress;
  UINT8     Socket;
  UINT8     InterleaveMode;
  UINT8     NmCachable; // indicate 2LM tad decode.
  UINT8     CxlAcceleratorMem;
  UINT8     CxlMemExpander;
  UINT8     NxmFlag;
  UINT8     RouteTableType; // 2 bits.
  UINT8     ChannelTabletype;
  UINT8     McInterleaveShift; // 3 bits
  UINT8     ChInterleaveShift;
  UINT8     InterleaveWays[MAX_ROUTE_TABLE];
  UINT8     TargetTable[MAX_TARGET];
} CHA_ARRAY;

typedef struct {
  CHA_ARRAY   DramRule[MAX_LOCAL_DRAM_RULE];
  UINT8       McTable[MAX_ROUTE_TABLE][MAX_TARGET];
  UINT8       ChTable[MAX_ROUTE_TABLE][MAX_TARGET];
}CLUSTER_INFO;


typedef struct {
  BOOLEAN  RemoteDramRuleValid;
  UINT64   StartAddress;
  UINT64   EndAddress;
  UINT8    Socket;
}REMOTE_DRAM_RULE;


typedef struct {
  BOOLEAN  Sad2TadValid;
  UINT64   Base;
  UINT64   Limit;
  UINT8    ChTad;
  UINT8    Ddr4Flag;
  UINT8    NmCacheableVld;
  UINT8    BlkVld;
  UINT8    PmemVld;
  BOOLEAN  Mirror;
}SAD2TAD;

typedef struct {
  UINT8    TadValid;
  UINT8    TargetWay;
  UINT8    TargetGran;
  UINT8    ChWay;
  UINT8    ChGran;
  UINT64   TadOffset;
  UINT8    TadSign;
}TADINFO;


typedef struct {
 UINT8     McDramRuleValid;
 UINT64    FmBase;
 UINT64    FmLimit;
 UINT8     FmTarget;
 UINT8     FmTargetWay;
 UINT8     FmChnWay;
 UINT8     FmChnGran;
 UINT32    FmChnL2Pid;
}CHSADINFO;

typedef struct {
  UINT8   RirIlvValid;
  UINT64  RirOffset;
  UINT8   RankId;
}RIRILVINFO;

typedef struct {
  UINT8    RirValid;
  UINT64   RirBase;
  UINT64   RirLimit;
  UINT8    RirWay;
  RIRILVINFO  RirIlv[MAX_ILV];
}RIRINFO;

typedef struct {
  UINT8    DdrtExist;
  UINT8    HbmDdr5Exist;
  UINT8    Density;
  UINT8    Width;
  UINT8    Sid;
  UINT8    ColWidth;
  UINT8    RowWidth;
}DIMMINFO;


typedef struct {
  UINT8     ChEnabled;
  TADINFO   TadInfo[MAX_TAD];
  CHSADINFO ChSad[MAX_MC_DRAM_RULE];
  RIRINFO   RirInfo[MAX_RIR];
  RIRINFO   DdrtRirInfo[MAX_RIR];
  DIMMINFO  DimmInfo[MAX_DIMM_NUM];
  UINT8     SecCh;
  UINT8     PriCh;

  UINT8     NmTadValid;
  UINT8     NmTargetWay;
  UINT8     NmChnWay;
  UINT8     NmTargetGran;
  UINT8     NmChnGran;
  UINT8     NmTargetLid;
  UINT8     NmChnLid;
  UINT64    NmCachingOffset;
  UINT8     NmChnCap;
  UINT8     NmRatioChnCap;
  UINT64    NmIndexStartAddr;
  UINT64    NmIndexEndAddr;


  UINT8     ClosePg;
  UINT8     RankXorEnable;
  UINT8     DimmXorEnable;
  UINT8     BankXorEnable;
  UINT8     SidXorEnable;
  UINT8     Cgbg;
  UINT8     PchXorEnable;

}CHINFO;

typedef struct {
  UINT8          McEnabled;
  SAD2TAD        Sad2Tad[MAX_SAD2TAD];
  CHINFO         ChInfo[MAX_CH_PER_MC];
  BOOLEAN        HbmMc;
}MC_INFR;

typedef struct {
  UINT8          BlockModeEn;
  UINT64         BlockBase;
  UINT64         BlockLimit;
}BLOCKMODEINFO;

typedef struct {
  UINT32                 TotalCha;         //total CHA number
  UINT32                 NumCluster;       //number of Cluster
  UINT32                 NumUmaCluster;    //number of UMA Cluster
  CLUSTER_INFO           ClusterInfo[MAX_CLUSTER];
  MC_INFR                McInfo[MAX_SYS_IMC];
  REMOTE_DRAM_RULE       RemoteDramRule[MAX_REMOTE_DRAM_RULE];
  BLOCKMODEINFO          BlockMode;
} AD_SOCKET_INFO;

#pragma pack()

/**
  Returns a bit field from a 64-bit value with bitfield removed.

  Returns value with the bitfield specified by the StartBit and the EndBit  removed.


  If StartBit is greater than 63, then ASSERT().
  If EndBit is greater than 63, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().

  @param  Operand   Operand on which to perform the bitfield operation.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..63.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..63.

  @return a bit field from a 64-bit value with bitfield removed.

**/
UINT64
EFIAPI
BitFieldRemove64 (
  IN      UINT64                    Operand,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit
  );

/**
  Insert data at range started from StartBit and end at EndBit.

  If 64-bit operations are not supported, then ASSERT().
  If StartBit is greater than 63, then ASSERT().

  @param  Operand   Operand on which to perform the bitfield operation.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..63.
  @param  EndBit    End bit index of the least significant.
  @param  InsertValue    insert data.

  @return  value after divide

**/
UINT64
EFIAPI
BitFieldInsert64 (
  IN      UINT64                    Operand,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT32                    InsertValue
  );


/**
  check whether this DIMM belong to HBM type.

  @param[in]    Socket    --   socket index
  @param[in]    Mc        --   MC index


  @retval   TRUE -- HBM dimm
 **/
BOOLEAN
EFIAPI
IsHbmDimm (
  IN      UINT8     Socket,
  IN      UINT8     Mc
  );

/**
 This routine is to check whether mc is exist.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index

 @retval       TRUE-- this MC is exist.

**/
BOOLEAN
EFIAPI
IsMcExist (
  IN      UINT8    Socket,
  IN      UINT8    Mc
  );

/**
 This routine is to check whether this chn is exist.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Chn          --   chn index

 @retval       TRUE-- this MC is exist.
**/
BOOLEAN
EFIAPI
IsChannelExist (
  IN      UINT8    Socket,
  IN      UINT8    Mc,
  IN      UINT8    Chn
  );

/**
 This routine is to get socket channel.

 @param[in]   Mc  -- memory controller index
 @param[in]   McChn  -- chnanel in memory controller


 @ret  socket channel

**/
UINT8
EFIAPI
GetSktCh (
  IN    UINT8     Mc,
  IN    UINT8     McChn
  );

/**
  This routine is to initialize existing system DRAM resource.

  capture all DRAM resource. Assume all valid item are continuous. And some range are overrapped. We must put InclusiveFlag = 0 at latter part.

  @param[in] DramScopePtr    A pointer to DRAM resource array.

  @retval none

**/
VOID
EFIAPI
InitDramScope (
  IN   DRAM_SCOPE   *DramScopePtr
  );


/**
 This routine is to initialize CHA arrary at POST phase.

 @param[in]    Socket    --   socket index
 @param[in]    pChaArray  --  point to array of CHA.

 @retval       none.

**/
VOID
EFIAPI
InitChaDramRule (
  IN   UINT8             Socket,
  IN   CLUSTER_INFO    * pCluster
  );

/**
 This routine is to initialize socket 0 remote dram rule.

 @param[in]    pRemoteDramRule    --   point to remote dram rule info.


 @retval       none.

**/
VOID
EFIAPI
InitRemoteDramRule (
  IN     REMOTE_DRAM_RULE  * pRemoteDramRule
  );

/**
 This routine is to check whether this socket is exist


 @param[in]    Socket --  socket index

 @retval       TRUE -- this socket is presetn; FALSE --  this socket is non-exist.

**/
BOOLEAN
EFIAPI
IsSocketExist (
  IN   UINT8     Socket
  );


/**
  check whether is UMA based cluster mode

  @param[in] Socket   --   socket index

  @retval TRUE -- UBC mode.

**/
BOOLEAN
EFIAPI
IsUbcModeEnabled (
  IN  UINT8    Socket
  );

/**
  check whether is SNC based cluster mode

  @param[in] Socket   --   socket index

  @retval TRUE -- SNC mode.

**/
BOOLEAN
EFIAPI
IsSncModeEnabled (
  IN  UINT8    Socket
  );
/**
  This routine is to get cluster number per socket. system could be one of following state (UBC, SNC, or no clustering).
  UBC and SNC are mutual exlusive.

  @param[in] Skt   --   socket index

  @retval cluster number per socket. Value 1 means no UBC or SNC.

**/
UINT32
EFIAPI
GetClusterNum (
  IN   UINT8   Skt
  );


/**
  get XOR defeature mode.
  SPR XOR defeature mode disabled. Or could get it from KTILK_UMA_CLUSTER_CONFIG_KTI_LLPMON_STRUCT.defeature_xor

  @retval 0 -- XOR defeature mode disabled, cluster XOR enabled.
          1 -- XOR defeature mode enabled,  cluster XOR disabled.

**/
BOOLEAN
EFIAPI
GetXorDefeatureMode (
  VOID
  );

/**
  This routine is to check whether this address belong to DRAM space.

  @param[in] Sa    --  System address

  @retval TRUE -- it is DRAM address; FALSE -- it is not DRAM address.

**/
BOOLEAN
EFIAPI
IsDramMemoryAddress (
  IN   UINT64    Sa
  );


/**
  Get socket id per system address.

  It assumes this system address belong to Dram rule.

  @param[in]       Sa  --  system address

  @retval          socket id

**/
UINT8
EFIAPI
GetSocketId (
  IN    UINT64      Sa
  );

/**
  Get cluster index per system address & socket.

  @param[in]       SystemAddress    System address to be translated
  @param[in]       Socket    --     Socket Index
  @param[in]       ClusterXorDefeature -- Cluster xor feature mode.

  @retval          socket index.

**/
UINT8
EFIAPI
GetClusterIndex (
  IN      UINT64      Sa,
  IN      UINT8       Socket,
  IN      BOOLEAN     ClusterXorDefeature
  );


/**
  Get local DRAM rule target id per input address & socket-cluster.

  @param[in]       Sa            -- System address to be translated
  @param[in]       Socket        -- Socket index
  @param[in]       Cluster       -- Cluster index, if value 0xff, scan all cluster on the socket.
  @param[in, out]  TargetId      --  target ID. bit3 = 1 means local dram rule.
  @param[in, out]  LocalDramRuleIndex --  local dram rule index

  @retval          TRUE -- get targetId successfully; FALSE -- get targetId failed.

**/
BOOLEAN
EFIAPI
GetTargetId (
  IN     UINT64    Sa,
  IN     UINT8     Socket,
  IN OUT UINT8    *Cluster,
  IN OUT UINT8    *TargetId,
  IN OUT UINT8    *LocalDramRuleIndex
  );


/**
  check whether this target ID belong to remote socket.

  @param[in]       TargetId  -- target id
  @param[in, out]  Socket           Point to socket.

  @retval          TRUE  ---  this target ID indicates remote socket
                   FALSE ---  this target ID indicates local socket target ID.

**/
BOOLEAN
EFIAPI
IsRemoteSocketTargetId (
  IN       UINT8      TargetId,
  IN OUT   UINT8     *Socket
  );

/**
  Get MC ID accroding socket-cluster-dram rule-target ID.

  @param[in]       Sa            -- System address to be translated
  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index
  @param[in]       TargetId      -- local target ID


  @retval         Mc id

**/
UINT8
EFIAPI
GetMcId (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex,
  IN      UINT8     TargetId
  );

/**
  Get channel ID accroding socket-cluster-dram rule-target ID.

  @param[in]       Sa            -- System address to be translated
  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index
  @param[in]       TargetId      -- local target ID


  @retval         channel id

**/
UINT8
EFIAPI
GetChId (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex,
  IN      UINT8     TargetId
  );

/**
  Check whether this local dram rule is used for CXL.

  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index


  @retval        TRUE -- CXL dram rule.

**/
BOOLEAN
EFIAPI
IsCxlDramRule (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex
  );


/**
  Check whether this local dram rule is used for NXM (Non Exist Memory) -- memory hole.

  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index


  @retval        TRUE -- NXM dram rule.

**/
BOOLEAN
EFIAPI
IsNxmDramRule (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex
  );

/**
  Check whether this local dram rule is NM cache

  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index


  @retval        TRUE -- NM cache dram rule.

**/
BOOLEAN
EFIAPI
IsNmCache (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex
  );

/**
  Get cluster index per system mode.

  @param[in]       Sa    System address to be translated
  @param[in]       Socket    --     Socket Index


  @retval          cluster index. 0xFF means all clusters on this socket.

**/
UINT8
EFIAPI
GetClusterPerSystemMode (
  IN     UINT64       Sa,
  IN     UINT8        Socket
  );

/**
 This routine is to read channel TAD info.

 @param[in]    Socket    --   socket index


 @retval       none.

**/
VOID
EFIAPI
InitChTad (
  IN     UINT8       Socket
  );

/**
 This routine is to get channel Tad index.

 @param[in]    Sa           --   System address
 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index

 @retval       Ch Tad index.

**/
UINT8
EFIAPI
GetChTad (
  IN     UINT64     Sa,
  IN     UINT64     Socket,
  IN     UINT64     Mc
  );

/**
  Get channel address per tad info and system address.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    TadIndex  --   tad index

   @retval       channel address.

**/
UINT64
EFIAPI
Forward1lmDecodeGetCa (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     TadIndex
  );

/**
 This routine is to get SAD2TAD index.

 @param[in]    Sa           --   System address
 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index



 @retval       SAD2Tad index.

**/
UINT8
EFIAPI
GetSad2TadIndexFromSa (
  IN     UINT64     Sa,
  IN     UINT64     Socket,
  IN     UINT64     Mc
  );

/**
 This routine is to initialize M2M sad2tad.

 @param[in]    Socket    --   socket index
 @param[in]    McInfo    --   point to array of MC.

 @retval       none.

**/
VOID
EFIAPI
InitM2mSadTad (
  IN     UINT8       Socket
  );

/**
 This routine is to initialize CHA block mode info.

 @param[in]    Skt    --   socket index


 @retval       none.

**/
VOID
EFIAPI
InitBlockMode (
  IN      UINT8      Skt
  );

/**
 This routine is to check whether block mode is enabled.


 @retval       TRUE --  block mode is enabled

**/
BOOLEAN
EFIAPI
IsBlockmodeEnable (
  VOID
  );

/**
  forward decode SA belong to block mode space.

  @param[in]       Sa        -- System address
  @param[in, out]  Socket    -- Socket index
  @param[in, out]  Mc        -- mc index
  @param[in, out]  chn       -- channel


  @retval        TRUE -- this system address belong to block mode space.

**/
BOOLEAN
EFIAPI
ForwardDecodeBlockAddress (
  IN      UINT64    Sa,
  IN OUT  UINT8    *Socket,
  IN OUT  UINT8    *Mc,
  IN OUT  UINT8    *Chn
  );

/**
  forward RIR decode for HBM/DDR5.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    Ca        --   Channel address
   @param[in, out]  Rank   --   point to rank
   @param[in, out]  Ra     --   point to rank address

   @retval       TRUE -- RIR decode succcessfully.

**/
BOOLEAN
EFIAPI
ForwardRirDecode (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT64    Ca,
  IN OUT  UINT8    *Rank,
  IN OUT  UINT64   *Ra
  );

/**
 this is for AMAP forward decode.

 @param[in]            Ra   -- rank address
 @param[in]            HbmType  -- indicate HBM type
 @param[in]            Width    -- indicate width
 @param[in]            Density  -- indicate density
 @param[in]            ClosePg  -- indicate close page
 @param[in]            Cgbg  -- indicate CGBG type
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- columen

 @ret none

**/
VOID
EFIAPI
ForwardAmap (
  IN         UINT64      Ra,
  IN         UINT8       HbmType,
  IN         UINT8       Width,
  IN         UINT8       Density,
  IN         UINT8       ClosePg,
  IN         UINT8       Cgbg,
  IN         UINT8       BankXorEnable,
  IN         UINT8       SidXorEnable,
  IN         UINT8       SidNum,
  IN OUT     UINT8      *SubRank,
  IN OUT     UINT8      *Bank,
  IN OUT     UINT32     *Row,
  IN OUT     UINT32     *Column
  );



/**
 this is for AMAP forward decode.

 @param[in]            Ra   -- rank address
 @param[in]            Socket    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            DimmIndex --   Dimm index
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- columen

 @ret none

**/
VOID
EFIAPI
ForwardAmapDecode (
  IN         UINT64      Ra,
  IN         UINT8       Socket,
  IN         UINT8       Mc,
  IN         UINT8       Channel,
  IN         UINT8       DimmIndex,
  IN OUT     UINT8      *SubRank,
  IN OUT     UINT8      *Bank,
  IN OUT     UINT32     *Row,
  IN OUT     UINT32     *Column
  );

/**
  Get NM channel address per 2LM decode.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @retval       NM channel address.

**/
UINT64
EFIAPI
Forward2lmDecodeGetCa (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  );



/**
  Get FM mc/chn from NM channel info

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in, out]  FmMc   --   Far mc
   @param[in, out]  FmChn  --   Far chn

   @retval       TRUE -- get far mc/chn successfully

**/
BOOLEAN
EFIAPI
GetFmInfoFromNmChn (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     *FmMc,
  IN      UINT8     *FmChn
  );

/**
 This routine is to get whether the target is DDRT RIR or DDR RIR.

 Note: when result is true, it contains one case which should be exclude -- DDR5 as NM and DDRT as volatile - FM.
       Caller code should ensure to invoke this routine at normal TAD flow as above case only exist at NM TAD flow.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Sad2TadIndex --   sad2tad index

 @retval       TRUE -- this is for DDRT RIR.

**/
BOOLEAN
EFIAPI
GetDdrFlag (
  IN    UINT8    Socket,
  IN    UINT8    Mc,
  IN    UINT8    Sad2TadIndex
  );

/**
  forward DDRT RIR decode for HBM/DDR5.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    Ca        --   Channel address
   @param[in, out]  Rank   --   point to rank
   @param[in, out]  Ra     --   point to rank address

   @retval       TRUE -- RIR decode succcessfully.

**/
BOOLEAN
EFIAPI
ForwardDdrtRirDecode (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT64    Ca,
  IN OUT  UINT8    *Rank,
  IN OUT  UINT64   *Ra
  );


/**

Get the DIMM Type for the given DIMM info

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    Rank      --   Rank index on channel

@retval DIMM type
**/
UINT8
EFIAPI
GetDimmType(
  IN    UINT8     Skt,
  IN    UINT8     Mc,
  IN    UINT8     Channel,
  IN    UINT8     Rank
  );

/**
 this is for AMAP forward reverse decode.

 @param[in, out]  Ra   -- rank address
 @param[in]       HbmType  -- indicate HBM type
 @param[in]       Width    -- indicate width
 @param[in]       Density  -- indicate density
 @param[in]       ClosePg  -- indicate close page
 @param[in]       Cgbg  -- indicate CGBG type
 @param[in]       BankXorEnable  -- bank xor enable
 @param[in]       SidXorEnable  -- sid xor enable
 @param[in]       SidNum  -- sid number
 @param[in]       SubRank  -- subrank
 @param[in]       Bank  -- bank
 @param[in]       Row -- row
 @param[in]       Column  -- column

 @ret TRUE -- reverse AMAP successfully.

**/
BOOLEAN
EFIAPI
ReverseAmap (
  IN OUT     UINT64     *Ra,
  IN         UINT8       HbmType,
  IN         UINT8       Width,
  IN         UINT8       Density,
  IN         UINT8       ClosePg,
  IN         UINT8       Cgbg,
  IN         UINT8       BankXorEnable,
  IN         UINT8       SidXorEnable,
  IN         UINT8       SidNum,
  IN         UINT8       SubRank,
  IN         UINT8       Bank,
  IN         UINT32      Row,
  IN         UINT32      Column
  );

/**
 this is for AMAP reverse decode.

 @param[in, out]       Ra   -- rank address
 @param[in]            Socket    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            DimmIndex --   Dimm index
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- columen

 @ret TRUE -- AMAP reverse successfully

**/
BOOLEAN
EFIAPI
ReverseAmapDecode (
  IN OUT     UINT64     *Ra,
  IN         UINT8       Socket,
  IN         UINT8       Mc,
  IN         UINT8       Channel,
  IN         UINT8       DimmIndex,
  IN         UINT8       SubRank,
  IN         UINT8       Bank,
  IN         UINT32      Row,
  IN         UINT32      Column
  );


/**
 this is for RIR reverse decode for all memory type.

 @param[in, out]       Ca   -- channenl address
 @param[in]            Dimmtype -- dimm type
 @param[in]            Skt    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            Rank      --   Rank index
 @param[in]            Ra        --   rank address form HBM/DDR5
 @param[in]            Dpa       --   Dpa address for DCPMM.

 @ret none

**/
BOOLEAN
EFIAPI
ReverseRirDecode (
  IN  OUT     UINT64       *Ca,
  IN          UINT8         DimmType,
  IN          UINT8         Skt,
  IN          UINT8         Mc,
  IN          UINT8         Channel,
  IN          UINT8         Rank,
  IN          UINT64        Ra,
  IN          UINT64        Dpa
  );


  /**
  This routine is to do revserse decode from CA to SA.

  @param [socket]   -- socket index
  @param [Mc]       -- memory controller index
  @param [Channel]  -- channel index per Mc
  @param [rank]     -- rank index
  @param [Ca]       -- channel address

  @ret   system address.

 **/
UINT64
EFIAPI
ReverseTadDecode (
  IN     UINT8   Socket,
  IN     UINT8   Mc,
  IN     UINT8   Channel,
  IN     UINT8   Rank,
  IN     UINT64  Ca
  );


/**
  calculate PCH value per system address and xor.

   @param[in]    Sa        --   System address
   @param[in]    PchXor    --   whether pch xor is enabled

   @retval       PCH

**/
UINT8
EFIAPI
CalcPch (
  IN      UINT64    Sa,
  IN      UINT8     PchXor
  );


/**
 This routine is to know whether this M2M tad entry belong to block region.


 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Sad2TadIndex --   sad2tad index


 @retval       TRUE -- this is block region.

**/
BOOLEAN
EFIAPI
IsLocateBlockMode (
  IN    UINT8    Socket,
  IN    UINT8    Mc,
  IN    UINT8    Sad2TadIndex
  );

/**
 This routine is to check whether the SA locate in specified M2M tad.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Sad2TadIndex --   sad2tad index

 @retval       TRUE -- it is located.

**/
BOOLEAN
EFIAPI
IsM2mTadRange (
  IN    UINT64     Sa,
  IN    UINT8      Socket,
  IN    UINT8      Mc,
  IN    UINT8      Sad2TadIndex
  );


/**
 this is for get chn address on the normal tad.

 @param[in]            Ra   -- rank address
 @param[in]            Socket    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index

 @ret chn address

**/
UINT64
EFIAPI
GetNormalTadChannelAddress (
  IN         UINT64      Sa,
  IN         UINT8       Socket,
  IN         UINT8       Mc,
  IN         UINT8       Channel
  );
/**
  Get target ways and channel ways per tad.


   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    TadIndex  --   tad index
   @param[in, out]  TargetWays -- target ways
   @param[in, out]  ChannelWays -- channel ways

   @retval       channel address.

**/
VOID
EFIAPI
GetTadInfo (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     TadIndex,
  IN OUT  UINT8    *TargetWays,
  IN OUT  UINT8    *ChannelWays
  );
#endif
