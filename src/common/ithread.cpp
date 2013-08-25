#include <common/ithread.h>
#include <pthread.h>
#include <iostream>
#include <signal.h>

using namespace std;
using namespace common;

IThread::IThread()
{
}

void IThread::run()
{
	pthread_create(&thread, NULL, static_routine, (void*)this);
}

void IThread::wait()
{
	pthread_join(thread, NULL);
}

void* IThread::static_routine(void* arg)
{
	IThread* thread = (IThread*)arg;
	return thread->start_routine();
}

void IThread::kill()
{
	//pthread_kill(thread, SIGALRM);
}
