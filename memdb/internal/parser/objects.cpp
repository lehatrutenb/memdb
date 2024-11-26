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

namespace memdb {
namespace parser {

// just big class that may do all not to do lots of them // it's not bad in that task cause it'll have no more than 1-2 empty fields

struct ConditionLeaf {
    ConditionLeaf(const std::string& table_, const std::string& column_) : table(table_), column(column_), hasVal(false){}
    ConditionLeaf(std::shared_ptr<DbType> val_) : val(val_), hasVal(true){}

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

    Condition() {
        isLeaf = true;
        std::shared_ptr<DbType> tr;
        tr.reset(dynamic_cast<DbType*>(new DbBool(true)));
        leaf = std::make_shared<ConditionLeaf>(ConditionLeaf(tr));
    }
    Condition(std::shared_ptr<ConditionLeaf> leaf_) : isLeaf(true), leaf(leaf_) {}
    Condition(Operation op, std::shared_ptr<Condition> left_, std::shared_ptr<Condition> right_) : isLeaf(false), leftCond(left_), rightCond(right_), Op(op) {}
    Condition(Operation op, std::shared_ptr<Condition> left_) : isLeaf(false), leftCond(left_), rightCond({}), Op(op) {}

    std::vector<std::pair<std::string, std::string>> SetWhatNeed() const { // pair not to write struct currently TODO
        std::vector<std::pair<std::string, std::string>> res;
        getWhatNeed(res);
        std::sort(res.begin(), res.end());
        res.erase(std::unique(res.begin(), res.end()), res.end()); // not to get from db twice
        return res;
    }

    void SetWhatNeed(std::map<std::pair<std::string, std::string>, std::shared_ptr<DbType>>& vals) {
        if (isLeaf) {
            if (leaf->hasVal) {
                return;
            }
            if (vals.find({leaf->table, leaf->column}) == vals.end()) {
                return;
            }
            leaf->hasVal = true;
            leaf->val = vals[{leaf->table, leaf->column}];
            return;
        }
        if (leftCond) {
            leftCond->SetWhatNeed(vals);
        }
        if (rightCond) {
            rightCond->SetWhatNeed(vals);
        }
    }

    std::shared_ptr<DbType> Compute() {
        if (isLeaf) {
            if (!leaf->hasVal) {
                // throw ex - val should be on that moment
                exit(-1);
            }
            return leaf->val;
        }

        if (IsMathOp(Op)) {
            if (leftCond && rightCond) {
                auto leftRes = leftCond->Compute()->Copy();
                DoOp(Op, leftRes, rightCond->Compute()); // will update left
                return leftRes;
            } else {
                // throw ex - 2 operands op
                exit(-1);
            }
        }

        if (IsCmpOp(Op)) {
            if (leftCond && rightCond) {
                std::shared_ptr<DbType> resObj;
                resObj.reset(dynamic_cast<DbType*>(new DbBool(DoOp(Op, leftCond->Compute(), rightCond->Compute()))));
                return resObj;
            } else {
                // throw ex - 2 operands op
                exit(-1);
            }
        }

        if (Op == Operation::LEN) {
            if (rightCond) {
                std::shared_ptr<DbType> resObj;
                auto rightRes = rightCond->Compute();
                resObj.reset(dynamic_cast<DbType*>(new DbInt32(DoOp(Op, rightRes, rightRes))));
                return resObj;
            } else {
                // throw ex - 1 operand op
                exit(-1);
            }
        }

        if (IsBoolOp(Op)) {
            if (Op == Operation::NOT) {
                std::shared_ptr<DbType> resObj;
                auto rightRes = rightCond->Compute();
                resObj.reset(dynamic_cast<DbType*>(new DbBool(DoOp(Op, rightRes, rightRes))));
                return resObj;
            } else {
                if (!leftCond || !rightCond) {
                    // throw ex - 2 op op
                    exit(-1);
                }
                std::shared_ptr<DbType> resObj;
                resObj.reset(dynamic_cast<DbType*>(new DbBool(DoOp(Op, leftCond->Compute(), rightCond->Compute()))));
                return resObj;
            }
        }

        // throw ex - unexp op
        exit(-1);

        /*std::shared_ptr<DbType> resLeft, resRight;
        if (left) {
            resLeft = left->Compute();
        }
        if (right) {
            resRight = right->Compute();
        }

        if (!left) {
            swap(resLeft, resRight);
        }
        if (left && right) {
            int res = DbType::DoOp(Op, left->leaf->val, right->leaf->val);
            if (IsMathOp(Op)) {
                return left->leaf->val;
            } else if (Op != Operation::LEN) {
                std::shared_ptr<DbType> resObj;
                resObj.reset(dynamic_cast<DbType*>(new DbBool(res)));
                return resObj;
            } else {
                std::shared_ptr<DbType> resObj;
                resObj.reset(dynamic_cast<DbType*>(new DbInt32(res)));
                return resObj;
            }
        } else {
            return resLeft;
        }*/
    }

