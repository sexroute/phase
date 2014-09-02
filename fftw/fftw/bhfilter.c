
float CalcgSESpectrum(float *pwave_,int iLen_,int iCutoffFreq_,int iSmpFreq_,float *pfOut_)
{//高通滤波-〉包络(hilbert 变换+求模)->频谱分析
	int nloop=0;
	vector<float*> pfa,pfb;
	pfa.clear();
	pfb.clear();
	
	int iFilterOrder=3;
	int iRtn =getFilterCoefandOrder(iSmpFreq_, iCutoffFreq_, iFilterOrder, pfa, pfb) ;
	if (iRtn < 0)
		return -1.0;
	
	float *pOut=new float[iLen_];
	memset(pOut,0,iLen_);
	_mathAnalysis.filterIIR_C2(pwave_,iLen_,&(pfb[0]),&(pfa[0]),pOut,iFilterOrder);
	_mathAnalysis.Evenlope(pOut,iLen_,pOut);
	writefile(pOut,iLen_,10);
	
	//修改：如果没有缺陷频率，就按照包络后波形的峰峰值计算 mb 2008.12.4
	float fgSE = _mathAnalysis.CalcPeak_PeakValueTrue(pOut,iLen_);
    ///增加：在做频谱之前去直流(09.4.28)
	float fMean = _mathAnalysis.mean(pOut,iLen_);
	for (int loop = 0; loop < iLen_; ++loop)
            pOut[loop] = pOut[loop] - fMean;

	SpectralMeasureMent(pOut,iLen_,pfOut_);

    pfa.clear();
	pfb.clear();
	delete [] pOut;
	return fgSE;//DistillgSEFromSpectrum(pfOut_,iLen_,iSmpFreq_,NULL,0);
}

