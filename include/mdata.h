/*
*	mdata.h:	arquivo de inclusão com as estruturas de dados definidas para a 
*				implementação do trabalho. Inclui a estrutura TCB (Thread Control Block)
*				e a estrutura empregada para o mutex.
*/

#ifndef MDATA_H
#define	MDATA_H

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

typedef struct TCBList{
	TCB* TCBElement;
    struct TCBList* next;
}TCBList;

typedef struct TIDList{
	int tid
	struct TIDList* next;
}TIDList;

typedef struct {
	int flag;
	TCB * next;
} mmutex_t;

#endif
