#ifdef STH_EXPORT_DLL
	#define STH_API extern "C" __declspec(dllexport)
#else
	#define STH_API extern "C" __declspec(dllimport)
	
	#ifdef _DEBUG
		#pragma comment(lib,"sthd.lib")
	#else
		#pragma comment(lib,"sth.lib")
	#endif // _DEBUG
#endif
#ifndef STH_API_FUNC
#define  STH_API_FUNC

	//************************************
	// Method:    FFT ����ʵ���鲿
	// FullName:  FFT
	// Access:    public static 
	// Returns:   int ����ֵ������_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 ����ʵ��һά���� 
	// Parameter: double * apOutPutReal  ���ʵ������
	// Parameter: double * apOutPutImg   ����鲿���� 
	// Parameter: int anInputLength		 ���볤��
	// Parameter: int & anOutputLength   ������ȣ�����
	//************************************
	STH_API int FFT( double * apInput,
					double * apOutPutReal,
					double * apOutPutImg,
					int   anInputLength,
					int& anOutputLength); 
	
	//************************************
	// Method:    FFT2 ���ط�ֵ��λ��
	// FullName:  FFT2
	// Access:    public static 
	// Returns:   int ����ֵ������_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 ����ʵ��һά���� 
	// Parameter: double * apOutPutAmp   �����ֵ����
	// Parameter: double * apOutPutPhase �����λ���� 
	// Parameter: int anInputLength		 ���볤��
	// Parameter: int & anOutputLength   ������ȣ�����
	//************************************
	STH_API int FFT2( double * apInput,
					double * apOutPutAmp,
					double * apOutPutPhase,					
					int   anInputLength,
					int& anOutputLength,
					BOOL abDividLength = 1,
					double adblRatio=2,
					double adblPhaseDiff=90
					); 

	//************************************
	// Method:    FFT5 ���ط�ֵ��
	// FullName:  FFT5
	// Access:    public static 
	// Returns:   int ����ֵ������_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 ����ʵ��һά���� 
	// Parameter: double * apOutPutAmp   �����ֵ����
	// Parameter: int anInputLength		 ���볤��
	// Parameter: int & anOutputLength   ������ȣ�����
	//************************************
	STH_API int FFT5( double * apInput,
					double * apOutPutAmp,
					int   anInputLength,
					int& anOutputLength,
					BOOL abDividLength = 1,
					double adblRatio=2
					); 


	//************************************
	// Method:    APFFT ���ؽ�����
	// FullName: APFFT 
	// Access:    public static 
	// Returns:   int ����ֵ������_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 ����ʵ��һά���� 
	// Parameter: double * apOutPutAmp   �����ֵ����
	// Parameter: int anInputLength		 ���볤��
	// Parameter: int & anOutputLength   ������ȣ�����
	//************************************

	STH_API int APFFT(	double *apInput,
						double * apInputFreqSequenceToAdjust,
						double * apOutPutAmp,
						double * apOutPutPhase,
						double * apOutPutFrequence,
						double adblSampleFreq,
						int anInputLength,
						int anInputFreqSequenceLength,
						int& anOutputLength,
						double adblRatio=2,
						double adblPhaseDiff=90);

	STH_API int WindowedWave(float *apInput, int anInputLength,int type);
	
	STH_API  double Round(double r);

	STH_API  int Sign(double arData);

	STH_API  int IsZero(double adblData);
	
	STH_API int Conv(double * apInputA, 
					double * apInputB, 
					double * apOutBuffer,
					int anInputALength, 
					int anInputBLength,
					int & anOutputBufferLength,
					double & adblSum,
					double adblRatio=1);



	STH_API  double MatlabMod(double adblX,double adblY);

	STH_API  int HASH(unsigned char *apInput, int anLength,unsigned int & anCRC);
	

	
	enum _EER_CODE_
	{
		ERR_FATAL_EXCEPTION=-1,
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
	enum E_WINDOW_TYPE
	{
		E_WINDOW_HANNING=0,
		E_WINDOW_HAMMING=1,
		E_WINDOW_BLACKMAN=2,
		E_WINDOW_NO =3		
	};
#endif