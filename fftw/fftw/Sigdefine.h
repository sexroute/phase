#pragma  once

#ifdef SIGMATH_EXPORTS
#define CLASS_DLL_DSE_SigMath  __declspec(dllexport)
#else
#define CLASS_DLL_DSE_SigMath  __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"sigmathd.lib")
#else
#pragma comment(lib,"sigmath.lib")
#endif  
#endif


#include <vector>
#include <map>
#include <complex>
using namespace std;


#define PI 3.1415926

//-----------------------------------------------------------------------//
/** \struct  S_BearingPar
* \brief  滚动轴承参数结构体。计算滚动轴承故障（通过）频率时，需要的参数；
*/
struct S_BearingPar
{
	float   fRollingDiam;          /*! 滚动轴承滚动体（滚珠）的直径，单位：mm.*/
	float   fContackAngle;         /*! 滚动轴承的压力角（又称接触角），单位：DEG.*/
	int     nNumofRolling;         /*! 滚动轴承滚动体的个数.*/
	float   fPitchDiam;            /*! 滚动轴承的节径，单位：mm.*/
};

//-----------------------------------------------------------------------//
/** \struct  S_BearingDefectFreq
* \brief  滚动轴承故障（通过）频率结构体。包括滚动轴承的各种故障频率。
*/
struct S_BearingDefectFreq
{
	float   fInnerFreq;             /*! 滚动轴承内圈通过频率，单位：Hz.*/        
	float   fRollingFreq;           /*! 滚动轴承滚动体（滚珠）的通过频率，单位：Hz.*/
	float   fOuterFreq;             /*! 滚动轴承外圈通过频率，单位：Hz.*/
	float   fCageFreq;              /*! 滚动轴承保持架的通过频率，单位：Hz.*/
};


///滤波器多项式的系数
class CFilterPolyCoef
{
public:
	CFilterPolyCoef()
	{
		_iOrder = 0;
		for (int i= 0; i< 5; ++i)
		{
			_fg[i] = .0;
			_fh[i] = .0;
		}
	}
public:
	///分母系数
	float _fh[5];
	///分子系数
	float _fg[5];
	///多项式的阶数
	int _iOrder;
};

///信号参数结构体
struct SSigParam
{
	SSigParam()
	{
		_fFreq = 0;
		_fAmp  = 0;
		_fPhase= 0;
	}
	///信号频率
	double _fFreq; 
	///信号幅值
	double _fAmp;
	///信号相位
	double _fPhase;
};

#define DSE_DataIsMiddle(a,low,high)  ( ((a)>(low)) && ((a)<(high)) )
#define DSE_DeletePoint(pt)          { if(0 != (pt)) { delete [] (pt); (pt)= 0;} } 

///用于函数执行，如果错误就返回整形的情况
#define BH_FunExec_ErrorReturnInt(rs) { if( (rs) < 0 ) return (rs); }


/**返回值的说明
  * 0 : 成功
  *-1 ：参数有误
  *-2 ：
  */
enum E_DseSigRet
{
	DSESIG_SUCC = 0,
	DSESIG_ERR_PARAM = -1,         ///输入参数有误
	DSESIG_ERR_VALUE_BEYOND = -2,  ///值越界
	DSESIG_ERR_SUBFUN      = -3,   ///子函数内部计算错误
	DSESIG_ERR_OUTPARAM = -4,      ///需要输出的参数有误
	DSESIG_ERR_FILTER   = -5,      ///滤波出错
	DSESIG_ERR_FFT      = -6,      ///FFT出错
	DSESIG_ERR_UNKNOWN = -10
};

/** 进行FFT之前对时间序列加窗，定义窗的枚举变量.*/
enum E_WINOW_TYPE
{
	WINDOW_NO,              /*!< not window.*/
	WINDOW_HANNING,         /*!< hanning.汉宁窗*/
	WINDOW_HAMMING,         /*!< hamming.汉明窗*/
	WINDOW_BLACKMANHARRIS,  /*!< Blackman-Harris。布莱克曼-哈里斯*/
	WINDOW_EXTRACTBLACKMAN, /*!< Exact Blackman*/
	WINDOW_BLACKMAN,        /*!< blackman.*/
	WINDOW_FLATTOP,         /*!< Flat Top*/
	WINDOW_BLACKMANHARRIS4, /*!< blackmanharris 4-term.*/
	WINDOW_BLACKMANHARRIS7, /*!< blackmanharris 7-term.*/
    WINDOW_LowSidelobe      /*!< Low Sidelobe*/
};

