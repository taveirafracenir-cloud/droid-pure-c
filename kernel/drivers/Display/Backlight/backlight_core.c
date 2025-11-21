#include "backlight.h"
#include <stdio.h>

/* Registrar dispositivo */
void backlight_register(struct backlight_device *dev)
{
    if (!dev) return;
    dev->brightness = 0;
    dev->enabled = false;
    printf("Backlight device '%s' registrado\n", dev->name);
}

/* Configurar brilho */
void backlight_set_brightness(struct backlight_device *dev, int value)
{
    if (!dev) return;

    if (value < 0) value = 0;
    if (value > dev->max_brightness) value = dev->max_brightness;

    dev->brightness = value;
    if (dev->set_brightness)
        dev->set_brightness(dev);

    printf("Backlight '%s' brilho: %d/%d\n", dev->name, dev->brightness, dev->max_brightness);
}

/* Ativar backlight */
void backlight_enable(struct backlight_device *dev)
{
    if (!dev) return;
    dev->enabled = true;
    if (dev->enable) dev->enable(dev);
    printf("Backlight '%s' ativado\n", dev->name);
}

/* Desativar backlight */
void backlight_disable(struct backlight_device *dev)
{
    if (!dev) return;
    dev->enabled = false;
    if (dev->disable) dev->disable(dev);
    printf("Backlight '%s' desativado\n", dev->name);
}
