#include "cookbook.cc"
#include <iostream>

using namespace std;

int main()
{

  cout<<"main started\n";
  cookbook *myCookBook = new cookbook();
  myCookBook->addRecipe();
  myCookBook->addRecipe();
  return 0;
}
