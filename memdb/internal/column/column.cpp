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
    const std::string_view name;
    std::shared_ptr<DbType> defVal;

    ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string_view name_): tps(tps_), name(name_) {
        defVal.reset(dynamic_cast<DbType*>(new DbTypeEmpty{}));
    }
    ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string_view name_, const std::shared_ptr<DbType> defVal_): tps(tps_), name(name_), defVal(defVal_) {}
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
private:
    std::string_view name;
    int rowTypeF = 0;
};

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
        curMassOp = dynamic_cast<DbTypeMassoP<T>*>(&cur);
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

    DbTypeMassoP<T>* curMassOp; // это же ок? // да, не обяз разименования, но это и так тяжёлые операции - их много не сделать
private:
    DbType_s<T> cur = DbType_s<T>{};
    std::optional<T> defVal;
};

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