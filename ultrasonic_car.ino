
#include <Servo.h> 
int pinLB=2;     // 
int pinLF=4;     // 

int pinRB=5;    // 
int pinRF=7;    // 

int speedPinA=6;
int speedPinB=3;

int inputPin = 13;  //ultrasonic echo 
int outputPin =12;  // ultrasonic trigger

int Fspeedd = 0;      // front distance
int Rspeedd = 0;      // right distance
int Lspeedd = 0;      // left distance
int directionn = 0;   //  determine direction of turn
Servo myservo;        // myservo
int delay_time = 250; // stable steering servo motor


int M_SPD = 150;     // Motor speed: (255 MAX)

int Fgo = 8;         // advance
int Rgo = 6;         // turn right
int Lgo = 4;         // turn left
int Bgo = 2;         // back

//Servo Motor alignment - Align servo motor
int AngleFront = 90;
int AngleRight = 135;
int AngleLeft = 45;

void setup()
 {
  Serial.begin(115200);     // 
  pinMode(pinLB,OUTPUT); // 
  pinMode(pinLF,OUTPUT); //
  pinMode(pinRB,OUTPUT); // 
  pinMode(pinRF,OUTPUT); //
  
  pinMode(speedPinA,OUTPUT);
  pinMode(speedPinB,OUTPUT);
  
  pinMode(inputPin, INPUT);    // Define ultrasound input pin
  pinMode(outputPin, OUTPUT);  // Define ultrasound output pin   

  myservo.attach(8);    // Define the servo motor output pin8 (PWM)
 }
void advance(int a)     // advance
    {                         // In the mid-point of the two wheels as a reference
     digitalWrite(speedPinA,M_SPD);
     digitalWrite(speedPinB,M_SPD);
     digitalWrite(pinRB,HIGH);  // right wheell advance
     digitalWrite(pinRF,LOW);  
     digitalWrite(pinLB,HIGH);  // left wheel advance
     digitalWrite(pinLF,LOW);
     delay(a * 100);     
    }

void left(int b)        //turn right (single wheel)
    {
     digitalWrite(speedPinA,LOW);
     digitalWrite(speedPinB,HIGH);
     digitalWrite(pinRB,HIGH);   //right stop
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH); //left advance
     digitalWrite(pinLF,LOW);
     delay(b * 100);
    }
void right(int c)         //turn left (single wheel)
    {
     digitalWrite(speedPinA,HIGH);
     digitalWrite(speedPinB,LOW);
     digitalWrite(pinRB,HIGH); //right wheel advance
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH);   //left stop
     digitalWrite(pinLF,LOW);
     delay(c * 100);
    }
void turnL(int d)        //turn right (double wheel)
    {
     digitalWrite(speedPinA,LOW);
     digitalWrite(speedPinB,HIGH);
     digitalWrite(pinRB,HIGH);  //right wheel back
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,LOW);  // left wheel advance
     delay(d * 100);
    }
void turnR(int e)        //turn left (double wheel)
    {
     digitalWrite(speedPinA,HIGH);
     digitalWrite(speedPinB,LOW);
     digitalWrite(pinRB,HIGH); //right wheel advance
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH);   //left stop
     digitalWrite(pinLF,LOW);
     delay(e * 100);
    }    
void stopp(int f)         //stop
    {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,LOW);
     delay(f * 100);
    }
void back(int g)          //back
    {
     digitalWrite(speedPinA,HIGH);
     digitalWrite(speedPinB,HIGH);
     digitalWrite(pinRB,LOW);  //right wheel back
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);  //left wheel back
     digitalWrite(pinLF,HIGH);
     delay(g * 100);     
    }
    
void detection()        //Measuring three angles (2.90.178)
    {   
     int delay_time = 250;    //waiting for servo motor stable 
     ask_pin_F();            // Read the distance of front
      
     if(Fspeedd < 15)         // if distance is less than 20cm in front
      {
         stopp(1);               //  clear output, motor stop
         back(2);                // 
      }
           
      if(Fspeedd < 30)         // if distance is less than 25cm in front
      {
         stopp(1);               //  clear output, motor stop   
         ask_pin_L();            //  measure left
         delay(delay_time);      //
         ask_pin_R();            //  measure right
         delay(delay_time);      // 
         myservo.write(AngleFront);
        
         if(Lspeedd > Rspeedd)   // compare distance of right and left
         {
            directionn = Lgo;      // turn left
         }
        
         if(Lspeedd <= Rspeedd)   //
         {
            directionn = Rgo;      // turn right
         } 
        
        if (Lspeedd < 15 && Rspeedd < 15)   //
        {
         directionn = Bgo;      //  reverse      
        }          
      }
      else                      //   
      {
        directionn = Fgo;        //    keep forward
      }
     
    }    
void ask_pin_F()   //  
    {
      myservo.write(AngleFront);
      digitalWrite(outputPin, LOW);   // 
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // 
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // 
      
      float duration = pulseIn(inputPin, HIGH);  // 
      float Fdistance= duration/29/2;       // 
      
      Serial.print("F distance:");      //
      Serial.println(Fdistance);         //
      Fspeedd = Fdistance;              // 
    }  
 void ask_pin_L()   //
    {
      myservo.write(AngleLeft);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   // 
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // 
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // 
      
      float duration = pulseIn(inputPin, HIGH);  // 
      float Ldistance= duration/29/2;       // 
      
      Serial.print("L distance:");       //
      Serial.println(Ldistance);         //
      Lspeedd = Ldistance;              // 
    }  
void ask_pin_R()   //  
    {
      myservo.write(AngleRight);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   //
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // 
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // 
      
      float duration = pulseIn(inputPin, HIGH);  // 
      float Rdistance= duration/29/2;       // 
      
      Serial.print("R distance:");       //
      Serial.println(Rdistance);         //
      Rspeedd = Rdistance;              // 
    }
void testSonic() //function to test the ultrasonic Distance Sensor
    {
      digitalWrite(outputPin, LOW);   // 
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // 
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // 
      
      float duration = pulseIn(inputPin, HIGH);  //   
      // The speed of sound is 340 m/s or 29 microseconds per centimeter.
      // The ping travels out and back, so to find the distance of the
      // object we take half of the distance travelled.
      float Fdistance= duration/29/2;       // 
      
      if (Fdistance < 10)
      {
         Serial.print("F distance LOW:");      //
         delayMicroseconds(10);
      }
     
      if (Fdistance > 10 && Fdistance < 20)
      {
         Serial.print("F distance MED:");      //
         delayMicroseconds(10);
      }
      
      
      if (Fdistance > 20 && Fdistance < 40)
      {
         Serial.print("F distance HIGH:");      //
         delayMicroseconds(10);
      }
      
      Serial.print("F distance:");      //
      Serial.println(Fdistance);         //
      Fspeedd = Fdistance;              // 
    }
      
    
void loop()
 {
    myservo.write(AngleFront);  //
    detection();        // Range detection and direction finder
    //testSonic();         // Ultrasonic Test function

      
   if(directionn == 2)  //             
   {
     back(8);                    //  
     turnL(2);                   //
     Serial.print(" Reverse ");   //
   }
   if(directionn == 6)           //    
   {
     back(1); 
     turnR(6);                   // 
     Serial.print(" Right ");    //
   }
   if(directionn == 4)          //    
   {  
     back(1);      
     turnL(6);                  // 
     Serial.print(" Left ");     //   
   }  
   if(directionn == 8)          //      
   { 
    advance(1);                 //   
    Serial.print(" Advance ");   //
    Serial.print("   ");    
   }
 }


