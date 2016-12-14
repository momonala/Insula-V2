int blinkPin = 13;        // pin to blink led at  beat
//======================SETUP & LOOP==================================================
void setup(){
  pinMode(blinkPin,OUTPUT);       // pin that will blink to your heartbeat!
  Serial.begin(115200);
  //setup_emg(); 
  setup_breath();           
}

void loop(){
  //Serial.print(100);        //for spot checking ChucK
  //Serial.print(",");        //for spot checking ChucK
  
  //emg_processing(); 
  //Serial.print(",");
  breath_processing();     //prints boolean of breath inhale(1) exhale(0)
  Serial.print(",");
  
  //-----PULSE-----
  pulseBool();              //print boolean (1) when pulse detected
  pulse_processing();       //ALWAYS ON run pulse-rate signal processing no print

  //rawPulseSignal();       //prints raw pulse signal data 
  //beatOnOff();            //for plotting visibility 
  //Serial.println();       //COMMENT IF USING "printOnBeats()"
  //printOnBeats();         //only print biodata when beat is detected 
  
  Serial.println();
  delay(40);
}
