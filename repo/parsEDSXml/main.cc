#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include "../curl/include/curl/curl.h"

using namespace std;
using namespace tinyxml2;

int main()
{
  std::vector<float> vecList;
  tinyxml2::XMLDocument doc;
  XMLError err = doc.LoadFile( "details.xml" );
  if(err)
    printf("Error %d \n", err);

  XMLElement* root = doc.RootElement();                    //Devices-Detail-Response
  //cout<<root->Name()<<"\n";                           

  //PollCount
  XMLNode* rootchild     = root->FirstChild();               
  //cout<<rootchild->Value()<<": "<<rootchild->FirstChild()->Value()<<"\n";

  //DevicesConnected
  XMLNode *siblingNode  = rootchild->NextSibling();           
  //cout<<siblingNode->Value()<<": "<<siblingNode->FirstChild()->Value()<<"\n";

  //LoopTime
  XMLNode *siblingNode2  = siblingNode->NextSibling();         
  //cout<<siblingNode2->Value()<<": "<<siblingNode2->FirstChild()->Value()<<"\n";

  //MAC
  //cout<<root->FirstChildElement("MACAddress")->Value()<<": "<<root->FirstChildElement("MACAddress")->FirstChild()->ToText()->Value()<<"\n";
  
  XMLNode* lastChild     = root->LastChild();               
  //cout<<lastChild->Value()<<"\n";
  //cout<<lastChild ->FirstChildElement("Temperature")->FirstChild()->ToText()->Value()<<"\n"; 

  /********************************/
  
  cout<<"traverse\n";
  while(rootchild!=NULL)
  {

    if(strcmp(rootchild->Value(),"owd_DS18B20")==0)
    {
      cout<<rootchild->Value()<<"-----------------\n";
    
      siblingNode = rootchild->FirstChild();
      while(siblingNode!=NULL)
      {     
        if (!siblingNode->NoChildren() &&
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

  CURL *curl;
  CURLcode res;

  //curl = curl_easy_init();

  
  return 0;
}
