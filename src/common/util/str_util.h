#ifndef  __UNIVERSE_UTIL_STR_H__
#define  __UNIVERSE_UTIL_STR_H__

struct ResultStr
{
	char str[256];
};

int SplitStr(char *content, const char *delim, ResultStr result[], int *result_cnt);

#endif // ! __UNIVERSE_UTIL_STR_H__
