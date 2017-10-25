
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

  int sensorPin0 = A0;		// motor 0
  int sensorValue0 = 0;
  
  int sensorPin1 = A1;		// motor 1
  int sensorValue1 = 0;
  
  int LED = 2;				// error indicator 
  
  int chipSelect = 4;
  
  int N = 0;      // increment for timer interrupt
    // One set of motors
  int enA = 10;   
  int inA1 = 8;  
  int inA2 = 7;
    // Other set of motors
  int enB = 9;
  int inB1 = 6;
  int inB2 = 5;

void setup(void)
{
  pinMode(LED, OUTPUT);
  pinMode(sensorPin0, INPUT);
  pinMode(sensorPin1, INPUT);
  
  // set all the motor control pins to outputs  
  pinMode(enA, OUTPUT);  
  pinMode(enB, OUTPUT);  
  pinMode(inA1, OUTPUT);  
  pinMode(inA2, OUTPUT);  
  pinMode(inB1, OUTPUT);  
  pinMode(inB2, OUTPUT);
  
  

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
  digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);			// blink LED twice if SD card is OK

   Timer1.initialize(100000);		// cause timer interrupt every 0.1 seconds (10Hz)
   Timer1.attachInterrupt(logData); // run logData function when timer interrupt goes off
}


void loop(void)
{ 
  motorForward(255); 	// turns on motor in forward direction with pwm duty cycle of 200/255
  while(1);    			// infinite loop
}


// The timer interrupt will log data to SD card every 100ms and stop everything after 5s
void logData(void)
{ 
  if( N < 50 )
  {  
  
  // writes motor 0 data to log0.txt
          // create string to store data
    String dataString0 = "";
      //read voltage at pin A0 (sensorPin) and append to string
    sensorValue0 = analogRead(sensorPin0);
    dataString0 += String(sensorValue0);
 
      //open file
    File dataFile0 = SD.open("log0.txt", FILE_WRITE);

      // if the file is available, write to it
    if (dataFile0) 
    {
      dataFile0.println(dataString0);
      dataFile0.close();
          //print to serial port
      Serial.println(dataString0);
    }
    // if the file isn't open, display error on serial, blink LED 2 times and stay on
    else 
    {
      Serial.println("error opening log0.txt");
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); 
    }
  
  
  // writes motor 1 data to log1.txt
          // create string to store data
    String dataString1 = "";
      //read voltage at pin A1 (sensorPin1) and append to string
    sensorValue1 = analogRead(sensorPin1);
    dataString1 += String(sensorValue1);
 
      //open file
    File dataFile1 = SD.open("log1.txt", FILE_WRITE);

      // if the file is available, write to it
    if (dataFile1) 
    {
      dataFile1.println(dataString1);
      dataFile1.close();
          //print to serial port
      Serial.println(dataString1);
    }
    // if the file isn't open, display error on serial, blink LED 2 times and stay on
    else 
    {
      Serial.println("error opening log1.txt");
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); 
    }
  }
  
  else if ( N = 50 ) // after 6 seconds, turns off motors, writes "DONE" to end of log0 and log1
    { 
      // now turn off motors  
    motorOff();
	
    // write "DONE" on sd card and serial monitor to differentiate data 
    String doneString = "DONE";
    
	File dataFile0 = SD.open("log0.txt", FILE_WRITE);
    if (dataFile0) 
    { dataFile0.println("DONE");
      dataFile0.close();
      Serial.println("DONE"); }
    else 
    { Serial.println("error opening log0.txt");
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); }
	  
	File dataFile1 = SD.open("log1.txt", FILE_WRITE);
    if (dataFile1) 
    { dataFile1.println("DONE");
      dataFile1.close();
      Serial.println("DONE"); }
    else 
    { Serial.println("error opening log1.txt");
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
      digitalWrite(LED,HIGH); }   
	
    while(1);           // infinite loop
  
	}
  
  N = N + 1;		// increment N, to avoid using multiple timer interrupts
  Serial.print("N= "); Serial.println(N); // display N in serial monitor for debugging purposes
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

void motorForward(int speed) // turns motor on in forward direction at certain speed
{
    // turn on motor side A  
  digitalWrite(inA1, HIGH);  
  digitalWrite(inA2, LOW); 
    // turn on motor side B  
  digitalWrite(inB1, HIGH);  
  digitalWrite(inB2, LOW);  
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enA, speed);
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
