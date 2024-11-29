#pragma once
#include <string_view>
#include "../type/type.cpp"
#include <memory>
#include <vector>
#include <optional>

namespace memdb {

class ColumnType {
public:
    ColumnType(Type t_): t(t_){};
    ColumnType(Type t_, int maxLength_): t(t_), maxLength(maxLength_){};
    Type t;
    int maxLength;
};

enum class ColumnAttrs {
    Default = 0,
    Unique = 1,
    Autoincrement = 2,
    Key = 4,
};

struct ColumnDescription  {
    const std::vector<ColumnAttrs> tps;
    const std::string name;
    std::shared_ptr<DbType> defVal;

    ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string name_): tps(tps_), name(name_) {
        std::shared_ptr<DbType> tmp{new DbTypeEmpty{}};
        defVal.swap(tmp);
    }
    ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string name_, const std::shared_ptr<DbType> defVal_): tps(tps_), name(name_), defVal(defVal_) {}
};

struct ColumnFullDescription : public ColumnType, public ColumnDescription {
    ColumnFullDescription(ColumnType ct, ColumnDescription cd): ColumnType(ct), ColumnDescription(cd){};
};

class Column {
public:
    Column(std::vector<ColumnAttrs> tps, std::string_view name_) : name(name_) {
        for (auto tp : tps) {
            rowTypeF |= static_cast<int>(tp);
        }
    }

    virtual ssize_t push() = 0;
    virtual ssize_t push(std::shared_ptr<DbType> x) = 0;
    virtual void del(ssize_t ind) = 0;
    virtual void update(ssize_t ind, std::shared_ptr<DbType> x) = 0;
    virtual std::shared_ptr<DbType> Get(ssize_t ind) = 0;
    virtual void Get(std::vector<bool>& is_fine, std::vector<void*>& res) = 0;
    virtual void* GetP(ssize_t ind) = 0;

    bool Unique() {
        return (rowTypeF&1) != 0;
    }

    bool Autoincrement() {
        return (rowTypeF&2) != 0;
    }

    bool Key() {
        return (rowTypeF&4) != 0;
    }

    std::string_view Name() {
        return name;
    }

    virtual DbType_s<int32_t>::FLIt begin() = 0; // type there doesn't really matter
    virtual DbType_s<int32_t>::FLIt end() = 0;
private:
    std::string_view name;
    int rowTypeF = 0;
};

template<typename T, typename DbT>
class ColumnBase : public Column {
public:
    ColumnBase(ColumnDescription descr) : Column(descr.tps, descr.name) {
        if (descr.defVal.get()->getType() == Type::Empty) {
            defVal = {};
        } else {
            defVal = std::optional<T>{dynamic_cast<DbT*>(descr.defVal.get())->get()};
        }
    }
    
    ssize_t push(std::shared_ptr<DbType> x) override {
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        return cur.push(x_);
    }

    ssize_t push() override {
        if (defVal.has_value()) {
            return cur.push(defVal.value());
        }
        // throw ex
        throw std::runtime_error("error");
        return -1;
    }

    void del(ssize_t ind) override {
        cur.del(ind);
    }

    void update(ssize_t ind, std::shared_ptr<DbType> x) override {
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        cur.update(ind, x_);
    }

    T get(ssize_t ind) {
        return cur.get(ind);
    }

    void Get(std::vector<bool>& isFine, std::vector<void*>& res) override {
        cur.get(isFine, res);
    }

    void* GetP(ssize_t ind) override {
        return cur.getp(ind);
    }

    virtual std::shared_ptr<DbType> Get(ssize_t ind) override {
        return std::shared_ptr<DbType>{new DbT{get(ind)}};
    }

    DbType_s<int32_t>::FLIt begin() override {
        return cur.begin();
    }

    DbType_s<int32_t>::FLIt end() override {
        return cur.end();
    }

private:
    DbType_s<T> cur = DbType_s<T>{};
    std::optional<T> defVal;
};

struct ColumnInt32 : public ColumnBase<int32_t, DbInt32> {
    ColumnInt32(const ColumnDescription& descr) : ColumnBase<int32_t, DbInt32>(descr) {}
};

struct ColumnBool : public ColumnBase<bool, DbBool> {
    ColumnBool(const ColumnDescription& descr) : ColumnBase<bool, DbBool>(descr) {}
};

struct ColumnString : public ColumnBase<std::string, DbString> {
    ColumnString(const ColumnDescription& descr) : ColumnBase<std::string, DbString>(descr) {}
};

struct ColumnBytes : public ColumnBase<std::vector<char>, DbBytes> {
    ColumnBytes(const ColumnDescription& descr) : ColumnBase<std::vector<char>, DbBytes>(descr) {}
};