    static inline std::vector<std::vector<Operation>> opPrior = {{Operation::NOT, Operation::LEN}, {Operation::MULTIPLY, Operation::DIVIDE, Operation::MOD}, 
                                                          {Operation::PLUS, Operation::MINUS},
                                                          {Operation::LESS, Operation::MORE, Operation::EQLESS, Operation::EQMORE},
                                                          {Operation::EQ, Operation::NOTEQ},
                                                          {Operation::XOR}, {Operation::AND}, {Operation::OR}};
    
    static bool hasPrior(Operation op, int i) {
        return std::count(opPrior[i].begin(), opPrior[i].end(), op);
    }

    // getWhatNeed and setWhatNeed not crush logic with || cause talbe will return empty values if can't get and they will be checked in compute

    private:
    void getWhatNeed(std::vector<std::pair<std::string, std::string>>& res) const {
        if (isLeaf) {
            if (leaf->hasVal) {
                return;
            }
            if (leaf->column != "") { // otherwise it is const or already set or ... // table == "" is ok if just 1 table given
                res.push_back({leaf->table, leaf->column}); // that is inside object to it is safe
            } else {
                // throw ex - column must be set
                exit(-1);
            }
            return;
        }
        if (leftCond) {
            leftCond->getWhatNeed(res);
        }
        if (rightCond) {
            rightCond->getWhatNeed(res);
        }
    }
};

struct PedObject {
    lexer::Command cmd;
    std::string table1 = "";
    std::string table2 = "";
    Condition cond = Condition();
};

struct Object {
    virtual void read(const std::vector<std::string_view>& inp, int& ind) = 0;
    void readk(const std::vector<std::string_view>& inp, int& ind, int k) {
        for (int i = 0; i < k; i++) {
            read(inp, ind);
        }
    }
};

struct ConditionT : public Tokenizer::Token {
    ConditionT() {};
    ConditionT(Condition t_) : t(t_){};
    Tokenizer::TokenT GetType() const override {
        return Tokenizer::TokenT::CONDITION;
    }
    bool Parse(std::vector<std::string>& inp, int& ind) override {
        // throw ex - unexp call
        exit(-1);
    }

    std::shared_ptr<Token> Copy() {
        std::shared_ptr<Token> copy;
        copy.reset(dynamic_cast<Token*>(new ConditionT(t)));
        return copy;
    }

