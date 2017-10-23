/* Logs analog data onto txt file on SD card. 
 * data*4.9=mV; mV/0.83=mA
 * SD Card Setup:
 *    ----------------|
 *  / 1 2 3 4 5 6 7 8 | 
 *  | 9               |  
 *  
 *  1 to pin D4 (SS/CS)
 *  2 to pin D11 (MOSI)
 *  3 to GND
 *  4 to 3.3V
 *  5 to pin D13 (SCK/CK)
 *  7 to pin D12 (MISO)
 */
#include <SPI.h>
#include <SD.h>
#define trigPin 3
#define echoPin 2

  int chipSelect = 4;
    // One set of motors
  int enA = 10;   
  int inA1 = 8;  
  int inA2 = 7;
    // Other set of motors
  int enB = 9;
  int inB1 = 6;
  int inB2 = 5;

  
void setup() {
 
  //pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
    // set all the motor control pins to outputs  
  pinMode(enA, OUTPUT);  
  pinMode(enB, OUTPUT);  
  pinMode(inA1, OUTPUT);  
  pinMode(inA2, OUTPUT);  
  pinMode(inB1, OUTPUT);  
  pinMode(inB2, OUTPUT);

  
  Serial.print("Initialzing SD card..."); 
    //LED on pin D2 stays on if card fails or not present, 
    //LED blinks twice if everything OK
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while(1);
  }
  Serial.println("Card initialized.");
}


void loop() {
  // put your main code here, to run repeatedly:
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  motorForward();

  if(distance >= 200 || distance <= 0) {
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
    // create string to store data
  String dataString = "";
    //read voltage at pin A0 (sensorPin) and append to string
  //sensorValue = analogRead(sensorPin);
  dataString += String(distance);
 
    //open file
  File dataFile = SD.open("log.txt", FILE_WRITE);

    // if the file is available, write to it
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
      //print to serial port
    Serial.println(dataString);
    
    //delay(500);   //100ms delay after each write
  }
    // if the file isn't open, display error on serial, blink LED 2 times and stay on
  else {
    Serial.println("error opening datalog.txt"); 
  }
  if (distance <= 30) {
    motorOff();
    while(1);
  }
  delay(250);
}

void motorOff()  // turns motors off
{
      digitalWrite(inA1, LOW);  
      digitalWrite(inA2, LOW);    
      digitalWrite(inB1, LOW);  
      digitalWrite(inB2, LOW);
      analogWrite(enB, 0);
      analogWrite(enA, 0);     
}

void motorForward() // turns motor on in forward direction at certain speed
{
    // turn on motor side A  
  digitalWrite(inA1, HIGH);  
  digitalWrite(inA2, LOW); 
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enA, 255);  
    // turn on motor side B  
  digitalWrite(inB1, HIGH);  
  digitalWrite(inB2, LOW);  
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enB, 255);    
}


void motorBackward() // turns motor on in backwards direction at certain speed
{
    // turn on motor side A  
  digitalWrite(inA1, LOW);  
  digitalWrite(inA2, HIGH); 
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enA, 255);  
    // turn on motor side B  
  digitalWrite(inB1, LOW);  
  digitalWrite(inB2, HIGH);  
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enB, 255);
}


