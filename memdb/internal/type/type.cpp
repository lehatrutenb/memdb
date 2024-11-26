#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

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
    Type getType() {
        return Type::Empty;
    }
    std::shared_ptr<DbType> Copy(){};
};

class DbInt32 : public DbType {
public:
    DbInt32(int x_) : x(x_){};
    Type getType() {
        return Type::Int32;
    }
    int32_t get() {
        return x;
    }

    std::shared_ptr<DbType> Copy() {
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
                exit(-1);
            }
            x /= another->x;
            break;
        case Operation::MOD:
        if (another->x == 0) {
                // throw ex MOD 0
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
            exit(-1);
            break;
        }
        return false;
    }
private:
    int32_t x;
};

class DbBool : public DbType {
public:
    DbBool(bool x_) : x(x_){};
    Type getType() {
        return Type::Bool;
    }
    bool get() {
        return x;
    }

    std::shared_ptr<DbType> Copy() {
        std::shared_ptr<DbType> copy{new DbBool(x)};
        return copy;
    }

    bool doOp(Operation op, const DbBool* another) { // update cur
        // throw err
        exit(-1);
        return true;
    }
private:
    bool x;
};

class DbBytes : public DbType {
public:
    DbBytes(std::vector<char> v_) : v(v_){};
    std::vector<char> get() {
        return v;
    }
    Type getType() {
        return Type::Bytes;
    }

    std::shared_ptr<DbType> Copy() {
        std::shared_ptr<DbType> copy{new DbBytes(v)};
        return copy;
    }

    bool doOp(Operation op, const DbBytes* another) { // update cur
        // throw err
        exit(-1);
        return true;
    }
private:
    std::vector<char> v;
};

class DbString : public DbType {
public:
    DbString(std::string s_) : s(s_){};
    std::string get() {
        return s;
    }
    Type getType() {
        return Type::String;
    }

    std::shared_ptr<DbType> Copy() {
        std::shared_ptr<DbType> copy{new DbString(s)};
        return copy;
    }

    bool doOp(Operation op, const DbString* another) { // update cur
        // throw err
        exit(-1);
        return true;
    }
private:
    std::string s;
};



static int DoOp(Operation op, std::shared_ptr<DbType> left, std::shared_ptr<DbType> right) {
    if (left->getType() != right->getType()) {
        // throw ex - types must be eq
        exit(-1);
    }
    if (left->getType() == Type::Empty) {
        // throw ex - types must be not empty
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
        exit(-1);
        break;
    }
}


template<typename T>
class DbTypeMassoP {
    virtual void get(std::vector<ssize_t> inds, std::vector<T>& res) = 0;
    virtual void get(std::vector<bool>& is_fine, std::vector<T>& res) = 0;
    virtual void getIndsWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check) = 0;
    virtual void getValsWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check, std::vector<T>& res) = 0;
    virtual void del(std::vector<ssize_t> inds) = 0;
    virtual void delWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check) = 0;
};

// vector - хорошо, если говорить в том контексте, что мы удаляем не 1 конкретный элемент, а какое-то ощутимое множетсво
// unordered_map  - аналог, но для более точечных удалений + хуже работает когда очень много элементов 
template<typename T>
class DbType_s : public DbTypeMassoP<T> {
public:
    T get(ssize_t ind) {
        // throw ex if not exist
        return v[ind];
    }

    void get(std::vector<ssize_t> inds, std::vector<T>& res) {
        res.reserve(inds.size());
        for (ssize_t i : inds) {
             // throw ex if not exist
            res.emplace_back(v[i]);
        }
    }

    void get(std::vector<bool>& is_fine, std::vector<T>& res) {
        for (int i = 0; i < sz; i++) {
            if (is_fine[i]) {
                // throw ex if not exist
                res.emplace_back(v[i]);
            }
        }
    }

    void getIndsWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check) {
        std::vector<ssize_t> res;
        res.reserve(inds.size());
        for (ssize_t i : inds) {
            bool ok = true;
            for (auto f_check : to_check) {
                 // throw ex if not exist
                ok &= f_check(v[i]);
                /*if (!ok) {
                    break;
                }*/ // TODO check if really helps
            }
            if (ok) {
                res.emplace_back(i);
            }
        }
        swap(inds, res);
    }

    void getValsWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check, std::vector<T>& res) { //  std::vector<bool check(T& arg)>& to_check
        res.reserve(inds.size());
        for (ssize_t i : inds) {
            bool ok = true;
            for (auto f_check : to_check) {
                 // throw ex if not exist
                ok &= f_check(v[i]);
                /*if (!ok) {
                    break;
                }*/ // TODO check if really helps
            }
            if (ok) {
                res.emplace_back(v[i]);
            }
        }
    }

    void update(ssize_t ind, T& obj) {
         // throw ex if not exist
        v[ind] = obj;
    }

    int push(T& obj) {
        v[++sz] = obj;
        return sz;
    }

    void del(int ind) {
        // throw ex if not exist
        v.erase(v.find(ind));
    }

    void del(std::vector<ssize_t> inds) {
        for (ssize_t i : inds) {
            del(i);
        }
    }

    // точно ли тут хотим индексы передавать а не просто идти по всем элементам?
    void delWhere(std::vector<ssize_t>& inds, std::vector<std::function<bool(T&)>>& to_check) { // function<void(void)>  std::vector<bool check(T& arg)>
        for (ssize_t i : inds) {
            bool ok = true;
            for (auto f_check : to_check) {
                ok &= f_check(v[i]);
                /*if (!ok) {
                    break;
                }*/ // TODO check if really helps
            }
            if (ok) {
                del(i);
            }
        }
    }
    ~DbType_s() = default;

private:
    std::unordered_map<ssize_t, T> v;
    ssize_t sz;
};

}

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