int g_iOrder_100Fs100K=4;//100K denotes 102400
  float g_fCa_100Fs100K[4][2]={-0.987160,0,\
                               -1.992610,0.992687,\
    						   -1.998382,0.998432,\
							   -1.999661,0.999705};
  float g_fCb_100Fs100K[4][3]={0.989003,-0.989003,0,\
	                           1.000000,-1.999986,1.000000,\
	                           1.000000,-1.999968,1.000000,\
							   1.000000,-1.999962,1.000000};  
 ///
  int g_iOrder_200Fs100K=4;
  float g_fCa_200Fs100K[4][2]={-1.969649,0.969881,\
	                           -1.993650,0.993850,\
	                           -1.998405,0.998563,\
	                           -1.999573,0.999722};
  float g_fCb_200Fs100K[4][3]={0.957464,-1.914914,0.957464,\
	                           1.000000,-1.999917,1.000000,\
	                           1.000000,-1.999875,1.000000,\
	                           1.000000,-1.999862,1.000000};
 ///
  int g_iOrder_500Fs100K=5;
  float g_fCa_500Fs100K[5][2]={-1.922772,0.925362,\
	                           -1.983183,0.984459,\
	                           -1.995284,0.996297,\
	                           -1.998110,0.999063,\
	                           -1.998877,0.999815};
  float g_fCb_500Fs100K[5][3]={0.929256,-1.858424,0.929256,\
	                           1.000000,-1.999463,1.000000,\
	                           1.000000,-1.999195,1.000000,\
	                           1.000000,-1.999105,1.000000,\
	                           1.000000,-1.999081,1.000000};
 ///
  int g_iOrder_1KFs100K=6;
  float g_fCa_1KFs100K[6][2]={-0.885221,0.000000,\
	                          -1.926550,0.933205,\
	                          -1.980693,0.985087,\
	                          -1.992348,0.996255,\
	                          -1.995250,0.999037,\
	                          -1.996051,0.999809};
  float g_fCb_1KFs100K[6][3]={0.900356,-0.900356,0.000000,\
	                          1.000000,-1.998738,1.000000,\
	                          1.000000,-1.997177,1.000000,\
	                          1.000000,-1.996527,1.000000,\
	                          1.000000,-1.996332,1.000000,\
	                          1.000000,-1.996282,1.000000};
 ///
  int g_iOrder_2KFs100K=6;
  float g_fCa_2KFs100K[6][2]={-1.695405,0.732688,\
 	                          -1.919173,0.939238,\
 	                          -1.969007,0.985237,\
	                          -1.980866,0.996184,\
	                          -1.983927,0.999011,\
	                          -1.984776,0.999804};
  float g_fCb_2KFs100K[6][3]={0.800066,-1.598900,0.800066,\
	                          1.000000,-1.991339,1.000000,\
	                          1.000000,-1.987024,1.000000,\
	                          1.000000,-1.985567,1.000000,\
	                          1.000000,-1.985156,1.000000,\
	                          1.000000,-1.985052,1.000000};
 ///
  int g_iOrder_5KFs100K=7;
  float g_fCa_5KFs100K[7][2]={-0.529817,0.000000,\
	                          -1.569048,0.713258,\
	                          -1.822864,0.928777,\
	                          -1.885132,0.981650,\
	                          -1.901060,0.995176,\
	                          -1.905254,0.998744,\
							  -1.906410,0.999751};
  float g_fCb_5KFs100K[7][3]={0.594438,-0.594438,0.000000,\
	                          1.000000,-1.968092,1.000000,\
	                          1.000000,-1.929285,1.000000,\
	                          1.000000,-1.913326,1.000000,\
	                          1.000000,-1.908539,1.000000,\
	                          1.000000,-1.907232,1.000000,\
	                          1.000000,-1.906905,1.000000};


  //25K denotes 25600
  int g_iOrder_100Fs25K=3;//25K denotes  100指截止频率，25K指采样频率为25600;
  float g_fCa_100Fs25K[3][2]={-0.96305174,0,\
                               -1.9798053254660888,0.98055080807528705,\
    						   -1.9965234228632363,0.99708907445327633};
  float g_fCb_100Fs25K[3][3]={0.97039959005808873,-0.97039957660846865,0,\
	                           1.000000,-1.9997824995335876,1.000000,\
	                           1.000000,-1.9995850562025161,1.000000};
 ///
  int g_iOrder_200Fs25K=3;
 // float g_fCa_200Fs25K[3][2]={-1.9022224554746434,0.90669422267973965,\
	                           -1.9802141016233712,0.98287028383580699,\
	                           -1.9949518019039867,0.9729111256714509};
 // float g_fCb_200Fs25K[3][3]={0.91865583064960576, -1.8370269733916205, 0.91865584684272872,\
	                           1.000000, -1.9984808715122599, 0.99999994083517063,\
	                           1.000000, -1.997968348905391, 1.000000};

  float g_fCa_200Fs25K[3][2]={-1.9022288270690466,0.90669975027045357,\
	                          -1.9802134372548563,0.98286950178154475,\
	                          -1.9949515987841422,0.99729088539330768};
  float g_fCb_200Fs25K[3][3]={0.91865826427854247, -1.8370318397970908, 0.91865827526736688,\
	                          1.000000, -1.998480880611796, 0.99999995983336265,\
	                          1.000000, -1.9979683485364563, 1.000000};


 ///
  int g_iOrder_500Fs25K=4;
  float g_fCa_500Fs25K[4][2]={-0.81764432179642432, 0,\
	                           -1.8801689683039409, 0.90034411977761009,\
	                           -1.9652996386800519, 0.98094342290884928,\
	                           -1.9820577343648993, 0.9968844944445403};
  float g_fCb_500Fs25K[4][3]={0.84980095630709507, -0.84980094800801442, 0,\
	                           1.000000, -1.9937724443171083, 1.000000,\
	                           1.000000, -1.9878073541052106, 1.000000,\
	                           1.000000, -1.9860837449681763, 1.000000};
 ///
  int g_iOrder_1KFs25K=4;
  float g_fCa_1KFs25K[4][2]={-0.6647095606882194,0.000000,\
	                          -1.7359147832732704,0.81223015539313681,\
	                          -1.9008198643559036,0.96254591011826884,\
	                          -1.9348225156072121,0.99382444707848194};
  float g_fCb_1KFs25K[4][3]={0.72351382706973899,-0.72351382744355397,0.000000,\
	                          1.000000,-1.9750183799637675,1.000000,\
	                          1.000000,-1.9513092462298145,1.000000,\
	                          1.000000,-1.9444973158308896,1.000000};
 ///
  int g_iOrder_2KFs25K=4;
  float g_fCa_2KFs25K[4][2]={-1.0589187803397906, 0.37395014917998493,\
 	                          -1.5894937728410803, 0.84292839556797139,\
 	                          -1.7298686094393907, 0.96710628266081911,\
	                          -1.7602859661655614, 0.99449600928634585};
  float g_fCb_2KFs25K[4][3]={0.51995058336452005,-1.0222558328363058,0.51995058336660283,\
	                          1.000000,-1.8385426681793455,1.000000,\
	                          1.000000,-1.7843653096035523,1.000000,\
	                          1.000000,-1.7716491492252575,1.000000};
 ///
  int g_iOrder_5KFs25K=5;
  float g_fCa_5KFs25K[5][2]={0.076393739135264385, 0.000000,\
	                          -0.24000936814365559, 0.47475413254318849,\
	                          -0.5649101450657501, 0.86266544776118426,\
	                          -0.65471222528463358, 0.97013150414179827,\
	                          -0.67445515401621914, 0.994957828777144};
  float g_fCb_5KFs25K[5][3]={0.19750199594930387,-0.19750199594930901,0.000000,\
	                          1.000000,-1.3044117593466225,1.000000,\
	                          1.000000,-0.83036926873328731,1.000000,\
	                          1.000000,-0.71256536000843118,1.000000,\
	                          1.000000,-0.68821578726731769,1.000000};


