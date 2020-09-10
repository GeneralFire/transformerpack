## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2006 - 2013 Intel Corporation. <BR>
#
# The source code contained or described herein and all documents related to the
# source code ("Material") are owned by Intel Corporation or its suppliers or
# licensors. Title to the Material remains with Intel Corporation or its suppliers
# and licensors. The Material may contain trade secrets and proprietary    and
# confidential information of Intel Corporation and its suppliers and licensors,
# and is protected by worldwide copyright and trade secret laws and treaty
# provisions. No part of the Material may be used, copied, reproduced, modified,
# published, uploaded, posted, transmitted, distributed, or disclosed in any way
# without Intel's prior express written permission.
#
# No license under any patent, copyright, trade secret or other intellectual
# property right is granted to or conferred upon you by disclosure or delivery
# of the Materials, either expressly, by implication, inducement, estoppel or
# otherwise. Any license under such intellectual property rights must be
# express and approved by Intel in writing.
#
# Unless otherwise agreed by Intel in writing, you may not remove or alter
# this notice or any other notice embedded in Materials by Intel or
# Intel's suppliers or licensors in any way.
##

#include <Library/BaseLib.h>

ASM_GLOBAL ASM_PFX(m16Start), ASM_PFX(m16Size), ASM_PFX(mThunk16Attr), ASM_PFX(m16Gdt), ASM_PFX(m16GdtrBase), ASM_PFX(mTransition)
ASM_GLOBAL ASM_PFX(InternalAsmThunk16)

# define the structure of IA32_REGS
.set  _EDI, 0       #size 4
.set  _ESI, 4       #size 4
.set  _EBP, 8       #size 4
.set  _ESP, 12      #size 4
.set  _EBX, 16      #size 4
.set  _EDX, 20      #size 4
.set  _ECX, 24      #size 4
.set  _EAX, 28      #size 4
.set  _DS,  32      #size 2
.set  _ES,  34      #size 2
.set  _FS,  36      #size 2
.set  _GS,  38      #size 2
.set  _EFLAGS, 40   #size 4
.set  _EIP, 44      #size 4
.set  _CS, 48       #size 2
.set  _SS, 50       #size 2
.set  IA32_REGS_SIZE, 52

    .text
    .code16

ASM_PFX(m16Start):

SavedGdt:     .space  6

ASM_PFX(BackFromUserCode):
    push    %ss
    push    %cs

    calll   L_Base1                     # push eip
L_Base1:
    pushfl
    cli                                 # disable interrupts
    push    %gs
    push    %fs
    push    %es
    push    %ds
    pushal
    .byte   0x66, 0xba                  # mov edx, imm32
ASM_PFX(ThunkAttr): .space  4
    testb   $THUNK_ATTRIBUTE_DISABLE_A20_MASK_INT_15, %dl
    jz      1f
    movw    $0x2401, %ax
    int     $0x15
    cli                                 # disable interrupts
    jnc     2f
1:
    testb   $THUNK_ATTRIBUTE_DISABLE_A20_MASK_KBD_CTRL, %dl
    jz      2f
    inb     $0x92, %al
    orb     $2, %al
    outb    %al, $0x92                  # deactivate A20M#
2:
    xorl    %eax, %eax
    movw    %ss, %ax
    leal    IA32_REGS_SIZE(%esp), %ebp
    mov     %ebp, (_ESP - IA32_REGS_SIZE)(%bp)
    mov     (_EIP - IA32_REGS_SIZE)(%bp), %bx
    shll    $4, %eax
    addl    %eax, %ebp
    .byte   0x66, 0xb8                  # mov eax, imm32
SavedCr4:   .space  4
    movl    %eax, %cr4
    lgdtl   %cs:(SavedGdt - L_Base1)(%bx)
    .byte   0x66, 0xb8                  # mov eax, imm32
SavedCr0:   .space  4
    movl    %eax, %cr0
    .byte   0xb8                        # mov ax, imm16
SavedSs:    .space  2
    movl    %eax, %ss
    .byte   0x66, 0xbc                  # mov esp, imm32
SavedEsp:   .space  4
    lretl                               # return to protected mode

_EntryPoint:    .long      ASM_PFX(ToUserCode) - ASM_PFX(m16Start)
                .word      0x8
_16Idtr:        .word      0x3ff
                .long      0
_16Gdtr:        .word      GdtEnd - _NullSegDesc - 1
_16GdtrBase:    .long      _NullSegDesc

