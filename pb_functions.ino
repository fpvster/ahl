/*
 * pushbutton management functions
 * 
 * new definitions:
 * 
 *   two (ok three) types of events - 'short press' 'long press' and 'any press'
 *   
 *   time of occurence: 
 *   
 *   - short press: button is pressed Tsp<T<Tlp, registered on release
 *   - long press: button is pressed Tlp<T, registered on T==Tlp
 * 
 *   'released' event does NOT go through debounce check anymore (this is a change)
 * 
 */



int pb_counter;
int pb_prev_read;          // PB_PRESSED/PB_RELEASED

int pb_event_valid;        // 0/1
int pb_event_value;        // count

#define PB_PRESSED   0
#define PB_RELEASED  1

#define PB_SAMPLE_FREQ 50
#define PB_COUNT_TSP   (PB_SAMPLE_FREQ/5)  /* 200mS */
#define PB_COUNT_TLP   (PB_SAMPLE_FREQ*2)  /* 2s */


int pb_short_press_event()
{
  return pb_event(PB_PRESSED, PB_COUNT_TSP-1, PB_COUNT_TLP-1);
}

int pb_long_press_event()
{
  return pb_event(PB_PRESSED, PB_COUNT_TLP-1, PB_COUNT_TLP+2);
}




void pb_setup()
{
  pb_counter = 0;
  pb_event_valid = 0;
} // void pb_setup()

void pb_poll()
{
  int current_read;
  current_read = digitalRead(pb1_pin);

  if(current_read == PB_PRESSED)
  {
    if(pb_counter < PB_COUNT_TLP)
    {      
      pb_abort_handlaunch(); // high priority safety feature: ABORT handlaunch on any press
      pb_counter++;
    }
    else if(pb_counter == PB_COUNT_TLP)
    {
      pb_event_valid = 1; // event valid
      pb_event_value = pb_counter; // event value
      pb_counter++;
    } // else - reached long press condition
  } // if(current_read == PB_PRESSED)
  else
  {
    if(current_read != pb_prev_read)
    {
      if((pb_counter > PB_COUNT_TSP) && (pb_counter < PB_COUNT_TLP))
      {
        // register short press event
        pb_event_valid = 1; // event valid
        pb_event_value = pb_counter; // event value
      }
    }
    pb_counter = 0;
  } // else

  pb_prev_read = current_read;
}


void pb_abort_handlaunch()
{
  if(main_st == MAIN_ST_HANDLAUNCH)
  {
    Serial.write("pb_abort_handlaunch()\n");
  }
} // pb_abort_handlaunch()




int pb_event(int expected, int count_min, int count_max)
{
  int retval;
  
  if(pb_event_valid && (pb_event_value > count_min) && (pb_event_value < count_max ))
  {
    retval = 1;
    pb_event_valid = 0; // clear 'event valid' flag
  }
  else
    retval = 0;

  return retval;
} // int pb_event(int expected)



