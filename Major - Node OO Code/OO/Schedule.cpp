/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "schedule.h"

Schedule::Schedule()
{
  this->scheduleUpdated = false;
  this->nextDoseSet = false;
}

Schedule::Schedule(int userID)
{
  this->userID = userID;
  this->scheduleUpdated = false;
  this->nextDoseSet = false;
}

Schedule::~Schedule()
{

}

// Sets the schedule position with the passed value
void Schedule::UpdateSchedule(String* temp)
{
  for (int i = 0; i < 7; i++)
  {
    if (doseSchedule[i][0] == temp[0])
    {
      for (int j = 0; j < 8; j++)
      {
        doseSchedule[i][j+1] = temp[j+1];
      }
    }
  }
}

void Schedule::SetSchedule()
{

}

// If the user has no connection or is offline during initial setup, set it to blank and display error message
void Schedule::SetScheduleOffline()
{
  doseSchedule[0][0] = "monday";
  doseSchedule[1][0] = "tuesday";
  doseSchedule[2][0] = "wednesday";
  doseSchedule[3][0] = "thursday";
  doseSchedule[4][0] = "friday";
  doseSchedule[5][0] = "saturday";
  doseSchedule[6][0] = "sunday";
  
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      doseSchedule[i][j+1] = "";
    }
  }

  this->scheduleUpdated = true;

  Serial.println("Likely problems:\n1. Incorrect network credentials\n2. Firewall problems\n3. WAMP not up");
}

// Returns specific index of schedule, used for the web handler
// and displaying the user schedule for each day
String Schedule::GetSchedule(int i, int j)
{
  return this->doseSchedule[i][j];
}

String Schedule::GetNextDoseDay()
{
  return this->nextDoseDay;
}

String Schedule::GetNextDoseTime()
{
  return this->nextDoseTime;
}

void Schedule::PrintSchedule()
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 8 + 1; j++)
    {
      Serial.println(this->doseSchedule[i][j]);
    }
  }
}

bool Schedule::ScheduleIsUpdated()
{
  if (this->scheduleUpdated)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// Parse the schedule from the SQL server response
void Schedule::ParseSchedule(String response)
{
  char bufferIn[512];
  char c;
  int count = 0;

  // Copy the response to the buffer
  response.toCharArray(bufferIn, 512);

  // Parse info, delimited by comma and new line
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      while (c != '\n')
      {
        c = bufferIn[count];
        if (c == ',' || c == '\n')
        {
          break;
        }
        else
        {
          this->doseSchedule[i][j] += c;
          count++;
        }
      }
      count++;
      c = bufferIn[count];
    }
  }
  this->scheduleUpdated = true;
  Serial.println("Schedule updated");
}

bool Schedule::NextDoseIsSet()
{
  if (nextDoseSet)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Schedule::CalculateNextDose(DateTime dt)
{
  String checkDay = dt.GetWeekDay();
  String checkTime = dt.GetTimeStamp();
  
  checkDay.toLowerCase();
  
  this->looped = false;

  while (!this->looped && !this->nextDoseSet)
  {
    for (int i = 0; i < 7; i++)
    {
      // begin checking the daily schedule
      if (this->looped && i == 6)                     
      {
        // no dose set
        this->nextDoseDay = "Schedule not set";
        this->nextDoseTime = "";
        Serial.println("No dose set");
        break;
      }
      if (checkDay == doseSchedule[i][0])       
      {
        // if the day to check (today on first loop) is the same as the schedule
        for (int j = 0; j < 8; j++)
        {
          // begin checking the individual day doses
          if (checkTime < doseSchedule[i][j+1])
          {
            // if the time to check is less than the current time
            Serial.print("Next dose set: ");
            Serial.print(doseSchedule[i][0]);
            Serial.print(" @ ");
            Serial.println(doseSchedule[i][j+1]);
            this->nextDoseDay=doseSchedule[i][0];
            this->nextDoseTime=doseSchedule[i][j+1];
            
            this->nextDoseSet = true;
            j = 0;
            i = 6;
            break;
          }
          // if it has reached the end of the day to check, set the day to the next day
          else if (j == 7)
          {
            // if it has already checked up to Sunday, reset the day to check to Monday
            // and begin checking again but set the "looped" flag to true to stop infinite
            // loops
            if (i == 6)
            {
              looped = true;
              checkDay = doseSchedule[0][0];
              checkDay.toLowerCase();
              i = -1;
              j = -1;
              break;
            }
            // increase the day
            else
            {
              checkDay = doseSchedule[i+1][0];
              checkDay.toLowerCase();
              checkTime = "00:01";
              j = -1;
              break;
            }
          }
        }
      }
    }
  }
}

void Schedule::ResetSchedule()
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 8 + 1; j++)
    {
      doseSchedule[i][j] = "";
    }
  }
}

void Schedule::UnsetNextDose()
{
  this->nextDoseDay = "";
  this->nextDoseTime = "";
  this->nextDoseSet = false;
}

void Schedule::SetNextDoseDay(String doseday)
{
  this->nextDoseDay = doseday;
}

void Schedule::SetNextDoseTime(String dosetime)
{
  this->nextDoseTime = dosetime;
}
