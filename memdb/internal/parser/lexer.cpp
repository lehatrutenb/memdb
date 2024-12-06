#include <string_view>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include <cctype>
#include "lexer.hpp"

namespace memdb {
namespace lexer {
    std::string AddSpace(const std::string_view& s, int& ind, bool& insideString, bool& insideLen, ssize_t r) { // care - change index itself
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
        if (isdigit(ch) || isalpha(ch) || ch == '.' || ch == '"' || ch == '_' || ch == '#' || ch == '@' || ch == ' ' || ch == '$' || ch == ';') {
            res.push_back(ch);
            return res;
        }
        res.push_back(' ');
        if (ind + 1 <= r && (ch == '|' && s[ind + 1] == '|')||(ch == '&' && s[ind + 1] == '&')||(ch == '^' && s[ind + 1] == '^')||((ch=='<'||ch=='>'||ch=='!')&&s[ind + 1]=='=')) {
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

    std::pair<bool, Other> IsOther(std::vector<std::string>& inp, int ind) {
        if (s2oth.find(inp[ind]) == s2oth.end()) {
            return {false, Other{}};
        }
        return {true, s2oth[inp[ind]]};
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
            return {false, ColumnType{Type::Empty}};
        }
        if (s2ct[inp[ind]] == Type::String || s2ct[inp[ind]] == Type::Bytes) {
            if (ind + 3 < inp.size()) {
                auto sz = IsInt(inp, ind + 2);
                if (IsBracket(inp, ind + 1).second == Bracket::RECTO && IsBracket(inp, ind + 3).second == Bracket::RECTC && sz.first == true) {
                    return {true, ColumnType(s2ct[inp[ind]], sz.second)};
                }
            }
            return {false, ColumnType{Type::Empty}};
        }
        return {true, ColumnType(s2ct[inp[ind]])};
    }

    std::pair<bool, std::string> IsString(std::vector<std::string>& inp, int ind) {
        if (inp[ind].front() == '"' && inp[ind].back() == '"') {
            return {true, inp[ind].substr(1, static_cast<int>(inp[ind].size()) - 2)};
        }
        return {false, {}};
    }

    std::pair<bool, std::vector<char>> IsBytes(std::vector<std::string>& inp, int ind) {
        if (inp[ind].size() >= 2 && inp[ind][0] == '0' && inp[ind][1] == 'x') {
            std::vector<char> v(inp[ind].size());
            v[0] = inp[ind][0];
            v[1] = inp[ind][1];
            for (int i = 2; i < inp[ind].size(); i++) {
                v[i] = inp[ind][i];
                if (('0' <= inp[ind][i] && inp[ind][i] <= '9')||('a' <= inp[ind][i] && inp[ind][i] <= 'f')) { // no isdigit here!
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



Tokenizer::CommandT::CommandT() {};
Tokenizer::CommandT::CommandT(lexer::Command t_) : t(t_){};
Tokenizer::TokenT Tokenizer::CommandT::GetType() const {
    return Tokenizer::TokenT::COMMAND;
}
bool Tokenizer::CommandT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind) {
    auto res = lexer::IsCommand(inpLowered, ind);
    if (res.first) {
        t = res.second;
        ind++;
    }
    return res.first;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::CommandT::Copy() {
    std::shared_ptr<Token> copy{new CommandT(t)};
    return copy;
}


Tokenizer::SubCommandT::SubCommandT(){};
Tokenizer::SubCommandT::SubCommandT(lexer::SubCommand t_) : t(t_){};
Tokenizer::TokenT Tokenizer::SubCommandT::GetType() const {
    return Tokenizer::TokenT::SUBCOMMAND;
}
bool Tokenizer::SubCommandT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind) {
    auto res = lexer::IsSubCommand(inpLowered, ind);
    if (res.first) {
        t = res.second;
        ind++;
    }
    return res.first;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::SubCommandT::Copy() {
    std::shared_ptr<Token> copy{new SubCommandT(t)};
    return copy;
}




Tokenizer::AttributeT::AttributeT(){};
Tokenizer::AttributeT::AttributeT(lexer::Attribute t_) : t(t_){};
Tokenizer::TokenT Tokenizer::AttributeT::GetType() const {
    return Tokenizer::TokenT::ATTRIBUTE;
}
bool Tokenizer::AttributeT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind) {
    auto res = lexer::IsAttribute(inpLowered, ind);
    if (res.first) {
        t = res.second;
        ind++;
    }
    return res.first;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::AttributeT::Copy() {
    std::shared_ptr<Token> copy{new AttributeT(t)};
    return copy;
}




Tokenizer::OtherT::OtherT(){};
Tokenizer::OtherT::OtherT(lexer::Other t_) : t(t_){};
Tokenizer::TokenT Tokenizer::OtherT::GetType() const  {
    return Tokenizer::TokenT::OTHER;
}
bool Tokenizer::OtherT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind)  {
    auto res = lexer::IsOther(inpLowered, ind);
    if (res.first) {
        t = res.second;
        ind++;
    }
    return res.first;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::OtherT::Copy()  {
    std::shared_ptr<Token> copy{new OtherT(t)};
    return copy;
}



Tokenizer::OperationT::OperationT(){};
Tokenizer::OperationT::OperationT(Operation t_) : t(t_){};
Tokenizer::TokenT Tokenizer::OperationT::GetType() const  {
    return Tokenizer::TokenT::OPERATION;
}
bool Tokenizer::OperationT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind)  {
    auto res = lexer::IsOperation(inp, ind);
    if (res.first) {
        t = res.second;
        ind++;
    }
    return res.first;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::OperationT::Copy()  {
    std::shared_ptr<Token> copy{new OperationT(t)};
    return copy;
}




Tokenizer::BracketT::BracketT(){};
Tokenizer::BracketT::BracketT(lexer::Bracket t_) : t(t_){};
Tokenizer::TokenT Tokenizer::BracketT::GetType() const  {
    return Tokenizer::TokenT::BRACKET;
}
bool Tokenizer::BracketT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind)  {
    auto res = lexer::IsBracket(inp, ind);
    if (res.first) {
        t = res.second;
        ind++;
    }
    return res.first;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::BracketT::Copy()  {
    std::shared_ptr<Token> copy{new BracketT(t)};
    return copy;
}



Tokenizer::ColumnTypeT::ColumnTypeT() : t(ColumnType{Type::Empty}){};
Tokenizer::ColumnTypeT::ColumnTypeT(ColumnType t_) : t(t_){};
Tokenizer::TokenT Tokenizer::ColumnTypeT::GetType() const  {
    return Tokenizer::TokenT::COLUMNTYPE;
}
bool Tokenizer::ColumnTypeT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind)  {
    auto res = lexer::IsColumnType(inpLowered, ind);
    if (res.first) {
        t = res.second;
        ind++;
        if (t.t == Type::String || t.t == Type::Bytes) {
            ind+=3;
        }
    }
    return res.first;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::ColumnTypeT::Copy()  {
    std::shared_ptr<Token> copy{new ColumnTypeT(t)};
    return copy;
}




Tokenizer::DBTypeT::DBTypeT(){};
Tokenizer::DBTypeT::DBTypeT(std::shared_ptr<DbType> t_) : t(t_){};
Tokenizer::TokenT Tokenizer::DBTypeT::GetType() const  {
    return Tokenizer::TokenT::DBTYPE;
}
bool Tokenizer::DBTypeT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind)  {
    {
        auto res = lexer::IsString(inp, ind);
        if (res.first) {
            std::shared_ptr<DbType> tmp{new DbString(res.second)};
            t.swap(tmp);
            ind++;
            return true;
        }
    }
    {
        auto res = lexer::IsBytes(inp, ind);
        if (res.first) {
            std::shared_ptr<DbType> tmp{new DbBytes(res.second)};
            t.swap(tmp);
            ind++;
            return true;
        }
    }
    {
        auto res = lexer::IsInt(inp, ind);
        if (res.first) {
            std::shared_ptr<DbType> tmp{new DbInt32(res.second)};
            t.swap(tmp);
            ind++;
            return true;
        }
    }
    {
        auto res = lexer::IsBool(inp, ind);
        if (res.first) {
            std::shared_ptr<DbType> tmp{new DbBool(res.second)};
            t.swap(tmp);
            ind++;
            return true;
        }
    }
    return false;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::DBTypeT::Copy()  {
    std::shared_ptr<Tokenizer::Token> copy{new DBTypeT(t)};
    return copy;
}



Tokenizer::StringT::StringT(){};
Tokenizer::StringT::StringT(std::string t_) : t(t_){};
Tokenizer::TokenT Tokenizer::StringT::GetType() const  {
    return Tokenizer::TokenT::STRING;
}
bool Tokenizer::StringT::Parse(std::vector<std::string>& inp, std::vector<std::string>& inpLowered, int& ind)  {
    t = inp[ind];
    ind++;
    return true;
}

std::shared_ptr<Tokenizer::Token> Tokenizer::StringT::Copy()  {
    std::shared_ptr<Token> copy{new StringT(t)};
    return copy;
}


std::vector<std::shared_ptr<Tokenizer::Token>> Tokenizer::Tokenize(std::vector<std::string> inp, int ind) {
    std::vector<std::string> inpLowered;
    inpLowered.reserve(inp.size());
    for (std::string s: inp) {
        for (char& ch : s) {
            ch = tolower(ch);
        }
        inpLowered.emplace_back(s);
    }
    std::vector<std::shared_ptr<Tokenizer::Token>> tokens;
    while (ind < inp.size()) {
        for (std::shared_ptr<Tokenizer::Token> t : tokenSeq) { // don't already want copy? TODO check
            if (t->Parse(inp, inpLowered, ind)) {
                tokens.push_back(t->Copy());
                break;
            }
        }
    }
    return tokens;
}


namespace parser {

std::pair<std::string, std::string> ParseTableColumn(const std::string& s) {
    ssize_t dotInd= 0;
    if ((dotInd = s.find_first_of('.')) != s.find_last_of('.')) {
        // throw ex - names cant have .
        throw std::runtime_error("error");
        exit(-1);
    }

    if (dotInd != s.npos) {
        return {s.substr(dotInd), s.substr(dotInd + 1, static_cast<int>(s.size()) - static_cast<int>(dotInd) - 1)};
    }
    return {"", s};
}

int TokenStructure::getCmd(lexer::Command cmd) {
    if (C2i.find(cmd) == C2i.end()) {
        return -1;
    }
    return C2i[cmd];
}

int TokenStructure::getScmd(lexer::SubCommand cmd) {
    if (Sc2i.find(cmd) == Sc2i.end()) {
        return -1;
    }
    return Sc2i[cmd];
}

TokenStructure::TokenStructure(const std::vector<std::shared_ptr<Tokenizer::Token>>&& tokens, ssize_t l, ssize_t r) : Tokens(std::move(tokens)) {
    for (int i = l; i <= r; ++i) {
        if (Tokens[i]->GetType() == Tokenizer::TokenT::COMMAND) {
            auto cmd = getValue<Tokenizer::CommandT, lexer::Command>(Tokens[i]);
            Commands.push_back({cmd, i});
            C2i[cmd] = i;
        } else if (Tokens[i]->GetType() == Tokenizer::TokenT::SUBCOMMAND) {
            auto subCmd = getValue<Tokenizer::SubCommandT, lexer::SubCommand>(Tokens[i]);
            SubCommands.push_back({subCmd, i});
            Sc2i[subCmd] = i;
        }
    }
}


}
}