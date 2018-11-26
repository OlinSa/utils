#ifndef UTILS_SRC_PROC_PID_H
#define UTILS_SRC_PROC_PID_H

#include <sys/types.h>
#include <unistd.h>

typedef struct{
    int pid; //The process ID.
    char comm[64]; //The filename of the executable
    char state; //process  state
    int ppid; //The PID of the parent of this process.
    int pgrp; //The process group ID of the process.
    int session; //The session ID of the process.
    int tty_nr; //The controlling terminal of the process
    int tpgid; //The ID of the foreground process group of the con‐trolling terminal of the process.
    unsigned int flags; //The kernel flags word of the process
    unsigned long minflt; //he number of minor faults the process has made which have not required loading a memory page from disk.
    unsigned long majflt; //The number of major faults the process has made which have required loading a memory page from disk.
    unsigned long cmajflt; //The number of major faults that the process's waited-for children have made.
    unsigned long utime; //Amount of time that this process has been scheduled in user mode
    unsigned long stime; //Amount of time that this process has been scheduled in kernel mode
    long cutime; //Amount of time that this process's waited-for children have been scheduled in user mode
    long cstime; // Amount of time that this process's waited-for chil‐dren have been scheduled in kernel mode
    long priority;
    long nice; //The nice value (see setpriority(2)), a value in the range 19 (low priority) to -20 (high priority).
    long num_threads; //Number of threads in this process
    long itrealvalue; // The time in jiffies before the next SIGALRM is sent to the process due to an interval timer
    unsigned long long starttime; //The time the process started after system boot
    unsigned long vsize; //virtual memory size in bytes
    long rss; //Resident Set Size: number of pages the process has in real memory.
    unsigned long rsslim; //Current soft limit in bytes on the rss of the process
    unsigned long startcode; //The address above which program text can run.
    unsigned long endcode; //The address below which program text can run
    unsigned long startstack; //The address of the start (i.e., bottom) of the stack.
    unsigned long kstkesp;//The current value of ESP (stack pointer), as found in the kernel stack page for the process.
    unsigned long kstkeip;//The current EIP (instruction pointer).
    unsigned long signal; //The bitmap of pending signals, displayed as a deci‐mal number.
    unsigned long blocked;//The bitmap of blocked signals, displayed as a deci‐mal number.
    unsigned long sigignore; //The bitmap of ignored signals, displayed as a deci‐mal number.
    unsigned long sigcatch; //The bitmap of caught signals, displayed as a decimal number.
    unsigned long wchan; //This is the "channel" in which the process is wait‐ing.
    unsigned long nswap; // Number of pages swapped
    unsigned long cnswap; //Cumulative nswap for child processes (not main‐tained).
    int exit_signal; //Signal to be sent to parent when we die.
    int processor; //CPU number last executed on
    unsigned rt_priority; // Real-time scheduling priority, a number in the range 1 to 99 for processes scheduled under a real-time policy, or 0, for non-real-time processes (see sched_setscheduler(2)).
    unsigned policy; //Scheduling policy
    unsigned long long delayacct_blkio_ticks; //Aggregated block I/O delays,
    unsigned long guest_time; //Guest time of the process
    long cguest_time; //Guest time of the process's children, measured in clock ticks
    unsigned long start_data; //Address above which program initialized and unini‐tialized (BSS) data are placed.
    unsigned long end_data; //Address below which program initialized and unini‐tialized (BSS) data are placed.
    unsigned long start_brk; //Address above which program heap can be expanded with brk(2).
    unsigned long arg_start; //Address above which program command-line arguments (argv) are placed
    unsigned long arg_end; //Address below program command-line arguments (argv) are placed
    unsigned long env_start; //Address above which program environment is placed.
    unsigned long env_end; //Address below which program environment is placed.
    int exit_code; //The thread's exit status in the form reported by waitpid(2).
}proc_pid_stat;

typedef struct{
    char cpu_name[32];
    unsigned long user;//Time spent in user mode.
    unsigned long nice;//Time spent in user mode with low priority
    unsigned long system;//Time spent in system mode.
    unsigned long idel;//Time spent in the idle task.
    unsigned long iowait;//Time waiting for IO to complete
    unsigned long irq;//Time servicing interrupts
    unsigned long softirq;//Time Servicing softirqs
    unsigned long steal;//Stolen time, which is the time spent in other operating systems when running in a virtu‐alized environment
    unsigned long guest;// Time spent running a virtual CPU for guest operating system
    unsigned long guest_nice;//Time spent running a niced guest (virtual CPU for guest operating systems under the control of the Linux kernel).
}proc_stat_t;


typedef struct{

}proc_cpu;
class CPidStat
{
public:
    CPidStat();
    CPidStat(pid_t pid);
    virtual ~CPidStat();
    bool init();
    void dump();
private:
    pid_t pid;
    proc_pid_stat pid_stat;
};

class CProcStat
{
public:
    CProcStat();
    virtual ~CProcStat();
    bool init();
    void dump();
private:
    proc_stat_t proc_stat;
};

#endif