int getFilterCoefandOrder(const int& iFs_, const int& iCutoffFreq_, int& iFilterOrders_, vector<float*>& pfA_, vector<float*>& pfB_)
{


	iFilterOrders_ = 0;
    int idx = 0;
	pfA_.clear();
	pfB_.clear();
	if (iFs_ == 25600)
	{
		switch(iCutoffFreq_)
		{
		case 100:
			iFilterOrders_ = g_iOrder_100Fs25K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_100Fs25K[idx];
				pfB_[idx] = g_fCb_100Fs25K[idx];
			}
			break;
		case 200:
			iFilterOrders_ = g_iOrder_200Fs25K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_200Fs25K[idx];
				pfB_[idx] = g_fCb_200Fs25K[idx];
			}
			break;
		case 500:
			iFilterOrders_ = g_iOrder_500Fs25K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_500Fs25K[idx];
				pfB_[idx] = g_fCb_500Fs25K[idx];
			}
			break;
		case 1000:
			iFilterOrders_ = g_iOrder_1KFs25K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_1KFs25K[idx];
				pfB_[idx] = g_fCb_1KFs25K[idx];
			}
			break;
		case 2000:
			iFilterOrders_ = g_iOrder_2KFs25K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_2KFs25K[idx];
				pfB_[idx] = g_fCb_2KFs25K[idx];
			}
			break;
		case 5000:
			iFilterOrders_ = g_iOrder_5KFs25K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_5KFs25K[idx];
				pfB_[idx] = g_fCb_5KFs25K[idx];
			}
			break;
		default:
			return -1;
		}
	}
	else if(iFs_ == 102400)
	{
		switch(iCutoffFreq_)
		{
		case 100:
			iFilterOrders_ = g_iOrder_100Fs100K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_100Fs100K[idx];
				pfB_[idx] = g_fCb_100Fs100K[idx];
			}
			break;
		case 200:
			iFilterOrders_ = g_iOrder_200Fs100K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_200Fs100K[idx];
				pfB_[idx] = g_fCb_200Fs100K[idx];
			}
			break;
		case 500:
			iFilterOrders_ = g_iOrder_500Fs100K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_500Fs100K[idx];
				pfB_[idx] = g_fCb_500Fs100K[idx];
			}
			break;
		case 1000:
			iFilterOrders_ = g_iOrder_1KFs100K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_1KFs100K[idx];
				pfB_[idx] = g_fCb_1KFs100K[idx];
			}
			break;
		case 2000:
			iFilterOrders_ = g_iOrder_2KFs100K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_2KFs100K[idx];
				pfB_[idx] = g_fCb_2KFs100K[idx];
			}
			break;
		case 5000:
			iFilterOrders_ = g_iOrder_5KFs100K;
			pfA_.assign(iFilterOrders_, NULL);
			pfB_.assign(iFilterOrders_, NULL);
			for (idx = 0; idx < iFilterOrders_; ++idx)
			{
				pfA_[idx] = g_fCa_5KFs100K[idx];
				pfB_[idx] = g_fCb_5KFs100K[idx];
			}
			break;
		default:
			return -1;
		}
	}
	else
		return -1;
	return 0;
}


