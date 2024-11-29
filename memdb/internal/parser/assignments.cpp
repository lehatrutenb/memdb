#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <memory>
#include <stack>
#include "../type/type.cpp"
#include "lexer.cpp"
#include "condition.cpp"

namespace memdb {
namespace parser {

struct Assignments {
    Assignments(){};

    struct ColumnExp {
        ColumnExp(std::pair<std::string, std::string> names_, Condition cond_) : TableColNames(names_), Exp(cond_){};
        std::pair<std::string, std::string> TableColNames;
        Condition Exp;
    };

    void Add(ColumnExp cexp) {
        As.emplace_back(cexp);
    }
    std::vector<ColumnExp> As;
};


/*
if (inp[l]->GetType() != Tokenizer::TokenT::BRACKET || inp[r]->GetType() != Tokenizer::TokenT::BRACKET) {
            return false;
        }
        if (getValue<Tokenizer::BracketT, lexer::Bracket>(inp[l]) != bo) {
            return false;
        }

*/
class AssignmentsParser {
public:
    Assignments Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r) {
        Assignments res;

        while (l < r) {
            if (inp[l]->GetType() != Tokenizer::TokenT::STRING) {
                // throw ex expected column name in assignment
                exit(-1);
            }

            std::string name = getValue<Tokenizer::StringT, std::string>(inp[l]);

            if (inp[l + 1]->GetType() != Tokenizer::TokenT::OPERATION || getValue<Tokenizer::OperationT, Operation>(inp[l+1]) != Operation::EQ) {
                // throw ex expected = operation in assignment
                exit(-1);
            }

            int commaInd = getNextToken<Tokenizer::OtherT, lexer::Other>(inp, l + 2, r, Tokenizer::TokenT::OTHER, lexer::Other::COMMA);
            if (commaInd == -1) {
                commaInd = r + 1;
                break;
            }

            Condition cond = parser::ConditionParser{}.Parse(inp, l + 2, commaInd - 1);
            res.Add(Assignments::ColumnExp(ParseTableColumn(name), cond));

            l = commaInd + 1;
        }

        return res;
    }
};

}
}