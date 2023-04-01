#include "common.h"
#include "proutine.h"

extern coroutine_t *coroutines[TASK_SIZE];

coroutine_t *current_coroutine = NULL;

static coroutine_t *find_ready_coroutine()
{
    coroutine_t *next = NULL;

    // 判断是不是所有任务都被调度了一次
    bool is_all_zero = true;
    bool is_null = true;

    for (int i = 1; i < TASK_SIZE; ++i)
    {
        coroutine_t *c = coroutines[i];

        if (c == NULL)
            continue;

        is_null = false;

        if (c->counter != 0)
        {
            is_all_zero = false;
            break;
        }
    }

    if (is_null)
        return coroutines[0];

    if (is_all_zero)
    {
        // 所有任务都被调度了一轮,重新赋值counter
        for (int i = 1; i < TASK_SIZE; ++i)
        {
            coroutine_t *tmp = coroutines[i];
            if (tmp != NULL)
                tmp->counter = tmp->priority;
        }

        // 重新设置counter后，再次查找可调度的任务
        return find_ready_coroutine();
    }

    for (int i = 1; i < TASK_SIZE; ++i)
    {
        coroutine_t *c = coroutines[i];

        if (c == NULL)
            continue;

        if (current_coroutine == c && COROUTINE_RUNNING == c->state)
            c->state = COROUTINE_READY;

        if (c->state != COROUTINE_READY)
            continue;

        if (next == NULL)
            next = c;
        else if (c->counter > next->counter)
            next = c;
    }

    if (next == NULL)
        next = coroutines[0];

    return next;
}

void sched()
{
    coroutine_t *ready_coroutine;

    if (current_coroutine != NULL &&
        current_coroutine->state != COROUTINE_SLEEPING &&
        current_coroutine->state != COROUTINE_BLOCKED)
        current_coroutine->state = COROUTINE_READY;

    ready_coroutine = find_ready_coroutine();

    current_coroutine = ready_coroutine;

    ready_coroutine->state = COROUTINE_RUNNING;

    __asm__("mov %%rsp, 0(%%rcx);" // 保存栈
            "mov %%rcx, %%rsp;"    // 切栈
            "callq %%rax;"         // 切到协程的执行流
            "pop %%rsp;"           // 恢复rsp
            "callq %%rbx;"         // coroutine_exit()
            "" ::"a"(ready_coroutine->fun),
            "c"(ready_coroutine->esp3 - 8), "D"(current_coroutine), "b"(coroutine_exit)
            : "memory");
}

void do_timer()
{
    if (current_coroutine == NULL)
        sched();

    if (current_coroutine != NULL && current_coroutine->counter > 0)
    {
        current_coroutine->counter--;
        return;
    }

    sched();
}