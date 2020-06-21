
/* Luminance meter
 calculate the light and display it by control the servo precisely.
 Also, you can see the specific value of brightness on the lcd screen.
 The range is 0 to 180. The greater the value, the lower the brightness.
 And you can push the button to launch another function.
 if it's dark, then the LED is on and the buzzer rings periodically.
*/

#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

Servo myservo;

const int Apin=1;
const int Dpin=2;
const int tonepin=6;
const int buttonPin=8;
const int servopin=9;
int buttonState=0; 
 
 
void setup() 
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(tonepin, OUTPUT);
  pinMode(buttonPin, INPUT);
  myservo.attach(servopin);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
}
 
void loop() 
{
  int i;
  int analog_val,digital_val,val;
  analog_val=analogRead(Apin);
  Serial.print("analog value:");
  Serial.print(analog_val);
  Serial.print(", d:");
  digital_val = digitalRead(Dpin); //1:dark  0:light
  Serial.println(digital_val);
  
  buttonState = digitalRead(buttonPin);
  val = map(analog_val,0,1024,0,180);
  myservo.write(val); //servo gets value
  lcd.print("Brightness:");
  lcd.write(val);
  delay(1000);
  
  // if push the button
  if (buttonState == HIGH){
    // it's dark
    if (digital_val == 1){
      digitalWrite(LED_BUILTIN, HIGH); // turn on the led
        // ring periodically
        for(i=0;i<10;i++)
        {
          digitalWrite(tonepin,HIGH);
          delay(10);
          digitalWrite(tonepin,LOW);
          delay(10);
        }
      
    }else{
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(tonepin,LOW);
    }
    delay(100);
  }else{  
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(tonepin,LOW);  
  }
  lcd.clear();
}
