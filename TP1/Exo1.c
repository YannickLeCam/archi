#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct arg
{
    int t;
    int p;
}arg;


void * f (void* args)
{
    arg* a;
    a= args;
    int* retour=malloc(sizeof(int));
    int r=0;
    printf("le t est : %d et le p est : %d \n",a->t,a->p);
    for(int i=0;i<=a->p;i++)
    {
        r+=(a->t-1)*a->p+i;
    }
    *retour=r;
    pthread_exit(retour);
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


    for (int i=0;i<n;i++)
    {
        tabarg[i].p=p;
        tabarg[i].t=i+1;


        pthread_create(&tid[i],NULL,f,&tabarg[i]);
    }

    int* r;
    int sum=0;
    for (int i = 0; i < n; i++)
    {
        pthread_join(tid[i],(void **)&r);
        printf("r est égal a : %d \n",*r);
        sum += *r;
    }

    printf(" la somme est de : %d \n",sum);
    
    return 0;

}
