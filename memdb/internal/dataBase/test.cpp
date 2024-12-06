#include "gtest/gtest.h"

#include <cstdio>
#include <random>
#include <vector>
#include <string>

//#include "../../internal/dataBase/db.hpp"
#include "db.hpp"
//#include "../../internal/parser/parser.hpp"
//#include "../../internal/parser/lexer.hpp"
//#define DEBUG

// TODO add check brackets with lots of them (but no need - alg is fixed and don't rely on them amt +-)
using namespace memdb;

std::vector<int32_t> sorted(std::vector<int32_t> v) {
    sort(v.begin(), v.end());
    return v;
}

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
    std::string_view req3 = R"(insert (id=6,login = "vasya2", password_hash = 0xdeadbeefdeadbeef, is_admin=false) to users)";
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
    std::string_view req6 = R"(insert (10,12,11,"name4") to users)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_ANY_THROW(db.Execute(req2));
    EXPECT_ANY_THROW(db.Execute(req3));
    EXPECT_ANY_THROW(db.Execute(req4));
    EXPECT_ANY_THROW(db.Execute(req5));
    EXPECT_NO_THROW(db.Execute(req6));
    EXPECT_ANY_THROW(db.Execute(req6));
}

TEST(ErrParseCheckInsert, 1) {
    Database db;
    std::string_view req1 = "create table users (c1: bytes[2])";
    std::string_view req2 = R"(insert (0xg) to users)";
    std::string_view req3 = R"(insert (0x12345) to users)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_ANY_THROW(db.Execute(req2));
    EXPECT_ANY_THROW(db.Execute(req3));
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
        EXPECT_ANY_THROW(row.Get<int32_t>("id2"));
        EXPECT_EQ(row.Get<int32_t>("id3"), 1);
        EXPECT_EQ(row.Get<std::string>("login"), "name4");
    }
    EXPECT_EQ(ind, 1);
}

struct Request {
    std::string_view req;
    std::vector<int32_t> exp;
};

// teard down tests here? TODO check if it's possible
TEST(CheckSelectString, 0) {  // which cols to get doesn't really matter
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id: int32, c1 : string[1], c2 : string[4], c3 : string[8])";
    std::string_view req2 = R"(insert (,"1", "2345", "67891011") to users)";
    std::string_view req3 = R"(insert (,"2", "23", "45") to users)";
    std::string_view req4 = R"(insert (,"3", "2345", "1303") to users)";
    std::string_view req5 = R"(select id from users where |c1| = 1)";
    std::string_view req6 = R"(select id from users where |c1 + c2| = 5)";
    std::string_view req7 = R"(select id from users where |c2| = |c3|)";
    std::string_view req8 = R"(select id from users where |c1 + "a"| = |c2|)";
    std::string_view req9 = R"(select id from users where |c1 + c2 + c3 + "a"| = 6)";
    std::string_view req10 = R"(select id from users where |c1| = 1 && |c2| = 4 && |c3| = 8)";

    
    std::vector<Request> reqs = {
        {req5, {0, 1, 2}},
        {req6, {0, 2}},
        {req7, {1, 2}},
        {req8, {1}},
        {req9, {1}},
        {req10, {0}}
    };

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    
    for (auto& req : reqs) {
#ifdef DEBUG
        std::cout << req.req << std::endl;
#endif
        TableView res;
        EXPECT_NO_THROW(res = db.Execute(req.req).value());
        std::vector<std::int32_t> colRes;
        for (auto row : res) {
            EXPECT_NO_THROW(colRes.push_back(row.Get<int32_t>("id")));
        }
        EXPECT_EQ(sorted(colRes), req.exp);
    }
}

TEST(CheckSelectString, 1) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id: int32, c1 : string[1], c2 : string[4], c3 : string[8])";
    std::string_view req2 = R"(insert (,"1", "2345", "67891011") to users)";
    std::string_view req3 = R"(insert (,"2", "23", "45") to users)";
    std::string_view req4 = R"(insert (,"3", "2345", "1303") to users)";
    std::string_view req5 = R"(select id from users where c1 + "3" = "23")";
    std::string_view req6 = R"(select id from users where c1 + "3" = c2)";
    std::string_view req7 = R"(select id from users where c1 + c2 + c3 + "0" = c1 + "2345" + c3 + "0")";
    std::string_view req8 = R"(select id from users where c1 = "2" && c3 = "45" && c3 + "0" = c3 + "0")";

    
    std::vector<Request> reqs = {
        {req5, {1}},
        {req6, {1}},
        {req7, {0,2}},
        {req8, {1}},
    };

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    
    for (auto& req : reqs) {
#ifdef DEBUG
        std::cout << req.req << std::endl;
#endif
        TableView res;
        EXPECT_NO_THROW(res = db.Execute(req.req).value());
        std::vector<std::int32_t> colRes;
        for (auto row : res) {
            EXPECT_NO_THROW(colRes.push_back(row.Get<int32_t>("id")));
        }
        EXPECT_EQ(sorted(colRes), req.exp);
    }
}

