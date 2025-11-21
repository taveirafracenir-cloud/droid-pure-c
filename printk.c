#include "../../include/printk/printk.h"

// Endereço da memória VGA (modo texto, 80x25)
volatile char* VIDEO = (volatile char*)0xB8000;
u16 cursor_pos = 0;

// Imprime um único caractere na tela
void putchar(char c) {
    if (c == '\n') {
        cursor_pos += 80 - (cursor_pos % 80); // pula para próxima linha
    } else {
        VIDEO[cursor_pos * 2] = c;   // caractere
        VIDEO[cursor_pos * 2 + 1] = 0x07; // cor (cinza claro no preto)
        cursor_pos++;
    }
}

// Imprime uma string
void printk(const char* str) {
    for (u32 i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}

// Imprime número em hexadecimal
void printk_hex(u32 value) {
    char hex[] = "0123456789ABCDEF";
    char buffer[9]; // 8 dígitos + '\0'
    buffer[8] = '\0';

    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex[value & 0xF];
        value >>= 4;
    }

    printk(buffer);
}

// Imprime número em decimal
void printk_dec(u32 value) {
    char buffer[11]; // 10 dígitos + '\0'
    buffer[10] = '\0';
    int i = 9;

    if (value == 0) {
        putchar('0');
        return;
    }

    while (value > 0) {
        buffer[i--] = '0' + (value % 10);
        value /= 10;
    }

    printk(&buffer[i+1]);
}
