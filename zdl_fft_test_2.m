clear;close all;clc
aoData =load('wave_test.txt');
anSize = length(aoData);
freq = 25.237263*1;
sampleFreq = 25600;

result1 = zdl_fft2(aoData,anSize,sampleFreq,freq);
N=anSize/2;
T = anSize/sampleFreq;
dt = 1/sampleFreq;
t=0:dt:T;
t=t(1:length(t)-1);
f1=freq;
a1=5;
a2=0;
a3=0;
a4=0;
ph1=20;
ph2=10;
ph3=10;
ph4=123;
y= a1*sin(2*pi*t*f1+ph1*pi/180)   +a2*sin(2*pi*t*f1*2+ph2*pi/180)   +a3*sin(2*pi*t*f1*3+ph3*pi/180)   +a4*sin(2*pi*t*f1*0.5+ph4*pi/180);

result = zdl_fft2(y,N*2,sampleFreq,f1);
