
#include <Arduino.h>

const int                                 PinCLK   = 2;     // Used for generating interrupts using CLK signal
const int                                 PinDT    = 4;     // Used for reading DT signal
const int                                 PinSW    = 8;     // Used for the push button switch
const int                                 PinLED   = 13;     // Used fome Mode signalization 
const int                                 ModeVolume    = 0;     
const int                                 ModeStation   = 1; 
   

static boolean rotating=false; 
int mode;
double time_press;
double time_cli_station;


boolean pressed;
boolean stopped;

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
    pinMode(PinLED, OUTPUT);
    mode = ModeVolume;
    time_press = millis();
    time_cli_station = millis();
    pressed = false;
    stopped = false;
    attachInterrupt(digitalPinToInterrupt(PinCLK), isr, FALLING);   

    } 

// -----------------------------------------------------------------------------

void loop() {
long press_length; 
long last_cli_time;



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
      last_cli_time =  millis() - time_cli_station; 
     
      if (digitalRead(PinDT) == digitalRead(PinCLK))  // CCW
       if (last_cli_time>=300) 
        { 
          Serial.print("cli.next\r"); 
          time_cli_station = millis();
        }
                      
      if (digitalRead(PinDT) != digitalRead(PinCLK))  // CCW                            
       if (last_cli_time>=300) 
        {
         Serial.print("cli.prev\r"); 
         time_cli_station = millis();
        }
    }
     
    rotating=false; // Reset the flag
  }
 
  
  if (!digitalRead(PinSW)) 
     {
      if (!pressed) time_press = millis();
      pressed=true;
      delay(2);
     }
     
    if (digitalRead(PinSW) && pressed) 
     {
     
       pressed=false;
       press_length= millis()-time_press;
       if (stopped) { Serial.print("cli.start\r"); stopped = false; }
       else
        if (press_length >= 300) { Serial.print("cli.stop\r"); stopped = true; mode==ModeStop;}
         else  
          if (mode==ModeVolume) 
             mode=ModeStation;
          else
            mode=ModeVolume;      
     }

     // change status LED
     
     if (stopped)  // LED blink 2 sec 
      if (millis() & 2048) digitalWrite(PinLED,0);
       else digitalWrite(PinLED,1);
     else
      {
      if (mode==ModeStation)    // LED blink 0.5sec
       if (millis() & 256) digitalWrite(PinLED,1);
       else digitalWrite(PinLED,0);
     
       if (mode==ModeVolume) digitalWrite(PinLED,1); // LED on
      }

              
 } //loop
