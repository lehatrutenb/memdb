#pragma once
#include "gtest/gtest.h"

#include <cstdio>
#include <random>
#include <vector>
#include <string>

#include "../../internal/dataBase/db.cpp"

using namespace memdb;

/*
create table users ({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)


insert (,"vasya", 0xdeadbeefdeadbeef) to users
insert (login = "vasya", password_hash = 0xdeadbeefdeadbeef) to users
insert (,"admin", 0x0000000000000000, true) to users
insert (
         is_admin = true,
         login = "admin",
         password_hash = 0x0000000000000000
     ) to users


select id, login from users where is_admin || id < 10

update users set is_admin = true where login = "vasya"
update users set login = login + "_deleted", is_admin = false where password_hash < 0x00000000ffffffff

delete users where |login| % 2 = 0

create ordered index on users by login create unordered index on users by is_admin

*/

TEST(CheckCreateTable, 0) {
    Database db;
    db.Execute("sdfdsf");
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