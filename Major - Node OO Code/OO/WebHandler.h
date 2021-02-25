/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * The WebHandler object controls all of the web pages for the dispenser.
 * The header is updated depending on the page called, and the header is 
 * passed into the page to display.
 */

#ifndef WebHandler_h
#define WebHandler_h

#include <WebServer.h>
#include <Arduino.h>
#include "schedule.h"
#include "DateTime.h";
#include "HostAP.h";
#include "SQLConnection.h";
#include "User.h";
#include "Log.h";

class WebHandler
{
  // Attributes
private:
  String header;
  String output;
  String tempDayTime[9];

public:
  WebServer server; // Hacky way, but only way I could get this work unfortunately
                    // because the server is initialised in setup, I can't implement
                    // a custom method in here to initialise it during setup.
  
  // Constructors
public:
  WebHandler();
  ~WebHandler();
  
  // Properties
public:
  void SetHeader(String input);
  String GetHeader();

  String* GetTempDayTimeArray();
  String GetHomePage(DateTime dt, Schedule* schedule);
  String GetCurrentSchedulePage(Schedule* schedule);
  String GetEditSchedulePage(Schedule* schedule);
  String GetUpdateSchedulePage(bool status);
  String GetConfigurationPage();
  String GetNetworkConfigPage(HostAP ap, SQLConnection conn);
  String GetDateTimeConfigPage(DateTime dt);
  String GetUpdateDateTimeConfig(DateTime dt);
  String GetUpdateNetworkConfig();
  String GetLog(Log* userlog);

  // Methods
public:
  void ClearHeader();
  void ClearOutput();
  void ParseArgs();
};

#endif
