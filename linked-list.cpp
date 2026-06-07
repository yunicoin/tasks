#include <iostream>

struct ListNode {
    int value;
    ListNode* next;
    ListNode(int val) : value(val), next(nullptr) {}
};

void Process(ListNode* head) 
{
  ListNode* current = head;
  int sum = head->value;
  while (current->next != nullptr) {
    current = current->next;        
    sum += current->value;
   if (sum == 0) {
     head->next=current;
   }
  }
  if (head->next != nullptr)
  {
    Process(head->next);
  }
    
}

