#ifndef DROIDPUREC_CPU_H
#define DROIDPUREC_CPU_H

#include "types.h"

// Estrutura que representa uma CPU
typedef struct {
    u32 id;
    u32 cores;
    u32 frequency; // em MHz
    u8 online;     // 1 = ativa, 0 = inativa
} CPU;

// Funções públicas do driver/firmware
void cpu_init(CPU* cpu);
void cpu_start(CPU* cpu);
void cpu_stop(CPU* cpu);
void cpu_info(CPU* cpu);

#endif
