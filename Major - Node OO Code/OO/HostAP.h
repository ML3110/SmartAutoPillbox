/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This is the HostAP object. It is used to broadcast a network to allow the
 * user to connect their device to the ESP32 to engage with the functionality
 * of the medication dispenser.
 * The properties (Get & Set) are self explanatory, so no code within the .cpp 
 * file has been commented.
 */
 
#ifndef HostAP_h
#define HostAP_h

#include <Arduino.h>

class HostAP
{
  // Attributes
private:
  IPAddress hostIP;
  IPAddress hostGateway;
  IPAddress hostSubnet;
  char* hostSSID;
  char* hostPass;
  
  // Constructors
public:
  HostAP();
  HostAP(char* ssid, char* pass, IPAddress ip, IPAddress gateway, IPAddress subnet);
  ~HostAP();
  
  // Properties
public:
  void SetHostIP(IPAddress ip);
  void SetHostGateway(IPAddress gateway);
  void SetHostSubnet(IPAddress subnet);
  void SetHostSSID(char* ssid);
  void SetHostPass(char* pass);

  char* GetHostSSID();
  char* GetHostPass();
  IPAddress GetHostIP();
  IPAddress GetHostGateway();
  IPAddress GetHostSubnet();
  
  // Methods
public:

};

#endif
