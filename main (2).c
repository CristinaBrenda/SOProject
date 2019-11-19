
/* LOGICA
 * para cada arquivo de entrada usar uma thread
 * usar essa mesma thread para ordenar os valores, em formato de vetor
 * preencher os vetores com 0,
 * gravar os vetores na memoria,
 * utilizar outra thread para gravar os vetores no arquivo 
 */
/* TESTE 1 DO PROJETO*/
/* EXEMPLO DE EXECUÇAO
 * ./buildmatrix 2 arq1.dat arq2.dat arq3.dat -o saida.dat
 * numero de arquivos de entrada possiveis 1 ate n   
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include <math.h>
#include <time.h>

int **lin;

typedef struct qtde{
    int linha;
    long qtdeInt;
    long colInicial;
	long colFinal;
}Qtde;


long tam_vet(FILE *arq){
    
    long length; 
    fseek(arq,0,SEEK_END);	
    length = ftell(arq); 
    fseek(arq,0,SEEK_SET); 
    return length/4;
}
void ler_arqs(FILE *arq,int *col,long tam){
    
    int j;
    col = (int *) malloc(tam *sizeof(int));
    for ( j=0; j<tam;j++){
        fread(&col[j],sizeof(int),1,arq);
        printf("[%d] ", col[j]);
    }
    
}
void ordenar_vet2(int num_lin,long vetor_tam){
	
	printf("%ld\n",vetor_tam);
	
	for (int i = 0; i < vetor_tam; i++){
        printf("%d ",lin[num_lin][i]);
        
    }
	
	printf("\n");
    int aux =0;
    int i;
    for (i = 0; i < vetor_tam; i++){
        for (int j = 0; j < vetor_tam; j++){
            if (lin[num_lin][i] < lin[num_lin][j])
            {
            //troca dos valores  
                aux = lin[num_lin][i];
                lin[num_lin][i] = lin[num_lin][j];
                lin[num_lin][j] = aux;
            }
        }
    }
    for(int p =0; p< vetor_tam; p++){
        printf("[%d] ",lin[num_lin][p]);
    } 
    /*
    for(int m = 0; m< qtd_arq; m++){
        lin[m] = malloc(sizeof(col));
        lin[m] = col;
    }
     * */
}

void merge(int arr[], int l, int m, int r) 
{ 
	
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
	
	for (int i = l; i <= r; i++){
        printf("%d ",arr[i]); 
    }
	//printf("\n");
} 

void *ordenar_vet(void *data){
    Qtde *qtde = (Qtde *)data;
	
	long vetor_tam = qtde->qtdeInt;
	int num_lin = qtde->linha;
	
	//printf("valor : %ld \t",qtde->colInicial);
	//printf("%ld \t",qtde->colFinal);
			
		
	
	//printf("%ld\n",vetor_tam);
	
	for (int i = qtde->colInicial; i < qtde->colFinal; i++){
        //printf("%d ",lin[num_lin][i]); 
    }
	
	//printf("\n");
    int aux =0;
    int i;
    for (i = qtde->colInicial; i < qtde->colFinal; i++){
        for (int j = qtde->colInicial; j < qtde->colFinal; j++){
            if (lin[num_lin][i] < lin[num_lin][j])
            {
            //troca dos valores  
                aux = lin[num_lin][i];
                lin[num_lin][i] = lin[num_lin][j];
                lin[num_lin][j] = aux;
            }
        }
    }
    for(int p =qtde->colInicial; p< qtde->colFinal; p++){
        //printf("[%d] ",lin[num_lin][p]);
    } 
    //printf("\n");
    /*
    for(int m = 0; m< qtd_arq; m++){
        lin[m] = malloc(sizeof(col));
        lin[m] = col;
    }
     * */
}

