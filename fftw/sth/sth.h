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
	// Method:    FFT 返回实部虚部
	// FullName:  FFT
	// Access:    public static 
	// Returns:   int 返回值，参照_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 输入实数一维数组 
	// Parameter: double * apOutPutReal  输出实部数组
	// Parameter: double * apOutPutImg   输出虚部数组 
	// Parameter: int anInputLength		 输入长度
	// Parameter: int & anOutputLength   输出长度，引用
	//************************************
	STH_API int FFT( double * apInput,
					double * apOutPutReal,
					double * apOutPutImg,
					int   anInputLength,
					int& anOutputLength); 
	
	//************************************
	// Method:    FFT2 返回幅值相位谱
	// FullName:  FFT2
	// Access:    public static 
	// Returns:   int 返回值，参照_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 输入实数一维数组 
	// Parameter: double * apOutPutAmp   输出幅值数组
	// Parameter: double * apOutPutPhase 输出相位数组 
	// Parameter: int anInputLength		 输入长度
	// Parameter: int & anOutputLength   输出长度，引用
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
	// Method:    FFT5 返回幅值谱
	// FullName:  FFT5
	// Access:    public static 
	// Returns:   int 返回值，参照_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 输入实数一维数组 
	// Parameter: double * apOutPutAmp   输出幅值数组
	// Parameter: int anInputLength		 输入长度
	// Parameter: int & anOutputLength   输出长度，引用
	//************************************
	STH_API int FFT5( double * apInput,
					double * apOutPutAmp,
					int   anInputLength,
					int& anOutputLength,
					BOOL abDividLength = 1,
					double adblRatio=2
					); 


	//************************************
	// Method:    APFFT 返回矫正谱
	// FullName: APFFT 
	// Access:    public static 
	// Returns:   int 返回值，参照_EER_CODE_
	// Qualifier:
	// Parameter: double * apInput		 输入实数一维数组 
	// Parameter: double * apOutPutAmp   输出幅值数组
	// Parameter: int anInputLength		 输入长度
	// Parameter: int & anOutputLength   输出长度，引用
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