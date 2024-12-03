#pragma once
#include <string_view>
#include <map>
#include "../table/table.hpp"
#include "../parser/parser.hpp"
#include <stdexcept>

/*
доп ограничения/ доп возможности:
1. Join надо обернуть в скобки
2. Если есть join то писать обязательно on
3. Update c join не будет ?
4. в condition можно делать строить логические последовательности так, чтобы колонки не зависили друг от друга - можно исправить изначально меняя inds в 
    соотвествии с такими сложными условиями
5. названия таблиц и колоннок не начинаются с цифр
6. -1 и подобные числа с однооперандным -/+ не парсятся
7. символы названий таблиц/колонок: isdigit(ch) || isalpha(ch) || ch == '.' || ch == '"' || ch == '_' || ch == '#' || ch == '@'
8. |"asdsa" + "adas" + "a"| разрешено
9. решил не забирать возможность - condition (where) можно с и составить так, что если колонки никогда не будут нужны, то туда можно писать некорректные
9.1 чтобы убрать - добавить if в computecondition
10. колонкам разрешено меняться в зависимости от других колонок этой же таблички и изм будут как 1 тразакция (a = "1" b = "2" a = a+b, b =b+a -> a=12 b = 21)
что можно легко добавить:
11. разрешено писать select col1 from table1 where ... вместо select table1.col1 from table1 where если табличка всего одна
12. не очень понял почему в тестовых запросов опускали запятую в конце хотя в усл сказанно,что если не пишут поле, то хотя бы запятую оставляют - добавил
1. разрешить использовать оба типа при insert одновременно

TODO rm + op from bytes
TODO check || for str bytes
TODO autoincrement - check that type ios int
TODO add tolower
TODO autoincrement - check that type ios int
add work with attrs
how not opperation will work? - test
add work with attrs
how not opperation will work? - test
*/

namespace memdb {
class Database {
public:
    Database()=default;

    void CreateTable(const std::string& tName, const std::vector<ColumnFullDescription>& colDescripts);

    void Insert(const std::string& tName, const std::vector<Table::Value>& values);

    void Update(const std::string& tName, parser::Assignments& dataToUpdate, parser::Condition& condition);

    std::optional<TableView> Select(std::vector<TableColumn>& tcs, parser::Condition& condWh, std::string& tableName); // if want to select from multiply tables - add code here not in tables - just merge them

    void Delete(std::string& tName, parser::Condition& condWh); // if want to select from multiply tables - add code here not in tables - just merge them
    std::optional<TableView> Execute(const std::string_view& request);

private:
    std::vector<Table> tables;
    std::map<std::string, ssize_t> tName2ind;
};

}