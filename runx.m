clear;clc;close all; 
lstrRevFileNameConst = 'Rev%d.txt'; 
lstrWaveFileNameConst = 'Wave%d.txt';
diffs = zeros(1);
for i=0:33
    lstrRevFileName = sprintf(lstrRevFileNameConst,i);
    lstrWaveFileName = sprintf(lstrWaveFileNameConst,i);
    diffs(i+1)=computePhase(lstrRevFileName,lstrWaveFileName);
end
plot(diffs);
   