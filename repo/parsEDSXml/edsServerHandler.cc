#include "edsServerHandler.h"
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <iomanip>
#include <time.h>
#include "tinyxml2.h"
#include <vector>
#include <utility>
using namespace std;
using namespace tinyxml2;

//Constructor
//-------------------------------

edsServerHandler::edsServerHandler(char*& ip)
{
   ipAddress = ip;
   struct hostent *hp;                                                                      
   sock_id = 0;
   msglen = 0;
   sensors.clear();

}


void edsServerHandler::connectToEdsServer()
{

   if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      cout<<stderr<<", Couldn't get a socket.\n";
   }
   else
   {
      //cout<<"Socket id "<<sock_id<<".\n";
   }
  
   memset(&servaddr,0,sizeof(servaddr));
   
   //get address
   if(((hp = gethostbyname(ipAddress)) == NULL))
   {
      cout<<stderr<<", Couldn't get an address.\n"; 
   }
   else
   {
      //cout<<stderr<<", got an address.\n";
   }

   memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);

   //fill in port number and type
   servaddr.sin_port = htons(80);
   servaddr.sin_family = AF_INET;

   //make the connection
   if(connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
   {
      cout<<stderr<<", Couldn't connect.\n";
   }
   else
   {
      //cout<<stderr<<", got a connection!!!\n";
   }
}

void edsServerHandler::readEdsServerData()
{
	
   char request[] = "GET /details.xml HTTP/1.0\n\r\n";
   //Send the HTTP request
   write(sock_id,request,strlen(request));

   //Sleep for a while so that the server get time to fill the buffer
   usleep(200000);//0,000 000 1
   
   //read the response
   msglen = read(sock_id,message,2024*1024);
   //cout<<"response from "<<sock_id<<" is "<< msglen<<" bytes long\n";
 
   close(sock_id);
   
   int i = 127; //Skip the initial response message
   int j = (msglen>127 ? msglen : 127);
   to = new char[j-i+1];
   strncpy(to, message+i, j-i);
   to[j-i]='\0';
   //print the reasponse
   //cout<<to;
 
}

void edsServerHandler::decodeServerData()
{
  string sensorid = "";
  tinyxml2::XMLDocument doc;
  XMLError err = doc.Parse(to);
  std::pair <string,string> sensorType;
  vector <std::pair <string,string>> sensorTypes;

  sensorTypes.push_back(std::make_pair("owd_DS18B20","Temperature"));
  sensorTypes.push_back(std::make_pair("owd_DS2423","Counter_A"));


  if(err)
  {
	  printf("Error %d \n", err);
	  std::system("clear");
  }
  else
  {
    for( auto sensorType : sensorTypes)
    {
      XMLElement* root    = doc.RootElement();       //Devices-Detail-Response
      XMLNode* rootchild  = root->FirstChild();      //PollCount
      XMLNode *siblingNode= rootchild->NextSibling();//DevicesConnected

      while(rootchild!=NULL)
      { 
        if((sensorType.first.compare(rootchild->Value())==0))
        {
          siblingNode = rootchild->FirstChild();
	  sensorData.type = rootchild->Value();

	  while(siblingNode!=NULL)
          {
		  if(!siblingNode->NoChildren() && (strcmp(siblingNode->Value(), "ROMId")==0))
		  {
			  sensorData.id = siblingNode->FirstChild()->Value();
		  }
		  if(!siblingNode->NoChildren() && (sensorType.second.compare(siblingNode->Value()) ==0))
		  {
                          sensorData.value = siblingNode->FirstChild()->Value();
		  }
		  siblingNode=siblingNode->NextSibling();
	  }
	sensors.push_back(sensorData);
	}
	rootchild = rootchild->NextSibling();
      }
    }
  }

  delete(to);
  doc.Clear();
}


void edsServerHandler::storeServerData()
{
  MYSQL_RES *result;
  MYSQL_ROW row;
  MYSQL *connection, mysql;
  int state;
  string dbName   = "mydb";
  string tbName   = "table";
  string sensorid = "";
  
  mysql_init(&mysql);

   connection = mysql_real_connect(&mysql,"192.168.1.45","root","root",0,0,0,0);

   if (connection == NULL)
   {
     cout<<mysql_error(&mysql);
     return ;
   }
 
      
  for( auto &sensor : sensors)
  {
    cout<<left;
    cout<<setw(13)<<sensor.type<<sensor.id<<": "<<setw(8)<<sensor.value<<"  "<<ipAddress<<"\n";  
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    string date = to_string(tm.tm_year + 1900).append((((1+tm.tm_mon) <=9) ? "0" + to_string(tm.tm_mon+1) : to_string(tm.tm_mon+1 )));
    state = mysql_query(connection, string("CREATE DATABASE "+dbName).c_str());
    state = mysql_query(connection, string("CREATE TABLE "+dbName+"."+ tbName + date + 
		   " (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, sensorid TEXT, data float(23,3), curr_timestamp TIMESTAMP)").c_str());
    string query = "INSERT INTO " + dbName + "." + tbName + date + " (sensorid, data) VALUES('" + sensor.id + "', '" + sensor.value + "')";
    state = mysql_query(connection, query.c_str());
  }
  
  mysql_close(connection);

}





















































