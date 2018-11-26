#include <stdio.h>
#include "proc.h"
#include "log.h"

CPidStat::CPidStat()
{
    this->pid = getpid();
}
CPidStat::CPidStat(pid_t pid)
{
    this->pid = pid;
}
CPidStat::~CPidStat()
{

}

bool CPidStat::init()
{
    char pid_file[32] ={0};
    snprintf(pid_file, sizeof(pid_file), "/proc/%d/stat", this->pid);
    FILE *fp = fopen(pid_file, "r");
    if(!fp) {
        LOG_ERR("open %s failed", pid_file);
        return false;
    }

    int ret = fscanf( fp,
            "%d %s %c %d %d %d %d %d "\
            "%u %lu %lu %lu %lu %lu %lu %ld "\
            "%ld %ld %ld %ld %ld %llu %lu %ld "\
            "%lu %lu %lu %lu %lu %lu %lu %lu "\
            "%lu %lu %lu %lu %lu %d %d %u "\
            "%u %llu %lu %ld %lu %lu %lu %lu "\
            "%lu %lu %lu %d",
            &pid_stat.pid,      pid_stat.comm,                      &pid_stat.state,        &pid_stat.ppid,         &pid_stat.pgrp,         &pid_stat.session,      &pid_stat.tty_nr,       &pid_stat.tpgid, \
            &pid_stat.flags,    &pid_stat.minflt,                   &pid_stat.cmajflt,      &pid_stat.majflt,       &pid_stat.cmajflt,      &pid_stat.utime,        &pid_stat.stime,        &pid_stat.cutime, \
            &pid_stat.cstime,   &pid_stat.priority,                 &pid_stat.nice,         &pid_stat.num_threads,  &pid_stat.itrealvalue,  &pid_stat.starttime,    &pid_stat.vsize,        &pid_stat.rss,\
            &pid_stat.rsslim,   &pid_stat.startcode,                &pid_stat.endcode,      &pid_stat.startstack,   &pid_stat.kstkesp,      &pid_stat.kstkeip,      &pid_stat.signal,       &pid_stat.blocked,\
            &pid_stat.sigignore,&pid_stat.sigcatch,                 &pid_stat.wchan,        &pid_stat.nswap,        &pid_stat.cnswap,       &pid_stat.exit_signal,  &pid_stat.processor,    &pid_stat.rt_priority,\
            &pid_stat.policy,   &pid_stat.delayacct_blkio_ticks,    &pid_stat.guest_time,   &pid_stat.cguest_time,  &pid_stat.start_data,   &pid_stat.end_data,     &pid_stat.start_brk,    &pid_stat.arg_start,\
            &pid_stat.arg_end,  &pid_stat.env_start,                &pid_stat.env_end,      &pid_stat.exit_code);
    fclose(fp);
    if(ret != 52){
        LOG_ERR("fscanf %s failed", pid_file);
        return false;
    }
    return true;
}

 void CPidStat::dump()
 {
     LOG_DEBUG(
             "%d %s %c %d %d %d %d %d "\
             "%u %lu %lu %lu %lu %lu %lu %ld "\
             "%ld %ld %ld %ld %ld %llu %lu %ld "\
             "%lu %lu %lu %lu %lu %lu %lu %lu "\
             "%lu %lu %lu %lu %lu %d %d %u "\
             "%u %llu %lu %ld %lu %lu %lu %lu "\
             "%lu %lu %lu %d",
             pid_stat.pid,      pid_stat.comm,                     pid_stat.state,        pid_stat.ppid,         pid_stat.pgrp,         pid_stat.session,      pid_stat.tty_nr,       pid_stat.tpgid, \
             pid_stat.flags,    pid_stat.minflt,                   pid_stat.cmajflt,      pid_stat.majflt,       pid_stat.cmajflt,      pid_stat.utime,        pid_stat.stime,        pid_stat.cutime, \
             pid_stat.cstime,   pid_stat.priority,                 pid_stat.nice,         pid_stat.num_threads,  pid_stat.itrealvalue,  pid_stat.starttime,    pid_stat.vsize,        pid_stat.rss,\
             pid_stat.rsslim,   pid_stat.startcode,                pid_stat.endcode,      pid_stat.startstack,   pid_stat.kstkesp,      pid_stat.kstkeip,      pid_stat.signal,       pid_stat.blocked,\
             pid_stat.sigignore,pid_stat.sigcatch,                 pid_stat.wchan,        pid_stat.nswap,        pid_stat.cnswap,       pid_stat.exit_signal,  pid_stat.processor,    pid_stat.rt_priority,\
             pid_stat.policy,   pid_stat.delayacct_blkio_ticks,    pid_stat.guest_time,   pid_stat.cguest_time,  pid_stat.start_data,   pid_stat.end_data,     pid_stat.start_brk,    pid_stat.arg_start,\
             pid_stat.arg_end,  pid_stat.env_start,                pid_stat.env_end,      pid_stat.exit_code);
 }



 CProcStat::CProcStat()
 {

 }
 CProcStat::~CProcStat()
 {

 }
 bool CProcStat::init()
 {
     char proc_stat_path[] = "/proc/stat";
     FILE *fp = fopen(proc_stat_path, "r");
     if(!fp) {
         LOG_ERR("open %s failed", proc_stat_path);
         return false;
     }
     int ret = fscanf(fp, "%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
            proc_stat.cpu_name, &proc_stat.user, &proc_stat.nice, &proc_stat.system, &proc_stat.idel, &proc_stat.iowait, &proc_stat.irq, &proc_stat.softirq, &proc_stat.steal, &proc_stat.guest, &proc_stat.guest_nice);
     fclose(fp);
     if(11 != ret) {
         LOG_ERR("fscanf failed");
         return false;
     }
     return true;
 }
 void CProcStat::dump()
 {
     LOG_DEBUG("%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
            proc_stat.cpu_name, proc_stat.user, proc_stat.nice, proc_stat.system, proc_stat.idel, proc_stat.iowait, proc_stat.irq, proc_stat.softirq, proc_stat.steal, proc_stat.guest, proc_stat.guest_nice);
 }
