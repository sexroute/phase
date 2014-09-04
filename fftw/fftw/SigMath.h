#pragma once

/**@brief 函数输入中的如果出现H方向的波形数据或V方向的波形数据，并不是严格意义上的水平和垂直波形数据。
  *而是相对，按照旋转来说，就是先遇到的信号就是H方向的，后遇到就是V方向的。
  */

#include "Sigdefine.h"
#include "Matrix.h"


class CLASS_DLL_DSE_SigMath CSigMath
{
public:
	CSigMath(void);
	~CSigMath(void);
public:///谱分析相关的函数
	/**@brief 能量法频谱校正。 （低频效果好12*df以下，如果小于3*df，不做校正）
	  *@param pWave_ 时间序列
	  *@param iSmpFreq_ 采样频率
	  *@param ilen_     时间序列长度
	  *@param fSpecCorrectFreq_ 需要校正的频率
	  *@param fCorrectedFreq_ 校正后的频率
	  *@param fCorrectedAmp_ 校正后频率对应的幅值
	  *@param fCorrectedPhase_ 校正后频率对应的相位
	  */
    int EnergySpecCorrect(const double *pWave_,const int& iSmpFreq_, const int &ilen_ ,const double& fSpecCorrectFreq_, double &fCorretedFreq_, double &fCorretedAmp_, double &fCorrecdPhase_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak,E_WINOW_TYPE eWinType_ = WINDOW_HANNING);

	/**@brief 频谱分析
	  *@param pwave 时域波形数据
	  *@param n     波形的长度
	  *@param pout  频谱数据的输出，必须事先分配好内存，长度为n/2
	  *@param type  频谱类型0：峰值谱，1:功率谱
	  *@param iWinType_ 加窗的类型，见窗类型定义。
	  *@param iPrecision_ 精度，如果大于6，就不做精度处理，实际是多少就是少。
	  */
	int SpectralMeasureMent(const double *pwave,int n,double *pout,E_SpectrumType type,E_WINOW_TYPE iWinType_ = WINDOW_HANNING, unsigned int iPrecision_ = 2);
	///必须把vOut_长度为n/2。
	int SpectralMeasureMent(const double *pwave,int n,vector<double>& vOut_,E_SpectrumType type,E_WINOW_TYPE iWinType_ = WINDOW_HANNING, unsigned int iPrecision_ = 2);

	/**@brief 倒谱分析
	  *@param pwave 时域波形数据
	  *@param n     波形的长度
	  *@param pout  频谱数据的输出，必须事先分配好内存，长度为n/2
	  *@param iNums_ 倒谱的从0到指定长度的点置为0，目的是消除前端的干扰
	  *@param iPrecision_ 精度，如果大于6，就不做精度处理，实际是多少就是少。
	  */
	int Cepstrum(const double *pwave, const int& n,double *pout, const int &iNums_ = 20, unsigned int iPrecision_ = 2);
    
	/**频谱校正功能（按照指定点校正）,使用的全相位法、能量校正法。 注：不能是功率谱。 自动判断采用何种校正方法。校正指定频率成分。
	  *该方法直接校正指定频率
	  *@param pData_ 波形数据
	  *@param iFs_ 采样频率
	  *@param ilen_ 数据长度
	  *@param vSigComponet_ 需要校正的频率成分n，长度在外面分配好。输入：为需要校正的频率成分信息（不输入相位和幅值）；输出：校正后频率成分信息(顺序是按照频率从低往高排) （返回值为真正校正的频率个数）。
	  *@return >=0 表示正确； < 0失败。大于0 情况下，返回的是实际的校正的频率数目
	  */
	int SpectrumLinesCalibrate(const double* pData_, const int& iFs_, const int& ilen_,vector<SSigParam>& vSigComponet_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak,E_WINOW_TYPE eWinType_ = WINDOW_HANNING);
    
	/**全频谱校正功能（按照指定点校正）,使用的全相位法、能量校正部分频谱校正。不需要指定频率，进行直接校正。
      **先校正1X，然后通过1X计算其他频率后，进行其他频率校正
	  *把校正后的值赋给已经计算好频谱的对应位置 注：不能是功率谱
	  *校正的频率成分为：1/2X,1X,2X,3X,
	  *注意：E_SpectrumType 和 E_WINOW_TYPE 类型必须与你所传递频谱数据类型相同。
	  *@param pData_ 波形数据
	  *@param iFs_ 采样频率
	  *@param ilen_ 数据长度
	  *@param pSpectrumAmp_ 输入：已经做好的频谱数据，即绘制谱图时的Y轴数据（（频谱幅值）。 长度为iSpectrumlen_ 。输出：校正后的谱峰值
	  *@param pSpectrumf_ 输入：已经做好频谱的频率，即绘制谱图时的X轴数据。 长度为iSpectrumlen_。输出：校正后的频谱的频率值（横轴）
	  *@param iSpectrumLen_ 引用类型值，输入：校正前频谱数据的长度；输出：校正后频谱数据的长度
	  *@param spectrumType_ 频谱类型
	  *@param eWinType_ 加窗类型
	  *@param iPrecision_ 精度，如果大于6，就不做精度处理，实际是多少就是少。unsigned int iPrecision_ = 2
	  *@return 0 表示正确 否则失败
	  */
    int SpectrumCalibrate(const double* pData_, const double& f0_, const int& iFs_, const int& ilen_, double* pSpectrumf_,double * pSpectrumAmp_,int& iSpectrumLen_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak, E_WINOW_TYPE eWinType_ = WINDOW_HANNING,unsigned int iPrecision_ = 2 );

	/**内部调用“SpectrumCalibrate”该函数，为兼容老接口，所以保留
	  */
	int apSpectrumCalibrateEx(const double* pData_, const double& f0_, const int& iFs_, const int& ilen_, double* pSpectrumf_,double * pSpectrumAmp_,int& iSpectrumLen_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak, E_WINOW_TYPE eWinType_ = WINDOW_HANNING,unsigned int iPrecision_ = 2);

