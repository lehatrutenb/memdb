#pragma once
#include <vector>
#include <unordered_map>
#include <functional>

namespace memdb {

enum class Type {
    Int32,
    Bool,
    Byte,
    String,
    Empty,
};

// TODO можно сделать класс не виртуальным чтобы не создавать лишний раз vtable

class DbType {
public:
    virtual Type getType()=0;
    virtual ~DbType()=default;
};

class DbTypeEmpty : public DbType {
    Type getType() {
        return Type::Empty;
    }
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
private:
    bool x;
};

class DbBytes : public DbType {
    Type getType() {
        return Type::Byte;
    }
};

class DbString : public DbType {
    Type getType() {
        return Type::String;
    }
};

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