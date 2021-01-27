#ifndef shafa_H
#define shafa_H

#include <unistd.h>
#include <getopt.h>
#include "modulof.h"

#define D_BLOCK_SIZE 65536              //default block size = 64 KBytes
#define K_BLOCK_SIZE 655360             // block size = 640 KBytes
#define M_BLOCK_SIZE 8388608            // block size = 8 MBytes
#define MM_BLOCK_SIZE 67108864          // block size = 64 MBytes

int shafaF (int argc, char **argv, char *nomeFicheiro);

#endif
