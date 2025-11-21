#define STACK_SIZE 4096

.section .bss
stack: resb STACK_SIZE
.section .bss
stack: resb 4096
%macro PUSH_ALL 0
    push rax
    push rbx
    push rcx
%endmacro

PUSH_ALL
