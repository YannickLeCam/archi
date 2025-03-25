#include <stdio.h>
//mmap
#include <sys/mman.h>
//pour open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>





int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Il manque un argument correspondant a un nom de fichier \n");
        exit(1);
    }
    int fd;
    if((fd=open(argv[1],O_RDONLY))==-1)
    {
        perror("OPEN_ERROR");
        exit(1);
    }

    struct stat st;
    fstat(fd,&st);
    char* retour_mmap;
    if((retour_mmap=mmap(NULL,st.st_size,PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED)
    {
        perror("MMAP_ERROR");
        exit(1);
    }
    char buf[1024];
    int j=0;//tete de lecture de buffeur

    for (int i = 0; i < st.st_size; i++)
    {
        if(retour_mmap[i]=='\n')
        {
            j=0;
        }else
        {
            buf[j]=retour_mmap[i];
            j++;
        }
    }
    buf[j]='\0';

    printf("la derniere ligne du fichier est : \n \"%s\" \n",buf);
    

    return 0;
}