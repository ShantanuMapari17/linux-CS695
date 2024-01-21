#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");

static int __init helloworld_init(void) {
    printk(KERN_INFO "Hello World! Module Loaded\n");
    return 0;
}

static void __exit helloworld_exit(void) {
    printk(KERN_INFO "Module Unloaded\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);