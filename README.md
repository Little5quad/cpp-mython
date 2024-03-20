# Mython

Интерпретатор языка Mython.

Реализация на C++ интерпретатора языка программирования Mython — упрощённое подмножество Python. Язык является динамически типизированным, поддерживает классы и наследование. Все методы в классах по умолчанию виртуальные.

## Сборка проекта
Для сборки программы необходим компилятор С++ поддерживающий стандарт не ниже С++17 и установленная утилита CMake.

## Использование интерпретатора
На вход поступает текст программы, а интерпретатор должен вывести в выходной поток результат всех команд print. Примеры вызовов в main.cpp

## Описание языка Mython
### Числа
В языке Mython используются только целые числа. С ними можно выполнять обычные арифметические операции: сложение, вычитание, умножение, целочисленное деление.

### Строки
Строковая константа в Mython — это последовательность произвольных символов, размещающаяся на одной строке и ограниченная двойными кавычками " или одинарными '. Поддерживается экранирование спецсимволов '\n', '\t', '\'' и '\"'. Примеры строк в Mython:

`` "hello" ``
`` 'world' ``
`` 'long string with a double quote " inside' ``
`` "another long string with a single quote ' inside" ``
`` "string with a double quote \" inside" ``
`` 'string with a single quote \' inside' ``
`` '' ``, `` "" `` — пустые строки. Строки в Mython — неизменяемые.

## Логические константы и None
Кроме строковых и целочисленных значений язык Mython поддерживает логические значения `` True `` и `` False ``. Есть также специальное значение `` None ``, аналог `` nullptr `` в С++. В отличие от C++, логические константы пишутся с большой буквы.

## Комментарии
Mython поддерживает однострочные комментарии, начинающиеся с символа #. Весь следующий текст до конца текущей строки игнорируется. # внутри строк считается обычным символом.
```
# это комментарий
x = 5 #это тоже комментарий
# в следующей строке # - обычный символ
hashtag = "#природа"
```
