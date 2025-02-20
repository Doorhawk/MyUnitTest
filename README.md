# UnitTest

Эта простая библиотека для написания модульных тестов в C++. Она позволяет проверять ожидаемые исключения, сравнивать значения с заданной точностью, группировать тесты и логировать их результаты.

## Установка

Скачайте бибиотеку и заголовочный файл, подключите к вашему проэкту

## Использование

### Проверка значений

#### Проверка равенства
EQVAL(a, b) проверяет, что a == b. Если значения не равны, тест будет отмечен проваленным
```cpp
EQVAL(10, 10); // OK
EQVAL(10, 5);  // FAIL
```
#### Проверка равенства с допуском
Для сравнения чисел с плавающей точкой используется EQVAL_EPS(a, b, eps), где eps — допустимая разница:
```cpp
EQVAL_EPS(3.1415, 3.1416, 0.001); // OK
EQVAL_EPS(3.1415, 3.14, 0.001);   // FAIL
```
#### Проверка логического условия
CHECK(obj) проверяет, что объект obj не является ложным:
```cpp
CHECK(1);   // OK
CHECK(0);   // FAIL
```
#### Проверка исключений
EXPECT_THROW(func,type) - проверяет выдаст ли функция func исключение с типом type,
EXPECT_THROW_MSG(func,type,msg) - дополнительно проверят сообшение исключения:
```cpp
void fun(){
    throw std::runtime_error("error");
}
EXPECT_THROW(fun(), std::runtime_error);    // OK
EXPECT_THROW(fun(), std::invalid_argument); // FAIL

EXPECT_THROW_MSG(fun(),std::runtime_error,"error");     // OK
EXPECT_THROW_MSG(fun(),std::invalid_argument,"error");  // FAIL
EXPECT_THROW_MSG(fun(),std::runtime_error,"hellow");    // FAIL
```
### Определение тестов

