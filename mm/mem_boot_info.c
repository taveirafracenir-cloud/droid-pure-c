#include "../include/DroidPureC/types.h"
#include "../include/printk/printk.h"

// Estrutura interna de memória (exemplo simples)
typedef struct {
    u32 total_memory;    // memória total em KB
    u32 free_memory;     // memória livre em KB
    u32 reserved_memory; // memória reservada pelo kernel
} MemoryInfo;

// Variável global de boot info
MemoryInfo boot_memory_info;

// Inicializa a memória do kernel
void mem_boot_init(u32 total_kb, u32 reserved_kb) {
    boot_memory_info.total_memory = total_kb;
    boot_memory_info.reserved_memory = reserved_kb;
    boot_memory_info.free_memory = total_kb - reserved_kb;

    printk("Memória inicializada:\n");
    printk("Total: ");
    printk_dec(boot_memory_info.total_memory);
    printk(" KB\n");

    printk("Reservada: ");
    printk_dec(boot_memory_info.reserved_memory);
    printk(" KB\n");

    printk("Livre: ");
    printk_dec(boot_memory_info.free_memory);
    printk(" KB\n");
}

// Função para atualizar memória livre
void mem_update_free(u32 free_kb) {
    boot_memory_info.free_memory = free_kb;
}

// Função para obter memória total
u32 mem_get_total() {
    return boot_memory_info.total_memory;
}

// Função para obter memória livre
u32 mem_get_free() {
    return boot_memory_info.free_memory;
}

// Função para obter memória reservada
u32 mem_get_reserved() {
    return boot_memory_info.reserved_memory;
}
