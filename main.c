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

long tam_arq(FILE *arq){
    
    long length; //tamanho do arquivo
    fseek(arq,0,SEEK_END);	
    length = ftell(arq); 
    fseek(arq,0,SEEK_SET); //começo do arquivo
    return length/4;
}


int main(int argc, char *argv[]) {
    
    
    int qtd_arq = argc - 4;
    int i,j = 0; 
    int num_th = atoi(argv[1]); //pos 1 esta o num de threads
    int *lin[qtd_arq]; //pos[0] apontará para outro vetor
    int max_vetor = 0 ;
    //prints para teste
    printf("QTD de arq = %d\n", qtd_arq);
    printf("Num de threads %d\n",num_th);
    
    //ler arquivo
    for(int arquivo = 2; arquivo < (argc-2) ; arquivo++){
        FILE *arq = fopen(argv[arquivo],"rb");
        rewind(arq);
        int *col;
        long tam;
        //n entendi, tam origianal é 8
        tam = tam_arq(arq);
        if(tam > max_vetor){
            max_vetor = tam;
        }
        
        //vetores dinamicos
        printf("tamanho do vetor = %ld\n",tam);
        col = (int *) malloc(tam *sizeof(int));
        for (j=0; j<tam;j++){
            fread(&col[j],sizeof(int),1,arq);
            printf("[%d] ", col[j]);
        }
        printf("\n");
        int aux =0;
        for (i = 0; i < tam; i++)
            {
                for (int j = 0; j < tam; j++)
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
        for(i =0; i< tam; i++){
            printf("[%d] ",col[i]);
        } 
        for(int m = 0; m< qtd_arq; m++){
            lin[m] = malloc(sizeof(col));
            lin[m] = col;
        }
        //for(i = 0; i< qtd_arq; i++){
        //    lin[i] = col;
       // }
        fclose(arq);  
    }
     
    printf("\n%d\n",max_vetor);
        /*
        //ta imprimindo lixo depois da ultima pos
        while(!feof(arq)){
            
            fscanf(arq,"%d",&vet[j]);
            printf("[%d] ", vet[j]);
            j++;
            
        }
        */
    //qtde de threads 
    switch(num_th){
        
        case 2:
            //blabla
        break;
        
        case 4:
            //blabla
        break;
        
        case 8:
           //blabla 
        break;
        
        case 16:
            //blabla
        break;
        
        default :
        printf("Qtd de threads inválida!\n");
        
    }
    
}
    
    
        
       
    
    
    
    
    
    
   


