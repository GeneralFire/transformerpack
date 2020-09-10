/** @file
  Memory Initialization Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include <RcRegs.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/HobLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SimRegsLookupLib.h>
#include <Library/MemMcIpLib.h>
#include "ScratchPadReg.h"
#include <Library/RcMemAllocLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/HardwareSemaphoreTargetLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/SimulationThreadLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Chip/Include/MemHostChip.h>
#include "Include/Pipe.h"

// Local Variables
RCSIM_SYNC_DATA gRcSimSyncData[MAX_SOCKET];
PSYSHOST        bspHost;
UINT32          *SimDispatchAddr[MAX_SOCKET];
UINT32          *SimDataAddr[MAX_SOCKET];
UINT32          *SimCmdAddr[MAX_SOCKET];

/**
 * Clear a dipatch register
 *
 * @param[in]  DispatchAddr      A pointer to the dispatch register.
 *
 * @retval None
 *
 **/
VOID
ClearDispatchRegister (
  IN UINT32   *DispatchAddr
  )
{
  //
  // Clear Dispatch register
  //
  *(volatile UINT32 *) DispatchAddr = 0x00;

  return;

} // ClearDispatchRegister

/**
 * Wait for a command from the master socket. If this
 * is a RcSim build, Sleep for 20ms between attempts to
 * avoid too much CPU usage
 *
 * @param[in]  DispatchAddr      A pointer to the dispatch register.
 *
 * @retval None
 *
 **/
VOID
WaitForCommandFromMaster (
  IN UINT32   *DispatchAddr
  )
{
  UINT32 WaitFlag;

  //
  // Wait for next command from master
  //
  do {
    WaitFlag = *(volatile UINT32 *)DispatchAddr;


  } while (WaitFlag == 0);

  return;

} // WaitForCommandFromMaster

/**
 * Wait for a command from the master socket. Do not delay
 * between attempts, even if this is a RcSim build.
 *
 * @param[in]  DispatchAddr      A pointer to the dispatch register.
 *
 * @retval None
 *
 **/
VOID
WaitForCommandFromMasterNoDelay (
  IN UINT32   *DispatchAddr
  )
{
  UINT32 WaitFlag;

  //
  // Wait for next command from master
  //
  do {
    WaitFlag = *(volatile UINT32 *)DispatchAddr;
  } while (WaitFlag == 0);

  return;

} // WaitForCommandFromMasterNoDelay

/**
 * Wait for a socket to clear the dispatch register. If this
 * is a RcSim build, Sleep for 20ms between attempts to
 * avoid too much CPU usage.
 *
 * @param[in]  Host    A pointer to the Host structure.
 * @param[in]  Socket  The socket to wait for
 *
 * @retval None
 *
 **/
VOID
WaitforDispatchFromSocket (
  IN PSYSHOST Host,
  IN UINT8 Socket
  )
{

  while (UsraCsrRead (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)) != 0x00) {


  }

  return;

} // WaitforDispatchFromSocket

/**
 * Wait for a socket to clear the dispatch register. Do not delay
 * between attempts, even if this is a RcSim build.
 *
 * @param[in]  Host    A pointer to the Host structure.
 * @param[in]  Socket  The socket to wait for
 *
 * @retval None
 *
 **/
VOID
WaitforDispatchFromSocketNoDelay (
  IN PSYSHOST Host,
  IN UINT8 Socket
  )
{

  while (UsraCsrRead (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)) != 0x00) {

  }

  return;

} // WaitforDispatchFromSocketNoDelay

/**
 * Removes current processor from non-eviction mode
 *
 * @param *PipeDispatchCmd -      MMCFG based reg address of pipe dispatch command register
 *
 * @retval N/A
 *
 **/
typedef
VOID
(*TearDownNemPointer) (
  UINT32     *PipeDispatchCmd
  );

