#include <Adafruit_NeoPixel.h>

#define PIN 6 //LED PIN
#define PINOne 12 //LED PIN
#define PIRPin 9  //PIR PIN
#define TempPIN A0     //TEMP PIN
#define soundPIN A4    //pin for the microphone

#define VALUE_MIN     45 //temp min
#define VALUE_MAX     75 //temp max
#define AUDIO_MAX     45 //temp max
 
             
const int speaker = 5;       //pin for the speaker
const int leftButton = 4;    //pin for the left button (control the motion sensing + lights + sounds) 
const int rightButton = 19;  //pin for the right button (control the motion sensing + lights) 
float decValue;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pinMode(speaker, OUTPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(PIRPin, INPUT);      
  strip.begin();
  strip.setBrightness(100);
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}


void getMotion() {
    if (digitalRead(PIRPin) == HIGH) {  // check if the input is HIGH
     Serial.println("Motion!");
      blink(strip.Color(255, 149, 20), 100); // Orange
   }
   else {
    Serial.println("No Motion!");
   }
}


void tone(unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds) {
  int x;   
  long delayAmount = (long)(1000000/frequencyInHertz);
  long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
  for (x=0; x<loopTime; x++) {     
     digitalWrite(speakerPin,HIGH); 
     delayMicroseconds(delayAmount); 
     digitalWrite(speakerPin,LOW);  
     delayMicroseconds(delayAmount);
  }  
}


void color(uint32_t c, uint8_t time) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(time);
  }
}

void blink(uint32_t c, uint8_t time) {
  for (int j=0; j<10; j++) {
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+2) {
        strip.setPixelColor(i+q, c);
      }
      strip.show();

      delay(time);

      for (uint16_t i=0; i < strip.numPixels(); i=i+2) {
        strip.setPixelColor(i+q, 0);       
      }
    }
  }
}


void temperature() {
  //temperature readings
  uint16_t value = analogRead(TempPIN);
  decValue = value/10;
  Serial.print("Temperature: ");
  Serial.println(decValue);
  if(decValue < VALUE_MIN) { //if temperature is too low
    color(strip.Color(2, 237, 225), 50); // Blue

  }
  else if(decValue > VALUE_MAX) { //if temperature is too high
        color(strip.Color(225, 0, 0), 50); // Red

  }
  else if (decValue > VALUE_MIN && decValue < VALUE_MAX) { //if temperature is just right 
    color(strip.Color(22, 204, 12), 50); // Green

  }
}

void enableTones() {
  uint16_t sound = analogRead(soundPIN);
  float sound2 = sound - 300;
  Serial.print("Sound: ");
  Serial.println(sound2);
  //button to turn on motion activation/sound
  if(digitalRead(leftButton)) {
    if(sound2 > AUDIO_MAX) { //if it is loud aka a scream 
     //enable makeTone function if motion is sensed
      blink(strip.Color(127, 127, 127), 50); // White
      tone(speaker,1760,100);   // output a 1760 Hz sound for a tenth of a second
    }
  } 
  if(digitalRead(rightButton)) {
    Serial.println("hi i want to talk to you!!");
    blink(strip.Color(255,92,205), 50); // Pink
  } 
}


void loop() {
  temperature();
  enableTones();
  getMotion();
  delay(1000);
}



