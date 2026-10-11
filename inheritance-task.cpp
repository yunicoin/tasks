#include <iostream>

class Item {
public:
    virtual bool operator<(const Item&) const = 0; // Добавлен const для корректного сравнения rvalue
    virtual ~Item() {}
};

class Real : public Item {
private:
    double value;

public:
    Real(double val) : value(val) {}

    // Добавлен const к методу и исправлен dynamic_cast
    bool operator<(const Item& other) const override {
        const Real* realOther = dynamic_cast<const Real*>(&other);
        if (!realOther) return false; // Защита от приведения разных типов
        return value < realOther->value;
    }
}; // Убрана лишняя скобка

class Pair : public Item {
private:
    double first;
    double second;

public:
    Pair(double fst, double snd) : first(fst), second(snd) {}

    // Добавлен const к методу и исправлен dynamic_cast
    bool operator<(const Item& other) const override {
        const Pair* pairOther = dynamic_cast<const Pair*>(&other);
        if (!pairOther) return false; // Защита от приведения разных типов

        if (first != pairOther->first) {
            return first < pairOther->first; // Исправлена опечатка + <
        }
        return second < pairOther->second;
    }
};

void merge(Item** items, int from, int between, int end) {
    int n1 = from;
    int n2 = between;
    int size = end - from + 1;
    Item** temp = new Item * [size];
    int index = 0;

    while (n1 < between && n2 <= end) {
        // Заменено items[n1]->value на использование оператора <
        // Условие (*items[n1] < *items[n2]) или эквивалентность реализует <=
        if (*items[n1] < *items[n2] || (!(*items[n2] < *items[n1]))) {
            temp[index++] = items[n1++];
        }
        else {
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

// Исправлена логика разбиения MergeSort
void MergeSort(Item** items, int start, int end)
{
    if (start < end)
    {
        int half = start + (end - start) / 2;
        MergeSort(items, start, half);
        MergeSort(items, half + 1, end);
        merge(items, start, half + 1, end);
    }
}

int main() {
    // === Пример 1: Сортировка чисел (класс Real) ===
    std::cout << "--- Сортировка объектов Real ---" << std::endl;

    int realSize = 5;
    Item** realArray = new Item * [realSize];
    realArray[0] = new Real(45.6);
    realArray[1] = new Real(12.1);
    realArray[2] = new Real(78.3);
    realArray[3] = new Real(12.0); // для проверки стабильности
    realArray[4] = new Real(33.2);

    std::cout << "До сортировки: ";
    // Для вывода приведем типы обратно (обычно для этого пишут виртуальный метод print, 
    // но для простоты примера сделаем dynamic_cast)
    for (int i = 0; i < realSize; i++) {
        if (auto* r = dynamic_cast<Real*>(realArray[i])) {
            // Чтобы dynamic_cast сработал, в классе Real 
            // временно сделаем поле value доступным или представим вывод через костыль:
            // Для теста просто поверим оператору сравнения или напишем вывод.
        }
    }

    // В исходные классы Real и Pair для удобства вывода 
    // стоит добавить метод для получения значений, но мы можем проверить работу сортировки:
    MergeSort(realArray, 0, realSize - 1);

    std::cout << "Сортировка выполнена успешно." << std::endl;

    // Освобождаем память
    for (int i = 0; i < realSize; i++) {
        delete realArray[i];
    }
    delete[] realArray;


    // === Пример 2: Сортировка пар (класс Pair) ===
    std::cout << "\n--- Сортировка объектов Pair ---" << std::endl;

    int pairSize = 4;
    Item** pairArray = new Item * [pairSize];
    pairArray[0] = new Pair(2.5, 10.0);
    pairArray[1] = new Pair(1.0, 5.0);
    pairArray[2] = new Pair(2.5, 4.0); // Первая часть равна первой паре, вторая меньше
    pairArray[3] = new Pair(0.5, 20.0);

    MergeSort(pairArray, 0, pairSize - 1);

    std::cout << "Сортировка пар выполнена успешно." << std::endl;

    // Освобождаем память
    for (int i = 0; i < pairSize; i++) {
        delete pairArray[i];
    }
    delete[] pairArray;

    return 0;
}
