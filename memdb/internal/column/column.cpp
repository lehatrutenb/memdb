#pragma once
#include <string_view>
#include <memory>
#include <vector>
#include <optional>
#include "column.hpp"

namespace memdb {

ColumnType::ColumnType(Type t_): t(t_){};
ColumnType::ColumnType(Type t_, int maxLength_): t(t_), maxLength(maxLength_){};




ColumnDescription::ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string name_): tps(tps_), name(name_) {
    std::shared_ptr<DbType> tmp{new DbTypeEmpty{}};
    defVal.swap(tmp);
}
ColumnDescription::ColumnDescription(const std::vector<ColumnAttrs> tps_, const std::string name_, const std::shared_ptr<DbType> defVal_): tps(tps_), name(name_), defVal(defVal_) {}


ColumnFullDescription::ColumnFullDescription(ColumnType ct, ColumnDescription cd): ColumnType(ct), ColumnDescription(cd){};


Column::Column(std::vector<ColumnAttrs> tps, std::string_view name_) : name(name_) {
    for (auto tp : tps) {
        rowTypeF |= static_cast<int>(tp);
    }
}

bool Column::Unique() {
    return (rowTypeF&1) != 0;
}

bool Column::Autoincrement() {
    return (rowTypeF&2) != 0;
}

bool Column::Key() {
    return (rowTypeF&4) != 0;
}

std::string_view Column::Name() {
    return name;
}




ColumnInt32::ColumnInt32(const ColumnFullDescription& descr) : ColumnBase<int32_t, DbInt32>(descr) {}
ssize_t ColumnInt32::push() {
    if (baseColT::defVal.has_value()) {
        auto val = defVal.value();
        return baseColT::cur.push(val);
    }
    if (Autoincrement()) {
        if (baseColT::cur.getLastInd() == -1) {
            int val = 0;
            return baseColT::cur.push(val);
        }
        auto val = baseColT::get(baseColT::cur.getLastInd()) + 1;
        return baseColT::cur.push(val);
    }
    // throw ex
    throw std::runtime_error("error");
    return -1;
}

ColumnBool::ColumnBool(const ColumnFullDescription& descr) : ColumnBase<bool, DbBool>(descr) {}

ColumnString::ColumnString(const ColumnFullDescription& descr) : ColumnBase<std::string, DbString>(descr) {}
ssize_t ColumnString::push(std::shared_ptr<DbType> x)  {
    if (baseColT::columnT.maxLength >= getValue<DbString, std::string>(x).size()) {
        return baseColT::push(x);
    }
    // throw ex got too large string
    throw std::runtime_error("error");
    return -1;
}

ColumnBytes::ColumnBytes(const ColumnFullDescription& descr) : ColumnBase<std::vector<char>, DbBytes>(descr) {}
ssize_t ColumnBytes::push(std::shared_ptr<DbType> x)  {
    if (baseColT::columnT.maxLength * 2 + 2 >= getValue<DbBytes, std::vector<char>>(x).size()) { // + 2 eq to 0x
        return baseColT::push(x);
    }
    // throw ex got too large byte arr
    throw std::runtime_error("error");
    return -1;
}
}