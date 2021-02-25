/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This is the dispenser object and controls the dispensing and calls the alert
 * if required.
 */

#ifndef Dispenser_h
#define Dispenser_h

#include <Arduino.h>

#include "schedule.h"
#include "DateTime.h"
#include "SQLConnection.h"
#include "User.h"
#include "Alert.h"
#include <Tone32.h>

class Dispenser 
{
  // Attributes
private:
  bool dispensed;
  String currentDay;
  String currentTime;
  String doseDay;
  String doseTime;
  unsigned long oldtime;
  int interval = 70000; //milliseconds
  String dispenseTime;
  
  // Constructors
public:
  Dispenser();
  ~Dispenser();
  
  // Properties
public:
  void SetDispensed();
  void SetNotDispensed();
  void SetDoseTime(Schedule* schedule);
  void SetDoseDay(Schedule* schedule);
  
  // Methods
public:
  void HandleDispenser(User user, Schedule* schedule, DateTime dt, SQLConnection conn, Alert alert);
  bool HasDispensed();
  bool DoseRequired(DateTime dt);
  void DispenseItem(Alert alert);
};

#endif
