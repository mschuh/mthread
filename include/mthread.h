/*
 * mthread.h: arquivo de inclusão com os prototypes das funções a serem
 *            implementadas na realização do trabalho.
 *
 * NÃO MODIFIQUE ESTE ARQUIVO.
 */

#ifndef __mthread__
#define __mthread__ 1

#include "mdata.h”"  /* arquivo de inclusão com as estruturas de dados definidas pelo
                     * grupo na implementação do trabalho. Inclui a estrutura TCB e a
                     * estrutura empregada para o mutex, como por exemplo:
                     *
                     * typedef mutex {
                     *   int flag;
                     *   TCB *next;
                     * } mmutex_t;
                     */
 

int mcreate (void (*start_routine)(void*), void *arg);
int myield(void);
int mjoin(int);
int mmutex_init(mmutex_t *);
int mlock (mmutex_t *);
int munlock (mmutex_t *);

#endif