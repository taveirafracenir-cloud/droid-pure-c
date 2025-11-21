// drivers/media/i2c/mycamera.c
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/v4l2-subdev.h>

struct mycam {
    struct v4l2_subdev sd;
    struct i2c_client *client;
};

static int mycam_probe(struct i2c_client *client)
{
    struct mycam *cam;

    cam = devm_kzalloc(&client->dev, sizeof(*cam), GFP_KERNEL);
    if (!cam)
        return -ENOMEM;

    v4l2_i2c_subdev_init(&cam->sd, client, NULL);

    dev_info(&client->dev, "mycamera inicializado!\n");

    return 0;
}

static const struct i2c_device_id mycam_id[] = {
    { "mycamera", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, mycam_id);

static struct i2c_driver mycam_driver = {
    .driver = {
        .name = "mycamera",
    },
    .probe = mycam_probe,
    .id_table = mycam_id,
};

module_i2c_driver(mycam_driver);

MODULE_LICENSE("Apache-2.0");
/*
 * Copyright 2025–2026 Francenir Inc.
 * Copyright 2025–2026 DroidPureC Kernel Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 * Driver de câmera próprio - Parte 2
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/v4l2-subdev.h>
#include <linux/v4l2-mediabus.h>

/* Estrutura do nosso dispositivo */
struct mycam {
    struct v4l2_subdev sd;
    struct i2c_client *client;
    int streaming;
};

/* Registradores genéricos do sensor */
#define MYCAM_REG_POWER 0x01
#define MYCAM_REG_CTRL  0x02

/* Inicializa o sensor */
static int mycam_init_sensor(struct mycam *cam)
{
    int ret;

    /* Power up do sensor */
    ret = i2c_smbus_write_byte_data(cam->client, MYCAM_REG_POWER, 0x01);
    if (ret < 0) {
        dev_err(&cam->client->dev,
            "HARDWARE ERROR: CAM_NO_DETECTED_UNKNOWN_CAM\n");
        return ret;
    }

    /* Configuração inicial do sensor (modo streaming desligado) */
    ret = i2c_smbus_write_byte_data(cam->client, MYCAM_REG_CTRL, 0x00);
    if (ret < 0)
        return ret;

    cam->streaming = 0;
    dev_info(&cam->client->dev, "mycamera sensor initialized\n");
    return 0;
}

/* Função para iniciar ou parar streaming */
static int mycam_s_stream(struct v4l2_subdev *sd, int enable)
{
    struct mycam *cam = container_of(sd, struct mycam, sd);
    int ret;

    if (enable) {
        /* Ativa streaming */
        ret = i2c_smbus_write_byte_data(cam->client, MYCAM_REG_CTRL, 0x01);
        if (ret < 0)
            return ret;
        cam->streaming = 1;
        dev_info(&cam->client->dev, "mycamera streaming ON\n");
    } else {
        /* Desativa streaming */
        ret = i2c_smbus_write_byte_data(cam->client, MYCAM_REG_CTRL, 0x00);
        if (ret < 0)
            return ret;
        cam->streaming = 0;
        dev_info(&cam->client->dev, "mycamera streaming OFF\n");
    }

    return 0;
}

/* Estrutura de operação do V4L2 */
static const struct v4l2_subdev_core_ops mycam_core_ops = {
    /* Pode adicionar reset, init, power down etc */
};

static const struct v4l2_subdev_video_ops mycam_video_ops = {
    .s_stream = mycam_s_stream,
};

static const struct v4l2_subdev_ops mycam_subdev_ops = {
    .core = &mycam_core_ops,
    .video = &mycam_video_ops,
};

/* Probe do driver */
static int mycam_probe(struct i2c_client *client)
{
    struct mycam *cam;
    int ret;

    cam = devm_kzalloc(&client->dev, sizeof(*cam), GFP_KERNEL);
    if (!cam)
        return -ENOMEM;

    cam->client = client;

    /* Inicializa o subdevice */
    v4l2_i2c_subdev_init(&cam->sd, client, &mycam_subdev_ops);

    /* Inicializa o sensor */
    ret = mycam_init_sensor(cam);
    if (ret < 0)
        return ret;

    dev_info(&client->dev, "mycamera driver ready\n");
    return 0;
}

/* Tabela de dispositivos I2C suportados */
static const struct i2c_device_id mycam_id[] = {
    { "mycamera", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, mycam_id);

/* Driver I2C */
static struct i2c_driver mycam_driver = {
    .driver = {
        .name = "mycamera",
    },
    .probe = mycam_probe,
    .id_table = mycam_id,
};

module_i2c_driver(mycam_driver);

MODULE_LICENSE("Apache-2.0");
MODULE_DESCRIPTION("Driver próprio de câmera Francenir/DroidPureC");
MODULE_AUTHOR("Francenir Inc. / DroidPureC Kernel Inc.");
/*
 * Copyright 2025–2026 Francenir Inc.
 * Copyright 2025–2026 DroidPureC Kernel Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 * Driver de câmera próprio - Parte 3
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/v4l2-subdev.h>
#include <linux/v4l2-mediabus.h>
#include <linux/slab.h>

#define MYCAM_WIDTH  640
#define MYCAM_HEIGHT 480
#define MYCAM_BPP    2   // bytes por pixel (ex: YUYV)

struct mycam_buffer {
    void *data;
    size_t size;
};

struct mycam {
    struct v4l2_subdev sd;
    struct i2c_client *client;
    int streaming;
    struct mycam_buffer *buf;
};

/* Aloca buffer de vídeo */
static int mycam_alloc_buffer(struct mycam *cam)
{
    cam->buf = kzalloc(sizeof(struct mycam_buffer), GFP_KERNEL);
    if (!cam->buf)
        return -ENOMEM;

    cam->buf->size = MYCAM_WIDTH * MYCAM_HEIGHT * MYCAM_BPP;
    cam->buf->data = kzalloc(cam->buf->size, GFP_KERNEL);
    if (!cam->buf->data) {
        kfree(cam->buf);
        cam->buf = NULL;
        return -ENOMEM;
    }

    return 0;
}

/* Libera buffer de vídeo */
static void mycam_free_buffer(struct mycam *cam)
{
    if (!cam || !cam->buf)
        return;

    kfree(cam->buf->data);
    kfree(cam->buf);
    cam->buf = NULL;
}

/* Captura um frame (simples leitura do buffer) */
static int mycam_capture_frame(struct mycam *cam, void *out, size_t len)
{
    if (!cam || !cam->buf || !cam->streaming)
        return -EINVAL;

    if (len > cam->buf->size)
        len = cam->buf->size;

    memcpy(out, cam->buf->data, len);
    return len;
}

/* Configura resolução do sensor (apenas variáveis, você pode expandir) */
static int mycam_set_format(struct mycam *cam, int width, int height)
{
    if (!cam)
        return -EINVAL;

    // Aqui você poderia escrever registradores do sensor via I2C
    // Ex: i2c_smbus_write_byte_data(cam->client, REG_WIDTH, width);
    // Ex: i2c_smbus_write_byte_data(cam->client, REG_HEIGHT, height);

    dev_info(&cam->client->dev,
        "mycamera format set %dx%d\n", width, height);

    return 0;
}

/* Stream ON/OFF atualizado para alocar buffer */
static int mycam_s_stream(struct v4l2_subdev *sd, int enable)
{
    struct mycam *cam = container_of(sd, struct mycam, sd);
    int ret;

    if (enable) {
        ret = mycam_alloc_buffer(cam);
        if (ret < 0)
            return ret;

        ret = i2c_smbus_write_byte_data(cam->client, 0x02, 0x01);
        if (ret < 0) {
            mycam_free_buffer(cam);
            return ret;
        }

        cam->streaming = 1;
        dev_info(&cam->client->dev, "mycamera streaming ON\n");
    } else {
        i2c_smbus_write_byte_data(cam->client, 0x02, 0x00);
        mycam_free_buffer(cam);
        cam->streaming = 0;
        dev_info(&cam->client->dev, "mycamera streaming OFF\n");
    }

    return 0;
}

/* Estrutura V4L2 */
static const struct v4l2_subdev_video_ops mycam_video_ops = {
    .s_stream = mycam_s_stream,
};

/* Subdev ops */
static const struct v4l2_subdev_ops mycam_subdev_ops = {
    .video = &mycam_video_ops,
};

/* Probe atualizado para buffer */
static int mycam_probe(struct i2c_client *client)
{
    struct mycam *cam;
    int ret;

    cam = devm_kzalloc(&client->dev, sizeof(*cam), GFP_KERNEL);
    if (!cam)
        return -ENOMEM;

    cam->client = client;
    v4l2_i2c_subdev_init(&cam->sd, client, &mycam_subdev_ops);

    ret = i2c_smbus_write_byte_data(client, 0x01, 0x01); // power up
    if (ret < 0) {
        dev_err(&client->dev, "HARDWARE ERROR: CAM_NO_DETECTED_UNKNOWN_CAM\n");
        return ret;
    }

    dev_info(&client->dev, "mycamera driver ready\n");
    return 0;
}
/*
 * Copyright 2025–2026 Francenir Inc.
 * Copyright 2025–2026 DroidPureC Kernel Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 * Driver de câmera próprio - Parte 4
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/v4l2-subdev.h>
#include <linux/v4l2-device.h>
#include <linux/v4l2-fh.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

struct mycam {
    struct v4l2_subdev sd;
    struct i2c_client *client;
    int streaming;
    void *buffer;
    size_t buf_size;
    struct v4l2_device v4l2_dev;
};

/* Define resolução e bytes por pixel */
#define MYCAM_WIDTH  640
#define MYCAM_HEIGHT 480
#define MYCAM_BPP    2

/* Inicializa o buffer de vídeo */
static int mycam_alloc_buffer(struct mycam *cam)
{
    cam->buf_size = MYCAM_WIDTH * MYCAM_HEIGHT * MYCAM_BPP;
    cam->buffer = vmalloc(cam->buf_size);
    if (!cam->buffer)
        return -ENOMEM;
    memset(cam->buffer, 0, cam->buf_size);
    return 0;
}

static void mycam_free_buffer(struct mycam *cam)
{
    if (!cam || !cam->buffer)
        return;
    vfree(cam->buffer);
    cam->buffer = NULL;
}

/* Função de captura de frame */
static ssize_t mycam_read_frame(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    struct mycam *cam = file->private_data;

    if (!cam->streaming)
        return -EINVAL;

    if (count > cam->buf_size)
        count = cam->buf_size;

    if (copy_to_user(user_buf, cam->buffer, count))
        return -EFAULT;

    return count;
}

/* Open e release */
static int mycam_open(struct inode *inode, struct file *file)
{
    struct mycam *cam = container_of(inode->i_cdev, struct mycam, sd);
    file->private_data = cam;
    return 0;
}

static int mycam_release(struct inode *inode, struct file *file)
{
    return 0;
}

/* Estrutura de operações do device */
static const struct file_operations mycam_fops = {
    .owner = THIS_MODULE,
    .open = mycam_open,
    .release = mycam_release,
    .read = mycam_read_frame,
};

/* Inicializa streaming */
static int mycam_s_stream(struct v4l2_subdev *sd, int enable)
{
    struct mycam *cam = container_of(sd, struct mycam, sd);
    int ret;

    if (enable) {
        ret = mycam_alloc_buffer(cam);
        if (ret < 0)
            return ret;

        ret = i2c_smbus_write_byte_data(cam->client, 0x02, 0x01); // liga sensor
        if (ret < 0) {
            mycam_free_buffer(cam);
            return ret;
        }

        cam->streaming = 1;
    } else {
        i2c_smbus_write_byte_data(cam->client, 0x02, 0x00); // desliga sensor
        mycam_free_buffer(cam);
        cam->streaming = 0;
    }

    return 0;
}

/* Probe atualizado para registrar device /dev/video0 */
static int mycam_probe(struct i2c_client *client)
{
    struct mycam *cam;
    int ret;

    cam = devm_kzalloc(&client->dev, sizeof(*cam), GFP_KERNEL);
    if (!cam)
        return -ENOMEM;

    cam->client = client;
    v4l2_i2c_subdev_init(&cam->sd, client, NULL);

    ret = i2c_smbus_write_byte_data(client, 0x01, 0x01); // power up sensor
    if (ret < 0) {
        dev_err(&client->dev, "HARDWARE ERROR: CAM_NO_DETECTED_UNKNOWN_CAM\n");
        return ret;
    }

    /* Registra device node /dev/video0 */
    ret = v4l2_device_register(&client->dev, &cam->v4l2_dev);
    if (ret < 0)
        return ret;

    dev_info(&client->dev, "mycamera device registered as /dev/video0\n");
    return 0;
}
/*
 * Copyright 2025–2026 Francenir Inc.
 * Copyright 2025–2026 DroidPureC Kernel Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 * Driver de câmera próprio - Parte 5
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/v4l2-subdev.h>
#include <linux/v4l2-device.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define MYCAM_MAX_WIDTH  1280
#define MYCAM_MAX_HEIGHT 720
#define MYCAM_BPP        2

struct mycam {
    struct v4l2_subdev sd;
    struct i2c_client *client;
    void *buffer;
    size_t buf_size;
    int streaming;
    int width;
    int height;
    struct mutex lock;
    struct v4l2_device v4l2_dev;
};

/* Aloca buffer baseado na resolução */
static int mycam_alloc_buffer(struct mycam *cam)
{
    mutex_lock(&cam->lock);
    cam->buf_size = cam->width * cam->height * MYCAM_BPP;
    cam->buffer = vmalloc(cam->buf_size);
    if (!cam->buffer) {
        mutex_unlock(&cam->lock);
        return -ENOMEM;
    }
    memset(cam->buffer, 0, cam->buf_size);
    mutex_unlock(&cam->lock);
    return 0;
}

static void mycam_free_buffer(struct mycam *cam)
{
    mutex_lock(&cam->lock);
    if (cam->buffer) {
        vfree(cam->buffer);
        cam->buffer = NULL;
    }
    mutex_unlock(&cam->lock);
}

/* Configura resolução */
static int mycam_set_format(struct mycam *cam, int width, int height)
{
    if (!cam)
        return -EINVAL;

    if (width > MYCAM_MAX_WIDTH) width = MYCAM_MAX_WIDTH;
    if (height > MYCAM_MAX_HEIGHT) height = MYCAM_MAX_HEIGHT;

    cam->width = width;
    cam->height = height;

    dev_info(&cam->client->dev, "mycamera format set: %dx%d\n", width, height);
    return 0;
}

/* Streaming ON/OFF */
static int mycam_s_stream(struct v4l2_subdev *sd, int enable)
{
    struct mycam *cam = container_of(sd, struct mycam, sd);
    int ret;

    mutex_lock(&cam->lock);
    if (enable) {
        ret = mycam_alloc_buffer(cam);
        if (ret < 0) {
            mutex_unlock(&cam->lock);
            return ret;
        }
        i2c_smbus_write_byte_data(cam->client, 0x02, 0x01); // ligar sensor
        cam->streaming = 1;
    } else {
        i2c_smbus_write_byte_data(cam->client, 0x02, 0x00); // desligar sensor
        mycam_free_buffer(cam);
        cam->streaming = 0;
    }
    mutex_unlock(&cam->lock);
    return 0;
}

/* mmap para userspace */
static int mycam_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mycam *cam = file->private_data;
    unsigned long pfn;

    if (!cam || !cam->buffer)
        return -EINVAL;

    pfn = vmalloc_to_pfn(cam->buffer);
    if (remap_pfn_range(vma, vma->vm_start, pfn,
                        cam->buf_size, vma->vm_page_prot))
        return -EAGAIN;

    return 0;
}

/* Read frame */
static ssize_t mycam_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    struct mycam *cam = file->private_data;
    size_t len;

    if (!cam->streaming)
        return -EINVAL;

    len = cam->buf_size;
    if (count < len)
        len = count;

    if (copy_to_user(user_buf, cam->buffer, len))
        return -EFAULT;

    return len;
}

