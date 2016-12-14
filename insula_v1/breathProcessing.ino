//Smoothing Function for Breath Rate: inhale (1) vs exhale (0)

//---------------------------BREATH RATE GLOBAL VARIABLES ---------------------
const int numReadingsBreath = 10;                  // number of samples to average over. Magnitude inversley correlated to processing speed. 
float inputPinBreath = A1;

float readingsBreath[numReadingsBreath];        // the readings from the analog input
int readIndexBreath = 0;                       // the index of the current reading
float totalBreath = 0;                        // the running total
float averageBreath = 0;                     // the average
float slopeBreath = 0;
int  breathOutput = 0;

//------------------------------setup ------------------------------------------
void setup_breath(){
  //intialize smoothing function 
  for (int thisReadingBreath = 0; thisReadingBreath < numReadingsBreath; thisReadingBreath++) {
    readingsBreath[thisReadingBreath] = 0;
  }
}

//----------------------------breath signal processing ----------------------
void breath_processing(){
  totalBreath = totalBreath - readingsBreath[readIndexBreath];        // subtract the last reading:
  readingsBreath[readIndexBreath] = analogRead(inputPinBreath);      // read from the sensor:
  totalBreath = totalBreath + readingsBreath[readIndexBreath];      // add the reading to the total:

  if (readIndexBreath >= numReadingsBreath-1) {                     // if we're at the end of the array...
    slopeBreath = (readingsBreath[readIndexBreath]-readingsBreath[0])/numReadingsBreath;
    readIndexBreath = 0;                                        // ...wrap around to the beginning:
  }
  else{
    slopeBreath = (readingsBreath[readIndexBreath]-readingsBreath[readIndexBreath+1])/numReadingsBreath;
    readIndexBreath = readIndexBreath + 1;                           // advance to the next position in the array:  
  }

  averageBreath = totalBreath / numReadingsBreath;            // calculate the average:
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
  
  //Serial.print("B");                      //Uncomment for Processing, comment for Arduino plotting
  Serial.print(breathOutput);
  
}
