clear;close all;clc
aoData =load('wave_test.txt');
anSize = length(aoData);
freq = 25.237263;
sampleFreq = 25600;

result1 = zdl_apfft(aoData,anSize,sampleFreq,freq);
anSize = anSize*2;
N=anSize/2;
T = anSize/sampleFreq;
dt = 1/sampleFreq;
t=-T/2:dt:T/T;
t=t(1:length(t)-1);
f1=freq;
a1=1;
a2=2;
a3=3;
a4=0.5;
ph1=30;
ph2=0;
ph3=0;
ph4=0;
y= a1*cos(2*pi*t*f1+ph1*pi/180)   +a2*cos(2*pi*t*f1*2+ph2*pi/180)   +a3*cos(2*pi*t*f1*3+ph3*pi/180)   +a4*cos(2*pi*t*f1*0.5+ph4*pi/180);
result = zdl_apfft(y,N*2,sampleFreq,f1);
