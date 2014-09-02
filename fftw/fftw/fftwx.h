#pragma once
#include "fftw3.h"

class CFFT_Wrapper
{
public:
	//************************************
	// Method:    FFT 返回实部虚部
	// FullName:  CFFT_Wrapper::FFT
	// Access:    public static 
	// Returns:   int 返回值，参照_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 输入实数一维数组 
	// Parameter: double * apOutPutReal  输出实部数组
	// Parameter: double * apOutPutImg   输出虚部数组 
	// Parameter: int anInputLength		 输入长度
	// Parameter: int & anOutputLength   输出长度，引用
	//************************************
	static int FFT( double * apInput,
					double * apOutPutReal,
					double * apOutPutImg,
					int   anInputLength,
					int& anOutputLength); 
	
	//************************************
	// Method:    FFT2 返回幅值相位
	// FullName:  CFFT_Wrapper::FFT2
	// Access:    public static 
	// Returns:   int 返回值，参照_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 输入实数一维数组 
	// Parameter: double * apOutPutAmp   输出幅值数组
	// Parameter: double * apOutPutPhase 输出相位数组 
	// Parameter: int anInputLength		 输入长度
	// Parameter: int & anOutputLength   输出长度，引用
	//************************************
	static int FFT2( double * apInput,
					double * apOutPutAmp,
					double * apOutPutPhase,
					int   anInputLength,
					int& anOutputLength); 

	//************************************
	// Method:    FFT3 高性能FFT,采样点截止到2的整数次幂
	// FullName:  CFFT_Wrapper::FFT3
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: double * apInput
	// Parameter: double * apOutPutAmp
	// Parameter: double * apOutPutPhase
	// Parameter: int anInputLength
	// Parameter: int & anOutputLength
	//************************************
	static int FFT3( double * apInput,
					 double * apOutPutAmp,
					 double * apOutPutPhase,
					 int   anInputLength,
					 int& anOutputLength); 

	static int APFFT(	double *apInput,
						double * apInputFreqSequenceToAdjust,
						double * apOutPutAmp,
						double * apOutPutPhase,
						double * apOutPutFrequence,
						int anInputLength,
						int anFreqSequenceLength,
						int& anOutputLength);
	enum _EER_CODE_
	{
		ERR_NO_ERROR = 0,
		ERR_NULL_INPUT_BUFFER ,
		ERR_NULL_OUTPUT_IMG_BUFFER ,
		ERR_NULL_OUTPUT_REAL_BUFFER ,
		ERR_INVALID_INPUT_LENGTH,
		ERR_INVALID_INPUT_FREQUENCE_LENGTH,
		ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH,
		ERR_NULL_OUTPUT_AMP_BUFFER ,
		ERR_NULL_OUTPUT_PHASE_BUFFER ,
		ERR_NULL_OUTPUT_FREQ_BUFFER,
		ERR_NULL_INPUT_FREQ_SEQUENCE_BUFFER,
	};
protected:
private:
};




