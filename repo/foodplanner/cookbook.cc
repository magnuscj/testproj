#include <iostream>
#include "recipe.cc"

using namespace std;

class cookbook
{


public:
  //Constructor
  cookbook()
  {
    cout<<"cookbook\n";
    myRecipe = new recipe();
  }

private:
  recipe *myRecipe;
  
};

