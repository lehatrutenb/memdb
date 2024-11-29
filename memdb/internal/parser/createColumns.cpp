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
#include "lexer.cpp"

namespace memdb {
namespace parser {

struct ColumnDescriptions {
public:
    std::vector<ColumnFullDescription> info;
};

struct ColumnParser {
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
    ColumnDescriptions Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r) {
        ColumnDescriptions res;
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

        while (l <= r) {
            res.info.push_back(parseOne(inp, l, r));
        }
        return res;
    }

    std::vector<ColumnAttrs> parseAttrs(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r) { // TODO change columnattrs to just value
        if (l > r) {
            // throw exp - failed to parse column attrs
            throw std::runtime_error("error");
            exit(-1);
        }
        std::vector<ColumnAttrs> res;
        for (int i = l + 1; i <= r; i++) {
            if (checkBracketed(inp, l, i, lexer::Bracket::FIGUREO, lexer::Bracket::FIGUREC)) {
                for (int j = l + 1; j < i; j++) {
                    if (inp[j]->GetType() == Tokenizer::TokenT::OTHER && getValue<Tokenizer::OtherT, lexer::Other>(inp[j]) == lexer::Other::COMMA) {
                        continue;
                    } else if (inp[j]->GetType() == Tokenizer::TokenT::ATTRIBUTE) {
                        switch (getValue<Tokenizer::AttributeT, lexer::Attribute>(inp[j])) {
                            case lexer::Attribute::AUTOINCREMENT:
                                res.push_back(ColumnAttrs::Autoincrement);
                                break;
                            case lexer::Attribute::KEY:
                                res.push_back(ColumnAttrs::Key);
                                break;
                            case lexer::Attribute::UNIQUE:
                                res.push_back(ColumnAttrs::Unique);
                                break;
                            default:
                                // throw ex - unexp attr in column attrs;
                                throw std::runtime_error("error");
                                exit(-1);
                        }
                    } else {
                        // throw ex - not exp value in cloumn attributes
                        throw std::runtime_error("error");
                        exit(-1);
                    }
                }
                l = i + 1;
                break;
            }
        }

        return res;
    }

    std::pair<bool, std::pair<std::string, ColumnType>> parseNameType(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r) {
        if (l == r) {
            return {false, {"", ColumnType(Type::Empty)}};
        }
        if (l > r || l + 2 > r) {
            // throw exp - failed to parse column attrs
            throw std::runtime_error("error");
            exit(-1);
        }

        if (inp[l]->GetType() != Tokenizer::TokenT::STRING) {
            // throw exp - expected name of column column attrs
            throw std::runtime_error("error");
            exit(-1);
        }
        if (inp[l + 1]->GetType() != Tokenizer::TokenT::OTHER || getValue<Tokenizer::OtherT, lexer::Other>(inp[l + 1]) != lexer::Other::TWOPOINT) {
            // throw exp - expected name of column column attrs
            throw std::runtime_error("error");
            exit(-1);
        }
        if (inp[l + 2]->GetType() != Tokenizer::TokenT::COLUMNTYPE) {
            // throw exp - expected name of column column attrs
            throw std::runtime_error("error");
            exit(-1);
        }
        l += 3;

        return {true, {getValue<Tokenizer::StringT, std::string>(inp[l - 3]), getValue<Tokenizer::ColumnTypeT, ColumnType>(inp[l - 1])}};
    }

    std::pair<bool, std::shared_ptr<DbType>> parseDefaultValue(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r) {
        if (l + 1 > r) {
            return {false, {}};
        }
        if (inp[l]->GetType() != Tokenizer::TokenT::OPERATION || getValue<Tokenizer::OperationT, Operation>(inp[l]) != Operation::EQ) {
            return {false, {}};
        }

        if (inp[l + 1]->GetType() != Tokenizer::TokenT::DBTYPE) {
            // throw ex - def value not found
            throw std::runtime_error("error");
        }

        l += 2;

        return {true, getValue<Tokenizer::DBTypeT, std::shared_ptr<DbType>>(inp[l - 1])};
    }

    ColumnFullDescription parseOne(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t& l, ssize_t r) {
        auto colAttrs = parseAttrs(inp, l, r);
        auto nameType = parseNameType(inp, l, r);
        if (!nameType.first) {
            // throw ex - invalid column init
            throw std::runtime_error("error");
            exit(-1);
        }

        auto defVal = parseDefaultValue(inp, l, r);
        l++; // comma at the end
        if (!defVal.first) {
            return ColumnFullDescription(nameType.second.second, ColumnDescription(colAttrs, nameType.second.first));
        }

        if (defVal.second->getType() != nameType.second.second.t) {
            // throw ex - default value type is incorrect
            throw std::runtime_error("error");
            exit(-1);
        }

        return ColumnFullDescription(nameType.second.second, ColumnDescription(colAttrs, nameType.second.first, defVal.second));
    }
};

}
}