#ifndef __H_INSTANCE_MGR_H__
#define __H_INSTANCE_MGR_H__
#include "../../common/base_type.h"
#include <set>

//坐标点:   位置的最小单位 移动时以坐标点为单位  (CM)
//GRID:     100 CM * 100 CM的方格 阻挡等按GRID来配置
//AROUND：  3 * 3 GRID的区域  视野同步管理用AROUND为单位
//MAP;      地图

static const int MAX_ACTOR_IN_AROUND   = 1024;
static const int MAX_ACTOR_IN_INSTANCE = 5000;
static const int MAX_AROUND_COUNT      = 16;

static const int GRID_SIDE_LEN   = 100;  //10个坐标单位构成一个GRID边长
static const int AROUND_SIDE_LEN = 3;    //3个GRID构成一个AROUND边长 (3X3=9个grid)
static const int AROUND_WIDTH = GRID_SIDE_LEN * AROUND_SIDE_LEN;   //AOURND 宽度
static const int AROUND_HEIGHT = GRID_SIDE_LEN * AROUND_SIDE_LEN;  //AROUND 高度

class Actor;


class Around
{
public:
	Around();
	int AddActor(Actor *actor);
	int RemoveActor(Actor *actor);
	int DumpAllActors(Actor **actorArr, int *actorCnt);
private:
	int actor_count;
	Actor* actor_list[MAX_ACTOR_IN_AROUND];
};

class Instance 
{
public:
	Instance()
		: mid(0)
		, map_id(0)
		, actor_count(0)
	{

	}
public:
	int EnterActor(Actor *actor);
	int LeaveActor(Actor *actor);
	Around* GetAroundByPos(const Pos *pos);

	void SetInstanceMid(u64 id)
	{
		mid = id;
	}

	int GetMapId();
	void SetMapId(int id)
	{
		map_id = id;
	}

	int GetActorCount();
	int DecActorCount();
	int IncActorCount();
private:
	u64 mid;
	int map_id;
	Around around_list[MAX_AROUND_COUNT][MAX_AROUND_COUNT];

	int actor_count;
};

class EnterInstanceParam
{
public:
	EnterInstanceParam() :
		map_id(0), instance_id(0)
	{
	}
	void SetMapId(int id)
	{
		map_id = id;
	}
	int GetMapId()
	{
		return map_id;
	}
	void SetInstanceId(u64 id)
	{
		instance_id = id;
	}
	u64 GetInstanceId()
	{
		return instance_id;
	}

private:
	int map_id;
	u64 instance_id;
};

//副本管理器
class InstanceMgr
{
public:
	static InstanceMgr*  GetSingleInstance();
	u64           AllocInstance(int map_id);
	void          FreeInstance(u64 instance_id);
	Instance*     GetInstanceById(u64 instance_id);

	int EnterActor(Actor *actor, EnterInstanceParam *enter_param);
	int LeaveActor(Actor *actor);

private:
	std::map<u64, Instance*> instances;
};

class MapInfo
{
public:
	MapInfo() 
		:map_id(0)
		, max_x_len(0)
		, max_y_len(0)
		, max_actor_per_inst(0)
	{
	}
	MapInfo(int id, int x_len, int y_len, int max_actor)
		:map_id(id)
		, max_x_len(x_len)
		, max_y_len(y_len)
		, max_actor_per_inst(max_actor)
	{
	}
	int GetMapId();
	int GetMaxActorPerInstance();
private:
	int map_id;
	int max_x_len;
	int max_y_len;

	//负载相关数据
	int max_actor_per_inst; //单个副本最大任务
};

//地图管理器
class MapMgr
{
public:
	static MapMgr* GetSingleInstance();
	MapInfo* GetMapById(int map_id);
	int InitAllMapInfo(); //WCC_TODO  map信息正常应该是通过资源文件来生成,这里为了方便先在内存中手动构造几个
private:
	std::map<int, MapInfo> game_maps;
};

#endif // !__H_INSTANCE_MGR_H__