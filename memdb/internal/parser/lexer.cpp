#pragma once
#include <string_view>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include <cctype>
#include "../type/type.cpp"

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
        INDEX
    };

    enum class Attribute {
        UNIQUE,
        AUTOINCREMENT,
        KEY,
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
        TWOPOINT
    };

    enum class ColumnType {
        INT32,
        BOOL,
        BYTES,
        STRING
    };

    std::map<std::string_view, Command> s2c = {
        {"create", Command::CREATE}, {"insert", Command::INSERT}, {"select", Command::SELECT}, {"update", Command::UPDATE}, {"delete", Command::DELETE}, 
        {"join", Command::JOIN}
    };

    std::map<std::string_view, SubCommand> s2sc = {
        {"where", SubCommand::WHERE}, {"on", SubCommand::ON}, {"set", SubCommand::SET}, {"from", SubCommand::FROM}, {"index", SubCommand::INDEX}
    };

    std::map<std::string_view, Attribute> s2a = {
        {"unique", Attribute::UNIQUE}, {"autoincrement", Attribute::AUTOINCREMENT}, {"key", Attribute::KEY}, {"ordered", Attribute::ORDERED},
        {"unordered", Attribute::UNORDERED}
    };

    // CARE | IS NOT REALLY OPERATION CURRENTLY TODO
    std::map<std::string_view, Operation> s2op = {
        {"+", Operation::PLUS}, {"-", Operation::MINUS}, {"*", Operation::MULTIPLY}, {"/", Operation::DIVIDE}, {"%", Operation::MOD}, {"=", Operation::EQ}, 
        {"<", Operation::LESS}, {">", Operation::MORE}, {"<=", Operation::EQLESS}, {">=", Operation::EQMORE}, {"!=", Operation::NOTEQ}, {"&&", Operation::AND}, 
        {"||", Operation::OR}, {"!", Operation::NOT}, {"^^", Operation::XOR}, {"|", Operation::LEN}
    };

    std::map<std::string_view, Bracket> s2br = {
        {"(", Bracket::CIRCLEO}, {")", Bracket::CIRCLEC}, {"|{", Bracket::LENO}, {"}|", Bracket::LENC}, {"{", Bracket::FIGUREO}, {"}", Bracket::FIGUREC}, {"[]", Bracket::RECTO}, {"]", Bracket::RECTC}, {":", Bracket::TWOPOINT}, 
    };

    std::map<std::string_view, ColumnType> s2ct = {
        {"int32", ColumnType::INT32}, {"bool", ColumnType::BOOL}, {"string", ColumnType::STRING}, {"bytes", ColumnType::BYTES}, 
    };

    std::string AddSpace(const std::string_view& s, int& ind, bool& insideString, bool& insideLen) { // care - change index itself
        char ch = s[ind];
        std::string res;
        if (ch == '"') {
            insideString ^= 1;
            res.push_back(ch);
            return res;
        } else if (insideString) {
            res.push_back(ch);
            return res;
        }
        if (isdigit(ch) || isalpha(ch) || ch == '.' || ch == '"' || ch == '_' || ch == '#' || ch == '@' || ch == ' ') {
            res.push_back(ch);
            return res;
        }
        if (ind == static_cast<int>(s.size()) - 1) {
            res.push_back(' ');
            res.push_back(ch);
            return res;
        } else {
            res.push_back(' ');
            if ((ch == '|' && s[ind + 1] == '|')||(ch == '&' && s[ind + 1] == '&')||(ch == '^' && s[ind + 1] == '^')||((ch=='<'||ch=='>'||ch=='!')&&s[ind + 1]=='=')) {
                ind++;
                res.push_back(ch);
                res.push_back(s[ind]);
            } else {
                if (ch == '|') {
                    insideLen ^= 1;
                    if (insideLen) {
                        res += "|{";
                    } else {
                        res += "}|";
                    }
                } else {
                    res.push_back(ch);
                }
            }
            res.push_back(' ');
            return res;
        }
    } 

    std::pair<bool, Command> IsCommand(std::vector<std::string>& inp, int ind) {
        if (s2c.find(inp[ind]) == s2c.end()) {
            return {false, Command{}};
        }
        return {true, s2c[inp[ind]]};
    }

    std::pair<bool, SubCommand> IsSubCommand(std::vector<std::string>& inp, int ind) {
        if (s2sc.find(inp[ind]) == s2sc.end()) {
            return {false, SubCommand{}};
        }
        return {true, s2sc[inp[ind]]};
    }

    std::pair<bool, Attribute> IsAttribute(std::vector<std::string>& inp, int ind) {
        if (s2a.find(inp[ind]) == s2a.end()) {
            return {false, Attribute{}};
        }
        return {true, s2a[inp[ind]]};
    }

    std::pair<bool, Operation> IsOperation(std::vector<std::string>& inp, int ind) {
        if (s2op.find(inp[ind]) == s2op.end()) {
            return {false, Operation{}};
        }
        return {true, s2op[inp[ind]]};
    }

    std::pair<bool, Bracket> IsBracket(std::vector<std::string>& inp, int ind) {
        if (ind + 1 < inp.size() && s2br.find(inp[ind] + inp[ind + 1]) != s2br.end()) {
             return {true, s2br[inp[ind] + inp[ind + 1]]};
        }
        if (s2br.find(inp[ind]) == s2br.end()) {
            return {false, Bracket{}};
        }
        return {true, s2br[inp[ind]]};
    }

    std::pair<bool, int> IsInt(std::vector<std::string>& inp, int ind) {
        if (std::all_of(inp[ind].begin(), inp[ind].end(), [](char ch) {return '0' <= ch && ch <= '9';})) {
            return {true, stoi(inp[ind])};
        }
        return {false, {}};
    }

    std::pair<bool, ColumnType> IsColumnType(std::vector<std::string>& inp, int ind) {
        if (s2ct.find(inp[ind]) == s2ct.end()) {
            return {false, ColumnType{}};
        }
        if (s2ct[inp[ind]] == ColumnType::STRING || s2ct[inp[ind]] == ColumnType::BYTES) {
            if (ind + 3 < inp.size() && IsBracket(inp, ind + 1).second == Bracket::RECTO && IsBracket(inp, ind + 3).second == Bracket::RECTC && IsInt(inp, ind + 2).first == true) {
                return {true, s2ct[inp[ind]]};
            }
            return {false, ColumnType{}};
        }
        return {true, s2ct[inp[ind]]};
    }

    std::pair<bool, std::string> IsString(std::vector<std::string>& inp, int ind) {
        if (inp[ind].front() == '"' && inp[ind].back() == '"') {
            return {true, inp[ind].substr(1, static_cast<int>(inp[ind].size()) - 2)};
        }
        return {false, {}};
    }

    std::pair<bool, std::vector<char>> IsBytes(std::vector<std::string>& inp, int ind) {
        if (inp[ind].size() >= 3 && inp[ind][0] == '0' && inp[ind][1] == 'x') {
            std::vector<char> v(inp[ind].size());
            v[0] = inp[ind][0];
            v[1] = inp[ind][1];
            for (int i = 2; i < inp[ind].size(); i++) {
                v[i] = inp[ind][i];
                if ('0' <= inp[ind][i] && inp[ind][i] <= '9') { // no isdigit here!
                    continue;
                }
                return {false, {}};
            }
            return {true, v};
        }
        return {false, {}};
    }

    std::pair<bool, bool> IsBool(std::vector<std::string>& inp, int ind) {
        if (inp[ind] == "true" || inp[ind] == "false") {
            return {true, inp[ind] == "true"};
        }
        return {false, {}};
    }
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
    CONDITION
};

