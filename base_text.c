
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

static volatile unsigned long *GPFCON = NULL;
static volatile unsigned long *GPFDAT = NULL;


static int base_text_open(struct inode *inode, struct file *file)
{
//	printk("it's open!\n");
	
	*GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
	*GPFCON |= ((1 << 8) | (1 << 10) | (1 << 12));	
	*GPFDAT |= (7 << 4);

	return 0;
}

static ssize_t base_text_write(struct file *file, const char __user *buff, size_t count)
{
//	printk("it's write\n");
	int val;

	copy_from_user(&val, buff, 4);

	switch(val)
	{
		case 1:			
			*GPFDAT &= ~(7 << 4);
			break;
		
		case 2:			
			*GPFDAT |= (7 << 4);
			break;
		
		default:
			break;
	}
	return 0;
}

static const struct file_operations base_text_fops = {
	.owner		= THIS_MODULE,
	.open		= base_text_open,
	.write      = base_text_write,
};

int major = 0;
static int base_text_init(void)
{
	major = register_chrdev(0, "cindy", &base_text_fops);
	
	basecode_class = class_create(THIS_MODULE, "cindyhome");
	basecode_class_drv = class_device_create(basecode_class, NULL, MKDEV(major, 0), NULL, "led");

	GPFCON =(volatile unsigned long )ioremap(0x56000050, 16);
	GPFDAT =(volatile unsigned long )(GPFCON + 1);
	
	return 0;
}

void base_text_exit(void)
{
	major = unregister_chrdev(major, "cindy");	
	class_device_unregister(basecode_class_drv);
	class_destroy(basecode_class);
	iounmap(GPFCON);
}

module_init(base_text_init);
module_exit(base_text_exit);
MODULE_LICENSE("GPL");


