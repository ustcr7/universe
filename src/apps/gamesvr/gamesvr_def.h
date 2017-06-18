#ifndef __H_GAMESVR_DEF_H__
#define __H_GAMESVR_DEF_H__

class Pos
{
public:
	Pos();
	Pos(int x, int y);
	int SetX(int x);
	int SetY(int y);
	int GetX() const;
	int GetY() const;
	int GetDistanceFromPos(const Pos* pos) const;
private:
	int _x;
	int _y;
};

#endif // !__H_GAMESVR_DEF_H__
