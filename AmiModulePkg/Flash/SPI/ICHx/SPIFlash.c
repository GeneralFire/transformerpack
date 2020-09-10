//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  Flash update routines file.
**/

//----------------------------------------------------------------------------
// Includes
#include <Uefi.h>
#include <Token.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/PciLib.h>
#include <Library/TimerLib.h>
#include <FlashPart.h>
#include <SpiFlash.h>

//----------------------------------------------------------------------------
// Local defines for transaction types
// SPI Register Configuration 
typedef struct {
    UINT32  BusDevFuncRegr; // SPBAR + 04h: Hardware Sequencing Flash Status Register (16bits)
    UINT8   HwSeqStsCtl;   // SPBAR + 04h: Hardware Sequencing Flash Status Register (16bits)
    UINT8   FlashAddr;  // SPBAR + 08h: Flash Address
    UINT8   FlashData;  // SPBAR + 10h: Flasg Data (64 bytes)
    UINT8   SwSeqSts;   // SPBAR + 90h: Software Sequencing Flash Status Register
    UINT8   SwSeqCtrl;  // SPBAR + 91h: Software Sequencing Flash Control Register
    UINT8   PreOpCode;  // SPBAR + 94h: Prefix Opcode Configuration
    UINT8   OpCodeType; // SPBAR + 96h: Opcode Type Configuration
    UINT8   OpCodeMenu; // SPBAR + 98h: Opcode Menu Configuration
    UINT8   BiosFdoc;   // SPBAR + 0B0h: Flash Descriptor Observability Control Register
    UINT8   BiosFdod;   // SPBAR + 0B4h: Flash Descriptor Observability Data Register   
    UINT8   BiosLVscc;  // SPBAR + 0C4h: Host Lower Vendor Specific Component Capabilities Register
    UINT8   BiosUVscc;  // SPBAR + 0C8h: Host Upper Vendor Specific Component Capabilities Register
    UINT8   BiosPTInx;  // SPBAR + 0CCh: Parameter Table Index
    UINT8   BiosPTData;  // SPBAR + 0D0h: Parameter Table Data
}SPI_REGS;

// SPI default opcode slots
#define     SPI_OPCODE_WRITE_INDEX      0x0
#define     SPI_OPCODE_READ_INDEX       0x1
#define     SPI_OPCODE_ERASE_INDEX      0x2
#define     SPI_OPCODE_READ_S_INDEX     0x3
#define     SPI_OPCODE_READ_ID_INDEX    0x4
#define     SPI_OPCODE_WRITE_S_INDEX    0x5
#define     SPI_OPCODE_WRITE_S_E_INDEX  0x6
#define     SPI_OPCODE_WRITE_E_INDEX    0x7
#define     SPI_OPCODE_READ_SFDP_INDEX  0x7
#define     SPI_OPCODE_AAI_INDEX        0x6
#define     SPI_OPCODE_WRITE_D_INDEX    0x7
#define     SPI_PREFIX_WRITE_S_EN       0x1
#define     SPI_PREFIX_WRITE_EN         0x0

// SPI Hardware Flash Cycle Type  
#define     SPI_HW_SEQ_READ             0x0
#define     SPI_HW_SEQ_WRITE            0x2
#define     SPI_HW_SEQ_ERASE_4K         0x3
// Below Hardware Flash Cycle Types are added from Skylake-PCH  
#define     SPI_HW_SEQ_ERASE_64K        0x4
#define     SPI_HW_SEQ_READ_SFDP        0x5
#define     SPI_HW_SEQ_READ_ID          0x6
#define     SPI_HW_SEQ_WRITE_STATUS     0x7
#define     SPI_HW_SEQ_READ_STATUS      0x8

#define     SPI_MAX_DATA_TRANSFER       0x40
#define     FLASH_VALID_SIGNATURE       0x0FF0A55A
#define     NO_ADDRESS_UPDATE           0
#ifndef SPI_OPCODE_TYPE_READ_NO_ADDRESS
#define     SPI_OPCODE_TYPE_READ_NO_ADDRESS     0x0
#define     SPI_OPCODE_TYPE_WRITE_NO_ADDRESS    0x1
#define     SPI_OPCODE_TYPE_READ_WITH_ADDRESS   0x2
#define     SPI_OPCODE_TYPE_WRITE_WITH_ADDRESS  0x3
#endif  // #ifndef SPI_OPCODE_TYPE_READ_NO_ADDRESS
#ifndef SPI_USE_HARDWARE_SEQUENCE
#define     SPI_USE_HARDWARE_SEQUENCE  0
#endif  // #ifndef SPI_USE_HARDWARE_SEQUENCE
#ifndef NO_MMIO_FLASH_ACCESS_DURING_UPDATE
#define NO_MMIO_FLASH_ACCESS_DURING_UPDATE 0
#endif

#define FLASH_DEVICE_BASE 0xFFFFFFFF - FLASH_SIZE + 1
#define CF8_TO_PCI(A) (UINT32)(((A & 0xFFFF00)  << 4) | (A & 0xFF))
//----------------------------------------------------------------------------
// Module level global data
extern UINT16       gFlashId;
extern FLASH_PART   *gFlashApi;
extern UINTN        gAmiFlashDeviceBase;
extern UINT32       gFlashBlockSize;
//----------------------------------------------------------------------------
// Function Externs
extern
VOID
SpiChipsetVirtualFixup(
    IN  EFI_RUNTIME_SERVICES *Rs
);
//----------------------------------------------------------------------------
// Local prototypes
static BOOLEAN
EFIAPI
CommonSpiEraseCommand(
    IN  volatile UINT8  *BlockAddress
);

static EFI_STATUS
EFIAPI
CommonSpiEraseCommandEx(
    IN  volatile UINT8  *BlockAddress,
    IN  UINT32          Length,
    IN  BOOLEAN         IsMappedAddress
);

static BOOLEAN
EFIAPI
CommonSpiProgramCommand(
    IN      volatile UINT8  *Address,
    IN      UINT8           *Byte,
    IN  OUT UINT32          *Length,
    IN      BOOLEAN         IsMappedAddress
);

static VOID
EFIAPI
CommonSpiReadCommand(
    IN      volatile UINT8  *Address,
    OUT     UINT8           *Byte,
    IN  OUT UINT32          *Length,
    IN      BOOLEAN         IsMappedAddress
);

static VOID
EFIAPI
CommonSpiBlockWriteEnable(
    IN  volatile UINT8  *BlockAddress,
    IN  UINT32          Length,
    IN  BOOLEAN         IsMappedAddress
);

static VOID
EFIAPI
CommonSpiBlockWriteDisable(
    IN  volatile UINT8  *BlockAddress,
    IN  UINT32          Length,
    IN  BOOLEAN         IsMappedAddress
);

static VOID
EFIAPI
CommonSpiDeviceWriteEnable(
    VOID
);

static VOID
EFIAPI
CommonSpiDeviceWriteDisable(
    VOID
);

static BOOLEAN
EFIAPI
CommonSpiGetEraseGranularity(
    IN  OUT UINT32  **Granularity,
    IN  OUT UINT32  *NumberOfEntries
);

static VOID
EFIAPI
CommonSpiDeviceVirtualFixup(
    IN  EFI_RUNTIME_SERVICES    *Rs
);

VOID
SpiBlockProtectUpdate(
    IN  UINT32  FlashOffset,
    IN  UINT32  Length,
    IN  UINT8   StatusData
);

//----------------------------------------------------------------------------
// Local Variables
FLASH_PART gCommonSpiFlash = {
    CommonSpiReadCommand,
    CommonSpiEraseCommand,
    CommonSpiEraseCommandEx,
    CommonSpiProgramCommand,
    CommonSpiBlockWriteEnable,
    CommonSpiBlockWriteDisable,
    CommonSpiDeviceWriteEnable,
    CommonSpiDeviceWriteDisable,
    CommonSpiGetEraseGranularity,
    CommonSpiDeviceVirtualFixup,
    1,                      // default value, should be changed in Init function
    SECTOR_SIZE_4KB,        // default value, should be changed in Init function
    NULL                    // Flash Part Number Pointer
};
EX_FLASH_PART gExFlashPart = {
      {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},0,0},
      FLASH_SIZE,           // flash size, should be changed in Init function
      0,                    // flash part id, should be changed in Init function
      0                     // flash part string, should be changed in
                            // Init function
};
#if defined(SPI_BASE) && (SPI_BASE != 0)
volatile UINT8  *gSpiBase = (UINT8*)(UINTN)SPI_BASE;
#else
volatile UINT8  *gSpiBase = (UINT8*)(UINTN)(SB_RCBA + SPI_BASE_ADDRESS);
#endif
UINT8           gDeviceVirtual = 0;
UINT8           gDeviceWriteEnabled = 0;
static UINTN    gBiosRegionBase = 0;
static UINTN    gBiosRegionLimit = 0;
static UINTN    gMaxFlashLimit = 0;
static UINT32   gGranularity[4];
const SPI_REGS  gSpiRegs[] = SPI_REGISTERS;
//----------------------------------------------------------------------------
// Function Definitions
/**
    The function delays for flash operation.
**/
VOID
SpiIoDelay(
    VOID
)
{
    MicroSecondDelay( 10 );
    return ;
}

