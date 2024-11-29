#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <forward_list>
#include <string>
#include <memory>
#include <stdexcept>

namespace memdb {

enum class Type {
    Int32,
    Bool,
    Bytes,
    String,
    Empty,
};

enum class Operation {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MOD,
    EQ,
    LESS,
    MORE,
    EQLESS,
    EQMORE,
    NOTEQ,
    AND,
    OR,
    NOT,
    XOR,
    LEN
};

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

// TODO можно сделать класс не виртуальным чтобы не создавать лишний раз vtable

class DbType {
public:
    virtual Type getType()=0;
    virtual std::shared_ptr<DbType> Copy()=0;
    virtual ~DbType()=default;
};

class DbTypeEmpty : public DbType {
    public:
    Type getType() override {
        return Type::Empty;
    }
    std::shared_ptr<DbType> Copy() override {};
};

class DbInt32 : public DbType {
public:
    DbInt32(int x_) : x(x_){};
    Type getType() override {
        return Type::Int32;
    }
    int32_t get() {
        return x;
    }

    std::shared_ptr<DbType> Copy() override {
        std::shared_ptr<DbType> copy{new DbInt32(x)};
        return copy;
    }

    bool doOp(Operation op, const DbInt32* another) { // update cur
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
            break;
        }
        return false;
    }

    int32_t x;
};

class DbBool : public DbType {
public:
    DbBool(bool x_) : x(x_){};
    Type getType() override {
        return Type::Bool;
    }
    bool get() {
        return x;
    }

    std::shared_ptr<DbType> Copy() override {
        std::shared_ptr<DbType> copy{new DbBool(x)};
        return copy;
    }

    bool doOp(Operation op, const DbBool* another) { // update cur
        // throw err
        throw std::runtime_error("error");
        exit(-1);
        return true;
    }

    bool x;
};

class DbBytes : public DbType {
public:
    DbBytes(std::vector<char> v_) : x(v_){};
    std::vector<char> get() {
        return x;
    }
    Type getType() override {
        return Type::Bytes;
    }

    std::shared_ptr<DbType> Copy() override {
        std::shared_ptr<DbType> copy{new DbBytes(x)};
        return copy;
    }

    bool doOp(Operation op, const DbBytes* another) { // update cur
        // throw err
        throw std::runtime_error("error");
        exit(-1);
        return true;
    }

    std::vector<char> x;
};

class DbString : public DbType {
public:
    DbString(std::string s_) : x(s_){};
    std::string get() {
        return x;
    }
    Type getType() override {
        return Type::String;
    }

    std::shared_ptr<DbType> Copy() override {
        std::shared_ptr<DbType> copy{new DbString(x)};
        return copy;
    }

    bool doOp(Operation op, const DbString* another) { // update cur
        // throw err
        throw std::runtime_error("error");
        exit(-1);
        return true;
    }

    std::string x;
};



