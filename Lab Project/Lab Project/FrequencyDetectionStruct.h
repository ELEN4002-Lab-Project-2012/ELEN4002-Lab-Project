#ifndef FreqDetections_H
#define FreqDetections_H

#include <vector>
using namespace std;

struct FreqDetections {
    vector<double> frequencies;
    vector<bool> detected;
    vector<double> Rvalues;
};
#endif