    /**使用校正谱计算特征值：0.5X，1X，2X，3X的幅值相位
	  *@param pData_ 波形数据
	  *@param  f0_   基准频率，一般为工频或1X频率
	  *@param iFs_ 采样频率
	  *@param ilen_ 数据长度
	  *@param vSigComponet_ 需要校正的频率成分4，长度需要在外部分配好
	  *@return 0 表示正确 否则失败
	  */
    int GetCalibratedSpectrumCharInfo(const double* pData_,const double& f0_, const int& iFs_, const int& ilen_,vector<SSigParam>& vSigComponet_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak,E_WINOW_TYPE eWinType_ = WINDOW_HANNING);

	/**全频谱校正功能（按照指定点校正）,使用的全相位法+全频法低频部分频谱校正。不需要指定频率，进行直接校正。（高频效果好12*df（包含）以上）
	*把校正后的值赋给已经计算好频谱的对应位置 注：不能是功率谱
	*校正的频率为：1/2X,1X,2X,3X,4X,0~2*df中间的一个数据，0~1X中间比较大的一个值
	*注意：E_SpectrumType类型必须与你所传递频谱数据类型相同。
	*@param pData_ 波形数据
	*@param iFs_ 采样频率
	*@param ilen_ 数据长度
	*@param pSpectrumAmp_ 输入：已经做好的频谱数据，即绘制谱图时的Y轴数据（（频谱幅值）。 长度为iSpectrumlen_ 。输出：校正后的谱峰值
	*@param pSpectrumf_ 输入：已经做好频谱的频率，即绘制谱图时的X轴数据。 长度为iSpectrumlen_。输出：校正后的频谱的频率值（横轴）
	*@param iSpectrumLen_ 引用类型值，输入：校正前频谱数据的长度；输出：校正后频谱数据的长度
	*@param spectrumType_ 频谱类型
	*@param eWinType_ 加窗类型
	*@return 0 表示正确 否则失败
	*/
	int apSpectrumCalibrateEx2(const double* pData_, const double& f0_, const int& iFs_, const int& ilen_, double* pSpectrumf_,double * pSpectrumAmp_,int& iSpectrumLen_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak, E_WINOW_TYPE eWinType_ = WINDOW_HANNING);


	/**@brief 计算全谱
	*@param  pWaveV_  输入垂直方向的波形数据
	*@param  pWaveH_  输入水平方向的波形数据
	*@param  ilen_ 波形数据长度
	*@param  f0_   基准频率，一般为工频或1X频率
	*@param  fFs_  采样频率
	*@param  iHiFcNum_ 要提取的高次谐波的数目
	*@param  fHiFcTh_  高次谐波的阈值，即超过这个阈值才提取该频率的谐波
	*@param  iLoFcNum_ 分频的数目
	*@param  mapFullSpecInfo_ 全谱的信息。主键值为倍频值（即f/f0），second为对应的值。必须提前在外部把内存分配好。长度为（iHiFcNum_+iLoFcNum_）的两倍
	*@param  bRemove50_ 是否消除50Hz干扰
	*/
	int FullSpectrum(const double *pWaveV_,const double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, vector<SSigParam>& mapFullSpectumInfo_,bool bRemove50_ = true);

	/**@brief 计算全谱第二种方法。该方法是不依赖于转速计算，计算每个谱线上的值。即X轴为（-iFs/2,iFs/2）。
	*@param  pWaveV_  输入垂直方向的波形数据
	*@param  pWaveH_  输入水平方向的波形数据
	*@param  ilen_ 波形数据长度
	*@param  f0_   基准频率，一般为工频或1X频率
	*@param  fFs_  采样频率
	*@param  vFullSpectrumX_ 全谱X轴输出，单位是Hz。长度为：ilen_.必须外部分配好
	*@param  vFullSpectrumY_ 全谱Y轴输出。长度为：ilen_.必须外部分配好
	*/
	int FullSpectrumII(const double *pWaveV_,const double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, vector<double>& vFullSpectrumX_,vector<double>& vFullSpectrumY_);



	/**@brief 互功率谱方法计算相位
	  *@param pWave_ 波形数据。即时间序列数据
	  *@param iLen_  波形数据长度
	  *@param fSpecFreq_ 需要计算相位的频率
	  *@param iSmpFreq_ 波形数据采样频率
	  *@param fPhase_   返回的相位
      */
	int CrossSpecPhase(const double *pWave_,const int& iLen_, double fSpecFreq_,const int& iSmpFreq_,double & dPhase_);

	int CrossPhaseDetect(const double *pWaveX_, const double *pWaveY_, const int& ilen_, const int& iSmpFreq_,const double& fSpecFreq_,double & fPhase_);

	/**@brief  提纯波形数据.分别得到低频波形和各谐波的波形，然后进行合成
	  *@param  pWave_ 波形数据
	  *@param  iLen_ 波形数据长度
	  *@param  f0_   基准频率，一般为工频或1X频率
	  *@param  fFs_  采样频率
	  *@param  iHiFcNum_ 要提取的高次谐波的数目
	  *@param  fHiFcTh_  高次谐波的阈值，即超过这个阈值才提取该频率的谐波
      *@param  iLoFcNum_ 分频的数目
      *@param  vSigParam_ 输出波形所包含的频率成分,必须在外部分配好向量表，长度为（iHiFcNum_+iLoFcNum_）
	  *@param  pWaveOutput_ 提纯后的波形，内存必须在外部分配好，长度为iLen_
	  *@param  bRemove50_ 是否消除50Hz干扰
	  *@return E_DseSigRet
      */
	 int SCorbitpurifier(const double *pWave_,const int& iLen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, vector<SSigParam>& vSigParam_,double * pWaveOutput_,bool bRemove50_ = true);
     
