%
% $Id: rcepstrum.m 32 2006-02-23 09:01:08Z nicb $
%
% here's what must be done:
%
% complex-valued fft of the signal
% real_part = log(modulus(fft(signal)))
% imag_part = arg(fft(imag(signal))
%
% the idea is that a logarithm of a complex number is:
% log(z) = log(|z|)+iarg(z)
%
% there's a point about phase unwrapping the bins before calculating the
% arg, but it is not clear why.

function [cr, ci] = rcepstrum (signal, blocksize)

	fftout = (1/blocksize)*fft(fftshift(signal), blocksize);
	logfft = complex(log(abs(fftout)), angle(fftout));
	cepstout = (0.25/blocksize)*fft(logfft);


	cr = abs(cepstout);
	ci = angle(cepstout);
endfunction
