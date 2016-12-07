//Smoothing Function for Breath Rate: inhale (1) vs exhale (0)
//EMG Incremental Thresholds 

//---BREATH RATE GLOBAL VARIABLES --------------------------------------------------------------------------------
const int numReadingsBreath = 13;                  // number of samples to average over. Magnitude inversley correlated to processing speed. 
float inputPinBreath = A1;

float readingsBreath[numReadingsBreath];        // the readings from the analog input
int readIndexBreath = 0;                       // the index of the current reading
float totalBreath = 0;                        // the running total
float averageBreath = 0;                     // the average
float oldAverageBreath = 0;  
float slopeBreath = 0;
int  breathOutput = 0;

//---EMG GLOBAL VARIABLES-------------------------------------------------------------------------------------
const int numReadingsEMG = 10;               //number of samples to average over. Magnitude inversley correlated to processing speed. 
float inputPinEMG = A0;

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

//-----------------------------------------------Set up---------------------------------------------------------------
void setup() {
  Serial.begin(115200);             //baud rate
  setup_emg(); 
  setup_breath(); 
}

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

void setup_breath(){
  for (int thisReadingBreath = 0; thisReadingBreath < numReadingsBreath; thisReadingBreath++) {
    readingsBreath[thisReadingBreath] = 0;
  }
}

//--------------------------------------loop-------------------------------------------------------------
void loop() {
  loop_emg(); 
  loop_breath(); 
  delay(40);      // delay in between reads for stability. decrease value to increase speed, but may increase noise. 
}

void loop_emg(){
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

  Serial.print("E");                               //Uncomment for Processing, comment for Arduino plotting
  //Serial.println(averageEMG);                   // for troubleshooting/plotting 
  //Serial.println(ratioEMG);                    // for troubleshooting/plotting 
  Serial.println(percentTotalEMG);
}

void loop_breath(){
  totalBreath = totalBreath - readingsBreath[readIndexBreath];        // subtract the last reading:
  readingsBreath[readIndexBreath] = analogRead(inputPinBreath);      // read from the sensor:
  totalBreath = totalBreath + readingsBreath[readIndexBreath];      // add the reading to the total:
  readIndexBreath = readIndexBreath + 1;                           // advance to the next position in the array:

  if (readIndexBreath>= numReadingsBreath) {                     // if we're at the end of the array...
    readIndexBreath = 0;                                        // ...wrap around to the beginning:
  }

  averageBreath = totalBreath / numReadingsBreath;            // calculate the average:
  slopeBreath = averageBreath-oldAverageBreath;
  //Serial.println(averageBreath);                         // get breath rate curve for troubleshooting/plotting

  if (slopeBreath >= 0.1) {                              // boolean of inhale (1)
     breathOutput = 1;                                  //if slope is between -0.1 & 0.1 (high noise) just keep old value 
  }
  else if (slopeBreath<=-0.1){                              // boolean of exhale (0)
      breathOutput = 0;
  } 
  else {
    breathOutput = breathOutput;
  }
  Serial.print("B");                      //Uncomment for Processing, comment for Arduino plotting
  Serial.println(breathOutput);
  oldAverageBreath = averageBreath;
}
