#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
//Fonctionne comme il faut
#define SIZE 10 //vu dans l'ennoncé

typedef struct arg
{
    int tache_a_faire;
    pthread_barrier_t* bar_prec;
    pthread_barrier_t* bar_suiv;
}arg;


void * f (void* args)
{
    arg* a;
    a= args;
    printf("Je dois faire la tache %d j'att \n",a->tache_a_faire);
    if(a->bar_prec!=NULL)
    {
        pthread_barrier_wait(a->bar_prec);
    }

    printf("J'ai fais la tache %d j'attends \n",a->tache_a_faire);

    if(a->bar_suiv!=NULL)
    {
        pthread_barrier_wait(a->bar_suiv);
    }
    printf("J'ai fais la tache %d j'ai finis d'attendre \n",a->tache_a_faire);
    pthread_exit(NULL);
}

int main()
{
    printf("init tab_tache \n");
    int tab_tache[SIZE]={1,21,22,31,32,33,34,41,42,5};
    printf("Init tab_bar \n");
    pthread_barrier_t tab_bar[10];
    printf("Init des barriere \n");
    //Init des barrière qui sont unique pour la plus part des 
    //éléments car chaqu'un a des condition qui lui sont propre
    
    pthread_barrier_init(&tab_bar[0],NULL,3);// 1 21 22
    pthread_barrier_init(&tab_bar[1],NULL,4);//21 31 32 33
    pthread_barrier_init(&tab_bar[2],NULL,2);//22 34
    pthread_barrier_init(&tab_bar[3],NULL,4);//31 32 33 41
    pthread_barrier_init(&tab_bar[4],NULL,2);//34 42
    pthread_barrier_init(&tab_bar[5],NULL,3);//41 42 5

    printf("init tabarg \n");
    arg *tabarg=malloc(SIZE*sizeof(arg));
    printf("init tabtid \n");
    pthread_t *tid = malloc(SIZE*sizeof(pthread_t));

    //On doit init chaque thread avec tous des barrieres unique 

    for (int i=0;i<SIZE;i++)
    {
        tabarg[i].tache_a_faire=tab_tache[i];
        if(tab_tache[i]==1)
        {
            tabarg[i].bar_prec=NULL;
            tabarg[i].bar_suiv=&tab_bar[0];
        }else if (tab_tache[i]==21)
        {
            tabarg[i].bar_prec=&tab_bar[0];
            tabarg[i].bar_suiv=&tab_bar[1];
        }else if (tab_tache[i]==22)
        {
            tabarg[i].bar_prec=&tab_bar[0];
            tabarg[i].bar_suiv=&tab_bar[2];
        }else if (tab_tache[i]>30&&tab_tache[i]<34)
        {
            tabarg[i].bar_prec=&tab_bar[1];
            tabarg[i].bar_suiv=&tab_bar[3];
        }else if (tab_tache[i]==34)
        {
            tabarg[i].bar_prec=&tab_bar[2];
            tabarg[i].bar_suiv=&tab_bar[4];
        }else if (tab_tache[i]==41)
        {
            tabarg[i].bar_prec=&tab_bar[3];
            tabarg[i].bar_suiv=&tab_bar[5];
        }else if (tab_tache[i]==42)
        {
            tabarg[i].bar_prec=&tab_bar[4];
            tabarg[i].bar_suiv=&tab_bar[5];
        }else if (tab_tache[i]==5)
        {
            tabarg[i].bar_prec=&tab_bar[5];
            tabarg[i].bar_suiv=NULL;
        }
        pthread_create(&tid[i],NULL,f,&tabarg[i]);
    }

    for (int i = 0; i < SIZE; i++)
    {
        pthread_join(tid[i],NULL);
    }
    
    return 0;

}
