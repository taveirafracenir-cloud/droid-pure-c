#include "backlight.h"
#include <stdio.h>

/* Função que envia brilho via PWM */
static void pwm_set_brightness(struct backlight_device *dev, int value)
{
    // Aqui você colocaria a comunicação real com hardware PWM
    printf("PWM: brilho definido para %d\n", value);
}

/* Habilitar PWM */
static void pwm_enable(struct backlight_device *dev)
{
    printf("PWM backlight habilitado\n");
}

/* Desabilitar PWM */
static void pwm_disable(struct backlight_device *dev)
{
    printf("PWM backlight desabilitado\n");
}

/* Registrar driver PWM */
void backlight_pwm_init(void)
{
    static struct backlight_device pwm_dev = {
        .name = "PWM Backlight",
        .max_brightness = 255,
        .set_brightness = pwm_set_brightness,
        .enable = pwm_enable,
        .disable = pwm_disable
    };

    backlight_register(&pwm_dev);
    backlight_enable(&pwm_dev);
    backlight_set_brightness(&pwm_dev, 128); // brilho inicial
}
