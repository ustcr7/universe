//#ifndef __H_INSTANCE_MGR_H__
//#define __H_INSTANCE_MGR_H__
//
//static const int MAX_ACTOR_IN_AROUND = 1024;
//static const int MAX_AROUND_COUNT = 16;
//static const int AROUND_SIDE_LEN = 3;   //3个GRID构成一个AROUND边长 (3X3=9个grid)
//static const int GRID_SIDE_LEN   = 10;  //10个坐标单位构成一个GRID边长
//
//class Actor;
//
//class Grid
//{
//
//};
//
//class Around
//{
//public:
//	Around()
//	{
//		actor_count = 0;
//		memset(actor_list, 0, sizeof(actor_list));
//	}
//	int AddActor(Actor *actor);
//	int RemoveActor(Actor *actor);
//private:
//	int actor_count;
//	Actor* actor_list[MAX_ACTOR_IN_AROUND];
//};
//
//class Instance 
//{
//public:
//	int EnterActor(Actor *actor);
//	int LeaveActor(Actor *actor);
//	Around* GetAroundByPos(Pos *pos);
//private:
//	u64 mid;
//	int map_id;
//	Around around_list[MAX_AROUND_COUNT][MAX_AROUND_COUNT];
//};
//
//class InstanceMgr
//{
//public:
//	u64 AllocInstance(int map_id);
//	void FreeInstance(u64 instance_mid);
//
//private:
//	std::set<u64> instances;
//};
//
//class Map
//{
//private:
//	int map_id;
//	int max_x_len;
//	int max_y_len;
//};
//
//#endif // !__H_INSTANCE_MGR_H__