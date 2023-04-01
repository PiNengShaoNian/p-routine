#include "common.h"
#include "coroutine.h"

#include <signal.h>

extern coroutine_t *coroutines[TASK_SIZE];

static void *coroutine_idle(void *arg)
{
    while (1)
    {
        INFO_PRINT("idle\n");
        sleep(1);
    }
}

void init()
{
    coroutine_create(NULL, coroutine_idle, "idle");
}