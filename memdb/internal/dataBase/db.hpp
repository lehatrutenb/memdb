#pragma once
#include <string_view>
#include <map>
#include "../table/table.hpp"
#include "../parser/parser.hpp"
#include <stdexcept>

namespace memdb {
class Database {
public:
    Database()=default;

    void CreateTable(const std::string& tName, const std::vector<ColumnFullDescription>& colDescripts);

    void Insert(const std::string& tName, const std::vector<Table::Value>& values);

    void Update(const std::string& tName, parser::Assignments& dataToUpdate, parser::Condition& condition);

    std::optional<TableView> Select(std::vector<TableColumn>& tcs, parser::Condition& condWh, std::string& tableName);

    void Delete(std::string& tName, parser::Condition& condWh);
    std::optional<TableView> Execute(const std::string_view& request);

private:
    std::vector<Table> tables;
    std::map<std::string, ssize_t> tName2ind;
};

}