VOID
TearDownNem (
  UINT32     *PipeDispatchCmd
  )
{
#ifdef __GNUC__
  asm
  (
    // save pipe dispatch command mailbox reg address in EDI
    "mov  %0, %%edi\n\t"        // edi = ptr to pipe dispatch command reg

    // set CR0.CD bit to globally disable cache
    "mov  %%cr0, %%eax\n\t"
    "or   $(1 << 30), %%eax\n\t"
    "mov  %%eax, %%cr0\n\t"

    // invalid/flush cache
    "invd\n\t"

    // clear Global MTRR Enable bit
    "mov $0x02FF, %%ecx\n\t"
    "rdmsr\n\t"
    "and $(~0x0800), %%eax\n\t"
    "wrmsr\n\t"

    // clear No Eviction Mode Setup/Run bits
    "mov $0x02E0, %%ecx\n\t"
    "rdmsr\n\t"
    "btr $1, %%eax\n\t"
    "wrmsr\n\t"
    "btr $0, %%eax\n\t"
    "wrmsr\n\t"

    // Renable fast strings after NEM exit
    // FAST_STRING_ENABLE = 1
    "mov   $0x01A0, %%ecx\n\t" // MSR_IA32_MISC_ENABLES           0x01A0
    "rdmsr\n\t"
    "bts   $0, %%eax\n\t"
    "wrmsr\n\t"

    // Clear pipe dispatch command mailbox
    "movl  $0, (%%edi)\n\t" // PIPE_DISPATCH_NULL            0
    :
    : "r" (PipeDispatchCmd)
    :
  );
  asm (
    "1:\n\t"
    "cli\n\t"
    "hlt\n\t"
    "jmp 1b\n\t"
    :
    : "r" (PipeDispatchCmd)
    :
  );
#else // GNU
  _asm
  {
    ; save pipe dispatch command mailbox reg address in EDI
    mov edi, PipeDispatchCmd        ; edi = ptr to pipe dispatch command reg

    ; set CR0.CD bit to globally disable cache
    mov  eax, cr0
    or   eax, BIT30
    mov  cr0, eax

    ; invalid/flush cache
    invd

    ; clear Global MTRR Enable bit
    mov ecx, MSR_IA32_MTRR_DEF_TYPE
    rdmsr
    and eax, NOT (0x0800)
    wrmsr

    ; clear No Eviction Mode Setup/Run bits
    mov ecx, MSR_NEM
    rdmsr
    btr eax, 1
    wrmsr
    btr eax, 0
    wrmsr

    ;
    ; Renable fast strings after NEM exit
    ; FAST_STRING_ENABLE = 1
    ;
    mov   ecx, MSR_IA32_MISC_ENABLE
    rdmsr
    bts   eax, 0
    wrmsr

    ; Clear pipe dispatch command mailbox
    mov DWORD PTR[edi], PIPE_DISPATCH_NULL
    _asm {
      loop1 :
      cli
      hlt
      jmp loop1
    }
  }

  #endif // __GNUC__
} // TearDownNem

/**
 * Slave side of Pipe data receive function for non-S3 and multi-thread path.
 *
 * @param DataAddr         Memory map IO address for pipe data register
 * @param DispatchAddr     Memory map IO address for pipe dispatcher register
 *
 **/
