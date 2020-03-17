
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>


static struct class *basecode_class;
static struct class_device	*basecode_class_drv;

volatile unsigned long *GPFCON = NULL;
volatile unsigned long *GPFDAT = NULL;

volatile unsigned long *GPGCON = NULL;
volatile unsigned long *GPGDAT = NULL;

static DECLARE_WAIT_QUEUE_HEAD(wq);

unsigned char val = 0;
unsigned int event_val =0;


/*
 * EINT0(GPF0)   D10(GPF4)
 * EINT2(GPF2)   D11(GPF5)
 * EINT11(GPG3)  D12(GPF6)
 * EINT19(GPG11)  ALL	
 */
 struct pin_des{
	int pin;
	int key_val;
 };

 struct pin_des every_pin_des[4]={
 	{S3C2410_GPF0, 0X01},
 	{S3C2410_GPF2, 0X02},
 	{S3C2410_GPG3, 0X03},
 	{S3C2410_GPG11, 0X04},
 };

static irqreturn_t irq_deal_code(int irq, void *dev_id)
{
//	printk("irq =%d\n\r",irq);
	struct pin_des *key_p =(struct pin_des *)dev_id;
	int val_pin = s3c2410_gpio_getpin(key_p->pin);

	if(val_pin)  //松开
	{
		val = 0x80 | (key_p->key_val);
	}
	else
	{
		val = (key_p->key_val);
	}
	
	wake_up_interruptible(&wq);
	event_val =1;
	return 0;
}


static int base_text_open(struct inode *inode, struct file *file)
{
	request_irq(IRQ_EINT0, irq_deal_code, IRQ_TYPE_EDGE_BOTH, "key0", &every_pin_des[0]);
	request_irq(IRQ_EINT2, irq_deal_code, IRQ_TYPE_EDGE_BOTH, "key1", &every_pin_des[1]);
	request_irq(IRQ_EINT11, irq_deal_code, IRQ_TYPE_EDGE_BOTH, "key2", &every_pin_des[2]);	
	request_irq(IRQ_EINT19, irq_deal_code, IRQ_TYPE_EDGE_BOTH, "key3", &every_pin_des[3]);
	
	return 0;
}

static ssize_t base_text_read(struct file *file, char __user *buff, size_t n, loff_t *offp)
{
	if(n !=1)
	{
		return -EINVAL;
	}

//	wait_event_interruptible(wq, condition);
	
	copy_to_user(buff, &val, 1);
//	condition =0;
	
	return 1;
}

static int base_text_release(struct inode *inode, struct file *file)
{
	free_irq(IRQ_EINT0, &every_pin_des[0]);
	free_irq(IRQ_EINT2, &every_pin_des[1]);
	free_irq(IRQ_EINT11, &every_pin_des[2]);
	free_irq(IRQ_EINT19, &every_pin_des[3]);
	
	return 0;
}

static unsigned int base_text_poll(struct file *file,
				 struct poll_table_struct *wait)
{
	unsigned int mask =0;
	poll_wait(file, &wq, wait);

	if(event_val)
	{
		mask |= POLLIN | POLLRDNORM;
	}
	event_val =0;
	return mask;
}


static const struct file_operations base_text_fops = {
	.owner		= THIS_MODULE,
	.open		= base_text_open,
	.read       = base_text_read,
	.release    = base_text_release,
	.poll       = base_text_poll,
};

int major = 0;
static int base_text_init(void)
{
	major = register_chrdev(0, "cindy", &base_text_fops);
	
	basecode_class = class_create(THIS_MODULE, "cindyhome");

	basecode_class_drv = class_device_create(basecode_class, NULL, MKDEV(major, 0), NULL, "key");
		
	GPFCON =(volatile unsigned long )ioremap(0x56000050, 16);
	GPFDAT =(volatile unsigned long )(GPFCON + 1);

	GPGCON =(volatile unsigned long )ioremap(0x56000060, 16);
	GPGDAT =(volatile unsigned long )(GPGCON + 1);
	
	return 0;
}

void base_text_exit(void)
{
	major = unregister_chrdev(major, "cindy");	
	class_device_unregister(basecode_class_drv);
	class_destroy(basecode_class);
	iounmap(GPFCON);
	iounmap(GPGCON);
}

module_init(base_text_init);
module_exit(base_text_exit);
MODULE_LICENSE("GPL");


