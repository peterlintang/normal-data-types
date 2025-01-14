% Batch 3-axis compass calibration using least squares ellipsoid fitting
%
% References:
%   [1] Renaudin - Complete Triaxis Magnetometer Calibration in the 
%                   Magnetic Domain (2010)
%
% This implementation relies on the theory explained in
%   https://teslabs.com/articles/magnetometer-calibration/ 
%
% 2020/06/03

clc
clear
close all
warning on

% Import raw magnetometer readings
file = 'sensor.dat'; 
%file = 'sensor-x-20.dat'; 
%file = 'sensor_data.txt';
%file='mag-org-1.txt';

% Import sensor readings
raw = importdata(file);
x_m = raw(:,1); 
y_m = raw(:,2); 
z_m = raw(:,3);

%v = double(zeros(10,1));
%offset = double(zeros(3,1));
%gain = double(zeros(3,3));
%matrix = double(zeros(3,3));
[v, offset, gain, matrix]=ellipsoid_fit_test(x_m, y_m, z_m);
fprintf('\n\nv =\n'); disp(v);

% Calibration %
% Memory to calibrated readings 
x_hat = zeros(length(x_m),1); 
y_hat = zeros(length(x_m),1); 
z_hat = zeros(length(x_m),1);
for i_iters = 1:length(x_m)
    % Sensor data
    h_hat = [x_m(i_iters); y_m(i_iters); z_m(i_iters)]; 
    
    % Calibration, Eqn(11)
    h = matrix*(h_hat - offset);
    
    % Calibrated values
    x_hat(i_iters) = h(1);
    y_hat(i_iters) = h(2);
    z_hat(i_iters) = h(3);
end

% Visualization %
% Sensor readings and ellipoid fit
scatter3(x_m, y_m, z_m, 'fill', 'MarkerFaceColor', 'red'); hold on; 
plot_ellipsoid(v); 
title({'Before magnetometer calibration', '(Ellipsoid fit)'});
xlabel('X-axis'); ylabel('Y-axis'); zlabel('Z-axis');
axis equal;

% After calibrations
figure;
scatter3(x_m, y_m, z_m, 'fill', 'MarkerFaceColor', 'blue'); hold on;
plot_sphere([0,0,0]', 40);
title({'After magnetometer calibration', '(Normalized to unit sphere)'});
xlabel('X-axis'); ylabel('Y-axis'); zlabel('Z-axis');
axis equal;


% Print calibration params
fprintf('3D magnetometer calibration based on ellipsoid fitting');
fprintf('\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~');
fprintf('\nThe calibration equation to be implemented:') 
fprintf('\n\t\t\t\th_hat = M*(h_m - b) \nWhere,')
fprintf('\nh_m   = Measured sensor data vector');
fprintf('\nh_hat = Calibrated sensor data vector');
fprintf('\n\nM =\n'); disp(matrix);
fprintf('\nb =\n'); disp(offset);
