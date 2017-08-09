
int gm_add_quests(Actor *actor, int quest_id)
{
	massert_retval(actor != NULL, ERR_INVALID_PARAM);
	int ret = actor_accept_quest(actor, quest_id);
	printf("actor %llu accept quest %d\n", actor->GetId(), quest_id);
	return 0;
}

