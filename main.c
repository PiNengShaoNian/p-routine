#include <stdio.h>
#include "coroutine.h"
#include "sched.h"

void *coroutine_fun_1(void *arg)
{
    coroutine_t *self = (coroutine_t *)arg;
    for (int i = 0; i < 10; ++i)
    {
        printf("%s %d\n", self->name, i);
    }
}

int main()
{
    coroutine_create(NULL, coroutine_fun_1, "c1");

    sched();

    return 0;
}