clear;clc;
N=100;
t=linspace(0,6*pi,N);
s=sin(t);
r=randn(1,N);
a=sqrt(0.009*sum(s.^2)./sum(r.^2));%¼Ó%10µÄÔëÉù
sr=s+a*r;
plot(s)
hold on
%figure(2)
plot(sr,'r')