#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library
MMA8452Q accel; // create an instance of the MMA8452Q class called 'accel'

const int delayVal = 10;       //up here for convenience  
const int veloArraySize = 5; //length of array to hold velocity over  
const int accelArraySize = 10; //length of array to avg accel over

bool gloveOn = true; //turn sensors on/off 
bool accelOn = true;        

//============================SETUP==========================================
void setup() {
  Serial.begin(115200);
  gloveSetup(); 
  accelSetup();
}

//===============================LOOP=====================================
void loop() {   
  Serial.print(100); //can use to check in ChucK
  Serial.print(",");

   //-----GLOVE---------------------------
  if (gloveOn == true){
    gloveReading();             //glove processing
  }
  //Serial.print("\t");
  if (gloveOn == true && accelOn == true){
    Serial.print(",");
  }

  //-----ACCEL----------------------------
  if (accelOn == true){        //accelerometer processing
    if (accel.available()){   //wait for new accel data    
      accel.read();
      accelPosition_Vel();      //accel (psuedo) position & velocity data  
      //printOrientation();     //orientation integer 
      //Serial.print("\t");
    }
  } 
  
  Serial.println();           //new line 
  delay(delayVal);            //for stability 
}
