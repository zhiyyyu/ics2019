static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

void itoa(int num, char* s){
  if(num == 0) {
    s[0] = '0'; s[1] = '\0'; return;
  }
  char *p = s;
  while(num){
    *p++ = num % 10 + '0';
    num /= 10;
  }
  *p = '\0';
  int len = strlen(s);
  for(int i=0;i<len/2;i++){
    int tmp = s[len-1-i];
    s[len-1-i] = s[i];
    s[i] = tmp;
  }
}