	 /**@brief计算根据指定参数绘制出来椭圆的长轴倾角，返回的是度数，不是弧度
	   *@param  pWaveV_  输入垂直方向的波形数据，即后遇到的数据
	   *@param  pWaveH_  输入水平方向的波形数据，即先遇到的数据
	   *@param  ilen_ 波形数据长度。两者波形长度必须相等.长度不能小于36
	   *@param  fAngle_ 引用值变量。倾角
	   *@return 是否正确
	   */
     int EllipBiasAngle(const double *pWaveV_,const double *pWaveH_, const int& ilen_, double& fAngle_);
	 ///fFreqV_ 垂直方向的频率，fPhaseV_垂直方向的相位。一般要求fFreqV_=fFreqH_
	 int EllipBiasAngle(double fFreqV_, double fPhaseV_, double fAmpV_, double fFreqH_, double fPhaseH_, double fAmpH_, double& fAngle_);



	 /**@brief 提纯轴心轨迹，带修正
	   *@param pWaveV_  输入垂直方向的波形数据，输出后提纯修正后的波形数据
	   *@param pWaveH_  输入水平方向的波形数据，输出后提纯修正后的波形数据
	   *@param  ilen_ 波形数据长度
	   *@param  f0_   基准频率，一般为工频或1X频率
	   *@param  fFs_  采样频率
	   *@param  iHiFcNum_ 要提取的高次谐波的数目
	   *@param  fHiFcTh_  高次谐波的阈值，即超过这个阈值才提取该频率的谐波
	   *@param  iLoFcNum_ 分频的数目
	   *@param  vSigParamV_ 垂直输出波形所包含的频率成分,必须在外部分配好向量表，长度为（iHiFcNum_+iLoFcNum_）
	   *@param  vSigParamH_ 水平输出波形所包含的频率成分,必须在外部分配好向量表，长度为（iHiFcNum_+iLoFcNum_）
	   *@param  bRemove50_ 是否消除50Hz干扰
	   */
     int SCorbitpurifie_Rp(double *pWaveV_, double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, vector<SSigParam>& vSigParamV_,vector<SSigParam>& vSigParamH_,bool bRemove50_ = true);
	 int SCorbitpurifie_Rp(double *pWaveV_, double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, bool bRemove50_ = true);

	 /**Method:    CheckOrbitShape  轴心轨迹分类
      *@brief   FullName:  CSigMath::CheckOrbitShape
      *@param  : double * pWaveV_ 垂直方向波形数据
      *@param  : double * pWaveH_ 水平方向波形数据
      *@param  : const int & ilen_ 波形数据长度
      *@param  : double f0_ 工频
      *@param  : const int & iFs_ 采样频率
      *@param  : double fThFactor_ 阈值因子
      *@return:   int 小于0表示错，反之为轴心轨迹形状，具体值见轴心轨迹形状宏定义 E_OrbitShape
	  */
	 int CheckOrbitShape(const double *pWaveV_, const double *pWaveH_, const int& ilen_,double f0_, const int& iFs_,double fThFactor_= 0.25);
  

	 /**@brief Fast Discrete Walsh-Hadamard Transform
	 *@param pWave_  波形数据
	 *@param ilen_   波形数据的长度
	 *@param pWaveOut_  输出的波形数据，必须在外部把内存分配好，长度为ilen_ 
	 */
	 int FWalshHT(const double * pWave_, const int&ilen_, double *pWaveOut_);

	/**两维曲线包络
	  *@param pXInput_,pYInput_ 点集向量，长度为iLen_
      *@param iLen_ 点集的长度
	  *@param pXOutput_,pYOutput_, 输出曲线的点集向量，必须在外部预先分配好，长度为iLen_。包络后曲线的长度为返回值
	  *@param fMaxDisCoef_ 包络曲线的最大长度系数。最大长度为点集x方向数据和y方向数据中的坐标最长距离乘以该系数。
	  *@param iPrecision_ 精度，如果大于6，就不做精度处理，实际是多少就是少。unsigned int iPrecision_ = 2
      *@return 小于0为失败，否则包络后曲线的实际点数
	  */
	int TwoDimCurveEnvelope(const vector<double>& pXInput_,const vector<double>& pYInput_, const int &iLen_,  vector<double>& pXOutput_, vector<double>& pYOutput_, const double& fMaxDisCoef_ = 0.2,unsigned int iPrecision_ = 2);
	int TwoDimCurveEnvelope(const double* pXInput_,const double* pYInput_, const int &iLen_,  double* pXOutput_, double* pYOutput_, const double& fMaxDisCoef_ = 0.2,unsigned int iPrecision_ = 2);

	  /**1维曲线包络
		*@param pInput_ 1维线向量，长度为iLen_.不能小于32个点,最起码要分3段
		*@param iLen_ 点集的长度
		*@param pUpperLine_ 曲线的包络后，上面的曲线，必须在外部预先分配好，长度为iLen_。包络后曲线的长度为返回值
		*@param pLowerLine_ 曲线的包络后，下面的曲线，必须在外部预先分配好，长度为iLen_。包络后曲线的长度为返回值
		*@param iPerSegMinPts_ 拟合分段时，每段的最少点数，不能小于5。
		*@return 小于0为失败，否则包络后曲线的实际点数
		*/
	int OneDimCurveEnvelope(const vector<double>& pInput_, const int &iLen_, vector<double>& pUpperLine_, vector<double>& pLowerLine_,const int& iPerSegMinPts_ = 10);

	/**计算1维的分段有效值柱图,一般要求ilen_要远大于iSegs_
	*@param pwave_ 1维向量，长度为ilen_
	*@param ilen_ 点集的长度
	*@param pbar_ 必须在外部预先分配好，长度为iSegs_。计算的柱值输出
	*@param iSegs_ 分段的数目
	*@return 小于0为失败，否则柱的数目
	*/
	int CalcRmsBar(const vector<double>& pwave_, const int& ilen_, vector<double>& pbar_, const int& iSegs_ = 8);
	int CalcRmsBar(const double* pwave_, const int& ilen_, double* pbar_, const int& iSegs_ = 8);

