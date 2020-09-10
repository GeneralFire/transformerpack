; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2011 - 2017 Intel Corporation. <BR>
;
; The source code contained or described herein and all documents related to the
; source code ("Material") are owned by Intel Corporation or its suppliers or
; licensors. Title to the Material remains with Intel Corporation or its suppliers
; and licensors. The Material may contain trade secrets and proprietary    and
; confidential information of Intel Corporation and its suppliers and licensors,
; and is protected by worldwide copyright and trade secret laws and treaty
; provisions. No part of the Material may be used, copied, reproduced, modified,
; published, uploaded, posted, transmitted, distributed, or disclosed in any way
; without Intel's prior express written permission.
;
; No license under any patent, copyright, trade secret or other intellectual
; property right is granted to or conferred upon you by disclosure or delivery
; of the Materials, either expressly, by implication, inducement, estoppel or
; otherwise. Any license under such intellectual property rights must be
; express and approved by Intel in writing.
;
; Unless otherwise agreed by Intel in writing, you may not remove or alter
; this notice or any other notice embedded in Materials by Intel or
; Intel's suppliers or licensors in any way.
;

; Make Sure to sync up these offsets with  CPU_INIT_INFO
%define BufferStartLocation RendezvousFunnelProcEnd - RendezvousFunnelProcStart
%define Cr3OffsetLocation BufferStartLocation + 0x4
%define GdtrLocation BufferStartLocation + 0x8
%define IdtrLocation BufferStartLocation + 0x12
%define MpExchangeInfoLocation BufferStartLocation + 0x20

; Make Sure to sync up these offsets with  HOT_ADD_CPU_EXCHANGE_INFO_OFFSET
%define LockLocation 0
%define StackStartAddressLocation 0x8
%define StackSizeLocation 0x10
%define CProcedureLocation 0x18
%define InitFlagLocation 0x20
%define SignalFlagLocation 0x24
%define PModeSignalFlagLocation 0x28
%define NumCpuCheckedInLocation 0x2C
%define BistBufferLocation 0x30
;-------------------------------------------------------------------------------------

;-------------------------------------------------------------------------------------
;RendezvousFunnelProc  procedure follows. All APs execute their procedure. This
;procedure serializes all the AP processors through an Init sequence. It must be
;noted that APs arrive here very raw...ie: real mode, no stack.
;ALSO THIS PROCEDURE IS EXECUTED BY APs ONLY ON 16 BIT MODE. HENCE THIS PROC
;IS IN MACHINE CODE.
;-------------------------------------------------------------------------------------
;RendezvousFunnelProc (&WakeUpBuffer,MemAddress);

;------------------------------------------------------------------------------
section .text
bits 16
;------------------------------------------------------------------------------

global ASM_PFX(RendezvousFunnelProc)
ASM_PFX(RendezvousFunnelProc):
RendezvousFunnelProcStart:

; At this point CS = 0x(vv00) and ip= 0x0.
        mov        ebp, eax
        mov        ax,  cs
        mov        ds,  ax
        mov        es,  ax
        mov        ss,  ax
        xor        ax,  ax
        mov        fs,  ax
        mov        gs,  ax

