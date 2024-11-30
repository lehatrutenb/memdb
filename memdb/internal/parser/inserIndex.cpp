#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <memory>
#include <stack>
#include "../type/type.cpp"
#include "../column/column.cpp"
#include "../table/table.cpp"
#include "lexer.cpp"

namespace memdb {
namespace parser {

struct InsertData {
public:
    std::vector<Table::Value> data;
};

struct InserValuesParser {
    bool checkBracketed(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r, lexer::Bracket bo, lexer::Bracket bc) {
        if (l >= r) {
            return false;
        }
        if (inp[l]->GetType() != Tokenizer::TokenT::BRACKET || inp[r]->GetType() != Tokenizer::TokenT::BRACKET) {
            return false;
        }
        if (getValue<Tokenizer::BracketT, lexer::Bracket>(inp[l]) != bo) {
            return false;
        }
        if (getValue<Tokenizer::BracketT, lexer::Bracket>(inp[r]) != bc) {
            return false;
        }
        return true;
    }

    InsertData Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r) {
        InsertData res;
        if (!checkBracketed(inp, l, r, lexer::Bracket::CIRCLEO, lexer::Bracket::CIRCLEC)) {
            // throw ex - expected brackets
            throw std::runtime_error("error");
            exit(-1);
        }
        if (l + 1 == r) {
            return res;
        }

        l++;
        r--;

        bool skipComma = false;
        while (l <= r) {
            switch(inp[l]->GetType()) {
                case Tokenizer::TokenT::OTHER:
                    if (!skipComma || l == r) { // l == r eq to abacaba,)
                        res.data.emplace_back(Table::Value(std::shared_ptr<DbType>{new DbTypeEmpty{}}));
                    }
                    l++;
                    skipComma = false;
                    break;
                case Tokenizer::TokenT::DBTYPE:
                    res.data.emplace_back(Table::Value(getValue<Tokenizer::DBTypeT, std::shared_ptr<DbType>>(inp[l])));
                    l++;
                    skipComma = true;
                    break;
                case Tokenizer::TokenT::STRING:
                    if (l + 2 > r || inp[l + 1]->GetType() != Tokenizer::TokenT::OPERATION || inp[l + 2]->GetType() != Tokenizer::TokenT::DBTYPE) {
                        // throw ex incorrect data in column insert
                        throw std::runtime_error("error");
                        exit(-1);
                    }
                    if (getValue<Tokenizer::OperationT, Operation>(inp[l + 1]) != Operation::EQ) {
                        // throw ex incorrect data in column insert - exp = between
                        throw std::runtime_error("error");
                        exit(-1);
                    }

                    res.data.emplace_back(Table::Value(getValue<Tokenizer::StringT, std::string>(inp[l]),
                                                       getValue<Tokenizer::DBTypeT, std::shared_ptr<DbType>>(inp[l + 2])));
                    l += 3;
                    skipComma = true;
                    break;
                default:
                    //throw ex - unexp type in insert columt data
                    throw std::runtime_error("error");
                    exit(-1);
            }
        }
        return res;
    }
};

}
}