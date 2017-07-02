#ifndef __H_UNIVERSE_COMMAND_MGR_H__
#define __H_UNIVERSE_COMMAND_MGR_H__

#include "../../common/massert.h"
#include "../../common/errcode.h"
#include <vector>

class CommandInfo
{
public:
	CommandInfo(const char *str)
	{
		SetCommandStr(str);
	}
	const char* GetCommandStr() const
	{
		return command_str;
	}
	int SetCommandStr(const char*str)
	{
		massert_retval(str != NULL, ERR_INVALID_PARAM);
		strncpy(command_str, str, sizeof(command_str));
	}
private:
	char command_str[128];
};


class CommandMgr
{
public:
	CommandMgr* GetInstance();
	int AddCommand(const char *command_str);
	const CommandInfo* GetCommand(const char *command_str) const;
	const CommandInfo* GetFirstMatchCommand(const char *command_str, int len) const;

private:
	static std::vector<CommandInfo> commands;
};

#endif