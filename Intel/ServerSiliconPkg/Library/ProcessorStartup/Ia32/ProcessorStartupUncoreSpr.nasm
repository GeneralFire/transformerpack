
; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2020 Intel Corporation. <BR>
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
%include "ProcessorStartupUncoreSpr.inc"
%include "ProcessorStartup.inc"

extern ASM_PFX(ReadPciLegacy)
extern ASM_PFX(WritePciLegacy)
extern ASM_PFX(_OEMCpuSecInit)

extern ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionBase))
extern ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionSize))
extern ASM_PFX(PcdGet32 (PcdSecInFsp))
; APTIOV_SERVER_OVERRIDE_RC_START : To resolve Build error
;extern ASM_PFX(PcdGet64 (PcdPciExpressBaseAddress))
; APTIOV_SERVER_OVERRIDE_RC_END : To resolve Build error

 %define MMIO_RULE19_BASEADDRESS (FixedPcdGet32 (PcdMmioRule19BaseAddress))

bits 32

;----------------------------------------------------------------------------
; Executed by all socket BSPs present in the system early after reset
; - Enable MMCFG_RULE on local socket
; - Set IO, MMIO target lists  to point to legacy socket
; - Init MMIO_RULE11 to cover the 64MB below 4GB for NEM DATA region
; -
;  Input:  All general purpose registers undefined
;          Stack may not be available
;          MMX4 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;  Output: All general purpose registers undefined.
;          MMCFG and some related CSRs are initialized to allow CSR accesses on local socket via MMCFG method
;          Following xmm registers hold MMCFG and bus number config scheme info (later used by otehr routines in this file)
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legacy socket id
;                             [27:8] CPU Family/Stepping info (from cpuid eax, bit[19:0])
;                             [29:28]  rsvd
;                             [30]    Legacy socket flag
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;----------------------------------------------------------------------------

global ASM_PFX(EarlyUncoreInit_Spr)
ASM_PFX(EarlyUncoreInit_Spr):
      ;
      ; All sockets come here after reset
      ;
      ; In CR4, set OSFXSR bit 9 and OSxmmEXCPT bit 10 to enable access to xmm registers used by this routine
      ;
      mov   eax, cr4
      or    eax, (BIT10+BIT9)
      mov   cr4, eax

      ; Use pre-defined scheme for temp config
      mov   eax, DEFAULT_COLDBOOT_CPUBUSNO
      movd  xmm4, eax

      ;
      ; At this point, only CF8/CFC IO port can be used for PCI config access
      ;

      ;
      ; Disable LOCK lockcontrol.lockdisable  at 0:8:0:0x50, bit0
      ; TODO:    bit0 is set by default already?
      ;
      ;mov   eax, CSR_LEGACY_LOCKCONTROL_SPR
      ;READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      ;or    ebx, BIT0                                   ; set bit0
      ;WRITE_PCI_DWORD_LEGACY

      ;
      ; Read local socket ID and legacy socket ID, and store them in xmm0
      ;
      mov   eax, CSR_LEGACY_SAD_CONTROL_CBO_SPR         ; get local  socket ID
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      mov   ecx, ebx
      and   ecx, (BIT2+BIT1+BIT0)                       ; keep bit[2:0] local socket ID, ignore bit3

      mov   eax, CSR_LEGACY_SAD_TARGET_CBO_SPR          ; get legacy socket ID
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      and   ebx, (BIT21+BIT20+BIT19+BIT18+BIT17)        ; keep bit[21:17] legacy_pch_target
      shr   ebx, 13                                     ; move the legacy_pch_target to [8:4]
      btr   ebx, 8                                      ; test & clear bit8  (local/remote?)
      jnc   short eui_02_spr                            ; remote socket ID

      ;
      ; For legacy socket,  use local socket ID as legacy socket ID, and set legacy socket flag bit
      ;
      mov   ebx, ecx
      shl   ebx, 4                                      ; bit[7:4] = Legacy socket id
      or    ebx, BIT30                                  ; bit[30] = Legacy Socket flag
eui_02_spr:
      or    ecx, ebx
      movd  xmm0, ecx                                   ; xmm0[7:4] = legacy socket id, [3:0]= local socket id

      ; xmm4 = CPUSUBNO [Bus0,Bus1]
      mov   eax, CSR_LEGACY_CPUBUSNO_UBOX_SPR            ; set UBOX busno 0/1/2/3
      movd  ebx, xmm4
      WRITE_PCI_DWORD_LEGACY

      movd  eax, xmm0                                    ; xmm0 bit[30] = Legacy Socket flag
      bt    eax, 30
      jnc   short eui_03_spr                             ; PBSP, don't set UBOX CPUBUSNO valid bit
      mov   eax, CSR_LEGACY_CPUBUSNO_VALID_UBOX_SPR      ; set Valid bit = 1
      READ_PCI_DWORD_LEGACY                              ; ebx = reg data
      or    ebx, BIT31                                   ; set bit31
      WRITE_PCI_DWORD_LEGACY
eui_03_spr:

      ;
      ; Get CPU Faimly/stepping info and store it in xmm0[27:8]
      ;
      mov  eax, 1
      cpuid
      and  eax, 0xfffff                                 ; keep Falily/Stepping info
      shl  eax, 8                                       ; shift to bit[27:8]
      movd ebx, xmm0
      or   ebx, eax
      movd xmm0, ebx

      ;
      ; Read warm reset flag from sticky scratch pad reg and save it in xmm0
      ;
      mov   eax, SSR_LEGACY_BOOT_CONFIG_INFO_CSR_SPR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      test  ebx, BIT5                                   ; Warm Reset Path (Cold Reset Flow Completed = 1)?
      jnz   eui_warmboot_spr                            ; Yes
      test  ebx, BIT4                                   ; Cold Reset Flow Entered flag = 1?
      jz    eui_coldboot_spr                            ; No (normal cold boot path)

      ; Warm reset occured before completion of MRC on the cold boot path.
      ; Detect and handle if this is special LT_SHUTDOWN case in 4S
      test  ebx, BIT29                                  ; Remote Socket released in LT_ENABLED system?
      jnz   short eui_coldboot_ltshutdown_spr           ; No (normal cold boot path without delay)

      ;
      ; Warm reset occured before completion of MRC on the cold boot path.
      ; In this case SBSP triggers a power-good reset while other socket BSPs waits
      ;

      CALL_MMX6 ASM_PFX(AmILegacySocketNoStack)         ; am I legacy socket?
      jnz   eui_06_spr                                  ; no, skip
      ; Legacy BSP comes here - Triggers a power-good reset.
      mov   al, 0xe
      mov   dx, 0xcf9
      out   dx, al

eui_06_spr:
      jmp   $                                           ; wait for reset to occur

eui_coldboot_spr:
; ltshutdown: delay before entering cold-boot path for "signed production ACM on a debug PCH"
      rdtsc
      mov    ebx, eax
  lt_01_spr:
      rdtsc
      sub    eax, ebx
      jnc    short lt_01a_spr
      neg    eax
      inc    eax
  lt_01a_spr:
      cmp    eax, 03*1024*1024*5                        ;;5ms on 3Ghz cpu
      jb     lt_01_spr

eui_coldboot_ltshutdown_spr:
      mov   eax, SSR_LEGACY_BOOT_CONFIG_INFO_CSR_SPR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      or    ebx, BIT4                                   ; set Cold Boot Flow Entered flag = 1
      ; Clear remote socket released bit
      btr   ebx, 29
      WRITE_PCI_DWORD_LEGACY                            ; write data to CSR
      jmp   eui_20_spr

eui_warmboot_spr:                                       ; Warm reset path
      ;and   ebx, NOT (BIT4)                             ; clear bit4 of scratch pad register
      btr   ebx, 4                                      ; clear bit4 of scratch pad register
      ; Clear remote socket released bit
      btr   ebx, 29

      WRITE_PCI_DWORD_LEGACY                            ; write data to CSR

      movd  ebx, xmm0
      or    ebx, BIT31                                  ; Set warm reset path flag in xmm reg
      movd  xmm0, ebx

eui_20_spr:
      ;
      ; Initialize  MMCFG_TARGET_LIST (at 1:29:1:ec)  to target local socket
      ;

      mov   eax, CSR_LEGACY_MMCFG_TARGET_LIST_CBO_SPR
      READ_PCI_DWORD_LEGACY                             ; ebx = default value of  MMCFG_TARGET_LIST CSR
      or    ebx, 0x88888888                             ; make sure that all entries are marked as "local"
      WRITE_PCI_DWORD_LEGACY                            ; write it back
      movd  xmm1, ebx                                   ; save it in xmm1

      ;
      ; Initialize  CSR_LEGACY_MMCFG_LOCAL_RULE_CFG_CBO & CSR_LEGACY_MMCFG_LOCAL_RULE_CFG2_CBO (at 1:29:1:c8 & cc)
      ;    ModeX=0 means  "send devices 16-31 at the root bus to the stack rather than to ubox"
      ;    ModeX=1 means  "send everything to the ubox"
      ;    For all sockets, set Mode=0
      ;
      mov  eax, CSR_LEGACY_MMCFG_LOCAL_RULE_CFG_CBO_SPR
      mov  ebx, DEFAULT_MMCFG_LOCAL_RULE_CFG_SPR
      WRITE_PCI_DWORD_LEGACY
      mov  eax, CSR_LEGACY_MMCFG_LOCAL_RULE_CFG2_CBO_SPR
      mov  ebx, DEFAULT_MMCFG_LOCAL_RULE_CFG2_SPR
      WRITE_PCI_DWORD_LEGACY

      ;Initialize MMCFG_LOCAL_RULE_ADDRESSx_CFG (at 1:29:1:c8, b0 & b4 & b8 & bc)
      ;program local stack bus numbers in Cha
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_SPR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS0_SPR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_SPR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS1_SPR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_CBO_SPR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS2_SPR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_CBO_SPR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS3_SPR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      ;
      ; Initialize and enable MMCFG_Rule register
      ; a. init Cbo MMCFG_RULE  (at 1:29:1:C0)
      ;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_SPR + 4      ; clear MMCFG high address
      xor   ebx, ebx
      WRITE_PCI_DWORD_LEGACY
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_SPR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      mov   ecx, MMCFG_RULE_ADDR_MASK
      not   ecx
      and   ebx, ecx                                    ; mask off base address
      and   bl, 0xf0                                    ; mask off Length, keep Valid_Targets
; APTIOV_SERVER_OVERRIDE_RC_START : To resolve Build error      
      ;or    ebx, [ASM_PFX(PcdGet64 (PcdPciExpressBaseAddress))] ; update base addr
      ;or    ebx, 8                                              ; update length
      or    ebx, DEFAULT_MMCFG_RULE                     ; update base addr and length
; APTIOV_SERVER_OVERRIDE_RC_END : To resolve Build error      
      or    ebx, DEFAULT_MMCFG_RULE                     ; update base addr and length
      ;;or    ebx, 0ffh SHL 6                           ; set valid bit for all local targets (default)
      or    ebx, BIT0                                   ; set enable bit
      WRITE_PCI_DWORD_LEGACY
      movd  xmm3, ebx                                   ; save current reg value in xmm3

; ----------------------------------------------------------------------------------------------
; From this point on, local CSRs can be accessed at default bus numbers via MMCFG method
; Since MMCFG_TARGET_LIST points to local socket only, all CSR access are local
; ----------------------------------------------------------------------------------------------

      ;
      ; Program IOPORT_TARGET_LIST, MMIO_TARGET_LIST  before any io transaction. Legacy socket is the target
      ;
      mov   eax, 0x10101010                             ; target list for legacy socket itself
      CALL_MMX6 ASM_PFX(AmILegacySocketNoStack)         ; am I legacy socket?
      je    eac_40_spr                                  ; yes

      movd  eax, xmm0
      and   eax, (BIT7+BIT6+BIT5+BIT4)                  ; eax [7:4] = legacy socket id, it is always run by PBSP, so the highest bit of target should be always 0
      shr   eax, 4
      mov   ebx, eax                                    ; ebx[3:0] = eax[3:0] = legacy socket id
      mov   cl, 8                                       ; number of bits for socket id
      mov   ch, 3
eac_30_spr:
      shl   ebx, cl
      or    eax, ebx
      dec   ch
      jnz   eac_30_spr

eac_40_spr:
      ;
      ; edi = MMCFG base addr
      ; eax = target list with all entries targeting legacy socket
      ;
      movd  edi, xmm3
      and   edi, MMCFG_RULE_ADDR_MASK                   ; edi = MMCFG base addr
      mov   ebx, CSR_IOPORT_TARGET_LIST_0_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_1_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_2_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_3_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_4_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_5_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_6_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_7_CBO_SPR
      mov   dword [ebx + edi], eax                      ; write data to csr

      ;
      ; MMIO RULE's target fields programming
      ;
      and   eax, (BIT4+BIT3+BIT2+BIT1+BIT0)
      shl   eax, 22
      mov   edx, eax
      mov   ch, 20
      mov   ebx, CSR_MMIO_RULE0_CBO_SPR
eac_50_spr:
      mov   eax, 0xf83fffff                             ; clear bit[26:22] of MMIO RULE for Target
      and   eax, dword [ebx + edi]
      or    eax, edx                                    ; set the target [26:22]
      mov   dword [ebx + edi], eax                      ; write data to csr
      add   ebx, 4
      dec   ch
      jnz   eac_50_spr

      ;
      ; Enable the MMIO_RULE19 to cover the 0xFC000000 to 0xFFFFFFFF so that the NEM DATA region can be mapped to it.
      ; The target is set to local IIO.
      ;
      mov   ebx, CSR_MMIO_RULE19_CBO_SPR
      mov   eax, 0xffc00002                                       ; clear bit[21:12] limit & bit[11:2] base & bit[0] rule_enable
      and   eax, dword [ebx + edi]
      mov   edx, ((MMIO_RULE19_LIMITADDRESS >> 22) << 12)         ; limitaddress = 0xFFFFFFFF of bit21:12==bit31:22
      or    edx, ((MMIO_RULE19_BASEADDRESS >> 22) << 2) + BIT0    ; Base = 0xFC000000 of bit11:2==bit31:22 + enable bit
      or    eax, edx
      mov   dword [ebx + edi], eax                                ; write data to csr
      RET_ESI_MMX3

