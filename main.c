#include <stdio.h>
#include "coroutine.h"
#include "sched.h"

void *coroutine_fun_1(void *arg)
{
    for (int i = 0; i < 10; ++i)
    {
        printf("#1 %d\n", i);
    }
}

int main()
{
    coroutine_create(NULL, coroutine_fun_1, "c1");

    sched();

    return 0;
}