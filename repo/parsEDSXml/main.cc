#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <iomanip>
#include <time.h>

using namespace std;
using namespace tinyxml2;

int main(int argc, char* argv[])
{
  int counter = 0;
  int errors  = 0;
  MYSQL_RES *result;
  MYSQL_ROW row;
  MYSQL *connection, mysql;
  int state;
  string dbName   = "mydb";
  string tbName   = "table";
  string sensorid = "";
  
  mysql_init(&mysql);

  
  //char sensorsTypes[2][2]={{"owd_DS18B20","Temperature"},
  //                         {"owd_DS2423" ,"Counter_A"}}; 
  
  bool errorFlag = false;
  struct sockaddr_in servaddr;
  struct hostent *hp;
  int  socketErrors = 0;
  int  sock_id = 0;
  char message[2024*1024] = {0};
  int  msglen = 0;
  char request[] = "GET /details.xml HTTP/1.0\n\r\n";
   
  std::system("clear");
  while(1)
  {
    connection = mysql_real_connect(&mysql,"192.168.1.45","root","root",0,0,0,0);

    if (connection == NULL)
    {
      cout<<mysql_error(&mysql);
      return 1;
    }
    counter++;
    cout << "\033[1;1H";
    for(int serverNo = 1 ; serverNo<argc ; serverNo++ )
      {
	//cout<<"Sensors for server "<<argv[serverNo]<<".\n";
	errorFlag = false;
      
	//Get a socket
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	  {
	    errorFlag = true;
	    fprintf(stderr,"Couldn't get a socket.\n"); //exit(EXIT_FAILURE);
	    
	    socketErrors++;
	  }
	else
	  {
	    //fprintf(stderr,"Got a socket.\n");
	    //cout<<"Socket id "<<sock_id<<".\n";
	  }

        if(!errorFlag)
	  {

	    memset(&servaddr,0,sizeof(servaddr));

	    //get address
	    if(((hp = gethostbyname(argv[serverNo])) == NULL))
	      {
		//fprintf(stderr,"Couldn't get an address.\n"); exit(EXIT_FAILURE);
		socketErrors++;
	      }
	    else
	      {
		//fprintf(stderr,"Got an address.\n");
	      }

	    memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);

	    //fill in port number and type
	    servaddr.sin_port = htons(80);
	    servaddr.sin_family = AF_INET;

	    //make the connection
	    if(connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
	      {
		errorFlag = true;
		//fprintf(stderr, "Couldn't connect.\n");
		socketErrors++;
	      }
	    else
	      {
		//fprintf(stderr,"Got a connection!!!\n");
	      }


	    //Send the HTTP request
	    write(sock_id,request,strlen(request));

	    //Sleep for a while so that the server get time to fill the buffer
	    usleep(200000);//0,000 000 1
	    //read the response
	    msglen = read(sock_id,message,2024*1024);
	    //printf("response is %d bytes long\n", msglen);

	    close(sock_id);
   
	    int i = 127; //Skip the initial response message
	    int j = (msglen>127 ? msglen : 127);
	    char *to = new char[j-i+1];
	    strncpy(to, message+i, j-i);
	    to[j-i]='\0';
	    //print the reasponse
	    //cout<<to;
  
	    tinyxml2::XMLDocument doc;
	    //XMLError err = doc.LoadFile( "details2.xml" );
	    XMLError err = doc.Parse(to);
  
	    if(err)
	      {
		printf("Error %d \n", err);
		std::system("clear");
		errors++;
	      }
	    else
	      {
		for(int i=0;i<1;i++)
		  {
		    XMLElement* root    = doc.RootElement();                    //Devices-Detail-Response

		    //PollCount
		    XMLNode* rootchild  = root->FirstChild();               

		    //DevicesConnected
		    XMLNode *siblingNode= rootchild->NextSibling();           
      
		    while(rootchild!=NULL)
		      { 
			if((strcmp(rootchild->Value(),"owd_DS18B20")==0) || (strcmp(rootchild->Value(),"owd_DS2423")==0))
			  {
			    cout<<left;
			    cout<<setw(15)<<argv[serverNo]<<setw(13)<<rootchild->Value();
    
			    siblingNode = rootchild->FirstChild();
			    while(siblingNode!=NULL)
			      {

				if(!siblingNode->NoChildren() &&
				   (strcmp(siblingNode->Value(), "ROMId")==0))
				  {
				    sensorid = siblingNode->FirstChild()->Value();
				    cout<<  siblingNode->FirstChild()->Value()<<"  ";
				  }
				
				if(!siblingNode->NoChildren() &&
				   (strcmp(siblingNode->Value(), "Temperature")==0))
				  {
				    cout<<setw(11)<<siblingNode->Value()<<": ";
				    cout<<siblingNode->FirstChild()->Value()<<"\n";

				     time_t t = time(NULL);
				     struct tm tm = *localtime(&t);
				     string date = to_string(tm.tm_year + 1900).append(to_string(tm.tm_mon + 1));//<< tm.tm_mon + 1;
				     string value = siblingNode->FirstChild()->Value(); 
				     string query = "INSERT INTO " + dbName + "." + tbName + date + " (sensorid, data) VALUES('" + sensorid + "', '" + value + "')";
				     
				     //Create db
				     state = mysql_query(connection, string("CREATE DATABASE "+dbName).c_str());
				     //cout<<"\n"<<state<<" "<<mysql_error(connection);
  
				     //Create table
				     state = mysql_query(connection, string("CREATE TABLE "+dbName+"."+ tbName + date + " (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, sensorid TEXT, data float(23,3), curr_timestamp TIMESTAMP)").c_str());
				     //cout<<"\n"<<state<<" "<<mysql_error(connection);
 
				     //Store data
				     state = mysql_query(connection, query.c_str());
				     //cout<<"\n"<<state<<" "<<mysql_error(connection)<<"\n";
				  }
				else if (!siblingNode->NoChildren() &&
					 (strcmp(siblingNode->Value(), "Counter_A"))==0)
				  {
				    cout<<setw(11)<<siblingNode->Value()<<": ";
				    cout<<siblingNode->FirstChild()->Value()<<"\n";

				  }
				siblingNode=siblingNode->NextSibling();
			      }
			  }
			rootchild = rootchild->NextSibling();
		      }
		  }
	      }
	   
	    delete(to);
	    doc.Clear();
	    mysql_free_result(result);
	    mysql_close(connection);
	  }
      }
    cout<<"Loop number "<<counter <<" with "<<errors<<" errors and "<<socketErrors<<" socket errors.\n";
    
    sleep(60);
  }
  return 0;
}
