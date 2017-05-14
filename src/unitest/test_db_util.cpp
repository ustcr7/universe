#include <stdio.h>
#include "ring_buffer/ring_buffer.h"
#include "errcode.h"
#include "massert.h"
#include "db/db_util.h"
#include <stdio.h>
using namespace universe;

int main()
{
    UniverseDbUtil *db_util = UniverseDbUtil::GetInstance();
	massert_retval(db_util != NULL, -1);

	int ret = db_util->Init();
	massert_retval(ret == 0, ret);

	db_util->Fini();

	return 0;
}

/*
create user 'wcc'@'localhost' identified by '123456';
create database test_db;
grant all on test_db.* to wcc@'localhost';

create table actor_name(
id int(4) not null primary key auto_increment,
name char(20) not null);

insert into actor_name values(1, "wcc");
*/