VOID
ReceivePipeDataSlave (
  IN UINT32   *DataAddr,
  IN UINT32   *DispatchAddr
  )
{
  UINT32      Base;
  UINT32      *Buffer;
  UINT32      DwordCount;
  UINT32      CurrentDwordCount;
  UINT32      Temp;
  //
  // Get PEI base address from master
  //
  Base = *(volatile UINT32 *) DataAddr;
  //
  // Clear Dispatch register
  //

  ClearDispatchRegister (DispatchAddr);

  //
  // Wait for next command from master
  //

  WaitForCommandFromMaster (DispatchAddr);

  //
  // Get PEI data size in DWORD from master
  //
  DwordCount = *(volatile UINT32 *) DataAddr;
  //
  // Clear Dispatch register
  //
  ClearDispatchRegister (DispatchAddr);

  Buffer = (UINT32 *)(UINTN) Base;
  ZeroMem ((VOID *) Buffer, DwordCount * sizeof (UINT32));

  for (CurrentDwordCount = 0; CurrentDwordCount < DwordCount;) {
    //
    // Wait for (DispatchReg != 0) then DataReg = Dword[DwordCounter]
    //

    //
    // For RcSim, use the no delay version of WaitForCommandFromMasterNoDelay
    // to transfer data as quickly as possible
    //

    WaitForCommandFromMasterNoDelay (DispatchAddr);

    Temp = *(volatile UINT32 *) DispatchAddr;

    CurrentDwordCount = (Temp >> 4) & 0x0FFFFFFF;
    if (CurrentDwordCount < DwordCount) {
      Buffer[CurrentDwordCount] = *(volatile UINT32 *) DataAddr;
    }

    //
    // Clear Dispatch register
    //
    ClearDispatchRegister (DispatchAddr);
  }
} // ReceivePipeDataSlave

/**
 * Slave side of Pipe data receive function for S3 and multi-thread path.
 *
 * @param CfgAddrBDF       Bus:Dev:Func portion of Cofig Address of local SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR
 * @param CsrOffsetMask    CSR offset address from caller
 * @param DataAddr         Memory map IO address for pipe data register
 * @param DispatchAddr     Memory map IO address for pipe dispatcher register
 *
 **/
VOID
ReceivePipeDataSlaveMultiThreadS3Resume (
  IN UINTN      CfgAddrBDF,
  IN CSR_OFFSET CsrOffsetMask,
  IN UINT32     *DataAddr,
  IN UINT32     *DispatchAddr
  )
{
  UINT32      NumberOfDwords;
  UINT32      CurrentDwordCount;
  UINT32      CurrentDword;
  UINT32      Base;
  UINT32      *Buffer;
  UINTN       Index;

  //
  // Get PEI base address from master
  //
  Base = *(volatile UINT32 *) DataAddr;
  //
  // Clear Dispatch register
  //
  ClearDispatchRegister (DispatchAddr);

  //
  // Wait for next command from master
  //

  WaitForCommandFromMaster (DispatchAddr);

  //
  // Get data size in DWORD from master
  //
  NumberOfDwords = *(volatile UINT32 *) DataAddr;
  //
  // Clear Dispatch register
  //
  ClearDispatchRegister (DispatchAddr);

  Buffer = (UINT32 *)(UINTN) Base;
  for (CurrentDwordCount = 0; CurrentDwordCount < NumberOfDwords; CurrentDwordCount += CurrentDword) {

    //
    // Wait for (DispatchReg != 0) then DataReg = Dword[DwordCounter]
    //

    WaitForCommandFromMaster (DispatchAddr);

    CurrentDword = ((NumberOfDwords - CurrentDwordCount) <  SCRATCH_PAD_NUM ) ? (NumberOfDwords - CurrentDwordCount) : SCRATCH_PAD_NUM;

    for (Index = 0; Index < CurrentDword; Index++) {
      Buffer[CurrentDword + Index] =  *(volatile UINT32 *)(CfgAddrBDF |  (BiosNonStickyScratchPadCsr[Index] & CsrOffsetMask.Data));
    }

    //
    // Clear Dispatch register
    //
    if((NumberOfDwords - CurrentDwordCount) >  SCRATCH_PAD_NUM ) {
      ClearDispatchRegister (DispatchAddr);
    }
  }

  ClearDispatchRegister (DispatchAddr);
} // ReceivePipeDataSlaveMultiThreadS3Resume

/**
 * Slave side of Pipe Initialization
 *
 * @param UINT32    NodeId           NodeId/SocketId of the package BSP as a pipe slave. Only the last 8-bit is meaningful
 * @param UINT32    CfgAddrBDF       Bus:Dev:Func portion of Cofig Address of local SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR
 *
 * @retval This function does not return to caller, but enters a "cli, hlt" loop at the end.
 *
 **/
