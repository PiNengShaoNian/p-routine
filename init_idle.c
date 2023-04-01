#include <signal.h>
#include "common.h"
#include "proutine.h"

static void *coroutine_idle(void *arg)
{
    while (1)
    {
        INFO_PRINT("idle\n");
        sleep(1);
    }
}

void init_idle()
{
    coroutine_create(NULL, coroutine_idle, "idle", 1);
}