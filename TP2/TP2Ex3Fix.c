#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Probleme que je ne pourrais corrigé surement a cause de mon ordi 
//Il y a des prio qui ne sont pas du tout respecté 
typedef struct arguments
{
    int id;
    int X;
    int N;
    pthread_mutex_t* tab_baguette;

}arguments;


void* gaucher(void* arg)
{
    arguments* arguments=arg;
    int indice_baguette1=arguments->id;
    int indice_baguette2;
    if (arguments->id==arguments->N-1)
    {
        indice_baguette2=0;
    }else
    {
        indice_baguette2=arguments->id+1;
    }
    
    
    
    for (int i = 0; i < arguments->X; i++)
    {   
        printf("Je suis le thread %d et je commence la tache %d\n",arguments->id,i);
        printf("je suis le thread %d et je veux prendre la baguette %d\n",arguments->id,indice_baguette1);
        pthread_mutex_lock(&arguments->tab_baguette[indice_baguette1]);
        printf("je suis le thread %d et j'ai pris la baguette %d\n",arguments->id,indice_baguette1);
        //sleep(2);//J'ai tricher pas de random_r pour cet exo
        printf("Je suis le thread %d et je veux prendre la baguette %d\n",arguments->id,indice_baguette2);
        pthread_mutex_lock(&arguments->tab_baguette[indice_baguette2]);
        printf("je suis le thread %d et j'ai pris la baguette %d\n",arguments->id,indice_baguette2);
        //sleep(2);
        printf("Je suis le thread %d et je mange et je vais libérer les baguettes %d %d et terminer la ma tache %d\n",arguments->id,indice_baguette1,indice_baguette2,i);
        pthread_mutex_unlock(&arguments->tab_baguette[indice_baguette1]);
        pthread_mutex_unlock(&arguments->tab_baguette[indice_baguette2]);

    }
    printf("JE SUIS LE THREAD %d ET J'AI FINIS \n",arguments->id);
    return NULL;
}
void* droitier(void* arg)
{
    arguments* arguments=arg;
    int indice_baguette2=0;
    int indice_baguette1=1;
    for (int i = 0; i < arguments->X; i++)
    {   
        printf("Je suis le thread %d et je commence la tache %d\n",arguments->id,i);
        printf("je suis le thread %d et je veux prendre la baguette %d\n",arguments->id,indice_baguette1);
        pthread_mutex_lock(&arguments->tab_baguette[indice_baguette1]);
        printf("je suis le thread %d et j'ai pris la baguette %d\n",arguments->id,indice_baguette1);
        sleep(2);//J'ai tricher pas de random_r pour cet exo
        printf("Je suis le thread %d et je veux prendre la baguette %d\n",arguments->id,indice_baguette2);
        pthread_mutex_lock(&arguments->tab_baguette[indice_baguette2]);
        printf("je suis le thread %d et j'ai pris la baguette %d\n",arguments->id,indice_baguette2);
        sleep(2);
        printf("Je suis le thread %d et je mange et je vais libérer les baguettes %d %d et terminer la ma tache %d\n",arguments->id,indice_baguette1,indice_baguette2,i);
        pthread_mutex_unlock(&arguments->tab_baguette[indice_baguette1]);
        pthread_mutex_unlock(&arguments->tab_baguette[indice_baguette2]);
    }
    

    return NULL;
}

int main(int argc,char * argv[])
{   
    int N =atoi(argv[1]);//le nb de bavards ET baguettes dispo
    int X=atoi(argv[2]);//Le nb de fois que chaque bavard vont ittérer leur trucs

    pthread_mutex_t* tab_baguette=calloc(N,sizeof(pthread_mutex_t));
    
    arguments* tab_argument=calloc(N,sizeof(arguments));
    //INIT DU TABLEAU DE BAGUETTE ET D'ARGUMENTS
    
    for (int i = 0;i<N;i++)
    {
        pthread_mutex_init(&tab_baguette[i],NULL);
        tab_argument[i].id=i;
        tab_argument[i].X=X;
        tab_argument[i].N=N;
    }
    pthread_t tid[argc-1];


    for(int i=0;i<N;i++)
    {
        tab_argument[i].tab_baguette=tab_baguette;
        if(i==0)
        {
            printf("je suis passer par la \n");
            pthread_create(&tid[i],NULL,droitier,&tab_argument[i]);
        }
        pthread_create(&tid[i],NULL,gaucher,&tab_argument[i]); 
    }


    for(int i=0;i<N;i++)
    {
        pthread_join(tid[i],NULL);
    }
    free(tab_argument);
    free(tab_baguette);
    return 0;
}
