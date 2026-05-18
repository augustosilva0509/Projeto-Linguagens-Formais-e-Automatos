#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct{
    int** lista;
    int* lista_tamanhos;
    int tamanho;
} COMBINACOES;

void adiciona_combinacao(COMBINACOES* combinacoes, int* combinacao, int combinacao_tamanho){
    combinacoes->lista[combinacoes->tamanho] = combinacao;
    combinacoes->lista_tamanhos[combinacoes->tamanho++] = combinacao_tamanho;
}

void adiciona_elemento(COMBINACOES* combinacoes, int elemento){
    int* combinacao;
    int combinacoes_tamanho = combinacoes->tamanho;
    for(int i = 0; i < combinacoes_tamanho; i++){
        // Aloca a memória necesária para a combinação
        combinacao = (int*)malloc(((combinacoes->lista_tamanhos[i]) + 1) * sizeof(int));

        // Copia a iésima combinação de combinacoes para combinacao
        memcpy(combinacao, combinacoes->lista[i], (combinacoes->lista_tamanhos[i]) * sizeof(int));

        // Adiciona o elemento para gerar uma combinação nova
        combinacao[combinacoes->lista_tamanhos[i]] = elemento;

        // Adiciona a combinação em combinacoes
        adiciona_combinacao(combinacoes, combinacao, (combinacoes->lista_tamanhos[i]) + 1);
    }
    
    // Adiciona a combinacao com somente o elemento
    combinacao = (int*)malloc(sizeof(int));
    *combinacao = elemento;
    adiciona_combinacao(combinacoes, combinacao, 1);   
}

int pega_indice_da_combinacao(COMBINACOES* combinacoes, int* combinacao, int combinacao_tamanho){
    for(int i = 0; i < combinacoes->tamanho; i++){
        int falta_pra_ser_igual = combinacao_tamanho; 
        for(int j = 0; j < combinacoes->lista_tamanhos[i]; j++){
            if(combinacoes->lista_tamanhos[i] != combinacao_tamanho)
                break;
            for(int k = 0; k < combinacao_tamanho; k++){
                if(combinacoes->lista[i][j] == combinacao[k]){
                    falta_pra_ser_igual--;
                }
            }
        }
        if(falta_pra_ser_igual == 0)
            return i;
    }
    return -1;
}

int indice_tem_elemento(COMBINACOES* combinacoes, int indice, int elemento){
    for(int i = 0; i < combinacoes->lista_tamanhos[indice]; i++){ 
        if(combinacoes->lista[indice][i] == elemento)
            return 1;
    }
    return 0;
}

COMBINACOES* construtor_combinacoes(int* elementos, int elementos_tamanho){
    COMBINACOES *combinacoes = (COMBINACOES*)malloc(sizeof(COMBINACOES));
    combinacoes->lista = (int**)malloc(pow(2, elementos_tamanho) * sizeof(int*));
    combinacoes->lista_tamanhos = (int*)malloc(pow(2, elementos_tamanho) * sizeof(int));
    combinacoes->tamanho = 0;

    for(int i = 0; i < elementos_tamanho; i++){
        adiciona_elemento(combinacoes, elementos[i]);
    }

    return combinacoes;
}

int checa_eof(FILE* arq){
    int pos = ftell(arq);
    int c = fgetc(arq);
    fseek(arq, pos, SEEK_SET);
    if(c == EOF)
        return 1;
    return 0;
}

