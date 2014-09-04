#include "stdafx.h"
#include "fftwx.h"
#include <math.h>
#include <vector>
#include <float.h>
#pragma comment(lib,"libfftw3-3.lib")
#define _FFT_PI 3.1415926535898

int CFFT_Wrapper::WindowedWave(float *apInput, int anInputLength,int type)
{
	//1.check parameter
	if (NULL == apInput)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (anInputLength<=0)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (type>WINDOW_NO)
	{
		return ERR_UNKOWN_WINDOW_TYPE;
	}

	float  factor = .0;
	int i = .0;

	switch(type)
	{ 
	case WINDOW_HANNING:
		for(i=0;i<anInputLength;i++)
		{ /*noted(marble):应该是factor=(float)(0.5)*(1-cos(6.2831853*i/(n-1))),但是为了和labview里面一致，就去掉0.5.*/
			factor=(float)(1-cos(6.2831853*i/(anInputLength-1)));
			apInput[i]=apInput[i]*factor;
		}
		break;
	case WINDOW_HAMMING:
		for(i=0;i<anInputLength;i++)
		{   
			factor=(float)(0.54-0.46*cos(6.2831853*i/(anInputLength-1)));
			apInput[i]=apInput[i]*factor;
		}
		break;
	case WINDOW_BLACKMAN:
		for(i=0;i<anInputLength;i++)
		{   
			factor=(float)(0.42-0.5*cos(2*_FFT_PI*i/(anInputLength-1))+0.08*cos(4*_FFT_PI*i/(anInputLength-1)));
			apInput[i]=apInput[i]*factor;
		}
		break;
	case WINDOW_NO:
	default:
		break;
	}

	return ERR_NO_ERROR;
}

int CFFT_Wrapper::FFT(  double * apInput, 
						double * apOutPutReal, 
						double * apOutPutImg, 
						int anInputLength, 
						int& anOutputLength )
{
	//1. parameter check
	if (NULL == apInput)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (NULL == apOutPutReal)
	{
		return ERR_NULL_OUTPUT_REAL_BUFFER;
	}

	if (NULL == apOutPutImg)
	{
		return ERR_NULL_OUTPUT_IMG_BUFFER;
	}

	if (anInputLength<=16)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (anOutputLength<(anInputLength/2))
	{
		anOutputLength= ceil(double(anInputLength)/2);
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	//2.do fftw3
	fftw_complex * lpOut = NULL;

	fftw_plan p = NULL;
	
	lpOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * anInputLength);

	p = fftw_plan_dft_r2c_1d(anInputLength,apInput,lpOut,FFTW_MEASURE);

	fftw_execute(p);

	fftw_destroy_plan(p);

	for (int i=0;i<anOutputLength;i++)
	{
		apOutPutReal[i] = lpOut[i][0];
		apOutPutImg[i]  = lpOut[i][1];
	}

	fftw_free(lpOut); 

	return ERR_NO_ERROR;

}

int CFFT_Wrapper::FFT2( double * apInput, 
					   double * apOutPutAmp, 
					   double * apOutPutPhase, 
					   int anInputLength, 
					   int& anOutputLength,
					   double adblRatio/*=2*/,
					   double adblPhaseDiff/*=90*/)
{
	//1. parameter check
	if (NULL == apInput)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (NULL == apOutPutAmp)
	{
		return ERR_NULL_OUTPUT_AMP_BUFFER;
	}

	if (NULL == apOutPutPhase)
	{
		return ERR_NULL_OUTPUT_PHASE_BUFFER;
	}

	if (anInputLength<=16)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (anOutputLength<(anInputLength/2))
	{
		anOutputLength= ceil(double(anInputLength)/2);
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	//2.do fftw3
	fftw_complex * lpOut = NULL;

	std::vector<double> loInput;

	loInput.resize(anInputLength);

	double * lpInput = &loInput.front();

	fftw_plan p = NULL;

	lpOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * anInputLength);

	p = fftw_plan_dft_r2c_1d(anInputLength,lpInput,lpOut,FFTW_MEASURE);

	memcpy(lpInput,apInput,sizeof(double)*anInputLength);

	fftw_execute_dft_r2c(p,apInput,lpOut);


	for (int i=0;i<anOutputLength;i++)
	{
		double ldblReal = lpOut[i][0];
		double ldblImage  = lpOut[i][1];
		double ldblMag = sqrt((ldblReal * ldblReal) + (ldblImage * ldblImage))*adblRatio*2/anInputLength;
		double ldblPhase = atan(ldblImage/ldblReal)/_FFT_PI*180.0;
		ldblPhase = (int)(ldblPhase*100)%36000;
		ldblPhase = ldblPhase/100;		
		if (ldblPhase<0)
		{
			ldblPhase += 360.00;
		}
		apOutPutAmp[i] = ldblMag;
		apOutPutPhase[i]= ldblPhase+adblPhaseDiff;
	}
	
	//处理零点
	apOutPutAmp[0] = apOutPutAmp[0]/anInputLength;
	apOutPutPhase[0] = apOutPutPhase[0]/anInputLength;

	fftw_destroy_plan(p);

	fftw_free(lpOut); 

	return ERR_NO_ERROR;
}

