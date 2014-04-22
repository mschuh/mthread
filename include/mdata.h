/*
*	mdata.h:	arquivo de inclusão com as estruturas de dados definidas para a 
*				implementação do trabalho. Inclui a estrutura TCB (Thread Control Block)
*				e a estrutura empregada para o mutex.
*/

#ifndef __mdata__
#define	__mdata__ 1

#define ERROR_CODE -1

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	READY, RUNNING, BLOCKED, FINISHED
}state_t;

typedef struct {
	ucontext_t context;
	int tid;
	int waitingThread;
	double execTime;
	state_t state;
} TCB;

typedef struct {
	TCB* TCBElement;
    TCBList* next;
} TCBList;

typedef mutex {
	int flag;
	TCB * next;
} mmutex_t;


#endif