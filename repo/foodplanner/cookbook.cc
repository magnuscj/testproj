#include <iostream>
#include "recipe.cc"
#include "cookbook.h"

using namespace std;


  //Constructor
  cookbook::cookbook()
  {
    head = NULL;    //Set head to null
    cout<<"cookbook\n";
  }

  //Add a new recipe
  void cookbook::addRecipe()
  {
    node *recipeNode= new node();  //Create a new recipe
    recipeNode->rec = new recipe();//Set the new recipe 
    head = recipeNode;             //Move the head to the new recipe
    printf("%p\n", head);
  }
  
