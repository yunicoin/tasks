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
     *head=*current->next;
   }
  }
  if (head->next != nullptr)
  {
    Process(head->next);
  }
    
}

int main() {
	ListNode* head = new ListNode(1);
	head->next = new ListNode(-1);
	head->next->next = new ListNode(2);
	head->next->next->next = new ListNode(-2);
	head->next->next->next->next = new ListNode(3);
	Process(head);
	// Выводим результат
	ListNode* current = head;
	while (current != nullptr) {
		std::cout << current->value << " ";
		current = current->next;
	}
	std::cout << std::endl;
	// Освобождаем память
	current = head;
	while (current != nullptr) {
		ListNode* temp = current;
		current = current->next;
		delete temp;
	}
	return 0;
}