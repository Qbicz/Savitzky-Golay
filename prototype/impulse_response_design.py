# Savitzky-Golay: show convolution coeffs on plot
import numpy as np
import matplotlib.pyplot as plt

M = 3 # window width is 2M+1
N = 2 # fitting polynomial degree
n = 10

# d is impulse sequence
d = np.concatenate([np.zeros(M), [1], np.zeros(M)])
impulse_domain = np.arange(-M,M+1)
print('d =', d)
a = np.polyfit(impulse_domain, d, N)
print('polynomial coefficients a =', a)
h = np.flipud(np.polyval(a, impulse_domain))

# plot impulse polynomial design
plt.plot(impulse_domain, d, 'bo', label='impulse sequence')
plt.plot(impulse_domain, h, 'r-', label='Impulse response h')
plt.ylabel('Value')
plt.xlabel('Sample')
plt.grid()
plt.legend()
plt.show()