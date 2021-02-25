/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

//////////////////////////
//  Pin initialisation  //
//////////////////////////

const int buzzer = 15;  //buzzer
const int pushBtn = 16; //push button

// Custom classes
#include "User.h";
#include "schedule.h";
#include "SQLConnection.h";
#include "HostAP.h";
#include "DateTime.h";
#include "WebHandler.h";]
#include "Dispenser.h";
#include "Alert.h";
#include "Log.h";

// Libraries
#include <HTTPClient.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Magic to stop brownouts from too low voltage (laptop connection) - do not remove
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//////////////////////////
// Object instantiation //
//////////////////////////

// User(id)
User user(1);

// SQLConnection(IPAddress, port, networkSSID, networkPassword)
SQLConnection conn(IPAddress(192, 168, 1, 134), 8080, "<ssid>", "<pass>"); // enter your own wi-fi network details here
//SQLConnection conn(IPAddress(192, 168, 43, 96), 8080, "<ssid>", "<pass>"); used for hotspot on phone

// HostAccessPoint(NetworkName, NetworkPass, IPAddress, DefaultGateway, SubnetMask)
HostAP ap("ESP32-Test", "H2g8XXopMR", IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

// Datetime
DateTime dt;

// Schedule(id)
Schedule* schedule = new Schedule(user.GetUserID());

// Webhandler
WebHandler webhandler;

// Dispenser
Dispenser dispenser;

// Alert
Alert alert;

// Log
//Log userlog;
Log* userlog = new Log();

// NTP declaration. UK 0 NTP server used.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.uk.pool.ntp.org", 3600);

// Count for Wi-Fi initialization
int count = 0;

//////////////////////////
///// Setup function /////
//////////////////////////

void setup()
{
  // Magic to stop brownouts from too low voltage (laptop connection) - do not remove
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // Begin serial connection and set baud to 115200
  Serial.begin(115200);

  // Pin initialisation
  pinMode(buzzer, OUTPUT);
  pinMode(pushBtn, INPUT);

  // Set up host AP
  WiFi.softAP(ap.GetHostSSID(), ap.GetHostPass());

  // Delay while AP comes up and is configures
  delay(200);
  WiFi.softAPConfig(ap.GetHostIP(), ap.GetHostGateway(), ap.GetHostSubnet());
  delay(200);

  // Configure the handling of the individual pages
  webhandler.server.on("/", handle_Home);
  webhandler.server.on("/home", handle_Home);
  webhandler.server.on("/currentsched", handle_CurrentSchedule);
  webhandler.server.on("/editsched", handle_EditSchedule);
  webhandler.server.on("/update", handle_SendUpdate);

  // Home commands
  webhandler.server.on("/requestsched", handle_RequestSchedule);
  webhandler.server.on("/log", handle_RequestLog);

  // Config handling
  webhandler.server.on("/config", handle_Configuration);
  webhandler.server.on("/networkconfig", handle_NetworkConfiguration);
  webhandler.server.on("/datetimeconfig", handle_DateTimeConfiguration);
  webhandler.server.on("/networkconfigupdate", handle_NetworkConfigurationUpdate);
  webhandler.server.on("/datetimeconfigupdate", handle_DateTimeConfigUpdate);

  // Instead of 404 just send back to home for now
  webhandler.server.onNotFound(handle_Home);

  // Start host AP
  webhandler.server.begin();
  Serial.println("AP up.");

  // Start OTA auth
  ArduinoOTA.begin();
  Serial.println("Authentication active.");

  // Create connection to WiFi-SQL server
  WiFi.begin(conn.GetServerSSID(), conn.GetServerPass());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    count++;
    if (count >= 5)
    {
      Serial.println("Error connecting to WiFi. Continuing in offline mode.");
      break;
    }
    // Moved this - may break
    else if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("\rConnected to WiFi network for network and SQL access. IP: ");
      Serial.println(WiFi.localIP());
    }
  }
  Serial.println("IP to connect to: ");
  Serial.println(ap.GetHostIP());
}

//////////////////////////
/////// Main loop  ///////
//////////////////////////

void loop()
{
  // Update current time
  timeClient.update();
  dt.UpdateDateTime(timeClient);

  // Handle dispensing
  dispenser.HandleDispenser(user, schedule, dt, conn, alert);

  // Handle the client
  webhandler.server.handleClient();

  // Handle authentication
  ArduinoOTA.handle();

  if (alert.ButtonIsPressed())
  {
    alert.DisableBuzzer();
  }
}