	/**计算1维的分段峰值柱图,一般要求ilen_要远大于iSegs_
	*@param pwave_ 1维向量，长度为ilen_
	*@param ilen_ 点集的长度
	*@param pbar_ 必须在外部预先分配好，长度为iSegs_。计算的柱值输出
	*@param iSegs_ 分段的数目
	*@return 小于0为失败，否则柱的数目
	*/
	int CalcPeakBar(const vector<double>& pwave_, const int& ilen_, vector<double>& pbar_, const int& iSegs_ = 8);
	int CalcPeakBar(const double* pwave_, const int& ilen_, double* pbar_, const int& iSegs_ = 8);

    /**自动寻优包络解调分析，自动寻找最优的截止频率 
	  *@param pwave_ 1维向量，长度为ilen_
	  *@param ilen_ 点集的长度
	  *@param iFs_  采样频率
	  *@param fgSEValue_ 返回计算得到的gSE值
	  *@param pEnvWave_ 返回冲击诊断分析后的波形，必须预先分配好内存，长度为ilen_。如果不需要返回就设置为NULL
	  *@return 小于0表示失败。大于0就表示自动寻优后的截止频率
	  */
    int AutoOptimizeEnvelopeAnalysis(const double* pwave_, const int& ilen_, const int& iFs_,double & fgSEValue_, double *pEnvWave_ = NULL);

	/**冲击诊断分析：包络分析，并计算gSE值
	  *@param pwave_ 1维向量，长度为ilen_
	  *@param ilen_ 点集的长度
	  *@param iFs_  采样频率
	  *@param iCutoffFreq_ 高通滤波的截止频率
	  *@param fgSEValue_ 返回计算得到的gSE值
	  *@param pEnvWave_ 返回冲击诊断分析后的波形，必须预先分配好内存，长度为ilen_。如果不需要返回就设置为NULL
	  *@param iPrecision_ 精度，如果大于6，就不做精度处理，实际是多少就是少。
	  *@return 小于0表示失败
	  */
	int gSEAnalysis(const double* pwave_, const int& ilen_, const int& iFs_,const int& iCutoffFreq_, double & fgSEValue_, double *pEnvWave_ = NULL,unsigned int iPrecision_ = 2);

    /**PeakVue算法
	  *@param pwave_ 1维向量，长度为ilen_
	  *@param ilen_ 点集的长度
	  *@param iFs_  采样频率
	  *@param iReSamplesFs_ 重采样的频率。必须让iFs_可以整除
	  *@param iCutoffFreq_ 高通滤波的截止频率
	  *@param pPeakVueWave_ 返回做PeakVue后的波形，必须预先分配好内存，长度为ilen_
	  *@return 小于0 ，表示计算失败，否则为PeakVue计算后的波形点数
	  */
	int PeakVue(const double *pwave_, const int& ilen_, const int& iFs_, const int& iReSamplesFs_,const int& iCuttoffFreq_,  double * pPeakVueWave_);
	

   //************************************
	// Method:    IntegrateDecimate_Customize 风电机泵的专用加速度重采样（具备抗混功能）和积分为速度算法。
	//            重采样后的采样频率只支持5120Hz（机泵），2560Hz，3200Hz（风电高速端），80Hz（风电低速端）。
	// FullName:  CBHDSP::IntegrateDecimate_Customize
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: const double * pdata_ 待积分的数据
	// Parameter: const unsigned int & ilen_ 数据的长度
	// Parameter: const int & iFs_   数据的采样频率。建议采样频率使用25600Hz、20480Hz和15360Hz
	// Parameter: const unsigned int & iWorkRev_  该测点数据对应的工作转速
	// Parameter: const int iExtractMutli_    抽取倍数。即抽取后的频率为 iFs_/iExtractMutli_.
	// Parameter: double * pIntergatedata_    积分后的数据，内存必须在调用前分配好，长度为 iIntegrateDataLen_
	// Parameter: unsigned int & iIntegrateDataLen_ 输入：预先分配积分后的数据输出的长度；输出：积分后的数据长度。
	// Parameter: float fSensitivity_ 灵敏度系数 即输出(积分后)=输入/fSensitivity_
	//************************************
	int IntegrateDecimate_Customize(const double* pdata_, const unsigned int& ilen_, const unsigned int& iFs_, const unsigned int& iWorkRev_,const int iExtractMutli_, double *pIntergatedata_, unsigned int& iIntegrateDataLen_, const float& fSensitivity_ = 0.001);
	
	//************************************
	// Method:    Integrate_Customize 风电机泵的专用加速度积分为速度算法。采样频率只支持5120Hz（机泵），2560Hz,3200Hz（风电高速端），80Hz（风电低速端）。
	// FullName:  CBHDSP::Integrate_Customize
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: const double * pdata_ 待积分的数据
	// Parameter: const unsigned int & ilen_ 数据的长度
	// Parameter: const int & iFs_   数据的采样频率,采样频率只支持5120Hz（机泵），3200Hz（风电高速端），80Hz（风电低速端）。
	// Parameter: const unsigned int & iWorkRev_  该测点数据对应的工作转速
	// Parameter: double * pIntergatedata_    积分后的数据，内存必须在调用前分配好，长度为 ilen_
	// Parameter: float fSensitivity_ 灵敏度系数 即输出(积分后)=输入/fSensitivity_
	//************************************
	int Integrate_Customize(const double* pdata_, const unsigned int& ilen_, const unsigned int& iFs_, const unsigned int& iWorkRev_,double *pIntergatedata_,const float& fSensitivity_ = 0.001);



