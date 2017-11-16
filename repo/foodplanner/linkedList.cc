#include "linkedList.h"
#include <iostream>

using namespace std;

linkedList::linkedList()
{
  head = NULL;    //Set head to null
}

/*template<typename T>
void linkedList::addItem(T itemToStore)
{
  /*node *dataNode = new node();    //Create a new recipe
    dataNode->data = itemToStore;  //Set the new recipe 
    head = dataNode;               //Move the head to the new recipe
    printf("%p data\n", head);*/
}
