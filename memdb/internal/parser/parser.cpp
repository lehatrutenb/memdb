#include <string_view>
#include <string>
#include <map>
#include <memory>
#include "parser.hpp"
#include <iostream>

//#include "objects.cpp"
/*
create table users ({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)



insert (,"vasya", 0xdeadbeefdeadbeef) to users
insert (login = "vasya", password_hash = 0xdeadbeefdeadbeef) to users
insert (,"admin", 0x0000000000000000, true) to users
insert (
         is_admin = true,
         login = "admin",
         password_hash = 0x0000000000000000
     ) to users


select id, login from users where is_admin || id < 10

update users set is_admin = true where login = "vasya"
update users set login = login + "_deleted", is_admin = false where password_hash < 0x00000000ffffffff

delete users where |login| % 2 = 0

select posts.id, users.login, posts.text from users join posts on users.id = posts.user_id where true
select posts.id, users.login, posts.text from users join posts on users.id = posts.user_id

create ordered index on users by login
create unordered index on users by is_admin
*/

namespace memdb {
namespace parser {


ParserT CreateTableParser::getType() const {
    return ParserT::CREATE_TABLE;
}

bool CreateTableParser::Check(parser::TokenStructure& ts) {
    return ts.getCmd(lexer::Command::CREATE) == 0 && ts.getScmd(lexer::SubCommand::TABLE) == 1 &&
            ts.Tokens.size() >= 2 && ts.Tokens[2]->GetType() == Tokenizer::TokenT::STRING;
}



ParserT InsertParser::getType() const {
    return ParserT::INSERT;
}

bool InsertParser::Check(parser::TokenStructure& ts) {
    return ts.getCmd(lexer::Command::INSERT) == 0 && ts.getScmd(lexer::SubCommand::TO) == ts.Tokens.size() - 2 &&
            ts.Tokens[ts.Tokens.size() - 1]->GetType() == Tokenizer::TokenT::STRING;
}



ParserT SelectParser::getType() const {
    return ParserT::SELECT;
}

bool SelectParser::Check(parser::TokenStructure& ts) {
    return ts.getCmd(lexer::Command::SELECT) == 0 && ts.getScmd(lexer::SubCommand::FROM) != -1 &&
            ts.getScmd(lexer::SubCommand::WHERE) > ts.getScmd(lexer::SubCommand::FROM);
}



ParserT UpdateParser::getType() const {
    return ParserT::UPDATE;
}

bool UpdateParser::Check(parser::TokenStructure& ts) {
    return ts.getCmd(lexer::Command::UPDATE) == 0 && ts.getScmd(lexer::SubCommand::SET) != -1 &&
            ts.getScmd(lexer::SubCommand::WHERE) > ts.getScmd(lexer::SubCommand::SET);
}



ParserT DeleteParser::getType() const {
    return ParserT::DELETE;
}

bool DeleteParser::Check(parser::TokenStructure& ts) {
    return ts.getCmd(lexer::Command::DELETE) == 0 && ts.getScmd(lexer::SubCommand::WHERE) == 2 &&
            ts.Tokens[1]->GetType() == Tokenizer::TokenT::STRING;
}



ParserT JoinParser::getType() const {
    return ParserT::JOIN;
}

bool JoinParser::Check(parser::TokenStructure& ts) {
    return ts.getCmd(lexer::Command::JOIN) == 1 && ts.getScmd(lexer::SubCommand::ON) == 3 &&
            ts.Tokens[0]->GetType() == Tokenizer::TokenT::STRING &&
            ts.Tokens[2]->GetType() == Tokenizer::TokenT::STRING;
}



ParserT CrateIndexParser::getType() const {
    return ParserT::CREATE_INDEX;
}

bool CrateIndexParser::Check(parser::TokenStructure& ts) {
    return ts.getCmd(lexer::Command::CREATE) == 0 && ts.getScmd(lexer::SubCommand::INDEX) == 2 &&
            ts.getScmd(lexer::SubCommand::ON) == 3 && ts.getScmd(lexer::SubCommand::BY) == 5 &&
            ts.Tokens[1]->GetType() == Tokenizer::TokenT::ATTRIBUTE &&
            ts.Tokens[4]->GetType() == Tokenizer::TokenT::STRING &&
            (getValue<Tokenizer::AttributeT, lexer::Attribute>(ts.Tokens[1]) == lexer::Attribute::ORDERED ||
            getValue<Tokenizer::AttributeT, lexer::Attribute>(ts.Tokens[1]) == lexer::Attribute::UNORDERED);
}




void Parser::parseSpaces(const std::string_view& s, ssize_t& ind) {
    while (ind < s.size() && s[ind] == ' ') {
        ind++;
    }
}

std::string Parser::parseWord(const std::string_view& s, ssize_t& ind) {
    parseSpaces(s, ind);
    std::string word = "";
    while (ind < s.size() && s[ind] != ' ') {
        word += s[ind];
        ind++;
    }
    return word;
}

parser::TokenStructure Parser::Parse(const std::string_view& preS, ssize_t l, ssize_t r) { // return struct not void TODO
    std::string s = "";
    bool insideString = false, insideLen = false;
    for (int i = l; i <= r; i++) { // to parse words with spaces between
        s += lexer::AddSpace(preS, i, insideString, insideLen, r);  // care - change index itself
    }

    if (insideLen || insideString) {
        // throw ex - string or len not closed
        throw std::runtime_error("error");
        exit(-1);
    }

    std::vector<std::string> inp;
    while (l < s.size()) {
        parseSpaces(s, l);
        inp.emplace_back(parseWord(s, l));
        parseSpaces(s, l);
    }

    std::vector<std::shared_ptr<Tokenizer::Token>> tokens = Tokenizer{}.Tokenize(inp, 0);

    if (tokens.empty()) {
        //throw ex - empty request
        throw std::runtime_error("error");
        exit(-1);
    }

    return parser::TokenStructure(std::move(tokens), 0, tokens.size() - 1);
}

}
}

