/*ioctl driver program to change the parent of the current process, provided the pid of parent*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>

#include "dr_doom_driver.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;


static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    int parent_pid;
 
    switch (cmd)
    {
        case CHANGE_PARENT_TO_PID:
            if(copy_from_user(&parent_pid, (int *)arg, sizeof(int)))
            {
                return -EACCES;
            }
            printk(KERN_INFO "Changing parent of process %u to %d\n", current->pid, parent_pid);
            if (current->pid == 1) {
                printk(KERN_INFO "Cannot change parent of init process\n");
                return -EINVAL;
            }

            struct task_struct *parent_task = pid_task(find_vpid(parent_pid), PIDTYPE_PID);
            if (parent_task == NULL) {
                printk(KERN_INFO "No process with pid %d\n", parent_pid);
                return -EINVAL;
            }

            current->real_parent = parent_task;
            current->parent = parent_task;

            list_del(&(current->sibling));
            list_add(&(current->sibling), parent_task->children.next);


            printk(KERN_INFO "Parent of process %u changed to %d\n", current->pid, parent_pid);
            break;
        default:
            return -EINVAL;
    }
 
    return 0;
}

 
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
};


static int __init spock_ioctl_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "dr_doom_ioctl")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "dr_doom")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    printk("------------ DR DOOM DRIVER ------------\n");
    printk(KERN_INFO "DR_DOOM_ioctl driver registered\n");
 
    return 0;
}
 
static void __exit spock_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    printk(KERN_INFO "Dr_DOOM_ioctl driver unregistered\n\n");
    printk("--------------------------------------\n");
}
 
module_init(spock_ioctl_init);
module_exit(spock_ioctl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");
MODULE_DESCRIPTION("dr_doom ioctl() Char Driver");