/*
*	mdata.h:	arquivo de inclusão com as estruturas de dados definidas para a
*				implementação do trabalho. Inclui a estrutura TCB (Thread Control Block)
*				e a estrutura empregada para o mutex.
*/

#ifndef MDATA_H
#define	MDATA_H

#define ERROR_CODE -1
#define SUCESS_CODE 0

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	READY, RUNNING, BLOCKED
}state_t;

typedef struct {
	ucontext_t context;
	int tid;
	int waitingThread;
	int contextFlag;
	double execTime;
	state_t state;
} TCB;

typedef struct TCBList{
	TCB* TCBElement;
    struct TCBList* next;
}TCBList;

typedef struct {
	int flag;
	TCBList * listMutex;
} mmutex_t;

#endif