/*
std::pair<bool, std::pair<std::string, parser::ColumnDescriptions>> Parser::ParseCreate(const std::string_view& preS, ssize_t l, ssize_t r) { // return struct not void TODO
    std::string s = "";
    bool insideString = false, insideLen = false;
    for (int i = l; i <= r; i++) { // to parse words with spaces between
        s += lexer::AddSpace(preS, i, insideString, insideLen, r);  // care - change index itself
    }

    if (insideLen || insideString) {
        // throw ex - string or len not closed
        throw std::runtime_error("error");
        exit(-1);
    }

    std::vector<std::string> inp;
    while (l < s.size()) {
        inp.emplace_back(parseWord(s, l));
    }

    std::vector<std::shared_ptr<Tokenizer::Token>> tokens = Tokenizer{}.Tokenize(inp, 0);

    if (tokens.empty()) {
        //throw ex - empty request
        throw std::runtime_error("error");
        exit(-1);
    }

    if (tokens.size() <= 2) {
        // throw ex - incorrect req
        throw std::runtime_error("error");
        exit(-1);
    }

    if (tokens[0]->GetType() != Tokenizer::TokenT::COMMAND) {
        // throw ex - incorrect req
        throw std::runtime_error("error");
        exit(-1);
    }
    if (parser::getValue<Tokenizer::CommandT, lexer::Command>(tokens[0]) != lexer::Command::CREATE) {
        return {false, {"", parser::ColumnDescriptions{}}};
    }

    //return parser::ConditionParser{}.Parse(tokens, 1, tokens.size() - 1);
    return {true, {parser::getValue<Tokenizer::StringT, std::string>(tokens[2]), parser::ColumnParser{}.Parse(tokens, 3, tokens.size() - 1)}};
    //auto res = parser::InserValuesParser{}.Parse(tokens, 0, tokens.size() - 1);
}

std::pair<bool, std::pair<std::string, parser::InsertData>> Parser::ParseInsert(const std::string_view& preS, ssize_t l, ssize_t r) { // return struct not void TODO
    std::string s = "";
    bool insideString = false, insideLen = false;
    for (int i = l; i <= r; i++) { // to parse words with spaces between
        s += lexer::AddSpace(preS, i, insideString, insideLen, r);  // care - change index itself
    }

    if (insideLen || insideString) {
        // throw ex - string or len not closed
        throw std::runtime_error("error");
        exit(-1);
    }

    std::vector<std::string> inp;
    while (l < s.size()) {
        inp.emplace_back(parseWord(s, l));
    }

    std::vector<std::shared_ptr<Tokenizer::Token>> tokens = Tokenizer{}.Tokenize(inp, 0);

    if (tokens.empty()) {
        //throw ex - empty request
        throw std::runtime_error("error");
        exit(-1);
    }

    if (tokens.size() <= 2) {
        // throw ex - incorrect req
        throw std::runtime_error("error");
        exit(-1);
    }

    if (tokens[0]->GetType() != Tokenizer::TokenT::COMMAND) {
        // throw ex - incorrect req
        throw std::runtime_error("error");
        exit(-1);
    }
    if (parser::getValue<Tokenizer::CommandT, lexer::Command>(tokens[0]) != lexer::Command::INSERT) {
        return {false, {"", parser::InsertData{}}};
    }


    //return parser::ConditionParser{}.Parse(tokens, 1, tokens.size() - 1);
    return {true, {parser::getValue<Tokenizer::StringT, std::string>(tokens[tokens.size() - 1]), parser::InserValuesParser{}.Parse(tokens, 1, tokens.size() - 2)}};
    //auto res = parser::InserValuesParser{}.Parse(tokens, 0, tokens.size() - 1);
}

}
}*/