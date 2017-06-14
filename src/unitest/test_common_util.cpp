#include "util/str_util.h"
#include "massert.h"
#include <string>
#include <string.h>
int main()
{
	char example_str[] = "send_cha_msg 1234567 hello_1234567!!";

	char content[128];
	strcpy(content, example_str);
	char delim[] = " ";
	ResultStr result[10];
	int result_cnt = sizeof(result) / sizeof(result[0]);
	int ret = SplitStr(content, delim, result, &result_cnt);
	massert_retval(ret == 0, ret);
	printf("content:[%s] split result:\n", example_str);
	for (int i = 0; i < result_cnt; ++i)
	{
		printf("%s\n", result[i].str);
	}

	return 0;
}