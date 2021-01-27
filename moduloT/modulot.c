/*

Modulo : t (calculo dos codigos dos simbolos)

Feito por : Andre Ferreira a93211, Bruno Campos a93307, MIEI/CD

/*

Modulo : t (calculo dos codigos dos simbolos)

Feito por : Andre Ferreira, a93211, MIEI/CD, Bruno Campos, a93307, MIEI/CD

Funções principais :

         -moduloT(função main do modulo);

         -Shannon : cria os codigos shannon fanno para uma ListaFreq ordenada crescentemente;

         -InsertSort : Insere um novo elemento numa lista duplamente ligada ,desde que este tenha uma frequência não nula, por ordem decrescente ;

Tipos de dados :

         -ListaFreq : Lista ligada onde cada elemento contem apontadores para o anterior e o proximo, a frequencia de um carater ASCII (maior que zero)
                            e um apontador para o codigo Shannon fanno correspondente.

Variaveis :

         -ListaF apontadores[256] : arrays de apontadores para elementos da lista de frequencias a ser codificada. O elemento v[0] aponta para o elemento da lista que contem
                                                     a frequencia correspondente ao primeiro carater ASCII (NULL), o v[i] aponta para o elemento da lista que contem a frequencia correspondente
                                                     ao i-ésimo carater ASCII.
                                                     Se v[i] estiver a NULL depois de lidas as frequencias isto indica que esse carater não aparece no ficheiro e logo não será codificado,

*/

#include "include\modulot.h"

ListaF InsertSort (ListaF *lista, int novaFreq, int cardinalidade){              //Insere numa lista dada um elemtento com frequência novaFreq por ordem decrescente, dando a carnalidade como parâmetro para ser o tamanho máximo do comprimento do código
    if (novaFreq == 0) return NULL;
    ListaF novo = malloc(sizeof(struct ListaFreq));                    //Inicializar o novo elemento da lista

    if(novo == NULL) {printf("Nao foi possivel alocar espaco para o novo nodo."); exit(EXIT_FAILURE); }         // verifica se foi possivel alocar espaço

    novo->freq = novaFreq;
    novo->ant = NULL;
    novo->cod = ( char *) malloc(cardinalidade*sizeof( char)); //Alocar espaço para o código

     if(novo->cod == NULL) {printf("Nao foi possivel alocar espaco para o codigo."); exit(EXIT_FAILURE); }      // verifica se foi possivel alocar espaço

    novo->cod[0] = '\0';
    novo->prox = NULL;
    ListaF atual;

    if(*lista == NULL){                            //Verificar se a lista esta vazia, se sim é só passar o pointer da lista passa a ser o novo elemento
      *lista=novo;
    }
    else if(novaFreq > (*lista)->freq ){                   //Verificar se o novo elemtento vai ser colocado no inicio da lista (que esta organizada decrescentemente)
        novo->prox = *lista;                             //Se for no inicio então faz se uma reorganização dos pointers e a lista passa a apontar para o novo elemento
        novo->prox->ant = novo;
        *lista = novo;
      }
    else {                                         //Caso o novo elemento não seja no inicio procura-se a sua posição
        atual = *lista;
        while (atual->prox != NULL &&  atual->prox->freq >= novo->freq) atual = atual->prox;                 //Para quando o elemento atual for o último da lista ou caso ele tenha um valor menor
        novo->prox = atual->prox;
        if (atual->prox != NULL)  novo->prox->ant = novo;
        atual->prox = novo;
        novo->ant = atual;
    }
    return novo;
}

void FreeLista (ListaF l){                       // dá free a todos os elementos de uma lista ligada
    ListaF tmp;
    while (l != NULL){
            free(l->cod);
       tmp = l;
       l = l->prox;
       free(tmp);
    }
}

ListaF lerFreq (char *freq,ListaF v[],int N){                                   // Lê as frequencias do ficheiro, coloca-os numa lista ligada (ListaFreq)
    int r , i=1,cardinalidade=N;                                                                                  // ordenada por ordem decrescente e coloca os apontadores para cada elemento no array v
    ListaF l=NULL;
    fpos_t  posicao;
    fscanf(freq, "%i;", &r);
    v[0]=InsertSort(&l,r,cardinalidade);
    if(v[0]==NULL) cardinalidade--;
        while ( i<N){                                                                                           // (exp.:v[0] aponta para o elemento da ListaFreq correspondente ao elemento 0 da tabela ASCII,
           fgetpos(freq,&posicao);                                                                  // v[1] aponta para o elemento da ListaFreq correspondente ao elemento 1 da tabela ASCII, etc.)
           if (fgetc(freq) !=';') {
                fsetpos(freq,&posicao);
                fscanf(freq, "%i;", &r);
           }
           v[i]=InsertSort(&l,r,cardinalidade);
           if(v[i]==NULL) cardinalidade--;
           i++;
        }
        return l;
}

