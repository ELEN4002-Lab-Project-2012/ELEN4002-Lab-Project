#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Logger
{
public:
    Logger(string fileName);
    void logAccuracy(const string testName, const double accuracy);
    void logR(const double freq, const vector<double> R);

private:
    string logFileName;
};