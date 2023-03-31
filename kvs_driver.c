#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/xarray.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "kvs_driver.h"

DEFINE_XARRAY(array);
dev_t dev;
struct cdev c_dev;
struct class *cl;
struct device *dev_ret;
/*
 * Change the value at index 'key' in given xarray.
 */
void * _change_entry(struct xarray *array, int key, char *value)
{
	char * value_kernel_space;
	value_kernel_space = (char *) kmalloc(100 * sizeof(char), GFP_KERNEL);
	strncpy(value_kernel_space, value, 100 * sizeof(char));
	return xa_store(array, key, value_kernel_space, GFP_KERNEL);
}


/*
 * Sets entry at index 'key' in the given xarray to NULL.
 */
void * _remove_entry(struct xarray *array, int key)
{
	return xa_erase(array, key);
}


/*
 * Return the value at index 'key' in the given xarray.
 * Returns "(null)" for empty entries.
 */
char * _show_entry(struct xarray *array, int key)
{
	char * content;
	if ((content = (char *) xa_load(array, key)))
	{
		return content;
	}
	else
	{
		return "(null)";
	}
}

/*
 * Remove all entries from the given xarray.
 */
void _clr_array(struct xarray *array)
{
	xa_destroy(array);
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
			printk(KERN_INFO "Changed:\t%i: %s\n", io_arg.key, io_arg.value);
			break;

		case KVS_REMOVE_VAL:
			if (copy_from_user(&io_arg, (ioctl_arg*)arg, sizeof(ioctl_arg)))
			{
				return -EACCES;
			}
			_remove_entry(&array, io_arg.key);
			printk(KERN_INFO "Removed:\t%i\n", io_arg.key);
			break;
		case KVS_SHOW_VAL:
			if (copy_from_user(&io_arg, (ioctl_arg*)arg, sizeof(ioctl_arg)))
			{
				return -EACCES;
			}
			strncpy(io_arg.value,_show_entry(&array, io_arg.key), 100 * sizeof(char));
			printk(KERN_INFO "Showed:\t%i: %s\n", io_arg.key, io_arg.value);
			if (copy_to_user((ioctl_arg*)arg, &io_arg,  sizeof(ioctl_arg)))
			{
				return -EACCES;
			}
			break;
		case KVS_CLR_ARR:
			_clr_array(&array);
			printk(KERN_INFO "Cleared");
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
	    goto fail_cdev;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
	    goto fail_class;
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "kvs")))
    {
	    goto fail_device;
    }

    return 0;

fail_device: class_destroy(cl);
fail_class: cdev_del(&c_dev);
	    ret = PTR_ERR(cl);
fail_cdev: unregister_chrdev_region(dev, DEV_COUNT);
	   return ret;
}

void __exit exit_function(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, DEV_COUNT);
    printk(KERN_ALERT "Bye\n");
}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
