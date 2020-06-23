#include <CurieTime.h>
#include <LiquidCrystal_I2C.h>
#include "pitch.h"

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 7              
OneWire oneWire(ONE_WIRE_BUS);       
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27,16,2);


const int tonepin =  6 ;
// detect tablet 
const int A_pin1 = 2 ;
const int D_pin1 = 2 ;
// detect coca-cola
const int A_pin2 = 3 ;
const int D_pin2 = 3 ;

const int buttonPin = 4 ;
const int red_led = 13 ;


int buttonState = 0;         // variable for reading the pushbutton status
int pillState = 1 ;
int awardState = 1 ;
int ring = 0 ;

// change this to make the song slower or faster
int tempo = 80;




// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // The Godfather theme
  // Score available at https://musescore.com/user/35463/scores/55160

  REST, 4, REST, 8, REST, 8, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //1
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8,
  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,
  
  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8, 
  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,

  NOTE_A4, 2, REST,8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8, //9
  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, 
  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8, 
   
  NOTE_E4, 2, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //13

  //repeats from 2
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8, //2
  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,
  
  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8, 
  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,

  NOTE_A4, 2, REST,8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8, //9
  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, 
  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8, 
   
  NOTE_E4, 2 //13
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;



void setup()
{
	Serial.begin(9600);
  //while(!Serial) ;
	pinMode(tonepin,OUTPUT);
	pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  sensors.begin(); 
  sensors.setWaitForConversion(false);
	// set the current time   hour:min:sec:day:month:year
	setTime(13,8,45,23,6,2020);
	// init lcd screen
	lcd.init();
	lcd.backlight();
}





void loop()
{   //print time to the screen
	  lcd.setCursor(0,0);
	  lcd.print("Hello,Mr ZHANG!");
    lcd.setCursor(0,1);
    lcd.print(hour());
    lcd.print(":");
    lcd.print(minute());
    lcd.print(":");
    lcd.print(second());
    lcd.print(" T:");
    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempC = sensors.getTempCByIndex(0);
   // Check if reading was successful
    if(tempC != DEVICE_DISCONNECTED_C) 
    {    lcd.print(tempC);

    }
    else{}
    if (hour()==13 && minute()==9 && second() == 0)
    {
        ring = 1 ;
        lcd.setCursor(0,0);
        lcd.print("Wake up!");
        Serial.print("Wake Up!");
        delay(1000);   
        play(); // play 
    }

      // press the button, shut the alarm clock.
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH && ring == 1) 
      {
        Serial.print("Pressed button");
        lcd.setCursor(0,0);
        lcd.print("Take pills!Sir");
        noTone(tonepin);
      }
      else{}
      if(ring == 1 )
      {
        // pills state  1：没拿  ；  0：拿了  初始状态：都是1
        pillState = digitalRead(D_pin1) ;
        // award state 
        awardState = digitalRead(D_pin2);
        // 拿走了可乐，没有吃药
        if (awardState == 0 && pillState == 1)
        {
          lcd.setCursor(0,0);
          lcd.print("Please take medicine!");
          Serial.print("Take pills!Sir");
          tone(tonepin,2000,10000);
          digitalWrite(LED_BUILTIN, HIGH);
        }
        // 拿走了可乐，吃了药
        else if (awardState == 0 && pillState == 0)
        {
          lcd.setCursor(0,0);
          lcd.print("Well Done, Sir");
          Serial.print("hello");
          noTone(tonepin);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);
          ring = 0 ;
        } 

     }
      
  Serial.print("Time now is: ");
  print2digits(minute());
  Serial.print(":");
  print2digits(second());
  Serial.println();

  delay(1000);


}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.print('0');
  }
  Serial.print(number);
}

// play the music 

void play() 
{
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(tonepin, melody[thisNote], noteDuration * 0.9);
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    // stop the waveform generation before the next note.
    noTone(tonepin);
    if (digitalRead(buttonPin)== HIGH)  {break;}
  }
}

	