/* Open/Release */
static int mycam_open(struct inode *inode, struct file *file)
{
    struct mycam *cam = container_of(inode->i_cdev, struct mycam, sd);
    file->private_data = cam;
    return 0;
}

static int mycam_release(struct inode *inode, struct file *file)
{
    return 0;
}

/* File operations */
static const struct file_operations mycam_fops = {
    .owner = THIS_MODULE,
    .open = mycam_open,
    .release = mycam_release,
    .read = mycam_read,
    .mmap = mycam_mmap,
};

/* Probe */
static int mycam_probe(struct i2c_client *client)
{
    struct mycam *cam;
    int ret;

    cam = devm_kzalloc(&client->dev, sizeof(*cam), GFP_KERNEL);
    if (!cam)
        return -ENOMEM;

    cam->client = client;
    cam->width = 640;
    cam->height = 480;
    mutex_init(&cam->lock);

    v4l2_i2c_subdev_init(&cam->sd, client, NULL);

    ret = i2c_smbus_write_byte_data(client, 0x01, 0x01); // power up
    if (ret < 0) {
        dev_err(&client->dev, "HARDWARE ERROR: CAM_NO_DETECTED_UNKNOWN_CAM\n");
        return ret;
    }

    dev_info(&client->dev, "mycamera ready: /dev/video0\n");
    return 0;
}

/* I2C driver table */
static const struct i2c_device_id mycam_id[] = {
    { "mycamera", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, mycam_id);

/* I2C driver */
static struct i2c_driver mycam_driver = {
    .driver = {
        .name = "mycamera",
    },
    .probe = mycam_probe,
    .id_table = mycam_id,
};

module_i2c_driver(mycam_driver);

MODULE_LICENSE("Apache-2.0");
MODULE_AUTHOR("Francenir Inc. / DroidPureC Kernel Inc.");
MODULE_DESCRIPTION("Driver próprio de câmera completo");
