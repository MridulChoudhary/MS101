//int LDR_Val = 0;     /*Variable to store photoresistor value*/
//int sensor =A0;  /*Analogue pin for photoresistor*/
int counter=0;
//int led= 9; 
float tachTime;
float previousmicros;
double rps;

int buttonPushCounter = 0;  // counter for the number of logic changes
int buttonState = 0;        // current state of the ldr_digital_logic_detection
int lastButtonState = 0;    // previous state of the ldr_logic_detection
const int buttonPin = 7;    //ldr digital pin connection

/////
int MOTOR_PWMPIN=6;
// Arduino digital pins for motor direction
int MOTOR_PLUS=10;
int MOTOR_MINUS=11;
double Set_Speed=1;
float Pwm_Value;
float p1=0;
float error;
double prev_error=0;
float de;
float diff;
float integral=0;
float kp=10;float kd=5; float ki=3;

void setup() {
    Serial.begin(9600);     /*Baud rate for serial communication*/
  //  pinMode(led, OUTPUT);   /*LED Pin set as output */
    pinMode(buttonPin, INPUT);
    pinMode(MOTOR_PWMPIN, OUTPUT);
    pinMode(MOTOR_PLUS, OUTPUT);
    pinMode(MOTOR_MINUS, OUTPUT);
    digitalWrite(MOTOR_PLUS, HIGH);
    digitalWrite(MOTOR_MINUS, LOW); 
    
}
void loop() 
        { 
             buttonState = digitalRead(buttonPin);
            //LDR_Val = analogRead(sensor);   /*Analog read LDR value*/
             //digitalWrite(led,HIGH);
      
             if (buttonState != lastButtonState)
             {  
                if (buttonState == HIGH)  //detecting rising edge
                {
                 buttonPushCounter++;
                } 
        
             }

          lastButtonState = buttonState;
          counter=counter+1;
          //delayMicroseconds(800);
          delay(5);  // This gives enough time for calc of rps
          //Serial.println(counter);
          if(counter%100==0)
          {
              //Serial.println(buttonPushCounter);
              tachTime = millis() - previousmicros;
              previousmicros = millis();
              rps = float(buttonPushCounter*1000)/(8*tachTime);//RPS CALC
             
             //  Serial.println("rps=");
             error=Set_Speed-rps;
             integral=integral+error;
  //          if(integral>50)
  //            integral=50;
  //          if(integral<-50)
  //            integral=-50;
             de=error-prev_error;
             diff=de;

            Pwm_Value=kp*error+ki*(integral)+kd*diff; //PID Calculation
            p1=Pwm_Value;
            prev_error=error;
            if(p1>255)
             { p1=255;} //limiting pwm value when it goes beyond the 255
        
            analogWrite(MOTOR_PWMPIN,p1);//In generalPWM VALUES of 55->3v,90->6v ,140->8v (out1-out2)V
            counter=0; //reset count for every 100 loops
            buttonPushCounter=0;//reset buttonPushCounter for every 100 loops
          }
          
      Serial.print(p1);
      Serial.print(" ");
      Serial.println(rps); 
    }
