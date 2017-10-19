/* Logs analog data onto txt file on SD card. 
 * data*4.9=mV; mV/0.83=mA
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

 */
#include <SPI.h>
#include <SD.h>

  int sensorPin = A0;
  int sensorValue = 0;
  int LED = 2;
  int chipSelect = 4;
  
void setup() {
 
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  
  Serial.print("Initialzing SD card..."); 
    //LED on pin D2 stays on if card fails or not present, 
    //LED blinks twice if everything OK
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    digitalWrite(LED, HIGH);
    while(1);
  }
  Serial.println("Card initialized.");
  digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
  digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);
}


void loop() {
    // create string to store data
  String dataString = "";
    //read voltage at pin A0 (sensorPin) and append to string
  sensorValue = analogRead(sensorPin);
  dataString += String(sensorValue);
 
    //open file
  File dataFile = SD.open("log.txt", FILE_WRITE);

    // if the file is available, write to it
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
      //print to serial port
    Serial.println(dataString);
    delay(100);   //100ms delay after each write
  }
    // if the file isn't open, display error on serial, blink LED 2 times and stay on
  else {
    Serial.println("error opening datalog.txt");
    digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
    digitalWrite(LED,HIGH); delay(500); digitalWrite(LED,LOW);  delay(500);
    digitalWrite(LED,HIGH); 
  }
}
