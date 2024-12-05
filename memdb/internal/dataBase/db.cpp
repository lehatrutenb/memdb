#pragma once
#include <string_view>
#include <map>
#include "db.hpp"
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
13. допустимые символы названий колонок/таблиц: 1234567890QWERTYUIOPASDFGHJKLZXCVBNMqwwertyuiopasdfghjklzxcvbnm@#$;_
1. разрешить использовать оба типа при insert одновременно

TODO check that user not try to get unknown cols in get
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

void Database::CreateTable(const std::string& tName, const std::vector<ColumnFullDescription>& colDescripts) {
    // throw ex i f colDescripts and colTps not same size
    if (tName2ind.find(tName) != tName2ind.end()) {
        // throw ex - table names should be unique
        throw std::runtime_error("error");
    }
    tName2ind[tName] = tables.size();
    tables.emplace_back(Table{tName});
    //colTps.push_back({});
    //colTps.back().resize(colDescripts.size());

    for (int i = 0; i < colDescripts.size(); i++) {
        tables.back().AddColumn(colDescripts[i]);
        //colTps.back()[i] = colDescripts[i];
    }
}

void Database::Insert(const std::string& tName, const std::vector<Table::Value>& values) {
    if (tName2ind.find(tName) == tName2ind.end()) {
        // throw ex
        throw std::runtime_error("error");
    }
    tables[tName2ind[tName]].Insert(values);
}

void Database::Update(const std::string& tName, parser::Assignments& dataToUpdate, parser::Condition& condition) {
    if (tName2ind.find(tName) == tName2ind.end()) {
        // throw ex
        throw std::runtime_error("error");
    }
    tables[tName2ind[tName]].Update(dataToUpdate, condition);
}

std::optional<TableView> Database::Select(std::vector<TableColumn>& tcs, parser::Condition& condWh, std::string& tableName) { // if want to select from multiply tables - add code here not in tables - just merge them
    if (tcs.empty()) {
        return {};
    }
    if (tableName == "" && tables.size() == 1) { // if single table in db can not to write it
        tableName = tName2ind.begin()->first;
    }
    for (auto& tc: tcs) { // try to push table name
        if (tc.table == "") {
            tc.table = tableName;
        }
    }
    if (tName2ind.find(tableName) == tName2ind.end()) {//)tName2ind.find(tcs[0].table) == tName2ind.end()) {
        // throw ex table not found
        throw std::runtime_error("error");
        exit(-1);
    }

    /*for (auto row: tables[tName2ind[tableName]].Select(tcs, condWh)) {
        std::cout << row.Get<int32_t>("id") << ' ' << row.Get<std::string>("login") << std::endl;
    }*/
    return std::optional<TableView>{tables[tName2ind[tableName]].Select(tcs, condWh)};
}

void Database::Delete(std::string& tName, parser::Condition& condWh) { // if want to select from multiply tables - add code here not in tables - just merge them
    if (tName2ind.find(tName) == tName2ind.end()) {
        // throw ex
        throw std::runtime_error("error");
    }
    tables[tName2ind[tName]].Delete(condWh);
}