int CFFT_Wrapper::Conv(double * apInputA, 
					   double * apInputB, 
					   double * apOutBuffer,
					   int anInputALength, 
					   int anInputBLength,
					   int & anOutputBufferLength,
					   double & ldblSum,
					   double adblRatio/*=1*/)
{
	if (NULL == apInputA)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (NULL == apInputB)
	{
		return ERR_NULL_INPUT_B_BUFFER;
	}

	if (NULL == apOutBuffer )
	{
		return ERR_NULL_OUTPUT_REAL_BUFFER;
	}
	
	if (anInputALength<=0)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (anInputBLength<=0)
	{
		return ERR_INVALID_INPUT_B_LENGTH;
	}

	int lnConvLength = (anInputALength+anInputBLength-1);
	
	if (anOutputBufferLength<lnConvLength)
	{
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	int i, j =0;

	ldblSum = 0;
	
	ZeroMemory(apOutBuffer,anOutputBufferLength*sizeof(double));

	for (i = 0; i < lnConvLength; i++) 
	{
		for (j = max(0, i + 1 - anInputBLength); j <= min(i, anInputALength - 1); j++) 
		{
			apOutBuffer[i] += apInputA[j] * apInputB[i - j];
		}

		ldblSum += apOutBuffer[i];
	}

	anOutputBufferLength = lnConvLength;

	return ERR_NO_ERROR;
}

double CFFT_Wrapper::Round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
};

int CFFT_Wrapper::IsZero(double adblData)
{

	if(abs(adblData)<=FLT_EPSILON  )
	{
		return TRUE;
	}

	return FALSE;
}

int CFFT_Wrapper::Sign(double arData)
{
	if (arData>0)
	{
		return 1;
	}

	if (arData<0)
	{
		return -1;
	}

	return 0;
}

