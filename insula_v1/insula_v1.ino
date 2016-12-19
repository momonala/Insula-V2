int blinkPin = 13;        // pin to blink led at  beat
int delayVal = 10; 

bool ChuckOn = true;
bool breathOn = true; 
bool pulseOn = true; 
bool emgOn = false; 

//======================SETUP==================================================
void setup(){
  pinMode(blinkPin,OUTPUT);       // pin that will blink to your heartbeat!
  Serial.begin(115200);
  setup_breath();
  //setup_emg();           
}

//======================LOOP==================================================
void loop(){
  if (ChuckOn = true{
    Serial.print(100);        //for spot checking ChucK
    Serial.print(",");
  }
  
  //-----BREATH------
  if (breathOn = true){
    breath_processing();      //ALWAYS ON run breath-rate signal processing no print 
    breathBool();             //prints boolean of breath inhale(1) exhale(0)
    
    //breathAvg();              //avg of raw data
    //breathSlope();
    Serial.print(",");
  }
  
  //-----PULSE-----
  if (pulseOn = true){
    pulseBool();              //print boolean (1) when pulse detected
    pulse_processing();       //ALWAYS ON run pulse-rate signal processing no print
  
    //rawPulseSignal();       //prints raw pulse signal data 
    //beatOnOff();            //for plotting visibility 
    //Serial.println();       //COMMENT IF USING "printOnBeats()"
    //printOnBeats();         //only print biodata when beat is detected
    //Serial.print(","); 
  }
  
  //-----EMG ------
  if (emgOn = true){
    //emg_processing(); 
  }
  
  Serial.println();  
  delay(delayVal);
}
