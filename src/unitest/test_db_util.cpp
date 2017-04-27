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