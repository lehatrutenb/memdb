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
#include "lexer.hpp"

namespace memdb {
namespace parser {

struct ColumnDescriptions {
public:
    std::vector<ColumnFullDescription> info;
};

struct ColumnParser {
    bool checkBracketed(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r, lexer::Bracket bo, lexer::Bracket bc);
    ColumnDescriptions Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r);

    std::vector<ColumnAttrs> parseAttrs(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r);

    std::pair<bool, std::pair<std::string, ColumnType>> parseNameType(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r);

    std::pair<bool, std::shared_ptr<DbType>> parseDefaultValue(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r);

    ColumnFullDescription parseOne(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r);
};

}
}