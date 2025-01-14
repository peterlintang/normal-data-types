function [v, offset, gain, matrix] = ellipsoid_fit_test(x_m, y_m, z_m) %#codegen
% 指定输入和输出的数据类型
% 使用 matlab.unittest.constraints 中的相关约束
%inputs = {
%    matlab.inputPort(double, "x_m", true),
%    matlab.inputPort(double, "y_m", true)
%    matlab.inputPort(double, "z_m", true)
%};
%outputs = {
%    matlab.outputPort(double, "v", true)
%    matlab.outputPort(double, "offset", true)
%    matlab.outputPort(double, "gain", true)
%    matlab.outputPort(double, "matrix", true)
%};
% Import sensor readings
%v=double(zeros(10,1));
%offset = double(zeros(3,1));
%gain = double(zeros(3,3));
%matrix = double(zeros(3,3));
v = ellipsoid_fit(x_m, y_m, z_m);
fprintf('mmmmmmmm v=\n');disp(v);
%v = real(v);
a = v(1); b = v(2); c = v(3);
f = v(4); g = v(5); h = v(6); 
p = v(7); q = v(8); r = v(9); 
d = v(10); 

M =[a, h, g; h, b, f; g, f, c]; % Original ellipsoid matrix 
%M = real(M);
fprintf('\n\nM=\n\n');disp(M);
u = [p, q, r]';
%k = d;

[evec, eval]=eig(M); % Compute eigenvectors matrix
evec = real(evec);
eval = real(eval);
fprintf('\n\n22222 evec eval=\n\n'); disp(evec);disp(eval);
rotation = evec'; % DCM = eigenvectors matrix
%eval = -eval;
M_ = evec'*M*evec; % Diagonalize M

pqr_ = [p,q,r]*evec;   
a_ = M_(1,1);
b_ = M_(2,2);
c_ = M_(3,3);
p_ = pqr_(1);
q_ = pqr_(2);
r_ = pqr_(3);
d_ = d;

ax_ = sqrt(p_^2/a_^2 + q_^2/(a_*b_) + r_^2/(a_*c_) - d_/a_);
bx_ = sqrt(p_^2/(a_*b_) + q_^2/b_^2 + r_^2/(b_*c_) - d_/b_);
cx_ = sqrt(p_^2/(a_*c_) + q_^2/(b_*c_) + r_^2/c_^2 - d_/c_);

offset = - M \ u; % Eqn(21)
gain = [1/ax_, 0, 0; 0, 1/bx_, 0; 0,0,1/cx_];
matrix = gain*rotation;
end

