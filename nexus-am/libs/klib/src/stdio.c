#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  // va_list ap;
  // int ret = -1;
  // va_start(ap, format);
  // ret = vprintf(format, ap);
  // va_end(ap);
  // return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *p = fmt, *q = out, *s;
  int num = 0;
  while(*p != '\0'){
    if(*p++ != '%') continue;
    switch (*p){
    case 'd':
      num = va_arg(ap, int);
      if(num < 0){
        *q++ = '-'; num = -num;
      }
      s = atoi(num);
      strcpy(q, s);
      q += strlen(s);
      break;
    case 's':
      s = va_arg(ap, char*);
      strcpy(q, s);
      q += strlen(s);
      break;
    default:
      *q++ = *p;
      break;
    }
    p++;
  }
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  int ret = -1;
  va_start(args, fmt);  // 变参放入堆栈构造
  ret = vsprintf(out, fmt, args);
  va_end(args);
  return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
