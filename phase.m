clear;clc;close all;
%1.计算整周期
rev=load('rev0.txt');
y=length(rev);
z=zeros(1);
i=1;
zIndex=1;
while i<y    
    lnrev = rev(i);
    if lnrev >2 
        z(zIndex) = i;
        zIndex=zIndex+1;
        for j=i:1:y
            lnrev = rev(j);
            if(lnrev <2)
                i=j;
                break;
            end
        end
    else
        i=i+1;
        %break;
    end
end

%2.截取波形
wave = load('wave0.txt');

revstartIndex = 1;%从第几个周期开始截断波形
revendIndex = -1;
wavestartIndex = z(revstartIndex);
waveendIndex = -1;
y=length(z);
i=y;
wavelength = length(wave);
while i > revstartIndex
    if(z(i)<=wavelength)
        waveendIndex =z(i);
        revendIndex = i;
        break;
    else
        i=i-1;
    end
end

wavenew = wave(wavestartIndex:waveendIndex);
revnew = rev(wavestartIndex:waveendIndex);
wavedata =fft(wavenew);
amp = (real(wavedata));
plot(amp);
%{
revnew = revnew/30-9.35;
subplot(311);
plot(wavenew);
subplot(312);
plot(revnew);
subplot(313);
plot(wavenew);
hold on
plot(revnew,'r-');
%}

%3.生成正弦波
TS=1 %采样时间1s
referwave = sin(2*pi*(waveendIndex-wavestartIndex+1)/TS);
