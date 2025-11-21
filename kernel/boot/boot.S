; boot.S - inicializa CPU e chama loader
.global _start

_start:
    cli                 ; desativa interrupções
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $stack_top, %sp

    call load_kernel     ; chama rotina que tenta carregar kernel

    ; se load_kernel retornar, é porque não encontrou kernel
    mov $msg_no_kernel, %si
    call print_string
    hlt                 ; para CPU

; -----------------------
; rotina para imprimir string (simples, usando BIOS/INT 10h em 16 bits real mode)
print_string:
    pusha
.next_char:
    lodsb
    cmp $0, %al
    je .done
    mov $0x0E, %ah      ; teletipo BIOS
    int $0x10
    jmp .next_char
.done:
    popa
    ret

; -----------------------
section .data
msg_no_kernel: db "No Kernel Found!", 0

section .bss
stack: resb 4096
stack_top:
