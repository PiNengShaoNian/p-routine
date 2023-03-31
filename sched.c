#include "common.h"
#include "coroutine.h"
#include "sched.h"

extern coroutine_t *coroutines[TASK_SIZE];

coroutine_t *current_coroutine = NULL;

static coroutine_t *find_ready_coroutine()
{
    for (int i = 0; i < TASK_SIZE; ++i)
        if (coroutines[i] != NULL && coroutines[i]->state == COROUTINE_READY)
            return coroutines[i];

    return NULL;
}

void sched()
{
    coroutine_t *ready_coroutine;

    ready_coroutine = find_ready_coroutine();

    if (ready_coroutine == NULL)
    {
        INFO_PRINT("no tasks to schedule\n");
        return;
    }

    current_coroutine = ready_coroutine;

    ready_coroutine->state = COROUTINE_RUNNING;

    __asm__("mov %%rsp, 0(%%rcx);" // 保存栈
            "mov %%rcx, %%rsp;"    // 切栈
            "callq %%rax;"         // 切到协程的执行流
            "pop %%rsp;"           // 恢复rsp
            "callq %%rbx;"         // coroutine_exit()
            "" ::"a"(ready_coroutine->fun),
            "c"(ready_coroutine->esp3 - 8), "b"(coroutine_exit)
            : "memory");
}