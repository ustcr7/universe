#include "command_mgr.h"

static CommandMgr *gs_command_mgr = NULL;

CommandMgr* CommandMgr::GetInstance()
{
	if (gs_command_mgr == NULL)
	{
		gs_command_mgr = new CommandMgr;
		massert_retval(gs_command_mgr != NULL, NULL);
	}
	return gs_command_mgr;
}

int CommandMgr::AddCommand(const char *command_str)
{
	massert_retval(GetCommand(command_str) == NULL, ERR_ALREADY_EXISTS);

	CommandInfo cmd(command_str);
	commands.push_back(cmd);

	return 0;
}

const CommandInfo* CommandMgr::GetCommand(const char *command_str) const
{
	for (std::vector<CommandInfo>::const_iterator iter = commands.begin(); iter != commands.end(); ++iter)
	{
		const CommandInfo &cmd = *iter;
		if (strcmp(cmd.GetCommandStr(), command_str) == 0)
		{
			return &cmd;
		}
	}
	return NULL;
}

const CommandInfo* CommandMgr::GetFirstMatchCommand(const char *command_str, int len) const
{
	for (std::vector<CommandInfo>::const_iterator iter = commands.begin(); iter != commands.end(); ++iter)
	{
		const CommandInfo &cmd = *iter;
		if (strncmp(cmd.GetCommandStr(), command_str, len) == 0)
		{
			return &cmd;
		}
	}
	return NULL;
}