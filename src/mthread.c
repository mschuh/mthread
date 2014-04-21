//não sei totalmente o que vai aqui mas sei que as listas de apto, bloqueado e bla bla são por aquis

#include "../include/mthread.h"
#include "../include/linkedlist.h"

TCBList* readyList = NULL;
TCBList* blockedList NULL;
TCB* runningThread = NULL;
int tidCounter = 0;

ucontext_t mainContext;
ucontext_t schedulerContext;
ucontext_t exitThreadContext;

int mcreate (void (*start_routine)(void*), void *arg)
{
	TCB* newThread;
	char stack[SIGSTKSZ];

	if (tidCounter == 0) //verifies if it's the first time a thread is created, then do initializations
	{
		if (getcontext (&mainContext) != -1) //generates a copy of the context for the main and returns -1 in case of error
		{
			TCB* mainThread = CreateTCB(); //allocates the TCB for the main thread

			if (mainThread != NULL) //CreateTCB returns null in case of error
			{
				//sets main TCB parameters
				mainThread->state = RUNNING; //the first thread running is the main
				runningThread = mainThread;
				mainThread->context = mainContext;

				//sets mainContext return point, allocates stack and defines its size
				mainThread->context = mainContext;
				mainThread->context.uc_stack.ss_sp = (char*)malloc(sizeof(stack));
				mainThread->context.uc_stack.ss_size = sizeof(stack);
				mainThread->context.uc_link = NULL; //return point when main ends should be null
			}
			else
				return ERROR_CODE;
		}
		else
			return ERROR_CODE;
	}


	//start of the creation of the new thread
	newThread = CreateTCB(); //allocates the TCB for the new thread

	if (newThread != NULL) //CreateTCB returns null in case of error
	{

		if (getcontext (&(newThread->context)) != -1) ////generates a copy of the context for the newThread and returns -1 in case of error
		{
			//sets newThread return point, allocates stack and defines its size
			newThread->context.uc_stack.ss_sp = (char*)malloc(sizeof(stack));
			newThread->context.uc_stack.ss_size = sizeof(stack);
			//newThread->context.uc_link = ?;  praonde vai, vai pra antes do escalonador, ver se tem alguem esperando por elas
			makecontext (&(newThread->context), (void (*)(void))start_routine, 1, arg);
			readyList = Insert (readyList, newThread);

			return newThread->tid;
		}
		else
			return ERROR_CODE;
	}
	else
		return 	ERROR_CODE;

}


TCB* CreateTCB (void)
{
	TCB* newTCB = (TCB*)malloc(sizeof(TCB)); //allocates the memory space for the TCB

	if (newTCB != NULL)
	{
	//sets TCB parameters
	newTCB->tid = tidCounter;
	newTCB->execTime = 0.0;
	newTCB->state = READY;
	newTCB->waitingThread = -1; //isn't waiting for any thread
	tidCounter++;
	}

	return newTCB; //if an error occured during the malloc it won't occupy the tid and return NULL anyway
}



int myield(void)
{
	runningThread->state = READY;
	readyList = Insert (readyList, runningThread); //tem que inserir de maneira ordenadas, zuous
	return Scheduler(); 
}

int mjoin(int thr)
{
	runningThread->state = BLOCKED;
	runningThread->waitingThread = thr;
	return Scheduler();
}