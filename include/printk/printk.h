#ifndef DROIDPUREC_PRINTK_H
#define DROIDPUREC_PRINTK_H

#include "types.h"

// Função principal de impressão de strings
void printk(const char* str);

// Função para imprimir um caractere
void putchar(char c);

// Funções para imprimir números
void printk_hex(u32 value);
void printk_dec(u32 value);

#endif
