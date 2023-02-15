#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>
#include <LiquidCrystal.h>
MPU6050 mpu; 
Servo myservo;
Servo ESC;
int error;
int potValue;
int u;
float Kp=0.4;
//float Ki=0.005;
float Kd=0.09;
float pid_p;
float pid_d;
bool ctrl= HIGH;
//float pid_i;
float time;
float timePrev;
float elapsedTime;
int Prevroll;
int control = 13;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(13, INPUT);
  lcd.begin(16, 2);
  Serial.begin(115200);
   time=millis();
  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

   myservo.attach(10);  // attaches the servo on pin 9 to the servo object
    ESC.attach(9,1000,2000);
    
}

void loop() {
  // Read normalized values 
  Vector normAccel = mpu.readNormalizeAccel();
     // Calculate Pitch & Roll
   timePrev=time;
  time=millis();
  elapsedTime=(time-timePrev)/1000;
  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  int roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
  
  
     if(digitalRead(control) == HIGH){
          delay(100);
          if(digitalRead(control) == HIGH){
            if(ctrl){
            ctrl=LOW;
            }
            else
            {
              ctrl=HIGH;
            }
          }
     }
  potValue = analogRead(A0);
  potValue = map(potValue, 0, 1023, 0, 180);
  ESC.write(potValue);
  roll=roll-1;
  pid_p=Kp*roll;
  pid_d=Kd*((roll-Prevroll)/elapsedTime);
  //pid_i=pid_i+(Ki*roll);
   lcd.setCursor(0,0);
   lcd.print("ANGLE:");
   lcd.setCursor(7,0);
   lcd.print(roll);
  
    if (ctrl) {
         lcd.setCursor(0,1);
        lcd.print("CONTROL:ON");
       //u=pid_p+pid_d+pid_i;
       u=pid_p+pid_d;
    }
    
  
    if (!ctrl) {
      lcd.setCursor(0,1);
      lcd.print("CONTROL:OFF");
      u=0;
    }
  
  myservo.write(u+90);

   Serial.print(" roll = ");
  Serial.print(roll);
  Serial.print(" PrevRoll= ");
  Serial.print(Prevroll);
  //Serial.print(" Roll = ");
  //Serial.print(roll);
  //Serial.print(" error = ");
  //Serial.print(error);
  Serial.print(" u = ");
  Serial.print(u);
   Serial.print(" time: ");
  Serial.print(time);
   Serial.print(" prevtime:");
  Serial.print(timePrev);
  Serial.println();
  
  
  Prevroll=roll;

  
 delay(30);
 lcd.clear();

}
