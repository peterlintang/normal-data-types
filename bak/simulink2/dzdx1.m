%page57
function dz=dzdx1(x,z)
	dz(1) = z(2);
	dz(2) = z(3);
	dz(3) = z(3)*x^(-1) - 3*x^(-2)*z(2) + 2*x^(-3)*z(1) + 9*x^3*sin(x);
	dz = [dz(1);dz(2);dz(3)];
end
