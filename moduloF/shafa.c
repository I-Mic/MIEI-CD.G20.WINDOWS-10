#include "include\shafa.h"



int shafaF (int argc, char **argv,char *nomeFicheiro){
    int  isForced = 0,opt,index;
    unsigned long block_size = D_BLOCK_SIZE;


    while ((opt = getopt (argc, argv, "-:m:b:c:")) != -1){
        switch(opt)
        {
            case 'm':
                break;
            case 'b':
                if (strcmp("K", optarg) ==0)block_size = K_BLOCK_SIZE;
                else if (strcmp("m", optarg) ==0) block_size = M_BLOCK_SIZE;
                else if( strcmp("M", optarg) ==0) block_size = MM_BLOCK_SIZE;
                else printf("Tamanho nao reconhecido, usar tamanho por defeito");
                break;
            case 'c':
                if (strcmp("r", optarg) ==0) isForced = 1;
                break;
            case ':':
                printf("Opcao -%c precisa de argumentos\n",optopt);
                return 0;
            case '?':
                printf("unknown option\n");
                break;
            case 1 :
                nomeFicheiro = optarg;
                break;
        }
    }

    moduloF(nomeFicheiro,block_size,isForced);
}
