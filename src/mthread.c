//*******************************************************************
//                                                                  *
//              Universidade Federal do Rio Grande do Sul           *
//	                 Instituto de Informática                 		*
//                                                                  *
//	               INF 01142 - Sistemas Operacionais I            	*
//                                                                  *
//               libumthread - Gerenciador de threads N:1           *
//                                                                  *
//	                   Felipe Nogueira - 219827                   	*
//                   	Matheus Schuh - 219824	                    *
//                                                                  *
//*******************************************************************



//************************** INCLUDES *******************************



#include "../include/linkedlist.h"
#include "../include/mthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>



//****************** LISTS AND GLOBAL VARIABLES *********************



TCBList* readyList = NULL;
TCBList* blockedList = NULL;
TCBList* aliveList = NULL;
TCB* runningThread = NULL;

int tidCounter = 0; //atributes a unique number to each thread
double	t0, t1; //used to made teh SPN policy in the scheduler
char stack[SIGSTKSZ]; //reference stack and stack size

ucontext_t mainContext;
ucontext_t schedulerContext;
ucontext_t exitThreadContext;



//***************** AUXILIARY FUNCTIONS *****************************



//Returns the timer at the point when it's called (in usec)
double	GetTime(void)
{
   struct  timeval time;
   double  Time;

   gettimeofday(&time, (struct timezone *) NULL);
   Time = ((double)time.tv_sec*1000000.0 + (double)time.tv_usec);
   return(Time);
}

//Allocate and initializate the fields of a TCB
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

//Scheduler that pops from the ready list and executes the next shortest process
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

//Deals with the end of a thread and its consequences
void ExitThread (void)
{
	TCB* blockedThread;
	blockedThread = RemoveWaiting(&blockedList, runningThread->tid); //verifies if any thread is blocked waiting for the end of this one

	if (blockedThread != NULL)
	{
		//put the blocked thread in the ready list again
		blockedThread->state = READY;
		blockedThread->waitingThread = -1;
		readyList = InsertSorted(readyList, blockedThread); 
	}

	Remove(&aliveList, runningThread->tid); //kills the thread removing it from the alive list
	free(runningThread); //deallocates the TCB
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



//************* MAIN FUNCTIONS PRESENTED IN HEADER ******************



//Creation of a thread. In sucess return its ID, in case of error returns -1
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

				t0 = GetTime(); //for correct measuring of main time
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
			newThread->context.uc_link = &exitThreadContext; //return point to a function that deals with thread termination
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

//Voluntary transfer of CPU usage. Returns 0 in sucess, -1 in error
int myield(void)
{
    if (runningThread == NULL)
        return ERROR_CODE;

	t1 = GetTime();
	runningThread->execTime = t1 - t0; //calculates execution time
	runningThread->state = READY; //puts thread in ready list
	readyList = InsertSorted(readyList, runningThread);

	runningThread->contextFlag = 0;
	getcontext(&(runningThread->context)); //assures the return of the function
	if (runningThread->contextFlag == 0)
	{
        runningThread->contextFlag = 1;
        if (Scheduler() == ERROR_CODE)
            return ERROR_CODE;
	}
    return SUCESS_CODE;
}

//Finish sincronization. Returns 0 in sucess, -1 in error
int mjoin(int thr)
{			
	t1 = GetTime();
	runningThread->execTime = t1 - t0; //calculates execution time

	
	if (thr > tidCounter || thr < 0 || (searchWaiting(aliveList, thr) == SUCESS_CODE)) //checks if the id is valid or if another thread is already waiting for this id
        	return ERROR_CODE;
	else if (searchTID(aliveList, thr) == SUCESS_CODE) //checks if the thread is alive
	{
		runningThread->state = BLOCKED;
		runningThread->waitingThread = thr;
		blockedList = Insert(blockedList, runningThread);

		runningThread->contextFlag = 0;
		getcontext(&(runningThread->context));

		if (runningThread->contextFlag == 0)
		{
		    runningThread->contextFlag = 1;
		    return Scheduler();
		}
	}
	else //if the thread is valid but has already finished
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


