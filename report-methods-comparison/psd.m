% FFT - PSD

t = tm(1:250,:);
x = ecg100

%sg = sgolayfilt(s, 2, 101);
y = u

Fs = 360

N = length(x);
xdft = fft(x);
xdft = xdft(1:N/2+1);
psdx = (1/(Fs*N)) * abs(xdft).^2;
psdx(2:end-1) = 2*psdx(2:end-1);
freq = 0:Fs/length(x):Fs/2;

plot(freq,10*log10(psdx), 'b')
grid on
title('Widmowa gêstoœæ mocy dla metody EMD, usuniêto 2 IMF')
xlabel('Frequency (Hz)')
ylabel('Power/Frequency (dB/Hz)')

hold on;

N = length(y);
ydft = fft(y);
ydft = ydft(1:N/2+1);
psdy = (1/(Fs*N)) * abs(ydft).^2;
psdy(2:end-1) = 2*psdy(2:end-1);
freq = 0:Fs/length(y):Fs/2;

plot(freq,10*log10(psdy), 'r')