#### Обычный тест
Для создания теста используется макрос TEST(name), name - имя теста должно быть уникальным:
```cpp
TEST(MyTest) {
    int a = 5, b = 5;
    EQVAL(a, b);  // Проверка на равенство
}
```
#### Тесты с параметрами
Для тестов с параметрами используются макросы TEST_PARAM_CASE(name,...) - для определения и TEST_PARAM(name,...) - для выполнеия, где ... - набо параметров.
При каждом вызове TEST_PARAM в риестр добавляетя новый тест.
```cpp
TEST_PARAM_CASE(TestSum, int a, int b, int expected) {
    EQVAL(a + b, expected);
}
TEST_PARAM(TestSum, 2, 3, 5); // OK
TEST_PARAM(TestSum, 10, 5, 16); // FAIL
```
Этот код анологичен:
```cpp
TEST(TestSum1) {
    EQVAL(2 + 3, 5);
}
TEST(TestSum2) {
    EQVAL(10 + 5, 16);
}
```
#### Тесты с набором данных
Для создания теста с набором даных используется TEST_DATA(name, (std::vector<T>{...}), T val).
Создается один тест с набором проверок внутри.
```cpp
TEST_DATA(TestData, (std::vector<int>{1, 2, 3, 4}), int val) {
	EQVAL(val, 2);
}
```
Этот код анологичен:
```cpp
TEST(TestData) {
	EQVAL(1, 2);
    EQVAL(2, 2);
    EQVAL(3, 2);
    EQVAL(4, 2);
}
```
### Группы тестов
Для создания группы тестов используется функци с приставкой _GROUP, первым аргументом функций идет названее группы.
(создание теста с параметром для групп выполняется той же функцией TEST_PARAM_CASE)
```cpp
TEST_GROUP(GroupName,TestName);
TEST_PARAM_GROUP(GroupName,TestName,...);
TEST_DATA_GROUP(GroupName,TestName,...);
```
Пример:
```cpp
TEST_GROUP(MathTests, TestAddition) {
    EQVAL(2 + 2, 4);
}
TEST_GROUP(MathTests, TestSubtraction) {
    EQVAL(5 - 3, 2);
}
```
### Запуск тестов
Для запуска тестов используется две функции:
```cpp
RUN_ALL_TESTS();             // - запускает все тесты
RUN_TESTS_GROUP(groupName);  // - запускает все тесты из группы 
```
Пример:
```cpp
#include "unittest.h"
TEST(checktest){
    int a = 10, b = 20;
    CHECK(a==b);
}
void fun(){
    throw std::runtime_error("error");
}
TEST_GROUP(firstGroup,expTest){
    EXPECT_THROW(fun(), std::runtime_error);
}
TEST_PARAM_CASE(paramTest, int a, int b) {
	EQVAL(a, b);
}
TEST_PARAM_GROUP(firstGroup,paramTest, 1, 1);
TEST_PARAM_GROUP(firstGroup,paramTest, 1, 2);
TEST_DATA_GROUP(secondGroup,dataTest, (std::vector<int>{1, 2, 3, 4}), int val) {
	EQVAL(val, 2);
}
int main(){
    RUN_ALL_TESTS();        
    RUN_TESTS_GROUP(firstGroup);
    RUN_TESTS_GROUP(secondGroup);
}
```
Вывод в консоль:
```console
        [All]:
[FAIL]  checktest, Error: !a == b
[OK]    expTest
[OK]    paramTest( 1, 1 )
[FAIL]  paramTest( 1, 2 ), Error: a != b, expected: 1, got 2
[FAIL]  dataTest, Error: val != 2, expected: 1, got 2
        complited: 2/5

        [firstGroup] group:
[OK]    expTest
[OK]    paramTest( 1, 1 )
[FAIL]  paramTest( 1, 2 ), Error: a != b, expected: 1, got 2
        [firstGroup] complited: 2/3

        [secondGroup] group:
[FAIL]  dataTest, Error: val != 2, expected: 1, got 2
        [secondGroup] complited: 0/1
```
### Подробности тестирования
TEST_LOG_ON() - выключает вывод подробнестей о каждой проверке внутри теста, (TEST_LOG_OFF() - выключает)
При выключенном состоянии:
```cpp
#include "unittest.h"
TEST(test) {
    CHECK(1 == 1);
    CHECK(2 == 2);
    CHECK(3 == 3);
    CHECK(4 == 10);
    CHECK(1 == 1);
}
int main() {
    TEST_LOG_OFF();
    RUN_ALL_TESTS();
}
```
Вывод:
```console
        [All]:
[FAIL]  test, Error: !4 == 10
        complited: 0/1
```
При включенном:
```cpp
#include "unittest.h"
...
int main() {
    TEST_LOG_ON();
    RUN_ALL_TESTS();
}
```
Вывод:
```console
        [All]:
[FAIL]  test, Error: !4 == 10
                OK 1 == 1
                OK 2 == 2
                OK 3 == 3
                FAIL !4 == 10
        complited: 0/1
```
### Прерывание теста
Если одна из функций проверки выдаст ложный результат тест завершится и всё последующие проверки в этом тесте выполняться не будут. 
Чтобы при ложной проверке тест не завершался используются функции с добавкой _UNSTOP:
```cpp
EQVAL_UNSTOP();
EQVAL_EPS_UNSTOP();
CHECK_UNSTOP();
EXPECT_THROW_UNSTOP(); 
EXPECT_THROW_MSG_UNSTOP();
```
Пример с обычной проверкой:
```cpp
#include "unittest.h"
TEST(test) {
    CHECK(1 == 1);
    CHECK(2 != 2);
    CHECK(3 == 3);
    CHECK(4 != 4);
    CHECK(5 == 5);
}
int main() {
    TEST_LOG_ON();
    RUN_ALL_TESTS();
}
```
Вывод:
```console
        [All]:
[FAIL]  test, Error: !(2 != 2)
                OK 1 == 1
                FAIL !(2 != 2)
        complited: 0/1
```
Пример с UNSTOP проверкой:
```cpp
#include "unittest.h"
TEST(test) {
    CHECK_UNSTOP(1 == 1);
    CHECK_UNSTOP(2 != 2);
    CHECK_UNSTOP(3 == 3);
    CHECK_UNSTOP(4 != 4);
    CHECK_UNSTOP(5 == 5);
}
int main() {
    TEST_LOG_ON();
    RUN_ALL_TESTS();
}
```
Вывод:
```console
        [All]:
[FAIL]  test, Error: check logs
                OK 1 == 1
                FAIL !(2 != 2)
                OK 3 == 3
                FAIL !(4 != 4)
                OK 5 == 5
        complited: 0/1
```