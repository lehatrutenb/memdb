#pragma once
#include <string_view>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include <cctype>
#include "../type/type.hpp"
#include "../column/column.hpp"

namespace memdb {

namespace lexer {
    enum class Command {
        CREATE,
        INSERT,
        SELECT,
        UPDATE,
        DELETE,
        JOIN
    };

    enum class SubCommand {
        WHERE,
        ON,
        SET,
        FROM,
        INDEX,
        TABLE,
        TO,
        BY
    };

    enum class Attribute {
        UNIQUE = 1,
        AUTOINCREMENT = 2,
        KEY = 4,
        ORDERED,
        UNORDERED
    };

    enum class Bracket {
        CIRCLEO,
        CIRCLEC,
        FIGUREO,
        FIGUREC,
        LENO,
        LENC,
        RECTO,
        RECTC,
    };

    enum class Other {
        TWOPOINT,
        COMMA
    };

   /*enum class ColumnType {
        INT32,
        BOOL,
        BYTES,
        STRING
    };*/

    static std::map<std::string_view, Command> s2c = {
        {"create", Command::CREATE}, {"insert", Command::INSERT}, {"select", Command::SELECT}, {"update", Command::UPDATE}, {"delete", Command::DELETE}, 
        {"join", Command::JOIN}
    };

    static std::map<std::string_view, SubCommand> s2sc = {
        {"where", SubCommand::WHERE}, {"on", SubCommand::ON}, {"set", SubCommand::SET}, {"from", SubCommand::FROM}, {"index", SubCommand::INDEX}, {"table", SubCommand::TABLE}, {"to", SubCommand::TO}, {"to", SubCommand::BY}
    };

    static std::map<std::string_view, Attribute> s2a = {
        {"unique", Attribute::UNIQUE}, {"autoincrement", Attribute::AUTOINCREMENT}, {"key", Attribute::KEY}, {"ordered", Attribute::ORDERED},
        {"unordered", Attribute::UNORDERED}
    };

    // CARE | IS NOT REALLY OPERATION CURRENTLY TODO
    static std::map<std::string_view, Operation> s2op = {
        {"+", Operation::PLUS}, {"-", Operation::MINUS}, {"*", Operation::MULTIPLY}, {"/", Operation::DIVIDE}, {"%", Operation::MOD}, {"=", Operation::EQ}, 
        {"<", Operation::LESS}, {">", Operation::MORE}, {"<=", Operation::EQLESS}, {">=", Operation::EQMORE}, {"!=", Operation::NOTEQ}, {"&&", Operation::AND}, 
        {"||", Operation::OR}, {"!", Operation::NOT}, {"^^", Operation::XOR}, {"|", Operation::LEN}
    };

    static std::map<std::string_view, Bracket> s2br = {
        {"(", Bracket::CIRCLEO}, {")", Bracket::CIRCLEC}, {"|{", Bracket::LENO}, {"}|", Bracket::LENC}, {"{", Bracket::FIGUREO}, {"}", Bracket::FIGUREC}, {"[", Bracket::RECTO}, {"]", Bracket::RECTC}, 
    };

    static std::map<std::string_view, Type> s2ct = {
        {"int32", Type::Int32}, {"bool", Type::Bool}, {"string", Type::String}, {"bytes", Type::Bytes}, 
    };

    static std::map<std::string_view, Other> s2oth = {
        {":", Other::TWOPOINT}, {",", Other::COMMA},
    };

    std::string AddSpace(const std::string_view& s, int& ind, bool& insideString, bool& insideLen);

    std::pair<bool, Command> IsCommand(std::vector<std::string>& inp, int ind);

    std::pair<bool, SubCommand> IsSubCommand(std::vector<std::string>& inp, int ind);

    std::pair<bool, Attribute> IsAttribute(std::vector<std::string>& inp, int ind);

    std::pair<bool, Operation> IsOperation(std::vector<std::string>& inp, int ind);

    std::pair<bool, Other> IsOther(std::vector<std::string>& inp, int ind);
    std::pair<bool, Bracket> IsBracket(std::vector<std::string>& inp, int ind);

    std::pair<bool, int> IsInt(std::vector<std::string>& inp, int ind);

    std::pair<bool, ColumnType> IsColumnType(std::vector<std::string>& inp, int ind);

