function loResult = zdl_fft2(apsignal,ansignals,adblSampleFreq,adblTargetFreq)
%1. 元整点数到2的整数倍
lnSlice = 4;
mods = mod(ansignals,lnSlice);
if mods ~= 0
    n_sub = length(apsignal);
    if mod(n_sub,lnSlice) ~=0        
        n_sub = n_sub-1;
        apsignal = apsignal(1:n_sub);
    end
    
    ansignals = n_sub;
end
smp = fix(ansignals/lnSlice);
N=smp;
s=apsignal;
%1
win=hanning(N)';win1=hann(N)';
win2=conv(win,win1);
win2=win2/sum(win2);
w=pi*2;

%2
s1=s(1:2*N-1);      %第1组（2N-1）个数据
    y1=s1.*win2;
    y1a=y1(N:end)+[0 y1(1:N-1)];
    out1=fft(y1a,N);
    a1=abs(out1);
    p1=mod(phase(out1),2*pi);
%3
s2=s(1+N:3*N-1);    %第2组（2N-1）个数据
    y2=s2.*win2;
    y2a=y2(N:end)+[0 y2(1:N-1)];
    out2=fft(y2a,N);
    a2=abs(out2);
    p2=mod(phase(out2),2*pi);
    g=mod((p2-p1)/pi/2,1);
h=2*pi*g.*(1-g.*g)./sin(pi*g);
aa1=abs((h.^2).*a2)/2;
df = adblSampleFreq/smp;
result = zeros(3,3);
for i=1:1:3
    k0=round(f0)+1;
end