struct Token {
    virtual TokenT GetType() const = 0;
    virtual std::shared_ptr<Token> Copy()=0;
    virtual bool Parse(std::vector<std::string>& inp, int& ind) = 0;
};

template<typename T>
static std::shared_ptr<Token> getSharedToken() {
    std::shared_ptr<Token> res;
    res.reset(new T{});
    return res;
}

template<typename T>
static std::shared_ptr<Token> getSharedToken(T* val) {
    std::shared_ptr<Token> res;
    res.reset(val);
    return res;
}

struct CommandT : public Token {
    CommandT() {};
    CommandT(lexer::Command t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::COMMAND;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        auto res = lexer::IsCommand(inp, ind);
        if (res.first) {
            t = res.second;
            ind++;
        }
        return res.first;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new CommandT(t)));
        return copy;
    }

    lexer::Command t;
};

struct SubCommandT : public Token {
    SubCommandT(){};
    SubCommandT(lexer::SubCommand t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::SUBCOMMAND;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        auto res = lexer::IsSubCommand(inp, ind);
        if (res.first) {
            t = res.second;
            ind++;
        }
        return res.first;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new SubCommandT(t)));
        return copy;
    }

    lexer::SubCommand t;
};

struct AttributeT : public Token {
    AttributeT(){};
    AttributeT(lexer::Attribute t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::ATTRIBUTE;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        auto res = lexer::IsAttribute(inp, ind);
        if (res.first) {
            t = res.second;
            ind++;
        }
        return res.first;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new AttributeT(t)));
        return copy;
    }

    lexer::Attribute t;
};


