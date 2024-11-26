#pragma once
#include <string_view>
#include <string>
#include <map>
#include <memory>
#include "../type/type.cpp"
#include "lexer.cpp"
#include "condition.cpp"
#include "createColumns.cpp"
#include "inserIndex.cpp"

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

class Parser {
public:
    Parser() = default;

    void parseSpaces(const std::string_view& s, int& ind) {
        while (ind < s.size() && s[ind] == ' ') {
            ind++;
        }
    }

    std::string parseWord(const std::string_view& s, int& ind) {
        parseSpaces(s, ind);
        std::string word = "";
        while (ind < s.size() && s[ind] != ' ') {
            word += s[ind];
            ind++;
        }
        return word;
    }

/*
    std::pair<std::string_view, std::string_view> ParseTableColumn(const std::string_view& s) {
        ssize_t dotInd= 0;
        if ((dotInd = s.find_first_of('.')) != s.find_last_of('.')) {
            // throw ex - names cant have .
            exit(-1);
        }

        if (dotInd != s.npos) {
            return {s.substr(dotInd), s.substr(dotInd + 1, static_cast<int>(s.size()) - static_cast<int>(dotInd) - 1)};
        }
        return {"", s};
    }*/

    /*void Parse(const std::string_view& preS, int l, int r) { // return struct not void TODO
        std::string s = "";
        bool insideString = false, insideLen = false;
        for (int i = l; i <= r; i++) { // to parse words with spaces between
            s += lexer::AddSpace(preS, i, insideString, insideLen);  // care - change index itself
        }

        if (insideLen || insideString) {
            // throw ex - string or len not closed
            exit(-1);
        }

        std::vector<std::string> inp;
        while (l < s.size()) {
            inp.emplace_back(parseWord(s, l));
        }

        std::vector<std::shared_ptr<Tokenizer::Token>> tokens = Tokenizer{}.Tokenize(inp, 0);

        if (tokens.empty()) {
            //throw ex - empty request
            exit(-1);
        }

        //auto res = parser::ConditionParser{}.Parse(tokens, 1, tokens.size() - 1);
        //auto res = parser::ColumnParser{}.Parse(tokens, 0, tokens.size() - 1);
        //auto res = parser::InserValuesParser{}.Parse(tokens, 0, tokens.size() - 1);
    }*/

    std::pair<bool, std::pair<std::string, parser::ColumnDescriptions>> ParseCreate(const std::string_view& preS, int l, int r) { // return struct not void TODO
        std::string s = "";
        bool insideString = false, insideLen = false;
        for (int i = l; i <= r; i++) { // to parse words with spaces between
            s += lexer::AddSpace(preS, i, insideString, insideLen);  // care - change index itself
        }

        if (insideLen || insideString) {
            // throw ex - string or len not closed
            exit(-1);
        }

        std::vector<std::string> inp;
        while (l < s.size()) {
            inp.emplace_back(parseWord(s, l));
        }

        std::vector<std::shared_ptr<Tokenizer::Token>> tokens = Tokenizer{}.Tokenize(inp, 0);

        if (tokens.empty()) {
            //throw ex - empty request
            exit(-1);
        }

        if (tokens.size() <= 2) {
            // throw ex - incorrect req
            exit(-1);
        }

        if (tokens[0]->GetType() != Tokenizer::TokenT::COMMAND) {
            // throw ex - incorrect req
            exit(-1);
        }
        if (parser::getValue<Tokenizer::CommandT, lexer::Command>(tokens[0]) != lexer::Command::CREATE) {
            return {false, {"", parser::ColumnDescriptions{}}};
        }

        //return parser::ConditionParser{}.Parse(tokens, 1, tokens.size() - 1);
        return {true, {parser::getValue<Tokenizer::StringT, std::string>(tokens[2]), parser::ColumnParser{}.Parse(tokens, 3, tokens.size() - 1)}};
        //auto res = parser::InserValuesParser{}.Parse(tokens, 0, tokens.size() - 1);
    }

    std::pair<bool, std::pair<std::string, parser::InsertData>> ParseInsert(const std::string_view& preS, int l, int r) { // return struct not void TODO
        std::string s = "";
        bool insideString = false, insideLen = false;
        for (int i = l; i <= r; i++) { // to parse words with spaces between
            s += lexer::AddSpace(preS, i, insideString, insideLen);  // care - change index itself
        }

        if (insideLen || insideString) {
            // throw ex - string or len not closed
            exit(-1);
        }

        std::vector<std::string> inp;
        while (l < s.size()) {
            inp.emplace_back(parseWord(s, l));
        }

        std::vector<std::shared_ptr<Tokenizer::Token>> tokens = Tokenizer{}.Tokenize(inp, 0);

        if (tokens.empty()) {
            //throw ex - empty request
            exit(-1);
        }

        if (tokens.size() <= 2) {
            // throw ex - incorrect req
            exit(-1);
        }

        if (tokens[0]->GetType() != Tokenizer::TokenT::COMMAND) {
            // throw ex - incorrect req
            exit(-1);
        }
        if (parser::getValue<Tokenizer::CommandT, lexer::Command>(tokens[0]) != lexer::Command::INSERT) {
            return {false, {"", parser::InsertData{}}};
        }


        //return parser::ConditionParser{}.Parse(tokens, 1, tokens.size() - 1);
        return {true, {parser::getValue<Tokenizer::StringT, std::string>(tokens[tokens.size() - 1]), parser::InserValuesParser{}.Parse(tokens, 1, tokens.size() - 2)}};
        //auto res = parser::InserValuesParser{}.Parse(tokens, 0, tokens.size() - 1);
    }
};

}

/*
int main() {
    //std::string_view req = "where is_admin || id < 10";
    //std::string_view req = "where is_admin && x^^y = 0 || |\"abc\"|!=2 || id < 10";
    //memdb::Parser{}.Parse(req, 0, req.size() - 1);

    //std::string_view req = "({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)";
    //memdb::Parser{}.Parse(req, 0, req.size() - 1);

    //std::string_view req = "(,\"vasya\", 0xdeadbeefdeadbeef)";
    //std::string_view req = "(login = \"vasya\", password_hash = 0xdeadbeefdeadbeef)";
    //memdb::Parser{}.Parse(req, 0, req.size() - 1);
}*/