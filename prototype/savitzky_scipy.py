# Savitzky-Golay scipy try

import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

# time vector
t = np.arange(0,10,0.1)

# ECG vector - put MIT-BIH data here
ecg = np.random.rand(1,100)*np.cos(t)
ecg = ecg[0] # extract 1-dim list from 2-dim list

# Savitzky-Golay filtering
# http://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.savgol_filter.html
ecg_filtered = signal.savgol_filter(ecg, 7, 2, deriv=0, delta=1.0, axis=-1, mode='interp', cval=0.0)
# keyword args with default values: deriv=0, delta=1.0, axis=-1, mode='interp', cval=0.0

# do the plotting with pyplot
plt.plot(t, ecg, 'b-', label="ECG")
plt.plot(t, ecg_filtered, 'g-', label="ECG filtered")
plt.ylabel('ECG signal')
plt.xlabel('Time [s]')
plt.legend()
plt.show()
