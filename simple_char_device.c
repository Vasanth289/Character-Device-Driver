#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple character driver");

static DEFINE_MUTEX(char_mutex);

volatile static int is_open = 0;

static int devnum;
static char message[1024];
static int num_bytes = 0;
static int numberOpens = 0;

ssize_t device_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;
	
	printk(KERN_INFO "Inside the  %s function\n", __FUNCTION__);
	
	if (offset == NULL) return -EINVAL;
	
	if (*offset >= num_bytes) return 0;
	
	while ((bytes_read < length) && (*offset < num_bytes))
	{
		put_user(message[*offset], &buffer[bytes_read]);
		*offset += 1;
		bytes_read++;
	}
	
	return bytes_read;
}

ssize_t device_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	int bytes_write= 0;
	
	printk(KERN_INFO "Inside the  %s function\n", __FUNCTION__);
	
	if (offset == NULL) return -EINVAL;
	
	if (*offset >= 1023) return -EINVAL;
	
	while ((bytes_write < length) && (*offset < 1023))
	{
		get_user(message[*offset], &buffer[bytes_write]);
		*offset += 1;
		bytes_write++;
	}
	num_bytes = bytes_write;
	printk(KERN_INFO "Message: %s", message);
   	return bytes_write;
}

int device_open (struct inode *pinode, struct file *pfile)
{
	if(!mutex_trylock(&char_mutex))
	{
		printk(KERN_INFO "Device in use by another process\n");
		return -EBUSY;
	}
	printk(KERN_INFO "Inside the  %s function\n", __FUNCTION__);
	
	if(is_open == 1)
	{
		printk(KERN_INFO "Error - Device already open\n");
		return -EBUSY;
	}
	is_open = 1;
	try_module_get(THIS_MODULE);
	numberOpens++;	
	printk(KERN_INFO "Device has been opened %d time(s)\n", numberOpens);
	return 0;
}

int device_release (struct inode *pinode, struct file *pfile)
{
	mutex_unlock(&char_mutex);
	printk(KERN_INFO "Inside the  %s function\n", __FUNCTION__);
	
	if(is_open == 0)
	{
		printk(KERN_INFO "Error - Device wasn't open\n");
		return -EBUSY;
	}
	is_open = 0;
	module_put(THIS_MODULE);
	printk(KERN_INFO "Device successfully closed.\n");
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_release,
};

int __init simple_char_device_init(void) 
{
	printk(KERN_INFO "Inside the  %s function\n", __FUNCTION__);
	
	strncpy(message, "Hello!", 1023);
	num_bytes = strlen(message);
	
	devnum = register_chrdev(0, "simple_char", &fops);
	if (devnum < 0)
	{
		printk(KERN_ALERT "simple_char failed to register a major number");
		return devnum;
	}
	printk(KERN_INFO "simple_char has major number as %d\n", devnum);
	
	mutex_init(&char_mutex);
	
	return 0;
}

void simple_char_device_exit(void) 
{
	printk(KERN_INFO "Inside the %s function\n", __FUNCTION__);
	mutex_destroy(&char_mutex);
	printk(KERN_INFO "Goodbye!\n");
	unregister_chrdev(devnum, "simple_char");
}

module_init(simple_char_device_init);
module_exit(simple_char_device_exit);
