#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Logger
{
public:
    Logger();
    void logAccuracy(const string testName, const double accuracy);
    void logR(string testName, const double freq, const vector<double> R);
    //void logR(const double freq, const double R);
    void plotR(const vector<double> R);

private:
    ofstream accuracyLogger;
    ofstream RLogger;
};