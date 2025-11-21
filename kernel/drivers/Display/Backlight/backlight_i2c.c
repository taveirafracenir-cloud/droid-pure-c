#include "backlight.h"
#include <stdio.h>

/* Função que envia brilho via I2C */
static void i2c_set_brightness(struct backlight_device *dev, int value)
{
    // Aqui você colocaria a comunicação I2C real
    printf("I2C: brilho definido para %d\n", value);
}

/* Habilitar I2C */
static void i2c_enable(struct backlight_device *dev)
{
    printf("I2C backlight habilitado\n");
}

/* Desabilitar I2C */
static void i2c_disable(struct backlight_device *dev)
{
    printf("I2C backlight desabilitado\n");
}

/* Registrar driver I2C */
void backlight_i2c_init(void)
{
    static struct backlight_device i2c_dev = {
        .name = "I2C Backlight",
        .max_brightness = 255,
        .set_brightness = i2c_set_brightness,
        .enable = i2c_enable,
        .disable = i2c_disable
    };

    backlight_register(&i2c_dev);
    backlight_enable(&i2c_dev);
    backlight_set_brightness(&i2c_dev, 200); // brilho inicial
}
