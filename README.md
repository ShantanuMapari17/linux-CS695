# 1. Hello LKM!
[Assignment link](https://docs.google.com/document/u/1/d/e/2PACX-1vQ5u_tBmDgtbHPr4wtyCt-2HWnreknpARu4ycR5gtFjRJqAxqwYBuWXfxldCNQNsFABBqAFT0qBMgZj/pub)

- Note: Please update the Makefile to build necessary kernel module

### 1.1 LKM-1
Write a kernel module to list all processes in a running or runnable state. Print their pid. Name the source file as lkm1.c
Running instructions
- `make`
- `sudo insmod lkm1.ko`
- `sudo rmmod lkm1`
- `sudo dmesg | tail -10`

### OUTPUT

```
[77614.647020] Listing all the tasks which are in running or runnable state:
[77614.647093] ********* ________TASK INFO________ **********
[77614.647094] Name of the Task : insmod , PID of the task 9467 
[77626.879611] ------- LKM1 Module Unloaded ----------
```



### 1.2 LKM2
Write a kernel module that takes process ID as input, and for each of its child processes, print their pid and process state. Name the source file as lkm2.c

- For creating child processes we have **create-child.c**
- Compiling instructions
  - `gcc -o create-child create-child.c `
  - `./create-child`
- load the kernel module by copying the pid from the output of create-child
    - `sudo insmod lkm2.ko my_pid=$$`
    - `sudo rmmod lkm2`
    - `sudo dmesg | tail -10`

- **OUTPUT**

```
[78698.736601] Name of the Task: create-child, PID of the task 10523 
[78698.736619] ****_______ Printing the children of the task _______****
[78698.736619] Child Name: create-child Child PID: 10524, 
[78698.736620]  State : TASK_INTERRUPTIBLE 1
[78698.736621] Child Name: create-child Child PID: 10525, 
[78698.736621]  State : TASK_INTERRUPTIBLE 1
[78698.736622] Child Name: create-child Child PID: 10526, 
[78698.736622]  State : TASK_INTERRUPTIBLE 1
[78698.736622] Child Name: create-child Child PID: 10527, 
[78698.736623]  State : TASK_INTERRUPTIBLE 1
[78705.531353] Good Bye LKM2 

```

### 1.3 LKM-3

Write a kernel module that takes process ID and a virtual address as its input. Determine if the virtual address is mapped, and if so, determine its physical address (pseudo physical address if running in a VM). Print the pid, virtual address, and corresponding physical address. Name the source file as lkm3.c

- We have to compile memory-demo.c then give pid and vaddr as command line args
    - `gcc -o memory memory-demo.c`
    - `./memory`
- now load the kernel module 
    - `sudo insmod lkm3.ko my_pid=$$ my_vaddr=$$`
    - `sudo rmmod lkm3`
    - `sudo dmesg | tail -10`
- OUTPUT

```
[79554.440487] ****____ Physical Address found for the given virtual adresss ____****
[79554.440502] Pid : 11912
                V_addr : 0xaaaae59966b0 
                Phy_add : 0x5f4116b0
[79566.985570] Good Bye LKM3
```

### 1.4 lkm4
For a specified process (via its pid), determine the size of the allocated virtual address space (sum of all vmas) and the mapped physical address space. Write a test program that allocates different sizes of memory in stages, and in each stage, observe the mapped memory stats using your LKM. Make sure that you allocate memory in the granularity of page size. Name the source file as lkm4.c

- Run previous program to find its pid
    - `./memory`
- copy the pid and give as command line arg to lkm4
    - `sudo insmod lkm4.ko my_pid=$$`

- OUTPUT
```
[80132.736298] -----***** LKM4 loaded ****----
[80132.736310] Total virtual mem : 2188Kb
[80132.736312] Total physical mem: 740Kb
[80132.736313] ----**** LKM4 end ****----
[80139.831530] Good Bye LKM4 
```


## Q2
### 2_I part-1
 - just run the script 
 - `sudo ./spock.sh`
 - `sudo dmesg | tail -10`

- app-test output
```
[info]: Loading spock program
[info]: spock program loaded
[info]: Testing spock program
ptr = 0xaaaaf75a72a0
Value = 5
Device file opened...
IOCTL-getting physical adress success...
IOCTL-writing to physical address success...
Value written to physical address is 6
[info]: Unloading spock program
```

- dmesg output
```
[80684.425801] ------------ SPOCK DRIVER ------------
[80684.425813] spock_ioctl driver registered
[80684.426300] VIRTUAL_TO_PHYSICAL
[80684.426301] v_addr = aaaaf75a72a0
[80684.426301] p_addr = 435ca2a0
[80684.426301] k_vadrr = ffff3ae3035ca2a0
[80684.426322] WRITE_TO_PHYSICAL
[80684.426322] val = 6
[80684.426322] Value written to physical address is 6
[80684.427532] spock_ioctl driver unregistered

[80684.427533] --------------------------------------
```

### 2_II dr doom
- Run the script
    - `sudo ./run_dr_doom.sh`
    - `sudo dmesg | tail -10`

- Script OUTPUT
```Control station PID: 17901
Soldier PID: 17902
Parent of current process changed 
Control station process 17901 started
./run_dr_doom.sh: line 26: 17901 Killed                  ./control_station
```

- dmesg output
```

[81034.032572] ------------ DR DOOM DRIVER ------------
[81034.032589] DR_DOOM_ioctl driver registered
[81034.033722] Changing parent of process 17902 to 17901
[81034.033726] Parent of process 17902 changed to 17901
[81036.043559] Dr_DOOM_ioctl driver unregistered
[81036.043573] --------------------------------------
```


## Q3 proc dir
Use `make` to compile the modules
## 3.1 HelloProc
- `sudo insmod hello_procfs.ko`
- `sudo rmmod hello_procfs`
- `sudo dmesg | tail -10`

## 3.2 
- `sudo insmod get_pagefaults.ko`
- `sudo rmmod get_pagefauls`
- `sudo dmesg | tail -10`
