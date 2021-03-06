#include "gamesvr_msg_processer.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/net/tcp_server.h"
#include <string.h>
#include "universe_cs.pb.h"
#include "actor_logic.h"
#include "actor.h"
#include "instance_mgr.h"
#include "gamesvr_def.h"


Around::Around()
{
	actor_count = 0;
	memset(actor_list, 0, sizeof(actor_list));
}

int Around::AddActor(Actor *actor)
{
	for (int i = 0; i < actor_count; ++i)
	{
		if (actor_list[i] == actor)
		{
			massert_retval(0, -1);
		}		
	}
	massert_retval(actor_count < MAX_ACTOR_IN_AROUND, -1);
	actor_list[actor_count] = actor;
	++actor_count;

	//BC to around actors
	return 0;
}

int Around::RemoveActor(Actor *actor)
{
	int index = -1;
	for (int i = 0; i < actor_count; ++i)
	{
		if (actor_list[i] == actor)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		massert_retval(0, -1);
	}

	actor_list[index] = actor_list[actor_count - 1];
	--actor_count;

	//BC to around actors
	return 0;
}

int Around::DumpAllActors(Actor **actorArr, int *actorCnt)
{
	//int ret = 0;
	massert_retval(actorArr != NULL && actorCnt != NULL, ERR_INVALID_PARAM);

	int maxCnt = *actorCnt;
	*actorCnt = 0;

	for (int i = 0; i < actor_count && *actorCnt < maxCnt; ++i)
	{
		actorArr[*actorCnt] = actor_list[i];
		*actorCnt = *actorCnt + 1;
	}

	return 0;
}

int Instance::EnterActor(Actor *actor)
{
	int ret = 0;
	Pos init_pos(0, 0);
	ret = actor->SetPos(&init_pos);
	massert_retval(ret == 0, ret);

	Around* enter_around = GetAroundByPos(&init_pos);
	massert_retval(enter_around != NULL, -1);
	enter_around->AddActor(actor);

	actor->SetInstanceId(mid);

	IncActorCount();

	printf("actor:%llu(name:%s) enter instance:%llu(mapid:%d) success, cur actor count:%d\n"
		, actor->GetId()
		, actor->GetName()
		, mid
		, map_id
	    , actor_count);

	return 0;
}

int Instance::LeaveActor(Actor *actor)
{
	massert_retval(actor != NULL, ERR_INVALID_PARAM);
	const Pos *pos = actor->GetPos();
	massert_retval(pos != NULL, ERR_INVALID_PARAM);

	Around* enter_around = GetAroundByPos(pos);
	massert_retval(enter_around != NULL, -1);
	enter_around->RemoveActor(actor);

	actor->SetInstanceId(0);

	DecActorCount();

	printf("actor:%llu(name:%s) leave instance:%llu(mapid:%d) success, cur actor count:%d\n"
		, actor->GetId()
		, actor->GetName()
		, mid
		, map_id
		, actor_count);

	return 0;
}

bool Instance::IsActorInInstance(const Actor *actor) const
{
	massert_retval(actor != NULL, false);

	return actor->GetInstanceId() == GetInstanceMid();
}

int Instance::GetActorDistance(const Actor *lhs_actor, const Actor *rhs_actor) const
{
	if(!IsActorInInstance(lhs_actor) || !IsActorInInstance(rhs_actor))
	{
	    return ERR_INST_NOT_IN_INSTANCE;
	}

	return lhs_actor->GetPos()->GetDistanceFromPos(rhs_actor->GetPos());
}

Around* Instance::GetAroundByPos(const Pos *pos)
{
	int around_x = pos->GetX() / AROUND_WIDTH;
	int around_y = pos->GetY() / AROUND_HEIGHT;

	massert_retval(around_x < MAX_AROUND_COUNT && around_y < MAX_AROUND_COUNT, NULL);

	return &around_list[around_x][around_y];
}
 
int Instance::GetMapId() const 
{
	return map_id;
}

int Instance::GetActorCount() const
{
	return actor_count;
}

int Instance::DecActorCount()
{
	--actor_count;
	return 0;
}
int Instance::IncActorCount()
{
	++actor_count;
	return 0;
}

static InstanceMgr *gs_instance_mgr = NULL;
InstanceMgr*  InstanceMgr::GetSingleInstance()
{
	if (gs_instance_mgr == NULL)
	{
		gs_instance_mgr = new InstanceMgr;
	}
	return gs_instance_mgr;
}
u64 InstanceMgr::AllocInstance(int map_id)
{
	static u64 global_instance_id = 0;
	++global_instance_id;

	Instance *inst = new Instance();
	massert_retval(inst != NULL, 0);
	inst->SetMapId(map_id);
	inst->SetInstanceMid(global_instance_id);

	instances.insert(std::make_pair(global_instance_id, inst));

	return global_instance_id;
}

