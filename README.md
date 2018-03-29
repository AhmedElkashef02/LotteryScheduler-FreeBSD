# LotteryScheduler-FreeBSD
Implementing a FreeBSD lottery scheduler.
This basic repository modifies the FreeBSD scheduler to use lottery scheduling rather than the current scheduler. Adds a new system call `gift(int,int)` to give tickets from  process to another.

## Automated Setup
- Run the executable file, which will remove and replace the new lottery scheduler and insert the new system call. Use:`./lottery_setup`
- Change Directory using `cd /usr/src/sys/amd64/conf`
- Copy the existence Kernel using `cp GENERIC ASGN2`
- Change Directory using `cd /usr/src/`
- Build the Kernel: `make buildkernel KERNCONF=ASGN2` Note: this may take time
- Install the Kernel: `make installkernel KERNCONF=ASGN2`
- Reboot

How does this scheduler work?
-------------------------
A lottery scheduler assigns each process’s threads some number of tickets, then randomly draws a ticket among those allocated to “ready” threads to decide which one to run. That thread is then allowed to run for a set time quantum, during which I/o may occur, or after which it is interrupted by a timer interrupt and this whole procedure is repeated. The number of tickets assigned to each thread determines the likelihood that it’ll run at each scheduling decision, and thus (over the long term) the relative amount of time that it gets to run. Threads that are more likely to get chosen each time will get chosen more often, and thus will get more CPU time.

How to test the nice(tickets) system call?
-------------------------
- Run the `test-nice.c` code by `./test-nice`
- This code uses the nice() system call with 30 tickets, printf statements will be printed showing the number of tickets before and after.

How to test the gift(PID, tickets) system call?
-------------------------
- Run any process in the background, an example would be `nano test.c &`
- Inspect the running process to know its PID using `ps -l`
- Enter the corrent PID in the `test-gift.c` compile and run it by `./test-gift`
- This code uses the gift() system call with 30 tickets, printf statements will be printed showing the number of tickets before and after for the calling and the receiving process.

###Note:
Calling gift(0,0) will return the number of tickets in the calling process
Calling gift(PID,tickets) with a negative number of tickets will deduct tickets from that process with ID = PID.

Attributions & Sources:
----------------------------
The implementation of this lottery scheduler is inspired by various sources, please check the following links:
- The work of loug38 in this link: https://github.com/loug38/Lottery-Scheduling-for-freeBSD
- Geeks For Geeks: https://www.geeksforgeeks.org/operating-system-lottery-scheduling/
- Wikipedia: https://en.wikipedia.org/wiki/Lottery_scheduling
- http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-lottery.pdf

Assignment By: Ahmed SalahEldin Farouk Elkashef - 1410216.