;----------------------------------------------------------------------------
; Executed by all socket BSPs present in the system early after reset
; - Enable MMCFG_RULE on local socket
; - Set IO, MMIO target lists  to point to legacy socket
; - Init MMIO_RULE11 to cover the 64MB below 4GB for NEM DATA region
; -
;  Input:  All general purpose registers undefined
;          Stack may not be available
;          MMX4 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;  Output: All general purpose registers undefined.
;          MMCFG and some related CSRs are initialized to allow CSR accesses on local socket via MMCFG method
;          Following xmm registers hold MMCFG and bus number config scheme info (later used by otehr routines in this file)
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legacy socket id
;                             [27:8] CPU Family/Stepping info (from cpuid eax, bit[19:0])
;                             [29:28]  rsvd
;                             [30]    Legacy socket flag
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;----------------------------------------------------------------------------

global ASM_PFX(EarlyUncoreInit_Gnr)
ASM_PFX(EarlyUncoreInit_Gnr):
      ;
      ; All sockets come here after reset
      ;
      ; In CR4, set OSFXSR bit 9 and OSxmmEXCPT bit 10 to enable access to xmm registers used by this routine
      ;
      mov   eax, cr4
      or    eax, (BIT10+BIT9)
      mov   cr4, eax

      ; Use pre-defined scheme for temp config
      mov   eax, DEFAULT_COLDBOOT_CPUBUSNO
      movd  xmm4, eax

      ;
      ; At this point, only CF8/CFC IO port can be used for PCI config access
      ;

      ;
      ; Disable LOCK lockcontrol.lockdisable  at 0:8:0:0x50, bit0
      ; TODO:    bit0 is set by default already?
      ;
      ;mov   eax, CSR_LEGACY_LOCKCONTROL_GNR
      ;READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      ;or    ebx, BIT0                                   ; set bit0
      ;WRITE_PCI_DWORD_LEGACY

      ;
      ; Read local socket ID and legacy socket ID, and store them in xmm0
      ;
      mov   eax, CSR_LEGACY_SAD_CONTROL_CBO_GNR         ; get local  socket ID
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      mov   ecx, ebx
      and   ecx, (BIT2+BIT1+BIT0)                       ; keep bit[2:0] local socket ID, ignore bit3

      mov   eax, CSR_LEGACY_SAD_TARGET_CBO_GNR          ; get legacy socket ID
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      and   ebx, (BIT21+BIT20+BIT19+BIT18+BIT17)        ; keep bit[21:17] legacy_pch_target
      shr   ebx, 13                                     ; move the legacy_pch_target to [8:4]
      btr   ebx, 8                                      ; test & clear bit8  (local/remote?)
      jnc   short eui_02_gnr                            ; remote socket ID

      ;
      ; For legacy socket,  use local socket ID as legacy socket ID, and set legacy socket flag bit
      ;
      mov   ebx, ecx
      shl   ebx, 4                                      ; bit[7:4] = Legacy socket id
      or    ebx, BIT30                                  ; bit[30] = Legacy Socket flag
eui_02_gnr:
      or    ecx, ebx
      movd  xmm0, ecx                                   ; xmm0[7:4] = legacy socket id, [3:0]= local socket id

      ; xmm4 = CPUSUBNO [Bus0,Bus1]
      mov   eax, CSR_LEGACY_CPUBUSNO_UBOX_GNR            ; set UBOX busno 0/1/2/3
      movd  ebx, xmm4
      WRITE_PCI_DWORD_LEGACY

      movd  eax, xmm0                                    ; xmm0 bit[30] = Legacy Socket flag
      bt    eax, 30
      jnc   short eui_03_gnr                             ; PBSP, don't set UBOX CPUBUSNO valid bit
      mov   eax, CSR_LEGACY_CPUBUSNO_VALID_UBOX_GNR      ; set Valid bit = 1
      READ_PCI_DWORD_LEGACY                              ; ebx = reg data
      or    ebx, BIT31                                   ; set bit31
      WRITE_PCI_DWORD_LEGACY
eui_03_gnr:

      ;
      ; Get CPU Faimly/stepping info and store it in xmm0[27:8]
      ;
      mov  eax, 1
      cpuid
      and  eax, 0xfffff                                 ; keep Falily/Stepping info
      shl  eax, 8                                       ; shift to bit[27:8]
      movd ebx, xmm0
      or   ebx, eax
      movd xmm0, ebx

      ;
      ; Read warm reset flag from sticky scratch pad reg and save it in xmm0
      ;
      mov   eax, SSR_LEGACY_BOOT_CONFIG_INFO_CSR_GNR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      test  ebx, BIT5                                   ; Warm Reset Path (Cold Reset Flow Completed = 1)?
      jnz   eui_warmboot_gnr                            ; Yes
      test  ebx, BIT4                                   ; Cold Reset Flow Entered flag = 1?
      jz    eui_coldboot_gnr                            ; No (normal cold boot path)

      ; Warm reset occured before completion of MRC on the cold boot path.
      ; Detect and handle if this is special LT_SHUTDOWN case in 4S
      test  ebx, BIT29                                  ; Remote Socket released in LT_ENABLED system?
      jnz   short eui_coldboot_ltshutdown_gnr           ; No (normal cold boot path without delay)

      ;
      ; Warm reset occured before completion of MRC on the cold boot path.
      ; In this case SBSP triggers a power-good reset while other socket BSPs waits
      ;

      CALL_MMX6 ASM_PFX(AmILegacySocketNoStack)         ; am I legacy socket?
      jnz   eui_06_gnr                                  ; no, skip
      ; Legacy BSP comes here - Triggers a power-good reset.
      mov   al, 0xe
      mov   dx, 0xcf9
      out   dx, al

eui_06_gnr:
      jmp   $                                           ; wait for reset to occur

eui_coldboot_gnr:
; ltshutdown: delay before entering cold-boot path for "signed production ACM on a debug PCH"
      rdtsc
      mov    ebx, eax
  lt_01_gnr:
      rdtsc
      sub    eax, ebx
      jnc    short lt_01a_gnr
      neg    eax
      inc    eax
  lt_01a_gnr:
      cmp    eax, 03*1024*1024*5                        ;;5ms on 3Ghz cpu
      jb     lt_01_gnr

eui_coldboot_ltshutdown_gnr:
      mov   eax, SSR_LEGACY_BOOT_CONFIG_INFO_CSR_GNR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      or    ebx, BIT4                                   ; set Cold Boot Flow Entered flag = 1
      ; Clear remote socket released bit
      btr   ebx, 29
      WRITE_PCI_DWORD_LEGACY                            ; write data to CSR
      jmp   eui_20_gnr

eui_warmboot_gnr:                                       ; Warm reset path
      ;and   ebx, NOT (BIT4)                             ; clear bit4 of scratch pad register
      btr   ebx, 4                                      ; clear bit4 of scratch pad register
      ; Clear remote socket released bit
      btr   ebx, 29

      WRITE_PCI_DWORD_LEGACY                            ; write data to CSR

      movd  ebx, xmm0
      or    ebx, BIT31                                  ; Set warm reset path flag in xmm reg
      movd  xmm0, ebx

eui_20_gnr:
      ;
      ; Initialize  MMCFG_TARGET_LIST (at 1:29:1:ec)  to target local socket
      ;

      mov   eax, CSR_LEGACY_MMCFG_TARGET_LIST_CBO_GNR
      READ_PCI_DWORD_LEGACY                             ; ebx = default value of  MMCFG_TARGET_LIST CSR
      or    ebx, 0x88888888                             ; make sure that all entries are marked as "local"
      WRITE_PCI_DWORD_LEGACY                            ; write it back
      movd  xmm1, ebx                                   ; save it in xmm1

      ;
      ; Initialize  CSR_LEGACY_MMCFG_LOCAL_RULE_CFG_CBO & CSR_LEGACY_MMCFG_LOCAL_RULE_CFG2_CBO (at 1:29:1:c8 & cc)
      ;    ModeX=0 means  "send devices 16-31 at the root bus to the stack rather than to ubox"
      ;    ModeX=1 means  "send everything to the ubox"
      ;    For all sockets, set Mode=0
      ;
      mov  eax, CSR_LEGACY_MMCFG_LOCAL_RULE_CFG_CBO_GNR
      mov  ebx, DEFAULT_MMCFG_LOCAL_RULE_CFG_SPR
      WRITE_PCI_DWORD_LEGACY
      mov  eax, CSR_LEGACY_MMCFG_LOCAL_RULE_CFG2_CBO_GNR
      mov  ebx, DEFAULT_MMCFG_LOCAL_RULE_CFG2_SPR
      WRITE_PCI_DWORD_LEGACY

      ;Initialize MMCFG_LOCAL_RULE_ADDRESSx_CFG (at 1:29:1:c8, b0 & b4 & b8 & bc)
      ;program local stack bus numbers in Cha
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_GNR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS0_GNR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_GNR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS1_GNR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_CBO_GNR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS2_GNR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_CBO_GNR
      mov   ebx, DEFAULT_MMCFG_LOCAL_RULE_ADDRESS3_GNR      ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      ;
      ; Initialize and enable MMCFG_Rule register
      ; a. init Cbo MMCFG_RULE  (at 1:29:1:C0)
      ;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_GNR + 4      ; clear MMCFG high address
      xor   ebx, ebx
      WRITE_PCI_DWORD_LEGACY
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_GNR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      mov   ecx, MMCFG_RULE_ADDR_MASK
      not   ecx
      and   ebx, ecx                                    ; mask off base address
      and   bl, 0xf0                                    ; mask off Length, keep Valid_Targets
; APTIOV_SERVER_OVERRIDE_RC_START : To resolve Build error      
      ;or    ebx, [ASM_PFX(PcdGet64 (PcdPciExpressBaseAddress))] ; update base addr
      ;or    ebx, 8                                              ; update length
      or    ebx, DEFAULT_MMCFG_RULE                     ; update base addr and length
; APTIOV_SERVER_OVERRIDE_RC_END : To resolve Build error 
      ;;or    ebx, 0ffh SHL 6                           ; set valid bit for all local targets (default)
      or    ebx, BIT0                                   ; set enable bit
      WRITE_PCI_DWORD_LEGACY
      movd  xmm3, ebx                                   ; save current reg value in xmm3

; ----------------------------------------------------------------------------------------------
; From this point on, local CSRs can be accessed at default bus numbers via MMCFG method
; Since MMCFG_TARGET_LIST points to local socket only, all CSR access are local
; ----------------------------------------------------------------------------------------------

      ;
      ; Program IOPORT_TARGET_LIST, MMIO_TARGET_LIST  before any io transaction. Legacy socket is the target
      ;
      mov   eax, 0x10101010                             ; target list for legacy socket itself
      CALL_MMX6 ASM_PFX(AmILegacySocketNoStack)         ; am I legacy socket?
      je    eac_40_gnr                                  ; yes

      movd  eax, xmm0
      and   eax, (BIT7+BIT6+BIT5+BIT4)                  ; eax [7:4] = legacy socket id, it is always run by PBSP, so the highest bit of target should be always 0
      shr   eax, 4
      mov   ebx, eax                                    ; ebx[3:0] = eax[3:0] = legacy socket id
      mov   cl, 8                                       ; number of bits for socket id
      mov   ch, 3
eac_30_gnr:
      shl   ebx, cl
      or    eax, ebx
      dec   ch
      jnz   eac_30_gnr

eac_40_gnr:
      ;
      ; edi = MMCFG base addr
      ; eax = target list with all entries targeting legacy socket
      ;
      movd  edi, xmm3
      and   edi, MMCFG_RULE_ADDR_MASK                   ; edi = MMCFG base addr
      mov   ebx, CSR_IOPORT_TARGET_LIST_0_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_1_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_2_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_3_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_4_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_5_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_6_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr
      mov   ebx, CSR_IOPORT_TARGET_LIST_7_CBO_GNR
      mov   dword [ebx + edi], eax                      ; write data to csr

      ;
      ; MMIO RULE's target fields programming
      ;
      and   eax, (BIT4+BIT3+BIT2+BIT1+BIT0)
      shl   eax, 22
      mov   edx, eax
      mov   ch, 20
      mov   ebx, CSR_MMIO_RULE0_CBO_GNR
eac_50_gnr:
      mov   eax, 0xf83fffff                             ; clear bit[26:22] of MMIO RULE for Target
      and   eax, dword [ebx + edi]
      or    eax, edx                                    ; set the target [26:22]
      mov   dword [ebx + edi], eax                      ; write data to csr
      add   ebx, 4
      dec   ch
      jnz   eac_50_gnr

      ;
      ; Enable the MMIO_RULE19 to cover the 0xFC000000 to 0xFFFFFFFF so that the NEM DATA region can be mapped to it.
      ; The target is set to local IIO.
      ;
      mov   ebx, CSR_MMIO_RULE19_CBO_GNR
      mov   eax, 0xffc00002                                       ; clear bit[21:12] limit & bit[11:2] base & bit[0] rule_enable
      and   eax, dword [ebx + edi]
      mov   edx, ((MMIO_RULE19_LIMITADDRESS >> 22) << 12)         ; limitaddress = 0xFFFFFFFF of bit21:12==bit31:22
      or    edx, ((MMIO_RULE19_BASEADDRESS >> 22) << 2) + BIT0    ; Base = 0xFC000000 of bit11:2==bit31:22 + enable bit
      or    eax, edx
      mov   dword [ebx + edi], eax                                ; write data to csr
      RET_ESI_MMX3

;----------------------------------------------------------------------------
; Executed by all socket BSPs present in the system early after reset
;
;  Input:  All general puropse registers undefined
;          Stack may not be available
;          MMX3 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;  Output: All general puropse registers undefined.
;----------------------------------------------------------------------------
global ASM_PFX(MidUncoreInit)
ASM_PFX(MidUncoreInit):

        RET_ESI_MMX3

;----------------------------------------------------------------------------
; Procedure to shadow SEC/PEI code into cache to prevent DMI SPI access during
;   mode activation.
;
;  Input:  All general puropse registers undefined
;          Stack may not be available
;          MMX3 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;  Output: All general puropse registers undefined.
;----------------------------------------------------------------------------
global ASM_PFX(ShadowCodeInCache)
ASM_PFX(ShadowCodeInCache):

        mov     eax, [ASM_PFX(PcdGet32 (PcdSecInFsp))]
        cmp     eax, 0
        jz      NotInFsp_4
        ; TODO - FSP Need to find better way to store defines and access
        movd    eax, xmm2                 ; restore fsp pointer
        mov     edi, dword [eax + 0x28]   ; Code region base
        mov     eax, dword [eax + 0x2C]   ; Code region size
        jmp     InFsp_4
NotInFsp_4:
        mov     eax, [ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionSize))]
        mov     edi, [ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionBase))]
InFsp_4:
        mov ecx, eax
        shr ecx, 6 ;divide by 64 Bytes (cache line size)
