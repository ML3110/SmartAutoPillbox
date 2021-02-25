/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "DateTime.h"

DateTime::DateTime()
{
  
}

DateTime::~DateTime()
{
  
}

void DateTime::SetTimeStamp(NTPClient timeClient)
{
  this->timeStamp = "";

  // If the response is less than 10, need to add a 0 to the beginning
  if (timeClient.getHours() < 10)
  {
    this->timeStamp += "0";
  }

  // Add the hours to the timestamp
  this->timeStamp += timeClient.getHours();
  
  this->timeStamp += ":";

  // Doesn't return leading 0 if less than 10 minutes past
  if (timeClient.getMinutes() < 10)
  {
    this->timeStamp += "0";
  }

  // Add the minutes to the timestamp
  this->timeStamp += timeClient.getMinutes();
}


void DateTime::SetWeekDay(NTPClient timeClient)
{
  // epoch time info from https://forum.arduino.cc/index.php?topic=120325.0
  int result = ((timeClient.getEpochTime() / 86400L) + 4) % 7;

  switch(result)
  {
    case 0:
      this->weekDay =  "Sunday";
      break;
    case 1:
      this->weekDay = "Monday";
      break;
    case 2:
      this->weekDay = "Tuesday";
      break;
    case 3:
      this->weekDay = "Wednesday";
      break;
    case 4:
      this->weekDay = "Thursday";
      break;
    case 5:
      this->weekDay = "Friday";
      break;
    case 6:
      this->weekDay = "Saturday";
      break;
    default:
      break;
  }
}

String DateTime::GetTimeStamp()
{
  return this->timeStamp;
}

String DateTime::GetWeekDay()
{
  return this->weekDay;
}

// Updates the timeClient
void DateTime::UpdateDateTime(NTPClient timeClient)
{
  SetTimeStamp(timeClient);
  SetWeekDay(timeClient);
}