/**
    This function Initial the SPI BASE Address
**/
BOOLEAN
InitializeSpiBase(
    VOID
)
{
    if( gSpiRegs->BusDevFuncRegr != 0 )
    {
#if defined(SMM_BUILD)
        {
            VOID    *SpiMmioAddr;

            //Get Spi pci device BAR
            SpiMmioAddr = (VOID*)(UINTN)(PciRead32( CF8_TO_PCI(gSpiRegs->BusDevFuncRegr) ) & (~(UINT32)SPI_BAR0_MASK));
            if( SpiMmioAddr != (VOID*)gSpiBase )
                return FALSE;
        }
#else
        gSpiBase = (UINT8*)(UINTN)(PciRead32( CF8_TO_PCI(gSpiRegs->BusDevFuncRegr) ) & (~(UINT32)SPI_BAR0_MASK));
#endif
    }

    return TRUE;
}

/**
    This function remove the zero Opcode from the Opmenu and PreFix Opcode

    @param FlashInfo SPI Flash Information Table.
**/
VOID
RemoveZeroOpcode(
    IN OUT  FLASH_INFO  *FlashInfo
)
{
#if SPI_USE_HARDWARE_SEQUENCE != 2
    UINT32    Index;
    UINT16    CurOpType;

    // If zero in Opcode menu, use ReadId Opcode instead
    for( Index = 0; Index < 8; Index++ )
    {
        if( *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeMenu + Index) == 0 )
        {
            //Update opcode
            *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeMenu + Index) = FlashInfo->ReadId.Opcode;
            //Update opcode type
            CurOpType = *((volatile UINT16*)(gSpiBase + gSpiRegs->OpCodeType));
            CurOpType &= (UINT16)(~(0x3 << (Index * 2)));
            CurOpType |= (UINT16)(FlashInfo->ReadId.OpcodeType << (Index * 2));
            *((volatile UINT16*)(gSpiBase + gSpiRegs->OpCodeType)) = CurOpType;
        }
    }

    // If zero in PreOpcode menu, use Write Enable Opcode instead
    if( *(volatile UINT8*)(gSpiBase + gSpiRegs->PreOpCode) == 0 )
        *(volatile UINT8*)(gSpiBase + gSpiRegs->PreOpCode) = FlashInfo->WriteEnable.Opcode;

    if( *(volatile UINT8*)(gSpiBase + gSpiRegs->PreOpCode + 1) == 0 )
        *(volatile UINT8*)(gSpiBase + gSpiRegs->PreOpCode + 1) = FlashInfo->WriteEnable.Opcode;
#endif
}

/**
    This function retuns the Flash Descriptor Override Pin Strap Status (FDOPSS).

    @retval TRUE The Flash Descriptor Security Override strap is set
    @retval FALSE The Flash Descriptor Security Override strap is not set
**/
BOOLEAN
IsFlashDescriptorOverrided(
    VOID
)
{
    return !(*(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl) & BIT13);
}

/**
    This function check 64K erase commnad is available.

    @retval TRUE 64K erase commnad is available
    @retval FALSE 64K erase commnad is not available
**/
BOOLEAN
EFIAPI
IsErase64KAvailable(
    VOID
)
{
    UINT32  Vscc;

    Vscc = *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosLVscc);
    //Vscc value bit 29 is 64K erase valid
    if( ((Vscc & (UINT32)BIT29) == BIT29) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
    This function gets the Base and Limit from the specified Flash Region

    @param Type Type of Flash region.
    @param Base Base Address of region.
    @param Limit Limit Address of region.

    @retval EFI_SUCCESS Operation Completed
    @retval EFI_INVALID_PARAMETER Operation not Completed
**/
EFI_STATUS
GetFlashRegion(
    FLASH_REGION_TYPE   Type,
    UINTN               *Base,
    UINTN               *Limit
)
{
    UINT32  Data;

    //First, check whether works in Flash Descriptor mode.
    *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosFdoc) = 0;
    Data = *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosFdod);
    if( Data != (UINT32)FLASH_VALID_SIGNATURE )
        return EFI_UNSUPPORTED;

    //Descriptor Mode, Get Region from SPI Memory Mapped Configuration Registers.
    if( Type < FlashRegionC )
    {
        if( Type == FlashRegionBios )
        {
            //From Emmitsburg EDS, BIOS_BFPREG BIT31 is SBRS
            //If SBRS is set, system is booted from region 6 (Secondary BIOS region).
            Data = *(volatile UINT32*)(gSpiBase + 0);
            if( (Data & BIT31) != 0)
            {
                Data = *(volatile UINT32*)(gSpiBase + 0x6C);
            }
            else
            {
                Data = *(volatile UINT32*)(gSpiBase + 0x58); 
            }
        }
        else
        {
            //FREG0 ~ FREG11 starts from offest 0x54 to 0x80
            Data = *(volatile UINT32*)(gSpiBase + 0x54 + (Type * sizeof(UINT32)) ); 
        }
    }
    else if( (FlashRegionC <= Type) && (Type < FlashRegionMax) )
    {
        //FREG12 ~ FREG15 starts from offset 0xE0
        Data = *(volatile UINT32*)(gSpiBase + 0xE0 + ((Type - FlashRegionC) * sizeof(UINT32)) ); 
    }
    else
    {
        return EFI_UNSUPPORTED;
    }

    if( Data == 0x7FFF )
        return EFI_UNSUPPORTED;
    if( Base != NULL )
        *Base = (UINTN)((Data & 0xFFFF) << 12);
    if( Limit != NULL )
        *Limit = (UINTN)((Data >> 4) | 0xFFF);

    return EFI_SUCCESS;
}

/**
    This function gets the Maximum Limit from the specified Flash Region

    @param Limit Maximum Limit Address of region.

    @retval EFI_SUCCESS Operation Completed
**/
EFI_STATUS 
GetMaxRegionLimit(
    UINTN   *Limit
)
{
    UINTN   Index;
    UINTN   MaxRegionLimit;

    for( *Limit = MaxRegionLimit = Index = 0; Index < FlashRegionMax; Index++ )
    { 
        if( EFI_ERROR(GetFlashRegion( Index, NULL, &MaxRegionLimit )) )
            continue;
        if( MaxRegionLimit > *Limit )
            *Limit = MaxRegionLimit;
    }

    return EFI_SUCCESS;
}

/**
    This function reads data from the specified Flash Region

    @param Type Type of Flash region.
    @param Address Start Address of region.
    @param Length number of bytes to read.
    @param Buffer pointer to data to read from the flash region.

    @retval EFI_SUCCESS Operation Completed
    @retval EFI_INVALID_PARAMETER Operation not Completed
**/
EFI_STATUS
FlashRegionOperation(
    IN  FLASH_REGION_TYPE   Type,
    IN  FLASH_OPERATION     Operation,
    IN  UINT32              Address,
    IN  UINT32              Length,
    IN  UINT8               *Buffer
)
{
    EFI_STATUS      Status;
    UINTN           RegionBase;
    UINTN           RegionLimit;
    UINTN           BiosRegionLimit;
    volatile UINT8  *FlashPartOffset;
    UINT32          TempLength;
    UINT32          Index;

    // Initial SPI Base.
    if( InitializeSpiBase() == FALSE )
        return EFI_INVALID_PARAMETER;

#if SPI_USE_HARDWARE_SEQUENCE == 0
    // Check whether SPI registers have been initialed, return error if no.
    if( *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeMenu + 1) == 0 )
        return EFI_NOT_READY;
#endif
    // Get RegionBase and RegioLimit.
    Status = GetFlashRegion( Type, &RegionBase, &RegionLimit );
    if( EFI_ERROR(Status) || ((RegionBase + Length) > (RegionLimit + 1)) )
        return EFI_INVALID_PARAMETER;
    // Get Bios RegionLimit for patching "CommonConvertSpiAddress" hook of CommonSpiReadCommand.
    Status = GetFlashRegion( FlashRegionBios, NULL, &BiosRegionLimit );
    if( EFI_ERROR(Status) )
        return EFI_INVALID_PARAMETER;

    FlashPartOffset = (volatile UINT8*)(RegionBase + Address);

    switch( Operation )
    {  
        case FlashRegionErase:
            Status = CommonSpiEraseCommandEx( FlashPartOffset, Length, FALSE );
            break;
        case FlashRegionRead:
        case FlashRegionWrite:
            TempLength = Length;
            Status = EFI_SUCCESS;
            for( Index = 0; Index < Length; Index = (Length - TempLength) )
            {
                if( Operation == FlashRegionRead )
                    CommonSpiReadCommand( FlashPartOffset + Index, Buffer + Index, &TempLength, FALSE );
                if( Operation == FlashRegionWrite )
                {
                    BOOLEAN Result;
                    Result = CommonSpiProgramCommand( FlashPartOffset + Index, Buffer + Index, &TempLength, FALSE );    
                    if( Result == FALSE )
                    {
                        Status = EFI_DEVICE_ERROR;
                        break;
                    }
                }
            }
        break;
        default: return EFI_INVALID_PARAMETER;
    }

    return Status;
}