FafWa:
        mov eax, DWORD [edi]
        add edi, 64
        loop FafWa
        ret

;----------------------------------------------------------------------------
; Executed by all socket BSPs present in the system early after reset
;
;  Input:  All general puropse registers undefined
;          Stack may not be available
;          MMX3 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;  Output: All general puropse registers undefined.
;----------------------------------------------------------------------------
global ASM_PFX(PostNemUncoreInit_Spr)
ASM_PFX(PostNemUncoreInit_Spr):
        call ASM_PFX(ProcessorWorkaround)       ; Early CPU WAs

        ;
        ; Detect Boot Guard Boot
        ;
        mov   ecx, MSR_BOOT_GUARD_SACM_INFO;
        rdmsr
        test  eax, BIT0                         ; BootGuard status = 1?
        jnz   SkipShadowInBtGEn                 ; Yes

        ; call Fafwa code to shadow the code to cache in cold and warm boot
        call  ASM_PFX(ShadowCodeInCache)

SkipShadowInBtGEn:
        ; skip the shadow when the BtG is enabled
        ret

;----------------------------------------------------------------------------
; Executed by all socket BSPs present in the system early after reset
;
;  Input:  All general puropse registers undefined
;          Stack may not be available
;          MMX3 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;  Output: All general puropse registers undefined.
;----------------------------------------------------------------------------
global ASM_PFX(PostNemUncoreInit_Gnr)
ASM_PFX(PostNemUncoreInit_Gnr):
        call ASM_PFX(ProcessorWorkaround)       ; Early CPU WAs

; CPL3 change - Enable Code after Mailbox Command is available.
        call  ASM_PFX(GetMmcfgBase)             ; esi = MMCFG.base
        mov   ebx, CSR_BIOSSCRATCHPAD7_GNR

        ; Only set P1 on warm reset
        mov   eax, dword [ebx + esi]
        and   eax, BIT5
        jz    EndCpl3_Gnr

        ; PCU Mailbox Interface
        mov   ebx, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN0_REG_GNR
        mov   cl, 30
RunBusyLoop1_Gnr:
        dec   cl
        jz    short RunBusyLoopExit_Gnr
        test  dword [ebx + esi], 0x80000000     ;Check for RUN_BUSY bit
        jnz   short RunBusyLoop1_Gnr

        ; Set mailbox data register
        mov   ebx, CSR_BIOS_MAILBOX_DATA_PCU_FUN0_REG_GNR
        mov   dword [ebx + esi], BIOS_MAILBOX_DATA_SWITCHTO_P1

        ; Write mailbox cmd register
        mov   ebx, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN0_REG_GNR
        mov   dword [ebx + esi], BIOS_MAILBOX_COMMAND_SWITCHTO_P1      ; Request for P1 speed

        mov   cl, 30
RunBusyLoop2_Gnr:
        dec   cl
        jz    short RunBusyLoopExit_Gnr
        test  dword [ebx + esi], 0x80000000     ;Check for RUN_BUSY bit
        jnz   short RunBusyLoop2_Gnr
RunBusyLoopExit_Gnr:

EndCpl3_Gnr:
        ;
        ; Detect Boot Guard Boot
        ;
        mov   ecx, MSR_BOOT_GUARD_SACM_INFO;
        rdmsr
        test  eax, BIT0                         ; BootGuard status = 1?
        jnz   SkipShadowInBtGEn_Gnr             ; Yes
; CPL3 change
        ; call Fafwa code to shadow the code to cache in cold and warm boot
        call  ASM_PFX(ShadowCodeInCache)

SkipShadowInBtGEn_Gnr:
        ; skip the shadow when the BtG is enabled
        ret

;----------------------------------------------------------------------------
; EnableAccessGlobalCSR_NoStack
; Executed by SBSP Legacy socket only
;     - Update MMCFT_TARGET_LIST to enable cross-socket CSR access,
;     - Enable CPUBUSNO/CPUBUSNO1
;-----------------------------------------------------------------------
;
;  Input:  All general purpose registers undefined
;          Stack may not be available
;          MMX3 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;          Following xmm registers have been initialized to hold such config info:
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [27:8] Cpu Family/Stepping info (from cpuid eax, bit[19:0])
;                             [29:28]  rsvd
;                             [30]    Legacy socket flag
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
;  Output: All general purpose registers undefined.
;          MMCFG_TARGET_LIST, CPUBUSNO related CSRs are modified/initialized.
;----------------------------------------------------------------------------

global ASM_PFX(EnableAccessGlobalCSR_NoStack_Spr)
ASM_PFX(EnableAccessGlobalCSR_NoStack_Spr):

      movd  edi, xmm3
      and   edi, MMCFG_RULE_ADDR_MASK           ; edi = MMCFG_BASE_ADDR

      ; init MMCFG CSR in IIO (at 0:0:0:90)
      mov   ebx, CSR_MMCFG_IIO_SPR
      movd  eax, xmm3                           ; eax = copy of MMCFG_RULE
      and   eax, MMCFG_RULE_ADDR_MASK           ; mask off bit[13:0]
      mov   dword [ebx + edi], eax              ; write to CSR
      ; clear the higher address dword
      xor   eax, eax
      mov   dword [ebx + edi + 4], eax          ; write to CSR

      ; calculate mmcfg limit = mmcfg_base + Max_Bus * 1MB)
      movd  ecx, xmm3                           ; ecx = copy of MMCFG_RULE
      and   ecx, 1110b
      shr   cl, 1                               ; cl = max bus number encoding.
                                                ;      0: 2048, 1: 1024, 2:512, 3: rsvd, 4: 256, 5: 128, 6: 64
      cmp   cl, 4
      jb    gcs20_spr
      dec   cl                                  ;  adjuct shift count in cl
gcs20_spr:
      mov   eax, 0x800                          ; eax = 2048 (buses)
      shr   eax, cl                             ; eax = 256 / 128 / 64 buses
      shl   eax, 20                             ; eax = eax * 100000h (i.e.Max_Bus * 1MB)

      movd  ecx, xmm3                           ; ecx = copy of MMCFG_RULE
      and   ecx, MMCFG_RULE_ADDR_MASK           ; ecx = MMCFG_BASE_ADDR
      add   eax, ecx                            ; eax = MMCFG_BASE_ADDR + MMCFG_SIZE
      sub   eax, 1                              ; last decode byte is last byte in previous 64MB block
      and   eax, 0xfc000000                     ; 64MB block address of last block of MMCFG
      add   ebx, CSR_MMCFG_IIO_BASE_LIMIT_GAP_SPR ; CSR_MMCFG_IIO + gap   (MMCFG.Limit) (HSXB0 Update: Change CSR_MMCFG_IIO_BASE_LIMIT_GAP_A0 to CSR_MMCFG_IIO_BASE_LIMIT_GAP)
      mov   dword [ebx + edi], eax              ; write back CSR

;-----------------------------------------------------------------------
;     - Reprogram MMCFG_TARGET_LIST to enable cross-socket CSR access from legacy socket
;     - Enable CPUBUSNO/CPUBUSNO1
;-----------------------------------------------------------------------

; Choose MMCFG target list based on SBSP (legacy) socketID, so that
;   - Each of the 8 sockets is mapped to a range of 32 (20h) buses
;   - Legacy socket is always mapped to the bus range 00h - 1Fh
;
; SBSP    MMCFG Target_list
; -----  -------------------------
;  S0        76543210   (Default)
;  S1        76543201
;  S2        76543102
;  S3        76542103
;  S4        76532104
;  S5        76432105
;  S6        75432106
;  S7        65432107

; Get legacy socketID
      movd  edx, xmm0                               ; DL[7:4] = legacy socketID
      shr   edx, 4
      and   edx, (BIT3+BIT2+BIT1+BIT0)              ; DL = legacy socketID

      mov   eax, S0_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 0                                   ; Legacy socketID = 0?
      je    gcs50_spr                               ; Yes
      mov   eax, S1_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 1                                   ; Legacy socketID = 1?
      je    gcs50_spr                               ; Yes
      mov   eax, S2_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 2                                   ; Legacy socketID = 2?
      je    gcs50_spr                               ; Yes
      mov   eax, S3_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 3                                   ; Legacy socketID = 3?
      je    gcs50_spr                               ; Yes
      mov   eax, S4_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 4                                   ; Legacy socketID = 4?
      je    gcs50_spr                               ; Yes
      mov   eax, S5_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 5                                   ; Legacy socketID = 5?
      je    gcs50_spr                               ; Yes
      mov   eax, S6_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 6                                   ; Legacy socketID = 6?
      je    gcs50_spr                               ; Yes
      mov   eax, S7_MMCFG_TARGET_LIST_GLOBAL
;      cmp   dl, 7                                   ; Legacy socketID = 7?
;      je    gcs50_spr                               ; Yes

gcs50_spr:
; Map the first 32 buses (00h to 1Fh) to legacy socket
      or    eax, BIT3                           ; set Local bit
      mov   ebx, CSR_MMCFG_TARGET_LIST_CBO_SPR
      mov   dword [ebx + edi], eax              ; write CSR
      movd  xmm1, eax                           ; save it in xmm1

;------------------------------------------------------------------------------
; Setup the non-default BUS NO in Legacy Socket
;------------------------------------------------------------------------------
      ;
      ; Disable current MMCFG_RULE (at default bus#)
      ;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_SPR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      and   ebx, MMCFG_RULE_CLR_ENABLE                  ; set enable bit
      WRITE_PCI_DWORD_LEGACY

      ;Initialize MMCFG_LOCAL_RULE_ADDRESSx_CFG (at 1:29:1:b0, b4, b8 & bc)
      ;program local stack bus numbers in Cha
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_SPR
      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_SPR          ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_SPR          ; ebx = CSR data
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_SPR
      WRITE_PCI_DWORD_LEGACY

      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_SPR          ; ebx = CSR data
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_CBO_SPR
      WRITE_PCI_DWORD_LEGACY

      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_SPR          ; ebx = CSR data
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_CBO_SPR
      WRITE_PCI_DWORD_LEGACY
      ;
      ; Initialize and enable MMCFG_Rule register
      ; a. init Cbo MMCFG_RULE  (at 1:29:1:C0)
      ;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_SPR + 4      ; clear MMCFG high address
      xor   ebx, ebx
      WRITE_PCI_DWORD_LEGACY
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_SPR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      mov   ecx, MMCFG_RULE_ADDR_MASK
      not   ecx
      and   ebx, ecx                                    ; mask off base address
      and   bl, 0xf0                                    ; mask off Length, keep Valid_Targets
; APTIOV_SERVER_OVERRIDE_RC_START : To resolve Build error      
      ;or    ebx, [ASM_PFX(PcdGet64 (PcdPciExpressBaseAddress))] ; update base addr
      ;or    ebx, 8                                              ; update length
      or    ebx, DEFAULT_MMCFG_RULE                     ; update base addr and length
; APTIOV_SERVER_OVERRIDE_RC_END : To resolve Build error 
      or    ebx, BIT0                                   ; set enable bit
      WRITE_PCI_DWORD_LEGACY

      mov   eax, LEGACY_COLDBOOT_CPUBUSNO           ; BUS NO U0 and U1 for PEI to Pre-KTI
      movd  xmm4, eax

;
; Program  UBOX CPUBUSNO/CPUBUSNO1 CSRs
;
      mov   ebx, CSR_CPUBUSNO_UBOX_SPR
      movd  eax, xmm4
      mov   dword [ebx + edi], eax           ; write CSR

;
; Set CPUBUSNO_VALID.busno_programmed bit to 1
;
      mov   ebx, CSR_CPUBUSNO_VALID_UBOX_SPR
      mov   eax, dword [ebx + edi]           ; eax = CSR data
      or    eax, BIT31                       ; set busno_programmed = 1
      mov   dword [ebx + edi], eax           ; write CSR

;
; Program default IOAPIC_TARGET_LIST
;
      mov   ebx, CSR_IOAPIC_TARGET_LIST_0_SPR
      mov   eax, DEFAULT_IOAPIC_TARGET_LIST_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_1_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_2_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_3_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_4_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_5_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_6_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_7_SPR
      mov   dword [ebx + edi], eax           ; write CSR

;-------------------------------------------------------------------------------
; From this point on, inter-socket CSR access is enabled
; Valid matching bus# must be used to access all CSRs.
;-------------------------------------------------------------------------------
;
; Program rootbusSvalid bit VTD to enable bus decode by MS2IOSF
;
      CALL_MMX6  ASM_PFX(AmILegacySocketNoStack)
      jnz   NonLegacySocket_Spr
      ;
      ; Enable rootbusSvalid to DMI stack MS2IOSF
      ;
      mov   ebx, CSR_ROOTBUS_IIO_VTD_REG_SPR
      mov   eax, dword [edi + ebx]        ; Read CSR
      or    eax, ROOT_BUSS_VALID          ; enable root bus S
      mov   dword [edi + ebx], eax        ; Write CSR

NonLegacySocket_Spr:

     RET_ESI_MMX3                              ; return

;----------------------------------------------------------------------------
; EnableAccessGlobalCSR_NoStack
; Executed by SBSP Legacy socket only
;     - Update MMCFT_TARGET_LIST to enable cross-socket CSR access,
;     - Enable CPUBUSNO/CPUBUSNO1
;-----------------------------------------------------------------------
;
;  Input:  All general purpose registers undefined
;          Stack may not be available
;          MMX3 = routine return address
;          MMCFG_Rule is enabled on local socket
;
;          Following xmm registers have been initialized to hold such config info:
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [27:8] Cpu Family/Stepping info (from cpuid eax, bit[19:0])
;                             [29:28]  rsvd
;                             [30]    Legacy socket flag
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
;  Output: All general purpose registers undefined.
;          MMCFG_TARGET_LIST, CPUBUSNO related CSRs are modified/initialized.
;----------------------------------------------------------------------------

global ASM_PFX(EnableAccessGlobalCSR_NoStack_Gnr)
ASM_PFX(EnableAccessGlobalCSR_NoStack_Gnr):

      movd  edi, xmm3
      and   edi, MMCFG_RULE_ADDR_MASK           ; edi = MMCFG_BASE_ADDR

      ; init MMCFG CSR in IIO (at 0:0:0:90)
      mov   ebx, CSR_MMCFG_IIO_SPR
      movd  eax, xmm3                           ; eax = copy of MMCFG_RULE
      and   eax, MMCFG_RULE_ADDR_MASK           ; mask off bit[13:0]
      mov   dword [ebx + edi], eax              ; write to CSR
      ; clear the higher address dword
      xor   eax, eax
      mov   dword [ebx + edi + 4], eax          ; write to CSR

      ; calculate mmcfg limit = mmcfg_base + Max_Bus * 1MB)
      movd  ecx, xmm3                           ; ecx = copy of MMCFG_RULE
      and   ecx, 1110b
      shr   cl, 1                               ; cl = max bus number encoding.
                                                ;      0: 2048, 1: 1024, 2:512, 3: rsvd, 4: 256, 5: 128, 6: 64
      cmp   cl, 4
      jb    gcs20_gnr
      dec   cl                                  ;  adjuct shift count in cl
