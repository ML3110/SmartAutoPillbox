/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "WebHandler.h"

WebHandler::WebHandler()
{
  
}

WebHandler::~WebHandler()
{
  
}

String WebHandler::GetHeader()
{
  return this->header;
}

// Builds homepage in "output" variable for the header
String WebHandler::GetHomePage(DateTime dt, Schedule* schedule)
{
  ClearOutput();

  Serial.println(schedule->GetNextDoseTime());
  
  this->output += "<h1>Medication Dispenser</h1>";
  this->output += "<h3>Home Page</h3>";
  this->output += "<h3>Current Day & Time:</h3>";
  this->output += "<p>";
  this->output += dt.GetWeekDay();
  this->output += " ";
  this->output += dt.GetTimeStamp();
  this->output += "</p>";

  this->output += "<h3>Next dose:</h3>";
  this->output += "<p style='text-transform:capitalize;'>";
  this->output += schedule->GetNextDoseDay();
  this->output += " ";
  this->output += schedule->GetNextDoseTime();
  this->output += "</p>";

  return this->output;
}

// Builds currentschedule in "output" variable for the header
String WebHandler::GetCurrentSchedulePage(Schedule* schedule)
{
  ClearOutput();
  
  this->output += "<h1>Current Schedule</h1>";

  // Loop for each day
  for (int i = 0; i < 7; i++)
  {
    int c = 0;
    
    this->output += "<label for='" + schedule->GetSchedule(i, 0) + "' class='accordion'>" + schedule->GetSchedule(i, 0) + " (...)</label>";
    this->output += "<input type='checkbox' id='" + schedule->GetSchedule(i, 0) + "' style='display:none' />";
    this->output += "<div class='shrink'>";
    
    this->output += "<table id='schedule'>";

    // Loop for 8 doses
    for (int j = 0; j < 8; j++)
    {
      // If the returned value of the schedule isn't blank, add the dose
      if (schedule->GetSchedule(i, j+1) != "")
      {
        c++;
        this->output += "<tr>";
        this->output += "<td>";
        this->output += "Dose ";
        this->output += c;
        this->output += ": ";
        this->output += "</td>";
        this->output += "<td>";
        this->output += schedule->GetSchedule(i, j+1); // dose
        this->output += "</td>";
        this->output += "</tr>";
      }
    }

      this->output += "<tr>";
      this->output += "<td colspan='2'>";
      this->output += "<button type='button'><a href='editsched?day=";
      this->output += schedule->GetSchedule(i, 0); // Day
      this->output += "'>Edit</a></button>";
      this->output += "</td>";
      this->output += "</tr>";
    
    this->output += "</table>";

    this->output += "</div>";
  }

  return this->output;
}

