#include <iostream>
#include <string.h>
#include <time.h>
#include "edsServerHandler.h"
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>
#include <vector>
#include <unistd.h>
using namespace std;
//using namespace tinyxml2;

void test(char* ipadr )
{
    edsServerHandler eds(ipadr);
    eds.connectToEdsServer();
    eds.readEdsServerData();
    eds.decodeServerData();
    eds.storeServerData();
}



int main(int argc, char* argv[])
{
  std::vector<std::thread> tve;
  std::vector<double> elapsedTime;

    int noOfBins = 10;
    int max = 5;
    int min = 0;
    std::vector<double> bins(noOfBins,0.0);


  while(1)//for(int j=0;j<30;j++)
  {
    std::cout << "\x1B[2J\x1B[H";
    auto start3 = std::chrono::system_clock::now();
    
    for(int i = 1;i < argc;i++)
    {
      tve.emplace_back(test, argv[i]);
    }
    for(auto& t : tve)
    {
      t.join();
    }
   tve.clear();  
    auto end3 = std::chrono::system_clock::now();
   
    std::chrono::duration<double> elapsed_seconds3 = end3-start3;
    std::time_t end_time3 = std::chrono::system_clock::to_time_t(end3);
    std::cout<< "Elapsed time: " << elapsed_seconds3.count() << "s\n";
  //  elapsedTime.push_back(elapsed_seconds3.count());
   // std::sort(elapsedTime.begin(), elapsedTime.end());
  //  for(auto elapsed : elapsedTime)
 // 	cout<<elapsed<<"s\n";
    sleep(60); 
  //
  //  double  cnt =  max/noOfBins;
  //  std::vector<double>::iterator it = bins.begin();
  //  for(int i =0;i<noOfBins;i++)
  //  {
  //      if( elapsed_seconds3.count() < cnt)
  //       TODO! 
  //  }  
  //}
  // 
 } 
  
  return 0;
}
