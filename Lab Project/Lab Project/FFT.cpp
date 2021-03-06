#include "FFT.h"

FFT::FFT(int complexSize, double freq, bool pad, Aquila::WindowType type):
    padded(pad), 
    plt("Spectrum"),
    windowType(type)        // Initialise the window type you want to apply before doing an FFT
{
    initFFT(complexSize, freq);
    // Zero the complex and absolute spectra
    zeroSpectrum();
    for(int i = 0; i != absFFTsize; i++)
    {
        absSpectrum[i] = 0;
        aveAbsSpectrum[i] = 0;
    }
}

void FFT::initFFT(int complexSize, double freq)
{
    if(padded) // Use zero padding
    {
        int padSize = complexSize*2;
        samplingFreq = freq; 
        freqRes = freq/padSize;
        fft_ptr = boost::shared_ptr<Aquila::OouraFft>(new Aquila::OouraFft(padSize));
        complexSpectrum = new Aquila::ComplexType[padSize];
        complexFFTsize = padSize;
        absFFTsize = padSize/2;                                         // Positive half
        absSpectrum = boost::shared_array<double>(new double[padSize/2]);        
        aveAbsSpectrum = boost::shared_array<double>(new double[padSize/2]);
        normalisedSpectrum = new double[absFFTsize];
    }
    else    // No zero padding
    {
        samplingFreq = freq; 
        freqRes = freq/complexSize;
        complexFFTsize = complexSize;
        absFFTsize = complexSize/2;
        absSpectrum = boost::shared_array<double>(new double[complexSize/2]);        
        aveAbsSpectrum = boost::shared_array<double>(new double[complexSize/2]);
        complexSpectrum = new Aquila::ComplexType[complexSize];
        fft_ptr = boost::shared_ptr<Aquila::OouraFft>(new Aquila::OouraFft(complexSize));
        normalisedSpectrum = new double[absFFTsize];
    }
}

void FFT::calcFFT(boost::shared_ptr<Signal> signal, int size)
{
    if(padded){
        double* windowedSignal = applyWindow(signal->getAveEEGSignal(), size);
        double* paddedSig = zeroPad(windowedSignal, size);
        fft_ptr->fft(paddedSig, complexSpectrum);
        delete windowedSignal;
        delete paddedSig;
        paddedSig = NULL;
        windowedSignal = NULL;
    }
    else {
        double* windowedSignal = applyWindow(signal->getAveEEGSignal(), size);
        fft_ptr->fft(windowedSignal, complexSpectrum);
    }
    
    initAbsoluteSpectrum();
}

void FFT::calcFFT(double* signal, int size)
{
    if(padded) {
        double* windowedSignal = applyWindow(signal, size);
        double* paddedSig = zeroPad(windowedSignal, size);
        fft_ptr->fft(paddedSig, complexSpectrum);
        delete windowedSignal;
        delete paddedSig;
        paddedSig = NULL;
        windowedSignal = NULL;
    }
    else {
        double* windowedSignal = applyWindow(signal, size);
        fft_ptr->fft(windowedSignal, complexSpectrum);
    }
    
    initAbsoluteSpectrum();
}

double FFT::integral(double f1, double f2)
{
    if(f1 > f2)
        cerr << "Lower frequency > higher frequency" << endl;
    if(f1 < 0)
        cerr << "Cannot find integral for negative frequencies" << endl;

    // First normalise the FFT:
    double max = getSpectrumMaxInRange(0, absFFTsize);
    for(int i = 0; i != absFFTsize; i++)
    {
        normalisedSpectrum[i] = absSpectrum[i]/max;
    }
    int n1 = 0, n2 = 0;                 // Index of f1 and f2. n is a temp variable 
    n1 = int(floor( f1 / freqRes ));    // Find the index corresponding to the freq. Note that f_i = n * delta_f
    n2 = int(floor( f2 / freqRes ));

    // Calculate the integral using a Riemann sum
    double sum = 0;
    for(int i = n1; i != n2; i++)
    {
        sum += normalisedSpectrum[i];
    }
    return sum*freqRes;
}

double* FFT::applyWindow(double* signal, int size)
{
    Aquila::Window windowFunction(windowType, size);
    const Aquila::Window::WindowDataType& windowData = windowFunction.getData(); // Data is returned in a vector with elements [0,1]
    double* windowedSignal = new double[size];  
    for(int i = 0; i != size; i++)
    {
        windowedSignal[i] = signal[i]*windowData[i];        // Multiply element by element
    }
    return windowedSignal;
}

