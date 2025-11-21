BITS 64

%define STACK_SIZE 4096

SECTION .bss
stack:      resb STACK_SIZE
stack_top:  equ stack + STACK_SIZE

SECTION .text
global _start

%macro PUSH_ALL 0
    push rax
    push rbx
    push rcx
%endmacro

_start:
    PUSH_ALL

    ; Seu código aqui
    jmp $
