/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#ifndef Log_h
#define Log_h

#include <Arduino.h>

class Log
{
  // Attributes
private:
  String logFile[56][2];
  
  // Constructors
public:
  Log();
  ~Log();
  
  // Properties
public:
  String GetLog(int i, int j);
  
  // Methods
public:
  void ResetLog();
  void ParseLog(String response);
  void PrintLog();
};

#endif
