#ifndef EDSSERVERHANDLER
#define EDSSERVERHANDLER
  
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <vector>

class edsServerHandler
{
  public:
    edsServerHandler(char*& ip);
    void connectToEdsServer();
    void readEdsServerData();
    void decodeServerData();
    void storeServerData();
  private:
  struct sockaddr_in servaddr;                                                     
  struct hostent *hp;
  char* edsServerIp;
  int sock_id;
  char message[2024*1024] = {0};
  int  msglen;
  char* to;
  char* ipAddress;
  struct sensor
  {
    std::string type;
    std::string id;
    std::string value;
  } sensorData;

  std::vector<sensor> sensors;
};
#endif
