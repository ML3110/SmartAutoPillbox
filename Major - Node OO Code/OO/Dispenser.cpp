/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "Dispenser.h"

Dispenser::Dispenser()
{
  dispensed = false;
}

Dispenser::~Dispenser()
{
  
}

void Dispenser::SetDispensed()
{
  this->dispensed = true;
}

void Dispenser::SetNotDispensed()
{
  this->dispensed = false;
}

bool Dispenser::HasDispensed()
{
  if (this->dispensed)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// Main function for the project and this object. Takes in alot of parameters all to control the dispensing.
// It is constantly called from the main() loop
void Dispenser::HandleDispenser(User user, Schedule* schedule, DateTime dt, SQLConnection conn, Alert alert)
{
  // If it has dispensed and the user has pressed the button, send a log update then calculate the next dose
  if (HasDispensed() && alert.ButtonIsPressed())
  {
    conn.SetLogURL(user.GetUserID(), schedule->GetNextDoseTime(), this->dispenseTime, dt.GetTimeStamp());
    conn.SendLogUpdate();

    SetNotDispensed();
    schedule->UnsetNextDose();

    schedule->CalculateNextDose(dt);
  }

  // If it hasn't dispensed, it checks to make sure the user hasn't updated the schedule.
  if (!HasDispensed())
  {
    SetDoseDay(schedule);
    SetDoseTime(schedule);

    // If a dose is required, dispense the item
    if (DoseRequired(dt) && schedule->NextDoseIsSet())
    {
      this->dispenseTime = dt.GetTimeStamp();
      DispenseItem(alert);
    }
  }
}

void Dispenser::SetDoseTime(Schedule* schedule)
{
  this->doseTime = schedule->GetNextDoseTime();
}

void Dispenser::SetDoseDay(Schedule* schedule)
{
  this->doseDay = schedule->GetNextDoseDay();
}

// Comparison of day and time
bool Dispenser::DoseRequired(DateTime dt)
{
  currentTime = dt.GetTimeStamp();
  currentDay = dt.GetWeekDay();
  
  currentDay.toLowerCase();
  
  if (currentDay == doseDay)
  {
    if (currentTime == doseTime && !HasDispensed())
    {  
      return true;
    }
    else
    {
      return false;
    }
  }
  else 
  {
    return false;
  }
}

void Dispenser::DispenseItem(Alert alert)
{
  Serial.println("Dispensed");
  SetDispensed();
  oldtime = millis();

  alert.TriggerBuzzer();
}
