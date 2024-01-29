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

static int my_pid = 1;
module_param(my_pid,int,S_IRUGO);
MODULE_PARM_DESC(my_pid,"Pid of the given task ");

/*
static bool isValid(struct mm_struct *mm, unsigned long my_vaddr){
    struct page *pg;
    pgd_t *pgd;
    p4d_t *p4d;
    pmd_t *pmd;
    pud_t *pud;
    pte_t *pte;

    unsigned long phy_addr=0;
    if(mm == NULL){
        // printk(KERN_INFO "No Memory is Allocated\n");
        return 0;
    }

    pgd = pgd_offset(mm, my_vaddr);
    if (pgd_none(*pgd) || pgd_bad(*pgd)){
        // printk(KERN_INFO " pgd error\n");
        return 0;
    }

    p4d = p4d_offset(pgd, my_vaddr);
    if (p4d_none(*p4d) || p4d_bad(*p4d)){
        // printk(KERN_INFO " p4d error\n");
        return 0;
    }

    pud = pud_offset(p4d, my_vaddr);
    if (pud_none(*pud) || pud_bad(*pud)){
        // printk(KERN_INFO " pud error\n");
        return 0;
    }

    pmd = pmd_offset(pud, my_vaddr);
    if (pmd_none(*pmd) || pmd_bad(*pmd)){
        // printk(KERN_INFO " pud error\n");
        return 0;
    }

    pte = pte_offset_map(pmd, my_vaddr);
    if (pte_none(*pte)){
        // printk(KERN_INFO " pte error\n");
        return 0;
    }

    pg = pte_page(*pte);

    if(pg==NULL){
        // printk(KERN_INFO "Page not found\n");
        return 0;
    }

    phy_addr = page_to_phys(pg);
    return phy_addr;
}
*/

static int __init calculateTotalVM(void){
    struct task_struct *task = get_pid_task(find_get_pid(my_pid), PIDTYPE_PID);
    struct mm_struct *mm;
    unsigned long totalVadd=0;
    struct vm_area_struct *vma;
    unsigned long vaddr = 0;
    unsigned long total_phy=0;
    printk("-----***** LKM4 loaded ****----");
    if(!task){
        printk("Task not Found\n");
        return 0;
    }
    
    mm=task->mm;

    // struct vma_iterator *vmi;
    
    VMA_ITERATOR(vmi, mm, 0);

    // vma_iter_init(vmi,mm,0);
    for_each_vma(vmi,vma){
        totalVadd += (vma->vm_end - vma->vm_start);
        // for(vaddr=vma->vm_start; vaddr<vma->vm_end ; vaddr+=PAGE_SIZE){
        //     if(isValid(mm,vaddr)){
        //         total_phy+=PAGE_SIZE;
        //     }
        // }
    }

    total_phy = get_mm_rss(mm);

    printk("Total virtual mem : %luKb\n", totalVadd/1024);
    printk("Total physical mem: %luKb\n", (total_phy<<PAGE_SHIFT)/1024);
    // printk("Debo's virtual mem: %luKb\n", (mm->total_vm << PAGE_SHIFT) / 1024);
    printk("----**** LKM4 end ****----");
    return 0;

}

static void __exit lkm4_exit(void){
    printk(KERN_INFO "Good Bye LKM4 \n");

}

module_init(calculateTotalVM);
module_exit(lkm4_exit);
