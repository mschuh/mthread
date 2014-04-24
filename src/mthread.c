#include "../include/linkedlist.h"
#include "../include/mthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

TCBList* readyList = NULL;
TCBList* blockedList = NULL;
TCBList* aliveList = NULL;
TCB* runningThread = NULL;
int tidCounter = 0;
char stack[SIGSTKSZ]; //reference stack
double	t0, t1;

ucontext_t mainContext;
ucontext_t schedulerContext;
ucontext_t exitThreadContext;


double	GetTime(void)
{
   struct  timeval time;
   double  Time;

   gettimeofday(&time, (struct timezone *) NULL);
   Time = ((double)time.tv_sec*1000000.0 + (double)time.tv_usec);
   return(Time);
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

int Scheduler (void)
{
	runningThread = Pop(&readyList);
	runningThread->state = RUNNING;
	t0 = GetTime();
	return setcontext(&(runningThread->context));
}

void CreateSchedulerContext (void)
{
	getcontext (&schedulerContext);
	schedulerContext.uc_stack.ss_sp = (char*)malloc(sizeof(stack));
	schedulerContext.uc_stack.ss_size = sizeof(stack);
	schedulerContext.uc_link = NULL;
	makecontext (&schedulerContext, (void (*)(void))Scheduler, 0);
}


void ExitThread (void)
{
	TCB* blockedThread;

	blockedThread = RemoveWaiting(&blockedList, runningThread->tid);

	if (blockedThread != NULL)
	{
		blockedThread->state = READY;
		blockedThread->waitingThread = -1;
		readyList = InsertSorted(readyList, blockedThread);
	}

	Remove(&aliveList, runningThread->tid);
	setcontext(&schedulerContext);
}


void CreateExitThreadContext (void)
{
	getcontext (&exitThreadContext);
	exitThreadContext.uc_stack.ss_sp = (char*)malloc(sizeof(stack));
	exitThreadContext.uc_stack.ss_size = sizeof(stack);
	exitThreadContext.uc_link = NULL;
	makecontext (&exitThreadContext, (void (*)(void))ExitThread, 0);

}

int mcreate (void (*start_routine)(void*), void *arg)
{
	TCB* newThread;

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

				aliveList = Insert(aliveList, mainThread);

				//sets mainContext return point, allocates stack and defines its size
                		mainThread->context = mainContext;
				mainThread->context.uc_stack.ss_sp = (char*)malloc(sizeof(stack));
				mainThread->context.uc_stack.ss_size = sizeof(stack);
				mainThread->context.uc_link = NULL; //return point when main ends should be null

				//creates context for scheduler function
				CreateSchedulerContext();

				//creates context for exit thread function
				CreateExitThreadContext();

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
			newThread->context.uc_link = &exitThreadContext;
			makecontext (&(newThread->context), (void (*)(void))start_routine, 1, arg);
			readyList = InsertSorted(readyList, newThread);

			aliveList = Insert(aliveList, newThread);

			return newThread->tid;
		}
		else
			return ERROR_CODE;
	}
	else
		return 	ERROR_CODE;

}

int myield(void)
{
    if (runningThread == NULL)
        return ERROR_CODE;

	t1 = GetTime();
	runningThread->execTime = t1 - t0;
	runningThread->state = READY;
	readyList = InsertSorted(readyList, runningThread);

	runningThread->contextFlag = 0;
	getcontext(&(runningThread->context));
	if (runningThread->contextFlag == 0)
	{
        runningThread->contextFlag = 1;
        if (Scheduler() == ERROR_CODE)
            return ERROR_CODE;
	}

    return SUCESS_CODE;
}

int mjoin(int thr)
{			
	t1 = GetTime();
	runningThread->execTime = t1 - t0;


	if (thr > tidCounter || thr < 0 || (searchWaiting(aliveList, thr) == SUCESS_CODE))
        	return ERROR_CODE;
	else if (searchTID(aliveList, thr) == SUCESS_CODE)
	{
		runningThread->state = BLOCKED;
		runningThread->waitingThread = thr;
		blockedList = Insert(blockedList, runningThread);
	
		runningThread->contextFlag = 0;
		getcontext(&(runningThread->context));
		if (runningThread->contextFlag == 0)
		{
		    runningThread->contextFlag = 1;
		    if (Scheduler() == ERROR_CODE)
		        return ERROR_CODE;
		}
	}
	else
	{
		runningThread->state = READY;
		readyList = InsertSorted(readyList, runningThread);
	
		runningThread->contextFlag = 0;
		getcontext(&(runningThread->context));
		if (runningThread->contextFlag == 0)
		{
		    runningThread->contextFlag = 1;
		    if (Scheduler() == ERROR_CODE)
		        return ERROR_CODE;
		}
	}

	return SUCESS_CODE;
}

int mmutex_init(mmutex_t *mutex) //initializes a mutex variable
{
	mutex->flag = 0;
	mutex->listMutex = NULL;
	
	return SUCESS_CODE;
}
int mlock (mmutex_t * mutex)
{
	if(mutex->flag == 0) //if the critic zone is free 
	{
		mutex->flag = 1;
		return SUCESS_CODE;
	}
	else	//the critic zone is not free, blocks the thread
	{
		runningThread->state = BLOCKED;
		runningThread->waitingThread = -1;
		InsertLast(mutex->listMutex, runningThread);

		//saves the context because the thread was blocked
		runningThread->contextFlag = 0; 
		getcontext(&(runningThread->context));
		if (runningThread->contextFlag == 0)
		{
			runningThread->contextFlag = 1;
        		if (Scheduler() == ERROR_CODE)
				return ERROR_CODE;
		}
	}
	
	return SUCESS_CODE;
}
int munlock (mmutex_t *mutex)
{
	mutex->flag = 0; //critic zone now is free
	
	TCB* mutexThread = (TCB*)malloc(sizeof(TCB));
	
	if (mutex->listMutex != NULL) //if the list is not empty
	{	
		mutexThread = Pop(&mutex->listMutex);
		mutexThread->state = READY; //set the next thread waiting tu use the critic zone to ready
	}

	return SUCESS_CODE;
}


