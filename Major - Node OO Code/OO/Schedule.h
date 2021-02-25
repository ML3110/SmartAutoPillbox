/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This object is for the users medication schedule. This is where the 
 * next dose is calculated and various functionality for the user schedule
 * from the SQL server.
 */

#ifndef Schedule_h
#define Schedule_h

#include <Arduino.h>
#include "DateTime.h"

class Schedule
{
  // Attributes
private:
  int userID;
  String doseSchedule[7][9];
  bool scheduleUpdated;
  bool nextDoseSet;
  String nextDoseDay;
  String nextDoseTime;
  bool looped;

  // Constructor
public:
  Schedule();
  Schedule(int userID);
  ~Schedule();

  // Properties
public:
  void SetSchedule();
  void SetScheduleOffline();
  void SetNextDoseDay(String doseday);
  void SetNextDoseTime(String dosetime);
  
  String GetSchedule(int i, int j);
  String GetNextDoseDay();
  String GetNextDoseTime();
  
  // Methods
public:
  bool ScheduleIsUpdated();
  bool NextDoseIsSet();
  void PrintSchedule();
  void ParseSchedule(String response);
  void UpdateSchedule(String* temp);
  void CalculateNextDose(DateTime dt);
  void ResetSchedule();
  void UnsetNextDose();
};

#endif
