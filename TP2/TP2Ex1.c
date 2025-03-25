#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>



int compteur=0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* f(void* arg)
{
    pthread_mutex_lock(&mutex);

    for(int i=0;i<1000000;i++)
    {
        compteur++;
    }

    pthread_mutex_unlock(&mutex);
}

int main()
{   
    pthread_t tid[4];

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