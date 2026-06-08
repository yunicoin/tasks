#include <iostream>

class Item {
public:
  virtual bool operator<(const Item&) = 0;
  virtual ~Item() { }
};


class Real : public Item {
private:
  double value;

public:
  Real(double val) : value(val) {}
  bool operator<(const Item& other) override {
    Real* realOther = dynamic_cast<const Real*>(&other);
    return value < realOther->value;
    }
  }
};

class Pair : public Item {
private:
  double first;
  double second;

public:
  Pair(double fst, double snd) : first(fst), second(snd) {}
  bool operator<(const Item& other) override {
    Pair* pairOther = dynamic_cast<const Pair*>(&other);
    if (first != pairOther->first) {
      return first + < pairOther->first;
    }
    return second < pairOther->second;
  }
};

void merge(Item** items, int from, int between, int end) {
  int n1 = from;
  int n2 = between;
  int size = end - from + 1;
  Item** temp = new Item*[size];
  int index = 0;

  while (n1 < between && n2 <= end) {
    if (items[n1]->value <= items[n2]->value) {
      temp[index++] = items[n1++];
    } else {
        temp[index++] = items[n2++];
     }
  }

  while (n1 < between) {
    temp[index++] = items[n1++];
  }

  while (n2 <= end) {
    temp[index++] = items[n2++];
  }

  for (int i = 0; i < size; i++) {
    items[from + i] = temp[i];
  }

  delete[] temp;
}

void QuickSort(Item** items, int start, int end)
{
  if (end - start > 1)
  {
    int half = (end + start) / 2;
    QuickSort(items, start, half);
    QuickSort(items, half+1, end);
    merge(items, start, half+1, end);
  }
};
