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

X = 35
MAFData = []
for i in range(len(data)):
    aver = 0
    for j in range(X):
        if i-j >= 0: 
            aver = aver + data[i-j]
    MAFData.append(aver/X)


Fs = len(data)/t[-1]  # sample rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = data # the data to make the fft from
yFiltered = MAFData ##the filtered data for the fft
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
plt.suptitle('Signal D: Averaging ' + str(X) + ' data points')
ax1.plot(t,y,'k') #unfiltered data
ax1.plot(t,yFiltered,'r')#filtered data
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')

ax2.loglog(frq,abs(Y),'k') # plotting the fft #unfiltered
ax2.loglog(frq,abs(Yfiltered),'r') # plotting the fft #filtered
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')

plt.show()

