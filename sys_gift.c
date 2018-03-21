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

// A system call that takes 2 number and returns their summation only if they are both positive numbers

int
sys_mysys(struct thread *td, struct mysys_args *args)
{
int a;
int b;
a = args->a;
b = args->b;
if( a < 0 || b < 0)
return 1;
td->td_retval[0] = a + b;
return 0;
}

