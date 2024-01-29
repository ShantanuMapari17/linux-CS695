#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");

static int my_pid=1;
module_param(my_pid,int,S_IRUGO);
MODULE_PARM_DESC(my_pid,"Pid of the given task ");

static int getInfoByPid(void) {

    struct task_struct *task = get_pid_task(find_get_pid(my_pid), PIDTYPE_PID); // get the task_struct of the given pid
    struct task_struct *child_task; // child task struct
    struct list_head *list;

    if (task) {
        printk(KERN_INFO "Name of the Task: %s, PID of the task %d \n", task->comm, task->pid);
        printk(KERN_INFO "****_______ Printing the children of the task _______****");       

        // iterate through the children of the task
        list_for_each(list, &task->children) {
            child_task = list_entry( list ,struct task_struct, sibling);
            printk(KERN_INFO "Child Name: %s Child PID: %d, ", child_task->comm, child_task->pid);
            if(task->__state == TASK_RUNNING){
                printk(KERN_INFO " State : TASK_RUNNING %d\n",task->__state);
            }
            else if(task->__state == TASK_INTERRUPTIBLE){
                printk(KERN_INFO " State : TASK_INTERRUPTIBLE %d\n",task->__state);
            }
            else if(task->__state == TASK_UNINTERRUPTIBLE){
                printk(KERN_INFO " State : TASK_UNINTERRUPTIBLE %d\n",task->__state);
            }
            else{
                printk(KERN_INFO " NOT RUNNING\n");
            }
        }
    } else {
        printk(KERN_INFO "Task not found with pid %d. Returned back!! \n", my_pid);
        return 0;
    }

    return 0;
}


static void __exit lkm2_exit(void){
    pr_info("Good Bye LKM2 \n");

}

module_init(getInfoByPid);
module_exit(lkm2_exit);
