function [mod_f,mod_a,mod_p]=FK_apfft_afreqNoLowFreq(y,fs,Fr,ifplot)
%
% fft/apfft时移相位差校正法+全频法低频部分频谱校正算法
%
% 输入：
% y：待分析信号
% fs：采样频率
% Fr：待校正的频率，可以是多个频率写成向量形式
% ifreal：是否是实信号
% ifplot: 是否绘制全相位频谱图
% 输出：
% mod_f: 校正后频率
% mod_a: 校正后幅值
% mod_p: 校正后相位
%
%默认使用hanning 窗
%
% 2009-12-3 by qinqiang
%     
% FengKun: 2010-1-27 10:14:43
%
%fft/apfft时移相位差法优点：
%1、可以直接生成无需校正的振幅谱和相位谱，以及所有离散谱线的频率偏离校正值
%2、可以分辨相对较密集的多个频率成分
%3、程序简单
%

bei=2;  %若是实信号，最后求出的幅值要乘2

NFFT=floor((length(y)+1)/2);
Deltaf=fs/NFFT;

y1 = y(NFFT:2*NFFT-1);%后N个输入数据
win =  hanning(NFFT)';
win1 = win/sum(win);%窗归1
y1= y1.*win1;
y1_fft = fft(y1,NFFT);  %%% FFT Cal No1 of 2
a1 = abs(y1_fft);%FFT振幅谱
p1 = mod(phase(y1_fft)*180/pi,360);%FFT相位谱


y2 = y(1:2*NFFT-1);%后N个输入数据
winn =  conv(win,win);%apFFT须要卷积窗  卷积
win2 = winn/sum(winn);%窗归1
y2= y2.*win2;
y2=y2(NFFT:end)+[0 y2(1:NFFT-1)];%构成长N的apFFT输入数据
y2_fft = fft(y2,NFFT);  %%% FFT Cal No2 of 2
a2 = abs(y2_fft);%apFFT振幅谱 
p2=mod( phase(y2_fft)*180/pi,360);%apFFT相位谱
df=mod((p1-p2)/180/(1-1/NFFT),1);%频率偏离校正值


aa=(a1.^2)./(a2+eps)*bei;  %振幅校正值
%%%默认最后生成的各校正谱的第一根谱线表示原始信号直流成分
aa(1)=a1(1)*2/NFFT*bei;
p2(1)=0;
df(1)=0;
%%%%默认最后生成的各校正谱的第二根谱线表示原始信号中的超低频成分
% [a_2,f_2,p_2]=dipinjz2(y(1:round(3*NFFT/4)),fs);%对信号中的超低频成分进行估计
% aa(2)=a_2;
% df(2)=mod(f_2-fs/NFFT,1);
% p(2)=p_2;
%%%%%%%%%%%%%%%%%%%%%%%%%%



r=round(Fr/fs*NFFT)+1;
r1=floor(Fr/fs*NFFT)+1;
%目标频率的频率偏离校正值      
mod_f=(r1-1+df(r))*fs/NFFT;
 
      
      %%%%%%%%%%%%%%%%%
      for i=1:length(Fr)%%%%%%%%假设分辨率是2Hz时，对于观察频率Fr=6.1,实际频率等于5.9的情况
                        %%%%%%%%算出来df=1.9,校正频率却等于floor(Fr/2)*2+df=7.9
                        %%%%%%%%这是因为实际频率为5.9，原理上该程序会默认频率在4～6之间，即算出df=1.5后正好
                        %%%%%%%%应该为4+1.5=5.9，但是由于人为的关系，将观测频率设为Fr=6.1，即告知程序
                        %%%%%%%%频率大概在6～8Hz之间，算出df=1.9,得到错误的校正频率等于floor(Fr/2)*2+df=7.9
                        %%%%%%%%这里加入一段判断法则避免这种情况出现
          dmf=mod_f(i)-Fr(i);
          if abs(dmf)>0.5*fs/NFFT
              mod_f(i)=mod_f(i)-sign(dmf)*fs/NFFT;
          end
      end
      
      
%目标频率的振幅校正值;
      mod_a=aa(r);
%目标频率的初相位校正值
      mod_p=p2(r);                                             %数据中点初相位
      mod_p=mod_p-mod(2*pi*mod_f*(NFFT-1)/fs*180/pi,360)+90;   %数据初始点初相位
      mod_p=mod(mod_p,360);
      
for i=1:length(Fr)  %特殊处理低频部分
    if Fr(i)<2*Deltaf
        mod_f(i)=Deltaf;mod_a(i)=aa(2);
        mod_p(i)=0;
    end
end
       
 %绘制运算过程图谱     
if ifplot==1
    figure;
subplot(5,1,1);stem(a1,'.');title('FFT幅值谱');ylim([0,max(a1)+0.2]);xlim([0 NFFT]);grid
subplot(5,1,2),stem(a2,'.');title('apFFT全相位幅值谱');ylim([0,max(a2)+0.2]);xlim([0 NFFT]);grid
subplot(5,1,3),stem(p2,'.');title('apFFT全相位相位谱(数据中点处相位)');ylim([0,400]);xlim([0 NFFT]);grid
subplot(5,1,4);stem(df,'.');title('频率偏差谱');ylim([0,1]);xlim([0 NFFT]);grid
subplot(5,1,5);stem(aa,'.');title('校正幅值谱');ylim([0,max(y)+1]);xlim([0 NFFT]);grid
end
