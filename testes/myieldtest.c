//*******************************************************************
//                                                                  *
//              Universidade Federal do Rio Grande do Sul           *
//                   Instituto de Informática                       *
//                                                                  *
//                 INF 01142 - Sistemas Operacionais I              *
//                                                                  *
//               libumthread - Gerenciador de threads N:1           *
//                                                                  *
//                     Felipe Nogueira - 219827                     *
//                    Matheus Schuh - 219824                        *
//                                                                  *
//*******************************************************************
//This test aims to simulate a simple program using the mthread
//libray. It creates two threads, each one emiting a message
//and identifying itself. The threads yield and the scheduler
//of the library takes the SPN model, so the shortest thread
//is executed first.
//************************** INCLUDES *******************************


#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include "../include/mthread.h"    /* Thread Library */
#include <string.h>     /* String handling */

/* prototype for thread routines */
void print_message_function ( void *ptr );
void print_message_function_long ( void *ptr ); 

/* struct to hold data to be passed to a thread
   this shows how multiple data items can be passed to a thread */
typedef struct str_thdata
{
    int thread_no;
    char message[100];
} thdata;

int main()
{
    thdata data1, data2;         /* structs to be passed to threads */
    int thread1, thread2;
    
    /* initialize data to pass to thread 1 */
    data1.thread_no = 1;
    strcpy(data1.message, "Hello!");

    /* initialize data to pass to thread 2 */
    data2.thread_no = 2;
    strcpy(data2.message, "Hi!");
    
    /* create threads 1 and 2 */    
    thread1 = mcreate ((void *) &print_message_function, (void *) &data1);
    if (thread1 == -1)
    {
        printf("Error creating thread 1, exiting...\n");
        exit(70); //error code for internal software error
    }

    thread2 = mcreate ((void *) &print_message_function_long, (void *) &data2);
    if (thread2 == -1)
    {
        printf("Error creating thread 2, exiting...\n");
        exit(70);
    }

    /* Main block now waits for both threads to terminate, before it exits
       If main block exits, both threads exit, even if the threads have not
       finished their work */ 
    if (mjoin(thread1) == -1)
    {
        printf("Error syncing thread 1, exiting...\n");
        exit(70);
    }

    if ( mjoin(thread2) == -1)
    {
        printf("Error syncing thread 2, exiting...\n");
        exit(70);
    }

    /* exit */  
    printf ("Perfect execution, now exiting normally...\n");
    exit(0);
} /* main() */

/**
 * print_message_function is used as the start routine for the threads used
 * it accepts a void pointer 
**/
void print_message_function ( void *ptr )
{
    thdata *data;            
    data = (thdata *) ptr;  /* type cast to a pointer to thdata */
    
    /* do the work */
    printf("Short Thread %d says %s \n", data->thread_no, data->message);
    printf("Thread %d yielding\n", data->thread_no);
    myield();
    printf("Short Thread running again cause is less time consuming says %s \n", data->message);
    
} /* print_message_function ( void *ptr ) */

void print_message_function_long ( void *ptr )
{
    thdata *data;   
    int i;         
    data = (thdata *) ptr;  /* type cast to a pointer to thdata */
    
    /* do the work */
    printf("Long Thread %d says %s \n", data->thread_no, data->message);
    for (i=0; i < 1000000; i++); //consumes CPU time
    printf("Thread %d yielding\n", data->thread_no);
    myield();
    printf("Long Thread running before cause is more time consuming says %s \n", data->message);
    
} /* print_message_function ( void *ptr ) */