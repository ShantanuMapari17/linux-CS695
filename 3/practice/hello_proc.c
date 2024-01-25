#include <linux/init.h> // module_init, module_exit
#include <linux/module.h> // MODULE_*, module_*
#include <linux/proc_fs.h> // proc_create, proc_remove
#include <linux/slab.h> // kmalloc, kfree
#include <linux/string.h> // strlen
#include <linux/fs.h> // struct file_operations
#include <linux/uaccess.h> // copy_from_user, copy_to_user

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");
MODULE_DESCRIPTION("Simple module featuring proc read");

#define ENTRY_NAME "hello_world" // proc entry name
#define PERMS 0644 // proc entry permissions
#define PARENT NULL // proc entry parent dir


static char *message;
static int read_p;

int hello_proc_open(struct inode *sp_inode, struct file *sp_file) {
    printk("proc called open\n");

    read_p = 1;
    message = kmalloc(sizeof(char) * 20, GFP_KERNEL);

    if (message == NULL) {
        printk("ERROR, hello_proc_open");
        return -ENOMEM;
    }
    strlcpy(message, "Hello, World!\n", 20);
    return 0;
}

ssize_t hello_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset) {
    int len = strlen(message);

    read_p = !read_p;
    if (read_p) {
        return 0;
    }

    printk("proc called read\n");
    copy_to_user(buf, message, len);
    return len;
}

int hello_proc_release(struct inode *sp_inode, struct file *sp_file) {
    printk("proc called release\n");
    kfree(message);
    return 0;
}

static const struct proc_ops fops = {
    .proc_open = hello_proc_open,
    .proc_read = hello_proc_read,
    .proc_release = hello_proc_release,
};

static int hello_init(void) {
    printk("----------- PROC DEMO BEGIN -----------\n");
    printk("/proc/%s create\n", ENTRY_NAME);

    if (!proc_create(ENTRY_NAME, PERMS, NULL, &fops)) {
        printk("ERROR! proc_create\n");
        remove_proc_entry(ENTRY_NAME, NULL);
        return -ENOMEM;
    }

    return 0;
}

static void hello_exit(void) {
    remove_proc_entry(ENTRY_NAME, NULL);
    printk("Removing /proc/%s.\n", ENTRY_NAME);

    printk("----------- PROC DEMO END -----------\n");
}

module_init(hello_init);
module_exit(hello_exit);
