#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*variaveis globais para poderem ser acessadas pelas threads*/
int M, N, T; /*em ordem Linhas, Colunas, Threads*/
float V; /*Valor a ser buscado*/
float **matriz;  /*Matriz a ser alocada dinamicamente */
int **matriz_aux; /*Matriz auxiliar a ser alocada dinamicamente e para printar as posicoes na ordem correta*/

/*funcao de cada thread para buscar por linha e alterar na matriz aux se achar o valor*/
void* busca(void* t){ 
	int k = *((int*) t); /*indice da thread*/
	int i;
	
	while(k < M){ /*enquanto as threads não atingirem o maximo de linhas*/
	
		for(i = 0; i < N; i++){ /*verifica se na matriz principal tem o valor*/
			if(matriz[k][i] == V)
				matriz_aux[k][i] == 1; /*coloca uma flag indicando a posicao na auxiliar*/
		}
		k = k+T; /*incrementa o indice da thread pra procurar na proxima linha onde as outras nao estiveram*/
	}
}

/*funcao para alocacao dinamica da matriz dado os parametros de seu tamanho*/
float **alocaMatriz (int M, int N){
    float **k; /*ponteiro para a matriz*/
    int i;

    k = (float**)calloc(M, sizeof(float*));/*aloca as linhas da matriz*/
    for(i = 0; i < M; i++){ /*aloca as colunas da matriz*/
        k[i] = (float*) calloc(N, sizeof(float));
    }
    return (k); /*retorna o ponteiro para a matriz*/
}

/*funcao para desalocar a matriz apos finalizar*/
float **liberaMatriz (int M, int N, float **mat)
{
  int  i;
  for (i=0; i<M; i++) free (mat[i]); /*libera as linhas da matriz*/
  free (mat);      /*libera a matriz*/
  return (NULL); /*retorna um ponteiro nulo*/
}


void preencheMatriz (FILE *arq, float **mat, int lin, int col){
    int linha, coluna;

    for(linha = 0; linha < lin; linha++){
        for(coluna = 0; coluna < col; coluna++){
                fscanf(arq, "%f", &mat[linha][coluna]);
        }
    }
}

int main()
{
    FILE *dados;
    char nome[31];
    int i, j, *t, rc;
    //pthread_t *thr;

    scanf("%d %d %d %f" , &M,&N,&T,&V);
    scanf("%s", nome); /*recebe o nome do arquivo*/

    dados = fopen(nome,"r"); /*abre o arquivo para leitura dos dados*/
    if(dados == NULL){       /*caso o arquivo nao exista encerra e notifica*/
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	    return -1;
    }
	
	//thr = malloc(M *sizeof(float));
	pthread_t thr[M];
    matriz = alocaMatriz(M, N);
    matriz_aux = alocaMatriz(M, N);
    preencheMatriz(dados,matriz, M, N);
    
    /*criacao das threads*/
    for(i = 0; i< M; i++){
    	t = malloc(sizeof(float));
    	*t = i;
    	rc = pthread_create(&thr[i], NULL, busca, t);
    	if (rc != 0) {
            printf("Erro ao criar a thread");
            exit(-1);
        }
	}
	
	for (i=0; i < M i++) {
        pthread_join(thr[i], NULL);
    }
    
    /*printa as posicoes aonde foram encontrados os valores*/
     for(i = 0; i < M; i++){
        for(j = 0; j< N; j++){
           if(matriz_aux[i][j] == 1)
           	printf("%d %d", i, j);
       }
    }
    
    matriz = liberaMatriz(M, N, matriz);
    matriz_aux = liberaMatriz(M, N, matriz_aux);

    return 0;
}