int main(){
    FILE* arq = fopen("afnd.txt", "r");
    
    // GUARDA O ALFABETO
    int alfabeto_tamanho = 0;
    char* alfabeto;
    {
        int temp_tamanho = 5;
        char* temp = (char*)malloc(temp_tamanho * sizeof(char));
        
        while(!checa_eof(arq)){
            if(alfabeto_tamanho == temp_tamanho){
                char* temp1 = (char*)malloc((temp_tamanho + 5) * sizeof(char));
                memcpy(temp1, temp, temp_tamanho * sizeof(char));
                temp_tamanho += 5;
                free(temp);
                temp = temp1;
            }

            char temp2 = 0;
            fscanf(arq, "alfabeto=[");
            
            fscanf(arq, "%c", &temp2);
            temp[alfabeto_tamanho++] = temp2;
            if(fgetc(arq) == ']'){
                break;
            }
        }
        fgetc(arq);
        
        alfabeto = (char*)malloc(alfabeto_tamanho * sizeof(char));
        memcpy(alfabeto, temp, alfabeto_tamanho * sizeof(char));
        free(temp);
    }

    // GUARDA O ESTADO INICIAL
    int numero_estado_inicial;
    {
        fscanf(arq, "q0=%i", &numero_estado_inicial);
        fscanf(arq, "\n");
    }
    
    // GUARDA OS ESTADOS FINAIS
    int estados_finais_contador = 0;
    int* estados_finais;
    {
        int estados_finais_tamanho = 5;
        int* temp = (int*)malloc(estados_finais_tamanho * sizeof(int));

        while(!checa_eof(arq)){
            if(estados_finais_contador == estados_finais_tamanho){
                int* temp1 = (int*)malloc((estados_finais_tamanho + 5) * sizeof(int));
                memcpy(temp1, temp, estados_finais_tamanho * sizeof(int));
                estados_finais_tamanho += 5;
                free(temp);
                temp = temp1;
            }

            int temp2 = 0;
            fscanf(arq, "qf=[");
            
            fscanf(arq, "%d", &temp2);
            temp[estados_finais_contador++] = temp2;
            if(fgetc(arq) == ']'){
                break;
            }
        }
        fgetc(arq);
        
        estados_finais = (int*)malloc(estados_finais_contador * sizeof(int));
        memcpy(estados_finais, temp, estados_finais_contador * sizeof(int));
        free(temp);
    }

    // GUARDA TODOS OS ESTADOS ÚNICOS
    int* estados_unicos;
    int estados_unicos_contador = 0;
    {
        int estados_unicos_tamanho = 5;
        int* temp = (int*)malloc(estados_unicos_tamanho * sizeof(int));

        while(!checa_eof(arq)){
            if(estados_unicos_contador == estados_unicos_tamanho){
                int* temp1 = (int*)malloc((estados_unicos_tamanho + 5) * sizeof(int));
                memcpy(temp1, temp, estados_unicos_tamanho * sizeof(int));
                estados_unicos_tamanho += 5;
                free(temp);
                temp = temp1;
            }

            int temp2 = 0;
            fscanf(arq, "Q=[");
            

            fscanf(arq, "%d", &temp2);
            temp[estados_unicos_contador++] = temp2;
            if(fgetc(arq) == ']'){
                break;
            }
        }
        fgetc(arq);
        
        estados_unicos = (int*)malloc(estados_unicos_contador * sizeof(int));
        memcpy(estados_unicos, temp, estados_unicos_contador * sizeof(int));
        free(temp);
    }
    
    // pega todas as combinações únicas de estados_unicos
    COMBINACOES* combinacoes = construtor_combinacoes(estados_unicos, estados_unicos_contador);
    
    // PEGA TODAS AS SAÍDAS POSSÍVEIS
    int** saida; 
    {
        saida = (int**)malloc(alfabeto_tamanho*sizeof(int*));

        for(int i = 0; i < alfabeto_tamanho; i++){
            const int saida_tamanho = combinacoes->tamanho;
            saida[i] = (int*)malloc(saida_tamanho * sizeof(int));
            
            for(int j = 0; j < saida_tamanho; j++)
                saida[i][j] = -1;

            int position = ftell(arq);
            
            // pega as saídas descritas no arquivo
            while(!checa_eof(arq)){
                int *estado = (int*)malloc(sizeof(int));
                *estado = 0;
                fscanf(arq,"%d", estado);
                fgetc(arq);

                char simbolo = fgetc(arq);
                
                fgetc(arq);fgetc(arq);
                
                if(simbolo == alfabeto[i]){
                    int saida_contador = 0;
                    {
                        int pos = ftell(arq);
                        char temp;
                        do{
                            temp = fgetc(arq);

                            if(temp == ','){
                                saida_contador++;
                            }
                            else if(saida_contador == 0 && isdigit(temp)){
                                saida_contador++;        
                            }
                        }
                        while(temp != ']');
                        fseek(arq, pos, SEEK_SET);
                    }
                    
                    
                    int* combinacao = (int*)malloc(saida_contador*sizeof(int));
                    int combinacao_tamanho = 0;
                    
                    for(int j = 0; j < saida_contador; j++){
                        int temp = -1;
                        if(j != saida_contador-1)
                            fscanf(arq, "%d,", &temp);
                        else
                            fscanf(arq, "%d", &temp);
                        
                        int unique = 1;
                        for(int k = 0; k < combinacao_tamanho; k++) 
                            if(combinacao[k] == temp) unique = 0;
                        
                        if(unique)
                            combinacao[combinacao_tamanho++] = temp;
                    }

                    int estado_idx = pega_indice_da_combinacao(combinacoes, estado, 1);
                    int saida_idx = pega_indice_da_combinacao(combinacoes, combinacao, combinacao_tamanho);

                    saida[i][estado_idx] = saida_idx;
                }
                while(fgetc(arq) != ']');
                
            }
            fseek(arq, position, SEEK_SET);
            
            // determina as saídas de cada combinação de estados

            for(int j = 0; j < saida_tamanho; j++){
                if(combinacoes->lista_tamanhos[j] == 1){
                    continue;
                }
                
                int* combinacao = (int*)malloc(estados_unicos_contador * sizeof(int)); 
                int combinacao_contador = 0;
                
                for(int k = 0; k < combinacoes->lista_tamanhos[j]; k++){
                    int combinacao_unaria[] = {combinacoes->lista[j][k]};
                    int combinacao_unaria_idx = pega_indice_da_combinacao(combinacoes, combinacao_unaria, 1);
                    
                    
                    if(saida[i][combinacao_unaria_idx] != -1){
                        for(int a = 0; a < combinacoes->lista_tamanhos[ saida[i][combinacao_unaria_idx] ]; a++){
                            int unique = 1;
                            
                            for(int b = 0; b < combinacao_contador; b++){
                                if(combinacoes->lista[saida[i][combinacao_unaria_idx]][a] == combinacao[b]){
                                    unique = 0;
                                    break;
                                }

                            }
                            if(unique)
                                combinacao[combinacao_contador++] = combinacoes->lista[saida[i][combinacao_unaria_idx]][a];
                        }
                    }
                    
                }
                saida[i][j] = pega_indice_da_combinacao(combinacoes, combinacao, combinacao_contador);
            }
        }
    }
    
    fclose(arq);
    // ATUALIZAR numeros_estados_finais E numeros_estados_finais_tamanho
    int* novo_estados_finais;
    int novo_estados_finais_tamanho = 0;
    {
        int* temp = (int*)malloc(combinacoes->tamanho * sizeof(int));
    
        // para cada estado final, vê cada indice que o tem incluso e guarda em temp
        for(int i = 0; i < estados_finais_contador; i++){
            for(int j = 0; j < combinacoes->tamanho; j++){
                if(indice_tem_elemento(combinacoes, j, estados_finais[i]))
                    temp[novo_estados_finais_tamanho++] = j;
            }
        }

        novo_estados_finais = (int*)malloc(novo_estados_finais_tamanho * sizeof(int));
        memcpy(novo_estados_finais, temp, novo_estados_finais_tamanho * sizeof(int));
        free(temp);
    }
    

    // TIRAR ESTADOS INÚTEIS usar numero_estado_inicial
    int* novo_estados_unicos;
    int novo_estados_unicos_contador = 0;
    {
        int* temp = (int*)malloc(combinacoes->tamanho * sizeof(int));
        temp[novo_estados_unicos_contador++] = pega_indice_da_combinacao(combinacoes, &numero_estado_inicial, 1);
        
        for(int i = 0; i < novo_estados_unicos_contador; i++){
            
            for(int j = 0; j < alfabeto_tamanho; j++){
                if(saida[j][temp[i]] == -1){
                    continue;
                }
                int unique = 1;
                for(int k = 0; k < novo_estados_unicos_contador; k++){
                    if(saida[j][temp[i]] == temp[k]){
                        unique = 0;
                    }
                }
                if(unique){
                    temp[novo_estados_unicos_contador++] = saida[j][temp[i]];
                }   
            }
        }
        
        novo_estados_unicos = (int*)malloc(novo_estados_unicos_contador * sizeof(int));
        memcpy(novo_estados_unicos, temp, novo_estados_unicos_contador * sizeof(int));
        free(temp);
    }
    
    // ATUALIZAR saida SEM OS ESTADOS INUTEIS
    int** novo_saida;
    {
        int** temp = (int**)malloc(alfabeto_tamanho * sizeof(int*));
        int* temp1 = (int*)malloc(combinacoes->tamanho * sizeof(int));
        for(int a = 0; a < alfabeto_tamanho; a++){
            temp[a] = (int*)malloc(novo_estados_unicos_contador * sizeof(int));
            
            for(int i = 0; i < novo_estados_unicos_contador; i++){
                temp[a][i] = saida[a][novo_estados_unicos[i]];

            }
        }
        novo_saida = temp;
    }

    // ATUALIZAR novo_saida E novo_estados_unicos E numero_estado_inicial E novo_estados_finais
    {
        // ATUALIZA novo_saida
        for(int a = 0; a < alfabeto_tamanho; a++){
            for(int i = 0; i < novo_estados_unicos_contador; i++){
                for(int j = 0; j < novo_estados_unicos_contador; j++){

                    if(novo_saida[a][i] == novo_estados_unicos[j]){
                        novo_saida[a][i] = j;
                        break;
                    }
                }
            }
        }
        // ATUALIZA numero_estado_inicial
        for(int i = 0; i < novo_estados_unicos_contador; i++){
            if(novo_estados_unicos[i] == pega_indice_da_combinacao(combinacoes, &numero_estado_inicial, 1)){
                numero_estado_inicial = i;
            }
        }

        // ATUALIZA novo_estados_finais
        
        int* temp = (int*)malloc(novo_estados_unicos_contador * sizeof(int));
        int temp_contador = 0;
        for(int i = 0; i < novo_estados_finais_tamanho; i++){
            for(int j = 0; j < novo_estados_unicos_contador; j++){
                if(novo_estados_finais[i] == novo_estados_unicos[j]){
                    temp[temp_contador++] = j;
                }
            }
        }
        free(novo_estados_finais);
        novo_estados_finais = temp;
        novo_estados_finais_tamanho = temp_contador;

        // ATUALIZA novo_estados_unicos
        for(int i = 0; i < novo_estados_unicos_contador; novo_estados_unicos[i] = ++i);
    }

    printf("SAIDA FINAL\n");
    printf("-----------\n");
    printf("q - ");
    for(int j = 0; j < novo_estados_unicos_contador; j++){
            printf("%d ", novo_estados_unicos[j]);
        }
    printf("\n--------------------\n");
    for(int i = 0; i < alfabeto_tamanho; i++){
        printf("%c - ", alfabeto[i]);
        for(int j = 0; j < novo_estados_unicos_contador; j++){
            if(novo_saida[i][j] == -1){
                printf("- ");
                continue;
            }
            printf("%d ", novo_saida[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    // GUARDAR VALORES EM test2.txt
    arq = fopen("afd.txt", "w");
    
    // GUARDA ALFABETO
    {
        fprintf(arq, "alfabeto=[");
        for(int a = 0; a < alfabeto_tamanho; a++){
            if(a != alfabeto_tamanho-1){
                fprintf(arq, "%c,", alfabeto[a]);
            }
            else{
                fprintf(arq, "%c]", alfabeto[a]);
            }
        }
        fprintf(arq, "\n");
    }

    //GUARDA q0
    {
        fprintf(arq, "q0=%d\n", numero_estado_inicial);
    }
    
    // GUARDA qf
    {
        fprintf(arq, "qf=[");
        for(int a = 0; a < novo_estados_finais_tamanho; a++){
            if(a != novo_estados_finais_tamanho-1){
                fprintf(arq, "%d,", novo_estados_finais[a]);
            }
            else{
                fprintf(arq, "%d]", novo_estados_finais[a]);
            }
        }
        fprintf(arq, "\n");
    }

    // GUARDA Q
    {
        fprintf(arq, "Q=[");
        for(int a = 0; a < novo_estados_unicos_contador; a++){
            if(a != novo_estados_unicos_contador-1){
                fprintf(arq, "%d,", novo_estados_unicos[a]);
            }
            else{
                fprintf(arq, "%d]", novo_estados_unicos[a]);
            }
        }
        fprintf(arq, "\n");
    }

    // GUARDA funcao_programa
    {
        for(int i = 0; i < novo_estados_unicos_contador; i++){
            for(int a = 0; a < alfabeto_tamanho; a++){
                fprintf(arq, "%d,%c,", i, alfabeto[a]);
                if(novo_saida[a][i] == -1){
                    fprintf(arq, "-\n");
                    continue;
                }
                fprintf(arq, "%d\n", novo_saida[a][i]);
            }
        }
    }

    fclose(arq);
}

