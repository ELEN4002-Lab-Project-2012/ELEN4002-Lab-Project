#include "Signal.h"

// TO DO: find a better way to intialise padded signals. 
Signal::Signal(int size, int padSize, double freq):
    samplingFreq(freq), 
    sampleSize(size), 
    paddedSize(padSize),
    plt("Input signal")
{
    freqRes = samplingFreq / sampleSize;       
    signal = new double[sampleSize];
    paddedSignal = new double[paddedSize];
    averagedSignal = new double[sampleSize];
    //loadTestData();
}

int Signal::counter = 0;                // counter to ensure that averaging only begins when signal buffer is full
bool Signal::ready = 0;                 // signal buffer is full

void Signal::loadTestData()             // Just an arbitrary example showing the FFT algorithm
{
    const double dt = 1.0/samplingFreq; 
    const double SSVEPfreq = 10, f1 = 2, f2 = 12, f3 = 18;
    srand((unsigned)time(0));           // Seed random number generator
    
    //cout << endl;
    for (std::size_t i = 0; i != sampleSize; ++i)
    {
        double randomNum = rand()%1;
        //                                         Fundamental                  Second harmonic                 Noise
        signal[i] = (double)rand()/RAND_MAX + sin(2*PI*SSVEPfreq*i*dt) + 0.5*sin(2*PI*2*SSVEPfreq*i*dt) + 0.4*sin(2*PI*f1*i*dt) + 0.6*sin(2*PI*f2*i*dt) + 0.5*sin(2*PI*f3*i*dt);
        //cout << signal[i] << ",";
    }
    //cout << endl;
}

void Signal::updateSignal(double *buffer, int bufferSize)
{
    rotate(signal, signal+bufferSize, signal+sampleSize);           // Shift all elements up 
    copy(buffer, buffer+bufferSize, signal+sampleSize-bufferSize);  // Replace the end elements with buffer
}

void Signal::averageSignal()
{
    for(int i = 0; i != sampleSize; i++)
        averagedSignal[i] = signal[i];
    eliminateDCOffset(averagedSignal, sampleSize);      // Remove the DC offset
}

void Signal::zeroPadSignal()
{
    
    // Copy existing elements. I had to use pointer arrays because Emotiv does. Is there another way to append zeroes?
    for(int i = 0; i != sampleSize; i++)
        paddedSignal[i] = signal[i];
    // In case no padding is added
    if(paddedSize == sampleSize)
        return;
    // Add new zeros to the end
    for(int i = sampleSize; i != paddedSize; i++)
        paddedSignal[i] = 0;
}

// This function closely approximates a low pass filter filter to eliminate DC offsets.  
void Signal::eliminateDCOffset(double *x, int size)
{
    counter++;
    if(counter > 3)
        ready = true;
    if(ready)
    {
        double windowAverage = getWindowAverage(x, size);
        //cout << "Widow average is " << windowAverage << endl;
        for (int i = 0; i != size; ++i)
            x[i] -= windowAverage;
    }
}


// Note that this is the average of UNPADDED signal. Important to call BEFORE padding
double Signal::getWindowAverage(double* x, int size)
{
    double sum = 0;   
    for (int i = 0; i != size; ++i)
        sum+=x[i];

    return sum/size;
}

// TO TEST OUTPUT------------------------------------------------------------------
void Signal::printSignalCSV()
{
    double dt = 1/samplingFreq;
    ofstream myfile;
    myfile.open("test2.csv");  
    myfile << "Time (s),Magnitude" << endl;
    for (int i=0; i!=sampleSize; i++)
    {
        myfile << i*dt << "," << signal[i] << endl;
    }
    myfile << endl;
    myfile.close();
}

void Signal::printPaddedSignalCSV()
{
    ofstream myfile;
    double dt = 1/samplingFreq;
    myfile.open("test2.csv");
    myfile << "Time (s),Magnitude" << endl;
    for (int i=0; i!=paddedSize; i++)
    {
        myfile << i*dt << "," << paddedSignal[i] << endl;
    }
    myfile << endl;
    myfile.close();
}

void Signal::displaySignalValues()
{
    cout << endl;
    for(int i = 0; i != sampleSize; i++)
    {
        cout << i << ", " << signal[i] << endl;
    }
}

void Signal::displayAveragedSignal()
{
    cout << "Averaged signal: " << endl;
    for(int i = 0; i != sampleSize; i++)
         cout << i << ", " << averagedSignal[i] << endl;
}

void Signal::plotSignal()
{
    plt.setTitle("Signal");
    plt.plot(signal, sampleSize);
}