    std::pair<bool, std::string> IsString(std::vector<std::string>& inp, int ind);

    std::pair<bool, std::vector<char>> IsBytes(std::vector<std::string>& inp, int ind);

    std::pair<bool, bool> IsBool(std::vector<std::string>& inp, int ind);
}

struct Tokenizer {

enum class TokenT {
    COMMAND,
    SUBCOMMAND,
    ATTRIBUTE,
    OPERATION,
    BRACKET,
    COLUMNTYPE,
    DBTYPE,
    STRING,
    OTHER,
    CONDITION
};

struct Token {
    virtual TokenT GetType() const = 0;
    virtual std::shared_ptr<Token> Copy()=0;
    virtual bool Parse(std::vector<std::string>& inp, int& ind) = 0;
};

template<typename T>
static std::shared_ptr<Token> getSharedToken() {
    std::shared_ptr<Token> res{new T{}};
    return res;
}

template<typename T>
static std::shared_ptr<Token> getSharedToken(T* val) {
    std::shared_ptr<Token> res{val};
    return res;
}

struct CommandT : public Token {
    public:
    CommandT();
    CommandT(lexer::Command t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;

    lexer::Command t;
};

struct SubCommandT : public Token {
    public:
    SubCommandT();
    SubCommandT(lexer::SubCommand t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;

    lexer::SubCommand t;
};

struct AttributeT : public Token {
    public:
    AttributeT();
    AttributeT(lexer::Attribute t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;

    lexer::Attribute t;
};

struct OtherT : public Token {
    public:
    OtherT();
    OtherT(lexer::Other t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;
    lexer::Other t;
};


struct OperationT : public Token {
    public:
    OperationT();
    OperationT(Operation t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;

    Operation t;
};

struct BracketT : public Token {
    public:
    BracketT();
    BracketT(lexer::Bracket t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;

    lexer::Bracket t;
};

struct ColumnTypeT : public Token {
    public:
    ColumnTypeT();
    ColumnTypeT(ColumnType t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;

    ColumnType t;
};

struct DBTypeT : public Token {
    public:
    DBTypeT();
    DBTypeT(std::shared_ptr<DbType> t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;
    std::shared_ptr<DbType> t;
};

struct StringT : public Token {
    public:
    StringT();
    StringT(std::string t_);
    TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy() override;

    std::string t;
};

std::vector<std::shared_ptr<Token>> tokenSeq = {getSharedToken<CommandT>(), getSharedToken<SubCommandT>(), getSharedToken<AttributeT>(),
            getSharedToken<OperationT>(), getSharedToken<BracketT>(), getSharedToken<ColumnTypeT>(), getSharedToken<DBTypeT>(), getSharedToken<OtherT>(), getSharedToken<StringT>()};

std::vector<std::shared_ptr<Token>> Tokenize(std::vector<std::string> inp, int ind);

};

namespace parser {
template<typename T, typename U>
U getValue(std::shared_ptr<Tokenizer::Token> token) {
    return dynamic_cast<T*>(token.get())->t;
}


template<typename T, typename U>
int getNextToken(const std::vector<std::shared_ptr<Tokenizer::Token>>& ts, ssize_t ind, ssize_t r, Tokenizer::TokenT tokenTExp, U resExp) {
    for (; ind <= r; ind++) {
        if (ts[ind]->GetType() == tokenTExp && getValue<T, U>(ts[ind]) == resExp) {
            return ind;
        }
    }
    return -1;
}

std::pair<std::string, std::string> ParseTableColumn(const std::string& s);

struct TokenStructure {
    template<typename T>
    struct ValueInd {
        T val;
        ssize_t ind;
    };

    std::vector<ValueInd<lexer::Command>> Commands;
    std::vector<ValueInd<lexer::SubCommand>> SubCommands;
    std::map<lexer::Command, ssize_t> C2i;
    std::map<lexer::SubCommand, ssize_t> Sc2i;
    std::vector<std::shared_ptr<Tokenizer::Token>> Tokens;

    int getCmd(lexer::Command cmd);

    int getScmd(lexer::SubCommand cmd);

    TokenStructure(const std::vector<std::shared_ptr<Tokenizer::Token>>&& tokens, ssize_t l, ssize_t r);
};
}
}