/*
 * Exemplo real de Assembly com pré-processador C
 * Estilo Linux kernel – válido no DroidPureC kernel
 */

#include "kernel/config.h"     // pode incluir headers C!

#define STACK_SIZE 4096
#define KERNEL_BASE 0x100000

    .section .bss
    .align 16

/* Reserva a pilha usando define (FUNCIONA!) */
stack_bottom:
    .space STACK_SIZE
stack_top:

/* Macro no estilo GNU Assembly */
.macro PUSH_ALL
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
.endm

/* Outra macro com argumentos */
.macro LOAD_IMM reg, val
    mov $\val, %\reg
.endm

/* Ponto de entrada do boot */
    .section .text
    .global _start
_start:

    /* Usa define para acessar constante */
    mov $KERNEL_BASE, %rax

    /* Usa macro para carregar valores */
    LOAD_IMM rbx, 0xDEADBEEF

    /* Usa macro de salvar contexto */
    PUSH_ALL

    /* Ajusta a pilha usando o define */
    mov $stack_top, %rsp

    /* Chama função C do kernel */
    call kernel_main

1:
    jmp 1b      /* loop infinito */
#define GDT_ENTRY_KERNEL_CS 1
#include <asm/offsets.h>
#ifdef CONFIG_SMP    
