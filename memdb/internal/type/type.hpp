#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <forward_list>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

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

bool IsBoolOp(Operation op);
bool IsMathOp(Operation op);
bool IsCmpOp(Operation op);

extern bool IsNonDefCmpOp(Operation op);

// TODO можно сделать класс не виртуальным чтобы не создавать лишний раз vtable

class DbType {
public:
    virtual Type getType()=0;
    virtual std::shared_ptr<DbType> Copy()=0;
    virtual ~DbType()=default;
};

class DbTypeEmpty : public DbType {
    public:
    Type getType() override;
    std::shared_ptr<DbType> Copy() override;
};

class DbInt32 : public DbType {
public:
    DbInt32(int x_);
    Type getType() override;
    int32_t get();

    std::shared_ptr<DbType> Copy() override;

    bool doOp(Operation op, const DbInt32* another);

    int32_t x;
};

class DbBool : public DbType {
public:
    DbBool(bool x_);
    Type getType() override;
    bool get();

    std::shared_ptr<DbType> Copy() override;

    bool doOp(Operation op, const DbBool* another);

    bool x;
};

class DbBytes : public DbType {
public:
    DbBytes(std::vector<char> v_);
    std::vector<char> get();
    Type getType() override;

    std::shared_ptr<DbType> Copy() override;

    bool doOp(Operation op, const DbBytes* another);

    std::vector<char> x;
};

class DbString : public DbType {
public:
    DbString(std::string s_);
    std::string get();
    Type getType() override;

    std::shared_ptr<DbType> Copy() override;

    int doOp(Operation op, const DbString* another);

    std::string x;
};

int DoOp(Operation op, std::shared_ptr<DbType> left, std::shared_ptr<DbType> right);

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

    ssize_t getLastInd() {
        if (fl.empty()) {
            return -1;
        } else {
            return *fl.begin();
        }
    }

    void update(ssize_t ind, T& obj) {
        // throw ex if not exist
        if (v.find(ind) == v.end()) {
            throw std::runtime_error("error");
        }
        v[ind] = obj;
    }

    ssize_t push(T& obj) {
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
        auto it_prev = fl.before_begin();
        auto it_nx = fl.begin();
        while (*it_nx != ind) {
            it_prev++;
            it_nx++;
        }
        fl.erase_after(it_prev);
        //fl.erase_after(ind2it[ind]);
        ind2it.erase(ind);
    }

    void del(std::vector<ssize_t> inds) {
        for (ssize_t i : inds) {
            del(i);
        }
    }

    ssize_t size() {
        return sz;
    }

    void getInds(std::vector<ssize_t>& res) {
        for (auto ind : fl) {
            res.push_back(ind);
        }
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
    TableColumn(std::string table_, std::string column_); // not add 1 arg not to create misunderstandings
    TableColumn(std::pair<std::string, std::string> tc);
    std::string table;
    std::string column;

    bool operator<(const TableColumn& other) const;

    bool operator==(const TableColumn& other) const;
};

//template std::string getValue<parser::Tokenizer::StringT, std::string>(std::shared_ptr<DbType> x);
template<typename T, typename U>
U getValue(std::shared_ptr<DbType> x) {
    return dynamic_cast<T*>(x.get())->x;
}
}