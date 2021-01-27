#ifndef modulot_H
#define modulot_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct ListaFreq{
    int freq;                     //frequencia
    char *cod;                    //codigo
    struct ListaFreq *prox, *ant;
} *ListaF;

void moduloT (char *nomeFicheiro);

#endif


