#include <iostream>
#include "tinyxml2.h"
#include <vector>

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
  cout<<root->Name()<<"\n";                           

  //PollCount
  XMLNode* rootchild     = root->FirstChild();               
  cout<<rootchild->Value()<<": "<<rootchild->FirstChild()->Value()<<"\n";

  //DevicesConnected
  XMLNode *siblingNode  = rootchild->NextSibling();           
  cout<<siblingNode->Value()<<": "<<siblingNode->FirstChild()->Value()<<"\n";

  //LoopTime
  XMLNode *siblingNode2  = siblingNode->NextSibling();         
  cout<<siblingNode2->Value()<<": "<<siblingNode2->FirstChild()->Value()<<"\n";

  //MAC
  cout<<root->FirstChildElement("MACAddress")->FirstChild()->ToText()->Value()<<"\n";
  cout<<root->FirstChildElement("MACAddress")->Value()<<"\n";

  XMLNode* lastChild     = root->LastChild();               
  cout<<lastChild->Value()<<"\n";
  cout<<lastChild ->FirstChildElement("Temperature")->FirstChild()->ToText()->Value()<<"\n"; 
  
  XMLNode* firstChild     = root->FirstChild();               
  cout<<firstChild->Value()<<"\n";
  //cout<<firstChild->FirstChildElement("Temperature")->FirstChild()->ToText()->Value()<<"\n";   
  //cout<<firstChild->FirstChildElement("Temperature")->FirstChild()->ToText()->Value()<<"\n"; 
  
  
  //  XMLNode* lastNode = siblingNode->LastChild(); 
  //cout<<lastNode->FirstChildElement("Temperature")->FirstChild()->ToText()->Value()<<"\n";



  //cout<<siblingNode->FirstChildElement("VoltagePower")->FirstChild()->ToText()->Value()<<"\n";
  
 
 
  
  return 0;
}