	/**数字积分函数进行积分。
	 *@param pfData_    需要积分的数据
	 *@param pfDataRs_  积分后结果输出，长度为数据长度
	 *@param iDataLen_  数据长度
	 *@param iSmpFreq_  采样频率     
	 *@param fFreqBandLo_ 带通滤波下限
	 *@param fFreqBandHi_ 带通滤波上限
	 *@Parameter: float fSensitivity_ 灵敏度系数 即输出(积分后)=输入/fSensitivity_
	 *@return  0 表示正确；-1表示参数不正确
	 */
	int IntegratorEx(const double *pfData_,double *pfDataRs_,const int& iDataLen_,const int& iSmpFreq_,double fFreqBandLo_ = 5, double fFreqBandHi_ = 1000,const float& fSensitivity_ = 1);


	/**使用频域方法进行积分数字积分函数API。
	 *@param pfData_    需要积分的数据
	* @param pfDataRs_  积分后结果输出，长度为数据长度
	*@param iDataLen_  数据长度
	*@param iSmpFreq_  采样频率     
	*@param fFreqBandLo_ 带通滤波下限
	*@param fFreqBandHi_ 带通滤波上限
	*@param wType_ 频谱加窗的类型
	*@return  0 表示正确；-1表示参数不正确
	*/
	int IntegratorEx2(const double *pfData_,double *pfDataRs_,const int& iDataLen_,const int& iSmpFreq_, double fFreqBandLo_ = 5, double fFreqBandHi_ = 1000,E_WINOW_TYPE wType_ = WINDOW_HANNING);

	/**对频谱进行积分
	*@param pSpectrum 输入要积分的频谱数据，计算成功后，为积分后的结果
	*@param df 频谱分辨率
	*@param ilen_ pSpectrum数据的数目
	*@param fSensitivity_ 灵敏度系数 即输出(积分后)=输入/fSensitivity_
	*@param eSpectrumType_  需要积分频谱数据的频谱类型
	*@param iStartidx_ 起始积分的索引
	*@return  0 表示正确；小于0表示错误
	*/
	int SpectrumIntegrate(double * pSpectrum_, const double& df_,const int& ilen_, const double& fSensitivity_= 1.0,E_SpectrumType eSpectrumType_ = E_SpectrumType_PEAK, const int& iStartidx_ = 2);

	/**使用标准样本与待检样本进行异常判断，根据置信度和标准差的倍数计算后返回判断的结果
	*@param sampleData_ 标准（正常）样本。n * k 矩阵。k是特征值数目，n是样本数目
	*@param testData_   待检（测试）样本。m * k 矩阵。k是特征值数目，m是样本数目
	*@param alpha_     置信度。是一个百分比数。默认 0.9，即90%
	*@param factorAlarm_    标准差的放大系数 早中期故障（预警）
	*@param factorDanger_      标准差的放大系数 晚期故障（已经严重，需要停车）
	*@return 0 表示正常，大于0待检样本异常。小于0：计算过程中出现异常。
	*/
	int AbnormalJudge(const CMatrix& sampleData_,const CMatrix& testData_, const float& alpha_ = 0.9,const float& factorAlarm_ = 2, const float& factorDanger_ = 3 );

	/**使用标准样本与待检样本进行异常判断。样本的特征值数目为k，根据置信度和标准差的倍数计算后返回判断的结果。
	*@param testData_   待检（测试）样本。m * k 矩阵。k是特征值数目，m是样本数目
	*@param sigma_   标准差矩阵 k*k
	*@param vMeanValue_ 均值向量，长度为k
	*@param alpha_     置信度。是一个百分比数。默认 0.9，即90%
	*@param factorAlarm_    标准差的放大系数 早中期故障（预警）
	*@param factorDanger_      标准差的放大系数 晚期故障（已经严重，需要停车）
	*@return 0 表示正常，大于0待检样本异常。小于0：计算过程中出现异常。
	*/
	int AbnormalJudge2(const CMatrix& testData_,const CMatrix& sigma_,const vector<double>& vMeanValue_, const float& alpha_ = 0.9,const float& factorAlarm_ = 2, const float& factorDanger_ = 3 );

	/**计算1组示功图的特征值（7个：面积，周长，峭度，歪度，walsh变换后得到的设定段数个值（默认为3个））
      *@param pV_   1个周期示功图曲线的体积序列，长度为ilen_
	  *@param pP_   1个周期示功图曲线的压力序列，长度为ilen_
	  *@param ilen_ 1个周期示功图曲线的点数
	  *@param fPs_  名义进气压力
	  *@param fPd_  名义排气压力
	  *@param vFeatures_ 特征向量，内存必须提前分配好，长度为4+iSegNums
	  *@param th_   阈值，walsh变换向量特征值计算时使用，默认为1
	  *@param iSegNums 段数，walsh变换向量分段计算超过阈值的数目。
	  */
	int PVGraphFeature(const double* pV_,const double* pP_, const int& ilen_,const double& fPs_, const double& fPd_, vector<double>& vFeatures_, const int& th_ = 1, const unsigned int& iSegNums_ = 3 );
	
	/**自动寻峰算法。只搜正峰值，不搜负峰值
	  *@brief pData_ 寻峰的数据序列
	  *@brief ilen_ 数据长度
	  *@brief iPeakNums_ 输入为期望寻峰的数目，输出为实际寻峰的数目
	  *@brief vPeakInd_ 峰值的索引。长度必须在外部分配好，长度为iPeakNums_
	  *@brief iInterval_ 两边小于其的数目。即峰值两边小于其值的数目
	  *@brief fth_：阈值，峰值必须大于该值才算峰值
	  */
	int Peak_Detection(const double* pData_, const int& ilen_, int& iPeakNums_,vector<int>& vPeakInd_, const int& iInterval_ = 2, const double& fth_ = .1);

