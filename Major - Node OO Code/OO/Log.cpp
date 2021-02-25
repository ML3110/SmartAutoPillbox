/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "Log.h"

Log::Log()
{

}

Log::~Log()
{
  
}

// Reset the log to a blank state
void Log::ResetLog()
{
  for (int i = 0; i < 56; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      this->logFile[i][j] = "";
    }
  }
}

// Copy the received response into a buffer, then parse delimiting by '\n' and ','
void Log::ParseLog(String response)
{
  char bufferIn[1200];
  char c;
  int count = 0;

  memset(bufferIn, 0, sizeof(bufferIn));
  
  response.toCharArray(bufferIn, 1200);

  for (int i = 0; i < 56; i++)
  {
    for (int j = 0; j < 2; j++)
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
          this->logFile[i][j] += c;
          count++;
        }
      }
      count++;
      c = bufferIn[count];
    }
  } 
  Serial.println("Log file updated");
}

// Returns specific index of log
String Log::GetLog(int i, int j)
{
  return this->logFile[i][j];
}

// Prints log
void Log::PrintLog()
{
  for (int i = 0; i < 56; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      Serial.print(this->logFile[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
}