VOID
PeiPipeSlaveInit (
    UINT32    NodeId,
    UINT32    CfgAddrBDF
  )
{
  PSYSHOST  Host;
  UINT8     Socket;
  UINT32    *DispatchAddr;
  UINT32    *dataAddr;
  UINT32    StickPad[SCRATCH_PAD_NUM];
  UINTN     Index;
  UINT32    BootMode;
  CSR_OFFSET CsrOffsetMask;
  SYS_SETUP *MemoryPolicy;
  UINT32    RelocateAddr;

  //
  // Initialize IDT table for AP so that we can save syshost table pointer in IDT - 8.
  // It also preserves the PEI services table pointer in case we need to use
  // PeiServices table pointer in the future.
  //
  IA32_DESCRIPTOR IdtDescriptor;
  AP_IDT_TABLE    IdtTableAp;

  IdtTableAp.SysHostPointer = 0;
  IdtTableAp.PeiServices = 0;

  for (Index = 0; Index < SEC_IDT_ENTRY_COUNT; Index++) {
    IdtTableAp.IdtTable[Index] = INITIAL_IDT_ENTRY_VALUE;
  }

  IdtDescriptor.Base  = (UINTN) &IdtTableAp.IdtTable;
  IdtDescriptor.Limit = (UINT16)(sizeof (IdtTableAp.IdtTable) - 1);

  AsmWriteIdtr (&IdtDescriptor);

  /*
    Clear Dispatch register
    Wait for get Host structure pointer dispatch command (DispatchReg != 0 && ComamndReg == synchHost)
    Get address for Host structure (read dataReg) to "Host"
    Clear Dispatch register
    Wait for  (DispatchReg != 0) then DataReg = # dwords to receive
    Set Dwordcounter = 0;
    Clear Dispatch register
     for "# of dewords"
           Wait for (DispatchReg != 0) then DataReg = Dword[DwordCounter]
           Clear Dispatch register

    Go to normal dispatcher (PipeDispatcher(Host);)
*/
  Socket = (UINT8) NodeId;

  CsrOffsetMask.Data = 0;
  CsrOffsetMask.Bits.offset = (UINT32) -1;

  DispatchAddr  = (UINT32 *) ( CfgAddrBDF | (SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR & CsrOffsetMask.Data));
  dataAddr      = (UINT32 *) ( CfgAddrBDF | (SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR & CsrOffsetMask.Data));

  //
  // Clear Dispatch register
  //
  ClearDispatchRegister (DispatchAddr);

  //
  //
  // Wait for get Host structure pointer dispatch command (DispatchReg != 0)
  //
  WaitForCommandFromMaster (DispatchAddr);

  //
  // Get address for Host structure (read dataReg) to "Host"
  //
  Host = (PSYSHOST)(*(volatile UINT32 *) dataAddr);

  //
  // Clear Dispatch register
  //
  ClearDispatchRegister (DispatchAddr);

  //
  // Wait for get Memory Policy structure pointer dispatch command (DispatchReg != 0)
  //
  WaitForCommandFromMaster (DispatchAddr);

  //
  // Get address for Memory Policy structure (read dataReg) to "Host"
  //

  MemoryPolicy = (SYS_SETUP *)(*(volatile UINT32 *) dataAddr);

  //
  // Clear Dispatch register
  //
  ClearDispatchRegister (DispatchAddr);

  //
  // Wait for  (DispatchReg != 0) then DataReg = # dwords to receive
  //
  WaitForCommandFromMaster (DispatchAddr);

  //
  // Get multithreaded/S3 resume flag from master
  //
  BootMode = *(volatile UINT32 *) dataAddr;

  //
  // Save
  //
  if (BootMode == S3Resume) {
    for (Index = 0; Index < SCRATCH_PAD_NUM; Index++) {
      StickPad[Index] = *(volatile UINT32 *)(CfgAddrBDF |  (BiosNonStickyScratchPadCsr[Index] & CsrOffsetMask.Data));
    }
  }

  //
  // Clear Dispatch register
  //

  ClearDispatchRegister (DispatchAddr);

  //
  // Wait for next command from master
  //
  WaitForCommandFromMaster (DispatchAddr);

  if (BootMode == S3Resume) {
    ReceivePipeDataSlaveMultiThreadS3Resume (CfgAddrBDF, CsrOffsetMask, dataAddr, DispatchAddr);
  } else {
    ReceivePipeDataSlave (dataAddr, DispatchAddr);
  }

  //
  // Wait for next command from master
  //

  WaitForCommandFromMaster (DispatchAddr);

  if (BootMode == S3Resume) {
    ReceivePipeDataSlaveMultiThreadS3Resume (CfgAddrBDF, CsrOffsetMask, dataAddr, DispatchAddr);
  } else {
    ReceivePipeDataSlave (dataAddr, DispatchAddr);
  }
  //
  // Init pei services table pointer for AP
  //
  ASSERT (Host->var.common.oemVariable != 0);
  SetPeiServicesTablePointer ((VOID*) (UINTN) Host->var.common.oemVariable);


  //
  // Reinitialize the performance tracker for this socket.
  //

  Host->var.mem.PerformanceTrackerInitialized = FALSE;

  //
  // Initialize Host pointer for NBSP after all its contents are synced from SBSP.
  // GetSysHostPointer () cannot be used before here because stack/heap/PeiServices
  // are not set up yet
  //
  SetSysHostPointer (Host);

  //
  // Do the same for the SysSetup pointer.
  //

  SetSysSetupPointer (MemoryPolicy);



  // Restore
  if (BootMode == S3Resume) {
    for (Index = 0; Index < SCRATCH_PAD_NUM; Index++) {
      *(volatile UINT32 *)(CfgAddrBDF | (BiosNonStickyScratchPadCsr[Index] & CsrOffsetMask.Data)) = StickPad[Index];
    }

    WaitForCommandFromMaster (DispatchAddr);

    ClearDispatchRegister (DispatchAddr);
  }

  //
  // load local socketId to Host->var.common.socketId
  //
  SetSysSocketId (Socket);

  //
  // Init variables that track nested call levels for semaphores
  //
  Host->var.common.printfDepth = 0;

  LoadCapId (Socket);

  LoadCpuId (Socket);

  //
  // Wait for command from Pipe Master
  //
  PipeDispatcher (Host);

  // Indicate entered exit code
  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), PIPE_SYNC_WAIT_FOR_TEARDOWN_ADDRESS);

  //
  // Get Permanent Memory address for Slave NEM tear down
  //
  do {
    RelocateAddr = UsraCsrRead (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host));
  } while (RelocateAddr == PIPE_SYNC_WAIT_FOR_TEARDOWN_ADDRESS);
  RcDebugPrint (SDBG_DEFAULT, "Skt%x NEM Tear Down @ %x\n", Socket, RelocateAddr);

  //
  // Ready to exit Pipe
  // This TearDownNem()  function does NOT return!
  //
  //TearDownNem (DispatchAddr);
  ((TearDownNemPointer)(UINTN) RelocateAddr) (DispatchAddr);

} // PeiPipeSlaveInit