/**
    This function verifies whether the command sent to the flash part has 
    completed 

    @param HwCycle checking SPI Hardware Sequence 
    @param SwCycle checking SPI Software Sequence 
**/
VOID
WaitForSpiCycleDone(
    IN  UINT8   HwCycle,
    IN  UINT8   SwCycle
)
{
    UINT32  Timeout;
    UINT8   CyclyDone;
    
    if( HwCycle )
    {
        for( Timeout = 0, CyclyDone = 0; Timeout < 0x4000000; Timeout++ )
        {
            CyclyDone = *(volatile UINT8*)(gSpiBase + gSpiRegs->HwSeqStsCtl);
            if( (CyclyDone & BIT0) || ((CyclyDone & BIT5) == 0) )
                break;
        }
        // write BIT2 to clear CycleDone status
        *(volatile UINT8*)(gSpiBase + gSpiRegs->HwSeqStsCtl) = BIT0 + BIT1 + BIT2;
    }

    if( SwCycle )
    {
        for( Timeout = 0, CyclyDone = 0; Timeout < 0x4000000; Timeout++ )
        {
            CyclyDone = *(volatile UINT8*)(gSpiBase + gSpiRegs->SwSeqSts);
            if( (CyclyDone & BIT2) || ((CyclyDone & BIT0) == 0) )
                break;
        }
        // write BIT2 to clear CycleDone status
        *(volatile UINT8*)(gSpiBase + gSpiRegs->SwSeqSts) = BIT2 + BIT3 + BIT4;
    }
}

/**
    This function issuess SPI Write Disable if AAIWordProgram.

**/
VOID
SpiWriteDisable(
    VOID
)
{
    // Opcode menu slot 7 is configured as "Write Disable" if AAIWordProgram.
    *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) = \
                                (SPI_OPCODE_WRITE_D_INDEX << 4) + BIT1;
    // Wait for spi software sequence cycle completed.
    WaitForSpiCycleDone( FALSE, TRUE );

    return;
}

/**
    This function checks whether need to issue the AAIWordProgram command.

    @param Addr Start address to be written.  
    @param Length Number of bytes to be written.

    @retval TRUE Need to issue the AAI command 
    @retval FALSE No need to issue the AAI command
**/
BOOLEAN
CheckAaiWordProram(
    IN  UINT32  Addr,
    IN  UINT32  Length
)
{
    if( (gExFlashPart.AAIWordProgram != 0) && !(Addr & 1) && (Length >= 2) )
        return TRUE;

    return FALSE;
}

/**
    This function reads the status register by specified flash address

    @param UpdateAddr flash address to be read the status register.  

    @retval value of the status register
**/
UINT8
CommonSpiReadStatus(
    IN  UINT32  UpdateAddr
)
{
    UINT16  SpiCmd;

    if( UpdateAddr ) *(volatile UINT32*)(gSpiBase + gSpiRegs->FlashAddr) = UpdateAddr;
#if SPI_USE_HARDWARE_SEQUENCE == 2
    // Flash SPI Cycle type 8 generated to "Read Status"
    SpiCmd = (SPI_HW_SEQ_READ_STATUS << 1) + BIT0;
    *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl + 2) = SpiCmd;
    // Wait for spi hardware sequence cycle completed.
    WaitForSpiCycleDone( TRUE, FALSE );
#else
    // Opcode menu slot 3 is configured as "Read Status Register"
    SpiCmd = SPI_OPCODE_READ_S_INDEX << 4;
    // indicate that data phase is required
    SpiCmd += (1 << 14);
    // Set BIT1 (Go)
    *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) = SpiCmd + BIT1;
    // Wait for spi software sequence cycle completed.
    WaitForSpiCycleDone( FALSE, TRUE );
#endif
    // return status register.
    return (*(volatile UINT8*)(gSpiBase + gSpiRegs->FlashData));
}

/**
    This function verifies whether the command sent to the flash part has
    completed
**/
VOID
WaitForWriteOperationCompleted(
    VOID
)
{
    UINT16  WaitStsRetry;
    UINT8   Status;

    for( WaitStsRetry = 0; WaitStsRetry < 0xFFFF; WaitStsRetry++ )
    {
        // read flash status register.
        Status = CommonSpiReadStatus( NO_ADDRESS_UPDATE );
        // Is operation busy ?
        if( !(Status & 0x1) )
            break;
    }
}

/**
    This function writes the status register by specified flash address

    @param WriteData Data to be written to the status register.
    @param OpcodeIndex Index of the OpCode
    @param IsDataPhase Data Phase need or not.
    @param PrefixOp Index of the Prefix OpCode
    @param SectorAddress flash address to be written to the status register
**/
VOID
CommonSpiWriteStatus(
    IN  UINT8   WriteData,
    IN  UINT8   OpcodeIndex,
    IN  UINT8   IsDataPhase,
    IN  UINT8   PrefixOp,
    IN  UINT32  SectorAddress
)
{
    UINT16  SpiCmd;
#if SPI_USE_HARDWARE_SEQUENCE == 2
    UINT16  SpiFlashCtl;
#endif

    // Wait for spi hardware/software sequence cycle completed.
    WaitForSpiCycleDone( TRUE, TRUE );
    *(volatile UINT8*)(gSpiBase + gSpiRegs->FlashData) = WriteData;
    *(volatile UINT32*)(gSpiBase + gSpiRegs->FlashAddr) = SectorAddress;
#if SPI_USE_HARDWARE_SEQUENCE == 2
    // Check WRSDIS bit
    SpiFlashCtl = *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl);
    if( SpiFlashCtl & BIT11 )
        return;
    // Flash SPI Cycle type 7 generated to "Write Status"
    SpiCmd = (SPI_HW_SEQ_WRITE_STATUS << 1) + BIT0;
    *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl + 2) = SpiCmd;
    // Wait for spi hardware sequence cycle completed.
    WaitForSpiCycleDone( TRUE, FALSE );
#else
    // Opcode menu slot 3 is configured as "Read Status Register"
    SpiCmd = OpcodeIndex << 4;
    // indicate that data phase is required
    SpiCmd += (IsDataPhase << 14);
    // BIT3(Preop 1)
    SpiCmd += (PrefixOp << 3);
    // Set BIT1 (Go), BIT2(Atomic w/ Prefix),
    *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) =  SpiCmd + BIT1 + BIT2;
    // Wait for spi software sequence cycle completed.
    WaitForSpiCycleDone( FALSE, TRUE );
#endif
    // wait for SPI flash operation completed.
    WaitForWriteOperationCompleted();
    // return status register.
    return;
}

/**
    Verifies if the device has been erased properly or if the current byte
    is the same as the byte to be written at this location.

    @param Destination starting address of where the data will be written
    @param Source starting address of the data that is supposed to be written or
        "NULL" for checking erase completed.
    @param Length length of the data set to check
    @param SpiCycle use the "SPI Cycle" or not
    @param IsMappedAddress Input param Destination is mapping address or not

    @retval TRUE the data is match or is clean.
    @retval FALSE the data is not match or is not clean.
**/
static
BOOLEAN
IsEqualOrClean(
    IN  volatile UINT8  *Destination,
    IN  UINT8           *Source,
    IN  UINT32          Length,
    IN  BOOLEAN         SpiCycle,
    IN  BOOLEAN         IsMappedAddress
)
{
    UINT8   Buffer[SPI_MAX_DATA_TRANSFER];
    UINT8   Data;
    UINT32  Index1;
    UINT32  Index2;
    UINT32  TempLength = Length;
    UINT32  RemainBytes = Length;
#if defined(NO_MMIO_FLASH_ACCESS_DURING_UPDATE) && (NO_MMIO_FLASH_ACCESS_DURING_UPDATE == 0)
    UINTN   FlashBase = 0xFFFFFFFF - gMaxFlashLimit;
    // Check the Top Swap enabled only if device is not converted to virtual for 
    // below reasons. 1) So Far TopSwap bit is set only if BIOS is in Recovery not 
    // in runtime. 2) The IsTopSwapOn() could not be converted to virtual then could
    // cause General Protection Fault in runtime.
    if( SpiCycle && gBiosRegionLimit && !gDeviceVirtual && !IsTopSwapOn() )
    {
        if( IsMappedAddress == TRUE )
        {
            if( ((UINTN)Destination >= (gBiosRegionBase + FlashBase)) &&
                ((UINTN)Destination <= (gBiosRegionLimit + FlashBase)) )
                SpiCycle = FALSE;
        }
        else
        {
            if( ((UINTN)Destination >= gBiosRegionBase) &&
                ((UINTN)Destination <= gBiosRegionLimit) )
                SpiCycle = FALSE;
        }
    }
#endif // #if defined NO_MMIO_FLASH_ACCESS_DURING_UPDATE && NO_MMIO_FLASH_ACCESS_DURING_UPDATE == 0
    for( Index1 = 0; Index1 < Length; TempLength = RemainBytes, Index1 = (Length - RemainBytes) )
    {
        // Initial local read buffer.
        SetMem( (VOID*)Buffer, SPI_MAX_DATA_TRANSFER, 0 );
        // Read flash through SPI Cycle if SpiCycle is requested.
        if( SpiCycle )
            CommonSpiReadCommand( Destination + Index1, Buffer, &RemainBytes, IsMappedAddress );
        else
        {
            // Read flash through flash memory mapping
            (RemainBytes > SPI_MAX_DATA_TRANSFER) ? \
                        (RemainBytes -= SPI_MAX_DATA_TRANSFER) : (RemainBytes = 0);
            CopyMem( (VOID*)Buffer, (VOID*)(Destination + Index1), TempLength - RemainBytes );
        }
        // Verify whether flash contents is the same as written Data or is clean.
        for( Index2 = 0; Index2 < (TempLength - RemainBytes); Index2++ )
        {
            Data = (Source != NULL) ? *(Source + Index1 + Index2) : (UINT8)(-FLASH_ERASE_POLARITY);
            if( Buffer[Index2] != Data )
                return FALSE;    
        }
    }

    return TRUE;
}

