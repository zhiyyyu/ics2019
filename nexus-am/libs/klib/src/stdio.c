#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char buf[512];
  va_list args;
  int ret = -1;
  va_start(args, fmt);  // 变参放入堆栈构造
  ret = vsprintf(buf, fmt, args);
  va_end(args);
  if(!ret) for(int i=0;buf[i] != '\0';i++) _putc(buf[i]);
  return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *p = fmt, *q = out, *s;
  int num = 0;
  unsigned int hex = 0;
  char num_s[32];
  while(*p != '\0'){
    if(*p == '%') {
      switch (*++p){
      case 'd':
        num = va_arg(ap, int);
        if(num < 0){
          *q++ = '-'; num = -num;
        }
        mitoa(num, num_s, 10);
        strcpy(q, num_s);
        q += strlen(num_s);
        break;
      case 'x':
        num = va_arg(ap, int);
        mitoa(num, num_s, 16);
        strcpy(q, num_s);
        q += strlen(num_s);
        break;
      case 's':
        s = va_arg(ap, char*);
        strcpy(q, s);
        q += strlen(s);
        break;
      default: return -1;
      }
      p++;
    } else{
      *q = *p; p++; q++;
    }
  }
  *q = '\0';
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
