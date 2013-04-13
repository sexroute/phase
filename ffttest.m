clear;clc;close all
freq=5120; %sample frequence
n=32;    %sample round
f0=277.1;    %object work frequence
phase0 = 60; %object phase in degree , /180*pi to get rad


dt=1/freq;
TS = n/f0;
T=0:dt:TS;
T=T(2:length(T));
smp = length(T)/2;

phase0_degree = phase0/180*pi;
sig = sin(2*pi*f0*T+phase0_degree);
sig_compare = sin(2*pi*f0*T);

subplot(3,1,1);
plot(T,sig,'r');
hold on
plot(T,sig_compare);
hold off

fftsig = fft(sig,smp);
fftsig_compare = fft(sig_compare,smp);

df=freq/smp;
freqs = 0:df:freq;
amps = abs(fftsig)*2/smp;
amps_compare = abs(fftsig_compare)*2/smp;

freqNum = fix(smp/2);
freqs = freqs(1:freqNum);

[famp,findex] = max(amps);
freq0_computed = freqs(findex);

[famp_comare,findex] = max(amps_compare);
freq0_computed_compare = freqs(findex);

subplot(3,1,2);
plot(freqs,amps(1:length(freqs)),'r');
hold on;
plot(freqs,amps_compare(1:length(freqs)));
hold off

fpha = phase(fftsig)*180/pi;
phase_computed = fpha(findex);

fpha_compare = phase(fftsig_compare)*180/pi;
phase_computed_compare = fpha_compare(findex);

subplot(3,1,3);
plot(freqs,mod(fpha(1:length(freqs)),360)*pi,'r');
hold on;
plot(freqs,mod(fpha_compare(1:length(freqs)),360)*pi);


phase_diff = phase_computed - phase_computed_compare  



