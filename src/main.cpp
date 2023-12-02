#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Servo.h>

#define s0 10
#define s1 11
#define s2 12
#define SIG_pin 13
#define latch 8
#define clock 9
#define data 7
#define clear 6
#define pot A0

byte channels [8] = {0};

// enumerate two modes (Game, Control)
typedef enum{Game, Control} unoMode;
// enumerate three choices (Rock, Paper, Scissors)
typedef enum{Rock, Paper, Scissors, Reset}gameChoice;
// enumerate four fingers (Thumb, Pointer, Middle, Pair)
typedef enum{Thumb, Pointer, Middle, Pair}fingerSelect;

// four servo objects to control thumb, pointer, middle, and ring/pink fingers
Servo thumb, pointer, middle, pair;

const byte PROGMEM fingerLED[5] = {0b10000000, 0b01100000, 0b01010000, 0b01001000, 0b01000100};
byte LED_byte;

// initialize mode, choice, and finger variables
unoMode mode = Game;
gameChoice choice = Rock;
fingerSelect finger = Thumb;

void gameMove();
void updateSystem(byte idx);
void shifter();

void setup() {

  Serial.begin(9600);

  //Set POT and Mux pin to input
  pinMode(pot, INPUT);
  pinMode(SIG_pin, INPUT);

  //Set mux pins to output
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);

  //Set shift register pins to output
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(clear, OUTPUT);

  // attach servos to pins
  thumb.attach(2);
  pointer.attach(3);
  middle.attach(4);
  pair.attach(5);

  digitalWrite(clear, LOW);
  digitalWrite(clear, HIGH);

  LED_byte = pgm_read_byte(&fingerLED[0]);
  shifter();

  thumb.write(170);
  pointer.write(170);
  middle.write(10);
  pair.write(10);
  delay(500);
  thumb.detach();
  pointer.detach();
  middle.detach();
  pair.detach();


}

void loop() {
  randomSeed(millis());

  for (byte idx = 0; idx < 8; idx++){
    digitalWrite(s0, HIGH && (idx & B00000001));
    digitalWrite(s1, HIGH && (idx & B00000010));
    digitalWrite(s2, HIGH && (idx & B00000100));
    channels[idx] = digitalRead(SIG_pin);
    if (channels[idx] == HIGH){
      updateSystem(idx);
      delay(100);
    }
  }

  if (mode == Control){
    int val = analogRead(pot);
    Serial.println(val);
    byte angle = 0;
    switch (finger){
      case Thumb:
        angle = map(val, 0, 1023, 170, 10);
        thumb.attach(2);
        thumb.write(angle);
        delay(200);
        thumb.detach();
        break;
      case Pointer:
        angle = map(val, 0, 1023, 170, 10);
        pointer.attach(3);
        pointer.write(angle);
        delay(200);
        pointer.detach();
        break;
      case Middle:
        angle = map(val, 0, 1023, 10, 170);
        middle.attach(4);
        middle.write(angle);
        delay(200);
        middle.detach();
        break;
      case Pair:
        angle = map(val, 0, 1023, 10, 170);
        pair.attach(5);
        pair.write(angle);
        delay(200);
        pair.detach();
        break;
    }
  }

}

// thumb (170 erect / 10 bent)
// pointer (170 erect / 10 bent)
// middle (10 erect / 170 bent)
// pair (10 erect / 170 bent)
void gameMove(){
  switch (choice){
    case Rock:
      pair.attach(5);
      middle.attach(4);
      pointer.attach(3);
      thumb.attach(2);

      pair.write(170);
      delay(200);
      middle.write(170);
      delay(200);
      pointer.write(10);
      delay(200);
      thumb.write(10);
      delay(3000);
      thumb.write(170);
      delay(200);
      pointer.write(170);
      delay(200);
      middle.write(10);
      delay(200);
      pair.write(10);
      delay(1000);
      thumb.detach();
      pointer.detach();
      middle.detach();
      pair.detach();

      break;
    case Paper:
      pointer.attach(3);

      pointer.write(90);
      delay(500);
      pointer.write(170);
      delay(2000);

      pointer.detach();
      break;
    case Scissors:
      pair.attach(5);
      thumb.attach(2);

      pair.write(170);
      delay(200);
      thumb.write(10);
      delay(3000);
      thumb.write(170);
      delay(200);
      pair.write(10);
      delay(1000);

      thumb.detach();
      pair.detach();
      break;
    case Reset:
      thumb.attach(2);
      pointer.attach(3);
      middle.attach(4);
      pair.attach(5);

      thumb.write(170);
      delay(200);
      pointer.write(170);
      delay(200);
      middle.write(10);
      delay(200);
      pair.write(10);
      delay(1000);

      thumb.detach();
      pointer.detach();
      middle.detach();
      pair.detach();
      break;
  }
}

void updateSystem(byte idx){
  switch (idx){
    case 0:
      if (mode == Game){
        mode = Control;
        LED_byte = pgm_read_byte(&fingerLED[1]);
        shifter();
      }
      else{
        mode = Game;
        LED_byte = pgm_read_byte(&fingerLED[0]);
        shifter();
        choice = Reset;
        gameMove();
      }
      break;
    case 1:
      if (mode == Game){
        byte rand_idx = random(0, 3);
        if (rand_idx == 0){choice = Rock;}
        else if (rand_idx == 1){choice = Paper;}
        else if (rand_idx == 2){choice = Scissors;}
        gameMove();
      }
      else{}
      break;
    case 2:
      if (mode == Game){}
      else{
        LED_byte = pgm_read_byte(&fingerLED[1]);
        finger = Thumb;
        shifter();
      }
      break;
    case 3:
      if (mode == Game){}
      else{
        LED_byte = pgm_read_byte(&fingerLED[2]);
        finger = Pointer;
        shifter();
      }
      break;
    case 4:
      if (mode == Game){}
      else{
        LED_byte = pgm_read_byte(&fingerLED[3]);
        finger = Middle;
        shifter();
      }
      break;
    case 5:
      if (mode == Game){}
      else{
        LED_byte = pgm_read_byte(&fingerLED[4]);
        finger = Pair;
        shifter();
      }
      break;
  }
}

void shifter(){
  digitalWrite(latch, LOW);
  shiftOut(data, clock, LSBFIRST, LED_byte);
  digitalWrite(latch, HIGH);
}