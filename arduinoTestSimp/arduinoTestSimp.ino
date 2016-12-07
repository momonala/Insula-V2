int sensorValue0;
int sensorValue1;
int sensorValue2;
int sensorValue3;
int sensorValue4;
byte myArray[] = {1, 2, 3, 4};

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1); //this will make the Arduino wait a max of only 100ms per incoming set of serial data, before moving on
}
void loop() {
  
  sensorValue0 = analogRead(A0);
  sensorValue1 = analogRead(A1);
  sensorValue2 = analogRead(A2);
  sensorValue3 = analogRead(A3);
  
  //Serial.print("S"); //UNCOMMENT FOR WRITING WITH PROCESSING
  Serial.print(sensorValue0);
  Serial.print(',');
  Serial.print(sensorValue1);
  Serial.print(',');
  Serial.print(sensorValue2);
  Serial.print(',');
  Serial.println(sensorValue3);
  
  //Serial.write(myArray, sizeof(myArray));
  delay(10);
  //Serial.flush();
}
