
#include <Arduino.h>

// -----------------------------------------------------------------------------
// constants

const int                                 PinCLK   = 2;     // Used for generating interrupts using CLK signal
const int                                 PinDT    = 4;     // Used for reading DT signal
const int                                 PinSW    = 8;     // Used for the push button switch

// -----------------------------------------------------------------------------
// global vars

volatile int                             virtualPosition    = 0;
static boolean rotating=false; 

// -----------------------------------------------------------------------------
// forward decls

void isr();
void loop();
void setup();

// -----------------------------------------------------------------------------
// Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK

void isr ()  {

    static unsigned long                lastInterruptTime = 0;

    unsigned long                       interruptTime = millis();

    rotating=true;
    } // ISR
// -----------------------------------------------------------------------------

void setup() {
    Serial.begin(9600);

    pinMode(PinCLK,INPUT);
    pinMode(PinDT, INPUT);
    pinMode(PinSW, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PinCLK), isr, CHANGE);   // interrupt 0 is always connected to pin 2 on Arduino UNO

    Serial.println("Start");

    } // setup

// -----------------------------------------------------------------------------

void loop() {


while(rotating)
  {
    delay(2);  // debounce by waiting 2 milliseconds
               // (Just one line of code for debouncing)
    if (digitalRead(4) == digitalRead(2))  // CCW
     virtualPosition--;
    else                          // If not CCW, then it is CW
     virtualPosition++;
    rotating=false; // Reset the flag
    Serial.println(virtualPosition);
  }
  if (!digitalRead(PinSW)) virtualPosition=0;     
            
      
  } //loop