std::optional<TableView> Database::Execute(const std::string_view& request) { // not void
    if (request.size() == 0) {
        return {};
    }

    parser::TokenStructure ts = parser::Parser{}.Parse(request, 0, request.size() - 1);
    parser::ParserT resParser = parser::ParserT::UNKNOWN;
    for (auto prsr: parser::Parsers) {
        if (prsr->Check(ts)) {
            resParser = prsr->getType();
            break;
        }
    }

    std::vector<std::shared_ptr<Tokenizer::Token>>& tokens = ts.Tokens;

    std::string tableName;
    parser::ColumnDescriptions columnFullDescription;
    parser::InsertData dataToInsert;
    parser::Assignments dataToUpdate;
    parser::SelectData dataToSelect;
    parser::Condition condition;
    switch (resParser) { // if i get parser type than acces to every token is safe
    case parser::ParserT::CREATE_TABLE:
        tableName = parser::getValue<Tokenizer::StringT, std::string>(tokens[2]);
        columnFullDescription = parser::ColumnParser{}.Parse(tokens, 3, tokens.size() - 1);
        CreateTable(tableName, columnFullDescription.info);
        break;
    case parser::ParserT::INSERT:
        tableName = parser::getValue<Tokenizer::StringT, std::string>(tokens.back());
        dataToInsert = parser::InserValuesParser{}.Parse(tokens, 1, ts.getScmd(lexer::SubCommand::TO) - 1);
        Insert(tableName, dataToInsert.data);
        break;
    case parser::ParserT::SELECT:
        tableName = parser::getValue<Tokenizer::StringT, std::string>(tokens[ts.getScmd(lexer::SubCommand::FROM) + 1]);
        dataToSelect = parser::SelectValuesParser{}.Parse(tokens, 1, ts.getScmd(lexer::SubCommand::FROM) - 1);
        condition = parser::ConditionParser{}.Parse(tokens, ts.getScmd(lexer::SubCommand::WHERE) + 1, tokens.size() - 1);
        return Select(dataToSelect.Data, condition, tableName);
    case parser::ParserT::UPDATE:
        tableName = parser::getValue<Tokenizer::StringT, std::string>(tokens[1]);
        dataToUpdate = parser::AssignmentsParser{}.Parse(tokens, ts.getScmd(lexer::SubCommand::SET) + 1, ts.getScmd(lexer::SubCommand::WHERE) - 1);
        condition = parser::ConditionParser{}.Parse(tokens, ts.getScmd(lexer::SubCommand::WHERE) + 1, tokens.size() - 1);
        Update(tableName, dataToUpdate, condition);
        break;
    case parser::ParserT::DELETE:
        tableName = parser::getValue<Tokenizer::StringT, std::string>(tokens[1]);
        condition = parser::ConditionParser{}.Parse(tokens, ts.getScmd(lexer::SubCommand::WHERE) + 1, tokens.size() - 1);
        Delete(tableName, condition);
        break;
    case parser::ParserT::JOIN:
        break;
    case parser::ParserT::CREATE_INDEX:
        break;
    default:
        // throw ex unknown request
        throw std::runtime_error("error");
        exit(-1);
    }

    return {};
}

/*void Execute(const std::string_view& request) { // not void
    auto res1 = Parser{}.ParseCreate(request, 0, request.size() - 1);
    auto res2 = Parser{}.ParseInsert(request, 0, request.size() - 1);

    if (res1.first) {
        std::vector<ColumnType> v1;
        std::vector<ColumnDescription> v2;
        for (auto el : res1.second.second.info) {
            v1.push_back(el.t);
            v2.push_back(static_cast<ColumnDescription>(el));
        }
        CreateTable(res1.second.first, v1, v2);
    } else {
        Insert(res2.second.first, res2.second.second.data);
    }
}*/

}

#ifdef DEBUG
#define EXPECT_NO_THROW(statement) statement
#define EXPECT_ANY_THROW(statement) statement
#define EXPECT_EQ(statement1, statement2) statement1
using namespace memdb;


