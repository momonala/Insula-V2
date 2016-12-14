//EMG Incremental Thresholds 
//---------------------------EMG GLOBAL VARIABLES---------------------------------
const int numReadingsEMG = 10;               //number of samples to average over. Magnitude inversley correlated to processing speed. 
float inputPinEMG = A2;

float readingsEMG[10];                       // the readings from the analog input
int readIndexEMG= 0;                        // the index of the current reading
float totalEMG = 0;                        // the running total
float averageEMG = 0;                     // the average of the signal

float normalizedEMG = 0;                 //normalization of EMG at rest
float readingEMG = 0;                   //current reading 
int flexTimeEMG = 0;
int ratioEMG = 0;                      //ratio between current reading and normalized rest
float maxEMG = 0;                     //max reading of flexion
int maxRatioEMG = 0;
int percentTotalEMG = 0;

//--------------------------------emg setup --------------------------------------
void setup_emg()
{
  for (int thisReadingEMG = 0; thisReadingEMG < numReadingsEMG; thisReadingEMG++) {  // initialize all the readings to 0:
    readingsEMG[thisReadingEMG] = 0;
  }
  Serial.println("P---Relax Arm---");
  delay(200); // delay 0.2 second
  
  for (int i=0; i <= 50; i++) {                   // get average of arm at rest to normalize for thresholding 
    readingEMG = analogRead(inputPinEMG); 
    normalizedEMG = normalizedEMG + readingEMG; 
    delay(30);
  }
  normalizedEMG = normalizedEMG/50;                // average of rest period's 50 data points
  //Serial.print("rest value: ");
  //Serial.println(normalizedEMG);  
  Serial.println("P---EMG Normalized---");
  Serial.println("P---Flex Now ---");               //get maximum reading from flexion for comparison

  flexTimeEMG = millis();                           //calibrate: user flexes an we get maximum reading of a flexion
  while(millis() < flexTimeEMG+2000){
    readingEMG = analogRead(inputPinEMG);
    if (readingEMG > maxEMG){
      maxEMG = readingEMG; 
    }
  }
  //Serial.print("Max EMG Value: ");
  //Serial.println(maxEMG);
  maxRatioEMG = maxEMG/normalizedEMG;
  //Serial.print("Max EMG Ratio: ");
  //Serial.println(maxRatioEMG);
  Serial.println("Intialization complete");
  delay(1000);
}

//----------------------------------emg signal processing --------------------------------
void emg_processing(){
  totalEMG = totalEMG - readingsEMG[readIndexEMG];            // SMOOTHING FUNCTION -- subtract the last reading:
  readingsEMG[readIndexEMG] = analogRead(inputPinEMG);       // read from the sensor:
  totalEMG = totalEMG + readingsEMG[readIndexEMG];          // add the reading to the total:
  readIndexEMG = readIndexEMG + 1;                         // advance to the next position in the array:
  if (readIndexEMG >= numReadingsEMG) {                   // if we're at the end of the array...
    readIndexEMG = 0;                                    // ...wrap around to the beginning:
  }
  averageEMG = totalEMG / numReadingsEMG;                  // calculate the average:
  ratioEMG = averageEMG/ normalizedEMG;                   // ratio between current reading and rest 
  percentTotalEMG = ratioEMG*100/maxRatioEMG;           // percent of current flex/maximum flex in intialization

  //Serial.print("E");                               //Uncomment for Processing, comment for Arduino plotting
  //Serial.println(averageEMG);                   // for troubleshooting/plotting 
  //Serial.println(ratioEMG);                    // for troubleshooting/plotting 
  Serial.println(percentTotalEMG);
}
