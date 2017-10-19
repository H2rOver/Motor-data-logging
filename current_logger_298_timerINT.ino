/*  Drives motors forward using L298 H Bridges
 *  Logs analog onto txt file on SD card (data*4.9=mV, mV/0.83=mA)
 *  Logs every 100ms, stops everything after 5 seconds 
 *  
 *
 *
 *  LED status indicator:
 *    ON: SD card error
 *    2 blinks, ON: txt file error
 *    2 blinks, OFF: SD card is OK
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
 *
 *  For motor wiring, assume A to be one side, and B to the other side. 
*/
#include <TimerOne.h>
#include <SPI.h>
#include <SD.h>

  int sensorPin = A0;
  int sensorValue = 0;
  int LED = 2;
  int chipSelect = 4;
  int N = 0;      // increment for timer interrupt
    // One set of motors
  int enA = 10;   
  int inA1 = 9;  
  int inA2 = 8;
    // Other set of motors
  int enB = 5;
  int inB1 = 7;
  int inB2 = 6;

void setup(void)
{
  pinMode(LED, OUTPUT);
  pinMode(sensorPin, INPUT);
  
  // set all the motor control pins to outputs  
  pinMode(enA, OUTPUT);  
  pinMode(enB, OUTPUT);  
  pinMode(inA1, OUTPUT);  
  pinMode(inA2, OUTPUT);  
  pinMode(inB1, OUTPUT);  
  pinMode(inB2, OUTPUT);
  
  
  Timer1.initialize(100000);
  Timer1.attachInterrupt(logData); // logData to run every 0.1 seconds (10Hz)
  Serial.begin(9600);
  
  Serial.print("Initialzing SD card..."); 
    //LED on pin D2 stays on if card fails or not present, 
    //LED blinks twice if everything OK
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Card failed, or not present");
    digitalWrite(LED, HIGH);
    while(1);
  }
  Serial.println("Card initialized.");
  digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
  digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);
  
}


// The timer interrupt will log data to SD card every 250ms and stop everything after 5s
void logData(void)
{ 
  Timer1.stop();    // stop timer 
  if( N < 50 )
  {
          // create string to store data
    String dataString = "";
      //read voltage at pin A0 (sensorPin) and append to string
    sensorValue = analogRead(sensorPin);
    dataString += String(sensorValue);
 
      //open file
    File dataFile = SD.open("log.txt", FILE_WRITE);

      // if the file is available, write to it
    if (dataFile) 
    {
      dataFile.println(dataString);
      dataFile.close();
          //print to serial port
      Serial.println(dataString);
      //delay(100);   //100ms delay after each write
    }
    // if the file isn't open, display error on serial, blink LED 2 times and stay on
    else 
    {
      Serial.println("error opening datalog.txt");
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); 
    }
  }
  
  else if ( N = 50 )
  {
    noInterrupts();     // turns interrupts off 
      // now turn off motors  
    motorOff;
    while(1);           // infinite loop
  }
  
  N = N + 1;
  Timer1.start();
}


// The main program will drive the motors
void loop(void)
{ 
  motorForward(200); // turns on motor in forward direction with pwm duty cycle of 200/255
  while(1);    // infinite loop
}



void motorOff()  // turns motors off
{
      digitalWrite(inA1, LOW);  
      digitalWrite(inA2, LOW);    
      digitalWrite(inB1, LOW);  
      digitalWrite(inB2, LOW);     
}

void motorForward(int speed) // turns motor on in forward direction at certain speed
{
    // turn on motor side A  
  digitalWrite(inA1, HIGH);  
  digitalWrite(inA2, LOW); 
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enA, speed);  
    // turn on motor side B  
  digitalWrite(inB1, HIGH);  
  digitalWrite(inB2, LOW);  
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enB, speed);    
}


void motorBackward(int speed) // turns motor on in backwards direction at certain speed
{
    // turn on motor side A  
  digitalWrite(inA1, LOW);  
  digitalWrite(inA2, HIGH); 
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enA, speed);  
    // turn on motor side B  
  digitalWrite(inB1, LOW);  
  digitalWrite(inB2, HIGH);  
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enB, speed);    
}
