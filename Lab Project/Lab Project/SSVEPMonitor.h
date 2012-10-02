#ifndef SSVEPMONITOR_H
#define SSVEPMONITOR_H

#include <vector>
#include <iostream>
using namespace std;
    
typedef vector<vector<double> > RValues;            // 2D vector to store R vs. frequency
struct FreqDetections {
    vector<double> frequencies;
    vector<bool> detected;
    vector<double> Rvalues;
};
struct FreqAccuracy {
    vector<double> frequencies;
    vector<double> Accuracy;
};

class SSVEPMonitor
{
public:
    SSVEPMonitor(vector<double> freqs);                     // For use with multiple frequencies
    SSVEPMonitor(double freq);                              // For use with one frequency
    FreqDetections monitorSSVEPDetections(const vector<double> &R, double threshold);  // Analyses R values (multiple frequencies)
    vector<double> getAllRForFreq(double freq);             // Return all R values for that frequency
    FreqDetections getLatestR() {return detections;}
    FreqAccuracy calculateAccuracy();

private:
    RValues allRvalues;                 // Stores all R values for all frequencies. Each primary index corresponds to each frequency
    FreqDetections detections;
    vector<double> frequencies;
    vector<int> counter;                // A counter for each frequency
    vector<int> nPositiveDetections;
    unsigned int nTotalDetections;
};

#endif