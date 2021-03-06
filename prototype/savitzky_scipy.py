# Savitzky-Golay scipy try

import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

def read_datafile(file_name):
    data = np.loadtxt(file_name, delimiter=',', unpack=True, skiprows=1)
    return data

print('Read MIT-BIH data...')    
data = read_datafile('../mit-bih-txt/mitdb100.txt')

# time vector
t = data[0]

# ECG vector - put MIT-BIH data here
ecg = data[1]

# Savitzky-Golay filtering
# http://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.savgol_filter.html
ecg_filtered = signal.savgol_filter(ecg, 7, 2, deriv=0, delta=1.0, axis=-1, mode='interp', cval=0.0)
# keyword args with default values: deriv=0, delta=1.0, axis=-1, mode='interp', cval=0.0

# do the plotting with pyplot

# plot single heart cycle with 1 QRS
plt.plot(t[:300], ecg[:300], 'b-', label="ECG")
plt.plot(t[:300], ecg_filtered[:300], 'g-', label="ECG filtered")
plt.plot(t[:300], data[2][:300], 'r-', label="Plain Data[2] from CSV")
plt.ylabel('ECG signal')
plt.xlabel('Time [s]')
plt.legend()
plt.show()
