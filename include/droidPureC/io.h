#ifndef DROIDPUREC_IO_H
#define DROIDPUREC_IO_H

#include "types.h"

// Funções básicas de saída
void kprint(const char* str);    // imprime uma string
void kprint_hex(u32 value);      // imprime um número em hexadecimal
void kprint_dec(u32 value);      // imprime um número decimal
void kprint_char(char c);        // imprime um único caractere

// Funções de entrada (futuro)
char kgetchar();                 // lê um caractere do teclado (opcional)
u8   kkey_pressed();             // retorna 1 se tecla pressionada, 0 caso contrário

#endif
