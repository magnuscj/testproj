#include <iostream>
#include "recipe.cc"

using namespace std;


class cookbook
{
  


public:

  //Constructor
  cookbook()
  {
    head = NULL;    //Set head to null
    cout<<"cookbook\n";
  }

  //Add a new recipe
  void addRecipe()
  {
    recipeNode = new node(); //Create a new recipe
    recipeNode->rec = new recipe();//Set the new recipe 
    head = recipeNode;         //Move the head to the new recipe
  }
  
private:
 
  struct node
  {
    recipe *rec;
    node *next;
  };
  node *head;
  node *recipeNode;
};

