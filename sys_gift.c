#include <sys/sysproto.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/sysent.h>
#ifndef _SYS_SYSPROTO_H_

struct mysys_args{
int a;
int b;
};

#endif

int
sys_gift(pid_t p_pid, int tickets)
{
  struct proc *this_p = pfind(getpid()); //1)defines the current process, 2)locks it
  struct thread *td;
  
  printf("before transfer: this process: %d", this_p->total_tickets);
  
  if(p_pid == 0 && tickets == 0) {
    td->td_retval[0] = this_p->total_tickets;
    PROC_UNLOCK(this_p);
  } else {
    // check that tickets is a valid number
    if (tickets > 0) {
      struct proc *target_p = pfind(p_pid);
      // check if its a valid process
      if (target_p == NULL) {
        PROC_UNLOCK(target_p);
        td->td_retval[0] = 0;
      } else {
        printf("before transfer: target process: %d", target_p->total_tickets);
        // check if this process can transfer
        if (this_p->total_tickets - tickets >= 1 && target_p->total_tickets + tickets <= 100000) {
          // transfer the tickets
          sched_increaseTickets(target_p, tickets);
          sched_decreaseTickets(this_p, tickets);
          printf("after transfer: this process: %d", this_p->total_tickets);
          printf("after transfer: target process: %d", target_p->total_tickets);
        }
      }
    }
  }
return 0;
}

