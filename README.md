# LotteryScheduler-FreeBSD
Implementing a FreeBSD lottery scheduler.
This basic repository modifies the FreeBSD scheduler to use lottery scheduling rather than the current scheduler. Adds a new system call `gift(int,int)` to give tickets from  process to another.

## How to change to the lottery scheduler

- Clone or download this repository on a FreeBSD system.
- Change directory using: `cd /usr/src/sys/kern/`
- remove the already existing `sched_ule.c`, `kern_switch.c`, `kern_resource.c` using this command: `rm sched_ule.c kern_resource.c kern_switch.c`
- Change directory using: `cd /usr/src/sys/sys/`
- remove the already existing `proc.h`, `runq.h` using this command: `rm runq.h proc.h`
- Navigate to the cloned directory and open it on terminal.
- copy new files into the same locations: `cp sched_ule.c kern_resource.c kern_switch.c /usr/src/sys/kern/` & `cp runq.h proc.h /usr/src/sys/sys/`

## How to add the `gift(pid, t)` system call

- `rm /usr/src/sys/kern`
- Navigate to the cloned directory and open it on terminal.
- `cp syscalls.master sys_gift.c /usr/src/sys/kern/`
- `cd /usr/src/sys/kern`
- `make sysent`
- `rm /usr/src/lib/libc/sys/Symbol.map`
- Navigate to the cloned directory and open it on terminal.
- `cp Symbol.map /usr/src/lib/libc/sys/`
- `rm /sys/conf/files`
- Navigate to the cloned directory and open it on terminal.
- `cp files /sys/conf/`
- build & install kernel, then reboot.


How does this scheduler work?
-------------------------
A lottery scheduler assigns each process’s threads some number of tickets, then randomly draws a ticket among those allocated to “ready” threads to decide which one to run. That thread is then allowed to run for a set time quantum, during which I/o may occur, or after which it is interrupted by a timer interrupt and this whole procedure is repeated. The number of tickets assigned to each thread determines the likelihood that it’ll run at each scheduling decision, and thus (over the long term) the relative amount of time that it gets to run. Threads that are more likely to get chosen each time will get chosen more often, and thus will get more CPU time.

Current Issues in this lottery scheduler:
----------------------------
- The scheduler does not implement a pool of random numbers and choose from them, it just generates a new random number before it chooses a new winner.

Assignment By: Ahmed SalahEldin Farouk Elkashef - 1410216.
