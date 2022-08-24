#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
  // 从free_中删掉，加入head
  if(!free_) assert(0);
  WP* ret = free_;
  free_ = free_->next;
  ret->next = head;
  head = ret;
  return ret;
}

void free_wp(WP* wp){
  // 从head中删掉，加入free_
  WP* p = head;
  if(head->NO == wp->NO){
    head = head->next;
    wp->next = head;
    head = wp;
    return;
  }
  while(p->next){
    if(p->next->NO == wp->NO){
      head = head->next;
      wp->next = head;
      head = wp;
      return;
    }
    p = p->next;
  }
}
