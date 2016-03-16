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
int ppm_period_us = 20000; // 20ms=50Hz
int n_channels = 8;
int idle_level = HIGH;
int ch_value[NUM_CHANNELS];
int ppm_out_pin = 12;
int mon_out_pin = 13;
int pulse_level;
int timer1_counter;

void setup() {
  // put your setup code here, to run once:
  pulse_level = (idle_level == LOW) ? HIGH : LOW;
  
  // channel value 1000-2000 range (PWM scale)
  ch_value[0] = 1000;
  ch_value[1] = 2000;
  ch_value[2] = 1500;
  ch_value[3] = 1000;
  ch_value[4] = 1800;
  ch_value[5] = 1500;

  pinMode(ppm_out_pin, OUTPUT);
  pinMode(mon_out_pin, OUTPUT);

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
}

/*
ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
  digitalWrite(ppm_out_pin, digitalRead(ppm_out_pin) ^ 1);
}
*/


void loop() {
  // put your main code here, to run repeatedly:
}
