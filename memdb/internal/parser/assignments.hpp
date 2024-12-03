#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <memory>
#include <stack>
#include "../type/type.hpp"
#include "lexer.hpp"
#include "condition.hpp"

namespace memdb {
namespace parser {

struct Assignments {
    Assignments();

    struct ColumnExp {
        ColumnExp(std::pair<std::string, std::string> names_, Condition cond_);
        std::pair<std::string, std::string> TableColNames;
        Condition Exp;
    };

    void Add(ColumnExp cexp);
    std::vector<ColumnExp> As;
};

class AssignmentsParser {
public:
    Assignments Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r);
};

}
}