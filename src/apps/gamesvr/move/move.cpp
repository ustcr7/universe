#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/timer/uv_timer.h"
#include "../actor.h"
#include "../actor_mgr.h"
#include <cmath>
struct MoveResult
{
	bool reach_end;       //是否到达终点
	Pos stop_pos;        //最终停留的点坐标
	int left_move;   //移动停止时剩余的长度
};

int ActorMoveTimeout(const char *callback_data, u32 callback_data_len);

int WalkAlong(const Pos *cur_pos, const Pos *dst_pos, int max_move, MoveResult *result)
{
	int diff_x = dst_pos->GetX() - cur_pos->GetX();
	int diff_y = dst_pos->GetY() - cur_pos->GetY();
	int dist = sqrt(diff_x*diff_x + diff_y*diff_y);

	if (max_move < dist)  //移动距离内不会到达该段终点
	{
		int cos_a = (dst_pos->GetX() - cur_pos->GetX()) / dist;
		int sin_a = (dst_pos->GetY() - cur_pos->GetY()) / dist;
		int new_pos_x = cur_pos->GetX() + max_move * cos_a;
		int new_pos_y = cur_pos->GetY() + max_move * sin_a;

		result->reach_end = false;
		result->stop_pos.SetX(new_pos_x);
		result->stop_pos.SetY(new_pos_y);
		result->left_move = 0;
		return 0;
	}

	result->reach_end = true;
	result->left_move = max_move - dist;
	result->stop_pos = *dst_pos;

	return 0;
}

int AddActorMoveTimer(const Actor *actor)
{
	//时间管理器
	UvTimer *timer = UvTimer::GetSingleInstance();
	massert_retval(timer != NULL, ERR_BAD_ALLOC);
	timer->InitTimer();
	u64 mid = actor->GetId();
	timer->AddTimer(500, ActorMoveTimeout, (const char*)&mid, sizeof(mid));

	return 0;
}

int ActorMoveTimeout(const char *callback_data, u32 callback_data_len)
{
	int ret = 0;
	u64 actor_mid = *(u64*)callback_data;

	printf("actor:%llu move time out\n", actor_mid);

	ActorMgr *actorMgr = ActorMgr::GetSingleInstance();
	massert_retval(actorMgr != NULL, ERR_BAD_ALLOC);

	Actor *actor = actorMgr->GetActorById(actor_mid);
	massert_retval(actor != NULL, ERR_NOT_FOUND);

	Pos cur_pos = *(actor->GetPos());
	int cur_path_index = actor->GetCurPathIndex();
	const MovePath *movePath = actor->GetMovePath();
	massert_retval(cur_path_index >= 0 && cur_path_index < movePath->path_count, ERR_INVALID_PARAM);

	int max_move = actor->GetSpeed() * 0.5;   //计算玩家0.5秒内最多移动距离

	while (max_move > 0)
	{
		MoveResult result;
		Pos dst_pos = movePath->paths[cur_path_index];
		ret = WalkAlong(&cur_pos, &dst_pos, max_move, &result);
		massert_retval(ret == 0, ret);

		if (cur_path_index == movePath->path_count - 1) //表示整个移动序列已经完成
		{
			actor->SetPos(&dst_pos);
			return 0;
		}
		if (result.left_move <= 0)                      //尚未走完整个序列,但是0.5s的位移已经走完
		{
			actor->SetPos(&dst_pos);
			AddActorMoveTimer(actor);
			return 0;
		}

		if (result.reach_end)                          //序列中当前段已经走完,但还剩余位移,需要继续移动
		{
			cur_path_index++;
			actor->SetCurPathIndex(cur_path_index);
			max_move = result.left_move;
		}
	}

	return 0;
}