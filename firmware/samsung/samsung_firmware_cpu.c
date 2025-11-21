#include "../../include/DroidPureC/cpu.h"
#include "../../include/DroidPureC/io.h"

// Inicializa a CPU
void cpu_init(CPU* cpu) {
    cpu->online = 0;
    cpu->id = 0;
    cpu->cores = 4;       // exemplo
    cpu->frequency = 2200; // MHz
    kprint("CPU inicializada!\n");
}

// Liga a CPU
void cpu_start(CPU* cpu) {
    cpu->online = 1;
    kprint("CPU ligada!\n");
}

// Desliga a CPU
void cpu_stop(CPU* cpu) {
    cpu->online = 0;
    kprint("CPU desligada!\n");
}

// Mostra informações da CPU
void cpu_info(CPU* cpu) {
    kprint("CPU ID: ");
    // aqui você precisaria criar função para imprimir números
    kprint_hex(cpu->id);
    kprint(", cores: ");
    kprint_hex(cpu->cores);
    kprint(", freq: ");
    kprint_hex(cpu->frequency);
    kprint(" MHz\n");
}
