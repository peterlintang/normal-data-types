
%page65
function F=myfun(x)
	F = [2*x(1)+3*x(2)-exp(-2*x(1));
		x(1)-x(2)+exp(-sqrt(x(2)))];
end