// Builds editschedule in "output" variable for the header
String WebHandler::GetEditSchedulePage(Schedule* schedule)
{
  ClearOutput();
  
  int c = 0;

  // Loop for each
  for (int i = 0; i < 7; i++)
  {
    // Finding the day
    if (server.arg(0) == schedule->GetSchedule(i, 0))
    {
      this->output += "<h1>Edit Day</h1>";

      this->output += "<button type='button'><a href='/currentsched'>Back</a></button>";

      this->output += "<table id='schedule'";
      this->output += "<tr>";
      this->output += "<th>" + schedule->GetSchedule(i, 0) + "</th>";
      this->output += "</tr>";

      this->output += "<tr>";
      this->output += "<form method='GET' action='update'>";
      this->output += "<tr>";

      this->output += "<input type='text' value='";
      this->output += schedule->GetSchedule(i, 0);

      // Have to hide the input so user doesn't see it, but it's needed for parameters on form submit
      this->output += "' name='day' readonly style='visibility:hidden;'>";
      this->output += "</tr>";

      // Loop for each dose
      for (int j = 0; j < 8; j++)
      {
        // If the returned value isn't blank, show it
        if (schedule->GetSchedule(i, j+1) != "")
        {
          c++;
          this->output += "<tr>";
          this->output += "<td>";
          this->output += "Dose ";
          this->output += c;
          this->output += ": ";
          this->output += "<input type='time' name='time";
          this->output += j+1;
          this->output += "' value='";
          this->output += schedule->GetSchedule(i, j+1);
          this->output += "'>";
          this->output += "</td>";
          this->output += "</tr>";
        }
      }

      this->output += "<tr><td></br></td></tr>";

      for (int j = 0; j < 8; j++)
      {
        // If the returned value is blank, display "Add dose" instead
        if (schedule->GetSchedule(i, j+1) == "")
        {
          c++;
          this->output += "<tr>";
          this->output += "<td>";
          this->output += "Add dose ";
          this->output += c;
          this->output += ": ";
          this->output += "<input type='time' name='time";
          this->output += j+1;
          this->output += "' value='";
          this->output += schedule->GetSchedule(i, j+1);
          this->output += "'>";
          this->output += "</td>";
          this->output += "</tr>";
        }
      }

      this->output += "<tr>";
      this->output += "<td>";
      this->output += "<input type='submit' value='Submit'>";
      this->output += "</td>";
      this->output += "</tr>";

      this->output += "</form>";
      this->output += "</tr>";
      this->output += "</table>";      
    }
  }

  return this->output;
}

// Parsing the args from the url
void WebHandler::ParseArgs()
{
  for (int i = 0; i < server.args(); i++)
  {
    tempDayTime[i] = server.arg(i);
  }

  for (int i = 1; i < 9; i++)
  {
    for (int j = 1; j < 9; j++)
    {
      if (tempDayTime[i] < tempDayTime[j])
      {
        String temp2 = tempDayTime[i];
        tempDayTime[i] = tempDayTime[j];
        tempDayTime[j] = temp2;
      }
    }
  }
}

String WebHandler::GetUpdateSchedulePage(bool status)
{
  ClearOutput();

  if (status)
  {
    this->output += "<h3>Day updated!</h3>";
    this->output += "<button type='button'><a href='currentsched'>Back</a></button>";
  }
  else
  {
    this->output += "<h3>Update request failed!</h3>";
    this->output += "<button type='button'><a href='currentsched'>Back</a></button>";
  }

  return this->output;
}

void WebHandler::ClearHeader()
{
  this->header = "";
}

