#pragma once
#include "../row/row.cpp"
#include "../type/type.cpp"
#include "../column/column.cpp"
#include "../parser/condition.cpp"
#include "../parser/assignments.cpp"
#include <map>
#include <string_view>
#include <numeric>

// it is not really max size - just size that will be allocated since start
namespace memdb {

struct TableView {
    public:
    class Row {
        public:
        Row(std::map<std::string, ssize_t> col2ind_, std::vector<void*> data_) : rcol2ind(col2ind_), data(data_){};
        template<typename T>
        T& Get(std::string colName) {
            if (rcol2ind.find(colName) == rcol2ind.end()) {
                // throw ex
                throw std::runtime_error("error");
                exit(-1);
            }
            return *static_cast<T*>(data[rcol2ind[colName]]);
        }

        void*& operator[](ssize_t ind) {
            return data[ind];
        }

        private:
            std::map<std::string, ssize_t> rcol2ind;
            std::vector<void*> data;
    };

    private:
    class Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Row;
        using pointer           = Row*;
        using reference         = Row&;
    public:
        explicit Iterator(std::vector<Row>::iterator now_) : now(now_){}
        explicit Iterator() = default;

        reference operator*() {
            return *now;
        }
        pointer operator->() {
            return &(*now); // TODO ???? is it correct
        }
        Iterator& operator++() {
            now++;
            return *this;
        }
        Iterator operator++(int) { const Iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const Iterator& o) const { return now == o.now; };
        bool operator!=(const Iterator& o) const { return now != o.now; };

    private:
        std::vector<Row>::iterator now;
    };

    public:
    
    TableView(std::map<std::string, ssize_t> col2ind_, std::vector<Row> columns_): col2ind(col2ind_), columns(columns_)  {}

    std::map<std::string, ssize_t> col2ind;
    std::vector<Row> columns;

    Iterator begin() {
        return Iterator(columns.begin());
    }
    Iterator end() {
        return Iterator(columns.end());
    }
};

class Table {
static inline std::vector<bool> isFine;
public:
    struct Value {
        Value(const std::shared_ptr<DbType>& fValue_) : fValue(fValue_) {};
        Value(const std::string& fName_, const std::shared_ptr<DbType>& fValue_) : fName(fName_), fValue(fValue_) {};
        const std::string fName;
        const std::shared_ptr<DbType> fValue;
    };

    Table(const std::string_view& tName_) : tName(tName_){};
    void AddColumn(const ColumnType& colTp, const ColumnDescription& descr) { // nullptr
        if (name2Ind.find(descr.name) != name2Ind.end()) {
            throw std::runtime_error("error");
            exit(-1); // throw exc
        }
        std::shared_ptr<Column> newCol;
        if (colTp.t == Type::Int32) {
            std::shared_ptr<Column> tm{new ColumnInt32(descr)};
            newCol.swap(tm);
        } else if (colTp.t == Type::Bool) {
            std::shared_ptr<Column> tm{new ColumnBool(descr)};
            newCol.swap(tm);
        } else if (colTp.t == Type::String) {
            std::shared_ptr<Column> tm{new ColumnString(descr)};
            newCol.swap(tm);
        } else if (colTp.t == Type::Bytes) {
            std::shared_ptr<Column> tm{new ColumnBytes(descr)};
            newCol.swap(tm);
        }
        /*switch (colTp.t) {
        case Type::Int32:
            std::shared_ptr<Column> tm{new ColumnInt32(descr)};
            newCol.swap(tm);
            break;
        /*case Type::Bool:
            newCol = std::make_shared<Column>(ColumnBool(tps, name, defVal));
            break;* /
        default:
            exit(-1); // throw exc
            break;
        }*/
        name2Ind[descr.name] = columns.size();
        columns.emplace_back(newCol);
    }