    Condition t;
};

struct ConditionParser {
    Condition Parse(const std::vector<std::shared_ptr<Tokenizer::Token>>& inp, ssize_t l, ssize_t r) {
        if (l == r) {
            if (inp[l]->GetType() == Tokenizer::TokenT::STRING) {
                auto parsed = ParseTableColumn(dynamic_cast<const Tokenizer::StringT*>(inp[l].get())->t);
                std::shared_ptr<ConditionLeaf> res = std::make_shared<ConditionLeaf>(ConditionLeaf(parsed.first, parsed.second));
                return Condition(res);
            } else if (inp[l]->GetType() == Tokenizer::TokenT::DBTYPE) {
                return Condition(std::make_shared<ConditionLeaf>(ConditionLeaf(dynamic_cast<const Tokenizer::DBTypeT*>(inp[l].get())->t)));
            }  else if (inp[l]->GetType() == Tokenizer::TokenT::CONDITION) {
                return dynamic_cast<const ConditionT*>(inp[l].get())->t;
            }

            // throw ex - token should be name.column or dbtype
            exit(-1);
        }

        // open all 
        std::vector<std::shared_ptr<Tokenizer::Token>> curTokens;
        for (int i = l; i <= r; i++) {
            std::shared_ptr<Tokenizer::Token> t = inp[i];
            if (t->GetType() == Tokenizer::TokenT::BRACKET) {
                auto tp = dynamic_cast<const Tokenizer::BracketT*>(inp[i].get())->t;
                if (tp == lexer::Bracket::CIRCLEC || tp == lexer::Bracket::LENC) {
                    int left = static_cast<int>(curTokens.size()) - 1, right = left;
                    while (left >= l) {
                        if (curTokens[left]->GetType() == Tokenizer::TokenT::BRACKET) {
                            tp = dynamic_cast<const Tokenizer::BracketT*>(curTokens[left].get())->t;
                            if (tp == lexer::Bracket::CIRCLEO || tp == lexer::Bracket::LENO) {
                                break;
                            }
                        }
                        left--;
                    }
                    if (left < l) {
                        // throw ex - not found opend bracet for close one
                        exit(-1);
                    }
                    Condition c = Parse(curTokens, left + 1, right); // cond inside brackets
                    while (right >= left) {
                        curTokens.pop_back();
                        right--;
                    }

                    if (tp == lexer::Bracket::CIRCLEO) {
                        curTokens.push_back(Tokenizer::getSharedToken<ConditionT>(new ConditionT(c)));
                    } else {
                        auto shCond = std::make_shared<Condition>(c);
                        curTokens.push_back(Tokenizer::getSharedToken<ConditionT>(new ConditionT(Condition(Operation::LEN, shCond))));
                    }
                } else {
                    curTokens.emplace_back(t);
                }
            } else {
                curTokens.emplace_back(t);
            }
        }

        while (true) {
            ssize_t lastSize = curTokens.size();
            for (int pr = 0; pr < Condition::opPrior.size(); pr++) {
                std::vector<std::shared_ptr<Tokenizer::Token>> curTokensNxt;
                for (int i = 0; i < curTokens.size(); i++) {
                    auto cur = curTokens[i];
                    if (cur->GetType() == Tokenizer::TokenT::OPERATION) {
                        Operation curOp = dynamic_cast<const Tokenizer::OperationT*>(curTokens[i].get())->t;
                        if (Condition::hasPrior(curOp, pr)) {
                            if (i + 1 >= curTokens.size() || curTokensNxt.empty()) {
                                // throw ex - not enough args per operation
                                exit(-1);
                            }
                            curTokensNxt.emplace_back(curTokens[i + 1]);
                            i++;
                            Condition c1 = Parse(curTokensNxt, curTokensNxt.size() - 2, curTokensNxt.size() - 2);
                            Condition c2 = Parse(curTokensNxt, curTokensNxt.size() - 1, curTokensNxt.size() - 1);
                            curTokensNxt.pop_back();
                            curTokensNxt.pop_back();
                            curTokensNxt.emplace_back(Tokenizer::getSharedToken<ConditionT>(new ConditionT(Condition(curOp, std::make_shared<Condition>(c1), std::make_shared<Condition>(c2)))));
                        } else {
                            curTokensNxt.emplace_back(curTokens[i]);
                        }
                    } else {
                        curTokensNxt.emplace_back(curTokens[i]);
                    }
                }
                swap(curTokens, curTokensNxt);
            }

            if (lastSize == curTokens.size()) { // nothing changed so break
                break;
            }
        }
        if (curTokens.size() != 1) {
            // throw ex - invalid condtion
            exit(-1);
        }
        return Parse(curTokens, 0, 0);
    }
};

}
}

/*Condition Parse(const std::vector<Tokenizer::Token>& inp, ssize_t& l, ssize_t& r) {
        std::deque<Tokenizer::Token> tokens;
        for (auto t : inp) {
            tokens.push_back(t);
        }
        for (int pr = 0; pr < opPrior.size(); pr++) {
            std::deque<Tokenizer::Token> tokensNxt;
            for (int i = 0; i < tokens.size(); i++) {
                auto cur = inp[i];
                switch (cur.GetType()) {
                case Tokenizer::TokenT::OPERATION:
                    if (!hasPrior(op, pr)) {
                        tokensNxt.push(cur);
                    }

                    break;
                case Tokenizer::TokenT::DBTYPE:
                    break;
                case Tokenizer::TokenT::STRING:
                    break;
                default:
                    //throw ex - unexp token inside condition
                    exit(-1);
                }
            }
            tokens = tokensNxt;
        }
    }*/