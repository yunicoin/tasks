/*
 * Программа считывает выражение со скобками, разбивает его на лексемы,
 * преобразовывает лексемы в польскую форму записи и вычисляет значение
 * выражения, используя полученную польскую форму.
 *
 * Выражение в польской форме записи удобно для вычислений на компьютере.
 * В таком выражении операции пишутся после чисел, все операции выполняются
 * слева направо, приоритетов выполнения операций нет. Кроме того,
 * отсутствуют скобки.
 *
 * Например, в польской форме 1 2 3 * + означает 1 + (2 * 3).
 * В польской форме 1 2 + 3 * означает (1 + 2) * 3.
 * В польской форме 1 2 3 + * также означает (1 + 2) * 3.
 *
 * То есть, каждая операция извлекает два числа слева от неё и вместо них кладёт
 * обратно результат операции. Операции выполняются по порядку слева направо.
 */
#include <iostream>
#include <cmath>

#define MAX_LEXEMES 100

/* Лексема является операцией, если поле op равно '+', '-', '*' или '/',
 * 'u' (унарный минус), '(' или ')'. Лексема является числом, которое будет
 * храниться в поле value, если поле op равно 0. Операциям назначены целочисленные
 * приоритеты. Операция с высоким приоритетом должна выполняться раньше операции
 * с низким приоритетом. */
struct Lexeme {
  char op;
  double value;
  int priority;
};

/* Функция предназначена для вывода массива лексем на экран. */
void PrintLexemes(Lexeme* lexemes, int numLexemes) {
  for (int k = 0; k < numLexemes; k++) {
    if (lexemes[k].op != 0) {
      std::cout << "Lexeme ('" << lexemes[k].op << "', "
          << lexemes[k].value << ", " << lexemes[k].priority << ")" << std::endl;
    } else {
      std::cout << "Lexeme ('value', "
          << lexemes[k].value << ", " << lexemes[k].priority << ")" << std::endl;
    }
  }
}

int ScanLexemes(Lexeme* lexemes) {
  char str[1024];
  char* ptr;
  int numLexemes = 0;

  /* Считываем выражение. */
  if (!std::cin.getline(str, 1024)) {
    std::cerr << "Can't read expression!" << std::endl;
    return -1;
  }

  /* Разбираем выражение посимвольно. Создаём массив лексем. */
  for (ptr = str; *ptr != 0; ) {
    /* Если текущий символ --- цифра или точка, запускаем процедуру чтения числа. */
    if (isdigit(*ptr) || *ptr == '.') {
      char* endPtr;

      if (numLexemes >= MAX_LEXEMES) {
        std::cerr << "Can't read new lexeme. Not enough space!" << std::endl;
        return -1;
      }

      lexemes[numLexemes].value = strtod(ptr, &endPtr);
      lexemes[numLexemes].op = 0;
      lexemes[numLexemes].priority = 0;

      /* endPtr --- позиция, на которой считывание числа завершилось. */
      if (endPtr == ptr) {
        /* Не считали ни одного символа. */
        std::cerr << "Can't read value from '" << ptr << "'!" << std::endl;
        return -1;
      }

      numLexemes++;

      /* Перескакиваем через считанное число. */
      ptr = endPtr;
    } else if (*ptr == '+' || *ptr == '-' || *ptr == '*' || *ptr == '/' ||
        *ptr == '(' || *ptr == ')') {
      /* Обрабатываем операцию. */
      if (numLexemes >= MAX_LEXEMES) {
        std::cerr << "Can't read new lexeme. Not enough space!" << std::endl;
        return -1;
      }

      lexemes[numLexemes].value = 0;

      if (*ptr == '-' && (numLexemes == 0 ||
          (numLexemes > 0 && lexemes[numLexemes - 1].op != 0))) {
        /* Это унарный минус. Он может стоять либо в начале выражения,
         * либо после открывающейся скобки. Приоритет унарного минуса
         * выше приоритета умножения или деления. */
        lexemes[numLexemes].op = 'u';
        lexemes[numLexemes].priority = 3;
      } else if (*ptr == '*' || *ptr == '/') {
        lexemes[numLexemes].op = *ptr;
        lexemes[numLexemes].priority = 2;
      } else if (*ptr == '+' || *ptr == '-') {
        lexemes[numLexemes].op = *ptr;
        lexemes[numLexemes].priority = 1;
      } else if (*ptr == '(') {
        /* У открывающейся скобки самый высокий приоритет. Любая операция,
         * расположенная перед ней, должна дождаться выполнения операций
         * в скобках. */
        lexemes[numLexemes].op = *ptr;
        lexemes[numLexemes].priority = 4;
      } else if (*ptr == ')') {
        /* У закрывающейся скобки самый низкий приоритет. Скобки схлопываются
         * только после того как выполнятся все операции внутри них. */
        lexemes[numLexemes].op = *ptr;
        lexemes[numLexemes].priority = 0;
      } else {
        std::cerr << "Something wrong has happened!" << std::endl;
        return -1;
      }

      numLexemes++;

      ptr++;
    } else if (std::isspace(*ptr)) {
      /* Пропускаем пробельный символ. */
      ptr++;
    }
  }

  /* Добавим фиктивную операцию '=' с самым низким приоритетом, чтобы
   * перед ней схлопывался стек операций. */
  lexemes[numLexemes].op = '=';
  lexemes[numLexemes].value = 0;
  lexemes[numLexemes].priority = 0;

  numLexemes++;

  return numLexemes;
}

