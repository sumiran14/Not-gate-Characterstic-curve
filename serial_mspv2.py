import serial
import time
import matplotlib.pyplot as plt
import csv
i      = 0   #to store which digit
j      = 0   #to store the number in the list
count  = 1  # to store which list gets the first value
vout   = [1023] 
vin    = [0]

ser = serial.Serial('COM8', 9600) #this opens the port  at COM8 and baudrate is 9600
time.sleep(1)
time.sleep(1)

#handshaking
ser.write(b'1')  #sends ones
print "1 is sent, waiting for handshake"
s = ser.read()  #waits for response 
print s
ao=0
while True:
	s = ser.read()
	if s == 'O':
		break
	s = int(s)
	if i == 0:
		num = s*1000
		i = i+1
		j=1
	elif i == 1:
		num = num + s*100
		i = i+1
		j=1
	elif i == 2:
		num = num + s*10
		i= i+1
		j=1
	elif i == 3:
		num = num +s
		i=0
		j=0
	if j == 0 and count == 1 :
		vin.append(ao)
		ao=ao+1
		count = 0
		print" vin   : ", num 
		num=0
	elif j == 0 and count == 0:
		vout.append(num)
		count = 1
		print" vout  : " , num
		num=0
	ser.write(b'1')
plt.scatter(vin,vout,marker = 'o')
plt.show()

with open('matlab_plot.csv', mode='wb') as matlab_data:
    matlab_plotter = csv.writer(matlab_data, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    for ao in range(0,255):
 		matlab_plotter.writerow([vin[ao],vout[ao]])