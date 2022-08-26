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
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  // va_list ap;
  // int ret = -1;
  // va_start(ap, format);
  // ret = vsprintf(str, format, ap);
  // va_end(ap);
  // return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
