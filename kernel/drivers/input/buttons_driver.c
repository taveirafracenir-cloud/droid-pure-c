#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

static struct input_dev *buttons_dev;

static int __init buttons_init(void)
{
    int err;

    buttons_dev = input_allocate_device();
    if (!buttons_dev)
        return -ENOMEM;

    buttons_dev->name = "droidpurec_buttons";
    buttons_dev->phys = "droidpurec/input0";
    buttons_dev->id.bustype = BUS_HOST;

    // Habilita eventos de tecla
    __set_bit(EV_KEY, buttons_dev->evbit);
    __set_bit(KEY_POWER, buttons_dev->keybit);
    __set_bit(KEY_VOLUMEUP, buttons_dev->keybit);
    __set_bit(KEY_VOLUMEDOWN, buttons_dev->keybit);

    err = input_register_device(buttons_dev);
    if (err) {
        input_free_device(buttons_dev);
        return err;
    }

    printk(KERN_INFO "DroidPureC buttons driver registered\n");
    return 0;
}

static void __exit buttons_exit(void)
{
    input_unregister_device(buttons_dev);
    printk(KERN_INFO "DroidPureC buttons driver removed\n");
}

module_init(buttons_init);
module_exit(buttons_exit);

MODULE_LICENSE("Apache-2.0");
MODULE_AUTHOR("Francenir Inc. / DroidPureC Kernel Inc.");
MODULE_DESCRIPTION("Driver simples para botões POWER, VOL UP e VOL DOWN");
