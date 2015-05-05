%
% HANNING(N) returns the N-point Hanning window in a column vector.
% This is the real hanning window as asserted by Daniel Arfib
% $Id: hanning.m 36 2006-03-09 10:52:46Z mb $
function w = hanning(n)
	
	w = .5*(1 - cos(2*pi*(0:n-1)'/(n)));

endfunction
