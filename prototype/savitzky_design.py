# Savitzky-Golay design according to Ronald W. Schafer "What is a SG filter?"

import numpy as np
import matplotlib.pyplot as plt

def read_datafile(file_name):
    data = np.loadtxt(file_name, delimiter=',', unpack=True, skiprows=1)
    return data

print('Read MIT-BIH data...')    
data = read_datafile('../mit-bih-txt/mitdb100short.txt')

# short test data - 300 samples
# time vector
t = data[0][:600]

# ECG vector
ecg = data[1][:600]

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
plt.plot(impulse_domain, d, label='impulse sequence')
plt.plot(impulse_domain, h, '-r', label='Impulse response h')

#context = ecg[n-M:n+M]
#print('Context:', context)

# TODO: perform a convolution from n-M to n+M of sum( h[n-m] * x[n])

# 
#ecg_filtered = 


# TODO: Find low-frequency baseline


# plot single heart cycle with 1 QRS
#plt.plot(t, ecg, 'b-', label="ECG")
#plt.plot(t, ecg_filtered, 'g-', label="Savitzky-Golay")
plt.ylabel('ECG signal')
plt.xlabel('Time [s]')
plt.legend()
plt.show()