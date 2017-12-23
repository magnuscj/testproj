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

using namespace std;
using namespace tinyxml2;

int main()
{
  int counter = 0;
  int errors  = 0;
  int socketErrors =0;
  struct sockaddr_in servaddr;
  struct hostent *hp;
  int sock_id;
  char message[2024*1024] = {0};
  int msglen;
  char request[] = "GET /details.xml HTTP/1.0\n\r\n";//
   
  std::system("clear");
  while(1)
  {
    counter++;
    cout << "\033[1;1H";
    //Get a socket
    if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      fprintf(stderr,"Couldn't get a socket.\n"); exit(EXIT_FAILURE);
      socketErrors++;
    }
    else
    {
      //fprintf(stderr,"Got a socket.\n");
    }

    //book uses bzero which my man pages say is deprecated
    //the man page said to use memset instead. :-)
    memset(&servaddr,0,sizeof(servaddr));

    //get address for google.com
    if((hp = gethostbyname("192.168.1.87")) == NULL)
    {
      fprintf(stderr,"Couldn't get an address.\n"); exit(EXIT_FAILURE);
      socketErrors++;
    }
    else
    {
      //fprintf(stderr,"Got an address.\n");
    }

    //bcopy is deprecated also, using memcpy instead
    memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);

    //fill int port number and type
    servaddr.sin_port = htons(80);
    servaddr.sin_family = AF_INET;

    //make the connection
    if(connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
       fprintf(stderr, "Couldn't connect.\n");
      socketErrors++;
    }
    else
    {
      //fprintf(stderr,"Got a connection!!!\n");
    }

    //NOW THE HTTP PART!!!
    //send the request
    write(sock_id,request,strlen(request));
    sleep(1);
    //read the response
    msglen = read(sock_id,message,2024*1024);
    printf("response is %d bytes long\n", msglen);

   
    int i = 127; //Skip the initial response message
    int j = msglen;
    char *to = new char[j-i+1];
    strncpy(to, message+i, j-i);
    to[j-i]='\0';
    //print the reasponse
    //      printf("%s", to);
  
    tinyxml2::XMLDocument doc;
    //XMLError err = doc.LoadFile( "details2.xml" );
    XMLError err = doc.Parse(to);
  
    if(err)
    {
      printf("Error %d \n", err);
      errors++;
    }
    else
    {
      XMLElement* root    = doc.RootElement();                    //Devices-Detail-Response

      //PollCount
      XMLNode* rootchild  = root->FirstChild();               

      //DevicesConnected
      XMLNode *siblingNode= rootchild->NextSibling();           
      
      /********************************/
  
      cout<<"Loop number "<<counter <<" with "<<errors<<" errors and "<<socketErrors<<" socket errors.\n";

      while(rootchild!=NULL)
      { 
        if(strcmp(rootchild->Value(),"owd_DS18B20")==0)
        {
          cout<<rootchild->Value()<<"-----------------\n";
    
          siblingNode = rootchild->FirstChild();
          while(siblingNode!=NULL)
          {     
            if(!siblingNode->NoChildren() &&
	      (strcmp(siblingNode->Value(), "Temperature")==0))
            {
	      cout<<siblingNode->Value()<<": ";
	      cout<<siblingNode->FirstChild()->Value()<<"\n";
            }
        
            siblingNode=siblingNode->NextSibling();
          }
        }
        rootchild = rootchild->NextSibling();
      }
    }
    sleep(59);
    delete(to);
    doc.Clear();
    
  }
  return 0;
}
