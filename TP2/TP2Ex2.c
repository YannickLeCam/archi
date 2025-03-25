#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int compteur_alnum=0;
int compteur_punct=0;
int compteur_space=0;
int compteur_autre=0;

pthread_mutex_t mutex_alnum = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_punct = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_space = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_autre = PTHREAD_MUTEX_INITIALIZER;


void* f1(void* arg)
{
    int fd=open((const char*)arg,O_RDONLY);
    if(fd==-1)
    {
        perror("OPEN_ERROR");
        exit(1);
    }
    char buf[2];
    int alnum_loc=0;
    int punct_loc=0;
    int space_loc=0;
    int autre_loc=0;
    while(read(fd,buf,1)!=0)
    {
        if(isalnum(buf[0]))
        {
            alnum_loc++;
        }
        else if(ispunct(buf[0]))
        {
            punct_loc++;
        }
        else if(isspace(buf[0]))
        {
            space_loc++;
        }
        else
        {
            autre_loc++;
        }         
    }
    pthread_mutex_lock(&mutex_alnum);
    compteur_alnum=compteur_alnum+alnum_loc;
    pthread_mutex_unlock(&mutex_alnum);
            
    pthread_mutex_lock(&mutex_punct);
    compteur_punct=compteur_punct+punct_loc;
    pthread_mutex_unlock(&mutex_punct);

    pthread_mutex_lock(&mutex_space);
    compteur_space=compteur_space+space_loc;
    pthread_mutex_unlock(&mutex_space);

    pthread_mutex_lock(&mutex_autre);
    compteur_autre=compteur_autre+autre_loc;
    pthread_mutex_unlock(&mutex_autre);

    close(fd);
    return NULL;
}

int main(int argc,char * argv[])
{   


    pthread_t tid[argc-1];


    for(int i=0;i<argc-1;i++)
    {
        pthread_create(&tid[i],NULL,f1,argv[i+1]); 
    }


    for(int i=0;i<argc-1;i++)
    {
        pthread_join(tid[i],NULL);
    }
//@@@@dsqkjhsq%%

    printf("Le compteur alnum est : %d \n",compteur_alnum);
    printf("Le compteur punct est : %d \n",compteur_punct);
    printf("Le compteur space est : %d \n",compteur_space);
    printf("Le compteur autre est : %d \n",compteur_autre);
    return 0;
}
