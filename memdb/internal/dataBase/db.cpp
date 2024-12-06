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
14. |0x12| = 2
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
    std::string_view req1 = "create table users ({autoincrement} id: int32, c1 : int32, c2 : string[10], c3 : bytes[10], c4 : string[10])";
    std::string_view req2 = R"(insert (,123, "456", 0x789, "101112") to users)";
    std::string_view req3 = R"(insert (,123, "456", 0x789, "101112") to users)";
    std::string_view req4 = R"(update users set c1=c1*10+|c2|,c2=c2+c2, c3=0x1, c4=c2+c4+"0" where id = 0)";
    std::string_view req5 = R"(select id from users where c1=1233 && c2="456456" && c4="4561011120")";
    std::string_view req6 = R"(select id from users where c1=123 && c2="456" && c4="101112")";


    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    EXPECT_NO_THROW(db.Execute(req5));
    EXPECT_NO_THROW(db.Execute(req6));
    return 0;
}
#endif