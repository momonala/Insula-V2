import csv
uniqueName = "output"
CKfile = open(uniqueName +'CK.csv', 'w') 
reader = csv.reader(open(uniqueName + "Clean.csv", "rb"))
i = 0
for row in reader:
	if (i > 0):
		j = 0
		while (j < len(row)):
			print row[j], j
			
			newData = row[j] + "\n"
			CKfile.write(newData)
			j += 1
	i += 1
CKfile.close()
print "files created! Done!"
			 
		
