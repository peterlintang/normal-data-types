function [maxValues, maxValuesIndex]=my_max(A) %#codegen
[rows, cols] = size(A); % 获取数组的行数和列数
maxValues = zeros(1, cols); % 初始化存储每列最大值的数组
 
for col = 1:cols
	maxValue = A(1, col); % 假设第一行的值为最大值
  	maxValuesIndex(col)=1;
	for row = 2:rows
		if A(row, col) > maxValue
			maxValue = A(row, col); % 更新当前列的最大值
    			maxValuesIndex(col)=row;
		end
	end
	maxValues(col) = maxValue; % 存储最大值
end
 
end