	/**搜索出最大的倍频周围几个频谱峰值的对应频率和值。注意需要提前把vSigParam_尺寸分配好
	 *搜索最大的低频3个，同频、倍频4个。即最高到4倍频，如果实际分辨率没有那么高，就不搜计算主导频率
	 *@brief vSigParam_ 搜索的结果输出，长度为iLoNums_+ iHiNums_
	 *@brief iLoNums_ 搜索的低频数目
	 *@brief iHiNums_ 搜索的倍频数目。1X，2X，...
	 *@return 小于0为失败，否则为搜到的有用信息个数
	 */
	int SpectrumLinesPeakSearchingEx(const double *pWave_, const int& iLen_, const int &iFs_, const double& f0_,vector<SSigParam>& vSigParam_,const int& iLoNums_ = 3, const int & iHiNums_ = 4);

	/**小波包压缩算法。注意需要提前把vSigParam_尺寸分配好。
	  *适用条件：对信号要求：长度不能小于64，必须是2^n，n>6
	  *@return 小于0为失败，大等于0为成功。
	  */
	int WPCompress(const vector<double>& vWave_, const int &iFs_, CCompressedParamInfo & compressedParamInfo_);

	/**vRestronctedData_ 信号在外部内存分配好，长度表示信号长度
	  *适用条件：对信号要求：长度不能小于64，必须是2^n，n>6
	  */
	int WPDecompress(const CCompressedParamInfo& compressedParamInfo_, vector<double>& vRestronctedData_);
public:
	/**@fn CalcBearingDefectFreq
	*@brief  计算滚动轴承的故障（通过）频率
	*@param  bearingpar_ 滚动轴承各种参数
	*@param  fWorkRev_   工作转速
    *@param  defectFreq_  [out] 输出计算缺陷频率
	*@return 0表示成功，小于0失败
	*/
	int  CalcBearingDefectFreq(const S_BearingPar& bearingpar_, const int& nWorkRev_, S_BearingDefectFreq& defectFreq_);

	/**@fn ApCalcBearingDefectFreq
	*@brief  简化近似计算滚动轴承的故障（通过）频率，注意：没有滚动体的通过频率
	*@param  rollingnum 滚动轴承滚动体个数
	*@param  fWorkRev_   工作转速
	*@param  defectFreq_  [out] 输出计算缺陷频率
	*@return 0表示成功，小于0失败
	*/
	int  ApCalcBearingDefectFreq(const int& rollingnum_, const int& nWorkRev_, S_BearingDefectFreq& defectFreq_);

   
	/**计算轴心位置或者活塞杆位置（有关水平、垂直、XY方向定义参见结构体S_AxisLocationInputInfo的注释）
	  *@param inputInfo_ 计算所需参数
	  *@param 引用变量 fXValue_ X方向值，单位：mm
      *@param 引用变量 fYValue_ Y方向值，单位：mm
	  *@return 0表示成功，小于0失败
	  */
	int CalcAxisCenterLocation(const S_AxisLocationInputInfo& inputInfo_, double& fXValue_, double& fYValue_); 

   /**函数名称：计算二维坐标中点的角度（单位是°）
  	 *@param fXCorrd_   输入：X轴坐标的值
	 *@param fYCorrd__  输入：Y轴坐标的值
	 *@return：返回值为角度
	 */
	double Get2DPtAngle(const double& fXCorrd_, const double& fYCorrd_);


   /**函数名称：计算二维坐标中点的半径
  	 *@param fXCorrd_   输入：X轴坐标的值
	 *@param fYCorrd__  输入：Y轴坐标的值
	 *@return：返回值为半径
	 */
	double Get2DPtRadius(const double& fXCorrd_, const double& fYCorrd_);
	
	/**函数名称：计算一次开停车一个测量位置上XY方向波特图矢量合成的序列。输入的5个变量长度必须一致，且要大于0
	  *@return：如果小于0，表示计算错误
	  *@param vAmpX_    输入：X方向或水平方向波特图的幅值数组。计算完成后，波特图输出的幅值序列
	  *@param vPhaseX_  输入：X方向或水平方向波特图的相位数组。计算完成后，波特图输出的相位序列
	  *@param vRev_     输入：波特图的转速数组
	  *@param vAmpY_    输入：Y方向或垂直方向波特图的幅值数组
	  *@param vPhaseY_  输入：Y方向或垂直方向波特图的相位数组
      */
	int GetSudXYVectorCompose(vector<double>& vAmpX_, vector<double>& vPhaseX_, const vector<double>& vRev_, const vector<double>& vAmpY_, const vector<double>& vPhaseY_);

    /**函数名称：计算两次开停车合成波特图的差序列，是第二组减去第一组。输入的5个变量长度必须一致，且要大于0.第一组为基准
	  *@return：如果小于0，表示计算错误
	  *@param vAmpX1_    输入：第一组X方向波特图的幅值数组。计算完成后，波特图输出的幅值序列
	  *@param vPhaseX1_  输入：第一组X方向波特图的相位数组。计算完成后，波特图输出的相位序列
	  *@param vRev1_     输入：波特图的转速数组。            计算完成后，波特图输出的转速序列。注意是，如果对应的转速为-1，则抛弃该组数据
	  *@param vAmpY1_    输入：第一组Y方向波特图的幅值数组。
	  *@param vPhaseY1_  输入：第一组Y方向波特图的相位数组。
	  *@param vAmpX2_    输入：第二组X方向波特图的幅值数组
	  *@param vPhaseX2_  输入：第二组X方向波特图的相位数组
	  *@param vAmpY2_    输入：第二组Y方向波特图的幅值数组
	  *@param vPhaseY2_  输入：第二组Y方向波特图的相位数组
	  *@param vRev2_    输入：波特图的转速数组
	  */
	int Get2SudComposeVectorDiff(vector<double>& vAmpX1_, vector<double>& vPhaseX1_, vector<double>& vRev1_, const vector<double>& vAmpY1_,const vector<double>& vPhaseY1_,  const vector<double>& vAmpX2_, const vector<double>& vPhaseX2_,const vector<double>& vAmpY2_, const vector<double>& vPhaseY2_,const vector<double>& vRev2_);

    
	//************************************
	// Method:    Get1PeriodCycleRunoutVector  
	// FullName:  CSigMath::Get1PeriodCycleRunoutVector
	// Access:    public 
	// Returns:   int
	// Qualifier:  1个周期某个测量方向的转子跳动量周向变化值
	// Parameter: const double * pWave_  输入的波形
	// Parameter: const int & iLen_      波形长度
	// Parameter: const int & iFs_       采样频率
	// Parameter: const double & f0_     工频
	// Parameter: vector<double> & vAmpOutput_  输出的幅值序列。必须提前分配好序列的长度，长度为iLen_
	// Parameter: vector<double> & vPhaseOutput_  输出的相位序列。必须提前分配好序列的长度，长度为iLen_
	// Parameter: ilenOutput_   输出后的向量长度。
	// Parameter: iInitialPhase_  初始相位角
	//************************************
	int Get1PeriodCycleRunoutVector(const double *pWave_, const int& iLen_, const int &iFs_, const double& f0_,vector<double>& vAmpOutput_,vector<double>& vPhaseOutput_,int& ilenOutput_,const double& iInitialPhase_ = .0);

public:
///动平衡系列算法。前缀为“DSB”表示双面动平衡，“SSB”表示单面动平衡
    
