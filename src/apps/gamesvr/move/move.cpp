#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/timer/uv_timer.h"
#include "../actor.h"
#include "../actor_mgr.h"
#include <cmath>
#include <cstdlib>
struct MoveResult
{
	bool reach_end;       //是否到达终点
	Pos stop_pos;        //最终停留的点坐标
	int left_move;   //移动停止时剩余的长度
};

int ActorMoveTimeout(const char *callback_data, u32 callback_data_len);

int WalkAlong(const Pos *cur_pos, const Pos *dst_pos, int max_move, MoveResult *result)
{
	//printf("walk src<%d-%d> dst<%d-%d>\n"		, cur_pos->GetX(), cur_pos->GetY(), dst_pos->GetX(), dst_pos->GetY());
	int diff_x = dst_pos->GetX() - cur_pos->GetX();
	int diff_y = dst_pos->GetY() - cur_pos->GetY();
	int dist = sqrt(diff_x*diff_x + diff_y*diff_y);

	if (max_move < dist)  //移动距离内不会到达该段终点
	{
		double cos_a = (dst_pos->GetX() - cur_pos->GetX()) / (float)dist; //WCC_TODO不要用浮点数
		double sin_a = (dst_pos->GetY() - cur_pos->GetY()) / (float)dist; //WCC_TODO不要用浮点数
		int new_pos_x = cur_pos->GetX() + max_move * cos_a;
		int new_pos_y = cur_pos->GetY() + max_move * sin_a;

		result->reach_end = false;
		result->stop_pos.SetX(new_pos_x);
		result->stop_pos.SetY(new_pos_y);
		result->left_move = 0;
		//printf("move %d, dst dist:%d, stop pos<%d-%d>\n", max_move, dist, new_pos_x, new_pos_y);
		return 0;
	}

	result->reach_end = true;
	result->left_move = max_move - dist;
	result->stop_pos = *dst_pos;

	return 0;
}

int AddActorMoveTimer(const Actor *actor, int delay_interval)
{
	//时间管理器
	UvTimer *timer = UvTimer::GetSingleInstance();
	massert_retval(timer != NULL, ERR_BAD_ALLOC);
	u64 mid = actor->GetId();
	timer->AddTimer(delay_interval, ActorMoveTimeout, (const char*)&mid, sizeof(mid));

	return 0;
}

int ActorMoveTimeout(const char *callback_data, u32 callback_data_len)
{
	int ret = 0;
	u64 actor_mid = *(u64*)callback_data;

	//printf("actor:%llu move time out\n", actor_mid);

	ActorMgr *actorMgr = ActorMgr::GetSingleInstance();
	massert_retval(actorMgr != NULL, ERR_BAD_ALLOC);

	Actor *actor = actorMgr->GetActorById(actor_mid);
	massert_retval(actor != NULL, ERR_NOT_FOUND);

	Pos cur_pos = *(actor->GetPos());
	int cur_path_index = actor->GetCurPathIndex();
	const MovePath *movePath = actor->GetMovePath();
	massert_retval(cur_path_index >= 0 && cur_path_index < movePath->path_count, ERR_INVALID_PARAM);

	int max_move = actor->GetSpeed() * 0.5;   //计算玩家0.5秒内最多移动距离
	//printf("max move len per 0.5s %d, cur path idex:%d, total path cnt:%d\n", max_move, cur_path_index, movePath->path_count);

	while (max_move > 0)
	{
		MoveResult result;
		Pos dst_pos = movePath->paths[cur_path_index];
		ret = WalkAlong(&cur_pos, &dst_pos, max_move, &result);
		massert_retval(ret == 0, ret);

		if (result.left_move <= 0)                      //尚未走完整个序列,但是0.5s的位移已经走完
		{
			actor->SetPos(&result.stop_pos);
			if(cur_path_index == movePath->path_count - 1 && result.reach_end)
			{
				return 0; //整个移动序列都走完了
			}
			AddActorMoveTimer(actor, 500);
			//printf("0.5s move finish\n");
			return 0;
		}

		if (result.reach_end)                          //序列中当前段已经走完,但还剩余位移,需要继续移动
		{
			if (cur_path_index == movePath->path_count - 1) //整个序列都走完了,还剩余移动距离直接抛弃
			{
				actor->SetPos(&result.stop_pos);
				return 0; //整个移动序列都走完了
			}

			cur_path_index++;
			actor->SetCurPathIndex(cur_path_index);
			max_move = result.left_move;
			cur_pos = result.stop_pos;
			//printf("path %d finish, left move:%d, cur path idx;%d,total path count:%d\n"
			//	, cur_path_index - 1, max_move, cur_path_index, movePath->path_count);
		}
	}

	return 0;
}