#include "stdafx.h"
#include "fftwx.h"
#include <math.h>
#include <vector>
#include <float.h>
#include <direct.h>
#include "DebugHelper.h"
//#define _FFTW_STATIC
#ifdef _FFTW_STATIC
	#ifdef DEBUG
		#pragma comment(lib,"lib/Static-Release/libfftw-3.3d.lib")
	#else
		#pragma comment(lib,"lib/Static-Release/libfftw-3.3.lib")
		#pragma comment(lib,"lib/Static-Release/libiomp5md.lib")
		#pragma comment(lib,"lib/Static-Release/libmmd.lib")
		#pragma comment(lib,"lib/Static-Release/libirc.lib")
		#pragma comment(lib,"lib/Static-Release/svml_dispmd.lib")
		#pragma comment(lib,"lib/Static-Release/libdecimal.lib")
	#endif
#else
	#ifdef DEBUG
		#pragma comment(lib,"libfftw-3.3d.lib")
	#else
		#pragma comment(lib,"libfftw-3.3.lib")
	#endif
#endif // _FFTW_STATIC


#define SINGLE_PRECISION 1
#ifdef SINGLE_PRECISION

#endif

#define _FFT_PI 3.1415926535898
#define _MIN_POINT 16
unsigned int G_HARDWARE_ID = -1;
#include "../../fftw-3.3.2/api/fftw3.h"
inline double CFFT_Wrapper::MatlabMod(double adblX,double adblY)
{
	if (CFFT_Wrapper::IsZero(adblY))
	{
		return adblX;
	}

	if (CFFT_Wrapper::IsZero(adblX-adblY))
	{
		return adblX;		
	}

	double ldblMod = adblX - floor(adblX/adblY)*adblY;

	return ldblMod;
}

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

	if (anInputLength<=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (anOutputLength<(anInputLength/2))
	{
		anOutputLength= ceil(double(anInputLength)/2);
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	//2.do fftw3
	fftwf_complex * lpOut = NULL;

	fftwf_plan p = NULL;
	
	lpOut = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * anInputLength);

	std::vector<float> loInput(anInputLength);

	for (int i=0;i<anInputLength;i++)
	{
		loInput[i] = apInput[i];
	}

	float * lpInput = &loInput.front();

	p = fftwf_plan_dft_r2c_1d(anInputLength,lpInput,lpOut,FFTW_MEASURE);

	fftwf_execute(p);

	fftwf_destroy_plan(p);

	for (int i=0;i<anOutputLength;i++)
	{
		apOutPutReal[i] = lpOut[i][0];
		apOutPutImg[i]  = lpOut[i][1];
	}

	fftwf_free(lpOut); 

	return ERR_NO_ERROR;

}