bool CMathAnalysis::filterIIR_C2(float *pfWave_,int iWaveLen_,float **ppfB_,float **ppfA_,float *pfOut_,int iNs_)
{
	/**    
	DirectII
	%             ns        b(0k)+b(1k)z^(-1)+ b(2k)z^(-2)
	%    H(z)=∏ -------------------------------------------------
	%            k=1       1.0 +a(1k)z^(-1)+ a(2k)z^(-2)
	% 
	%s[i]=x[i]-a1*s[i-1]-a2*s[i-2] 
	%y[i]=b0*s[i]+b1*s[i-1]+b2*s[i-2]
	% ——  ——  ——  —— Input ——  ——  ——  —— '
	n=2;
	% n，整型常量，为每个子系统的阶次，一般为2 
	% ns，整型常量，为子系统个数
	% 如果阶次为偶数Order＝ns×n；如果为奇数Order=ns×n-1;
	% a，矩阵，大小ns×n，a(i,j)表示第i个子系统的第j个系数
	*/
	int nloop=0,mloop=0;
	float *pfStage=new float[iWaveLen_];
	float *pfStages=new float[iWaveLen_];
	memset(pfStage,0,iWaveLen_);
	memset(pfStages,0,iWaveLen_);
	/*	for (nloop=0;nloop<iWaveLen_;++nloop){
	pfOut_[nloop]=pfWave_[nloop];
	}*/
	memcpy(pfOut_,pfWave_,sizeof(float)*iWaveLen_);
	for (nloop=0;nloop<iNs_;++nloop) 
	{
		pfStages[0]=pfOut_[0];
		pfStages[1]=pfOut_[1]-ppfA_[nloop][0]*pfStages[0];
		pfStage[0]=(ppfB_[nloop][0]*pfStages[0]);
		pfStage[1]=(ppfB_[nloop][0]*pfStages[1]+ppfB_[nloop][1]*pfStages[0]);
		for (mloop=2;mloop<iWaveLen_;++mloop)
		{
			pfStages[mloop]=pfOut_[mloop]-ppfA_[nloop][0]*pfStages[mloop-1]-ppfA_[nloop][1]*pfStages[mloop-2];
			pfStage[mloop]=(ppfB_[nloop][0]*pfStages[mloop]+ppfB_[nloop][1]*pfStages[mloop-1]+ppfB_[nloop][2]*pfStages[mloop-2]);
		}
		memcpy(pfOut_,pfStage,sizeof(float)*iWaveLen_);
	}
#ifdef _DEBUG
	//	writefile(pfOut_,iWaveLen_,nloop);
#endif
	delete [] pfStage;
	delete [] pfStages;
	return true;
}


