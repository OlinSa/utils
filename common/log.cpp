#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include "log.h"
#include "utils.h"

static const char *g_LevelName[] = {"emerg", "crit", "err", "warning", "info", "debug"};

void Log::Printf(enum LogLevel level, const char *file, const char *func, int l, const char *fmt, ...)
{
    if(level >_level) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
    switch (level) {
    case LOG_LEVEL_EMERG:
        std::cout<<RED;
        break;
    case LOG_LEVEL_CRIT:
        std::cout<<YELLOW;
        break;
    case LOG_LEVEL_ERR:
        std::cout<<BOLDCYAN;
        break;
    case LOG_LEVEL_WARINING:
        std::cout<<BOLDMAGENTA;
        break;
    case LOG_LEVEL_INFO:
        std::cout<<BOLDCYAN;
        break;
    case LOG_LEVEL_DEBUG:
        std::cout<<GREEN;
        break;
    default:
        std::cout<<"unsupport level("<<level<<")"<<std::endl;
        return;
    }
    const char *filename = strrchr(file, '/');
    if(filename){
        filename += 1;
    }else {
        filename = file;
    }
    std::cout<<"["<<g_LevelName[level]<<"]"<<RESET;
    std::cout<<"["<<filename<<"]"<<func<<" ("<< l<< "):\t";
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    std::cout<<std::endl;
    return;
}