    void Insert(const std::vector<Value>& values) {
        bool listType = false;
        bool mapType = false;

        std::vector<int> setted(columns.size(), -1);
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
                    throw std::runtime_error("error");
                }
                setted[curCol->second] = i;
            } else {
                // throw ex - should be correct
                throw std::runtime_error("error");
            }
        }
        if (listType && mapType) {
            // throw ex - should be one of these
            throw std::runtime_error("error");
        }

        if (listType) {
            if (values.size() != columns.size()) {
                // throw ex - should be one per each col
                throw std::runtime_error("error");
            }
            for (int i = 0; i < values.size(); i++) {
                if (values[i].fValue.get()->getType() != Type::Empty) {
                    setted[i] = i;
                }
            }
            //std::iota(setted.begin(), setted.end(), 0);
        }

        for (int i = 0; i < columns.size(); i++) {
            if (setted[i] == -1) {
                columns[i]->push(); // will throw ex form inside if no default
                continue;
            }

            columns[i]->push(values[setted[i]].fValue);
        }
    }

    bool checkCondNeed(std::vector<TableColumn>& need) {
        for (auto& tc : need) {
            if (tc.table != tName && tc.table != "") {
                // throw ex - unexp table name got
                throw std::runtime_error("error");
                exit(-1);
            }
            if (name2Ind.find(tc.column) == name2Ind.end()) {
                // throw ex - unexp column name got
                throw std::runtime_error("error");
                exit(-1);
            }
            /*if (tc.table == "") {
                tc.table = tName;
            }*/ // not try to set table to have simular "" in both leaf cond and need tablecolumn
        }
        return true;
    }

    ssize_t where(parser::Condition& cond) {
        std::vector<TableColumn> need = cond.GetWhatNeed();
        if (!checkCondNeed(need)) {
            return 0;
        }
        std::map<TableColumn, std::shared_ptr<DbType>> vals;

        isFine.resize(columns[0]->size());
        ssize_t fineAmt = 0;
        bool gotFineInds = false;
        // and are there wait to write base loop
        for (auto it = columns[0]->begin(); it != columns[0]->end(); it++) { // CARE it is safe cause we don't delete full colums and each col must have simular size
            ssize_t ind = *it;
            for (auto& tc : need) {
                vals[tc] = columns[name2Ind[tc.column]]->Get(ind);
            }
            auto res = cond.Compute(vals);
            if (res->getType() != Type::Bool) {
                // throw ex expected bool as a result of where cond
                throw std::runtime_error("error");
                exit(-1);
            }
            isFine[ind] = getValue<DbBool, bool>(res);
            fineAmt += isFine[ind];
        }
        return fineAmt;
    }

    void Update(parser::Assignments& as, parser::Condition& condWh) {
        where(condWh);
        std::vector<std::vector<TableColumn>> need;
        need.reserve(as.As.size());
        for (auto& colExp : as.As) {
            need.emplace_back(colExp.Exp.GetWhatNeed());
        }
        std::vector<std::map<TableColumn, std::shared_ptr<DbType>>> vals(need.size()); // если не хотим делать изменения 'транзакционно' 
                                                // а хотим задать порядок - надо убрать массив и после каждого получения знач пересчитывать

        bool gotFineInds = false;
        // and are there wait to write base loop
        for (auto it = columns[0]->begin(); it != columns[0]->end(); it++) { // CARE it is safe cause we don't delete full colums and each col must have simular size
            ssize_t ind = *it;
            if (!isFine[ind]) {
                continue;
            }
            for (int colAsInd = 0; colAsInd < need.size(); colAsInd++) {
                for (auto& tc : need[colAsInd]) {
                    tc.table = tName;
                    vals[colAsInd][tc] = columns[name2Ind[tc.column]]->Get(ind);
                }
            }

            for (int colAsInd = 0; colAsInd < need.size(); colAsInd++) {
                columns[ind]->update(ind, as.As[colAsInd].Exp.Compute(vals[colAsInd]));
            }
        }
    }

    //TableView(std::map<std::string, sssize_t>& col2ind_, std::vector<Row> columns_): col2ind(col2ind_), columns(columns_)  {}
    // Row(std::map<std::string, sssize_t>& col2ind_, std::vector<void*> data_) : col2ind(col2ind_), data(data_){};
    TableView Select(const std::vector<TableColumn>& tcs, parser::Condition& condWh) {
        ssize_t rowAmt = where(condWh);
        ssize_t colAmt = 0;
        for (const auto& tc : tcs) {
            if (tc.table != tName) {
                continue;
            }
            colAmt++;
            if (name2Ind.find(tc.column) == name2Ind.end()) {
                // throw ex not found column in table
                throw std::runtime_error("error");
                exit(-1);
            }
        }
        // Row(std::map<std::string_view, sssize_t>& col2ind_, std::vector<void*> data_) : col2ind(col2ind_), data(data_){};
        //Row(std::map<std::string_view, sssize_t>& col2ind_, std::vector<void*> data_) : col2ind(col2ind_), data(data_){};
        std::vector<TableView::Row> data(rowAmt, TableView::Row(name2Ind, std::vector<void*> (colAmt)));
        ssize_t curInd = 0;
        for (auto it = columns[0]->begin(); it != columns[0]->end(); it++) { // CARE it is safe cause we don't delete full colums and each col must have simular size
            ssize_t ind = *it;
            if (!isFine[ind]) {
                continue;
            }
            
            ssize_t colInd = 0;
            for (const auto& tc : tcs) {
                if (tc.table != tName) {
                    continue;
                }
                data[curInd][colInd] = columns[name2Ind[tc.column]]->GetP(ind);
                colInd++;
            }
            curInd++;
        }
        return TableView(name2Ind, data);
    }

private:
    std::string tName;
    std::vector<std::shared_ptr<Column>> columns;
    std::map<std::string, ssize_t> name2Ind;
};

}