#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>



#define NB_THREAD 12
#define NB_JOB 5
#define NB_MACHINE_DISPO_INIT 5

int machine_dispo;


pthread_cond_t c=PTHREAD_COND_INITIALIZER;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

typedef struct args {
    int p;
    int id;
}args;


void * f (void* arg)
{
    args* a = arg;
    int seed=a->id;
    
    for(int i=0;i<=a->p;i++){
        srand(seed);
        int alea=rand_r(&seed)%3+1;
        pthread_mutex_lock(&m);
        while(machine_dispo==0)
        {
            pthread_cond_wait(&c,&m);
        }
        machine_dispo--;
        pthread_mutex_unlock(&m);
        printf("je suis le thread %d et je fais mon %d job et je dors %d\n",a->id,i,alea);
        sleep(alea);
        pthread_cond_signal(&c);
        machine_dispo++;
    }
    pthread_exit(NULL);
}


int main()
{

    pthread_t tid[NB_THREAD];
    args* tabarg=malloc(sizeof(args)*NB_THREAD);

    machine_dispo=NB_MACHINE_DISPO_INIT;
    

    for(int i=0;i<NB_THREAD;i++)
    {
        tabarg[i].p=NB_JOB;
        tabarg[i].id=i;
        pthread_create(&tid[i],NULL,f,&tabarg[i]);
    }
    

    for(int i=0;i<NB_THREAD;i++)
    {
        pthread_join(tid[i],NULL);

    }
    
    return 0;
}