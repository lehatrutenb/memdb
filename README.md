формальное и полное описание приведено в memdb.pdf

## Короткое описание:
In-memmory база данных с таблицами и колонками поддерживающая типы int32, bytes[], string, bool

Запросы sql-подобные c самописным их парсером - для анализа выражений строилось абстрактное синтаксическое дерево

## Поддерживаемые запросы:
#### create table `имя таблицы` (описание колонок)
Описание колонок - последовательность резделённых запятой описаний вида: [{attributes}] `name`: `type` [= `value`]

#### insert (`values`) to `table`
#### select `columns` from `table` where `condition`
#### update `table` set `assignments` where `condition`
#### delete `table` where `condition`

### различные примеры:

create table users ({autoincrement} id: int32, c1 : int32, c2 : string[10], c3 : bytes[10], c4 : string[20])

create table users ({autoincrement} id : int32, {unique} c1:int32, c2:int32)

insert (10,,11,"name3") to users

insert (,login = "petyapetya", password_hash = 0xdeadbeefdeadbeef,) to users

select id from users where |c1 + c2 + c3 + "a"| = 6

select id from users where c1 + c2 + c3 + "0" = c1 + "2345" + c3 + "0"

select id from users where c1 + c2 + c3 >= 4

select id from users where c1||c2&&c1

select id from users where c1=1233 && c2="456456" && c3=0x1 && c4="4561011120"

delete users where id = 0 || id = 1

update users set c1=c1*10+|c2|,c2=c2+c2, c3=0x1, c4=c2+c4+"0" where id = 0