/**
    This function converts the flash address to its physical address

    @param Address specified flash address need to be converted
    @param CnvtAddress specified flash address after converted

    @retval EFI_SUCCESS Address converted successfully
    @retval EFI_INVALID_PARAMETER Invalid Address
**/
EFI_STATUS
CommonConvertSpiAddress(
    IN  volatile UINT8  *Address,
    OUT UINT32          *CnvtAddress
)
{
    UINTN   BiosBase = gBiosRegionBase;
    UINTN   BiosLimit = gBiosRegionLimit;
    UINTN   FlashLimit = gMaxFlashLimit;

    // if flash identified, here checks the BIOS region of Flash Descriptor Table.
    if( gBiosRegionLimit == 0 )
    {
        // Do not call the GetFlashRegion() with "static" variable, the "static" 
        // variable can't be updated if Pre-Memory drivers of PEI phase.
        if( EFI_ERROR(GetFlashRegion( FlashRegionBios, &BiosBase, &BiosLimit )) )
        {
            // No Flash Descriptor mode, use Flash Capacity as BiosRegion.
            FlashLimit = (gExFlashPart.FlashCapacity - 1);
            BiosLimit = FlashLimit;
        }
        else
            GetMaxRegionLimit( &FlashLimit );

        gBiosRegionBase = BiosBase;
        gBiosRegionLimit = BiosLimit;
        gMaxFlashLimit = FlashLimit;
    }

    if( gDeviceVirtual )
    {
        // Address - offset from Flash Device Base.
        Address -= gAmiFlashDeviceBase;
        // Address - 32bit memory mapping address.
        Address += (0xFFFFFFFF - FLASH_SIZE) + 1;
    }

    // Address - physical address in the flash.
    Address += (BiosLimit + 1);
    // check if address inside the flash region.
    if( (UINT32)(UINTN)Address > (UINT32)FlashLimit )
        return EFI_INVALID_PARAMETER;

    *CnvtAddress = (UINT32)(UINTN)Address;
    return EFI_SUCCESS;
}

