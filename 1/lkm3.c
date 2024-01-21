#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/list.h>
#include <linux/mm_types.h>
#include<linux/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");

static int my_pid=1;
static unsigned long my_vaddr = 0x0;

module_param(my_pid,int,S_IRUGO);
MODULE_PARM_DESC(my_pid,"Pid of the given task ");

module_param(my_vaddr, ulong, S_IRUGO);
MODULE_PARM_DESC(my_vaddr, "Given Virtual address");


static int __init Virtual_addr_to_physical_addr(void){
    struct task_struct *task = get_pid_task(find_get_pid(my_pid), PIDTYPE_PID);
    struct mm_struct *mm;
    // struct vm_area_struct *vma;
    struct page *pg;
    pgd_t *pgd;
    p4d_t *p4d;
    pmd_t *pmd;
    pud_t *pud;
    pte_t *pte;

    unsigned long phy_addr;
    if(task == NULL){
        printk(KERN_INFO "Task not found with the given PID\n");
        return 0;
    }

    mm = task->mm;
    if(mm == NULL){
        printk(KERN_INFO "No Memory is Allocated\n");
        return 0;
    }

    pgd = pgd_offset(mm, my_vaddr);
    if (pgd_none(*pgd) || pgd_bad(*pgd)){
        printk(KERN_INFO " pgd error\n");
        return 0;
    }

    p4d = p4d_offset(pgd, my_vaddr);
    if (p4d_none(*p4d) || p4d_bad(*p4d)){
        printk(KERN_INFO " p4d error\n");
        return 0;
    }

    pud = pud_offset(p4d, my_vaddr);
    if (pud_none(*pud) || pud_bad(*pud)){
        printk(KERN_INFO " pud error\n");
        return 0;
    }

    pmd = pmd_offset(pud, my_vaddr);
    if (pmd_none(*pmd) || pmd_bad(*pmd)){
        printk(KERN_INFO " pud error\n");
        return 0;
    }

    pte = pte_offset_map(pmd, my_vaddr);
    if (pte_none(*pte)){
        printk(KERN_INFO " pte error\n");
        return 0;
    }

    pg = pte_page(*pte);

    if(pg==NULL){
        printk(KERN_INFO "Page not found\n");
        return 0;
    }

    phy_addr = page_to_phys(pg);
    

    printk(KERN_INFO "****____ Physical Address found for the given virtual adresss ____****\n");
    printk(KERN_INFO "Pid : %d\n V_addr : %lx \n Phy_add : %lx", my_pid, my_vaddr, phy_addr);
    return 0;
}

static void __exit lkm3_exit(void){
    printk(KERN_INFO "Good Bye LKM3 \n");

}

module_init(Virtual_addr_to_physical_addr);
module_exit(lkm3_exit);