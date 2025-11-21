; loader.S - tenta carregar kernel
.global load_kernel
load_kernel:
    ; Aqui você colocaria código real de leitura de disco
    ; Por enquanto simulamos "kernel não encontrado"
    mov $0, %eax   ; retorna 0 = falha
    ret
