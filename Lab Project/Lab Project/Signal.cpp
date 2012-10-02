#include "Signal.h"

// TO DO: find a better way to intialise padded signals. 
Signal::Signal(int size, int padSize, double freq, Aquila::WindowType window):
    samplingFreq(freq), 
    sampleSize(size), 
    paddedSize(padSize),
    plt("Input signal"),
    windowFunction(window, size)
{
    freqRes = samplingFreq / sampleSize;       
    signal = new double[sampleSize];
    paddedSignal = new double[paddedSize];
    averagedSignal = new double[sampleSize];
}

int Signal::counter = 0;                // counter to ensure that averaging only begins when signal buffer is full
bool Signal::ready = 0;                 // signal buffer is full
int Signal::testCounter = 0;

void Signal::loadTestData()             // Just an arbitrary example showing the FFT algorithm
{
    const double dt = 1.0/samplingFreq; 
    const double testFreq = 13, f1 = 2, f2 = 15, f3 = 18;
    srand((unsigned)time(0));           // Seed random number generator
    testCounter++;
    
    double* signal1 = new double[sampleSize];
    double* signal2 = new double[sampleSize];
    double* signal3 = new double[sampleSize];
    double* signal4 = new double[sampleSize];

    for (std::size_t i = 0; i != sampleSize; ++i)
    {
        signal1[i] = 1*(double)rand()/RAND_MAX + 0.5*sin(2*PI*testFreq*i*dt) + 0.4*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*2*i*dt) + 0.6*sin(2*PI*11*i*dt) + 0.5*sin(2*PI*18*i*dt);
        signal2[i] = 1*(double)rand()/RAND_MAX + 0.4*sin(2*PI*testFreq*i*dt) + 0.3*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*3*i*dt) + 0.4*sin(2*PI*7*i*dt) + 0.6*sin(2*PI*19*i*dt);
        signal3[i] = 1*(double)rand()/RAND_MAX + 0.3*sin(2*PI*testFreq*i*dt) + 0.6*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*4*i*dt) + 0.5*sin(2*PI*8*i*dt) + 0.5*sin(2*PI*21*i*dt);
        signal4[i] = 1*(double)rand()/RAND_MAX + 0.6*sin(2*PI*testFreq*i*dt) + 0.5*sin(2*PI*2*testFreq*i*dt) + 0.3*sin(2*PI*5*i*dt) + 0.4*sin(2*PI*12*i*dt) + 0.4*sin(2*PI*17*i*dt);
    }
    if(testCounter == 1)
        signal = signal1;
    else if(testCounter ==2 )
        signal = signal2;
    else if(testCounter == 3)
        signal = signal3;
    else if(testCounter == 4) {
        signal = signal4;
        testCounter = 0;
    }
}

void Signal::updateSignal(double *buffer, int bufferSize)
{
    rotate(signal, signal+bufferSize, signal+sampleSize);           // Shift all elements up 
    copy(buffer, buffer+bufferSize, signal+sampleSize-bufferSize);  // Replace the end elements with buffer
}

// Function that averages the signal and applies the window function
void Signal::processSignal()
{
    for(int i = 0; i != sampleSize; i++)
        averagedSignal[i] = signal[i];
    
    highPassFilter(averagedSignal, sampleSize);      // Remove the DC offset using HP. Result is stored in averagedSignal
    applyWindowFunction();                           // Apply window to filtered signal

    //printAveragedSignalCSV();
}

void Signal::applyWindowFunction()
{
    const Aquila::Window::WindowDataType& windowData = windowFunction.getData(); // Data is returned in a vector with elements [0,1]
    for(int i = 0; i != sampleSize; i++)
    {
        averagedSignal[i] = averagedSignal[i]*windowData[i];        // Multiply element by element
    }
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
void Signal::highPassFilter(double *x, int size)
{
    counter++;
    if(counter > 3)     // Wait till the fourth sample
        ready = true;
    if(ready)
    {
        double windowAverage = getWindowAverage(x, size);
        //cout << "Widow average is " << windowAverage << endl;
        for (int i = 0; i != size; ++i)
            x[i] -= windowAverage;
    }
}

// DO NOT DELETE
// Apply a high pass filter to the data
/*void Signal::highPassFilter(double *x, const int size)
{
    // Calculate the FFT
    Aquila::OouraFft fft(size);
    Aquila::ComplexType* spectrum = new Aquila::ComplexType[size];
    fft.fft(x, spectrum);

    // Generate a HPF spectrum
    Aquila::ComplexType* filterSpectrum = new Aquila::ComplexType[size];
    const Aquila::FrequencyType f_hp = 6;       // Set cutoff at 6Hz (literature standard)
    for (std::size_t i = 0; i != size; ++i)
    {
        if (i < (size * f_hp / samplingFreq))
            filterSpectrum[i] = 0.0;
        else
            filterSpectrum[i] = 1.0;
    }
    // the following line does the multiplication of two spectra
    std::transform(spectrum, spectrum + size, filterSpectrum, spectrum, std::multiplies<Aquila::ComplexType>());
    // Inverse FFT
    fft.ifft(spectrum, x);

    delete spectrum; delete filterSpectrum; // Free up memory
    spectrum = NULL; filterSpectrum = NULL;
}*/


// Note that this is the average of UNPADDED signal. Important to call BEFORE padding
double Signal::getWindowAverage(double* x, int size)
{
    double sum = 0;   
    for (int i = 0; i != size; ++i)
        sum+=x[i];

    return sum/size;
}

// TO TEST OUTPUT------------------------------------------------------------------
void Signal::printAveragedSignalCSV()
{
    double dt = 1/samplingFreq;
    ofstream myfile;
    myfile.open("test2.csv");  
    myfile << "Time (s),Magnitude" << endl;
    for (int i=0; i!=sampleSize; i++)
    {
        myfile << i*dt << "," << averagedSignal[i] << endl;
    }
    myfile << endl;
    myfile.close();
}

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
