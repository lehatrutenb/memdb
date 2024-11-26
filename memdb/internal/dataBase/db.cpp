#pragma once
#include <string_view>
#include "../table/table.cpp"

/*
доп ограничения:
1. Join надо обернуть в скобки
2. Если есть join то писать обязательно on
3. Update c join не будет ?
4. в condition можно делать строить логические последовательности так, чтобы колонки не зависили друг от друга - можно исправить изначально меняя inds в 
    соотвествии с такими сложными условиями
5. названия таблиц и колоннок не начинаются с цифр
6. -1 и подобные числа с однооперандным -/+ не парсятся
7. символы названий таблиц/колонок: isdigit(ch) || isalpha(ch) || ch == '.' || ch == '"' || ch == '_' || ch == '#' || ch == '@'
что можно легко добавить:
1. разрешить использовать оба типа при insert одновременно
*/
namespace memdb {
class Database {
public:
    Database()=default;

    void CreateTable(const std::string_view& tName, const std::vector<ColumnType>& colTps_, const std::vector<ColumnDescription>& colDescripts) {
        colTps = colTps_;
        // throw ex i f colDescripts and colTps not same size
        if (tName2ind.find(tName) != tName2ind.end()) {
            // throw ex - table names should be unique
        }
        tables.emplace_back(Table{});
        for (int i = 0; i < colDescripts.size(); i++) {
            tables.back().AddColumn(colTps[i], colDescripts[i]);
        }
    }

    void Insert(const std::string_view& tName, const std::vector<Table::Value>& values) {
        if (tName2ind.find(tName) == tName2ind.end()) {
            // throw ex
        }
        tables[tName2ind[tName]].Insert(values);
    }

    void Execute(const std::string_view& request) { // not void

    }

private:
    std::vector<Table> tables;
    std::map<std::string_view, ssize_t> tName2ind;
    std::vector<ColumnType> colTps;
};

/*
enum class Type {
    Int32,
    Bool,
    Byte,
    String,
};

class ColumnType {
public:
    Type t;
    int maxLength;
};

enum class ColumnAttrs {
    Default = 0,
    Unique = 1,
    Autoincrement = 2,
    Key = 4,
};

*/
//const ColumnType& colTp, const std::vector<ColumnAttrs>& tps, const std::string_view& name, const std::shared_ptr<DbType> defVal=std::make_shared<DbType>(nullptr)
//ColumnInt32(std::vector<ColumnAttrs> tps, std::string_view name_, std::shared_ptr<DbType> defVal_) : Column(tps, name_) {
}

using namespace memdb;

int main() {
    Database db;
    const std::string_view tName = "table1";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    const std::string_view tName2 = "table2";
    std::vector<ColumnType> colTps2 = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts2 = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName2, colTps2, colDescripts);

    std::shared_ptr<DbType> value;
    value.reset(dynamic_cast<DbType*>(new DbInt32(123)));
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("table1", to_add);

    std::shared_ptr<DbType> value2;
    value.reset(dynamic_cast<DbType*>(new DbInt32(321)));
    std::vector<Table::Value> to_add2 = {Table::Value(value2)};
    db.Insert("table2", to_add);
}