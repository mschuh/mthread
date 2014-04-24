
/* 
Programa primo.c 

Exemplo de uso de programação concorrente. Analise o que o programa
faz e verifique se ele está correto. Execute mais de uma vez.

Compile com: gcc -o primo primo.c -Wall -lpthread -lm 
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/mthread.h"
#include <math.h> 

#define MAX 8

int counter = 0;

int isPrime(int num) {
   int i;

   if(num == 0)
     return 1;
   for(i = 2; i <= (int) sqrt((double)num); i++)
       if(num % i == 0) 
         return 0;
   return 1;
}

void primos(void * id_ptr) {
  int *id = (int *)id_ptr;
  int i;
  long int limit = (long int) pow(10,5);

printf ("\n----executing thread id: %d------\n", *id);

  while (counter < limit) {
    counter = counter + 1;
    if (isPrime(counter)) {
       if ( counter % 2 == 0 )
          printf("%8d (id=%1d) PRIMO!?\n", counter, *id);
       else
          printf("%8d (id=%1d)", counter, *id);
    }
    for (i=0; i < pow(10,5); i++); /*apenas para consumir tempo CPU*/
  }

printf ("\n----finishing thread id: %d------\n", *id);


}

int main(int argc, char* argv[]) {
    int i=1;
    int i2 = 2;
    int id1;
    int id2;

    id1 = mcreate(primos, &i);
    printf ("\n\nID 1 VALE: %d\n\n", id1);
    id2 = mcreate(primos, &i2);
    printf ("\n\nID 2 VALE: %d\n\n", id2);

    mjoin(id1);
    mjoin(id2);
   
    exit(0);
}
