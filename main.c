#include <stdio.h>
#include "coroutine.h"
#include "sched.h"

extern void init();

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
    // coroutine_create(NULL, coroutine_fun, "c1");
    // coroutine_create(NULL, coroutine_fun, "c2");

    init();

    sched();

    return 0;
}