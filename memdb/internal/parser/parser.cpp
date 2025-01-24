#include <string_view>
#include <string>
#include <map>
#include <memory>
#include "parser.hpp"
#include <iostream>

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