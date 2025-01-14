% 创建模拟数据
x = linspace(0, 10, 100);
y = 5 * sin(x) + 3 * cos(x) + 2;
 
% 添加噪声
noise = randn(size(x));
y = y + noise;
 
% 定义一个椭圆形状的模型
ellipseModel = @(coeff, x) coeff(1) * x.^2 + coeff(2) * x .* y + coeff(3) * y.^2;
 
% 初始参数
initialGuess = [1 1 1]; % 初始参数可能需要根据实际情况调整
 
% 拟合椭圆
[coeff, fitInfo] = fit(x, y, ellipseModel, initialGuess);
 
% 绘制数据点和拟合的椭圆
figure;
plot(x, y, 'o', 'MarkerSize', 8, 'MarkerFaceColor', 'b');
hold on;
 
% 绘制拟合的椭圆
[xFit, yFit] = meshgrid(linspace(min(x), max(x), 100), linspace(min(y), max(y), 100));
zFit = coeff(1) * xFit.^2 + coeff(2) * xFit .* yFit + coeff(3) * yFit.^2;
surf(xFit, yFit, zFit, 'FaceColor', 'r', 'EdgeColor', 'none');
 
xlabel('x-axis');
ylabel('y-axis');
zlabel('z-axis');
title('Ellipse Fit');
legend('Data', 'Ellipse');
hold off;