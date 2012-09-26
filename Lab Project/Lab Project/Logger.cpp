#include "Logger.h"

Logger::Logger(string fileName):
    logFileName(fileName)
{
    // Refresh the files
    ofstream myfile;
    myfile.open(logFileName);
    myfile << "Test name,Accuracy" << endl;
    myfile.close();

    myfile.open("RValues.csv");
    myfile << "freq,R values..." << endl;
    myfile.close();
}

void Logger::logAccuracy(const string testName, const double accuracy)
{
    ofstream myfile;
    myfile.open(logFileName, ios::app);
    myfile << testName << "," << accuracy << endl;
    myfile.close();
}

void Logger::logR(const double freq, const vector<double> R)
{
    ofstream myfile;
    myfile.open("RValues.csv", ios::app);
    myfile << freq << ",";
    for(int i = 0; i != R.size(); i++)
        myfile << R.at(i) << ",";

    myfile << endl;
}