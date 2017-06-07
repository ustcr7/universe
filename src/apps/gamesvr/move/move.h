#ifndef __H_GAMESVR_MOVE_H
#define __H_GAMESVR_MOVE_H


//client: �����ƶ�·��,���͸�svr
//server: svr�����ƶ�·��,�����ƶ�, 

//���������ڰ���ҵ���ʵλ�� ͬ����client, �������λ��ƫ��ϴ�, client������λ��ǿ����ק����ʵλ�� ?������ק�ǿͻ�������?

//ΪʲôҪ�ж�ʱ�߼�???  <<--- �����ӳ�

// �ͻ��˷����ƶ����� 
// ������У������,
//               У��ʧ��,����Ϣ���ͻ���,ֹͣ�ƶ�
//               У��ɹ�, ������������ʱ��0.5s���������ƶ�һ��
//                         �ͻ���0.5s�ϱ�һ������λ��, ����ͷ��������,ǿ���ÿͻ��˷�����ק



//��ǰλ�� (cur_x, cur_y)  �ƶ�����(d)

// 1. 0.5s �ƶ������
//         /|(cur_x + б�߳�*cos(a), cur_y + б�߳�*sia(a))
//        / |
//       /  |
//      /   |
//     /    |
//    /_____|
//  (cur_x, cur_y)
//   cos(a) = (abs(dst_x - cur_x)/sqrt((dst_x-cur_x)^2+(dst_y-cur_y)^2))

// 2. 0.5s λ�ƿ����, ��0.5s������ηֱ����
//
struct Actor;
int AddActorMoveTimer(const Actor *actor, int delay_interval);

#endif // !__H_GAMESVR_MOVE_H