struct OperationT : public Token {
    OperationT(){};
    OperationT(Operation t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::OPERATION;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        auto res = lexer::IsOperation(inp, ind);
        if (res.first) {
            t = res.second;
            ind++;
        }
        return res.first;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new OperationT(t)));
        return copy;
    }

    Operation t;
};

struct BracketT : public Token {
    BracketT(){};
    BracketT(lexer::Bracket t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::BRACKET;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        auto res = lexer::IsBracket(inp, ind);
        if (res.first) {
            t = res.second;
            ind++;
        }
        return res.first;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new BracketT(t)));
        return copy;
    }

    lexer::Bracket t;
};

struct ColumnTypeT : public Token {
    ColumnTypeT(){};
    ColumnTypeT(lexer::ColumnType t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::COLUMNTYPE;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        auto res = lexer::IsColumnType(inp, ind);
        if (res.first) {
            t = res.second;
            ind++;
            if (t == lexer::ColumnType::STRING || t == lexer::ColumnType::BYTES) {
                ind+=3;
            }
        }
        return res.first;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new ColumnTypeT(t)));
        return copy;
    }

    lexer::ColumnType t;
};

struct DBTypeT : public Token {
    DBTypeT(){};
    DBTypeT(std::shared_ptr<DbType> t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::DBTYPE;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        {
            auto res = lexer::IsString(inp, ind);
            if (res.first) {
                t.reset(dynamic_cast<DbType*>(new DbString(res.second)));
                ind++;
                return true;
            }
        }
        {
            auto res = lexer::IsBytes(inp, ind);
            if (res.first) {
                t.reset(dynamic_cast<DbType*>(new DbBytes(res.second)));
                ind++;
                return true;
            }
        }
        {
            auto res = lexer::IsInt(inp, ind);
            if (res.first) {
                t.reset(dynamic_cast<DbType*>(new DbInt32(res.second)));
                ind++;
                return true;
            }
        }
        {
            auto res = lexer::IsBool(inp, ind);
            if (res.first) {
                t.reset(dynamic_cast<DbType*>(new DbBool(res.second)));
                ind++;
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new DBTypeT(t)));
        return copy;
    }

    std::shared_ptr<DbType> t;
};

struct StringT : public Token {
    StringT(){};
    StringT(std::string t_) : t(t_){};
    TokenT GetType() const override {
        return TokenT::STRING;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        t = inp[ind];
        ind++;
        return true;
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new StringT(t)));
        return copy;
    }

    std::string t;
};

std::vector<std::shared_ptr<Token>> tokenSeq = {getSharedToken<CommandT>(), getSharedToken<SubCommandT>(), getSharedToken<AttributeT>(),
            getSharedToken<OperationT>(), getSharedToken<BracketT>(), getSharedToken<ColumnTypeT>(), getSharedToken<DBTypeT>(), getSharedToken<StringT>()};

std::vector<std::shared_ptr<Token>> Tokenize(std::vector<std::string> inp, int ind) {
    std::vector<std::shared_ptr<Token>> tokens;
    while (ind < inp.size()) {
        for (std::shared_ptr<Token> t : tokenSeq) { // don't already want copy? TODO check
            if (t->Parse(inp, ind)) {
                tokens.push_back(t->Copy());
                break;
            }
        }
    }
    return tokens;
}

};

std::pair<std::string, std::string> ParseTableColumn(const std::string& s) {
    ssize_t dotInd= 0;
    if ((dotInd = s.find_first_of('.')) != s.find_last_of('.')) {
        // throw ex - names cant have .
        exit(-1);
    }

    if (dotInd != s.npos) {
        return {s.substr(dotInd), s.substr(dotInd + 1, static_cast<int>(s.size()) - static_cast<int>(dotInd) - 1)};
    }
    return {"", s};
}
}

/*#define COMMAND(name) {#name, Lexer::##name}


COMMAND(ABA),
COMMAND(ZZZ)*/