/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

#include "HostAP.h"

HostAP::HostAP()
{
  
}

HostAP::HostAP(char* ssid, char* pass, IPAddress ip, IPAddress gateway, IPAddress subnet)
{
  SetHostSSID(ssid);
  SetHostPass(pass);
  SetHostIP(ip);
  SetHostGateway(gateway);
  SetHostSubnet(subnet);
}

HostAP::~HostAP()
{

}

void HostAP::SetHostIP(IPAddress ip)
{
  this->hostIP = ip;
}

void HostAP::SetHostGateway(IPAddress gateway)
{
  this->hostGateway = gateway;
}

void HostAP::SetHostSubnet(IPAddress subnet)
{
  this->hostSubnet = subnet;
}

void HostAP::SetHostSSID(char* ssid)
{
  this->hostSSID = ssid;
}

void HostAP::SetHostPass(char* pass)
{
  this->hostPass = pass;
}

char* HostAP::GetHostSSID()
{
  return this->hostSSID;
}

char* HostAP::GetHostPass()
{
  return this->hostPass;
}

IPAddress HostAP::GetHostIP()
{
  return this->hostIP;
}
IPAddress HostAP::GetHostGateway()
{
  return this->hostGateway;
}
IPAddress HostAP::GetHostSubnet()
{
  return this->hostSubnet;
}