int CFFT_Wrapper::FFT2( double * apInput, 
					   double * apOutPutAmp, 
					   double * apOutPutPhase, 
					   int anInputLength, 
					   int& anOutputLength,
					   BOOL abDividLength /*= 1*/,
					   double adblRatio/*=2*/,
					   double adblPhaseDiff/*=90*/
					   )
{
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

	if (anInputLength<=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}
	
	//保证是偶数
	if (anInputLength%2!=0)
	{
		anInputLength = anInputLength-1;
	}

	int lnDividLength = 1;

	int lnActuralOutputLength = anInputLength;

	if (abDividLength)
	{
		lnDividLength = anInputLength/2;

		lnActuralOutputLength = ceil(double(anInputLength)/2);
	}

	int lnFFTLoopLength = ceil(double(anInputLength)/2);

	if (anOutputLength<lnActuralOutputLength)
	{
		anOutputLength= lnActuralOutputLength;

		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}
	
	anOutputLength = lnActuralOutputLength;	

	//2.do fftw3
	fftwf_complex * lpOut = NULL;

	std::vector<float> loInput;

	loInput.resize(anInputLength);

	float * lpInput = &loInput.front();

	fftwf_plan p = NULL;

	lpOut = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * anInputLength);

	p = fftwf_plan_dft_r2c_1d(anInputLength,lpInput,lpOut,FFTW_ESTIMATE  );

	for (int i=0;i<anInputLength;i++)
	{
		lpInput[i] = apInput[i];
	}

	//memcpy(lpInput,apInput,sizeof(double)*anInputLength);

	//fftw_execute_dft_r2c(p,apInput,lpOut);

	fftwf_execute(p);

	for (int i=0;i<lnFFTLoopLength;i++)
	{
		double ldblReal = lpOut[i][0];

		double ldblImage  = lpOut[i][1];

		double ldblMag = sqrt((ldblReal * ldblReal) + (ldblImage * ldblImage))*adblRatio/lnDividLength;

		double ldblPhase = atan2(ldblImage,ldblReal)/_FFT_PI*180.0;

		ldblPhase = (int)(ldblPhase*10000)%3600000;

		ldblPhase = ldblPhase/10000;	

		if (ldblPhase<0)
		{
			ldblPhase += 360.00;
		}

		apOutPutAmp[i] = ldblMag;

		apOutPutPhase[i]= ldblPhase+adblPhaseDiff;
		
		if (i==0)
		{
			continue;
		}

		if (lnFFTLoopLength<lnActuralOutputLength)
		{
			apOutPutAmp[lnFFTLoopLength*2-i] = apOutPutAmp[i];

			apOutPutPhase[lnFFTLoopLength*2-i] = apOutPutPhase[i];
		}

	}
	
	//处理零点
	apOutPutAmp[0] = apOutPutAmp[0]/lnDividLength;

	apOutPutPhase[0] = apOutPutPhase[0]/lnDividLength;

	fftwf_destroy_plan(p);

	fftwf_free(lpOut); 

	return ERR_NO_ERROR;
}


