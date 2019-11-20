/* 
 * Autores:
 *      Brenda Cristina de Souza Silva
 *      Mariana Albino Queiroz Andrade  
 * 
 * Programa recebe n arquivos binarios como parametros, os ordena 
 * e armazena seus elementos ordenados
 * em formato de matrix em um arquivo de saida binario
 * utilizando threads.
 * 
 * Para garantir que a aplicação funcione corretamente
 * deve-se executá-lo no seguinte formato, seguindo 
 * a ordem de parametros: 
 *    - nome_do_programa
 *    - numero_de_threads
 *    - arquivos_de_entrada
 *    - -o
 *    - arquivo_de_saida 
 * EXEMPLO DE EXECUÇÃO
 * ./buildmatrix 2 arq1.dat arq2.dat arq3.dat -o saida.dat 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include <math.h>
#include <time.h>

#define MAX 20
typedef char *palavra; //armazenara o nome do arquivo de saida

int **lin;

typedef struct qtde{
    int linha;
    int qtdeInt;
    int colInicial;
	int colFinal;
}Qtde;


int tam_vet(FILE *arq){
    
    int length; 
    fseek(arq,0,SEEK_END);	
    length = ftell(arq); 
    fseek(arq,0,SEEK_SET); 
    return length/4;
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
	
	
} 

void *ordenar_vet(void *data){
    Qtde *qtde = (Qtde *)data;
	
    int vetor_tam = qtde->qtdeInt;
    int num_lin = qtde->linha;

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
 
}

int main(int argc, char *argv[]) {
    
    
    int qtd_arq = argc - 4;
    int j=0; 
    int num_th = atoi(argv[1]); //na pos[1] esta o num de threads
    int vetor_tam[qtd_arq]; //vetor armazena a qtd de elementos de cada arquivo
    lin = (int **) malloc (qtd_arq*sizeof(int*));
    int max_vetor = 0 ;
    clock_t tempo[2];
    
    palavra arqSaida;
    
    arqSaida = argv[argc-1];
    FILE*arqFinal = fopen(arqSaida,"wb"); //abertura do arquivo de saida
    
	
    pthread_t t[num_th];
 
    tempo[0] = clock(); //hora inicial do processamento
    
    //percorrer arquivos de entrada
    
    for(int arquivo = 0; arquivo < (argc-4) ; arquivo++){
        FILE *arq = fopen(argv[arquivo+2],"rb");
        rewind(arq);
        vetor_tam[arquivo] = tam_vet(arq);
       
        if(vetor_tam[arquivo] > max_vetor){
            max_vetor = vetor_tam[arquivo];
        }
        
        printf("======Arquivo original:=======\n");     
        lin[arquivo] = (int *) malloc(vetor_tam[arquivo] *sizeof(int));
        for (j=0; j<vetor_tam[arquivo];j++){
            fread(&lin[arquivo][j],sizeof(int),1,arq);
            printf("[%d] ",lin[arquivo][j]);
            
            
        }
        printf("\n");
 
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

        }

        Qtde * q1 = (Qtde*) malloc(sizeof(Qtde));
        q1->qtdeInt = vetor_tam[arquivo];
        q1->linha = arquivo;
        q1->colInicial = inicial;
        inicial+=vetor_tam[arquivo]/num_th+(vetor_tam[arquivo]%num_th);
        q1->colFinal = inicial;
        
        pthread_create(&t[num_th-1], NULL, ordenar_vet, (void *)q1);

        
        void * ret;
        for(int i=0;i<num_th;i++){//iteração para o término das threads e  a soma dos valores retornados por elas
                pthread_join(t[i], &ret);

        }

        
        int umMeio = vetor_tam[arquivo]/2; //um meio (1/2) do arquivo
        int umQuarto = vetor_tam[arquivo]/4; //um quarto (1/4) do arquivo
        int umOitavo = vetor_tam[arquivo]/8; //um oitavo (1/8) do arquivo
        int umDezesseis = vetor_tam[arquivo]/16; //um dezesseis(1/16) avos do arquivo
        int posFinal = vetor_tam[arquivo] -1 ;//posicao final do arquivo

        //switch para a qtd de threads
        
        switch (num_th){
            case 2: 
                merge(lin[arquivo],0,umMeio-1,posFinal); 
            
                break; 

            case 4: 

                merge(lin[arquivo],0,umQuarto-1,(umQuarto*2)-1);
                merge(lin[arquivo],umQuarto*2,(umQuarto*3)-1,posFinal);
                merge(lin[arquivo],0,(umQuarto*2)-1,posFinal);
            
                break;

            case 8:

                merge(lin[arquivo],0,umOitavo-1,(umOitavo*2)-1); //primeira serie de merge
                merge(lin[arquivo],umOitavo*2,(umOitavo*3)-1,(umOitavo*4)-1);
                merge(lin[arquivo],umOitavo*4,(umOitavo*5)-1,(umOitavo*6)-1);
                merge(lin[arquivo],umOitavo*6,(umOitavo*7)-1,posFinal); //fim da primeira serie de merge
                
                merge(lin[arquivo],0,(umOitavo*2)-1,(umOitavo*4-1)); //segunda serie de merge
                merge(lin[arquivo],umOitavo*4,(umOitavo*6)-1,posFinal); //fim da segunda serie de merge
                
                merge(lin[arquivo],0,(umOitavo*4)-1,posFinal); //merge final
           
                break;

            case 16:

                merge(lin[arquivo],0,umDezesseis-1,(umDezesseis*2)-1); //primeira serie de merge
                merge(lin[arquivo],umDezesseis*2,(umDezesseis*3)-1,(umDezesseis*4)-1);
                merge(lin[arquivo],umDezesseis*4,(umDezesseis*5)-1,(umDezesseis*6)-1);
                merge(lin[arquivo],umDezesseis*6,(umDezesseis*7)-1,(umDezesseis*8)-1);
                merge(lin[arquivo],umDezesseis*8,(umDezesseis*9)-1,(umDezesseis*10)-1);
                merge(lin[arquivo],umDezesseis*10,(umDezesseis*11)-1,(umDezesseis*12)-1);
                merge(lin[arquivo],umDezesseis*12,(umDezesseis*13)-1,(umDezesseis*14)-1);
                merge(lin[arquivo],umDezesseis*14,(umDezesseis*15)-1,posFinal); //fim da primeira serie
                
                merge(lin[arquivo],0,(umDezesseis*2)-1,(umDezesseis*4)-1); //segunda serie de merge
                merge(lin[arquivo],umDezesseis*4,(umDezesseis*6)-1,(umDezesseis*8)-1);
                merge(lin[arquivo],umDezesseis*8,(umDezesseis*10)-1,(umDezesseis*12)-1);
                merge(lin[arquivo],umDezesseis*12,(umDezesseis*14)-1,posFinal); //fim da segunda serie
                
                merge(lin[arquivo],0,(umDezesseis*4)-1,(umDezesseis*8)-1); //terceira serie de merge
                merge(lin[arquivo],umDezesseis*8,(umDezesseis*12)-1,posFinal); //fim da terceira serie
                
                merge(lin[arquivo],0,(umDezesseis*8)-1,posFinal); //merge final


                break;
                
                default:
                    printf("numero de threads invalido\n");

        }
                 
         
    }
 
    tempo[1] = clock(); //hora final do processamento
        
    //Preencher vetor com 0
      
    for(int n = 0; n < qtd_arq ; n++){

        if (vetor_tam[n] < max_vetor)
        {
          lin[n] = realloc(lin[n],max_vetor*sizeof(int)); //realocar memoria para os vetores menores que o maior vator
        }
       
        for(int p = vetor_tam[n]; p < max_vetor; p++){
            lin[n][p] = 0;
        }
        //prints para teste
        //printf("\n======Vetor final:=======\n");
        for(int o = 0; o < max_vetor; o++){

            fwrite(&lin[n][o],sizeof(int),1,arqFinal); //gravando no arquivo de saida 
            //printf("[%d] ",lin[n][o]);
        }
        //printf("\n");

        
    }
     fclose(arqFinal);
    
     printf("\n=====TEMPO DE EXECUÇÃO======\n");
     
     
     double time_spent = (tempo[1] - tempo[0]) *1000.0 / CLOCKS_PER_SEC ;
         printf("Tempo: %g ms\n",time_spent);
     
    return 0; 
}