%
% $Id: centroid.m 26 2006-02-04 15:44:44Z nicb $
%

function c = centroid (buff,sr)
	[rows, columns] = size(buff);
	binsize = sr/columns;
	halfbuff = columns/2;
	tmp = [1:halfbuff];
	
	num = sum (tmp.*buff(1:halfbuff));
	den = sum (buff(1:halfbuff));

	c = num/den;
	c = binsize*(c-0.5);
endfunction
