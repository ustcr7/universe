//#include "instance_mgr.h"
//
//
//int Around::AddActor(Actor *actor)
//{
//	for (int i = 0; i < actor_count; ++i)
//	{
//		if (actor_list[i] == actor)
//		{
//			massert_retval(0, -1);
//		}		
//	}
//	massert_retval(actor_count < MAX_ACTOR_IN_AROUND, -1);
//	actor_list[actor_count] = actor;
//	++actor_count;
//
//	//BC to around actors
//}
//
//int Around::RemoveActor(Actor *actor)
//{
//	int index = -1;
//	for (int i = 0; i < actor_count; ++i)
//	{
//		if (actor_list[i] == actor)
//		{
//			index = i;
//			break;
//		}
//	}
//	if (index == -1)
//	{
//		massert_retval(0, -1);
//	}
//
//	actor_list[index] = actor_list[actor_count - 1];
//	--actor_count;
//
//	//BC to around actors
//	return 0;
//}
//
//int Instance::EnterActor(Actor *actor)
//{
//	int ret = 0;
//	Pos init_pos(100, 100);
//	ret = actor->SetPos(&init_pos);
//	massert_retval(ret == 0, ret);
//
//	Around* enter_around = GetAroundByPos(&init_pos);
//	massert_retval(enter_aournd != NULL, -1);
//	enter_around->AddActor(actor);
//
//	return 0;
//}
//
//int Instance::LeaveActor(Actor *actor)
//{
//
//}
//
//Around* Instance::GetAroundByPos(Pos *pos)
//{
//	int x = pos->x;
//	int y = pos->y;
//
//	int grid_x = (pos->x) / GRID_SIDE_LEN;
//	int grid_y = (pos->y) / GRID_SIDE_LEN;
//
//	int around_x = grid_x / AROUND_SIDE_LEN;
//	int around_y = grid_y / AROUND_SIDE_LEN;
//
//	massert_retval(around_x < MAX_AROUND_COUNT && around_y < MAX_AROUND_COUNT, -1);
//
//	return &around_list[around_x][around_y];
//}
//
//u64 InstanceMgr::AllocInstance(int map_id);
//void InstanceMgr::FreeInstance(u64 instance_mid);
