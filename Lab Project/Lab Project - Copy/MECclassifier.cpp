#include "MECclassifier.h"

MECclassifier::MECclassifier(int size, double sampleFreq, int numChannels, bool padding, Aquila::WindowType window):
    sampleSize(size),
    samplingFreq(sampleFreq),
    freqRes(sampleFreq/size),
    counter(0),
    nChannels(numChannels),
    Y(size, numChannels),            // Nt x Ny
    swFFT(size, sampleFreq, padding),
    windowFunction(window)
{
    // Initialise the channels
    for(int i = 0; i != nChannels; i++)
    {
        boost::shared_ptr<Signal> signal_ptr(new Signal(sampleSize, 2*sampleSize, samplingFreq, window));
        channels.push_back(signal_ptr);
    }
    //cout << "MEC classifier instantiated" << endl;        // #################
}

bool MECclassifier::isSSVEPpresent(double desiredFreq)
{
    const int minCount = 3, maxCount = 5;

    // If the SSVEP frequency is detected on any channel, increment the counter
    if(detectTargetFreq(desiredFreq) == true){
        if(counter < maxCount)      // The counter can reach a maximum of maxCount. 
            counter++;
    }
    else {
        if(counter > 0)
            counter--;              // Counter cannot go below 0      
    }

    //cout << "MEC counter after frequency detection = " << counter << endl;
    
    // counter value of "minCount" indicates a successful detection.
    // "Hysteresis" of "minCount - minCount + 1". That is, if the count reaches 5, it requires 
    // 3 non-successful detections to return an overall negative SSVEP detection. 
    if(counter >= minCount && counter <= maxCount)      
        return true;
    else
        return false;
}

void MECclassifier::updateEEGData(double* dataO1, double* dataO2, double* dataP7, double* dataP8, int nSamplesTaken)
{
    // Sorry, hard coded!
    channels.at(0)->updateSignal(dataO1, nSamplesTaken);
    channels.at(1)->updateSignal(dataO2, nSamplesTaken);
    channels.at(2)->updateSignal(dataP7, nSamplesTaken);
    channels.at(3)->updateSignal(dataP8, nSamplesTaken);

    for(int i = 0; i != nChannels; i++) {
        channels.at(i)->processSignal();         // Average and window the signal for each channel
    }

    for(int i = 0; i != sampleSize; i++) {       // Fill the Y matrix with data.
        for(int j = 0; j != nChannels; j++) {
            Y(i, j) = channels.at(j)->getAveEEGSignal()[i];
        }
    }
    //cout << Y << endl;
}

void MECclassifier::loadTestData()
{
    // Initialise signal data
    for(int i = 0; i != nChannels; i++) {
        ;
        channels.at(i)->loadTestData();
        channels.at(i)->processSignal();
    }
    // Initialise Y matrix
    for(int i = 0; i != sampleSize; i++) {       // Fill the Y matrix with data.
        for(int j = 0; j != nChannels; j++) {
            Y(i, j) = channels.at(j)->getAveEEGSignal()[i];
        }
    }
    
    /*cout << endl << "SAMPLE SIZE = " << sampleSize << endl;
    srand((unsigned)time(0));                           // Seed random number generator
    double* signal1 = new double[sampleSize];
    double* signal2 = new double[sampleSize];
    double* signal3 = new double[sampleSize];
    double* signal4 = new double[sampleSize];
    double testFreq = 15;
    const double dt = 1.0/samplingFreq; 

    for (std::size_t i = 0; i != sampleSize; ++i)
    {  
        signal1[i] = 1*(double)rand()/RAND_MAX + 0.7*sin(2*PI*testFreq*i*dt) + 0.5*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*2*i*dt) + 0.6*sin(2*PI*11*i*dt) + 0.5*sin(2*PI*18*i*dt);
        signal2[i] = 1*(double)rand()/RAND_MAX + 0.7*sin(2*PI*testFreq*i*dt) + 0.5*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*3*i*dt) + 0.4*sin(2*PI*7*i*dt) + 0.6*sin(2*PI*19*i*dt);
        signal3[i] = 1*(double)rand()/RAND_MAX + 0.7*sin(2*PI*testFreq*i*dt) + 0.5*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*4*i*dt) + 0.5*sin(2*PI*8*i*dt) + 0.5*sin(2*PI*21*i*dt);
        signal4[i] = 1*(double)rand()/RAND_MAX + 0.7*sin(2*PI*testFreq*i*dt) + 0.5*sin(2*PI*2*testFreq*i*dt) + 0.3*sin(2*PI*5*i*dt) + 0.4*sin(2*PI*12*i*dt) + 0.4*sin(2*PI*17*i*dt);
    }
    mat Y(sampleSize, nChannels);
    for(int i = 0; i != sampleSize; i++) {       // Fill the Y matrix with data.
        Y(i, 0) = signal1[i];
        Y(i, 1) = signal2[i];
        Y(i, 2) = signal3[i];
        Y(i, 3) = signal4[i];
    }
    cout << "Y = " << endl << Y << endl;*/
}

