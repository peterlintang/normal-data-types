
function Sum = SUModd(N)
%#codegen 
	Sum = 0; 
	ii = 1; 
	while ne(ii, N)
		if ne(mod(ii, 2), 0)
			Sum = Sum + ii;
		else
			Sum = Sum;
		end
		ii = ii + 1;
	end
	Sum
end
