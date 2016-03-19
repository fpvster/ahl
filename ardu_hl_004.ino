/*
 * sketch: ppm_instrumented_001
 * 
 * description: emit rc ppm signal according to preset values
 * just to be able to watch on logic analyzer and perhaps
 * drive through Futaba transmitter as trainer (student) input
 * 
 */


#define INTER_PULSE_DELAY 300
#define NUM_CHANNELS      6
int idle_level = LOW;
int ch_value;
int pwm_out_pin = 12;
int mon_out_pin = 14;
int led_pin     = 13;
int pb1_pin     = 11;
int pulse_level;
int timer1_counter;


#define MAIN_ST_SAFE       0
#define MAIN_ST_PASSTHRU   1
#define MAIN_ST_HANDLAUNCH 2

int main_st;

void setup() {
  // put your setup code here, to run once:
  pulse_level = (idle_level == LOW) ? HIGH : LOW;
  
  ch_value = 1000;
  main_st  = MAIN_ST_SAFE;

  pinMode(pwm_out_pin, OUTPUT);
  pinMode(mon_out_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);

  pinMode(pb1_pin, INPUT_PULLUP);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  //timer1_counter = 49152;   // preload timer 65536-16MHz/256/4Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  pb_setup();
  Serial.begin(115200);
  Serial.write("setup done\n");
}


void loop() {
  // put your main code here, to run repeatedly:


  switch(main_st)
  {
    case MAIN_ST_SAFE:
      if(pb_short_press_event())
      {
        main_st = MAIN_ST_PASSTHRU;
        Serial.write("MAIN_ST_SAFE => MAIN_ST_PASSTHRU\n");
        led_confirm();
      }
      break; //  case MAIN_ST_SAFE

    case MAIN_ST_PASSTHRU:
      if(pb_long_press_event())
      {        
        main_st = MAIN_ST_HANDLAUNCH;
        Serial.write("MAIN_ST_PASSTHRU => MAIN_ST_HANDLAUNCH\n");
        led_confirm();
      }
      else if(pb_short_press_event())
      {
        Serial.write("MAIN_ST_PASSTHRU , need LONG press to go to next state\n");
        led_warning();
      }
      break; // case MAIN_ST_PASSTHRU

    case MAIN_ST_HANDLAUNCH:
      if(pb_short_press_event() || pb_long_press_event())
      {        
        main_st = MAIN_ST_SAFE;
        Serial.write("MAIN_ST_HANDLAUNCH => MAIN_ST_SAFE\n");
        led_confirm();
      }
      break; // case MAIN_ST_HANDLAUNCH
      
  } // switch(main_st)



  /*
  // if(!digitalRead(pb1_pin))
  if(pb_short_press_event())
  {
    ch_value = 3000 - ch_value;
    Serial.write("changed channel value\n");
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);    
  }
  if(pb_long_press_event())
  {
    ch_value = 1000;
    Serial.write("changed channel value to 1000\n");
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);    
  }
   */

}
