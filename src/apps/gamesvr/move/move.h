#ifndef __H_GAMESVR_MOVE_H
#define __H_GAMESVR_MOVE_H


//client: 计算移动路径,发送给svr
//server: svr按照移动路径,进行移动, 

//服务器定期把玩家的真实位置 同步给client, 如果发现位移偏差较大, client将表现位置强制拖拽到真实位置 ?看来拖拽是客户端做的?

//为什么要有对时逻辑???  <<--- 网络延迟

// 客户端发送移动序列 
// 服务器校验序列,
//               校验失败,发消息给客户端,停止移动
//               校验成功, 服务器开启定时器0.5s服务器内移动一次
//                         客户端0.5s上报一下自身位置, 如果和服务差别过大,强制让客户端发生拖拽



//当前位置 (cur_x, cur_y)  移动向量(d)

// 1. 0.5s 移动不跨段
//         /|(cur_x + 斜边长*cos(a), cur_y + 斜边长*sia(a))
//        / |
//       /  |
//      /   |
//     /    |
//    /_____|
//  (cur_x, cur_y)
//   cos(a) = (abs(dst_x - cur_x)/sqrt((dst_x-cur_x)^2+(dst_y-cur_y)^2))

// 2. 0.5s 位移跨段了, 把0.5s拆成两段分别计算
//
struct Actor;
int AddActorMoveTimer(const Actor *actor, int delay_interval);

#endif // !__H_GAMESVR_MOVE_H

