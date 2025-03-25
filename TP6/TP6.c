#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdnoreturn.h>

#define DELAI_MAX        1000           // en ms

#define CHK(prim)   do { if ((prim) == -1) { raler (#prim); } } while (0)
#define TCHK(exp)   do { if ((errno=(exp)) > 0) { raler (#exp); } } while (0)


typedef struct arguments
{
    int id;
    pthread_barrier_t* prec;
    pthread_barrier_t* suiv;
    pthread_barrier_t* gen;
    pthread_mutex_t* mutex;
}arguments;

noreturn void raler (const char *msg)
{
    perror (msg) ;
    exit (1) ;
}

void attente_aleatoire (unsigned int *seed, int max_en_ms)
{
    int delai_en_ms ;

    delai_en_ms = max_en_ms * (rand_r (seed) / (double) RAND_MAX) ;
    usleep (delai_en_ms * 1000) ;       // convertir en micro-secondes
}

void *un_thread (void *arg)
{
    unsigned int seed ;         // pour rand_r
    int thrnum ;                // mon numéro de thread
    char nombre [100] ;         // numéro de thread converti en ASCII

    arguments* args=arg;                // pour -Wall -Wextra -Werror

    thrnum = args->id;                // TODO remplacer 0 par numéro de thread

    // on initialise seed avec notre numéro de thread pour avoir des
    // résultats reproductibles
    seed = thrnum ;
    attente_aleatoire (&seed, DELAI_MAX) ;

    // TODO attendre l'ordre d'afficher le '+'
    pthread_mutex_lock(args->mutex);

    CHK (write (1, "+", 1)) ;
    pthread_mutex_unlock(args->mutex);
    // TODO attendre l'ordre d'afficher mon numéro
    pthread_barrier_wait(args->gen);
    pthread_barrier_wait(args->gen);
    if(args->prec!=NULL){
        pthread_barrier_wait(args->prec);//On signal au suivant que c'est terminé 
    }
    (void) snprintf (nombre, sizeof nombre, "%d ", args->id) ;
    CHK (write (1, nombre, strlen (nombre))) ;
    if(args->suiv!=NULL){
        pthread_barrier_wait(args->suiv);//On signal au suivant que c'est terminé 
    }
    return NULL ;
}




int main (int argc, char *argv [])
{
    int nthreads ;
    unsigned int seed = 0 ;             // pour rand_r

    if (argc != 2 || (nthreads = atoi (argv [1])) <= 0)
    {
        fprintf (stderr, "usage: %s nb-threads\n", argv [0]) ;
        fprintf (stderr, "\tavec nb-threads > 0\n") ;
        exit (1) ;
    }

    pthread_t tid [nthreads] ;
    pthread_mutex_t mu;
    pthread_mutex_init(&mu,NULL);
    pthread_barrier_t tab_bar[nthreads];
    pthread_barrier_t bar_gen;
    pthread_barrier_init(&bar_gen,NULL,nthreads+1);//pour que le main puisse donner le go aux autre
    arguments tab_arg [nthreads];
    for (int i = 0; i < nthreads; i++)
    {
        pthread_barrier_init(&tab_bar[i],NULL,2);
    }
    
    pthread_mutex_lock(&mu);
    for (int i = 0 ; i < nthreads ; i++)
    {
        
        tab_arg[i].id=i;
        tab_arg[i].mutex=&mu;
        tab_arg[i].gen=&bar_gen;
        if (i==0)
        {
            tab_arg[i].prec=NULL;
            tab_arg[i].suiv=&tab_bar[i];
        }
        else if (i==nthreads-1)
        {
            tab_arg[i].suiv=NULL;
            tab_arg[i].prec=&tab_bar[i-1];
        }else
        {
            tab_arg[i].prec=&tab_bar[i-1];
            tab_arg[i].suiv=&tab_bar[i];          
        }  
        TCHK (pthread_create (&tid [i], NULL, un_thread, &tab_arg[i]));
    }

    attente_aleatoire (&seed, DELAI_MAX) ;
    
    CHK (write (1, "debut\n", 6)) ;
    pthread_mutex_unlock(&mu);
    // TODO donner l'ordre aux threads d'écrire leur '+'
    pthread_barrier_wait(&bar_gen);
    CHK (write (1, "\n", 1)) ;          // doit être affiché après tous les '+'
    pthread_barrier_wait(&bar_gen);

    // TODO prévenir chaque thread à son tour d'afficher son numéro

    for (int i = 0 ; i < nthreads ; i++)
    {
        // attendre la terminaison de chaque thread
        TCHK (pthread_join (tid [i], NULL)) ;

    }

    CHK (write (1, "\n", 1)) ;

    exit (0) ;
}
