#include "thread.h"
#include <stdio.h>
#include <assert.h>
#include <errno.h>
//#include <stdint.h>

/* test du join d'un thread qui fait plein de yield().
 *
 * le programme doit retourner correctement.
 * valgrind doit être content.
 *
 * support nécessaire:
 * - thread_create()
 * - thread_exit()
 * - thread_join() avec récupération valeur de retour, avec thread_exit()
 *   sur un thread qui yield() plusieurs fois vers celui qui joine.
 */

static void * thfunc(void *dummy __attribute__((unused)))
{
  unsigned i;
  for(i=0; i<10; i++) {
    printf("  le fils 1 yield\n");
    thread_yield();
  }
  thread_exit((void*)0xdeadbeef);
  return NULL; /* unreachable, shut up the compiler */
}

void * func(void *th){
	unsigned i;
	int err;
	for(i=0; i<5; i++) {
    	printf("  le fils 2 yield\n");
    	thread_yield();
  	}
	err = thread_join((thread_t) th,NULL);
	assert(err == EINVAL);
	NULL;
}

int main()
{
  thread_t th1,th2;
  int err;
  void *res = NULL;

  err = thread_create(&th1, thfunc, NULL);
  assert(!err);
  err = thread_create(&th2,func,th1);
  assert(!err);
  thread_yield();
  printf("le main joine...\n");
  err = thread_join(th1, &res);
  //assert(err == EAFNOSUPPORT);
  //assert(res == (void*) 0xdeadbeef);

  printf("join OK\n");
  return 0;
}