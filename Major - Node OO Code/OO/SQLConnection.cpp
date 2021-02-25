/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "SQLConnection.h"

SQLConnection::SQLConnection()
{
  
}

SQLConnection::SQLConnection(IPAddress ip, int port, char* ssid, char* pass)
{
  SetServerIP(ip);
  SetServerPort(port);
  SetServerSSID(ssid);
  SetServerPass(pass);
}

SQLConnection::~SQLConnection()
{
  
}

void SQLConnection::SetServerIP(IPAddress ip)
{
  this->sqlServerIP = ip;
}

void SQLConnection::SetServerPort(int port)
{
  this->sqlServerPort = port;
}

void SQLConnection::SetServerSSID(char* ssid)
{
  this->serverSSID = ssid;
}

void SQLConnection::SetServerPass(char* pass)
{
  this->serverPass = pass;
}

void SQLConnection::SetScheduleURL(int ID, String day, String d1, String d2, String d3, String d4, String d5, String d6, String d7, String d8)
{
  // First clear the URL
  ClearURL();

  // Begin the URL
  this->url = "http://";
  this->url += GetServerIP().toString();
  this->url += ":";
  this->url += GetServerPort();

  // If the day parameter is blank, we're not specifying a day. So we're likely querying
  // the entire week schedule.
  if (day == "")
  {
    this->url += "GET /major/GetData.php?id=";
    this->url += ID;
  }

  // Else if the day parameter is not blank, we're updating the schedule on the SQL server
  else if (day != "")
  {
    this->url = "GET /major/UpdateData.php?id=";
    this->url += ID;
    this->url += "&day=";
    this->url += day;
    this->url += "&d1=";
    this->url += d1;
    this->url += "&d2=";
    this->url += d2;
    this->url += "&d3=";
    this->url += d3;
    this->url += "&d4=";
    this->url += d4;
    this->url += "&d5=";
    this->url += d5;
    this->url += "&d6=";
    this->url += d6;
    this->url += "&d7=";
    this->url += d7;
    this->url += "&d8=";
    this->url += d8;
  }
}

void SQLConnection::SetLogURL(int userID,String dosetime, String dispensetime, String userAction)
{
  // Clear URL
  ClearURL();
  
  this->url = "http://";
  this->url += GetServerIP().toString();
  this->url += ":";
  this->url += GetServerPort();

  // If the parameters are blank, we're getting the log
  if (dosetime == "" && dispensetime == "" && userAction == "")
  {
    this->url += "GET /major/GetLog.php?id=";
    this->url += userID;
  }

  // Else we're updating the log so set parameters in URL
  else
  {
    this->url = "GET /major/UpdateLog.php?id=";
    this->url += userID;
    this->url += "&doset=";
    this->url += dosetime;
    this->url += "&dispenset=";
    this->url += dispensetime;
    this->url += "&userpress=";
    this->url += userAction;
  }
  Serial.print("Log URL updated: ");
  Serial.println(this->url);
}

IPAddress SQLConnection::GetServerIP()
{
  return this->sqlServerIP;
}

int SQLConnection::GetServerPort()
{
  return this->sqlServerPort;
}

char* SQLConnection::GetServerSSID()
{
  return this->serverSSID;
}

char* SQLConnection::GetServerPass()
{
  return this->serverPass;
}

bool SQLConnection::QuerySchedule()
{
  HTTPClient http;
  int httpCode;

  // Begin HTTP connection
  http.begin(this->url);

  // Get the response code
  httpCode = http.GET();

  // If the httpcode is succesful get the response
  if (httpCode > 0)
  {
    Serial.println("Schedule query success.");
    
    response = "";
    response = http.getString();

    http.end();

    return true;
  }

  else
  {
    Serial.println("Schedule query failed.");
    http.end();
    return false;
  }
}

bool SQLConnection::QueryLog()
{
  HTTPClient http;
  int httpCode;

  http.begin(this->url);

  httpCode = http.GET();

  if (httpCode > 0)
  {
    Serial.println("Log query success.");
    
    response = "";
    response = http.getString();

    http.end();

    return true;
  }
  else
  {
    Serial.println("Log query failed.");
    http.end();

    return false;
  }
}

String SQLConnection::GetResponse()
{
  return this->response;
}

bool SQLConnection::SendScheduleUpdate()
{
  if (client.connect(sqlServerIP, sqlServerPort))
  {
    Serial.println("Schedule update success.");
    client.println(url);
    return true;
  }
  else
  {
    Serial.println("Schedule update failed.");
    return false;
  }
}

void SQLConnection::SendLogUpdate()
{
  if (client.connect(sqlServerIP, sqlServerPort))
  {
    Serial.println("Log update success.");
    client.println(url);
  }
  else
  {
    Serial.println("Log update failed.");
  }
}

void SQLConnection::ClearURL()
{
  this->url = "";
}