// Handle home page
void handle_Home()
{
  // If not authenticated yet
  if (!webhandler.server.authenticate(user.GetAuthUser(), user.GetAuthPass()))
  {
    return webhandler.server.requestAuthentication();
  }

  // Else user is authenticated 
  else
  {
    // Set the URL for SQL queries
    conn.SetScheduleURL(user.GetUserID());

    // If connection is set for SQL do the below
    if (WiFi.status() == WL_CONNECTED)
    {
      // If schedule hasn't yet been updated
      if (!schedule->ScheduleIsUpdated())
      {
        Serial.print("Schedule not updated");
        // If query is succesful
        if (conn.QuerySchedule())
        {
          // Parse the response
          schedule->ParseSchedule(conn.GetResponse());
        }

        // Otherwise begin offline mode
        else
        {
          schedule->SetScheduleOffline();
        }
      }
    }

    // Connection to SQL server is down
    else
    {
      schedule->SetScheduleOffline();
    }

    // Calculate the next dose
    schedule->CalculateNextDose(dt);

    // Set the web handler header to the home page
    webhandler.SetHeader(webhandler.GetHomePage(dt, schedule));
    webhandler.server.send(200, "text/html", webhandler.GetHeader());
  }
}

// Get the current schedule page
void handle_CurrentSchedule()
{
  webhandler.SetHeader(webhandler.GetCurrentSchedulePage(schedule));
  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Get the edit schedule page
void handle_EditSchedule()
{
  webhandler.SetHeader(webhandler.GetEditSchedulePage(schedule));
  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Sends the updated schedule to the server
void handle_SendUpdate()
{
  String* tempUpdatedSched;

  // Parse arguments from server
  webhandler.ParseArgs();

  // Get a temp day/time array from webhandler
  tempUpdatedSched = webhandler.GetTempDayTimeArray();

  // Set the schedule URL with the previously provided temp schedule array
  conn.SetScheduleURL(user.GetUserID(), tempUpdatedSched[0], tempUpdatedSched[1], tempUpdatedSched[2], tempUpdatedSched[3], tempUpdatedSched[4], tempUpdatedSched[5], tempUpdatedSched[6], tempUpdatedSched[7], tempUpdatedSched[8]);

  // If the update is succesful, display success
  if (conn.SendScheduleUpdate())
  {
    schedule->UnsetNextDose();
    schedule->UpdateSchedule(tempUpdatedSched);
    webhandler.SetHeader(webhandler.GetUpdateSchedulePage(true));
  }

  // Else display failed
  else
  {
    webhandler.SetHeader(webhandler.GetUpdateSchedulePage(false));
  }

  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Handles the schedule request 
void handle_RequestSchedule()
{
  // If the query is succesful, reset the schedule and parse the response from the SQL server
  if (conn.QuerySchedule())
  {
    Serial.println("Connection OK");
    schedule->ResetSchedule();
    schedule->ParseSchedule(conn.GetResponse());
    webhandler.SetHeader(webhandler.GetHomePage(dt, schedule));
    webhandler.server.send(200, "text/html", webhandler.GetHeader());
  }
  schedule->PrintSchedule();
}

// Handles the config page
void handle_Configuration()
{
  webhandler.SetHeader(webhandler.GetConfigurationPage());
  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Handles the network config page
void handle_NetworkConfiguration()
{
  webhandler.SetHeader(webhandler.GetNetworkConfigPage(ap, conn));
  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Handles the datetime config page
void handle_DateTimeConfiguration()
{
  webhandler.SetHeader(webhandler.GetDateTimeConfigPage(dt));
  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Handles the update for the network config page
void handle_NetworkConfigurationUpdate()
{
  webhandler.SetHeader(webhandler.GetUpdateNetworkConfig());
  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Handles the update for the datetime config page
void handle_DateTimeConfigUpdate()
{
  webhandler.SetHeader(webhandler.GetUpdateDateTimeConfig(dt));
  webhandler.server.send(200, "text/html", webhandler.GetHeader());
}

// Handles the log request
void handle_RequestLog()
{
  conn.SetLogURL(user.GetUserID());
  
  if (conn.QueryLog())
  {
    Serial.println("Log connection success");
    userlog->ResetLog();
    userlog->ParseLog(conn.GetResponse());
  }
  webhandler.SetHeader(webhandler.GetLog(userlog));
  webhandler.server.send(200, "text/html", webhandler.GetHeader()); 
}
