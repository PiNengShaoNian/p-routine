#include "common.h"
#include "sched.h"
#include <pthread.h>
#include <signal.h>

#define CLOCK_RATE 300000

static pthread_mutex_t mutex;
static pthread_cond_t cond;

static void sig_handler(int sig)
{
    sigset_t sigset;
    sigemptyset(&sigset);

    switch (sig)
    {
    case SIGUSR1:
        sigaddset(&sigset, SIGUSR1);
        sigprocmask(SIG_UNBLOCK, &sigset, NULL);

        sched();
        break;
    default:
        ERROR_PRINT("signals not registered for processing\n");
        exit(-1);
    }
}

static void *thread_timer(void *arg)
{
    INFO_PRINT("start simulate CPU clock interrupts\n");

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    INFO_PRINT("start generate interrupts\n");
    while (true)
    {
        kill(getpid(), SIGUSR1);

        usleep(CLOCK_RATE);
    }
}

void init_timer()
{
    pthread_t tid;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&tid, &attr, thread_timer, NULL);

    pthread_attr_destroy(&attr);

    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
    {
        ERROR_PRINT("failed to register signal processing function\n");
        exit(-1);
    }
}

void start_timer()
{
    pthread_cond_broadcast(&cond);
}
