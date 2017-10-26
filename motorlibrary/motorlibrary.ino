/*
  PWM duty cycle 
  100% - 255
  75%  - 191
  50%  - 127
  25%  - 63
*/

// Left motors
  int enA = 10;   
  int inA1 = 8;  
  int inA2 = 7;
// Right motors
  int enB = 9;
  int inB1 = 6;
  int inB2 = 5;
  
  class MotorControl {

  public :MotorControl(){
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



void motorRight(int speed) // turns motor on in backwards direction at certain speed
{
    // turn on motor side A  
  digitalWrite(inA1, HIGH);  
  digitalWrite(inA2, LOW); 
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enA, speed);  
    // turn on motor side B  
  digitalWrite(inB1, LOW);  
  digitalWrite(inB2, HIGH);  
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enB, speed);    
}

void motorLeft(int speed) // turns motor on in backwards direction at certain speed
{
    // turn on motor side A  
  digitalWrite(inA1, LOW);  
  digitalWrite(inA2, HIGH); 
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enA, speed);  
    // turn on motor side B  
  digitalWrite(inB1, HIGH);  
  digitalWrite(inB2, LOW);  
    // set pwm to 'speed' out of possible range 0~255  
  analogWrite(enB, speed);    
}
};

void setup(void)
{

  // set all the motor control pins to outputs  
  pinMode(enA, OUTPUT);  
  pinMode(enB, OUTPUT);  
  pinMode(inA1, OUTPUT);  
  pinMode(inA2, OUTPUT);  
  pinMode(inB1, OUTPUT);  
  pinMode(inB2, OUTPUT);
  
  

  Serial.begin(9600);
  
}


void loop(void)
{ 
  MotorControl test;
  
  test.motorForward(127); 	// turns on motor in forward direction with pwm duty cycle of 200/255
  while(1);    			// infinite loop
}


