#include "common/ithread.h"
#include <pthread.h>
#include <iostream>
#include <signal.h>

using namespace std;
using namespace common;

IThread::IThread()
{
}

void IThread::Run()
{
	pthread_create(&thread, NULL, static_routine, (void*)this);
}

void IThread::Wait()
{
	pthread_join(thread, NULL);
}

void* IThread::static_routine(void* arg)
{
	IThread* thread = (IThread*)arg;
	return thread->start_routine();
}

void IThread::Kill()
{
	//pthread_kill(thread, SIGALRM);
}
