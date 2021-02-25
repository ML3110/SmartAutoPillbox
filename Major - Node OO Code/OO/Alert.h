/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This object is the alert. If the dose is required, both an LED and buzzer alert
 * the user until they press the button to say they've received the dose.
 * The alert is then disabled.
 */

#ifndef Alert_h
#define Alert_h

#include <Arduino.h>
#include <Tone32.h>

class Alert
{
  // Attributes
private:
  bool state;
  int pushBtn;
  int buzzer;
  
  // Constructors
public:
  Alert();
  ~Alert();
  
  // Properties
public:
  void SetPushButtonPin(int pin);
  void SetBuzzerPin(int pin);
  
  // Methods
public:
  bool ButtonIsPressed();
  void DisableBuzzer();
  void TriggerBuzzer();
};

#endif
