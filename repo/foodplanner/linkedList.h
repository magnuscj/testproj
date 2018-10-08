#ifndef LINKEDLIST_H
#define LINKEDLIST_H


class linkedList
{
 public:
  linkedList();
  //template<typename T>
  //void addItem(T t);
 private:
  struct node
  {
    int *data;
    node *next;
  };
  node *head;
};

#endif
