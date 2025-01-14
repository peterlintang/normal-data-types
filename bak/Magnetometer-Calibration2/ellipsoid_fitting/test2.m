% 假设数据
xData = rand(1, 100) * 10;
yData = rand(1, 100) * 10;
zData = 5 * sin(xData) .* cos(yData) + randn(1, 100) * 0.1;
 
% 定义椭球模型函数
function z = ellipsoidModel(a, b, c, x, y)
    z = a * sin(x) .* cos(y) + b * sin(x) .* sin(y) + c * cos(x);
end
 
% 拟合椭球模型
f = @(p, x, y) ellipsoidModel(p(1), p(2), p(3), x, y); % 创建模型函数句柄
p0 = [1, 1, 1]; % 初始参数值
[fitresult, gof] = fit([xData', yData'], zData', f, p0);
 
% 提取拟合参数
a = fitresult.a;
b = fitresult.b;
c = fitresult.c;
 
% 可视化数据点和拟合的椭球
figure;
scatter3(xData, yData, zData, 'filled');
hold on;
[x, y] = meshgrid(-10:0.1:10, -10:0.1:10);
z = ellipsoidModel([a b c], x, y);
surf(x, y, z);
xlabel('x');
ylabel('y');
zlabel('z');
title('Data Points and Fitted Ellipsoid');