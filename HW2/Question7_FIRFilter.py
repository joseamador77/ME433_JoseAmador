import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function
import csv

t = [] # column 0
data = [] # column 1

with open('sigD.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        t.append(float(row[0])) # leftmost column
        data.append(float(row[1])) # second column


h = [
    0.000000000000000000,
    0.000053600973729140,
    0.000390331218529125,
    0.001014718222262658,
    0.001311234558944241,
    -0.000000000000000001,
    -0.004174752152728933,
    -0.010957115422247910,
    -0.017044081172488703,
    -0.015900533682709973,
    0.000000000000000005,
    0.034911707676412017,
    0.085716754281499594,
    0.140849797423831663,
    0.183804096911168835,
    0.200048482327596533,
    0.183804096911168835,
    0.140849797423831635,
    0.085716754281499594,
    0.034911707676412045,
    0.000000000000000005,
    -0.015900533682709973,
    -0.017044081172488724,
    -0.010957115422247919,
    -0.004174752152728935,
    -0.000000000000000001,
    0.001311234558944240,
    0.001014718222262659,
    0.000390331218529123,
    0.000053600973729140,
    0.000000000000000000,
]


FIRData = []
numCoeff = len(h)
for i in range(len(data)):
    newData = 0
    for j in range(numCoeff):
        if i-j >= 0:
            newData += h[j]*data[i-j]
    FIRData.append(newData)

    

Fs = len(data)/t[-1]  # sample rate A = 10000 B = 3300 C = 2500 D = 400 
print(Fs)
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = data # the data to make the fft from
yFiltered = FIRData ##the filtered data for the fft
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]
Yfiltered = np.fft.fft(yFiltered)/n # fft computing and normalization
Yfiltered = Yfiltered[range(int(n/2))]


fig, (ax1, ax2) = plt.subplots(2, 1)
plt.suptitle('Signal D: FIR filter w/ ' + str(len(h)) + ' coeffecients, \n cutoff freq = 40 Hz and bandwidth of 60 Hz' )
ax1.plot(t,data,'k') #unfiltered data
ax1.plot(t,yFiltered,'r')#filtered data
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')

ax2.loglog(frq,abs(Y),'k') # plotting the fft #unfiltered
ax2.loglog(frq,abs(Yfiltered),'r') # plotting the fft #filtered
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')

plt.show()

