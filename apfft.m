function [amps,phases] = apfft(apsignal,ansignals)

mods = mod(ansignals,2);
if mods ~= 0
    ansignals(length(ansignals)) = 0;
end
smp = ansignals/2;

sig = apsignal;

win=hanning(smp)';%汉宁窗,后面的一瞥为转换为列向量
winn=conv(win,win);%汉宁窗的卷积
win2=winn;%汉宁窗卷积的归一化

len = length(win2);
sig=sig(1:len).*win2;
pts = smp-1;
data1 = sig(pts:pts*2);
data0 = [0,sig(1:pts)];
sig= data0+data1;%长度为N，送入FFT处理，出来为APFFT

fftsig = fft(sig,pts);
amps = abs(fftsig)*2/pts;
phases = (angle(fftsig)*180/pi);