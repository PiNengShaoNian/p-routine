#include "common.h"
#include "coroutine.h"
#include "sched.h"

coroutine_t *coroutines[TASK_SIZE] = {0};

extern coroutine_t *current_coroutine;

static int find_free_id()
{
    for (int i = 0; i < TASK_SIZE; ++i)
    {
        if (coroutines[i] == NULL)
            return i;
    }

    return -1;
}

int coroutine_create(int *pid, coroutine_func_t entry, char *name)
{
    int id;
    coroutine_t *coroutine;

    id = find_free_id();
    if (id == -1)
    {
        ERROR_PRINT("number of coroutine reaches maximum\n");
        return -1;
    }

    coroutine = malloc(sizeof(coroutine_t));
    if (coroutine == NULL)
    {
        ERROR_PRINT("malloc failed\n");
        return -1;
    }

    coroutine->state = COROUTINE_INIT;
    coroutine->id = id;
    coroutine->fun = entry;
    coroutine->esp3 = malloc(C_STACK_SIZE) + C_STACK_SIZE;

    memcpy(coroutine->name, name, strlen(name));

    coroutines[id] = coroutine;
    if (pid != NULL)
        *pid = id;

    coroutine->state = COROUTINE_READY;

    return 0;
}

void coroutine_exit()
{
    if (current_coroutine == NULL)
    {
        ERROR_PRINT("current_coroutine can't be NULL\n");
        return;
    }

    for (int i = 0; i < TASK_SIZE; ++i)
    {
        coroutine_t *tmp = coroutines[i];
        if (tmp == NULL || tmp != current_coroutine)
            continue;

        coroutines[i] = NULL;

        INFO_PRINT("coroutine exit: %s\n", tmp->name);

        free(tmp->esp3 - C_STACK_SIZE);
        free(tmp);
    }

    sched();
}
