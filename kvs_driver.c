#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/xarray.h>
#include "kvs_driver.h"
int ret; 
char *content;
struct xarray array;

int __init init_function(void)
{
    printk(KERN_ALERT "Hello\n");
    xa_init(&array);

    xa_store(&array, 1, "test", 0);
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