/**
 * Master side work function to send data to a remote socket
 *
 * @param Socket       remote socket id
 * @param Buffer       Buffer to contain the bugger
 * @param DwordCount   Total number of DWORD that needs to send to remote
 * @param DstBuffer    The destination buffer to receive the data.
 *
 **/
VOID
SendPipeDataMaster (
  IN UINT8     Socket,
  IN UINT32    *Buffer,
  IN UINTN     DwordCount,
  IN UINT32    *DstBuffer
  )
{
  PSYSHOST Host;
  UINT32   Base;
  UINTN    CurrentDwordCount;

  Host = GetSysHostPointer ();

  Base = (UINT32)(UINTN) DstBuffer;
  RcDebugPrint (SDBG_DEFAULT, "Send data buffer\n");

  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), Base);
  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);

  WaitforDispatchFromSocket (Host, Socket);

  RcDebugPrint (SDBG_DEFAULT, "Send data dwordcount %x\n", DwordCount);
  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), DwordCount);
  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);

  WaitforDispatchFromSocket (Host, Socket);

  RcDebugPrint (SDBG_DEFAULT, "Send data...");

  for (CurrentDwordCount = 0; CurrentDwordCount < DwordCount; CurrentDwordCount++) {
    //
    // Send DWORD[n] value
    //
    if (Buffer[CurrentDwordCount] != 0) {

      //
      // For RcSim ONLY, the normal WaitforDispatchFromSocket will insert a delay between read attempts
      // to avoid consuming all the CPU resources it can. Since we are in the middle of a data transfer,
      // use the no delay version of WaitforDispatchFromSocket to transfer as quickly as possible.
      //

      WaitforDispatchFromSocketNoDelay (Host, Socket);

      UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR (Host), Buffer[CurrentDwordCount]);
      UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR (Host), ((CurrentDwordCount << 4) | 0x1));
    }
  }
  // To terminate the AP

  WaitforDispatchFromSocket (Host, Socket);

  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR (Host), 0);
  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR (Host), ((DwordCount << 4) | 0x1));

  WaitforDispatchFromSocket (Host, Socket);

  RcDebugPrint (SDBG_DEFAULT, "complete\n");
} // SendPipeDataMaster