; Switch to flat mode.
flat32Start:
        mov        si, BufferStartLocation
        mov        edx, dword [si]     ; EDX is keeping the start address of wakeup buffer

        mov        si, Cr3OffsetLocation
        mov        ecx, dword [si]     ; ECX is keeping the value of CR3

        mov        si, GdtrLocation
    o32 lgdt       [cs:si]

        mov        si, IdtrLocation
    o32 lidt       [cs:si]

        xor        ax,  ax
        mov        ds,  ax

        mov        eax, cr0            ; Get control register 0
        or         eax, 000000003h     ; Set PE bit (bit #0) and MP
        mov        cr0, eax

FLAT32_JUMP:
a32 o32 jmp 0x20:strict dword 0

;------------------------------------------------------------------------------
bits 32
;------------------------------------------------------------------------------
ProtModeStart:                         ; protected mode entry point
    o16 mov        ax,  18h
    o16 mov        ds,  ax
    o16 mov        es,  ax
    o16 mov        fs,  ax
    o16 mov        gs,  ax
    o16 mov        ss,  ax             ; Flat mode setup.

        mov        eax, cr4
        or         ax,  0620h
        mov        cr4, eax

        mov        cr3, ecx

        mov        esi, edx            ; Save wakeup buffer address

        mov        ecx, 0c0000080h     ; EFER MSR number.
        rdmsr                          ; Read EFER.
        bts        eax, 8              ; Set LME=1.
        wrmsr                          ; Write EFER.

        mov        eax, cr0            ; Read CR0.
        bts        eax, 31             ; Set PG=1.
        mov        cr0, eax            ; Write CR0.

LONG_JUMP:
        jmp 0x38:strict dword 0

;------------------------------------------------------------------------------
bits 64
default rel
;------------------------------------------------------------------------------
LongModeStart:
        mov         ax,  0x30
        mov         ds,  ax
        mov         es,  ax
        mov         ss,  ax

        ;
        ; Get pointer to MpExchangeInfo Structure
        ;
        mov         edi, esi
        add         edi, MpExchangeInfoLocation
        mov         edi, dword [edi]
        xchg        esi, edi           ; esi - MpExchangeInfoPtr; edi - Buffer Start

        ; get ApicId
        mov        eax, 0
        cpuid
        cmp        eax, 0xb
        jnb        X2Apic              ; br, if proc capable of 32bit apicid

        mov        eax, 1
        cpuid
        shr        ebx, 24
        and        ebx, 0xff           ; EBX is APIC ID
        jmp        checkInitFlag

X2Apic:
        mov        eax, 0xb            ;get apicid from cpuid_edx(0xb,0)
        xor        ecx, ecx
        cpuid
        mov        ebx, edx            ; EBX has Apic ID

checkInitFlag:
        ;
        ; Check InitFlag for command
        ;
        cmp         dword [esi + InitFlagLocation], 0
        jz          CallApFunc

        cmp         dword [esi + InitFlagLocation], 1
        jz          RecordBist

        cmp         dword [esi + InitFlagLocation], 2
        jz          CheckInandHlt

        cmp         dword [esi + InitFlagLocation], 3
        jz          RecordBist

SkipFlag:
        wbinvd
        jmp        GoToSleep

CheckInandHlt:
        inc        dword [esi + NumCpuCheckedInLocation]
        jmp        SkipFlag

RecordBist:
        mov        dword [esi + BistBufferLocation + (8* ebx)], 1       ; Valid Flag
        mov        dword [esi + BistBufferLocation + (8* ebx) + 4], ebx ; BIST
        ; Now wait till the BSP processes the BIST info and assigns unique CPU number to each thread.

Wait4Signal:
        pause
        cmp        dword [esi + SignalFlagLocation], 0
        jz         Wait4Signal

CallApFunc:
        ;
        ; ProgramStack
        ;
        xor         rcx, rcx
        mov         ecx, dword [esi + BistBufferLocation + (8 * ebx)]    ; RCX = CpuNumber
        inc         rcx
        mov         rax, qword [esi + StackSizeLocation]
        mul         rcx                               ; RAX = StackSize * (CpuNumber + 1)

        mov         rbx, qword [esi + StackStartAddressLocation]
        add         rax, rbx                          ; RAX = StackStart + StackSize * (CpuNumber + 1)

        sub         rax, 0x10
        mov         rsp, rax

        ;
        ; Call C Function
        ;
        mov         rax, qword [esi + CProcedureLocation]

        test        rax, rax
        jz          GoToSleep
        push        rsi
        sub         rsp, 0x20
        call        rax
        add         rsp, 0x20
        pop         rsi

Wait4PMSignal:
        pause
        cmp        dword [esi + PModeSignalFlagLocation], 0
        jz         Wait4PMSignal
        wbinvd

GoToSleep:
        cli
        hlt
        jmp         $-2

RendezvousFunnelProcEnd:

;-------------------------------------------------------------------------------------
;  AsmGetAddressMap (&AddressMap);
;-------------------------------------------------------------------------------------
; typedef struct {
;   UINT8 *RendezvousFunnelAddress;
;   UINTN PModeEntryOffset;
;   UINTN FlatJumpOffset;
;   UINTN LModeEntryOffset;
;   UINTN LongJumpOffset;
;   UINTN Size;
; } MP_ASSEMBLY_ADDRESS_MAP;

global ASM_PFX(AsmGetHotAddCodeAddressMap)
ASM_PFX(AsmGetHotAddCodeAddressMap):
        mov         rax, RendezvousFunnelProcStart
        mov         qword [rcx], rax
        mov         qword [rcx+0x8], ProtModeStart - RendezvousFunnelProcStart
        mov         qword [rcx+0x10], FLAT32_JUMP - RendezvousFunnelProcStart
        mov         qword [rcx+0x18], LongModeStart - RendezvousFunnelProcStart
        mov         qword [rcx+0x20], LONG_JUMP - RendezvousFunnelProcStart
        mov         qword [rcx+0x28], RendezvousFunnelProcEnd - RendezvousFunnelProcStart
        ret
