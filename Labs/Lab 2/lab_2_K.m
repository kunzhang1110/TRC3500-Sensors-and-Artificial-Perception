clc;clear;

% measurements
deflec=[0.020 0.0185 0.0181 0.0178 0.0175 0.0173];
dV_read= [0.56 0.53 0.51 0.50 0.49 0.48];

% constants
I=2.083e-12;
t = 1e-3;
l = 0.255;
Vs = 5;
gain = 560;

strain = 3.*deflec*t/(2*l^2);

dV = dV_read./gain;
r = 4.*dV./(Vs-2.*dV);

K = r./strain;
mean_K = mean(K)*ones(1,length(K));

plot(deflec,K)
hold on
plot(deflec,mean_K,'r')
title('Gauge Factor vs. Deflection');
xlabel('Deflection (mm)')
ylabel('Gauge Factor')
legend('K','Mean K')
grid on
%plot(deflec,dV_read)