// Builds a normal HTML page including nav, and adds the output variable to it within the main section
void WebHandler::SetHeader(String input)
{
  ClearHeader();

  // Set head, title, style, header, etc..
  this->header += "<!DOCTYPE html> <html>";
  this->header += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">";
  this->header += "<title>Smart Medication Dispenser</title>";
  
  this->header += "<style>html{font-family:Helvetica;display:inline-block;margin:0 auto;text-align:center}body{margin:0;background-color:#BFDBFF}h1{color:#444;}.accordion{border: 5px solid #D9E9FF;display:block;padding:}h3{color:#444;}.main{font-size:20px;margin-top:70px;}a {color:#000;}#schedule{width:100%;text-align:center;padding:8px}#schedule th{padding-top:12px;padding-bottom:12px;background-color:#62A6FF;color:000;text-transform:capitalize}.accordion{display: block;padding-top:12px;padding-bottom:12px;background-color:#62A6FF;color:000;text-transform:capitalize}.shrink{display:none;}input:checked + .shrink{display:block;}.header {background-color:#62A6FF;box-shadow: 1px 1px 4px 0 rgba(0,0,0,.1);top:0;position:fixed;width:100%;z-index:3;}.header ul {margin:0;padding:0;list-style:none;overflow:hidden;background-color:#99AFCC;}.header li a {display:block;padding: 20px 20px;border-right: 1px solid #f4f4f4;text-decoration:none;}.header li a:hover,.header .menu-btn:hover {background-color:#f4f4f4;}.header .logo {display:block;float:right;font-size:20px;padding: 20px 20px;text-decoration:none;}.header .menu {clear:both;max-height:0;}.header .menu-icon {cursor:pointer;display:inline-block;float:left;padding: 28px 20px;position:relative;user-select:none;}.header .menu-icon .navicon {background:#333;display:block;height:2px;position:relative;width:18px;}.header .menu-icon .navicon:before,.header .menu-icon .navicon:after {background:#333;content:'';display:block;height:100%;position:absolute;transition: all .2s ease-out;width:100%;}.header .menu-icon .navicon:before {top:5px;}.header .menu-icon .navicon:after {top:-5px;}.header .menu-btn {display: none;}.header .menu-btn:checked ~ .menu {max-height:240px;}.header .menu-btn:checked ~ .menu-icon .navicon {background:transparent;}.header .menu-btn:checked ~ .menu-icon .navicon:before {transform:rotate(-45deg);}.header .menu-btn:checked ~ .menu-icon .navicon:after {transform:rotate(45deg);}.header .menu-btn:checked ~ .menu-icon:not(.steps) .navicon:before,.header .menu-btn:checked ~ .menu-icon:not(.steps) .navicon:after {top:0;}</style>";
  this->header += "</head>";

  this->header += "<header class='header'>";
  this->header += "<a href='home' class='logo'>Medication Dispenser</a>";
  this->header += "<input class='menu-btn' type='checkbox' id='menu-btn'>";
  this->header += "<label class='menu-icon' for='menu-btn'><span class='navicon'></span></label>";
  this->header += "<ul class='menu'>";
  this->header += "<li><a href='home'>Home</a>";
  this->header += "<li><a href='currentsched'>Current Schedule</a>";
  this->header += "<a href='config'>Configuration</a>";
  this->header += "<a href='log'>Log</a>";
  this->header += "</ul>";
  this->header += "</header>";

  this->header += "<body>";

  // Main
  this->header += "<div class='main'>";

  // Add the output from the other methods
  this->header += this->output;

  this->header += "</div>";
  this->header += "</body>";
  this->header += "</html>";
}

String* WebHandler::GetTempDayTimeArray()
{
  return this->tempDayTime;
}

void WebHandler::ClearOutput()
{
  this->output = "";
}

String WebHandler::GetConfigurationPage()
{
  ClearOutput();

  this->output += "<h1>Configuration</h1>";
  this->output += "<button type='button'><a href='networkconfig'>Network Config</a></button></br></br>";
  this->output += "<button type='button'><a href='datetimeconfig'>Date/Time Config</a></button>";

  return this->output;
}

String WebHandler::GetNetworkConfigPage(HostAP ap, SQLConnection conn)
{
  ClearOutput();

  this->output += "<table id='schedule'>";
  this->output += "<tr>";
  this->output += "<th colspan='2'>Network Config</th>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td style='font-weight:bold;'>Stored SSID:</td>";
  this->output += "<td>";
  this->output += conn.GetServerSSID();
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td style='font-weight:bold;'>Stored Passphrase:</td>";
  this->output += "<td>";
  this->output += "******************";
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td style='font-weight:bold;'>Host IP:</td>";
  this->output += "<td>";
  this->output += ap.GetHostIP().toString();
  this->output += "</td>";
  this->output += "</tr>";

  this->output += "<tr>";
  this->output += "<td></br></td>";
  this->output += "</tr>";

  this->output += "<tr>";
  this->output += "<td colspan='2'>";
  this->output += "<a href='networkconfigupdate'><button>Update</button></a>";
  this->output += "</td>";
  this->output += "</tr>";

  this->output += "</table>";

  return this->output;
}

