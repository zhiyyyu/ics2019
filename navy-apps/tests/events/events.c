#include <stdio.h>

int main() {
  FILE *fp = fopen("/dev/events", "r");
  int time = 0;
  printf("Start to receive events...\n");
  int i = 0;
  while (1) {
    if(i++ > 10000) break;
    char buf[256];
    char *p = buf, ch;
    while ((ch = fgetc(fp)) != -1) {
      *p ++ = ch;
      putchar(ch);
      if(ch == '\n') {
        *p = '\0';
        break;
      }
    }
    printf("buf: %s buf[0] %c\n", buf, buf[0]);

    int is_time = buf[0] == 't';
    time += is_time;
    if (!is_time) {
      printf("receive event: %s\n", buf);
    }
    else if (time % 1024 == 0) {
      printf("receive time event for the %dth time: %s\n", time, buf);
    }
  }

  fclose(fp);
  return 0;
}

