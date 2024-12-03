#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <memory>
#include <stack>
#include "../type/type.hpp"
#include "../column/column.hpp"
#include "../table/table.hpp"
#include "lexer.hpp"

namespace memdb {
namespace parser {

struct InsertData {
public:
    std::vector<Table::Value> data;
};

struct InserValuesParser {
    bool checkBracketed(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r, lexer::Bracket bo, lexer::Bracket bc);

    InsertData Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r);
};

}
}