	//************************************
	// Method:    SSB_CalcEffectCoef 。单面动平衡影响系数的计算
	// FullName:  CSigMath::SSB_CalcEffectCoef
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const double & fAmp0_    试重前幅值，单位：um
	// Parameter: const double & fPhase0_  试重前相位，单位：°(度)
	// Parameter: const double & fWeight_    试重质量，单位：g（克）
	// Parameter: const double & fWeightPhase_ 试重角度，单位：°(度)
	// Parameter: const double & fAmp1_      试重后幅值，单位：um
	// Parameter: const double & fPhase1_    试重后相位，单位：°(度)
	// Parameter: & complex<double> & cfA_   输出的影响系数
	//************************************
	int SSB_CalcEffectCoef(const double& fAmp0_,const double& fPhase0_, const double& fWeight_, const double& fWeightPhase_,const double& fAmp1_,const double& fPhase1_, complex<double>& cfCoef_);

	//************************************
	// Method:    SSB_GetCounterWeight  得到单面动平衡的配重的质量及相位信息
	// FullName:  CSigMath::SSB_GetCounterWeight
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const double & fAmp0_  原始振动幅值，单位：um
	// Parameter: const double & fPhase0_ 原始振动相位，单位：°(度)
	// Parameter: const complex<double> & cfCoef_  影响系数
	// Parameter: const bool & bRotationClockwise_ 旋转方向是否为顺时针旋转（从驱动端向非驱动端看）
	// Parameter: double & fWeight_       配重的质量，单位一定和计算影响系数时，试重的质量相同
	// Parameter: double & fWeightPhase_  配重的相位，单位：°(度)
	//************************************
	int SSB_GetCounterWeight(const double& fAmp0_,const double& fPhase0_,const complex<double>& cfCoef_, const bool& bRotationClockwise_,double& fWeight_, double& fWeightPhase_);

    //************************************
    // Method:    DSB_CalcEffectCoef   双面动平衡4个影响系数的计算。0端：指靠近驱动端，1端：指远离驱动端
    // FullName:  CSigMath::DSB_CalcEffectCoef
    // Access:    public 
    // Returns:   int
    // Qualifier:
    // Parameter: const double & fAmp0      0端原始振动幅值，单位：um
    // Parameter: const double & fPhase0_   0端原始振动相位，单位：°(度)
    // Parameter: const double & fAmp1      1端原始振动幅值，单位：um
    // Parameter: const double & fPhase1_   1端原始振动相位，单位：°(度)
    // Parameter: const double & fWeight0_       仅0端试重质量，单位：g（克）
    // Parameter: const double & fWeightPhase0_  仅0端试重相位，单位：°(度)
    // Parameter: const double & fAmp00      仅0端试重时，0端振动幅值，单位：um
    // Parameter: const double & fPhase00_   仅0端试重时，0端振动相位，单位：°(度)
    // Parameter: const double & fAmp10      仅0端试重时，1端振动幅值，单位：um
    // Parameter: const double & fPhase10_   仅0端试重时，1端振动相位，单位：°(度)
    // Parameter: const double & fWeight1_       仅1端试重质量，单位：g（克）
    // Parameter: const double & fWeightPhase1_  仅1端试重相位，单位：°(度)
    // Parameter: const double & fAmp01      仅1端试重时，0端振动幅值，单位：um
    // Parameter: const double & fPhase01_   仅1端试重时，0端振动相位，单位：°(度)
    // Parameter: const double & fAmp11      仅1端试重时，1端振动幅值，单位：um
    // Parameter: const double & fPhase11_   仅1端试重时，1端振动相位，单位：°(度)
    // Parameter: complex<double> & cfCoef00_  0端对0端的影响系数
    // Parameter: complex<double> & cfCoef10_  0端对1端的影响系数
    // Parameter: complex<double> & cfCoef01_  1端对0端的影响系数
    // Parameter: complex<double> & cfCoef11_  1端对1端的影响系数
    //************************************
	int DSB_CalcEffectCoef(const double& fAmp0,const double& fPhase0_,const double& fAmp1,const double& fPhase1_, const double& fWeight0_,const double& fWeightPhase0_,const double& fAmp00,const double& fPhase00_,const double& fAmp10,const double& fPhase10_, const double& fWeight1_,const double& fWeightPhase1_,const double& fAmp01,const double& fPhase01_,const double& fAmp11,const double& fPhase11_, complex<double>& cfCoef00_, complex<double>& cfCoef10_,complex<double>& cfCoef01_,complex<double>& cfCoef11_);
   

