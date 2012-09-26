#include "FFT.h"

FFT::FFT(int complexSize, double freq, bool pad):
    padded(pad), 
    plt("Spectrum") 
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
    }
}

void FFT::calcFFT(boost::shared_ptr<Signal> signal, int size)
{
    if(padded){
        double* paddedSig = zeroPad(signal->getAveEEGSignal(), size);
        fft_ptr->fft(paddedSig, complexSpectrum);
        delete paddedSig;
        paddedSig = NULL;
    }
    else
        fft_ptr->fft(signal->getAveEEGSignal(), complexSpectrum);
    
    initAbsoluteSpectrum();
    //calcAbsAveSpectrum();                               // Calculate the average of 5 FFT spectra
}

void FFT::calcFFT(double* signal, int size)
{
    if(padded) {
        double* paddedSig = zeroPad(signal, size);
        fft_ptr->fft(paddedSig, complexSpectrum);
        delete paddedSig;
        paddedSig = NULL;
    }
    else {
        //cout << "Signal size = " << size << "; Complex spectrum size = " << complexFFTsize << endl;
        fft_ptr->fft(signal, complexSpectrum);
    }
    
    initAbsoluteSpectrum();
    //calcAbsAveSpectrum();
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
    myfile.open("test2.csv");
    myfile << "Frequency (Hz),Magnitude" << endl;
    for (int i=0; i!=absFFTsize; i++)
    {
        myfile << i*freqRes << "," << absSpectrum[i] << endl;
    }
    myfile << endl;
    myfile.close();
}