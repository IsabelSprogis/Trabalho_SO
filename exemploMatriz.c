#include <stdio.h>
#include <pthread.h>

struct matriz_arg
{ 
    int LINHAS;
    int COLUNAS;
    float *matriz;

    int thread_id;
    int inicio_linha;
    int inicio_coluna;
    int fim_linha;
    int fim_coluna;
};

void *run(void *thread_arg) 
{ 
    struct matriz_arg *mydata; 
    mydata = (struct matriz_arg *) thread_arg;

    int i, j;

    printf("\nThread id:%d\n", mydata->thread_id);

    for ( i = mydata->inicio_linha; 
          i <= mydata->fim_linha; 
          i++)
    {
        for (j = mydata->inicio_coluna;
             j <= mydata->fim_coluna; 
             j++)
        {
          printf("matriz[%d][%d] = %.2f\t", i, j, 
                  mydata->matriz[ i * mydata->COLUNAS + j ]);
        }

        printf("\n");
    }
    pthread_exit(NULL);
}

int main() 
{ 
    pthread_t thread_id[2]; 
    struct matriz_arg thread_data[2]; 

    float matriz[] = { 1,  2,  3, 4,
                       5,  6,  7, 8,
                       9, 10, 11, 12,
                      13, 14, 15, 16}; 



    thread_data[0].LINHAS = 4; 
    thread_data[0].COLUNAS = 4;
    thread_data[0].matriz = matriz;
    thread_data[0].thread_id = 0;
    thread_data[0].inicio_linha = 0;
    thread_data[0].fim_linha=1;
    thread_data[0].inicio_coluna=0;
    thread_data[0].fim_coluna=3;

    pthread_create(&thread_id[0],NULL,run,(void *)&thread_data[0]); 
    
    thread_data[1].LINHAS = 4;
    thread_data[1].COLUNAS = 4;
    thread_data[1].matriz = matriz;
    thread_data[1].thread_id = 1;
    thread_data[1].inicio_linha = 2;
    thread_data[1].fim_linha=3;
    thread_data[1].inicio_coluna=0;
    thread_data[1].fim_coluna=3;
    
    pthread_create(&thread_id[1],NULL,run,(void *)&thread_data[1]);
    
    pthread_join(thread_id[0],NULL); 
    pthread_join(thread_id[1],NULL);

    return 0; 
}



