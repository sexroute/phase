#pragma once



class CFFT_Wrapper
{
public:
	//************************************
	// Method:    FFT ����ʵ���鲿
	// FullName:  CFFT_Wrapper::FFT
	// Access:    public static 
	// Returns:   int ����ֵ������_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 ����ʵ��һά���� 
	// Parameter: double * apOutPutReal  ���ʵ������
	// Parameter: double * apOutPutImg   ����鲿���� 
	// Parameter: int anInputLength		 ���볤��
	// Parameter: int & anOutputLength   ������ȣ�����
	//************************************
	static int __cdecl FFT( double * apInput,
					double * apOutPutReal,
					double * apOutPutImg,
					int   anInputLength,
					int& anOutputLength); 
	
	//************************************
	// Method:    FFT2 ���ط�ֵ��λ
	// FullName:  CFFT_Wrapper::FFT2
	// Access:    public static 
	// Returns:   int ����ֵ������_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 ����ʵ��һά���� 
	// Parameter: double * apOutPutAmp   �����ֵ����
	// Parameter: double * apOutPutPhase �����λ���� 
	// Parameter: int anInputLength		 ���볤��
	// Parameter: int & anOutputLength   ������ȣ�����
	//************************************
	static int __cdecl FFT2( double * apInput,
					double * apOutPutAmp,
					double * apOutPutPhase,					
					int   anInputLength,
					int& anOutputLength,
					BOOL abDividLength = 1,
					double adblRatio=2,
					double adblPhaseDiff=90
					); 

	//************************************
	// Method:    FFT3 ������FFT,�������ֹ��2����������
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
	static int __cdecl FFT3( double * apInput,
					 double * apOutPutAmp,
					 double * apOutPutPhase,
					 int   anInputLength,
					 int& anOutputLength); 

	static int __cdecl FFT4( double * apInput,
							double * apOutPutAmp,
							double * apOutPutPhase,					
							int   anInputLength,
							int& anOutputLength,
							BOOL abDividLength = 1,
							double adblRatio=2,
							double adblPhaseDiff=90
							); 


	static int __cdecl FFT5( double * apInput,
					double * apOutPutAmp,
					int   anInputLength,
					int& anOutputLength,
					BOOL abDividLength = 1,
					double adblRatio=2
					); 

	static int __cdecl APFFT(	double *apInput,
						double * apInputFreqSequenceToAdjust,
						double * apOutPutAmp,
						double * apOutPutPhase,
						double * apOutPutFrequence,
						double adblSampleFreq,
						int anInputLength,
						int anInputFreqSequenceLength,
						int& anOutputLength,
						double adblRatio=2,
						double adblPhaseDiff=90,
						int abStable = FALSE);

	static int __cdecl WindowedWave(float *apInput, int anInputLength,int type);
	
	static inline  double __cdecl Round(double r);

	static inline int __cdecl Sign(double arData);

	static inline int __cdecl IsZero(double adblData);
	
	static int  __cdecl Conv(double * apInputA, 
			double * apInputB, 
			double * apOutBuffer,
			int anInputALength, 
			int anInputBLength,
			int & anOutputBufferLength,
			double & ldblSum,
			double adblRatio=1);

	static int __cdecl PreparePlan();

	static inline double __cdecl MatlabMod(double adblX,double adblY);

	static inline int __cdecl CRC16(unsigned char *apInput, int anLength,unsigned short & asCRC);
	
	static inline int __cdecl HASH(unsigned char *apInput, int anLength,unsigned int & anCRC);
	
	static int __cdecl LoadHanningConv(double * apOutBuffer, 
								int anInputALength, 
								int & anOutputBufferLength, 
								double & ldblSum);

	static int __cdecl SaveHanningConv(double * apOutBuffer, 
						int anInputALength, 
						int & anOutputBufferLength, 
						double adblSum);

	static int __cdecl LoadPlan(int anInputLength);
	static unsigned int __cdecl GetCPUTYPE();
	static int __cdecl SavePlan(int anInputLength);
	static int __cdecl AppendAllPlan(char * apPlan,int anCharLength);
	static int __cdecl LoadAllPlan();
	
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
		ERR_UNKOWN_WINDOW_TYPE,
		ERR_ERROR_SAMPLE_FREQUENCE,
		ERR_NULL_INPUT_B_BUFFER ,
		ERR_INVALID_INPUT_B_LENGTH,
		ERR_NO_CACHE,
		ERR_CACHE_CORRUPT,
	};
	enum _WINDOW_TYPE
	{
		WINDOW_HANNING=0,
		WINDOW_HAMMING=1,
		WINDOW_BLACKMAN=2,
		WINDOW_NO =3		
	};
protected:
private:
};









