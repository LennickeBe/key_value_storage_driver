#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/xarray.h>
#include <linux/mm.h>

#include "kvs_driver.h"

struct xarray array;
dev_t dev;
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


int __init init_function(void)
{
    int ret; 

    printk(KERN_ALERT "Hello\n");
    if ((ret = alloc_chrdev_region(&dev, DEV_MAJOR, DEV_MINOR, "kvs_ioctl")))
    {
	    return ret;
    }
    return 0;
}

void __exit exit_function(void)
{
    unregister_chrdev_region(dev, DEV_MINOR);
    printk(KERN_ALERT "Bye\n");
}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
