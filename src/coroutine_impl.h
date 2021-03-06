#ifndef __COROUTINE_IMPL_H__
#define __COROUTINE_IMPL_H__

#include <poll.h>
#include "typedef.h"
#include "coroutine.h"
#include "coroutine_task.h"
#include "context.h"
#include "epoll.h"

#ifdef __cplusplus
extern "C" {
#endif

struct env_t {
  coroutine_t **callstack;
  int callstacksize;
  coroutine_t *main;
  epoll_context_t *epoll;

  taskpool_t *pool;

  void *arg;

  coroutine_t *occupy;
  coroutine_t *pending;
};

struct coroutine_stack_t {
  coroutine_t *coroutine;
  int size;
  char *end;
  char *start;
};

typedef enum state_t {
  INIT,
  RUNNING,
  STOPPED
} state_t;

struct coroutine_specific_t {
  void *value;
};

struct coroutine_t {
  coroutine_fun_t fun;
  env_t *env;
  void *arg;
  char main;
  state_t state;

  context_t context;

  char *stack_sp;
  unsigned int save_size;
  char *save_buffer;

  coroutine_stack_t *stack;

  task_t *task;

  coroutine_specific_t spec[1024];
};

typedef int (*poll_fun_t)(struct pollfd fds[], nfds_t nfds, int timeout);
int poll_inner(epoll_context_t *ctx, struct pollfd fds[], nfds_t nfds, int timeout, poll_fun_t pollfunc);

int	coroutine_poll(epoll_context_t *ctx,struct pollfd fds[], nfds_t nfds, int timeout_ms);
epoll_context_t *get_epoll_context();

env_t* get_curr_thread_env();
void do_init_curr_thread_env();

coroutine_t* coroutine_new(coroutine_fun_t fun, void *arg);
void coroutine_resume(coroutine_t *co);

void coroutine_yield_context();
coroutine_t* coroutine_self();
unsigned long long get_now();

#ifdef __cplusplus
}
#endif

#endif  // __COROUTINE_IMPL_H__
