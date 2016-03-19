


//
// Timer interrupt service routine
// this fires every 20mS (for 50Hz refresh rate)
// set PWM out pulse for the needed duration (1-2mS)
// and return
//
ISR(TIMER1_OVF_vect)
{
   TCNT1 = timer1_counter;   // preload timer

   digitalWrite(mon_out_pin, digitalRead(mon_out_pin)^1);

   digitalWrite(pwm_out_pin, pulse_level);
   delayMicroseconds(ch_value);
   digitalWrite(pwm_out_pin, idle_level);

   pb_poll();
      
} // ISR(TIMER1_OVF_vect)