String WebHandler::GetDateTimeConfigPage(DateTime dt)
{
  ClearOutput();

  this->output += "<table id='schedule'>";
  this->output += "<tr>";
  this->output += "<th colspan='2'>Date/Time Config</th>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td style='font-weight:bold;'>Date:</td>";
  this->output += "<td>";
  this->output += dt.GetWeekDay();
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td style='font-weight:bold;'>Time:</td>";
  this->output += "<td>";
  this->output += dt.GetTimeStamp();
  this->output += "</td>";
  this->output += "</tr>";

  this->output += "<tr>";
  this->output += "<td></br></td>";
  this->output += "</tr>";

  this->output += "<tr>";
  this->output += "<td colspan='2'>";
  this->output += "<a href='datetimeconfigupdate'><button>Update</button></a>";
  this->output += "</td>";
  this->output += "</tr>";

  this->output += "</table>";

  return this->output;
}

String WebHandler::GetUpdateDateTimeConfig(DateTime dt)
{
  ClearOutput();

  this->output += "<table id='schedule'>";
  this->output += "<tr>";
  this->output += "<th colspan='2'>Set Day/Time</th>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<form method='GET' action='updatedatetime'>";
  this->output += "<td style='font-weight:bold;'>Day:</td>";
  this->output += "<td>";
  this->output += "<select id='day' name='day'>";
  this->output += "<option value='monday'>Monday</option>";
  this->output += "<option value='tuesday'>Tuesday</option>";
  this->output += "<option value='wednesday'>Wednesday</option>";
  this->output += "<option value='thursday'>Thursday</option>";
  this->output += "<option value='friday'>Friday</option>";
  this->output += "<option value='saturday'>Saturday</option>";
  this->output += "<option value='sunday'>Sunday</option>";
  this->output += "</select>";
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td style='font-weight:bold;'>Time:</td>";
  this->output += "<td>";
  this->output += "<input type='time' name='time' value='00:00'>";
  this->output += "</td>";
  this->output += "</tr>";

  this->output += "<tr>";
  this->output += "<td></br></td>";
  this->output += "</tr>";

  this->output += "<tr>";
  this->output += "<td colspan='2'>";
  this->output += "<a href='home'><button>Update</button></a>";
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "</form>";
  this->output += "</table>";

  return this->output;
}

String WebHandler::GetLog(Log* userlog)
{
  ClearOutput();

  this->output += "<h1>7 Day Log</h1>";

  int c = 0;
  
  for (int i = 0; i < 7; i++)
  {
    this->output += "<label for='" + userlog->GetLog(c,0) + "' class='accordion'>" + userlog->GetLog(c,0) + " (...)</label>";
    this->output += "<input type='checkbox' id='" + userlog->GetLog(c, 0) + "' style='display:none' />";
    this->output += "<div class='shrink'>";
    this->output += "<table id='schedule'>";
  
    for (int j = 0; j < 8; j++)
    {
      this->output += "<tr>";
    
      for (int k = 0; k < 2; k++)
      {
        if (userlog->GetLog(c, k) != "")
        {
          this->output += "<td>";
          this->output += userlog->GetLog(c,k);
          this->output += "</td>";
        }
      }
      this->output += "</tr>";
      c++;
    }
    this->output += "</table>";
    this->output += "</div>";
  }
  
  return this->output;
}

String WebHandler::GetUpdateNetworkConfig()
{
  ClearOutput();

  this->output += "<table id='schedule'>";
  this->output += "<tr>";
  this->output += "<th colspan='2'>Change Network Config</th>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<form method='GET' action='updatenetwork'>";
  this->output += "<td style='font-weight:bold;'>SSID:</td>";
  this->output += "<td>";
  this->output += "<input type='text' id='ssid' name='ssid'>";
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td style='font-weight:bold;'>Passphrase:</td>";
  this->output += "<td>";
  this->output += "<input type='password' id='pass' name='pass'>";
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td></br></td>";
  this->output += "</tr>";
  
  this->output += "<tr>";
  this->output += "<td colspan='2'>";
  this->output += "<a href='home'><button>Update</button></a>";
  this->output += "</td>";
  this->output += "</tr>";
  
  this->output += "</form>";
  this->output += "</table>";
  
  return this->output;
}