ListaF CalcularMeio (ListaF start,ListaF end){                               //Calcula o posição para a separação dos 0 e 1 para ser usado o alggoritmo de Shannon-Fano
  ListaF meio;
  int freq_direita,freq_esquerda;
  freq_direita = end->freq;
  freq_esquerda = start->freq;                                               //Apartir de dois somatórios das frequências um apartir da esquerda (começa pelo primeiro elementos da lista e acrescenta os elementos pela direita) e outro apartir da direita
  while(start->prox != end){                                                 //(começa pelo último elemento da lista e acrescenta os próximos elementos pela esquerda) comparamos entre eles qual possoui o maior valor,caso seja o comparamos entre eles qual possoui o
    while(freq_esquerda>freq_direita && (start->prox != end)){                      //menor valor e é lhe dado o próximo elemento da lista para aumentar o seu somatório e altera-se os pointers que limitam os somatórios, quando estes apontam para elementos consecutivos a
      end = end->ant;
      freq_direita += end->freq;                                               //função pára, returnado o elemento o irão começar os 1 para o algoritmo de shannon-fano
    }
    while(freq_esquerda<=freq_direita && (start->prox != end)){
      start = start->prox;
      freq_esquerda += start->freq;
    }
  }
  return meio=start->prox;
}

int add_bit_to_code (char *c,ListaF start,ListaF end){                     //Função que acrescenta o caracter c a elementos da lista que estejam entre os 2 apontadores dados
    while (start != end && start != NULL){
      strcat(start->cod,c);
      start = start->prox;
    }
}

void Shannon(ListaF start,ListaF end){                                     //Função de Shannon-Fano que gera os códigos para cada elemento da lista, guardando-os no campo cod das listas
  if (start == end);
  else{
    ListaF meio;
    meio = CalcularMeio(start,end);                                        //Calcula o ponto de separação dos 0 e 1
    add_bit_to_code("0",start,meio);                                       //Acrescenta o bit 1 à direita da separação inclusive
    add_bit_to_code("1",meio,end->prox);                                   //Acrescenta o bit 0 à esquerda da separação
    Shannon(start,meio->ant);                                              //Refaz numa forma recursiva
    Shannon(meio,end);
    }
}

void escreveCod(char *cod,ListaF v[],int N){                 // escreve os codigos de um bloco
    int i = 1;
    if ( v[0]!=NULL ) fprintf(cod, "%s",v[0]->cod);                  // inicializaçao da escrita (fora do ciclo já que é o unico que não se enquadra abaixo)
    while(i<N){
            if ( v[i]==NULL ) fprintf(cod,";");                                            // se v[i]==NULL, significa que a freq é zero e logo não tem codigo
            else fprintf(cod,";%s",v[i]->cod);
            i++;
    }
}


void moduloT (char *fich) {                                        // lê o ficheiro .freq  e converte a informaçao para o ficheiro .cod
    clock_t t;                            // contar tempo de execução
    t = clock();
    int numblocos,                       // nº blocos
          i=0;
    char modo;                    // N(normal) ou R(RLE)
    int nomelength;
    char nome[1024]="\0";
    FILE *freq,                                    // ficheiro .freq
            *cod;                                     // ficheiro .cod
    ListaF lista = NULL, endlist;

    freq = fopen(fich,"r");
    if (freq == NULL){ printf("O ficheiro .freq nao existe.\n"); exit(EXIT_FAILURE); }                           //verifica se o ficheiro a ler existe
    nomelength=strlen(fich)-4;
    strncpy(nome, fich,nomelength);                              // retira e altera o nome do ficheiro de .freq para .cod
    strcat(nome, "cod");
    cod = fopen(nome,"w");
    if(cod == NULL){ printf("Nao foi possivel criar o ficheiro .cod.\n"); exit(EXIT_FAILURE); }            // verifica se é possivel criar o ficheiro .cod

    ListaF apontadores[256];                   // array onde vão ficar os endereços para cada elemento da lista que indicara seus codigos

    fscanf(freq,"@%c@%i", &modo, &numblocos);                         //ex.: @R@2@.......
    fprintf(cod,"@%c@%i", modo, numblocos);

    int tamanho[numblocos];
    while ( i < numblocos){
        fscanf(freq,"@%i@",&tamanho[i]);                                             // ciclo que lê tamanhos e frequencias
        lista = lerFreq(freq,apontadores,256);
        for(endlist=lista; endlist->prox ; endlist=endlist->prox);
        Shannon(lista,endlist);                                                                //aplica o algoritmo de shannon fannon

        fprintf(cod,"@%i@",tamanho[i]);
        escreveCod(cod,apontadores,256);                                                                  // e escreve tamanhos e codigos
        i++;
        FreeLista(lista);
        }

    fprintf(cod,"@0");                                                                        // fim do ficheiro

    fclose(freq);
    fclose(cod);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    printf("Bruno Campos a93307, Andre Ferreira a93211, MIEI/CD, 3-jan-2021 \nModulo: t (calculo dos codigos dos simbolos) \nNumero de blocos: %i ",numblocos);
    printf("\nTamanho dos blocos analisados no ficheiro de simbolos: %i/%i bytes \nTempo de execucao do modulo (milissegundos): %f \nFicheiro gerado: %s\n",tamanho[0], tamanho[numblocos-1],time_taken*1000,nome);

}
