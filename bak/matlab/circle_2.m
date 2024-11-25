clear;
clc;
close all;


P = [1 7; 2 6; 5 8; 7 7; 9 5; 3 7]';
n= length(P);


plot(P(1,:), P(2,:), '*');

%build deisgn matrix
A = [ P(1,:); P(2,:); ones(1,n)]';
b = sum(P.*P, 1)';

ls solution
a= (A'*A)^(-1)*A'*b;

xc = 0.5*a(1);
yc = 0.5*a(2);
R  =  sqrt((a(1)^2+a(2)^2)/4+a(3));
R

viscircles([xc, yc],R);
axis equal
