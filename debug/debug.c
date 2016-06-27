//#include "debug/debug.h"
#include "debug.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef OS_WINDOWS
  #include <windows.h>
#endif
  
#ifdef OS_LINUX
  #include <syslog.h>
#endif
  


void debug(char *fmt,...)
{
  va_list     args;
  static char buf[8*1024];

  va_start(args,fmt);
  vsnprintf(buf, sizeof(buf)-3, fmt, args);
  
  #ifdef OS_WINDOWS
  OutputDebugString(buf);
  #endif
  
  #ifdef OS_LINUX
  openlog("slog", LOG_PID|LOG_CONS, LOG_USER);
  syslog(LOG_INFO, buf);
  closelog();
  #endif
  
  va_end(args);
}