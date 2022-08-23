#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

	// 输出(nemu) ，并读取一行输入然后返回
  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
	int n = 1;
	// printf("%d %s\n", strlen(args), args);
  if(args) n = atoi(args);
	for(int i=0;i<n;i++){
		cpu_exec(-1);
	}
  return 0;
}

static int cmd_info(char *args) {
	// isa/riscv32/reg.c: isa_reg_display
	if(!strcmp(args, "r")){
		// printf("info r\n");
		isa_reg_display();
	} else if(!strcmp(args, "w")){
		// printf("info w\n");
        TODO();
	} else{
		TODO();
	}
  return 0;
}

static int cmd_p(char *args) {
  return 0;
}

static int cmd_x(char *args) {
  uint32_t n = atoi(strtok(NULL, " "));
  // 去掉开头的0x
  // paddr_t addr = strtol(strtok(NULL, " ")+2, NULL, 16);
  bool success = false;
  paddr_t addr = expr(strtok(NULL, " "), &success);
  if(!success) return -1;
  for(int i=0;i<n;i++){
    Log("0x%x: 0x%x\n", addr+i, paddr_read(addr, i));
  }
  return 0;
}

static int cmd_w(char *args) {
  return 0;
}

static int cmd_d(char *args) {
  return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "si [N]: Execute N step", cmd_si },
	{ "info", "info SUBCMD: Print some infos", cmd_info },
	{ "p", "p EXPR: Calc expr value", cmd_p },
	{ "x", "x N EXPR: Print value of addr[EXPR, EXPR+N)", cmd_x },
	{ "w", "w EXPR: Stop when EXPR changes", cmd_w },
	{ "d", "d N: Delete watchpoint N", cmd_d },

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    // 按空格划分指令，每调用一次分割一次
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
		// info ...：分割成info和...
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