	//************************************
	// Method:    DSB_GetCounterWeight   得到双面动平衡的0、1端配重的质量及相位信息。0端：指靠近驱动端，1端：指远离驱动端
	// FullName:  CSigMath::DSB_GetCounterWeight
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const double & fAmp0      0端原始振动幅值，单位：um
    // Parameter: const double & fPhase0_   0端原始振动相位，单位：°(度)
    // Parameter: const double & fAmp1      1端原始振动幅值，单位：um
    // Parameter: const double & fPhase1_   1端原始振动相位，单位：°(度)
	// Parameter: const complex<double> & cfCoef00_  0端对0端的影响系数
	// Parameter: const complex<double> & cfCoef10_  0端对1端的影响系数
	// Parameter: const complex<double> & cfCoef01_  1端对0端的影响系数
	// Parameter: const complex<double> & cfCoef11_  1端对1端的影响系数
	// Parameter: const bool & bRotationClockwise_   旋转方向是否为顺时针旋转（从驱动端向非驱动端看）
	// Parameter: double & fWeight0_             0端配重的质量，单位一定和计算影响系数时，试重的质量相同
	// Parameter: double & fWeightPhase0_        0端配重的相位，单位：°(度)
	// Parameter: double & fWeight1_             1端配重的质量，单位一定和计算影响系数时，试重的质量相同
	// Parameter: double & fWeightPhase1_        1端配重的相位，单位：°(度)
	//************************************
	int DSB_GetCounterWeight(const double& fAmp0_,const double& fPhase0_,const double& fAmp1_,const double& fPhase1_, const complex<double>& cfCoef00_, const complex<double>& cfCoef10_,const complex<double>& cfCoef01_,const complex<double>& cfCoef11_,const bool& bRotationClockwise_, double& fWeight0_, double& fWeightPhase0_,double& fWeight1_, double& fWeightPhase1_);  

public: ///计算特征部分
	/**computes the peak-peak value of  data sequence 有效峰峰值
	*@param: pfwave_ denotes data sequence
	*@param: iLen_  denotes the length of data sequence
	*@return: Peak-Peak value
	*/
	float CalcPeak_PeakValue(const float *pfWave_,const int&  iLen_);
	// 有效峰峰值
	double CalcPeak_PeakValue(const double *pfWave_,const int& iLen_);
    

	/**c计算指定数据的均方差和平均值
	*@param: pdata_ 数据
	*@param: ilen_  数据长度
	*@param: fstd_ 返回均方差值
	*@param: fmean_ 返回平均值
	*@return: 小于0失败。0表示成功
	*/
	int CalcStdMean(const double *pdata_,const int& ilen_, double& fstd_, double& fmean_);

	///计算特征值
    double FeatureValue(const double *pfWave_,const int& iLen_,E_FeatureValueType featureType_);

	/**判断指定频谱索引周围是否存在明显谱峰。
	  *@param pSpectrum_ 频谱数据
	  *@param ilen_   pfWave_的数据个数
	  *@param iSpecInd_  指定索引
	  *@param iSideRange_  在指定索引的搜索范围： iSpecInd_-iSideRange_, iSpecInd+iSideRange_
      *@return 小于0:函数执行失败; 0: 就是不存在; 大于0:为谱峰对应索引
	  */
    int IsExistSpectrumPeak(const double *pSpectrum_, const unsigned int& ilen_, const unsigned int& iSpecInd_, const unsigned int& iSideRange_  = 2 );

	///临时测试一些函数
	double Test1(double * p1, double * p2_,int i1_);
private:
    /**按理论公式，计算、绘制指定多成分的所有全谱参数（倍频数、正、反进动圆半径）
	  *@param vSigParamV_ 垂直方向的信号信息
	  *@param vSigParamH_ 垂直方向的信号信息
	  *@param mapFullSpecInfo_ 全谱的信息。主键值为倍频值（即f/f0），second为对应的值。必须提前在外部把内存分配好。长度为vSigParamV_长度的两倍
	  */
	int FullSpectrumParaNFreq(const float& f0_,const vector<SSigParam>& vSigParamV_, const vector<SSigParam>& vSigParamH_, vector<SSigParam>& mapFullSpecInfo_);

	/*! 对时间序列进行加窗。直接对指针指向的时间序列进行修改的。
	  !@param  *pwave, 输入的时间序列，直接对这个序列进行加窗.
	  !@param n,时间序列长度
	  !@param type 所加窗的类型
	  */
	void  WindowedWave(double *pwave,int n,int type=WINDOW_HANNING);

	/**filter 
	  *@brief: cascaded filter stage algorithm using direct-form equations
	  *@param: pfwave_ denotes data sequence
	  *@param: iWaveLen_ denotes the length of data
	  *@param: ppfB_(ns*3) denotes the filter coefficients of numerator in each second-order stage  
	  *@param: ppfA_(ns*2) denotes the filter coefficients of denominator in each second-order stage
	  *@param: iNs_  denotes number of second-order stage
	  *@param: pfOut_ denotes data sequence of filtered
	  *@return: successed(true) of failed(false)
	  */
	bool filterIIR_C(double *pfWave_,int iWaveLen_,double **ppfB_,double **ppfA_,double *pfOut_,int iNs_);
	bool filterIIR_C2(float *pfWave_,int iWaveLen_,float **ppfB_,float **ppfA_,float *pfOut_,int iNs_);
public:
	int writefile(const float * ag_fwave, int ag_num,int index);
	int writefile(const double * ag_fwave, int ag_num,int index);
	int writefile(const vector<double> vData, int index_);

private:///一些与信号处理无关的公共函数，
	///转化单精度数组为双精度数组
	int FaToDa(const float *pfInput_, double * pdOutput_,int ilen_);
	///转化双精度数组为单精度数组
	int DaToFa(const double *pfInput_, float * pfOutput_,int ilen_);
};
