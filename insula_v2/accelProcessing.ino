//=================================ACCELEROMETER PROCESSING===============================
//---------------------------------GLOBAL VARIABLES --------------------------------------
int readingsX[accelArraySize];      // X the readings from the analog input
int accelIndexX = 0;                // the index of the current reading
int totalX = 0;                     // the running total
int readingsY[accelArraySize];      //Y
int accelIndexY = 0;             
int totalY = 0;                
int readingsZ[accelArraySize];       //Z
int accelIndexZ = 0;              
int totalZ = 0;     

int accelX = 0; //running avg of position of accel
int accelY = 0;
int accelZ = 0;

int accelVelx = 0; //velocity of acceleration
int accelVely = 0;
int accelVelz = 0;

int orientation; //orientation from accelerometer 

//----------------------------------SIGNAL PROCESSING----------------------------------------
void accelSetup(){
   accel.init();   // Default init. Set accelerometer with a full-scale range of +/-2g, and output data rate of 800 Hz 
   for (int thisReading = 0; thisReading < accelArraySize; thisReading++) {
    readingsX[thisReading] = 0; //intialzie readings in array to zero 
    readingsY[thisReading] = 0; 
    readingsZ[thisReading] = 0;
  }
}

//---------------------------------Accel Raw Data------------------------------
//12 bit, units of g's. 2nd value is # of decimals
void accelPosition_Vel(){
  //------------------Accel X ----------------------------------
  totalX = totalX - readingsX[accelIndexX]; //sub't last reading from total 
  readingsX[accelIndexX] = accel.cx*100; //read from sensor, adj for float --> int  
  totalX = totalX + readingsX[accelIndexX]; //add to total 
  if (accelIndexX >= accelArraySize-1) {
    accelVelx = (readingsX[accelIndexX]-readingsX[0])/accelArraySize; //corresponding last reading is the beginning of the array 
    accelIndexX = 0; //wrap to beginning of array if at end 
  }
  else{
    accelVelx = (readingsX[accelIndexX]-readingsX[accelIndexX+1])/accelArraySize; //corresponding last value is the next in array    
    accelIndexX ++; //advance
  }
  accelX = totalX / accelArraySize; //calc avg 
  
  //------------------Accel Y ----------------------------------
  totalY = totalY - readingsY[accelIndexY]; //sub't last reading from total 
  readingsY[accelIndexY] = accel.cy*100; //read from sensor, adj for float --> int  
  totalY = totalY + readingsY[accelIndexY]; //add to total 
  if (accelIndexY >= accelArraySize-1) {
    accelVely = (readingsY[accelIndexY]-readingsY[0])/accelArraySize; //corresponding last reading is the beginning of the array 
    accelIndexY = 0; //wrap to beginning of array if at end 
  }
  else{
    accelVely = (readingsY[accelIndexY]-readingsY[accelIndexY+1])/accelArraySize; //corresponding last value is the next in array    
    accelIndexY ++; //advance
  }
  accelY = totalY / accelArraySize; //calc avg 

//------------------Accel Z ----------------------------------
  totalZ = totalZ - readingsZ[accelIndexZ]; //sub't last reading from total 
  readingsZ[accelIndexZ] = accel.cz*100; //read from sensor, adj for float --> int  
  totalZ = totalZ + readingsZ[accelIndexZ]; //add to total 
  if (accelIndexZ >= accelArraySize-1) {
    accelVelz = (readingsZ[accelIndexZ]-readingsZ[0])/accelArraySize; //corresponding last reading is the beginning of the array 
    accelIndexZ = 0; //wrap to beginning of array if at end 
  }
  else{
    accelVelz = (readingsZ[accelIndexZ]-readingsZ[accelIndexZ+1])/accelArraySize; //corresponding last value is the next in array    
    accelIndexZ ++; //advance
  }
  accelZ = totalZ / accelArraySize; //calc avg 


// ------------------Accel Print --------------------------------------
  //position
  Serial.print(accelX);
  Serial.print(",");
  Serial.print(accelY);
  Serial.print(",");
  Serial.print(accelZ);
  Serial.print(",");
  
  //velocity (pseudo)
  Serial.print(accelVelx);
  Serial.print(",");
  Serial.print(accelVely);
  Serial.print(",");
  Serial.print(accelVelz);

}

//-------------------------------Accel Orientation ----------------------------
void printOrientation(){
  byte pl = accel.readPL();
  switch (pl){
  case PORTRAIT_U: //Portrait Up
    orientation = 0; 
    break;
  case PORTRAIT_D: //Portrait Down
    orientation = 1;
    break;
  case LANDSCAPE_R: //Landscape Right
    orientation = 2;
    break;
  case LANDSCAPE_L: //Landscape Left
    orientation = 3;
    break;
  case LOCKOUT: //Flat
    orientation = 4;
    break;
  }
  Serial.print(orientation);
}
