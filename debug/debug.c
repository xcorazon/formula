//#include "debug/debug.h"
#include "debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <stdlib.h>

#ifdef OS_WINDOWS
  #include <windows.h>
  #define vswprintf _vsnwprintf
#endif
  
#ifdef OS_LINUX
  #include <syslog.h>
#endif
  


void debug(wchar_t *fmt,...)
{
  va_list     args;
  static wchar_t buf[8*1024];

  va_start(args,fmt);
  vswprintf(buf, sizeof(buf) / sizeof(wchar_t), fmt, args);
  
  #ifdef OS_WINDOWS
  OutputDebugStringW(buf);
  #endif
  
  #ifdef OS_LINUX
  FILE *fp;
  fp = fopen("slog.txt", "a");
  if(fp) {
    wcscat(buf, L"\n");
    fwprintf(fp, buf);
    fclose(fp);
  }
  #endif
  
  va_end(args);
}