/**
    This function remove the write status Opcode from the Opmenu

    @param FlashInfo SPI Flash Information Table.
**/
VOID
RemoveWriteStatusOpcode(
    IN  OUT FLASH_INFO  *FlashInfo
)
{
#if SPI_USE_HARDWARE_SEQUENCE != 2
    if( InitializeSpiBase() == FALSE )
        return;

    // Using current SPI settings if dummy structure.
    if( FlashInfo->ReadId.Opcode == 0 )
        return;

    FlashInfo->WriteStatus.Opcode = FlashInfo->ReadId.Opcode;
    FlashInfo->WriteStatus.OpcodeType = FlashInfo->ReadId.OpcodeType;

    //Program second DWORD of Opcode commands
    *((volatile UINT32*)(gSpiBase + gSpiRegs->OpCodeMenu + 4)) = (UINT32)
        // Read device ID
      ((FlashInfo->ReadId.Opcode << ((SPI_OPCODE_READ_ID_INDEX - 4) * 8)) | \
        // Write Status Register
       (FlashInfo->WriteStatus.Opcode << \
                                    ((SPI_OPCODE_WRITE_S_INDEX - 4) * 8)) | \
        // Write Status Enable Register
       (FlashInfo->WriteStatusEnable.Opcode << \
                                    ((SPI_OPCODE_WRITE_S_E_INDEX - 4) * 8)) | \
        // Serial Flash Discoverable Paramenters Register
       (SPI_READ_SFDP_COMMAND << ((SPI_OPCODE_READ_SFDP_INDEX - 4) * 8)));

    //Program opcode types
    *((volatile UINT16*)(gSpiBase + gSpiRegs->OpCodeType)) = (UINT16)
        // write with address. (Clear to 0 for un-safe opcode if Hardware Sequence)
        (((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Write.OpcodeType) << (SPI_OPCODE_WRITE_INDEX * 2) | \
        // read with address. (Clear to 0 for un-safe opcode if Hardware Sequence)
         ((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Read.OpcodeType) << (SPI_OPCODE_READ_INDEX * 2) | \
        // write with address. (Clear to 0 for un-safe opcode if Hardware Sequence)
         ((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Erase.OpcodeType) << (SPI_OPCODE_ERASE_INDEX * 2) | \
        // read w/o no adress.
         FlashInfo->ReadStatus.OpcodeType << (SPI_OPCODE_READ_S_INDEX * 2) | \
        // read with address.
         FlashInfo->ReadId.OpcodeType << (SPI_OPCODE_READ_ID_INDEX * 2) | \
        // write w/o address.
         FlashInfo->WriteStatus.OpcodeType << (SPI_OPCODE_WRITE_S_INDEX * 2) | \
        // write w/o address.
         FlashInfo->WriteStatusEnable.OpcodeType << (SPI_OPCODE_WRITE_S_E_INDEX * 2) | \
        // Read with address.
         SPI_OPCODE_TYPE_READ_WITH_ADDRESS << (SPI_OPCODE_READ_SFDP_INDEX * 2));
#endif

    return;
}

/**
    This function initials the SPI OpCode/Type to SPI registers. 

    @param FlashInfo SPI Flash Information Table.
**/
BOOLEAN
InitializeSpiEnvironment(
    IN  OUT FLASH_INFO  *FlashInfo
)
{
    if( InitializeSpiBase() == FALSE )
        return FALSE;

#if SPI_USE_HARDWARE_SEQUENCE != 2
    // Using current SPI settings if dummy structure.
    if( FlashInfo->ReadId.Opcode == 0 )
        return FALSE;

    //Program first DWORD of opcode commands
    *((volatile UINT32*)(gSpiBase + gSpiRegs->OpCodeMenu + 0)) = (UINT32)
        // Write Byte (Clear to 0 for un-safe opcode if Hardware Sequence)
        ( (((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Write.Opcode) << (SPI_OPCODE_WRITE_INDEX * 8)) | \
        // Read Data (Clear to 0 for un-safe opcode if Hardware Sequence)
          (((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Read.Opcode) << (SPI_OPCODE_READ_INDEX * 8)) | \
        // Erase 64k Sector (Clear to 0 for un-safe opcode if Hardware Sequence)
          (((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Erase.Opcode) << (SPI_OPCODE_ERASE_INDEX * 8)) | \
        // Read Device Status Reg
          (FlashInfo->ReadStatus.Opcode << (SPI_OPCODE_READ_S_INDEX * 8)) );

    //Program second DWORD of Opcode commands
    *((volatile UINT32*)(gSpiBase + gSpiRegs->OpCodeMenu + 4)) = (UINT32)
        // Read device ID
      ((FlashInfo->ReadId.Opcode << ((SPI_OPCODE_READ_ID_INDEX - 4) * 8)) | \
        // Write Status Register
       (FlashInfo->WriteStatus.Opcode << \
                                    ((SPI_OPCODE_WRITE_S_INDEX - 4) * 8)) | \
        // Write Status Enable Register
       (FlashInfo->WriteStatusEnable.Opcode << \
                                    ((SPI_OPCODE_WRITE_S_E_INDEX - 4) * 8)) | \
        // Serial Flash Discoverable Paramenters Register
       (SPI_READ_SFDP_COMMAND << ((SPI_OPCODE_READ_SFDP_INDEX - 4) * 8)));

    //Program opcode types
    *((volatile UINT16*)(gSpiBase + gSpiRegs->OpCodeType)) = (UINT16)
        // write with address. (Clear to 0 for un-safe opcode if Hardware Sequence)
        (((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Write.OpcodeType) << (SPI_OPCODE_WRITE_INDEX * 2) | \
        // read with address. (Clear to 0 for un-safe opcode if Hardware Sequence)
         ((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Read.OpcodeType) << (SPI_OPCODE_READ_INDEX * 2) | \
        // write with address. (Clear to 0 for un-safe opcode if Hardware Sequence)
         ((SPI_USE_HARDWARE_SEQUENCE)? 0 : FlashInfo->Erase.OpcodeType) << (SPI_OPCODE_ERASE_INDEX * 2) | \
        // read w/o no adress.
         FlashInfo->ReadStatus.OpcodeType << (SPI_OPCODE_READ_S_INDEX * 2) | \
        // read with address.
         FlashInfo->ReadId.OpcodeType << (SPI_OPCODE_READ_ID_INDEX * 2) | \
        // write w/o address.
         FlashInfo->WriteStatus.OpcodeType << (SPI_OPCODE_WRITE_S_INDEX * 2) | \
        // write w/o address.
         FlashInfo->WriteStatusEnable.OpcodeType << (SPI_OPCODE_WRITE_S_E_INDEX * 2) | \
        // Read with address.
         SPI_OPCODE_TYPE_READ_WITH_ADDRESS << (SPI_OPCODE_READ_SFDP_INDEX * 2));

    //set up the prefix opcodes for commands
    *((volatile UINT16*)(gSpiBase + gSpiRegs->PreOpCode)) = (UINT16)
        ((FlashInfo->WriteStatusEnable.Opcode << 8) | \
                                    (FlashInfo->WriteEnable.Opcode));

    //set up Program Opcode and Optype if AAIWordProgram.
    if( gExFlashPart.AAIWordProgram != 0 )
    {
        UINT8   OpType = 0;
        *((volatile UINT16*)(gSpiBase + gSpiRegs->OpCodeMenu + 6)) = 0x4AD; 
        OpType = *((volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeType + 1));
        OpType = ((OpType & 0xF) | \
                 (SPI_OPCODE_TYPE_WRITE_NO_ADDRESS << 6) | \
                 (SPI_OPCODE_TYPE_WRITE_WITH_ADDRESS << 4));
        *((volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeType + 1)) = OpType;
    }
#if defined(FAST_READ_SUPPORT) && (FAST_READ_SUPPORT != 0)
#if defined(SPI_FREQUENCY) && (SPI_FREQUENCY != 0)
    // Here to sets frequency to use for all SPI software sequencing cycles.
    {
        UINT32  SwStatusRegr = 0;
        SwStatusRegr = *((volatile UINT32*)(gSpiBase + gSpiRegs->SwSeqSts));
        SwStatusRegr &= ~ (BIT26 | BIT25 | BIT24);
        SwStatusRegr |= (SPI_FREQUENCY << 24);
        *((volatile UINT32*)(gSpiBase + gSpiRegs->SwSeqSts)) = SwStatusRegr;
    }
#endif // #if defined SPI_FREQUENCY && SPI_FREQUENCY != 0
#endif // #if defined FAST_READ_SUPPORT && FAST_READ_SUPPORT != 0
    //Revmoe zero from Opmenu
    RemoveZeroOpcode( FlashInfo );
#endif // #if SPI_USE_HARDWARE_SEQUENCE != 2
    return TRUE;
}

#if defined(SPI_INITIALIZE_WITH_VSCC) && (SPI_INITIALIZE_WITH_VSCC == 1)
/**
    This function checks whether SPI Quad Read support through SFDP or not.

    @retval EFI_SUCCESS Flash part have the Quad Read support
    @retval EFI_UNSUPPORTED Flash part have not the Quad Read support
**/
EFI_STATUS
ProgramVsccByDescriptor(
    VOID
)
{
    UINT8       VsccBuffer[SPI_MAX_DATA_TRANSFER];
    UINT32      VsccTable;
    UINT32      VsccLength;
    UINT32      MaxRead;
    UINT32      Index1;
    UINT32      Index2;
    EFI_STATUS  Status;

    //Read Flash Upper Map
    SetMem( (VOID*)VsccBuffer, SPI_MAX_DATA_TRANSFER, 0 );
    Status = FlashRegionOperation(
                FlashRegionDescriptor,
                FlashRegionRead,
                0xEFC,
                sizeof(UINT32),
                VsccBuffer );
    if( EFI_ERROR(Status) ||
        (*(UINT32*)VsccBuffer == 0) ||
        (*(UINT32*)VsccBuffer == -1) )
        return Status;

    // VSCC Table Base Address represents address bits [11:4]
    VsccTable = (*(UINT32*)VsccBuffer & 0xFF) << 4;
    // VSCC Table Length is the 1-based number of DWORDs.
    VsccLength = (*(UINT32*)VsccBuffer & 0xFF00) >> 6;

    for( Index1 = 0; Index1 < VsccLength; Index1 += SPI_MAX_DATA_TRANSFER )
    {
        SetMem( (VOID*)VsccBuffer, SPI_MAX_DATA_TRANSFER, 0 );
        MaxRead = ((VsccLength - Index1) >= SPI_MAX_DATA_TRANSFER) ? \
                                    SPI_MAX_DATA_TRANSFER : (VsccLength - Index1);
        // Read VSCC Table
        Status = FlashRegionOperation(
                    FlashRegionDescriptor,
                    FlashRegionRead,
                    VsccTable + Index1,
                    MaxRead,
                    VsccBuffer );
        if( EFI_ERROR(Status) )
            return Status;

        for( Index2 = 0; Index2 < MaxRead; Index2 += 8 )
        {
            if( *(UINT32*)(VsccBuffer + Index2) != gExFlashPart.FlashVenDevId )
                continue;

            VsccTable = *(UINT32*)(VsccBuffer + Index2 + 0x04);
            if( gSpiRegs->BiosLVscc != 0 )
                *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosLVscc) = VsccTable;
            if( gSpiRegs->BiosUVscc != 0 )
                *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosUVscc) = VsccTable;
            return EFI_SUCCESS;
        }
    }

    return EFI_NOT_FOUND;
}

/**
    This function checks whether SPI Quad Read support through SFDP or not.

    @retval EFI_SUCCESS Flash part have the Quad Read support
    @retval EFI_UNSUPPORTED Flash part have not the Quad Read support
**/
EFI_STATUS
SerialFlashDiscoveryForQuadRead(
    VOID
)
{
    UINT8   SfdpData[256]; 
    UINT16  SpiCmd;
    UINT16  Offset;
    UINT32  Index1;
    UINT16  Index2;

    SetMem( (VOID*)SfdpData, 256, 0 );
    for( Index1 = 0; Index1 < 256; Index1 += 64 )
    {
        // Set SPI read-address = 0, 64, 128, 256 
        *(volatile UINT32*)(gSpiBase + gSpiRegs->FlashAddr) = Index1;
#if (SPI_USE_HARDWARE_SEQUENCE == 2)
        // Flash SPI Cycle type 5 generated to "Read SFDP"
        SpiCmd = ((64 - 1) << 8);
        SpiCmd += ((SPI_HW_SEQ_READ_SFDP << 1) + BIT0);
        *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl + 2) = SpiCmd;
        // Wait for spi hardware sequence cycle completed.
        WaitForSpiCycleDone( TRUE, FALSE );
#else // #if SPI_USE_HARDWARE_SEQUENCE == 0 OR 1
        // set opcode for "5A"
        SpiCmd = SPI_OPCODE_READ_SFDP_INDEX << 4;
        // set transaction = 64 bytes
        SpiCmd += ((64 - 1) << 8);
        // indicate that data phase is required
        SpiCmd += (1 << 14);
        // Go (BIT1)
        *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) = SpiCmd + BIT1;
        // Wait for spi software sequence cycle completed.
        WaitForSpiCycleDone( FALSE, TRUE );
#endif // #if SPI_USE_HARDWARE_SEQUENCE == 2
        SpiIoDelay();
        for( Index2 = 0; Index2 < 64; Index2++ )
            *(SfdpData + Index1 + Index2) = *(volatile UINT8*)(gSpiBase + gSpiRegs->FlashData + Index2);
        // Serial Flash Discoverable Parameters (SFDP) Signature = 50444653h
        if( Index1 == 0 )
        {
            Offset = 0;
            if( *(UINT32*)&SfdpData[Offset] != 0x50444653 )
            {
                if( *(UINT32*)&SfdpData[++Offset] != 0x50444653 )
                {
                    if( gSpiRegs->BiosPTInx == 0 )
                        return EFI_UNSUPPORTED;
                    // Try to read the "SFDP" from SPI Parameter Table Register
                    *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosPTInx) = 0;
                    MicroSecondDelay( 1 );
                    Index1 = *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosPTData + 4);
                    if( Index1 == 0x50444653 )
                        return EFI_SUCCESS;
                    return EFI_UNSUPPORTED;
                }
            }
        }
    }
    // SFDP opode at address Ch bits 23:00 = Parameter ID 0 table Address
    Index1 = (*(UINT32*)&SfdpData[Offset + 0xC] & 0x00FFFFFF);
    // SFDP opode at address 05h(SFPD Major Revisions) must = 0001h
    // SFDP opode at address 0Ah(Serial Flash Basic Major Revisions) must = 0001h
    if( (SfdpData[Offset + 5] != 0x1) ||
        (SfdpData[Offset + 0xA] != 0x1) ||
        ((Index1 + 0x10) > 256) )
        return EFI_UNSUPPORTED;
    // Parameter ID 0 Table BIT[21] - Fast Read Quad I/O.
    // Parameter ID 0 Table BIT[22] - Fast Read Quad Output.
    if( *(UINT32*)&SfdpData[Index1 + 1] & (BIT21 + BIT22) )
        return EFI_SUCCESS;

    return EFI_UNSUPPORTED;
}

/**
    This function re-initials the SPI OpCode/Type to SPI registers. 

    @param FlashInfo SPI Flash Information Table.
**/
VOID
ProgramVsccBySfdp(
    IN  OUT FLASH_INFO  *FlashInfo
)
{
    // Program UPPER/LOWER VSCC register.
    UINT32      Vscc;
    EFI_STATUS  Status;

    // Erase Opcode.
    Vscc = FlashInfo->Erase.Opcode << 8;
    // Block/Sector Erase Size.
    if( FlashInfo->SectorSize == SECTOR_SIZE_4KB )
        Vscc |= BIT0;
    else if( FlashInfo->SectorSize == SECTOR_SIZE_8KB )
        Vscc |= BIT1;
    else if( FlashInfo->SectorSize == SECTOR_SIZE_64KB )
        Vscc |= (BIT0 + BIT1);
    // Write Granularity.
    if( FlashInfo->PageSize != 1 )
        Vscc |= BIT2;
    // Write Status Required.
    if( FlashInfo->WriteStatusEnable.Opcode == 0x50 )
        Vscc |= BIT3;
    // Write Enable On Write Status.
    if( FlashInfo->WriteStatusEnable.Opcode == 0x39 )
        Vscc |= BIT4;
    Status = SerialFlashDiscoveryForQuadRead();
    // Updated by Intel VSCCommn.bin Content Rev# 3.9 (2014)
    if( !EFI_ERROR(Status) )
    {
        switch( (UINT8)gExFlashPart.FlashVenDevId )
        {
            case 0xC2 : // MXIC
                Vscc |= BIT6;
                break;
            case 0x1F : // Atmel (BIT4 - Write Enable on Write to Status "06h")
                Vscc |= BIT4;
                // Only DQ series (DID:0x8x) w/ Quad Mode support.
                if( ((gExFlashPart.FlashVenDevId >> 12) & 0xF) != 8 )
                    break;
                Vscc |= (BIT5 + BIT6);
                break;
            case 0xBF : // SST/Microchip
                Vscc |= BIT7;
                break;
            case 0x20 : // Numonyx/Micron
            case 0x1C : // EON
            case 0x8C : // ESMT
                break ;
            case 0x37 : // AMIC (Only LQ series (DID:0x40) w/ Quad Mode support)
                if( (UINT8)(gExFlashPart.FlashVenDevId >> 8) != 0x40 )
                    break;
            default :   // Winbond, Spansion, GigaDevice, Fidelix
                Vscc |= BIT5;
                break;
        }
    }    
    if( Status != EFI_WRITE_PROTECTED )
    {
        if( gSpiRegs->BiosLVscc != 0 )
            *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosLVscc) = Vscc;
        if( gSpiRegs->BiosUVscc != 0 )
            *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosUVscc) = Vscc;
    }
}
#endif // #if defined SPI_INITIALIZE_WITH_VSCC && SPI_INITIALIZE_WITH_VSCC == 1

