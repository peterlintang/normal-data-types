close all;                          % close all figures
clear;                              % clear all variables
clc;                                % clear the command terminal

%% simulate data 
c = [-0.5; 0.2; 0.1]; % ellipsoid center
r = [1; 1; 1]; % semiaxis radii

[x,y,z] = ellipsoid(c(1),c(2),c(3),r(1),r(2),r(3),6);
D = [x(:),y(:),z(:)];

% add noise:
n = length(D);
noiseIntensity = 0.05; %噪声强度 
D = D + randn(n,3) * noiseIntensity;


%% matlab internal fitting 

[A,b,expmfs] = magcal(D, 'eye')
%fprintf( 'away from cetner %.5g\n', norm( b' - c) );
C = (D-b)*A; % calibrated data

figure(1)
plot3(D(:,1),D(:,2),D(:,3),'LineStyle','none','Marker','X','MarkerSize',2)
hold on
grid(gca,'on')
plot3(C(:,1),C(:,2),C(:,3),'LineStyle','none','Marker', ...
            'o','MarkerSize',2,'MarkerFaceColor','r') 
axis equal
xlabel('uT'); ylabel('uT');zlabel('uT') 
legend('Uncalibrated Samples', 'Calibrated Samples','Location', 'southoutside')
title("Uncalibrated vs Calibrated" + newline + "Magnetometer Measurements")
axis equal
hold off
