% read dd
% cong excel biao ge dao ru signals shu ju dui xiang
% wei yan zheng, parameters ye ke tong guo lei si fang shi dao ru
% pag 174
[sig_data, sig_str] = xlsread('DataDic.xls', 1);
sig_name = sig_str(2 : end, 1);
sig_dimensions = sig_data( :, 1 : 2);
sig_description = sig_str(2 : end, 4);
sig_min = sig_data( : , 4);
sig_max = sig_data( : , 5);
sig_initval = sig_data( : , 6);
sig_units = sig_str(2 : end, 8);
sig_datatypes = sig_str(2 : end, 9);
sig_csc = sig_str(2 : end, 10);

% get num of signals;
sig_num = size(sig_data, 1);

% create signal objects
for ii = 1 : sig_num
	eval([sig_name{ii}, ' = Simulink.Signal;']);
	eval([sig_name{ii}, ".Dimensions = [',num2str(sig_dimensions(ii, : )), '];"]);
	eval([sig_name{ii}, '.Description = ''', sig_description{ii}, ''';']);
	eval([sig_name{ii}, '.Min = ', num2str(sig_min(ii)), ';']);
	eval([sig_name{ii}, '.Min = ', num2str(sig_max(ii)), ';']);
	eval([sig_name{ii}, '.InitialVale = ''', num2str(sig_initval(ii)),''';']);
	eval([sig_name{ii}, '.DocUnits = ''', sig_units{ii}, ''';']);
	eval([sig_name{ii}, '.DataType = ''', sig_datatypes{ii},''';']);
	eval([sig_name{ii}, '.CoderInfo.StorageClass = ''', sig_csc{ii},''';']);
end
line_h = find_system(gcs, 'findall', 'on', 'Type', 'line');
set(line_h, 'MustResolveToSignalObject', 1);



















