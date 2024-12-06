#pragma once
#include <string_view>
#include "../type/type.hpp"
#include <memory>
#include <vector>
#include <optional>
#include <iostream>

namespace memdb {

class ColumnType {
public:
    ColumnType(Type t_);
    ColumnType(Type t_, int maxLength_);
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

    ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string name_);
    ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string name_, const std::shared_ptr<DbType> defVal_);
};

struct ColumnFullDescription : public ColumnType, public ColumnDescription {
    ColumnFullDescription(ColumnType ct, ColumnDescription cd);
};

class Column {
public:
    using ColumnStructP = void*;
    Column(std::vector<ColumnAttrs> tps, std::string_view name_);

    virtual ssize_t push() = 0;
    virtual ssize_t push(std::shared_ptr<DbType> x) = 0;
    virtual void del(ssize_t ind) = 0;
    virtual void update(ssize_t ind, std::shared_ptr<DbType> x) = 0;
    virtual std::shared_ptr<DbType> Get(ssize_t ind) = 0;
    virtual void Get(std::vector<bool>& is_fine, std::vector<void*>& res) = 0;
    virtual ColumnStructP GetP() = 0;
    virtual void getInds(std::vector<ssize_t>& res) = 0;

    bool Unique();

    bool Autoincrement();

    bool Key();

    std::string_view Name();

    virtual ssize_t size() = 0;
    virtual ssize_t MaxInd() = 0;
    virtual DbType_s<int32_t>::FLIt begin() = 0; // type there doesn't really matter
    virtual DbType_s<int32_t>::FLIt end() = 0;
private:
    std::string_view name;
    int rowTypeF = 0;
};

struct ColumnInt32;
struct ColumnString;
struct ColumnBytes;

template<typename T, typename DbT>
class ColumnBase : public Column {
public:
friend ColumnInt32;
friend ColumnString;
friend ColumnBytes;
    ColumnBase(ColumnFullDescription descr) : Column(descr.tps, descr.name), columnT(descr){
        if (descr.defVal.get()->getType() == Type::Empty) {
            defVal = {};
        } else {
            defVal = std::optional<T>{dynamic_cast<DbT*>(descr.defVal.get())->get()};
        }
        cur = DbType_s<T>(Column::Unique());
    }
    
    ssize_t push(std::shared_ptr<DbType> x) override {
        if (x->getType() == Type::Empty) { // not obligitary (cause checks in insert) but let it be
            return push();
        }
        if (x->getType() != DbT{}.getType()) {
            // throw ex got another type
            throw std::runtime_error("error");
            return -1;
        }
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        return cur.push(x_);
    }

    virtual ssize_t push() override {
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

    virtual void update(ssize_t ind, std::shared_ptr<DbType> x) override {
        T x_ = dynamic_cast<DbT*>(x.get())->get();
        cur.update(ind, x_);
    }

    T get(ssize_t ind) {
        return cur.get(ind);
    }

    void Get(std::vector<bool>& isFine, std::vector<void*>& res) override {
        cur.get(isFine, res);
    }

/*
    std::unordered_map<ssize_t, T>::iterator GetP(ssize_t ind) override {
        return cur.getp(ind);
    }
*/
    virtual std::shared_ptr<DbType> Get(ssize_t ind) override {
        return std::shared_ptr<DbType>{new DbT{get(ind)}};
    }

    DbType_s<int32_t>::FLIt begin() override {
        return cur.begin();
    }

    DbType_s<int32_t>::FLIt end() override {
        return cur.end();
    }

    ssize_t size() override {
        return cur.size();
    }

    ssize_t MaxInd() override {
        return cur.MaxInd();
    }

    void getInds(std::vector<ssize_t>& res) override {
        cur.getInds(res);
    }

    ColumnStructP GetP() override {
        return static_cast<ColumnStructP>(&cur);
    }

private:
    ColumnType columnT;
    std::optional<T> defVal;
    DbType_s<T> cur;
};

struct ColumnInt32 : public ColumnBase<int32_t, DbInt32> {
    using baseColT = ColumnBase<int32_t, DbInt32>;
    ColumnInt32(const ColumnFullDescription& descr);
    virtual ssize_t push() override;
};

struct ColumnBool : public ColumnBase<bool, DbBool> {
    ColumnBool(const ColumnFullDescription& descr);
};

struct ColumnString : public ColumnBase<std::string, DbString> {
    using baseColT = ColumnBase<std::string, DbString>;
    ColumnString(const ColumnFullDescription& descr);
    virtual ssize_t push(std::shared_ptr<DbType> x) override;
    virtual void update(ssize_t ind, std::shared_ptr<DbType> x) override;
};

struct ColumnBytes : public ColumnBase<std::vector<char>, DbBytes> {
    using baseColT = ColumnBase<std::vector<char>, DbBytes>;
    ColumnBytes(const ColumnFullDescription& descr);
    virtual ssize_t push(std::shared_ptr<DbType> x) override;
    virtual void update(ssize_t ind, std::shared_ptr<DbType> x) override;
};
}