#pragma once
#include "gtest/gtest.h"

#include <cstdio>
#include <random>
#include <vector>
#include <string>

#include "../../internal/dataBase/db.cpp"

using namespace memdb;

TEST(CheckCreateTable, 0) {
    Database db;
    const std::string_view tName = "table1";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);
}

TEST(CheckCreateManeTables, 1) {
    Database db;
    const std::string_view tName = "table1";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    const std::string_view tName = "table2";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);
}


TEST(CheckCreateTable, 2) {
    Database db;
    const std::string_view tName = "";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);
}

TEST(CheckCreateColumn, 0) {
    Database db;
    const std::string_view tName = "table1";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    std::shared_ptr<DbType> value{new DbInt32(123)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("column1", to_add);

    std::shared_ptr<DbType> value{new DbInt32(321)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("column2", to_add);
}

TEST(CheckCreateManeColumnsManyTables, 1) {
    Database db;
    const std::string_view tName = "table1";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    const std::string_view tName = "table2";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    std::shared_ptr<DbType> value{new DbInt32(123)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("column1", to_add);

    std::shared_ptr<DbType> value{new DbInt32(321)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("column2", to_add);
}


TEST(CheckZeroNamedColumn, 3) {
    Database db;
    const std::string_view tName = "table";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    std::shared_ptr<DbType> value{new DbInt32(123)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("", to_add);

    std::shared_ptr<DbType> value{new DbInt32(321)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("table1", to_add);
}

TEST(CheckCreateTableSame, 4) {
    Database db;
    const std::string_view tName = "table_same";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    const std::string_view tName = "table_same";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    EXPECT_ANY_THROW(db.CreateTable(tName, colTps, colDescripts));
}

TEST(CheckCreateColumnSame, 4) {
    Database db;
    const std::string_view tName = "table";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnDescription> colDescripts = {ColumnDescription({ColumnAttrs::Default}, "column1")};
    db.CreateTable(tName, colTps, colDescripts);

    std::shared_ptr<DbType> value{new DbInt32(123)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    db.Insert("col_same", to_add);

    std::shared_ptr<DbType> value{new DbInt32(321)};
    std::vector<Table::Value> to_add = {Table::Value(value)};
    EXPECT_ANY_THROW(db.Insert("col_same", to_add));
}


/*
std::string runProgrammGetOutput(std::string code) {
    std::ofstream file("input");
    file << code;
    file.close();
    
    std::ostringstream output;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    emulator::Emulator emulator("./input");
    emulator.run();

    std::cout.rdbuf(cout_buff);

    return output.str();
}


TEST(CheckSmallProgramm, 0) {
    EXPECT_EQ(runProgrammGetOutput("BEGIN PUSH 123 OUT END"), "123");
    EXPECT_EQ(runProgrammGetOutput("BEGIN PUSH 123 POPR reg0 PUSHR reg0 PUSHR reg0 POP OUT END"), "123");
    EXPECT_EQ(runProgrammGetOutput("BEGIN PUSH -123 PUSH 2 MUL OUT END"), "-246");
    EXPECT_EQ(runProgrammGetOutput("BEGIN PUSH 2 PUSH 10 DIV PUSH 100 SUB OUT END"), "95");
}

TEST(CheckJmp, 0) {
    EXPECT_EQ(runProgrammGetOutput("BEGIN JMP skipCode PUSH 1 OUT skipCode PUSH 2 OUT END"), "2");
    EXPECT_EQ(runProgrammGetOutput("goBack JMP goEnd BEGIN JMP skipCode PUSH 1 OUT skipCode JMP goBack PUSH 2 OUT goEnd END"), "");
    EXPECT_EQ(runProgrammGetOutput("BEGIN PUSH 0 PUSH 1 PUSH 2 JNE goEnd OUT goEnd END"), "");
    EXPECT_EQ(runProgrammGetOutput("BEGIN PUSH 0 PUSH 1 PUSH 2 JEQ goEnd OUT goEnd END"), "0");
}

TEST(CheckFunc, 0) {
    EXPECT_EQ(runProgrammGetOutput("func PUSH 2 POPR reg0 RET BEGIN CALL func PUSHR reg0 OUT END"), "2");
}

TEST(CheckFuncCallFunc, 0) {
    EXPECT_EQ(runProgrammGetOutput("func PUSH 123 CALL func2 OUT RET BEGIN CALL func END func2 PUSH 321 OUT RET"), "321123");
}

TEST(CheckRecursion, 0) {
    EXPECT_EQ(runProgrammGetOutput("funcRec PUSHR reg0 OUT PUSH 1 PUSHR reg0 SUB POPR reg0 PUSHR reg0 PUSH 0 JAE goToRet CALL funcRec goToRet RET BEGIN PUSH 5 POPR reg0 CALL funcRec END"),
                                   "54321");
}


TEST(CheckIn, 0) {
    std::ofstream file("input");
    file << "BEGIN IN OUT END";
    file.close();

    std::istringstream input;
    input.str("123");
    auto cin_buff = std::cin.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    
    std::ostringstream output;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    emulator::Emulator emulator("./input");
    emulator.run();

    std::cout.rdbuf(cout_buff);
    std::cin.rdbuf(cin_buff);

    EXPECT_EQ(output.str(), "123");
}
*/
/*
int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}*/