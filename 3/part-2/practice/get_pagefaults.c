/*Kernel module to find page_faults in the system and listing in /proc/get_pagefaults */

#include <linux/init.h> // module_init, module_exit
#include <linux/module.h> // MODULE_*, module_*
#include <linux/proc_fs.h> // proc_create, proc_remove
#include <linux/mm.h>   // all_vm_events
#include <linux/slab.h> // kmalloc, kfree
#include <linux/string.h> // strlen
#include <linux/fs.h> // struct file_operations
#include <linux/uaccess.h>
#include <linux/vmstat.h> // copy_from_user, copy_to_user/
#include <linux/vm_event_item.h>    // vm_event_states


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");

#define ENTRY_NAME "get_pagefaults"
#define PERMS 0644
#define PARENT NULL
#define MAX_LEN 1024

static unsigned long my_events[NR_VM_EVENT_ITEMS];
static unsigned long pg_faults;
static char* message;
static int read_p;

static int my_proc_open(struct inode *sp_inode, struct file *sp_file){
    printk(KERN_INFO "[info]: proc called open\n");

    read_p = 1;
    message = kmalloc(sizeof(char) * MAX_LEN, GFP_KERNEL);
    if(message == NULL){
        printk(KERN_WARNING "hello_procfs: open: message is null\n");
        return -ENOMEM;
    }

    all_vm_events(my_events);
    pg_faults = my_events[PGFAULT];
    sprintf(message, "Page Faults: %ld\n", pg_faults);    

    return 0;
}

static ssize_t my_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset){
    int len = strlen(message);
    printk(KERN_INFO "[info]: proc called read\n");
    read_p = !read_p;
    if(read_p){
        return 0;
    }
    copy_to_user(buf, message, len);
    return len;
}

static int my_proc_release(struct inode *sp_inode, struct file *sp_file){
    printk(KERN_INFO "[info]: proc called release\n");
    kfree(message);
    return 0;
}

/*definig my_proc_fops*/
static const struct proc_ops my_proc_fops = {
    .proc_open = my_proc_open,
    .proc_read = my_proc_read,
    .proc_release = my_proc_release,
};

static int __init my_procfs_init(void){
    printk("------- PAGE FAULTS INIT -------\n");
    printk(KERN_INFO "[info]: procfs_init to get pg faults\n");
    struct proc_dir_entry *entry;
    entry = proc_create(ENTRY_NAME, PERMS, NULL, &my_proc_fops);
    if(entry == NULL){
        printk(KERN_WARNING "[warning]: procfs_init: proc_create failed\n");
        return -ENOMEM;
    }
    return 0;
}

static void __exit my_procfs_exit(void){
    printk(KERN_INFO "[info]: procfs_exit\n");
    remove_proc_entry(ENTRY_NAME, NULL);

    printk("------- PAGE FAULTS EXIT -------\n");
}

module_init(my_procfs_init);
module_exit(my_procfs_exit);