void InstanceMgr::FreeInstance(u64 instance_mid)
{
	Instance* inst = GetInstanceById(instance_mid);
	massert_retnone(inst != NULL);

	//some check
	if (inst->GetActorCount() != 0)
	{
		printf("fatal error inst %llu still has actor:%d\n", instance_mid, inst->GetActorCount());
		massert_noeffect(0);
	}

	delete inst;

	instances.erase(instance_mid);
}

Instance* InstanceMgr::GetInstanceById(u64 instance_id)
{
	std::map<u64, Instance*>::iterator iter = instances.find(instance_id);
	if (iter == instances.end())
	{
		return NULL;
	}

	return iter->second;
}

//WCC_TODO:目前只有一种负载均衡策略:一向想一个副本分配玩家,直到副本达到最大人数
Instance*     InstanceMgr::GetInstanceByMapid(u64 map_id)
{
	MapMgr *mapMgr = MapMgr::GetSingleInstance();
	MapInfo *mapInfo = mapMgr->GetMapById(map_id);
	massert_retval(mapInfo != NULL, NULL);
	int max_actor_per_inst = mapInfo->GetMaxActorPerInstance();

	for (std::map<u64, Instance*>::iterator iter = instances.begin()
		; iter != instances.end()
		; ++iter)
	{
		Instance *inst = iter->second;
		if (inst->GetMapId() != (int)map_id)
		{
			continue;
		}
		if (inst->GetActorCount() >= max_actor_per_inst)
		{
			continue;
		}

		//other condition

		return inst;
	}

	return NULL;
}


int InstanceMgr::EnterActor(Actor *actor, EnterInstanceParam *enter_param)
{
	massert_retval(actor != NULL && enter_param != NULL, ERR_INVALID_PARAM);

	Instance *inst = NULL;

	do 
	{
		if (enter_param->GetInstanceId() != 0)
		{
			inst = GetInstanceById(enter_param->GetInstanceId());
			massert_retval(inst != NULL, ERR_INVALID_PARAM);
			int map_id = inst->GetMapId();
			MapMgr *mapMgr = MapMgr::GetSingleInstance();
			massert_retval(mapMgr != NULL, ERR_UNKNOWN);
			MapInfo *mapInfo = mapMgr->GetMapById(map_id);
			massert_retval(mapInfo != NULL, ERR_UNKNOWN);
			if (inst->GetActorCount() < mapInfo->GetMaxActorPerInstance())
			{
				break;  //副本剩余人数充足,允许玩家进入
			}
			else
			{
				massert_retval(0, ERR_NOT_ENOUGH);
			}
		}

		int map_id = enter_param->GetMapId();
		inst = GetInstanceByMapid(map_id);
		if (inst != NULL)
		{
			break;
		}
		
		massert_retval(enter_param->GetMapId() > 0, ERR_INVALID_PARAM);
		u64 instance_id = AllocInstance(map_id);
		massert_retval(instance_id != 0 && instance_id != INVALID64, ERR_BAD_ALLOC);
		inst = GetInstanceById(instance_id);
	} while (0);

	massert_retval(inst != NULL, ERR_BAD_ALLOC);
	inst->EnterActor(actor);

	return 0;
}

int InstanceMgr::LeaveActor(Actor *actor)
{
	printf("for debug actor:%llu instance id:%llu\n", actor->GetId(), actor->GetInstanceId());
	Instance *inst = GetInstanceById(actor->GetInstanceId());
	massert_retval(inst != NULL, ERR_INVALID_PARAM);
	inst->LeaveActor(actor);

	return 0;
}

int  MapInfo::GetMaxActorPerInstance()
{
	return max_actor_per_inst;
}

int MapInfo::GetMapId()
{
	return map_id;
}

int MapMgr::InitAllMapInfo()
{
	MapInfo m1(10000, 1024, 768, 1024);
	MapInfo m2(10001, 512,  512, 30);

	game_maps.insert(std::make_pair(m1.GetMapId(), m1));
	game_maps.insert(std::make_pair(m2.GetMapId(), m2));

	return 0;
}

MapInfo* MapMgr::GetMapById(int map_id)
{
	std::map<int, MapInfo>::iterator iter = game_maps.find(map_id);
	if (iter == game_maps.end())
	{
		return  NULL;
	}

	return &(iter->second);
}

static MapMgr *gs_map_mgr = NULL;
MapMgr* MapMgr::GetSingleInstance()
{
	if (gs_map_mgr == NULL)
	{
		gs_map_mgr = new MapMgr();
		massert_retval(gs_map_mgr != NULL, NULL);
	}

	return gs_map_mgr;
}