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
#include "edsServerHandler.h"
#include <thread>
#include <chrono>


using namespace std;
using namespace tinyxml2;

int main(int argc, char* argv[])
{
  auto start = std::chrono::system_clock::now();
  for(int i = 1; i<argc;i++)
  {
    edsServerHandler eds(argv[i]);
    eds.connectToEdsServer();
    eds.readEdsServerData();
    eds.decodeServerData();
    eds.storeServerData();
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "finished computation at " << std::ctime(&end_time)
		                << "elapsed time: " << elapsed_seconds.count() << "s\n";

  auto start2 = std::chrono::system_clock::now();

    edsServerHandler eds2(argv[1]);
    edsServerHandler eds3(argv[2]);

    eds2.connectToEdsServer();
    eds3.connectToEdsServer();


    std::thread A(&edsServerHandler::readEdsServerData,&eds2);
    std::thread B(&edsServerHandler::readEdsServerData,&eds3);

    A.join();
    B.join();

    eds2.decodeServerData();
    eds3.decodeServerData();

    eds2.storeServerData();
    eds3.storeServerData();
  auto end2 = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds2 = end2-start2;
  std::time_t end_time2 = std::chrono::system_clock::to_time_t(end2);
  std::cout << "finished computation at " << std::ctime(&end_time2)
		                << "elapsed time: " << elapsed_seconds2.count() << "s\n";


  return 0;
}
