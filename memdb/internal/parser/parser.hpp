#pragma once
#include <string_view>
#include <string>
#include <map>
#include <memory>
#include "../type/type.hpp"
#include "lexer.hpp"
#include "condition.hpp"
#include "createColumns.hpp"
#include "insertIndex.hpp"
#include "assignments.hpp"
#include "selectValues.hpp"
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


enum class ParserT {
        CREATE_TABLE,
        INSERT,
        SELECT,
        UPDATE,
        DELETE,
        JOIN,
        CREATE_INDEX,
        UNKNOWN
    };

struct CmdParser {
    virtual ParserT getType() const = 0;
    virtual bool Check(parser::TokenStructure& ts) = 0;
};

class CreateTableParser : public CmdParser {
    public:
    ParserT getType() const override;

    bool Check(parser::TokenStructure& ts) override;
};

class InsertParser : public CmdParser {
    public:
    ParserT getType() const override;

    bool Check(parser::TokenStructure& ts) override;
};

class SelectParser : public CmdParser {
    public:
    ParserT getType() const override;

    bool Check(parser::TokenStructure& ts) override;
};

class UpdateParser : public CmdParser {
    public:
    ParserT getType() const override;
    bool Check(parser::TokenStructure& ts) override;
};

class DeleteParser : public CmdParser {
    public:
    ParserT getType() const override;

    bool Check(parser::TokenStructure& ts) override;
};

class JoinParser : public CmdParser {
    public:
    ParserT getType() const override;

    bool Check(parser::TokenStructure& ts) override;
};


class CrateIndexParser : public CmdParser {
    public:
    ParserT getType() const override;

    bool Check(parser::TokenStructure& ts) override;
};

template<typename T>
static std::shared_ptr<CmdParser> getSharedParser() {
    std::shared_ptr<CmdParser> res{new T{}};
    return res;
}


static std::vector<std::shared_ptr<CmdParser>> Parsers = {
                                                        getSharedParser<CreateTableParser>(), getSharedParser<InsertParser>(), 
                                                        getSharedParser<SelectParser>(), getSharedParser<UpdateParser>(),
                                                        getSharedParser<DeleteParser>(), getSharedParser<JoinParser>(),
                                                        getSharedParser<CrateIndexParser>()
                                                        };

class Parser {
public:
    Parser() = default;

    void parseSpaces(const std::string_view& s, ssize_t& ind);

    std::string parseWord(const std::string_view& s, ssize_t& ind);


    parser::TokenStructure Parse(const std::string_view& preS, ssize_t l, ssize_t r);

    //std::pair<bool, std::pair<std::string, parser::ColumnDescriptions>> ParseCreate(const std::string_view& preS, ssize_t l, ssize_t r);
    //std::pair<bool, std::pair<std::string, parser::InsertData>> ParseInsert(const std::string_view& preS, ssize_t l, ssize_t r);
};

}
}