#ifndef modulof_H
#define modulof_H



#include <time.h>
#include "fsize.h"
#include <string.h>
#define MIN_BLOCK_SIZE 1024        // Tamanho minimo para o ultimo bloco = 1KByte

int moduloF( char *str, unsigned long defined_size, int isForced);


#endif
