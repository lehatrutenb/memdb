#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <memory>
#include <stack>
#include "selectValues.hpp"

namespace memdb {
namespace parser {


SelectData SelectValuesParser::Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r) {
    SelectData res;

    while (l <= r) {
        if (inp[l]->GetType() != Tokenizer::TokenT::STRING) {
            // throw ex expected table.column in select
            throw std::runtime_error("error");
            exit(-1);
        }

        res.Data.emplace_back(TableColumn(std::move(ParseTableColumn(getValue<Tokenizer::StringT, std::string>(inp[l])))));

        if (l + 1 >= r) {
            break;
        }
        if (inp[l + 1]->GetType() != Tokenizer::TokenT::OTHER || getValue<Tokenizer::OtherT, lexer::Other>(inp[l + 1]) != lexer::Other::COMMA) {
            // throw ex - expect comma in select between columns names
            throw std::runtime_error("error");
            exit(-1);
        }

        l += 2;
    }
    return res;
}

}
}