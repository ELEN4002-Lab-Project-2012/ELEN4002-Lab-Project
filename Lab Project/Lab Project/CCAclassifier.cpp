#include "CCAclassifier.h"
// Size is the seize of the window!
CCAclassifier::CCAclassifier(int size, double sampleFreq, int numChannels):
    sampleSize(size),
    samplingFreq(sampleFreq),
    freqRes(sampleFreq/size),
    nChannels(numChannels),
	X(4,sampleSize)
{
    // Initialise the channels
    for(int i = 0; i != nChannels; i++)
    {
        boost::shared_ptr<Signal> signal_ptr(new Signal(sampleSize, 2*sampleSize, samplingFreq));
        channels.push_back(signal_ptr);
    }
	// Generate matrix of ideal signals ONCE!
	double Fseek = 11;
	double f1 = Fseek, f2 = 2*Fseek, f3 = 3*Fseek;        // Fundamental, second harmonic and Third harmonic
    double dt = 1/samplingFreq;

    mat t(1,sampleSize);                       // Time scale
    for (int i = 0; i != sampleSize; ++i)
        t(0,i) = dt*i;
	/*
	% Pure harmonics:
    y1 = cos(2*pi*Fc_Seek*t);     % First harmonic
    y2 = sin(2*pi*Fc_Seek*t);     % First harmonic
    y3 = cos(4*pi*Fc_Seek*t);     % First harmonic
    y4 = sin(4*pi*Fc_Seek*t);     % First harmonic
    y5 = cos(6*pi*Fc_Seek*t);     % First harmonic
    y6 = sin(6*pi*Fc_Seek*t);     % First harmonic

    Y = [y1; y2; y3; y4;y5;y6 ]; 
	*/
	mat y1 = cos(2*PI*f1*t );
	mat y2 = sin(2*PI*f1*t );
	mat y3 = cos(2*PI*f2*t );
	mat y4 = sin(2*PI*f2*t );
	mat y5 = cos(2*PI*f3*t );
	mat y6 = sin(2*PI*f3*t );

	Y = join_cols(y1,y2);
	Y = join_cols(Y,y3);
	Y = join_cols(Y,y4);
	Y = join_cols(Y,y5);
	Y = join_cols(Y,y6);

  
	

    cout << "CCA classifier instantiated" << endl;
}

double CCAclassifier::calculateRatio(double desiredFreq)
{
		X.save("Xmat.csv",csv_ascii);
		Y.save("Ymat.csv",csv_ascii);
		//X.print("X:");
	mat Z = join_cols(X,Y);
		//Z.save("Zmat.csv",csv_ascii);
	mat Ctemp = strans(Z) ; //Z.' - Non conjugated transpose = strans
	mat C = cov(Ctemp, 0); // Normalisation with N-1
		//C.save("Cmat.csv",csv_ascii);
		//C.print("C:");
	
	int Sx = X.n_rows -1;
	int Sy = Y.n_rows-1;

	mat Cxx = C.submat(0,0,Sx,Sx);// + ((10^(-8)) * eye(Sx+1,Sx+1)));
		Cxx.print("Cxx:");
	mat Cxy = C.submat(0,Sx+1,Sx,Sx+1+Sy);
		//Cxy.print("Cxy:");
	mat Cyx = Cxy.t() ;
		//Cyx.print("Cyx:");
	mat Cyy = C.submat(Sx+1,Sx+1,Sx + Sy+1, Sx+ Sy+1) ;//  +((10^(-8))  * eye(Sy+1,Sy+1)));
		//Cyy.print("Cyy:");

	try
	{	mat Rx; 
		chol(Rx,Cxx); //try Ensure +ve definite or try another methoc
		if (Rx.n_cols != 0)
		{
			mat invRx = inv(Rx);

	
			Z = invRx.t() * Cxy * solve( Cyy , Cyx) * invRx;
	
			cx_vec eigval;
			cx_mat Rmat;

			eig_gen(eigval, Rmat, Z);
			mat R = sqrt((real(Rmat)));
			double Ratio = R.max();
			return Ratio;
		}
		else
		{
		return 0 ;
		}
	}
	catch(int e)
	{
		return 0;
	}

	
}

void CCAclassifier::updateEEGData(double* dataO1, double* dataO2, double* dataP7, double* dataP8, int nSamplesTaken)
{
    // Sorry, hard coded!
    channels.at(0)->updateSignal(dataO1, nSamplesTaken);
    channels.at(1)->updateSignal(dataO2, nSamplesTaken);
    channels.at(2)->updateSignal(dataP7, nSamplesTaken);
    channels.at(3)->updateSignal(dataP8, nSamplesTaken);

	


    for(int i = 0; i != nChannels; i++) 
	{
        channels.at(i)->processSignal();                        // Average the signal for each channel (remove DC offset)
    }
	

	FFT testFFT(sampleSize, samplingFreq, 1, Aquila::WIN_HAMMING);
	testFFT.calcFFT(channels.at(0)->getEEGSignal(), sampleSize); 
	testFFT.printSpectrumCSV("UnFilteredFFT.csv");
	testFFT.zeroSpectrum();
	testFFT.calcFFT(channels.at(0)->getAveEEGSignal(), sampleSize); 
	testFFT.printSpectrumCSV("FilteredFFT.csv");
   testFFT.zeroSpectrum();
	
    for(int i = 0;i !=nChannels ; i++) 
	{       
        for(int j = 0; j != sampleSize; j++) 
		{
            X(i, j) = channels.at(i)->getAveEEGSignal()[j];
        }
    }
	cout << "Loop Done";

	
}

void CCAclassifier::loadTestData()
{
	
    // Initialise signal data
    for(int i = 0; i != nChannels; i++) {
        channels.at(i)->loadTestData();
    }
	
	for(int i = 0; i != nChannels; i++) 
	{
        channels.at(i)->processSignal();                        // Average the signal for each channel (remove DC offset)
    }

	FFT testFFT(sampleSize, samplingFreq, 1, Aquila::WIN_HAMMING);
	testFFT.calcFFT(channels.at(0)->getEEGSignal(), sampleSize); 
	testFFT.printSpectrumCSV("UnFilteredFFT.csv");
	testFFT.zeroSpectrum();
	testFFT.calcFFT(channels.at(0)->getAveEEGSignal(), sampleSize); 
	testFFT.printSpectrumCSV("FilteredFFT.csv");
	testFFT.zeroSpectrum();
    // Initialise X matrix
    for(int i = 0;i !=nChannels ; i++) 
	{       
        for(int j = 0; j != sampleSize; j++) 
		{
            X(i, j) = channels.at(i)->getAveEEGSignal()[j];
        }
    }
	
}


/*
 0.034894037789514123                
-0.081251899445473369                
 0.0165727477707273                  
-0.017053963566709823                
-0.028689286088943639                
-0.015434782845975548                
-0.037351046936487373                
-0.062753937123087314                
-0.031368764427579021                
 0.0050945562385500409               
-0.046179449020005217                
-0.13570230434431391                 
-0.094456054351959473                
 0.12873825387069171                 
 0.35198856384392174                 
 0.35198856384392174                 
 0.12873825387069171                 
-0.094456054351959473                
-0.13570230434431391                 
-0.046179449020005217                
 0.0050945562385500409               
-0.031368764427579021                
-0.062753937123087314                
-0.037351046936487373                
-0.015434782845975548                
-0.028689286088943639                
-0.017053963566709823                
 0.0165727477707273                  
-0.081251899445473369                
 0.034894037789514123

*/