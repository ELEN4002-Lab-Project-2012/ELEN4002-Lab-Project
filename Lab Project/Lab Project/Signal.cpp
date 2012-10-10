
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
}

int Signal::counter = 0;                // counter to ensure that averaging only begins when signal buffer is full
bool Signal::ready = 0;                 // signal buffer is full
int Signal::testCounter = 0;

void Signal::loadTestData()             // Just an arbitrary example showing the FFT algorithm
{
    const double dt = 1.0/samplingFreq; 
    const double testFreq = 11, f1 = 2, f2 = 16, f3 = 20;
    srand((unsigned)time(0));           // Seed random number generator
    testCounter++;
    
    double* signal1 = new double[sampleSize];
    double* signal2 = new double[sampleSize];
    double* signal3 = new double[sampleSize];
    double* signal4 = new double[sampleSize];

    for (std::size_t i = 0; i != sampleSize; ++i)
    {
        signal1[i] = 1*(double)rand()/RAND_MAX + 0.6*sin(2*PI*testFreq*i*dt) + 0.4*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*2*i*dt) + 0.6*sin(2*PI*9*i*dt) + 0.5*sin(2*PI*16*i*dt);
        signal2[i] = 1*(double)rand()/RAND_MAX + 0.5*sin(2*PI*testFreq*i*dt) + 0.3*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*3*i*dt) + 0.4*sin(2*PI*7*i*dt) + 0.6*sin(2*PI*20*i*dt);
        signal3[i] = 1*(double)rand()/RAND_MAX + 0.4*sin(2*PI*testFreq*i*dt) + 0.6*sin(2*PI*2*testFreq*i*dt) + 0.4*sin(2*PI*4*i*dt) + 0.5*sin(2*PI*8*i*dt) + 0.5*sin(2*PI*22*i*dt);
        signal4[i] = 1*(double)rand()/RAND_MAX + 0.7*sin(2*PI*testFreq*i*dt) + 0.5*sin(2*PI*2*testFreq*i*dt) + 0.3*sin(2*PI*5*i*dt) + 0.4*sin(2*PI*13*i*dt) + 0.4*sin(2*PI*15*i*dt);
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

// Function that averages the signal
void Signal::processSignal()
{
    for(int i = 0; i != sampleSize; i++)
        averagedSignal[i] = signal[i];
    
    //highPassFilter(averagedSignal, sampleSize);      // Remove the DC offset using HP. Result is stored in averagedSignal
	firHighPassFilter(averagedSignal, sampleSize);
    //printAveragedSignalCSV();
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



void Signal::firHighPassFilter(double* InputData, int sampleSize)
{
	const int size_filter = 41;
	double filterCoeff[size_filter];
	double output[512];
	filterCoeff[0] = -0.0011397890081787398;               
	filterCoeff[1] =0.00958293759756681     ;            
	filterCoeff[2] =0.0065705368964966461    ;           
	filterCoeff[3] =0.0036782389883905993     ;          
	filterCoeff[4] =0.003736235806468854       ;         
	filterCoeff[5] =0.012079382486167441        ;        
	filterCoeff[6] =0.024895747794782461         ;       
	filterCoeff[7] =0.028412073714475506          ;      
	filterCoeff[8] =0.011709082075425785           ;     
	filterCoeff[9] =-0.018391761708099832           ;     
	filterCoeff[10] =-0.03861755408506172             ;    
	filterCoeff[11] =-0.031029330394827786             ;   
	filterCoeff[12] =-0.0059406919757752225              ; 
	filterCoeff[13] = 0.001000482063055628                ;
	filterCoeff[14] =-0.037470999832249066;                
	filterCoeff[15] =-0.1045386396821905   ;               
	filterCoeff[16] =-0.14038242184928887   ;              
	filterCoeff[17] =-0.088790767823845082   ;             
	filterCoeff[18] = 0.047488907751349181    ;            
	filterCoeff[19] = 0.19482492707118723      ;           
	filterCoeff[20] = 0.25830189285206206       ;          
	filterCoeff[21] = 0.19482492707118723        ;         
	filterCoeff[22] = 0.047488907751349181        ;        
	filterCoeff[23] =-0.088790767823845082         ;       
	filterCoeff[24] =-0.14038242184928887           ;      
	filterCoeff[25] =-0.1045386396821905             ;     
	filterCoeff[26] =-0.037470999832249066            ;    
	filterCoeff[27] =0.001000482063055628              ;  
	filterCoeff[28] =-0.0059406919757752225             ;  
	filterCoeff[29] =-0.031029330394827786               ; 
	filterCoeff[30] =-0.03861755408506172                 ;
	filterCoeff[31] =-0.018391761708099832                ;
	filterCoeff[32] = 0.011709082075425785                ;
	filterCoeff[33] = 0.028412073714475506                ;
	filterCoeff[34] = 0.024895747794782461                ;
	filterCoeff[35] = 0.012079382486167441                ;
	filterCoeff[36] = 0.003736235806468854                ;
	filterCoeff[37] = 0.0036782389883905993               ;
	filterCoeff[38] = 0.0065705368964966461               ;
	filterCoeff[39] = 0.00958293759756681                 ;
	filterCoeff[40] =-0.0011397890081787398;

	for(int j = 0;j < sampleSize;j++)
	{
		output[j] = 0;
		for(int i = size_filter-1;i >=0;i--)
		{  
			if(j>=i)
			{
				output[j] += filterCoeff[i]*InputData[j-i];;
			}
		}
	
	
	}
	for(int j = 0;j < sampleSize;j++)
	{
		InputData[j] = output[j];
	}


}


/*
filterCoeff[0] = 0.034894037789514123   ;             
	filterCoeff[1] = -0.081251899445473369;                
	filterCoeff[2] =  0.0165727477707273   ;               
	filterCoeff[3] = -0.017053963566709823  ;              
	filterCoeff[4] = -0.028689286088943639   ;             
	filterCoeff[5] = -0.015434782845975548    ;            
	filterCoeff[6] = -0.037351046936487373     ;           
	filterCoeff[7] = -0.062753937123087314      ;          
	filterCoeff[8] = -0.031368764427579021       ;         
	filterCoeff[9] =  0.0050945562385500409       ;        
	filterCoeff[10] = -0.046179449020005217         ;       
	filterCoeff[11] = -0.13570230434431391           ;      
	filterCoeff[12] = -0.094456054351959473           ;     
	filterCoeff[13] =  0.12873825387069171             ;    
	filterCoeff[14] =  0.35198856384392174              ;   
	filterCoeff[15] =  0.35198856384392174               ;  
	filterCoeff[16] =  0.12873825387069171                ; 
	filterCoeff[17] = -0.094456054351959473               ; 
	filterCoeff[18] = -0.13570230434431391                 ;
	filterCoeff[19] = -0.046179449020005217                ;
	filterCoeff[20] =  0.0050945562385500409               ;
	filterCoeff[21] = -0.031368764427579021     ;           
	filterCoeff[22] = -0.062753937123087314     ;           
	filterCoeff[23] = -0.037351046936487373     ;           
	filterCoeff[24] = -0.015434782845975548    ;            
	filterCoeff[25] = -0.028689286088943639     ;           
	filterCoeff[26] = -0.017053963566709823   ;             
	filterCoeff[27] =  0.0165727477707273    ;              
	filterCoeff[28] = -0.081251899445473369 ;               
	filterCoeff[29] =  0.034894037789514123;


*/

