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
};

}
}