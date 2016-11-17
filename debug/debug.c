//#include "debug/debug.h"
#include "debug.h"
#undef __STRICT_ANSI__ 
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <stdlib.h>

#ifdef OS_WINDOWS
  #include <windows.h>
#endif
  
#ifdef OS_LINUX
  #include <syslog.h>
#endif
  


void debug(char *fmt,...)
{
  va_list     args;
  static wchar_t buf[8*1024];

  va_start(args,fmt);
  
  /*
  #ifdef OS_WINDOWS
  OutputDebugString(buf);
  #endif
  */
  
  #ifdef OS_LINUX
  vswprintf(buf, sizeof(buf) / sizeof(wchar_t), fmt, args);
  openlog("slog", LOG_PID|LOG_CONS, LOG_USER);
  syslog(LOG_INFO, buf);
  closelog();
  #endif
  
  va_end(args);
}