/* Функция принимает выражение, разбитое на лексемы и записывает это выражение
 * в польской форме записи. Функция возвращает количество лексем в польской форме. */
int ToPolandForm(Lexeme* lexemes, int numLexemes, Lexeme* polandLexemes) {
  /* В этот стек будем складывать операции. */
  Lexeme stack[MAX_LEXEMES];
  int stackSize = 0;
  int numPolandLexemes = 0;

  for (int k = 0; k < numLexemes; k++) {
    /* Число добавляем сразу в польскую запись, а операцию кладём в стек. */
    if (lexemes[k].op == 0)
      polandLexemes[numPolandLexemes++] = lexemes[k];
    else {
      stack[stackSize++] = lexemes[k];
    }

    /* В цикле перемещаем операции из стека в польскую запись в соответствии
     * с приоритетом операций. */
    bool working = true;

    while (stackSize >= 2 && working) {
      working = false;

      /* Скобки обрабатываем отдельно. Если открывающаяся и закрывающаяся скобки
       * идут подряд, то просто убираем их из стека. */
      if (stack[stackSize - 2].op == '(' && stack[stackSize - 1].op == ')') {
        stackSize -= 2;
        working = true;
      } else if (stack[stackSize - 2].op != '(' &&
          stack[stackSize - 2].priority >= stack[stackSize - 1].priority) {
        /* Если за операцией с высоким приоритетом следует операция
         * с меньшим или таким же приоритетом, то следует выполнить первую из них.
         * Перемещаем её из стека в польскую запись. */
        polandLexemes[numPolandLexemes++] = stack[stackSize - 2];
        stack[stackSize - 2] = stack[stackSize - 1];
        stackSize--;
        working = true;
      }
    }

  }

//  std::cout << "Stack:" << std::endl;
//  PrintLexemes(stack, stackSize);

  /* В стеке должна остаться только одна операция '='. */
  if (stackSize != 1) {
    std::cerr << "Incorrect expression!" << std::endl;
    return -1;
  }

  if (stack[stackSize - 1].op != '=') {
    std::cerr << "Incorrect expression!" << std::endl;
    return -1;
  }

  /* На самом деле, функция не проверяет все возможные ошибки. Например,
   * нет проверки того, что у бинарных операторов есть два аргумента,
   * а у унарного оператора ровно один аргумент.
   *
   * Таким образом, некоторые некорректные выражения будут преобразовываться в
   * польскую форму, например, выражение 1*2-, однако в этих случаях
   * будет выдаваться ошибка на этапе вычисления значения выражения в польской
   * форме. */

  return numPolandLexemes;
}

/* Функция вычисляет выражение, записанное в польской форме записи. */
double ComputePolandExpression(Lexeme* expression, int N) {
  double stack[MAX_LEXEMES];
  int stackSize = 0;

  if (N <= 0)
    return 0;

  for (int k = 0; k < N; k++) {

    if (expression[k].op == 0) {
      /* Кладём новую лексему в стек. */

      stack[stackSize++] = expression[k].value;
    } else if (expression[k].op == '+' && stackSize >= 2) {
      /* Операция сложения. Вытаскиваем два элемента из стека,
       * складываем их и результат кладём обратно в стек. */
      stack[stackSize - 2] = stack[stackSize - 2] + stack[stackSize - 1];
      stackSize--;
    } else if (expression[k].op == '-' && stackSize >= 2) {
      /* Вычитание. */
      stack[stackSize - 2] = stack[stackSize - 2] - stack[stackSize - 1];
      stackSize--;
    } else if (expression[k].op == '*' && stackSize >= 2) {
      /* Умножение. */
      stack[stackSize - 2] = stack[stackSize - 2] * stack[stackSize - 1];
      stackSize--;
    } else if (expression[k].op == '/' && stackSize >= 2) {
      /* Деление. */
      stack[stackSize - 2] = stack[stackSize - 2] / stack[stackSize - 1];
      stackSize--;
    } else if (expression[k].op == 'u' && stackSize >= 1) {
      /* Это операция унарный минус. Она принимает один аргумент. */
      stack[stackSize - 1] = -stack[stackSize - 1];
    } else {
      /* NAN это Not a Number. Специальное значение, которое можно использовать
       * для того, чтобы вернуть ошибку. */
      return NAN;
    }
  }

  /* После вычисления операций в стеке должно остаться ровно одно число ---
   * результат выражения. В противном случае выражение некорректно. */
  if (stackSize == 1)
    return stack[0];

  return NAN;
}

int main() {
  Lexeme lexemes[MAX_LEXEMES] = { 0 };
  int numLexemes = 0;
  Lexeme polandLexemes[MAX_LEXEMES] = { 0 };
  int numPolandLexemes = 0;

  numLexemes = ScanLexemes(lexemes);

  if (numLexemes <= 0) {
    std::cerr << "Read " << numLexemes << " lexemes!" << std::endl;
    return 1;
  }

  std::cout << "Lexemes:" << std::endl;
  PrintLexemes(lexemes, numLexemes);

  numPolandLexemes = ToPolandForm(lexemes, numLexemes, polandLexemes);

  if (numPolandLexemes <= 0) {
    std::cerr << "Read " << numPolandLexemes << " poland lexemes!" << std::endl;
    return 1;
  }

  std::cout << "Poland form:" << std::endl;
  PrintLexemes(polandLexemes, numPolandLexemes);

  double result = ComputePolandExpression(polandLexemes, numPolandLexemes);

  std::cout << "Result: " << result << std::endl;

}



