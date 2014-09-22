#Python module that plots the data we read from the serial port on a 
#real time graph with dynamic y-axis (Temp scale) updating
import serial
import numpy as np
from matplotlib import pyplot as plt
from time import sleep

#------------Set up Serial Port Reading-----------------------------------------
ser = serial.Serial(3, 9600)
trans_color = '#FFFFFE'
temp = 27

#------------Initializing an animated plot-------------------------------------
plt.ion()
ydata = [0] * 50
ax1=plt.axes()  

#Create initial graph
line, = plt.plot(ydata)
plt.ylim([10,40])

#Set up grid lines
plt.grid(b=True, which='major', color='r', linestyle='-')
plt.grid(b=True, which='minor', color='r', linestyle='--')
	 
if __name__ == "__main__":	
	while True:  
		sleep(.1) #Delay for updating
		
		#Read serial data
		data = ser.readline().rstrip()
		print data
		
		#Lable the axes
		plt.xlabel('Time')
		plt.ylabel('Temperature C')
		plt.suptitle('Temperature Variation Over Time')
		
		#Set up new Temperature Scale
		ymin = float(min(ydata))-10
		ymax = float(max(ydata))+10
		plt.ylim([ymin,ymax])
		
		#Add new value at the end and remove the first value
		ydata.append(data)
		del ydata[0]
		

		
		#Show new graph
		line.set_xdata(np.arange(len(ydata)))
		line.set_ydata(ydata)  
		plt.draw()
		