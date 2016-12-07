#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library
MMA8452Q accel; // create an instance of the MMA8452Q class called 'accel'

const int delayVal = 10;       //up here for convenience  
const int veloArraySize = 5; //length of array to hold velocity over  
const int accelArraySize = 10; //length of array to avg accel over
bool gloveOn = true; 
bool accelOn = true;    // turn accelerometer on or of 

int normalize0 = 0;
int normalize1 = 0;
int normalize2 = 0;
int normalize3 = 0;

int sensorPin0 = A0;          // Sensor 0: select the input pin
int ar0[veloArraySize];       // the readings from  analog input (ar = analogRead)
int readIndex0 = 0;           // index of  current reading
int velocity0 = 0;            // velocity of the finger contraction
int sensorPin1 = A1;          // Sensor 1
int ar1[veloArraySize];    
int readIndex1 = 0;              
int velocity1 = 0;
int sensorPin2 = A2;       // Sensor 2
int ar2[veloArraySize];     
int readIndex2 = 0;
int velocity2 = 0;              
int sensorPin3 = A3;       // Sensor 3
int ar3[veloArraySize];    
int readIndex3 = 0;
int velocity3 = 0;

int readingsX[accelArraySize];      // X the readings from the analog input
int accelIndexX = 0;                // the index of the current reading
int totalX = 0;                     // the running total
int readingsY[accelArraySize];      //Y
int accelIndexY = 0;             
int totalY = 0;                
int readingsZ[accelArraySize];       //Z
int accelIndexZ = 0;              
int totalZ = 0;     

int accelX = 0; //avg of X
int accelY = 0;
int accelZ = 0;

//============================SETUP==========================================
void setup() {
  Serial.begin(115200);
  gloveSetup(); 
  accelSetup();
}

void gloveSetup(){
   for (int thisReading = 0; thisReading < veloArraySize; thisReading++) {
    ar0[thisReading] = 0; //intialzie readings in array to zero 
    ar1[thisReading] = 0; 
    ar2[thisReading] = 0; 
    ar3[thisReading] = 0; 
  }
  normalize0 = analogRead(sensorPin0); //normalize rest values 
  normalize1 = analogRead(sensorPin1);
  normalize2 = analogRead(sensorPin2);
  normalize3 = analogRead(sensorPin3);
}

void accelSetup(){
   accel.init();   // Default init. Set accelerometer with a full-scale range of +/-2g, and output data rate of 800 Hz 
   for (int thisReading = 0; thisReading < accelArraySize; thisReading++) {
    readingsX[thisReading] = 0; //intialzie readings in array to zero 
    readingsY[thisReading] = 0; 
    readingsZ[thisReading] = 0;
  }
}

//===============================LOOP=====================================
void loop() {
  if (gloveOn == true){
    gloveReading();             //glove processing
  }
  //Serial.print("\t");
  if (accelOn == true){        //accelerometer processing
    if (accel.available()){   //wait for new accel data    
      accel.read();
      Serial.print(",");
      printCalculatedAccels();  //analog, processed accel data 
      //Serial.print("\t");
      //printOrientation();       //orientation data
    }
  }
  Serial.println();           //new line 
  delay(delayVal);            //for stability 
}

