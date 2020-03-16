
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
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

/*
 * EINT0(GPF0)   D10(GPF4)
 * EINT2(GPF2)   D11(GPF5)
 * EINT11(GPG3)  D12(GPF6)
 * EINT19(GPG11)  ALL	
 */

static int base_text_open(struct inode *inode, struct file *file)
{
//	*GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
//	*GPFCON |= ((1 << 8) | (1 << 10) | (1 << 12));	
//	*GPFDAT |= (7 << 4);
	*GPFCON &= ~((0x3 << 0) | (0x3 << 4));
	*GPGCON &= ~((0x3 << 6) | (0x3 << 22));

	return 0;
}

static ssize_t base_text_read(struct file *file, char __user *buff, size_t n, loff_t *offp)
{
	unsigned char key[4];

	if(n !=sizeof(key))
	{
		return -EINVAL;
	}
	
	key[0] = ((*GPFDAT) & (1 << 0))? 1 : 0;
	key[1] = ((*GPFDAT) & (1 << 2))? 1 : 0;
	
	key[2] = ((*GPGDAT) & (1 << 3))? 1 : 0;
	key[3] = ((*GPGDAT) & (1 << 11))? 1 : 0;

	copy_to_user(buff, key, sizeof(key));
	
	return sizeof(key);
}

static const struct file_operations base_text_fops = {
	.owner		= THIS_MODULE,
	.open		= base_text_open,
	.read       = base_text_read,
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


