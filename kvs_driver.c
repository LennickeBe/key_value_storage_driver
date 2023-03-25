#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/xarray.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "kvs_driver.h"

struct xarray array;
dev_t dev;
struct cdev c_dev;
struct class *cl;
struct device *dev_ret;
/*
 * Change the value at index 'key' in given xarray.
 */
void * _change_entry(struct xarray *array, int key, char *value)
{
	return xa_store(array, key, value, GFP_DMA);
}


/*
 * Sets entry at index 'key' in the given xarray to NULL.
 */
void * _remove_entry(struct xarray *array, int key)
{
	return xa_erase(array, key);
}


/*
 * Adds the value at index 'key' to the given xarray
 * if there is no value present at that index.
 */
int _add_entry(struct xarray *array, int key, char *value)
{
	return xa_insert(array, key, value, GFP_DMA);
}


long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	ioctl_arg io_arg;

	switch(cmd)
	{
		case KVS_CHANGE_VAL:
			if (copy_from_user(&io_arg, (ioctl_arg*)arg, sizeof(ioctl_arg)))
			{
				return -EACCES;
			}
			_change_entry(&array, io_arg.key, io_arg.value);
			break;
		default:
			return -EINVAL;
	}
	
	return 0;
}

struct file_operations ioctl_fops =
{
	.owner = THIS_MODULE,
	.unlocked_ioctl = my_ioctl
};


int __init init_function(void)
{
    int ret;

    printk(KERN_ALERT "Hello\n");
    if ((ret = alloc_chrdev_region(&dev, DEV_MAJOR, DEV_COUNT, "kvs_ioctl")))
    {
	    return ret;
    }

    cdev_init(&c_dev, &ioctl_fops);

    if ((ret = cdev_add(&c_dev, dev, DEV_COUNT)))
    {
	    return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
	    cdev_del(&c_dev);
	    unregister_chrdev_region(dev, DEV_COUNT);
	    return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "kvs")))
    {
	    class_destroy(cl);
	    cdev_del(&c_dev);
	    unregister_chrdev_region(dev, DEV_COUNT);
	    return PTR_ERR(cl);
    }

    return 0;
}

void __exit exit_function(void)
{
    char *content;
    content = xa_load(&array, 1);
    printk(KERN_INFO "%s\n", content);

    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, DEV_COUNT);
    printk(KERN_ALERT "Bye\n");
}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
