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


#include "spock-driver.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
// static int status = 1, dignity = 3, ego = 5;
static unsigned long v_addr=0;
static unsigned long p_addr=0;
static unsigned long k_vadrr=0;
static struct page* pg;

/*function to get physical address from v_addr and mm_struct pointer*/
static unsigned long getPhysical(unsigned long v_addr, struct mm_struct *mm)
{
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep, pte;
    unsigned long p_addr = 0;
    struct page *page;
 
    pgd = pgd_offset(mm, v_addr);
    if(pgd_none(*pgd) || pgd_bad(*pgd))
    {
        printk(KERN_INFO "not mapped in pgd\n");
        return 0;
    }

    p4d = p4d_offset(pgd, v_addr);

    if(p4d_none(*p4d) || p4d_bad(*p4d))
    {
        printk(KERN_INFO "not mapped in p4d\n");
        return 0;
    }

    pud = pud_offset(p4d, v_addr);

    if(pud_none(*pud) || pud_bad(*pud))
    {
        printk(KERN_INFO "not mapped in pud\n");
        return 0;
    }

    pmd = pmd_offset(pud, v_addr);

    if(pmd_none(*pmd) || pmd_bad(*pmd))
    {
        printk(KERN_INFO "not mapped in pmd\n");
        return 0;
    }

    pte = *pte_offset_map(pmd, v_addr);
    if(!pte_present(pte))
    {
        printk(KERN_INFO "not mapped in pte\n");
        return 0;
    }

    /*page from pte*/
    page = pte_page(pte);
    pg=page;
    /*pte to physical address*/
    p_addr = pte_pfn(pte) << PAGE_SHIFT;
    /*add offset within page*/
    p_addr |= (v_addr & ~PAGE_MASK);

    return p_addr;


}

/*function to get kernel virtual address from physical address */
static unsigned long getKVirt(unsigned long p_addr)
{
    return (unsigned long)phys_to_virt(p_addr);
}
 
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
    int val;
 
    switch (cmd)
    {
        case VIRTUAL_TO_PHYSICAL:
            printk(KERN_INFO "VIRTUAL_TO_PHYSICAL\n");
            if(copy_from_user(&v_addr, (long *)arg, sizeof(long)))
                return -EACCES;
            printk(KERN_INFO "v_addr = %lx\n", v_addr);
            p_addr = getPhysical(v_addr,current->mm);
            printk(KERN_INFO "p_addr = %lx\n", p_addr);
            k_vadrr = getKVirt(p_addr);
            printk(KERN_INFO "k_vadrr = %lx\n", k_vadrr);
            break;

        case WRITE_TO_PHYSICAL:
            printk(KERN_INFO "WRITE_TO_PHYSICAL\n");
            if(copy_from_user(&val, (int *)arg, sizeof(int)))
                return -EACCES;
            printk(KERN_INFO "val = %d\n", val);
            *(int *)k_vadrr = val;
            
            printk(KERN_INFO "Value written to physical address is %d\n", *(int *)k_vadrr);
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
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "spock_ioctl")) < 0)
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
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "my_spock")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    printk("------------ SPOCK DRIVER ------------\n");
    printk(KERN_INFO "spock_ioctl driver registered\n");
 
    return 0;
}
 
static void __exit spock_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    printk(KERN_INFO "spock_ioctl driver unregistered\n\n");
    printk("--------------------------------------\n");
}
 
module_init(spock_ioctl_init);
module_exit(spock_ioctl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");
MODULE_DESCRIPTION("SPOCK ioctl() Char Driver");