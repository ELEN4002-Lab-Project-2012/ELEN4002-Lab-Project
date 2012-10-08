#include "SSVEPMonitor.h"

SSVEPMonitor::SSVEPMonitor(vector<double> freqs):
    nTotalDetections(0),
    allRvalues(freqs.size()),
    minCount(0), maxCount(0)
{
    frequencies = freqs;
    detections.frequencies = freqs;
    for(int i = 0; i != freqs.size(); i++) {
        counter.push_back(0);
        detections.detected.push_back(0);
        detections.Rvalues.push_back(0);
        nPositiveDetections.push_back(0);
    }
}    

SSVEPMonitor::SSVEPMonitor(double freq):
    allRvalues(1),   // Size 1 (for 1 freq)
    nTotalDetections(0)
{
    frequencies.push_back(freq);
    detections.frequencies = frequencies;
    for(int i = 0; i != frequencies.size(); i++) {
        counter.push_back(0);
        detections.detected.push_back(0);
        detections.Rvalues.push_back(0);
        nPositiveDetections.push_back(0);
    }
}

vector<double> SSVEPMonitor::getAllRForFreq(double freq)
{
    // Search for the frequency among existing frequencies
    int freqIndex = 0; bool found = false;
    for(int i = 0; i != frequencies.size(); i++) {
        if(frequencies.at(i) == freq) {
            freqIndex = i;
            found = true;
        }
    }
    if(!found)
        cerr << "Cannot return R values for freq = " << freq << endl;
    return allRvalues.at(freqIndex);
}

FreqDetections SSVEPMonitor::monitorSSVEPDetections(const vector<double> &R, double threshold)
{
    if(frequencies.size() != R.size())
        cerr << "Freq vector size not equal to Ratio vector size" << endl;

    nTotalDetections++;
    for(int i = 0; i != frequencies.size(); i++)
    {
        minCount = 6; 
        maxCount = 13;
        detections.Rvalues.at(i) = R.at(i);     // The latest R value for reference (overwrite, don't push)
        if(allRvalues.size() > 10000)
            cerr << "allRvalues reached a maximum. Please restart the program" << endl;
        
        allRvalues.at(i).push_back(R.at(i));    // add the latest R value in the 
        // If the SSVEP frequency is detected on any channel, increment the counter
        if(R.at(i) > threshold)
        {
            if(counter.at(i) < maxCount)        // The counter can reach a maximum of maxCount. 
                counter.at(i)++;
        }
        else 
        {
            if(counter.at(i) > 0)
                counter.at(i)--;                // Counter cannot go below 0      
        }
    
        // counter value of "minCount" indicates a successful detection.
        // "Hysteresis" of "minCount - minCount + 1". That is, if the count reaches 5, it requires 
        // 3 non-successful detections to return an overall negative SSVEP detection. 
        if(counter.at(i) >= minCount && counter.at(i) <= maxCount) {
            cout << "counter.at(i) = " << counter.at(i) << endl;
            cout << " SSVEP present for freq = " << frequencies.at(i) << " :-)" << endl;
            detections.detected.at(i) = true;
            nPositiveDetections.at(i)++;
        }
        else {
            detections.detected.at(i) = false;
            cout << "SSVEP not present for freq = " << frequencies.at(i) << " :(" << endl;
        }
    }

    return detections;
}

FreqAccuracy SSVEPMonitor::calculateAccuracy()
{
    FreqAccuracy accuracy;
    if(nTotalDetections < 3)
        cerr << "Too few detections to calculate accuracy" << endl;

    // Remember to get rid of the first few iteration results (up to minCount) which are always negative. 
    for(int i = 0; i != frequencies.size(); i++) {
        accuracy.Accuracy.push_back(((double)nPositiveDetections.at(i)/(double)(nTotalDetections-(minCount-1)))*100); 
        accuracy.frequencies.push_back(frequencies.at(i));
    }
    
    return accuracy;
}