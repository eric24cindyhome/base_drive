
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
static struct class_device	*basecode_class_drv[4];

static volatile unsigned long *GPFCON = NULL;
static volatile unsigned long *GPFDAT = NULL;


static int base_text_open(struct inode *inode, struct file *file)
{
//	printk("it's open!\n");
	int minor = MINOR(inode->i_rdev);

	switch(minor)
	{
		case 0:
			*GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
			*GPFCON |= ((1 << 8) | (1 << 10) | (1 << 12));	
			*GPFDAT |= (7 << 4);
			break;
			
		case 1:
			*GPFCON &= ~(3 << 8);
			*GPFCON |= (1 << 8);	
			*GPFDAT |= (1 << 4);
			break;
			
		case 2:
			*GPFCON &= ~(3 << 10);
			*GPFCON |= (1 << 10);	
			*GPFDAT |= (1 << 5);
			break;
			
		case 3:
			*GPFCON &= ~(3 << 12);
			*GPFCON |= (1 << 12);	
			*GPFDAT |= (1 << 6);
			break;

		default:
			break;
	}

	return 0;
}

static ssize_t base_text_write(struct file *file, const char __user *buff, size_t count)
{
//	printk("it's write\n");
	int minor = MINOR(file->f_dentry->d_inode->i_rdev);
	int val;

	copy_from_user(&val, buff, 4);

	switch(minor)
	{
		case 0:		
			if(val)
			{
				*GPFDAT &= ~(7 << 4);
			}
			else
			{
				*GPFDAT |= (7 << 4);
			}
			break;	
		
		case 1:						
			if(val)
			{
				*GPFDAT &= ~(1 << 4);
			}
			else
			{
				*GPFDAT |= (1 << 4);
			}
			break;
		
		case 2: 					
			if(val)
			{
				*GPFDAT &= ~(1 << 5);
			}
			else
			{
				*GPFDAT |= (1 << 5);
			}
			break;
		
		case 3: 					
			if(val)
			{
				*GPFDAT &= ~(1 << 6);
			}
			else
			{
				*GPFDAT |= (1 << 6);
			}
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
	int i =0;
	major = register_chrdev(0, "cindy", &base_text_fops);
	
	basecode_class = class_create(THIS_MODULE, "cindyhome");

	for(i =0; i <4; i++)
	{
		basecode_class_drv[i] = class_device_create(basecode_class, NULL, MKDEV(major, i), NULL, "led%d", i);
		
	}
	GPFCON =(volatile unsigned long )ioremap(0x56000050, 16);
	GPFDAT =(volatile unsigned long )(GPFCON + 1);
	
	return 0;
}

void base_text_exit(void)
{
	int i;
	
	major = unregister_chrdev(major, "cindy");	
	for(i =0; i <4; i++)
	{
		class_device_unregister(basecode_class_drv[i]);
	}
	class_destroy(basecode_class);
	iounmap(GPFCON);
}

module_init(base_text_init);
module_exit(base_text_exit);
MODULE_LICENSE("GPL");