/**
 * Master side of pipe initialization
 *
 * @param Host - pointer to sysHost structure on stack
 *
 * @retval N/A
 *
 **/
VOID
InitializePipe (
  IN PSYSHOST  Host
  )
{
  UINT8   Socket;
  UINT32  BootMode;
  UINT32  *DataToSendBuffer;
  UINT32  DataToSendDwordCount;
  UINT32  *AddressOnSlaveSide;
  SYS_SETUP *Setup;


  EFI_HOB_HANDOFF_INFO_TABLE  *HandOffHob;
  UINT32                      StartOfCarMem;
  UINT32                      CarMemSize;

  HandOffHob = GetFirstHob (EFI_HOB_TYPE_HANDOFF);
  ASSERT (HandOffHob != NULL);
  if (HandOffHob == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "EFI Hand Off HOB not located, exit pipe initialization.\n");
    return;
  }

  //
  // Get the Address of Cache as Ram mem and the size.
  //

  StartOfCarMem = PcdGet32 (PcdTemporaryRamBase);
  CarMemSize = (UINT32) (HandOffHob->EfiFreeMemoryBottom - StartOfCarMem);


  Setup = GetSysSetupPointer ();

  //
  // We'll send BootMode = S3 resume only if MRC is multithreaded
  //
  if ( (Setup->mem.options & MULTI_THREAD_MRC_EN) &&
    ((GetSysResetRequired() == 0)) &&
    (GetSysBootMode () == S3Resume) ) {
    BootMode = S3Resume;
  } else {
    BootMode = NormalBoot;
  }

  //
  // Control will come here only on SBSP
  // (non-SBSP's are looping on their local SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR waiting for a jmp address)
  //

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (Socket) || (Socket == GetSysSbspSocketIdNv ())) {
      continue;
    }

    /*
    At this point non-SBSPs (a.k.a. NBSPs) are held in ParkNbsp PROC in ProcessorStartupUncore.asm, looping on its local scracth pad CSR
    For each non-SBSP present in the system, send command/data via its local scratch pad CSRs in the following order :

            A new stack pointer (that protects future Host location)
            The entry address of PeiPipeSlaveInit() function

#ifdef DECOMPRES_RC
               Address of decompression destination buffer
               Address of decompression source buffer
               Address of decompression function
#endif

            "Multi-Threaded MRC" flag (enable/disable)
            if "Multi-Threaded MRC" == "Enabled"
            {
               Address of Host structure
               Size of Host structure in DWords
               Host structure data, one dword at a time
            }
*/

    //
    // Send new stack pointer (that protects future Host location)
    //
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), (UINT32) ALIGN_VALUE ((UINTN) &Socket, 4));
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host), PIPE_DISPATCH_SYNCH_PSYSHOST);
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);

    RcDebugPrint (SDBG_DEFAULT, "Pass PIPE_DISPATCH_SYNCH_PSYSHOST to socket %d\n", Socket);

    WaitforDispatchFromSocket (Host, Socket);

    //
    // Send &PeiPipeSlaveInit
    //
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), (UINT32)&PeiPipeSlaveInit);
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);
    RcDebugPrint (SDBG_DEFAULT, "Pass PeiPipeSlaveInit\n");

    WaitforDispatchFromSocket (Host, Socket);

