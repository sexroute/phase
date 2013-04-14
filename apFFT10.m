clc;clear;close all
N=128;
n=-N+1:N-1;
%fs=4e9;%采样频率，根据采样定理，大于2f
s=3*cos(10.0*2*n*pi/N+pi/18)+2*cos(20.1*2*n*pi/N+pi/6)+2*cos(30.2*2*n*pi/N+pi*5/18)+2*cos(40.3*2*n*pi/N+7*pi/18)+2*cos(50.4*2*n*pi/N+pi/2);%丢掉的1e9会在后面补充回来
%抽样信号定义块

y1=s(N:2*N-1);%N长的数据送入FFT处理
y2=s(1:2*N-1);%2N-1长的数据送入APFFT处理
win=hanning(N)'%汉宁窗,后面的一瞥为转换为列向量
win1=win/sum(win);%汉宁窗归一化
winn=conv(win,win);%汉宁窗的卷积
win2=winn/sum(winn);%汉宁窗卷积的归一化
%窗的定义块
 
y11_fft=fft(y1.*win,N);
a_fft=abs(y11_fft);%FFT幅频
p_fft= angle(y11_fft)*180/pi ;%FFT相频
%FFT的相频和幅频表达
%figure(5);
%stem(0:N-1,abs(fft(y1,N)),'.r');%看看FFT不加窗的情况

y22=y2.*win2;
y222=[0,y22(1:N-1)]+y22(N:end);%长度为N，送入FFT处理，出来为APFFT
y222_fft=fft(y222,N);
a_apfft=abs(y222_fft);%APFFT幅频
p_apfft=mod(phase(y222_fft)*180/pi,360);%APFFT相频
%APFFT的相频和幅频表达
%y222t=y2(N:end)+[0,y2(1:N-1)];
%figure(6);
%stem(0:N-1,abs(fft(y222t,N)),'.r');%看看APFFT不加窗的情况


%绘图程序块
t1=0:N-1;
t2=t1;%*fs/N;%横轴的定义

figure(1);
%a_fft1=a_fft/sum(a_fft);%将幅度归一化，使得不同N的幅频有可比性
stem(t2,a_fft,'.k');%FFT的幅频曲线
grid on;
title('FFT幅频曲线');
ylabel('幅值');
xlabel('频率（HZ）');
xlim([0,60]);
ylim([0,1.2]);
%axis([1.2e9,1.8e9,0,0.5]);
figure(2);
stem(t2,p_fft,'.k');%FFT的相频曲线
xlim([0,60]);
%ylim([-200,200]);
grid on;
title('FFT相频曲线');
ylabel('度（°）');
xlabel('频率（HZ）');

figure(3);
%a_apfft1=a_apfft/(sum(a_apfft)^2);
stem(t2,a_apfft,'.k');%APFFT的幅频曲线
grid on;
title('apFFT幅频曲线');
ylabel('幅值');
xlabel('频率（HZ）');
xlim([0,60]);
ylim([0,10]);
figure(4);
stem(t2,p_apfft,'.k');%APFFT的幅频曲线
xlim([0,60]);
ylim([0,100]);
grid on;
title('apFFT相频曲线');
ylabel('度（°）');
xlabel('频率（HZ）');


%数字处理程序块
[a,b]=max(a_fft);
a_fft(b)=0;
[c,d]=max(a_fft);
a_fft(d)=0;
e=max(a_fft);
display('FFT旁瓣主瓣比');
FFT_sidelobe=e/a%FFT的旁边与主瓣比值

[h,i]=max(a_apfft);
a_apfft(i)=0;
[j,k]=max(a_apfft);
a_apfft(k)=0;
l=max(a_apfft);
display('apFFT旁瓣主瓣比');
APFFT_sielobe=l/h





