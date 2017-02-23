#include "include.h"

void semaphore()
{
    int nThread = 2;
    int nSemaphore = 1;
    int nRet = -1;
    pthread_t threadIDs[nThread];
    nRet = sem_init(&sem, 0, nSemaphore);
    if(nRet == -1)
    {
        perror("Semaphore intialization failed!!!\n");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < nThread; ++i)
    {
        nRet = pthread_create(&threadIDs[i], NULL, thread, NULL);
        if(nRet != 0)
        {
            perror("pthreas_create failed!!!\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i < nThread; ++i)
    {
        nRet = pthread_join(threadIDs[i], NULL);
        if(nRet != 0)
        {
            printf("Threan %d join failed!!!\n", i);
            exit(EXIT_FAILURE);
        }
    }

    sem_destroy(&sem);
}