gcs20_gnr:
      mov   eax, 0x800                          ; eax = 2048 (buses)
      shr   eax, cl                             ; eax = 256 / 128 / 64 buses
      shl   eax, 20                             ; eax = eax * 100000h (i.e.Max_Bus * 1MB)

      movd  ecx, xmm3                           ; ecx = copy of MMCFG_RULE
      and   ecx, MMCFG_RULE_ADDR_MASK           ; ecx = MMCFG_BASE_ADDR
      add   eax, ecx                            ; eax = MMCFG_BASE_ADDR + MMCFG_SIZE
      sub   eax, 1                              ; last decode byte is last byte in previous 64MB block
      and   eax, 0xfc000000                     ; 64MB block address of last block of MMCFG
      add   ebx, CSR_MMCFG_IIO_BASE_LIMIT_GAP_SPR ; CSR_MMCFG_IIO + gap   (MMCFG.Limit) (HSXB0 Update: Change CSR_MMCFG_IIO_BASE_LIMIT_GAP_A0 to CSR_MMCFG_IIO_BASE_LIMIT_GAP)
      mov   dword [ebx + edi], eax              ; write back CSR

;-----------------------------------------------------------------------
;     - Reprogram MMCFG_TARGET_LIST to enable cross-socket CSR access from legacy socket
;     - Enable CPUBUSNO/CPUBUSNO1
;-----------------------------------------------------------------------

; Choose MMCFG target list based on SBSP (legacy) socketID, so that
;   - Each of the 8 sockets is mapped to a range of 32 (20h) buses
;   - Legacy socket is always mapped to the bus range 00h - 1Fh
;
; SBSP    MMCFG Target_list
; -----  -------------------------
;  S0        76543210   (Default)
;  S1        76543201
;  S2        76543102
;  S3        76542103
;  S4        76532104
;  S5        76432105
;  S6        75432106
;  S7        65432107

; Get legacy socketID
      movd  edx, xmm0                               ; DL[7:4] = legacy socketID
      shr   edx, 4
      and   edx, (BIT3+BIT2+BIT1+BIT0)              ; DL = legacy socketID

      mov   eax, S0_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 0                                   ; Legacy socketID = 0?
      je    gcs50_gnr                               ; Yes
      mov   eax, S1_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 1                                   ; Legacy socketID = 1?
      je    gcs50_gnr                               ; Yes
      mov   eax, S2_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 2                                   ; Legacy socketID = 2?
      je    gcs50_gnr                               ; Yes
      mov   eax, S3_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 3                                   ; Legacy socketID = 3?
      je    gcs50_gnr                               ; Yes
      mov   eax, S4_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 4                                   ; Legacy socketID = 4?
      je    gcs50_gnr                               ; Yes
      mov   eax, S5_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 5                                   ; Legacy socketID = 5?
      je    gcs50_gnr                               ; Yes
      mov   eax, S6_MMCFG_TARGET_LIST_GLOBAL
      cmp   dl, 6                                   ; Legacy socketID = 6?
      je    gcs50_gnr                               ; Yes
      mov   eax, S7_MMCFG_TARGET_LIST_GLOBAL
;      cmp   dl, 7                                   ; Legacy socketID = 7?
;      je    gcs50_gnr                               ; Yes

gcs50_gnr:
; Map the first 32 buses (00h to 1Fh) to legacy socket
      or    eax, BIT3                           ; set Local bit
      mov   ebx, CSR_MMCFG_TARGET_LIST_CBO_GNR
      mov   dword [ebx + edi], eax              ; write CSR
      movd  xmm1, eax                           ; save it in xmm1

