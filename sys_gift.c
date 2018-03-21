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
  struct proc *target_p = pfind(p_pid);
  struct proc *this_p = pfind(getpid());
  printf("before transfer: this process: %d", this_p->total_tickets);
  printf("before transfer: target process: %d", target_p->total_tickets);
  // check if this process can transfer
  if (this_p->total_tickets - tickets >= 1 && target_p->total_tickets + tickets <= 100000) {
    // transfer the tickets
    this_p->total_tickets -= tickets;
    target_p->total_tickets += tickets;
  } else {
    // do not transfer, and print out the tickets of this_p
    printf("total tickets for invoking process is: %d\n", this_p->total_tickets); 
  }
  printf("after transfer: this process: %d", this_p->total_tickets);
  printf("after transfer: target process: %d", target_p->total_tickets);
return 0;
}

