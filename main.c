#include "common.h"
#include "proutine.h"

extern void init_idle();

void *coroutine_fun(void *arg)
{
    coroutine_t *self = (coroutine_t *)arg;

    for (int i = 0; i < 100000; ++i)
    {
        printf("协程 ·%s· 获得调度\n", self->name);
        sleep(1);
    }
}

int main()
{
    init_timer();
    init_idle();

    // 等待1s,让模拟时钟中断的线程启动完毕
    sleep(1);
    start_timer();

    coroutine_create(NULL, coroutine_fun, "c1", 2);
    coroutine_create(NULL, coroutine_fun, "c2", 1);
    coroutine_create(NULL, coroutine_fun, "c3", 1);

    getchar();

    return 0;
}