#ifdef COMPRESS_RC
    //
    // Send decompress destination buffer address
    //
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), Host->var.common.rcDest);
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);

    WaitforDispatchFromSocket (Host, Socket);

    //
    // Send decompress source buffer address
    //
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), Host->var.common.rcSrc);
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);

    WaitforDispatchFromSocket (Host, Socket);

    //
    // Send decompress function address
    //
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), Host->var.common.rcDecompressSourceAddr);
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);

    WaitforDispatchFromSocket (Host, Socket);

#endif // COMPRESS_RC


    //
    // Configure memory range
    //


    //
    // We will send all of Cache as Ram memory from the master to the slave.
    //

    DataToSendBuffer = (UINT32 *)(UINTN) StartOfCarMem;
    DataToSendDwordCount = CarMemSize / sizeof (UINT32); // CarMemSize must be a multiple of 4

    //
    // Send the address of Host to the slave side. Since we send all of CAR to the slave, we only
    // need to tell the slave where Host is located in CAR, which will be the same in slaves as
    // it is in the master.
    //

    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), (UINT32) Host);

    AddressOnSlaveSide = DataToSendBuffer;


    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);
    RcDebugPrint (SDBG_DEFAULT, "Pass pointer to Host\n");

    WaitforDispatchFromSocket (Host, Socket);

    //
    // Send the slave side address of the Memory Policy.
    //


    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), (UINT32) Setup);


    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);
    RcDebugPrint (SDBG_DEFAULT, "Sending address of Memory Policy\n");

    WaitforDispatchFromSocket (Host, Socket);

    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), BootMode);
    UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), 0x1);
    RcDebugPrint (SDBG_DEFAULT, "Pass boot mode %x\n", BootMode);

    WaitforDispatchFromSocket (Host, Socket);

    if (BootMode == S3Resume) {
      MultiThreadS3ResumeScratchPadRestore (Socket, DataToSendBuffer, DataToSendDwordCount, AddressOnSlaveSide);
    } else {
      SendPipeDataMaster (Socket, DataToSendBuffer, DataToSendDwordCount, AddressOnSlaveSide);
    }


    DataToSendBuffer = (UINT32 *)(UINTN) (ALIGN_VALUE ((UINTN) &Socket, 4)); // current stack frame should already cover pei stack
    DataToSendDwordCount = (PcdGet32 (PcdTemporaryRamBase) + PcdGet32 (PcdTemporaryRamSize) - (UINTN) DataToSendBuffer) / sizeof (UINT32);

    //
    // Address on the slave side is the same as the address on the master side.
    //

    AddressOnSlaveSide = DataToSendBuffer;

    if (BootMode == S3Resume) {
      MultiThreadS3ResumeScratchPadRestore (Socket, DataToSendBuffer, DataToSendDwordCount, AddressOnSlaveSide);
    } else {
      SendPipeDataMaster (Socket, DataToSendBuffer, DataToSendDwordCount, AddressOnSlaveSide);
    }



  } // socket loop

  return;

} // InitializePipe

