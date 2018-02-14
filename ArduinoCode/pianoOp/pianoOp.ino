#include <EEPROM.h>
#include <avr/power.h>

                
int MUXEN_SP = PD0;
int MUXS0_SP = PD1;
int MUXS1_SP = PD2;
int MUXS2_SP = PD3;
int MUXS3_SP = PD4;
int MUXSIG_SP = PD5;


int outPins_SP[] = {MUXS0_SP,
                    MUXS1_SP,
                    MUXS2_SP,
                    MUXS3_SP,
                    MUXEN_SP,
                    MUXSIG_SP};


int IN1 = 0b00010000;
int IN2 = 0b00100000;
int IN3 = 0b01000000;
int IN4 = 0b10000000;

int IN5 = 0b01000000;
int IN6 = 0b10000000;

int IN7 = 0b01000000;
int IN8 = 0b10000000;

int IN9 = 0b00000100;
int IN10 = 0b01000000;

int IN11 = 0b00000001;
int IN12 = 0b00000010;

int inPins[] = {IN1, IN2, IN3,
              IN4, IN5, IN6,
              IN7, IN8, IN9,
              IN10, IN11, IN12};


void setup() {

    DDRD = 0x3F;
//   for (int i=0; i <= sizeof(inPins)-1; i++){
//      pinMode(inPins[i], INPUT);
//   }
    DDRB &= ~0b11110000;
    DDRC &= ~0b11000000;
    DDRD &= ~0b11000000;
    DDRE &= ~0b01000100;
    DDRC &= ~0b00000011;
    
   //Initialize the enable pin for the MUX.
   PORTD &= ~_BV(0);

}


void loop() {

  
//  PORTD |= _BV(PD5); // Pin SIG goes high
  int pinsPressed[12] = { 0 };
  int pinWasPressed = 0;
  readPinInput(pinsPressed);
  
//  writePinFromBinaryOnD(0);
   
   for (int i=0; i <= 11; i++){
      if(pinsPressed[i]){
        writePinFromBinaryOnD(i);
        pinWasPressed = 1;
//        PORTD |= _BV(PD5); // Pin SIG goes high
      }
      
//      else
//      {
//        PORTD &= ~_BV(PD5); // Pin SIG goes low
//      }
   }

   if (!pinWasPressed){
       PORTD &= ~_BV(PD5); // Pin SIG goes low
   }
   else
   {
      PORTD |= _BV(PD5); // Pin SIG goes high
   }
}

int readPinInput(int *pinsPressed){
    
    //Read Port B
    for (int i=0; i <= 3; i++){
       if((PINB & inPins[i]) == 0){
          writePinFromBinaryOnD(i);
          pinsPressed[i] = 1;
      }
    }

    //Read Port C
    for (int i=4; i <= 5; i++){
       if((PINC & inPins[i]) == 0){
          writePinFromBinaryOnD(i);
          pinsPressed[i] = 1;
      }
      else
      {
        pinsPressed[i] = 0;
      }
    }

    //Read Port D
    for (int i=6; i <= 7; i++){
       if((PIND & inPins[i]) == 0){
          writePinFromBinaryOnD(i);
          pinsPressed[i] = 1;
      }
      else
      {
        pinsPressed[i] = 0;
      }
    }
    
    //Read Port E
    for (int i=8; i <= 9; i++){
       if((PINE & inPins[i]) == 0){
          writePinFromBinaryOnD(i);
          pinsPressed[i] = 1;
      }
      else
      {
        pinsPressed[i] = 0;
      }
    }    

    //Read Port F
    for (int i=10; i <= 11; i++){
       if((PINF & inPins[i]) == 0){
          writePinFromBinaryOnD(i);
          pinsPressed[i] = 1;
      }
      else
      {
        pinsPressed[i] = 0;
      }
    }
    
//    if (pinPressed == -1){
//        PORTD &= ~_BV(PD5); // Pin SIG goes high
//    }
//    else
//    {
//        PORTD |= _BV(PD5); // Pin SIG goes high
//    }
}

void writePinFromBinaryOnD(int val)
{
  for (int i=1; i <= 4; i++){
      if (bitRead(val, i-1) == 1){
          PORTD |= _BV(i);
       }
      else
       {
          PORTD &= ~_BV(i); 
       }
      }
 }