void CMathAnalysis::Evenlope(const float *pwave,int n,float *pout)
{
	float *preal,*pimag;
	int n2;

	n2=n/2;
	preal=new float[n];
	pimag=new float[n];
	memcpy(preal,pwave,n*sizeof(float));
	memset(pimag,0,n*sizeof(float));

	Hilbert(preal,pimag,n);
	//求模
	for(int loopn=0;loopn<n;++loopn)
		pout[loopn]=sqrt(preal[loopn]*preal[loopn]+pimag[loopn]*pimag[loopn]);     
	delete pimag;
	delete preal;
}


float CMathAnalysis::CalcPeak_PeakValueTrue(const float *pfWave_,int iLen_)
{
	//使用统计手段求通频峰峰值
	//将信号均分成n (数据长度为2^n) 段，求出每N段的峰峰值，
	//然后去除N段中>3σ的点再求平均值
	if(NULL == pfWave_ || iLen_ < 6)
		return -1;

	int ilen=iLen_>0?iLen_:1;
	int iCycle;//数据总共分的段数
	float ftmp=ilen;
	ftmp=log(ftmp)/log(2.0);


	iCycle=ftmp>1.0?ftmp:1;
	int iCodeletLen=ilen/iCycle;
	int iRemainLen=ilen-iCodeletLen*iCycle;
	int iloop=0;

	float fMax=.0,fMin=.0;
	vector<float> fPP;
	fPP.assign(iCycle+1,0);
	

	for(iloop=0;iloop<iCycle;++iloop)
	{
		fMin=CalcMaxMinValue(&(pfWave_[iloop*iCodeletLen]),iCodeletLen,fMax);
		fPP[iloop]=fMax-fMin;
	}

	ftmp=(float)iRemainLen/ilen;
	if (ftmp>0.02)
	{//如果剩余长度不足总长度的2%，则略去
		fMin=CalcMaxMinValue(&(pfWave_[iCycle*iCodeletLen]),iRemainLen,fMax);
		fPP[iCycle]=fMax-fMin;
		++iCycle;
	}  
	
	float fDelta=CalcStd(&(fPP[0]),iCycle);
	float fLevel=mean(&(fPP[0]),iCycle)+3*fDelta;

	fDelta=0;
	iRemainLen=0;
	for(iloop=0;iloop<iCycle;++iloop)
	{
		if(fPP[iloop]<=fLevel)
		{//把满足要求的点加起来，除以长度就是峰峰值
			fDelta+=fPP[iloop];
			++iRemainLen;
		}
	}
	fPP.clear();
	if(iRemainLen>0)
		return fDelta/iRemainLen;
	else
		return 0;
}


void  CMathAnalysis::Hilbert(float *preal,float *pimag,int n)
{
	int n2;

	Fft2_DIT(preal,pimag,n,0);

	n2=n/2;
	//
	for(int loopn=1;loopn<n2;++loopn)
	{
		preal[loopn]=preal[loopn]*2;
		pimag[loopn]=pimag[loopn]*2;
		preal[loopn+n2]=0;
		pimag[loopn+n2]=0;
	}
	//
	Fft2_DIT(preal,pimag,n,1);
}