;------------------------------------------------------------------------------
; Setup the non-default BUS NO in Legacy Socket
;------------------------------------------------------------------------------
      ;
      ; Disable current MMCFG_RULE (at default bus#)
      ;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_GNR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      and   ebx, MMCFG_RULE_CLR_ENABLE                  ; set enable bit
      WRITE_PCI_DWORD_LEGACY

      ;Initialize MMCFG_LOCAL_RULE_ADDRESSx_CFG (at 1:29:1:b0, b4, b8 & bc)
      ;program local stack bus numbers in Cha
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_GNR
      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_GNR          ; ebx = CSR data
      WRITE_PCI_DWORD_LEGACY

      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_GNR          ; ebx = CSR data
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_GNR
      WRITE_PCI_DWORD_LEGACY

      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_GNR          ; ebx = CSR data
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_CBO_GNR
      WRITE_PCI_DWORD_LEGACY

      mov   ebx, LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_GNR          ; ebx = CSR data
      mov   eax, CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_CBO_GNR
      WRITE_PCI_DWORD_LEGACY
      ;
      ; Initialize and enable MMCFG_Rule register
      ; a. init Cbo MMCFG_RULE  (at 1:29:1:C0)
      ;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_GNR + 4      ; clear MMCFG high address
      xor   ebx, ebx
      WRITE_PCI_DWORD_LEGACY
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_GNR
      READ_PCI_DWORD_LEGACY                             ; ebx = reg data
      mov   ecx, MMCFG_RULE_ADDR_MASK
      not   ecx
      and   ebx, ecx                                    ; mask off base address
      and   bl, 0xf0                                    ; mask off Length, keep Valid_Targets
; APTIOV_SERVER_OVERRIDE_RC_START : To resolve Build error      
      ;or    ebx, [ASM_PFX(PcdGet64 (PcdPciExpressBaseAddress))] ; update base addr
      ;or    ebx, 8                                              ; update length
      or    ebx, DEFAULT_MMCFG_RULE                     ; update base addr and length
; APTIOV_SERVER_OVERRIDE_RC_END : To resolve Build error      
      or    ebx, DEFAULT_MMCFG_RULE                     ; update base addr and length
      or    ebx, BIT0                                   ; set enable bit
      WRITE_PCI_DWORD_LEGACY

      mov   eax, LEGACY_COLDBOOT_CPUBUSNO           ; BUS NO U0 and U1 for PEI to Pre-KTI
      movd  xmm4, eax

;
; Program  UBOX CPUBUSNO/CPUBUSNO1 CSRs
;
      mov   ebx, CSR_CPUBUSNO_UBOX_GNR
      movd  eax, xmm4
      mov   dword [ebx + edi], eax           ; write CSR

;
; Set CPUBUSNO_VALID.busno_programmed bit to 1
;
      mov   ebx, CSR_CPUBUSNO_VALID_UBOX_GNR
      mov   eax, dword [ebx + edi]           ; eax = CSR data
      or    eax, BIT31                       ; set busno_programmed = 1
      mov   dword [ebx + edi], eax           ; write CSR

;
; Program default IOAPIC_TARGET_LIST
;
      mov   ebx, CSR_IOAPIC_TARGET_LIST_0_GNR
      mov   eax, DEFAULT_IOAPIC_TARGET_LIST_SPR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_1_GNR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_2_GNR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_3_GNR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_4_GNR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_5_GNR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_6_GNR
      mov   dword [ebx + edi], eax           ; write CSR
      mov   ebx, CSR_IOAPIC_TARGET_LIST_7_GNR
      mov   dword [ebx + edi], eax           ; write CSR

;-------------------------------------------------------------------------------
; From this point on, inter-socket CSR access is enabled
; Valid matching bus# must be used to access all CSRs.
;-------------------------------------------------------------------------------
;
; Program rootbusSvalid bit VTD to enable bus decode by MS2IOSF
;
      CALL_MMX6  ASM_PFX(AmILegacySocketNoStack)
      jnz   NonLegacySocket_Gnr
      ;
      ; Enable rootbusSvalid to DMI stack MS2IOSF
      ;
      mov   ebx, CSR_ROOTBUS_IIO_VTD_REG_GNR
      mov   eax, dword [edi + ebx]        ; Read CSR
      or    eax, ROOT_BUSS_VALID          ; enable root bus S
      mov   dword [edi + ebx], eax        ; Write CSR

NonLegacySocket_Gnr:

     RET_ESI_MMX3                              ; return

;-------------------------------------------------------------------------
; CPU Early Workaround
;Executed by all package BSPs
; Input:
;    Stack available
; Output:
;    All general purpose registers destroyed
;-------------------------------------------------------------------------
global ASM_PFX(ProcessorWorkaround)
ASM_PFX(ProcessorWorkaround):

  call  ASM_PFX(_OEMCpuSecInit)
  ret

;-------------------------------------------------------------------------
; Read a Dword from PCI register 00h thru FFh using IO port CF8h/CFCh
; Input:
;   Stack is avaialble
;   xmm registers initialized.
;   xmm0 register bit [7:4]  contains Legacy Socket ID
; Output: AL = SBSP Socket ID
; Registers changed: None except eax
;-------------------------------------------------------------------------
global ASM_PFX(GetSbspSktId)
ASM_PFX(GetSbspSktId):
        movd eax, xmm0                  ; AL[7:4] = SBSP socket id
        shr  eax, 4
        and  eax, (BIT3+BIT2+BIT1+BIT0)
        ret

;-------------------------------------------------------------------------
; Returns Socket/Packge ID of the current processor
; Input:
;   Stack is avaialble
;   xmm registers initialized
;   xmm0 register bit[3:0] contains local Socket ID
; Output: CL = Socket/Package ID of the current processor
; Registers changed: None except ecx
;-------------------------------------------------------------------------
global ASM_PFX(GetLocalSktId)
ASM_PFX(GetLocalSktId):

     movd  ecx, xmm0                                      ; xmm0[3:0] stores the socketID
     and   ecx, (BIT3+BIT2+BIT1+BIT0)                     ; cl = local socket ID
     ret

;----------------------------------------------------------------------------
; SetupProcessorCheckinData
;  Executed by all socket BSPs.  Store data in its own local scratchpad register:
;       bit[0]        - check in flag
;       bit[23:8]   - ApicId  (only 16-bit is used)
;       bit[30:27] - stepping
;  Input:  Stack available
;       Local MMCFG config CSR  access is enabled
;  Output: All general puropse registers preserved.
;----------------------------------------------------------------------------

global ASM_PFX(SetupProcessorCheckinData)
ASM_PFX(SetupProcessorCheckinData):

        pushad

        mov    eax, 1
        cpuid                                               ; ebx[31:24] = initial APIC ID, eax[3:0] = stepping
        mov    edi, eax                                     ; save CPUID to edi, don't modify it till end of this routine
        and    ebx, 0xff000000
        shr    ebx, 16t                                     ; APIC ID in ebx[23:8]
        and    eax, 0xf
        shl    eax, 27t                                     ; eax[30:27] = stepping
        mov    esi, eax                                     ; esi [30:27] = setpping
        push   ebx                                          ; save ebx

        ; check to see if processor is x2Apic capable
        mov    eax, 0
        cpuid
        pop    ebx                                          ; restore ebx
        cmp    eax, 0xb                                     ; check to see if processor is x2Apic capable
        jb     SaveDataAndSetCheckIn                        ; processor is not x2APIC capable

        ; processor is x2Apic capable, get ApicId from cpuid leaf B
        mov    eax, 0xb
        xor    ecx, ecx
        cpuid
        movzx  ebx, dx                                      ; though X2Apid is 32bit wide,
        shl    ebx, 8                                       ; HW support up to 12 bits (bit31:12=0)

SaveDataAndSetCheckIn:
        ; ebx [23:8] = Apic ID,  esi [30:27] = stepping
        ; call   GetLocalSktId                                 ; CL = own local socket ID

        IS_SPR_CPU_FROM_XMM0
        jne    SetupProcessorCheckinDataMark1a
        mov    eax, SR_PBSP_CHECKIN_CSR_SPR
        jne    SetupProcessorCheckinDataMark1b
SetupProcessorCheckinDataMark1a:
        mov    eax, SR_PBSP_CHECKIN_CSR_GNR
SetupProcessorCheckinDataMark1b:
        call   ASM_PFX(ReadCpuCsr)                           ; EAX = current CSR data
        and    eax, 0x870000ff                               ; clear bit[30:27], [23:8]
        or     ebx, eax                                      ;  ebx <- apci id
        or     ebx, esi                                      ;  ebx <- stepping
        or     ebx, 1                                        ; set check in bit
        IS_SPR_CPU_FROM_XMM0
        jne    SetupProcessorCheckinDataMark2a
        mov    eax, SR_PBSP_CHECKIN_CSR_SPR
        jne    SetupProcessorCheckinDataMark2b
SetupProcessorCheckinDataMark2a:
        mov    eax, SR_PBSP_CHECKIN_CSR_GNR
SetupProcessorCheckinDataMark2b:
        call   ASM_PFX(WriteCpuCsr)

        popad
        ret

;-------------------------------------------------------------------------
; UncoreBusCfgSyncNbsp_Spr
;   A PBSP waits for and hand-shakes with SBSP to receive re-programming of
;   MMCFG/CPUBUSNO related registers
;
; Input:
;     Stack is available
;     Following xmm registers hold MMCFG and bus number config info
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
; Output:
;    PBSP has switched to the new MMCFG/CPUBUSNO configuration with global view of the MMCFG config space
;
; Registers changed: None
;-------------------------------------------------------------------------


global ASM_PFX(UncoreBusCfgSyncNbsp_Spr)
ASM_PFX(UncoreBusCfgSyncNbsp_Spr):

        pushad

;
;  At this point, all CSR accesses are local to the socket, and default bus# 0...5 are used for local targets
;

;
;  Disable CHA MMCFG_RULE
;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_SPR             ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      btr   ebx, 0                                         ; clear Enable bit
      call  ASM_PFX(WritePciDwordLegacy)                   ; write to reg
      movd  xmm3, ebx                                      ; update copy in xmm3
;
;  From this point, only CF8/CFC IO can be used for CSR access
;
;
;  Set flag in local scratchpad CSR to indicate to SBSP "I'm ready for MMCFG/BUSNO re-programming"
;
      mov   eax, SR_LEGACY_BUSCFG_SYNC_CSR_SPR              ; eax = reg address
      mov   ebx, BusCfgSync_PBSP_Ready                      ; ebx = reg dword data
      call  ASM_PFX(WritePciDwordLegacy)                    ; write to reg

PbspWait10_Spr:
;
;  Loop here on local scratchpad CSR
;  waiting for local MMCFG/CPUBUSNO etc.  CSRs be updated by SBSP,
;  until notified by SBSP to enable new configuration
;
      mov   eax, SR_LEGACY_BUSCFG_SYNC_CSR_SPR             ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      cmp   ebx, BusCfgSync_PBSP_Go
      jne   PbspWait10_Spr
;
;  At this point, some local CSRs have been updated by SBSP with new settings
;  Update local copy of the new CHA MMCFG_RULE, UBOX CPUBUSNO (in xmm registers)
;
      mov   eax, CSR_LEGACY_MMCFG_TARGET_LIST_CBO_SPR      ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      movd  xmm1, ebx                                      ; update xmm reg with new data
;
; Get local copy of mmcfg base to support multisegments
;
      mov   eax, SR_LEGACY_BUSCFG_DATA_CSR_SPR
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      or    ebx, BIT0
      movd  xmm3, ebx                                      ; update xmm reg with new data
      mov   eax, CSR_LEGACY_CPUBUSNO_UBOX_SPR
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      movd  xmm4, ebx                                      ; update xmm reg with new data

;
;  Enable UBOX CPUBUSNO_VALID
;
      mov   eax, CSR_LEGACY_CPUBUSNO_VALID_UBOX_SPR        ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      or    ebx, BIT31                                     ; set Valid bit = 1
      call  ASM_PFX(WritePciDwordLegacy)                   ; write to reg
;
;  From this point on, new CPUBUSNO must be used
;  Get new bus0/bus1 value
;
      movd  ecx, xmm4
      mov   edx, ecx

      and   ecx, 0xff                                      ; bit[7:0] = bus0 field
      shl   ecx, 16                                        ; ecx[23:16] = new bus# for local target 0 (Bus0, UBOX0)

      and   edx, 0xff00                                    ; bit[15:8] = bus1 field
      shl   edx, 8                                         ; edx[23:16] = new bus# for local target 1 (Bus1, UBOX1)

;
;  Enable  MMCFG_RULE in CHA, on Bus1
;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_SPR              ; eax = reg address
      and   eax, 0xff00ffff                                 ; clear bus# field
      or    eax, edx                                        ; update bus# field with new Bus1
      call  ASM_PFX(ReadPciDwordLegacy)                     ; ebx = reg dword data
      or    ebx, BIT0                                       ; set Enable bit = 1
      call  ASM_PFX(WritePciDwordLegacy)                    ; write to reg
;
; At this point, new CPUBUSNO and MMCFG_RULE are all enabled on the PBSP socket
;
;
;  Enable UBOX copy of MMCFG_RULE on Bus0
;

;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [Begin]
;
      ;
      ; Get S0 U1 MMCFG address and save in ecx[11:0]
      ;
      mov   eax, CSR_BIOSNONSTICKYSCRATCHPAD14_SPR          ; read the scratchpad14 to get S0 U1 MMCFG address
      call  ASM_PFX(ReadCpuCsr)                             ; eax = reg dword data
      mov   ecx, eax                                        ; ecx = S0 U1 mmcfg address
      shr   ecx, 20                                         ; ecx[11:0] = the upper 3 nibbles of S0 U1 mmcfg address
;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [End]
;

;
;  Send message back to SBSP to indicate "I'm done with enabling the new MMCFG/CPUBUS config"
;  via local scratchpad CSR in UBOX on Bus0
;
      mov   eax, SR_BUSCFG_SYNC_CSR_SPR                     ; eax = reg address
      mov   ebx, BusCfgSync_PBSP_Done                       ; ebx = reg dword data
      call  ASM_PFX(WriteCpuCsr)                            ; write to reg
;
;  Wait for SBSP to receive/process the message and clear the scartchpad CSR as acknoledgement
;
PbspWait50_Spr:
      mov   eax, SR_BUSCFG_SYNC_CSR_SPR                     ; eax = reg address
      call  ASM_PFX(ReadCpuCsr)                             ; eax = reg data
      cmp   eax, BusCfgSync_PBSP_Done
      je    PbspWait50_Spr                                  ; loop until not equal

;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [Begin]
;
;   Note: This WA only support up to 2 sockets configuration
;   Setup U1 bus Mmcfg address of both Sbsp and Pbsp for ucode trap(vmsr0x62)
;
      mov   eax, SR_PBSP_CHECKIN_CSR_SPR
      call  ASM_PFX(ReadCpuCsr)                            ; EAX = current CSR data
      and   eax, 0xf8000000                                ; check if a0 stepping by read bit[30:27]
      cmp   eax, 0
      jnz   SkipWriteMsr62_Spr

      mov   eax, CSR_EMULATION_FLAG_UBOX_CFG_REG           ; check if run with simics
      call  ASM_PFX(ReadCpuCsr)                            ; eax = reg dword data
      test  eax, (BIT2+BIT1+BIT0)                          ; 0 = real hardware
      jnz   SkipWriteMsr62_Spr

      ;
      ; Save ecx because ecx[11:0] holds the upper 3 nibbles of S0 U1 mmcfg address
      ;
      push  esi                                            ; Save esi on stack
      mov   esi, ecx                                       ; Save ecx in esi

      ;
      ; Read MSR(8Bh) to verify microcode version
      ;
      mov   ecx, MSR_IA32_BIOS_SIGN_ID
      xor   eax, eax                                       ; Clear EAX
      xor   edx, edx                                       ; Clear EDX
      wrmsr                                                ; Write 0 to MSR 8Bh
      mov   eax, 1
      cpuid                                                ; this will update MSR 8Bh
      mov   ecx, MSR_IA32_BIOS_SIGN_ID
      rdmsr                                                ; edx = current microcode signature/revision

      ;
      ; Restore ecx, then ecx[11:0] still holds the upper 3 nibbles of S0 U1 mmcfg address
      ;
      mov   ecx, esi                                       ; restore ecx data from esi
      pop   esi                                            ; restore esi from stack

      ;
      ; Check the Microcode Revision to see if it support the trap for U1 Bus Mmcfg address
      ;
      cmp   dl, 0x13                                       ; edx = current microcode signature/revision
      jb    SkipWriteMsr62_Spr                             ; Jump if the Ucode Trap is not supported by this MicroCode Revision

      ;
      ; Get S1 U1 MMCFG address and save in ebx[27:16]
      ;
      movd  edx, xmm4
      and   edx, 0xff00                                    ; edx[15:8] = bus1 field
      shl   edx, 8                                         ; edx[23:16] = new bus# for local target 1 (Bus1, UBOX1)
      movd  ebx, xmm3                                      ; MmcfgBase of Pbsp: read from ScratchPad15 register
      and   ebx, 0xF0000000                                ; ebx[31:28] = MmcfgBase of Pbsp
      shr   ebx, 4                                         ; ebx[27:24] = MmcfgBase of Pbsp
      or    ebx, edx                                       ; edx[27:16] = the upper 3 nibbles of S1 U1 mmcfg address

      ;
      ; edx[27:16] = the upper 3 nibbles of S1 U1 mmcfg address
      ; ecx[11:0]  = the upper 3 nibbles of S0 U1 mmcfg address
      ;
      or    ebx, ecx
      mov   eax, ebx                                       ; Put the data in eax, and program it to MSR(0x62)
      mov   ecx, MSR_BDF_PLA_VMSR_62H
      wrmsr

SkipWriteMsr62_Spr:
;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [End]
;

      popad
      ret

;-------------------------------------------------------------------------
; UncoreBusCfgSyncNbsp_Gnr
;   A PBSP waits for and hand-shakes with SBSP to receive re-programming of
;   MMCFG/CPUBUSNO related registers
;
; Input:
;     Stack is available
;     Following xmm registers hold MMCFG and bus number config info
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
; Output:
;    PBSP has switched to the new MMCFG/CPUBUSNO configuration with global view of the MMCFG config space
;
; Registers changed: None
;-------------------------------------------------------------------------


global ASM_PFX(UncoreBusCfgSyncNbsp_Gnr)
ASM_PFX(UncoreBusCfgSyncNbsp_Gnr):

        pushad

;
;  At this point, all CSR accesses are local to the socket, and default bus# 0...5 are used for local targets
;

;
;  Disable CHA MMCFG_RULE
;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_GNR             ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      btr   ebx, 0                                         ; clear Enable bit
      call  ASM_PFX(WritePciDwordLegacy)                   ; write to reg
      movd  xmm3, ebx                                      ; update copy in xmm3
;
;  From this point, only CF8/CFC IO can be used for CSR access
;
;
;  Set flag in local scratchpad CSR to indicate to SBSP "I'm ready for MMCFG/BUSNO re-programming"
;
      mov   eax, SR_LEGACY_BUSCFG_SYNC_CSR_GNR              ; eax = reg address
      mov   ebx, BusCfgSync_PBSP_Ready                      ; ebx = reg dword data
      call  ASM_PFX(WritePciDwordLegacy)                    ; write to reg

PbspWait10_Gnr:
;
;  Loop here on local scratchpad CSR
;  waiting for local MMCFG/CPUBUSNO etc.  CSRs be updated by SBSP,
;  until notified by SBSP to enable new configuration
;
      mov   eax, SR_LEGACY_BUSCFG_SYNC_CSR_GNR             ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      cmp   ebx, BusCfgSync_PBSP_Go
      jne   PbspWait10_Gnr
;
;  At this point, some local CSRs have been updated by SBSP with new settings
;  Update local copy of the new CHA MMCFG_RULE, UBOX CPUBUSNO (in xmm registers)
;
      mov   eax, CSR_LEGACY_MMCFG_TARGET_LIST_CBO_GNR      ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      movd  xmm1, ebx                                      ; update xmm reg with new data
;
; Get local copy of mmcfg base to support multisegments
;
      mov   eax, SR_LEGACY_BUSCFG_DATA_CSR_GNR
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      or    ebx, BIT0
      movd  xmm3, ebx                                      ; update xmm reg with new data
      mov   eax, CSR_LEGACY_CPUBUSNO_UBOX_GNR
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      movd  xmm4, ebx                                      ; update xmm reg with new data

;
;  Enable UBOX CPUBUSNO_VALID
;
      mov   eax, CSR_LEGACY_CPUBUSNO_VALID_UBOX_GNR        ; eax = reg address
      call  ASM_PFX(ReadPciDwordLegacy)                    ; ebx = reg dword data
      or    ebx, BIT31                                     ; set Valid bit = 1
      call  ASM_PFX(WritePciDwordLegacy)                   ; write to reg
;
;  From this point on, new CPUBUSNO must be used
;  Get new bus0/bus1 value
;
      movd  ecx, xmm4
      mov   edx, ecx

      and   ecx, 0xff                                      ; bit[7:0] = bus0 field
      shl   ecx, 16                                        ; ecx[23:16] = new bus# for local target 0 (Bus0, UBOX0)

      and   edx, 0xff00                                    ; bit[15:8] = bus1 field
      shl   edx, 8                                         ; edx[23:16] = new bus# for local target 1 (Bus1, UBOX1)

;
;  Enable  MMCFG_RULE in CHA, on Bus1
;
      mov   eax, CSR_LEGACY_MMCFG_RULE_CBO_GNR              ; eax = reg address
      and   eax, 0xff00ffff                                 ; clear bus# field
      or    eax, edx                                        ; update bus# field with new Bus1
      call  ASM_PFX(ReadPciDwordLegacy)                     ; ebx = reg dword data
      or    ebx, BIT0                                       ; set Enable bit = 1
      call  ASM_PFX(WritePciDwordLegacy)                    ; write to reg
;
; At this point, new CPUBUSNO and MMCFG_RULE are all enabled on the PBSP socket
;
;
;  Enable UBOX copy of MMCFG_RULE on Bus0
;

;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [Begin]
;
      ;
      ; Get S0 U1 MMCFG address and save in ecx[11:0]
      ;
      mov   eax, CSR_BIOSNONSTICKYSCRATCHPAD14_GNR          ; read the scratchpad14 to get S0 U1 MMCFG address
      call  ASM_PFX(ReadCpuCsr)                             ; eax = reg dword data
      mov   ecx, eax                                        ; ecx = S0 U1 mmcfg address
      shr   ecx, 20                                         ; ecx[11:0] = the upper 3 nibbles of S0 U1 mmcfg address
;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [End]
;

;
;  Send message back to SBSP to indicate "I'm done with enabling the new MMCFG/CPUBUS config"
;  via local scratchpad CSR in UBOX on Bus0
;
      mov   eax, SR_BUSCFG_SYNC_CSR_GNR                     ; eax = reg address
      mov   ebx, BusCfgSync_PBSP_Done                       ; ebx = reg dword data
      call  ASM_PFX(WriteCpuCsr)                            ; write to reg
;
;  Wait for SBSP to receive/process the message and clear the scartchpad CSR as acknoledgement
;
PbspWait50_Gnr:
      mov   eax, SR_BUSCFG_SYNC_CSR_GNR                     ; eax = reg address
      call  ASM_PFX(ReadCpuCsr)                             ; eax = reg data
      cmp   eax, BusCfgSync_PBSP_Done
      je    PbspWait50_Gnr                                  ; loop until not equal

;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [Begin]
;
;   Note: This WA only support up to 2 sockets configuration
;   Setup U1 bus Mmcfg address of both Sbsp and Pbsp for ucode trap(vmsr0x62)
;
      mov   eax, SR_PBSP_CHECKIN_CSR_GNR
      call  ASM_PFX(ReadCpuCsr)                            ; EAX = current CSR data
      and   eax, 0xf8000000                                ; check if a0 stepping by read bit[30:27]
      cmp   eax, 0
      jnz   SkipWriteMsr62_Gnr

      mov   eax, CSR_EMULATION_FLAG_UBOX_CFG_REG           ; check if run with simics
      call  ASM_PFX(ReadCpuCsr)                            ; eax = reg dword data
      test  eax, (BIT2+BIT1+BIT0)                          ; 0 = real hardware
      jnz   SkipWriteMsr62_Gnr

      ;
      ; Save ecx because ecx[11:0] holds the upper 3 nibbles of S0 U1 mmcfg address
      ;
      push  esi                                            ; Save esi on stack
      mov   esi, ecx                                       ; Save ecx in esi

      ;
      ; Read MSR(8Bh) to verify microcode version
      ;
      mov   ecx, MSR_IA32_BIOS_SIGN_ID
      xor   eax, eax                                       ; Clear EAX
      xor   edx, edx                                       ; Clear EDX
      wrmsr                                                ; Write 0 to MSR 8Bh
      mov   eax, 1
      cpuid                                                ; this will update MSR 8Bh
      mov   ecx, MSR_IA32_BIOS_SIGN_ID
      rdmsr                                                ; edx = current microcode signature/revision

      ;
      ; Restore ecx, then ecx[11:0] still holds the upper 3 nibbles of S0 U1 mmcfg address
      ;
      mov   ecx, esi                                       ; restore ecx data from esi
      pop   esi                                            ; restore esi from stack

      ;
      ; Check the Microcode Revision to see if it support the trap for U1 Bus Mmcfg address
      ;
      cmp   dl, 0x13                                       ; edx = current microcode signature/revision
      jb    SkipWriteMsr62_Gnr                             ; Jump if the Ucode Trap is not supported by this MicroCode Revision

      ;
      ; Get S1 U1 MMCFG address and save in ebx[27:16]
      ;
      movd  edx, xmm4
      and   edx, 0xff00                                    ; edx[15:8] = bus1 field
      shl   edx, 8                                         ; edx[23:16] = new bus# for local target 1 (Bus1, UBOX1)
      movd  ebx, xmm3                                      ; MmcfgBase of Pbsp: read from ScratchPad15 register
      and   ebx, 0xF0000000                                ; ebx[31:28] = MmcfgBase of Pbsp
      shr   ebx, 4                                         ; ebx[27:24] = MmcfgBase of Pbsp
      or    ebx, edx                                       ; edx[27:16] = the upper 3 nibbles of S1 U1 mmcfg address

      ;
      ; edx[27:16] = the upper 3 nibbles of S1 U1 mmcfg address
      ; ecx[11:0]  = the upper 3 nibbles of S0 U1 mmcfg address
      ;
      or    ebx, ecx
      mov   eax, ebx                                       ; Put the data in eax, and program it to MSR(0x62)
      mov   ecx, MSR_BDF_PLA_VMSR_62H
      wrmsr

SkipWriteMsr62_Gnr:
;
; WA for SPR Ax stepping CHA BDF PLA issue (S1507951535) [End]
;

      popad
      ret

;----------------------------------------------------------------------------
; EnableAccessGlobalCSR
;  Executed by SBSP (Legacy socket) only
;  Input:  Stack available
;             Local MMCFG config CSR  access is enabled
;  Output:
;             MMCFG_TARGET_LIST updated
;             CPUBUSNO enabled
;             Inter-socket CSR accesses enabled
;
;  All general puropse registers preserved.
;----------------------------------------------------------------------------
global ASM_PFX(EnableAccessGlobalCSR)
ASM_PFX(EnableAccessGlobalCSR):

       pushad
       IS_SPR_CPU_FROM_XMM0
       jne EnableAccessGlobalCSRMark1a
       CALL_MMX3 ASM_PFX(EnableAccessGlobalCSR_NoStack_Spr)
       jmp EnableAccessGlobalCSRMark1b
EnableAccessGlobalCSRMark1a:
       CALL_MMX3 ASM_PFX(EnableAccessGlobalCSR_NoStack_Gnr)
EnableAccessGlobalCSRMark1b:
       popad
       ret

;-------------------------------------------------------------------------
; Non-SBSPs execute this routine to clear BSP flag in MSR and
; loop on a local scratch pad register for command from SBSP
; Input: None
;
; Output: None
;-------------------------------------------------------------------------


global ASM_PFX(ParkNbsp)
ASM_PFX(ParkNbsp):

     ; clear bit8 BSP flag in MSR
     mov     ecx, MSR_IA32_APIC_BASE
     rdmsr
     and     eax, ~ BIT8                                 ; clear bit8 BSP flag in MSR
     wrmsr

     call  ASM_PFX(GetLocalSktId)                                   ; CL = Local Socketr ID

   ; When SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR becomes non-zero,
   ;       SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR will be the jmp location

SitAndSpin_Spr:
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     call  ASM_PFX(ReadCpuCsr)

     cmp   eax, PIPE_DISPATCH_COMMAND_MULTI_SKT_UPD_GO_HLT
     jne   Check_Kti_Routine

     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                          ; Clear the dispactch CSR
     call  ASM_PFX(WriteCpuCsr)
     ; tell sbsp, pbsp already go to "hlt"
     call  ASM_PFX(NotifyPbspInHaltToSbsp)
     mov   eax, 0
     jmp   GoOnSpin

Check_Kti_Routine:
     cmp   eax, PIPE_DISPATCH_COMMAND_KTI_ROUTINE
     jne   Check_Kti_Adaptation

     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                          ; Clear the dispactch CSR
     call  ASM_PFX(WriteCpuCsr)

     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR     ; Pipe Data register in local socket
     call  ASM_PFX(ReadCpuCsr)
     cmp   eax, 0xffffffff
     jz    SitAndSpin_Spr

     call  ASM_PFX(ApSpeedTranstion)                         ; Call KTI Speed Transition Routine for AP
     mov   eax, 0                                            ; continue the loop
     jmp   GoOnSpin

Check_Kti_Adaptation:
     cmp   eax, PIPE_DISPATCH_COMMAND_KTI_ADAPTATION
     jne   GoOnSpin

     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                          ; Clear the dispactch CSR
     call  ASM_PFX(WriteCpuCsr)

     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR     ; Pipe Data register in local socket
     call  ASM_PFX(ReadCpuCsr)
     cmp   eax, 0xffffffff
     jz    SitAndSpin_Spr

     call  ASM_PFX(ApAdaptation)                             ; Call KTI txeq adaptation Routine for AP
     mov   eax, 0                                            ; continue the previous loop

 GoOnSpin:
     cmp   eax, 0
     jz    SitAndSpin_Spr

     mov   eax, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_SPR ; Pipe Command register in local socket
     call  ASM_PFX(ReadCpuCsr)

     cmp   eax, PIPE_DISPATCH_SYNCH_PSYSHOST
     jz    GoToAction_Spr

     ; clear out dispatch register because command is invalid
     ;mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     ;xor   ebx, ebx                                          ; Clear the dispactch CSR
     ;call  ASM_PFX(WriteCpuCsr)
     jmp   SitAndSpin_Spr

GoToAction_Spr:

     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR     ; Pipe Data register in local socket
     call  ASM_PFX(ReadCpuCsr)
     mov   esp, eax                                          ; sync up stack pointer with SBSP

     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                          ; Clear the dispactch CSR
     call  ASM_PFX(WriteCpuCsr)

WaitForJmpPointer_Spr:
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     call  ASM_PFX(ReadCpuCsr)

     cmp   eax, 0
     jz    SHORT WaitForJmpPointer_Spr

     mov  eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR
     call ASM_PFX(GetBusNum)                               ; update EAX[27:20] with the correct bus #
     call ASM_PFX(GetMmcfgBase)                            ; esi = MMCFG Base address
     add  esi, eax                                         ; form cfg register address in esi
     and  esi, 0xfffff000                                  ; keep only Bus:Dev:Func portion of it

     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR   ; Pipe Data register in local socket
     call  ASM_PFX(ReadCpuCsr)

%ifdef COMPRESS_RC

     ; ESI [31:12] is the B:D:F:00 portion of the PCIE address of the local scratch register
     ; CL = NodeID/SocketID
     push esi
     push ecx
     push eax

     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                          ; Clear the dispactch CSR
     call  ASM_PFX(WriteCpuCsr)

DecompDestWait_Spr:
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     call  ASM_PFX(ReadCpuCsr)

     cmp   eax, 0
     jz    SHORT DecompDestWait_Spr

     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR     ; Pipe Data register in local socket
     call  ASM_PFX(ReadCpuCsr)
     push  eax                                               ; Put Decompress Destination Buffer Address on Stack

     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                          ; Clear the dispactch CSR
     call  ASM_PFX(WriteCpuCsr)

DecompSrcWait_Spr:
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     call  ASM_PFX(ReadCpuCsr)

     cmp   eax, 0
     jz    SHORT DecompSrcWait_Spr

     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR     ; Pipe Data register in local socket
     call  ASM_PFX(ReadCpuCsr)
     push  eax                                               ; Put Decompress Source Buffer Address on Stack

     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     xor   ebx, ebx                                          ; Clear the dispactch CSR
     call  ASM_PFX(WriteCpuCsr)

;            Wait for SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR == 0
;            Make SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR = decompress function address
;            Make SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR = 1 (non-zero)
DecompFuncWait_Spr:
     mov   eax, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR ; Pipe Dispatch register in local socket
     call  ASM_PFX(ReadCpuCsr)

     cmp   eax, 0
     jz    SHORT DecompFuncWait_Spr

     mov   eax, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR     ; Pipe Data register in local socket
     call  ASM_PFX(ReadCpuCsr)

     cmp   eax, 0
     jz    SHORT NoCompressionSupported_Spr

     call  eax                                             ; Call Decompression Routine

NoCompressionSupported_Spr:
     add   esp, 8                                          ; Balance stack

     pop eax
     pop ecx
     pop esi
%endif
   ; EAX = Function pointer to PipeSlaveInit( UINT32 NodeId, UINT32 CfgAddrBDF)
   ; Emulate a C-style function call
     push   esi                                             ; push input arg (PCIE Config Address w/o register offset)
     push   ecx                                             ; push input arg (local socketId)
     call   eax                                             ; call PipeSlaveInit( UINT32 NodeId, UINT32 CfgAddrBDF)
                                                            ; Note: Control will not return to here


;-------------------------------------------------------------------------
; Check if warm reset path.  Can be called by each socket BSP
; Input:
;   Stack is NOT available
;   MMX6 = return address via RET_ESI_MMX6 macro
;   Register xmm0[31] has been initialized to indicate warm reset
;          xmm0  [31] = 0   Power Good reset path
;                               1   Warm reset path
;
; Output:
;              ZF = 1      Cold reset path
;              ZF = 0      Warm reset path
;
; Registers changed:  ESI
;-------------------------------------------------------------------------

global ASM_PFX(IsWarmResetNoStack)
ASM_PFX(IsWarmResetNoStack):

      movd  esi, xmm0
      test  esi, BIT31
     ;  ZF = 1      Cold reset path
     ;  ZF = 0      Warm reset path

      RET_ESI_MMX6

;-------------------------------------------------------------------------
; Check if I am legacy socket. Can be called by each socket BSP
; Input:
;   Stack is NOT available
;   MMX6 = return address via RET_ESI_MMX6 macro
;   xmm0 registers initialized and contains socket id info:
;          xmm0  [3:0]= Local socket id,
;                     [7:4]= Legacy socket id
;                     [30] = Legacy Socket flag
;
; Output:  ZF = 1       Yes, I am legacy socket
;              ZF = 0      No
;
; Registers changed: esi
;-------------------------------------------------------------------------

global ASM_PFX(AmILegacySocketNoStack)
ASM_PFX(AmILegacySocketNoStack):

      movd  esi, xmm0
      and   esi, BIT30              ;  [30] = Legacy Socket flag
      xor   esi, BIT30              ;  am I legacy socket?    ZF = 1/0  -> Yes/No

      RET_ESI_MMX6

;-------------------------------------------------------------------------
; Callable version of  AmILegacySocketNoStack()
; Check if I am legacy socket. Can be called by all sockets
; Input:
;              Stack is available
; Output:
;              ZF = 1       Yes, I am legacy socket
;              ZF = 0       No
;
; Registers changed: none
;-------------------------------------------------------------------------

global ASM_PFX(AmILegacySocket)
ASM_PFX(AmILegacySocket):

      push  esi
      CALL_MMX6  ASM_PFX(AmILegacySocketNoStack)
      pop   esi
      ret

;-------------------------------------------------------------------------
; Get MMCFG.BASE
; Input:
;     Stack is NOT available
;     MMX6 = return address via RET_ESI_MMX6 macro
;     CL = Local target BusID (0,1,...5)
;     Following xmm registers hold MMCFG and bus number config info
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [27:8] Cpu Family/Stepping info (from cpuid eax, bit[19:0])
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
; Output: EDI = MMCFG.base
;         The caller can then use
;            mov ebx, CSR_DEV_FUNC_OFFSET
;            mov eax, dword ptr [edi] [ebx]       where ebx = bus/dev/func/offset in PCIe format
;
; Registers changed: edi, esi
;-------------------------------------------------------------------------

global ASM_PFX(GetMmcfgNoStack)
ASM_PFX(GetMmcfgNoStack):

      movd  edi, xmm3
      test  edi, BIT0                    ; MMCFG.enable already set?
      jnz   gmc_10                       ; yes

gmc_error:
      PORT80(MMCFG_NOT_YET_ENABLED_ERROR_CODE) ; error code to port 80h
      jmp  gmc_error                     ; dead loop

gmc_10:
      and   edi, MMCFG_RULE_ADDR_MASK    ; edi = MMCFG.base

      RET_ESI_MMX6

;-------------------------------------------------------------------------
; Get MMCFG.BASE + Bus# field
; Input:
;   Stack is NOT available
;   MMX6 = return address via RET_ESI_MMX6 macro
;   CL = Local target BusID (0,1,...5)
;     Following xmm registers hold MMCFG and bus number config info
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [27:8] Cpu Family/Stepping info (from cpuid eax, bit[19:0])
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
; Output: EDI = MMCFG.base + bus offset
;         The caller can then use
;            mov ebx, CSR_DEV_FUNC_OFFSET
;            mov eax, dword ptr [edi] [ebx]
;
; Registers changed: cl, edi, esi
;-------------------------------------------------------------------------

global ASM_PFX(GetMmcfgBus_NoStack)
ASM_PFX(GetMmcfgBus_NoStack):

      movd  edi, xmm3
      test  edi, BIT0                   ; MMCFG.enable already set?
      jnz   gmb_10                      ; yes

gmb_error:
      PORT80(MMCFG_NOT_YET_ENABLED_ERROR_CODE)  ; error code to port 80h
      jmp  gmb_error                    ; dead loop

gmb_10:
      movd  esi, xmm4                   ; esi = CPUBUSNO[3,2,1,0]
      cmp   cl, 4
      jb    gmb_20
      movd  esi, xmm5                   ; esi = CPUBUSNO[_,_, 5,4]
      sub   cl, 4                       ; adjust BusID

gmb_20:
      shl   cl, 2                       ; cl = cl * 4
      shr   esi, cl                     ; esi[7:0] = target bus#
      and   esi, 0xff
      shl   esi, 20                     ; esi[27:20] = target bus#

      and   edi, MMCFG_RULE_ADDR_MASK   ; edi = MMCFG.base
      add   edi, esi                    ; edi = MMCFG.base + bus offset

      RET_ESI_MMX6

;-------------------------------------------------------------------------
; Read a Dword from PCIe config register using mmcfg method
; Limitation:  Currently does not support config space size > 256 MB ( 256 buses)
; Input:
;   EAX = PCIe config address format with format defined as follows
;     EAX[31:28] = 0h
;     EAX[27:20] = Bus# of local target (Bus0,Bus1,...Bus5)
;     EAX[19:15] = Dev #
;     EAX[14:12] = Func #
;     EAX[11:0]  = Reg offset (dword aligned)
;     Following xmm registers hold MMCFG and bus number config info
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [27:8] Cpu Family/Stepping info (from cpuid eax, bit[19:0])
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
; Output: EAX = regiser dword data
; Registers changed: None except EAX
;-------------------------------------------------------------------------

global ASM_PFX(ReadCpuCsr)
ASM_PFX(ReadCpuCsr):

      push  esi
      call  ASM_PFX(GetBusNum)                           ; update EAX[27:20] with the correct bus# for the socket ID
      call  ASM_PFX(GetMmcfgBase)                        ; esi = MMCFG Base Address
      add   esi, eax                                     ; form cfg register full address in esi
      mov   eax, dword [esi]                             ; read dword
      pop   esi
      ret

;-------------------------------------------------------------------------
; Write a Dword from PCIe config register using mmcfg method
; Limitation:  Currently does not support config space size > 256 MB ( 256 buses)
;
; Input:
;   Stack is available
;   EAX = PCIe config address format with format defined as follows
;     EAX[31:28] = 0h
;     EAX[27:20] = Bus# of local target (Bus0,Bus1,...Bus5)
;     EAX[19:15] = Dev #
;     EAX[14:12] = Func #
;     EAX[11:0]  = Reg offset (dword aligned)
;     EBX  = Dword data to be written to reg
;     Following xmm registers hold MMCFG and bus number config info
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [27:8] Cpu Family/Stepping info (from cpuid eax, bit[19:0])
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
; Output: None
; Registers changed: None
;-------------------------------------------------------------------------

global ASM_PFX(WriteCpuCsr)
ASM_PFX(WriteCpuCsr):

      push  eax
      push  esi
      call  ASM_PFX(GetBusNum)                           ; update EAX[27:20] with the correct bus# for the socket ID
      call  ASM_PFX(GetMmcfgBase)                        ; esi = MMCFG Base Address
      add   esi, eax                                     ; form cfg register full address in esi
      mov   dword [esi], ebx                             ; write dword
      pop   esi
      pop   eax
      ret

;-------------------------------------------------------------------------
;  Update EAX[27:20] with the target Bus number from MSR 300h
; Input:
;   EAX = PCIe config address format with format defined as follows
;     EAX[31:28] = 0h
;     EAX[27:20] = Bus# of local target (Bus0,Bus1,...Bus5)
;     EAX[19:15] = Dev #
;     EAX[14:12] = Func #
;     EAX[11:0]  = Reg offset (dword aligned)
;     Following xmm registers hold MMCFG and bus number config info
;                xmm0 = [3:0]  local socket id
;                             [7:4]  legact socket id
;                             [27:8] Cpu Family/Stepping info (from cpuid eax, bit[19:0])
;                             [31] = Warm Reset Path flag
;                xmm1 = MCFG_TARGET_LIST CSR current data
;                xmm3 = MMCFG_RULE CSR current data
;                xmm4 = CPUSUBNO [Bus0,Bus1,Bus2,Bus3]
;                xmm5 = CPUBUSNO [Bus4,Bus5]
;
; Output: EAX[27:20] updated with the actual bus# intended
; Registers changed: None except EAX[27:20]
;-------------------------------------------------------------------------
global ASM_PFX(GetBusNum)
ASM_PFX(GetBusNum):

      push  ebx
      push  ecx
      push  edx
      push  esi
      push  edi

      push  eax                       ; save input eax
      push  ecx                       ; save input ecx

      mov   ecx, MSR_CPU_BUS_NO_10NM
      rdmsr                           ; edx:eax  = current CPUBUSNO/CPUBUSNO1
      test  edx, BIT31                ; Bus number valid?
      jnz   short gbn10
      mov   eax, DEFAULT_COLDBOOT_CPUBUSNO
      xor   edx, edx

 gbn10:
      mov   esi, eax
      mov   edi, edx
      pop   ecx                       ; restore input ecx
      pop   eax                       ; restore input eax

; Bus# byte-array
;              edi                                            esi
; ---------------------    --------------------------
; [Rsvd, Rsvd, Bus5, Bus4]       [Bus3, Bus2, Bus1, Bus0]

      ; get SPR Bus# field of PCIe address in EAX
      mov   ecx, eax
      shr   ecx, 20
      and   ecx, 0xff                    ;  [7:0] = bus#

      cmp   ecx, BUS_U0                  ; BusID >= BusU0?
      jb    short gbn21_icx
      sub   ecx, BUS_U0                  ; adjust BusID to use bus U0 or U1 from msr 0x300
      jmp   short gbn23_icx
 gbn21_icx:
      cmp   ecx, 2                       ; BusID >= 2, using IIO bus from msr 0x300, neglect bus 4-5 which not used in ICX
      jae   short gbn23_icx

      and   esi, 0xffff0000              ; mask off bus 0/1 which is uncore bus, replace with IIO bus 0/1
      or    esi, ((DEFAULT_BUS_NO1 << 8) +  DEFAULT_BUS_NO0)
 gbn23_icx:                               ; here eax has original CSR address, esi has bus number to be used, cl has bus index of esi
      jmp   short gbn20_icx


; esi =  proper Bus# byte-array
; ecx = index into Bus# byte-array (value = 0,1,2 or 3)

; shift proper bus#  into esi[27:20]
 gbn20_icx:
      shl   ecx, 3                       ; ecx = ecx * 8,  bit position of bus# field
      shr   esi, cl                      ; esi[7:0] = target bus#
      and   esi, 0xff                    ; esi[7:0] = bus#
      shl   esi, 20                      ; esi[27:20] = bus#

      and   eax, 0xfffff               ; keep only Dev:Func:Offset
      or    eax, esi                     ; eax = Bus:Dev:Func:Offset

      pop   edi
      pop   esi
      pop   edx
      pop   ecx
      pop   ebx
      ret

;-------------------------------------------------------------------------
; Get MMCFG base address
;
;  Input:
;   Stack is avaialble
;   xmm3 register has been initialized and contains MMCFG_RULE
;
;  Output: ESI = 32-bit MMCFG base address
;  Registers changed:  none except ESI
;-------------------------------------------------------------------------
global ASM_PFX(GetMmcfgBase)
ASM_PFX(GetMmcfgBase):

      ; PCIe config base address actually written into the MMCFG_RULE CSR
      movd  esi, xmm3                                       ; xmm3 stores MMCFG_RULE
      and   esi, MMCFG_RULE_ADDR_MASK                       ; make sure the lower bits are 0s
      ret

;------------------------------------------------------------------------------
; Read a PCI config dword via lagacy CF8/CFC method
; Stack is available
; input: EAX = legacy PCI address format with Bus:Dev:Func:Offset
; output: EBX = dword register data
;
; Registers changed:  EBX
;------------------------------------------------------------------------------
global ASM_PFX(ReadPciDwordLegacy)
ASM_PFX(ReadPciDwordLegacy):

   push  eax
   push  edx
   or    eax, BIT31                  ; make sure bit31 is set
   READ_PCI_DWORD_LEGACY
   pop   edx
   pop   eax
   ret

;------------------------------------------------------------------------------
; Write a DWord data to PCI config reg via lagacy CF8/CFC method
; Stack is available
; input: EAX = legacy PCI address format with Bus:Dev:Func:Offset
;          EBX = dword data to write
;
; Registers changed: None
;------------------------------------------------------------------------------
global ASM_PFX(WritePciDwordLegacy)
ASM_PFX(WritePciDwordLegacy):

   push  eax
   push  edx
   or    eax, BIT31                  ; make sure bit31 is set
   WRITE_PCI_DWORD_LEGACY
   pop   edx
   pop   eax
   ret

;------------------------------------------------------------------------------
; Check Pcode status, if it is Run_Busy or free
; input: None
;
; Output:
;              ZF = 1  RunBusy or error
;              ZF = 0  Pcode Done
; Registers changed: None
;------------------------------------------------------------------------------
global ASM_PFX(CheckPcodeRunBusy)
ASM_PFX(CheckPcodeRunBusy):

   push  ecx
   push  eax
   mov   ecx, DELAY_COUNT
wait_pcode_free:
   mov   eax, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_SPR
   call  ASM_PFX(ReadCpuCsr)
   test  eax, RUN_BUSY_BIT
   loopnz wait_pcode_free
   cmp   ecx, 0

pcode_run_busy_exit:
   pop   eax
   pop   ecx
   ret

;------------------------------------------------------------------------------
; Read the UPI Agent's CSR
; input: eax --- address
;        cl  --- UPI link index
;
; Output: eax --- CSR value
; Registers changed: eax
;------------------------------------------------------------------------------
global ASM_PFX(ReadUpiAgentCsr)
ASM_PFX(ReadUpiAgentCsr):

   push  edx
   xor   edx, edx
   mov   dl, cl
   shl   edx, 15
   add   eax, edx                   ; move to the corresponding UPI device according to the link index
   call  ASM_PFX(ReadCpuCsr)
   pop   edx
   ret

;------------------------------------------------------------------------------
; Write the UPI Agent's CSR
; input: eax --- address
;        cl  --- UPI link index
;        ebx --- CSR value
;
; Output: none
; Registers changed: eax
;------------------------------------------------------------------------------
global ASM_PFX(WriteUpiAgentCsr)
ASM_PFX(WriteUpiAgentCsr):

   push  edx
   xor   edx, edx
   mov   dl, cl
   shl   edx, 15
   add   eax, edx                  ; move to the corresponding UPI device according to the link index
   call  ASM_PFX(WriteCpuCsr)
   pop   edx
   ret

;------------------------------------------------------------------------------
; Check if phy layer reset is needed
; input: eax --- Input total upi ratio which is used to indicate the existed link
;
; Output: None
; Registers changed: None
;------------------------------------------------------------------------------
global ASM_PFX(TriggerCpReset)
ASM_PFX(TriggerCpReset):

   push  eax
   push  ecx

   mov   esi, eax         ; save the Upi Ratio
   xor   ecx, ecx
trigger_cp_reset_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff         ; check the link's upi ratio is valid
   jz    trigger_cp_reset_move_to_next

   ;
   ; If the link's upi ratio is valid, do the programming below
   ; Check the link's peer socket id, if the peer socket id > Local socket id, do not file cp reset
   ;
   mov   eax, CSR_KTILP0_KTI_LLPMON_REG_SPR
   mov   ecx, edi                            ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   shr   eax, BASE_NODEID_KTILP0_OFFSET
   and   ax, 0xf                             ; get the base_nodeid of the link
   call  ASM_PFX(GetLocalSktId)
   cmp   al, cl                              ; compare the local socket id with the link's peer socket id
   jna   trigger_cp_reset_move_to_next

   ;
   ; primary port, set the cp_reset
   ;
   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   mov   ecx, edi                            ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   mov   ebx, eax
   or    ebx, CP_RESET_BIT                   ; if local socket id < peer socket id, file the cp_reset
   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   call  ASM_PFX(WriteUpiAgentCsr)

trigger_cp_reset_move_to_next:
   mov   ecx, edi                            ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET        ; reach the maximum supported UPI link?
   jnz   trigger_cp_reset_check_next_link

trigger_cp_reset_exit:
   pop   ecx
   pop   eax
   ret

;------------------------------------------------------------------------------
; Speed transition flow which need to be run in each AP
; input: EAX = The AP's UPI links' target upi ratio
;
; Registers changed: None
;------------------------------------------------------------------------------
global ASM_PFX(ApSpeedTranstion)
ASM_PFX(ApSpeedTranstion):

   push  eax
   push  ebx
   push  ecx
   push  edx
   push  esi
   push  edi

   //
   // Step 1: Trigger the cp_reset if needed
   // The link's peer socket id > its own socket id, clear the c_init_begin and trigger the cp_reset
   //
   call  ASM_PFX(TriggerCpReset)

   mov   esi, eax         ; save the Upi Ratio
   xor   ecx, ecx
check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff         ; check the link's upi ratio is valid
   jz    move_to_next

   ;
   ; check if the port reaches ResetC, then do the following programming
   ;
   mov   ecx, DELAY_COUNT
poll_resetc:
   push  ecx
   mov   ecx, edi                            ; restore the current link number
   mov   eax, CSR_KTIREUT_PH_CSS_SPR
   call  ASM_PFX(ReadUpiAgentCsr)
   pop   ecx
   cmp   al, 2                               ; check s_tx_state is ResetC (2).
   loopnz  poll_resetc
   or    ecx, ecx
   jz    move_to_next

   ;
   ; Step 2: Trigger link layer reset on the link
   ;
   mov   eax, CSR_KTILCL_KTI_LLPMON_REG_SPR
   mov   ecx, edi                  ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   mov   ebx, eax
   or    ebx, LINK_LAYER_RESET_BIT
   mov   eax, CSR_KTILCL_KTI_LLPMON_REG_SPR
   call  ASM_PFX(WriteUpiAgentCsr)

   mov   ecx, DELAY_COUNT
poll_llr:
   push  ecx
   mov   ecx, edi                            ; restore the current link number
   mov   eax, CSR_KTILCL_KTI_LLPMON_REG_SPR
   call  ASM_PFX(ReadUpiAgentCsr)
   pop   ecx
   test  al, LINK_LAYER_RESET_BIT            ; check link_layer_reset bit is 0
   loopnz  poll_llr
   or    ecx, ecx
   jz    move_to_next

   ;
   ; Step 3: Poll the cri_pclkchangeok_flag in PIPE_CLK_RATE_CTRL_KTI_REUT_REG
   ;
   mov   ecx, DELAY_COUNT
poll_pclk_change:
   push  ecx
   mov   ecx, edi                            ; restore the current link number
   mov   eax, CSR_PIPE_CLK_RATE_CTRL_SPR
   call  ASM_PFX(ReadUpiAgentCsr)
   pop   ecx
   test  eax, CRI_PCLKCHANGOK_FLAG_BIT       ; check if the UPI subsystem pclk is changed
   loopz  poll_pclk_change
   or    ecx, ecx
   jz    move_to_next

   ;
   ; Step 4: send B2P command to set the target upi ratio and mode
   ;
   call  ASM_PFX(CheckPcodeRunBusy)
   jz    move_to_next
   ;
   ; Send the upi ratio to B2P data
   ;
   mov   ebx, esi
   mov   ecx, edi                             ; restore the current link number
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   and   ebx, 0x000000ff
   mov   eax, CSR_BIOS_MAILBOX_DATA_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)
   ;
   ; Send the speed switching command to B2P commnad
   ;
   push  ebx
   mov   ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ                 ; set the comand of speed switch
   mov   ecx, edi                                           ; restore the current link number
   xor   eax, eax
   mov   al, cl
   shl   ax, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB          ; set the Link Instance
   or    ebx, eax
   pop   eax
   cmp   al, 0                                                                     ; check if it requires to switch to slow speed
   jz    s4_slow_mode
   or    ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_FAST_MODE_BIT                ; set the fast mode bit
