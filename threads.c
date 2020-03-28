
#include "os.h"

extern  long stack_address_runningthread;
extern  long start_address_runningthread;
extern  long * tcb_address_runningthread;
extern  long runningthreadid;
extern int strcpyN(char *to, char *from, int bufsiz);

#define NAMESIZE 16

struct tcb {
    LL_PTRS;
    char    name[NAMESIZE];
    long    threadid;
    long    stack;
    long    regs[17];   // 17th reg is the SPSR saved by context switch
} tcbs[ NUM_THREADS ];

llobject_t TF;
struct tcb *tfree;

struct tcb *active_thread;
struct tcb *null_thread;

llobject_t RQ;
struct tcb *runq;

llobject_t SQ;
struct tcb *sleepq;


void
dump_tcb(long threadid)
{
    if (threadid < 0 || threadid > NUM_THREADS) {
        log("dump_tcb: invalid threadid", threadid);
        return;
    }
    if (threadid == NUM_THREADS) {
        threadid = active_thread->threadid;
    }
    log("Dumping TCB for thread", threadid);
    struct tcb *tp = &tcbs[threadid];
    log(tp->name, tp->threadid);
    log(" stack", tp->stack);
    log(" tcb @", (long)(tp->regs));
    log("  r0  ", tp->regs[REG_r0]);
    log("  r1  ", tp->regs[REG_r1]);
    log("  r2  ", tp->regs[REG_r2]);
    log("  r3  ", tp->regs[REG_r3]);
    log("  r4  ", tp->regs[REG_r4]);
    log("  r5  ", tp->regs[REG_r5]);
    log("  r6  ", tp->regs[REG_r6]);
    log("  r7  ", tp->regs[REG_r7]);
    log("  r8  ", tp->regs[REG_r8]);
    log("  r9  ", tp->regs[REG_r9]);
    log("  r10 ", tp->regs[REG_r10]);
    log("  r11 ", tp->regs[REG_r11]);
    log("  r12 ", tp->regs[REG_r12]);
    log("  sp  ", tp->regs[REG_sp]);
    log("  lr  ", tp->regs[REG_lr]);
    log("  pc  ", tp->regs[REG_pc]-4);
    log("  spsr", tp->regs[REG_spsr]);

    return;
}

void
create_thread(char *name, void *text)
{
    struct tcb *tp;

    if (name == (char *)NULL) {
        tp = null_thread;
        tp->regs[REG_sp] = tp->stack;
        tp->regs[REG_pc] = (long)text+4;
        tp->regs[REG_spsr] = 0x150; // USR_mode, IRQ_int on, FIG_int off, disables imprecise aborts
        strcpyN(tp->name, "NULL thread", NAMESIZE);
        tp->name[NAMESIZE-1] = '\0';
    } else {
        tp = (struct tcb *)LL_POP(tfree);
        if (tp == (struct tcb *)NULL) {
            log("WARNING: out of processes in create_thread", NOVAL);
            return ;
        }

        tp->regs[REG_sp] = tp->stack;
        tp->regs[REG_pc] = (long)text+4;
        tp->regs[REG_spsr] = 0x150; // USR_mode, IRQ_int on, FIG_int off, disables imprecise aborts
        strcpyN(tp->name, name, NAMESIZE);
        tp->name[NAMESIZE-1] = '\0';

        LL_PUSH(runq, tp);
    }

    log("create_thread:",NOVAL);
    log(tp->name, tp->threadid);
    log(" stack =", tp->regs[REG_sp]);
    log(" start =", tp->regs[REG_pc] - 4);
    log(" tcb   =", (long)tp->regs);

    return;
}





/*! \brief Determine the thread to run.

    NOTE: You should NOT use the `tcbs` variable in this function.
        The operations on the TCBs should be carried out through
        `runq`, `tfree` and `sleepq`.

    \param new_state Determines the behaviour of the scheduler.
        If new_state is THREAD_INIT, the scheduler should 
        select the first thread to run from runq, then assign 
        values to the following variables:
                - stack_address_runningthread
                    the address of the thread stack
                - start_address_runningthread
                    the start address of the thread
                - tcb_address_runningthread
                    the address of `regs` of the thread TCB 
                - runningthreadid
                    the id of the thread
                - active_thread
                    the address of the thread TCB 
                    
        If new_state is THREAD_RUN, the scheduler should 
        select a different thread to run from runq (if 
        there are more than 1 threads in runq), then assign 
        values to the following variables:
                - tcb_address_runningthread
                    the address of `regs` of the thread TCB 
                - runningthreadid
                    the id of the thread
                - active_thread
                    the address of the thread TCB 
            - NOTE: you should select threads smartly to
                avoid thread starvation
            - NOTE: if there is no thread in runq, 
                you should run null_thread

        If new_state is THREAD_SLEEP, the scheduler 
        should put the currently running thread to sleep
        by poping its TCB out of runq and pushing its TCB 
        to sleepq. Then repeat the logic in THREAD_RUN to
        select the next thread to run.
*/
void scheduler(int new_state) {
    // your code goes here
}



/*! \brief Wake up a thread and set its context-saved r0 

    This function could be used to complete a non-blocking SVC 
    call. If the SVC call is supposed to return a value to 
    its user program (for example, `syscall_read_word` expects 
    its SVC call to return the word just read), you should
    put that return value in the thread's context-saved r0.

    \param threadid Specifies the thread to wake up. You need 
        to remove its TCB from sleepq, then push it to runq.

    \param returnval The value to put into the thread's 
        context-saved r0
*/
void wake_thread(long threadid, long returnval) {
    if (threadid < 0 || threadid >= NUM_THREADS) {
        log("ERROR: wake_thread: invalid threadid", threadid);
        return;
    }
    struct tcb *tp = &tcbs[threadid];

    // your code goes here 
}

void
init_threads()
{
    int i,j;

    tfree = (struct tcb *)&TF;
    LL_INIT(tfree);

    runq = (struct tcb *)&RQ;
    LL_INIT(runq);

    sleepq = (struct tcb *)&SQ;
    LL_INIT(sleepq);

    for (i=0; i<NUM_THREADS; i++) {
        strcpyN(tcbs[i].name, "[uninitialized]", NAMESIZE);
        tcbs[i].name[NAMESIZE-1] = '\0';
        tcbs[i].threadid = i;
        tcbs[i].stack = 0x20000 + ((i+1) << 12);    // hack for now
        for (j=0; j<17; j++) {
            tcbs[i].regs[j] = 0;
        }
        if (i > 0) {
            struct tcb *tp = &tcbs[i];
            LL_APPEND(tfree, tp);
        }
    }

    active_thread = (struct tcb *)NULL;
    null_thread = &tcbs[0];

    create_thread((char *)NULL, hang);
}

void
dump_processes()
{
    struct tcb *tp;
    log("PS: Active processes ...", NOVAL);
    dump_tcb(active_thread->threadid);
    LL_EACH(runq, tp, struct tcb) {
        dump_tcb(tp->threadid);
    }
}
