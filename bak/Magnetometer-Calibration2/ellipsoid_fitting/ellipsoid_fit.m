function [u]=ellipsoid_fit(x,y,z) %#codegen
%inputs = {
%    matlab.inputPort(double, "x", true),
%    matlab.inputPort(double, "y", true)
%    matlab.inputPort(double, "z", true)
%};
%outputs = {
%    matlab.outputPort(double, "u", true)
%};
% Ellipsoid fitting algorithm
%
% Inputs:
%   x = nx1 column vector of x coordinates of input data.
%   y = nx1 column vector of y coordinates of input data.
%   z = nx1 column vector of y coordinates of input data.
%
% Output:
%   u = [a,b,c,f,g,h,p,q,r,d], a vector corresponding to coefficients of 
%       the general quadric surface given by equation,  
%       ax2 + by2 + cz2 + 2fyz + 2gxz + 2hxy + 2px + 2qy + 2rz + d = 0.
%     
% Source:
%   [1] Li - Least Square Ellipsoid Fitting (2004)
%   
% 2020/06/03

%u=double(zeros(10,1));
% Design matrix
D = [ x.^2, y.^2, z.^2, 2.*y.*z, 2.*x.*z, 2.*x.*y, 2*x, 2*y, 2*z, ...
    ones(length(x),1)]';
fprintf('\n\nD =\n'); disp(D);

% Constraint kJ > I^2
% Ellipsoid if k = 4 
k = 4;

% Eqn(7)
C1 = [ -1, 0.5*k-1, 0.5*k-1, 0, 0, 0;
       0.5*k-1, -1, 0.5*k-1, 0, 0, 0;
       0.5*k-1, 0.5*k-1, -1, 0, 0, 0;
       0, 0, 0, -k, 0, 0;
       0, 0, 0, 0, -k, 0;
       0, 0, 0, 0, 0, -k];
fprintf('\n\nC1 =\n'); disp(C1);

% Eqn(11)
S = D*D';
S11 = S(1:6,1:6);   % 6X6
S12 = S(1:6,7:10);  % 6X4
S21 = S(7:10,1:6);  % 4X6
S22 = S(7:10,7:10); % 4X4
fprintf('\n\n S11 S12 S21 S22=\n'); disp(S11);disp(S12);disp(S21);disp(S22);
% Eqn(14) and Eqn(15)
% eval = Diagonal matrix of eigenvalues
% evec = Matix containing corresponding eigenvectors in its column
M            = C1\(S11 - S12*(S22\S21)); 
fprintf('\n\nM =\n'); disp(M);
[evec, eval] = eig(M);
evec=real(evec);
eval=real(eval);
fprintf('\n\n evec,eval =\n'); disp(evec);disp(eval);

% Locate the index of the 'only' positive eigenvalue.
% If M is singular, then u1 is eigenvector of largest eigen-value.
[max_element_row,~]     = max(eval); 
[~, max_column_index]   = max(max_element_row);


% Eigenvector for  positive eigenvalue.
u1 = evec(:, max_column_index);
fprintf('\n\n u1 =\n'); disp(u1);
u2 = -(S22\S21)*u1;
fprintf('\n\n u2=\n');disp(u2);

u = [u1',u2']';
fprintf('\n\n u=\n');disp(u);
end