int CFFT_Wrapper::APFFT( double *apInput, 
						 double * apInputFreqSequenceToAdjust, 
						 double * apOutPutAmp, 
						 double * apOutPutPhase, 
						 double * apOutPutFrequence, 
						 double adblSampleFreq,
						 int anInputLength, 
						 int anInputFreqSequenceLength, 
						 int& anOutputLength,
						 double adblRatio/*=2*/,
						 double adblPhaseDiff/*=90*/)
{
	//1. parameter check
	if (NULL == apInput)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (NULL == apOutPutAmp)
	{
		return ERR_NULL_OUTPUT_AMP_BUFFER;
	}

	if (NULL == apOutPutPhase)
	{
		return ERR_NULL_OUTPUT_PHASE_BUFFER;
	}

	if (NULL == apOutPutFrequence)
	{
		return ERR_NULL_OUTPUT_FREQ_BUFFER;
	}

	if (NULL == apInputFreqSequenceToAdjust)
	{
		return ERR_NULL_INPUT_FREQ_SEQUENCE_BUFFER;
	}

	if (anInputLength<=16)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (anInputFreqSequenceLength<1)
	{
		return ERR_INVALID_INPUT_FREQUENCE_LENGTH;
	}

	if (anOutputLength<(anInputFreqSequenceLength))
	{
		anOutputLength= (anInputFreqSequenceLength);
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	if (adblSampleFreq<0)
	{
		return ERR_ERROR_SAMPLE_FREQUENCE;
	}


	//2.构造hanning窗
	/************************************************************************/
	/* win =  hanning(NFFT)';
	 * win1 = win/sum(win);%窗归1                                           */
	/************************************************************************/
	
	int lnBufferLength = floor(((double)anInputLength+1)/2.0);
	
	std::vector<double> loHanning(anInputLength);
	
	std::vector<double> loHanning2(anInputLength);

	std::vector<double> loHanning_normalize(anInputLength);

	std::vector<double> loHanning2_normalize(anInputLength);

	std::vector<double> loHanningCov(anInputLength);

	std::vector<double> loHanningCov_normalize(anInputLength);

	std::vector<double> loY2Out(lnBufferLength);

	std::vector<double> loFFTY1Out(anInputLength);

	std::vector<double> loFFTY2Out(anInputLength);

	std::vector<double> loPhaseY1out(anInputLength);

	std::vector<double> loPhaseY2out(anInputLength);

	std::vector<double> loDiff(anInputLength);

	std::vector<double> loAmpDiff(anInputLength);

	std::vector<double> loIndex(anInputFreqSequenceLength);

	std::vector<double> loFreqAdjust(anInputFreqSequenceLength);


	double ldblHanningSum = 0.0;

	double ldblHanningSum2 = 0.0;

	double ldblHanningCovSum = 0.0;

	double * lpY1 = &apInput[lnBufferLength-1];
	
	double * lpY2 =  &apInput[0];

	double * lpY2Out = &loY2Out.front();

	double ldbldebug = 0;
	
	//2.0 生成hanning窗函数

	int lnHanningHalfLength = lnBufferLength/2;

	//先假设，要确保到这里的是偶数

	for(int i=0;i<lnHanningHalfLength;i++)        
	{
		loHanning[i]=0.5*(1-cos(2.0*_FFT_PI*((double)i+1.0)/((double)lnBufferLength+1))); 

		ldblHanningSum = ldblHanningSum+ loHanning[i];

		loHanning[lnBufferLength-1-i] = loHanning[i];

		loHanning2[lnBufferLength+i] = loHanning[i];

		loHanning2[anInputLength-1-i]=loHanning[i];
		
		ldblHanningCovSum = ldblHanningCovSum+ loHanning2[anInputLength-1-i];

		if (i==lnHanningHalfLength-1)
		{
			double ldbTest = loHanning[i];

			ldbTest = loHanning[i];
		}
	}

	ldblHanningSum = ldblHanningSum*2;

	ldblHanningCovSum = ldblHanningCovSum*2;

	for (int i=0;i<lnBufferLength;i++)
	{
		loHanning_normalize[i] = loHanning[i]/ldblHanningSum;

		loHanning2_normalize[lnBufferLength-1-i] = loHanning2[lnBufferLength-1-i]/ldblHanningSum2;//归一化
	}

	//2.1 计算窗卷积
	/************************************************************************/
	/* winn =  conv(win,win);%apFFT须要卷积窗  卷积
	 * win2 = winn/sum(winn);%窗归1                                         */
	/************************************************************************/

	int lnCovLength = 2*lnBufferLength-1;

	if (loHanningCov.size()<lnCovLength)
	{
		loHanningCov.resize(lnCovLength,.0);
	}

	//2.1.1 卷积窗

	int lnRet = CFFT_Wrapper::Conv(&loHanning.front(),
								   &loHanning.front(),
								   &loHanningCov.front(),
								    lnBufferLength,
								    lnBufferLength,
								    lnCovLength,
									ldblHanningCovSum,
								   1);

	double * ldblLast = &loHanningCov[lnCovLength-1];
	
	if (lnRet!=CFFT_Wrapper::ERR_NO_ERROR)
	{
		ASSERT(FALSE);

		return lnRet;
	}



	for(int n=0;n< lnCovLength;n++)
	{
		loHanningCov_normalize[n] = loHanningCov[n]/ldblHanningCovSum;
	}

	ldbldebug = loHanningCov_normalize[lnCovLength-1];

	//2.2  y1= y1.*win1;
	for (int i=0;i<lnBufferLength;i++)
	{
		lpY1[i]= lpY1[i]*loHanning_normalize[i];
	}

	 ldbldebug = lpY1[lnBufferLength-1];

	//2.3 
	/************************************************************************/
	/* y2 = y(1:2*NFFT-1);%后N个输入数据
	  winn =  conv(win,win);%apFFT须要卷积窗  卷积
	  win2 = winn/sum(winn);%窗归1
	  y2= y2.*win2;                                                         */
	/************************************************************************/

	 
	for(int n=0;n< lnCovLength;n++)
	{
		lpY2[n] = lpY2[n]*loHanningCov_normalize[n];
	}

	double * lpdebug = &lpY2[lnCovLength-10];
	
	//2.4 y2=y2(NFFT:end)+[0 y2(1:NFFT-1)]
	for (int i = 0;i<lnBufferLength;i++)
	{
		if (i==0)
		{
			loY2Out[i]  =  lpY2[lnBufferLength+i] + 0;
		}
		else
		{
			loY2Out[i] =   lpY2[lnBufferLength+i] +  lpY2[i+1] ;
		}
		
	}

	//3.1 FFT
	double * lpOutY1FFT = &loFFTY1Out.front();
	
	double * lpOutPhaseY1 = &loPhaseY1out.front();

	int lnY1FFTLength = loFFTY1Out.size();

	double * lpOutY2FFT = &loFFTY2Out.front();

	double * lpOutPhaseY2 = &loPhaseY2out.front();

	int lnY2FFTLength = loFFTY2Out.size();
	
	int lnRetY1 = CFFT_Wrapper::FFT2(lpY1,lpOutY1FFT,lpOutPhaseY1,lnBufferLength,lnY1FFTLength,1,0);

	if (lnRetY1!=CFFT_Wrapper::ERR_NO_ERROR)
	{
		ASSERT(FALSE);

		return lnRetY1;
	}
	
	int lnRetY2 = CFFT_Wrapper::FFT2(lpY2Out,lpOutY2FFT,lpOutPhaseY2,lnBufferLength,lnY2FFTLength,1,0);

	if (lnRetY2!=CFFT_Wrapper::ERR_NO_ERROR)
	{
		ASSERT(FALSE);

		return lnRetY2;
	}

	//3.2 
	/************************************************************************/
	/* df=mod((p1-p2)/180/(1-1/NFFT),1);%频率偏离校正值                                                                     */
	/************************************************************************/
	for (int i=0;i<lnBufferLength;i++)
	{
		double ldblData = (lpOutPhaseY1[i]-lpOutPhaseY2[i])/180.0;
		
		int lnData =(int) (ldblData*100.0/(1.0-1.0/(double)lnBufferLength));

		lnData = lnData%100;

		loDiff[i] = (double)lnData/100.0;
	}

	//3.3
	/************************************************************************/
	/* aa=(a1.^2)./(a2+eps)*bei;  %振幅校正值                                                                     */
	/************************************************************************/
	for (int i=0;i<lnBufferLength;i++)
	{
		double ldblData = (lpOutY1FFT[i]*lpOutY1FFT[i])/(lpOutY2FFT[i]+FLT_EPSILON  )*adblRatio;

		loAmpDiff[i] = ldblData;
	}

	//3.4 矫正谱第一个值处理
	/************************************************************************/
	/* aa(1)=a1(1)*2/NFFT*bei;
	   p2(1)=0;
	   df(1)=0;                                                            */
	/************************************************************************/
	loAmpDiff[0] = lpOutY1FFT[0]/lnBufferLength*adblRatio;

	lpOutPhaseY2[0] =.0;

	loAmpDiff[0] = 0;

	//3.5 目标频率的频率偏离校正值
	/************************************************************************/
	/* r=round(Fr/fs*NFFT)+1;
	   r1=floor(Fr/fs*NFFT)+1;  
	   mod_f=(r1-1+df(r))*fs/NFFT;
	   dmf=mod_f(i)-Fr(i);
	   if abs(dmf)>0.5*fs/NFFT
	   mod_f(i)=mod_f(i)-sign(dmf)*fs/NFFT;
	   end
	   */
	/************************************************************************/
	double ldblDf = adblSampleFreq/(double)lnBufferLength;

	for (int i=0;i<anInputFreqSequenceLength;i++)
	{
		loIndex[i] = CFFT_Wrapper::Round(apInputFreqSequenceToAdjust[i]/ldblDf)+1;

		int lnIndexTempRound = loIndex[i];

		if (lnIndexTempRound>= lnBufferLength || lnIndexTempRound<0)
		{
			continue;
		}

		int lnIndexTempFloor = floor(apInputFreqSequenceToAdjust[i]/ldblDf)+1;

		loFreqAdjust[i] = (lnIndexTempFloor-1+ loDiff[lnIndexTempRound] )*adblSampleFreq/(double)lnBufferLength;

		double dmf=loFreqAdjust[i]-apInputFreqSequenceToAdjust[i];

		double ldblTest = (0.5*adblSampleFreq/(double)lnBufferLength);

		if (fabs(dmf)>ldblTest)
		{
			loFreqAdjust[i] = loFreqAdjust[i] - CFFT_Wrapper::Sign(dmf)*adblSampleFreq/(double)lnBufferLength;
		}
		/************************************************************************/
		/*  3.5.1 for i=1:length(Fr)  %特殊处理低频部分
			if Fr(i)<2*Deltaf
			mod_f(i)=Deltaf;mod_a(i)=aa(2);
			mod_p(i)=0;
			end
			end                                                                     */
		/************************************************************************/
		double ldblAmpAdjusted = .0;
		
		double ldblPhaseAdjusted = .0;
		
		double ldblFreqAdjusted = .0;

		if (loFreqAdjust[i]<2*ldblDf)
		{
			loFreqAdjust[i] = ldblDf;

			ldblAmpAdjusted = loAmpDiff[2];

			ldblPhaseAdjusted = 0;

			ldblFreqAdjusted = loFreqAdjust[i] ;
		}else

		{
		
			/************************************************************************/
			/* 	3.5.2%目标频率的振幅校正值;
			mod_a=aa(r);
			%目标频率的初相位校正值
			mod_p=p2(r);                                             %数据中点初相位
			mod_p=mod_p-mod(2*pi*mod_f*(NFFT-1)/fs*180/pi,360)+90;   %数据初始点初相位
			mod_p=mod(mod_p,360);                                                                     */
			/************************************************************************/
			ldblAmpAdjusted = loAmpDiff[lnIndexTempRound];

			ldblPhaseAdjusted = lpOutPhaseY2[lnIndexTempRound];

			double ldblTemp = 2*_FFT_PI*((double)lnBufferLength-1)*loDiff[lnIndexTempRound]\
				/adblSampleFreq*180.0/_FFT_PI;

			ldblTemp = ldblTemp*100.0;

			ldblTemp =(double)((int)ldblTemp%36000)/100.0 + 90.0;

			ldblPhaseAdjusted = ldblPhaseAdjusted - ldblTemp;
			
			ldblPhaseAdjusted = (double)((int)ldblPhaseAdjusted*100/36000)/100.0;								

			ldblFreqAdjusted = loFreqAdjust[i] ;
		}

		/************************************************************************/
		/* 	 3.5.3 赋值输出 
			 double * apOutPutAmp, 
			 double * apOutPutPhase, 
			 double * apOutPutFrequence,                                                                      */
		/************************************************************************/
		apOutPutAmp[i] = ldblAmpAdjusted;

		apOutPutFrequence[i] = ldblFreqAdjusted;

		apOutPutPhase[i] = ldblPhaseAdjusted;
	}

	anOutputLength = anInputFreqSequenceLength;

	return ERR_NO_ERROR;
}

