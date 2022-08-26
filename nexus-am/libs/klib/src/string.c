#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  char* p = s;
  size_t len = 0;
  while(p != '\0') p++, len++;
  return len;
}

char *strcpy(char* dst,const char* src) {
  char* p = src;
  while(p != '\0'){
    *dst = *p++;
  }
  *dst = *p;
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char* p = src;
  while(p != '\0' && n > 0){
    *dst = *p++; n--;
  }
  if(n > 0) *dst = *p;
  return dst;
}

char* strcat(char* dst, const char* src) {
  char *p = dst, *q = src;
  while(p != '\0') p++;
  while(q != '\0'){
    *p = *q; p++; q++;
  }
  *p = *q;
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  char *p = s1, *q = s2;
  while(p != '\0' && q != '\0' && p == q) s1++, s2++;
  if(p == '\0' && q == '\0') return 0;
  if(p == '\0') return -1;
  if(q == '\0') return 1;
  return *p < *q ? -1 : 1;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  char *p = s1, *q = s2;
  while(p != '\0' && q != '\0' && p == q && n > 0) s1++, s2++, n--;
  if(n > 0) return *p < *q ? -1 : 1;
  if(p == '\0' && q == '\0') return 0;
  if(p == '\0') return -1;
  if(q == '\0') return 1;
  return *p < *q ? -1 : 1;
}

void* memset(void* v,int c,size_t n) {
  char* a = v;
  for(size_t i=0;i<n;i++){
    a[i] = c;
  }
  return c;
}

void* memcpy(void* out, const void* in, size_t n) {
  char *p = out, *q = in;
  while(n--){
    *p = *q; p++; q++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char *p = s1, *q = s2;
  while(n--){
    if(*p > *q) return 1;
    if(*p < *q) return -1;
    p++, q++;
  }
  return 0;
}

#endif