s4_slow_mode:
   or    ebx, RUN_BUSY_BIT                                                         ; set Run_Busy Bit
   mov   eax, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)

   call  ASM_PFX(CheckPcodeRunBusy)
   jz    move_to_next

move_to_next:
   mov   ecx, edi                            ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET        ; reach the maximum supported UPI link?
   jnz   check_next_link

   ;
   ; Step 5: Send Trigger bit, after all links programming are finished
   ;
   xor   ecx, ecx
s5_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff
   jz    s5_move_to_next

   call  ASM_PFX(CheckPcodeRunBusy)
   jz   speed_transition_exit
   ;
   ; Be ready to send the b2p Command
   ; Send the upi ratio to B2P data
   ;
   and   ebx, 0x000000ff
   mov   eax, CSR_BIOS_MAILBOX_DATA_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)

   push  ebx                                                                       ; save the target upi ratio
   mov   ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ                                        ; set the comand of speed switch
   mov   ecx, edi                                                                  ; restore the current link number
   xor   eax, eax
   mov   al, cl
   shl   ax, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB          ; set the Link Instance
   or    ebx, eax
   pop   eax                                                                       ; restore the target upi ratio
   cmp   al, 0                                                                     ; check if it requires to switch to slow speed
   jz    s5_slow_mode
   or    ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_FAST_MODE_BIT                ; set the FAST MODE bit
