int blinkPin = 13;        // pin to blink led at  beat
int delayVal = 10; 
//======================SETUP & LOOP==================================================
void setup(){
  pinMode(blinkPin,OUTPUT);       // pin that will blink to your heartbeat!
  Serial.begin(115200);
  //setup_emg(); 
  setup_breath();          
}

void loop(){
  Serial.print(100);        //for spot checking ChucK
  Serial.print(",");        //for spot checking ChucK

  //-----EMG ------
  //emg_processing(); 
  //Serial.print(",");
  
  //-----BREATH------
  breath_processing();      //ALWAYS ON run breath-rate signal processing no print 
  breathBool();             //prints boolean of breath inhale(1) exhale(0)
  
  //breathAvg();              //avg of raw data
  //breathSlope();
  Serial.print(",");
  
  //-----PULSE-----
  pulseBool();              //print boolean (1) when pulse detected
  pulse_processing();       //ALWAYS ON run pulse-rate signal processing no print

  //rawPulseSignal();       //prints raw pulse signal data 
  //beatOnOff();            //for plotting visibility 
  //Serial.println();       //COMMENT IF USING "printOnBeats()"
  //printOnBeats();         //only print biodata when beat is detected 
  
  Serial.println();
  delay(delayVal);
}
