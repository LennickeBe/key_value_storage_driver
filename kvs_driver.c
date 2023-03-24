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

int _add_entry(struct xarray *array, int key, char *value)
{
	if ( (errno=xa_err(
		xa_cmpxchg(array, key, NULL, value, GFP_DMA)
		         )
	   ) )
	{
		return errno;
	}
	else
	{
		return 0;
	}
}



int __init init_function(void)
{
    printk(KERN_ALERT "Hello\n");
    xa_init(&array);
    _add_entry(&array, 1, "test"); 
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
