#include "Logger.h"

Logger::Logger()
{
    // Refresh the files
    accuracyLogger.open("TestSuite.csv");
    accuracyLogger << "Test name,Accuracy" << endl;

    RLogger.open("RValues.csv");
    RLogger << "Test Name,Frequency,R values..." << endl;
}

void Logger::logAccuracy(const string testName, const double accuracy)
{
    accuracyLogger << testName << "," << accuracy << endl;
}

void Logger::logR(string testName, const double freq, const vector<double> R)
{
    RLogger << testName << "," << freq << ",";
    for(int i = 0; i != R.size(); i++)
        RLogger << R.at(i) << ",";

    RLogger << endl;
}

void Logger::plotR(const vector<double> R)
{
    const int displaySize = 50;
    
    if(R.size() > displaySize)
    {
        ofstream myfile;
        myfile.open("RPlot.csv");
        for(int i = R.size()-(displaySize-1); i != R.size(); i++)
            myfile << i << "," << R.at(i) << endl;
    }
    else
        return;
}