//========================GLOVE PROCESSING=====================================
void gloveReading(){
  //---------------------SENSOR 0--------------------
  ar0[readIndex0] = analogRead(sensorPin0)-normalize0;  // read from sensor 0, normalize, add value to current index 
  Serial.print(ar0[readIndex0]); //sensor value 0 adjusted. send to serial 
  Serial.print(",");
  if (readIndex0 >= veloArraySize-1) {   // if we're at the end of the array...
    velocity0 = (ar0[readIndex0]-ar0[0])/veloArraySize; //corresponding last reading is the beginning of the array 
    readIndex0 = 0; // ...wrap around to the beginning, index 0
  }
  else{
    velocity0 = (ar0[readIndex0]-ar0[readIndex0+1])/veloArraySize; //corresponding last value is the next in array 
    readIndex0 += 1; // advance to the next position in the array:
  }
  Serial.print(velocity0);
  Serial.print(",");
  

  //-----------------------SENSOR 1-----------------
  ar1[readIndex1] = analogRead(sensorPin1)-normalize1;  // read from sensor 1
  Serial.print(ar1[readIndex1]); //sensor value 1 adjusted. send to serial 
  Serial.print(",");  
  if (readIndex1 >= veloArraySize-1) {   
    readIndex1 = 0; 
  }
  else{
    velocity1 = (ar1[readIndex1]-ar1[readIndex1+1])/veloArraySize; 
    readIndex1 += 1; 
  }
  Serial.print(velocity1);
  Serial.print(",");

  //------------------------SENSOR 2------------------
  ar2[readIndex2] = analogRead(sensorPin2)-normalize2;  // read from sensor 2
  Serial.print(ar2[readIndex2]); //sensor value 2 adjusted. send to serial
  Serial.print(","); 
  if (readIndex2 >= veloArraySize-1) { 
    readIndex2 = 0; 
  }
  else{
    velocity2 = (ar2[readIndex2]-ar2[readIndex2+1])/veloArraySize; 
    readIndex2 += 1; 
  }
  Serial.print(velocity2);
  Serial.print(",");
  
  //-----------------------SENSOR 3-------------------
  ar3[readIndex3] = analogRead(sensorPin3)-normalize3;  // read from sensor 3
  Serial.print(ar3[readIndex3]); //sensor value 2 adjusted. send to serial
  Serial.print(",");
  if (readIndex3 >= veloArraySize-1) {  
    readIndex3 = 0; 
  }
  else{
    velocity3 = (ar3[readIndex3]-ar3[readIndex3+1])/veloArraySize; 
    readIndex3 += 1; 
  }
  Serial.print(velocity3);

}

//=================================ACCELEROMETER PROCESSING===============================
//---------------------------------Accel Raw Data------------------------------
//12 bit, units of g's. 2nd value is # of decimals
void printCalculatedAccels(){
  //------------------Accel X ----------------------------------
  totalX = totalX - readingsX[accelIndexX]; //sub't last reading from total 
  readingsX[accelIndexX] = accel.cx*100; //read from sensor, adj for float --> int  
  totalX = totalX + readingsX[accelIndexX]; //add to total 
  accelIndexX ++; //advance
  if (accelIndexX >= accelArraySize) {
    accelIndexX = 0; //wrap to beginning of array if at end 
  }
  accelX = totalX / accelArraySize; //calc avg 
  Serial.print(accelX);
  Serial.print(",");
  //------------------Accel Y ----------------------------------
  totalY = totalY - readingsY[accelIndexY]; //sub't last reading from total 
  readingsY[accelIndexY] = accel.cy*100; //read from sensor, adj for float --> int  
  totalY = totalY + readingsY[accelIndexY]; //add to total 
  accelIndexY ++; //advance
  if (accelIndexY >= accelArraySize) {
    accelIndexY = 0; //wrap to beginning of array if at end 
  }
  accelY = totalY / accelArraySize; //calc avg 
  Serial.print(accelY);
  Serial.print(",");
//------------------Accel Z ----------------------------------
  totalZ = totalZ - readingsZ[accelIndexZ]; //sub't last reading from total 
  readingsZ[accelIndexZ] = accel.cz*100; //read from sensor, adj for float --> int  
  totalZ = totalZ + readingsZ[accelIndexZ]; //add to total 
  accelIndexZ ++; //advance
  if (accelIndexZ >= accelArraySize) {
    accelIndexZ = 0; //wrap to beginning of array if at end 
  }
  accelZ = totalZ / accelArraySize; //calc avg 
  Serial.print(accelZ);
}

//-------------------------------Accel Orientation ----------------------------
void printOrientation(){
  byte pl = accel.readPL();
  switch (pl){
  case PORTRAIT_U:
    Serial.print("Portrait Up");
    break;
  case PORTRAIT_D:
    Serial.print("Portrait Down");
    break;
  case LANDSCAPE_R:
    Serial.print("Landscape Right");
    break;
  case LANDSCAPE_L:
    Serial.print("Landscape Left");
    break;
  case LOCKOUT:
    Serial.print("Flat");
    break;
  }
}

