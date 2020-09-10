; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2018 Intel Corporation. <BR>
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

    SECTION .text

%include "ProcessorStartupPlatform.inc"
%include "ProcessorStartupChipset.inc"
%include "ProcessorStartup.inc"
%include "ProcessorStartupUncore.inc"

extern ASM_PFX(PcdGet32 (PcdSecInFsp))

extern ASM_PFX(CheckGenuineIntelCpu)
extern ASM_PFX(EarlyCoreInit)
extern ASM_PFX(EarlyUncoreInit_Icx)
extern ASM_PFX(EarlyChipsetInit)
extern ASM_PFX(LoadProcessorMicrocode)
extern ASM_PFX(MidUncoreInit)
extern ASM_PFX(MidChipsetInit)
extern ASM_PFX(PreNemUncoreInit)
extern ASM_PFX(ConfigDfx)
extern ASM_PFX(ConfigDcuMode)
extern ASM_PFX(NemInit)
extern ASM_PFX(EstablishNemStack)
extern ASM_PFX(TxtInit)
extern ASM_PFX(PostNemUncoreInit)
extern ASM_PFX(SetupProcessorCheckinData)
extern ASM_PFX(AmILegacySocket)
extern ASM_PFX(UncoreBusCfgSyncNbsp_Icx)
extern ASM_PFX(ParkNbsp_Icx)
extern ASM_PFX(LateChipsetInit)
extern ASM_PFX(EnableAccessGlobalCSR)

 extern ASM_PFX(EarlyUncoreInit_Skx)
 extern ASM_PFX(UncoreBusCfgSyncNbsp_Skx)
 extern ASM_PFX(ParkNbsp_Skx)

#if FixedPcdGetBool(PcdSecInFsp) == 0
 extern ASM_PFX(EarlyChpsetInit_OemHook)
 extern ASM_PFX(LateChipsetInit_OemHook)
#endif
; APTIOV_SERVER_OVERRIDE_RC_START: Added support to Load Debug Microcode Patch on Production CPUs
 extern ASM_PFX(EarlyChpsetInit_CrbHook)
; APTIOV_SERVER_OVERRIDE_RC_END: Added support to Load Debug Microcode Patch on Production CPUs

bits 32

global ASM_PFX(ProcessorStartup)
ASM_PFX(ProcessorStartup):

        movd    mm1, esi
        movd    mm2, edi

;       mm0 : Assumed to be BIST state (not used by this module, but cant be destroyed)
;       mm1 : ProcessorStartup return address
;       mm2 : LAST_ADDRESS of decode region
;       mm3 : Used for stack by CALL_MMX3/RET_ESI_MMX3 macro
;       mm4 : Used for stack by CALL_MMX4/RET_ESI_MMX4 macro
;       mm5 : Used for stack by CALL_MMX5/RET_ESI_MMX5 macro
;       mm6 : Used for stack by CALL_MMX6/RET_ESI_MMX6 macro

        PORT80 (POWER_ON_POST_CODE)
        CALL_MMX3 ASM_PFX(CheckGenuineIntelCpu)
        CALL_MMX3 ASM_PFX(EarlyCoreInit)

        IS_SKX_CPU
        je          short Cpuid_Skx1

        CALL_MMX3 ASM_PFX(EarlyUncoreInit_Icx)

        jmp         short Cpuid_Icx1
Cpuid_Skx1:
        CALL_MMX3 ASM_PFX(EarlyUncoreInit_Skx)
Cpuid_Icx1:
;
; At this point,  MMCFG_RULE in CHA is enabled. MMCFG_TARGET_LIST points to local socket only,
; threfore inter-socket CSR accesses are not enabled yet.
; Local socket CSRs can be accessed via either CF8/CFC IO or mmcfg methods.
; CPUBUSNO in UBOX is not enabled, so defalut bus# for local targets (stacks) must be used.
;
        CALL_MMX3 ASM_PFX(ConfigDfx)
        CALL_MMX3 ASM_PFX(EarlyChipsetInit)
        PORT80 (MICROCODE_POST_CODE)
#if FixedPcdGetBool(PcdSecInFsp) == 0
        ;
        ;OEM_HOOK after EarlyChipsetInit
        ;
        CALL_MMX3 ASM_PFX(EarlyChpsetInit_OemHook)
#endif
; APTIOV_SERVER_OVERRIDE_RC_START: Added support to Load Debug Microcode Patch on Production CPUs
        CALL_MMX3 ASM_PFX(EarlyChpsetInit_CrbHook)
; APTIOV_SERVER_OVERRIDE_RC_END: Added support to Load Debug Microcode Patch on Production CPUs
        ;
        ;
        mov eax, [ASM_PFX(PcdGet32 (PcdSecInFsp))]
        cmp eax, 0
        jnz SkipUcode
        CALL_MMX3 ASM_PFX(LoadProcessorMicrocode)
SkipUcode:
        CALL_MMX3 ASM_PFX(MidUncoreInit)
        CALL_MMX3 ASM_PFX(MidChipsetInit)

        PORT80 (NEM_CACHE_INIT_POST_CODE)

        CALL_MMX3 ASM_PFX(PreNemUncoreInit)
        CALL_MMX3 ASM_PFX(ConfigDcuMode)
        CALL_MMX3 ASM_PFX(NemInit)
        CALL_MMX3 ASM_PFX(EstablishNemStack)

        ; After this point functions must be called using stack
        ;  Functions after here may NOT destroy ESP!

        PORT80 (NEM_CACHE_ENABLED_POST_CODE)
        call ASM_PFX(TxtInit)
        call ASM_PFX(PostNemUncoreInit)
        call ASM_PFX(SetupProcessorCheckinData)

        PORT80 (SBSP_PBSP_BRANCH_POST_CODE)
        ;
        ; Early SBSP Selection
        ;
        call ASM_PFX(AmILegacySocket)              ; Am I legacy-socket?
        jz      Sbsp                               ; Yes
Pbsp:
        ;
        ; PBSPs come here...
        ;
        IS_SKX_CPU
        je      short Cpuid_Skx2

        call ASM_PFX(UncoreBusCfgSyncNbsp_Icx)     ; Wait for SBSP to globally program MMCFG/BUSNUM  on all PBSP sockets
        call ASM_PFX(ParkNbsp_Icx)                 ; Loop for SBSP command to proceed. PBSPs will not return after the call

        jmp     short Cpuid_Icx2
Cpuid_Skx2:
        call ASM_PFX(UncoreBusCfgSyncNbsp_Skx)     ; Wait for SBSP to globally program MMCFG/BUSNUM  on all PBSP sockets
        call ASM_PFX(ParkNbsp_Skx)                 ; Loop for SBSP command to proceed. PBSPs will not return after the call
Cpuid_Icx2:

Sbsp:
        ;
        ; Only SBSP comes here to continue. All other package BSPs are held in a loop
        ;
        call ASM_PFX(LateChipsetInit)
#if FixedPcdGetBool(PcdSecInFsp) == 0
        ;
        ;OEM_HOOK after LateChipsetInit
        ;
        call ASM_PFX(LateChipsetInit_OemHook)
#endif
        ;
        ; SBSP enables inter-socket global CSR access
        ;
        call ASM_PFX(EnableAccessGlobalCSR)
        PORT80 (SEC_CPU_INIT_END_POST_CODE)        ; end of processor/socket init in SEC phase

        mov     eax, [ASM_PFX(PcdGet32 (PcdSecInFsp))]
        cmp     eax, 0
        jz      NotFsp
        mov     eax, 0
NotFsp:
        movd    esi, mm1
        jmp     esi
