//Modulo f criado por Manuel Novais, a89512 e Andre Vinagreiro a89986



#include "include\modulof.h"

unsigned long countrun(unsigned long i, unsigned char *array,unsigned char current){        //Conta as vezes que o char se repete
    unsigned long r = 1;
    while (array[i+1] == current ){
        r++;
        i++;
    }
    return r;
}


unsigned char* encode(unsigned char* src,unsigned long  len,unsigned long  *rlesize)        //devolve a versao comprimida em RLE do array
{
    unsigned char current;
    unsigned char* dest = malloc(sizeof(char)*len*3); //caso o ficheiro apenas contenha o simbolo {0} seguido de um outro diferente o tamanho maximo sera len_original*3


    unsigned long j = 0;
    unsigned long i,rLen;


    for (i = 0; i < len; i++) {                       //vai valor a valor ver as repeti�oes
            current = src[i];
            rLen= countrun(i,src,current);


            if(rLen>3 || current == '\0'){            //caso o objeto se repita mais que 3 vezes, ou seja {0}
                dest[j++] = '\0';
                dest[j++] = current;
                dest[j++] = rLen;
                i+=(rLen-1);
            }
            else{
                dest[j++] = src[i];
            }


        }

    *rlesize=j;                                     //da o tamanho do array para rlesize

    return dest;                                    //devolve o array comprimido por rle
}


int *contadoresShort(unsigned char *conteudo,unsigned long size){   //Funcao que calcula a frequencia de cada objeto de um determinado buffer

	int *freq = calloc(256,sizeof(int));

        for (unsigned long i = 0;i<size; i++){
			freq[conteudo[i]]++;
	}
	return freq;

}

void writeFreqFile(FILE *destination, int *array,unsigned long block_size){ //Funcao que escreve numa file .freq as frequencias dos objetos
    int k = array[0];
    fprintf(destination,"%lu@%d", block_size,k);
     for(int i=1; i<256; i++){
            if(k != array[i]) fprintf(destination,";%d", array[i]);
            else fprintf(destination,";");
            k = array[i];
        }
    fprintf(destination,"@");

}


