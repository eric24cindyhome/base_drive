
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


static int base_text_open(struct inode *inode, struct file *file)
{
	printk("it's open!\n");
	return 0;
}

static ssize_t base_text_write(struct file *file, const char __user *buff, size_t count)
{
	printk("it's write\n");
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
	return 0;
}

void base_text_exit(void)
{
	major = unregister_chrdev(major, "cindy");
}

module_init(base_text_init);
module_exit(base_text_exit);
MODULE_LICENSE("GPL");


