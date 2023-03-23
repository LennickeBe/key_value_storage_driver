#include <linux/init.h>
#include <linux/module.h>

static int __init init_function(void)
{
    printk(KERN_ALERT "Hello");
    return 0;
}

static void __exit exit_function(void)
{
    printk(KERN_ALERT "Bye");
}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