TEST(CheckSelectInt, 0) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id: int32, c1 : int32, c2 : int32, c3 : int32)";
    std::string_view req2 = R"(insert (,0,1,2) to users)";
    std::string_view req3 = R"(insert (,3,4,5) to users)";
    std::string_view req4 = R"(insert (,6,7,8) to users)";
    std::string_view req5 = R"(select id from users where c2 - c1 = 1)";
    std::string_view req7 = R"(select id from users where c1 + c2 + c3 >= 4)";
    std::string_view req8 = R"(select id from users where c1 * c2 * c3 * c2 != 0)";
    std::string_view req9 = R"(select id from users where c3/c2 = c3)";
    std::string_view req10 = R"(select id from users where c3%c2 = c3/c3)";
    std::string_view req11 = R"(select id from users where c2 < 4)";
    std::string_view req12 = R"(select id from users where c2 <= 4)";
    std::string_view req13 = R"(select id from users where c2 >= 4)";
    std::string_view req14 = R"(select id from users where c2 > 4)";
    std::string_view req15 = R"(select id from users where c1 = 3 && c2 + c3 = 9)";
    std::string_view req16 = R"(select id from users where c1 * c2 * c3 * c2 = 60 * 4)";

    
    std::vector<Request> reqs = {
        {req5, {0, 1, 2}},
        {req7, {1, 2}},
        {req8, {1, 2}},
        {req9, {0}},
        {req10, {1, 2}},
        {req11, {0}},
        {req12, {0,1}},
        {req13, {1,2}},
        {req14, {2}},
        {req15, {1}},
        {req16, {1}},
    };

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    
    for (auto& req : reqs) {
#ifdef DEBUG
        std::cout << req.req << std::endl;
#endif
        TableView res;
        EXPECT_NO_THROW(res = db.Execute(req.req).value());
        std::vector<std::int32_t> colRes;
        for (auto row : res) {
            EXPECT_NO_THROW(colRes.push_back(row.Get<int32_t>("id")));
        }
        EXPECT_EQ(sorted(colRes), req.exp);
    }
}

TEST(CheckSelectBytes, 0) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id: int32, c1 : bytes[2], c2 : bytes[2], c3 : bytes[2])";
    std::string_view req2 = R"(insert (,0x1234,0x12,0x34) to users)";
    std::string_view req3 = R"(insert (,0x1,0x0,0x9) to users)";
    std::string_view req4 = R"(insert (,0xabcd,0xefab,0x8) to users)";
    std::string_view req5 = R"(select id from users where c1 = 0x1234)";
    std::string_view req6 = R"(select id from users where |c1| = 1)";
    std::string_view req7 = R"(select id from users where c1 = 0xabcd && c2=0xefab)";

    
    std::vector<Request> reqs = {
        {req5, {0}},
        {req6, {1}},
        {req7, {2}},
    };

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    
    for (auto& req : reqs) {
#ifdef DEBUG
        std::cout << req.req << std::endl;
#endif
        TableView res;
        EXPECT_NO_THROW(res = db.Execute(req.req).value());
        std::vector<std::int32_t> colRes;
        for (auto row : res) {
            EXPECT_NO_THROW(colRes.push_back(row.Get<int32_t>("id")));
        }
        EXPECT_EQ(sorted(colRes), req.exp);
    }
}

TEST(CheckSelectBool, 0) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id: int32, c1 : bool, c2 : bool, c3 : bool)";
    std::string_view req2 = R"(insert (,true,true,true) to users)";
    std::string_view req3 = R"(insert (,false,false,false) to users)";
    std::string_view req4 = R"(insert (,true,false,false) to users)";
    std::string_view req5 = R"(select id from users where c1 && c2 || c3)";
    std::string_view req6 = R"(select id from users where !c1)";
    std::string_view req7 = R"(select id from users where (c1^^c2^^c3) != false)";
    std::string_view req8 = R"(select id from users where c1||c2)";
    std::string_view req9 = R"(select id from users where c1||c2&&c1)";

    
    std::vector<Request> reqs = {
        {req5, {0}},
        {req6, {1}},
        {req7, {0,2}},
        {req8, {0,2}},
        {req9, {0, 2}},
    };

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    
    for (auto& req : reqs) {
#ifdef DEBUG
        std::cout << req.req << std::endl;
#endif
        TableView res;
        EXPECT_NO_THROW(res = db.Execute(req.req).value());
        std::vector<std::int32_t> colRes;
        for (auto row : res) {
            EXPECT_NO_THROW(colRes.push_back(row.Get<int32_t>("id")));
        }
        EXPECT_EQ(sorted(colRes), req.exp);
    }
}