bool MECclassifier::detectTargetFreq(double f)
{
    const double phaseShift = PI/8;     // 22.5deg

    // If there is one positive detection for any phase => +ve detection
    for(int i = 0; i != 4; i++)
    {
        mat Sw = findWeightedSignal(f, i*phaseShift);               // Find the new weighted signal
        //cout << "X phaseshift = " << i*phaseShift << endl;       
        double* SwArray = new double[sampleSize];                   // Transform it into a double* array
        for(int i = 0; i != sampleSize; i++)
            SwArray[i] = Sw(i, 0);

        swFFT.calcFFT(SwArray, sampleSize);

        // Equation 11 -------
        double upper = swFFT.getSpectrumMaxInRange(f-0.1*f, f+0.1*f);               // At f
        double lower = swFFT.getSpectrumMaxInRange(8.0, swFFT.getMaxFreqInFFT());   // Over the whole spectrum
        double R = upper/lower;
        cout << "R = " << R << ". ";             
        Rvalues.push_back(R);

        swFFT.zeroSpectrum();                       // Don't forget to zero everything (Aquilla's problem!)
        delete SwArray;                             // Clean up memory before returning
        swFFT.printSpectrumCSV();
        if (R > 0.95){                              // May have to change this if R is not precise
                           // NB printing to test output
            return true;
        }
        else
            return false;
    }  
}

/**
*   @param phaseShift Phase shift of the pure harmonics in X
*/
mat MECclassifier::findWeightedSignal(double fundamentalFreq, const double phaseShift)
{
    double f1 = fundamentalFreq, f2 = 2*fundamentalFreq;        // Fundamental and second harmonic
    double dt = 1/samplingFreq;

    mat t(sampleSize, 1);                       // Time scale
    for (int i = 0; i != sampleSize; ++i)
        t(i, 0) = dt*i;
    
    mat x1 = join_rows(sin(2*PI*f1*t + phaseShift), cos(2*PI*f1*t + phaseShift));
    mat x2 = join_rows(sin(2*PI*f2*t + phaseShift), cos(2*PI*f2*t + phaseShift));
    mat X  = join_rows(x1, x2);

    mat X_t    = trans(X);                     // Transpose of X
    mat Ytilde = Y - X*inv(X_t*X)*X_t*Y;       // Equation 6
    mat Ytilde_t = trans(Ytilde);
    //printCSV(Ytilde, sampleSize, dt); 

    cx_vec eigval_complex;                     // Possible to get complex numbers
    cx_mat eigvec;

    eig_gen(eigval_complex, eigvec, Ytilde_t*Ytilde);       // Equation 7
    mat eigval = abs(eigval_complex);

    uvec indices = sort_index(eigval);                      // Indices of old matrix in new sorted order. 
    eigval = sort(eigval);                                  // Sort eigvalues in ascending order

    cx_mat temp = eigvec;
    for(int col = 0; col != eigvec.n_cols; col++) {         // Arrange eigvectors in corresponding order
        for (int row = 0; row != eigvec.n_rows; row++) {
            eigvec(row, col) = temp(row, indices(col));
        }
    }

    mat W = abs(eigvec)*diagmat(1/sqrt(eigval));            // Equation 8
    //cout << eigval << endl;
    double eigvalSum_Ny = sum(eigval.col(0)), eigvalSum_Ns = 0;
    //cout << "eigvalSum_Ny = " << eigvalSum_Ny << endl;
    int Ns = 0;
    for (int i = 0; i != nChannels; i++)                    // Equation 9
    {
        eigvalSum_Ns += eigval(i, 0);
        //cout << "Loop eigvalSum_Ns = " << eigvalSum_Ns << endl;
        if (eigvalSum_Ns > 0.1*eigvalSum_Ny) {
            Ns = i+1;
            //cout << "found Ns = " << Ns << endl;
            break;
        }
    }
    mat Sw(sampleSize, 1);              // New weight signal with minimum selected number of electrode channels
    Sw.zeros();
    for (int i = 0; i != Ns; i++) {     // Equation 10
        Sw = Y*W.col(i);
    }
    //printCSV(Sw, sampleSize, dt);     // NB: printing
    return Sw;
}

