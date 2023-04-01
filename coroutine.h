#ifndef P_ROUTINE_COROUTINE_H
#define P_ROUTINE_COROUTINE_H

#define TASK_SIZE 32

// 协程的栈大小4k
#define C_STACK_SIZE (4096 << 10)

typedef void *(*coroutine_func_t)(void *);

typedef enum coroutine_state
{
    COROUTINE_INIT,     // 初始化
    COROUTINE_RUNNING,  // 执行
    COROUTINE_READY,    // 就绪
    COROUTINE_BLOCKED,  // 阻塞
    COROUTINE_SLEEPING, // 睡眠
    COROUTINE_WAITING,  // 等待
    COROUTINE_DIED,     // 死亡
} coroutine_state_t;

typedef struct coroutine
{
    coroutine_state_t state;
    coroutine_func_t fun;
    char *esp3;
    char name[16]; // 协程名称
    int id;        // 协程id
    int priority;  // 调度优先级，越大越先调度，获得的时间片越多
    int counter;   // 运行时间片
} coroutine_t;

int coroutine_create(int *pid, coroutine_func_t entry, char *name, int priority);

void coroutine_exit();

#endif // P_ROUTINE_COROUTINE_H