///进行DFT，基于时间抽取的
int  CMathAnalysis::Fft2_DIT(float *pr,float *pi,int n,int nflag)
{
	//float *pf_imagine,*pf_real;
	int m,j,n1,k,n2,p,q;
	float fxt,fyt,fee,fsin,fcos,fa,fxx,fyy,ftype;

	m=CalcLog2(n);
	/*pf_imagine=new float[n];
	pf_real=new float[n];
	memcpy(pf_real,pwave,n*sizeof(float));
	memset(pf_imagine,0,n*sizeof(float));*/
	//inverse bits
	j=0;
	n1=n-1;
	n2=n/2;
	for(int i=0;i<n1;++i)
	{
		if(i<j)
		{
			fxt=pr[j];
			pr[j]=pr[i];
			pr[i]=fxt;

			fxt=pi[j];
			pi[j]=pi[i];
			pi[i]=fxt;

		}
		k=n2;
		while(k<=j)
		{
			j=j-k;
			k=k/2;
		}
		j=j+k;
	}
	//calculate FFT	
	if(nflag==0)
		ftype=-2*PI;
	else
		ftype=2*PI;
	for(k=1;k<=m;++k)
	{
		n2=pow(2,k-1);
		n1=pow(2,k);
		fee=ftype/n1;
		for(j=0;j<n2;++j)
		{
			fa=j*fee;
			fcos=cos(fa);
			fsin=sin(fa);
			for(p=j;p<n;p+=n1)
			{
				q=p+n2;

				fxt=pr[p];
				fyt=pi[p];

				fxx=pr[q]*fcos-pi[q]*fsin;
				fyy=pr[q]*fsin+pi[q]*fcos;

				pr[p]=fxt+fxx;
				pi[p]=fyt+fyy;
				pr[q]=fxt-fxx;
				pi[q]=fyt-fyy;
			}
		}
	}
	if(nflag==1)
	{
		for(int loopn=0;loopn<n;++loopn)
		{	
			pr[loopn]=pr[loopn]/n;
			pi[loopn]=pi[loopn]/n;
		}

	}
	return 0;
	//计算模，并进行标准化
	/*k=n/2;
	fxx=2.0/n;
	for(j=0;j<k;++j)
	{
	pout[j]=fxx*sqrt(pf_real[j]*pf_real[j]+pf_imagine[j]*pf_imagine[j]);
	}

	delete pf_imagine;
	delete pf_real;*/
}


void SpectralMeasureMent(float *pwave,int n,float *pout,int type) 
{
	 float ffactors; 
	 float *preal,*pimag;
	 int n2;
     int loopn=0;
	 n2=n/2;
	 preal=new float[n];
	 pimag=new float[n];
     ffactors=2.0/n;
	 memcpy(preal,pwave,n*sizeof(float));
	 memset(pimag,0,n*sizeof(float));
     
	 switch(type)
	 {
	 case POWERSPECTRAL:
		 /*功率谱为时间序列自相关后的频谱分析，即同等于序列进行fft后，然后与fft结果的共扼相乘的结果除以序列长度的平方，再乘以2*/
		 WindowedWave(preal,n,1);
		 _mathAnalysis.Fft2_DIT(preal,pimag,n,0);
		 ffactors=2.0/(n*n);
		 for(loopn=0;loopn<n2;++loopn)
			 pout[loopn]=ffactors*(preal[loopn]*preal[loopn]+pimag[loopn]*pimag[loopn]);     
		 break;
	 case PEAKSPECTRAL:
	 default:
         WindowedWave(preal,n,1);
		 _mathAnalysis.Fft2_DIT(preal,pimag,n,0);
		 for(loopn=0;loopn<n2;++loopn){
			 pout[loopn]=ffactors*sqrt(preal[loopn]*preal[loopn]+pimag[loopn]*pimag[loopn]);     
		 }
		 break;
	 }
	 delete preal;
	 delete pimag;
}


void WindowedWave(float *pwave, int n,int type)
{
	float  factor;
	int i;

	switch(type)
	{ 
	case WINDOW_HANNING:
		for(i=0;i<n;i++)
		{ /*noted(marble):应该是factor=(float)(0.5)*(1-cos(6.2831853*i/(n-1))),但是为了和labview里面一致，就去掉0.5.*/
			factor=(float)(1-cos(6.2831853*i/(n-1)));
			pwave[i]=pwave[i]*factor;
		}
		break;
	case WINDOW_HAMMING:
		for(i=0;i<n;i++)
		{   
			factor=(float)(0.54-0.46*cos(6.2831853*i/(n-1)));
			pwave[i]=pwave[i]*factor;
		}
		break;
	case WINDOW_BLACKMAN:
		for(i=0;i<n;i++)
		{   
			factor=(float)(0.42-0.5*cos(2*PI*i/(n-1))+0.08*cos(4*PI*i/(n-1)));
			pwave[i]=pwave[i]*factor;
		}
		break;
	case WINDOW_NO:
	default:
		break;
	}
}