int CFFT_Wrapper::FFT5( double * apInput, 
					   double * apOutPutAmp, 
					   int anInputLength, 
					   int& anOutputLength,
					   BOOL abDividLength /*= 1*/,
					   double adblRatio/*=2*/
					   )
{
	if (NULL == apInput)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (NULL == apOutPutAmp)
	{
		return ERR_NULL_OUTPUT_AMP_BUFFER;
	}


	if (anInputLength<=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	//保证是偶数
	if (anInputLength%2!=0)
	{
		anInputLength = anInputLength-1;
	}

	int lnDividLength = 1;

	int lnActuralOutputLength = anInputLength;

	if (abDividLength)
	{
		lnDividLength = anInputLength/2;

		lnActuralOutputLength = ceil(double(anInputLength)/2);
	}

	int lnFFTLoopLength = ceil(double(anInputLength)/2);

	if (anOutputLength<lnActuralOutputLength)
	{
		anOutputLength= lnActuralOutputLength;

		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	anOutputLength = lnActuralOutputLength;	

	//2.do fftw3
	fftwf_complex * lpOut = NULL;

	std::vector<float> loInput;

	loInput.resize(anInputLength);

	float * lpInput = &loInput.front();

	fftwf_plan p = NULL;

	lpOut = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * anInputLength);

	p = fftwf_plan_dft_r2c_1d(anInputLength,lpInput,lpOut,FFTW_ESTIMATE  );


	for (int i=0;i<anInputLength;i++)
	{
		lpInput[i] = apInput[i];
	}
	//memcpy(lpInput,apInput,sizeof(float)*anInputLength);


	//fftw_execute_dft_r2c(p,apInput,lpOut);

	fftwf_execute(p);

	for (int i=0;i<lnFFTLoopLength;i++)
	{
		double ldblReal = lpOut[i][0];

		double ldblImage  = lpOut[i][1];

		double ldblMag = sqrt((ldblReal * ldblReal) + (ldblImage * ldblImage))*adblRatio/lnDividLength;

		//double ldblPhase = atan2(ldblImage,ldblReal)/_FFT_PI*180.0;

		//ldblPhase = (int)(ldblPhase*10000)%3600000;

		//ldblPhase = ldblPhase/10000;	

		//if (ldblPhase<0)
		//{
		//	ldblPhase += 360.00;
		//}

		apOutPutAmp[i] = ldblMag;

		//apOutPutPhase[i]= ldblPhase+adblPhaseDiff;

		if (i==0)
		{
			continue;
		}

		if (lnFFTLoopLength<lnActuralOutputLength)
		{
			apOutPutAmp[lnFFTLoopLength*2-i] = apOutPutAmp[i];

		//	apOutPutPhase[lnFFTLoopLength*2-i] = apOutPutPhase[i];
		}

	}

	//处理零点
	apOutPutAmp[0] = apOutPutAmp[0]/lnDividLength;

	//apOutPutPhase[0] = apOutPutPhase[0]/lnDividLength;

	fftwf_destroy_plan(p);

	fftwf_free(lpOut); 

	return ERR_NO_ERROR;
}

int CFFT_Wrapper::FFT4( double * apInput, 
										double * apOutPutAmp, 
										double * apOutPutPhase, 
										int anInputLength, 
										int& anOutputLength,
										BOOL abDividLength /*= 1*/,
										double adblRatio/*=2*/,
										double adblPhaseDiff/*=90*/
										)
{
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

	if (anInputLength<=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	//保证是偶数
	if (anInputLength%2!=0)
	{
		anInputLength = anInputLength-1;
	}

	int lnDividLength = 1;

	int lnActuralOutputLength = anInputLength;

	if (abDividLength)
	{
		lnDividLength = anInputLength/2;

		lnActuralOutputLength = ceil(double(anInputLength)/2);
	}

	int lnFFTLoopLength = ceil(double(anInputLength)/2);

	if (anOutputLength<lnActuralOutputLength)
	{
		anOutputLength= lnActuralOutputLength;

		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	anOutputLength = lnActuralOutputLength;	

	//2.do fftw3
	fftwf_complex * lpOut = NULL;

	std::vector<float> loInput;

	loInput.resize(anInputLength);

	float * lpInput = &loInput.front();

	fftwf_plan p = NULL;

	lpOut = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * anInputLength);

	CFFT_Wrapper::LoadPlan(anInputLength);

	p = fftwf_plan_dft_r2c_1d(anInputLength,lpInput,lpOut,FFTW_ESTIMATE);

	for (int i=0;i<anInputLength;i++)
	{
		lpInput[i] = apInput[i];
	}

	//memcpy(lpInput,apInput,sizeof(double)*anInputLength);

	//fftw_execute_dft_r2c(p,apInput,lpOut);

	fftwf_execute(p);


	for (int i=0;i<lnFFTLoopLength;i++)
	{
		double ldblReal = lpOut[i][0];

		double ldblImage  = lpOut[i][1];

		double ldblMag = sqrt((ldblReal * ldblReal) + (ldblImage * ldblImage))*adblRatio/lnDividLength;

		double ldblPhase = atan2(ldblImage,ldblReal)/_FFT_PI*180.0;

		ldblPhase = (int)(ldblPhase*10000)%3600000;

		ldblPhase = ldblPhase/10000;	

		if (ldblPhase<0)
		{
			ldblPhase += 360.00;
		}

		apOutPutAmp[i] = ldblMag;

		apOutPutPhase[i]= ldblPhase+adblPhaseDiff;

		if (i==0)
		{
			continue;
		}

		if (lnFFTLoopLength<lnActuralOutputLength)
		{
			apOutPutAmp[lnFFTLoopLength*2-i] = apOutPutAmp[i];

			apOutPutPhase[lnFFTLoopLength*2-i] = apOutPutPhase[i];
		}

	}

	//处理零点
	apOutPutAmp[0] = apOutPutAmp[0]/lnDividLength;

	apOutPutPhase[0] = apOutPutPhase[0]/lnDividLength;

	CFFT_Wrapper::SavePlan(anInputLength);

	fftwf_destroy_plan(p);

	fftwf_free(lpOut); 

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
			apOutBuffer[i] += apInputA[j] * apInputB[i - j]*adblRatio;
		}

		ldblSum += apOutBuffer[i];
	}

	anOutputBufferLength = lnConvLength;

	return ERR_NO_ERROR;
}