TEST(CheckDelete, 0) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id : int32, {unique} c1:int32, c2:int32)";
    std::string_view req2 = R"(insert (,0, 1) to users)";
    std::string_view req3 = R"(insert (,1, 2) to users)";
    std::string_view req4 = R"(insert (,0, 2) to users)";
    std::string_view req5 = R"(delete users where id = 0)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req5));
    EXPECT_NO_THROW(db.Execute(req4));
}

TEST(CheckDelete, 1) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id : int32, {unique} c1:int32, c2:int32)";
    std::string_view req2 = R"(insert (,0, 1) to users)";
    std::string_view req3 = R"(insert (,1, 2) to users)";
    std::string_view req4 = R"(insert (,2, 2) to users)";
    std::string_view req5 = R"(delete users where id = 0 || id = 1)";
    std::string_view req6 = R"(select id,c1,c2 from users where true)"; // даже так

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
        std::vector<int32_t> got = {row.Get<int32_t>("c1"), row.Get<int32_t>("c2")};
        if (row.Get<int32_t>("id") == 2) {
            EXPECT_EQ(got[0], 2);
            EXPECT_EQ(got[1], 2);
        } else {
            EXPECT_EQ(row.Get<int32_t>("id"), -1); // or 2  doesn't matter - just throw err
        }
    }
    EXPECT_EQ(ind, 1);
}

TEST(CheckUpdate, 0) {
    Database db;
    std::string_view req1 = "create table users ({autoincrement} id: int32, c1 : int32, c2 : string[10], c3 : bytes[10], c4 : string[20])";
    std::string_view req2 = R"(insert (,123, "456", 0x789, "101112") to users)";
    std::string_view req3 = R"(insert (,123, "456", 0x789, "101112") to users)";
    std::string_view req4 = R"(update users set c1=c1*10+|c2|,c2=c2+c2, c3=0x1, c4=c2+c4+"0" where id = 0)";
    std::string_view req5 = R"(select id from users where c1=1233 && c2="456456" && c3=0x1 && c4="4561011120")";
    std::string_view req6 = R"(select id from users where c1=123 && c2="456" && c3=0x789 && c4="101112")";

    
    std::vector<Request> reqs = {
        {req5, {0}},
        {req6, {1}},
    };

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    
    for (auto& req : reqs) {
#ifdef DEBUG
        std::cout << req.req << std::endl;
#endif
        TableView res;
        EXPECT_NO_THROW(res = db.Execute(req.req).value());
        std::vector<std::int32_t> colRes;
        for (auto row : res) {
            EXPECT_NO_THROW(colRes.push_back(row.Get<int32_t>("id")));
        }
        EXPECT_EQ(sorted(colRes), req.exp);
    }
}

TEST(CheckRegister, 0) {
    Database db;
    std::string_view req1 = "CrEate TaBlE users ({autoiNcrement} id: inT32, c1 : iNt32, c2 : String[10], c3 : bYtes[10], c4 : strIng[20])";
    std::string_view req2 = R"(iNsert (,123, "456", 0x789, "101112") tO users)";
    std::string_view req3 = R"(insErt (,123, "456", 0x789, "101112") To users)";
    std::string_view req4 = R"(uPdate users sEt c1=c1*10+|c2|,c2=c2+c2, c3=0x1, c4=c2+c4+"0" wheRe id = 0)";
    std::string_view req5 = R"(sElect id froM users wHere c1=1233 && c2="456456" && c3=0x1 && c4="4561011120")";
    std::string_view req6 = R"(seleCt id From users Where c1=123 && c2="456" && c3=0x789 && c4="101112")";
    std::string_view req7 = R"(dElete users whEre id = 0)";

    EXPECT_NO_THROW(db.Execute(req1));
    EXPECT_NO_THROW(db.Execute(req2));
    EXPECT_NO_THROW(db.Execute(req3));
    EXPECT_NO_THROW(db.Execute(req4));
    EXPECT_NO_THROW(db.Execute(req5));
    EXPECT_NO_THROW(db.Execute(req6));
    EXPECT_NO_THROW(db.Execute(req7));
}

int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}