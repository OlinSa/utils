#include "proc.h"
#include "log.h"
#include "unistd.h"

int main()
{
    CPidStat pid_stat;
    if(pid_stat.init()) {
        pid_stat.dump();
    }else{
        LOG_ERR("pid_stat error");
    }

    CProcStat proc_stat;
    if(proc_stat.init()){
        proc_stat.dump();
    }else{
        LOG_ERR("proc_state error");
    }


    return 0;
}
