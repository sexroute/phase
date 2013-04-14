clear;clc;close all; 
lstrRevFileNameConst = 'Rev%d.txt'; 
lstrWaveFileNameConst = 'Wave%d.txt';
diffs0 = zeros(1);
diffs1 = zeros(1);
index = 1;
for i=3:33
    lstrRevFileName = sprintf(lstrRevFileNameConst,i);
    lstrWaveFileName = sprintf(lstrWaveFileNameConst,i);
    
    diffs0(index)=computePhase(lstrRevFileName,lstrWaveFileName,0);
    diffs1(index)=computePhase(lstrRevFileName,lstrWaveFileName,1);
    index = index+1;
end
stem(diffs0);
hold on
stem(diffs1,'r');
   