double* FFT::zeroPad(double* signal, int sampleSize)
{
    // Remember that the padded size = complexFFTsize now
    double* paddedSignal = new double[complexFFTsize];     // Temp variable to store padded signal
        
    // Copy existing elements. I had to use pointer arrays because Emotiv does. Is there another way to append zeroes?
    for(int i = 0; i != sampleSize; i++)
        paddedSignal[i] = signal[i];

    // Add new zeros to the end
    for(int i = sampleSize; i != complexFFTsize; i++)
        paddedSignal[i] = 0;

    return paddedSignal;
}

void FFT::calcAbsAveSpectrum()
{
    FFTbuffer.push_back(absSpectrum);
    // When the buffer reaches 6, we want to erase the first FFT and then do the average.
    if(FFTbuffer.size() == 6)               
    {
        //cout << "Calculating average of FFT for buffer size " << FFTbuffer.size() << endl;
        FFTbuffer.erase(FFTbuffer.begin());
        
        for(int i = 0; i != absFFTsize; i++)            // Loop through all the elements of the FFT
        {
            double sum = 0;
            for(int j = 0; j != FFTbuffer.size(); j++)  // Loop through all FFT sets
            {
                sum += (FFTbuffer.at(j))[i];
            }
            aveAbsSpectrum[i] = sum/FFTbuffer.size();   // store the average
        }
    }
}

void FFT::zeroSpectrum()
{
    for(int i = 0; i != complexFFTsize; i++)
        complexSpectrum[i] = 0;
}

double FFT::getSpectrumMaxInRange(int nLower, int nUpper)
{
    int currMax = nLower;

    if(nLower > absFFTsize || nLower < 0 || nUpper > absFFTsize || nUpper < 0)
        cerr << "Requested getMax index out of range" << endl;
    if(nLower == nUpper)
        cerr << "Requested getMax range not large enough" << endl;

    for (int i = nLower+1; i <= nUpper; i++) {
        if (absSpectrum[i] > absSpectrum[currMax]) {
            currMax = i;
        }
        //cout << "absSpectrum[currMax] = " << absSpectrum[currMax] << "; absSpectrum[i] = " << absSpectrum[i] << "; currMax = " << currMax << endl;
    }
    return absSpectrum[currMax];
}

double FFT::getSpectrumMaxInRange(double f1, double f2)
{
    if(f1 > f2)
        cerr << "Lower frequency > higher frequency" << endl;

    int n1 = 0, n2 = 0, n = 0;          // Index of f1 and f2. n is a temp variable 
    n1 = int(floor( f1 / freqRes ));    // Find the index corresponding to the freq. Note that f_i = n * delta_f
    n2 = int(floor( f2 / freqRes ));

    //cout << "f1 = " << f1 << "; n1 = " << n1 << endl;
    //cout << "f2 = " << f2 << "; n2 = " << n2 << endl;

    return getSpectrumMaxInRange(n1, n2);
}

double FFT::getSpectrumAverage()
{
    double sum = 0;   
    
    for (int i = 0; i != absFFTsize; ++i)
        sum+=absSpectrum[i];

    return sum/absFFTsize;
}

void FFT::initAbsoluteSpectrum()
{
    for (std::size_t i = 0; i != absFFTsize; ++i)
        absSpectrum[i] = abs(complexSpectrum[i]);

	absSpectrum[0] = 0;
absSpectrum[1] = 0;
absSpectrum[2] = 0;

}

// ----------- Plotting functions ------------
void FFT::plotFFT()
{
    plt.setTitle("Spectrum");
    plt.plotSpectrum(complexSpectrum, complexFFTsize);
}

void FFT::displayAverageSpectrum()
{
    cout << "Averaged spectrum: " << endl;
    for(int i = 0; i != absFFTsize; i++)
        cout << i << ", " << aveAbsSpectrum[i] << endl;
}

void FFT::displaySpectrumValues()
{
    cout << endl;
    for(int i = 0; i != absFFTsize; i++)
        cout << i << ", " << absSpectrum[i] << endl;
}

void FFT::printSpectrumCSV()
{
    ofstream myfile;
    myfile.open("test.csv");
    myfile << "Frequency (Hz),Magnitude" << endl;
    for (int i=0; i!=absFFTsize; i++)
    {
        myfile << i*freqRes << "," << absSpectrum[i] << endl;
        //myfile << i*freqRes << "," << 1 << endl;
    }
    myfile << endl;
    myfile.close();
}

void FFT::printSpectrumCSV(string fileName )
{
    ofstream myfile;
    myfile.open(fileName);
   // myfile << "Frequency (Hz),Magnitude" << endl;
    for (int i=0; i!=absFFTsize; i++)
    {
        myfile << i*freqRes << "," << absSpectrum[i] << endl;
    }
    myfile << endl;
    myfile.close();
}