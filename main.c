#include "common.h"
#include "coroutine.h"
#include "sched.h"
#include "timer.h"

extern void init_idle();

void *coroutine_fun(void *arg)
{
    coroutine_t *self = (coroutine_t *)arg;
    for (int i = 0; i < 10; ++i)
    {
        printf("%s %d\n", self->name, i);
    }
}

int main()
{
    init_timer();
    init_idle();

    coroutine_create(NULL, coroutine_fun, "c1");
    coroutine_create(NULL, coroutine_fun, "c2");

    // 等待1s,让模拟时钟中断的线程启动完毕
    sleep(1);
    start_timer();

    getchar();

    return 0;
}