int main() {
   Database db;
    std::string_view req1 = "create table users ({autoincrement} id : int32, {autoincrement} id2 : int32, id3 : int32=1, {unique} login: string[32])";
    std::string_view req2 = R"(insert (,,,"name1") to users)";
    std::string_view req3 = R"(insert (id=10,,,login = "name2") to users)";
    std::string_view req4 = R"(insert (10,,11,"name3") to users)";
    std::string_view req5 = R"(insert (,10,,"name4") to users)";
    std::string_view req6 = R"(select id, id3, login from users where id=11)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    EXPECT_NO_THROW(db.Execute(req5));
    TableView res;
    db.Execute(req6);
    EXPECT_NO_THROW(res = db.Execute(req6).value());
    int ind = 0;
    for (auto row : res) {
        ind++;
        EXPECT_EQ(row.Get<int32_t>("id"), 11);
        //EXPECT_ANY_THROW(row.Get<int32_t>("id2"));
        EXPECT_EQ(row.Get<int32_t>("id3"), 1);
        EXPECT_EQ(row.Get<std::string>("login"), "name4");
    }
    EXPECT_EQ(ind, 1);
    return 0;
    /*Database db;
    std::string_view req1 = "create table users ({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)";
    std::string_view req2 = R"(insert (,"vasya", 0xdeadbeefdeadbeef,) to users)";
    std::string_view req3 = R"(insert (login = "vasya", password_hash = 0xdeadbeefdeadbeef) to users)";
    std::string_view req3_2 = R"(insert (login = "petyapetya", password_hash = 0xdeadbeefdeadbeef) to users)";
    std::string_view req4 = R"(insert (,"admin", 0x0000000000000000, true) to users)";
    std::string_view req5 = R"(select id, login from users where is_admin)";
    std::string_view req6 = R"(select id, login from users where is_admin || id < 10)";
    std::string_view req7 = R"(update users set is_admin = true where login = "vasya")";
    std::string_view req8 = R"(update users set login = login + "_deleted", is_admin = false where password_hash < 0x00000000ffffffff)";
    //std::string_view req9 = R"(delete users where |login| % 2 = 0)";
    std::string_view req9 = R"(delete users where login = "vasya")";
    std::string_view req9_1 = R"(select id, login from users where login = "vasya")";

    db.Execute(req1);
    db.Execute(req2);
    db.Execute(req3);
    db.Execute(req3_2);
    db.Execute(req4);
    std::string_view reqselect = R"(select id, login, password_hash from users where id > 1 && |login| = |"petya" + "petya"|)";
    auto res = db.Execute(reqselect).value();
    for (auto row: res) {
        std::cout << row.Get<int32_t>("id") << ' ' << row.Get<std::string>("login") << std::endl;
    }
    return 0;

    { // update test
        {
            auto res = db.Execute(req9_1).value();
            for (auto row: res) {
                std::cout << row.Get<int32_t>("id") << ' ' << row.Get<std::string>("login") << std::endl;
            }
        }
        std::string_view req10 = R"(update users set login = login + "123" where password_hash = 0xdeadbeefdeadbeef)";
        db.Execute(req10);
        {
            std::string_view req9_2 = R"(select id, login from users where login = "vasya123")";
            auto res = db.Execute(req9_2).value();
            for (auto row: res) {
                std::cout << row.Get<int32_t>("id") << ' ' << row.Get<std::string>("login") << std::endl;
            }
        }
    }

    { // delete test
        {
            auto res = db.Execute(req9_1).value();
            for (auto row: res) {
                std::cout << row.Get<int32_t>("id") << ' ' << row.Get<std::string>("login") << std::endl;
            }
        }
        db.Execute(req9);
        {
            auto res = db.Execute(req9_1).value();
            for (auto row: res) {
                std::cout << row.Get<int32_t>("id") << ' ' << row.Get<std::string>("login") << std::endl;
            }
        }
    }
    /*db.Execute(req5);
    for (auto row: db.Execute(req5).value()) {
        std::cout << row.Get<int32_t>("id") << ' ' << row.Get<std::string>("login") << std::endl;
    }* /
    
    int x;
    x = 1;

    /*const std::string_view tName = "table1";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    const std::string_view tName2 = "table2";
    std::vector<ColumnType> colTps2 = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts2 = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName2, colTps2, colDescripts);

    std::shared_ptr<DbType> value{new DbInt32(123)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("table1", to_add);

    std::shared_ptr<DbType> value2{new DbInt32(321)};
    std::vector<Table::Value> to_add2 = {Table::Value(value2)};
    db.Insert("table2", to_add);*/
}
#endif