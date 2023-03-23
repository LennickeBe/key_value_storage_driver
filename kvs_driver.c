#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

dev_t dev = MKDEV(12,3);
dev_t alloc_dev;
int ret; 

static int __init init_function(void)
{
    printk(KERN_ALERT "Hello\n");
    ret = register_chrdev_region(dev, 2, "kernel_io");
    printk(KERN_INFO "%i\n", ret);
    ret = alloc_chrdev_region(&alloc_dev, 3, 2, "kernel_io");
    printk(KERN_INFO "%i\n", ret);
    return 0;
}

static void __exit exit_function(void)
{
    unregister_chrdev_region(dev, 2);
    printk(KERN_ALERT "Bye\n");
}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
