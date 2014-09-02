function result = zdl_apfft(apsignal,ansignals,adblSampleFreq,adblTargetFreq)
%1. 元整点数到2的整数倍
mods = mod(ansignals,2);
if mods ~= 0
    n_sub = length(apsignal);
    if mod(n_sub,2) ~=0        
        n_sub = n_sub-1;
        apsignal = apsignal(1:n_sub);
    end
    
    ansignals = n_sub;
end
smp = fix(ansignals/2);
N = smp;
y = apsignal;

win = hanning(N)';
win1 = win/sum(win);    %窗归一   用于把加权求和的窗进行归一化



y1 = y(N:2*N-1);
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

ee = mod((p1-p2)/180/(1-1/N),1);    %频率偏离矫正值  
df = adblSampleFreq/N;
aa = (a1.^2)./a2*2; %振幅校正值  为什么要乘以2
f1 = adblTargetFreq;
fftsig = zeros(3,3);
for i = 1:1:3
compute_x = i;
r=round(f1*compute_x/df);%求谱线个数
%disp('频率校正值')
fff=floor(f1*compute_x)+ee(r+1);
fftsig(i,1)=fff;
%disp('振幅校正值')
aaa=aa(r+1);
fftsig(i,2)= aaa;
%disp('初相位校正值');
ppp=p2(r+1);
fftsig(i,3)= ppp;
end 
result = fftsig;
   