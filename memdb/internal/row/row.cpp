#pragma once
#include <string_view>
/*
namespace memdb {

class Row {
public:
    Row(vector<RowType> tps, std::string_view name_) : name(name_) {
        for (int tp : tps) {
            rowTypeF |= tp;
        }
    }

    bool unique() {
        return (rowTypeF&1) != 0;
    }

    bool autoincrement() {
        return (rowTypeF&2) != 0;
    }

    bool key() {
        return (rowTypeF&4) != 0;
    }

    string_view name() {
        return name;
    }
private:
    std::string_view name;
    int rowTypeF = 0;
};

}*/