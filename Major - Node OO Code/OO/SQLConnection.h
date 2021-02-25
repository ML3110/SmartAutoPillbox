/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This object controls the connection to the SQL server. It both reads the 
 * user schedule from the server and sends log updates.
 */

#ifndef SQLConnection_h
#define SQLConnection_h

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "schedule.h"

class SQLConnection
{
  // Attributes
private:
  WiFiClient client;
  IPAddress sqlServerIP;
  int sqlServerPort;
  char* serverSSID;
  char* serverPass;
  String response;
  String url;
  
  // Constructors
public:
  SQLConnection();
  SQLConnection(IPAddress ip, int port, char* ssid, char* pass);
  ~SQLConnection();
  
  // Properties
public:
  void SetServerIP(IPAddress ip);
  void SetServerPort(int port);
  void SetServerSSID(char* ssid);
  void SetServerPass(char* pass);
  void SetScheduleURL(int userID, String day = "", String d1 = "", String d2 = "", String d3 = "", String d4 = "", String d5 = "", String d6 = "", String d7 = "", String d8 = "");
  void SetLogURL(int userID, String dosetime = "", String dispensetime = "", String userAction = "");

  IPAddress GetServerIP();
  char* GetServerSSID();
  char* GetServerPass();
  int GetServerPort();
  String GetResponse();
  
  // Methods
public:
  bool QuerySchedule();
  bool QueryLog();
  bool SendScheduleUpdate();
  void SendLogUpdate();
  void ClearURL();
};

#endif
