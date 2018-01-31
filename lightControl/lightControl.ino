#include <EEPROM.h>
#include <avr/power.h>

// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte mode = 0;
String morseWord;

//Define unit length in ms
#define UNIT_LENGTH    250

//Build a struct with the morse code mapping
static const struct {const char letter, *code;} MorseMap[] =
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', "-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { ' ', "     " }, //Gap between word, seven units 
    
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
    
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '"', ".-..-." },
  { '@', ".--.-." },
  { '&', ".-..." },
};



//int P_1 = 33;
int P_1 = 8;
int P_2 = 17;
int P_3 = 15;
int P_4 = 16;
int P_5 = 9;
int P_6 = 10;
int P_7 = 11;
int P_8 = 5;
int P_9 = 13;
int P_10 = 3;
int P_11 = 2;
int P_12 = PB0;
int P_13 = 1;
int P_14 = 4;
int P_15 = 30;
int P_16 = 12;
int P_17 = 25;
int P_18 = 100;
int P_19 = 7;
int P_20 = 23;
int P_21 = 22;
int P_22 = 21;
int P_23 = 20;
int P_24 = 19;
int P_25 = 18;

int lightPins[] = {P_1, P_2, P_3, P_4, P_5,
                   P_6, P_7, P_8, P_9, P_10,
                   P_11, P_12, P_13, P_14, P_15,
                   P_16, P_12, P_17, P_18, P_19, P_20,
                   P_21, P_8, P_22, P_23, P_10, P_24, P_25};

// the setup function runs once when you press reset or power the board
void setup() {
//  clock_prescale_set(clock_div_1);
//  CKSEL = 0x2;
//  CLKPR = 0x40; // write the CLKPCE bit to one and all the other to zero
//
//  // Change clock division.
//  CLKPR = clockPrescaler; // write the CLKPS0..3 bits while writing the CLKPE bit to zero

  mode = EEPROM.read(address);
  EEPROM.write(address, (mode+1)%4);
  
  DDRE |= 0x04;  // pinMode PE2 OUTPUT
  
  // initialize digital pin P_1 as an output.
  for (int i=0; i <= 28; i++){
      pinMode(lightPins[i], OUTPUT);
   }
}

// the loop function runs over and over again forever
void loop() {
   switch (mode) {
    case 0:
      for (int i=0; i < 28; i++){
        digitalWrite(lightPins[i], HIGH);
        delay(100);
        digitalWrite(lightPins[i], LOW);
        if (lightPins[i] == 100){
            PINE = 0x04;  // Toggle PE2
            delay(100);
            PINE = 0x04;  // Toggle PE2 
        }
      }
      break;
    case 1:
        for (int i=0; i < 28; i++){
          digitalWrite(lightPins[i], HIGH);
          delay(100);
          if (lightPins[i] == 100){
            PINE = 0x04;  // Toggle PE2
            delay(100);          
          }
        }

        for (int i=0; i < 28; i++){
          delay(100);
          digitalWrite(lightPins[i], LOW);
          if (lightPins[i] == 100){
            delay(100);
            PINE = 0x04;  // Toggle PE2
          
          }
        }
      break;
    case 2:
      morseWord = encode( "MY BROTHER IS AWESOME " );
      sendMorse(morseWord);
      break;

    case 3:
       for (int i=0; i < 28; i++){
          digitalWrite(lightPins[i], LOW);
          if (lightPins[i] == 100){
            PINE &= 0x04;  // Clear PE2
          }
        }
      break;
   }
}

void sendMorse(String morseWord)
{
    for(int i=0; i<=morseWord.length(); i++)
      {
        switch( morseWord[i] )
        {
          case '.': //dit
            writeAll( 1 );
            delay( UNIT_LENGTH );
            writeAll( 0 );
            delay( UNIT_LENGTH );
          
            break;

          case '-': //dah
            writeAll( 1 );
            delay( UNIT_LENGTH*3 );
            writeAll( 0 );
            delay( UNIT_LENGTH );
          
            break;

          case ' ': //gap
            delay( UNIT_LENGTH );
        }
      }
}

void writeAll(int val)
{
  if (val == 1)
  {
     for (int i=0; i < 28; i++){
        digitalWrite(lightPins[i], HIGH);
        if (lightPins[i] == 100){
            PORTE |= 0x04;  // Set PE2
                      
        }
      }
  }
  else if (val == 0)
  {
    for (int i=0; i < 28; i++){
          digitalWrite(lightPins[i], LOW);
          if (lightPins[i] == 100){
            PORTE &= 0xFB;  // Clear PE2
          
          }
        }
    
  }
  
}



String encode(const char *string)
{
  size_t i, j;
  String morseWord = "";
  
  for( i = 0; string[i]; ++i )
  {
    for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
    {
      if( toupper(string[i]) == MorseMap[j].letter )
      {
        morseWord += MorseMap[j].code;
        break;
      }
    }
    morseWord += " "; //Add tailing space to seperate the chars
  }

  return morseWord;  
}
