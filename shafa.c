#include <unistd.h>

/**includes e defines do moduloF**/
#include "moduloF\include\shafa.h"

/**includes e defines do moduloT**/
#include "moduloT\include\shafa.h"

int main (int argc, char **argv)
{
  char *nomeFicheiro;
  int c;



 while ((c = getopt (argc, argv, "-:m:")) != -1){
    switch (c)
      {
      case 'm':
          //caso seja -m f (modulo f)
          if (strcmp("f", optarg) ==0 ){
                shafaF(argc,argv,nomeFicheiro);
          }
          //caso seja -m t (modulo t)
          else if (strcmp("t", optarg) ==0 ){
                shafaT(nomeFicheiro);
          }
          //caso seja -m c (modulo c)
          else if (strcmp("c", optarg) ==0 ){

          }
          //caso seja -m d (modulo d)
          else if (strcmp("d", optarg) ==0 ){

          }
         break;

      case 1:
            nomeFicheiro = optarg;
            break;
      case ':':
            printf("Opcao -%c  precisa de argumentos\n",optopt);
            return 0;
      case '?':
            printf("unknown option\n");
            break;
      }
 }

}


