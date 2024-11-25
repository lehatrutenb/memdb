#include "../row/row.cpp"
#include "../type/type.cpp"
#include "../column/column.cpp"
#include <map>
#include <string_view>
#include <numeric>

namespace memdb {
class Table {
public:
    struct Value {
        Value(const std::shared_ptr<DbType>& fValue_) : fValue(fValue_) {};
        Value(const std::string_view& fName_, const std::shared_ptr<DbType>& fValue_) : fName(fName_), fValue(fValue_) {};
        const std::string_view fName;
        const std::shared_ptr<DbType> fValue;
    };

    Table()=default;
    void AddColumn(const ColumnType& colTp, const ColumnDescription& descr) { // nullptr
        if (name2Ind.find(descr.name) != name2Ind.end()) {
            exit(-1); // throw exc
        }
        std::shared_ptr<Column> newCol;
        switch (colTp.t) {
        case Type::Int32:
            newCol.reset(new ColumnInt32(descr));
            break;
        /*case Type::Bool:
            newCol = std::make_shared<Column>(ColumnBool(tps, name, defVal));
            break;*/
        default:
            exit(-1); // throw exc
            break;
        }
        name2Ind[descr.name] = columns.size();
        columns.emplace_back(newCol);
    }

    void Insert(const std::vector<Value>& values) {
        bool listType = false;
        bool mapType = false;

        std::vector<int> setted(columns.size());
        for (int i = 0; i < values.size(); i++) {
            const auto& value = values[i];

            listType |= (value.fName == "" && value.fValue.get()->getType() == Type::Empty);
            mapType |= !(value.fName == "");
            if (value.fName == "" ) {
                continue;
            }

            if (name2Ind.find(value.fName) != name2Ind.end()) {
                auto curCol = name2Ind.find(value.fName);
                if (setted[curCol->second] != -1) {
                    // throw ex - col names should be unique
                }
                setted[curCol->second] = i;
            } else {
                // throw ex - should be correct
            }
        }
        if (listType && mapType) {
            // throw ex - should be one of these
        }

        if (listType) {
             if (values.size() != columns.size()) {
                // throw ex - should be one per each col
            }
            std::iota(setted.begin(), setted.end(), 0);
        }

        for (int i = 0; i < columns.size(); i++) {
            if (setted[i] == -1) {
                columns[i]->push(); // will throw ex form inside if no default
            }

            columns[i]->push(values[setted[i]].fValue);
        }
    }

private:
    std::vector<std::shared_ptr<Column>> columns;
    std::map<std::string_view, size_t> name2Ind;
};

}