/**轴心轨迹形状枚举定义,Class={1 2 3 4 5}={圆/椭圆,香蕉，内8，外8，杂乱}*/
enum E_OrbitShape
{
   OrbitShape_Unknown = 0, ///未知
   OrbitShape_Circle  = 1,  ///圆
   OrbitShape_Ellipse = 1,  ///椭圆
   OrbitShape_Banana  = 2,  ///香蕉
   OrbitShape_In8     = 3,  ///内8
   OrbitShape_Out8    = 4,  ///外8
   OrbitShape_Disorder= 5  ///杂乱   
};
///特征类型枚举定义
enum E_FeatureValueType
{
    E_FeatureValueType_RMS = 0,
	E_FeatureValueType_Effective  = 1,      ///峰值(有效峰值)
    E_FeatureValueType_PeakPeak_Effective = 2,  ///峰峰值(有效峰峰值)
	E_FeatureValueType_Kur = 3,
	E_FeatureValueType_Skew = 4,
	E_FeatureValueType_Peak_Statistic  = 5,     ///统计意义真峰值
	E_FeatureValueType_PeakPeak_Statistic = 6,  ///统计意义真峰峰值
	E_FeatureValueType_AVERAGE_P       = 7,     ///平均幅值
	E_FeatureValueType_Peak_Direct     = 8,  ///直接计算真峰值
	E_FeatureValueType_PeakPeak_Direct = 9   ///直接计算真峰峰值
};

/** 频谱分析类型的枚举. */
enum E_SpectrumType
{
	E_SpectrumType_PEAK = 0,        /*峰值谱*/
	E_SpectrumType_POWER,           /*功率(峰值)谱*/
    E_SpectrumType_Peak_Peak,       /*峰-峰值谱*/
	E_SpectrumType_RMS,             /*有效值谱*/
	E_SpectrumType_POWER_RMS,       /*功率（有效值）谱*/
};

///转轴或活塞杆径向传感器的安装方式
enum E_AxisSensorMounting
{
	E_AxisSensorMounting_LeftTop = 0,       ///左上
    E_AxisSensorMounting_RightTop= 1,       ///右上
	E_AxisSensorMounting_TopTop  = 2,       ///上上
	E_AxisSensorMounting_BottomBottom = 3,  ///下下
    E_AxisSensorMounting_LeftBottom = 4,    ///左下
    E_AxisSensorMounting_RightBottom= 5,    ///右下
	E_AxisSensorMounting_Unknown =10        ///未知
};


/**轴心位置或活塞杆位置计算输入结构体参数。
  *垂直水平的定义（下面左右判断的前提：站驱动端面向做功端看。水平方向为x，右侧为正向，垂直方向为y，上方为正向）：
  *    1、如果是垂直地面和平行地面安装，则根据实际情况即可。
  *    2、如果是上上安装或者下下安装方式，则是以顺时针方向旋转，先遇到的测点为垂直V，另外一个测点即为水平
  *
  */
struct S_AxisLocationInputInfo
{
    S_AxisLocationInputInfo()
	{
		fRadius = .0;
		fGapVolt_V = .0;
		fGapVolt_H = .0;
		fDC_H = .0;
		fDC_V = .0;
		fSensitivity = 8.0;
		sensorMounting = E_AxisSensorMounting_TopTop;
	}

	float   fRadius ;			 /*轴或者活塞杆半径, 单位:mm*/
	float   fGapVolt_V;			 /*垂直初始间隙电压，单位：V.*/
	float   fGapVolt_H;		     /*水平初始间隙电压，单位：V.*/
	float   fDC_V;			     /*垂直当前直流量，单位：V.*/        
	float   fDC_H;			     /*水平当前直流量，单位：V.*/
	float   fSensitivity;        //灵敏度。单位：V/mm.
	E_AxisSensorMounting sensorMounting;//安装方式
};

///信号压缩后的输出参数信息。不支持多线程
class CCompressedParamInfo
{
public:
	CCompressedParamInfo()
	{
         _ilevel = 5;
	}

	~CCompressedParamInfo()
	{
      _vCompressedWpCoef.clear();
	  _vCompressedWpCoefIdx.clear();
	  _vBasisTreeIdx.clear();
	}
	void SetLevel(int level = 5){_ilevel = level;}
	const int GetLevel() const {return _ilevel;}
	const vector<double> GetCompressedWpCoef() const{return _vCompressedWpCoef;}
	const vector<unsigned int> GetCompressedWpCoefIdx() const{return _vCompressedWpCoefIdx;}
	void SetCompressedWpCoefIdx(const vector<unsigned int>& vCompressedWpcoefIdx_) {
		_vCompressedWpCoefIdx.clear();
		_vCompressedWpCoefIdx.insert(_vCompressedWpCoefIdx.begin(),vCompressedWpcoefIdx_.begin(),vCompressedWpcoefIdx_.end());
		}
	void SetCompressedWpCoef(const vector<double>& vCompressedWpcoef_) {
		_vCompressedWpCoef.clear();
		_vCompressedWpCoef.insert(_vCompressedWpCoef.begin(),vCompressedWpcoef_.begin(),vCompressedWpcoef_.end());
	}
	const vector<unsigned int> GetCompressedBasisTreeIdx() const{return _vBasisTreeIdx;}
	void SetCompressedBasisTreeIdx(const vector<unsigned int>& vCompressedBasisTreeIdx_) {
		_vBasisTreeIdx.clear();
		_vBasisTreeIdx.insert(_vBasisTreeIdx.begin(),vCompressedBasisTreeIdx_.begin(),vCompressedBasisTreeIdx_.end());
	}
private:
	int _ilevel;
	vector<double> _vCompressedWpCoef;
	vector<unsigned int> _vCompressedWpCoefIdx;
	vector<unsigned int> _vBasisTreeIdx;///最优树为1值的索引
};