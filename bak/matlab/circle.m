clear;
clc;
close all;


P = [1 7; 2 6; 5 8; 7 7; 9 5; 3 7];
B = [(P.*P)*[1 1]', P(:,1), P(:,2)];
b = -ones(length(P), 1);
res = (B'*B)^(-1)*B'*b;

xc = -res(2)/(2*res(1));
yc = -res(3)/(2*res(1));

c = sqrt(1 - res(1)^2 - res(2)^2 - res(3)^2);
r = sqrt((res(2)^2 + res(3)^2)/(4*res(1)^2) - c/res(1));

plot(P(:,1), P(:,2), '*')
viscircles([xc, yc],r);
axis equal
