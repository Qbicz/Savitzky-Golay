#!/usr/bin/python

# Savitzky-Golay design according to Ronald W. Schafer "What is a SG filter?"


import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import time

def read_datafile(file_name):
    data = np.loadtxt(file_name, delimiter=' ', unpack=True, skiprows=1)
    return data

print('Read MIT-BIH data...')    
data = read_datafile('../mit-bih-txt/mitdb100.txt')

# short test data - 300 samples
# time vector
t = data[0][:]

# ECG vector
x = data[1][:]

# Savitzky-Golay filtering
M = 500 # window width is 2M+1
N = 2 # fitting polynomial degree

###
##
# measure performance
start = time.time()


# d is impulse sequence
d = np.concatenate([np.zeros(M), [1], np.zeros(M)])
impulse_domain = np.arange(-M,M+1)
print('d =', d)
a = np.polyfit(impulse_domain, d, N)
print('polynomial coefficients a =', a)
h = np.flipud(np.polyval(a, impulse_domain))

# TODO: perform a convolution from n-M to n+M of sum( h[n-m] * x[n])
y = []
#print(y)
n = 5
impulse_domain = np.arange(n-M,n+M+1)
a = np.polyfit(impulse_domain, d, N)
h = np.flipud(np.polyval(a, impulse_domain))
y = np.append(y, 0)
#plt.plot(h, 'b-', label="Odpowiedz impulsowa filtru")
#plt.plot(d, 'r-', label="Sekwencja z jednostkowym impulsem")


# Treatment of first and last points: use 'mirror' extrapolation
xfirst = np.flipud(x[1:M+1])
xlast = np.flipud(x[len(x)-M-1:len(x)-1])
print(xfirst)
print('x[0:10] =', x[0:10])
print('...')
print('x[-10:]', x[-10:])
print(xlast)

x_mirror = np.concatenate((xfirst, x, xlast))

# for n in range(M, len(x_mirror)-M):
    # # TODO: compute h only once
    # impulse_domain = np.arange(n-M,n+M+1)
    # a = np.polyfit(impulse_domain, d, N)
    # h = np.flipud(np.polyval(a, impulse_domain))
    # y = np.append(y, 0)
    # print(n)
    
    # # reference
    # hr = signal.savgol_coeffs(2*M+1, N, deriv=0, delta=1.0)
    # #print('hr: ', hr)
    # #print('h:', h)
    
    # for m in range(n-M, n+M+1):
        # print('n=%d, m=%d' % (n,m))
        # print('h[n-m]', h[n-m])
        # print('x_mirror[m]', x_mirror[m])
        # print('y[n]', y[n-M])
        # y[n-M] += h[n-m] * x_mirror[m] # use extrapolated points
   
# 'valid' means there is no convolution if signals dont overlap completely
y = np.convolve(h[::-1], x_mirror, mode='valid') 
print(y[1:20])
#for i in range(3):
#    y = np.append(y,0)
 
#
##
### end performance measurement
end = time.time()
print('Algorithm took %f seconds.' % (end-start))
 
ecg_filtered = y
ecg = x

start = time.time()
ecg_reference = signal.savgol_filter(ecg, 2*M+1, N, deriv=0, delta=1.0, axis=-1, mode='mirror', cval=0.0)
end = time.time()
print('Reference took %f seconds.' % (end-start))
print('filtered signal ', ecg_filtered[0:100])

print(len(t), len(x), len(y), len(x_mirror))

# TODO: Find low-frequency baseline


# plot single heart cycle with 1 QRS
#plt.plot(t, ecg, 'b-', label="ECG")
#plt.plot(t, ecg_filtered, 'g-', label="Savitzky-Golay")
#plt.plot(t, ecg_reference, 'ro', label="Sav-Gol SciPy mirror")






#Here we compare with implementation

#implem_data = read_datafile("out_mitdb100_implem_aligned.txt")
#implem_ecg = implem_data[2][:]

#print(len(implem_ecg))
print(len(ecg))

# MSE calculation
#mse_cpp = ((implem_ecg - ecg_reference) ** 2).max()
#mse_py = ((y - ecg_reference) ** 2).max()
#print('MSE: ', mse_cpp, mse_py)

ecg_baseline = ecg_filtered

plt.plot(t, ecg_baseline, 'g-', label="Izolinia")
plt.plot(t, x, 'r-', label="Sygnał EKG")

plt.plot(t, x - ecg_baseline, 'b-', label="Izolinia usunieta")

#plt.plot(t, implem_ecg, 'r-', label="C++ implementation")

plt.ylabel('Sygnał EKG [mV]')
plt.xlabel('Czas [s]')
plt.legend(loc=3)
#plt.title('Porównanie filtracji Savitzky-Golay')
plt.grid()
plt.show()
