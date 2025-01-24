#include <string_view>
#include <map>
#include "db.hpp"
#include <stdexcept>

namespace memdb {

void Database::CreateTable(const std::string& tName, const std::vector<ColumnFullDescription>& colDescripts) {
    // throw ex i f colDescripts and colTps not same size
    if (tName2ind.find(tName) != tName2ind.end()) {
        // throw ex - table names should be unique
        throw std::runtime_error("error");
    }
    tName2ind[tName] = tables.size();
    tables.emplace_back(Table{tName});

    for (int i = 0; i < colDescripts.size(); i++) {
        tables.back().AddColumn(colDescripts[i]);
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