import serial, csv, msvcrt
ser = serial.Serial('COM3',115200) #object to read from serial port. Find COM data via Arduino>>Tools>>Port 

print ("Connected to serial port: {}".format(ser.name))
ser.flushInput()
print "press any key to break loop"

output = open("output.csv",'wb') #output .csv file 
wr = csv.writer(output, dialect='excel') 
wr.writerow(['f1', 'v1', 'f2', 'v2', 'f3', 'v3', 'f4', 'v4', 'a1', 'a2', 'a3']) #header 
arduinoDataArray = []

while True: # always true 
	ser.reset_input_buffer()
	ser.flushInput()
	if(ser.inWaiting()== 0): #wait until there is data from serial 
		pass #do nothing 
		
	arduinoString = ser.readline() #read the serial port when there is data 
	dataArray = arduinoString.split(',')
	try: 
		#print dataArray 
		arduinoData0 = int(dataArray[0]) 
		arduinoData1 = int(dataArray[1]) 
		arduinoData2 = int(dataArray[2]) 
		arduinoData3 = int(dataArray[3])
		arduinoData4 = int(dataArray[4]) 
		arduinoData5 = int(dataArray[5]) 
		arduinoData6 = int(dataArray[6]) 
		arduinoData7 = int(dataArray[7])
		arduinoData8 = int(dataArray[8]) 
		arduinoData9 = int(dataArray[9]) 
		arduinoData10 = int(dataArray[10])
		arduinoDataArray = [arduinoData0,arduinoData1,arduinoData2,arduinoData3,arduinoData4,arduinoData5,arduinoData6,arduinoData7,arduinoData8,arduinoData9,arduinoData10]
		print arduinoDataArray 
		
	except ValueError, v: 
		#print "Value Error:", v, arduinoString
		pass
	except IndexError, e: 
		#print "Index Error:", e, arduinoString
		pass
		
	if msvcrt.kbhit(): #press any key to break loop
		break
	
	wr = csv.writer(output, dialect='excel')
	wr.writerow(arduinoDataArray) #write array to a .csv row 
	output.flush 

ser.close() #exit gracefully 
output.close()


