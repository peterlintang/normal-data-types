clear;
clc;
close all;


P = [1 7; 2 6; 5 8; 7 7; 9 5; 3 7];

%给定初值
xc = 5.3794;
yc = 7.2532;
r = 3.0370;
res = [xc; yc; r];

max_iters = 20;

max_dif = 10^(-6); % max difference between consecutive results
for i = 1 : max_iters
    J = Jacobian(res(1), res(2), P);
    R = Residual(res(1), res(2), res(3), P);
    prev = res;
    res = res - (J'*J)^(-1)*J'*R;
    dif = abs((prev - res)./res); 
    if dif < max_dif
        fprintf('Convergence met after %d iterations\n', i);
        break;
    end
end
if i == max_iters
    fprintf('Convergence not reached after %d iterations\n', i);
end

xc = res(1);
yc = res(2);
r = res(3);

plot(P(:,1), P(:,2), '*')
 viscircles([xc, yc],r);
 axis equal
 
function J = Jacobian(xc, yc, P) 
    len = size(P);
    r = sqrt((xc - P(:,1)).^2 + (yc - P(:,2)).^2);
    J = [(xc - P(:,1))./r, (yc - P(:,2))./r, -ones(len(1), 1)];
end


function R = Residual(xc, yc, r, P)
    R = sqrt((xc - P(:,1)).^2 + (yc - P(:,2)).^2) - r;
end