int main(int argc, char *argv[]) {
    
    
    int qtd_arq = argc - 4;
    //int *lin[qtd_arq];
    int i=0, j=0, m = 0; 
    int num_th = atoi(argv[1]); //na pos[1] esta o num de threads
    int *col;
    long vetor_tam[qtd_arq]; //vetor armazena a qtd de elementos de cada arquivo
    lin = (int **) malloc (qtd_arq*sizeof(int*));
    int max_vetor = 0 ;
    clock_t tempo[2];
   
    //prints para teste
    printf("QTD de arq = %d\n", qtd_arq);
    printf("Num de threads %d\n",num_th);
    
	
	pthread_t t[num_th];
    
	//printf("valor : %d\n",num_th%(argc-3));
    
	
        tempo[0] = clock();
    //ler arquivo
    for(int arquivo = 0; arquivo < (argc-4) ; arquivo++){
        FILE *arq = fopen(argv[arquivo+2],"rb");
        rewind(arq);
        
        //long tam;
        vetor_tam[arquivo] = tam_vet(arq);
        //tam = tam_vet(arq);
        if(vetor_tam[arquivo] > max_vetor){
            max_vetor = vetor_tam[arquivo];
        }
        
               
        lin[arquivo] = (int *) malloc(vetor_tam[arquivo] *sizeof(int));
        for (j=0; j<vetor_tam[arquivo];j++){
            fread(&lin[arquivo][j],sizeof(int),1,arq);
            //printf("[%d] ", col[j]);
        }
        
        printf("\n");
        //ordenar vetor
	
      
        
        //for(i = 0; i< qtd_arq; i++){
        //    lin[i] = col;
       // }
         
        fclose(arq);  
		
		
		
		
		int inicial=0;
		
		for(int i =0;i<num_th-1;i++){
			Qtde * q1 = (Qtde*) malloc(sizeof(Qtde));
			q1->qtdeInt = vetor_tam[arquivo];
			q1->linha = arquivo;
			q1->colInicial = inicial;
			inicial+=vetor_tam[arquivo]/num_th;
			q1->colFinal = inicial;
                        
			pthread_create(&t[i], NULL, ordenar_vet, (void *)q1);

			//printf("valor : %ld \t",q1->colInicial);
			//printf("%ld \t",q1->colFinal);
		}
		
		Qtde * q1 = (Qtde*) malloc(sizeof(Qtde));
		q1->qtdeInt = vetor_tam[arquivo];
		q1->linha = arquivo;
		q1->colInicial = inicial;
		inicial+=vetor_tam[arquivo]/num_th+(vetor_tam[arquivo]%num_th);
		q1->colFinal = inicial;
                //printf("Subvetor resto termina em %d",inicial);
		pthread_create(&t[num_th-1], NULL, ordenar_vet, (void *)q1);

		//printf("\nvalor : %ld \t",q1->colInicial);
		//printf("\nvalor %ld \t",q1->colFinal);
			
		
		//printf("final: %d \t",inicial);
		
		
		
		void * ret;
		for(int i=0;i<num_th;i++){//iteração para o término das threads e  a soma dos valores retornados por elas
			pthread_join(t[i], &ret);
                        
		}
                
                printf("\n");
                
                //merge 
                merge(lin[arquivo],0,vetor_tam[arquivo]/2-1,vetor_tam[arquivo]-1); 
                
                
                /*
		//switch para a qtd de threads
                switch (num_th){
                    case 2: // funciona corretamente
                        merge(lin[arquivo],0,vetor_tam[arquivo]/2-1,vetor_tam[arquivo]-1); 
                        printf("\n\nValores merge: %ld %ld\n\n",vetor_tam[arquivo]/2-1,vetor_tam[arquivo]-1 );
                        break; 
                    
                    case 4: //erro para arquivos com elementos nao multiplos de 4
                      merge(lin[arquivo],0,vetor_tam[arquivo]/4-1,((vetor_tam[arquivo]/4)*2)-1);
                        printf("\n\nValores merge: 0 %ld %ld \n\n",vetor_tam[arquivo]/4-1,((vetor_tam[arquivo]/4)*2)-1 );
                        
                        merge(lin[arquivo],(vetor_tam[arquivo]/4)*2,
                                ((vetor_tam[arquivo]/4)*3)-1,
                                vetor_tam[arquivo]-1);
                        printf("\n\nValores merge: %ld %ld %ld\n\n",((vetor_tam[arquivo]/4)*2)-1,
                                ((vetor_tam[arquivo]/4)*3)-1,
                                vetor_tam[arquivo]-1);
                        
                        merge(lin[arquivo],0,(vetor_tam[arquivo]/2)-1,vetor_tam[arquivo]-1);
                        printf("\n\nValores merge: %ld %ld\n\n",vetor_tam[arquivo]/2-1,vetor_tam[arquivo]-1 );
                        break;
                       
                    case 6:
                        //merge(lin[arquivo],0,vetor_tam[arquivo]/6-1,((vetor_tam[arquivo]/6)*2)-1);
                       // printf("\n Valores merge %ld %ld\n",vetor_tam[arquivo]/6-1,((vetor_tam[arquivo]/6)*2)-1);
                        
                        
                        
                        break;
                    default:
                        printf("numero de threads invalido\n");
                        
                }
                */   
                    
                    
                  
                
		//merge(lin[arquivo],0,vetor_tam[arquivo]/2-1,vetor_tam[arquivo]-1,num_th);
    }
	
     printf("\nmaior vetor possui %d posicoes\n",max_vetor);
    //ATE AQUI FUNCIONA CORRETAMENTE
     tempo[1] = clock();
        
    //Preencher vetor com 0
    //FUNCIONA CORRETAMENTE
    for(int n = 0; n < qtd_arq ; n++){

        if (vetor_tam[n] < max_vetor)
        {
          lin[n] = realloc(lin[n],max_vetor*sizeof(int)); //realocar memoria para os vetores menores que o maior vator
        }
       
        printf("Tamanho vetor do arq%d = %ld\n",n+1,vetor_tam[n]);
        
            printf("Vetor:%d, p=%ld ate %d\n", n+1, vetor_tam[n], max_vetor);
            for(int p = vetor_tam[n]; p < max_vetor; p++){
                lin[n][p] = 0;
            }

            for(int o = 0; o < max_vetor; o++){
                printf("[%d] ",lin[n][o]);
            }
            printf("\n");
        
        
    }
    
     printf("\n\n=====TEMPO DE EXECUÇÃO======\n\n");
     
     
     double time_spent = (tempo[1] - tempo[0]) *1000.0 / CLOCKS_PER_SEC ;
         printf("Tempo: %g ms\n\n",time_spent);
     
    return 0; 
}