


ISR(TIMER1_OVF_vect)
{
  TCNT1 = timer1_counter;   // preload timer

   static int n;
// digitalWrite(ppm_out_pin, digitalRead(ppm_out_pin)^1);
   digitalWrite(mon_out_pin, digitalRead(mon_out_pin)^1);

   for(n=0; n<NUM_CHANNELS; n++)
   {
      digitalWrite(ppm_out_pin, pulse_level);
      delayMicroseconds(500);
      digitalWrite(ppm_out_pin, idle_level);
      delayMicroseconds(ch_value[n] - 500);
   }

} // ISR(TIMER1_OVF_vect)

/*
ISR(TIMER1_OVF_vect)
{
  TCNT1 = timer1_counter;   // preload timer

  int n;

  for(n=0; n<NUM_CHANNELS; n++)
  {
    digitalWrite(ppm_out_pin, pulse_level);
    delayMicroseconds(500);
    digitalWrite(ppm_out_pin, idle_level);
    delayMicroseconds(ch_value[n]-500);
  }
} // ISR(TIMER1_OVF_vect)
*/
