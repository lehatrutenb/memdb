#include "table.hpp"
#include <map>
#include <string_view>
#include <numeric>
#include <iostream>

// it is not really max size - just size that will be allocated since start
namespace memdb {

/*
void*& TableView::Row::operator[](ssize_t ind) {
    return data[ind];
}
*/


TableView::Iterator::Iterator(std::vector<Row>::iterator now_) : now(now_)  {}

TableView::Iterator::reference TableView::Iterator::operator*() {
    return *now;
}
TableView::Iterator::pointer TableView::Iterator::operator->() {
    return &(*now); // TODO ???? is it correct
}
TableView::Iterator& TableView::Iterator::operator++() {
    now++;
    return *this;
}
TableView::Iterator TableView::Iterator::operator++(int) { const Iterator tmp = *this; ++(*this); return tmp; }
bool TableView::Iterator::operator==(const Iterator& o) const { return now == o.now; };
bool TableView::Iterator::operator!=(const Iterator& o) const { return now != o.now; };


TableView::TableView() {}
TableView::TableView(std::map<std::string, ssize_t> col2ind_, std::vector<ssize_t> colInds_, std::vector<Column::ColumnStructP> colPs_): col2ind(col2ind_), colInds(colInds_), colPs(colPs_) {
    readyRows.reserve(colInds.size());
    for (auto& ind: colInds) {
        readyRows.push_back(Row(col2ind, ind, colPs)); //TODO why I can't use references here
    }
}

TableView::Iterator TableView::begin() {
    return Iterator(readyRows.begin());
}
TableView::Iterator TableView::end() {
    return Iterator(readyRows.end());
}




Table::Value::Value(const std::shared_ptr<DbType>& fValue_) : fValue(fValue_) {};
Table::Value::Value(const std::string& fName_, const std::shared_ptr<DbType>& fValue_) : fName(fName_), fValue(fValue_) {};

Table::Table(const std::string_view& tName_) : tName(tName_){};
void Table::AddColumn(const ColumnFullDescription& descr) { // nullptr
    if (name2Ind.find(descr.name) != name2Ind.end()) {
        throw std::runtime_error("error");
        exit(-1); // throw exc
    }
    std::shared_ptr<Column> newCol;
    if (descr.t == Type::Int32) {
        std::shared_ptr<Column> tm{new ColumnInt32(descr)};
        newCol.swap(tm);
    } else if (descr.t == Type::Bool) {
        std::shared_ptr<Column> tm{new ColumnBool(descr)};
        newCol.swap(tm);
    } else if (descr.t == Type::String) {
        std::shared_ptr<Column> tm{new ColumnString(descr)};
        newCol.swap(tm);
    } else if (descr.t == Type::Bytes) {
        std::shared_ptr<Column> tm{new ColumnBytes(descr)};
        newCol.swap(tm);
    }
    name2Ind[descr.name] = columns.size();
    columns.emplace_back(newCol);
}

void Table::Insert(const std::vector<Value>& values) {
    bool listType = false;
    bool mapType = false;

    std::vector<int> setted(columns.size(), -1);
    for (int i = 0; i < values.size(); i++) {
        const auto& value = values[i];

        listType |= (value.fName == "" && value.fValue.get()->getType() != Type::Empty);
        mapType |= !(value.fName == "");
        if (value.fName == "" ) {
            continue;
        }

        if (name2Ind.find(value.fName) != name2Ind.end()) {
            auto curCol = name2Ind.find(value.fName);
            if (setted[curCol->second] != -1) {
                // throw ex - insert column same column setted twice
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

    if (values.size() > columns.size()) {
        // throw ex - should be one per each col
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
    } else {
        for (int i = 0; i < values.size(); i++) {
            if (values[i].fValue.get()->getType() == Type::Empty) {
                if (setted[i] != -1) {
                    // throw ex - insert column same column setted twice
                    throw std::runtime_error("error");
                }
            }
        }
    }


    for (int i = 0; i < columns.size(); i++) {
        if (setted[i] == -1) {
            columns[i]->push(); // will throw ex form inside if no default
            continue;
        }

        columns[i]->push(values[setted[i]].fValue);
    }
}

bool Table::checkCondNeed(std::vector<TableColumn>& need) {
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

ssize_t Table::where(parser::Condition& cond) {
    std::vector<TableColumn> need = cond.GetWhatNeed();
    if (!checkCondNeed(need)) {
        return 0;
    }
    std::map<TableColumn, std::shared_ptr<DbType>> vals;
    isFine.resize(columns[0]->MaxInd());
    ssize_t fineAmt = 0;
    bool gotFineInds = false;
    // and are there wait to write base loop
    for (auto it = columns[0]->begin(); it != columns[0]->end(); it++) { // CARE it is safe cause we don't delete full colums and each col must have simular size
    //for (auto ind : *columns[0]) { // CARE it is safe cause we don't delete full colums and each col must have simular size
        ssize_t ind = *it;
        for (auto& tc : need) {
            vals[tc] = columns[name2Ind[tc.column]]->Get(ind);
            vals[TableColumn(std::pair<std::string, std::string>{"", tc.column})] = vals[tc]; // try to set if table wasn't spec
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

void Table::Update(parser::Assignments& as, parser::Condition& condWh) {
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
                vals[colAsInd][TableColumn(std::pair<std::string, std::string>{"", tc.column})] = vals[colAsInd][tc]; // try to set if table wasn't spec
            }
        }

        for (int colAsInd = 0; colAsInd < need.size(); colAsInd++) {
            auto tableColInd = name2Ind.find(as.As[colAsInd].TableColNames.second);
            if (tableColInd == name2Ind.end()) {
                // unexpected column name in update
                throw std::runtime_error("error");
                exit(-1);
            }
            columns[tableColInd->second]->update(ind, as.As[colAsInd].Exp.Compute(vals[colAsInd]));
        }
    }
}

void Table::Delete(parser::Condition& condWh) {
    where(condWh);

    bool gotFineInds = false;
    // and are there wait to write base loop
    std::vector<ssize_t> inds;
    columns[0]->getInds(inds);
    /*for (auto it = columns[0]->begin(); it != columns[0]->end(); it++) { // CARE it is safe cause we don't delete full colums and each col must have simular size
        ssize_t ind = *it;
        if (!isFine[ind]) {
            continue;
        }
        inds.push_back(ind);
    }*/

    for (auto ind : inds) { // CARE it is safe cause we don't delete full colums and each col must have simular size
        if (!isFine[ind]) {
            continue;
        }
        for (int colInd = 0; colInd < columns.size(); colInd++) {
            columns[colInd]->del(ind);
        }
    }
}

//TableView(std::map<std::string, sssize_t>& col2ind_, std::vector<Row> columns_): col2ind(col2ind_), columns(columns_)  {}
// Row(std::map<std::string, sssize_t>& col2ind_, std::vector<void*> data_) : col2ind(col2ind_), data(data_){};
TableView Table::Select(const std::vector<TableColumn>& tcs, parser::Condition& condWh) {
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

    ssize_t colInd = 0;
    std::vector<Column::ColumnStructP> tableViewColumns(colAmt, nullptr);
    std::map<std::string, ssize_t> curCol2ind;
    for (const auto& tc : tcs) {
        if (tc.table != tName) {
            continue;
        }
        curCol2ind[tc.column] = colInd;
        tableViewColumns[colInd++] = columns[name2Ind[tc.column]]->GetP();
    }
    // Row(std::map<std::string_view, sssize_t>& col2ind_, std::vector<void*> data_) : col2ind(col2ind_), data(data_){};
    //Row(std::map<std::string_view, sssize_t>& col2ind_, std::vector<void*> data_) : col2ind(col2ind_), data(data_){};
    //std::vector<TableView::Row> data(rowAmt, TableView::Row(name2Ind, std::vector<ssize_t> (colAmt)));
    std::vector<ssize_t> inds;
    inds.reserve(rowAmt);
    //ssize_t curInd = 0;
    for (auto it = columns[0]->begin(); it != columns[0]->end(); it++) { // CARE it is safe cause we don't delete full colums and each col must have simular size
        ssize_t ind = *it;
        if (!isFine[ind]) {
            continue;
        }
        inds.emplace_back(ind);
        
        /*colInd = 0;
        for (const auto& tc : tcs) {
            if (tc.table != tName) {
                continue;
            }
            //data[curInd][colInd] = columns[name2Ind[tc.column]]->GetP(ind);
            data[curInd][colInd] = columns[name2Ind[tc.column]]->GetP(ind);
            colInd++;
        }
        curInd++;*/
    }
    //auto res = TableView(name2Ind, data);
    //std::cout <<&data << ' ' << &res.columns << ' ' << &res.columns[0] << ' ' << &res.col2ind << ' ' << &name2Ind << std::endl;
    return TableView(curCol2ind, inds, tableViewColumns);
}

}