#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <forward_list>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include "type.hpp"

namespace memdb {

bool IsBoolOp(Operation op) {
    static std::vector<Operation> bOps = {Operation::AND, Operation::OR, Operation::NOT, Operation::XOR};
    return (std::find(bOps.begin(), bOps.end(), op) != bOps.end());
}

bool IsMathOp(Operation op) {
    static std::vector<Operation> mOps = {Operation::PLUS, Operation::MINUS, Operation::MULTIPLY, Operation::DIVIDE, Operation::MOD};
    return (std::find(mOps.begin(), mOps.end(), op) != mOps.end());
}

bool IsCmpOp(Operation op) {
    static std::vector<Operation> cOps = {Operation::EQ, Operation::LESS, Operation::MORE, Operation::EQLESS, Operation::EQMORE, Operation::NOTEQ};
    return (std::find(cOps.begin(), cOps.end(), op) != cOps.end());
}

bool IsNonDefCmpOp(Operation op) {
    static std::vector<Operation> cOps = {Operation::EQ, Operation::EQLESS, Operation::EQMORE, Operation::NOTEQ};
    return (std::find(cOps.begin(), cOps.end(), op) != cOps.end());
}


Type DbTypeEmpty::getType() {
    return Type::Empty;
}
std::shared_ptr<DbType> DbTypeEmpty::Copy() {
    return  {};
};



DbInt32::DbInt32(int x_) : x(x_){};
Type DbInt32::getType() {
    return Type::Int32;
}
int32_t DbInt32::get() {
    return x;
}

std::shared_ptr<DbType> DbInt32::Copy() {
    std::shared_ptr<DbType> copy{new DbInt32(x)};
    return copy;
}

bool DbInt32::doOp(Operation op, const DbInt32* another) { // update cur
    switch (op)
    {
    case Operation::PLUS:
        x += another->x;
        break;
    case Operation::MINUS:
        x -= another->x;
        break;
    case Operation::MULTIPLY:
        x *= another->x;
        break;
    case Operation::DIVIDE:
        if (another->x == 0) {
            // throw ex divide by 0
            throw std::runtime_error("error");
            exit(-1);
        }
        x /= another->x;
        break;
    case Operation::MOD:
    if (another->x == 0) {
            // throw ex MOD 0
            throw std::runtime_error("error");
            exit(-1);
        }
        x %= another->x;
        break;
    case Operation::LESS:
        return (x < another->x);
        break;
    case Operation::MORE:
        return (x > another->x);
        break;
    default:
        // throw ex - bad op
        throw std::runtime_error("error");
        exit(-1);
    }
    return false;
}



DbBool::DbBool(bool x_) : x(x_){};
Type DbBool::getType() {
    return Type::Bool;
}
bool DbBool::get() {
    return x;
}

std::shared_ptr<DbType> DbBool::Copy() {
    std::shared_ptr<DbType> copy{new DbBool(x)};
    return copy;
}

bool DbBool::doOp(Operation op, const DbBool* another) { // update cur
    switch (op) {
    case Operation::EQ:
        return x == another->x;
    case Operation::NOTEQ:
        return x != another->x;
    case Operation::AND:
        return x && another->x;
    case Operation::OR:
        return x || another->x;
    case Operation::NOT:
        throw std::runtime_error("error UNIMPLEMENTED"); // TODO
        exit(-1);
    case Operation::XOR:
        return (x ^ another->x);
    default:
        // throw ex - bad op
        throw std::runtime_error("error");
        exit(-1);
    }
    return true;
}



DbBytes::DbBytes(std::vector<char> v_) : x(v_){};
std::vector<char> DbBytes::get() {
    return x;
}
Type DbBytes::getType() {
    return Type::Bytes;
}

std::shared_ptr<DbType> DbBytes::Copy() {
    std::shared_ptr<DbType> copy{new DbBytes(x)};
    return copy;
}

int DbBytes::doOp(Operation op, const DbBytes* another) { // update cur
    switch (op) {
    case Operation::EQ:
        return x == another->x;
    case Operation::NOTEQ:
        return x != another->x;
    case Operation::LEN:
        return x.size() - 2; // 0x
    default:
        // throw ex - bad op
        throw std::runtime_error("error");
        exit(-1);
    }
    return 1;
}




DbString::DbString(std::string s_) : x(s_){};
std::string DbString::get() {
    return x;
}
Type DbString::getType() {
    return Type::String;
}

std::shared_ptr<DbType> DbString::Copy() {
    std::shared_ptr<DbType> copy{new DbString(x)};
    return copy;
}

int DbString::doOp(Operation op, const DbString* another) { // update cur
    switch (op) {
    case Operation::EQ:
        return x == another->x;
    case Operation::NOTEQ:
        return x != another->x;
    case Operation::PLUS:
        x += another->x;
        return 1;
    case Operation::LEN:
        return x.size();
    default:
        // throw ex - bad op
        throw std::runtime_error("error");
        exit(-1);
    }
    return 1;
}




int DoOp(Operation op, std::shared_ptr<DbType> left, std::shared_ptr<DbType> right) {
    if (left->getType() != right->getType()) {
        // throw ex - types must be eq
        throw std::runtime_error("error");
        exit(-1);
    }
    if (left->getType() == Type::Empty) {
        // throw ex - types must be not empty
        throw std::runtime_error("error");
        exit(-1);
    }

    if (left->getType() != Type::String && left->getType() != Type::Bytes && IsNonDefCmpOp(op)) {
        switch (op)
        {
        case Operation::EQ:
            return DoOp(Operation::LESS, left, right) == 0 && DoOp(Operation::MORE, left, right) == 0;
        case Operation::NOTEQ:
            return DoOp(Operation::LESS, left, right) == 1 || DoOp(Operation::MORE, left, right) == 1;
        case Operation::EQLESS:
            return DoOp(Operation::MORE, left, right) == 0;
        case Operation::EQMORE:
            return DoOp(Operation::LESS, left, right) == 0;
        }
    }

    switch (left->getType())
    {
    case Type::Int32:
        return dynamic_cast<DbInt32*>(left.get())->doOp(op, dynamic_cast<DbInt32*>(right.get()));
    case Type::Bool:
        if (op == Operation::NOT) {
            return dynamic_cast<DbBool*>(left.get())->doOp(op, nullptr);
        }
        return dynamic_cast<DbBool*>(left.get())->doOp(op, dynamic_cast<DbBool*>(right.get()));
    case Type::Bytes:
        return dynamic_cast<DbBytes*>(left.get())->doOp(op, nullptr);
    case Type::String:
        if (op == Operation::LEN) {
            return dynamic_cast<DbString*>(left.get())->doOp(op, nullptr);
        }
        return dynamic_cast<DbString*>(left.get())->doOp(op, dynamic_cast<DbString*>(right.get()));
    default:
        // throw ex - unexp type used in operation
        throw std::runtime_error("error");
        exit(-1);
        break;
    }
}



TableColumn::TableColumn(std::string table_, std::string column_) : table(table_), column(column_){}; // not add 1 arg not to create misunderstandings
TableColumn::TableColumn(std::pair<std::string, std::string> tc) : table(tc.first), column(tc.second){};
bool TableColumn::operator<(const TableColumn& other) const {
    if (table != other.table) {
        return table < other.table;
    }
    return column < other.column;
}

bool TableColumn::operator==(const TableColumn& other) const {
    return table == other.table && column == other.column;
}


}