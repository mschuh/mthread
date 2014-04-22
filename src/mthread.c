#include "../include/mthread.h"
#include "../include/linkedlist.h"

TCBList* readyList = NULL;
TCBList* blockedList NULL;
TCB* runningThread = NULL;
int tidCounter = 0;
double	t0, t1;

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
			newThread->context.uc_link = exitThreadContext;
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

void CreateSchedulerContext (void)
{
	getcontext (&schedulerContext);
	schedulerContext.uc_stack = (char*)malloc(sizeof(stack));
	schedulerContext.uc_stack.ss_size = sizeof(stack);
	schedulerContext.uc_link = NULL;
	makecontext (&schedulerContext, (void (*)(void))Scheduler, 0);
}

void CreateExitThreadContext (void)
{
	getcontext (&exitThreadContext);
	exitThreadContext.uc_stack = (char*)malloc(sizeof(stack));
	exitThreadContext.uc_stack.ss_size = sizeof(stack);
	exitThreadContext.uc_link = NULL;
	makecontext (&exitThreadContext, (void (*)(void))ExitThread, 0);

}

/* Fornece o temporizador no momento da chamada (em usec)*/
double	GetTime(void)
{
   struct  timeval time;
   double  Time;
   
   gettimeofday(&time, (struct timezone *) NULL);
   Time = ((double)time.tv_sec*1000000.0 + (double)time.tv_usec);
   return(Time);
}

int myield(void)
{
	t1 = GetTime();
	runningThread->execTime = t1 - t0;
	runningThread->state = READY;
	readyList = Insert (readyList, runningThread); //tem que inserir de maneira ordenadas, zuous
	return Scheduler(); 
}

int mjoin(int thr)
{
	t1 = GetTime();
	runningThread->execTime = t1 - t0;
	runningThread->state = BLOCKED;
	runningThread->waitingThread = thr;
	blockedList = Insert (blockedList, runningThread);
	return Scheduler();
}

int Scheduler (void)
{
	runningThread = pop (readyList);
	runningThread->state = RUNNING;
	t0 = GetTime();
	setcontext(&(runningThread->context));
}

void ExitThread (void)
{
	TCB* blockedThread;

	blockedThread = Remove(blockedList, runningThread->id);
	
	if (blockedThread != NULL)
	{
		blockedThread->state = READY;
		blockedThread->waitingThread = -1;
		readyList = Insert(readyList, blockedThread);
		setcontext(&schedulerContext);
	}
	else
		return NULL;
}