s5_slow_mode:
   or    ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_TRIGGER_BIT                  ; set the trigger bit
   or    ebx, RUN_BUSY_BIT                                                         ; set Run_Busy Bit
   mov   eax, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)

   call  ASM_PFX(CheckPcodeRunBusy)
   jz   speed_transition_exit

   jmp  s6_start

s5_move_to_next:
   mov   ecx, edi         ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET
   jnz   s5_check_next_link

   ;
   ; Step 6: Clear the cri_pclkchangeok_flag, set CInitBegin = 1 to start training on every valid link
   ;         (no matter primary or secondary port)
   ;
s6_start:
   xor   ecx, ecx
s6_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff
   jz    s6_move_to_next

   mov   eax, CSR_PIPE_CLK_RATE_CTRL_SPR
   mov   ecx, edi                  ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   mov   ebx, eax
   or    ebx, CRI_PCLKCHANGOK_FLAG_BIT                ; Clear the cri_pclkchangeok_flag by writing 1
   mov   eax, CSR_PIPE_CLK_RATE_CTRL_SPR
   call  ASM_PFX(WriteUpiAgentCsr)

   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   mov   ecx, edi                  ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   mov   ebx, eax
   or    ebx, C_INIT_BEGIN_BIT
   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   call  ASM_PFX(WriteUpiAgentCsr)

