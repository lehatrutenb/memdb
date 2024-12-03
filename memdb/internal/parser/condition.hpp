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

namespace memdb {
namespace parser {

// just big class that may do all not to do lots of them // it's not bad in that task cause it'll have no more than 1-2 empty fields

struct ConditionLeaf {
    ConditionLeaf(const std::string& table_, const std::string& column_);
    ConditionLeaf(std::shared_ptr<DbType> val_);

    std::string table;
    std::string column;

    bool hasVal = false;
    std::shared_ptr<DbType> val;
};


struct Condition {
    Operation Op;
    bool isLeaf;
    std::shared_ptr<Condition> leftCond;
    std::shared_ptr<Condition> rightCond;

    std::shared_ptr<ConditionLeaf> leaf;

    Condition();
    Condition(std::shared_ptr<ConditionLeaf> leaf_);
    Condition(Operation op, std::shared_ptr<Condition> left_, std::shared_ptr<Condition> right_);
    Condition(Operation op, std::shared_ptr<Condition> left_);

    std::vector<TableColumn> GetWhatNeed() const;

    std::shared_ptr<DbType> getLeafVal(std::map<TableColumn, std::shared_ptr<DbType>>& vals) const;

    std::shared_ptr<DbType> Compute(std::map<TableColumn, std::shared_ptr<DbType>>& vals) const;

    static inline std::vector<std::vector<Operation>> opPrior = {{Operation::NOT, Operation::LEN}, {Operation::MULTIPLY, Operation::DIVIDE, Operation::MOD}, 
                                                          {Operation::PLUS, Operation::MINUS},
                                                          {Operation::LESS, Operation::MORE, Operation::EQLESS, Operation::EQMORE},
                                                          {Operation::EQ, Operation::NOTEQ},
                                                          {Operation::XOR}, {Operation::AND}, {Operation::OR}};
    
    static bool hasPrior(Operation op, int i);

    // getWhatNeed and setWhatNeed not crush logic with || cause talbe will return empty values if can't get and they will be checked in compute

    private:
    void getWhatNeed(std::vector<TableColumn>& res) const;
};

struct PedObject {
    lexer::Command cmd;
    std::string table1 = "";
    std::string table2 = "";
    Condition cond = Condition();
};

struct Object {
    virtual void read(const std::vector<std::string_view>& inp, int& ind) = 0;
    void readk(const std::vector<std::string_view>& inp, int& ind, int k);
};

struct ConditionT : public Tokenizer::Token {
    ConditionT() {};
    ConditionT(Condition t_);
    Tokenizer::TokenT GetType() const override;
    bool Parse(std::vector<std::string>& inp, int& ind) override;

    std::shared_ptr<Token> Copy();

    Condition t;
};

struct ConditionParser {
    Condition Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r);
};

}
}