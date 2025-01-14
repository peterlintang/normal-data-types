clear
clc
A=[-2.0498         0         0         0         0         0;
         0    0.2968         0         0         0         0;
         0         0   -0.3108         0         0         0;
         0         0         0   -0.4204         0         0;
         0         0         0         0   -0.8339         0;
         0         0         0         0         0   -0.8475]
[maxValues, maxValuesIndex]=my_max(A);
disp(maxValues); % 显示每列的最大值
disp(maxValuesIndex);