/**
    This procedure unlocks "Global Block Protection" for Microchip/SST 26VF flash parts. 
**/
VOID
GlobalBlockProtectionUnlock(
    VOID
)
{
    UINT8   SavedOpType;
    UINT8   SavedOpCode;

    if( gExFlashPart.GlobalBlockUnlock != TRUE )
        return;

    SavedOpCode = *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeMenu + 7);
    SavedOpType = *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeType + 1);
    // Microchip/SST "Global Block Protection" Command "0x98".
    *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeMenu + 7) = 0x98;
    // Do nothing if SPI Registers is locked.
    if( *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeMenu + 7) != 0x98 )
        return;
    // Update SPI OpMenu[7] to "WRITE W/O ADDRESS".
    *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeType + 1) = \
                                                (SavedOpType & 0x3F) | BIT6;
    // Opcode menu slot 7 is configured as "Write Disable" if AAIWordProgram.
    *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) = \
                                    (SPI_OPCODE_WRITE_D_INDEX << 4) + BIT1;
    // wait for chipset SPI cycle completed.
    WaitForSpiCycleDone( FALSE, TRUE );
    *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeMenu + 7) = SavedOpCode;
    *(volatile UINT8*)(gSpiBase + gSpiRegs->OpCodeType + 1) = SavedOpType;
    return;
}

/**
    This function re-initials the SPI OpCode/Type to SPI registers. 

    @param FlashInfo SPI Flash Information Table.
**/
VOID
ReinitializeSpiEnvironment(
    IN  OUT FLASH_INFO  *FlashInfo
)
{
    if( InitializeSpiEnvironment( FlashInfo ) == FALSE )
        return;
#if defined(SPI_INITIALIZE_WITH_VSCC) && (SPI_INITIALIZE_WITH_VSCC == 1)
    if( EFI_ERROR(ProgramVsccByDescriptor()) )
        ProgramVsccBySfdp( FlashInfo );
#endif // #if defined SPI_INITIALIZE_WITH_VSCC && SPI_INITIALIZE_WITH_VSCC == 1
    GlobalBlockProtectionUnlock ();
    //Update SPI status register to make SPI device writable
    SpiBlockProtectUpdate( 0, 0, 0 );
    //Remove SPI write status Opcode from Opmenu
    RemoveWriteStatusOpcode( FlashInfo );
    //Revmoe zero from Opmenu
    RemoveZeroOpcode( FlashInfo );
}

/**
    This function read the Manufacturer/Device ID from flash part.

    @param FlashInfo SPI Flash Information Table.
    @param FlashAddress Flash address of the flash part. Can be used to
                        send ID command

    @retval Value of the Manufacturer/Device ID to be read from flash part
**/
UINT32
CommonSpiReadIdEx(
    IN  FLASH_INFO  *FlashInfo,
    IN  UINT32      FlashAddress
)
{
    UINT16  SpiCmd;

    // Set SPI read-address = 0
    *(volatile UINT32*)(gSpiBase + gSpiRegs->FlashAddr) = 0;
#if SPI_USE_HARDWARE_SEQUENCE == 2
    // Flash SPI Cycle type 6 generated to "Read JEDEC ID"
    SpiCmd = ((3 - 1) << 8);
    SpiCmd += ((SPI_HW_SEQ_READ_ID << 1) + BIT0);
    *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl + 2) = SpiCmd;
    // Wait for spi hardware sequence cycle completed.
    WaitForSpiCycleDone( TRUE, FALSE );
#else // #if SPI_USE_HARDWARE_SEQUENCE == 0 OR 1
    // set opcode for "Read ID"
    SpiCmd = SPI_OPCODE_READ_ID_INDEX << 4;
    // set transaction = 3 bytes
    SpiCmd += ((3 - 1) << 8);
    // indicate that data phase is required
    SpiCmd += (1 << 14);
    // Go (BIT1)
    *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) =  SpiCmd + BIT1;
    // Wait for spi software sequence cycle completed.
    WaitForSpiCycleDone( FALSE, TRUE );
#endif // #if SPI_USE_HARDWARE_SEQUENCE == 2
    SpiIoDelay();
    return (*(volatile UINT32*)(gSpiBase + gSpiRegs->FlashData) & 0x00FFFFFF);
}

/**
    This function read the Manufacturer/Device ID from flash part.

    @param FlashInfo SPI Flash Information Table.
    @param FlashId ID of the flash part.

    @retval TRUE Read Flash ID completed
**/
BOOLEAN
CommonSpiReadId(
    IN      FLASH_INFO  *FlashInfo,
    IN  OUT UINT32      *FlashId
)
{
    if( InitializeSpiEnvironment( FlashInfo ) == FALSE )
        return FALSE;

    *FlashId = CommonSpiReadIdEx( FlashInfo, 0 );
    return TRUE;
}

