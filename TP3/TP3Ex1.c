#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>



int compteur=0;

sem_t semaphore;

void* f(void* arg)
{
    sem_wait(&semaphore);//P()

    for(int i=0;i<1000000;i++)
    {
        compteur++;
    }

    sem_post(&semaphore);//V()
    return NULL;
}

int main()
{   
    pthread_t tid[4];
    sem_init(&semaphore,1,1);
    for(int i=0;i<4;i++)
    {
        pthread_create(&tid[i],NULL,f,NULL);        
    }


    for(int i=0;i<4;i++)
    {
        pthread_join(tid[i],NULL);
    }


    printf("Le compteur est : %d \n",compteur);

}