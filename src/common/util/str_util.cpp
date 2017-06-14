#include "str_util.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/net/tcp_server.h"
#include <cstring>

int SplitStr(char *content, const char *delim, ResultStr result[], int *result_cnt)
{
	//int ret = 0;
	massert_retval(content != NULL && delim != NULL && result != NULL && result_cnt != NULL, ERR_INVALID_PARAM);

	int max_result_cnt = *result_cnt;
	*result_cnt = 0;

	char *pch = NULL;
	pch = strtok(content, delim);
	while (pch != NULL)
	{
		if (*result_cnt > max_result_cnt)
		{
			break;
		}
		strcpy(result[*result_cnt].str, pch);  //WCC_TODO: not safe
		*result_cnt = *result_cnt + 1;
		pch = strtok(NULL, delim);
	}

	return 0;
}