/*
// TODO check if we push correct types inside - throw ex otherwise
class ColumnInt32 : public Column {
public:
    using T = int32_t;
    using DbT = DbInt32;

    ColumnInt32(ColumnDescription descr) : Column(descr.tps, descr.name) {
        if (descr.defVal.get()->getType() == Type::Empty) {
            defVal = {};
        } else {
            defVal = std::optional<T>{dynamic_cast<DbT*>(descr.defVal.get())->get()};
        }
        //curMassOp = dynamic_cast<DbTypeMassoP<T>*>(&cur);
    }
    
    ssize_t push(std::shared_ptr<DbType> x) {
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        return cur.push(x_);
    }

    ssize_t push() {
        if (defVal.has_value()) {
            return cur.push(defVal.value());
        }
        // throw ex
        return -1;
    }

    void del(ssize_t ind) {
        cur.del(ind);
    }

    void update(ssize_t ind, std::shared_ptr<DbType> x) { // std::shared_ptr<DbType>
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        cur.update(ind, x_);
    }

    T get(ssize_t ind) {
        return cur.get(ind);
    }

    void Get(std::vector<bool>& isFine, std::vector<void*> res) override {
        cur.get(isFine, res);
    }

    virtual std::shared_ptr<DbType> Get(ssize_t ind) {
        return std::shared_ptr<DbType>{new DbT{get(ind)}};
    }

    DbType_s<int32_t>::FLIt begin() {
        return cur.begin();
    }

    DbType_s<int32_t>::FLIt end() {
        return cur.end();
    }

    //DbTypeMassoP<T>* curMassOp; // это же ок? // да, не обяз разименования, но это и так тяжёлые операции - их много не сделать
private:
    DbType_s<T> cur = DbType_s<T>{};
    std::optional<T> defVal;
};



/*
std::shared_ptr<DbType> convertBoolToInt32(std::shared_ptr<DbType> val) {
    return val.get()->getType() == Type::Empty ? val : std::shared_ptr<DbType>{new DbInt32(getValue<DbBool, bool>(val))};
}

class ColumnBool : public ColumnInt32 {
public:
    using T = bool;
    using DbT = DbBool;

    ColumnBool(ColumnDescription descr) : ColumnInt32(ColumnDescription(descr.tps, descr.name, convertBoolToInt32(descr.defVal))) {}
    
    T get(ssize_t ind) {
        return ColumnInt32::get(ind) != 0;
    }
    std::shared_ptr<DbType> Get(ssize_t ind) {
        return std::shared_ptr<DbType>{new DbT{get(ind)}};
    }
};

class ColumnBytes : public Column {
public:
    using T = std::vector<char>;
    using DbT = DbBytes;

    ColumnBytes(ColumnDescription descr) : Column(descr.tps, descr.name) {
        if (descr.defVal.get()->getType() == Type::Empty) {
            defVal = {};
        } else {
            defVal = std::optional<T>{dynamic_cast<DbT*>(descr.defVal.get())->get()};
        }
        //curMassOp = dynamic_cast<DbTypeMassoP<T>*>(&cur);
    }
    
    ssize_t push(std::shared_ptr<DbType> x) {
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        return cur.push(x_);
    }

    ssize_t push() {
        if (defVal.has_value()) {
            return cur.push(defVal.value());
        }
        // throw ex
        return -1;
    }

    void del(ssize_t ind) {
        cur.del(ind);
    }

    void update(ssize_t ind, std::shared_ptr<DbType> x) { // std::shared_ptr<DbType>
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        cur.update(ind, x_);
    }

    T get(ssize_t ind) {
        return cur.get(ind);
    }
    virtual std::shared_ptr<DbType> Get(ssize_t ind) {
        return std::shared_ptr<DbType>{new DbT{get(ind)}};
    }

    DbType_s<int32_t>::FLIt begin() { // int32_t , not void for ex just not to get errors when c++ try to get address of void in type.cpp
        return cur.begin();
    }

    DbType_s<int32_t>::FLIt end() {
        return cur.end();
    }

    //DbTypeMassoP<T>* curMassOp; // это же ок? // да, не обяз разименования, но это и так тяжёлые операции - их много не сделать
private:
    DbType_s<T> cur = DbType_s<T>{};
    std::optional<T> defVal;
};

std::shared_ptr<DbType> convertStringToBytes(std::shared_ptr<DbType> val) {
    if (val.get()->getType() == Type::Empty) {
        return val;
    }
    std::string str = getValue<DbString, std::string>(val);
    return val.get()->getType() == Type::Empty ? val : std::shared_ptr<DbType>{new  DbBytes(std::vector<char> (str.begin(), str.end()))};
}

class ColumnString : public ColumnBytes {
public:
    using T = std::string;
    using DbT = DbString;

    ColumnString(ColumnDescription descr) : ColumnBytes(ColumnDescription(descr.tps, descr.name, convertStringToBytes(descr.defVal))) {}
    
    T get(ssize_t ind) {
        auto res = ColumnBytes::get(ind);
        return std::string(res.begin(), res.end());
    }
    std::shared_ptr<DbType> Get(ssize_t ind) {
        return std::shared_ptr<DbType>{new DbT{get(ind)}};
    }
};

template<typename T, typename U>
U getValue(std::shared_ptr<Column> x, ssize_t ind) {
    return dynamic_cast<T*>(x.get())->get(ind);
}*/

/*
class ColumnView {
public:
    template<typename T>
    T& Get(ssize_t ind) {
        return *static_cast<T*>(data[ind]);
    }
    ssize_t size() {
        return data.size();
    }
private:
    std::vector<void*> data;
};*/

/*
class ColumnBool : Column {
public:
    using T = bool;
    using DbT = DbBool;

    ColumnBool(std::vector<ColumnAttrs> tps, std::string_view name_, std::shared_ptr<DbType> defVal_) : Column(tps, name_) {
        if (defVal_.get() == nullptr) {
            defVal = {};
        } else {
            defVal = std::optional<T>{dynamic_cast<DbT*>(defVal_.get())->get()};
        }
    }
    
    int push(std::shared_ptr<DbType> x) {
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        return cur.push(x_);
    }

    void del(int ind) {
        cur.del(ind);
    }

    void update(int ind, std::shared_ptr<DbType> x) {
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        cur.update(ind, x_);
    }

    T get(int ind) {
        return cur.get(ind);
    }
private:
    std::optional<T> defVal;
    DbType_s<T> cur;
};
*/

}

/*
using namespace memdb;

int main() {
    ColumnDescription descr({}, "abacaba");
    ColumnInt32 test(descr);
    std::shared_ptr<Column> p{new ColumnInt32(descr)};
    ColumnInt32 test2= *dynamic_cast<ColumnInt32*>(p.get());
}
*/