s6_move_to_next:
   mov   ecx, edi         ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET
   jnz   s6_check_next_link

speed_transition_exit:
   pop   edi
   pop   esi
   pop   edx
   pop   ecx
   pop   ebx
   pop   eax
   ret

;------------------------------------------------------------------------------
; MDFS traing step, PBSP notify SBSP before it enters halt state
; input: None
;
; Registers changed: None
;------------------------------------------------------------------------------
global ASM_PFX(NotifyPbspInHaltToSbsp)
ASM_PFX(NotifyPbspInHaltToSbsp):
   push  eax
   push  ebx

   mov    eax, CSR_BIOSNONSTICKYSCRATCHPAD2_SPR ; write sratchpad to let sbsp know pbsp already entered halt state
   call   ASM_PFX(ReadCpuCsr)                   ; EAX = current CSR data
   mov    ebx, eax
   or     ebx, BIT1
   mov    eax, CSR_BIOSNONSTICKYSCRATCHPAD2_SPR
   call   ASM_PFX(WriteCpuCsr)
   hlt                                          ; enter halt state, resume with the help of pcode
   btr    ebx, 1                                ; after resume, clear the halt state bit in SP2
   call   ASM_PFX(WriteCpuCsr)
   pop   ebx
   pop   eax
   ret

;------------------------------------------------------------------------------
; Txeq adaptation flow which need to be run in each AP
; input: EAX = The AP's UPI links' target upi ratio
;        EBX = The step of the adaptation flow, 0: loopback; 1: slow mode
;
; Registers changed: None
;------------------------------------------------------------------------------
global ASM_PFX(ApAdaptationSteps)
ASM_PFX(ApAdaptationSteps):

   push  eax
   push  ebx
   push  ecx
   push  edx
   push  esi
   push  edi

   //
   // Step 1: Trigger the cp_reset if in Loopback step
   // The link's peer socket id > its own socket id, trigger the cp_reset
   //
   mov   esi, eax         ; save the Upi Ratio
   cmp   ebx, UPI_TXEQ_ADAPTATION_SLOWMODE_PHASE
   jz    skip_cp_reset_in_slowmode_step

   xor   ecx, ecx
adaptation_s1_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff         ; check the link's upi ratio is valid
   jz    adaptation_s1_move_to_next

   ;
   ; If the link's upi ratio is valid, do the programming below
   ; Check the link's peer socket id, if the peer socket id > Local socket id, do not file cp reset
   ;
   mov   eax, CSR_KTILP0_KTI_LLPMON_REG_SPR
   mov   ecx, edi                            ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   shr   eax, BASE_NODEID_KTILP0_OFFSET
   and   ax, 0xf                             ; get the base_nodeid of the link
   call  ASM_PFX(GetLocalSktId)
   cmp   al, cl                              ; compare the local socket id with the link's peer socket id
   jna   adaptation_s1_move_to_next

   ;
   ; primary port, set the cp_reset
   ;
   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   mov   ecx, edi                            ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   mov   ebx, eax
   or    ebx, CP_RESET_BIT                   ; if local socket id < peer socket id, file the cp_reset
   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   call  ASM_PFX(WriteUpiAgentCsr)

adaptation_s1_move_to_next:
   mov   ecx, edi                            ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET        ; reach the maximum supported UPI link?
   jnz   adaptation_s1_check_next_link

skip_cp_reset_in_slowmode_step:

   ;
   ; Step2: polling pclkchangeok and programming the upi ratio via B2P command
   ;
   xor   ecx, ecx
adaptation_s2_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff         ; check the link's upi ratio is valid
   jz    adaptation_s2_move_to_next

   mov   ecx, DELAY_COUNT
adaptation_s2_poll_pclk_change:
   push  ecx
   mov   ecx, edi                            ; restore the current link number
   mov   eax, CSR_PIPE_CLK_RATE_CTRL_SPR
   call  ASM_PFX(ReadUpiAgentCsr)
   pop   ecx
   test  eax, CRI_PCLKCHANGOK_FLAG_BIT       ; check if the UPI subsystem pclk is changed
   loopz  adaptation_s2_poll_pclk_change
   or    ecx, ecx
   jz    adaptation_s2_move_to_next

   call  ASM_PFX(CheckPcodeRunBusy)
   jz    adaptation_s2_move_to_next
   ;
   ; Send the upi ratio to B2P data
   ;
   mov   ebx, esi
   mov   ecx, edi                             ; restore the current link number
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   and   ebx, 0x000000ff
   mov   eax, CSR_BIOS_MAILBOX_DATA_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)
   ;
   ; Send the speed switching command to B2P commnad
   ;
   push  ebx
   mov   ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ                 ; set the comand of speed switch
   mov   ecx, edi                                           ; restore the current link number
   xor   eax, eax
   mov   al, cl
   shl   ax, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB          ; set the Link Instance
   or    ebx, eax
   pop   eax
   cmp   al, 0                                                                     ; check if it requires to switch to slow speed
   jz    adaptation_s2__slow_mode
   or    ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_FAST_MODE_BIT                ; set the fast mode bit
adaptation_s2__slow_mode:
   or    ebx, RUN_BUSY_BIT                                                         ; set Run_Busy Bit
   mov   eax, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)

   call  ASM_PFX(CheckPcodeRunBusy)
   jz    adaptation_s2_move_to_next

adaptation_s2_move_to_next:
   mov   ecx, edi                            ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET        ; reach the maximum supported UPI link?
   jnz   adaptation_s2_check_next_link

   ;
   ; Step 3: Send Trigger bit, after all links programming are finished
   ;
   xor   ecx, ecx
adaptation_s3_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff
   jz    adaptation_s3_move_to_next

   call  ASM_PFX(CheckPcodeRunBusy)
   jz   adaptation_steps_exit
   ;
   ; Be ready to send the b2p Command
   ; Send the upi ratio to B2P data
   ;
   and   ebx, 0x000000ff
   mov   eax, CSR_BIOS_MAILBOX_DATA_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)

   push  ebx                                                                       ; save the target upi ratio
   mov   ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ                                        ; set the comand of speed switch
   mov   ecx, edi                                                                  ; restore the current link number
   xor   eax, eax
   mov   al, cl
   shl   ax, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB          ; set the Link Instance
   or    ebx, eax
   pop   eax                                                                       ; restore the target upi ratio
   cmp   al, 0                                                                     ; check if it requires to switch to slow speed
   jz    adaptation_s3_slow_mode
   or    ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_FAST_MODE_BIT                ; set the FAST MODE bit
adaptation_s3_slow_mode:
   or    ebx, MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_TRIGGER_BIT                  ; set the trigger bit
   or    ebx, RUN_BUSY_BIT                                                         ; set Run_Busy Bit
   mov   eax, CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_SPR
   call  ASM_PFX(WriteCpuCsr)

   call  ASM_PFX(CheckPcodeRunBusy)
   jz   adaptation_steps_exit

   jmp  adaptation_s4_start

adaptation_s3_move_to_next:
   mov   ecx, edi         ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET
   jnz   adaptation_s3_check_next_link

   ;
   ; Step 4: Clear the cri_pclkchangeok_flag
   ;
adaptation_s4_start:
   xor   ecx, ecx
adaptation_s4_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff
   jz    adaptation_s4_move_to_next

   cmp   bl, 0                         ;only set in slow mode step
   jnz   adaptation_s4_clr_pclk
   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   mov   ecx, edi                  ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   mov   ebx, eax
   and   ebx, ~ (C_INIT_BITS)                ; Clear c_init to 0
   mov   eax, CSR_KTIREUT_PH_CTR1_SPR
   call  ASM_PFX(WriteUpiAgentCsr)

adaptation_s4_clr_pclk:
   mov   eax, CSR_PIPE_CLK_RATE_CTRL_SPR
   mov   ecx, edi                  ; restore the current link number
   call  ASM_PFX(ReadUpiAgentCsr)
   mov   ebx, eax
   or    ebx, CRI_PCLKCHANGOK_FLAG_BIT                ; Clear the cri_pclkchangeok_flag by writing 1
   mov   eax, CSR_PIPE_CLK_RATE_CTRL_SPR
   call  ASM_PFX(WriteUpiAgentCsr)

adaptation_s4_move_to_next:
   mov   ecx, edi         ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET
   jnz   adaptation_s4_check_next_link

adaptation_steps_exit:
   pop   edi
   pop   esi
   pop   edx
   pop   ecx
   pop   ebx
   pop   eax
   ret

;------------------------------------------------------------------------------
; Kti Adaptation flow which need to be run in each AP
;  Step1: switch the links speed to target and the links will be in fast loopback mode
;  Step2: switch the links in loopback mode to slow mode L0 which will build the link up again
; input: EAX = The AP's UPI links' target upi ratio
;
; Registers changed: None
;------------------------------------------------------------------------------
global ASM_PFX(ApAdaptation)
ASM_PFX(ApAdaptation):
   push   eax
   push   ebx
   push   ecx
   push   edi
   push   esi

   mov    esi, eax         ; save the Upi Ratio
   mov    ebx, UPI_TXEQ_ADAPTATION_LOOPBACK_PHASE
   call   ASM_PFX(ApAdaptationSteps)        ; switching to fast loopback mode

   ; polling the sp_use_back_channel bit to be 1
   xor   ecx, ecx
ap_adaptation_check_next_link:
   mov   edi, ecx         ; save the current link number
   mov   ebx, esi
   xor   eax, eax
   mov   al, 8
   mul   cl
   xchg  ax, cx
   shr   ebx, cl
   cmp   bl, 0xff
   jz    ap_adaptation_move_to_next

   mov   ecx, DELAY_COUNT
poll_spusebackchannel:
   push  ecx
   mov   ecx, edi                                ; restore the current link number
   mov   eax, CSR_KTIREUT_PH_CIS_SPR
   call  ASM_PFX(ReadUpiAgentCsr)
   pop   ecx
   test  al, SP_USE_BACK_CHANNEL_INUSE_BIT       ; check sp_use_back_channel to be set to 1
   loopz  poll_spusebackchannel
   or    ecx, ecx
   jz    disable_link

set_link_to_slow_mode:
   mov   ecx, edi         ; restore the current link number
   mov   al,  8
   mul   cl
   mov   ebx, UPI_SLOW_MODE_MASK
   xchg  ax, cx
   shl   ebx, cl
   mov   eax, esi
   and   eax, ebx
   mov   esi, eax         ; update the upi ratio
   jmp   ap_adaptation_move_to_next

disable_link:
   mov   ecx, edi         ; restore the current link number
   mov   al,  8
   mul   cl
   mov   ebx, UPI_LINK_DISABLE_MASK
   xchg  ax, cx
   shl   ebx, cl
   mov   eax, esi
   or    eax, ebx
   mov   esi, eax         ; update the upi ratio

ap_adaptation_move_to_next:
   mov   ecx, edi         ; restore the current link number
   inc   cl
   cmp   cl, MAX_KTI_PORTS_PER_SOCKET
   jnz   ap_adaptation_check_next_link

   mov   eax, esi
   mov   ebx, UPI_TXEQ_ADAPTATION_SLOWMODE_PHASE
   call  ASM_PFX(ApAdaptationSteps)        ; switching to slow mode

   pop   esi
   pop   edi
   pop   ecx
   pop   ebx
   pop   eax
   ret