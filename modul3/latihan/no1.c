#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int faktorial (int a) {
    if(a<1){
        return 1;
    }
    else{
        return a * faktorial (a-1);
    }
}

void *start_faktorial (void *arg){
    printf("%d! = %d\n", atoi((char*)arg), faktorial(atoi((char*)arg)));
}

int main (int argc, char *argv[]) {
    pthread_t tid[argc];
    pid_t child;
    int err, j=1;
	while(j<argc) // loop sejumlah thread
	{
		err=pthread_create(&tid[j],NULL,&start_faktorial,(void*)argv[j]); //membuat thread
		if(err!=0) //cek error
		{
			printf("\ncan't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("create thread %d success\n", j);
		}
		j++;
	}
    int k=1;
    while(k<argc)
	{
		pthread_join(tid[k],NULL);
        k++;
	}
	exit(0);
	return 0;
}