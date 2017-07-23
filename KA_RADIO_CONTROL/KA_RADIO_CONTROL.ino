
#include <Arduino.h>

const int                                 PinCLK   = 2;     // Used for generating interrupts using CLK signal
const int                                 PinDT    = 4;     // Used for reading DT signal
const int                                 PinSW    = 8;     // Used for the push button switch
const int                                 ModeVolume    = 0;     
const int                                 ModeStation   = 1; 
const int                                 ModeStop   = 2;     

static boolean rotating=false; 
int mode;
double time_press;
boolean pressed;

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
    Serial.begin(115200);

    pinMode(PinCLK,INPUT);
    pinMode(PinDT, INPUT);
    pinMode(PinSW, INPUT_PULLUP);
    mode = ModeVolume;
    time_press = millis();
    pressed = false;
    attachInterrupt(digitalPinToInterrupt(PinCLK), isr, FALLING);   

    } 

// -----------------------------------------------------------------------------

void loop() {
long press_length;  


while(rotating)
  {
    delay(2);  // debounce by waiting 2 milliseconds
    
    if (mode==ModeVolume)
    {
      if (digitalRead(PinDT) == digitalRead(PinCLK))  // CCW
       Serial.print("cli.vol+\r");
      else                          
       Serial.print("cli.vol-\r");
    }
    
    if (mode==ModeStation)
    {
      if (digitalRead(PinDT) == digitalRead(PinCLK))  // CCW
       Serial.print("cli.next\r");
      else                         
       Serial.print("cli.prev\r");
    }
     
    rotating=false; // Reset the flag
  }
 
  
  if (!digitalRead(PinSW)) 
     {
      pressed=true;
      time_press = millis();
      delay(50);
     }
     
    if (digitalRead(PinSW) && pressed) 
     {
       pressed=false;
       press_length= millis()-time_press;
       Serial.println(press_length);
       if (press_length >= 100) Serial.print("cli.stop\r");
       else  
        if (mode==ModeStation) 
           mode=ModeVolume;
        else
           mode=ModeStation;      
     }
              
 } //loop
