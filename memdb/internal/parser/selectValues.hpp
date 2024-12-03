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

struct SelectData {
public:
    std::vector<TableColumn> Data;
};

struct SelectValuesParser {
    SelectData Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r);
};

}
}