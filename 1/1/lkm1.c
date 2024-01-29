#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shantanu Mapari");

static int __init printRunnableRunningTasks(void) {
    struct task_struct *task;
    printk(KERN_INFO "Listing all the tasks which are in running or runnable state:\n");

    for_each_process(task) {
        
        if(task->__state == TASK_RUNNING )
        {
            printk("********* ________TASK INFO________ **********\n");
            printk(KERN_INFO "Name of the Task : %s , PID of the task %d \n", task->comm,task->pid);
        }
    }
    return 0;
}

static void __exit helloworld_exit(void) {
    printk(KERN_INFO "Module Unloaded\n");
}

module_init(printRunnableRunningTasks);
module_exit(helloworld_exit);