/**
    This API function erases a block in the flash. Flash model specific code
    will branch out from this routine

    @param BlockAddress Block that need to be erased
    @param Length Number of bytes that need to be erased
    @param IsMappedAddress Input param BlockAddress is mapping address or not

    @retval EFI_SUCCESS erase complete
    @retval EFI_DEVICE_ERROR erase not completed
**/
EFI_STATUS
EFIAPI
CommonSpiEraseCommandEx(
    IN  volatile UINT8  *BlockAddress,
    IN  UINT32          Length,
    IN  BOOLEAN         IsMappedAddress
)
{
    volatile UINTN  SectorAddr;
    volatile UINT32 FlashOffset;
    UINT16          EraseRetry;
    UINT16          SpiCmd;
    BOOLEAN         IsClean;
    UINT32          ProcessLength;
    UINT32          CompleteLength;
    static BOOLEAN  TryErase64K = TRUE;

    if( InitializeSpiEnvironment( &gExFlashPart.FlashCommandMenu ) == FALSE )
        return EFI_INVALID_PARAMETER;

    if( Length % gFlashApi->FlashSectorSize )
        return EFI_INVALID_PARAMETER;

    SectorAddr = (UINTN)BlockAddress;
    CompleteLength = 0;
    //Check 64K earase command available
    if( TryErase64K == TRUE )
    {
        TryErase64K = IsErase64KAvailable();
    }

    for( ; CompleteLength < Length; SectorAddr += ProcessLength, CompleteLength += ProcessLength )
    {
#if defined(SPI_USE_HARDWARE_SEQUENCE) && (SPI_USE_HARDWARE_SEQUENCE == 2)
        if( (TryErase64K == TRUE) &&
            (SectorAddr % SECTOR_SIZE_64KB == 0) &&
            ((Length - CompleteLength) >= SECTOR_SIZE_64KB) )
        {
            //If 64K earase command is available, 
            //address is alignment on 64K and rest length is large than 64K
            ProcessLength = SECTOR_SIZE_64KB;
        }
        else
#endif
        {
            //Other cases keep using 4K erase
            ProcessLength = SECTOR_SIZE_4KB;
        }

        if( IsMappedAddress == TRUE )
        {
            if( EFI_ERROR(CommonConvertSpiAddress( (volatile UINT8*)SectorAddr, (UINT32*)(&FlashOffset) )) )
                return EFI_INVALID_PARAMETER;
        }
        else
            FlashOffset = (UINT32)SectorAddr;

        IsClean = IsEqualOrClean(
                    (volatile UINT8*)SectorAddr,
                    NULL,
                    ProcessLength,
                    TRUE,
                    IsMappedAddress );
        if( IsClean == TRUE )
            continue;

        for( EraseRetry = 0; EraseRetry < FLASH_RETRIES; EraseRetry++ )
        {
            // Wait for spi hardware/software sequence cycle completed.
            WaitForSpiCycleDone( TRUE, TRUE );
            *(volatile UINT32*)(gSpiBase + gSpiRegs->FlashAddr) = FlashOffset;
#if defined(SPI_USE_HARDWARE_SEQUENCE) && (SPI_USE_HARDWARE_SEQUENCE != 0)
            if( ProcessLength == SECTOR_SIZE_4KB )
            {
                // Flash SPI Cycle type 3 generated to "4K Block Erase"
                SpiCmd = (SPI_HW_SEQ_ERASE_4K << 1) + BIT0;
            }
#if defined(SPI_USE_HARDWARE_SEQUENCE) && (SPI_USE_HARDWARE_SEQUENCE == 2)
            if( (gFlashApi->FlashSectorSize == SECTOR_SIZE_64KB) ||
                (ProcessLength == SECTOR_SIZE_64KB) )
            {
                // Flash SPI Cycle type 4 generated to "64K Block Erase"
                SpiCmd = (SPI_HW_SEQ_ERASE_64K << 1) + BIT0;
            }
#endif // #if SPI_USE_HARDWARE_SEQUENCE == 2
            *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl + 2) = SpiCmd;
            // Wait for spi hardware sequence cycle completed.
            WaitForSpiCycleDone( TRUE, FALSE );
#else // #if SPI_USE_HARDWARE_SEQUENCE == 0
            // opcode index 2 is programmed for erase command.
            // Set BIT1 (Go), BIT2(Atomic w/ Prefix)
            SpiCmd = (SPI_OPCODE_ERASE_INDEX << 4) + BIT1 + BIT2;
            *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) = SpiCmd;
            // Wait for spi software sequence cycle completed.
            WaitForSpiCycleDone( FALSE, TRUE );
#endif // #if SPI_USE_HARDWARE_SEQUENCE != 0
            // wait for SPI flash operation completed.
            WaitForWriteOperationCompleted();
#if defined(ENABLE_ROM_CACHE_IN_DXE) && (ENABLE_ROM_CACHE_IN_DXE == 1)
            // Clear cache for this range only when BIOS region is modified.
            if( (FlashOffset >= gBiosRegionBase) && (FlashOffset <= gBiosRegionLimit) )
            {
                UINTN   BiosAddress;
                //Convert physical flash part address to 4G mapping address
                BiosAddress = 0xFFFFFFFF - gBiosRegionLimit + FlashOffset;
                if( ProcessLength == SECTOR_SIZE_64KB )
                    WriteBackInvalidateDataCacheRange( (VOID*)BiosAddress, SECTOR_SIZE_64KB );
                else
                    WriteBackInvalidateDataCacheRange( (VOID*)BiosAddress, gFlashApi->FlashSectorSize );
            }
#endif
            //Check flash part is clean now
            IsClean = IsEqualOrClean(
                        (volatile UINT8*)SectorAddr,
                        NULL,
                        ProcessLength,
                        TRUE,
                        IsMappedAddress );
            if( IsClean == TRUE )
                break;
        }
        if( IsClean == FALSE )
        {
            if( ProcessLength == SECTOR_SIZE_64KB )
            {
                //If 64K erase failed, set ProcessLength to 0 and TryErase64K to FALSE
                //for erasing same address with 4K erase command
                ProcessLength = 0;
                TryErase64K = FALSE;
                continue;
            }
            return EFI_DEVICE_ERROR;
        }
        // According to Intel Server Platform Services Integration Guide, 
        // Section#5.3.5, Waits 10 milliseconds delay while erase blocks.
        MicroSecondDelay( 10 );
    }

    return EFI_SUCCESS;
}

/**
    This API function erases a block in the flash. Flash model specific code
    will branch out from this routine

    @param BlockAddress Block that need to be erased
**/
BOOLEAN
EFIAPI
CommonSpiEraseCommand(
    IN  volatile UINT8  *BlockAddress
)
{
    return (EFI_ERROR(CommonSpiEraseCommandEx( BlockAddress, gFlashBlockSize, TRUE ))) ? FALSE : TRUE;
}

/**
    This function programs a byte data to the specified location

    @param Address Location where the data to be written
    @param Byte Byte to be written
    @param Length Number of bytes to write
    @param IsMappedAddress Input param Address is mapping address or not
**/
BOOLEAN
EFIAPI
CommonSpiProgramCommand(
    IN      volatile UINT8  *Address,
    IN      UINT8           *Byte,
    IN  OUT UINT32          *Length,
    IN      BOOLEAN         IsMappedAddress
)
{
    UINT8   FlashRetry;
    UINT16  ProgBytes;
    UINT16  Index;
    UINT16  SpiCmd;
    UINT16  Retry;
    UINT16  MaxNumBytes;
    UINT32  FlashOffset;

    if( InitializeSpiEnvironment( &gExFlashPart.FlashCommandMenu ) == FALSE )
        return FALSE;

    ProgBytes = (UINT16)gCommonSpiFlash.FlashProgramSize;
    if( gCommonSpiFlash.FlashProgramSize != 1 )
    {
        // Limit the max transfer to the number of bytes the chipset can
        // transfer per cycle
        if( *Length >= SPI_MAX_DATA_TRANSFER )
            ProgBytes = SPI_MAX_DATA_TRANSFER;
        else
            ProgBytes = (UINT16)*Length;
        // this is currently for the WINBOND parts only
        // mask off lowest 8 bits of address so that we can determine how
        // many bytes we can write before we hit the end of a page
        MaxNumBytes = 0x100 - ((UINT32)(UINTN)Address & 0xFF);
        if( (UINT32)(UINTN)Address & 0x1 )
            ProgBytes = 1;
        else if( ProgBytes > MaxNumBytes )
            ProgBytes = MaxNumBytes;
    }
    else if( CheckAaiWordProram( (UINT32)(UINTN)Address, *Length ) )
        ProgBytes = 2;

    for( FlashRetry = 0; FlashRetry < FLASH_RETRIES; FlashRetry++ )
    {
        //Skip to program if input buffer is clean
        if( IsEqualOrClean( (volatile UINT8*)Byte, NULL, ProgBytes, FALSE, FALSE ) )
            break;

        //Program input data
        for( Index = 0; Index < ProgBytes; Index++ )
        {
            for( Retry = 0; Retry < (FLASH_RETRIES * 100); Retry++ )
            {
                *(volatile UINT8*)(gSpiBase + gSpiRegs->FlashData + Index) = *(Byte + Index);
                if( *(Byte + Index) == *(gSpiBase + gSpiRegs->FlashData + Index) )
                    break;
            }
        }
        //Wait for spi hardware/software sequence cycle completed.
        WaitForSpiCycleDone( TRUE, TRUE );
        if( IsMappedAddress == TRUE )
        {
            if( EFI_ERROR(CommonConvertSpiAddress( Address, &FlashOffset )) )
                return FALSE;
        }
        else
            FlashOffset = (UINT32)(UINTN)Address;

        *(volatile UINT32*)(gSpiBase + gSpiRegs->FlashAddr) = FlashOffset;
#if SPI_USE_HARDWARE_SEQUENCE != 0
        // Flash SPI Cycle type 2 generated to "Write"
        SpiCmd = (SPI_HW_SEQ_WRITE << 1) + BIT0;
        // BIT[8..13] - update the number of bytes to be read.
        SpiCmd += (ProgBytes - 1) << 8;
        *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl + 2) = SpiCmd;
        // Wait for spi hardware sequence cycle completed.
        WaitForSpiCycleDone( TRUE, FALSE );
#else // #if SPI_USE_HARDWARE_SEQUENCE = 0
        // BIT14 - indicate that it's data cycle.
        SpiCmd = (1 << 14);
        // BIT[8..13] - update the number of bytes to be written.
        SpiCmd += (ProgBytes - 1) << 8;
        // opcode index 0 is programmed for program command.
        // Set BIT1 (Go), BIT2(Atomic w/ Prefix)
        if( CheckAaiWordProram( FlashOffset, ProgBytes ) )
            SpiCmd += ((SPI_OPCODE_AAI_INDEX << 4) + BIT1 + BIT2);
        else
            SpiCmd += ((SPI_OPCODE_WRITE_INDEX << 4) + BIT1 + BIT2);
        *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) = SpiCmd;
        // Wait for spi software sequence cycle completed.
        WaitForSpiCycleDone( FALSE, TRUE );
#endif // #if SPI_USE_HARDWARE_SEQUENCE != 0
        // wait for chipset SPI flash operation completed.
        WaitForWriteOperationCompleted();

#if defined(ENABLE_ROM_CACHE_IN_DXE) && (ENABLE_ROM_CACHE_IN_DXE == 1)
        // Clear cache for this range only when BIOS region is modified.
        if( (FlashOffset >= gBiosRegionBase) && (FlashOffset <= gBiosRegionLimit) )
        {
            UINTN   BiosAddress;
            //Convert physical flash part address to 4G mapping address
            BiosAddress = 0xFFFFFFFF - gBiosRegionLimit + FlashOffset;
            WriteBackInvalidateDataCacheRange( (VOID *) BiosAddress, ProgBytes );
        }
