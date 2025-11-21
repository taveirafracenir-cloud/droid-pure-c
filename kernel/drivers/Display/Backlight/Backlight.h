/*
 * Copyright 2025–2026 Francenir Inc.
 * Copyright 2025–2026 DroidPureC Kernel Inc.
 *
 * Licensed under the Apache License, Version 2.0
 *
 * Driver de backlight real - cabeçalho
 */

#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <stdint.h>
#include <stdbool.h>

/* Estrutura do backlight */
struct backlight_device {
    const char *name;
    int brightness;       // 0 - 255
    int max_brightness;   // normalmente 255
    bool enabled;
    void (*set_brightness)(struct backlight_device *dev, int value);
    void (*enable)(struct backlight_device *dev);
    void (*disable)(struct backlight_device *dev);
};

/* API de backlight */
void backlight_register(struct backlight_device *dev);
void backlight_set_brightness(struct backlight_device *dev, int value);
void backlight_enable(struct backlight_device *dev);
void backlight_disable(struct backlight_device *dev);

#endif // BACKLIGHT_H
