#pragma once
#include <string_view>
#include <string>
#include <map>
#include "objects.cpp"
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

create ordered index on users by login
create unordered index on users by is_admin
*/

namespace memdb {

using namespace parser;

struct Lexer {
    std::map<std::string, Command> s2c = {
        {"create", Command::CREATE}, {"insert", Command::CREATE}, {"select", Command::CREATE}, {"update", Command::CREATE}, {"delete", Command::CREATE}, 
        {"join", Command::CREATE}
    };

    std::map<std::string, SubCommand> s2sc = {
        {"where", SubCommand::WHERE}, {"on", SubCommand::ON}, {"set", SubCommand::SET}, {"from", SubCommand::FROM}, {"index", SubCommand::INDEX}
    };

    std::map<std::string, Attribute> s2a = {
        {"unique", Attribute::UNIQUE}, {"autoincrement", Attribute::AUTOINCREMENT}, {"key", Attribute::KEY}, {"ordered", Attribute::ORDERED},
        {"unordered", Attribute::UNORDERED}
    };

    std::map<std::string, Operation> s2op = {
        {"+", Operation::PLUS}, {"-", Operation::MINUS}, {"*", Operation::MULTIPLY}, {"/", Operation::DIVIDE}, {"%", Operation::MOD}, {"=", Operation::EQ}, 
        {"<", Operation::LESS}, {">", Operation::MORE}, {"<=", Operation::EQLESS}, {">=", Operation::EQMORE}, {"!=", Operation::NOTEQ}, {"&&", Operation::AND}, 
        {"||", Operation::OR}, {"!", Operation::NOT}, {"XOR", Operation::XOR}, {"|", Operation::LEN}
    };
};

class Parser {
    Parser() = default;

    void parse_spaces(const std::string_view& s, ssize_t& ind) {
        while (ind < s.size() && s[ind] == ' ') {
            ind++;
        }
    }

    std::string parse_word(const std::string_view& s, ssize_t& ind) {
        parse_spaces(s, ind);
        std::string word = "";
        while (ind < s.size() && s[ind] != ' ') {
            word += s[ind];
            ind++;
        }
        return word;
    }

    void parse(const std::string_view& s, ssize_t& ind) { // return struct not void TODO

    }
};

}

int main() {

}