#endif

        // Issue SPI Write Disable if SST AAIWordProgram supported.
        if( CheckAaiWordProram( FlashOffset, ProgBytes ) )
            SpiWriteDisable();
        // write operation appeared to succeed, now read back byte and compare
        // set control for 1-byte data read, no prefix
        if( IsEqualOrClean( Address, Byte, ProgBytes, TRUE, IsMappedAddress ) )
            break;
        if( FLASH_RETRIES == (FlashRetry + 1) )
            return FALSE;
    }
    // Don't forget to return the number of bytes not written
    *Length = *Length - (UINT32)ProgBytes;
    return TRUE;
}

/**
    This function read data from the specified location

    @param Address Location where the data to be read
    @param Byte Data buffer to be read
    @param Length Number of bytes to read
    @param IsMappedAddress Input param Address is mapping address or not
**/
VOID
EFIAPI
CommonSpiReadCommand(
    IN      volatile UINT8  *Address,
    OUT     UINT8           *Byte,
    IN  OUT UINT32          *Length,
    IN      BOOLEAN         IsMappedAddress
)
{
    UINT32  FlashOffset; 
    UINT16  SpiCmd;
    UINT16  Index;
    UINT16  MaxRead;

    if( InitializeSpiEnvironment( &gExFlashPart.FlashCommandMenu ) == FALSE )
        return;

    if( IsMappedAddress == TRUE )
    {
        if( EFI_ERROR(CommonConvertSpiAddress( Address, &FlashOffset )) )
            return;
    }
    else
        FlashOffset = (UINT32)(UINTN)Address;

    MaxRead = 0x100 - (FlashOffset & 0xFF);
    if( MaxRead > SPI_MAX_DATA_TRANSFER ) MaxRead = SPI_MAX_DATA_TRANSFER;
    if( MaxRead > *Length ) MaxRead = (UINT16)*Length;
    // Wait for spi hardware/software sequence cycle completed.
    WaitForSpiCycleDone( TRUE, TRUE );
    // update the read address.
    *(volatile UINT32*)(gSpiBase + gSpiRegs->FlashAddr) = FlashOffset;
#if SPI_USE_HARDWARE_SEQUENCE != 0
    // Flash SPI Cycle type 0 generated to "Read"
    SpiCmd = ( SPI_HW_SEQ_READ << 1 ) + BIT0;
    // BIT[8..13] - update the number of bytes to be read.
    SpiCmd += ((MaxRead - 1) << 8);
    *(volatile UINT16*)(gSpiBase + gSpiRegs->HwSeqStsCtl + 2) = SpiCmd;
    // Wait for spi hardware sequence cycle completed.
    WaitForSpiCycleDone( TRUE, FALSE );
#else // #if SPI_USE_HARDWARE_SEQUENCE = 0
    // Opcode menu slot 1 is configured as "Read Flash"
    SpiCmd = ( SPI_OPCODE_READ_INDEX << 4 ) + BIT1;
    // indicate that data phase is required
    SpiCmd += (1 << 14);
    // BIT[8..13] - update the number of bytes to be read.
    SpiCmd += ((MaxRead - 1) << 8);
    *(volatile UINT16*)(gSpiBase + gSpiRegs->SwSeqCtrl) = SpiCmd;
    // Wait for spi software sequence cycle completed.
    WaitForSpiCycleDone( FALSE, TRUE );
#endif // #if SPI_USE_HARDWARE_SEQUENCE != 0
    // read data
    for( Index = 0; Index < MaxRead; Index++ )
        *(Byte + Index) = *(volatile UINT8*)(gSpiBase + gSpiRegs->FlashData + Index);
    *Length = *Length - (UINT32)MaxRead;
}

/**
    This function writes the status register by specified flash address

    @param FlashOffset Offset to access flash part
    @param Length Length to access flash part
    @param StatusData Data to be written to the status register.
**/
VOID
SpiBlockProtectUpdate(
    IN  UINT32  FlashOffset,
    IN  UINT32  Length,
    IN  UINT8   StatusData
)
{
    UINT8   StatusReg;
    UINT8   PrefixOp;
    UINT8   DataPhase = 1;
    UINT8   BpBits;

    StatusReg = CommonSpiReadStatus( FlashOffset );
    BpBits = (BIT2 + BIT3 + BIT4);
    switch( (UINT8)gExFlashPart.FlashVenDevId )
    {
        // if SST flash, prefix 1 w/o address
        case 0xBF :
            PrefixOp = SPI_PREFIX_WRITE_S_EN;
            break;
        // if ATMEL flash, prefix 0 w/ address
        case 0x1F :
            BpBits = (BIT2 + BIT3);
        default :
        // default flash, prefix 0 w/o address
            PrefixOp = SPI_PREFIX_WRITE_EN;
    }
    StatusReg &= BpBits;              // keep valid bits.
    StatusData &= BpBits;             // keep valid bits.
    if( StatusReg != StatusData )
    {
        CommonSpiWriteStatus(
            StatusData,
            SPI_OPCODE_WRITE_S_INDEX,
            DataPhase,
            PrefixOp,
            FlashOffset );
    }
}

/**
    This function contains any flash specific code need to enable a
    particular flash block write

    @param BlockAddress Location where the block register to be set
    @param Length Length Length to access flash part
    @param IsMappedAddress Input param BlockAddress is mapping address or not
**/
VOID
EFIAPI
CommonSpiBlockWriteEnable(
    IN  volatile UINT8  *BlockAddress,
    IN  UINT32          Length,
    IN  BOOLEAN         IsMappedAddress
)
{
    UINT32  FlashOffset;

    if( InitializeSpiEnvironment( &gExFlashPart.FlashCommandMenu ) == FALSE )
        return;

    if( IsMappedAddress == TRUE )
    {
        if( EFI_ERROR(CommonConvertSpiAddress( BlockAddress, &FlashOffset )) )
            return;
    }
    else
        FlashOffset = (UINT32)(UINTN)BlockAddress;
        
    SpiBlockProtectUpdate( FlashOffset, Length, 0 );
}

/**
    This function contains any flash specific code need to disable a 
    particular flash block write

    @param BlockAddress Location where the block register to be set
    @param Length Length Length to access flash part
    @param IsMappedAddress Input param BlockAddress is mapping address or not
**/
VOID
EFIAPI
CommonSpiBlockWriteDisable(
    IN  volatile UINT8  *BlockAddress,
    IN  UINT32          Length,
    IN  BOOLEAN         IsMappedAddress
)
{
    if( InitializeSpiEnvironment( &gExFlashPart.FlashCommandMenu ) == FALSE )
        return;
#if BLOCK_PROTECT_ENABLE
{
    UINT32  FlashOffset;

    if( InitializeSpiEnvironment( &gExFlashPart.FlashCommandMenu ) == FALSE )
        return;

    if( IsMappedAddress == TRUE )
    {
        if( EFI_ERROR(CommonConvertSpiAddress( BlockAddress, &FlashOffset )) )
            return;
    }
    else
        FlashOffset = (UINT32)(UINTN)BlockAddress;

    SpiBlockProtectUpdate( FlashOffset, Length, 0xFF );
}
#endif
}

/**
    This function contains any flash specific code need to enable flash write
**/
VOID
EFIAPI
CommonSpiDeviceWriteEnable(
    VOID
)
{
    // check is DeviceWrite enabled, if yes, don't enable it again,
    // else, enable it.
    if( !gDeviceWriteEnabled )
    {
        gDeviceWriteEnabled = 1;
    }
}

/**
    This function contains any flash specific code need to disable flash write
**/
VOID
EFIAPI
CommonSpiDeviceWriteDisable(
    VOID
)
{
    // check is DeviceWrite enabled, if yes, disable it,
    // if no, don't disable it.
    if( gDeviceWriteEnabled )
    {
        gDeviceWriteEnabled = 0;
    }
}

/**
    This function returns PCH's erase granularity. It returns 4K granularity by
    default, and detect VSCC value's 64K erase supported bit.

    @param Granularity Pointer to supported granularity array
    @param NumberOfEntries Granularity array entries
**/
static BOOLEAN
EFIAPI
CommonSpiGetEraseGranularity(
    IN  OUT UINT32  **Granularity,
    IN  OUT UINT32  *NumberOfEntries
)
{
    if( (Granularity == NULL) || (NumberOfEntries == NULL) )
        return FALSE;

    //Initial first granularity with SPI identify's sector size
    *NumberOfEntries = 1;
    gGranularity[(*NumberOfEntries - 1)] = gCommonSpiFlash.FlashSectorSize;

    //Check Intel SPI VSCC register for 64K erase supported if SPI identify's
    //sector size is not 64K
    if( gCommonSpiFlash.FlashSectorSize != SECTOR_SIZE_64KB )
    {
        UINT32  Vscc;

        Vscc = *(volatile UINT32*)(gSpiBase + gSpiRegs->BiosLVscc);
        //Vscc value bit 29 is 64K erase valid
        if( ((Vscc & (UINT32)BIT29) == BIT29) )
        {
            *NumberOfEntries += 1;
            gGranularity[(*NumberOfEntries - 1)] = SECTOR_SIZE_64KB;
        }
    }

    *Granularity = gGranularity;

    return TRUE;
}

/**
    This function will be invoked by the core to convert runtime pointers 
    to virtual address

    @param Rs Pointer to runtime services
**/
static
VOID
EFIAPI
CommonSpiDeviceVirtualFixup(
    IN  EFI_RUNTIME_SERVICES    *Rs
)
{
    gDeviceVirtual = 1;
    Rs->ConvertPointer( 0, (VOID**)&gSpiBase );

    return;
}
