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
struct gift_args {
        int p_pid;
        int tickets;
};
#endif

int
sys_gift(struct thread *td, struct gift_args *args)
{
        int p_pid;
        int tickets;
        int total_tickets_per_proc = 0;
        int total_tickets_per_targ = 0;

        p_pid = args->p_pid;
        tickets = args->tickets;

        struct proc *this_p = td->td_proc; //1)defines the current process
        PROC_LOCK(this_p);

        FOREACH_THREAD_IN_PROC(this_p, td) {
                thread_lock(td);
                total_tickets_per_proc += td->tickets;
                thread_unlock(td);
        }

        // check if the owner process is root process
        if (this_p->p_ucred == 0) {
                printf("Error(): gift() does not work from a root process.\n");
                PROC_UNLOCK(this_p);
                return 0;
        }
        // if gift(0,0);
        if(p_pid == 0 && tickets == 0) {
                td->td_retval[0] = total_tickets_per_proc;
                PROC_UNLOCK(this_p);
                return 0;
        }
        // check that tickets is a valid number
        if (tickets > 0) {
                struct proc *target_p = pfind(p_pid);
                // check if its a valid process
                if (target_p == NULL) {
                        printf("Error(): Target process is not valid.\n");
                        PROC_UNLOCK(this_p);
                        return 0;
                }
                // check if the target process is root process
                if (target_p->p_ucred == 0) {
                        printf("Error(): Target process is a root process.\n");
                        PROC_UNLOCK(this_p);
                        PROC_UNLOCK(target_p);
                        return 0;
                }
                // count tickets per target process
                FOREACH_THREAD_IN_PROC(target_p, td) {
                        thread_lock(td);
                        total_tickets_per_targ += td->tickets;
                        thread_unlock(td);
                }

                // check if this process can transfer
                if (total_tickets_per_proc - tickets >= 1 && total_tickets_per_targ + tickets <= 100000) {
                        
                        printf("before transfer: this process: %d\n", total_tickets_per_proc);
                        printf("before transfer: target process: %d\n", total_tickets_per_targ);
                        
                        // transfer the tickets
                        sched_increaseTickets(target_p, tickets);
                        sched_decreaseTickets(this_p, tickets);
                        
                        // recount total tickets for parent process
                        total_tickets_per_proc = 0;
                        FOREACH_THREAD_IN_PROC(this_p, td) {
                                thread_lock(td);
                                total_tickets_per_proc += td->tickets;
                                thread_unlock(td);
                        }
                        
                        // recount total tickets for target process
                        total_tickets_per_targ = 0;
                        FOREACH_THREAD_IN_PROC(target_p, td) {
                                thread_lock(td);
                                total_tickets_per_targ += td->tickets;
                                thread_unlock(td);
                        }
                        
                        printf("after transfer: this process: %d\n", total_tickets_per_proc);
                        printf("after transfer: target process: %d\n", total_tickets_per_targ);
                        
                        PROC_UNLOCK(target_p);
                        PROC_UNLOCK(this_p);
                        return 0;
                } else {
                        printf("Error(): Ticket transfer is larger than 100,000. Cannot proceed.\n");
                        PROC_UNLOCK(this_p);
                        PROC_UNLOCK(target_p);
                        return 0;
                }
        } else {
                // if the tickets are negative: decrease that number from the process tickets
                struct proc *target_p = pfind(p_pid);
                // check if its a valid process
                if (target_p == NULL) {
                        printf("Error(): Target process is not valid.\n");
                        PROC_UNLOCK(this_p);
                        return 0;
                }
                // check if the target process is root process
                if (target_p->p_ucred == 0) {
                        printf("Error(): Target process is a root process.\n");
                        PROC_UNLOCK(this_p);
                        PROC_UNLOCK(target_p);
                        return 0;
                }
                // count tickets per target process
                FOREACH_THREAD_IN_PROC(target_p, td) {
                        thread_lock(td);
                        total_tickets_per_targ += td->tickets;
                        thread_unlock(td);
                }
                // check if this process can be deducted from
                if (total_tickets_per_targ - tickets >= 1) {
                        printf("before deduction: target process: %d\n", total_tickets_per_targ);
                        
                        sched_decreaseTickets(target_p, tickets);
                        
                        // recount total tickets for target process
                        total_tickets_per_targ = 0;
                        FOREACH_THREAD_IN_PROC(target_p, td) {
                                thread_lock(td);
                                total_tickets_per_targ += td->tickets;
                                thread_unlock(td);
                        }
                        
                        printf("after deduction: target process: %d\n", total_tickets_per_targ);
                        PROC_UNLOCK(target_p);
                        PROC_UNLOCK(this_p);
                        return 0;
                }
        }
        return 0;
}
