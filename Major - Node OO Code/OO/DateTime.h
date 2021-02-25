/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This is the DateTime object. It updates the day and time via NTP.
 */

#ifndef DateTime_h
#define DateTime_h

#include <Arduino.h>
#include <NTPClient.h>

class DateTime
{
  // Attributes
private:
  String timeStamp;
  String weekDay;
  String formattedDate;
  
  // Constructors
public:
  DateTime();
  ~DateTime();

  // Properties
public:
  void SetTimeStamp(NTPClient timeClient);
  void SetWeekDay(NTPClient timeClient);

  String GetTimeStamp();
  String GetWeekDay();

  // Methods
public:
  void  UpdateDateTime(NTPClient timeClient);
};

#endif
