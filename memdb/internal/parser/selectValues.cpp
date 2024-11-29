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

struct SelectData {
public:
    std::vector<TableColumn> Data;
};

struct SelectValuesParser {
    SelectData Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r) {
        SelectData res;

        while (l < r) {
            if (inp[l]->GetType() != Tokenizer::TokenT::STRING) {
                // throw ex expected table.column in select
                exit(-1);
            }

            res.Data.emplace_back(TableColumn(std::move(ParseTableColumn(getValue<Tokenizer::StringT, std::string>(inp[l])))));

            if (l + 1 >= r) {
                break;
            }
            if (inp[l + 1]->GetType() != Tokenizer::TokenT::OTHER || getValue<Tokenizer::OtherT, lexer::Other>(inp[l + 1]) != lexer::Other::COMMA) {
                // throw ex - expect comma in select between columns names
                exit(-1);
            }

            l += 2;
        }
        return res;
    }
};

}
}