int moduloF( char *str, unsigned long defined_size, int isForced)
{
    clock_t begin = clock();                //Comecar a calcular o tempo de execucao
    int isRLE = 0;
    unsigned long long total;
    long long n_blocks,totalRLE=0;
    unsigned long size_of_last_block, block_size, rlesize,firstBlockSize,lastBlockSize;

    char fileName[1000],fileNameAux[1000];
    snprintf(fileName, sizeof fileName, "%s", str);

    FILE *source = fopen(fileName, "rb");


        //caso de erro ao carregar o ficheiro
        if (source == NULL){
                fputs("Failed to open the file\n", stderr);
                return -1;
            }


    //Da o tamanho dos blocos e o seu numero
    block_size = defined_size;
    n_blocks = fsize(source, NULL, &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;

    /**Verificar limites de tamanhos nos blocos*/

    //Se o ultimo bloco for menor que 1KByte
    if (size_of_last_block < MIN_BLOCK_SIZE){
        n_blocks--;
        size_of_last_block+=block_size;
    }
    //Se o tamanho total for menor que 1KByte
    if (size_of_last_block < MIN_BLOCK_SIZE){
        printf("Ficheiro nao temos limites minimos de tamanho\n");
        return 0;
    }
    //Se so existir 1 bloco
    if(n_blocks==1){
            block_size = size_of_last_block;
            size_of_last_block = 0;
    }


    unsigned char *buffer = malloc(sizeof(char)*block_size);
    fread(buffer,sizeof(char),block_size,source);                                           //Le o primeiro bloco do ficheiro e guarda no array buffer

    unsigned char *bufferRLE = encode(buffer,block_size,&rlesize);                          //comprime buffer usando RLE e grava em bufferRLE, guarda o tamanho em rlesize
    firstBlockSize = rlesize;
    totalRLE+=rlesize;                                                                      //guarda o tamanho total de rle para mais tarde fazer a taxa de compressao total

    float compressionRate = (((float)block_size-(float)rlesize)/(float)block_size)*100;     //calcula a taxa de compressao em percentagem



    /** caso a compressao do primeiro bloco seja de 5% ou compressao forcada **/
    if(compressionRate>5 || isForced){
            isRLE = 1;
            strcat(fileName, ".rle");
            FILE *destination = fopen(fileName,"wb");              //cria ficheiro para guardar a compressao RLE

            snprintf(fileNameAux, sizeof fileNameAux, "%s.freq", fileName);
            FILE *frequencia = fopen(fileNameAux,"w");

            //caso de erro ao carregar algum dos ficheiros
                if (destination == NULL || frequencia == NULL){
                        fputs("Failed to open the file\n", stderr);
                        return -1;
                }

            fwrite(bufferRLE,sizeof(char),rlesize,destination);         //Escreve o  bloco comprimido por RLE na FILE .rle

            int *freq_array = contadoresShort(bufferRLE,rlesize);       //Guarda o calculo das frequencias do bloco
            fprintf(frequencia,"@R@%lld@", n_blocks);                   //Escreve a primeira parte na FILE .freq
            writeFreqFile(frequencia,freq_array,rlesize);               //Escreve as frequencias do bloco na FILE .freq

            //liberta espaco alocado nos arrays
            free(bufferRLE);
            free(freq_array);




                for(long long i=2;i<n_blocks;i++){                          //trata dos blocos de tamanho igual
                        fread(buffer,sizeof(char),block_size,source);       //Guarda os dados do bloco no buffer

                        bufferRLE = encode(buffer,block_size,&rlesize);     //comprime buffer usando rle e grava em bufferRLE

                        totalRLE+=rlesize;                                  //guarda o tamanho total de rle para mais tarde fazer a taxa de compressao total

                        fwrite(bufferRLE,rlesize,sizeof(char),destination); //Escreve o  bloco comprimido por RLE na FILE .rle

                        freq_array = contadoresShort(bufferRLE,rlesize);    //Guarda o calculo das frequencias do bloco
                        writeFreqFile(frequencia,freq_array,rlesize);       //Escreve as frequencias do bloco na FILE .freq

                        //liberta espaco alocado nos arrays
                        free(bufferRLE);
                        free(freq_array);





                }

            if(size_of_last_block>0){                                   //Trata do ultimo bloco, se este existir
            fread(buffer,sizeof(char),size_of_last_block,source);       //Guarda os dados do bloco no buffer

            bufferRLE = encode(buffer,size_of_last_block,&rlesize);     //comprime buffer usando rle e grava em bufferRLE

            totalRLE+=rlesize;                                          //guarda o tamanho total de rle para mais tarde fazer a taxa de compressao total

            lastBlockSize = rlesize;
            fwrite(bufferRLE,rlesize,sizeof(char),destination);         //Escreve o  bloco comprimido por RLE na FILE .rle

            freq_array = contadoresShort(bufferRLE,rlesize);            //Guarda o calculo das frequencias do bloco
            writeFreqFile(frequencia,freq_array,rlesize);               //Escreve as frequencias do bloco na FILE .freq

            //Liberta espaco alocado nos arrays
            free(bufferRLE);
            free(freq_array);
            }

            fprintf(frequencia,"0");                                   //Escreve a ultima parte na FILE .freq


            //Fecha os ficheiros
            fclose(destination);
            fclose(frequencia);
    }

    /**caso a compressao nao seja de 5% nem forcada**/
    else{

            strcat(fileName,".freq");
            FILE *frequencia = fopen(fileName,"w");

            //caso de erro ao carregar o ficheiros
            if (frequencia == NULL){
                        fputs("Failed to open the file\n", stderr);
                        return -1;
                }


            int *freq_array = contadoresShort(buffer,block_size);               //Guarda o calculo das frequencias do primeiro bloco
            fprintf(frequencia,"@N@%lld@", n_blocks);                           //Escreve a primeira parte na FILE .freq
            writeFreqFile(frequencia,freq_array,block_size);                    //Escreve as frequencias do bloco na FILE .freq

            //liberta espaco alocado no array
            free(freq_array);


            for(long long i=2;i<n_blocks;i++){                                  // trata dos blocos de tamanho igual
                    fread(buffer,sizeof(char),block_size,source);               //guarda os dados do bloco no buffer

                    freq_array = contadoresShort(buffer,block_size);            //Guarda o calculo das frequencias do bloco
                    writeFreqFile(frequencia,freq_array,block_size);            //Escreve as frequencias do bloco na FILE .freq

                    //liberta espaco alocado no array
                    free(freq_array);

            }

            if(size_of_last_block>0){                                           //Trata do ultimo bloco, se este existir
            fread(buffer,sizeof(char),size_of_last_block,source);               //Guarda os dados do bloco no buffer

            freq_array = contadoresShort(buffer,size_of_last_block);            //Guarda o calculo das frequencias do bloco
            writeFreqFile(frequencia,freq_array,size_of_last_block);            //Escreve as frequencias do bloco na FILE .freq

            //Liberta espaco alocado no array
            free(freq_array);
            }

            fprintf(frequencia,"0");                                            //Escreve a ultima parte na FILE .freq

            //Fecha os ficheiro
            fclose(frequencia);

        }
    //Fecha o ficheiro source e liberta o espaco alocado no buffer
    fclose(source);
    free(buffer);


    clock_t end = clock();                                              //Para a contagem do tempo de execucao
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;         //calcula o tempo de execucao


    //Escrever para a linha de comandos
    printf("Manuel Novais, a89512, Andre Vinagreiro a89986, MIEI/CD, 3-jan-2021\nModulo: f (calculo das frequencias dos simbolos)\n");
    printf("Numero de blocos: %1d\n",n_blocks);
    printf("Tamanho dos blocos analisados no ficheiro original: %lu/%lu bytes\n",block_size, size_of_last_block);
        if(isRLE){
                printf("Compressao RLE: %s ( %.2f%% compressao)\n",fileNameAux,(((float)total-(float)totalRLE)/(float)total)*100);
                printf("Tamanho dos blocos analisados no ficheiro RLE: %1u/%1u bytes\n",firstBlockSize,lastBlockSize);
                printf("Tempo de execucao do modulo (milisegundos): %f\n",time_spent*1000);
                printf("Ficheiros gerados: %s, %s\n",fileName,fileNameAux);
        }
        else{
                printf("Tempo de execucao do modulo (milisegundos): %f\n",time_spent*1000);
                printf("Ficheiros gerados: %s\n",fileName);
        }


   return 0;
}
