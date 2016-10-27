# Savitzky-Golay design according to Ronald W. Schafer "What is a SG filter?"

import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

def read_datafile(file_name):
    data = np.loadtxt(file_name, delimiter=',', unpack=True, skiprows=1)
    return data

print('Read MIT-BIH data...')    
data = read_datafile('../mit-bih-txt/mitdb100short.txt')

# short test data - 300 samples
# time vector
t = data[0][60:80]

# ECG vector
x = data[1][60:80]

# Savitzky-Golay filtering
M = 3
N = 2
n = 10

# d is impulse sequence
d = np.concatenate([np.zeros(M), [1], np.zeros(M)])
impulse_domain = np.arange(-M,M+1)
print(d)
a = np.polyfit(impulse_domain, d, N)
print(a)
h = np.flipud(np.polyval(a, impulse_domain))

# plot impulse polynomial design
#plt.plot(impulse_domain, d, label='impulse sequence')
#plt.plot(impulse_domain, h, '-r', label='Impulse response h')

#context = ecg[n-M:n+M]
#print('Context:', context)

# TODO: perform a convolution from n-M to n+M of sum( h[n-m] * x[n])
y = x[0:3]
print(y)

for n in range(M, len(x)-M):
    impulse_domain = np.arange(n-M,n+M+1)
    a = np.polyfit(impulse_domain, d, N)
    h = np.flipud(np.polyval(a, impulse_domain))
    y = np.append(y, 0)
    print(n)
    
    # reference
    hr = signal.savgol_coeffs(2*M+1, N, deriv=0, delta=1.0)
    print('hr: ', hr)
    print('h:', h)
    
    for m in range(n-M, n+M):
        #print('n=%d, m=%d' % (n,m))
        #print('h[n-m]', h[n-m])
        #print(x[m])
        #print(y[n])
        y[n] += h[n-m] * x[m]
                

for i in range(3):
    y = np.append(y,0)
 
ecg_filtered = y
ecg = x

ecg_ref1 = signal.savgol_filter(ecg, 7, 2, deriv=0, delta=1.0, axis=-1, mode='mirror', cval=0.0)
ecg_ref2 = signal.savgol_filter(ecg, 7, 2, deriv=0, delta=1.0, axis=-1, mode='interp', cval=0.0)

print(len(t), len(x), len(y))

# TODO: Find low-frequency baseline


# plot single heart cycle with 1 QRS
plt.plot(t, ecg, 'b-', label="ECG")
plt.plot(t, ecg_filtered, 'g-', label="Savitzky-Golay")
plt.plot(t, ecg_ref1, 'r-', label="Savitzky-Golay Scipy mirror")
plt.plot(t, ecg_ref1, 'ko', label="Savitzky-Golay Scipy interp")
plt.ylabel('ECG signal')
plt.xlabel('Time [s]')
plt.legend()
plt.show()