
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
void ordenar_vet(FILE *arq, int *col, long tam,int qtd_arq, int *lin[sizeof(qtd_arq)]){
    int aux =0;
    int i;
    for (i = 0; i < tam; i++){
        for (int j = 0; j < tam; j++){
            if (col[i] < col[j])
            {
            //troca dos valores  
                aux = col[i];
                col[i] = col[j];
                col[j] = aux;
            }
        }
    }
    for(int p =0; p< tam; p++){
        printf("[%d] ",col[p]);
    } 
    for(int m = 0; m< qtd_arq; m++){
        lin[m] = malloc(sizeof(col));
        lin[m] = col;
    }
}
int main(int argc, char *argv[]) {
    
    
    int qtd_arq = argc - 4;
    int *lin[qtd_arq];
    int i,j = 0; 
    int num_th = atoi(argv[1]); //na pos[1] esta o num de threads
    int *col;
    long vetor_tam[qtd_arq];
    
    int max_vetor = 0 ;
    //prints para teste
    printf("QTD de arq = %d\n", qtd_arq);
    printf("Num de threads %d\n",num_th);
    
    //ler arquivo
    for(int arquivo = 0; arquivo < (argc-4) ; arquivo++){
        FILE *arq = fopen(argv[arquivo+2],"rb");
        rewind(arq);
        
        long tam;
        vetor_tam[arquivo] = tam_vet(arq);
        //tam = tam_vet(arq);
        if(vetor_tam[arquivo] > max_vetor){
            max_vetor = vetor_tam[arquivo];
        }
        
        printf("tamanho do vetor = %ld\n",vetor_tam[arquivo]);
        
        //ler arquivo
        //ler_arqs(arq,col,tam);
       
        col = (int *) malloc(vetor_tam[arquivo] *sizeof(int));
        for (j=0; j<vetor_tam[arquivo];j++){
            fread(&col[j],sizeof(int),1,arq);
            printf("[%d] ", col[j]);
        }
        
        printf("\n");
        //ordenar vetor
        //ordenar_vet(arq,col,tam,qtd_arq,lin);
       
        int aux =0;
        for (i = 0; i < vetor_tam[arquivo]; i++)
            {
                for (int j = 0; j < vetor_tam[arquivo]; j++)
                {
                    if (col[i] < col[j])
                    {
                    //troca dos valores  
                        aux = col[i];
                        col[i] = col[j];
                        col[j] = aux;
                    }
                }
            }
         
        //printando o vetor ordenado
        for(i =0; i< vetor_tam[arquivo]; i++){
            printf("[%d] ",col[i]);
        } 
        for(int m = 0; m< qtd_arq; m++){
            //lin[m] = malloc(sizeof(int)*tam);
            lin[m] = col;
        }
        
        printf("\n");
        
        //for(i = 0; i< qtd_arq; i++){
        //    lin[i] = col;
       // }
         
        fclose(arq);  
    }
     printf("\nmaior vetor possui %d posicoes\n",max_vetor);
    //ATE AQUI FUNCIONA CORRETAMENTE
    
    //Preencher vetor com 0
    
    for(int n = 0; n < qtd_arq ; n++){
       
        printf("Tamanho vetor do arq%d = %ld\n",n+1,vetor_tam[n]);
        
            //lin[n-2];
            for(int p = 0; p < max_vetor; p++){
                lin[n][vetor_tam[n]+p] = 0;
                
            }
            for(int o = 0; o < max_vetor; o++){
                printf("[%d] ",lin[n][o]);
            }
            printf("\n");
        
        
    }
      
     
  
       
    
    
    
    
}
    
    
        
       
    
    
    
    
    
    
   

