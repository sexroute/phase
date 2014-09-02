close all;clc;clear all;

N=256;%现在固定在个数据点数
t=-N+1:N+1;
f1=19.3;
a1=2.90;
a2=0;
a3=0;
a4=0;
ph1=3.1;
ph2=0;
ph3=0;
ph4=0;
ComputeIndex = 2;
y= a1*cos(2*pi*t*f1/N+ph1*pi/180)   +a2*cos(2*pi*t*f1*2/N+ph2*pi/180)   +a3*cos(2*pi*t*f1*3/N+ph3*pi/180)   +a4*cos(2*pi*t*f1*0.5/N+ph4*pi/180);
y1 = y(N:2*N-1);    
win = hanning(N)';
win1 = win/sum(win);    %窗归一   用于把加权求和的窗进行归一化
y11 = y1.*win1;
y11_fft = fft(y11,N);%做N点的FFT运算
a1 = abs(y11_fft);  %FFT振幅谱
p1 = mod(phase(y11_fft)*180/pi,360);   %FFT相位谱 angle(v)也可以求  

%再进行apFFT进行运算
y2 = y(1:2*N-1);    %2N-1个输入数据
win = hanning(N)';
winn = conv(win,win);   %apFFT须要卷积窗
win2 = winn/sum(winn);  %窗归一
y22 = y2.*win2;
y222 = y22(N:end)+[0 y22(1:N-1)];   %构成长N的apFFT输入数据  求和
y2_fft = fft(y222,N);
a2 = abs(y2_fft);   %apFFT振幅谱
p2 = mod(phase(y2_fft)*180/pi,360); 
                                                                        
%校正量的计算  
ee = mod((p1-p2)/180/(1-1/N),1);    %频率偏离矫正值  
aa = (a1.^2)./a2*2; %振幅校正值  为什么要乘以2
    
subplot(5,1,1);stem(a1,'.');title('FFT amplitude spectrum');ylim([0,1]);xlim([0 N/2]); grid
subplot(5,1,2);stem(a2,'.');title('apFFT amplitude spectrum');ylim([0,1]);xlim([0 N/2]); grid
subplot(5,1,3);stem(p2,'.');title('apFFT phase spectrum');ylim([0,400]);xlim([0 N/2]); grid
subplot(5,1,4);stem(ee,'.');title('frequency correction spectrum');ylim([-1,1]);xlim([0 N/2]); grid
subplot(5,1,5);stem(aa,'.');title('amplitude correction spectrum');ylim([0,1]);xlim([0 N/2]); grid
disp('频率原始值')
Index=round(f1*ComputeIndex)%求谱线个数
disp('频率校正值')
Freq=floor(f1*ComputeIndex)+ee(Index+1)
disp('振幅校正值')
Amp=aa(Index+1)
disp('初相位校正值')
Phase=p2(Index+1)




