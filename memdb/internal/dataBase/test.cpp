#pragma once
#include "gtest/gtest.h"

#include <cstdio>
#include <random>
#include <vector>
#include <string>

//#include "../../internal/dataBase/db.hpp"
#include "db.hpp"
//#include "../../internal/parser/parser.hpp"
//#include "../../internal/parser/lexer.hpp"

using namespace memdb;

TEST(CheckCreateTable, 0) {
    Database db;
    std::string tName = "table1";
    std::vector<ColumnType> colTps = {ColumnType(Type::Int32)};
    std::vector<ColumnFullDescription> colDescripts = {ColumnFullDescription(ColumnType(Type::Int32), ColumnDescription({ColumnAttrs::Default}, "column1"))};
    db.CreateTable(tName, colDescripts);
}

TEST(CheckCreateManyTables, 1) {
    Database db;
    std::string tName = "table1";
    std::vector<ColumnFullDescription> colDescripts = {ColumnFullDescription(ColumnType(Type::Int32), ColumnDescription({ColumnAttrs::Default}, "column1"))};
    db.CreateTable(tName, colDescripts);

    std::string tName2 = "table2";
    std::vector<ColumnFullDescription> colDescripts2 = {ColumnFullDescription(ColumnType(Type::Bool), ColumnDescription({ColumnAttrs::Unique}, "column1"))};
    db.CreateTable(tName2, colDescripts2);
}


TEST(CheckCreateEmptyNameTable, 2) {
    Database db;
    std::string tName = "";
    std::vector<ColumnFullDescription> colDescripts = {ColumnFullDescription(ColumnType(Type::Int32), ColumnDescription({ColumnAttrs::Default}, "column1"))};
    db.CreateTable(tName, colDescripts);
}

//EXPECT_ANY_THROW(db.Insert("col_same", to_add));

TEST(CheckCreateTable, 3) {
    Database db;
    std::string_view req = "create table users ({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)";
    EXPECT_NO_THROW(db.Execute(req));
}

TEST(CheckCreateTable, 4) {
    Database db;
    std::string_view req = "create table users ({key, autoincrement, unique} id_#@. : int32, {} login: string[0], password_hash: bytes[100], { } is_admin: bool = false)";
    EXPECT_NO_THROW(db.Execute(req));
}

TEST(CheckCreateTable, 5) {
    Database db;
    std::string_view req = R"(create table 1234567890QWERTYUIOPASDFGHJKLZXCVBNMqwwertyuiopasdfghjklzxcvbnm@#$;_ ({key, autoincrement, unique} qwwertyuiopasdfghjklzxcvbnm@#$;_ : int32=12323, {} login:string[0]="aba!@#$%^&*{}[]',.;:=+-/\|", password_hash: bytes[100]=0xabcdef1234567890, { } is_admin: bool = false))";
    EXPECT_NO_THROW(db.Execute(req));
}

TEST(CheckCreateTable, 6) {
    Database db;
    std::string_view req = R"(create table users ())";
    EXPECT_NO_THROW(db.Execute(req));
}

TEST(CheckCreateTableBad, 0) {
    Database db;
    std::string_view req = R"(create table users ({,} id : int32))";
    EXPECT_ANY_THROW(db.Execute(req));
    std::string_view req2 = R"(create table users ({} id : int))";
    EXPECT_ANY_THROW(db.Execute(req2));
    std::string_view req3 = R"(create table users ({} id : int32=""))";
    EXPECT_ANY_THROW(db.Execute(req3));
    std::string_view req4 = R"(create table users ({unique key} id : int32))";
    EXPECT_ANY_THROW(db.Execute(req4));
}

TEST(SmokeCheckInsert, 0) {
    Database db;
    std::string_view req1 = "create table users ({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)";
    std::string_view req2 = R"(insert (,"vasya", 0xdeadbeefdeadbeef,) to users)";
    std::string_view req3 = R"(insert (id=6,login = "vasya", password_hash = 0xdeadbeefdeadbeef, is_admin=false) to users)";
    std::string_view req4 = R"(insert (,login = "petyapetya", password_hash = 0xdeadbeefdeadbeef,) to users)";
    std::string_view req5 = R"(insert (10,"admin", 0x0000000000000000, true) to users)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    EXPECT_NO_THROW(db.Execute(req5));
}

TEST(SmokeCheckInsert, 1) {
    Database db;
    std::string_view req1 = "create table users ({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)";
    std::string_view req2 = R"(insert (,"", 0xdeadbeefdeadbeef,) to users)";
    std::string_view req3 = R"(insert (id=6,login = "12345678901234567890123456789012", password_hash = 0xded, is_admin=false) to users)";
    std::string_view req4 = R"(insert (,login = "petyapetya", password_hash = 0xdeadbeefdeadbeef,) to users)";
    std::string_view req5 = R"(insert (10,"admin", 0x0000000000000000, true) to users)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    EXPECT_NO_THROW(db.Execute(req5));
}

TEST(SmokeCheckInsert, 2) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id : int32, {autoincrement} id2 : int32, id3 : int32=1, {unique} login: string[32])";
    std::string_view req2 = R"(insert (,,,"name1") to users)";
    std::string_view req3 = R"(insert (id=10,,,login = "name2") to users)";
    std::string_view req4 = R"(insert (10,,11,"name3") to users)";
    std::string_view req5 = R"(insert (,10,,"name4") to users)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    EXPECT_NO_THROW(db.Execute(req5));
}

TEST(ErrParseCheckInsert, 0) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id : int32, {autoincrement} id2 : int32, id3 : int32=1, {unique} login: string[32])";
    std::string_view req2 = R"(insert (,,"name1") to users)";
    std::string_view req3 = R"(insert (,,,id=10,login = "name2") to users)";
    std::string_view req4 = R"(insert (id=10,12,11,"name3") to users)";
    std::string_view req5 = R"(insert ("10",12,11,"name4") to users)";
    std::string_view req6 = R"(insert ("10",12,11,"name4") to users)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_ANY_THROW(db.Execute(req2));
    EXPECT_ANY_THROW(db.Execute(req3));
    EXPECT_ANY_THROW(db.Execute(req4));
    EXPECT_ANY_THROW(db.Execute(req5));
    EXPECT_ANY_THROW(db.Execute(req6));
}

TEST(CheckSelect, 0) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id : int32, {autoincrement} id2 : int32, id3 : int32=1, {unique} login: string[32])";
    std::string_view req2 = R"(insert (,,,"name1") to users)";
    std::string_view req3 = R"(insert (id=10,,,login = "name2") to users)";
    std::string_view req4 = R"(insert (10,,11,"name3") to users)";
    std::string_view req5 = R"(insert (,10,,"name4") to users)";
    std::string_view req6 = R"(select id, id3, login from users where id=11)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    EXPECT_NO_THROW(db.Execute(req5));
    TableView res;
    EXPECT_NO_THROW(res = db.Execute(req6).value());
    int ind = 0;
    for (auto row : res) {
        ind++;
        EXPECT_EQ(row.Get<int32_t>("id"), 11);
        EXPECT_EQ(row.Get<int32_t>("id2"), 10);
        EXPECT_EQ(row.Get<int32_t>("id3"), 12);
        EXPECT_EQ(row.Get<std::string>("login"), "name4");
    }
    EXPECT_EQ(ind, 1);
}

int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}