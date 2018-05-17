#include "..\e2daction.h"


e2d::RotateTo::RotateTo(double duration, double rotation)
	: RotateBy(duration, 0)
{
	_endVal = rotation;
}

e2d::RotateTo * e2d::RotateTo::create(double duration, double rotation)
{
	return Create<RotateTo>(duration, rotation);
}

e2d::RotateTo * e2d::RotateTo::clone() const
{
	return Create<RotateTo>(_duration, _endVal);
}

void e2d::RotateTo::_init()
{
	RotateBy::_init();
	_deltaVal = _endVal - _startVal;
}