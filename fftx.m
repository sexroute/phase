clc;
clear all; close all
A1=1;   %频率F1信号的幅度
F1=49.8; %信号1频率(Hz)
Fs=3200; %采样频率(Hz)
P1=0; %信号1相位(度)
N=256; %采样点数
t=[0:1/Fs:N/Fs]; %采样时刻
%信号
S=A1*sin(2*pi*F1*t+pi*P1/180);
%显示原始信号
subplot(411);plot(S);
title('原始信号时域图');
Y = fft(S,N); %做FFT变换
Ayy = (abs(Y)); %取模
subplot(412);stem(Ayy(1:N)); %显示原始的FFT模值结果
title('FFT 模值'); xlim([0 50]);
Ayy=Ayy/(N/2);   %换算成实际的幅度
Ayy(1)=Ayy(1)/2;
F=([1:N]-1)*Fs/N; %换算成实际的频率值，Fn=(n-1)*Fs/N
subplot(413);stem(F(1:N/2),Ayy(1:N/2));   %显示换算后的FFT模值结果
title('幅度-频率曲线图');
Pyy=[1:N/2];
for i=1:N/2
Pyy(i)=angle(Y(i)); %计算相位
Pyy(i)=Pyy(i)*180/pi; %换算为角度
end;
subplot(414);stem(F(1:N/2),Pyy(1:N/2));   %显示相位图
title('相位-频率曲线图');
[Aym,Ayl]=max(Ayy(1:N/2));
fprintf('%5.6f   %5.6f   %5.6f   %4d\n',Ayy(Ayl-1),Ayy(Ayl),Ayy(Ayl+1),Ayl);
Z=Specorr(S,Fs,N,25,75,1);
fprintf('%5.6f   %5.6f   %5.6f\n',Z(1),Z(2),Z(3));
Z=Specorr(S,Fs,N,25,75,2);
fprintf('%5.6f   %5.6f   %5.6f\n',Z(1),Z(2),Z(3));