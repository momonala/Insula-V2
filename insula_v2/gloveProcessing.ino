//========================GLOVE PROCESSING=====================================
//------------------------GLOBAL VARIABLES-------------------------------------
int normalize0; //normalize glove vals
int normalize1;
int normalize2;
int normalize3;

int max0 = 100; //max readings from glove, seeded
int max1 = 100;
int max2 = 100;
int max3 = 100;

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

//----------------------------SIGNAL PROCESSING --------------------------------------------

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


void gloveReading(){
  //---------------------SENSOR 0--------------------
  ar0[readIndex0] = analogRead(sensorPin0)-normalize0;  // read from sensor 0, normalize, add value to current index
  if (ar0[readIndex0] > max0){
    max0 = ar0[readIndex0]; //keep track of highest value
  }
  ar0[readIndex0] = map(ar0[readIndex0], 0, max0, 0, 127); //map value to MIDI 7 bit
  if (readIndex0 >= veloArraySize-1) {   // if we're at the end of the array...
    velocity0 = (ar0[readIndex0]-ar0[0])/veloArraySize; //corresponding last reading is the beginning of the array 
    readIndex0 = 0; // ...wrap around to the beginning, index 0
  }
  else{
    velocity0 = (ar0[readIndex0]-ar0[readIndex0+1])/veloArraySize; //corresponding last value is the next in array 
    readIndex0 += 1; // advance to the next position in the array:
  }
  
  //-----------------------SENSOR 1-----------------
  ar1[readIndex1] = analogRead(sensorPin1)-normalize1;  // read from sensor 1
  if (ar1[readIndex1] > max1){
    max1 = ar1[readIndex1]; 
  }
  ar1[readIndex1] = map(ar1[readIndex1], 0, max1, 0, 127);
  if (readIndex1 >= veloArraySize-1) {   
    readIndex1 = 0; 
  }
  else{
    velocity1 = (ar1[readIndex1]-ar1[readIndex1+1])/veloArraySize; 
    readIndex1 += 1; 
  }

  //------------------------SENSOR 2------------------
  ar2[readIndex2] = analogRead(sensorPin2)-normalize2; 
  if (ar2[readIndex2] > max2){
    max2 = ar2[readIndex2]; //keep track of highest value
  }
  ar2[readIndex2] = map(ar2[readIndex2], 0, max2, 0, 127);
  if (readIndex2 >= veloArraySize-1) { 
    readIndex2 = 0; 
  }
  else{
    velocity2 = (ar2[readIndex2]-ar2[readIndex2+1])/veloArraySize; 
    readIndex2 += 1; 
  }
  
  //-----------------------SENSOR 3-------------------
  ar3[readIndex3] = analogRead(sensorPin3)-normalize3; 
  if (ar3[readIndex3] > max3){
    max3 = ar3[readIndex3]; //keep track of highest value
  }
  ar3[readIndex3] = map(ar3[readIndex3], 0, max3, 0, 127);
  if (readIndex3 >= veloArraySize-1) {  
    readIndex3 = 0; 
  }
  else{
    velocity3 = (ar3[readIndex3]-ar3[readIndex3+1])/veloArraySize; 
    readIndex3 += 1; 
  }

  //-------------------------GLOVE PRINTING----------------------------
  Serial.print(ar0[readIndex0]); //sensor value 0 adjusted. send to serial  
  Serial.print(",");
  Serial.print(velocity0);
  Serial.print(",");
  
  Serial.print(ar1[readIndex1]); //sensor value 0 adjusted. send to serial  
  Serial.print(",");
  Serial.print(velocity1);
  Serial.print(",");
  
  Serial.print(ar2[readIndex2]); //sensor value 0 adjusted. send to serial  
  Serial.print(",");
  Serial.print(velocity2);
  Serial.print(",");
  
  Serial.print(ar3[readIndex3]); //sensor value 0 adjusted. send to serial  
  Serial.print(",");
  Serial.print(velocity3);
}