ASM_PFX(ToUserCode):
    movw    %ss, %dx
    movw    %cx, %ss                    # set new segment selectors
    movw    %cx, %ds
    movw    %cx, %es
    movw    %cx, %fs
    movw    %cx, %gs
    movl    %eax, %cr0                  # real mode starts at next instruction
                                        #  which (per SDM) *must* be a far JMP.
    ljmpw   $0,$0                       # will be filled in by InternalAsmThunk16
L_Base:                                 #  to point here.
    movl    %ebp, %cr4
    movw    %si, %ss                    # set up 16-bit stack segment
    xchgl   %ebx, %esp                  # set up 16-bit stack pointer

    movw    IA32_REGS_SIZE(%esp), %bp   # get BackToUserCode address from stack
    mov     %dx, %cs:(SavedSs - ASM_PFX(BackFromUserCode))(%bp)
    mov     %ebx, %cs:(SavedEsp - ASM_PFX(BackFromUserCode))(%bp)
    lidtl   %cs:(_16Idtr - ASM_PFX(BackFromUserCode))(%bp)
    popal
    pop     %ds
    pop     %es
    pop     %fs
    pop     %gs
    popfl
    lretl                               # transfer control to user code

_NullSegDesc:   .quad   0
_16CsDesc:
                .word   -1
                .word   0
                .byte   0
                .byte   0x9b
                .byte   0x8f            # 16-bit segment, 4GB limit
                .byte   0
_16DsDesc:
                .word   -1
                .word   0
                .byte   0
                .byte   0x93
                .byte   0x8f            # 16-bit segment, 4GB limit
                .byte   0
GdtEnd:

    .code32
#
#   @param  RegSet  The pointer to a IA32_DWORD_REGS structure
#   @param  Transition  The pointer to the transition code
#   @return The address of the 16-bit stack after returning from user code
#
ASM_PFX(InternalAsmThunk16):
    push    %ebp
    push    %ebx
    push    %esi
    push    %edi
    push    %ds
    push    %es
    push    %fs
    push    %gs
    movl    36(%esp), %esi              # esi <- RegSet
    movzwl  _SS(%esi), %edx
    mov     _ESP(%esi), %edi
    add     $(-(IA32_REGS_SIZE + 4)), %edi
    movl    %edi, %ebx                  # ebx <- stack offset
    imul    $0x10, %edx, %eax
    push    $(IA32_REGS_SIZE / 4)
    addl    %eax, %edi                  # edi <- linear address of 16-bit stack
    pop     %ecx
    rep
    movsl                               # copy RegSet
    movl    40(%esp), %eax              # eax <- address of transition code
    movl    %edx, %esi                  # esi <- 16-bit stack segment
    lea     (SavedCr0 - ASM_PFX(m16Start))(%eax), %edx
    movl    %eax, %ecx
    andl    $0xf, %ecx
    shll    $12, %eax
    lea     (ASM_PFX(BackFromUserCode) - ASM_PFX(m16Start))(%ecx), %ecx
    movw    %cx, %ax
    stosl                               # [edi] <- return address of user code
    addl    $(L_Base - ASM_PFX(BackFromUserCode)), %eax
    movl    %eax, (L_Base - SavedCr0 - 4)(%edx)
    sgdtl   (SavedGdt - SavedCr0)(%edx)
    sidtl   0x24(%esp)
    movl    %cr0, %eax
    movl    %eax, (%edx)                # save CR0 in SavedCr0
    andl    $0x7ffffffe, %eax           # clear PE, PG bits
    movl    %cr4, %ebp
    mov     %ebp, (SavedCr4 - SavedCr0)(%edx)
    andl    $0xffffffcf, %ebp           # clear PAE, PSE bits
    pushl   $0x10
    pop     %ecx                        # ecx <- selector for data segments
    lgdtl   (_16Gdtr - SavedCr0)(%edx)
    pushfl
    lcall   *(_EntryPoint - SavedCr0)(%edx)
    popfl
    lidtl   0x24(%esp)
    lea     -IA32_REGS_SIZE(%ebp), %eax
    pop     %gs
    pop     %fs
    pop     %es
    pop     %ds
    pop     %edi
    pop     %esi
    pop     %ebx
    pop     %ebp
    ret

    .const:

ASM_PFX(m16Size):        .word      ASM_PFX(InternalAsmThunk16)  - ASM_PFX(m16Start)
ASM_PFX(mThunk16Attr):   .word      ASM_PFX(ThunkAttr)          - ASM_PFX(m16Start)
ASM_PFX(m16Gdt):         .word      _NullSegDesc        - ASM_PFX(m16Start)
ASM_PFX(m16GdtrBase):    .word      _16GdtrBase         - ASM_PFX(m16Start)
ASM_PFX(mTransition):    .word      _EntryPoint         - ASM_PFX(m16Start)
