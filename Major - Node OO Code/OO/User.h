/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * The user object stores the user ID and schedule. The properties are
 * self explanatory, so no code within the .cpp file has been commented.
 */

#ifndef User_h
#define User_h

#include <Arduino.h>
#include "schedule.h"

class User
{
  // Attributes
private:
  int userID;
  const char* authUser = "admin";
  const char* authPass = "pass";
  Schedule* schedule;

  // Constructor
public:
  User();
  User(int id);
  ~User();

  // Properties
public:
  void SetUserID(int id);
  void SetAuthUser(const char* user);
  void SetAuthPass(const char* pass);

  int GetUserID();
  const char* GetAuthUser();
  const char* GetAuthPass();

  // Methods
  void BindSchedule(Schedule* schedule);
  void DeleteSchedule();
  void UpdateSchedule(Schedule* schedule);
};

#endif
