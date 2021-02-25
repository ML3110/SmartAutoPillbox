/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "Alert.h"

Alert::Alert()
{
  this->buzzer = 15;
  this->pushBtn = 16;
}

Alert::~Alert()
{
  
}

void Alert::SetPushButtonPin(int pin)
{
  this->pushBtn = pin;
}

void Alert::SetBuzzerPin(int pin)
{
  this->buzzer = pin;
}

// Checks the initialised pin for the state. If it is high (pressed), return true.
bool Alert::ButtonIsPressed()
{
  if (digitalRead(this->pushBtn) == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// Disables buzzer by setting noTone on the buzzer pin.
void Alert::DisableBuzzer()
{
  tone(this->buzzer, 10, 0, 0);
  noTone(this->buzzer, 0);
}

// Alarms the buzzer to a frequency of 330 Hz
void Alert::TriggerBuzzer()
{
  tone(this->buzzer, 330, 0, 0);
}
