#include "../include/mthread.h"
#include <stdio.h>

//global variables
int x = 0, y = 0;

/* this function is run by the second thread */
void inc_x(void *x_void_ptr)
{

/* increment x to 100 */
int *x_ptr = (int *)x_void_ptr;

printf ("\n----executing thread------\n");

while(++(*x_ptr) < 100);


printf("x increment finished\n");

printf ("\n----finishing thread------\n");


}

int main()
{

/* show the initial values of x and y */
printf("x: %d, y: %d\n", x, y);

int id = mcreate(inc_x, &x);
int id_2 = mcreate (inc_x, &x);

/* create a second thread which executes inc_x(&x) */
if(id == -1) {

fprintf(stderr, "Error creating thread\n");
return 1;

}
/* increment y to 100 in the first thread */
while(++y < 100);

printf("y increment finished\n");

mjoin(id);
printf ("main has been desblocked");
mjoin (id_2);

/* show the results - x is now 100 thanks to the second thread */
printf("x: %d, y: %d\n", x, y);

return 0;

}
