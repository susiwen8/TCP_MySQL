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


int P(int id, int sem_id)
{
    struct sembuf sem;
    sem.sem_num  = id;
    sem.sem_op   = -1;
    sem.sem_flg = 0;

    if(semop(sem_id, &sem, 1) < 0)
    {
        perror("Mutex has a error from P\n");
        return SEMerr; 
    }
    return 0;
}

int V(int id, int sem_id)
{
    struct sembuf sem;
    sem.sem_num  = id;
    sem.sem_op   = 1;
    sem.sem_flg = 0;

    if(semop(sem_id, &sem, 1) < 0)
    {
        perror("Mutex has error from V\n");
        return SEMerr;
    }
    return 0;
}

int creationMutex()
{
    key_t key;
    int sem_id;
    //if((key = ftok(CleServer, 'S')) < 0)
    //    return CLEerr;
    sem_id = semget(key, N_CLIENTS, 0666 | IPC_CREAT);
    return sem_id;
}

int destructionMutex(int sem_id)
{
    return (semctl(sem_id, 0, IPC_RMID, 0));
}
