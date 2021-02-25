/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "User.h"

User::User()
{
  this->userID = NULL;
  this->schedule = NULL;
}

User::User(int id)
{
  SetUserID(id);
  this->schedule = NULL;
}

User::~User()
{

}

int User::GetUserID()
{
  return this->userID;
}

const char* User::GetAuthUser()
{
  return this->authUser;
}

const char* User::GetAuthPass()
{
  return this->authPass;
}

void User::SetUserID(int id)
{
  this->userID = id;
}

void User::SetAuthUser(const char* user)
{
  this->authUser = user;
}

void User::SetAuthPass(const char* pass)
{
  this->authPass = pass;
}

void User::BindSchedule(Schedule* schedule)
{
  this->schedule = schedule;
}

void User::DeleteSchedule()
{
  this->schedule = NULL;
}

void User::UpdateSchedule(Schedule* schedule)
{
  DeleteSchedule();
  this->schedule = schedule;
}
