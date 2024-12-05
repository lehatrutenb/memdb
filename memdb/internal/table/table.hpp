#pragma once
#include "../type/type.hpp"
#include "../column/column.hpp"
#include "../parser/condition.hpp"
#include "../parser/assignments.hpp"
#include <map>
#include <string_view>
#include <numeric>
#include <iostream>

// it is not really max size - just size that will be allocated since start
namespace memdb {

struct TableView {
    public:
    class Row {
        public:
        Row(std::map<std::string, ssize_t> col2ind_, ssize_t ind_, std::vector<Column::ColumnStructP> colPs_) : rcol2ind(col2ind_), ind(ind_), rcolPs(colPs_) {};
        template<typename T>
        T& Get(std::string colName) {
            if (rcol2ind.find(colName) == rcol2ind.end()) {
                // throw ex
                throw std::runtime_error("error");
                exit(-1);
            }
            return static_cast<DbType_s<T>*>(rcolPs[rcol2ind[colName]])->get(ind);
        }

        //void*& operator[](ssize_t ind); what for?

        private:
            std::map<std::string, ssize_t> rcol2ind;
            std::vector<Column::ColumnStructP> rcolPs;
            ssize_t ind;
    };

    private:
    class Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Row;
        using pointer           = Row*;
        using reference         = Row&;
    public:
        explicit Iterator(std::vector<Row>::iterator now_);
        explicit Iterator() = default;

        reference operator*();
        pointer operator->();
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& o) const;
        bool operator!=(const Iterator& o) const;

    private:
        std::vector<Row>::iterator now;
    };

    public:
    
    TableView();
    TableView(std::map<std::string, ssize_t> col2ind_, std::vector<ssize_t> colInds_, std::vector<Column::ColumnStructP> colPs_);

    std::map<std::string, ssize_t> col2ind;
    std::vector<ssize_t> colInds;
    std::vector<Row> readyRows;
    std::vector<Column::ColumnStructP> colPs;

    Iterator begin();
    Iterator end();
};

class Table {
static inline std::vector<bool> isFine;
public:
    struct Value {
        Value(const std::shared_ptr<DbType>& fValue_);
        Value(const std::string& fName_, const std::shared_ptr<DbType>& fValue_);
        const std::string fName;
        const std::shared_ptr<DbType> fValue;
    };

    Table(const std::string_view& tName_);
    void AddColumn(const ColumnFullDescription& descr);

    void Insert(const std::vector<Value>& values);

    bool checkCondNeed(std::vector<TableColumn>& need);

    ssize_t where(parser::Condition& cond);

    void Update(parser::Assignments& as, parser::Condition& condWh);

    void Delete(parser::Condition& condWh);

    TableView Select(const std::vector<TableColumn>& tcs, parser::Condition& condWh);

private:
    std::string tName;
    std::vector<std::shared_ptr<Column>> columns;
    std::map<std::string, ssize_t> name2Ind;
};

}