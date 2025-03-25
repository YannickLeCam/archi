#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
//Fonctionne comme il faut ! 
typedef struct arg
{
    int n;
    int p;
    pthread_barrier_t* adr_bar_prec;
    pthread_barrier_t* adr_bar_suiv;
}arg;


void * f (void* args)
{
    arg* a;
    a= args;
    char * str=malloc(sizeof(char)*a->p);
    for (int i = 0; i < a->p; i++)
    {
        str[i]='#';
    }
    if(a->adr_bar_prec!=NULL){
        pthread_barrier_wait(a->adr_bar_prec); //On check si le precedent a fini
    }
    printf("%s \n",str);
    if(a->adr_bar_suiv!=NULL){
        pthread_barrier_wait(a->adr_bar_suiv);//On signal au suivant que c'est terminé 
    }
}

void * f1 (void* args)
{
    arg* a;
    a= args;
    char * str=malloc(sizeof(char)*a->p);
    for (int i = 0; i < a->p; i++)
    {
        str[i]='-';
    }
    if(a->adr_bar_prec!=NULL){
        pthread_barrier_wait(a->adr_bar_prec);
    }
    printf("%s \n",str);
    if(a->adr_bar_suiv!=NULL){
        pthread_barrier_wait(a->adr_bar_suiv);
    }
}

int main(int argc,char * argv[])
{
    printf("init ? \n");
    int n=atoi(argv[1]);
    int p=atoi(argv[2]);
    printf("init tabarg \n");
    arg *tabarg=malloc(n*sizeof(arg));
    printf("init tabtid \n");
    pthread_t *tid = malloc(n*sizeof(pthread_t));
    pthread_barrier_t bar_tab[n];


    for (int i=0;i<n;i++)
    {

        tabarg[i].p=p;
        tabarg[i].n=i;
        if(i%2==0)
        {
            pthread_barrier_init(&bar_tab[i],NULL,2);
            //Il faut pas mettre de barriere suivant pour le dernier element car sinon ca tourne dans le vide
            if(i==n-1)
            {
                tabarg[i].adr_bar_suiv=NULL;
            }else
            {
                tabarg[i].adr_bar_suiv=&bar_tab[i];
            }
            //Obliger d'initialiser l'élément 0 car il n'a rien avant qui le bloque
            if(i==0)
            {
                tabarg[i].adr_bar_prec=NULL;
            }
            else
            {
                tabarg[i].adr_bar_prec=&bar_tab[i-1];
            }
            
            pthread_create(&tid[i],NULL,f,&tabarg[i]);
        }
        else
        {
            pthread_barrier_init(&bar_tab[i],NULL,2);
            
            if(i==n-1)
            {
                tabarg[i].adr_bar_suiv=NULL;
            }else
            {
                tabarg[i].adr_bar_suiv=&bar_tab[i];
            }
            tabarg[i].adr_bar_prec=&bar_tab[i-1];        
            pthread_create(&tid[i],NULL,f1,&tabarg[i]);
        }
    }


    for (int i = 0; i < n; i++)
    {
        pthread_join(tid[i],NULL);
    }
    
    return 0;

}
