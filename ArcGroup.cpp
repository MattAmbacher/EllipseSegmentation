#include "ArcGroup.h"
#include "LineSegment.h"

ArcGroup::ArcGroup(std::vector<LineSegment> l, double x, double y, double r) 
{
	LA = l;
	xc = x;
	yc = y;
	R = r;
}