inline double CFFT_Wrapper::Round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
};

inline int CFFT_Wrapper::IsZero(double adblData)
{

	if(abs(adblData)<=FLT_EPSILON  )
	{
		return TRUE;
	}

	return FALSE;
}

inline int CFFT_Wrapper::Sign(double arData)
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

static unsigned short g_ccitt_table[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
inline int CFFT_Wrapper::CRC16( unsigned char *apInput, int anLength,unsigned short & asCRC )
{
	if (NULL == apInput)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (anLength<=0)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	unsigned short lnCRC = 0;

	while (anLength-- > 0)
	{
		lnCRC = g_ccitt_table[(lnCRC >> 8 ^ *apInput++) & 0xff] ^ (lnCRC << 8);
	
	}
	asCRC = ~lnCRC;
	return ERR_NO_ERROR;
}


DWORD deax = 0;  
DWORD debx = 0;  
DWORD decx = 0;  
DWORD dedx = 0; 

void ExeCPUID(DWORD veax)  
{
	__asm
	{
		mov eax,veax
			cpuid
			mov deax,eax
			mov debx,ebx
			mov decx,ecx
			mov dedx,edx
	}
}

unsigned int CFFT_Wrapper::GetCPUTYPE()
{
	if (G_HARDWARE_ID==-1)
	{
		const DWORD id = 0x80000002; //从0x80000002开始,到0x80000004结束
		unsigned char CPUType[1024] = {0};//用来存储CPU型号信息

		for(DWORD t = 0 ; t < 2 ; t++ )
		{
			ExeCPUID(id+t);
			//每次循环结束,保存信息到数组
			memcpy(CPUType+16*t+ 0,&deax,4);
			memcpy(CPUType+16*t+ 4,&debx,4);
			memcpy(CPUType+16*t+ 8,&decx,4);
			memcpy(CPUType+16*t+12,&dedx,4);
		}
		unsigned int lnHashData = 0;
		
		CFFT_Wrapper::HASH(CPUType,49,lnHashData);

		InterlockedExchange((volatile LONG *)&G_HARDWARE_ID,(LONG)lnHashData);
	}

	return G_HARDWARE_ID;
}


int CFFT_Wrapper::LoadAllPlan()
{

	mkdir("./plan_cache");

	char lpFileName[100] = {0};

	unsigned int lnHardwareFeature = CFFT_Wrapper::GetCPUTYPE();

	sprintf(lpFileName,"./plan_cache/plan_%u_all.txt",lnHardwareFeature);

	FILE * lpFile = NULL;
	
	lpFile = fopen(lpFileName,"r+");

	if (NULL == lpFile)
	{
		return ERR_NO_CACHE;
	}
	int lnReadLength =10*1024*1024;

	std::vector<CHAR> loBuffer(lnReadLength,0);
	
	int lnReadCount = fread(&loBuffer.front(),sizeof(CHAR),lnReadLength,lpFile);

	//fftw_import_wisdom_from_file(lpFile); 
	int lnRet = fftwf_import_wisdom_from_string(&loBuffer.front());
	fclose(lpFile);

	return ERR_NO_ERROR;
}

int CFFT_Wrapper::LoadPlan(int anInputLength)
{
	if (anInputLength<=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	mkdir("./plan_cache");

	char lpFileName[100] = {0};

	unsigned int lnHardwareFeature = CFFT_Wrapper::GetCPUTYPE();

	sprintf(lpFileName,"./plan_cache/plan_%u_%d.txt",lnHardwareFeature,anInputLength);

	FILE * lpFile = fopen(lpFileName,"r");

	if (NULL == lpFile)
	{
		return ERR_NO_CACHE;
	}

	fftwf_import_wisdom_from_file(lpFile); 
	
	fclose(lpFile);

	return ERR_NO_ERROR;
}

int CFFT_Wrapper::AppendAllPlan(char *  apPlan,int anCharLength)
{
	if (anCharLength<=0)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (NULL == apPlan)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	mkdir("./plan_cache");

	char lpFileName[100] = {0};

	unsigned lnHardwareFeature = CFFT_Wrapper::GetCPUTYPE();

	sprintf(lpFileName,"./plan_cache/plan_%u_all.txt",lnHardwareFeature);

	FILE * lpFile = fopen(lpFileName,"wb+");

	if (NULL == lpFile)
	{
		return ERR_NO_CACHE;
	}	

	fprintf(lpFile, "%s", apPlan);

	fclose(lpFile);

	return ERR_NO_ERROR;
}


int CFFT_Wrapper::SavePlan(int anInputLength)
{
	if (anInputLength<=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	mkdir("./plan_cache");

	char lpFileName[100] = {0};

	unsigned lnHardwareFeature = CFFT_Wrapper::GetCPUTYPE();

	sprintf(lpFileName,"./plan_cache/plan_%u_%d.txt",lnHardwareFeature,anInputLength);

	FILE * lpFile = fopen(lpFileName,"wb+");

	if (NULL == lpFile)
	{
		return ERR_NO_CACHE;
	}	

	fftwf_export_wisdom_to_file(lpFile);

	fclose(lpFile);

	return ERR_NO_ERROR;
}

inline int CFFT_Wrapper::HASH( unsigned char *apInput, int anLength,unsigned int & anCRC )
{
	if (NULL == apInput)
	{
		return ERR_NULL_INPUT_BUFFER;
	}

	if (anLength<=0)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	UINT nHash = 0;
	while (anLength--)
	{
		nHash = (nHash << 5) + nHash + *apInput++;
	}

	anCRC =nHash;
	return ERR_NO_ERROR;

}

int CFFT_Wrapper::SaveHanningConv(double * apOutBuffer,
								  int anInputALength, 
								  int & anOutputBufferLength, 
								  double  adblSum)
{
	if (NULL==apOutBuffer)
	{
		return ERR_NULL_OUTPUT_REAL_BUFFER;
	}
	
	if (anInputALength <=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (anOutputBufferLength<(2*anInputALength-1))
	{
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	unsigned int lnHashComputed = 0;

	int lnRet = CFFT_Wrapper::HASH((unsigned char*)apOutBuffer,
									anOutputBufferLength,
									lnHashComputed);

	mkdir("./hanning_cache");

	CHAR lpFileName[100] = {0};

	sprintf(lpFileName,("./hanning_cache/hanning_conv_%d.txt"),anInputALength);

	FILE * lpFile = fopen(lpFileName,"wb+");

	if (lpFile)
	{
		fwrite(
			&lnHashComputed,
			1*sizeof(int)/sizeof(char),
			1,
			lpFile);

		fwrite(
			&adblSum,
			1*sizeof(double)/sizeof(char),
			1,
			lpFile);

		fwrite(
			apOutBuffer,
			anOutputBufferLength*sizeof(double)/sizeof(char),
			1,
			lpFile);

		fflush(lpFile);
		fclose(lpFile);
	}

	return ERR_NO_ERROR;

}

int CFFT_Wrapper::LoadHanningConv(double * apOutBuffer, 
								  int anInputALength, 
								  int & anOutputBufferLength, 
								  double & adblSum)
{
	if (NULL==apOutBuffer)
	{
		return ERR_NULL_OUTPUT_REAL_BUFFER;
	}

	if (anInputALength <=_MIN_POINT)
	{
		return ERR_INVALID_INPUT_LENGTH;
	}

	if (anOutputBufferLength<(2*anInputALength-1))
	{
		return ERR_NOT_ENOUGH_OUTPUT_BUFFER_LENGTH;
	}

	anOutputBufferLength= 2*anInputALength-1;	

	int lnBufferDataLength = anOutputBufferLength + 2;

	std::vector<double> loHanningCov2(lnBufferDataLength);
	/************************************************************************/
	/* int32      *1 hash
	*  double     *2 sum
	*  double ... *anOutputBufferLength
	/************************************************************************/

	int lnRealDataCharLength =  sizeof(int)+ sizeof(double) + anOutputBufferLength*sizeof(double);

	mkdir("./hanning_cache");

	CHAR lpFileName[100] = {0};

	sprintf(lpFileName,("./hanning_cache/hanning_conv_%d.txt"),anInputALength);

	FILE * lpFile = fopen(lpFileName,"rb");

	if (!lpFile)
	{
		return ERR_NO_CACHE;
		
	}else
	{
		fread(&loHanningCov2.front(),
			lnRealDataCharLength,
			1,
			lpFile);
		fclose(lpFile);
	}
	unsigned int * lpHash = ((unsigned int *)&loHanningCov2.front());

	unsigned int lnHash = *lpHash;

	lpHash = lpHash+1;

	double * lpSum = (double *)lpHash;

	adblSum = * lpSum;

	double * lpCov = (lpSum+1);

	unsigned int lnHashComputed = 0;

	int lnRet = CFFT_Wrapper::HASH((unsigned char*)lpCov,
											anOutputBufferLength,
											lnHashComputed);

	if (lnHashComputed!= lnHash)
	{
		anOutputBufferLength = 0;

		return ERR_CACHE_CORRUPT;
	}

	memcpy(apOutBuffer,lpCov,sizeof(double)*anOutputBufferLength);

	return ERR_NO_ERROR;
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
						 double adblPhaseDiff/*=90*/,
						 int abStable /*= FALSE*/)
{
	_DECLARE_PERF_MEASURE_TIME();

	_BEGIN_PERF_MEASURE_TIME();
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

	if (anInputLength%2!=0)
	{
		anInputLength = anInputLength-1;
	}

	if (anInputLength<=_MIN_POINT)
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

	//_END_PERF_MEASURE_TIME("0");

	//2.构造hanning窗
	/************************************************************************/
	/* win =  hanning(NFFT)';
	 * win1 = win/sum(win);%窗归1                                           */
	/************************************************************************/

	_BEGIN_PERF_MEASURE_TIME();
	
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

	std::vector<double> loInputCopy(anInputLength,0);

	memcpy(&loInputCopy.front(),apInput,sizeof(double)*anInputLength);
	
	double ldblHanningSum = 0.0;

	double ldblHanningSum2 = 0.0;

	double ldblHanningCovSum = 0.0;

	double * lpY1 = &apInput[lnBufferLength-1];
	
	double * lpY2 =  &loInputCopy[0];

	double * lpY2Out = &loY2Out.front();

	double ldbldebug = 0;

	//_END_PERF_MEASURE_TIME("1");
	_BEGIN_PERF_MEASURE_TIME();
	
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
	}

	//	_END_PERF_MEASURE_TIME("2.0");
		_BEGIN_PERF_MEASURE_TIME();

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

	int lnRet = CFFT_Wrapper::LoadHanningConv(&loHanningCov.front(),
											  lnBufferLength,
											  lnCovLength,
											  ldblHanningCovSum);
	if (lnRet!= ERR_NO_ERROR)
	{
		lnRet = CFFT_Wrapper::Conv(&loHanning.front(),
									&loHanning.front(),
									&loHanningCov.front(),
									lnBufferLength,
									lnBufferLength,
									lnCovLength,
									ldblHanningCovSum,
									1);

		CFFT_Wrapper::SaveHanningConv(&loHanningCov.front(),
										lnBufferLength,
										lnCovLength,
										ldblHanningCovSum);
	}




	//_END_PERF_MEASURE_TIME("2.1");
	_BEGIN_PERF_MEASURE_TIME();

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

	 double * lpdebug = lpY1 + lnBufferLength-1;

	 //_END_PERF_MEASURE_TIME("2.2");
	 _BEGIN_PERF_MEASURE_TIME();

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

	// lpdebug = &lpY2[lnCovLength-10];
	
	//2.4 y2=y2(NFFT:end)+[0 y2(1:NFFT-1)]
	for (int i = 0;i<lnBufferLength;i++)
	{
		if (i==0)
		{
			loY2Out[i]  =  lpY2[lnBufferLength+i-1] + 0;
		}
		else
		{
			loY2Out[i] =   lpY2[lnBufferLength+i-1] +  lpY2[i-1] ;
		}
		
	}

	//_END_PERF_MEASURE_TIME("2.3");
	_BEGIN_PERF_MEASURE_TIME();

	lpdebug = &loY2Out[lnBufferLength-10];

	//3.1 FFT
	double * lpOutY1FFT = &loFFTY1Out.front();
	
	double * lpOutPhaseY1 = &loPhaseY1out.front();

	int lnY1FFTLength = loFFTY1Out.size();

	double * lpOutY2FFT = &loFFTY2Out.front();

	double * lpOutPhaseY2 = &loPhaseY2out.front();

	int lnY2FFTLength = loFFTY2Out.size();
	
	int lnRetY1 = ERR_NO_ERROR;

	if (abStable)
	{
		lnRetY1= CFFT_Wrapper::FFT4(lpY1,lpOutY1FFT,lpOutPhaseY1,lnBufferLength,lnY1FFTLength,0,1,0);
	}else
	{
		lnRetY1= CFFT_Wrapper::FFT2(lpY1,lpOutY1FFT,lpOutPhaseY1,lnBufferLength,lnY1FFTLength,0,1,0);
	}	

	if (lnRetY1!=CFFT_Wrapper::ERR_NO_ERROR)
	{
		ASSERT(FALSE);

		return lnRetY1;
	}

	 ldbldebug = lpOutY1FFT[lnY1FFTLength-1];

	 lpdebug  = &lpOutY1FFT[lnY1FFTLength/2-1];
	
	int lnRetY2 = ERR_NO_ERROR;
	
	if (abStable)
	{
		lnRetY2 = CFFT_Wrapper::FFT4(lpY2Out,lpOutY2FFT,lpOutPhaseY2,lnBufferLength,lnY2FFTLength,0,1,0);
	}else
	{
		lnRetY2 = CFFT_Wrapper::FFT2(lpY2Out,lpOutY2FFT,lpOutPhaseY2,lnBufferLength,lnY2FFTLength,0,1,0);
	}
	

	if (lnRetY2!=CFFT_Wrapper::ERR_NO_ERROR)
	{
		ASSERT(FALSE);

		return lnRetY2;
	}

	ldbldebug = lpOutY2FFT[lnY2FFTLength-1];

	lpdebug  = &lpOutY2FFT[lnY2FFTLength/2-1];

	ldbldebug = lpOutPhaseY2[lnY2FFTLength-1];

	lpdebug  = &lpOutPhaseY2[lnY2FFTLength/2-1];

	//_END_PERF_MEASURE_TIME("3.1");
	_BEGIN_PERF_MEASURE_TIME();

	//3.2 
	/************************************************************************/
	/* df=mod((p1-p2)/180/(1-1/NFFT),1);%频率偏离校正值                                                                     */
	/************************************************************************/
	for (int i=0;i<lnBufferLength;i++)
	{
		double ldblData = (lpOutPhaseY1[i]-lpOutPhaseY2[i])/180.0;
		
		 ldblData = (ldblData/(1.0-1.0/(double)lnBufferLength));

		 ldblData = CFFT_Wrapper::MatlabMod(ldblData,1.0);

		loDiff[i] = ldblData;
	}

	//_END_PERF_MEASURE_TIME("3.2");
	_BEGIN_PERF_MEASURE_TIME();

	//3.3
	/************************************************************************/
	/* aa=(a1.^2)./(a2+eps)*bei;  %振幅校正值                                                                     */
	/************************************************************************/
	for (int i=0;i<lnBufferLength;i++)
	{
		double ldblData = (lpOutY1FFT[i]*lpOutY1FFT[i])/(lpOutY2FFT[i]+FLT_EPSILON  )*adblRatio;

		loAmpDiff[i] = ldblData;
	}

	//_END_PERF_MEASURE_TIME("3.3");
	_BEGIN_PERF_MEASURE_TIME();

	ldbldebug = loAmpDiff[lnBufferLength-1];

	lpdebug  = &loAmpDiff[lnBufferLength-10];

	//3.4 矫正谱第一个值处理
	/************************************************************************/
	/* aa(1)=a1(1)*2/NFFT*bei;
	   p2(1)=0;
	   df(1)=0;                                                            */
	/************************************************************************/
	loAmpDiff[0] = lpOutY1FFT[0]*2/lnBufferLength*adblRatio;

	lpOutPhaseY2[0] =.0;

	loDiff[0] =.0;

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

		loFreqAdjust[i] = (lnIndexTempFloor-1+ loDiff[lnIndexTempRound-1] )*adblSampleFreq/(double)lnBufferLength;

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

		if (apInputFreqSequenceToAdjust[i]<2*ldblDf)
		{
			loFreqAdjust[i] = ldblDf;

			ldblAmpAdjusted = loAmpDiff[1];

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
			ldblAmpAdjusted = loAmpDiff[lnIndexTempRound-1];

			ldblPhaseAdjusted = lpOutPhaseY2[lnIndexTempRound-1];

			ldblFreqAdjusted = loFreqAdjust[i] ;

			double ldblTemp = 2*_FFT_PI*((double)lnBufferLength-1)*ldblFreqAdjusted\
				/adblSampleFreq*180.0/_FFT_PI;

			ldblTemp = CFFT_Wrapper::MatlabMod(ldblTemp,360.0);			

			ldblPhaseAdjusted = ldblPhaseAdjusted - ldblTemp;

			ldblPhaseAdjusted = ldblPhaseAdjusted+ 90.0;
			
		}

		if (ldblPhaseAdjusted<0)
		{
			ldblPhaseAdjusted += 360.0;
		}

		ldblPhaseAdjusted = 90-ldblPhaseAdjusted;

		if (ldblPhaseAdjusted<0)
		{
			ldblPhaseAdjusted = ldblPhaseAdjusted+360.0;
		}

		ldblPhaseAdjusted = CFFT_Wrapper::MatlabMod(ldblPhaseAdjusted,360.0);	

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

	//_END_PERF_MEASURE_TIME("3.5");
	_BEGIN_PERF_MEASURE_TIME();

	anOutputLength = anInputFreqSequenceLength;

	return ERR_NO_ERROR;
}

int CFFT_Wrapper::PreparePlan()
{

	_DECLARE_PERF_MEASURE_TIME();
	_BEGIN_PERF_MEASURE_TIME();
	for (int i = 8; i <= 16384; i = i+2)
	{
		// Create new buffers and fill
		std::vector<float> loInput;

		int anInputLength = i;

		loInput.resize(anInputLength,0);

		float * lpInput = &loInput.front();

		fftwf_plan p = NULL;

		fftwf_complex* lpOut = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * anInputLength);

		p = fftwf_plan_dft_r2c_1d(anInputLength,lpInput,lpOut,FFTW_MEASURE  );
		
		if (NULL!= p)
		{
			fftwf_destroy_plan(p);

			char * lpStrWisdom = fftwf_export_wisdom_to_string(malloc);

			if (NULL!= lpStrWisdom)
			{
				CFFT_Wrapper::AppendAllPlan(lpStrWisdom,lstrlen(lpStrWisdom));

				try
				{
					free(lpStrWisdom);
				}
				catch (CMemoryException* e)
				{
					
				}
				catch (CFileException* e)
				{
				}
				catch (CException* e)
				{
				}

				
			}
		}		

		fftwf_free(lpOut); 


	}
	_END_PERF_MEASURE_TIME("PreparePlan");

	return CFFT_Wrapper::ERR_NO_ERROR;
}



