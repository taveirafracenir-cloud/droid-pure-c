/*
 * Copyright 2025–2026 Francenir Inc.
 * Copyright 2025–2026 DroidPureC Kernel Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 */

#include <linux/kernel.h>
#include <linux/module.h>

/* Estrutura para cada frequência de áudio */
struct audio_rate {
    unsigned int hz;
    const char *name;
};

/* Tabela de todas frequências suportadas */
static struct audio_rate audio_rates[] = {
    { 44100, "44.1 kHz CD" },
    { 48000, "48 kHz" },
    { 96000, "96 kHz" },
    { 192000, "192 kHz" },
};

/* Quantidade de taxas suportadas */
#define AUDIO_RATES_COUNT (sizeof(audio_rates)/sizeof(audio_rates[0]))

/* Exemplo de uso: printar todas taxas */
static void print_audio_rates(void)
{
    int i;
    for (i = 0; i < AUDIO_RATES_COUNT; i++) {
        printk(KERN_INFO "Audio rate: %u Hz (%s)\n",
               audio_rates[i].hz,
               audio_rates[i].name);
    }
}

/* Inicialização do módulo */
static int __init audio_table_init(void)
{
    printk(KERN_INFO "Audio rates table loaded\n");
    print_audio_rates();
    return 0;
}

/* Cleanup */
static void __exit audio_table_exit(void)
{
    printk(KERN_INFO "Audio rates table unloaded\n");
}

module_init(audio_table_init);
module_exit(audio_table_exit);

MODULE_LICENSE("Apache-2.0");
MODULE_AUTHOR("Francenir Inc. / DroidPureC Kernel Inc.");
MODULE_DESCRIPTION("Tabela completa de frequências de áudio para driver próprio");
