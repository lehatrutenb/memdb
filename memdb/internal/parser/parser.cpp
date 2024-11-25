#pragma once
#include <string_view>
#include <string>
#include <map>
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

create ordered index on users by login create unordered index on users by is_admin
*/

namespace memdb {

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

enum class Lexer {
    ABA,
    ZZZ
};

std::map<std::string, Lexer> m = {
#include "lexer.def"
};

}

int main() {
    std::map<int, int> v = {{1, 2}, {3, 4}};
}