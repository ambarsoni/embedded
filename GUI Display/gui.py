#Python Module for a GUI that displays a background image as well as
#a constatnly updating temperature in real time as read from the 
#Serial Port
import serial
from Tkinter import *
from PIL import Image, ImageTk
from time import sleep

#Initialize the GUI
root = Tk()

#Constantly changing value
temp = StringVar()
temp.set('Current Temperature: \n' + str(28) + ' C')

#Set background image
trans_color = '#FFFFFE'
img = ImageTk.PhotoImage(Image.open('cornell.jpg'))
img_label = Label(root, image=img)
img_label.pack()
img_label.img = img  #label so don't overwrite
root.update()
overlay = Toplevel(root)
geo = '{}x{}+{}+{}'.format(root.winfo_width(), root.winfo_height(), root.winfo_rootx(), root.winfo_rooty())
overlay.geometry(geo)
overlay.overrideredirect(1)
overlay.wm_attributes('-transparent', trans_color)
overlay.config(background=trans_color)

#Label to be overlayed
lbl = Label(overlay, textvariable=temp, font=("Helvetica", 35), fg="green")
lbl.config(background=trans_color)
lbl.pack()

#Updates the handler once the label changes 
def moved(e):
	geo = '{}x{}+{}+{}'.format(root.winfo_width(), root.winfo_height(),
		root.winfo_rootx(), root.winfo_rooty())
	overlay.geometry(geo)

root.bind('<Configure>', moved)


if __name__ == "__main__":
	while(1):
		sleep(.1) #Delay for updating
		
		#Read serial port
		ser = serial.Serial(3, 9600)
		data = ser.readline().rstrip()
		ser.close()
		
		#Convert data
		data = int(data)
		print data
		
		#Update label with new temperature
		temp.set('Current Temperature: \n' + str(data) + ' C')
		root.update_idletasks()
		root.update()
		
	

