#include "Thread.h"

Thread::Thread(boost::shared_ptr<Runnable> r): runnable(r) {
	if(!runnable.get())
		cerr << "Thread initialisation failed" << endl;
	hThread = (HANDLE)_beginthreadex(NULL,0,Thread::startThreadRunnable, (LPVOID)this, CREATE_SUSPENDED, &wThreadID);
	if(!hThread)
		cerr << "_beginthreadex failed at " << endl;

    GetExitCodeThread( hThread, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
    //printf( "initial thread 1 exit code = %u\n", dwExitCode );
}

Thread::~Thread() {
    GetExitCodeThread( hThread, &dwExitCode );
    //printf( "thread exited with code %u\n", dwExitCode );
	if(wThreadID != GetCurrentThreadId()) {
		DWORD rc = CloseHandle(hThread);
		if(!rc) 
			cerr << "CloseHandle failed at ";
	}
}

unsigned __stdcall Thread::startThreadRunnable(LPVOID pVoid) {
	Thread* runnableThread = static_cast<Thread*>(pVoid);
	runnableThread->runnable->run();
    return 1;
}

void Thread::start() {
	assert(hThread);
	DWORD rc = ResumeThread(hThread);
	// thread created is in suspended state, 
	// so this starts it running
	if(!rc) 
        cerr << "ResumeThread failed" << endl;

}

bool Thread::waitForCompletion()
{
    WaitForSingleObject( hThread, INFINITE );
    return true;
}