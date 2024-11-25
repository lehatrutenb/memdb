#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "../type/type.cpp"

namespace memdb {
namespace parser {

enum class Command {
    CREATE,
    INSERT,
    SELECT,
    UPDATE,
    DELETE,
    JOIN
};

enum class SubCommand {
    WHERE,
    ON,
    SET,
    FROM,
    INDEX
};

enum class Attribute {
    UNIQUE,
    AUTOINCREMENT,
    KEY,
    ORDERED,
    UNORDERED
};

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
    Condition() {
        isLeaf = true;
        std::shared_ptr<DbType> tr;
        tr.reset(dynamic_cast<DbType*>(new DbBool(true)));
        leaf = std::make_shared<ConditionLeaf>(ConditionLeaf(tr));
    }
    Condition(std::shared_ptr<ConditionLeaf> leaf_) : isLeaf(true), leaf(leaf_) {}
    Condition(Operation op, std::shared_ptr<Condition> left_, std::shared_ptr<Condition> right_) : isLeaf(false), left(left_), right(right_) {}

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
            leaf->val = std::make_shared<DbType>(vals[{leaf->table, leaf->column}]); // copy cause want to change later (in compute)
            return;
        }
        if (left) {
            left->SetWhatNeed(vals);
        }
        if (right) {
            right->SetWhatNeed(vals);
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

        std::shared_ptr<DbType> resLeft, resRight;
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
            int res = DbType::DoOp(op, left->leaf->val, right->leaf->val);
            if (IsMathOp(op)) {
                return left->leaf->val;
            } else if (op != Operation::LEN) {
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
        }
    }

    // getWhatNeed and setWhatNeed not crush logic with || cause talbe will return empty values if can't get and they will be checked in compute

    Operation op;
    bool isLeaf;
    std::shared_ptr<Condition> left;
    std::shared_ptr<Condition> right;

    std::shared_ptr<ConditionLeaf> leaf;

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
        if (left) {
            left->getWhatNeed(res);
        }
        if (right) {
            right->getWhatNeed(res);
        }
    }
};

struct PedObject {
    Command cmd;
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

}
}