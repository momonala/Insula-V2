int pulsePin = 0;
//-----------------------SIGNAL PROCESS VARIABLES --------------------------------------
int BPM;
int Signal;                // raw data
int IBI = 600;             // interbeat interval, must be seeded 
boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
boolean QS = false;        // absolute beat detector 
int QSvis = 0;             //for plotting visibility  

volatile int rate[10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI
volatile int P =512;                      // used to find peak in pulse wave, seeded
volatile int T = 512;                     // used to find trough in pulse wave, seeded
volatile int thresh = 525;                // used to find instant moment of heart beat, seeded
volatile int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

//--------------------PRINTING FUCTIONS ----------------------------------------
void rawPulseSignal(){
  //Serial.print('S');
  Serial.print(Signal); //RAW SIGNAL
  Serial.print(",");
}
void pulseBool(){
  Serial.print(QS);              //boolean of pulse detection
}

void beatOnOff(){
  Serial.print(QSvis);        //just output beat detection  
  Serial.print(",");  
} 

  void printOnBeats(){
  if (QS == true){                // beat found, BPM and IBI determined
    Serial.print(millis());       //elapsed time
    Serial.print(",");
    //Serial.print("BPM: ");
    Serial.print(BPM); //HEART RATE 
    Serial.print(",");
    //Serial.print(" IBI: ");
    Serial.println(IBI); //INTERBEAT INTERVAL 
  }
}

//----------------------PULSE SIGNAL PROCESSING (MAGIC HAPPENS HERE)--------------------------------
void pulse_processing(){
  QS = false;                  //reset for next beat 
  QSvis = 0;                   //reset
  
  Signal = analogRead(pulsePin);              // Pulse Sensor raw data  
  sampleCounter = millis();                   //time passed (ms) since program stared 
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

//----------find the peak and trough of the pulse wave----------
  if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                          // (T) lowest point in pulse wave 
      T = Signal;                             // keep track of 
    }
  }

  if(Signal > thresh && Signal > P){        // thresh condition helps avoid noise
    P = Signal;                             // (P) highest point in pulse wave
  }                                        

//----------heart beat detection ----------
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse
  
//----------ensure we have realistic BPM value on startup -----------
      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                      
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
      }   


//----------calculate BPM!! ----------------------------- 
      word runningTotal = 0;                  // clear the runningTotal variable of the last 10 IBI values
      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the newest IBI to  rate array
      runningTotal += rate[9];                // add the newest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute (60k ms)? that's BPM!
      QS = true;                              // flag for beat detection 
      QSvis = 100;                            // and one for plotter visibility! 
      // NOTE: QS/vis FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

//----------end of beat -------------------------------
  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat/party is over
    digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

//----------lack of beat? reset variables to start seed  -------------------
  if (N > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }
}

/* USING CODE FROM PULSESENSOR.COM :-)
 KEPT: 
 signal processing algorithm
 
 CHANGES: 
 organized and commented 
 got rid of Interupt service routine to be compatible with other programs I was running 
 used milis() to ensure proper timing of BPM and IBI 
 
*/
