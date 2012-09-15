#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Logger
{
public:
    Logger(string fileName);
    void logHeader(const char[]);

private:
    string logFileName;

};