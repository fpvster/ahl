

void led_confirm()
{
  digitalWrite(led_pin, HIGH);
  delay(100);
  digitalWrite(led_pin, LOW);
}



void led_warning()
{
  digitalWrite(led_pin, HIGH);
  delay(600);
  digitalWrite(led_pin, LOW);
}




