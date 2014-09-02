#include "stdafx.h"
#include "fftwx.h"
#include <math.h>
#include <vector>
#pragma comment(lib,"libfftw3-3.lib")
#define _FFT_PI 3.1415926535898
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
					   int& anOutputLength )
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
		double ldblMag = sqrt((ldblReal * ldblReal) + (ldblImage * ldblImage))*4/anInputLength;
		double ldblPhase = atan(ldblImage/ldblReal)/_FFT_PI*180.0;
		ldblPhase = (int)(ldblPhase*100)%36000;
		ldblPhase = ldblPhase/100;		
		if (ldblPhase<0)
		{
			ldblPhase += 360.00;
		}
		apOutPutAmp[i] = ldblMag;
		apOutPutPhase[i]= ldblPhase;
	}
	
	//处理零点
	apOutPutAmp[0] = apOutPutAmp[0]/anInputLength;
	apOutPutPhase[0] = apOutPutPhase[0]/anInputLength;

	fftw_destroy_plan(p);

	fftw_free(lpOut); 

	return ERR_NO_ERROR;
}

int CFFT_Wrapper::APFFT( double *apInput, 
						 double * apInputFreqSequenceToAdjust, 
						 double * apOutPutAmp, 
						 double * apOutPutPhase, 
						 double * apOutPutFrequence, 
						 int anInputLength, 
						 int anFreqSequenceLength, 
						 int& anOutputLength )
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

	if (anFreqSequenceLength<1)
	{
		return ERR_INVALID_INPUT_FREQUENCE_LENGTH;
	}

	if (anOutputLength<(anFreqSequenceLength))
	{
		anOutputLength= (anFreqSequenceLength);
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	//2.构造hanning窗
}

