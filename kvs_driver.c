#include <linux/init.h>
#include <linux/module.h>

static int __init init(void)
{
    printk(KERN_ALERT "Hello");
    return 0;
}

static void __exit exit(void)
{
    printk(KERN_ALERT "Bye");
}

module_init(init);
module_exit(exit);