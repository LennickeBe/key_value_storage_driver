#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/xarray.h>
#include <linux/mm.h>
#include "kvs_driver.h"
int ret; 
char *content;
struct xarray array;
int errno;


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
    printk(KERN_ALERT "Hello\n");
    xa_init(&array);
    
    _add_entry(&array, 1, "test"); 
    content = xa_load(&array, 1);
    printk(KERN_INFO "content %s\n", content);

    ret = _add_entry(&array, 1, "test");
    printk(KERN_INFO "ret %i\n", ret);

    content = _change_entry(&array, 1, "whack");
    printk(KERN_INFO "content %s\n", content);
    content = xa_load(&array, 1);
    printk(KERN_INFO "content %s\n", content);



    _remove_entry(&array, 1);
    content = xa_load(&array, 1);
    printk(KERN_INFO "content %s\n", content);


    return 0;
}

void __exit exit_function(void)
{
    printk(KERN_ALERT "Bye\n");
}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
