#include <sys/sysproto.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/sysent.h>
#include <sys/lock.h>
#include <sys/mutex.h>

#ifndef _SYS_SYSPROTO_H_
struct gift_args{
int p_pid;
int tickets;
};
#endif

int
sys_gift(struct thread *td, struct mysys_args *args)
{
  int p_pid;
  int tickets;
  
  p_pid = args->p_pid;
  tickets = args->tickets;
  
  struct proc *this_p = td->td_proc; //1)defines the current process
  PROC_LOCK(this_p);
  
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

