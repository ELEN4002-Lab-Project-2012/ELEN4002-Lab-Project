#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <string>                   // for STL string class
#include <windows.h>                // for HANDLE
#include <process.h>                // for _beginthread()
#include <iostream>
#include <boost\shared_ptr.hpp>
#include "Runnable.h"
using namespace std;

/*  Why use a thread wrapper class? 
    The point is that we don't want to change our existent classes by adding multithreading functionality.
    Adding a Thread wrapper classes handles all multithreading internally so that we don't alter our classes.
    The only necessary change is inheriting from Runnable and adding a run() method (see simpleRunnable.h)

    Thread wrapper class. This class handles all internal details regarding C++ multithreading. Brilliant!
    To use the class, simple instantiate the Thread with the type of object you want to run on it.
    Then call the start function, which calls the base class (Runnable's) run() function. Make sure that the
    Runnable derived class's run() function calls the main loop for the class that you want to run on the thread
*/

class Thread {
public:
	Thread(boost::shared_ptr<Runnable> run);
	virtual ~Thread();
	void start();
    bool waitForCompletion();
private:
    string threadName;
	HANDLE hThread;
	unsigned wThreadID;
    DWORD dwExitCode;

	boost::shared_ptr<Runnable> runnable;
	static unsigned __stdcall startThreadRunnable(LPVOID pVoid);
};

#endif