import serial, csv, msvcrt
import matplotlib.pyplot as plt

optoChucK = False
plotOn = True 

ser = serial.Serial('COM4',115200) #object to read from serial port. Find COM data via Arduino>>Tools>>Port 

print ("Connected to serial port: {}".format(ser.name))
ser.flushInput()
print "press any key to break loop"

uniqueName = "pulseTesting"
fullName = uniqueName + "clean.csv"
cleanCSVfile = open(fullName,'wb') #output .csv file 
wr = csv.writer(cleanCSVfile, dialect='excel') 
#wr.writerow(['f1', 'v1', 'f2', 'v2', 'f3', 'v3', 'f4', 'v4', 'a1', 'a2', 'a3']) #header
wr.writerow(['time', 'bpm', 'ibi']) #header 
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
		dataArray = map(int, dataArray) #convert strings to ints 
		print dataArray
	except ValueError, v: 
		#print "Value Error:", v, arduinoString
		pass
	except IndexError, e: 
		#print "Index Error:", e, arduinoString
		pass
		
	wr = csv.writer(cleanCSVfile, dialect='excel')
	wr.writerow(dataArray) #write array to a .csv row 
	cleanCSVfile.flush

	if msvcrt.kbhit(): #press any key to break loop
		break
	
ser.close() #exit gracefully 
cleanCSVfile.close() #complete writing clean data file 

if (optoChucK == True):
	#must optimize clean, readable .csv file for ChucK by putting all data in one column
	inFile = csv.reader(open(fullName, "rb"))
	CKfile = open(uniqueName +'CK.csv', 'w') 
	print "optimizing file for Chuck..."
	i = 0
	for row in inFile:
		if (i > 0):
			j = 0
			while (j < len(row)):
				newData = row[j] + "\n"
				CKfile.write(newData)
				j += 1
		i += 1
		
	CKfile.close()
	print "files created! Done!"
	
if (plotOn == True):
	time = []
	bpm = []

	inFile = csv.reader(open(fullName,'r'), delimiter=',')
	i = 0
	for row in inFile:
		if (i > 0): #ignore header 
			time.append(int(row[0]))
			bpm.append(int(row[1]))
		i += 1

	plt.figure("bpm over time")
	plt.xlabel("time(ms)")
	plt.ylabel("bpm")
	plt.ylim(40, 90)
	plt.plot(time,bpm)
	plt.show()