static int DoOp(Operation op, std::shared_ptr<DbType> left, std::shared_ptr<DbType> right) {
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
        return dynamic_cast<DbBytes*>(left.get())->doOp(op, dynamic_cast<DbBytes*>(right.get()));
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

/*
template<typename T>
class DbTypeMassoP {
    virtual void get(std::vector<ssize_t> inds, std::vector<T>& res) = 0;
    virtual void get(std::vector<bool>& is_fine, std::vector<T>& res) = 0;
    virtual void getIndsWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check) = 0;
    virtual void getValsWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check, std::vector<T>& res) = 0;
    virtual void del(std::vector<ssize_t> inds) = 0;
    virtual void delWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check) = 0;
};*/

// vector - хорошо, если говорить в том контексте, что мы удаляем не 1 конкретный элемент, а какое-то ощутимое множетсво
// unordered_map  - аналог, но для более точечных удалений + хуже работает когда очень много элементов 

template<typename T>
class DbType_s { //: public DbTypeMassoP<T> {
public:
    T get(ssize_t ind) {
        // throw ex if not exist
        if (v.find(ind) == v.end()) {
            throw std::runtime_error("error");
        }
        return v[ind];
    }

    void* getp(ssize_t ind) {
        // throw ex if not exist
        if (v.find(ind) == v.end()) {
            throw std::runtime_error("error");
        }
        return static_cast<void*>(&v[ind]);
    }

    void get(std::vector<ssize_t> inds, std::vector<T>& res) {
        res.reserve(inds.size());
        for (ssize_t i : inds) {
             // throw ex if not exist
            if (v.find(i) == v.end()) {
                throw std::runtime_error("error");
            }
            res.emplace_back(v[i]);
        }
    }

    void get(std::vector<bool>& is_fine, std::vector<T>& res) {
        for (int i = 0; i < sz; i++) {
            if (is_fine[i]) {
                // throw ex if not exist
                if (v.find(i) == v.end()) {
                    throw std::runtime_error("error");
                }
                res.emplace_back(v[i]);
            }
        }
    }

    void get(std::vector<bool>& is_fine, std::vector<void*>& res) {
        for (int i = 0; i < sz; i++) {
            if (is_fine[i]) {
                // throw ex if not exist
                if (v.find(i) == v.end()) {
                    throw std::runtime_error("error");
                }
                res.emplace_back(static_cast<void*>(&v[i]));
            }
        }
    }

    void update(ssize_t ind, T& obj) {
        // throw ex if not exist
        if (v.find(ind) == v.end()) {
            throw std::runtime_error("error");
        }
        v[ind] = obj;
    }

    int push(T& obj) {
        v[++sz] = obj;
        fl.push_front(sz); // push front is fine cause location principe
        ind2it[sz - 1] = fl.begin();
        ind2it[sz] = fl.before_begin();

        return sz;
    }

    void del(int ind) {
        // throw ex if not exist
        if (v.find(ind) == v.end()) {
            throw std::runtime_error("error");
        }
        v.erase(ind);
        fl.erase_after(ind2it[ind]);
        ind2it.erase(ind);
    }

    void del(std::vector<ssize_t> inds) {
        for (ssize_t i : inds) {
            del(i);
        }
    }

    // точно ли тут хотим индексы передавать а не просто идти по всем элементам?
    /*void delWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check) { // function<void(void)>  std::vector<bool check(T& arg)>
        for (ssize_t i : inds) {
            bool ok = true;
            for (auto f_check : to_check) {
                ok &= f_check(v[i]);
                /*if (!ok) {
                    break;
                }* / // TODO check if really helps
            }
            if (ok) {
                del(i);
            }
        }
    }*/

    ssize_t size() {
        return sz;
    }

    using FLIt = std::forward_list<ssize_t>::iterator;
    FLIt begin() {
        return fl.begin();
    }

    FLIt end() {
        return fl.end();
    }

    ~DbType_s() = default;

private:
    std::unordered_map<ssize_t, T> v;
    std::forward_list<ssize_t> fl;
    std::unordered_map<ssize_t, FLIt> ind2it; // cmps it to el before
    ssize_t sz;
};

struct TableColumn {
public:
    TableColumn(std::string table_, std::string column_) : table(table_), column(column_){}; // not add 1 arg not to create misunderstandings
    TableColumn(std::pair<std::string, std::string> tc) : table(tc.first), column(tc.second){};
    std::string table;
    std::string column;

    bool operator<(const TableColumn& other) const {
        if (table != other.table) {
            return table < other.table;
        }
        return column < other.column;
    }

    bool operator==(const TableColumn& other) const {
        return table == other.table && column == other.column;
    }
};

template<typename T, typename U>
U getValue(std::shared_ptr<DbType> x) {
    return dynamic_cast<T*>(x.get())->x;
}

}

/*
int main() {
    memdb::DbType_s<std::string> test{};
    test.begin();
}*/


/*
class DbType_s {
public:
    virtual void get(int ind) = 0;
    virtual void update(int ind, DbType) = 0;
    virtual void push(int ind) = 0;
    virtual ~DbType_s() = 0;
};

class Int32_s : DbType_s {
};

class Bool_s : DbType_s {
};

class Bytes_s : DbType_s {
};

class String_s : DbType_s {
};
*/