/**
 * Master side of exiting Pipe on slave processors
 *
 * @param Host  - Pointer to the system Host (root) structure
 *
 * @retval N/A
 *
 **/
VOID
ExitPipe (
  IN PSYSHOST  Host
  )
{
  UINT8 index;
  UINT8 SbspSocketIdNv;
  EFI_HOB_GUID_TYPE *GuidHob;
  UINT32 RelocateAddr;

  SbspSocketIdNv = GetSysSbspSocketIdNv ();
  //
  //  Do nothing but return if a reset request is pending
  //  Note: Since a reset will be triggered soon anyway, we keep pipe slaves looping on pipe command
  //        to avoid potentail issues with INVD instuction in TearDownNem() on a cold boot path
  //        when there are 4 CPU packages present and KTI RC has not yet fully completed the
  //        routings among non-SBSP sockets.
  //
  if (GetSysResetRequired()) {
    return;
  }

  //
  // Get relocate address from HOB
  //
  GuidHob = GetFirstGuidHob (&gPbspTearDownNemAddressHobGuid);
  if (GuidHob == NULL) {
    ASSERT (!(GuidHob == NULL));
    CpuDeadLoop ();
  }
  RelocateAddr = *(UINT32*) GET_GUID_HOB_DATA (GuidHob);
  CopyMem ((VOID*)(UINTN) RelocateAddr, (VOID *)(UINTN) TearDownNem, ((UINTN) ReceivePipeDataSlave - (UINTN) TearDownNem));

  //
  // Dispatch the slaves to exit Pipe one at a time
  //
  for (index = 0; index < MAX_SOCKET; index++) {
    if (index == SbspSocketIdNv) {
      continue;
    }
    //
    // Continue if this socket is not present
    //
    if (SocketPresent (index) == FALSE) {
      continue;
    }

    //
    // Send "Exit Pipe" command to the slave
    //
    UsraCsrWrite (index, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), PIPE_DISPATCH_EXIT);
    //
    // Poll indicator
    //
    while (UsraCsrRead (index, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host)) != PIPE_SYNC_WAIT_FOR_TEARDOWN_ADDRESS);
    //
    // Send PBSP premenent address for NEM tear down to the slave
    //
    UsraCsrWrite (index, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), RelocateAddr);
    //
    // wait for the slave to complete so we can make sure all non-BSP sockets are done
    //
    while (UsraCsrRead (index, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)) != (UINT32)PIPE_DISPATCH_NULL);
  }

  //
  //  Ensure all APs are in the Wait for SIPI state (e.g. They are no longer needed!
  //  This includes all other logical processors in the same physical processor
  //  as the BSP and all logical processors in other physical processors.
  //  If any APs are awake, the BIOS must put them back into the Wait for
  //  SIPI state by issuing a broadcast INIT IPI to all excluding self.
  //
  if (KTI_HOST_OUT_PTR->numCpus > 1) {
#ifdef __GNUC__
    asm
    (
      "mov $0x0FEE00300, %%edi\n\t"
      "mov $0x0000C4500, %%eax\n\t"
      "mov %%eax, (%%edi)\n\t"
      "1:\n\t"
      "mov (%%edi), %%eax\n\t"
      "bt $12, %%eax\n\t"    /*  Check if send is in progress */
      "jc 1b\n\t"            /*  Loop until idle */
      :
      :
      : "eax", "edi"
    );
#else
    _asm
    {
      mov edi, 0FEE00300h
      mov eax, 0000C4500h
      mov[edi], eax
      loopstart :
      mov eax, [edi]
      bt eax, 12    /*  Check if send is in progress */
      jc loopstart  /*  Loop until idle */
    }
#endif // __GNUC__
  }
} // ExitPipe
