//Smoothing Function for Breath Rate: inhale (1) vs exhale (0)

//---------------------------BREATH RATE GLOBAL VARIABLES ---------------------
const int numReadingsBreath = 17;                  // number of samples to average over. Magnitude inversley correlated to processing speed. 
const float arraySize =  float(numReadingsBreath);   //float for more percise division 
float inputPinBreath = A1;
const float slopeThreshold = 0.1176;            //slope detection (divisible by numReadingsBreath)

float readingsBreath[numReadingsBreath];        // the readings from the analog input
int readIndexBreath = 0;                       // the index of the current reading
float totalBreath = 0.0;                        // the running total
float averageBreath = 0.0;                     // the average
float slopeBreath = 0.0;
boolean  breathOutput = false;

//------------------------------setup ------------------------------------------
void setup_breath(){
  //intialize smoothing function 
  for (int thisReadingBreath = 0; thisReadingBreath < numReadingsBreath; thisReadingBreath++) {
    readingsBreath[thisReadingBreath] = 0;
  }
}

//---------------------------printing -----------------------------------------
void breathBool(){
  //Serial.print("B");                      //Uncomment for Processing, comment for Arduino plotting
  Serial.print(breathOutput);
}

void breathAvg(){
  //Serial.print("B");                      //Uncomment for Processing, comment for Arduino plotting
  Serial.print(averageBreath);
}

void breathSlope(){
  //Serial.print("B");                      //Uncomment for Processing, comment for Arduino plotting
  Serial.print(slopeBreath);
}

//----------------------------breath signal processing ----------------------
void breath_processing(){
  totalBreath = totalBreath - readingsBreath[readIndexBreath];        // subtract the last reading:
  readingsBreath[readIndexBreath] = analogRead(inputPinBreath);      // read from the sensor:
  totalBreath = totalBreath + readingsBreath[readIndexBreath];      // add the reading to the total:

  if (readIndexBreath >= numReadingsBreath-1) {                     // if we're at the end of the array...
    slopeBreath = (readingsBreath[readIndexBreath]-readingsBreath[0])/arraySize;
    readIndexBreath = 0;                                        // ...wrap around to the beginning:
  }
  else{
    slopeBreath = (readingsBreath[readIndexBreath]-readingsBreath[readIndexBreath+1])/arraySize;
    readIndexBreath = readIndexBreath + 1;                           // advance to the next position in the array:  
  }

  averageBreath = totalBreath / arraySize;            // calculate the average:
  //Serial.println(averageBreath);                         // get breath rate curve for troubleshooting/plotting

  if (slopeBreath >= slopeThreshold) {                              // boolean of inhale (1)
     breathOutput = true;                                  //if slope is between -0.1 & 0.1 (high noise) just keep old value 
  }
  else if (slopeBreath<= -1*slopeThreshold){                              // boolean of exhale (0)
      breathOutput = false;
  } 
  else {
    breathOutput = breathOutput;
  }  
  //slopeBreath *= 100.0;
}
