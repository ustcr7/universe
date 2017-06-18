#include "gamesvr_def.h"
#include "gamesvr_msg_processer.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include <cmath>

Pos::Pos()
{
	_x = 0;
	_y = 0;
}
Pos::Pos(int x, int y) 
{
	_x = x;
	_y = y;
}
int Pos::SetX(int x)
{
	_x = x;
	return 0;
}
int Pos::SetY(int y)
{
	_y = y;
	return 0;
}
int Pos::GetX() const
{
	return _x;
}
int Pos::GetY() const
{
	return _y;
}

int Pos::GetDistanceFromPos(const Pos* pos) const
{
	massert_retval(pos != NULL, 0);

	return sqrt(pow(GetX() - pos->GetX(), 2) + pow(GetY() - pos->GetY(),2) );
}