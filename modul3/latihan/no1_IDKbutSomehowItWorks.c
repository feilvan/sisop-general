#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int faktorial (int a);
void *start_faktorial (void *arg);
void BubbleSort(int a[], int array_size);

int main (int argc, char *argv[]) {
    pthread_t tid[argc];
    pid_t child;
    int int_arg[argc];
    for(int i=1; i<argc; i++){
        int_arg[i] = atoi(argv[i]);
    }
    BubbleSort(int_arg,argc);
    int err, j=1;
	while(j<argc) // loop sejumlah thread
	{
        err=pthread_create(&tid[j],NULL,&start_faktorial,(void*)&int_arg[j-1]); //membuat thread
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
    for(int k=1; k<argc; k++){
        pthread_join(tid[k],NULL);
    }
	exit(0);
	return 0;
}

void *start_faktorial (void *arg){
    printf("%d! = %d\n", *(int*)arg, faktorial(*(int*)arg));
}

int faktorial (int a) {
    if(a<1){
        return 1;
    }
    else{
        return a * faktorial (a-1);
    }
}

void BubbleSort(int a[], int array_size)
{
    int i, j, temp;
    for (i = 0; i < (array_size - 1); ++i)
    {
        for (j = 0; j < array_size - 1 - i; ++j )
        {
            if (a[j] > a[j+1])
            {
                temp = a[j+1];
                a[j+1] = a[j];
                a[j] = temp;
            }
        }
    }
}
