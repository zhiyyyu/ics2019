#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ, TK_NEQ,
  TK_REG,
  TK_NUM, TK_HEX,
  TK_DEREF,
  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"^\\*", TK_DEREF},        
  {" +", TK_NOTYPE},          // spaces
  {"\\+", '+'},               // plus
  {"-", '-'},                 // minus
  {"\\*", '*'},               // mul
  {"/", '/'},                 // div
  {"==", TK_EQ},              // equal
  {"\\(", '('},               // left
  {"\\)", ')'},               // right
  {"[1-9][0-9]*", TK_NUM},    // number
  {"0(X|x)[0-9]*", TK_HEX},   // hex
  {"(\\$(0|pc)|ra|(s|g|t)p|t[0-6]|s[0-11]|a[0-7])", TK_REG},    // register
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    // 编译所有的正则表达式
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;
extern const char* regsl[];

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  char reg_name[32];

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      // 匹配正则表达式
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        // eo: end, so: start
        int substr_len = pmatch.rm_eo;

        Log("token[%d] match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            nr_token+1, i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        switch (rules[i].token_type) {
        case TK_NUM: case TK_HEX:
          strncpy(tokens[nr_token].str, substr_start, substr_len); break;
        case TK_REG:
          strncpy(reg_name, substr_start, substr_len);
          if(!strcmp(reg_name, "$pc")){
            sprintf(tokens[nr_token].str, "%d", cpu.pc); break;
            // strcpy(tokens[nr_token].str, itoa(cpu.pc)); break;
          }
          for(int i=0;i<32;i++){
            if(!strcmp(reg_name, regsl[i])){
              sprintf(tokens[nr_token].str, "%d", cpu.gpr[i]._32); break;
              // strcpy(tokens[nr_token].str, itoa(cpu.gpr[i]._32)); break;
            }
          }
          break;
        default: break;
        }
        tokens[nr_token++].type = rules[i].token_type;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t eval(uint32_t p, uint32_t q);
bool check_parentheses(uint32_t p, uint32_t q);
uint32_t get_main_op(uint32_t p, uint32_t q);
bool is_op(int type);

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // Log("make token: success.");
  *success = true;
  return eval(0, nr_token-1);
}

uint32_t eval(uint32_t p, uint32_t q) {
  // Log("eval(%d, %d)", p, q);
  if (p > q) {
    /* Bad expression */
    // panic("bad experssion");
    return 0;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    uint32_t val;
    switch (tokens[p].type){
    case TK_HEX:
      val = strtol(tokens[p].str+2, NULL, 16); break;
    case TK_NUM: case TK_REG:
      val = atoi(tokens[p].str); break;
    default:
      TODO(); break;
    }
    // Log("single token: 0X%x", val);
    return val;
  }
  else if (check_parentheses(p, q)) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    Log("(...)");
    return eval(p + 1, q - 1);
  }
  else {
    /* We should do more things here. */
    // the position of main op in the token expression
    uint32_t op = get_main_op(p, q);
    uint32_t val1 = eval(p, op - 1);
    uint32_t val2 = eval(op + 1, q);
    // Log("expr: %d %c %d", val1, tokens[op].type, val2);

    switch (tokens[op].type) {
      case '+': return val1 + val2; break;
      case '-': return val1 - val2; break;
      case '*': return val1 * val2; break;
      case '/': return val1 / val2; break;
      case TK_DEREF: return paddr_read(val2, 4);
      default: assert(0);
    }
  }
}

bool check_parentheses(uint32_t p, uint32_t q){
  if(tokens[p].type != '(' || tokens[q].type != ')'){
    return false;
  }
  int idx = 0;
  for(int i=p;i<=q;i++){
    if(tokens[i].type == '('){
      idx++;
    } else if(tokens[i].type == ')'){
      idx--;
      if(idx == 0) return false;
    }
  }
  if(idx > 0) return false;
  return true;
}

uint32_t get_main_op(uint32_t p, uint32_t q){
  int p_idx = 0, t_idx = -1;
  for(int i=q;i>=(int)p;i--){
    // Log("t_idx %d i %d\n", t_idx, i);
    if(tokens[i].type == ')'){
      p_idx++;
    } else if(tokens[i].type == '('){
      p_idx--;
    } else if(p_idx == 0 && is_op(tokens[i].type)){
      t_idx = i;
    }
  }
  assert(t_idx > 0);
  return t_idx;
}

inline bool is_op(int type){
  return type == '+' || type == '-' || type == '*' || type == '/';
}