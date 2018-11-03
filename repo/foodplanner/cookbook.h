#ifndef COOKBOOK
#define COOKBOOK

class cookbook
{
  
public:
  cookbook();
  void addRecipe();

private:
 struct node
  {
    recipe *rec;
    node *next;
  };
  node *head;
  
};


#endif
